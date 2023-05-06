
# fcntl函数
```
#include<fcntl.h>

fcntl(sockRecv, F_SETOWN, getpid());
```
> 上述函数调用的作用是：将文件描述符`sockRecv`指向的拥有者改为`getpid()`函数返回的ID的进程；处理SIGURG信号时必须指定处理信号的进程.

# 检查输入缓冲
> `MSG_PEEK`和`MSG_DONTWAIT`同时使用时可以用来检查输入缓冲区是否有待接收的数据，并且不会删除这些数据.

