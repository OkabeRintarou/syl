# -*- coding:utf-8 -*-


def decrypt(security):
	from itertools import permutations
	lower = 'abcdefghijklmnopqrstuvwxyz'
	for x in permutations(lower,3):
		index = 0
		ret = ''
		for num in security:
			ret += (chr(num ^ ord(x[index])))
			index += 1
			if index > 2:
				index = 0
		yield ret.split(' ')


def main():
	with open('p059_cipher.txt','r') as f:
		input = [int(x) for x in f.read().split(',')]
		count  = 0
		for words in decrypt(input):
			for word in words:
				if '~'  not in word and '{' not in word and '@' not in word and '%' not in word and '`' not in word:
					print word				
			count += 1
		print count

if __name__ == "__main__":
	main()
