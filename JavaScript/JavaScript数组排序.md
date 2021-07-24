**sort() 方法是最强大的数组方法之一。**

## 数组排序

`sort()` 方法以字母顺序对数组进行排序：

### 实例

```javascript
var fruits = ["Banana", "Orange", "Apple", "Mango"];
fruits.sort();            // 对 fruits 中的元素进行排序
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_sort)



## 反转数组

`reverse()` 方法反转数组中的元素。

您可以使用它以降序对数组进行排序：

### 实例

```javascript
var fruits = ["Banana", "Orange", "Apple", "Mango"];
fruits.sort();            // 对 fruits 中的元素进行排序
fruits.reverse();         // 反转元素顺序
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_sort_reverse)



## 数字排序

默认地，sort() 函数按照*字符串*顺序对值进行排序。

该函数很适合字符串（"Apple" 会排在 "Banana" 之前）。

不过，`如果数字按照字符串来排序，则 "25" 大于 "100"，因为 "2" 大于 "1"`。

正因如此，`sort() 方法在对数值排序时会产生不正确的结果`。

我们通过一个*比值函数*来修正此问题：

### 实例

```javascript
var points = [40, 100, 1, 5, 25, 10];
points.sort(function(a, b){return a - b}); 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_sort_numeric_1)

使用相同的技巧对数组进行降序排序：

### 实例

```javascript
var points = [40, 100, 1, 5, 25, 10];
points.sort(function(a, b){return b - a}); 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_sort_numeric_2)



## 比值函数

比较函数的目的是定义另一种排序顺序。

比较函数应该返回一个负，零或正值，这取决于参数：

```javascript
function(a, b){return a-b}
```

当 sort() 函数比较两个值时，会将值发送到比较函数，并根据所返回的值（负、零或正值）对这些值进行排序。

### 实例：

当比较 40 和 100 时，sort() 方法会调用比较函数 function(40,100)。

该函数计算 40-100，然后返回 -60（负值）。

排序函数将把 40 排序为比 100 更低的值。

您可以使用下面的代码片段来测试数值和字母排序：

```html
<button onclick="myFunction1()">以字母顺序排序</button>
<button onclick="myFunction2()">以数字顺序排序</button>

<p id="demo"></p>

<script>
var points = [40, 100, 1, 5, 25, 10];
document.getElementById("demo").innerHTML = points;

function myFunction1() {
		points.sort();
		document.getElementById("demo").innerHTML  = points;
}
function myFunction2() {
		points.sort(function(a, b){return  a - b});
		document.getElementById("demo").innerHTML = points;
}
</script>
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_sort_alpha)



## 以随机顺序排序数组

### 实例

```javascript
var points = [40, 100, 1, 5, 25, 10];
points.sort(function(a, b){return 0.5 - Math.random()}); 
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_sort_random)



## 查找最高（或最低）的数组值

JavaScript 不提供查找数组中最大或最小数组值的内建函数。

不过，在对数组进行排序之后，您能够使用索引来获得最高或最低值。

升序排序：

### 实例

```javascript
var points = [40, 100, 1, 5, 25, 10];
points.sort(function(a, b){return a - b});

// 现在 points[0] 包含最低值
// 而 points[points.length-1] 包含最高值
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_sort_low)

降序排序：

### 实例

```javascript
var points = [40, 100, 1, 5, 25, 10];
points.sort(function(a, b){return b - a});

// 现在 points[0] 包含最高值
// 而 points[points.length-1] 包含最低值
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_sort_high)

如果您仅仅需要找到最高或最低值，对整个数组进行排序是效率极低的方法。



## 对数组使用 Math.max()

您可以使用 `Math.max.apply` 来查找数组中的最高值：

### 实例

```javascript
function myArrayMax(arr) {
    return Math.max.apply(null, arr);
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_sort_math_max)

Math.max.apply([1, 2, 3]) 等于 Math.max(1, 2, 3)。



## 对数组使用 Math.min()

您可以使用 `Math.min.apply` 来查找数组中的最低值：

### 实例

```javascript
function myArrayMin(arr) {
    return Math.min.apply(null, arr);
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_sort_math_min)

Math.min.apply([1, 2, 3]) 等于 Math.min(1, 2, 3)。



## 我的 Min / Max JavaScript 方法

最快的解决方法是使用“自制”方法。

此函数遍历数组，用找到的最高值与每个值进行比较：

### 实例（查找 Max）

```javascript
function myArrayMax(arr) {
    var len = arr.length
    var max = -Infinity;
    while (len--) {
        if (arr[len] > max) {
            max = arr[len];
        }
    }
    return max;
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_sort_max)

此函数遍历数组，用找到的最低值与每个值进行比较：

### 实例（查找 Min）

```javascript
function myArrayMin(arr) {
    var len = arr.length
    var min = Infinity;
    while (len--) {
        if (arr[len] < min) {
            min = arr[len];
        }
    }
    return min;
}
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_sort_min)



## 排序对象数组

JavaScript 数组经常会包含对象：

### 实例

```javascript
var cars = [
{type:"Volvo", year:2016},
{type:"Saab", year:2001},
{type:"BMW", year:2010}];
```

即使对象拥有不同数据类型的属性，sort() 方法仍可用于对数组进行排序。

解决方法是通过比较函数来对比属性值：

### 实例

```javascript
cars.sort(function(a, b){return a.year - b.year});
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_sort_object_1)

比较字符串属性会稍复杂：

### 实例

```javascript
cars.sort(function(a, b){
	  var x = a.type.toLowerCase();
	  var y = b.type.toLowerCase();
	  if (x < y) {return -1;}
	  if (x > y) {return 1;}
	  return 0;
});
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_array_sort_object_2)
