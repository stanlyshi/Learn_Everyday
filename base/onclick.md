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
![点击测试](https://www.w3school.com.cn/tiy/t.asp?f=js_intro_inner_html)
运行前
```
Javascript 能够改变 HTML 内容。
```
运行结果
```
Hello Javascript!
```
