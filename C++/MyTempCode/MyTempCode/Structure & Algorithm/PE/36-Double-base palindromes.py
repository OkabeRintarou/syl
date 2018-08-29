# -*- coding:utf-8 -*-
"""
The decimal number, 585 = 10010010012 (binary), is palindromic in both bases.

Find the sum of all numbers, less than one million, which are palindromic in base 10 and base 2.

(Please note that the palindromic number, in either base, may not include leading zeros.)

"""
def is_palindromic(num):
	num_string =  str(num)
	return num_string == num_string[::-1]

def is_bin_palindromic(num):
	num_string = bin(num)[2:]
	return num_string == num_string[::-1]

def program():
	the_sum = 0
	one_million = 1000000
	for itr in range(1,one_million):
		if is_palindromic(itr) and is_bin_palindromic(itr):
			the_sum += itr
	return the_sum

if __name__ == "__main__":
	print program()
# Answer: 872187
