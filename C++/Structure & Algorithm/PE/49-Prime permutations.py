# -*- coding:utf-8 -*-

"""
The arithmetic sequence, 1487, 4817, 8147, in which each of the terms
increases by 3330, is unusual in two ways: (i) each of the three terms 
are prime, and, (ii) each of the 4-digit numbers are permutations of one
another.

There are no arithmetic sequences made up of three 1-, 2-, or 3-digit primes,
 exhibiting this property, but there is one other 4-digit increasing sequence.

What 12-digit number do you form by concatenating the three terms in this 
sequence?

"""

from itertools import combinations
from itertools import permutations
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
	ls = [int(i) for i in str(num)]
	ls.sort()

	return combinations([i[0] * 1000 + i[1] * 100 + i[2] * 10 + i[3] 
			for i in permutations(ls,4)
			if i[3] % 2 != 0 and i[0] * 1000 + i[1] * 100 + i[2] * 10 + i[3] > 1000],3)

primes = [x for x in sieveprimes(range(2,10000)) if x > 1000]


if __name__ == '__main__':
	ret = []
	for prime in primes:
		for i in split_number(prime):
			if i[2] - i[1] == i[1] - i[0] != 0:
				if i[0] in primes and i[1] in primes and i[2] in primes:
					if [i[0],i[1],i[2]] not in ret:
						ret.append([i[0],i[1],i[2]])
	print ret
# Answer:296962999629
	