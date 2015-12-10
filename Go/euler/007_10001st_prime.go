package main

import "fmt"

func SievePrime(upper uint64)(primes []uint64,size int){
	flag := make([]bool,upper + 1)
	var i,j uint64
	for i = 0; i <= upper;i++{
		flag[i] = true
	}
	for i = 2; i <= upper / 2;i++{
		if flag[i]{
			for j = i << 1; j <= upper;j += i{
				flag[j] = false
			}
		}
	}
	
	primes =  make([]uint64,upper)
	size = 0
	for i = 2; i < upper;i++{
		if flag[i]{
			primes[size] = i
			size++
		}
	}

	return primes,size
}

func main(){
	primes,_  := SievePrime(120000)
	fmt.Println(primes[10000])
}
