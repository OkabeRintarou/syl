# -*- coding:utf-8 -*-
"""
145 is a curious number, as 1! + 4! + 5! = 1 + 24 + 120 = 145.

Find the sum of all numbers which are equal to the sum of the factorial of their digits.

Note: as 1! = 1 and 2! = 2 are not sums they are not included.

"""
fac_list = [1,1,2,6,24,120,720,5040,40320,362880]
limit = 2540160

def split_int(num):
	result = []
	for i,ch in enumerate(str(num)):
		result.append(int(ch))
	return result

def judge(num):
	the_sum = 0
	for i in split_int(num):
		the_sum += fac_list[i]
	return num == the_sum

def get_sum():
	all_sum = 0
	for i in range(3,limit):
		if judge(i):
			all_sum += i
	return all_sum

if __name__ == "__main__":
	print get_sum()
# Answer : 40730
