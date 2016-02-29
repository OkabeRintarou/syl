package tool

import "strconv"

func StringAdd(lhs,rhs string)string{

    var maxLen int
    if len(lhs) < len(rhs){
        maxLen = len(rhs)
    }else{
        maxLen = len(lhs)
    }
    var arr1 []byte = make([]byte,maxLen)
    var arr2 []byte = make([]byte,maxLen)
    for i,j := len(lhs) - 1,maxLen - 1; i >= 0;i,j = i - 1,j - 1{
        arr1[j] = byte(lhs[i] - '0')
    }
    for i,j := len(rhs) - 1,maxLen - 1;i >= 0;i,j = i - 1,j - 1{
        arr2[j] = byte(rhs[i] - '0')
    }

    var str string
    var carry byte = 0
    for i := maxLen - 1; i >= 0;i--{
        var num byte = arr1[i] + arr2[i] + carry
        carry = num / 10
        str += strconv.Itoa(int(num % 10))
    }
    if carry > 0{
        str += strconv.Itoa(int(carry))
    }


    return  Reverse(str)
}

func Reverse(s string) string {
    r := []rune(s)
    for i, j := 0, len(r)-1; i < j; i, j = i+1, j-1 {
            r[i], r[j] = r[j], r[i]
        }
    return string(r)
}

