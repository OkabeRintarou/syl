package bitcask

import (
	"os"
	"io"
	"time"
	"errors"
	"encoding/binary"
	"hash/crc32"
)

var ErrCorruptFile = errors.New("Currupt hint file")

const (
	HeaderSize = 20 // 4 + 8 + 4 + 4

	HintEntryHeaderSize = 24
)

var ErrCrc32 = errors.New("ChecksumIEEE error")

type entry struct {
	crc32		uint32
	timeStamp	uint64
	keySize		uint32
	valueSize	uint32
	key			[]byte
	value		[]byte
}

func newEntry(key,value []byte) (*entry,[]byte) {
	e := &entry{}
	e.key = key
	e.value = value
	e.keySize = uint32(len(key))
	e.valueSize = uint32(len(value))
	e.timeStamp = uint64(time.Now().Unix())

	data := make([]byte,HeaderSize + e.keySize + e.valueSize)
	binary.LittleEndian.PutUint64(data[4:12],e.timeStamp)
	binary.LittleEndian.PutUint32(data[12:16],e.keySize)
	binary.LittleEndian.PutUint32(data[16:HeaderSize],e.valueSize)
	copy(data[HeaderSize:HeaderSize + e.keySize],e.key)
	copy(data[HeaderSize + e.keySize:HeaderSize + e.keySize + e.valueSize],e.value)
	e.crc32 = crc32.ChecksumIEEE(data[4:])
	binary.LittleEndian.PutUint32(data[0:4],e.crc32)
	return e,data
}

func decodeEntry(fp *os.File) (*entry,error) {
	dataCRC32 := make([]byte,4)
	_,err := fp.Read(dataCRC32)
	if err != nil {
		return nil,ErrCorruptFile
	}
	storeCRC32 := binary.LittleEndian.Uint32(dataCRC32)
	dataTimeStamp := make([]byte,8)
	_,err = fp.Read(dataTimeStamp)
	if err != nil && err == io.EOF {
		return nil,ErrCorruptFile
	}
	timeStamp := binary.LittleEndian.Uint64(dataTimeStamp)

	dataKeySize := make([]byte,4)
	_,err = fp.Read(dataKeySize)
	if err != nil && err == io.EOF {
		return nil,ErrCorruptFile
	}
	keySize := binary.LittleEndian.Uint32(dataKeySize)

	dataValueSize := make([]byte,4)
	_,err = fp.Read(dataValueSize)
	if err != nil && err == io.EOF {
		return nil,ErrCorruptFile
	}
	valueSize := binary.LittleEndian.Uint32(dataValueSize)

	dataKey := make([]byte,keySize)
	_,err = fp.Read(dataKey)
	if err != nil && err == io.EOF {
		return nil,ErrCorruptFile
	}

	dataValue := make([]byte,valueSize)
	n,err := fp.Read(dataValue)
	if err != nil && n != len(dataValue) {
		return nil,ErrCorruptFile
	}

	data := make([]byte,HeaderSize + keySize + valueSize - 4)
	copy(data[:8],dataTimeStamp)
	copy(data[8:12],dataKeySize)
	copy(data[12:16],dataValueSize)
	copy(data[16:16 + keySize],dataKey)
	copy(data[16 + keySize:],dataValue)

	calcCRC32 := crc32.ChecksumIEEE(data)
	if calcCRC32 != storeCRC32 {
		return nil,ErrCrc32
	}
	return &entry{
				crc32:storeCRC32,
				timeStamp:timeStamp,
				keySize:keySize,
				valueSize:valueSize,
				key:dataKey,
				value:dataValue,
			},err
}

type hintEntry struct {
	timeStamp		uint64
	keySize			uint32
	valueSize		uint32
	valuePos		int64
	key				[]byte
}


func encodeHintEntry(timeStamp uint64,keySize,valueSize uint32,valuePos int64,key []byte) []byte {
	data := make([]byte,HintEntryHeaderSize + len(key))
	binary.LittleEndian.PutUint64(data[0:8],timeStamp)
	binary.LittleEndian.PutUint32(data[8:12],keySize)
	binary.LittleEndian.PutUint32(data[12:16],valueSize)
	binary.LittleEndian.PutUint64(data[16:HintEntryHeaderSize],uint64(valuePos))
	copy(data[HintEntryHeaderSize:],key)
	return data
}

// Decode next hint entry from fp
func decodeHintEntry(fp *os.File,fildId int) (timeStamp uint64,keySize,valueSize uint32,valuePos int64,key []byte,err error) {
	data := make([]byte,8)
	_,err = fp.Read(data)
	if err != nil {
		return
	}
	timeStamp = binary.LittleEndian.Uint64(data)
	_,err = fp.Read(data[:4])
	if err != nil && err == io.EOF {
		err = ErrCorruptFile

		return
	}
	keySize = binary.LittleEndian.Uint32(data[:4])
	_,err = fp.Read(data[:4])
	if err != nil && err == io.EOF {
		err = ErrCorruptFile
		return
	}
	valueSize = binary.LittleEndian.Uint32(data[:4])
	_,err = fp.Read(data)
	if err != nil && err == io.EOF {
		err = ErrCorruptFile
		return
	}
	valuePos = int64(binary.LittleEndian.Uint64(data))

	data = make([]byte,keySize)
	n,err := fp.Read(data)
	if err != nil && n != len(data){
		err = ErrCorruptFile
		return
	}
	key = data
	err = nil
	return
}


