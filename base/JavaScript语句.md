**在 HTML 中，JavaScript 语句是由 web 浏览器“执行”的“指令”。**

## 实例

```
var x, y, z;	// 语句 1
x = 22;		// 语句 2
y = 11;		// 语句 3
z = x + y;	// 语句 4
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_statements)

## JavaScript 程序

*计算机程序*是由计算机“执行”的一系列“指令”。

在编程语言中，这些编程*指令*被称为语句。

*JavaScript 程序*就是一系列的编程*语句*。

>**注释：**_在 HTML 中，JavaScript 程序由 web 浏览器执行。

## JavaScript 语句

JavaScript 语句由以下构成：

值、运算符、表达式、关键词和注释。

这条语句告诉浏览器在 id="demo" 的 HTML 元素中输出 "Hello Kitty."：

### 实例

```
document.getElementById("demo").innerHTML = "Hello Kitty.";
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_statement)

大多数 JavaScript 程序都包含许多 JavaScript 语句。

这些语句会按照它们被编写的顺序逐一执行。

>**注释：** _JavaScript 程序（以及 JavaScript 语句）常被称为 JavaScript 代码。

## 分号 ;

分号分隔 JavaScript 语句。

请在每条可执行的语句之后添加分号：

```
a = 5;
b = 6;
c = a + b;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_statements_semicolon1)

如果有分号分隔，允许在同一行写多条语句：

```
a = 5; b = 6; c = a + b;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_statements_semicolon2)

您可能在网上看到不带分号的例子。

>**提示：** _以分号结束语句不是必需的，但我们仍然强烈建议您这么做。

## JavaScript 空白字符

JavaScript 会忽略多个空格。您可以向脚本添加空格，以增强可读性。

下面这两行是相等的：

```
var person = "Bill";
var person="Bill"; 
```

在运算符旁边（ = + - * / ）添加空格是个好习惯：

```
var x = y + z;
```

## JavaScript 行长度和折行

为了达到最佳的可读性，程序员们常常喜欢把代码行控制在 80 个字符以内。

如果 JavaScript 语句太长，对其进行折行的最佳位置是某个运算符：

### 实例

```
document.getElementById("demo").innerHTML =
 "Hello Kitty.";
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_statements_linebreak)

## JavaScript 代码块

JavaScript 语句可以用花括号（{...}）组合在代码块中。

代码块的作用是定义一同执行的语句。

您会在 JavaScript 中看到成块组合在一起的语句：

### 实例

```
function myFunction() {
    document.getElementById("demo").innerHTML = "Hello Kitty.";
    document.getElementById("myDIV").innerHTML = "How are you?";
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_statements_blocks)

>**注释：** _在本教程中我们为代码块用了 4 个空格的缩进。

>**提示：** _您将稍后在本教程中学到更多有关函数的内容。

## JavaScript 关键词

JavaScript 语句常常通过某个关键词来标识需要执行的 JavaScript 动作。

下面的表格列出了一部分将在教程中学到的关键词：

| 关键词        | 描述                                              |
| :------------ | :------------------------------------------------ |
| break         | 终止 switch 或循环。                              |
| continue      | 跳出循环并在顶端开始。                            |
| debugger      | 停止执行 JavaScript，并调用调试函数（如果可用）。 |
| do ... while  | 执行语句块，并在条件为真时重复代码块。            |
| for           | 标记需被执行的语句块，只要条件为真。              |
| function      | 声明函数。                                        |
| if ... else   | 标记需被执行的语句块，根据某个条件。              |
| return        | 退出函数。                                        |
| switch        | 标记需被执行的语句块，根据不同的情况。            |
| try ... catch | 对语句块实现错误处理。                            |
| var           | 声明变量。                                        |

>**注释：** _JavaScript 关键词指的是保留的单词。保留词无法用作变量名。

## 课外阅读

JavaScript 高级教程： [ECMAScript 语法](https://www.w3school.com.cn/js/pro_js_syntax.asp)
