# -*- coding:utf-8 -*-

"""
The series, 11 + 22 + 33 + ... + 1010 = 10405071317.

Find the last ten digits of the series, 11 + 22 + 33 + ... + 10001000.

"""

if __name__ == "__main__":
	the_sum = 0
	for x in xrange(1,1001):
		the_sum += (x ** x)
	print str(the_sum)[-10:]
# Answer:9110846700
# -*- coding:utf-8 -*-

"""
The series, 11 + 22 + 33 + ... + 1010 = 10405071317.

Find the last ten digits of the series, 11 + 22 + 33 + ... + 10001000.

"""

if __name__ == "__main__":
	the_sum = 0
	for x in xrange(1,1001):
		the_sum += (x ** x)
	print str(the_sum)[-10:]
# Answer:9110846700
