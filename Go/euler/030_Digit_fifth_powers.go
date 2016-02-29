package main

import "fmt"
import "math"
import "strconv"

func sumOfFifthPowers(num int)int{
    numStr := strconv.Itoa(num)

    var arr []int = make([]int,0,len(numStr))
    for i := 0; i < len(numStr);i++{
        arr = append(arr,int(numStr[i] - '0'))
    }
    
    var sum int = 0
    for i := 0; i < len(arr);i++{
        sum += int(math.Pow(float64(arr[i]),5))
    }
    return sum
}

func main(){
    const upper int = 1000000

    var sum int = 0
    for i := 10; i < upper;i++{
        if sumOfFifthPowers(i) == i{
            sum += i
        }
    }
    fmt.Println(sum)
}
