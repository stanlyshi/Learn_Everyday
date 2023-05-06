#include <stdio.h>
#include <nmq2.h>

int main()
{
    context_t ctx1;
    char *fname = tempnam(NULL, "rtoax-nmq-"); // UGLY
    printf("fname = %s\n", fname);
    
    
    ctx_init(&ctx1, fname);
    
    ctx_create(&ctx1, 4, 10, 1024);

    node_t node0, node1;

    node_init(&node0, &ctx1, 0);
    node_init(&node1, &ctx1, 1);

    unsigned long addr = 1023;
    size_t sz = sizeof(unsigned long);
    
    node_send(&node0, 1, &addr, sizeof(unsigned long));
    addr = 0;

    
    node_recv(&node1, 0, &addr, &sz);
    
    printf("addr = =%ld\n", addr);

    remove(fname);
}
