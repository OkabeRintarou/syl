package util

import "testing"

func TestBitmap(t *testing.T) {
	for size := uint64(1); size <= uint64(16); size++ {
		test(t, size)
	}
	test(t, 100)
	test(t, 1000)
	test(t, 1024)
}

func test(t *testing.T, size uint64) {
	bm := NewBitmap(size)
	if (bm.Size() % 8) != 0 {
		t.Fatalf("Expect size %d, got %d\n", ((size+7)/8)*8, bm.Size())
	}
	for i := uint64(0); i < size; i++ {
		if bm.Test(i) {
			t.Fatal("Expect false, got true")
		}
		bm.Set(i)
		if !bm.Test(i) {
			t.Fatalf("Expect true, got false")
		}
		bm.Unset(i)
		if bm.Test(i) {
			t.Fatal("Expect false, got true")
		}
		bm.Set(i)
	}

	bm.Clear()
	for i := uint64(0); i < size; i++ {
		if bm.Test(i) {
			t.Fatal("Expect false, got true")
		}
	}
}
