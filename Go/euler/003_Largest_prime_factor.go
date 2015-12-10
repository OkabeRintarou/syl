package main

import ("fmt";
		"math"
	   )

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
	
	ans := make([]uint64,upper)
	idx := 0
	for i = 2; i < upper;i++{
		if flag[i]{
			ans[idx] = i
			idx++
		}
	}

	return ans,idx
}



func main(){
	const upper uint64  = 600851475143

	var factor uint64
	var lastFactor uint64
	var n uint64

	n = upper
	if n % 2 == 0 {
		lastFactor = 2
		n /= 2
		for n % 2 == 0{
			n /= 2
		}
	}else{
		lastFactor = 1
	}

	factor = 3
	var maxFactor uint64
	maxFactor = uint64(math.Sqrt(float64(n)))
	for n > 1 && factor <= maxFactor{
		if n % factor == 0{
			lastFactor = factor
			n /= factor
			for n % factor == 0{
				n /= factor
			}
			maxFactor = uint64(math.Sqrt(float64(n)))
		}

		factor += 2
	}
	if n == 1{
		fmt.Println(lastFactor)
	}else{
		fmt.Println(n)
	}
}
