**字符串值，数值，布尔值，数组，对象。**

## JavaScript 数据类型

JavaScript 变量能够保存多种*数据类型*：`数值、字符串值、数组、对象`等等：

```javascript
var length = 7;                             // 数字
var lastName = "Gates";                      // 字符串
var cars = ["Porsche", "Volvo", "BMW"];         // 数组
var x = {firstName:"Bill", lastName:"Gates"};    // 对象 
```

## 数据类型的概念

在编程过程中，数据类型是重要的概念。

为了能够操作变量，了解数据类型是很重要的。

如果没有数据类型，计算机就无法安全地解决这道题：

```javascript
var x = 911 + "Porsche";
```

给 "Volvo" 加上 911 有意义吗？这么做会发生错误还是会产生一个结果？

JavaScript 会这样处理上面的例子：

```javascript
var x = "911" + "Porsche";
```

当数值和字符串相加时，JavaScript 将把数值视作字符串。

### 实例

```javascript
var x = 911 + "Porsche";
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_datatypes_addstrings_1)

### 实例

```javascript
var x = "Porsche" + 911;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_datatypes_addstrings_2)

`JavaScript 从左向右计算表达式`。不同的次序会产生不同的结果：

### JavaScript：

```
var x = 911 + 7 + "Porsche";
```

结果：

```
918Porsche
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_datatypes_addstrings_3)

### JavaScript：

```javascript
var x = "Porsche" + 911 + 7;
```

结果：

```j
Porsche9117
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_datatypes_addstrings_4)

在第一个例子中，`JavaScript 把 911 和 7 视作数值，直到遇见 "Porsche"`。

在第二个例子中，`由于第一个操作数是字符串，因此所有操作数都被视为字符串`。

## JavaScript 拥有动态类型

`JavaScript 拥有动态类型`。这意味着相同变量可用作不同类型：

### 实例

```javascript
var x;               // 现在 x 是 undefined
var x = 7;           // 现在 x 是数值
var x = "Bill";      // 现在 x 是字符串值
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_datatypes_dynamic)

## JavaScript 字符串值

字符串（或文本字符串）是一串字符（比如 "Bill Gates"）。

字符串被引号包围。您可使用单引号或双引号：

### 实例

```javascript
var carName = "Porsche 911";   // 使用双引号
var carName = 'Porsche 911';   // 使用单引号
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_datatypes_string_quotes)

您可以在字符串内使用引号，只要这些引号与包围字符串的引号不匹配：

### 实例

```javascript
var answer = "It's alright";             // 双引号内的单引号
var answer = "He is called 'Bill'";    // 双引号内的单引号
var answer = 'He is called "Bill"';    // 单引号内的双引号
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_datatypes_string)

您将在本教程中学到更多有关字符串的知识。

## JavaScript 数值

JavaScript 只有一种数值类型。

写数值时用不用小数点均可：

### 实例

```javascript
var x1 = 34.00;     // 带小数点
var x2 = 34;        // 不带小数点
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_datatypes_number)

超大或超小的数值可以用科学计数法来写：

### 实例

```javascript
var y = 123e5;      // 12300000
var z = 123e-5;     // 0.00123
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_datatypes_number_large)

您将在本教程中学到更多有关数值的知识。

## JavaScript 布尔值

布尔值只有两个值：true 或 false。

### 实例

```javascript
var x = true;
var y = false;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_datatypes_bolean)

布尔值经常用在条件测试中。

您将在本教程中学到更多有关条件测试的知识。

## JavaScript 数组

JavaScript 数组用方括号书写。

数组的项目由逗号分隔。

下面的代码声明（创建）了名为 cars 的数组，包含三个项目（汽车品牌）：

### 实例

```javascript
var cars = ["Porsche", "Volvo", "BMW"];
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_datatypes_array)

数组索引基于零，这意味着第一个项目是 [0]，第二个项目是 [1]，以此类推。

您将在本教程中学到更多有关数组的知识。

## JavaScript 对象

JavaScript 对象用花括号来书写。

对象属性是 `name:value` 对，由逗号分隔。

### 实例

```javascript
var person = {firstName:"Bill", lastName:"Gates", age:62, eyeColor:"blue"};
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_datatypes_object)

