*** PASSING OPEN FILE DESCRIPTORS OVER UNIX DOMAIN SOCKETS ***

https://openforums.wordpress.com/2016/08/07/open-file-descriptor-passing-over-unix-domain-sockets/
-----------------------------------------------------------------------------------------------------------------------------

Note:- This article is an example walk-through written for developers keeping in mind that you are aware of very 
basic network/socket programming concepts.


Some basics
All of you must have heard about file descriptors or popularly known as fds in Unix world. It is basically an index/
indicator generally used to provide access to files, network sockets etc. File descriptor will always be a non-
negative integer which has been part of POSIX standard from old days. Accordingly we have 3 standard POSIX file 
descriptors namely standard input or stdin with value 0, standard output or stdout with value 1 and standard error or 
stderr with value 2. Standard I/O library from C allows us to create fds via different system calls like open(), creat
(), socket(), socketpair(), pipe() etc. For example, if we want to write some data into a file we normally invoke open
(2) or create(2) to have an open file descriptor and subsequent write(2) operation is performed via that fd. Finally 
when we are done with operations we close the fd.


That’s all about some basic file operations work flow. On the other side we have Unix Domain Sockets which is 
commonly used for data communications between processes running on the same host. Don’t ask me why we use sockets 
rather than named pipes or shared memory techniques for inter-process communication. That’s just because my 
intention of this article is not to compare the performance shown by these various methods on different platforms. 
You can still surf the web and find yourselves other articles explaining the same. And these sockets can be of 
various types among STREAM, DATAGRAM, RAW and so on which can operate under different communication domains like UNIX
, IP, PACKET etc. Unless I restrict myself to a particualr socket type and communication domain this article would 
expand exponentially matching every combinations. So I would like to choose STREAM sockets under UNIX address family 
opearting with IP protocol. For more details refer to man pages for socket(2), ip(7), protocols(5), unix(7).


Let’s get deeper
Understanding these concepts of open file descriptors and unix domain sockets leads us to the core content of this 
article i.e, passing of file descriptors among processes over unix domain sockets. You may be wondering on why we 
would pass an fd to another process since we can always share an open fd by forking a child. But think about it for a 
second, what if we need to share an fd after a child is forked. Hm..Isn’t that tricky? Or is that a case we need to 
solve? You will come to know that in Unix world over time this has been a requirement and now I am going to explain 
how do we solve this problem citing two different sample programs. Make sure that you are more or less comfortable 
with the terms and concepts we have discussed so far to follow things much easier.


Client
-----------------------------------------------------------------------------------------------------------------------------
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while(0)

static void send_fd(int socket, int *fds, int n)  // send fd by socket
{
    struct msghdr msg = {0};
    struct cmsghdr *cmsg;
    char buf[CMSG_SPACE(n * sizeof(int))], dup[256];
    memset(buf, '\0', sizeof(buf));
    struct iovec io = { .iov_base = &dup, .iov_len = sizeof(dup) };


    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(n * sizeof(int));

    memcpy ((int *) CMSG_DATA(cmsg), fds, n * sizeof (int));

    if (sendmsg (socket, &msg, 0) < 0)
        handle_error ("Failed to send message");
}

int main(int argc, char *argv[]) 
{
    int sfd, fds[2];
    struct sockaddr_un addr;

    if (argc != 3) {
        fprintf (stderr, "Usage: %s <file-name1> <file-name2>\n", argv[0]);
        exit (1);
    }

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1)
        handle_error ("Failed to create socket");

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "/tmp/fd-pass.socket", sizeof(addr.sun_path)-1);

    fds[0] = open(argv[1], O_RDONLY);
    if (fds[0] < 0)
        handle_error ("Failed to open file 1 for reading");
    else
        fprintf (stdout, "Opened fd %d in parent\n", fds[0]);

    fds[1] = open(argv[2], O_RDONLY);
    if (fds[1] < 0)
        handle_error ("Failed to open file 2 for reading");
    else
        fprintf (stdout, "Opened fd %d in parent\n", fds[1]);

    if (connect(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
        handle_error ("Failed to connect to socket");

    send_fd (sfd, fds, 2);

    exit(EXIT_SUCCESS);
}
-----------------------------------------------------------------------------------------------------------------------------



Server
-----------------------------------------------------------------------------------------------------------------------------
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while(0)

static int * recv_fd(int socket, int n) 
{
    int *fds = malloc (n * sizeof(int));
    struct msghdr msg = {0};
    struct cmsghdr *cmsg;
    
    char buf[CMSG_SPACE(n * sizeof(int))], dup[256];
    memset(buf, '\0', sizeof(buf));
    
    struct iovec io = { .iov_base = &dup, .iov_len = sizeof(dup) };

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    if (recvmsg (socket, &msg, 0) < 0)
        handle_error ("Failed to receive message");

    cmsg = CMSG_FIRSTHDR(&msg);

    memcpy (fds, (int *) CMSG_DATA(cmsg), n * sizeof(int));

    return fds;
}

int main(int argc, char *argv[]) 
{
    ssize_t nbytes;
    char buffer[256];
    int sfd, cfd, *fds;
    struct sockaddr_un addr;

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1)
        handle_error ("Failed to create socket");

    if (unlink ("/tmp/fd-pass.socket") == -1 && errno != ENOENT)
        handle_error ("Removing socket file failed");

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "/tmp/fd-pass.socket", sizeof(addr.sun_path)-1);

    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
        handle_error ("Failed to bind to socket");

    if (listen(sfd, 5) == -1)
        handle_error ("Failed to listen on socket");

    cfd = accept(sfd, NULL, NULL);
    if (cfd == -1)
        handle_error ("Failed to accept incoming connection");

    fds = recv_fd (cfd, 2);

    for (int i=0; i<2; ++i) {
        fprintf (stdout, "Reading from passed fd %d\n", fds[i]);
        while ((nbytes = read(fds[i], buffer, sizeof(buffer))) > 0)
            write(1, buffer, nbytes);
        *buffer = '\0';
    }

    if (close(cfd) == -1)
        handle_error ("Failed to close client socket");

    return 0;
}

