**JavaScript 函数是被设计为执行特定任务的代码块。**

**JavaScript 函数会在某代码调用它时被执行。**

## 实例

```javascript
function myFunction(p1, p2) {
    return p1 * p2;              // 该函数返回 p1 和 p2 的乘积
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_function)

## JavaScript 函数语法

JavaScript 函数通过 `function` 关键词进行定义，其后是*函数名*和括号 `()`。

函数名可包含`字母、数字、下划线`和`美元符号`（规则与变量名相同）。

圆括号可包括由逗号分隔的参数：

```javascript
(参数 1, 参数 2, ...)
```

由函数执行的代码被放置在花括号中：*{}*

```javascript
function name(参数 1, 参数 2, 参数 3) {
    要执行的代码
}
```

*函数参数（Function parameters）*是在函数定义中所列的名称。

*函数参数（Function arguments）*是当调用函数时由函数接收的真实的*值*。

在函数中，参数是局部变量。

在其他编程语言中，函数近似程序（Procedure）或子程序（Subroutine）。

## 函数调用

函数中的代码将在其他代码调用该函数时执行：

- 当事件发生时（当用户点击按钮时）
- 当 JavaScript 代码调用时
- 自动的（自调用）

您将在本教程中学到更多有关函数调用的知识。

## 函数返回

当 JavaScript 到达 `return` 语句，函数将停止执行。

如果函数被某条语句调用，JavaScript 将在调用语句之后“返回”执行代码。

函数通常会计算出*返回值*。这个返回值会返回给调用者：

### 实例

计算两个数的乘积，并返回结果：

```javascript
var x = myFunction(7, 8);        // 调用函数，返回值被赋值给 x

function myFunction(a, b) {
    return a * b;                // 函数返回 a 和 b 的乘积
}
```

x 的结果将是：

```
56
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_function_return)

## 为何使用函数？

您能够对代码进行复用：只要定义一次代码，就可以多次使用它。

您能够多次向同一函数传递不同的参数，以产生不同的结果。

### 实例

把华氏度转换为摄氏度：

```javascript
function toCelsius(fahrenheit) {
    return (5/9) * (fahrenheit-32);
}

document.getElementById("demo").innerHTML = toCelsius(77);
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_farenheit_to_celsius)

## () 运算符调用函数

使用上面的例子，toCelsius 引用的是函数对象，而 toCelsius() 引用的是函数结果。

### 实例

访问没有 () 的函数将返回函数定义：

```javascript
function toCelsius(fahrenheit) {
    return (5/9) * (fahrenheit-32);
}

document.getElementById("demo").innerHTML = toCelsius;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_farenheit_to_celsius_2)

## 用作变量值的函数

函数的使用方法与变量一致，在所有类型的公式、赋值和计算中。

### 实例

使用变量来存储函数的值：

```javascript
var x = toCelsius(77);
var text = "The temperature is " + x + " Celsius";
```

您能够把函数当做变量值直接使用：

```javascript
var text = "The temperature is " + toCelsius(77) + " Celsius";
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_function_variable)

您将在本教程中学到更多有关函数的知识。

## 局部变量

在 JavaScript 函数中声明的变量，会成为函数的*局部变量*。

局部变量只能在函数内访问。

### 实例

```javascript
// 此处的代码不能使用 carName

function myFunction() {
    var carName = "Volvo";
    // code here CAN use carName
}

// 此处的代码可以使用 carName
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_function_scope)

由于局部变量只能被其函数识别，因此可以在不同函数中使用相同名称的变量。

局部变量在函数开始时创建，在函数完成时被删除。

## 课外书

如需更多有关 *JavaScript 函数*的知识，请阅读 JavaScript 高级教程中的相关内容：

- [ECMAScript 函数概述](https://www.w3school.com.cn/js/pro_js_functions.asp)

  本节讲解函数的概念，ECMAScript 如何声明并调用函数，以及函数如何返回值。

- [ECMAScript arguments 对象](https://www.w3school.com.cn/js/pro_js_functions_arguments_object.asp)

  本节介绍了此对象的基本用法，然后讲解了如何使用 length 属性来测定函数的参数数目，以及模拟函数重载。

- [ECMAScript Function 对象（类）](https://www.w3school.com.cn/js/pro_js_functions_function_object.asp)

  本节讲解了如何使用 Function 类创建函数，然后介绍了 Function 对象的属性和方法。

- [ECMAScript 闭包（closure）](https://www.w3school.com.cn/js/pro_js_functions_closures.asp)

  本节讲解了闭包（closure）的概念，并分别为您展示了简单和稍复杂的两个闭包实例。
