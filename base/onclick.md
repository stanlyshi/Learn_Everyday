## 1.JavaScript 能够改变 HTML 内容

getElementById() 是多个 JavaScript HTML 方法之一。
本例使用该方法来“查找” id="demo" 的 HTML 元素，并把元素内容（innerHTML）更改为 "Hello JavaScript"：
**实例**
```
document.getElementById("demo").innerHTML = "Hello JavaScript";
```
>_提示：JavaScript 同时接受双引号和单引号！_

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


## 2.JavaScript 能够改变 HTML 属性

本例通过改变 <img> 标签的 src 属性（source）来改变一张 HTML 图像：

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



## 3.JavaScript 能够改变 CSS 样式

改变 HTML 元素的样式，是改变 HTML 属性的一种变种：
**实例**
```
document.getElementById("demo").style.fontSize = "25px";
```

```javascript
<!DOCTYPE html>
<html>
    <body>
        <h2>JavaScript 能够做什么</h2>
        <p id="demo">JavaScript 能够改变 HTML 元素的样式。</p>
        <button type="button" onclick="document.getElementById('demo').style.fontSize='35px'">点击我！</button>
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
**Javascript 能够改变 HTML 元素的样式。**
```


## 4.JavaScript 能够隐藏 HTML 元素
可通过改变 display 样式来隐藏 HTML 元素：
**实例**
```
document.getElementById("demo").style.display="none";
```

```javascript
<!DOCTYPE html>
<html>
    <body>
        <h2>JavaScript 能够做什么</h2>
        <p id="demo">JavaScript 能够隐藏 HTML 元素。</p>
        <button type="button" onclick="document.getElementById('demo').style.display='none'">点击我！</button>
    </body>
</html>
```
[点击测试](https://www.w3school.com.cn/tiy/t.asp?f=js_intro_hide)

运行前
```
Javascript 能够隐藏 HTML 元素。
```
运行结果
```

```


## 5.JavaScript 能够显示隐藏的 HTML 元素
可通过改变 display 样式来显示隐藏的 HTML 元素：
**实例**
```
document.getElementById("demo").style.display="block";
```

```javascript
<!DOCTYPE html>
<html>
    <body>
        <h2>JavaScript 能够做什么</h2>
        <p>JavaScript 能够显示隐藏的 HTML 元素。</p>
        <p id="demo" style="display:none">Hello JavaScript!</p>
        <button type="button" onclick="document.getElementById('demo').style.display='block'">点击我！</button>
    </body>
</html>
```
[点击测试](https://www.w3school.com.cn/tiy/t.asp?f=js_intro_hide)

运行前
```

```
运行结果
```
Hello Javascript!
```

## 6.案例


