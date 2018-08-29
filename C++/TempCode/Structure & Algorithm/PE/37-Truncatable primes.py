# -*- coding:utf-8 -*-
"""

The number 3797 has an interesting property. Being prime itself, it is possible to continuously 
remove digits from left to right, and remain prime at each stage: 3797, 797, 97, and 7. Similarly 
we can work from right to left: 3797, 379, 37, and 3.

Find the sum of the only eleven primes that are both truncatable from left to right and right to left.

NOTE: 2, 3, 5, and 7 are not considered to be truncatable primes.

"""
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

def split_number(num):
	L = [num]
	string = str(num)
	for i in range(1,len(string)):
		L.append(int(string[i:]))
		L.append(int(string[0:i]))
	return L

import time
start = time.time()
primes = findprime(range(2,1000000))

def is_truncatable(num):
    if num < 11:
        return False
    for i in split_number(num):
        if i not in primes:
            return False
    return True

def  program():
    L = []
    for i in primes:
        if is_truncatable(i):
            L.append(i)
    return L


if __name__ == "__main__":
    print sum(program())
    print time.time() - start
# Answer:748317
