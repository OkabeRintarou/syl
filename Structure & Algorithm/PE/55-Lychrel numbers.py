# -*- coding:utf-8 -*-
def main():
	def is_palindromic(num):
		return str(num) == str(num)[::-1]
	def palindromic(num):
		return int(str(num)[::-1])
	ITERATIONS = 50
	TEN_THOUSAND = 10000
	count  = 0
	
	for num in xrange(1,TEN_THOUSAND + 1):
		iterations = 0
		while iterations < ITERATIONS:
			num += palindromic(num)
			if is_palindromic(num):
				break
			iterations += 1
		if is_palindromic(num) == False:
			count += 1
	print count

if __name__ == '__main__':
	main()
# Answer:249
