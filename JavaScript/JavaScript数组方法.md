**JavaScript 数组的力量隐藏在数组方法中。**

## 把数组转换为字符串

JavaScript 方法 `toString()` 把`数组转换为数组值（逗号分隔）的字符串`。

### 实例

```
var fruits = ["Banana", "Orange", "Apple", "Mango"];
document.getElementById("demo").innerHTML = fruits.toString(); 
```

结果

```
Banana,Orange,Apple,Mango
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_tostring)

`join()` 方法也可将所有数组元素结合为一个字符串。

它的行为类似 `toString()`，但是您还可以规定分隔符：

### 实例

```
var fruits = ["Banana", "Orange","Apple", "Mango"];
document.getElementById("demo").innerHTML = fruits.join(" * "); 
```

结果

```
Banana * Orange * Apple * Mango
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_join)

## Popping 和 Pushing

在处理数组时，删除元素和添加新元素是很简单的。

Popping 和 Pushing 指的是：

从数组*弹出*项目，或向数组*推入*项目。

## Popping

`pop()` 方法从数组中删除最后一个元素：

### 实例

```
var fruits = ["Banana", "Orange", "Apple", "Mango"];
fruits.pop();              // 从 fruits 删除最后一个元素（"Mango"）
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_pop)

`pop()` 方法返回“被弹出”的值：

### 实例

```
var fruits = ["Banana", "Orange", "Apple", "Mango"];
var x = fruits.pop();      // x 的值是 "Mango"
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_pop_out)

## Pushing

`push()` 方法（在数组结尾处）向数组添加一个新的元素：

### 实例

```
var fruits = ["Banana", "Orange", "Apple", "Mango"];
fruits.push("Kiwi");       //  向 fruits 添加一个新元素
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_push)

`push()` 方法返回新数组的长度：

### 实例

```
var fruits = ["Banana", "Orange", "Apple", "Mango"];
var x =  fruits.push("Kiwi");   //  x 的值是 5
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_push_length)

## 位移元素

位移与弹出等同，但处理首个元素而不是最后一个。

`shift()` 方法会删除首个数组元素，并把所有其他元素“位移”到更低的索引。

### 实例

```
var fruits = ["Banana", "Orange", "Apple", "Mango"];
fruits.shift();            // 从 fruits 删除第一个元素 "Banana"
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_shift)

`shift()` 方法返回被“位移出”的字符串：

### 实例

```
var fruits = ["Banana", "Orange", "Apple", "Mango"];
fruits.shift();             // 返回 "Banana"
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_shift_return)

`unshift()` 方法（在开头）向数组添加新元素，并“反向位移”旧元素：

### 实例

```
var fruits = ["Banana", "Orange", "Apple", "Mango"];
fruits.unshift("Lemon");    // 向 fruits 添加新元素 "Lemon"
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_unshift)

`unshift()` 方法返回新数组的长度。

### 实例

```
var fruits = ["Banana", "Orange", "Apple", "Mango"];
fruits.unshift("Lemon");    // 返回 5
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_unshift_return)

## 更改元素

通过使用它们的*索引号*来访问数组元素：

数组*索引（下标）*以 0 开始。[0] 是第一个数组元素，[1] 是第二个，[2] 是第三个 ...

### 实例

```
var fruits = ["Banana", "Orange", "Apple", "Mango"];
fruits[0] = "Kiwi";        // 把 fruits 的第一个元素改为 "Kiwi"
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_change)

`length 属性`提供了向数组追加新元素的简易方法：

### 实例

```
var fruits = ["Banana", "Orange", "Apple", "Mango"];
fruits[fruits.length] = "Kiwi";          // 向 fruits 追加 "Kiwi"
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_change_add)

## 删除元素

既然 JavaScript 数组属于对象，其中的元素就可以使用 JavaScript delete 运算符来*删除*：

### 实例

```
var fruits = ["Banana", "Orange", "Apple", "Mango"];
delete fruits[0];           // 把 fruits 中的首个元素改为 undefined
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_delete)

使用 `delete` 会`在数组留下未定义的空洞`。请使用 pop() 或 shift() 取而代之。

## 拼接数组

`splice()` 方法可用于向数组添加新项：

### 实例

```
var fruits = ["Banana", "Orange", "Apple", "Mango"];
fruits.splice(2, 0, "Lemon", "Kiwi");
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_splice)

