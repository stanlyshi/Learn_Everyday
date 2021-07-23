**JavaScript 只有`一种`数值类型。**

**书写数值时`带不带小数点均可`。**

## JavaScript 数值

书写 JavaScript 数值既可以带小数点，也可以不带：

### 实例

```javascript
var x = 3.14;    // 带小数点的数值
var y = 3;       // 不带小数点的数值
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_1)

超大或超小的数可通过科学计数法来写：

### 实例

```javascript
var x = 123e5;    // 12300000
var y = 123e-5;   // 0.00123
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_2)

## JavaScript 数值始终是 64 位的浮点数

与许多其他编程语言不同，JavaScript 不会定义不同类型的数，比如整数、短的、长的、浮点的等等。

JavaScript 数值始终以双精度浮点数来存储，根据国际 IEEE 754 标准。

此格式用 64 位存储数值，其中 0 到 51 存储数字（片段），52 到 62 存储指数，63 位存储符号：

| 值(aka Fraction/Mantissa) | 指数              | 符号       |
| :------------------------ | :---------------- | :--------- |
| 52 bits(0 - 51)           | 11 bits (52 - 62) | 1 bit (63) |

## 精度

整数（不使用指数或科学计数法）会被精确到 15 位。

### 实例

```javascript
var x = 999999999999999;   // x 将是 999999999999999
var y = 9999999999999999;  // y 将是 10000000000000000
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_inaccurate_1)

小数的最大数是 17 位，但是浮点的算数并不总是 100% 精准：

### 实例

```javascript
var x = 0.2 + 0.1;         // x 将是 0.30000000000000004
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_inaccurate_2)

使用乘除法有助于解决上面的问题：

### 实例

```javascript
var x = (0.2 * 10 + 0.1 * 10) / 10;       // x 将是 0.3
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_inaccurate_3)

## 数字和字符串相加

### 警告！！

JavaScript 的`加法和级联（concatenation）都使用 + 运算符`。

`数字用加法`。`字符串用级联`。

如果您对两个数相加，结果将是一个数：

### 实例

```javascript
var x = 10;
var y = 20;
var z = x + y;           // z 将是 30（一个数）
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_add)

如果对两个字符串相加，结果将是一个字符串的级联：

### 实例

```javascript
var x = "10";
var y = "20";
var z = x + y;           // z 将是 1020（字符串）
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_add_strings_1)

如果您对一个数和一个字符串相加，结果也是字符串级联：

### 实例

```javascript
var x = 10;
var y = "20";
var z = x + y;           // z 将是 1020（一个字符串）
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_add_strings_2)

如果您对一个字符串和一个数字相加，结果也是字符串级联：

### 实例

```javascript
var x = "10";
var y = 20;
var z = x + y;           // z 将是 1020（字符串）
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_add_strings_3)

常见的错误是认为结果应该是 30：

### 实例

```javascript
var x = 10;
var y = 20;
var z = "The result is: " + x + y;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_add_strings_4)

常见的错误是认为结果应该是 102030：

### 实例

```javascript
var x = 10;
var y = 20;
var z = "30";
var result = x + y + z;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_add_strings_5)

JavaScript 从左向右进行编译。

因为 x 和 y 都是数，10 + 20 将被相加。

因为 z 是字符串，30 + "30" 被级联。

## 数字字符串

JavaScript 字符串可以拥有数字内容：

```javascript
var x = 100;         // x 是数字

var y = "100";       // y 是字符串
```

在所有数字运算中，JavaScript 会尝试将字符串转换为数字：

该例如此运行：

```javascript
var x = "100";
var y = "10";
var z = x / y;       // z 将是 10
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_string_1)

该例也将如此运行：

```javascript
var x = "100";
var y = "10";
var z = x * y;       // z 将是 1000
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_string_2)

该例如此运行：

```javascript
var x = "100";
var y = "10";
var z = x - y;       // z 将是 90
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_string_3)

