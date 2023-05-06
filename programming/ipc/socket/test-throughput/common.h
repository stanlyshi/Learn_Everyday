#ifndef _COMMON_H
#define _COMMON_H

#include <netinet/tcp.h>
#include <getopt.h>
#include <stdbool.h>

#define MAX_LINE       65535
//#define PORT           2152
#define BACKLOG        10
#define LISTENQ        6666
#define MAX_CONNECT    20

typedef enum {PEER_SERVER=1, PEER_CLIENT}peer_type_t;
typedef enum {PROTO_TCP=1, PROTO_UDP} proto_type_t;

typedef enum {
    PARSE_IPV4 = 'a',
#define PARSE_IPV4_LONG        "dst-addr"
#define PARSE_IPV4_DESCRPTION  "address to link, client only, example: 10.170.6.66"

    PARSE_PORT = 'p',
#define PARSE_PORT_LONG        "port"
#define PARSE_PORT_DESCRPTION  "port to link, [0-65535]"

    PARSE_PEER_TYPE = 'T',
#define PARSE_PEER_TYPE_LONG        "peer-type"
#define PARSE_PEER_TYPE_DESCRPTION  "server or client"

    PARSE_PROTO_TYPE = 't',
#define PARSE_PROTO_TYPE_LONG       "proto-type"
#define PARSE_PROTO_TYPE_DESCRPTION "tcp or udp"

    PARSE_PKG_SIZE = 's',
#define PARSE_PKG_SIZE_LONG         "pkg-size"
#define PARSE_PKG_SIZE_DESCRPTION   "size of per recv or send packet, [0-65535]"

    PARSE_PKG_NUM = 'n',
#define PARSE_PKG_NUM_LONG          "pkg-num"
#define PARSE_PKG_NUM_DESCRPTION    "number of all packet, that will send or receive, [0-10000000]"
    
    PARSE_REPLY = 'w',
#define PARSE_REPLY_LONG          "reply"
#define PARSE_REPLY_DESCRPTION    "when receive a packet, will reply total same size packet to peer"

    PARSE_READ_REPLY = 'r',
#define PARSE_READ_REPLY_LONG          "read-reply"
#define PARSE_READ_REPLY_DESCRPTION    "after write a packet, will read a packet from peer"

    PARSE_TEST_MALLOC = 'm',
#define PARSE_TEST_MALLOC_LONG          "test-malloc"
#define PARSE_TEST_MALLOC_DESCRPTION    "when receive a packet, will malloc,memset and free"


    PARSE_TEST_PRINTF = 'f',
#define PARSE_TEST_PRINTF_LONG          "test-printf"
#define PARSE_TEST_PRINTF_DESCRPTION    "when receive a packet, will print a log"
}parse_args_type_t;

#define WHOLE_PARSE_OPT "a:T:t:s:n:wrmf"

#define get_opt_long(key) key##_LONG
#define get_opt_description(key) key##_DESCRPTION


struct parse_args {
    unsigned int ipv4addr;
    int port;
    peer_type_t  peer_type;  //端类型
    proto_type_t proto_type; //协议类型

    unsigned int pkg_size;
    unsigned long int pkg_num;

    bool reply_when_recv;
    bool recv_after_send;


    bool test_malloc_free;
    bool test_printf;
#define ARGS_INITIALIZER    {0,0,0,0,0,0,false, false, false}   
};


void parse_args_config(int argc, char **argv, struct parse_args *args);

int tcp_server_run(struct parse_args *arguments);
int udp_server_run(struct parse_args *arguments);

int tcp_client_run(struct parse_args *arguments);
int udp_client_run(struct parse_args *arguments);



int tcpsocket_server(int port);
int tcpsocket_client(const char *ipv4, struct sockaddr_in *servaddr, int port);

int udpsocket_server(int port);
int udpsocket_client(const char *ipv4, struct sockaddr_in *servaddr, int port);



void statistic_throughput(char *description, struct timeval *before, struct timeval *after, unsigned long int bytes);

long int gettimeval(struct timeval *tv);

inline void test_malloc_and_free(char *pkg, unsigned int size);
inline void test_printf(char *pkg, unsigned int size);


#endif /*<_COMMON_H>*/
