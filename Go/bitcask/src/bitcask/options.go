package bitcask

const _1K = uint32(1 << 10)
const _1M = uint32(1 << 20)

type Options struct {
	CreateIfMissing		bool
	MaxFileSize			uint32
}


func NewOptions() *Options {
	opt := &Options{}
	opt.CreateIfMissing = false

	return opt
}

func NewDefaultOptions() *Options {
	opt := &Options{}
	opt.CreateIfMissing = true
	opt.MaxFileSize = _1K * 4
	return opt
}
