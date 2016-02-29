package main

import ("fmt"
        "math"
       )

func isPrime(num int)bool{

    if num < 2{
        return false
    }
    if num == 2{
        return true
    }else if num & 1 == 0{
        return false
    }else{
        var upper int = int(math.Sqrt(float64(num))) + 1
        for i := 2; i < upper;i++{
            if num % i == 0{
                return false
            }
        }
    }

    return true
}

func quadraticPrimes(a,b int)int{
    var l int = 0
    for n := 0; ;n++{
        if isPrime(n * n + a * n + b){
            l++
        }else{
            break
        }
    }
    return l
}

func main(){

    var lower int = -999
    var upper int = 1000

    var max int  = 0
    cura,curb := 0,0

    for a := lower; a < upper;a++{
        for b := 2;b < upper;b++{
            if !isPrime(b){
                continue
            }
            tmp := quadraticPrimes(a,b)
            if tmp > max{
                max = tmp
                cura,curb = a,b
            }
        }
    }

    fmt.Println(cura * curb,cura,curb)

}

