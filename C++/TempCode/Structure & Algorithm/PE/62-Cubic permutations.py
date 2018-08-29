# -*- coding:utf-8 -*-

def main():

	def key_func(num):
		num =  list(str(num))
		num.sort()
		return tuple(num)

	from collections import defaultdict
	items_list =  [ i * i *i for i in xrange(1,1000000)]
	hash_table = defaultdict(list)
	
	for x in items_list:
		key = key_func(x)
		hash_table[key].append(x)
		if len(hash_table[key] ) == 5:
			print min(hash_table[key])
			break
      
if __name__ == '__main__':
	main()
# Answer:127035954683
