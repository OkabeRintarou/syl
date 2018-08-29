# -*- coding:utf-8 -*-

"""
By replacing the 1st digit of the 2-digit number *3, it turns out that six of
 the nine possible values: 13, 23, 43, 53, 73, and 83, are all prime.

By replacing the 3rd and 4th digits of 56**3 with the same digit, this 5-digit
number is the first example having seven primes among the ten generated numbers,
yielding the family: 56003, 56113, 56333, 56443, 56663, 56773, and 56993. 
Consequently 56003, being the first member of this family, is the smallest
prime with this property.

Find the smallest prime which, by replacing part of the number (not necessarily 
adjacent digits) with the same digit, is part of an eight prime value family.

"""

def sieveprimes(target):
	"""target:以质数开头连续的自然数区间,返回区间内所有质数的列表"""
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

def split_number(num):
	return [ int(x) for x in str(num)]

def fit(num):
	number_list = split_number(num)
	return set([x for x in number_list 
	if (x == 0 or x == 1 or x == 2) and number_list.count(x) >= 2])

primes = set(sieveprimes(range(2,1000000)))

if __name__ == '__main__':
	from time import time
	start = time()
	quit = False
	for num in xrange(100000,1000000):
		fit_set = fit(num)

		test = []
		if fit_set:
			
			for x in fit_set:
				count = 0
				for i in xrange(0,10):
					if i != x:
						if num in primes:
							new_num = int(str(num).replace(str(x),str(i)))
							if new_num in primes and new_num > 100000:
								test.append(new_num)
								count += 1
				if count >= 7:
					print num,test
					quit = True
		if quit:
			break
	print time() - start
# Answer:121313 
# The eight primes are [,121313,222323, 323333, 424343, 525353, 626363, 828383, 929393]					
