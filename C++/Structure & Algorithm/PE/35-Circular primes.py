# -*- coding:utf-8 -*-
"""

The number, 197, is called a circular prime because all rotations of the digits: 197, 971, and 719, are themselves prime.

There are thirteen such primes below 100: 2, 3, 5, 7, 11, 13, 17, 31, 37, 71, 73, 79, and 97.

How many circular primes are there below one million?


"""

# 埃拉托斯特尼筛法
    
def findprime(L):
    i = 0
    count = 0
    while L[i] ** 2 < L[-1]:
        for j in range(i + 1,len(L)):
            if L[j] % L[i] == 0:
                L[j] = 0
                count += 1
        L.sort()
        L = L[count:]
        count = 0
        i += 1
    return set(L)

def get_rotations(num):
	L = [num]
	stri = str(num)
	for i in range(1,len(stri)):
		L.append(int(stri[i:] + stri[0:i]))
	return list(set(L))

import time

def program():
	one_million = 1000000
	count = 0
	new = []
	S =findprime(range(2,one_million))

	for i in S:
		if i in new:
			pass
		else:
			all_in = True
			rotations = get_rotations(i)
			
			for j in rotations:
				if j not in S:
					all_in = False
					break
			if all_in:
				new.append(i)
				for j in rotations:
					new.append(j)

	print len(list(set(new)))

if __name__ == "__main__":
	
	start = time.time()
	program()
	print time.time() - start
# Answer : 55
