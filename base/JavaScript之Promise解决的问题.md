## Promise解决的问题

相信每个前端都遇到过这样一个问题，当一个异步任务的执行需要依赖另一个异步任务的结果时，我们一般会将两个异步任务嵌套起来，这种情况发生一两次还可以忍，但是发生很多次之后，就形成了所谓的回调地狱，代码层层嵌套，环环相扣，很明显，逻辑稍微复杂一些，这样的程序就会变得难以维护。就比如我们昨天的那个回调地狱的例子：

```javascript
function funA(callback) {
	console.log("A");
	setTimeout(() = > {
		callback()
	}, 100)
}

function funB() {
	console.log("B")
}

function funC(callback) {
	console.log("C")
	setTimeout(() = > {
		callback()
	}, 1000)
}

function funD() {
	console.log("D")
}

function funE() {
	console.log("E")
}

function funF() {
	console.log("F")
}

funA(() = > {
	funB()
	funC(() = > {
		funD()
	})
	funE()
})
funF()
```

对于这种情况，程序员们想了很多解决方案（比如将代码模块化），但流程控制上，还是没有避免大量的嵌套。但在ES6之后的标准里，Promise的标准化，一定程度上解决了JavaScript的流程操作问题。

## 什么是Promise

在 **《异步与性能》** 的第三章中有这么个场景来比喻 Promise：

```
　　我走到快餐店的柜台前，点了一个起士汉堡。并交了1.47美元的现金。通过点餐和付款，我为得到一个 值（起士汉堡）制造了一个请求。我发起了一个事务。

　　但是通常来说，起士汉堡不会立即到我手中。收银员交给一些东西代替我的起士汉堡：一个带有点餐排队号的收据。这个点餐号是一个“我欠你”的许诺（Promise），它保证我最终会得到我的起士汉堡。

　　于是我就拿着我的收据和点餐号。我知道它代表我的 未来的起士汉堡，所以我无需再担心它——除了挨饿！

　　在我等待的时候，我可以做其他的事情，比如给我的朋友发微信说，“嘿，一块儿吃午餐吗？我要吃起士汉堡”。

　　我已经在用我的 未来的起士汉堡 进行推理了，即便它还没有到我手中。我的大脑可以这么做是因为它将点餐号作为起士汉堡的占位符号。这个占位符号实质上使这个值 与时间无关。它是一个 未来的值。

　　最终，我听到，“113号！”。于是我愉快地拿着收据走回柜台前。我把收据递给收银员，拿回我的起士汉堡。 换句话说，一旦我的 未来的值 准备好，我就用我的许诺值换回值本身。

　　但还有另外一种可能的输出。它们叫我的号，但当我去取起士汉堡时，收银员遗憾地告诉我，“对不起，看起来我们的起士汉堡卖光了。”把这种场景下顾客有多沮丧放在一边，我们可以看到 未来的值 的一个重要性质：它们既可以表示成功也可以表示失败。

　　每次我点起士汉堡时，我都知道我要么最终得到一个起士汉堡，要么得到起士汉堡卖光的坏消息，并且不得不考虑中午吃点儿别的东西。

　　我由等待汉堡变成了等到或者等不到，这个过程不可逆，
```

上面很形象的介绍了promise，上面的等待汉堡和得到汉堡，汉堡卖光了，得不到汉堡，分别对应promise的三种状态 

```javascript
pending: 进行中，既不是成功，也不是失败状态。 
fulfilled: 意味着操作成功完成。 
rejected: 意味着操作失败。
```

## Promise的基本用法

语法

```javascript
new Promise( function(resolve, reject) {...} ); //reject参数 可不选
```

参数

```javascript
executor
```

`executor`是带有 `resolve` 和 `reject` 两个参数的函数 。`Promise`构造函数执行时立即调用`executor` 函数， `resolve` 和 `reject` 两个函数作为参数传递给`executor`（`executor` 函数在`Promise`构造函数返回新建对象前被调用）。`resolve` 和 `reject` 函数被调用时，分别将`promise`的状态改为`fulfilled`（完成）或`rejected`（失败）。`executor` 内部通常会执行一些异步操作，一旦完成，可以调用`resolve`函数来将`promise`状态改成`fulfilled`，或者在发生错误时将它的状态改为rejected。
如果在`executor`函数中抛出一个错误，那么该`promise` 状态为`rejected`。`executor`函数的返回值被忽略。

