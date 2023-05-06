#include <stdio.h>
#include <nmq.h>

int main()
{
    context_t ctx1;
    char *fname = tempnam(NULL, "rtoax-nmq-"); // UGLY
    printf("fname = %s\n", fname);
    
    ctx1.ops = &default_ctx_ops;
    ctx1.ops->init(&ctx1, fname);
    ctx1.ops->create(&ctx1, 4, 10, 1024);

    node_t node0, node1;

    node0.ops = &default_node_ops;
    node0.ops->init(&node0, &ctx1, 0);
    node1.ops = &default_node_ops;
    node1.ops->init(&node1, &ctx1, 1);

    unsigned long addr = 1023;
    size_t sz = sizeof(unsigned long);
    
    node0.ops->send(&node0, 1, &addr, sizeof(unsigned long));
    addr = 0;

    
    node1.ops->recv(&node1, 0, &addr, &sz);
    
    printf("addr = =%ld\n", addr);

    remove(fname);
}
