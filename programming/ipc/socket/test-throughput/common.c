#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "common.h"


static void print_usage(char *name)
{
    printf("\n\n%s [option] [value].\n", name?name:"PROG");
    printf("Usage:");
    
    printf("\t-%c, --%s :\t%s\n", PARSE_IPV4, get_opt_long(PARSE_IPV4), get_opt_description(PARSE_IPV4));
    printf("\t-%c, --%s :\t%s\n", PARSE_PORT, get_opt_long(PARSE_PORT), get_opt_description(PARSE_PORT));
    printf("\t-%c, --%s :\t%s\n", PARSE_PEER_TYPE, get_opt_long(PARSE_PEER_TYPE), get_opt_description(PARSE_PEER_TYPE));
    printf("\t-%c, --%s :\t%s\n", PARSE_PROTO_TYPE, get_opt_long(PARSE_PROTO_TYPE), get_opt_description(PARSE_PROTO_TYPE));
    printf("\t-%c, --%s :\t%s\n", PARSE_PKG_SIZE, get_opt_long(PARSE_PKG_SIZE), get_opt_description(PARSE_PKG_SIZE));
    printf("\t-%c, --%s :\t%s\n", PARSE_PKG_NUM, get_opt_long(PARSE_PKG_NUM), get_opt_description(PARSE_PKG_NUM));
    printf("\t-%c, --%s :\t%s\n", PARSE_REPLY, get_opt_long(PARSE_REPLY), get_opt_description(PARSE_REPLY));
    printf("\t-%c, --%s :\t%s\n", PARSE_READ_REPLY, get_opt_long(PARSE_READ_REPLY), get_opt_description(PARSE_READ_REPLY));
    printf("\t-%c, --%s :\t%s\n", PARSE_TEST_MALLOC, get_opt_long(PARSE_TEST_MALLOC), get_opt_description(PARSE_TEST_MALLOC));
    printf("\t-%c, --%s :\t%s\n", PARSE_TEST_PRINTF, get_opt_long(PARSE_TEST_PRINTF), get_opt_description(PARSE_TEST_PRINTF));
    printf("\n");
}

