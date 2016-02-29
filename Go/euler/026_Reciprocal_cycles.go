package main

import "fmt"

func biggerThan(num int,initValue int)int{
    for ;initValue < num;initValue *= 10{
    }
    return initValue
}

func divideInverse(divided int)int{

    var divisor int = biggerThan(divided,1)
    var mp map[int]bool = make(map[int]bool)

    for{
        tmp := divisor - divisor / divided * divided

        if tmp == 0{
            return 1
        }
        _,ok := mp[tmp]
        if ok{
            return len(mp)
        }else{
            mp[tmp] = true
            divisor = biggerThan(divided,tmp)
        }
    }

    return -1
}

func main(){

    var max int = 0
    var index = 1
    for i := 1; i < 1000;i++{
        tmp := divideInverse(i)
        if tmp > max{
            max = tmp
            index = i
        }
    }

    fmt.Println(index)
}