第一个参数（2）定义了应添加新元素的位置（拼接）。

第二个参数（0）定义应删除多少元素。

其余参数（“Lemon”，“Kiwi”）定义要添加的新元素。

`splice()` 方法返回一个包含已删除项的数组：

### 实例

```
var fruits = ["Banana", "Orange", "Apple", "Mango"];
fruits.splice(2, 2, "Lemon", "Kiwi");
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_splice_return)

## 使用 splice() 来删除元素

通过聪明的参数设定，您能够使用 splice() 在数组中不留“空洞”的情况下移除元素：

### 实例

```
var fruits = ["Banana", "Orange", "Apple", "Mango"];
fruits.splice(0, 1);        // 删除 fruits 中的第一个元素
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_splice_remove)

第一个参数（0）定义新元素应该被*添加*（接入）的位置。

第二个参数（1）定义应该*删除多个*元素。

其余参数被省略。没有新元素将被添加。

## 合并（连接）数组

`concat()` 方法通过合并（连接）现有数组来创建一个新数组：

### 实例（合并两个数组）

```
var myGirls = ["Cecilie", "Lone"];
var myBoys = ["Emil", "Tobias", "Linus"];
var myChildren = myGirls.concat(myBoys);   // 连接 myGirls 和 myBoys
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_concat_1)

`concat()` 方法`不会更改现有数组`。它总是返回一个新数组。

`concat()` 方法可以使用任意数量的数组参数：

### 实例（合并三个数组）

```
var arr1 = ["Cecilie", "Lone"];
var arr2 = ["Emil", "Tobias", "Linus"];
var arr3 = ["Robin", "Morgan"];
var myChildren = arr1.concat(arr2, arr3);   // 将arr1、arr2 与 arr3 连接在一起
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_concat_2)

`concat()` 方法也可以将值作为参数：

### 实例（将数组与值合并）

```
var arr1 = ["Cecilie", "Lone"];
var myChildren = arr1.concat(["Emil", "Tobias", "Linus"]); 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_concat_3)

## 裁剪数组

`slice()` 方法用数组的某个片段切出新数组。

本例从数组元素 1 （"Orange"）开始切出一段数组：

### 实例

```
var fruits = ["Banana", "Orange", "Lemon", "Apple", "Mango"];
var citrus = fruits.slice(1); 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_slice_1)

`slice() 方法创建新数组`。它`不会从源数组中删除任何元素`。

本例从数组元素 3 （"Apple"）开始切出一段数组：

### 实例

```
var fruits = ["Banana", "Orange", "Lemon", "Apple", "Mango"];
var citrus = fruits.slice(3); 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_slice_2)

`slice() 可接受两个参数`，比如 (1, 3)。

该方法会从开始参数选取元素，直到结束参数（不包括）为止。

### 实例

```
var fruits = ["Banana", "Orange", "Lemon", "Apple", "Mango"];
var citrus = fruits.slice(1, 3); 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_slice_3)

如果结束参数被省略，比如第一个例子，则 slice() 会切出数组的剩余部分。

### 实例

```
var fruits = ["Banana", "Orange", "Lemon", "Apple", "Mango"];
var citrus = fruits.slice(2); 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_slice_4)

## 自动 toString()

如果需要原始值，则 JavaScript 会自动把数组转换为字符串。下面两个例子将产生相同的结果：

### 实例

```
var fruits = ["Banana", "Orange", "Apple", "Mango"];
document.getElementById("demo").innerHTML = fruits.toString(); 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_tostring)

### 实例

```
var fruits = ["Banana", "Orange", "Apple", "Mango"];
document.getElementById("demo").innerHTML = fruits; 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_automatic)

所有 JavaScript 对象都拥有 `toString()` 方法。

## 数组排序

我们将在下一章学习到数组排序的知识~。

## 查找数组中的最大和最小值

没有在 JavaScript 数组中查找最高和最低值的内建函数。

您将在本教程的下一章学习如何解决这个问题。

## 完整的数组参考手册

如需完整的参考手册，请访问我们完整的 [JavaScript 数组参考手册](https://www.w3school.com.cn/jsref/jsref_obj_array.asp)。

该参考手册包含所有数组属性和方法的描述和实例。
