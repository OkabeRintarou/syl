package sort

import (
	"testing"
)

func TestBubbleSort(t *testing.T) {
	for i := 0; i < 100; i++ {
		data := makeData(10000)
		BubbleSort(data)
		if !isSorted(data) {
			t.Fatal("expect sorted")
		}
	}
}
