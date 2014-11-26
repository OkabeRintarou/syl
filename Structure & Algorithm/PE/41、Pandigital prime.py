# -*- coding:utf-8 -*-

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

def split_num(num):
	s = str(num)
	r = [x for x in s]
	r.sort()
	return r

if __name__ == "__main__":
	from time import time
	start = time()
	primes = sieveprimes(range(2,7654322))
	primes.reverse()
	t = ['1','2','3','4','5','6','7'],['1','2','3','4']
	for i in range(0,len(primes)):
		if split_num(primes[i]) in t:
			print primes[i]
			break
	print time() - start
# Answer: 7652413
