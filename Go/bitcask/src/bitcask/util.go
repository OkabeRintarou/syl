package bitcask

import (
	"fmt"
	"os"
	"strings"
)

const paddingSize = 10
var formatter = "%" + fmt.Sprintf("0%dd",paddingSize)

func fileName(fileId uint32) string {
	return fmt.Sprintf(formatter,fileId)
}


func filesWithSuffix(dirName,suffix string) ([]*os.File,error) {
	_,err := os.Stat(dirName)
	if err != nil {
		return nil,err
	}
	dirFp,err := os.OpenFile(dirName,os.O_RDONLY,os.ModeDir)
	if err != nil {
		return nil,err
	}
	defer dirFp.Close()

	filenames,err := dirFp.Readdirnames(-1)
	if err != nil {
		return nil,err
	}
	fnWithSuffix := make([]string,0,16)
	for _,fn := range filenames {
		if strings.HasSuffix(fn,suffix) {
			fnWithSuffix = append(fnWithSuffix,fn)
		}
	}

	files := make([]*os.File,len(fnWithSuffix))
	defer func() {
		for i := 0;i < len(files);i++ {
			if files[i] != nil {
				files[i].Close()
			} else {
				break
			}
		}
	}()

	for i := 0;i < len(files);i++ {
		files[i],err = os.Open(dirName + string(os.PathSeparator) + fnWithSuffix[i])
		if err != nil {
			return nil,err
		}
	}

	return files,nil
}
