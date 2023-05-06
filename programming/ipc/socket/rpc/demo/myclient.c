/**
 * References:
 *
 * http://www.cs.rutgers.edu/~pxk/417/notes/sockets/udp.html
 * http://stackoverflow.com/questions/9778806/serializing-a-class-with-a-pointer-in-c
 * http://stackoverflow.com/questions/504810/how-do-i-find-the-current-machines-full-hostname-in-c-hostname-and-domain-info
 *
 * Coding Style:
 *
 * http://www.cs.swarthmore.edu/~newhall/unixhelp/c_codestyle.html
 */
    
#include <stdio.h>
#include <stdlib.h>

#include "rpc_types.h"

int main(int argc, char *argv[]) 
{
    if(argc < 3)
    {
        printf("Usage: %s hostname/ip port\n", argv[0]);
        exit(1);
    }
    
    char *hostname = argv[1];
    int port = atoi(argv[2]);
    
    int a = -10, b = 20;
    return_type ans;
    ans = make_remote_call(hostname,
                           port, "addtwo", 2,
                           sizeof(int), (void *)(&a),
                           sizeof(int), (void *)(&b));
    int result = *(int *)(ans.return_val);

    printf("Client, got result: %d\n", result);

    ans = make_remote_call(hostname,
                           port, "addtwo1", 2,
                           sizeof(int), (void *)(&a),
                           sizeof(int), (void *)(&b));


    ans = make_remote_call(hostname,
                           port, "", 2,
                           sizeof(int), (void *)(&a),
                           sizeof(int), (void *)(&b));


    ans = make_remote_call(hostname,
                           port, "warrenthefuckingmansmith", 2,
                           sizeof(int), (void *)(&a),
                           sizeof(int), (void *)(&b));

    result = *(int *)(ans.return_val);

    printf("Client, got result: %d\n", result);

    return 0;
}
