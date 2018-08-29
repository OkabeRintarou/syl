# -*- coding:utf-8 -*-

"""
The number, 1406357289, is a 0 to 9 pandigital number because it is made up of each of the
digits 0 to 9 in some order, but it also has a rather interesting sub-string divisibility 
property.

Let d1 be the 1st digit, d2 be the 2nd digit, and so on. In this way, we note the following:

d2d3d4=406 is divisible by 2
d3d4d5=063 is divisible by 3
d4d5d6=635 is divisible by 5
d5d6d7=357 is divisible by 7
d6d7d8=572 is divisible by 11
d7d8d9=728 is divisible by 13
d8d9d10=289 is divisible by 17
Find the sum of all 0 to 9 pandigital numbers with this property.

"""

from itertools import permutations

def get_i_j_k(t,i,j,k):
	return t[i-1] * 100 + t[j-1] * 10 + t[k-1]

def get_num(t):
	count = 0
	for i,v in enumerate(t):
		count += ( v * (10 ** (9 - i)))
	return count

def program():
	count = 0
	for i in permutations(range(10),10):
		if i[0] != 0 and i[3] % 2 == 0 and i[5] % 5 == 0:
			if get_i_j_k(i,3,4,5) % 3 ==0 and get_i_j_k(i,5,6,7) % 7 == 0 and get_i_j_k(i,6,7,8) % 11 == 0 and get_i_j_k(i,7,8,9) % 13 == 0  and get_i_j_k(i,8,9,10) % 17 == 0:
				count += get_num(i)
	return count

if __name__ == "__main__":
	print program()
	
# Answer : 16695334890
