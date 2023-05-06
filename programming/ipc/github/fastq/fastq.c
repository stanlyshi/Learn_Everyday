/**********************************************************************************************************************\
*  文件： fastq.c
*  介绍： 低时延队列
*  作者： 荣涛
*  日期：
*       2021年1月25日    创建与开发轮询功能
*       2021年1月27日 添加 通知+轮询 功能接口，消除零消息时的 CPU100% 问题
*       2021年1月28日 调整代码格式，添加必要的注释
*       2021年2月1日 添加多入单出队列功能 ： 采用 epoll 实现
\**********************************************************************************************************************/
#include <stdint.h>
#include <assert.h>
    
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <syscall.h>
#include <sys/types.h>
#include <sys/eventfd.h> //eventfd
#include <sys/select.h> //FD_SETSIZE
#include <sys/epoll.h>

#include "fastq.h"

#ifdef MODULE_ID_MAX //VOS moduleID
#define FASTQ_ID_MAX    MODULE_ID_MAX
#else
#define FASTQ_ID_MAX    256
#endif


#if (!defined(__i386__) && !defined(__x86_64__))
# error Unsupported CPU
#endif

#ifndef likely
#define likely(x)    __builtin_expect(!!(x), 1)
#endif

#ifndef unlikely
#define unlikely(x)  __builtin_expect(!!(x), 0)
#endif

#define DEBUG
#ifdef DEBUG

#define gettid() syscall(__NR_gettid)
#define LOG_DEBUG(fmt...)  do{printf("\033[33m[%d]", gettid());printf(fmt);printf("\033[m");}while(0)
#else
#define LOG_DEBUG(fmt...) 
#endif


/**
 * The atomic counter structure.
 */
typedef struct {
	volatile int64_t cnt;  /**< Internal counter value. */
} atomic64_t;

// header
struct fastq_header {
    unsigned int nodes;
    unsigned int rings;
    unsigned int size;
    size_t msg_size;
};

// fastq_ring
struct fastq_ring {
    unsigned int _size;
    size_t _msg_size;
    size_t _offset;

    char _pad1[128]; //强制对齐，省的 cacheline 64 字节 的限制，下同
    
    // 读方权限为 读写
    // 写方权限为 只读
    volatile unsigned int _head;

    char _pad2[128];    
    
    // 写方权限为 读写
    // 读方权限为 只读
    volatile unsigned int _tail;

    char _pad3[128];    

    int _evt_fd; //队列eventfd通知
};

// fastq 统计功能
struct fastq_stats {
    unsigned long fastq_id; //用于标识模块或其他任意ID
    atomic64_t rx_npkgs;    //接收总数
    atomic64_t tx_npkgs;    //发送总数
};

/*
    fastq_context 数据结构，详情请见 fastq_create();
    2021年1月27日      荣涛  
    
                |           |           n_rings               |               n_rings               |
                +-----------+--------+--------+-----+---------+----------+----------+-----+---------+
    self->ptr-->|  header   | ring0  |  ring1 | ... |  ringN  |  entity0 |  entity1 | ... | entityN |
                +-----------+--------+--------+-----+---------+----------+----------+-----+---------+
                |           |                                 |                          /          |
    self->hdr---+           |                                 |                         /           |
                            |                                 |                        /            |
                            |                                 |                       /             |
    self->ring--------------+                                 |                      /              |
                                                              |                     /               |
                                                              |                    /                |
    self->data------------------------------------------------+                   /                 |
                                                                                 /                  |
                                                                                /                   |
                                                                               /                    |
                                                                              /                     |
                                                                             /                      |
                                                                            /                       |
                                                                           /                        |
                                                                          /                         |
                                                                         /                          |
                                                                        /                           |
                                                                       /                            |
                                                                      /                             |
                                                                     /                              |
                                                                    /                               |
                                                                   /                                |
                                                                  /                                 |
                                                                 /                                  |
                                                                /                                   |
                                                               /                                    |
                                                              | msg_size|                           |
                                                              +---------+---------+-------+---------+
                                                      entity  |  msg0   |  msg1   |  ...  |  msgM   |
                                                              +---------+---------+-------+---------+
                                                              |         real_size*msg_size          |    
*/
struct fastq_context;


