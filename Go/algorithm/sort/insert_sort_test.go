package sort

import (
	"testing"
)

func TestInsertSort(t *testing.T) {
	for i := 0; i < 10; i++ {
		data := makeData(10000)
		InsertSort(data)
		if !isSorted(data) {
			t.Fatal("expect sorted")
		}
	}
}
