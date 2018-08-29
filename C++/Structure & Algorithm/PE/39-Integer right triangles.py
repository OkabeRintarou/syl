# -*- coding:utf-8 -*-

"""
If p is the perimeter of a right angle triangle with integral length sides, {a,b,c}, there are exactly three solutions for p = 120.

{20,48,52}, {24,45,51}, {30,40,50}

For which value of p ≤ 1000, is the number of solutions maximised?

"""
import time

if __name__ == "__main__":
	start =  time.time()
	squ = set([i ** 2 for i in range(1,1000)])
	L = []
	for a in range(1,1000):
		for b in range(1,1000):
			c2 =  a ** 2 + b ** 2
			if a < b and c2 in squ:
				c = (int)(c2 ** 0.5)
				if a + b + c <= 1000:
					L.append(a + b + c)


	list_count = []
	for i in L:
		list_count.append(L.count(i))
	max_num = max(list_count)
	print max_num
	for i in L:
		if L.count(i) == max_num:
			print  i
			break
	print time.time() - start
# Answer : 840