package main

import "fmt"

func main(){

	sum := 0
	vec := make([]int,1000)
	idx := 0
	for i:=1;i < 1000;i++{
		if i % 3 == 0 || i % 5 == 0{
			vec[idx] = i
			idx += 1
		}
	}
	for _,v := range vec{
		sum += v
	}
	fmt.Println(sum)
}
