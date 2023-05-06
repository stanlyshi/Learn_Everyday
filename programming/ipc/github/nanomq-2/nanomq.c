/********************************************************************\
*  文件： nanomq.c
*  介绍： 低时延队列
*  作者： 荣涛
*  日期：
*       2021年1月25日    
\********************************************************************/

#include <stdint.h>
#include <assert.h>
    
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "nanomq.h"


#if (!defined(__i386__) && !defined(__x86_64__))
# error Unsupported CPU
#endif

#ifndef likely
#define likely(x)    __builtin_expect(!!(x), 1)
#endif

#ifndef unlikely
#define unlikely(x)  __builtin_expect(!!(x), 0)
#endif

// Forced inlining 
#ifndef force_inline
#define force_inline __attribute__ ((__always_inline__))
#endif


// POD for header data
struct nmq_header {
    unsigned int nodes;
    unsigned int rings;
    unsigned int size;
    size_t msg_size;
};

// POD for nmq_ring
struct nmq_ring {
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


/**
 * Compiler barrier
 */
static inline void force_inline comp() { asm volatile("": : :"memory"); }

// Memory barriers
// This version requires SSE capable CPU.
static inline void force_inline memrw() { asm volatile("mfence":::"memory"); }
static inline void force_inline memr()  { asm volatile("lfence":::"memory"); }
static inline void force_inline memw()  { asm volatile("sfence":::"memory"); }

// mfence ensures that tsc reads are properly serialized
// On Intel chips it's actually enough to just do lfence but
// that would require some conditional logic. 

#if defined __x86_64__
static inline uint64_t __tsc(void)
{
   unsigned int a, d;
   asm volatile ("rdtsc" : "=a" (a), "=d"(d));
   return ((unsigned long) a) | (((unsigned long) d) << 32);
}

#else
static inline uint64_t __tsc(void)
{
   uint64_t c;
   asm volatile ("rdtsc" : "=A" (c));
   return c;
}

#endif

static inline void __relax()  { asm volatile ("pause":::"memory"); } 
static inline void __lock()   { asm volatile ("cli" ::: "memory"); }
static inline void __unlock() { asm volatile ("sti" ::: "memory"); }



force_inline static unsigned int power_of_2(unsigned int size) {
    unsigned int i;
    for (i=0; (1U << i) < size; i++);
    return 1U << i;
}


force_inline  bool nmq_ctx_create(struct nmq_context *self, unsigned int nodes, unsigned int size, unsigned int msg_size) 
{

    int fd = 0;

    unsigned int i;
    unsigned int real_size = power_of_2(size);
    unsigned int n_rings = 2*(nodes * (nodes - 1)) / 2;
    unsigned int file_size = sizeof(struct nmq_header) + sizeof(struct nmq_ring)*n_rings + n_rings*real_size*msg_size;

    self->p_ = mmap(NULL, file_size, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, fd, 0);
    if (self->p_ == NULL) 
        return false;

    memset(self->p_, 0, file_size);

    self->header_ = (struct nmq_header*)self->p_;
    self->ring_ = (struct nmq_ring*)(self->header_ + 1);
    self->data_ = (char*)(self->ring_ + n_rings);

    self->header_->nodes = nodes;
    self->header_->rings = n_rings;
    self->header_->size = real_size - 1;
    self->header_->msg_size = msg_size + sizeof(size_t);

    for (i = 0; i < self->header_->rings; i++) {
        self->ring_[i]._size = real_size - 1;
        self->ring_[i]._msg_size = self->header_->msg_size;
        self->ring_[i]._offset = &self->data_[i*real_size*msg_size] - self->data_;
    }

    return true;
}


// Node pair to nmq_ring
force_inline static unsigned int nmq_ctx_np2r(struct nmq_context *self, unsigned int from, unsigned int to) {
    assert(from != to);
    assert(from < self->header_->nodes);
    assert(to < self->header_->nodes);
    if (from > to) {
        return to * (self->header_->nodes - 1) + from - 1;
    } else {
        return to * (self->header_->nodes - 1) + from;
    }
}

force_inline  void nmq_ctx_print(struct nmq_context *self) {
    printf("nodes: %u, size: %u, msgsz: %lu\n", self->header_->nodes, self->header_->size, self->header_->msg_size);
    unsigned int i;
    for (i = 0; i < self->header_->rings; i++) {
        printf("%3i: %10u %10u\n", i, self->ring_[i]._head, self->ring_[i]._tail);
    }
}
force_inline static struct nmq_ring* nmq_ctx_get_ring(struct nmq_context *self, unsigned int from, unsigned int to) {
    // TODO set errno and return error condition
    assert(self->p_ != NULL);
    return &self->ring_[nmq_ctx_np2r(self, from, to)];
}

force_inline static bool nmq_ctx_send(struct nmq_context *self, struct nmq_ring *ring, const void *msg, size_t size) {
    assert(size <= (ring->_msg_size - sizeof(size_t)));

    unsigned int h = (ring->_head - 1) & ring->_size;
    unsigned int t = ring->_tail;
    if (t == h)
        return false;

    char *d = &self->data_[self->ring_->_offset + t*ring->_msg_size];
    
    memcpy(d, &size, sizeof(size));
    memcpy(d + sizeof(size), msg, size);

    // Barrier is needed to make sure that item is updated 
    // before it's made available to the reader
    memw();

    ring->_tail = (t + 1) & ring->_size;
    return true;
}

force_inline  bool nmq_ctx_sendto(struct nmq_context *self, unsigned int from, unsigned int to, const void *msg, size_t size) {
    struct nmq_ring *ring = nmq_ctx_get_ring(self, from, to);
    while (!nmq_ctx_send(self, ring, msg, size)) {__relax();}
    return true;
}

force_inline  bool nmq_ctx_sendnb(struct nmq_context *self, unsigned int from, unsigned int to, const void *msg, size_t size) {
    struct nmq_ring *ring = nmq_ctx_get_ring(self, from, to);
    return nmq_ctx_send(self, ring, msg, size);
}

force_inline static bool nmq_ctx_recv(struct nmq_context *self, struct nmq_ring *ring, void *msg, size_t *size) {
    unsigned int t = ring->_tail;
    unsigned int h = ring->_head;
    if (h == t)
        return false;

    char *d = &self->data_[self->ring_->_offset + h*ring->_msg_size];

    size_t recv_size;
    memcpy(&recv_size, d, sizeof(size_t));
    assert(recv_size <= *size && "buffer too small");
    *size = recv_size;
    memcpy(msg, d + sizeof(size_t), recv_size);

    // Barrier is needed to make sure that we finished reading the item
    // before moving the head
    comp();

    ring->_head = (h + 1) & self->ring_->_size;
    return true;
}

force_inline  bool nmq_ctx_recvfrom(struct nmq_context *self, unsigned int from, unsigned int to, void *msg, size_t *size) {
    struct nmq_ring *ring = nmq_ctx_get_ring(self, from, to);
    while (!nmq_ctx_recv(self, ring, msg, size)) {__relax();}
    return true;
}

force_inline  bool nmq_ctx_recvnb(struct nmq_context *self, unsigned int from, unsigned int to, void *s, size_t *size) {
    return nmq_ctx_recv(self, nmq_ctx_get_ring(self, from, to), s, size);
}


force_inline static bool nmq_ctx_recv2(struct nmq_context *self, unsigned int to, void *msg, size_t *size) {
    // TODO "fair" receiving
    unsigned int i;
    while (true) {
        for (i = 0; i < self->header_->nodes; i++) {
            if (to != i && nmq_ctx_recvnb(self, i, to, msg, size)) 
                return true;
        }
        __relax();
    }
    return false;
}
force_inline static ssize_t nmq_ctx_recvnb2(struct nmq_context *self, unsigned int to, void *msg, size_t *size) {
    // TODO "fair" receiving
    unsigned int i;
    for (i = 0; i < self->header_->nodes; i++) {
        if (to != i && nmq_ctx_recvnb(self, i, to, msg, size)) 
            return true;
    }
    return false;
}

