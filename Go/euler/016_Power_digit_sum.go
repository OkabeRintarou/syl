package main

import "fmt"

func multiply2(arr []int){
	for i := len(arr) - 1; i >= 0;i--{
		arr[i] = arr[i] << 1
	}

	var num,mod int
	for i := len(arr) - 1;i >= 0;i--{
		if arr[i] >= 10{
			num = arr[i] / 10
			mod = arr[i] % 10
			arr[i] = mod
			if i - 1 >= 0{
				arr[i - 1] += num
			}
		}

	}
}

func solution(length int){
	arr := make([]int,length)
	arr[length - 1] = 1
	for i := 1; i <= length;i++{
		multiply2(arr)
	}

	var count = 0
	for i := 0; i < length;i++{
		count += arr[i]
	}
	fmt.Println(count)
}

func main(){
	solution(1000)
}
