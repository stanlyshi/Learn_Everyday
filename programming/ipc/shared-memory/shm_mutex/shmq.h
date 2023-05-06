#ifndef __SHMQ_H
#define __SHMQ_H 1

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <pthread.h>

typedef void*   shmq_t;

#define BYTE    (8)
#define KB      (1024*BYTE)
#define MB      (1024*KB)

#define SHMQ_MB 1/* share memory's size is SHMQ_MB(MB) */

shmq_t shmq_master_open(const char *name, int data_size);
shmq_t shmq_slave_open(const char *name, int data_size);

int shmq_read(shmq_t shmq, void *data, int data_len, int offset);
int shmq_write(shmq_t shmq, const void *data, int data_len, int offset);

int shmq_close(shmq_t shmq);


#endif /*<__SHMQ_H>*/