#include <stdio.h>

#include "cache.h"

int cache_constructor(void* obj, void* notused1, int notused2)
{
    return printf("cache_constructor\n");
}


void cache_destructor(void* obj, void* notused)
{
    printf("cache_destructor\n");
}


int main()
{
    cache_t* cache1 = cache_create("cache1", 1024, 256,
                                      cache_constructor,
                                      cache_destructor);

    cache_set_limit(cache1, 512);
    
    void *s1 = cache_alloc(cache1);
    void *s2 = cache_alloc(cache1);
    void *s3 = cache_alloc(cache1);
    
    cache_free(cache1, s1);

    cache_destroy(cache1);
}
