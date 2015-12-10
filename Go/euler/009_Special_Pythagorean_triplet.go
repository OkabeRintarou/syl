package main

import "fmt"

func main(){
	for n:=1;n < 500;n++{
		for m:= n + 1;m<500;m++{
			a := m * m - n * n
			b := 2 * m * n
			c := m * m + n * n
			if a + b + c == 1000{
				fmt.Println(a,b,c,a * b * c)
				break
			}
		}
	}
}
