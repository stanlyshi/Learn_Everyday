## JavaScript 运算符

### 实例

向变量赋值，并把它们相加：

```javascript
var x = 7;		// 向 x 赋值 7
var y = 8;		// 向 y 赋值 8
var z = x + y;		// 向 z (x + y) 赋值 15
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_operator)

*赋值*运算符（=）把值赋给变量。

### 赋值

```javascript
var x = 15;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_operator_equal)

*加法*运算符（+）对数字相加：

### 加法

```javascript
var x = 7;
var y = 8;
var z = x + y;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_operator_addition)

*乘法*运算符（*）对数字相乘：

### 乘法

```javascript
var x = 7;
var y = 8;
var z = x * y; 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_operator_multiplication)

## JavaScript 算数运算符

算数运算符用于对数字执行算数运算：

| 运算符 | 描述 |
| :----- | :--- |
| +      | 加法 |
| -      | 减法 |
| *      | 乘法 |
| /      | 除法 |
| %      | 系数 |
| ++     | 递加 |
| --     | 递减 |

>**注释：** [JS 算数](https://www.w3school.com.cn/js/js_arithmetic.asp)这一章对算数运算符进行了完整描述。

## JavaScript 赋值运算符

赋值运算符向 JavaScript 变量赋值。

| 运算符 | 例子   | 等同于    |
| :----- | :----- | :-------- |
| =      | x = y  | x = y     |
| +=     | x += y | x = x + y |
| -=     | x -= y | x = x - y |
| *=     | x *= y | x = x * y |
| /=     | x /= y | x = x / y |
| %=     | x %= y | x = x % y |

加法赋值运算符（+=）向变量添加一个值。

### 赋值

```javascript
var x = 7;
x += 8; 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_operator_plusequal)

>**注释：** [JS 赋值](https://www.w3school.com.cn/js/js_assignment.asp)这一章中完整描述了赋值运算符。

## JavaScript 字符串运算符

\+ 运算符也可用于对字符串进行相加（concatenate，级联）。

### 实例

```javascript
txt1 = "Bill";
txt2 = "Gates";
txt3 = txt1 + " " + txt2; 
```

txt3 的结果将是：

```
Bill Gates
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_operator_concatenate_1)

+= 赋值运算符也可用于相加（级联）字符串：

### 实例

```javascript
txt1 = "Hello ";
txt1 += "Kitty!"; 
```

txt1 的结果将是：

```
Hello Kitty!
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_operator_concatenate_2)

**提示：**在用于字符串时，**+** 运算符被称为级联运算符。

## 字符串和数字的相加

相加两个数字，将返回和，但对一个数字和一个字符串相加将返回一个字符串：

### 实例

```javascript
x = 7 + 8;
y = "7" + 8;
z = "Hello" + 7;
```

x、y 和 z 的结果将是：

```
15
78
Hello7
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_operator_concatenate_3)

**提示：**如果您对数字和字符串相加，结果将是字符串！

## JavaScript 比较运算符

| 运算符 | 描述           |
| :----- | :------------- |
| ==     | 等于           |
| ===    | 等值等型       |
| !=     | 不相等         |
| !==    | 不等值或不等型 |
| >      | 大于           |
| <      | 小于           |
| >=     | 大于或等于     |
| <=     | 小于或等于     |
| ?      | 三元运算符     |