/**
 *  提供一种接收方和发送方 CPU利用率 负载均衡 的策略
 *  倘若一直轮询，当没有消息传递时，CPU利用率也会占满 100%。
 *  为了解决上述问题，提供一种解决方案。
 *  
 *  2021年1月27日 荣涛 rongtao@sylincom.com
 *>>
 *  上述问题得到了初步的解决，参考 NAPI 并不能解决，因为用户
 *  态模拟“中断”的CPU开销是巨大的，同时，引入中断势必要增加大量
 *  代码指令，这对“低时延”是一种较大的打击，所以，最终封装了
 *  两个接口 fastq_sendto_main 和 fastq_recv_main 底层实现 “通知”
 *  后“轮询”的机制解决空闲状态下CPU利用率居高不下的问题。
 *
 */


// 从 event fd 查找 ring 的最快方法
static  struct {
    struct fastq_ring *_ring;
} _evtfd_to_ring[FD_SETSIZE] = {NULL};


// 从 id 查找 context 最快的方法
static struct {
    struct fastq_context *context;
} _id_to_fastq[FASTQ_ID_MAX] = {NULL};


// 内存屏障
static inline void always_inline mbarrier() { asm volatile("": : :"memory"); }
// This version requires SSE capable CPU.
static inline void always_inline mrwbarrier() { asm volatile("mfence":::"memory"); }
static inline void always_inline mrbarrier()  { asm volatile("lfence":::"memory"); }
static inline void always_inline mwbarrier()  { asm volatile("sfence":::"memory"); }
static inline void always_inline __relax()  { asm volatile ("pause":::"memory"); } 
static inline void always_inline __lock()   { asm volatile ("cli" ::: "memory"); }
static inline void always_inline __unlock() { asm volatile ("sti" ::: "memory"); }


static inline int always_inline
atomic64_cmpset(volatile uint64_t *dst, uint64_t exp, uint64_t src)
{
	uint8_t res;

	asm volatile(
			"lock ; "
			"cmpxchgq %[src], %[dst];"
			"sete %[res];"
			: [res] "=a" (res),     /* output */
			  [dst] "=m" (*dst)
			: [src] "r" (src),      /* input */
			  "a" (exp),
			  "m" (*dst)
			: "memory");            /* no-clobber list */

	return res;
}

static inline uint64_t always_inline
atomic64_exchange(volatile uint64_t *dst, uint64_t val)
{
	asm volatile(
			"lock ; "
			"xchgq %0, %1;"
			: "=r" (val), "=m" (*dst)
			: "0" (val),  "m" (*dst)
			: "memory");         /* no-clobber list */
	return val;
}

static inline void always_inline
atomic64_init(atomic64_t *v)
{
	atomic64_cmpset((volatile uint64_t *)&v->cnt, v->cnt, 0);
}

static inline int64_t always_inline
atomic64_read(atomic64_t *v)
{
    return v->cnt;
}

static inline void always_inline
atomic64_set(atomic64_t *v, int64_t new_value)
{
    atomic64_cmpset((volatile uint64_t *)&v->cnt, v->cnt, new_value);
}

static inline void always_inline
atomic64_add(atomic64_t *v, int64_t inc)
{
	asm volatile(
			"lock ; "
			"addq %[inc], %[cnt]"
			: [cnt] "=m" (v->cnt)   /* output */
			: [inc] "ir" (inc),     /* input */
			  "m" (v->cnt)
			);
}

static inline void always_inline
atomic64_sub(atomic64_t *v, int64_t dec)
{
	asm volatile(
			"lock ; "
			"subq %[dec], %[cnt]"
			: [cnt] "=m" (v->cnt)   /* output */
			: [dec] "ir" (dec),     /* input */
			  "m" (v->cnt)
			);
}

