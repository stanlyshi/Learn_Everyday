/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:17 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:05 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:46 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:17 CST. */
#include "tcp_common.h"

void* get_in_addr(struct sockaddr_storage* ss)
{
    if (ss->ss_family == AF_INET) 
    {
        return &(((struct sockaddr_in*)ss)->sin_addr);
    }

    return &(((struct sockaddr_in6*)ss)->sin6_addr);
}

char* get_ip_addr(struct sockaddr_storage* ss) 
{
    size_t addr_len =
        ss->ss_family == AF_INET ?
          INET_ADDRSTRLEN : INET6_ADDRSTRLEN;

    char* s = malloc(addr_len);
    
    if (s == NULL) 
    {
        return NULL;
    }
    
    inet_ntop(ss->ss_family,
              get_in_addr(ss),
              s,
              addr_len);
    
    return s; 
}

short parse_port(char* s) 
{
    char* tail;
    long int r = strtol(s, &tail, 10);

    if (tail != NULL) 
    {
        return (short)-1;
    }

    return (short)r;
}

int tcp_connect(char* hostname, char* port) 
{
    char* ip;
    int csock;
    int rv;
    struct addrinfo hints, *si, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(hostname, port, &hints, &si)) != 0) 
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    // loop through all the results and connect to the first we can
    for(p = si; p != NULL; p = p->ai_next) 
    {
        if ((csock = socket(p->ai_family, p->ai_socktype,
                            p->ai_protocol)) == -1) 
        {
            perror("socket");
            continue;
        }

        if (connect(csock, p->ai_addr, p->ai_addrlen) == -1) 
        {
            close(csock);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) 
    {
        fprintf(stderr, "client: failed to connect\n");
        return -1;
    }

    ip = get_ip_addr((struct sockaddr_storage *)p);
    printf("client: connecting to %s ...\n", ip);
    free(ip);

    freeaddrinfo(si); // all done with this structure

    return csock;
}

int tcp_bind(char* port) 
{
    int rv;
    int sockfd;
    int yes = 1;
    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) 
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) 
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) 
        {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd,
                       SOL_SOCKET,
                       SO_REUSEADDR, &yes,
                       sizeof(int)) == -1) 
        {
            perror("setsockopt");
            return -1;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) 
        {
            close(sockfd);
            perror("server: bind");
            return -1;
        }

        break;
    }

    if (p == NULL)  
    {
        fprintf(stderr, "server: failed to bind\n");
        return -1;
    }

    freeaddrinfo(servinfo); // all done with this structure

    return sockfd;
}

int tcp_listen(int ssock, size_t blog) 
{
    if (listen(ssock, blog) == -1) 
    {
        perror("listen");
        return -1;
    }

    return 1;
}

int tcp_accept(int ssock, struct sockaddr_storage* sa) 
{
    socklen_t sz = sizeof(struct sockaddr_storage);
    int csock = accept(ssock, (struct sockaddr *)sa, &sz);
    return csock;
}

void print_usage(char* app) 
{
    printf("usage : ./%s <port> <backlog> "\
        "\t<port> - the listening port. (default : 8888). "\
        "\t<backlog> - the number of pending connections the server will hold."
        , app);
}

int start_server_select(char* port, int backlog) 
{
    char* ip;
    socklen_t addrlen;
    int ssock, csock;
    struct sigaction sa;
    struct sockaddr_storage ca;
    
    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
    int fdmax;        // maximum file descriptor number
    int i;

    if ((ssock = tcp_bind(port)) == -1) 
    {
        return -1;
    }

    if (tcp_listen(ssock, backlog) == -1) 
    {
        return -1;
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    
    if (sigaction(SIGCHLD, &sa, NULL) == -1) 
    {
        perror("sigaction");
        return -1;
    }

    printf("server: waiting for connections on port %s ...\n", port);

    FD_ZERO(&master);  // clear the master and temp sets
    FD_ZERO(&read_fds);

    // add the listener to the master set
    FD_SET(ssock, &master);

    // keep track of the biggest file descriptor
    fdmax = ssock; // so far, it's this one

    while(1) 
    {
        read_fds = master; // copy it
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) 
        {
            perror("select");
            exit(4);
        }
        
        // run through the existing connections looking for data to read
        for(i = 0; i <= fdmax; i++) 
        {
            if (FD_ISSET(i, &read_fds)) 
            { // we got one!!
                if (i == ssock) 
                {
                    // handle new connections
                    addrlen = sizeof(ca);
                    csock = accept(ssock, (struct sockaddr *)&ca, &addrlen);
    
                    if (csock == -1) {
                        perror("accept");
                    } else 
                    {
                        ip = get_ip_addr(&ca);
                        if (ip) 
                        {
                            printf("server: got connection from %s\n", ip);
                            free(ip);
                        }
                        else 
                        {
                            printf("server: failed to get ip address of the client!");
                        }
    
                        if (send(csock, "Hello, world!", 13, 0) == -1) 
                        {
                            perror("send");
                        }
    
                        close(csock);
                   }
                }
            }
        }
    }
}

int start_server_iteration(char* port, int backlog) 
{
    char* ip;
    int ssock, csock;
    struct sigaction sa;
    struct sockaddr_storage ca;
    
    if ((ssock = tcp_bind(port)) == -1) 
    {
        return -1;
    }

    if (tcp_listen(ssock, backlog) == -1) 
    {
        return -1;
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    
    if (sigaction(SIGCHLD, &sa, NULL) == -1) 
    {
        perror("sigaction");
        return -1;
    }

    printf("server: waiting for connections on port %s ...\n", port);

    while(1) 
    {  // main accept() loop
        csock = tcp_accept(ssock, &ca);

        if (csock == -1) 
        {
            perror("accept");
            continue;
        }

        ip = get_ip_addr(&ca);
        
        if (ip) 
        {
            printf("server: got connection from %s\n", ip);
            free(ip);
        }
        else 
        {
            printf("server: failed to get ip address of the client!");
        }

        if (send(csock, "Hello, world!", 13, 0) == -1) 
        {
            perror("send");
        }

        close(csock);
    }
}

int start_server_fork(char* port, int backlog) 
{
    char* ip;
    int ssock, csock;
    struct sigaction sa;
    struct sockaddr_storage ca;
    
    if ((ssock = tcp_bind(port)) == -1) 
    {
        return -1;
    }

    if (tcp_listen(ssock, backlog) == -1) 
    {
        return -1;
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    
    if (sigaction(SIGCHLD, &sa, NULL) == -1) 
    {
        perror("sigaction");
        return -1;
    }

    printf("server: waiting for connections on port %s ...\n", port);

    while(1) 
    {  // main accept() loop
        csock = tcp_accept(ssock, &ca);

        if (csock == -1) 
        {
            perror("accept");
            continue;
        }

        ip = get_ip_addr(&ca);
        
        if (ip) 
        {
            printf("server: got connection from %s\n", ip);
            free(ip);
        }
        else 
        {
            printf("server: failed to get ip address of the client!");
        }

        if (!fork()) 
        { // this is the child process
            close(ssock); // child doesn't need the listener

            if (send(csock, "Hello, world!", 13, 0) == -1) 
            {
                perror("send");
            }

            close(csock);
            
            exit(0);
        }
        
        close(csock);  // parent doesn't need this
    }
}

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}
