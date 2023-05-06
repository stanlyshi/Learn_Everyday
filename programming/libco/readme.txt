协程Libco库
https://github.com/Tencent/libco

Libco是ac / c ++协程库，在微信服务中广泛使用。自2013年以来，它已在成千上万的计算机上运行。

通过与libco链接，您可以轻松地将同步后端服务转换为协程服务。与多线程方法相比，协程服务将提供出色的并发性。使用系统挂钩，您可以轻松地以同步方式进行编码，但可以异步执行。

您还可以使用co_create / co_resume / co_yield接口创建异步后端服务。这些界面将使您更好地控制协程。

通过libco复制堆栈模式，您可以轻松地建立支持数千万tcp连接的后端服务。

作者：sunnyxu（sunnyxu@tencent.com），leiffyli（leiffyli@tencent.com），dengoswei@gmail.com（dengoswei@tencent.com），sarlmolchen（sarlmolchen@tencent.com）