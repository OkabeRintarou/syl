

### 检索数据

###### 检索单个列  
```sql
select  dept_no from departments;

```
###### 检索多个列
从一个表中检索多个列，必须在SELECT关键字后给出多个列名，列名之间必须以逗号分隔
```sql
select dept_no,dept_name from departments;
```
###### 检索所有列
给定一个通配符（*）则返回表中所有列
```sql
select * from departments;
```
### 排序检索数据
**子句：**SQL语句由子句构成，有些子句是必需的，而有的可选。一个子句通常由一个关键字加上所提供的数据组成
，例如SELECT语句的FROM子句。为了排序SELECT检索出的数据可使用ORDER BY子句:取一个或多个列的名字，据此对输出进行排序
```sql
select dept_name from departments order by dept_no;
```