static inline void always_inline
atomic64_inc(atomic64_t *v)
{
	asm volatile(
			"lock ; "
			"incq %[cnt]"
			: [cnt] "=m" (v->cnt)   /* output */
			: "m" (v->cnt)          /* input */
			);
}

static inline void always_inline
atomic64_dec(atomic64_t *v)
{
	asm volatile(
			"lock ; "
			"decq %[cnt]"
			: [cnt] "=m" (v->cnt)   /* output */
			: "m" (v->cnt)          /* input */
			);
}

static inline int64_t always_inline
atomic64_add_return(atomic64_t *v, int64_t inc)
{
	int64_t prev = inc;

	asm volatile(
			"lock ; "
			"xaddq %[prev], %[cnt]"
			: [prev] "+r" (prev),   /* output */
			  [cnt] "=m" (v->cnt)
			: "m" (v->cnt)          /* input */
			);
	return prev + inc;
}

static inline int64_t always_inline
atomic64_sub_return(atomic64_t *v, int64_t dec)
{
	return atomic64_add_return(v, -dec);
}

static inline int always_inline
atomic64_inc_and_test(atomic64_t *v)
{
	uint8_t ret;

	asm volatile(
			"lock ; "
			"incq %[cnt] ; "
			"sete %[ret]"
			: [cnt] "+m" (v->cnt), /* output */
			  [ret] "=qm" (ret)
			);

	return ret != 0;
}

static inline int always_inline
atomic64_dec_and_test(atomic64_t *v)
{
	uint8_t ret;

	asm volatile(
			"lock ; "
			"decq %[cnt] ; "
			"sete %[ret]"
			: [cnt] "+m" (v->cnt),  /* output */
			  [ret] "=qm" (ret)
			);
	return ret != 0;
}

static inline int always_inline
atomic64_test_and_set(atomic64_t *v)
{
	return atomic64_cmpset((volatile uint64_t *)&v->cnt, 0, 1);
}

static inline void always_inline
atomic64_clear(atomic64_t *v)
{
	atomic64_set(v, 0);
}


always_inline static unsigned int 
power_of_2(unsigned int size) {
    unsigned int i;
    for (i=0; (1U << i) < size; i++);
    return 1U << i;
}

// Node pair to fastq_ring
always_inline static unsigned int 
fastq_ctx_n2epfd(struct fastq_context *self, unsigned int node) {
    assert(node < self->hdr->nodes);
    return self->pepfd[node];
}


// Node pair to fastq_ring
always_inline static unsigned int 
fastq_ctx_np2r(struct fastq_context *self, unsigned int from, unsigned int to) {
    assert(from != to);
    assert(from < self->hdr->nodes);
    assert(to < self->hdr->nodes);
    if (from > to) {
        return to * (self->hdr->nodes - 1) + from - 1;
    } else {
        return to * (self->hdr->nodes - 1) + from;
    }
}


