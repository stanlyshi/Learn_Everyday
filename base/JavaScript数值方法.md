**Number 方法帮助您处理数值。**

## Number 方法和属性

原始值（比如 3.14 或 2016），无法拥有属性和方法（因为它们不是对象）。

但是通过 JavaScript，方法和属性也可用于原始值，因为 JavaScript 在执行方法和属性时将原始值视作对象。

## toString() 方法

`toString()` 以字符串返回数值。

所有数字方法可用于任意类型的数字（字面量、变量或表达式）：

### 实例

```javascript
var x = 123;
x.toString();            // 从变量 x 返回 123
(123).toString();        // 从文本 123 返回 123
(100 + 23).toString();   // 从表达式 100 + 23 返回 123
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_tostring)

## toExponential() 方法

`toExponential()` 返回字符串值，它包含已被四舍五入并使用指数计数法的数字。

参数定义小数点后的字符数：

### 实例

```javascript
var x = 9.656;
x.toExponential(2);     // 返回 9.66e+0
x.toExponential(4);     // 返回 9.6560e+0
x.toExponential(6);     // 返回 9.656000e+0
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_toexponential)

该参数是可选的。如果您没有设置它，JavaScript 不会对数字进行舍入。

## toFixed() 方法

`toFixed()` 返回字符串值，它包含了指定位数小数的数字：

### 实例

```javascript
var x = 9.656;
x.toFixed(0);           // 返回 10
x.toFixed(2);           // 返回 9.66
x.toFixed(4);           // 返回 9.6560
x.toFixed(6);           // 返回 9.656000
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_tofixed)

`toFixed(2)` 非常适合处理金钱。

## toPrecision() 方法

`toPrecision()` 返回字符串值，它包含了指定长度的数字：

### 实例

```javascript
var x = 9.656;
x.toPrecision();        // 返回 9.656
x.toPrecision(2);       // 返回 9.7
x.toPrecision(4);       // 返回 9.656
x.toPrecision(6);       // 返回 9.65600
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_toprecision)

## valueOf() 方法

`valueOf()` 以数值返回数值：

### 实例

```javascript
var x = 123;
x.valueOf();            // 从变量 x 返回 123
(123).valueOf();        // 从文本 123 返回 123
(100 + 23).valueOf();   // 从表达式 100 + 23 返回 123
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_valueof)

在 JavaScript 中，数字可以是原始值（typeof = number）或对象（typeof = object）。

在 JavaScript 内部使用 `valueOf()` 方法可将 Number 对象转换为原始值。

没有理由在代码中使用它。

所有 JavaScript 数据类型都有 `valueOf()` 和 `toString()` 方法。

## 把变量转换为数值

这三种 JavaScript 方法可用于将变量转换为数字：

- `Number() 方法`
- `parseInt() 方法`
- `parseFloat() 方法`

这些方法并非数字方法，而是`全局 JavaScript 方法`。

## 全局方法

JavaScript 全局方法可用于所有 JavaScript 数据类型。

这些是在处理数字时最相关的方法：

| 方法         | 描述                         |
| :----------- | :--------------------------- |
| Number()     | 返回数字，由其参数转换而来。 |
| parseFloat() | 解析其参数并返回浮点数。     |
| parseInt()   | 解析其参数并返回整数。       |

## Number() 方法

Number() 可用于把 JavaScript 变量转换为数值：

### 实例

```javascript
x = true;
Number(x);        // 返回 1
x = false;     
Number(x);        // 返回 0
x = new Date();
Number(x);        // 返回 1404568027739
x = "10"
Number(x);        // 返回 10
x = "10 20"
Number(x);        // 返回 NaN
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_global_number)

如果无法转换数字，则返回 NaN。

## 用于日期的 Number() 方法

`Number()` 还可以把日期转换为数字：

### 实例

```javascript
Number(new Date("2019-04-15"));    // 返回 1506729600000
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_global_number_date)

上面的 `Number()` 方法返回 1970 年 1 月 1 日至今的毫秒数。

## parseInt() 方法

`parseInt()` 解析一段字符串并返回数值。允许空格。只返回首个数字：

### 实例

```javascript
parseInt("10");         // 返回 10
parseInt("10.33");      // 返回 10
parseInt("10 20 30");   // 返回 10
parseInt("10 years");   // 返回 10
parseInt("years 10");   // 返回 NaN
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_global_parseint)

如果无法转换为数值，则返回 `NaN` (Not a Number)。

## parseFloat() 方法

`parseFloat()` 解析一段字符串并返回数值。允许空格。只返回首个数字：

### 实例

```javascript
parseFloat("10");        // 返回 10
parseFloat("10.33");     // 返回 10.33
parseFloat("10 20 30");  // 返回 10
parseFloat("10 years");  // 返回 10
parseFloat("years 10");  // 返回 NaN
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_global_parsefloat)

如果无法转换为数值，则返回 `NaN` (Not a Number)。

## 数值属性

| 属性              | 描述                             |
| :---------------- | :------------------------------- |
| MAX_VALUE         | 返回 JavaScript 中可能的最大数。 |
| MIN_VALUE         | 返回 JavaScript 中可能的最小数。 |
| NEGATIVE_INFINITY | 表示负的无穷大（溢出返回）。     |
| NaN               | 表示非数字值（"Not-a-Number"）。 |
| POSITIVE_INFINITY | 表示无穷大（溢出返回）。         |

## JavaScript MIN_VALUE 和 MAX_VALUE

`MAX_VALUE` 返回 JavaScript 中可能的最大数字。

### 实例

```javascript
var x = Number.MAX_VALUE;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_max_value)

`MIN_VALUE` 返回 JavaScript 中可能的最小数字。

### 实例

```javascript
var x = Number.MIN_VALUE;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_min_value)

## JavaScript POSITIVE_INFINITY

### 实例

```javascript
var x = Number.POSITIVE_INFINITY;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_positive_infinity_1)

溢出时返回 `POSITIVE_INFINITY`：

### 实例

```javascript
var x = 1 / 0;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_positive_infinity_2)

## JavaScript NEGATIVE_INFINITY

### 实例

```javascript
var x = Number.NEGATIVE_INFINITY;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_negative_infinity_1)

溢出时返回 `NEGATIVE_INFINITY`：

### 实例

```javascript
var x = -1 / 0;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_negative_infinity_2)

## JavaScript NaN - 非数字

### 实例

```javascript
var x = Number.NaN;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_nan)

`NaN` 属于 JavaScript 保留字，指示数字并非合法的数字。

尝试使用非数字字符串进行算术运算将导致 `NaN`（非数字）：

### 实例

```javascript
var x = 100 / "Apple";  // x 将是 NaN (Not a Number)
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_nan_divide_string)

## 数字属性不可用于变量

数字属性属于名为 `number` 的 JavaScript 数字对象包装器。

这些属性只能作为 `Number.MAX_VALUE` 访问。

使用 myNumber.MAX_VALUE，其中 myNumber 是变量、表达式或值，将返回 `undefined`：

### 实例

```javascript
var x = 6;
var y = x.MAX_VALUE;    // y 成为 undefined
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_number_max_undefined)

## 完整的 JavaScript Number 参考手册

如需完整的参考手册，请访问我们完整的 [JavaScript Number 参考手册](https://www.w3school.com.cn/jsref/jsref_obj_number.asp)。

参考手册包含所有 Number 对象属性和方法的描述和实例。