但是该例不会如上例般运行：

```javascript
var x = "100";
var y = "10";
var z = x + y;       // z 不会是 110（而是 10010）
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_string_4)

在最后一个例子中，JavaScript 用 + 运算符对字符串进行了级联。

## NaN - 非数值

NaN 属于 JavaScript 保留词，指示某个数不是合法数。

尝试用一个非数字字符串进行除法会得到 NaN（Not a Number）：

### 实例

```javascript
var x = 100 / "Apple";  // x 将是 NaN（Not a Number）
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_divide_string)

不过，假如字符串包含数值，则结果将是数：

### 实例

```javascript
var x = 100 / "10";     // x 将是 10
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_divide_number)

您可使用全局 JavaScript 函数 isNaN() 来确定某个值是否是数：

### 实例

```javascript
var x = 100 / "Apple";
isNaN(x);               // 返回 true，因为 x 不是数
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_isnan_true)

要小心 NaN。假如您在数学运算中使用了 NaN，则结果也将是 NaN：

### 实例

```javascript
var x = NaN;
var y = 5;
var z = x + y;         // z 将是 NaN
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_nan_math)

结果也许是串连接：

### 实例

```javascript
var x = NaN;
var y = "5";
var z = x + y;         // z 将是 NaN5
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_nan_concat)

`NaN 是数`，`typeof NaN 返回 number`：

### 实例

```javascript
typeof NaN;             // 返回 "number"
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_nan_typeof)

## Infinity

Infinity （或 -Infinity）是 JavaScript 在计算数时超出最大可能数范围时返回的值。

### 实例

```javascript
var myNumber = 2;

while (myNumber != Infinity) {          // 执行直到 Infinity
    myNumber = myNumber * myNumber;
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_infinity)

除以 0（零）也会生成 Infinity：

### 实例

```javascript
var x =  2 / 0;          // x 将是 Infinity
var y = -2 / 0;          // y 将是 -Infinity
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_infinity_zero)

Infinity 是数：typeOf Infinity 返回 number。

### 实例

```javascript
typeof Infinity;        // 返回 "number"
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_infinity_typeof)

## 十六进制

JavaScript 会把前缀为 `0x` 的数值常量解释为十六进制。

### 实例

```javascript
var x = 0xFF;             // x 将是 255
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_hex)

绝不要用前导零写数字（比如 07）。

一些 JavaScript 版本会把带有前导零的数解释为八进制。

默认地，Javascript 把数显示为十进制小数。

但是您能够使用 `toString()` 方法把数输出为十六进制、八进制或二进制。

### 实例

```javascript
var myNumber = 128;
myNumber.toString(16);     // 返回 80
myNumber.toString(8);      // 返回 200
myNumber.toString(2);      // 返回 10000000
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_tostring)

## 数值可以是对象

通常 JavaScript 数值是通过字面量创建的原始值：var x = 123

但是也可以通过关键词 new 定义为对象：var y = new Number(123)

### 实例

```javascript
var x = 123;
var y = new Number(123);

// typeof x 返回 number
// typeof y 返回 object
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_object_1)

请不要创建数值对象。这样会拖慢执行速度。

new 关键词使代码复杂化，并产生某些无法预料的结果：

当使用 == 相等运算符时，相等的数看上去相等：

### 实例

```javascript
var x = 500;             
var y = new Number(500);

// (x == y) 为 true，因为 x 和 y 有相等的值
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_object_2)

当使用 === 相等运算符后，相等的数变为不相等，因为 === 运算符需要类型和值同时相等。

### 实例

```javascript
var x = 500;             
var y = new Number(500);

// (x === y) 为 false，因为 x 和 y 的类型不同
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_object_3)

甚至更糟。对象无法进行对比：

### 实例

```javascript
var x = new Number(500);             
var y = new Number(500);

// (x == y) 为 false，因为对象无法比较
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_object_4)

JavaScript 对象无法进行比较。

