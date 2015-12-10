package main

import "fmt"

func ispalindrome(num uint64)bool{
	var rev uint64
	var org uint64
	rev = 0
	org = num
	for ;num > 0; num /= 10{
		rev = rev * 10 + num % 10
	}

	return rev == org
}

func main(){

	max := uint64(0)

	for i := 999; i > 99;i--{
		for j := 999;j > 99;j--{
			num := uint64(i * j)
			if ispalindrome(num) && num > max {
				max = num
			}
		}
	}
	fmt.Println(max)
}
