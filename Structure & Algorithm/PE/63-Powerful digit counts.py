# -*- coding:utf-8 -*-
def main():
	count  = 0
	for n in xrange(1,1000):
		for x in xrange(1,11):
			if len(str(x ** n)) == n:
				count += 1
	print count
if __name__ == '__main__':
	main()
# Answer:49
