#!/usr/bin/python3
# -*- coding:utf-8 -*-

import sys
import os

def win_newline_2_linux_newline(strings):
	length = len(strings)
	new_strings = ""
	i = 0
	while i < length:
		if strings[i] == 0x0D:
			if i != length - 1 and strings[i +1] == 0x0A:
				new_strings += '\n'
		else:
			new_strings += strings[i]
		i += 1
	return new_strings

def cp_file(src_file,dest_file):
	try:
		with open(src_file) as src:
			string = src.readlines()
			write_s = ""
			for s in string:
				write_s += win_newline_2_linux_newline(s)
			with open(dest_file,"w+") as dest:
				dest.write(write_s)
	except OSError as e:
		print(e)
		return False
	except BaseException as e:
		print(e)
		return False

def do_file_or_dir(filename,depth=0):
	if os.path.isfile(filename):
		cp_file(filename,filename)
	elif os.path.isdir(filename):
		for f in os.listdir(filename):
			f = filename + '/' + f
			do_file_or_dir(f,depth + 1)


if __name__ == '__main__':
	if len(sys.argv) < 2:
		sys.stderr.write("<usage>: %s <file(or dir)-lists>\n" % sys.argv[0])
		sys.exit(1)
	for filename in sys.argv[1:]:
		filename = os.getcwd() + '/' + filename
		do_file_or_dir(filename)
	print('Converted Succeed!')

