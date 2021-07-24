**数组迭代方法对每个数组项进行操作。**



## Array.forEach()

`forEach()` 方法为每个数组元素调用一次函数（回调函数）。

### 实例

```javascript
var txt = "";
var numbers = [45, 4, 9, 16, 25];
numbers.forEach(myFunction);

function myFunction(value, index, array) {
  txt = txt + value + "<br>"; 
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_foreach)

**注释：**该函数接受 3 个参数：

- 项目值
- 项目索引
- 数组本身

上面的例子只用了 value 参数。这个例子可以重新写为：

### 实例

```javascript
var txt = "";
var numbers = [45, 4, 9, 16, 25];
numbers.forEach(myFunction);

function myFunction(value) {
  txt = txt + value + "<br>"; 
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_foreach_2)

所有浏览器都支持 `Array.forEach()`，除了 Internet Explorer 8 或更早的版本：

|      |      |      |      |      |
| ---- | ---- | ---- | ---- | ---- |
| Yes  | 9.0  | Yes  | Yes  | Yes  |



## Array.map()

`map()` 方法通过对每个数组元素执行函数来创建新数组。

`map()` 方法不会对没有值的数组元素执行函数。

`map()` 方法不会更改原始数组。

这个例子将每个数组值乘以2：

### 实例

```javascript
var numbers1 = [45, 4, 9, 16, 25];
var numbers2 = numbers1.map(myFunction);

function myFunction(value, index, array) {
  return value * 2;
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_map)

请注意，该函数有 3 个参数：

- 项目值
- 项目索引
- 数组本身

当回调函数仅使用 value 参数时，可以省略索引和数组参数：

### 实例

```javascript
var numbers1 = [45, 4, 9, 16, 25];
var numbers2 = numbers1.map(myFunction);

function myFunction(value) {
  return value * 2;
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_map_2)

所有浏览器都支持 Array.map()，除了 Internet Explorer 8 或更早的版本：

|      |      |      |      |      |
| ---- | ---- | ---- | ---- | ---- |
| Yes  | 9.0  | Yes  | Yes  | Yes  |



## Array.filter()

`filter()` 方法创建一个包含通过测试的数组元素的新数组。

这个例子用值大于 18 的元素创建一个新数组：

### 实例

```javascript
var numbers = [45, 4, 9, 16, 25];
var over18 = numbers.filter(myFunction);

function myFunction(value, index, array) {
  return value > 18;
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_filter)

请注意此函数接受 3 个参数：

- 项目值
- 项目索引
- 数组本身

在上面的例子中，回调函数不使用 index 和 array 参数，因此可以省略它们：

### 实例

```javascript
var numbers = [45, 4, 9, 16, 25];
var over18 = numbers.filter(myFunction);

function myFunction(value) {
  return value > 18;
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_filter_2)

所有浏览器都支持 `Array.filter()`，除了 Internet Explorer 8 或更早的版本：

|      |      |      |      |      |
| ---- | ---- | ---- | ---- | ---- |
| Yes  | 9.0  | Yes  | Yes  | Yes  |



## Array.reduce()

`reduce()` 方法在每个数组元素上运行函数，以生成（减少它）单个值。

`reduce()` 方法在数组中从左到右工作。另请参阅 `reduceRight()`。

`reduce()` 方法不会减少原始数组。

这个例子确定数组中所有数字的总和：

### 实例

```javascript
var numbers1 = [45, 4, 9, 16, 25];
var sum = numbers1.reduce(myFunction);

function myFunction(total, value, index, array) {
  return total + value;
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_reduce)

请注意此函数接受 4 个参数：

- 总数（初始值/先前返回的值）
- 项目值
- 项目索引
- 数组本身

上例并未使用 index 和 array 参数。可以将它改写为：

### 实例

```javascript
var numbers1 = [45, 4, 9, 16, 25];
var sum = numbers1.reduce(myFunction);

function myFunction(total, value) {
  return total + value;
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_reduce_2)

`reduce() 方法能够接受一个初始值`：

### 实例

```javascript
var numbers1 = [45, 4, 9, 16, 25];
var sum = numbers1.reduce(myFunction, 100);

function myFunction(total, value) {
  return total + value;
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_reduce_initial)

所有浏览器都支持 `Array.reduce()`，除了 Internet Explorer 8 或更早的版本：

|      |      |      |      |      |
| ---- | ---- | ---- | ---- | ---- |
| Yes  | 9.0  | Yes  | Yes  | Yes  |



## Array.reduceRight()

`reduceRight()` 方法在每个数组元素上运行函数，以生成（减少它）单个值。

`reduceRight()` 方法在数组中从右到左工作。另请参阅 reduce()。

`reduceRight()` 方法不会减少原始数组。

这个例子确定数组中所有数字的总和：

### 实例

```javascript
var numbers1 = [45, 4, 9, 16, 25];
var sum = numbers1.reduceRight(myFunction);

function myFunction(total, value, index, array) {
  return total + value;
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_reduceright)

请注意此函数接受 4 个参数：

- 总数（初始值/先前返回的值）
- 项目值
- 项目索引
- 数组本身

上例并未使用 index 和 array 参数。可以将它改写为：

### 实例

