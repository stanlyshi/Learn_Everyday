# readv & writev函数
```
#include<sys/uio.h>

ssize_t writev(int filedes, const struct iovec* iov, int iovcnt);
//成功时返回发送的字节数，失败时返回-1

ssizea_t readv(int filedes, const struct iovec* iov, int iovcnt);
```
- filedes:文件描述符
- iov:iovec结构体数组的首地址
- iovcnt:第二个参数数组的长度

```
struct iovec
{
    void* iov_base; //缓冲地址
    size_t iov_len; //缓冲大小
}
```