#include "common.h"

int clients_fd[MAX_CLIENT_NUM];

char buf[MSG_LENGTH];

void main()
{
    int listen_fd;
    int com_fd; 
    int ret = 0;
    int i;
    int len;
    struct sockaddr_un clt_addr;
    
    listen_fd = unsocket_server(UNIX_DOMAIN);

    len = sizeof(clt_addr);
    
    com_fd = accept(listen_fd, (struct sockaddr*)&clt_addr, &len);
	int val = 1;
	setsockopt(listen_fd, SOL_TCP, TCP_NODELAY, &val, sizeof(val));
	setsockopt(com_fd, SOL_TCP, TCP_NODELAY, &val, sizeof(val));
    if(com_fd < 0) {
        perror("cannot accept client connect request.");
        close(listen_fd);
        unlink(UNIX_DOMAIN);
    }
    
    unsigned long nloop = 0;
    
    struct timeval start, end;
    unsigned long recv_bytes = 0;
    
    gettimeofday(&start, NULL);
	while(++nloop <= TEST_NLOOP)
	{
		ret = read(com_fd, buf, MSG_LENGTH);
//        perror("read");
        recv_bytes += ret;
//        printf("recv_bytes = %ld\n", recv_bytes);
	}
    gettimeofday(&end, NULL);
    printf_rate("Read", recv_bytes, diff_timeval_usec(&end, &start));

    close(com_fd);
    close(listen_fd);
}
