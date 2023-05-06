#include "common.h"

void main()
{
    int connect_fd;
    int ret = 0, len;

    connect_fd  = unsocket_client(UNIX_DOMAIN);
    int val = 1;
	setsockopt(connect_fd, SOL_TCP, TCP_NODELAY, &val, sizeof(val));
    printf("fd: %d\n", connect_fd);
	while(1)
	{
		scanf("%s", buf);
        
		ret = write(connect_fd, buf, strlen(buf)); 
		printf("send bytes = %d: %s\n", ret, buf);
	}

    len = read(connect_fd, buf, MSG_LENGTH);
    
    printf("%d:%s\n", len, buf);
    
    close(connect_fd);

}
