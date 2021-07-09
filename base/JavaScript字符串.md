**JavaScript 字符串用于存储和操作文本。**

## JavaScript 字符串

JavaScript 字符串是引号中的零个或多个字符。

### 实例

```javascript
var x = "Bill Gates"
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_string)

您能够使用单引号或双引号：

### 实例

```javascript
var carname = "Porsche 911";
var carname = 'Porsche 911';
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_string_quotes)

您可以在字符串中使用引号，只要不匹配围绕字符串的引号即可：

### 实例

```javascript
var answer = "It's good to see you again!";
var answer = "He is called 'Bill'";
var answer = 'He is called "Bill"';
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_string_quotes_mixed)

## 字符串长度

内建属性 length 可返回字符串的*长度*：

### 实例

```javascript
var txt = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
var sln = txt.length;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_string_length)

## 特殊字符

由于字符串必须由引号包围，JavaScript 会误解这段字符串：

```javascript
var y = "中国是瓷器的故乡，因此 china 与"China（中国）"同名。"
```

该字符串将被切为 "中国是瓷器的故乡，因此 china 与"。

避免此问题的解决方法是，使用 *\ 转义字符*。

反斜杠转义字符把特殊字符转换为字符串字符：

| 代码 | 结果 | 描述   |
| :--- | :--- | :----- |
| \'   | '    | 单引号 |
| \"   | "    | 双引号 |
| \\   | \    | 反斜杠 |

### 实例

序列 \" 在字符串中插入双引号：

### 实例

```javascript
var x = "中国是瓷器的故乡，因此 china 与\"China（中国）\"同名。"
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_string_escape_quotes_1)

序列 \' 在字符串中插入单引号：

### 实例

```javascript
var x = 'It\'s good to see you again';
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_string_escape_quotes_2)

序列 \\ 在字符串中插入反斜杠：

### 实例

```javascript
var x = "字符 \\ 被称为反斜杠。";
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_string_escape_backslash)

转义字符（\）也可用于在字符串中插入其他特殊字符。

其他六个 JavaScript 中有效的转义序列：

| 代码 | 结果       |
| :--- | :--------- |
| \b   | 退格键     |
| \f   | 换页       |
| \n   | 新行       |
| \r   | 回车       |
| \t   | 水平制表符 |
| \v   | 垂直制表符 |

这六个转义字符最初设计用于控制打字机、电传打字机和传真机。它们在 HTML 中没有任何意义。

## 长代码行换行

为了最佳可读性， 程序员们通常会避免每行代码超过 80 个字符串。

如果某条 JavaScript 语句不适合一整行，那么最佳换行位置是某个运算符之后：

### 实例

```javascript
document.getElementById("demo").innerHTML =
"Hello Kitty.";
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_statements_linebreak)

您也可以*在字符串中*换行，通过一个反斜杠即可：

### 实例

```javascript
document.getElementById("demo").innerHTML = "Hello \
Kitty!";
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_string_break_1)

\ 方法并不是 ECMAScript (JavaScript) 标准。

某些浏览器也不允许 \ 字符之后的空格。

对长字符串换行的最安全做法（但是有点慢）是使用字符串加法：

### 实例

```javascript
document.getElementById("demo").innerHTML = "Hello" + 
"Kitty!";
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_string_break_2)

您不能通过反斜杠对代码行进行换行：

### 实例

```javascript
document.getElementById("demo").innerHTML = \ 
"Hello Kitty!";
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_string_break_code)

## 字符串可以是对象

通常，JavaScript 字符串是原始值，通过字面方式创建：

```javascript
var firstName = "Bill"
```

但是字符串也可通过关键词 new 定义为对象：

```javascript
var firstName = new String("Bill")
```

### 实例

```javascript
var x = "Bill";
var y = new String("Bill");

// typeof x 将返回 string
// typeof y 将返回 object
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_string_object_1)

> 请不要把字符串创建为对象。它会拖慢执行速度。
>
> new 关键字使代码复杂化。也可能产生一些意想不到的结果。

当使用 == 相等运算符时，相等字符串是相等的：

### 实例

```javascript
var x = "Bill";             
var y = new String("Bill");

// (x == y) 为 true，因为 x 和 y 的值相等
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_string_object_2)

当使用 === 运算符时，相等字符串是不相等的，因为 === 运算符需要类型和值同时相等。

### 实例

```javascript
var x = "Bill";             
var y = new String("Bill");

// (x === y) 为 false，因为 x 和 y 的类型不同（字符串与对象）
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_string_object_3)

甚至更糟。对象无法比较：

### 实例

```javascript
var x = new String("Bill");             
var y = new String("Bill");

// (x == y) 为 false，因为 x 和 y 是不同的对象
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_string_object_4)

### 实例

```javascript
var x = new String("Bill");             
var y = new String("Bill");

// (x === y) 为 false，因为 x 和 y 是不同的对象
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_string_object_5)

请注意 (x==y) 与 (x===y) 的区别。

JavaScript 对象无法进行对比，比较两个 JavaScript 将始终返回 false。
