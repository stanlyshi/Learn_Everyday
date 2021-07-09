# js中定义变量有三种方式

## 1.const

const定义的变量在js中意味着这个变量唯一（即不可以重复定义）且不可修改并且在定义变量时就需要去初始化，如果你在js中重复同一个const变量，及如下所示
```javascript
 const test="sa";
		const test="1";
```
则会在运行时报错
```
Uncaught SyntaxError: Identifier 'test' has already been declared
```
如果你在定义了个const变量后去修改
```javascript
const test="sa";
		test="1";
```
则会出现另一个错误
```
index.1.html:17 Uncaught TypeError: Assignment to constant variable.
```

## 2.var
如果你在js中定义一个var变量，那么在js中可以重复定义但是会出现值覆盖的情形，
```javascript
<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8" />
		<title></title>
		<script src="https://cdn.bootcdn.net/ajax/libs/vue/2.6.12/vue.js"></script>
		<script src="https://cdn.bootcdn.net/ajax/libs/vue-router/3.1.6/vue-router.js"></script>
	</head>
	<body>
		<div id="sa">
			<label>{{name}}</label>
		</div>
	</body>
	<script>
		var a="my";
		var b="you"
		var v = new Vue({
			data: {
				name:a
			},
			methods: {
				after:function(){
					this.$router.go(-1)
				},
				goto:function(){
					this.$router.go(1)
				},
			},
		}).$mount("#sa")
	</script>
</html>
```
运行结果
```
my
```

并且你在定义的时候可以不用初始化值，还可以对该变量的值进行修改

## 3.let
.let是块级作用域，函数内部使用let定义后，对函数外部无影响。
```javascript
<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8" />
		<title></title>
		<script src="https://cdn.bootcdn.net/ajax/libs/vue/2.6.12/vue.js"></script>
		<script src="https://cdn.bootcdn.net/ajax/libs/vue-router/3.1.6/vue-router.js"></script>
	</head>
	<body>
		<div id="sa">
			<label v-on:click="before">{{name}}</label>
		</div>
	</body>
	<script>
		var v = new Vue({
			data: {
				name:"hello"
			},
			methods: {
				after:function(){
					let a=3;					
				},
				before:function(){
					console.log(this.a)
				}
			},
			
		}).$mount("#sa")
	</script>
</html>
```

运行结果
```
hello
```
