#define _GNU_SOURCE
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>
#include <assert.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/un.h>
#include <sys/stat.h>

#include "shmq.h"

#define __SHMQ_MASTER   1
#define __SHMQ_SLAVE    2

/* shmq's logging */
#define __SHMQ_DEBUG    1
#if __SHMQ_DEBUG
#define shmq_dbg(fmt...)        __shmq_log__(__func__, __LINE__, fmt)
#else
#define shmq_dbg(fmt...)        do{}while(0)
#endif
#define shmq_err(fmt...)        __shmq_log__(__func__, __LINE__, fmt)

/* crypto struct: sequence info */
struct __shmq_seq_struct {
    enum{__SHMQ_UNUSED=0,   __SHMQ_USED} used; 
    enum{__SHMQ_WRITEABLE=1,__SHMQ_READABLE} code;
    unsigned long read_calltimes;
    unsigned long write_calltimes;
    unsigned long seq_ack;
}__attribute__((packed));

/* crypto struct: unix socket info */
struct __shmq_unix_sock_struct {
    enum {SRV, CLT} type;
    int fd;
    struct sockaddr_un addr;
}__attribute__((packed));


/* share memory queue struct */
struct shmq_hdr {
    union {
#define SHMQ_NAME_LEN   64        
        char name[SHMQ_NAME_LEN];
        char __unix_sock_path[SHMQ_NAME_LEN];
    };
    
    struct stat stat;
    
    pthread_rwlock_t rwlock;

#define SHMQ_BASIC_NUM_UNSOCK   3  
#define SHMQ_EXTRA_CLT_UNSOCK   0
#define SHMQ_MAX_UNSOCK         (SHMQ_BASIC_NUM_UNSOCK+SHMQ_EXTRA_CLT_UNSOCK)
    struct __shmq_unix_sock_struct unsock[SHMQ_MAX_UNSOCK];

    struct __shmq_seq_struct ensure;

    unsigned int    shm_size;
    unsigned int    data_len;
    
}__attribute__((packed));

struct shmq_struct {

    struct shmq_hdr hdr;

    char            data[0];

}__attribute__((packed));


/* logging */
static inline int __attribute__((unused)) __shmq_log__(const char *func, int line, char *fmt, ...)
{
    int n=0;
    va_list va;
    va_start(va, fmt);
    n+=fprintf(stderr, "[%s:%d] ", func?func:"", line);
    n+=vfprintf(stderr, fmt, va);
    fflush(stderr);
    
    va_end(va);
    
    return n;
}


static int __shmq_unsocket_server(const char *path, struct sockaddr_un *srv_addr)
{    
    int listen_fd, ret;

    if(!path)
    {
        shmq_err("Invalide unix socket path\n");
        return -1;
    }
    
    listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(listen_fd < 0)
    {
        shmq_err("socket error: %s\n", strerror(errno));
        return -1;
    }
    
    srv_addr->sun_family = AF_UNIX;
    strncpy(srv_addr->sun_path, path, sizeof(srv_addr->sun_path)-1);
    
    unlink(path);
    
    ret = bind(listen_fd, (struct sockaddr *)srv_addr, sizeof(struct sockaddr_un));
    if(ret == -1)
    {
        shmq_err("bind error: %s\n", strerror(errno));
        close(listen_fd);
        unlink(path);
        return -1;
    }
    
    ret = listen(listen_fd, 1);
    if(ret == -1)
    {
        shmq_err("listen error: %s\n", strerror(errno));
        close(listen_fd);
        unlink(path);
        return -1;
    }
    
    chmod(path, 00777);
        
    return listen_fd;
}


static int __shmq_unsocket_client(const char *path, struct sockaddr_un *srv_addr)
{
    int connect_fd;
    int ret = 0;
    
    if(!path)
    {
        shmq_err("Invalide unix socket path\n");
        return -1;
    }
    
    connect_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(connect_fd < 0)
    {
        shmq_err("socket error: %s\n", strerror(errno));
        return -1;
    }
    srv_addr->sun_family = AF_UNIX;
    strcpy(srv_addr->sun_path, path);
    
    ret = connect(connect_fd, (struct sockaddr *)srv_addr, sizeof(struct sockaddr_un));
    if(ret == -1)
    {
        shmq_err("connect error: %s\n", strerror(errno));
        close(connect_fd);
        return -1;
    }
    
    return connect_fd;
}


