# sed -i命令详解

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```sh
[root@www ~]# sed [-nefr] [动作]
选项与参数：
-n ：使用安静(silent)模式。在一般 sed 的用法中，所有来自 STDIN 的数据一般都会被列出到终端上。但如果加上 -n 参数后，则只有经过sed 特殊处理的那一行(或者动作)才会被列出来。
-e ：直接在命令列模式上进行 sed 的动作编辑；
-f ：直接将 sed 的动作写在一个文件内， -f filename 则可以运行 filename 内的 sed 动作；
-r ：sed 的动作支持的是延伸型正规表示法的语法。(默认是基础正规表示法语法)
-i ：直接修改读取的文件内容，而不是输出到终端。

动作说明： [n1[,n2]]function
n1, n2 ：不见得会存在，一般代表『选择进行动作的行数』，举例来说，如果我的动作是需要在 10 到 20 行之间进行的，则『 10,20[动作行为] 』

function：
a ：新增， a 的后面可以接字串，而这些字串会在新的一行出现(目前的下一行)～
c ：取代， c 的后面可以接字串，这些字串可以取代 n1,n2 之间的行！
d ：删除，因为是删除啊，所以 d 后面通常不接任何咚咚；
i ：插入， i 的后面可以接字串，而这些字串会在新的一行出现(目前的上一行)；
p ：列印，亦即将某个选择的数据印出。通常 p 会与参数 sed -n 一起运行～
s ：取代，可以直接进行取代的工作哩！通常这个 s 的动作可以搭配正规表示法！例如 1,20s/old/new/g 就是啦！
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)



**sed -i** 就是直接对文本文件进行操作的

```sh
sed -i 's/原字符串/新字符串/' /home/1.txt
sed -i 's/原字符串/新字符串/g' /home/1.txt
```


这两条命令的区别就是，看示例吧

这是1.txt的内容

```sh
#cat 1.txt
d
ddd
#ff
```

再看执行这两条命令的区别吧

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```sh
sed -i 's/d/7523/' /home/1.txt
执行结果
7523
7523dd
#ff

sed -i 's/d/7523/g' /home/1.txt
执行结果
7523
752375237523
#ff
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

**去掉 “行首” 带“@”的首字母@**

```sh
sed -i 's/^@//' file
```

 

**特定字符串的行前插入新行**

```sh
sed -i '/特定字符串/i 新行字符串' file
```

 

**特定字符串的行后插入新行**

```sh
sed -i '/特定字符串/a 新行字符串' file
```

 

**特定字符串的删除**

```sh
sed -i '/字符串/d' file
```
