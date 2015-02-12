
##### 1.1 将序列解包为单独变量   
python使用赋值操作符可以将一个有N个元素的序列解包为N个变量的集合
```python
data = ['ACME',50,91.1,(2012,12,21)]
name,shares,price,date = data
# name = 'ACME',shares = 50,price = 91.9,date = (2012,12,21)
```
解包可以作用于任何可迭代的对象，包括元组、列表、字符串、文件、迭代器和生成器等,前提是变量的个数要与序列中元素的个数相同。解包时可以使用下划线(或者其他常见的一次性变量名称，例如：ign)指定想要丢弃的特定的值：
```python
data = ['ACME',50,91.1,(2012,12,21)]
_,shares,price,_ = data
# shares = 50,price = 91.9
```
##### 1.2 从任意长度的可迭代对象中解包元素
当想要从一个拥有大于N个元素的可迭代对象解包为N个变量，可以使用“星号表达式”：
```python
record = ('Dave','dave@sample.com','773-555-1212','847-555-1212')
name,email,*phone_numbers = record
# name = 'Dave',email = 'dave@sample.com',phone_numbers = ['773-555-1212','847-555-1212']
```
##### 1.3 保存最新的N条记录
当想要记录下某操作后最新的N条记录可以使用collections模块下的deque类
```python
from collections import deque

def search(lines,pattern,histroy):
    previous_lines = deque(maxlen=5)
    for line in lines:
        if pattern in line:
            yield line,previous_lines
            previous_lines.append(line)
```
使用deque(maxlen=N)创建一个固定长度的双端队列。deque可以在O(1)复杂度内在结构的**两端**插入和删除元素，而list在列表头删除和插入元素需要O(N)
##### 1.4 找出最大（或最小）的N个元素
想要返回一个集合中最大（或最小）的N个元素的列表时，可以使用heapq模块的nlargest和nsamllest方法
```python
import heapq
nums = [1,8,2,23,7,-4,18,23,42,37,2]
print(heapq.nlargest(3,num)) # [42,37,23]
print(heapq.nsmallest(3,num)) # [-4,1,2]
```
该方法也接受一个key参数用于比较复杂的数据结构：
```python
portfolio = [{'name':'IBM','shares':100,'price':91.9},
		     {'name':'AAPL','shares':50,'price':543.2},
			 {'name':'FB','shares':200,'price':21.09},
		     {'name':'HPQ','shares':35,'price':31.75},
			 {'name':'YHOO','shares':45,'price':115.6}
			]
	import heapq
	cheap = heapq.nsmallest(3,portfolio,key = lambda s:s['price'])
	print(cheap)
	# [{'price': 21.09, 'name': 'FB', 'shares': 200}, {'price': 31.75, 'name': 'HPQ', 'shares': 35},{'price': 91.9, 'name': 'IBM', 'shares': 100}]
```
##### 1.5 实现一个优先级队列
实现一个优先级队列，每次pop操作返回优先级最高的元素
```pythoon
import heapq
class PriorityQueue:
	def __init__(self):
		self._queue = []
		self._index = 0
	def push(self,item,priority):
		heapq.heappush(self._queue,(-priority,self._index,item))
		self._index += 1
	def pop(self):
		return heapq.heappop(self._queue)[-1]
```
##### 1.6 多值映射
创建一个将键映射到多个值得字典，可以将值存储在另一个容器（如list、set)中：
```python
pairs = [('a',1),('b',2),('a',3),('a',5)]
d = {}
for key,value in pairs:
	if not d.has_key(key):
		d[key] = []
	d[key].append(value)
# {'a': [1, 3, 5], 'b': [2]}
```
使用collections模块中的defaultdict可以使代码更整洁：
```python
from collections import defaultdict
d =  defaultdict(list)
for key,value  in pairs:
	d[key].append(value)
```
##### 1.7 使字典有序
如果想要创建一个字典并且想要在迭代或序列化时控制元素的顺序时，可以使用collections模块中的OrderedDict，它能够在迭代时严格维持元素插入的顺序：
```python
from collections import OrderedDict
d = OrderedDict()
d['foo'] = 1
d['bar'] = 2
d['spam'] = 3
d['grok'] = 4
for key in d:
    print(key,d[key])
# ('foo',1) ('bar',2) ('spam',3) ('grok',4)
```
OrderedDict在内部维持了一个双向链表，所以OrderedDict的大小是普通大小的两倍多