上例中的对象（person）有四个属性：`firstName、lastName、age` 以及 `eyeColor`。

您将在本教程中学到更多有关对象的知识。

## typeof 运算符

您可使用 JavaScript 的 `typeof` 来确定 JavaScript 变量的类型：

`typeof` 运算符返回变量或表达式的类型：

### 实例

```javascript
typeof ""                  // 返回 "string"
typeof "Bill"              // 返回 "string"
typeof "Bill Gates"          // 返回 "string"
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_datatypes_typeof_string)

### 实例

```javascript
typeof 0                   // 返回 "number"
typeof 314                 // 返回 "number"
typeof 3.14                // 返回 "number"
typeof (7)                 // 返回 "number"
typeof (7 + 8)             // 返回 "number"
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_datatypes_typeof_number)

`typeof` 运算符对数组返回 `"object"`，因为在 JavaScript 中数组属于对象。

## Undefined

在 JavaScript 中，没有值的变量，其值是 `undefined`。`typeof` 也返回 `undefined`。

### 实例

```javascript
var person;                  // 值是 undefined，类型是 undefined
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_datatypes_undefined_1)

`任何变量均可通过设置值为 undefined 进行清空`。`其类型也将是 undefined`。

### 实例

```javascript
person = undefined;          // 值是 undefined，类型是 undefined
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_datatypes_undefined_2)

## 空值

`空值与 undefined 不是一回事`。

`空的字符串变量既有值也有类型`。

### 实例

```javascript
var car = "";                // 值是 ""，类型是 "string"
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_datatypes_empty)

## Null

在 JavaScript 中，`null` 是 `"nothing"`。它被看做不存在的事物。

不幸的是，在 JavaScript 中，null 的数据类型是对象。

您可以把 null 在 JavaScript 中是对象理解为一个 bug。它本应是 null。

您可以通过设置值为 null 清空对象：

### 实例

```javascript
var person = null;           // 值是 null，但是类型仍然是对象
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_datatypes_null)

您也可以通过设置值为 undefined 清空对象：

### 实例

```javascript
var person = undefined;     // 值是 undefined，类型是 undefined
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_datatypes_undefined_3)

## Undefined 与 Null 的区别

Undefined 与 null 的值相等，但类型不相等：

```javascript
typeof undefined              // undefined
typeof null                   // object
null === undefined            // false
null == undefined             // true
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_datatypes_undefined_4)

## 原始数据

原始数据值是一种没有额外属性和方法的单一简单数据值。

typeof 运算符可返回以下原始类型之一：

- `string`
- `number`
- `boolean`
- `undefined`

### 实例

```javascript
typeof "Bill"              // 返回 "string"
typeof 3.14                // 返回 "number"
typeof true                // 返回 "boolean"
typeof false               // 返回 "boolean"
typeof x                   // 返回 "undefined" (假如 x 没有值)
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_datatypes_typeof_primitive)

## 复杂数据

typeof 运算符可返回以下两种类型之一：

- `function`
- `object`

`typeof 运算符把对象、数组或 null 返回 object`。

`typeof 运算符不会把函数返回 object`。

### 实例

```javascript
typeof {name:'Bill', age:62} // 返回 "object"
typeof [1,2,3,4]             // 返回 "object" (并非 "array"，参见下面的注释)
typeof null                  // 返回 "object"
typeof function myFunc(){}   // 返回 "function"
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_datatypes_typeof_complexdata)

typeof 运算符把数组返回为 "object"，因为在 JavaScript 中数组即对象。

## 课外阅读

JavaScript 高级教程：

- [ECMAScript 原始类型](https://www.w3school.com.cn/js/pro_js_primitivetypes.asp)
- [ECMAScript 类型转换](https://www.w3school.com.cn/js/pro_js_typeconversion.asp)
- [ECMAScript 引用类型](https://www.w3school.com.cn/js/pro_js_referencetypes.asp)
