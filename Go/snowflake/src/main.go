package main

import "snowflake"
import "fmt"

func main() {
        sf := snowflake.New(0,0)
        for i := 0; i < 100000; i++ {
                fmt.Println(sf.ID())
        }
}
