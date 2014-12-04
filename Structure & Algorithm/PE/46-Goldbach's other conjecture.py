# -*- coding:utf-8 -*-

"""
It was proposed by Christian Goldbach that every odd composite number can be written 
as the sum of a prime and twice a square.

9 = 7 + 2×12
15 = 7 + 2×22
21 = 3 + 2×32
25 = 7 + 2×32
27 = 19 + 2×22
33 = 31 + 2×12

It turns out that the conjecture was false.

What is the smallest odd composite that cannot be written as the sum of a prime and 
twice a square?

"""
def sieveprimes(target):
	"""target:以质数开头连续的自然数区间,然会区间内所有质数的列表"""
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

primes = set(sieveprimes(range(2,1000000)))

def is_can(num):
	from math import sqrt
	for x in xrange(0,int(sqrt(num/2) + 1)):
		if ( num - 2 * x * x  )  in primes:
			return True
	return False

if __name__ == "__main__":
	for x in xrange(3,100000,2):
		if is_can(x) == False:
			print x
# Answer:5777		
