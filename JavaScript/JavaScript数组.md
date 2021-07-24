**JavaScript 数组用于在单一变量中存储多个值。**

## 实例

```javascript
var cars = ["Saab", "Volvo", "BMW"];
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array)

## 什么是数组？

数组是一种特殊的变量，它能够一次存放一个以上的值。

如果您有一个项目清单（例如，汽车品牌列表），在单个变量中存储汽车品牌应该是这样的：

```javascript
var car1 = "Saab";
var car2 = "Volvo";
var car3 = "BMW"; 
```

不过，假如您希望遍历所有汽车并找到一个特定的值？假如不是三个汽车品牌而是三百个呢？

解决方法就是数组！

数组可以用一个单一的名称存放很多值，并且还可以通过引用索引号来访问这些值。

## 创建数组

使用数组文本是创建 JavaScript 数组最简单的方法。

### 语法：

```javascript
var array-name = [item1, item2, ...];
```

### 实例

```javascript
var cars = ["Saab", "Volvo", "BMW"];
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array)

空格和折行并不重要。声明可横跨多行：

### 实例

```javascript
var cars = [
    "Saab",
    "Volvo",
    "BMW"
];
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_newlines)

请不要最后一个元素之后写逗号（比如 "BMW",）。

可能存在跨浏览器兼容性问题。

## 使用 JavaScript 关键词 new

下面的例子也会创建数组，并为其赋值：

### 实例

```javascript
var cars = new Array("Saab", "Volvo", "BMW");
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_new)

以上两个例子效果完全一样。无需使用 new Array()。

出于简洁、可读性和执行速度的考虑，请使用第一种方法（数组文本方法）。

## 访问数组元素

我们通过引用*索引号（下标号）*来引用某个数组元素。

这条语句访问 cars 中的首个元素的值：

```javascript
var name = cars[0];
```

这条语句修改 cars 中的首个元素：

```javascript
cars[0] = "Opel";
```

### 实例

```javascript
var cars = ["Saab", "Volvo", "BMW"];
document.getElementById("demo").innerHTML = cars[0]; 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_element)

[0] 是数组中的第一个元素。[1] 是第二个。数组索引从 0 开始。

## 改变数组元素

这条语句修改了 cars 中第一个元素的值：

```javascript
cars[0] = "Opel";
```

### 实例

```javascript
var cars = ["Saab", "Volvo", "BMW"];
cars[0] = "Opel";
document.getElementById("demo").innerHTML = cars[0];
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_element_change)

## 访问完整数组

通过 JavaScript，可通过引用数组名来访问完整数组：

### 实例

```javascript
var cars = ["Saab", "Volvo", "BMW"];
document.getElementById("demo").innerHTML = cars; 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_full)

## 数组是对象

数组是一种特殊类型的对象。在 JavaScript 中对数组使用 typeof 运算符会返回 "object"。

但是，JavaScript 数组最好以数组来描述。

数组使用*数字*来访问其“元素”。在本例中，person[0] 返回 Bill：

### 数组：

```javascript
var person = ["Bill", "Gates", 62];
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_array)

对象使用*名称*来访问其“成员”。在本例中，person.firstName 返回 Bill：

### 对象：

```javascript
var person = {firstName:"John", lastName:"Doe", age:46};
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_object)

## 数组元素可以是对象

JavaScript 变量可以是对象。数组是特殊类型的对象。

正因如此，您可以在相同数组中存放不同类型的变量。

您可以在数组保存对象。您可以在数组中保存函数。你甚至可以在数组中保存数组：

```javascript
myArray[0] = Date.now;
myArray[1] = myFunction;
myArray[2] = myCars;
```

## 数组属性和方法

JavaScript 数组的真实力量隐藏在数组的属性和方法中：

### 实例

```javascript
var x = cars.length;   // length 属性返回元素的数量
var y = cars.sort();   // sort() 方法对数组进行排序
```

我们将在下一章学习数组方法。

## length 属性

length 属性返回数组的长度（数组元素的数目）。

### 实例

```javascript
var fruits = ["Banana", "Orange", "Apple", "Mango"];
fruits.length;                       // fruits 的长度是 4
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_length)

length 属性始终大于最高数组索引（下标）。

## 访问第一个数组元素

### 实例

```javascript
fruits = ["Banana", "Orange", "Apple", "Mango"];
var first = fruits[0];
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_first)

## 访问最后一个数组元素

### 实例

```javascript
fruits = ["Banana", "Orange", "Apple", "Mango"];
var last = fruits[fruits.length - 1];
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_last)

## 遍历数组元素

遍历数组的最安全方法是使用 "for" 循环：

### 实例

