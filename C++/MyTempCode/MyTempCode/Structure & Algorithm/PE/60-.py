# -*- coding:utf-8 -*-

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

ONE_HUNDRED_THROUSAND = 100
ONE_MILLION = 1000000
primes_list = sieveprimes(range(2,ONE_HUNDRED_THROUSAND))
primes_set = sieveprimes(range(2,ONE_MILLION))

def main():
	from itertools import combinations
	from itertools import permutations
	def concatenate(x):
		return [int(str(i[0]) + str(i[1])) for i in permutations(x,2)]
	
if __name__ == '__main__':
	main()