**JavaScript 注释用于解释 JavaScript 代码，增强其可读性。**

**JavaScript 注释也可以用于在测试替代代码时阻止执行。**

------



## 单行注释

单行注释以 // 开头。

任何位于 // 与行末之间的文本都会被 JavaScript 忽略（不会执行）。

本例在每条代码行之前使用单行注释：

### 实例

```
// 改变标题：
 document.getElementById("myH").innerHTML = "我的第一张页面";
// 改变段落：
 document.getElementById("myP").innerHTML = "我的第一个段落。";
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_comments_1)

本例在每行结尾处使用了单行注释来解释代码：

### 实例

```
var x = 5;      // 声明 x，为其赋值 5
var y = x + 2;  // 声明 y，为其赋值 x + 2
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_comments_2)

## 多行注释

多行注释以 /* 开头，以 */ 结尾。

任何位于 /* 和 */ 之间的文本都会被 JavaScript 忽略。

本例使用多行注释（注释块）来解释代码：

### 实例

```
/*
 下面的代码会改变
 网页中
 id = "myH" 的标题
 以及 id = "myP" 的段落：
*/
document.getElementById("myH").innerHTML = "我的第一张页面";
document.getElementById("myP").innerHTML = "我的第一个段落。"; 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_comments_3)

**注释：**使用单行注释最为常见。

**提示：**注释块常用于官方声明。

## 使用注释来阻止执行

使用注释来防止代码执行很适合代码测试。

在代码行之前添加 // 会把可执行的代码行更改为注释。

本例使用 // 来阻止代码行的执行：

### 实例

```
//document.getElementById("myH").innerHTML = "我的第一张页面";
document.getElementById("myP").innerHTML = "我的第一个段落。"; 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_comments_4)

本例使用注释块来阻止多行代码的执行：

### 实例

```
/*
document.getElementById("myH").innerHTML = "我的第一张页面";
document.getElementById("myP").innerHTML = "我的第一个段落。";
*/
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_comments_5)
