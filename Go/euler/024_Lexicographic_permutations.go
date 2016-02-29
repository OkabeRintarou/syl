package main

import "fmt"

func sort(strs []string){
    sort_impl(strs,0,len(strs) - 1);
}


func sort_impl(strs []string,l,r int){
    if l < r{
        m := l - 1
        back := strs[r]
        for i := l ; i < r; i++{
            if strs[i] < back{
                m += 1
                swap(strs,i,m)
            }
        }

        m += 1
        swap(strs,m,r)

        sort_impl(strs,l, m - 1)
        sort_impl(strs,m + 1,r)
    }
}

func swap(strs []string,i,j int){
    tmp := strs[i]
    strs[i] = strs[j]
    strs[j] = tmp
}

func factorial(num int)int{
    mul := 1
    for i := 2; i <= num;i++{
        mul *= i
    }
    return mul
}

func insertTargetString(target []string,in string)[]string{

    retStr := make([]string,(len(target[0]) + 1) * factorial(len(target[0])))
    cur := 0

    for _,str := range target{
        for i := 0; i <= len(str);i++{
            retStr[cur] = str[:i] + in + str[i:]
            /*
            if  cur == 1000000 && len(str) == 9{
                fmt.Println(":",retStr[cur])
                return retStr
            }
            */
            cur += 1
        }
    }
    return retStr
}

func main(){
    threeLex := []string{"012","021","102","120","201","210"}
    chars := []string{"3","4","5","6","7","8","9"}
    target := threeLex
    for _,v := range chars{
        target = insertTargetString(target,v)
    }

    sort(target)

    oneMillion := 1000000
    fmt.Print(target[oneMillion - 1])
}
