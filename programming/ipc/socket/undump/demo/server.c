/**
 *  UNIX套接字示例代码：服务端
 *    
 *  作者：荣涛
 *  时间：2020年11月9日
 */

#include "common.h"

int listen_fd;

void sig_handler(int signum)
{
    printf("------------------------------------------------\n");
    close(listen_fd);
    exit(1);
}


void main()
{
    int com_fd; 
    int ret = 0;
    int i;
    int len;
    struct sockaddr_un clt_addr;

    signal(SIGINT, sig_handler);
    
    listen_fd = unsocket_server(UNIX_DOMAIN);
    printf("------------------------------------------------\n");
    printf("-- UNIX Server %s\n", UNIX_DOMAIN);
    while(1)
    {
        len = sizeof(clt_addr);
        com_fd = accept(listen_fd, (struct sockaddr*)&clt_addr, &len);

		int val = 1;
		setsockopt(listen_fd, SOL_TCP, TCP_NODELAY, &val, sizeof(val));
		setsockopt(com_fd, SOL_TCP, TCP_NODELAY, &val, sizeof(val));
        if(com_fd < 0)
        {
            perror("cannot accept client connect request.");
            close(listen_fd);
            unlink(UNIX_DOMAIN);
            break;
        }
        
        memset(buf, 0, MSG_LENGTH);

		while(1)
		{
			len = read(com_fd, buf, MSG_LENGTH);
			printf("-- len= %d, recv: %s\n", len, buf);
            memshow("-- ", buf, len);
			memset(buf, 0, MSG_LENGTH);
			if(len<=0)
				break;
		}

        buf[0] = 'X';
        
        write(com_fd, buf, MSG_LENGTH);
        
        close(com_fd);
    }
}
