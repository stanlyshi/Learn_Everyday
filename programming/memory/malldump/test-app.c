#include <malloc.h>

static void print_mallinfo()
#ifdef DEBUG
{
    struct mallinfo mf1 = mallinfo();
    printf("------------------------------------------\n");
    printf("mallinfo.arena      = %d.\n", mf1.arena);
    printf("mallinfo.ordblks    = %d.\n", mf1.ordblks);
    printf("mallinfo.smblks     = %d.\n", mf1.smblks);
    printf("mallinfo.hblks      = %d.\n", mf1.hblks);
    printf("mallinfo.hblkhd     = %d.\n", mf1.hblkhd);
    printf("mallinfo.usmblks    = %d.\n", mf1.usmblks);
    printf("mallinfo.fsmblks    = %d.\n", mf1.fsmblks);
    printf("mallinfo.uordblks   = %d.\n", mf1.uordblks);
    printf("mallinfo.fordblks   = %d.\n", mf1.fordblks);
    printf("mallinfo.keepcost   = %d.\n", mf1.keepcost);
}
#else
{}
#endif

int main()
{
    char *p1 = malloc(1024);
    print_mallinfo();
    free(p1);
    p1 = malloc(2024);
    print_mallinfo();

    sleep(1000);
}

