package bitcask

import (
	"io"
	"os"
//	"fmt"
	"sync"
	"strings"
	"strconv"
	"errors"
)

const defaultDirPerm os.FileMode = 0755
const activeHintFile = "active.hint"
const activeDataFile = "active.data"

var ErrUnknowHintFile = errors.New("Unknow hint file")
var ErrKeyNotExist = errors.New("Key Not Exist")

type Bitcask struct {
	mu			sync.Mutex

	activeFile	*bFile
	oldFiles	*bFiles
	kds			*keyDirs

	nextFileId	uint32
	opt			*Options
	dirName		string
}

func Open(dirName string) (*Bitcask,error) {

	return OpenOpt(dirName,nil)
}

func OpenOpt(dirName string,opt *Options) (*Bitcask,error) {

	if opt == nil {
		opt = NewDefaultOptions()
	}
	_,err := os.Stat(dirName)
	if err != nil {
		if opt.CreateIfMissing && os.IsNotExist(err) {
			err := os.Mkdir(dirName,defaultDirPerm)
			if err != nil {
				return nil,err
			}
		} else {
			return nil,err
		}
	}

	bc := &Bitcask{}
	bc.dirName = dirName
	bc.opt = opt
	bc.kds = NewKeyDir()
	bc.oldFiles = newBFiles()
	err = bc.rebuildIndex()
	if err != nil {
		return nil,err
	}

	bc.activeFile,err = createBFile(bc.dirName,activeDataFile,activeHintFile,int(bc.nextFileId))
	if err != nil {
		return nil,err
	}
	bc.nextFileId++
	return bc,nil
}

func (bc *Bitcask)Put(key,value []byte) error {
	bc.mu.Lock()
	defer bc.mu.Unlock()
	bc.checkSize()
	dir,err := bc.activeFile.append(key,value)
	if err != nil {
		return err
	}
	bc.kds.set(string(key),dir)
	return nil
}

func (bc *Bitcask)Get(key []byte) ([]byte,error) {
	bc.mu.Lock()
	defer bc.mu.Unlock()

	d := bc.kds.get(string(key))
	if d == nil {
		return nil,ErrKeyNotExist
	}

	var bf *bFile
	var err error

	if d.fileId == bc.activeFile.fileId {
		bf = bc.activeFile
	} else {
		bf = bc.oldFiles.get(d.fileId)
		if bf == nil {
			return nil,ErrKeyNotExist
		}
	}
	data := make([]byte,d.valueSize)
	n,err := bf.file.ReadAt(data,d.valuePos)
	if err != nil && n != len(data) {
		return nil,err
	}
	return data,nil
}

func (bc *Bitcask)Delete(key []byte) error {
	bc.mu.Lock()
	defer bc.mu.Unlock()

	bc.checkSize()

	_,err := bc.activeFile.append(key,nil)
	if err != nil {
		return err
	}
	bc.kds.del(string(key))
	return nil
}

func (bc *Bitcask)rebuildIndex() error {
	hintFps,err := filesWithSuffix(bc.dirName,".hint")
	if err != nil {
		return err
	}

	bc.nextFileId = uint32(0)
	activeHintFileName := ""

	for _,fp := range hintFps {
		lastIndex := strings.LastIndex(fp.Name(),string(os.PathSeparator))
		if lastIndex < 0 {
			continue
		}
		filename := string(fp.Name()[lastIndex + 1:])
		if filename == activeHintFile {
			activeHintFileName = fp.Name()
		} else {
			fileId,err := strconv.Atoi(filename[:len(filename) - 5])
			if err != nil {
				return ErrUnknowHintFile
			}
			if fileId > int(bc.nextFileId) {
				bc.nextFileId = uint32(fileId)
			}
			err = bc.rebuildFromHintFile(fp.Name(),uint32(fileId))
			if err != nil {
				return err
			}
		}
	}

	bc.nextFileId++

	if activeHintFileName != "" {
		lastIndex := strings.LastIndex(activeHintFileName,string(os.PathSeparator))
		if lastIndex < 0 {
			panic("Invalid filename,must contain path separator")
		}
		newHintFileName := activeHintFileName[:lastIndex + 1] + fileName(bc.nextFileId) + ".hint"
		activeDataFileName := bc.dirName + string(os.PathSeparator) + activeDataFile
		_,err := os.Stat(activeDataFileName)
		if err != nil && os.IsNotExist(err) {
			// exist active.hint but cannot find active.data,return direct
			return nil
		}
		newDataFileName := activeHintFileName[:lastIndex + 1] + fileName(bc.nextFileId) + ".data"

		err = os.Rename(activeHintFileName,newHintFileName)
		if err != nil {
			return errors.New("Rename active hint file failed")
		}
		err = os.Rename(activeDataFileName,newDataFileName)
		if err != nil {
			err = os.Rename(newHintFileName,activeHintFileName)
			if err != nil {
				panic("Recovery active file failed")
			}
			return errors.New("Rename active data file failed")
		}

		err = bc.rebuildFromHintFile(newHintFileName,bc.nextFileId)
		if err != nil {
			return err
		}
		bc.nextFileId++
	}
	return nil
}

func (bc *Bitcask)rebuildFromHintFile(hintFile string,fileId uint32) error {
	fp,err := os.Open(hintFile)
	if err != nil {
		return err
	}

	bf,err := openBFile(bc.dirName,int(fileId))
	if err != nil {
		return err
	}
	bc.oldFiles.set(fileId,bf)
	for {
		timeStamp,_,valueSize,valuePos,key,err := decodeHintEntry(fp,int(fileId))
		if err != nil {
			if err == io.EOF {
				break
			} else {
				return err
			}
		} else {
			bc.kds.set(string(key),&dir{fileId,valueSize,valuePos,timeStamp})
		}
	}
	return nil
}

// If file meets a size threshold,it will be closed and new active file will be created
func (bc *Bitcask)checkSize() {
	if bc.activeFile.writeOffset >= int64(bc.opt.MaxFileSize) {
		bc.activeFile.close()
		oldDataFile := bc.dirName + string(os.PathSeparator) + activeDataFile
		newDataFile := bc.dirName + string(os.PathSeparator) + fileName(bc.nextFileId) + ".data"
		err := os.Rename(oldDataFile,newDataFile)
		if err != nil {
			panic("Active data file convert to old file fail")
		}
		oldHintFile := bc.dirName + string(os.PathSeparator) + activeHintFile
		newHintFile := bc.dirName + string(os.PathSeparator) + fileName(bc.nextFileId) + ".hint"
		err = os.Rename(oldHintFile,newHintFile)
		if err != nil {
			err = os.Rename(newDataFile,oldDataFile)
			if err != nil {
				panic("Recover failed")
			}
			panic("Active hint file convert to old hint file fail")
		}

		bf,err := openBFile(bc.dirName,int(bc.nextFileId))
		if err != nil {
			panic("Open bFile fail")
		}
		bc.oldFiles.set(bc.nextFileId,bf)
		bc.nextFileId++

		bc.activeFile,err = createBFile(bc.dirName,activeDataFile,activeHintFile,int(bc.nextFileId))
		if err != nil {
			panic("Create bFile fail")
		}
	}
}
