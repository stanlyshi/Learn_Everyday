## JavaScript 变量

JavaScript 变量是存储数据值的容器。

在本例中，x、y 和 z 是变量：

### 实例

```javascript
var x = 7;
var y = 8;
var z = x + y; 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_variables)

从上例中，您可获得：

- x 存储值 7
- y 存储值 8
- z 存储值 15

## 类似代数

在本例中，price1、price2 以及 total 是变量：

### 实例

```javascript
var price1 = 7;
var price2 = 8;
var price3 = 12;
var total = price1 + price2 + price3;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_variables_total)

在编程中，类似代数，我们使用变量（比如 price1）来存放值。

在编程中，类似代数，我们在表达式中使用变量（total = price1 + price2）。

从上例中，您可以算出 total 的值是 15。

> **提示：** JavaScript 变量是存储数据值的容器。

## JavaScript 标识符

所有 JavaScript *变量*必须以*唯一的名称*的*标识*。

这些唯一的名称称为*标识符*。

标识符可以是短名称（比如 x 和 y），或者更具描述性的名称（age、sum、totalVolume）。

构造变量名称（唯一标识符）的通用规则是：

- 名称可包含字母、数字、下划线和美元符号
- 名称必须以字母开头
- 名称也可以 $ 和 _ 开头（但是在本教程中我们不会这么做）
- 名称对大小写敏感（y 和 Y 是不同的变量）
- 保留字（比如 JavaScript 的关键词）无法用作变量名称

> **提示：** JavaScript 标识符对大小写敏感。

## 赋值运算符

在 JavaScript 中，等号（=）是赋值运算符，而不是“等于”运算符。

这一点与代数不同。下面的代码在代数中是不合理的：

```javascript
x = x + 5
```

然而在 JavaScript 中，它非常合理：把 x + 5 的值赋给 x。

（计算 x + 5 的值并把结果放入 x 中。x 的值递增 5。）

> **注释：** `JavaScript 中的“等于”运算符是 ==`。

## JavaScript 数据类型

JavaScript 变量可存放数值，比如 100，以及文本值，比如 "Bill Gates"。

在编程中，文本值被称为字符串。

JavaScript 可处理多种数据类型，但是现在，我们只关注数值和字符串值。

字符串被包围在双引号或单引号中。数值不用引号。

如果把数值放在引号中，会被视作文本字符串。

### 实例

```javascript
var pi = 3.14;
var person = "Bill Gates";
var answer = 'How are you!';
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_variables_types)

## 声明（创建） JavaScript 变量

在 JavaScript 中创建变量被称为“声明”变量。

您可以通过 var 关键词来声明 JavaScript 变量：

```javascript
var carName;
```

声明之后，变量是没有值的。（技术上，它的值是 undefined。）

如需*赋值*给变量，请使用等号：

```javascript
carName = "porsche";
```

您可以在声明变量时向它赋值：

```javascript
var carName = "porsche";
```

在上面的例子中，我们创建了名为 carName 的变量，并向其赋值 "porsche"。

然后，我们在 id="demo" 的 HTML 段落中“输出”该值：

### 实例

```html
<p id="demo"></p>

<script>
var carName = "porsche";
document.getElementById("demo").innerHTML = carName; 
</script>
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_variables_create)

> **提示：** 在脚本的开头声明所有变量是个好习惯！

## 一条语句，多个变量

您可以在一条语句中声明许多变量。

以 var 作为语句的开头，并以*逗号*分隔变量：

```javascript
var person = "Bill Gates", carName = "porsche", price = 15000;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_variables_multi)

声明可横跨多行：

```javascript
var person = "Bill Gates",
carName = "porsche",
price = 15000;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_variables_multiline)

## Value = undefined

在计算机程序中，被声明的变量经常是不带值的。值可以是需被计算的内容，或是之后被提供的数据，比如数据输入。

不带有值的变量，它的值将是 undefined。

变量 carName 在这条语句执行后的值是 undefined：

### 实例

```javascript
var carName;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_variables_undefined)

## 重复声明 JavaScript 变量

如果再次声明某个 JavaScript 变量，将不会丢它的值。

在这两条语句执行后，变量 carName 的值仍然是 "porsche"：

### 实例

```javascript
var carName = "porsche";
var carName; 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_variables_redefine)

## JavaScript 算术

与代数类似，您能够通过 JavaScript 变量进行算术运算，使用 = 和 + 之类的运算符：

### 实例

```javascript
var x = 3 + 5 + 8;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_variables_add_numbers)

字符串也可以使用加号，但是字符串将被级联：

### 实例

```javascript
var x = "Bill" + " " + "Gates";
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_variables_add_strings)

还可以试试这个：

### 实例

```javascript
var x = "8" + 3 + 5;
```
`当运算符遇到字符串开始，即将随后需要运算的值转换为字符，故x最后为“835”`

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_variables_add_string_number)

> **提示：** 如果把要给数值放入引号中，其余数值会被视作字符串并被级联。

现在试试这个：

### 实例

```javascript
var x = 3 + 5 + "8";
```
`当运算符遇到字符串开始，即将随后需要运算的值转换为字符，故x最后为“88”`

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_variables_add_number_string)

## 课外阅读

JavaScript 高级教程： [ECMAScript 语法](https://www.w3school.com.cn/js/pro_js_syntax.asp)、 [ECMAScript 变量](https://www.w3school.com.cn/js/pro_js_variables.asp)
