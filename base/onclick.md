## JavaScript 能够改变 HTML 内容

```javascript
<!DOCTYPE html>
<html>
<body>

<h2>JavaScript 能做什么</h2>

<p id="demo">JavaScript 能够改变 HTML 内容。</p>

<button type="button" onclick='document.getElementById("demo").innerHTML = "Hello JavaScript!"'>点击我！</button>

</body>
</html>
```
[点击测试](https://www.w3school.com.cn/tiy/t.asp?f=js_intro_inner_html)

运行前
```
Javascript 能够改变 HTML 内容。
```
运行结果
```
Hello Javascript!
```


## JavaScript 能够改变 HTML 属性

```javascript
<!DOCTYPE html>
<html>
<body>

<h2>JavaScript 能做什么？</h2>

<p>JavaScript 能够改变 HTML 属性值。</p>

<p>在本例中，JavaScript 改变了图像的 src 属性值。</p>

<button onclick="document.getElementById('myImage').src='/i/eg_bulbon.gif'">开灯</button>

<img id="myImage" border="0" src="/i/eg_bulboff.gif" style="text-align:center;">

<button onclick="document.getElementById('myImage').src='/i/eg_bulboff.gif'">关灯</button>

</body>
</html>
```
[点击测试](https://www.w3school.com.cn/tiy/t.asp?f=js_intro_lightbulb)



## JavaScript 能够改变 CSS 样式

```javascript
<!DOCTYPE html>
<html>
<body>

<h2>JavaScript 能够做什么</h2>

<p id="demo">JavaScript 能够改变 HTML 元素的样式。</p>

<button type="button" onclick="document.getElementById('demo').style.fontSize='35px'">
点击我！
</button>


</body>
</html> 
```
[点击测试](https://www.w3school.com.cn/tiy/t.asp?f=js_intro_style)
运行前
```
Javascript 能够改变 HTML 元素的样式。
```
运行结果
```
# Javascript 能够改变 HTML 元素的样式。
```

