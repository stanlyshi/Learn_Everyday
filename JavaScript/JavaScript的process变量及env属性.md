## const isProduction = process.env.NODE_ENV === 'production'; 作用

## 1.process

要理解 `process.env.NODE_ENV` 就必须要了解 `process`，process 是 node 的全局变量，并且 process 有 env 这个属性，但是没有 NODE_ENV 这个属性。

大家可以创建一个 js （比如index.js）文件，然后打印 `process（console.log(process)）`，在 node 环境下运行，就可以看到 process 里面所有的内容（可以看到里面有 env 这个属性）。 

[![process](https://github.com/stanlyshi/javascript_study/raw/main/base/images/process.png)](https://github.com/stanlyshi/javascript_study/raw/main/base/images/process.png)



## 2.process.env.NODE_ENV的作用

这个变量并不是 `process.env` 直接就有的，而是通过设置得到的。 

这个变量的作用是：我们可以通过判断这个变量区分开发环境或生产环境。



## 3.如何设置 process.env.NODE_ENV

[![env](https://github.com/stanlyshi/javascript_study/raw/main/base/images/env.png)](https://github.com/stanlyshi/javascript_study/raw/main/base/images/env.png)
