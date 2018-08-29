# -*- coding:utf-8 -*-




def main():
	
	
	def differentiate_cards(card):
		""" 
		High Card(0) < One Pair(1) < Two Pairs(2) < Three of a Kind(3) < 
		Straight(4) < Flush(5) < Full House(6) < Four of a Kind(7) < Straight Flush(8)
		< Royal Flush(9)
		"""
		def High_Card(numbers,colors):
			return len(set(numbers)) == 5
		def One_Pair(numbers,colors):
			return len(set(numbers)) == 4
		def Two_Pairs(numbers,colors):
			if len(set(numbers)) != 3:
				return False
			return [numbers.count(i) for i in numbers].count(2) == 4
		def Three_of_a_Kind(numbers,colors):
			if len(set(numbers)) != 3:
				return False
			for i in numbers:
				if numbers.count(i) == 3:
					return True
			return False
		def Straight(numbers,colors):
			for i in xrange(1,len(numbers)):
				if numbers[i] - numbers[i-1] != 1:
					return False
			return True
		def Flush(numbers,colors):
			return len(set(colors)) == 1
		def Full_House(numbers,colors):
			numbers_set = set(numbers)
			if len(numbers_set) != 2:
				return False
			a = numbers[0]
			b= [x for x in numbers if x != a][0]
			return (numbers.count(a) == 2 and numbers.count(b) == 3) or\
				(numbers.count(a) == 3 and numbers.count(b) == 2)
		def Four_of_a_Kind(numbers,colors):
			for i in set(numbers):
				if numbers.count(i) == 4:
					return True
			return False
		def Straight_Flush(numbers,colors):
			return Straight(numbers,colors) and Flush(numbers,colors)
		def Royal_Flush(numbers,colors):
			Royal = [10,11,12,13,14]
			return numbers == Royal and Flush(numbers,colors)

		cards = {'1':1,'2':2,'3':3,'4':4,'5':5,'6':6,'7':7,'8':8,'9':9,
		  'T':10,'t':10,'J':11,'j':11,'Q':12,'q':12,'K':13,'k':13,'A':14,'a':14}
		numbers = [cards[i[0]] for i in card]
		numbers.sort()
		colors = [i[1] for i in card]
		
		if Royal_Flush(numbers,colors):return 9
		elif Straight_Flush(numbers,colors):return 8
		elif Four_of_a_Kind(numbers,colors):return 7
		elif Full_House(numbers,colors):return 6
		elif Flush(numbers,colors):return 5
		elif Straight(numbers,colors):return 4
		elif Three_of_a_Kind(numbers,colors):return 3
		elif Two_Pairs(numbers,colors):return 2
		elif One_Pair(numbers,colors):return 1
		elif High_Card(numbers,colors):return 0

	def compare(cards1,cards2):
		
		if differentiate_cards(cards1) ==  differentiate_cards(cards2):
			print cards1,cards2
			return True

	#print differentiate_cards(['2D','2S','2S'])
	
	with open('p054_poker.txt','r') as f:
		count = 0
		while True:
			s = f.readline()
			if s == '':
				break
			else:
				cards = s.split()
				if compare(cards[0:5],cards[5:]):
					count += 1
		print count
	
if __name__ == '__main__':
	main()