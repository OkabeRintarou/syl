package sort

import (
	"testing"
)

func TestQuickSort(t *testing.T) {
	for i := 0; i < 100; i++ {
		data := makeData(0)
		QuickSort(data)
		if !isSorted(data) {
			t.Fatal("expect sorted")
		}
	}
}
