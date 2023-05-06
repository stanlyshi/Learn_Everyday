/**
 *  UNIX套接字示例代码
 *
 *  作者：荣涛
 *  时间：2020年11月9日
 */

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


void memshow(const char *prefix, void *ptr, ssize_t size)
{
//  format:  
//	0x0000:  4500 0034 fcd2 4000 4006 3fef 7f00 0001
//	0x0010:  7f00 0001 988e 268f 2b67 f87a 7d08 8461
//	0x0020:  8010 01f8 fe28 0000 0101 080a a355 7aed
    unsigned char *byte = (unsigned char *)ptr;
    unsigned char *line_hdr=NULL;
    int i=0, j=0, cnt=0;
    int line=0;
    for(i=0;i<size;i++){
        if(cnt%16==0) {
            line=printf("%s0x%04x:  ", prefix, cnt);
            line_hdr = byte;
        }

        printf("%02x", *byte);
        
        
        if(cnt%2)printf(" ");
        byte++;

        cnt++;
        if(cnt%16==0) {
//            printf(" | ");
//            for(j=0;j<16;j++){
//                if(i+j<=size){
//                    printf("%c", isascii(*line_hdr)?*line_hdr:'.');
//                    line_hdr++;
//                }
//            }
            printf("\n");
            fflush(stdout);
        }
    }
    printf("\n");
    fflush(stdout);
}

