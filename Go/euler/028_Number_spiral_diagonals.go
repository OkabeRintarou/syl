package main

import "fmt"


func next(start,step uint64)(sum,newStart uint64){
    sum = start << 2 + 6 * step
    newStart = start + (step << 2) + 2

    return sum,newStart
}

func main(){
    var totals uint64 = 1

    start,step := uint64(3),uint64(2)
    var count int = 1
    var sum uint64
    for{
        sum,start = next(start,step)
        step += 2
        count += 2
        totals += sum
        if count >= 1001{
            break
        }
    }

    fmt.Println(totals)
}
