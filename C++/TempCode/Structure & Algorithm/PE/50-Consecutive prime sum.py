# -*- coding:utf-8 -*-

"""
The prime 41, can be written as the sum of six consecutive primes:

41 = 2 + 3 + 5 + 7 + 11 + 13
This is the longest sum of consecutive primes that adds to a prime below 
one-hundred.

The longest sum of consecutive primes below one-thousand that adds to a prime,
contains 21 terms, and is equal to 953.

Which prime, below one-million, can be written as the sum of the most 
consecutive primes?

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

if __name__ == '__main__':
	ONE_MILLION = 1000000
	primes = sieveprimes(range(2,ONE_MILLION + 1))
	primes_set = set(primes)
	max_len = 0
	max_sum = 0
	start = 0

	for i in xrange(0,len(primes)):
		if ONE_MILLION - i < max_len:
			break
		the_sum = 0
		for j in primes[i:]:
			the_sum += j
			if the_sum > ONE_MILLION:
				the_sum -= j
				break
		index = primes.index(j) - 1
		while the_sum not in primes_set:
			the_sum -= primes[index]
			index -= 1
		if index - i > max_len:
			max_len = index - i
			max_sum = the_sum
			start = i
			print max_sum,start,max_len
	print "max_sum = ",max_sum
	print "start = ",start
	print "end   = ",max_len
# Answer:997651
