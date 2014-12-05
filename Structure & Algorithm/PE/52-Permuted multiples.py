# -*- coding:utf-8 -*-

"""
It can be seen that the number, 125874, and its double, 251748, contain exactly
 the same digits, but in a different order.

Find the smallest positive integer, x, such that 2x, 3x, 4x, 5x, and 6x, contain
 the same digits.

"""

def main():
	def fit(x):
		if times(x,1) == times(x,2) == times(x,3) == times(x,4) == times(x,5) == times(x,6):
			return True
		return False
	def times(num,time):
		ret = [int(x) for x in str(num * time)]
		ret.sort()
		return ret

	for x in xrange(1,10000000):
		if fit(x):
			print x
			break

if __name__ == '__main__':
	main()
# Answer:142857
