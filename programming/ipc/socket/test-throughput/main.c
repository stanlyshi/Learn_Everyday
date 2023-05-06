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

static void server(struct parse_args *args)
{
    switch(args->proto_type) {
        case PROTO_TCP:
            tcp_server_run(args);
            break;
        case PROTO_UDP:
            udp_server_run(args);
            break;
    }
}

static void client(struct parse_args *args)
{
    switch(args->proto_type) {
        case PROTO_TCP:
            tcp_client_run(args);
            break;
        case PROTO_UDP:
            udp_client_run(args);
            break;
    }
}

int main(int argc, char *argv[])
{
    struct parse_args args = ARGS_INITIALIZER;

    parse_args_config(argc, argv, &args);

    switch(args.peer_type) {
        case PEER_SERVER:
            server(&args);
            break;
        case PEER_CLIENT:
            client(&args);
            break;
    }


    return 0;
}