```javascript
var fruits, text, fLen, i;

fruits = ["Banana", "Orange", "Apple", "Mango"];
fLen = fruits.length;
text = "<ul>";
for (i = 0; i < fLen; i++) {
     text += "<li>" + fruits[i] + "</li>";
} 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_loop_for)

您也可以使用 `Array.foreach()` 函数：

### 实例

```javascript
var fruits, text;
fruits = ["Banana", "Orange", "Apple", "Mango"];

text = "<ul>";
fruits.forEach(myFunction);
text += "</ul>";

function myFunction(value) {
  text += "<li>" + value + "</li>";
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_loop_foreach)

## 添加数组元素

向数组添加新元素的最佳方法是使用 push() 方法：

### 实例

```javascript
var fruits = ["Banana", "Orange", "Apple", "Mango"];
fruits.push("Lemon");                // 向 fruits 添加一个新元素 (Lemon)
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_add_push)

也可以使用 length 属性向数组添加新元素：

### 实例

```javascript
var fruits = ["Banana", "Orange", "Apple", "Mango"];
fruits[fruits.length] = "Lemon";     // 向 fruits 添加一个新元素 (Lemon)
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_add_length)

### 警告！

添加最高索引的元素可在数组中创建未定义的“洞”：

### 实例

```javascript
var fruits = ["Banana", "Orange", "Apple", "Mango"];
fruits[6] = "Lemon";                 // 向 fruits 添加一个新元素 (Lemon)
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_holes)

## 关联数组

很多编程元素支持命名索引的数组。

具有命名索引的数组被称为关联数组（或散列）。

JavaScript *不支持*命名索引的数组。

在 JavaScript 中，数组只能使用*数字索引*。

### 实例

```javascript
var person = [];
person[0] = "Bill";
person[1] = "Gates";
person[2] = 62;
var x = person.length;          // person.length 返回 3
var y = person[0];              // person[0] 返回 "Bill"
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_associative_1)

 警告！

假如您使用命名索引，JavaScript 会把数组重定义为标准对象。

之后，所有数组的方法和属性将产生非正确结果。

### 实例：

```javascript
var person = [];
person["firstName"] = "Bill";
person["lastName"] = "Gates";
person["age"] = 62;
var x = person.length;         // person.length 将返回 0
var y = person[0];              // person[0] 将返回 undefined
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_associative_2)

## 数组和对象的区别

在 JavaScript 中，`数组使用数字索引`。

在 JavaScript 中，`对象使用命名索引`。

`数组是特殊类型的对象，具有数字索引`。

## 何时使用数组，何时使用对象？

- JavaScript 不支持关联数组
- `如果希望元素名为字符串（文本）则应该使用对象`。
- `如果希望元素名为数字则应该使用数组`。

## 避免 new Array()

没有必要使用 JavaScript 的内建数组构造器 `new Array()`。

*请使用 `[]` 取而代之！*

下面两条不同的语句创建了名为 points 的新的空数组：

```javascript
var points = new Array();         // 差
var points = [];                  // 优
```

下面两条不同的语句创建包含六个数字的新数组：

```javascript
var points = new Array(40, 100, 1, 5, 25, 10); // 差
var points = [40, 100, 1, 5, 25, 10];          // 优
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_literal)

new 关键词只会使代码复杂化。它还会产生某些不可预期的结果：

```javascript
var points = new Array(40, 100);  // 创建包含两个元素的数组（40 和 100）
```

假如删除其中一个元素会怎么样？

```javascript
var points = new Array(40);       // 创建包含 40 个未定义元素的数组！！！
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_new_error)

## 如何识别数组

常见的问题是：我如何知晓某个变量是否是数组？

问题在于 JavaScript 运算符 `typeof` 返回 "object"：

```javascript
var fruits = ["Banana", "Orange", "Apple", "Mango"];

typeof fruits;             // 返回 object
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_typeof)

`typeof` 运算符返回 "`object`"，因为 `JavaScript 数组属于对象`。

### 解决方案 1：

为了解决这个问题，ECMAScript 5 定义了新方法 `Array.isArray()：`

```javascript
Array.isArray(fruits);     // 返回 true
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_isarray_method)

此方案的问题在于 ECMAScript 5 *不支持老的浏览器*。

### 解决方案 2：

创建您自己的 `isArray()` 函数以解决此问题：

```javascript
function isArray(x) {
    return x.constructor.toString().indexOf("Array") > -1;
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_isarray_function)

假如参数为数组，则上面的函数始终返回 true。

或者更准确的解释是：假如对象原型包含单词 "Array" 则返回 true。

### 解决方案 3：

假如对象由给定的构造器创建，则 `instanceof`  运算符返回 true：

```javascript
var fruits = ["Banana", "Orange", "Apple", "Mango"];
 
fruits instanceof Array     // 返回 true
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_instanceof)