对更多对Promise的描述感兴趣的可以 点击查看MDN Promise下面我们开始上代码

新建一个Promise的实例：

```javascript
let promise = new Promise((resolve, reject) = > {
	setTimeout(() = > {
		let random = Math.random()
		if (random > 0.5) {
			resolve(`resolve$ {random}`)
		} else {
			resolve(`reject$ {random}`)
		}
	}, 1000)
})
```

由上所示，Promise的构造函数接收一个函数作为参数，该函数接受两个额外的函数，resolve和reject，这两个函数分别代表将当前Promise置为fulfilled(已成功)和rejected(已失败)两个状态。Promise正是通过这两个状态来控制异步操作的结果。接下来我们将讨论Promise的用法，实际上Promise上的实例promise是一个对象，不是一个函数。在声明的时候，Promise传递的参数函数会立即执行，因此Promise使用的正确姿势是在其外层再包裹一层函数。

```javascript
let run = function() {
  return new Promise((resolve, reject) => {
	setTimeout(() => {
		let random = Math.random()
		if (random > 0.5) {
			resolve(`resolve:${random}`)
		} else {
			reject(`reject:${random}`)
		}
	}, 1000)
})
}

run()
```

这是Promise的正常用法，接下来，就是对异步操作结果的处理，接着上面创建的函数run()

```javascript
run().then(
function(value) {
	console.log(value)
})
```

每个Promise的实例对象，都有一个then的方法，这个方法就是用来处理之前各种异步逻辑的结果。

then方法可以接受两个回调函数作为参数。第一个回调函数是Promise对象的状态变为resolved时调用，第二个回调函数是Promise对象的状态变为rejected时调用。其中，第二个函数是可选的，不一定要提供。这两个函数都接受Promise对象传出的值作为参数。
下面是一个用Promise对象实现的 Ajax 操作的例子:

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>JavaScript异步</title>
</head>
<body>
<script src="https://unpkg.com/jquery@3.3.1/dist/jquery.min.js"></script>
<script>
    new Promise((resolve, reject) => {
        $.ajax({
            url: "https://easy-mock.com/mock/5c249dbe46e8386d0b21b475/example_copy_copy/promisetest",
            success: res => {
                if (res.code == 0) {
                    resolve(res.data)
                } else {
                    reject(res.desc)
                }
            }
        });
    })
        .then(res => {
            console.log(res);
        },err =>{
            console.log(err)
        })
</script>
</body>
</html>
```

当res.code == 0 为接口调用成功 输出：

![](https://img-blog.csdnimg.cn/20190104180929688.png)]

手动把 上面代码 res.code == 0 改为 res.code == 1 就会抱一个错误，输出：



![](https://img-blog.csdnimg.cn/20190104191333700.png)

如果异步操作获得了我们想要的结果，那我们将调用resolve函数，在then的第一个作为参数的匿名函数中可以获取数据，如果我们得到了错误的结果，调用reject函数，在then函数的第二个作为参数的匿名函数中获取错误处理数据。
这样，一个次完整的Promise调用就结束了。对于Promise的then()方法，then总是会返回一个Promise实例，因此你可以一直调用then，形如run().then().then().then().then().then()…
在一个then()方法调用异步处理成功的状态时，你既可以return一个确定的“值”，也可以再次返回一个Promise实例，当返回的是一个确切的值的时候，then会将这个确切的值传入一个默认的Promise实例，并且这个Promise实例会立即置为fulfilled状态，以供接下来的then方法里使用。看代码：

```javascript
  let num = 0
    let run = function() {
        return new Promise(resolve => {
            resolve(`${num}`)})
    }

run().then(val => {
    console.log(val)
    return val
})
    .then(val =>{
        val++
        console.log(val)
        return val
    })
    .then(val =>{
        val++
        console.log(val)
    })
```

输出：

```
0

1

2
```

根据这个特性，我们就可以将相互依赖的多个异步逻辑，进行比较顺序的管理了，解决了让人头痛的回调地狱问题。

今天我们就先到这里，明天我们讲一下Promise.then()与Promise.catch()