>**注释：** [JS 比较](https://www.w3school.com.cn/js/js_comparisons.asp)这一章中完整描述了比较运算符。

## JavaScript 逻辑运算符

| 运算符 | 描述   |
| :----- | :----- |
| &&     | 逻辑与 |
| \|\|   | 逻辑或 |
| !      | 逻辑非 |

>**注释：** [JS 比较](https://www.w3school.com.cn/js/js_comparisons.asp)这一章中完整描述了逻辑运算符。

## JavaScript 类型运算符

| 运算符     | 描述                                  |
| :--------- | :------------------------------------ |
| typeof     | 返回变量的类型。                      |
| instanceof | 返回 true，如果对象是对象类型的实例。 |

>**注释：** [JS 类型转换](https://www.w3school.com.cn/js/js_type_conversion.asp)这一章完整描述了类型运算符。

## JavaScript 位运算符

位运算符处理 32 位数。

该运算中的任何数值运算数都会被转换为 32 位的数。结果会被转换回 JavaScript 数。

| 运算符 | 描述         | 例子    | 等同于       | 结果 | 十进制 |
| :----- | :----------- | :------ | :----------- | :--- | :----- |
| &      | 与           | 5 & 1   | 0101 & 0001  | 0001 | 1      |
| \|     | 或           | 5 \| 1  | 0101 \| 0001 | 0101 | 5      |
| ~      | 非           | ~ 5     | ~0101        | 1010 | 10     |
| ^      | 异或         | 5 ^ 1   | 0101 ^ 0001  | 0100 | 4      |
| <<     | 零填充左位移 | 5 << 1  | 0101 << 1    | 1010 | 10     |
| >>     | 有符号右位移 | 5 >> 1  | 0101 >> 1    | 0010 | 2      |
| >>>    | 零填充右位移 | 5 >>> 1 | 0101 >>> 1   | 0010 | 2      |

上例使用 4 位无符号的例子。但是 JavaScript 使用 32 位有符号数。

因此，在 JavaScript 中，~ 5 不会返回 10，而是返回 -6。

~00000000000000000000000000000101 将返回 11111111111111111111111111111010。

>**注释：** 我们将在 [JS 位运算](https://www.w3school.com.cn/js/js_bitwise.asp)这一章为您详解位运算符。

## 课外书

如需更多有关 *JavaScript 运算符*的知识，请阅读 JavaScript 高级教程中的相关内容：

- [ECMAScript 一元运算符](https://www.w3school.com.cn/js/pro_js_operators_unary.asp)

  一元运算符只有一个参数，即要操作的对象或值。本节讲解 ECMAScript 中最简单的运算符 - 一元运算符。

- [ECMAScript 位运算符](https://www.w3school.com.cn/js/pro_js_operators_bitwise.asp)

  位运算符是在数字底层进行操作的。本节深入讲解了有关整数的知识，并介绍了 ECMAScript 的各种位运算符。

- [ECMAScript Boolean 运算符](https://www.w3school.com.cn/js/pro_js_operators_boolean.asp)

  Boolean 运算符非常重要。本节深入讲解三种 Boolean 运算符：NOT、AND 和 OR。

- [ECMAScript 乘性运算符](https://www.w3school.com.cn/js/pro_js_operators_multiplicative.asp)

  本节讲解 ECMAScript 的乘性运算符：乘法、除法、取模运算符，以及它们的特殊行为。

- [ECMAScript 加性运算符](https://www.w3school.com.cn/js/pro_js_operators_additive.asp)

  本节讲解 ECMAScript 的加性运算符：加法、减法运算符，以及它们的特殊行为。

- [ECMAScript 关系运算符](https://www.w3school.com.cn/js/pro_js_operators_relational.asp)

  关系运算符执行的是比较运算。本节讲解关系运算符的常规比较方式，以及如何比较字符串与数字。

- [ECMAScript 等性运算符](https://www.w3school.com.cn/js/pro_js_operators_equality.asp)

  等性运算符用于判断变量是否相等。ECMAScript 提供两套等性运算符：等号和非等号，以及全等号和非全等号。

- [ECMAScript 条件运算符](https://www.w3school.com.cn/js/pro_js_operators_conditional.asp)

  本节讲解 ECMAScript 中的条件运算符。

- [ECMAScript 赋值运算符](https://www.w3school.com.cn/js/pro_js_operators_assignment.asp)

  本节讲解 ECMAScript 中的赋值运算符。

- [ECMAScript 逗号运算符符](https://www.w3school.com.cn/js/pro_js_operators_comma.asp)

  本节讲解 ECMAScript 中的逗号运算符。
