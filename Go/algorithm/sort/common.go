package sort

import (
	"math/rand"
)

const maxLen = 500000

func makeData(tick int) []int {
	var length int
	if tick == 0 {
		length = rand.Intn(maxLen + 1)
	} else {
		length = rand.Intn(tick + 1)
	}
	data := make([]int, length)
	for i := 0; i < length; i++ {
		data[i] = rand.Intn(maxLen)
	}
	return data
}

func isSorted(data []int) bool {
	if len(data) == 0 {
		return true
	}
	for i := 0; i < len(data)-1; i++ {
		if data[i] > data[i+1] {
			return false
		}
	}
	return true
}
