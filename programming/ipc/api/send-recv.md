# send & recv函数
```
#include<sys/socket.h>

ssize_t send(int sockfd, const void* buf, size_t nbytes, int flags);
//成功时返回发送的字节数，失败时返回-1

ssize_t recv(int sockfd, const void* buf, size_t nbytes, int flags);
//成功时返回接收到的字节数（收到EOF时返回0），失败时返回-1
```
> 这两个函数的最后一个参数是收发数据时的可选项，可以进行按位于'|'运算符进行指定多个选项，可选项及其含义如下：

 可选项(Option) | 含义    | send  | recv   
 :------:|:--------:|:-------:|:------:
 MSG_OOB|用于传输带外的数据（out-of-band data)|Y|Y
 MSG_PEEK|验证输入缓冲中是否存在接收的数据|N|Y
 MSG_DONTROUTE|数据传输过程中不参照路由表，在本地网络中寻找目的地|Y|N
 MSG_DONTWAIT|调用IO函数时不阻塞|Y|Y
 MSG_WAITALL|防止函数返回，直到接收请求的全部的字节数|N|Y
