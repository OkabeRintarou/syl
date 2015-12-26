package main

import "fmt"

func solution1(length int){
	arr := make([][]uint64,length + 1)
	var i,j int
	for i = 0; i <= length;i++{
		arr[i] = make([]uint64,length + 1)
	}

	for i = 0; i <=length;i++{
		arr[length][i] = 1
		arr[i][length] = 1
	}

	for i = length - 1; i >= 0; i--{
		for j = length - 1; j >= 0; j--{
			if i + 1 <= length{
				arr[i][j] += arr[i + 1][j]
			}
			if j + 1 <= length{
				arr[i][j] += arr[i][j + 1]
			}
		}
	}

	fmt.Println(arr[0][0])
}

func main(){
	solution1(20)
}
