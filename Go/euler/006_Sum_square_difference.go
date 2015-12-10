package main

import "fmt"

func main(){
	const upper = 100
	var sum1,sum2 int
	sum1 = 0
	sum2 = 0

	for i := 1; i <= upper;i++{
		sum1 += (i * i)
		sum2 += i
	}

	sum2 *= sum2
	fmt.Println(sum2 - sum1)
}
