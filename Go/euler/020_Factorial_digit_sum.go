package main

import "fmt"

func multiplyn(arr []int,n int){
	for i := len(arr) - 1; i >= 0;i--{
		arr[i] = arr[i] * n
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


func main(){
	arr := make([]int,200)
	arr[len(arr) - 1] = 1
	for i := 2; i <= 100;i++{
		multiplyn(arr,i)
	}

	var count int  = 0
	for i := 0; i < len(arr);i++{
		count += arr[i]
	}
	fmt.Println(count)
}
