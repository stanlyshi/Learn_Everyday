### awk后面的RS, ORS, FS, OFS 用法



### 一、RS 与 ORS 差在哪

我们经常会说，awk是基于行列操作文本的，但如何定义“行”呢？这就是RS的作用。
`默认情况下，RS的值是\n`。下面通过实例来理解下RS。 

```shell
root@Debian:~# echo '1a2a3a4a5' | awk '{print $1}'
1a2a3a4a5
root@Debian:~# echo '1a2a3a4a5' | awk 'BEGIN{RS="a"}{print $1}'
1
2
3
4
5
```

我们可以看到，在更改了RS的值后，awk定义的行已经不是我们实际意义中的行了。
上面RS固定的字符串，`RS也可以定义为正则表达式`。 

```shell
root@Debian:~# echo '1ab2bc3cd4de5' | awk 'BEGIN{RS="[a-z]+"}{print $1,RS,RT}'
1 [a-z]+ ab
2 [a-z]+ bc
3 [a-z]+ cd
4 [a-z]+ de
5 [a-z]+ 
```

当我们将RS设置为正则表达式的时候，RT这个变量就有作用了，`RS的值始终为我们设定的正则`，`RT的值则是这个正则实际匹配到的内容`。

如果RS被设置为空，那么awk会将连续的空行作为行分隔符，与RS设置成"\n\n+"有什么区别？？？

  `a. 忽略文件开头和结尾的空行。且文件不以记录分隔符结束，即最后不是空行，会将最后一个记录的尾\n去掉`

  `b. 不设置RT变量（测试未发现规律，暂时认为RT变量不可用）`

  `c. 影响FS变量`



**RS的4种情况**：



**1、RS是记录分隔符，默认分隔符是\n，**

```shell
root@Debian:~# seq 5 | awk '{print $0}'
1
2
3
4
5
```

```shell
root@Debian:~# cat test1
111 222
333 444
555 666

root@Debian:~# awk '{print $0}' test1
111 222
333 444
555 666
```

> 其实你可以把上面test1文件里的内容理解为，111 222\n333 444\n555 6666，利用\n进行分割。



**2、RS为自定义分割符**

以固定字符串作为行分隔符，同时设置变量RT为固定字符串

```shell
root@Debian:~# seq 5 | awk 'BEGIN{ORS="a"}{print $0}'
1a2a3a4a5a
```

```shell
root@Debian:~# echo "111 222|333 444|555 666"|awk 'BEGIN{RS="|"}{print $0,RT}'
111 222 |
333 444 |
555 666
```



**3、RS为正则表达式**

以正则表达式作为行分隔符，同时设置变量RT为正则表达式实际匹配到的字符串

```shell
root@Debian:~# echo "111 222a333 444b555 666"|awk 'BEGIN{RS="[a-z]+"}{print $1,RS,RT}' 
111 [a-z]+ a
333 [a-z]+ b
555 [a-z]+ 
```



**4、RS为空字符**

以连续的空行作为行分隔符，如果FS为单个字符，会将\n强制加入到FS变量中

理解了RS，再来理解ORS就简单了。RS是awk读取文件时的行分隔符，`ORS则是awk输出时的行结束符`。

更简单的讲，就是awk在输出时，会在每行记录后面增加一个ORS变量所设定的值。

ORS的值只能设定为字符串，默认情况下，ORS的值是\n

```shell
root@Debian:~# cat -n test2
     1  111 222
     2
     3  333 444
     4  333 444
     5
     6
     7  555 666

root@Debian:~# awk 'BEGIN{RS=""}{print $0}' test2
111 222
333 444
333 444
555 666

root@Debian:~# awk 'BEGIN{RS="";}{print "<",$0,">"}' test2
< 111 222 >
< 333 444
333 444 >	# 这一行和下面一行，是一行 
< 555 666 >
```



**ORS记录输出分符符，默认值是\n**

把ORS理解成RS反过程，这样更容易记忆和理解，看下面的例子。

```shell
root@Debian:~# awk 'BEGIN{ORS="\n"}{print $0}' test1	# awk '{print $0}' test1二者是一样的
111 222
333 444
555 666
```

```shell
root@Debian:~# awk 'BEGIN{ORS="|"}{print $0}' test1
111 222|333 444|555 666|
```



### 二、FS 与 OFS 差在哪

RS是awk用来定义“行”的，那么FS就是awk用来定义“列”的。

