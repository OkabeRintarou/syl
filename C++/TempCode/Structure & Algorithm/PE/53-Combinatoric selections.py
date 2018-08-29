# -*- coding:utf-8 -*-

"""
There are exactly ten ways of selecting three from five, 12345:
123, 124, 125, 134, 135, 145, 234, 235, 245, and 345
In combinatorics, we use the notation, 5C3 = 10.
It is not until n = 23, that a value exceeds one-million: 23C10 = 1144066.
How many, not necessarily distinct, values of  nCr, for 1 ≤ n ≤ 100, 
are greater than one-million?
"""

def main():
	from time import time
	start =  time()
	factorial = [1,1]
	for i in xrange(2,101):
		factorial.append(factorial[i-1] * i)
	count = 0
	ONE_MILLION = 1000000
	for n in xrange(1,101):
		for r in xrange(0,n+1):
			if factorial[n] / (factorial[r] * factorial[n-r]) > ONE_MILLION:
				count += 1
	print time() - start
	print count
if __name__ == '__main__':
	main()
# Answer : 4075
