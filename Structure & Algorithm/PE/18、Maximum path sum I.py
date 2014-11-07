# -*- coding;utf-8 -*-
"""
src = https://projecteuler.net/problem=18

By starting at the top of the triangle below and moving to adjacent numbers on the row below, the maximum total from top to bottom is 23.

      3
     7 4
    2 4 6
   8 5 9 3

That is, 3 + 7 + 4 + 9 = 23.

Find the maximum total from top to bottom of the triangle below:

1							75
2                         95 64
3                        17 47 82
4                       18 35 87 10
5                      20 04 82 47 65
6                     19 01 23 75 03 34
7                    88 02 77 73 07 63 67
8                  99 65 04 28 06 16 70 92
9               41 41 26 56 83 40 80 70 33
10               41 48 72 33 47 32 37 16 94 29
11             53 71 44 65 25 43 91 52 97 51 14
12            70 11 33 28 77 73 17 78 39 68 17 57
13           91 71 52 38 17 14 91 43 58 50 27 29 48
14          63 66 04 68 89 53 67 30 73 16 69 87 40 31
15         04 62 98 27 23 09 70 98 73 93 38 53 60 04 23

"""

dic = {0:[75],1:[95,64],2:[17,47,82],
		3:[18,35,87,10],4:[20,4,82,47,65],5:[19,1,23,75,3,34],
		6:[88,2,77,73,7,63,67],7:[99,65, 4, 28, 6, 16, 70, 92],
		8:[41, 41, 26, 56, 83, 40, 80, 70, 33],9:[41, 48, 72, 33, 47, 32, 37, 16, 94, 29],
		10:[53, 71, 44, 65, 25, 43, 91, 52, 97, 51, 14],11:[70, 11, 33, 28, 77, 73, 17, 78, 39, 68, 17, 57],
		12:[91, 71, 52, 38, 17, 14, 91, 43, 58, 50, 27, 29, 48],
		13:[63, 66, 04, 68, 89, 53, 67, 30, 73, 16, 69, 87, 40, 31],
		14:[4, 62, 98, 27, 23, 9, 70, 98, 73, 93, 38, 53, 60, 4, 23]}

def program18(dic):
	dic_len = len(dic)
	max_result = [[] for i in range(dic_len)]
	for i in range(dic_len):
		if i == 0:
			max_result[0].append(dic[0][0])
			#print max_result[0][0]
		else:
			for j in range(0,i + 1):
				if j == 0:	
					max_result[i].append(max_result[i-1][0] + dic[i][0])
				elif j == i:
					max_result[i].append(max_result[i-1][i-1] + dic[i][j])
				else:
					max_result[i].append(max(max_result[i-1][j-1],max_result[i-1][j]) + dic[i][j])
	
	return max(max_result[i])

if __name__ == "__main__":
	print program18(dic)
# Answer : 1074
