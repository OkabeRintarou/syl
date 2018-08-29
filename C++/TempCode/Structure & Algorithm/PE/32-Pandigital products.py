# -*- coding:utf-8 -*-
"""
We shall say that an n-digit number is pandigital if it makes use of all the digits 1 to n exactly
nce; for example, the 5-digit number, 15234, is 1 through 5 pandigital.The product 7254 is unusual,
as the identity, 39 × 186 = 7254, containing multiplicand, multiplier, and product is 1 through 9 
pandigital.ind the sum of all products whose multiplicand/multiplier/product identity can be 
written as a 1 through 9 pandigital.

"""
compare_list = ['1','2','3','4','5','6','7','8','9']
pandigital_list = []

def int_to_list(a):
	lst = []
	for i,ch in enumerate(str(a)):
		lst.append(ch)
	return lst

def is_pandigital(a,b,c):
	lst = []
	lst += int_to_list(a)
	lst += int_to_list(b)
	lst += int_to_list(c)

	lst.sort()

	return lst == compare_list

def get_sum_of_pandigital():
	all_sum = []


	for a in range(1,10):
		for b in range(1000,10000):
			c = a * b
			if(is_pandigital(a,b,c)):
				if c not in all_sum:
					all_sum.append(c)
		
	for a in range(10,100):
		for b in range(100,1000):
			c =  a * b
			if(is_pandigital(a,b,c)):
				if c not in all_sum:
					all_sum.append(c)
	return all_sum

if __name__ == "__main__":
	result = get_sum_of_pandigital()
	print result
	print sum(result)

# [6952, 7852, 5796, 5346, 4396, 7254, 7632]
# Answer :45228