always_inline  bool 
fastq_create(struct fastq_context *self, unsigned int nodes, unsigned int size, unsigned int msg_size) 
{
    if(unlikely(!self) || unlikely(!size) || unlikely(!msg_size)) {
        fprintf(stderr, "[%s %d] invalid parameters.\n", __func__, __LINE__);
        return false;
    }
    int fd = 0;

    unsigned int i;
    unsigned int from, to;

    /* Ring中的节点数为 2的幂次 */
    unsigned int real_size = power_of_2(size);

    /**
    两节点 A B ，包含 A->B, B->A, n_rings = 2
    三节点 A B C ， 包含 A->B, A->C, B->A, B->C, C->A, C->B, n_rings = 6
     */
    unsigned int n_rings = 2*(nodes * (nodes - 1)) / 2;
    unsigned int file_size = sizeof(struct fastq_header) + sizeof(int)*nodes \
                                + sizeof(struct fastq_ring)*n_rings + n_rings*real_size*msg_size;

    /* 进程间通信的优化在此进行 */
    self->ptr = mmap(NULL, file_size, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, fd, 0);
    if (self->ptr == NULL) 
        return false;

    memset(self->ptr, 0, file_size);

    self->hdr = (struct fastq_header*)self->ptr;
    self->pepfd = (int*)(self->hdr + 1);
    self->ring = (struct fastq_ring*)(self->pepfd + nodes);
    self->data = (char*)(self->ring + n_rings);

    self->hdr->nodes = nodes;
    self->hdr->rings = n_rings;
    self->hdr->size = real_size - 1;
    self->hdr->msg_size = msg_size + sizeof(size_t);

    for (i = 0; i < self->hdr->rings; i++) {
        self->ring[i]._size = real_size - 1;
        self->ring[i]._msg_size = self->hdr->msg_size;
        self->ring[i]._offset = &self->data[i*real_size*msg_size] - self->data;
        self->ring[i]._evt_fd = eventfd(0, EFD_CLOEXEC);
        assert(self->ring[i]._evt_fd); //都TMD没有fd了，你也是厉害
        
        _evtfd_to_ring[self->ring[i]._evt_fd]._ring = &self->ring[i];
    }

    LOG_DEBUG("create epoll.\n");
    
    for (to = 0; to < self->hdr->nodes; to++) {
        self->pepfd[to] = epoll_create(1);
        assert(self->pepfd[to]);
    }
    
    LOG_DEBUG("add eventfd to epoll.\n");
    for (from = 0; from < self->hdr->nodes; from++) {
        for (to = 0; to < self->hdr->nodes; to++) {
            if(from == to) continue;
            
            LOG_DEBUG("add %d to %d.\n", from, to);
            struct fastq_ring *__ring = &self->ring[fastq_ctx_np2r(self, from, to)];
            assert(__ring);
            LOG_DEBUG("add %d to %d, event fd = %d.\n", from, to, __ring->_evt_fd);
            
            struct epoll_event event;
            event.data.fd = __ring->_evt_fd;
            event.events = EPOLLIN; //必须采用水平触发
            epoll_ctl(self->pepfd[to], EPOLL_CTL_ADD, event.data.fd, &event);
        }
    }
    
    return true;
}



always_inline void 
fastq_ctx_print(FILE*fp, struct fastq_context *self) {
    if(unlikely(!self) || unlikely(!fp)) {
        fprintf(stderr, "[%s %d] invalid parameters.\n", __func__, __LINE__);
        return ;
    }
    
    fprintf(fp, "nodes: %u, size: %u, msgsz: %lu\n", \
                self->hdr->nodes, self->hdr->size, self->hdr->msg_size);
    
    unsigned int i;
    for (i = 0; i < self->hdr->rings; i++) {
        fprintf(fp, "%3i: %10u %10u\n", i, self->ring[i]._head, self->ring[i]._tail);
    }
}

always_inline static struct fastq_ring* 
fastq_ctx_get_ring(struct fastq_context *self, unsigned int from, unsigned int to) {
    // TODO set errno and return error condition
    assert(self->ptr != NULL);
    return &self->ring[fastq_ctx_np2r(self, from, to)];
}

always_inline static bool 
fastq_ctx_send(struct fastq_context *self, struct fastq_ring *ring, const void *msg, size_t size) {
    assert(size <= (ring->_msg_size - sizeof(size_t)));

    unsigned int h = (ring->_head - 1) & ring->_size;
    unsigned int t = ring->_tail;
    if (t == h)
        return false;

    char *d = &self->data[self->ring->_offset + t*ring->_msg_size];
    
    memcpy(d, &size, sizeof(size));
    memcpy(d + sizeof(size), msg, size);

    // Barrier is needed to make sure that item is updated 
    // before it's made available to the reader
    mwbarrier();

    ring->_tail = (t + 1) & ring->_size;
    return true;
}

always_inline  bool 
fastq_sendto(struct fastq_context *self, unsigned int from, unsigned int to, const void *msg, size_t size) {
    struct fastq_ring *ring = fastq_ctx_get_ring(self, from, to);
    while (!fastq_ctx_send(self, ring, msg, size)) {__relax();}
    return true;
}

