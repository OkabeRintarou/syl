package main

import "fmt"

const one_million = 1000000

func solution1(){

	var longest int = 0
	var item int = 0

	for i := 1; i < one_million;i++{
		var count int = 0
		n := uint64(i)
		for n != 1{
			count++
			if count > longest{
				longest = count
				item = i
			}

			if n % 2 == 0{
				n /= 2
			}else{
				n = n * 3 + 1
			}
		}

	}
	fmt.Println(item)
}

func solution2(){
	arr := make([]int,1000001)
	var item int
	var longest int = 0

	for i := 1; i < one_million;i++{
		var count int = 0
		n := uint64(i)

		for n != 1{
			if  n < one_million && arr[n] != 0{
				arr[i] = arr[n] + 1	+ count
				if arr[i] > longest{
					longest = arr[i]
					item = i
					break
				}
			}
			count++
			if count > longest{
				longest = count
				item = i
			}

			if n % 2 == 0 {
				n /= 2
			}else{
				n = n * 3 + 1
			}
		}
	}
	
	fmt.Println(item)
}

func main(){
	solution1()
	solution2()
}