-----------------------------------------------------------------------------------------------------------------------------

Note:- Client program on execution expects two non-empty regular text files as command line arguments.

Consider the client-server program given above(even though its named client and server they reside on the same host..
of course they can). Here server initially opens a unix domain stream socket fd with the default IP protocol. In 
order to make sure that we don’t have any stale socket files we remove the same if there existed a socket file 
before. After clearing the sockaddr_un structure we fill up the structure with AF_UNIX address family and socket file 
path. As usual we bind the socket fd with the specified socket file and listens for incoming connections. As soon as 
client comes up and connect to this very same socket file, server accepts this connection and is ready for 
communication.


On the other side client program begins as server by creating a unix domain stream socket fd with IP protocol and 
prepares the sockaddr_un structure for establishing the connection to server. Afterwards it opens two already 
existing non-empty regular text files provided via command-line arguments during the execution of the program and 
connects to server via its own fd. sendmsg() and recvmsg() are two network system calls used to send and receive 
messages on a socket respectively. But they can just merely used for exchanging messages and not open fds. Generally 
we make use of msghdr structure for packing up the message payload in format expected by sendmsg() system call. For 
details refer sendmsg(2).


ADVERTISEMENT
REPORT THIS AD

    struct msghdr {
        void         *msg_name;       /* optional address */
        socklen_t     msg_namelen;    /* size of address */
        struct iovec *msg_iov;        /* scatter/gather array */
        size_t        msg_iovlen;     /* # elements in msg_iov */
        void         *msg_control;    /* ancillary data, see below */
        size_t        msg_controllen; /* ancillary data buffer len */
        int           msg_flags;      /* flags (unused) */
    };

In order to send extra data we use the cmsghdr and its derivative macros integrated into standard msghdr structure. 
See below:


    struct cmsghdr {
        size_t cmsg_len;    /* Data byte count, including header
        int    cmsg_level;  /* Originating protocol */
        int    cmsg_type;   /* Protocol-specific type */
        /* followed by
        unsigned char cmsg_data[]; */
    };

The interesting part here that if you look further into man page for cmsg(3) there is an in-built option for passing 
an array of open fds with the help of CMSG_DATA() macro. Wow…that’s exactly what we were searching for.


In our client program we prepare both msghdr and cmsghdr structures are setup within the call to a function named 
send_fd() whose arguments includes the open fd array and count of number of fds. It is necessary to have at least one 
iov vector defined to get sendmsg() to work as expected. Following that we declare a character array named dup and 
prepare the iovec structure.


    struct iovec io = { .iov_base = &dup, .iov_len = sizeof(dup) };
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;

Another character array equal to the size of fd array is also declared using the special cmsg macro CMSG_SPACE() 
which returns the number of bytes for the ancillary data we are going to send.


    msg.msg_control = buf
;                                                                                                      

    msg.msg_controllen = sizeof(buf);

With the help of CMSG_FIRSTHDR() macro we could get a pointer to the first cmsghdr in the ancillary data buffer 
associated with the passed msghdr and is initialized to cmsghdr structure pointer cmsg. As per the man page 
cmsg_level and cmsg_type are set to SOL_SOCKET and SCM_RIGHTS respectively.cmsg_len is returned by invoking CMSG_LEN(
) taking into account the necessary alignment required inside the cmsg structure.


    cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET
;                                                                                              

    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(n * sizeof(int));

Finally we copy the open fd array into the type casted data portion of csmsghdr structure through the pointer 
returned by CMSG_DATA().


    memcpy ((int *) CMSG_DATA(cmsg), fds, n * sizeof (int));

and. . .we call sendmsg() and we are done. Yes, but now lets get back to server side. Here also we prepare msghdr 
structure(I hope you know why we don’t initialize cmsghdr here. Because we are going to receive it from server) and 
receive payload from client via recvmsg(). On receiving successfully we initialize cmsg to the first cmsghdr in the 
ancillary data.


    cmsg = CMSG_FIRSTHDR(&msg);

