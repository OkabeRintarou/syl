package main

import "fmt"
import "os"
import "io/ioutil"
import "strings"
import "strconv"

func main(){
	const filename = "013_Large_sum.txt"
	file,err := os.Open(filename)
	if err != nil{
		panic(err)
	}

	defer file.Close()
	data,err:= ioutil.ReadAll(file)
	if err != nil{
		panic(err)
	}

	strarr := strings.Split(string(data[:len(data)]),"\n")[:100]

	var arr = make([]byte,50)
	var sum = 0
	for i :=49;i >= 0;i--{
		for j := 0; j < 100;j++{
			num,err := strconv.Atoi(strarr[j][i:i+1])
			if err != nil{
				panic(err)
			}
			sum += num
		}

		rem := sum % 10
		sum /= 10
		arr[i] = byte(strconv.Itoa(rem)[0])
	}
	var result string
	if sum > 0{
		result = (strconv.Itoa(sum) + string(arr[:len(arr)]))[0:10]
	}

	fmt.Println(result)
}
