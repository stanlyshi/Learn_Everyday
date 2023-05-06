/********************************************************************\
 *  文件：nmq.h
 *  介绍：nmq.hpp C语言移植，回调函数与 operations 
 *  作者：荣涛
 *  日期：2021年1月22日
\********************************************************************/
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include <assert.h>
#include <string.h>

#include <bones/barrier.h>
#include <bones/cpu.h>
#include <bones/compiler.h>

// POD for header data
struct header {
    unsigned int nodes;
    unsigned int rings;
    unsigned int size;
    size_t msg_size;
};
typedef volatile unsigned int vo_uint; 

// POD for nmq_ring
struct nmq_ring {
    unsigned int _size;
    size_t _msg_size;
    size_t _offset;

    char _pad1[128];
    // R/W access by the reader
    // R/O access by the writer
    vo_uint _head;

    char _pad2[128];    
    // R/W access by the writer
    // R/O access by the reader
    vo_uint _tail;
};

struct context_operations;

typedef struct {
#define CTX_FNAME_LEN   512

    char fname_[CTX_FNAME_LEN];
    void *p_;
    struct header *header_;
    struct nmq_ring *ring_;
    char *data_;

    struct context_operations *ops;
  
}context_t;


struct context_operations {
    force_inline void (*init)(context_t *self, const char *fname);
    force_inline bool (*create)(context_t *self, unsigned int nodes, unsigned int size, unsigned int msg_size);
    force_inline bool (*open)(context_t *self, unsigned int nodes, unsigned int size, unsigned int msg_size);
    force_inline unsigned int (*np2r)(context_t *self, unsigned int from, unsigned int to);
    force_inline void (*print)(context_t *self);
    force_inline struct nmq_ring* (*get_ring)(context_t *self, unsigned int from, unsigned int to);
    force_inline bool (*send)(context_t *self, struct nmq_ring *ring, const void *msg, size_t size);
    force_inline bool (*sendto)(context_t *self, unsigned int from, unsigned int to, const void *msg, size_t size);
    force_inline bool (*sendnb)(context_t *self, unsigned int from, unsigned int to, const void *msg, size_t size);
    force_inline bool (*recv)(context_t *self, struct nmq_ring *ring, void *msg, size_t *size);
    force_inline bool (*recvfrom)(context_t *self, unsigned int from, unsigned int to, void *msg, size_t *size);
    force_inline bool (*recvnb)(context_t *self, unsigned int from, unsigned int to, void *s, size_t *size);
    force_inline bool (*recv2)(context_t *self, unsigned int to, void *msg, size_t *size);
    force_inline ssize_t (*recvnb2)(context_t *self, unsigned int to, void *msg, size_t *size);
};

struct node_operations;

typedef struct  {
    context_t *context_;
    unsigned int node_;
    struct node_operations *ops;
}node_t;

struct node_operations {
    force_inline void (*init)(node_t *self, context_t *context, unsigned int node);
    force_inline bool (*send)(node_t *self, unsigned int to, const void *msg, size_t size);
    force_inline bool (*sendnb)(node_t *self, unsigned int to, const void *msg, size_t size);
    force_inline bool (*recv)(node_t *self, unsigned int from, void *msg, size_t *size);
    force_inline bool (*recvnb)(node_t *self, unsigned int from, void *msg, size_t *size);
    force_inline bool (*recv2)(node_t *self, void *msg, size_t *size);
    force_inline bool (*recvnb2)(node_t *self, void *msg, size_t *size);
};




force_inline static unsigned int po2(unsigned int size) {
    unsigned int i;
    for (i=0; (1U << i) < size; i++);
    return 1U << i;
}


force_inline static void ctx_init(context_t *self, const char *fname)
{
    strncpy(self->fname_, fname, CTX_FNAME_LEN);
}


