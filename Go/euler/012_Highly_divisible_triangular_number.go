package main

import "fmt"

func primes(num uint64)map[uint64]uint64{
	var rc map[uint64]uint64

	rc = make(map[uint64]uint64)

	for i := uint64(2); i <= num;i++{
		if num % i == 0{
			var count uint64 = 1
			num /= i
			for;num % i == 0;{
				count++
				num /= i
			}
			rc[i] = count
		}
	}

	return rc
}

func main(){

	sum := uint64(0)

	for i := 1; ;i++{
		sum += uint64(i)

		rc := primes(sum)

		prod := uint64(1)

		for _,v := range(rc){
			prod *= (v + 1)
		}
		if prod > 500{
			fmt.Println(sum)
			break
		}
	}
}