Now we copy the type caste cmsg pointer to data portion of cmsghdr structure into our integer array declared to carry 
the open fds.


    memcpy (fds, (int *) CMSG_DATA(cmsg), n * sizeof(int));

Back in main() we iterate through this populated fd array and reads the content of files from server. Last but not 
the least lets go through a similar program which duplicates the received fd so as to print message directly onto 
client’s stdout.


Client

#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while(0)

static
void send_fd (int socket) {
        struct msghdr msg = {0};
        struct cmsghdr *cmsg;
        char buf[CMSG_SPACE(sizeof(int))], *dup = “hello world”;
        memset(buf, ‘\0’, sizeof(buf));
        struct iovec io = { .iov_base = &dup, .iov_len = sizeof(dup) };

        msg.msg_iov = &io;
        msg.msg_iovlen = 1;
        msg.msg_control = buf;
        msg.msg_controllen = sizeof (buf);

        cmsg = CMSG_FIRSTHDR(&msg);
        cmsg->cmsg_level = SOL_SOCKET;
        cmsg->cmsg_type = SCM_RIGHTS;
        cmsg->cmsg_len = CMSG_LEN(sizeof(int));

        int *fdptr = (int *)CMSG_DATA(cmsg);
        *fdptr = 1;

        if (sendmsg(socket, &msg, 0) == -1)
                handle_error(“Error sending client stdout”);
}

int main() {
        int sfd;
        struct sockaddr_un addr;

        sfd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sfd == -1)
                handle_error(“Failed to create socket”);

        memset(&addr, 0, sizeof(struct sockaddr_un));
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, “/tmp/fd-pass.socket”, sizeof(addr.sun_path) – 1);

        if (connect(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
                handle_error(“Failed to connect to socket”);

        send_fd (sfd);

        if (close(sfd) == -1)
                handle_error (“Failed to close socket”);

    exit (EXIT_SUCCESS);
}

Server

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while(0)

static
void monitor(int socket) {
        size_t nread;
        struct msghdr msg = {0};
        struct cmsghdr *cmsg;
        char buf[CMSG_SPACE(sizeof(int))], dup[256];
        memset(buf, ‘\0’, sizeof(buf));
        struct iovec io = { .iov_base = &dup, .iov_len = sizeof(dup) };

        msg.msg_iov = &io;
        msg.msg_iovlen = 1;
        msg.msg_control = buf;
        msg.msg_controllen = sizeof(buf);

        if ( recvmsg(socket, &msg, 0) < 0 )
                handle_error(“Failed to receive mesage”);

        cmsg = CMSG_FIRSTHDR(&msg);

        int *fdptr = (int *)CMSG_DATA(cmsg);

        dup2(*fdptr, 1);
        printf(“This message will be displayed on client stdout\n”);
        exit(0);
}

int main() {
        int sfd, cfd;
        struct sockaddr_un addr;
        socklen_t peer_addr_size;
        pid_t pid;

        sfd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sfd == -1)
                handle_error(“Failed to create socket”);

        if (unlink (“/tmp/fd-pass.socket”) == -1 && errno != ENOENT)
                handle_error (“Removing socket file failed”);

        memset(&addr, 0, sizeof(struct sockaddr_un));
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, “/tmp/fd-pass.socket”, sizeof(addr.sun_path) – 1);

        if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
                handle_error(“Failed to bind to socket”);

        if (listen(sfd, 5) == -1)
                handle_error(“Failed to listen on socket”);

        while ((cfd = accept(sfd, NULL, NULL)) != -1) {
                if ((pid = fork()) == -1)
                        handle_error (“Failed to fork child”);

                if (!pid) {
                        close(sfd);
                        monitor(cfd);
                }
                if (close(cfd) == -1)
                        handle_error (“Failed to close client socket”);
        }
        handle_error(“Failed to accept incoming connection”);

        return 0;
}

We use dup2(2) system call to duplicate an fd to another fd. See man page for dup2(2) for more details on how 
duplication of fd works. On server side we create a socket fd and waits for incoming client connection. After 
connecting to this socket client prepares cmsghdr structure along with msghdr so as to send fd number 1 i.e, its own 
stdout.


    int *fdptr = (int *)CMSG_DATA(cmsg);
    *fdptr = 1;

Server on receiving client’s stdout, duplicates the same to new fd number 1 as follows:

    int *fdptr = (int *)CMSG_DATA(cmsg)
;                                                                                        

    dup2(*fdptr, 1);

Wait..think about what we have done now. This is the trickiest part. Because dup2 system call will silently close the 
new fd number 1 i.e, server’s stdout and next message will be printed on to client’s stdout and I think that was 
really a cool technique. Now onwards printf statements from server side will display messages on client side.


That’s all I have for now..Try out some more examples involving various combinations of read/write system calls 
backed by fd passing over unix domain sockets and feel free to leave your thoughts/suggestions/questions here as 
comments.
