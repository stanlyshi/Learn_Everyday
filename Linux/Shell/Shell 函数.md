# Shell 函数

linux shell 可以用户定义函数，然后在shell脚本中可以随便调用。

shell中函数的定义格式如下：

```shell
[ function ] funname [()]

{

  action;

  [return int;]

}
```

说明：

- 1、可以带function fun() 定义，也可以直接fun() 定义,不带任何参数。
- 2、参数返回，可以显示加：return 返回，如果不加，将以最后一条命令运行结果，作为返回值。 return后跟数值n(0-255

下面的例子定义了一个函数并进行调用：

## 实例

```shell
#!/bin/bash*

demoFun**(****)****{**
  **echo** "这是我的第一个 shell 函数!"
**}**
**echo** "-----函数开始执行-----"
demoFun
**echo** "-----函数执行完毕-----"
```

输出结果：

```
-----函数开始执行-----
这是我的第一个 shell 函数!
-----函数执行完毕-----
```

下面定义一个带有return语句的函数：

## 实例

```shell
#!/bin/bash*

funWithReturn**(****)****{**
  **echo** "这个函数会对输入的两个数字进行相加运算..."
  **echo** "输入第一个数字: "
  **read** aNum
  **echo** "输入第二个数字: "
  **read** anotherNum
  **echo** "两个数字分别为 $aNum 和 $anotherNum !"
  **return** $**(****(**$aNum+$anotherNum**)****)**
**}**
funWithReturn
**echo** "输入的两个数字之和为 $? !"
```

输出类似下面：

```
这个函数会对输入的两个数字进行相加运算...
输入第一个数字: 
1
输入第二个数字: 
2
两个数字分别为 1 和 2 !
输入的两个数字之和为 3 !
```

函数返回值在调用该函数后通过 $? 来获得。

注意：所有函数在使用前必须定义。这意味着必须将函数放在脚本开始部分，直至shell解释器首次发现它时，才可以使用。调用函数仅使用其函数名即可。

------

## 函数参数

在Shell中，调用函数时可以向其传递参数。在函数体内部，通过 $n 的形式来获取参数的值，例如，$1表示第一个参数，$2表示第二个参数...

带参数的函数示例：

## 实例

```shell
#!/bin/bash

funWithParam(){
  echo "第一个参数为 $1 !"
  echo "第二个参数为 $2 !"
  echo "第十个参数为 $10 !"
  echo "第十个参数为 ${10} !"
  echo "第十一个参数为 ${11} !"
  echo "参数总数有 $# 个!"
  echo "作为一个字符串输出所有参数 $ !"
}
funWithParam 1 2 3 4 5 6 7 8 9 34 73
```

输出结果：

```
第一个参数为 1 !
第二个参数为 2 !
第十个参数为 10 !
第十个参数为 34 !
第十一个参数为 73 !
参数总数有 11 个!
作为一个字符串输出所有参数 1 2 3 4 5 6 7 8 9 34 73 !
```

注意，$10 不能获取第十个参数，获取第十个参数需要${10}。当n>=10时，需要使用${n}来获取参数。

另外，还有几个特殊字符用来处理参数：

| 参数处理 | 说明                                                         |
| :------- | :----------------------------------------------------------- |
| $#       | 传递到脚本或函数的参数个数                                   |
| $        | 以一个单字符串显示所有向脚本传递的参数                       |
| $$       | 脚本运行的当前进程ID号                                       |
| $!       | 后台运行的最后一个进程的ID号                                 |
| $@       | 与$相同，但是使用时加引号，并在引号中返回每个参数。          |
| $-       | 显示Shell使用的当前选项，与set命令功能相同。                 |
| $?       | 显示最后命令的退出状态。0表示没有错误，其他任何值表明有错误。 |

 [Shell 流程控制](https://www.runoob.com/linux/linux-shell-process-control.html)

[Shell 输入/输出重定向](https://www.runoob.com/linux/linux-shell-io-redirections.html) 

## 2 篇笔记 写笔记

1. 仅对其上一条指令负责，一旦函数返回后其返回值没有立即保存入参数，那么其返回值将不再能通过 $? 获得。

   测试代码：

   ```shell
   #!/bin/bash
   function demoFun1(){
       echo "这是我的第一个 shell 函数!"
       return `expr 1 + 1`
   }
   
   demoFun1
   echo $?
   
   function demoFun2(){
    echo "这是我的第二个 shell 函数!"
    expr 1 + 1
   }
   
   demoFun2
   echo $?
   demoFun1
   echo 在这里插入命令！
   echo $?
   ```

   输出结果：

   ```
   这是我的第一个 shell 函数!
   2
   这是我的第二个 shell 函数!
   2
   0
   这是我的第一个 shell 函数!
   在这里插入命令！
   0
   ```

   调用 demoFun2 后，函数最后一条命令 expr 1 + 1 得到的返回值（$?值）为 0，意思是这个命令没有出错。所有的命令的返回值仅表示其是否出错，而不会有其他有含义的结果。

   第二次调用 demoFun1 后，没有立即查看 $? 的值，而是先插入了一条别的 echo 命令，最后再查看 $? 的值得到的是 0，也就是上一条 echo 命令的结果，而 demoFun1 的返回值被覆盖了。

   下面这个测试，连续使用两次 echo $?，得到的结果不同，更为直观：

   ```shell
   #!/bin/bash
   
   function demoFun1(){
       echo "这是我的第一个 shell 函数!"
       return `expr 1 + 1`
   }
   
   demoFun1
   echo $?
   echo $?
   ```

   输出结果：

   ```
   这是我的第一个 shell 函数!
   2
   0
   ```

   [乱码kun](javascript:;)  乱码kun ly1657282@gmail.com2年前 (2019-07-13)

2. 函数与命令的执行结果可以作为条件语句使用。要注意的是，和 C 语言不同，shell 语言中 0 代表 true，0 以外的值代表 false。

   请参见下例：

   ```shell
   #!/bin/bash
   
   echo "Hello World !" | grep -e Hello
   echo $?
   echo "Hello World !" | grep -e Bye
   echo $?
   if echo "Hello World !" | grep -e Hello
   then
       echo true
   else
       echo false
   fi
   
   if echo "Hello World !" | grep -e Bye
   then
       echo true
   else
       echo false
   fi
   
   function demoFun1(){
       return 0
   }
   
   function demoFun2(){
       return 12
   }
   
   if demoFun1
   then
       echo true
   else
       echo false
   fi
   
   if demoFun2
   then
       echo true
   else
       echo false
   fi
   ```

   其执行结果如下：

   ```
   Hello World !
   0
   1
   Hello World !
   true
   false
   true
   false
   ```

   grep 是从给定字符串中寻找匹配内容的命令。首先看出如果找到了匹配的内容，会打印匹配部分且得到的返回值 $? 为 0，如果找不到，则返回值 $? 为 1。

   接下来分别将这两次执行的 grep 命令当作条件语句交给 if 判断，得出返回值 $? 为 0，即执行成功时，条件语句为 true，当返回值 $? 为 1，即执行失败时，条件语句为 false。

   之后再用函数的 return 值作为测试，其中 demoFun1 返回值为 0，demoFun2 返回值选择了任意一个和 0 不同的整数，这里为 12。

   将函数作为条件语句交给 if 判断，得出返回值为 0 时，依然为 true，而返回值只要不是 0，条件语句都判断为 false。
   
