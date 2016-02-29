package main

import "fmt"
import "math"

func sumOfProperDivisors(num uint)uint{
    sum := uint(1)
    upper := uint(math.Sqrt(float64(num))) + 1
    arr := make([]uint,0,num)

    for i := uint(2); i < upper;i++{
       if num % i == 0 {
            arr = append(arr,i)
       }
    }

    for _,v := range(arr){
        if v * v != num{
            sum += (v + (num / v))
        }else{
            sum += v
        }
    }
    return sum
}

func isAbundant(num uint)bool{
    return sumOfProperDivisors(num) > num
}

func genAbundantNumbers()[]uint{
    arr := make([]uint,0,28124)
    for i := uint(12);i < uint(28124);i++{
        if isAbundant(i){
            arr = append(arr,i)
        }
    }
    return arr
}


func main(){
    arr := genAbundantNumbers()

    sum := uint64(0)
    upper := 28124

    hole := make([]int,upper)

    for _,v1 := range arr{
        for _,v2 := range arr{
            if  v1 + v2 < uint(upper){
                hole[v1 + v2] = 1
            }
        }
    }

    for i := 0; i < upper;i++{
        if hole[i] == 0{
            sum += uint64(i)
        }
    }

    fmt.Println(sum)
}
