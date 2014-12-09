#索引
* [标题](#标题)
* [换行](#换行)
* [引用](#引用)
* [列表](#列表)
* [代码区域](#代码区域)
* [图片](#图片)

#标题
rkdown提供了两种方式（Setext和Atx）来显示标题

### 语法

Setext方式

```
标题1
====================

标题2
---------------------

Atx方式
#标题1
## 标题2
###### 标题6
```

#换行
在文字的末尾使用两个或两个以上的空格来表示换行

#引用
行首使用>加上一个空格表示引用段落，内部可以嵌套多个引用


#列表
无序列表使用*、+或-后面加上空格来表示
###语法：
```
* Item1
* Item2
* Item3

+ Item1
+ Item2
+ Item3

- Item1
- Item2
- Item3

```

### 效果：

* Item1
* Item2
* Item3

+ Item1
+ Item2
+ Item3

- Item1
- Item2
- Item3


有序列表使用数字加英文句号加空格表示

###语法：
```
1. Item1
2. Item2
3. Item3

```

###效果：
1. Item1
2. Item2
3. Item3


#代码区域

行内代码使用反斜杠`表示
代码段落则是在每行文字前加4个空格或者一个缩进符表示

###语法：
```
Bash中可以使用echo来进行输出
    $ echo 'SOmething'
    $ echo -e '\tSomething\n'
```
sh中可以使用echo进行输出   

    $ echo 'SOmething'       
    $ echo -e '\tSomething\n' 

#强调
Markdown使用\*或\_表示强调
###语法：
```
单星号 = *斜体*
单下划线 = _斜体_
双星号 =  **加粗**
双下划线 = __加粗__
```
###效果：
单星号 = *斜体*  
单下划线 = _斜体_  
双星号 = **加粗**  
双下划线 = __加粗__  

#链接
Markdown支持两种风格的链接:Inline和Reference
###语法
**Inline:**以中括号标记显示的链接文本,后面紧跟用小括号包围的链接.如果链接有title属性,则在链接中使用空格家"title属性".  
**Reference:**一般应用于多个不同的位置使用相同链接.通常分为两个部分:  
调用部分为[链接文本][ref];
定义部分可以出现在文本中的其他位置,格式为[ref]:http://some/link/address(可选的标题)  
注:ref中不区分大小写
```
这是一个Inline[示例](https://github.com/OkabeRintarou/syl/tree/master/git-man/markdown)
这是一个Reference[示例][ref]
[ref]:https://github.com/OkabeRintarou/syl/tree/master/git-man/markdown
```
###效果:
这是一个Inline[示例](https://github.com/OkabeRintarou/syl/tree/master/git-man/markdown)              
这是一个Reference[示例][ref]
[ref]:https://github.com/OkabeRintarou/syl/tree/master/git-man/markdown
#图片
图片的使用方法基本上和链接类似,只是在中括号前加叹号     
**注:Markdown不能设置图片大小,如果必须设置则应该使用HTML标记<img>**   
###语法：
```
Inline示例:![替代文本](/img/racket.jpg "可选的title")
Reference示例:![替代文本][pic]
[pic]:/img/racket.jpg
HTML示例:<img src="/img/racket" alt="替代文本" title="标题文本" width="200" />
```
###效果:
![](img/racket.jpg "可选的title")
