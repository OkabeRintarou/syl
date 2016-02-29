package main

import "fmt"
import "os"
import "io/ioutil"
import "strings"

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

func main(){
	const filename string  = "022_Names_scores.txt"
	file,err := os.Open(filename)
	if err != nil{
		panic(err)
	}
	defer file.Close()
	data,err := ioutil.ReadAll(file)
	if err != nil{
		panic(err)
	}
	str := strings.Split(string(data),",")

	table := map[int8]int{
		'A':1,'B':2,'C':3,'D':4,'E':5,'F':6,'G':7,'H':8,'I':9,'J':10,
	    'K':11,'L':12,'M':13,'N':14,'O':15,'P':16,'Q':17,'R':18,'S':19,'T':20,
	    'U':21,'V':22,'W':23,'X':24,'Y':25,'Z':26}

    sort(str)

	sum := uint64(0)

	for i,v := range str{
		tmp := uint64(0)
		for j := 1; j < len(v) - 1;j++{
			tmp += uint64(table[int8(v[j])])
		}

		sum += uint64(tmp * uint64(i + 1))
	}
	fmt.Println(sum)
}
