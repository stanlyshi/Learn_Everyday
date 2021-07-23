**HTML 事件是发生在 HTML 元素上的“事情”。**

**当在 HTML 页面中使用 JavaScript 时，JavaScript 能够“应对”这些事件。**

## HTML 事件

HTML 事件可以是浏览器或用户做的某些事情。

下面是 HTML 事件的一些例子：

- `HTML 网页完成加载`
- `HTML 输入字段被修改`
- `HTML 按钮被点击`

通常，当事件发生时，用户会希望做某件事。

JavaScript 允许您在事件被侦测到时执行代码。

*通过 JavaScript 代码*，HTML 允许您向 HTML 元素添加事件处理程序。

使用单引号：

```html
<element event='一些 JavaScript'>
```

使用双引号：

```html
<element event="一些 JavaScript">
```

在下面的例子中，`onclick` 属性（以及代码）被添加到 `<button>` 元素：

### 实例

```html
<button onclick='document.getElementById("demo").innerHTML=Date()'>现在的时间是？</button>
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_event_onclick)

在上面的例子中，JavaScript 代码改变了 id="demo" 的元素的内容。

在接下来的例子中，代码（使用 this.innerHTML）改变了其自身元素的内容：

### 实例

```html
<button onclick="this.innerHTML=Date()">现在的时间是？</button>
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_event_onclick_this)

JavaScript 代码通常有很多行。事件属性调用函数更为常见：

### 实例

```html
<button onclick="displayDate()">现在的时间是？</button>
```

[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_event_onclick_function)

## 常见的 HTML 事件

下面是一些常见的 HTML 事件：

| 事件        | 描述                         |
| :---------- | :--------------------------- |
| onchange    | HTML 元素已被改变            |
| onclick     | 用户点击了 HTML 元素         |
| onmouseover | 用户把鼠标移动到 HTML 元素上 |
| onmouseout  | 用户把鼠标移开 HTML 元素     |
| onkeydown   | 用户按下键盘按键             |
| onload      | 浏览器已经完成页面加载       |

更完整的列表：[W3School JavaScript 参考手册 HTML DOM 事件](https://www.w3school.com.cn/jsref/dom_obj_event.asp)。

## JavaScript 能够做什么？

事件处理程序可用于处理、验证用户输入、用户动作和浏览器动作：

- `每当页面加载时应该做的事情`
- `当页面被关闭时应该做的事情`
- `当用户点击按钮时应该被执行的动作`
- `当用户输入数据时应该被验证的内容`
- `等等`

让 JavaScript 处理事件的不同方法有很多：

- `HTML 事件属性可执行 JavaScript 代码`
- `HTML 事件属性能够调用 JavaScript 函数`
- `您能够向 HTML 元素分配自己的事件处理函数`
- `您能够阻止事件被发送或被处理`
- `等等`

您将在 HTML DOM 章节中学到更多有关事件和事件处理程序的知识。
