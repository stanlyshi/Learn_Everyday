#include <stdio.h>

#include "crc32c.h"


int main()
{
    char buf[64] = {'A'};
    
    crc32c_init();

    uint32_t crc = crc32c(0, buf, 64);

    printf("buf = %x\n", crc);
}