static int __shmq_shm_open(const char *name, int *fd, int shm_size) 
{
    shmq_dbg("\n");

    if(!name || !shm_size)
    {
        shmq_err("Invalide params.\n");
        return 0;
    }
    
    int ret = 0;
    int mfd = shm_open (name, O_RDWR, S_IRUSR | S_IWUSR);
    if (mfd < 0) 
    {
        mfd = shm_open (name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
        if (mfd < 0) 
        {
            fprintf(stderr, "shm_open error: %s\n", strerror(errno));
            return -1;
        }
        ret = 1;
        if (ftruncate(mfd, shm_size) == -1) 
        {
            fprintf(stderr,"ftruncate error: %s\n", strerror(errno));
            return -1;
        }
    }
    shmq_dbg("\n");
    
    *fd = mfd;
    return ret;
}

static void __shmq_sigpipe_handler(int num)
{
    shmq_dbg("Partner Peer closed.\n");
    //shmq_close(shmq);
}


static struct shmq_struct * __shmq_open(const char *name, int shm_size, int master)
{
    shmq_dbg("\n");
    
    if(!name || !shm_size)
    {
        shmq_err("Invalide params.\n");
        return 0;
    }
    
    struct shmq_struct *shmq = NULL;
    int mfd = 0;
    int ret = __shmq_shm_open(name, &mfd, shm_size);
    
    if (mfd < 0)
    {
        shmq_err("__shmq_shm_open error: mfd = %d\n", mfd);
        return NULL;
    }

    int prot = PROT_READ|PROT_WRITE;
    shmq_dbg("prot: %d\n", prot);
    
    shmq = (struct shmq_struct *)mmap(NULL, shm_size, prot, MAP_SHARED, mfd, 0);
    if (shmq == MAP_FAILED) 
    {
        shmq_err("mmap error: %s\n", strerror(errno));
        return NULL;
    }
    fstat(mfd, &shmq->hdr.stat);
    shmq_dbg("stat.st_size = %d\n", shmq->hdr.stat.st_size);
    close(mfd);
    
    if (ret && master == __SHMQ_MASTER) 
    {
        pthread_rwlockattr_t rwattr;
        pthread_rwlockattr_init(&rwattr);
        pthread_rwlockattr_setpshared(&rwattr, PTHREAD_PROCESS_SHARED);
        pthread_rwlock_init(&(shmq->hdr.rwlock), &rwattr);
    }
    
    signal(SIGPIPE, __shmq_sigpipe_handler);
    
    /* create unix socket fd */
    switch(master)
    {
        /* master peer */
        case __SHMQ_MASTER:
        {
            int len;
            shmq->hdr.unsock[0].fd = __shmq_unsocket_server(name, &shmq->hdr.unsock[0].addr);
            shmq_dbg("srv fd %d\n", shmq->hdr.unsock[0].fd);
            shmq->hdr.unsock[2].fd = accept(shmq->hdr.unsock[0].fd, (struct sockaddr*)&shmq->hdr.unsock[2].addr, &len);
            if(shmq->hdr.unsock[2].fd < 0)
            {
                shmq_err("cannot accept client connect request.\n");
                close(shmq->hdr.unsock[0].fd);
                unlink(name);
            }
            shmq_dbg("accept fd %d\n", shmq->hdr.unsock[2].fd);
            
            strncpy(shmq->hdr.name, name, sizeof shmq->hdr.name);
            
            shmq->hdr.shm_size  = shm_size;
            shmq->hdr.data_len  = shm_size-sizeof(struct shmq_hdr);
            
            shmq_dbg("shm: size %d, datalen %d\n", shmq->hdr.shm_size, shmq->hdr.data_len);
            
            shmq->hdr.ensure.code   = __SHMQ_WRITEABLE;
            shmq->hdr.ensure.seq_ack= 0;
            shmq->hdr.ensure.used   = __SHMQ_USED;
            
            shmq_dbg("shm: seq_ack %d, used %d\n", shmq->hdr.ensure.seq_ack, shmq->hdr.ensure.used);
        }
            break;

        /* slave peer */
        case __SHMQ_SLAVE:
            shmq->hdr.unsock[1].fd = __shmq_unsocket_client(name, &shmq->hdr.unsock[1].addr);
            shmq_dbg("clt fd %d\n", shmq->hdr.unsock[1].fd);
            
            shmq_dbg("shm: size %d, datalen %d\n", shmq->hdr.shm_size, shmq->hdr.data_len);
            shmq_dbg("shm: seq_ack %d, used %d\n", shmq->hdr.ensure.seq_ack, shmq->hdr.ensure.used);
            
            break;

        default:
            break;
    }
    
    return (struct shmq_struct *)shmq;
}


/* Open API */
shmq_t shmq_master_open(const char *name, int data_size)
{
    int shm_size = data_size + sizeof(struct shmq_struct);
    
    return (shmq_t)__shmq_open(name, shm_size, __SHMQ_MASTER);
}
shmq_t shmq_slave_open(const char *name, int data_size)
{
    int shm_size = data_size + sizeof(struct shmq_struct);
    
    return (shmq_t)__shmq_open(name, shm_size, __SHMQ_SLAVE);
}


static int __shmq_shm_munmap(struct shmq_struct *shmq) 
{
    if(!shmq)
    {
        shmq_err("Invalide params.\n");
        return 0;
    }

    int shm_size = shmq->hdr.shm_size;
    char name[sizeof shmq->hdr.name] = {0};
    strncpy(name, shmq->hdr.name, sizeof shmq->hdr.name);
    
    if (munmap(shmq, shm_size) == -1) 
    {
        fprintf(stderr, "munmap error: %s\n", strerror(errno));
    }
    if (shm_unlink(name) == -1) 
    {
        fprintf(stderr, "shm_unlink error: %s\n", strerror(errno));
    }
    
    return 0;
}

 
int __shmq_close(struct shmq_struct* shmq)
{
    shmq_dbg("shmq %p\n", shmq);
    
    if(!shmq)
    {
        shmq_err("Invalide params.\n");
        return 0;
    }
    
    int isock;
    for(isock=0;isock<SHMQ_MAX_UNSOCK;isock++)
    {
        if(shmq->hdr.unsock[isock].fd)
            close(shmq->hdr.unsock[isock].fd);
        shmq_dbg("close fd %d\n", shmq->hdr.unsock[isock].fd);
    }

    shmq_dbg("Ready to unlink.\n");
    if(shmq->hdr.__unix_sock_path)
        unlink(shmq->hdr.__unix_sock_path);
    shmq_dbg("unlink: %s\n", shmq->hdr.__unix_sock_path?shmq->hdr.__unix_sock_path:"");

    return __shmq_shm_munmap(shmq);
}
int shmq_close(shmq_t shmq)
{
    return __shmq_close((struct shmq_struct*)shmq);
}  


/**
 *  Read shm queue
 */
static int __shmq_read(struct shmq_struct* shmq, void *buf, int count, int offset)
{
    shmq_dbg("\n");
    
    if(!shmq || !buf || !count)
    {
        shmq_err("Invalide params.\n");
        return 0;
    }

    memset(buf, 0x00, count);
    
    static struct __shmq_seq_struct __seq;
    int n;
    memset(&__seq, 0, sizeof (struct __shmq_seq_struct));
    n = read(shmq->hdr.unsock[1].fd, &__seq, sizeof(struct __shmq_seq_struct));
    
    if(__seq.code != __SHMQ_READABLE && !__seq.used)
    {
        shmq_err("shmq not readable.\n");
        return 0;
    }
    
    shmq_dbg("lock read\n");
    
    pthread_rwlock_rdlock(&shmq->hdr.rwlock);
    
    /* sequence, acknowlegement */
    __seq.code = __SHMQ_WRITEABLE;
    __seq.seq_ack  += 1;
    __seq.used = __SHMQ_USED;

    int read_end_pos = count+offset;
    
    int len = read_end_pos > shmq->hdr.data_len ? shmq->hdr.data_len-offset:count;

    shmq_dbg("read: len = %d\n", len);

    /* memory copy from shmq to data */
    memcpy(buf, shmq->data+offset, len);

    /* sequence, acknowlegement */
    shmq->hdr.ensure.code = __seq.code;
    shmq->hdr.ensure.seq_ack  = __seq.seq_ack;
    shmq->hdr.ensure.used = __seq.used;
    
    
    shmq_dbg("read: seq %d, ack %d, used %d\n", shmq->hdr.ensure.seq_ack, shmq->hdr.ensure.seq_ack, shmq->hdr.ensure.used);
    
    pthread_rwlock_unlock(&shmq->hdr.rwlock);
    
    n = write(shmq->hdr.unsock[1].fd, &__seq, sizeof(struct __shmq_seq_struct));
    shmq_dbg("write: __seq: seq_ack %d, used %d\n", __seq.seq_ack, __seq.used);
    memset(&__seq, 0, sizeof (struct __shmq_seq_struct));
    
    shmq_dbg("unlock read\n");

    return len;
}


int shmq_read(shmq_t shmq, void *buf, int count, int offset)
{
    struct shmq_struct* _shmq = shmq;
    if(_shmq->hdr.data_len<count+offset || count+offset<0 || count<0)
    {
        shmq_err("read out of range. %d>%d need < \n", count+offset, _shmq->hdr.data_len);
        //return 0;
    }
    return __shmq_read((struct shmq_struct*)shmq, buf, count, offset);
}


/**
 *  Write to shm queue
 */
static int __shmq_write(struct shmq_struct* shmq, const void *buf, int count, int offset)
{
    shmq_dbg("\n");

    if(!shmq || !buf || !count)
    {
        shmq_err("Invalide params.\n");
        return 0;
    }
    
    static struct __shmq_seq_struct __seq;
    
    shmq_dbg("lock write\n");
    pthread_rwlock_wrlock(&shmq->hdr.rwlock);
    shmq_dbg("lock write success\n");
    
    if(shmq->hdr.ensure.code != __SHMQ_WRITEABLE)
    {
        shmq_err("shmq not writeable.\n");
    }
    
    int read_end_pos = count+offset;
    
    int len = read_end_pos > shmq->hdr.data_len ? shmq->hdr.data_len-offset:count;
    
    shmq_dbg("write: len = %d\n", len);

    /* memory copy from data to shmq */
    memcpy(shmq->data+offset, buf, len);
    
    shmq->hdr.ensure.code = __SHMQ_READABLE;
    
    shmq_dbg("write: seq_ack %d, used %d\n", shmq->hdr.ensure.seq_ack, shmq->hdr.ensure.used);
    
    /* sequence, acknowlegement */
    __seq.code = shmq->hdr.ensure.code;
    __seq.seq_ack  = shmq->hdr.ensure.seq_ack;
    __seq.used = shmq->hdr.ensure.used;
    
    pthread_rwlock_unlock(&shmq->hdr.rwlock);
    shmq_dbg("unlock write\n");

    shmq_dbg("write to accept fd\n");
    
    int n;
    n = write(shmq->hdr.unsock[2].fd, &__seq, sizeof(struct __shmq_seq_struct));
    if(n == EPIPE)
    {
        shmq_err("Partner Peer closed.\n");
        return 0;
    }
    shmq_dbg("write %d bytes\n", n);
    
    memset(&__seq, 0, sizeof( struct __shmq_seq_struct));
    n = read(shmq->hdr.unsock[2].fd, &__seq, sizeof(struct __shmq_seq_struct));
    shmq_dbg("read: __seq: seq_ack %d, used %d\n", __seq.seq_ack, __seq.used);
    
    memset(&__seq, 0, sizeof( struct __shmq_seq_struct));
    
    return len;
}

int shmq_write(shmq_t shmq, const void *buf, int count, int offset)
{
    struct shmq_struct* _shmq = shmq;
    if(_shmq->hdr.data_len<count+offset || count+offset<0 || count<0)
    {
        shmq_err("write out of range. %d>%d need < \n", count+offset, _shmq->hdr.data_len);
        return 0;
    }   
    return __shmq_write((struct shmq_struct*)shmq, buf, count, offset);
}


