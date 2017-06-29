package bitcask

import (
	"os"
	"strings"
)

type bFile struct {
	file			*os.File
	writeOffset		int64
	fileId			uint32
	hintFile		*os.File
	hintOffset		int64
}

func newBFile() *bFile {
	return &bFile{}
}

func openBFile(dirName string,fileId int) (*bFile,error) {
	filename := strings.Join([]string{dirName,string(os.PathSeparator),fileName(uint32(fileId)),".data"},"")
	fp,err := os.OpenFile(filename,os.O_RDONLY,os.ModePerm)
	if err != nil {
		return nil,err
	}

	return &bFile {
				file:fp,
				fileId:uint32(fileId),
			},nil
}

func createBFile(dirName,fileName,hintFileName string,fileId int) (*bFile,error) {
	writeFileName := dirName + string(os.PathSeparator) + fileName
	fp,err := os.Create(writeFileName)
	if err != nil {
		return nil,err
	}
	createHintFileName := dirName + string(os.PathSeparator) + hintFileName
	hintFp,err := os.Create(createHintFileName)
	if err != nil {
		fp.Close()
		os.Remove(writeFileName)
		return nil,err
	}
	return &bFile {
				file:fp,
				fileId:uint32(fileId),
				hintFile:hintFp,
				writeOffset:int64(0),
				hintOffset:int64(0),
			},nil
}

func (fp *bFile)close() {
	fp.file.Sync()
	fp.file.Close()
	if fp.hintFile != nil {
		fp.hintFile.Sync()
		fp.hintFile.Close()
	}
	fp.writeOffset = int64(0)
	fp.hintOffset = int64(0)
	fp.fileId++
}

func (fp *bFile)append(key,value []byte) (*dir,error) {
	entry,data := newEntry(key,value)
	_,err := fp.file.WriteAt(data,fp.writeOffset)
	if err != nil {
		return nil,err
	}

	valuePos := fp.writeOffset + int64(HeaderSize) +  int64(len(key))
	hintEntry := encodeHintEntry(entry.timeStamp,entry.keySize,entry.valueSize,valuePos,key)
	_,err = fp.hintFile.WriteAt(hintEntry,fp.hintOffset)
	if err != nil {
		return nil,err
	}
	fp.hintOffset += int64(len(hintEntry))
	fp.writeOffset += int64(len(data))

	return &dir{fp.fileId,entry.valueSize,valuePos,entry.timeStamp},nil
}


type bFiles struct {
	files map[uint32]*bFile
}

func newBFiles() *bFiles {
	return &bFiles{make(map[uint32]*bFile)}
}

func (bf *bFiles)set(fileId uint32,file *bFile) {
	bf.files[fileId] = file
}

func (bf *bFiles)get(fileId uint32) *bFile {
	file,ok := bf.files[fileId]
	if !ok {
		return nil
	}
	return file
}


