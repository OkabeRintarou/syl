# -*- coding:utf-8 -*-

"""
The first two consecutive numbers to have two distinct prime factors are:

14 = 2 × 7
15 = 3 × 5

The first three consecutive numbers to have three distinct prime factors are:

644 = 2² × 7 × 23
645 = 3 × 5 × 43
646 = 2 × 17 × 19.

Find the first four consecutive integers to have four distinct prime factors. 
What is the first of these numbers?

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

primes = sieveprimes(range(2,100000))

def divided(denominator,member):
	return (denominator / member) * member == denominator

def output(num):
	ls = []
	i = 0
	while num != primes[i] :
		if divided(num,primes[i]) == False:
			i += 1
		else:
			ls.append(primes[i])
			num /= primes[i]
	ls.append(primes[i])

	return ls
		
if __name__ == "__main__":
	ls = []
	for x in xrange(100,1000000):
		a = set(output(x))
		b = set(output(x + 1))
		c = set(output(x + 2))
		d = set(output(x + 3))
		if len(a) == len(b) == len(c) == len(d) == 4:
			print x
			break
# Answer :134043		