设置变量 FS 与使用 -F 参数是一样的。 



**1、FS默认**

以连续的 `空格` 或 `制表符(\t)` 或 `换行符(\n)` 作为列分隔符

FS 有1个特例，就是将FS设置为一个空格，FS=" " ，这也是FS的默认值 

此时，awk会将连续的`空格`或`制表符(\t)`或`换行符(\n)`作为列的分隔符

```shell
root@Debian:~# echo ' 1 2' | awk 'BEGIN{FS=" "}{print $1}'
1
```

那么，``FS=" " 与 FS="[ \t\n]+" 有区别么？？？`

答案是肯定的 

```shell
root@Debian:~# echo ' 1 2' | awk 'BEGIN{FS=" "}{print $1}'
1
root@Debian:~# echo ' 1 2' | awk 'BEGIN{FS="[ \t\n]+"}{print $1}'

```

`当FS=" "时，awk会自动去掉行首和行尾的空格或制表符(\t)或换行符(\n)`，但`FS="[ \t\n]+"是不会的`
同样，`FS也可以设置为空字符""` 

```shell
root@Debian:~# echo '123' | awk 'BEGIN{FS=""}{print $1,$2}'
1 2
```

`当FS被设置为空字符串的时候，awk会将一行记录的每个字符做为单独的一列`



**2、FS指定字符串**

即，以固定字符串作为列分隔符

```shell
root@Debian:~# echo '1,2' | awk -F , '{print $1}'
1
root@Debian:~# echo '1,2' | awk 'BEGIN{FS=","}{print $1}'
1
```

```shell
root@Debian:~# echo "111|222|333"|awk 'BEGIN{FS="|"}{print $1}'
111
```

**注意**：RS被设定成`非\n`时，`\n`会成FS分割符中的一个

```shell
root@Debian:~# cat test1
111 222
333 444
555 666
root@Debian:~# awk 'BEGIN{RS="444";}{print $2,$3}' test1
222 333
666 
```

222和333之间是有一个\n的，当RS设定成444后，222和333被认定成同一行的二列了，其实按常规思想是二行的一列才对。

类似的，当我们想以固定的长度来分隔列的时候，可以使用 FIELDWIDTHS 来代替 FS

例如，一行记录的前3个字符作为第一列，接下来的2个字符作为第二列，接下来的4个字符作为第三列 

```shell
root@Debian:~# echo '123456789' | awk 'BEGIN{FIELDWIDTHS="3 2 4"}{print $1,$2,$3}'
123 45 6789
root@Debian:~# echo '123456789' | awk 'BEGIN{FIELDWIDTHS="3 2 3"}{print $1,$2,$3}'
123 45 678
root@Debian:~#  echo '123456789' | awk 'BEGIN{FIELDWIDTHS="3 2 5"}{print $1,$2,$3}'
123 45 6789
```

如果定义的长度小于实际的长度，awk会截断，如果大于实际长度，则以实际长度为准。



**3、FS用正则表达式**

以正则表达式作为列分隔符

```shell
root@Debian:~# echo '1ab2bc3cd4de5' | awk 'BEGIN{FS="[a-z]+"}{print $1,$2,$5}'
1 2 5
```

```shell
root@Debian:~# echo "111||222|333"|awk 'BEGIN{FS="[|]+"}{print $1}'  
111
```

**4、FS为空字符""**

即，以每个字符做为单独的一列

```shell
root@Debian:~# echo "111|222|333"|awk 'BEGIN{FS=""}{NF++;print $0}'  
1 1 1 | 2 2 2 | 3 3 3
```

**5、OFS列输出分隔符**

```shell
root@Debian:~# cat test1
111 222
333 444
555 666
root@Debian:~# awk 'BEGIN{OFS="|";}{print $1,$2}' test1' test1
111|222
333|444
555|666
root@Debian:~# awk 'BEGIN{OFS="|";}{print $1 OFS $2}' test1  
111|222
333|444
555|666
```

test1只有二列，如果100列，都写出来太麻烦了吧。

```shell
root@Debian:~# cat test1
111 222
333 444
555 666
root@Debian:~# awk 'BEGIN{OFS="|";}{print $0}' test1
111 222
333 444
555 666
root@Debian:~# awk 'BEGIN{OFS="|";}{NF=NF;print $0}' test1
111|222
333|444
555|666
```

**接下来我们来看看上节提到的问题：**

当 RS="" 时，会将\n强制加入到FS变量中 

```shell
root@Debian:~# cat > urfile <<-EOF
1
a
2
a
3
EOF
root@Debian:~# awk -v RS="" '{print "#" $0 "#"}' urfile
#1
a#
#2
a#
#3#
root@Debian:~# awk -F "b" -v RS="" '{print $1}' urfile
1
2
3
root@Debian:~# awk -F "c" -v RS="" '{print $1}' urfile
1
2
3
root@Debian:~# awk -F "c" -v RS="\n\n+" '{print "#" $1 "#"}' urfile
#1
a#
#2
a#
#3#
```

如果FS为单个字符，\n始终存在在 FS 中，而 RS="\n\n+" 则不会。

了解的 FS ，我们来看看 OFS ，FS是awk读入记录时的列分隔符，OFS则是awk输出时的列分隔符。

我们平时使用的 print $1,$2 等价于 print $1 OFS $2 

```shell
root@Debian:~# echo '1 2' | awk -v OFS="|" '{print $1,$2}'
1|2
root@Debian:~# echo '1 2' | awk -v OFS="|" '{print $1 OFS $2}'
1|2
```

如果一行记录有很多列，同时想改变输出的分隔符，print $1,$2,$3 ... 启不是很麻烦？

```shell
当然有简单的方法： root@Debian:~# echo '1 2 3 4 5' | awk -v OFS="|" '{print $0}'
1 2 3 4 5
root@Debian:~# echo '1 2 3 4 5' | awk -v OFS="|" '{$1=$1;print $0}'
1|2|3|4|5
root@Debian:~# echo '1 2 3 4 5' | awk -v OFS="|" '{NF+=0;print $0}'
1|2|3|4|5
```

为了使OFS的设置生效，需要改变`$0` ，这里我们是对 awk 撒了个小谎

`$1=$1 或者 NF+=0， $0 本身的内容实际上没有任何改变，只是为了使 OFS 的设置生效`

在理解了 RS 和 FS 之后，我们来回顾开始的那句话：“awk是基于行列操作文本的”

这个说法实际上不是很准确，因为在改变了 RS 后，awk 中的“行”已经不是一般的“行”了

同样，改变了 FS 后，awk 中的“列”也已经不是一般的“列”了

因此，准确的应该这样讲：“`awk是基于 记录(record) 和 域(field) 操作文本的`”



### 三、0 与 "0" 差在哪



我们先来看一个例子： 

```shell
root@Debian:~# awk 'BEGIN{if(0) print "true";else print "false"}'
false
root@Debian:~# awk 'BEGIN{if("0") print "true";else print "false"}'
true
```

为什么同样是 0 ，结果却不一样？

其实要解释这个问题，只需要弄清楚awk中的“真”与“假”。

以下3种情况是“假”，其他情况都为“真”

    1) 数字 0
    2) 空字符串
    3) 未定义的值 

```shell
root@Debian:~# awk 'BEGIN{a=0;if(a) print "true";else print "false"}'
false
root@Debian:~# awk 'BEGIN{a="";if(a) print "true";else print "false"}'
false
root@Debian:~# awk 'BEGIN{if(a) print "true";else print "false"}'
false
```

awk如何去重？ 

awk '! a[$0] ++'

在解释之前，我们先要了解awk的一个特性：

awk 会根据语境来给未定义的变量赋初始值

```shell
root@Debian:~# awk 'BEGIN{print a "" 1}'
1
root@Debian:~# awk 'BEGIN{print a + 1}'
1
```

对于未定义的变量，

`如果要进行字符串操作，会被赋成空字符串 ""`

`如果要进行数学运算，会被赋成数字 0`

现在我们看看上面的代码 ! a[$0] ++ 等价于 if(! a[$0] ++) print $0

对于首次出现的记录，a[$0]的值是未定义的，由于后面的 ++ 是数学计算，所以a[$0]会被赋值成数字0

也是由于 ++ 操作符，会先取值，再计算，所以对于第一行记录实际上是if(! 0) print $0

! 是取反，0 是假，! 0 就是真，那么就会执行后面的 print $0

对于后面出现的重复记录，a[$0] 经过 ++ 的计算已经变为 1、2、3 。。。

而 ! 1  ! 2  ! 3 ... 都为假，不会打印。

下面我们用黑哥的一段代码来深刻体会一下，用awk打印奇数行： 

```shell
root@Debian:~# seq 10 | awk 'i=!i'
1
3
5
7
9
```

