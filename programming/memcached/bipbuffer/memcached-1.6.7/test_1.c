#include <stdio.h>

#include "bipbuffer.h"

int main()
{
    
    bipbuf_t *buf1 = bipbuf_new(1024);

    
    unsigned char *str = bipbuf_request(buf1, 256);

    bipbuf_free(buf1);
}
