
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
```python3
record = ('Dave','dave@sample.com','773-555-1212','847-555-1212')
name,email,*phone_numbers = record
# name = 'Dave',email = 'dave@sample.com',phone_numbers = ['773-555-1212','847-555-1212']
```
