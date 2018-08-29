# -*- coding:utf-8 -*-

def main():
	def sum_num(num):
		ret = 0
		for x in str(num):
			ret += int(x)
		return ret

	max_result = 0
	for a in xrange(0,101):
		for b in xrange(0,101):
			tmp = sum_num(a ** b)
			if tmp > max_result:
				max_result = tmp
	print max_result
if __name__ == '__main__':
	main()