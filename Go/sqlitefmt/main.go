package main

import (
	"encoding/binary"
	"errors"
	"fmt"
	"io/ioutil"
	"os"
	"strings"
)

type Header struct {
	PageSize               uint16
	WriteVersion           uint8
	ReadVersion            uint8
	ReservedBytes          uint8
	MaxPayloadFraction     uint8
	MinPayloadFraction     uint8
	MinLeafPayloadFraction uint8
	ChangeCount            uint32
	Size                   uint32
	FirstFree              uint32
	FreeSize               uint32
	SchemaCookie           uint32
	SchemaFormatNumber     uint32
	SuggestedCacheSize     uint32
	AutoVacuum             uint32
	TextEncoding           uint32
	UserVersionNumber      uint32
	IncrementalVacuum      uint32
	ApplicationID          uint32
	WriteLibraryVersion    uint32
	VersionValidFor        uint32
}

const (
	headerString = "SQLite format 3\000"

	SQLITE_UTF8    = 1
	SQLITE_UTF16LE = 2
	SQLITE_UTF16BE = 3
)

var (
	ErrTruncated = errors.New("need more content")
	ErrFormat    = errors.New("corrupted file content")
)

func parseHeader(data []byte) (*Header, error) {
	if len(data) < 100 {
		return nil, ErrTruncated
	}

	if !strings.EqualFold(string(data[:16]), headerString) {
		return nil, ErrFormat
	}

	h := new(Header)
	h.PageSize = binary.BigEndian.Uint16(data[16:])
	if (h.PageSize&(h.PageSize-1)) != 0 || h.PageSize < 512 || h.PageSize > 32768 {
		return nil, ErrFormat
	}

	h.WriteVersion = data[18]
	h.ReadVersion = data[19]
	h.ReservedBytes = data[20]

	h.MaxPayloadFraction = data[21]
	h.MinPayloadFraction = data[22]
	h.MinLeafPayloadFraction = data[23]
	h.ChangeCount = binary.BigEndian.Uint32(data[24:])
	h.Size = binary.BigEndian.Uint32(data[28:])

	h.FirstFree = binary.BigEndian.Uint32(data[32:])
	h.FreeSize = binary.BigEndian.Uint32(data[36:])
	h.SchemaCookie = binary.BigEndian.Uint32(data[40:])
	h.SchemaFormatNumber = binary.BigEndian.Uint32(data[44:])
	h.SuggestedCacheSize = binary.BigEndian.Uint32(data[48:])
	h.AutoVacuum = binary.BigEndian.Uint32(data[52:])
	h.TextEncoding = binary.BigEndian.Uint32(data[56:])
	h.UserVersionNumber = binary.BigEndian.Uint32(data[60:])
	h.IncrementalVacuum = binary.BigEndian.Uint32(data[64:])
	h.ApplicationID = binary.BigEndian.Uint32(data[68:])

	for i := 72; i < 92; i++ {
		if data[i] != 0 {
			return nil, ErrFormat
		}
	}
	h.VersionValidFor = binary.BigEndian.Uint32(data[92:])
	h.WriteLibraryVersion = binary.BigEndian.Uint32(data[96:])
	if h.Size > 0 && h.VersionValidFor != h.ChangeCount {
		h.Size = 0
	}
	return h, nil
}

func main() {
	if len(os.Args) != 2 {
		fmt.Fprintf(os.Stderr, "usage: %s <sqlite db file>", os.Args[0])
		os.Exit(1)
	}

	content, err := ioutil.ReadFile(os.Args[1])
	if err != nil {
		panic(err)
	}
	header, err := parseHeader(content)

	if err != nil {
		panic(err)
	}
	_ = header
}
