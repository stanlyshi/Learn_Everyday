## JavaScript 不提供任何内建的打印或显示函数。

## JavaScript 显示方案
JavaScript 能够以不同方式“显示”数据：

* 使用 window.alert() 写入警告框
* 使用 document.write() 写入 HTML 输出
* 使用 innerHTML 写入 HTML 元素
* 使用 console.log() 写入浏览器控制台

## 使用 innerHTML
如需访问 HTML 元素，JavaScript 可使用``` document.getElementById(id) ```方法。
```id ```属性定义 HTML 元素。innerHTML 属性定义 HTML 内容：

**实例**
```javascript
<!DOCTYPE html>
<html>
<body>

<h1>我的第一张网页</h1>

<p>我的第一个段落</p>

<p id="demo"></p>

<script>
 document.getElementById("demo").innerHTML = 5 + 6;
</script>

</body>
</html> 
```
[亲自试一试](https://www.w3school.com.cn/tiy/t.asp?f=js_output_dom)

>_提示：更改 HTML 元素的 innerHTML 属性是在 HTML 中显示数据的常用方法。

使用 document.write()
出于测试目的，使用 document.write() 比较方便：

实例
<!DOCTYPE html>
<html>
<body>

<h1>我的第一张网页</h1>

<p>我的第一个段落</p>

<script>
document.write(5 + 6);
</script>

</body>
</html> 
亲自试一试
注意：在 HTML 文档完全加载后使用 document.write() 将删除所有已有的 HTML ：

实例
<!DOCTYPE html>
<html>
<body>

<h1>我的第一张网页</h1>

<p>我的第一个段落</p>

<button onclick="document.write(5 + 6)">试一试</button>

</body>
</html>
亲自试一试
提示：document.write() 方法仅用于测试。

使用 window.alert()
您能够使用警告框来显示数据：

实例
<!DOCTYPE html>
<html>
<body>

<h1>我的第一张网页</h1>

<p>我的第一个段落</p>

<script>
window.alert(5 + 6);
</script>

</body>
</html> 
亲自试一试
使用 console.log()
在浏览器中，您可使用 console.log() 方法来显示数据。

请通过 F12 来激活浏览器控制台，并在菜单中选择“控制台”。

实例
<!DOCTYPE html>
<html>
<body>

<h1>我的第一张网页</h1>

<p>我的第一个段落</p>

<script>
console.log(5 + 6);
</script>

</body>
</html>
亲自试一试
