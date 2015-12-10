package main

import "fmt"

func doPrime(num int)map[int]int{
	m := make(map[int]int)

	if num < 2{
		return m
	}

	factor := 2

	for num > 1{
		if num % factor == 0{
			m[factor] += 1
			num /= factor
			for num % factor == 0{
				m[factor] += 1
				num /= factor
			}
		}
		factor += 1
	}

	return m
}

func getProduct(m map[int]int)int{
	prod := 1
	for k,v := range(m){
		for i := 0; i < v;i++{
			prod *= k
		}
	}
	return prod
}

func main(){

	table := map[int]int{
						2:1,3:1,5:1,
						7:1,11:1,13:1,
						17:1,19:1,
						}

	for i := 2; i <= 20;i++{
		m := doPrime(i)
		for k,v := range(m){
			if v > table[k]{
				table[k] = v
			}
		}
	}

	fmt.Println(getProduct(table))
}