force_inline static bool ctx_create(context_t *self, unsigned int nodes, unsigned int size, unsigned int msg_size) {
    int fd = open(self->fname_, O_RDWR|O_CREAT|O_EXCL, S_IRUSR|S_IWUSR);
    if (fd == -1) 
        return false;
    
    unsigned int i;
    unsigned int real_size = po2(size);
    unsigned int n_rings = 2*(nodes * (nodes - 1)) / 2;
    unsigned int file_size = sizeof(struct header) + sizeof(struct nmq_ring)*n_rings + n_rings*real_size*msg_size;

    if (ftruncate(fd, file_size) == -1) 
    return false;

    self->p_ = mmap(NULL, file_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (self->p_ == NULL) 
        return false;

    memset(self->p_, 0, file_size);

    self->header_ = (struct header*)self->p_;
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

force_inline static bool ctx_open(context_t *self, unsigned int nodes, unsigned int size, unsigned int msg_size) {
    int fd = open(self->fname_, O_RDWR);
    if (fd == -1)
      return self->ops->create(self, nodes, size, msg_size);
    
    struct stat buf;
    if (fstat(fd, &buf) == -1) 
      return false;
    unsigned int file_size = buf.st_size;
    
    if (ftruncate(fd, file_size) == -1)
      return false;

    self->p_ = mmap(NULL, file_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (self->p_ == NULL)
      return false;
    
    self->header_ = (struct header*)self->p_;
    self->ring_ = (struct nmq_ring*)(self->header_ + 1);
    self->data_ = (char*)(self->ring_ + self->header_->rings);    
    
    return true;
}

// Node pair to nmq_ring
force_inline static unsigned int ctx_np2r(context_t *self, unsigned int from, unsigned int to) {
    assert(from != to);
    assert(from < self->header_->nodes);
    assert(to < self->header_->nodes);
    if (from > to) {
        return to * (self->header_->nodes - 1) + from - 1;
    } else {
        return to * (self->header_->nodes - 1) + from;
    }
}

force_inline static void ctx_print(context_t *self) {
    printf("nodes: %u, size: %u, msgsz: %lu\n", self->header_->nodes, self->header_->size, self->header_->msg_size);
    unsigned int i;
    for (i = 0; i < self->header_->rings; i++) {
        printf("%3i: %10u %10u\n", i, self->ring_[i]._head, self->ring_[i]._tail);
    }
}
force_inline static struct nmq_ring* ctx_get_ring(context_t *self, unsigned int from, unsigned int to) {
    // TODO set errno and return error condition
    assert(self->p_ != NULL);
    return &self->ring_[self->ops->np2r(self, from, to)];
}

force_inline static bool ctx_send(context_t *self, struct nmq_ring *ring, const void *msg, size_t size) {
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

force_inline static bool ctx_sendto(context_t *self, unsigned int from, unsigned int to, const void *msg, size_t size) {
    struct nmq_ring *ring = self->ops->get_ring(self, from, to);
    while (!self->ops->send(self, ring, msg, size)) __relax();
    return true;
}

force_inline static bool ctx_sendnb(context_t *self, unsigned int from, unsigned int to, const void *msg, size_t size) {
    struct nmq_ring *ring = self->ops->get_ring(self, from, to);
    return self->ops->send(self, ring, msg, size);
}

force_inline static bool ctx_recv(context_t *self, struct nmq_ring *ring, void *msg, size_t *size) {
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

force_inline static bool ctx_recvfrom(context_t *self, unsigned int from, unsigned int to, void *msg, size_t *size) {
    struct nmq_ring *ring = self->ops->get_ring(self, from, to);
    while (!self->ops->recv(self, ring, msg, size)) {__relax();}
    return true;
}

force_inline static bool ctx_recvnb(context_t *self, unsigned int from, unsigned int to, void *s, size_t *size) {
    return self->ops->recv(self, self->ops->get_ring(self, from, to), s, size);
}


force_inline static bool ctx_recv2(context_t *self, unsigned int to, void *msg, size_t *size) {
    // TODO "fair" receiving
    unsigned int i;
    while (true) {
        for (i = 0; i < self->header_->nodes; i++) {
            if (to != i && self->ops->recvnb(self, i, to, msg, size)) 
                return true;
        }
        __relax();
    }
    return false;
}
force_inline static ssize_t ctx_recvnb2(context_t *self, unsigned int to, void *msg, size_t *size) {
    // TODO "fair" receiving
    unsigned int i;
    for (i = 0; i < self->header_->nodes; i++) {
        if (to != i && self->ops->recvnb(self, i, to, msg, size)) 
            return true;
    }
    return false;
}


force_inline static void node_init(node_t *self, context_t *context, unsigned int node)
{
    self->node_ = node;
    self->context_ = context;
}


force_inline static bool node_send(node_t *self, unsigned int to, const void *msg, size_t size) {
    return self->context_->ops->sendto(self->context_, self->node_, to, msg, size);
}

force_inline static bool node_sendnb(node_t *self, unsigned int to, const void *msg, size_t size) {
    return self->context_->ops->sendnb(self->context_, self->node_, to, msg, size);
}

force_inline static bool node_recv(node_t *self, unsigned int from, void *msg, size_t *size) {
    return self->context_->ops->recvfrom(self->context_, from, self->node_, msg, size);
}

force_inline static bool node_recvnb(node_t *self, unsigned int from, void *msg, size_t *size) {
    return self->context_->ops->recvnb(self->context_, from, self->node_, msg, size);
}

force_inline static bool node_recv2(node_t *self, void *msg, size_t *size) {
  return self->context_->ops->recv2(self->context_, self->node_, msg, size);
}

force_inline static bool node_recvnb2(node_t *self, void *msg, size_t *size) {
    return self->context_->ops->recvnb2(self->context_, self->node_, msg, size);
}


static struct context_operations default_ctx_ops = {
    .init = ctx_init,
    .create = ctx_create,
    .open = ctx_open,
    .np2r = ctx_np2r,
    .print = ctx_print,
    .get_ring = ctx_get_ring,
    .send = ctx_send,
    .sendto = ctx_sendto,
    .sendnb = ctx_sendnb,
    .recv = ctx_recv,
    .recvfrom = ctx_recvfrom,
    .recvnb = ctx_recvnb,
    .recv2 = ctx_recv2,
    .recvnb2 = ctx_recvnb2,
};



struct node_operations default_node_ops = {
    .init = node_init,
    .send = node_send,
    .sendnb = node_sendnb,
    .recv = node_recv,
    .recvnb = node_recvnb,
    .recv2 = node_recv2,
    .recvnb2 = node_recvnb2,
};


