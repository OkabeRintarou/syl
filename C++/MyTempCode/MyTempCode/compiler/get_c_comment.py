# -*- coding:utf-8 -*-

FILENAME = "bar.txt"

def get_c_comment(filename):
	with open(filename,'r+') as f:
		read = f.read()
		in_comment = ""
		i = 0
		last_comment_begin = -1
		ret = []

		while i < len(read):
			if i + 1 < len(read) and read[i] == '/' and read[i + 1] == '*':
				i += 2
				last_comment_begin = i

				while i + 1 < len(read) and (read[i] != '*' or read[i + 1] != '/'):
					in_comment += read[i]
					i += 1
				ret.append(in_comment)
				in_comment = ""

			i += 1
	return ret

if __name__ == '__main__':
	print get_c_comment(FILENAME)
