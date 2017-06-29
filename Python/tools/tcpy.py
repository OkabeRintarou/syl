#!/usr/bin/python3
# -*- coding:utf-8 -*-

import os,sys
import argparse

__write_string__ = "#!/usr/bin/python3\n# -*- coding:utf-8 -*-\n"

def write_to_file(filename):
	with open(filename,'w') as f:
		f.write(__write_string__)

if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('filename',help = "touch new file")
	filename = parser.parse_args().filename

	files = os.listdir(".")
	for file in files:
		if file == filename:
			print('{} already exist.overwrite?(y/n)'.format(filename),end = '-->')
			yes_or_no = input()
			while True:
				y_start = (yes_or_no.startswith('y') or yes_or_no.startswith('Y'))
				n_start = (yes_or_no.startswith('n') or yes_or_no.startswith('N'))
				if y_start:
					write_to_file(filename)
					sys.exit(0)
				elif n_start:
					print('exit')
					sys.exit(1)
					break
				else:
					print('{} already exist.overwrite?(y/n)'.format(filename),end = '-->')
					yes_or_no = input()
		
	write_to_file(filename)
	sys.exit(0)
