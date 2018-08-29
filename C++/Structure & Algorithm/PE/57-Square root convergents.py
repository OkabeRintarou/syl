# -*- coding:utf-8 -*-
from fractions import Fraction

frac ={1:Fraction(3,2)}

def main():
	count = 0
	for i in xrange(2,1001):	
		frac[i] = 1 + Fraction(1,1+frac[i-1])
		if len(str(frac[i].numerator)) > len(str(frac[i].denominator)):
			count += 1
	print count
if __name__ == '__main__':
	main()
# Answer:153
