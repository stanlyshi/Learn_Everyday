#include "common.h"

char buf[MSG_LENGTH];

void main()
{
    int connect_fd;
    int ret = 0, len;

    connect_fd  = unsocket_client(UNIX_DOMAIN);
    int val = 1;
	setsockopt(connect_fd, SOL_TCP, TCP_NODELAY, &val, sizeof(val));
    
    unsigned long nloop = 0;
    
    struct timeval start, end;
    unsigned long send_bytes = 0;
    
    gettimeofday(&start, NULL);

	while(++nloop <= TEST_NLOOP)
	{
		ret = write(connect_fd, buf, MSG_LENGTH);
        send_bytes += ret;
	}
    gettimeofday(&end, NULL);
    printf_rate("Send", send_bytes, diff_timeval_usec(&end, &start));
    
    close(connect_fd);

}