always_inline  bool 
fastq_sendnb(struct fastq_context *self, unsigned int from, unsigned int to, const void *msg, size_t size) {
    struct fastq_ring *ring = fastq_ctx_get_ring(self, from, to);
    return fastq_ctx_send(self, ring, msg, size);
}

always_inline bool 
fastq_sendto_main(struct fastq_context *self, unsigned int from, unsigned int to, const void *msg, size_t size) {
    struct fastq_ring *ring = fastq_ctx_get_ring(self, from, to);
    while (!fastq_ctx_send(self, ring, msg, size)) {__relax();}
    
    eventfd_write(ring->_evt_fd, 1);
    
    return true;
}

always_inline bool 
fastq_sendtry_main(struct fastq_context *self, unsigned int from, unsigned int to, const void *msg, size_t size) {
    struct fastq_ring *ring = fastq_ctx_get_ring(self, from, to);
    bool ret = fastq_ctx_send(self, ring, msg, size);
    if(ret) {
        eventfd_write(ring->_evt_fd, 1);
    }
    return ret;
}



always_inline static bool 
fastq_ctx_recv(struct fastq_context *self, struct fastq_ring *ring, void *msg, size_t *size) {
    unsigned int t = ring->_tail;
    unsigned int h = ring->_head;
    if (h == t)
        return false;

    char *d = &self->data[self->ring->_offset + h*ring->_msg_size];

    size_t recv_size;
    memcpy(&recv_size, d, sizeof(size_t));
    assert(recv_size <= *size && "buffer too small");
    *size = recv_size;
    memcpy(msg, d + sizeof(size_t), recv_size);

    // Barrier is needed to make sure that we finished reading the item
    // before moving the head
    mbarrier();

    ring->_head = (h + 1) & self->ring->_size;
    return true;
}

always_inline  bool 
fastq_recvfrom(struct fastq_context *self, unsigned int from, unsigned int to, void *msg, size_t *size) {
    struct fastq_ring *ring = fastq_ctx_get_ring(self, from, to);
    while (!fastq_ctx_recv(self, ring, msg, size)) {
        __relax();
    }
    return true;
}

always_inline  bool 
fastq_recv_main(struct fastq_context *self, unsigned int node, msg_handler_t handler) {

    eventfd_t cnt;
    int nfds;
    struct epoll_event events[8];
    
    unsigned long addr;
    size_t size = sizeof(unsigned long);
    struct fastq_ring *ring = NULL;

    while(1) {
        nfds = epoll_wait(self->pepfd[node], events, 8, -1);
        for(;nfds--;) {
            ring = _evtfd_to_ring[events[nfds].data.fd]._ring;
            eventfd_read(ring->_evt_fd, &cnt);
//            if(cnt>1) {
//                printf("cnt = =%ld\n", cnt);
//            }
            for(; cnt--;) {
                while (!fastq_ctx_recv(self, ring, &addr, &size)) {
                    __relax();
                }
                
                handler(addr, size);
                addr = 0;
                size = sizeof(unsigned long);
            }

        }
    }
    return true;
}


always_inline  bool 
fastq_recvnb(struct fastq_context *self, unsigned int from, unsigned int to, void *s, size_t *size) {
    return fastq_ctx_recv(self, fastq_ctx_get_ring(self, from, to), s, size);
}


always_inline static bool 
fastq_ctx_recv2(struct fastq_context *self, unsigned int to, void *msg, size_t *size) {
    // TODO "fair" receiving
    unsigned int i;
    while (true) {
        for (i = 0; i < self->hdr->nodes; i++) {
            if (to != i && fastq_recvnb(self, i, to, msg, size)) 
                return true;
        }
        __relax();
    }
    return false;
}
always_inline static ssize_t 
fastq_recvnb2(struct fastq_context *self, unsigned int to, void *msg, size_t *size) {
    // TODO "fair" receiving
    unsigned int i;
    for (i = 0; i < self->hdr->nodes; i++) {
        if (to != i && fastq_recvnb(self, i, to, msg, size)) 
            return true;
    }
    return false;
}

