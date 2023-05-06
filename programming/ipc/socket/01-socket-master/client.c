/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:11 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:02 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:43 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:14 CST. */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>

const char *NAME = "./my_sock";
#define MSG "Hello world!"
#define MAX 1024
const int PORT = 2010;

int orig_sock;
//int main()
int socket_init(void)
{
	struct sockaddr_in serv_adr;
	struct hostent *host;

	host = gethostbyname("localhost");

	if ((orig_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("generate error");
		return 1;
	}

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	memcpy(&serv_adr.sin_addr, host->h_addr, host->h_length);
	serv_adr.sin_port = htons(PORT);

	if (connect(orig_sock, (struct sockaddr*)&serv_adr,
		sizeof(serv_adr)) < 0) {
		perror("connect error");
		return 2;
	}

	return 0;
}

#define SIZE 1048576*8
int main()
{
//	char buf[MAX];
        char *buf;
        ssize_t ret = 0;
        FILE *fp = fopen("a.bin", "wb");;

        buf = malloc(SIZE);

	socket_init();

        buf[0] = '1';
        printf("Sending a request\n");
        write(orig_sock, &buf[0], 1);
	printf("Waiting for server...\n");
        do {
                ret += read(orig_sock, buf+ret, SIZE - ret);
        } while (ret != SIZE);
        printf("read total: %lu bytes\n", ret);
        fwrite(buf, SIZE, 1, fp);

        buf[0] = '0';
        printf("Sending end request, bye~\n");
        write(orig_sock, &buf[0], 1);

	printf("\n");
	close(orig_sock);
        free(buf);
        fclose(fp);
	return 0;
}

