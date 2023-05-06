## 信号量
```
#include<semaphore.h>
int sem_init(sem_t* sem, int pshared, unsigned int value);
int sem_destroy(sem_t* sem);
//成功时返回0，失败时返回其他值
```
- sem:保存信号量的变量地址值
- pshared:传递其他值时，创建多个进程共享的信号量，传递0时，创建只允许一个进程使用的信号量
- value:指定初始值

```
#include<semaphore.h>
int sem_post(sem_t* sem);
int sem_wait(sem_t* sem);
//成功时返回0，失败时返回其他值
```
- sem:传递给sem_post时信号量增加1，传递给sem_wait时信号量减少1
