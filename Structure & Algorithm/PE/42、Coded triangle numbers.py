# -*- coding:utf-8 -*-

"""
The nth term of the sequence of triangle numbers is given by, tn = ½n(n+1); so the first 
ten triangle numbers are:

1, 3, 6, 10, 15, 21, 28, 36, 45, 55, ...

By converting each letter in a word to a number corresponding to its alphabetical position 
and adding these values we form a word value. For example, the word value for SKY is 
19 + 11 + 25 = 55 = t10. If the word value is a triangle number then we shall call the 
word a triangle word.

Using words.txt (right click and 'Save Link/Target As...'), a 16K text file containing 
nearly two-thousand common English words, how many are triangle words?

"""

def get_list_from_file(f):
	text = open(f).read()
	return text.split(',')

triangle_number_list = [int(1.0 / 2 * n * (n + 1 ) ) for n in range(1,16 * 26 + 1)]
alpha_dict = {'A':1,'B':2,'C':3,'D':4,'E':5,'F':6,'G':7,
			  'H':8,'I':9,'J':10,'K':11,'L':12,'M':13,'N':14,
			  'O':15,'P':16,'Q':17,'R':18,'S':19,'T':20,'U':21,
			  'V':22,'W':23,'X':24,'Y':25,'Z':26}
def is_triangle_string(s):
	num = 0
	for c in s:
		if c != '\"':
			num += alpha_dict[c]
	if num in triangle_number_list:
		return True
	return False

# text文件中最大长度的字母为16个字节
if __name__ == "__main__":
	filename = 'p042_words.txt'
	
	count = 0
	for s in get_list_from_file(filename):
		if is_triangle_string(s):
			count += 1
	print count
# Answer:162