```javascript
var numbers1 = [45, 4, 9, 16, 25];
var sum = numbers1.reduceRight(myFunction);

function myFunction(total, value) {
  return total + value;
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_reduceright_2)

所有浏览器都支持 `Array.reduceRight()`，除了 Internet Explorer 8 或更早的版本：

|      |      |      |      |      |
| ---- | ---- | ---- | ---- | ---- |
| Yes  | 9.0  | Yes  | Yes  | Yes  |



## Array.every()

`every()` 方法检查所有数组值是否通过测试。

这个例子检查所有数组值是否大于 18：

### 实例

```javascript
var numbers = [45, 4, 9, 16, 25];
var allOver18 = numbers.every(myFunction);

function myFunction(value, index, array) {
  return value > 18;
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_every)

请注意此函数接受 3 个参数：

- 项目值
- 项目索引
- 数组本身

如果回调函数仅使用第一个参数（值）时，可以省略其他参数：

### 实例

```javascript
var numbers = [45, 4, 9, 16, 25];
var allOver18 = numbers.every(myFunction);

function myFunction(value) {
  return value > 18;
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_every_2)

所有浏览器都支持 `Array.every()`，除了 Internet Explorer 8 或更早的版本：

|      |      |      |      |      |
| ---- | ---- | ---- | ---- | ---- |
| Yes  | 9.0  | Yes  | Yes  | Yes  |



## Array.some()

`some()` 方法检查某些数组值是否通过了测试。

这个例子检查某些数组值是否大于 18：

### 实例

```javascript
var numbers = [45, 4, 9, 16, 25];
var someOver18 = numbers.some(myFunction);

function myFunction(value, index, array) {
  return value > 18;
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_some)

请注意此函数接受 3 个参数：

- 项目值
- 项目索引
- 数组本身

所有浏览器都支持 `Array.some()`，除了 Internet Explorer 8 或更早的版本：

|      |      |      |      |      |
| ---- | ---- | ---- | ---- | ---- |
| Yes  | 9.0  | Yes  | Yes  | Yes  |



## Array.indexOf()

`indexOf()` 方法在数组中搜索元素值并返回其位置。

**注释：**第一个项目的位置是 0，第二个项目的位置是 1，以此类推。

### 实例

检索数组中的项目 "Apple"：

```
var fruits = ["Apple", "Orange", "Apple", "Mango"];
var a = fruits.indexOf("Apple");
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_indexof)

所有浏览器都支持 `Array.indexOf()`，除了 Internet Explorer 8 或更早的版本：

|      |      |      |      |      |
| ---- | ---- | ---- | ---- | ---- |
| Yes  | 9.0  | Yes  | Yes  | Yes  |

### 语法

```javascript
array.indexOf(item, start)
```

| *item*  | 必需。要检索的项目。                                         |
| ------- | ------------------------------------------------------------ |
| *start* | 可选。从哪里开始搜索。负值将从结尾开始的给定位置开始，并搜索到结尾。 |

如果未找到项目，Array.indexOf() 返回 -1。

如果项目多次出现，则返回第一次出现的位置。



## Array.lastIndexOf()

`Array.lastIndexOf()` 与 `Array.indexOf()` 类似，但是从数组结尾开始搜索。

### 实例

检索数组中的项目 "Apple"：

```javascript
var fruits = ["Apple", "Orange", "Apple", "Mango"];
var a = fruits.lastIndexOf("Apple");
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_lastindexof)

所有浏览器都支持 `Array.lastIndexOf()`，除了 Internet Explorer 8 或更早的版本：

|      |      |      |      |      |
| ---- | ---- | ---- | ---- | ---- |
| Yes  | 9.0  | Yes  | Yes  | Yes  |

### 语法

```javascript
array.lastIndexOf(item, start)
```

| *item*  | 必需。要检索的项目。                                         |
| ------- | ------------------------------------------------------------ |
| *start* | 可选。从哪里开始搜索。负值将从结尾开始的给定位置开始，并搜索到开头。 |



## Array.find()

`find()` 方法返回通过测试函数的第一个数组元素的值。

这个例子查找（返回）大于 18 的第一个元素的值：

### 实例

```javascript
var numbers = [4, 9, 16, 25, 29];
var first = numbers.find(myFunction);

function myFunction(value, index, array) {
  return value > 18;
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_find)

请注意此函数接受 3 个参数：

- 项目值
- 项目索引
- 数组本身

老旧的浏览器不支持 `Array.find()`。下面列出了完全支持此方法的首个浏览器版本：

|      |      |      |      |      |
| ---- | ---- | ---- | ---- | ---- |
| 45   | 12   | 25   | 8    | 32   |



## Array.findIndex()

`findIndex()` 方法返回通过测试函数的第一个数组元素的索引。

这个例子查找大于 18 的第一个元素的索引：

### 实例

```javascript
var numbers = [4, 9, 16, 25, 29];
var first = numbers.findIndex(myFunction);

function myFunction(value, index, array) {
  return value > 18;
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_find_index)

请注意此函数接受 3 个参数：

- 项目值
- 项目索引
- 数组本身

老旧的浏览器不支持 `Array.findIndex()`。下面列出了完全支持此方法的首个浏览器版本：

|      |      |      |      |      |
| ---- | ---- | ---- | ---- | ---- |
| 45   | 12   | 25   | 8    | 32   |
