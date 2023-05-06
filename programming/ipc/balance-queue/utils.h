/**
 *  utils.h
 */

#ifndef __x86_64__
# error "Not support Your Arch, just support x86-64"
#endif

#ifndef __BALANCE_QUEUE_UTILS_H
#define __BALANCE_QUEUE_UTILS_H 1

#include <errno.h>
#include <assert.h>
#include <stdbool.h>

/**
 *  获取tick
 */
#ifndef RDTSC
#define RDTSC() ({\
    register uint32_t a,d; \
    __asm__ __volatile__( "rdtsc" : "=a"(a), "=d"(d)); \
    (((uint64_t)a)+(((uint64_t)d)<<32)); \
    })
#endif
    
/**
 *  cas，默认 smp 架构
 */
#ifndef CAS
#define CAS(ptr, val_old, val_new) ({ \
    char ret; \
    __asm__ __volatile__("lock; "\
        "cmpxchgl %2,%0; setz %1"\
        : "+m"(*ptr), "=q"(ret)\
        : "r"(val_new),"a"(val_old)\
        : "memory"); \
    ret;})
#endif

#ifndef UNUSED
#define UNUSED __attribute__((unused))
#endif
#ifndef likely
#define likely(exp) __builtin_expect(!!exp, 1)
#endif
#ifndef unlikely
#define unlikely(exp) __builtin_expect(!!exp, 0)
#endif

#ifndef CACHELINE_ALIGN
#define CACHELINE_SIZE 64
#define CACHELINE_ALIGN __attribute__((aligned(CACHELINE_SIZE)))
#endif 


typedef unsigned long bits_set;
#define BITS_SET_INITIALIZER 0UL

#define BITS_SETSIZE                    64
#define BITS_SET(bit, p_bits_set)       (*(p_bits_set) |= 1UL<<bit)
#define BITS_CLR(bit, p_bits_set)       (*(p_bits_set) &= ~(1UL<<bit))
#define BITS_ISSET(bit, p_bits_set)     (*(p_bits_set) & (1UL<<bit))
#define BITS_ZERO(p_bits_set)           (*(p_bits_set) = 0UL)

#endif /*<__BALANCE_QUEUE_UTILS_H>*/