void parse_args_config(int argc, char **argv, struct parse_args *args)
{
    int c;
    /* Flag set by ‘--verbose'. */
    static int verbose_flag;
    static struct option options[] =
    {
        {get_opt_long(PARSE_IPV4), required_argument, 0, PARSE_IPV4},
        {get_opt_long(PARSE_PORT), required_argument, 0, PARSE_PORT},
        {get_opt_long(PARSE_PEER_TYPE), required_argument, 0, PARSE_PEER_TYPE},
        {get_opt_long(PARSE_PROTO_TYPE), required_argument, 0, PARSE_PROTO_TYPE},
        {get_opt_long(PARSE_PKG_SIZE), required_argument, 0, PARSE_PKG_SIZE},
        {get_opt_long(PARSE_PKG_NUM), required_argument, 0, PARSE_PKG_NUM},
        {get_opt_long(PARSE_REPLY), no_argument, 0, PARSE_REPLY},
        {get_opt_long(PARSE_READ_REPLY), no_argument, 0, PARSE_READ_REPLY},
        {get_opt_long(PARSE_TEST_MALLOC), no_argument, 0, PARSE_TEST_MALLOC},
        {get_opt_long(PARSE_TEST_PRINTF), no_argument, 0, PARSE_TEST_PRINTF},
        {0, 0, 0, 0}
    };
        
    while (1)
    {
        /* getopt_long stores the option index here. */
        int option_index = 0;
        
        c = getopt_long (argc, argv, WHOLE_PARSE_OPT, options, &option_index);
        
        /* Detect the end of the options. */
        if (c == -1)
            break;
        switch (c)
        {
            case PARSE_IPV4:
                inet_pton(AF_INET, optarg, &args->ipv4addr);
                break;
            case PARSE_PORT:
                args->port = atoi(optarg);
                break;
            
            case PARSE_PEER_TYPE:
                if(strcmp(optarg, "client")==0) {
                    args->peer_type = PEER_CLIENT;
                } else if(strcmp(optarg, "server")==0) {
                    args->peer_type = PEER_SERVER;
                } else {
                    print_usage(argv[0]);
                    exit(0);
                }
                break;
            case PARSE_PROTO_TYPE:
                if(strcmp(optarg, "tcp")==0) {
                    args->proto_type = PROTO_TCP;
                } else if(strcmp(optarg, "udp")==0) {
                    args->proto_type = PROTO_UDP;
                } else {
                    print_usage(argv[0]);
                    exit(0);
                }
                break;
            case PARSE_PKG_SIZE:
                args->pkg_size = atoi(optarg);
                break;
            case PARSE_PKG_NUM:
                args->pkg_num = atol(optarg);
                break;
            case PARSE_REPLY:
                args->reply_when_recv = true;
                break;
            case PARSE_READ_REPLY:
                args->recv_after_send = true;
                break;
            case PARSE_TEST_MALLOC:
                args->test_malloc_free = true;
                break;
            case PARSE_TEST_PRINTF:
                args->test_printf = true;
                break;
            case '?':
            default:
                print_usage(argv[0]);
                exit(0);
        }
    }

    if(args->port <= 0 || args->port >= 65535) {
        goto usage_and_exit;
    }
    if(args->peer_type != PEER_SERVER && args->peer_type != PEER_CLIENT) {
        goto usage_and_exit;
    }
    if(args->proto_type != PROTO_TCP && args->proto_type != PROTO_UDP) {
        goto usage_and_exit;
    }
    
    if(args->peer_type==PEER_CLIENT) {
        if(args->pkg_size <= 0 || args->pkg_size >= 65535) {
            goto usage_and_exit;
        }
        if(args->pkg_num <= 0 || args->pkg_num >= 10000000) {
            goto usage_and_exit;
        }
    }

    
    if(args->peer_type == PEER_CLIENT) {
        if(args->ipv4addr == 0) {
            printf("When Client, destination address must need.\n");
            printf("\t-%c, --%s:\t%s\n", PARSE_IPV4, get_opt_long(PARSE_IPV4), get_opt_description(PARSE_IPV4));
            goto usage_and_exit;
        }
    }
    if(args->peer_type==PEER_CLIENT) {
    char addr[16] = {0};
    inet_ntop(AF_INET, &args->ipv4addr, addr, 16);
    printf("Dst Address:    %s\n", addr);
    }
    
    printf("Port:           %d\n", args->port);
    printf("Peer Type:      %s\n", args->peer_type==PEER_SERVER?"Server":(args->peer_type==PEER_CLIENT?"client":"unknown"));
    printf("Proto Type:     %s\n", args->proto_type==PROTO_TCP?"TCP":(args->proto_type==PROTO_UDP?"UDP":"unknown"));
    
    printf("Packet Size:    %d\n", args->pkg_size);
    printf("Packet Number:  %ld\n", args->pkg_num);
    if(args->peer_type==PEER_SERVER)
    printf("Reply Packet:   %s\n", args->reply_when_recv?"Yes":"No");
    if(args->peer_type==PEER_CLIENT)
    printf("Recv Reply:     %s\n", args->recv_after_send?"Yes":"No");
    printf("Test Malloc:    %s\n", args->test_malloc_free?"Yes":"No");
    printf("Test Printf:    %s\n", args->test_printf?"Yes":"No");
    
    return;

usage_and_exit:
    print_usage(argv[0]);
    exit(0);
}

void statistic_throughput(char *description, struct timeval *before, struct timeval *after, unsigned long int bytes)
{
//    printf("\t -- before time: %ld, %ld\n", before->tv_sec, before->tv_usec);
//    printf("\t --  after time: %ld, %ld\n", after->tv_sec, after->tv_usec);
    printf("-- %s: Total %.3lf Mbps, bytes = %ld(bits:%ld)\n", 
                            description?description:"Unknown Description", 
                            8*bytes*1.0/((after->tv_sec-before->tv_sec)*1000000
    						            +after->tv_usec-before->tv_usec), bytes, bytes*8);
}

long int gettimeval(struct timeval *tv)
{
    gettimeofday(tv, NULL);    
}


inline void test_malloc_and_free(char *pkg, unsigned int size)
{
	char *p = malloc(size);
    memset(p, 0, size);
	memcpy(p, pkg, size);
	free(p);
}

inline void test_printf(char *pkg, unsigned int size)
{
	printf("Recv a packet.\n");
}



