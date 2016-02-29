package main

import "fmt"
import "tool"

func main(){
    var str1 = "1"
    var str2 = "1"
    for i := 3;;i++{
        var tmp = tool.StringAdd(str1,str2)
        if len(tmp) >= 1000{
            fmt.Println(i)
            break
        }else{
            str1 = str2
            str2 = tmp
        }
    }
}
