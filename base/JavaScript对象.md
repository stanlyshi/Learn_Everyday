## 真实生活中的对象、属性和方法

在真实生活中，汽车是一个*对象*。

汽车有诸如车重和颜色等*属性*，也有诸如启动和停止的*方法*：

| 对象                                              | 属性                                                         | 方法                                                         |
| :------------------------------------------------ | :----------------------------------------------------------- | :----------------------------------------------------------- |
| ![car](https://www.w3school.com.cn/i/porsche.jpg) | car.name = porsche<br />car.model = 911<br />car.length = 4499mm<br />car.color = white | car.start()<br />car.drive()<br />car.brake()<br />car.stop() |

所有汽车都拥有同样的*属性*，但属性值因车而异。

所有汽车都拥有相同的*方法*，但是方法会在不同时间被执行。



## JavaScript 对象

您之前已经学到，JavaScript 变量是数据值的容器。

这段代码把一个*单一值*（porsche）赋给名为 car 的*变量*：

```javascript
var car = "porsche";
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_object_variable)

对象也是变量。但是对象包含很多值。

这段代码把*多个值*（porsche, 911, white）赋给名为 car 的*变量*：

```javascript
var car = {type:"porsche", model:"911", color:"white"};
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_object_object)

值以*名称:值*对的方式来书写（名称和值由冒号分隔）。

JavaScript 对象是*被命名值*的容器。

## 对象属性

（JavaScript 对象中的）名称:值对被称为*属性*。

```javascript
var person = {firstName:"Bill", lastName:"Gates", age:62, eyeColor:"blue"};
```

| 属性      | 属性值 |
| :-------- | :----- |
| firstName | Bill   |
| lastName  | Gates  |
| age       | 62     |
| eyeColor  | blue   |

## 对象方法

对象也可以有*方法*。

方法是在对象上执行的*动作*。

方法以*函数定义*被存储在属性中。

| 属性      | 属性值                                                    |
| :-------- | :-------------------------------------------------------- |
| firstName | Bill                                                      |
| lastName  | Gates                                                     |
| age       | 62                                                        |
| eyeColor  | blue                                                      |
| fullName  | function() {return this.firstName + " " + this.lastName;} |

方法是作为属性来存储的函数。

### 实例

```javascript
var person = {
  firstName: "Bill",
  lastName : "Gates",
  id       : 678,
  fullName : function() {
    return this.firstName + " " + this.lastName;
  }
};
```

## this 关键词

在函数定义中，this 引用该函数的“拥有者”。

在上面的例子中，this 指的是“拥有” fullName 函数的 *person 对象*。

换言之，this.firstName 的意思是 *this 对象*的 firstName 属性。

请在 [JS this 关键词](https://www.w3school.com.cn/js/js_this.asp)这一章学习更多有关 this 关键词的知识。

## 对象定义

我们定义（创建）了一个 JavaScript 对象：

### 实例

```javascript
var person = {firstName:"Bill", lastName:"Gates", age:62, eyeColor:"blue"};
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_object_create_1)

空格和折行都是允许的。对象定义可横跨多行：

### 实例

```javascript
var person = {
    firstName:"Bill",
    lastName:"Gates",
    age:50,
    eyeColor:"blue"
};
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_object_create_2)

## 访问对象属性

您能够以两种方式访问属性：

```javascript
objectName.propertyName
```

或者

```javascript
objectName["propertyName"]
```

### 例子 1

```javascript
person.lastName;
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_object_property_1)

### 例子 2

```javascript
person["lastName"];
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_object_property_2)

## 访问对象方法

您能够通过如下语法访问对象方法：

```javascript
objectName.methodName()
```

### 实例

```javascript
name = person.fullName();
```
运行结果
```
Bill Gates
```
[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_object_method)

如果您*不使用 ()* 访问 fullName 方法，则将返回*函数定义*：

### 实例

```javascript
name = person.fullName;
```
运行结果
```
function() { return this.firstName + " " + this.lastName; }
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_object_function)

方法实际上是以属性值的形式存储的函数定义。

## 请不要把字符串、数值和布尔值声明为对象！

如果通过关键词 "new" 来声明 JavaScript 变量，则该变量会被创建为对象：

```javascript
var x = new String();        // 把 x 声明为 String 对象
var y = new Number();        // 把 y 声明为 Number 对象
var z = new Boolean();       //	把 z 声明为 Boolean 对象
```

请避免字符串、数值或逻辑对象。他们会增加代码的复杂性并降低执行速度。

您将在本教程的稍后章节学到更多有关对象的知识。

## 课外书

如需更多有关 *JavaScript 对象*的知识，请阅读 JavaScript 高级教程中的相关内容：

- [ECMAScript 面向对象技术](https://www.w3school.com.cn/js/pro_js_object_oriented.asp)

  本节简要介绍了面向对象技术的术语、面向对象语言的要求以及对象的构成。

- [ECMAScript 对象应用](https://www.w3school.com.cn/js/pro_js_object_working_with.asp)

  本节讲解了如何声明和实例化对象，如何引用和废除对象，以及绑定的概念。

- [ECMAScript 对象类型](https://www.w3school.com.cn/js/pro_js_object_types.asp)

  本节介绍了 ECMAScript 的三种类型：本地对象、内置对象和宿主对象，并提供了指向相关参考手册的链接。

- [ECMAScript 对象作用域](https://www.w3school.com.cn/js/pro_js_object_scope.asp)

  本节讲解了 ECMAScript 作用域以及 this 关键字。

- [ECMAScript 定义类或对象](https://www.w3school.com.cn/js/pro_js_object_defining.asp)

  本节详细讲解了创建 ECMAScript 对象或类的各种方式。

- [ECMAScript 修改对象](https://www.w3school.com.cn/js/pro_js_object_modifying.asp)

  本节讲解了如何通过创建新方法或重定义已有方法来修改对象。
