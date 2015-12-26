package main

import "fmt"
import "os"
import "io"
import "bufio"
//import "io/ioutil"
import "strings"
import "strconv"

func main(){
	const filename string = "018_Maximum_path_sum_1.txt"
	file,err := os.Open(filename)
	if err != nil{
		panic(err)
	}
	defer file.Close()

	arr := make([][]int,15)
	var idx int  = 0
	buff := bufio.NewReader(file)
	for{
		line,_,err := buff.ReadLine()
		if err != nil || io.EOF == err{
			break
		}
		strarr := strings.Split(string(line[:len(line)])," ")
		arr[idx] = make([]int,len(strarr))
		for i := 0; i < len(strarr);i++{
			arr[idx][i],err = strconv.Atoi(strarr[i])
			if err != nil{
				panic(err)
			}
		}

		idx++
	}

	for i := 0; i < len(arr);i++{
		for j := 0; j < len(arr[i]);j++{
			// (i,j)
			var cnt int = arr[i][j]
			if i - 1 >= 0{
				if j - 1 >= 0{
					// (i - 1,j - 1)
					cnt += arr[i - 1][j - 1]
				}
				if j < len(arr[i - 1]){
					// (i - 1,j)
					if arr[i][j] + arr[i - 1][j] < cnt{
						arr[i][j] = cnt
					}else{
						arr[i][j] += arr[i - 1][j]
					}
				}else{
					arr[i][j] = cnt
				}
			}
		}
	}

	var max int  = 0
	for i := 0; i < len(arr[len(arr) - 1]);i++{
		if arr[len(arr) - 1][i] > max{
			max = arr[len(arr) - 1][i]
		}
	}
	fmt.Println(max)

}
