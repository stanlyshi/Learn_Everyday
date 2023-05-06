#include "common.h"

char buf[MSG_LENGTH];

int unsocket_server(const char *PATH)
{
    struct sockaddr_un srv_addr;
    
    int listen_fd, ret;
    
    listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    
    if(listen_fd < 0)
    {
        return -1;
    }
    else
    {
        srv_addr.sun_family = AF_UNIX;
        strncpy(srv_addr.sun_path, PATH, sizeof(srv_addr.sun_path)-1);
        
        unlink(PATH);
        
        ret = bind(listen_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
        
        if(ret == -1)
        {
            close(listen_fd);
            unlink(PATH);
            return -1;
        }
        
        ret = listen(listen_fd, 1);
        if(ret == -1)
        {
            close(listen_fd);
            unlink(PATH);
            return -1;
        }
        
        chmod(PATH, 00777);
    }
    return listen_fd;
}

int unsocket_client(const char *PATH)
{
    int connect_fd;
    int ret = 0;
    
    static struct sockaddr_un srv_addr;
    
    connect_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    
    if(connect_fd < 0)
    {
        return -1;
    }
    else
    {
        srv_addr.sun_family = AF_UNIX;
        strcpy(srv_addr.sun_path, UNIX_DOMAIN);
        
        ret = connect(connect_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
        
        if(ret == -1)
        {
            close(connect_fd);
            return -1;
        }
    }
    
    return connect_fd;
}

