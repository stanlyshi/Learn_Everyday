/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:04 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:58 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:39 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:11 CST. */
/**
 *  测试物理连接
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <net/if.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>

#define LINKTEST_GLINK  0x0000000a

struct linktest_value{
    unsigned int cmd;
    unsigned int data;
};

static void usage(const char *pname)
{
    fprintf(stderr, "usage: %s<device>\n", pname);
    fprintf(stderr, "\t 0:link detected.\n");
    fprintf(stderr, "\t %d:%s\n", ENODEV, strerror(ENODEV));
    fprintf(stderr, "\t %d:%s\n", ENONET, strerror(ENONET));
    fprintf(stderr, "\t %d:%s\n", EOPNOTSUPP, strerror(EOPNOTSUPP));
    exit(1);
}

static int linktest(const char *devname)
{
    struct ifreq ifr;
    struct linktest_value edata;
    int fd;
    
    //setup our control structures
    memset(&ifr, 0, sizeof(ifr));
    
    strcpy(ifr.ifr_name, devname);
    
    //open control socket
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    
    if(fd < 0)
    {
        return -ECOMM;
    }
    
    errno = 0;
    
    edata.cmd = LINKTEST_GLINK;
    
    ifr.ifr_data = (caddr_t)&edata;
    
    if(!ioctl(fd, SIOCETHTOOL, &ifr))
    {
        if(edata.data)
        {
            fprintf(stdout, "link detected on %s\n", devname);
            return 0;
        }
        else
        {
            errno = ENONET;
        }
    }
    perror("linktest: ");
    return errno;
    
}


int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        usage(argv[0]);
        
    }
    
    return linktest(argv[1]);
}