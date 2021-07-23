**处理数值的典型情景是算数。**

## JavaScript 算数运算符

算术运算符对数值（文字或变量）执行算术运算。

| 运算符 | 描述                                                      |
| :----- | :-------------------------------------------------------- |
| +      | 加法                                                      |
| -      | 减法                                                      |
| *      | 乘法                                                      |
| **     | 幂（[ES2016](https://www.w3school.com.cn/js/js_es6.asp)） |
| /      | 除法                                                      |
| %      | 系数                                                      |
| ++     | 递增                                                      |
| --     | 递减                                                      |

## 算数运算符

典型的算术运算会操作两个数值。

这两个数可以是字面量：

### 实例

```javascript
var x = 7 + 8;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_arithmetic_operation)

或变量：

### 实例

```javascript
var x = a + b;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_arithmetic_variables)

或表达式：

### 实例

```javascript
var x = (7 + 8) * a;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_arithmetic_expressions)

## 运算符和操作数

在算术运算中，数被称为*操作数*。

（两个操作数之间执行的）运算由*运算符*定义。

| 操作数 | 运算符 | 操作数 |
| :----- | :----- | :----- |
| 7      | +      | 8      |

## 加法

*加法*运算符（+）加数：

```javascript
var x = 7;
var y = 8;
var z = x + y; 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_operator_addition)

## 减法

*减法*运算符（-）减数。

```javascript
var x = 7;
var y = 8;
var z = x - y; 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_operator_subtraction)

## 乘法

*乘法*运算符（*）乘数。

```javascript
var x = 7;
var y = 8;
var z = x * y; 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_operator_multiplication)

## 除法

*除法*运算符（/）除数。

```javascript
var x = 7;
var y = 2;
var z = x / y; 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_operator_division)

## 系数

*系数*运算符（%）返回除法的余数。

```javascript
var x = 7;
var y = 2;
var z = x % y; 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_operator_modulus)

> **注释：** 在算术中，两个整数的除法产生商和余数。

> **注释：** 在数学中，模运算的结果是算术除法的余数。

## 递增

*递增*运算符（++）对数值进行递增。

```javascript
var x = 7;
x++;
var z = x;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_operator_increment)

## 递减

*递减*运算符（--）对数值进行递减。

```javascript
var x = 7;
x--;
var z = x; 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_operator_decrement)

## 幂

取幂运算符（**）将第一个操作数提升到第二个操作数的幂。

### 实例

```javascript
var x = 5;
var z = x ** 2;          // 结果是 25
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_arithmetric_exponent)

x ** y 产生的结果与 Math.pow(x,y) 相同:

### 实例

```javascript
var x = 5;
var z = Math.pow(x,2);   // 结果是 25
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_arithmetric_exponent_math_pow)

## 运算符优先级

运算符优先级（Operator precedence）描述了在算术表达式中所执行操作的顺序。

### 实例

```javascript
var x = 200 + 50 * 2;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_arithmetic_precedence1)

上例的结果是 250 * 2 还是 200 + 100 呢？

是加法还是乘法优先呢？

在传统的学校数学中，乘法是优先的。

乘法（*）和除法（%）比加法（+）和减法（-）拥有更高的*优先级*。

同时，（就像在学校的数学中那样）能够通过使用括号来改变优先级：

### 实例

```javascript
var x = (200 + 50) * 2;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_arithmetic_precedence2)

当使用括号时，括号中的运算符会首先被计算。

当多个运算拥有相同的优先级时（比如加法和减法），对它们的计算是从左向右的：

### 实例

```javascript
var x = 200 + 50 - 2;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_arithmetic_precedence3)

## JavaScript 运算符优先级值

| 值   | 运算符     | 描述             | 实例             |
| :--- | :--------- | :--------------- | :--------------- |
| 20   | ( )        | 表达式分组       | (3 + 4)          |
|      |            |                  |                  |
| 19   | .          | 成员             | person.name      |
| 19   | []         | 成员             | person["name"]   |
| 19   | ()         | 函数调用         | myFunction()     |
| 19   | new        | 创建             | new Date()       |
|      |            |                  |                  |
| 17   | ++         | 后缀递增         | i++              |
| 17   | --         | 后缀递减         | i--              |
|      |            |                  |                  |
| 16   | ++         | 前缀递增         | ++i              |
| 16   | --         | 前缀递减         | --i              |
| 16   | !          | 逻辑否           | !(x==y)          |
| 16   | typeof     | 类型             | typeof x         |
|      |            |                  |                  |
| 15   | **         | 求幂 (ES7)       | 10 ** 2          |
|      |            |                  |                  |
| 14   | *          | 乘               | 10 * 5           |
| 14   | /          | 除               | 10 / 5           |
| 14   | %          | 模数除法         | 10 % 5           |
|      |            |                  |                  |
| 13   | +          | 加               | 10 + 5           |
| 13   | -          | 减               | 10 - 5           |
|      |            |                  |                  |
| 12   | <<         | 左位移           | x << 2           |
| 12   | >>         | 右位移           | x >> 2           |
| 12   | >>>        | 右位移（无符号） | x >>> 2          |
|      |            |                  |                  |
| 11   | <          | 小于             | x < y            |
| 11   | <=         | 小于或等于       | x <= y           |
| 11   | >          | 大于             | x > y            |
| 11   | >=         | 大于或等于       | x >= y           |
| 11   | in         | 对象中的属性     | "PI" in Math     |
| 11   | instanceof | 对象的实例       | instanceof Array |
|      |            |                  |                  |
| 10   | ==         | 相等             | x == y           |
| 10   | ===        | 严格相等         | x === y          |
| 10   | !=         | 不相等           | x != y           |
| 10   | !==        | 严格不相等       | x !== y          |
|      |            |                  |                  |
| 9    | &          | 按位与           | x & y            |
| 8    | ^          | 按位 XOR         | x ^ y            |
| 7    | \|         | 按位或           | x \| y           |
| 6    | &&         | 逻辑与           | x && y           |
| 5    | \|\|       | 逻辑否           | x \|\| y         |
| 4    | ? :        | 条件             | ? "Yes" : "No"   |
|      |            |                  |                  |
| 3    | =          | 赋值             | x = y            |
| 3    | +=         | 赋值             | x += y           |
| 3    | -=         | 赋值             | x -= y           |
| 3    | *=         | 赋值             | x *= y           |
| 3    | %=         | 赋值             | x %= y           |
| 3    | <<=        | 赋值             | x <<= y          |
| 3    | >>=        | 赋值             | x >>= y          |
| 3    | >>>=       | 赋值             | x >>>= y         |
| 3    | &=         | 赋值             | x &= y           |
| 3    | ^=         | 赋值             | x ^= y           |
| 3    | \|=        | 赋值             | x \|= y          |
|      |            |                  |                  |
| 2    | yield      | 暂停函数         | yield x          |
| 1    | ,          | 逗号             | 7 , 8            |

> **注意：** 淡红色指示实验性或建议性的技术（ECMASScript 2016 或 ES7）

> **提示：** 括号中的表达式会在值在表达式的其余部分中被使用之前进行完全计算。
