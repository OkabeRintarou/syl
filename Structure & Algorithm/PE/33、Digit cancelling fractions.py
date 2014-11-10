# -*- coding:utf-8 -*-

"""
  The fraction 49/98 is a curious fraction, as an inexperienced mathematician in attempting to 
simplify it may incorrectly believe that 49/98 = 4/8, which is correct, is obtained by cancelling
the 9s.

  We shall consider fractions like, 30/50 = 3/5, to be trivial examples.

  There are exactly four non-trivial examples of this type of fraction, less than one in value, 
 and containing two digits in the numerator and denominator.

  If the product of these four fractions is given in its lowest common terms, find the value of 
the denominator.
"""
from fractions import Fraction

exclusion_list = [(1,1,1),(2,2,2),(3,3,3),(4,4,4),(5,5,5),(6,6,6),(7,7,7),(8,8,8),(9,9,9)]

def get_abc(numerator , denominator):
	a = numerator / 10
	b = numerator - numerator / 10 * 10
	if denominator / 10 != b:
		return (0,0,0)
	else:
		return (a,b,denominator - denominator / 10 * 10)

def program():
	result = Fraction(1,1)
	
	for numerator in range(10,100):
		for denominator in range(10,100):
			a,b,c = get_abc(numerator,denominator)
			if c != 0 and ((a,b,c) not in exclusion_list) and  Fraction(numerator,denominator) == Fraction(a,c):
				result *= Fraction(a,c)
				print a,b,c
	return result

if __name__ == "__main__":
	print program()

# for fraction: 16/64 19/95 26/65 49/98
# Answer:100
	