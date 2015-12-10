package main

import "fmt"

func main(){
	const four_million uint = 4000000
	var a,b uint
	a,b = 1,2
	sum := b
	for b < four_million{
		a,b = b,a+b
		if b & 1 == 0{
			sum += b
		}
	}
	fmt.Println(sum)
}
