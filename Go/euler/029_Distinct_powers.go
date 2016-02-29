package main

import "fmt"
import "tool"
import "strconv"

func stringMul(lhs,rhs string)string{
    rhsNum,err := strconv.Atoi(rhs)
    if err != nil{
        fmt.Println(err)
    }
    var ret string = lhs
    for i := 0; i < rhsNum - 1;i++{
        ret = tool.StringAdd(ret,lhs)
    }
    return ret
}

func exp(numStr string,expStr string)string{
    expNum,_ := strconv.Atoi(expStr)
    var ret string = numStr
    for i := 0; i < expNum - 1;i++{
        ret = stringMul(ret,numStr)
    }
    return ret
}

func main(){
    var mp map[string]bool = make(map[string]bool)
    lower,upper := 2,100

    var count int = 0
    for a := lower ; a <= upper;a++{

        fmt.Println("a = ",a)
        for b := lower ; b <= upper;b++{
            str := exp(strconv.Itoa(a),strconv.Itoa(b))
            _,ok := mp[str]
            if !ok{
                mp[str] = true
                count++
            }
        }
    }

    fmt.Println(count)
}
