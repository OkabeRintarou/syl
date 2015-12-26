package main

import "fmt"
import "log"

var table map[int]int = map[int]int{
			1:len("one"),
			2:len("two"),
			3:len("three"),
			4:len("four"),
			5:len("five"),
			6:len("six"),
			7:len("seven"),
			8:len("eight"),
			9:len("nine"),
			10:len("ten"),
			11:len("eleven"),
			12:len("twelve"),
			13:len("thirteen"),
			14:len("fourteen"),
			15:len("fifteen"),
			16:len("sixteen"),
			17:len("seventeen"),
			18:len("eighteen"),
			19:len("nineteen"),
			20:len("twenty"),
			30:len("thirty"),
			40:len("forty"),
			50:len("fifty"),
			60:len("sixty"),
			70:len("seventy"),
			80:len("eighty"),
			90:len("ninety"),
}
const hundred_len = len("hundred")
const and_len = len("and")
const one_thousand_len = len("onethousand")


func count_1_to_99()int{
	var count int = 0
	for i := 1; i <= 20;i++{
		count += table[i]
	}

	for i := 21; i < 100;i++{
		var mod int = i % 10
		var tenths int = i - mod
		count += (table[tenths] + table[mod])
	}
	return count
}

func get_1_to_100(num int)int{
	if num >= 1 && num <= 20{
		return table[num]
	}else if(num >= 21 && num < 100){
		var mod int =  num % 10
		var tenths  int = num  - mod
		return table[tenths] + table[mod]
	}else{
		log.Fatal("Never arrive here!")
	}
	return -1
}

func main(){
	var count int = 0
	count += count_1_to_99()
	for i := 1; i < 10;i++{
		count += (table[i] + hundred_len)
	}
	// 101 - 999,not include 100,200 etc
	for i := 1; i < 10;i++{
		count += ((table[i] + hundred_len + and_len) * 99) + count_1_to_99()
	}
	count += one_thousand_len
	fmt.Println(count)
}
