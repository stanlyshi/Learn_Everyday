**JavaScript 语法是一套规则，它定义了 JavaScript 的语言结构。**

```javascript
var x, y;	// 如何声明变量
x = 7; y = 8;	// 如何赋值
z = x + y;	// 如何计算值
```

## JavaScript 值

JavaScript 语句定义两种类型的值：混合值和变量值。

混合值被称为**字面量（literal）**。变量值被称为**变量**。

## JavaScript 字面量

书写混合值最重要的规则是：

写**数值**有无小数点均可：

```javascript
15.90

10011
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_syntax_numbers)

**字符串**是文本，由双引号或单引号包围：

```javascript
"Bill Gates"

'Bill Gates' 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_syntax_strings)

## JavaScript 变量

在编程语言中，*变量*用于*存储*数据值。

JavaScript 使用 var 关键词来*声明*变量。

= 号用于为变量*赋值*。

在本例中，x 被定义为变量。然后，x 被赋的值是 7：

```javascript
var x;

x = 7;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_syntax_variables)

## JavaScript 运算符

JavaScript 使用*算数运算符*（+ - * /）来*计算值*：

```javascript
(7 + 8) * 10
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_syntax_operators)

JavaScript 使用*赋值运算符*（=）向变量*赋值*：

```javascript
var x, y;
var x = 7;
var y = 8;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_syntax_assignment)

## JavaScript 表达式

表达式是值、变量和运算符的组合，计算结果是值。

```javascript
6 * 10
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_syntax_expressions)

表达式也可包含变量值：

```javascript
x * 10
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_syntax_expressions_variables)

值可以是多种类型，比如数值和字符串。

例如，"Bill" + " " + "Gates"，计算为 "Bill Gates"：

```javascript
"Bill" + " " + "Gates"
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_syntax_expressions_strings)

## JavaScript 关键词

JavaScript *关键词*用于标识被执行的动作。

var 关键词告知浏览器创建新的变量：

```javascript
var x = 7 + 8;
var y = x * 10; 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_syntax_keywords)

## JavaScript 注释

并非所有 JavaScript 语句都被“执行”。

双斜杠 // 或 /* 与 **/* 之间的代码被视为*注释*。

注释会被忽略，不会被执行：

```javascript
var x = 7;   // 会执行

// var x = 8;   不会执行
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_syntax_comments)

## JavaScript 标识符

标识符是名称。

在 JavaScript 中，标识符用于命名变量（以及关键词、函数和标签）。

在大多数编程语言中，合法名称的规则大多相同。

在 JavaScript 中，首字符必须是字母、下划线（-）或美元符号（$）。

连串的字符可以是字母、数字、下划线或美元符号。

**提示：**数值不可以作为首字符。这样，JavaScript 就能轻松区分标识符和数值。

## JavaScript 对大小写敏感

所有 JavaScript 标识符*对大小写敏感*。

[^]: 大写与小写区别对待



变量 lastName 和 lastname，是两个不同的变量。

```javascript
lastName = "Gates";
lastname = "Jobs"; 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_syntax_case)

JavaScript 不会把 *VAR* 或 *Var* 译作关键词 *var*。

## JavaScript 与驼峰式大小写

历史上，程序员曾使用三种把多个单词连接为一个变量名的方法：

### 连字符：

```javascript
first-name, last-name, master-card, inter-city.
```

**注释：**JavaScript 中不能使用连字符。它是为减法预留的。

### 下划线：

```javascript
first_name, last_name, master_card, inter_city.
```

### 驼峰式大小写（Camel Case）：

```javascript
FirstName, LastName, MasterCard, InterCity.
```

![camelCase](https://www.w3school.com.cn/i/camelcase.png)

JavaScript 程序员倾向于使用以小写字母开头的驼峰大小写：

```javascript
firstName, lastName, masterCard, interCity
```

## JavaScript 字符集

JavaScript 使用 *Unicode* 字符集。

Unicode 覆盖世界上几乎所有的字符、标点和符号。

如需了解详情，请访问我们完整的 [Unicode 参考手册](https://www.w3school.com.cn/tags/html_ref_utf8.asp)。
