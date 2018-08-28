package sort

import (
	"testing"
)

func TestMergeSort(t *testing.T) {
	for i := 0; i < 100; i++ {
		data := makeData(0)
		MergeSort(data)
		if !isSorted(data) {
			t.Fatal("expect sorted")
		}
	}
}
