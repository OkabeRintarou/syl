package main

import "fmt"

func is_leap(year int)bool{
	return  (year  % 4 == 0 && year  % 100 != 0) || (year % 400 == 0)
}

func main(){
	var month_day = [13]int{0,31,28,31,30,31,30,31,31,30,31,30,31}
	var day uint64  = 0
	var count uint64  = 0
	for year := 1900;year <= 2000;year++{
		for month := 1; month <= 12;month++{
			if (day + 1) % 7 == 0{
				if (year != 1900){
					count++
				}
			}
			if month != 2{
				day += uint64(month_day[month])
			}else{
				if is_leap(year){
					day += 29
				}else{
					day += 28
				}
			}
		}
	}
	fmt.Println(count)
}
