package util

type Bitmap struct {
	size uint64
	data []byte
}

func NewBitmap(size uint64) *Bitmap {
	size = (size + 7) / 8
	data := make([]byte, size, size)
	return &Bitmap{size * 8, data}
}

func (bm *Bitmap) Test(index uint64) bool {
	index, shift := bm.locate(index)
	if index >= bm.size {
		return false
	}
	return ((bm.data[index] >> shift) & 0x01) == 0x01
}

func (bm *Bitmap) Set(index uint64) {
	index, shift := bm.locate(index)
	if index >= bm.size {
		panic("index out of bound")
	}
	bm.data[index] |= (0x01 << shift)
}

func (bm *Bitmap) Unset(index uint64) {
	index, shift := bm.locate(index)
	if index >= bm.size {
		panic("index out of bound")
	}
	bm.data[index] &= (^(0x01 << shift))
}

func (bm *Bitmap) Clear() {
	for i, _ := range bm.data {
		bm.data[i] = 0
	}
}

func (bm *Bitmap) Size() uint64 {
	return bm.size
}

func (bm *Bitmap) locate(index uint64) (uint64, uint64) {
	return index / 8, 7 - index%8
}
