# -*- coding:utf-8 -*-

from __future__ import division

def sieveprimes(target):
	i = 0
	last = target[-1]
	while target[i] * 2  <= last:
		step = target[i]
		index = i + step
		while step == 0:
			step = target[i]
			index = i + step
			i += 1		
		while index < len(target):
			target[index] = 0
			index += step
		i += 1	
	target = list(set(target))[1:]
	target.sort()
	return target

ONE_MILLION = 1000000
primes_set = set(sieveprimes(range(2,ONE_MILLION)))

def is_prime(num):
	from math import sqrt
	if num < 2:return False
	elif num == 2:return True
	if num % 2 == 0:
		return False
	for x in xrange(3,int(sqrt(num)) + 1,2):
		if num % x == 0:
			return False
	return True
def main():
	def gen(n): # n >= 3
		yield n ** 2 - 3 * (n - 1)
		yield n ** 2 - 2 * (n - 1)
		yield n ** 2 - (n - 1)
	all = 5
	fit = 3
	
	for n in xrange(5,100001,2):
		all += 4
		for x in gen(n):
			if x < ONE_MILLION and x in primes_set:
				fit += 1
			elif x > ONE_MILLION and is_prime(x):
				fit += 1
		if fit / all < 0.1:
			print n;break 
		
if __name__ == '__main__':
	from time import time
	start = time()
	main()
	print time() - start
# Answer:26241
