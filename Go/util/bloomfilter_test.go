package util

import "testing"
import "crypto/md5"
import "io"
import "strconv"

func TestContains(t *testing.T) {
	var f HashFunction = func(s string) uint64 {
		return uint64(s[0])
	}
	bf := NewBloomFilter(2, HashFunctions{f})
	bf.Add("hello")
	if !bf.Contains("hello") {
		t.Fatal("Expect true, got false")
	}
	s := "i am a student"
	if bf.Contains(s) {
		t.Fatal("Expect false, got true")
	}
	if !bf.Contains("hello") {
		t.Fatal("Expect true, got false")
	}
	bf.Add(s)

	hashes := HashFunctions{
		func(s string) uint64 {
			h := md5.New()
			io.WriteString(h, s)
			sum := h.Sum(nil)
			var ret uint64 = 0
			for i := 0; i < len(sum); i += 2 {
				ret <<= 8
				ret |= uint64((sum[i] ^ sum[i+1]))
			}
			return ret
		},
		func(s string) uint64 {
			var ret uint64 = 0
			var g uint64
			for _, v := range s {
				ret = (ret << 4) + uint64(v)
				g = uint64(ret & 0xf0000000)
				if g != 0 {
					ret = ret ^ (g >> 24)
					ret ^= g
				}
			}
			return ret
		},
	}

	bf = NewBloomFilter(1000000, hashes)
	rnd := func(i int) string {
		h := md5.New()
		io.WriteString(h, strconv.Itoa(i))
		return string(h.Sum(nil))
	}

	for i := 0; i < 1000; i++ {
		if bf.Contains(rnd(i)) {
			t.Fatalf("%f probabilty expect false, got true\n", 2000.0/1000000)
		}
	}
}
