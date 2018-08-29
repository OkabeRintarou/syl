# -*- coding:utf- -*-

"""
An irrational decimal fraction is created by concatenating the positive integers:

0.123456789101112131415161718192021...

It can be seen that the 12th digit of the fractional part is 1.

If dn represents the nth digit of the fractional part, find the value of the following expression.

d1 × d10 × d100 × d1000 × d10000 × d100000 × d1000000

"""
if __name__ == "__main__":
	L = [ str(i) for i in range(0,333333)]
	st = "".join(L)
	i = 1
	result = 1
	while  i < 1000001:
		result *= int(st[i])
		i *= 10
	print result
# Answer : 210
