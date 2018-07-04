package util

type HashFunction func(string) uint64

type HashFunctions []HashFunction

type BloomFilter struct {
	m         uint64
	bm        *Bitmap
	functions HashFunctions
}

func NewBloomFilter(m uint64, functions HashFunctions) *BloomFilter {
	bm := NewBitmap(m)
	return &BloomFilter{
		m:         m,
		bm:        bm,
		functions: functions,
	}
}

func (bf *BloomFilter) Contains(key string) bool {
	for _, hash := range bf.functions {
		i := hash(key) % bf.m
		if !bf.bm.Test(i) {
			return false
		}
	}
	return true
}

func (bf *BloomFilter) Add(key string) {
	for _, hash := range bf.functions {
		bf.bm.Set(hash(key) % bf.m)
	}
}
