/**
 *  bqueue.h
 */

#ifndef __x86_64__
# error "Not support Your Arch, just support x86-64"
#endif

#ifndef __BALANCE_QUEUE_H
#define __BALANCE_QUEUE_H 1

#include <sys/types.h>


// POD for ring
struct ring {
    unsigned int _size;
    size_t _msg_size;
    size_t _offset;

    char _pad1[128];
    // R/W access by the reader
    // R/O access by the writer
    volatile unsigned int _head;

    char _pad2[128];    
    // R/W access by the writer
    // R/O access by the reader
    volatile unsigned int _tail;
};


typedef struct {
    struct header {
        unsigned int nodes;
        unsigned int rings;
        unsigned int size;
        size_t msg_size;
    };
    struct ring ring_;
}bqueue_t;


#endif /*<__BALANCE_QUEUE_H>*/
