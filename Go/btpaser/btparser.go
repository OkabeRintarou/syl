package btparser

import (
	"os"
	"errors"
	"io/ioutil"
	"strings"
	"strconv"
	"crypto/sha1"
	"io"
)

var ErrUnrecognizedTorrent = errors.New("Unrecognized torrent file")

type TorrentFile struct {
	creationDate		string
	createdBy		string
	announce		string
	announceList		[][]string
	comment 		string
	files			[]FileInfo

	infohash		[]byte
	startIndex		int
}


func (tf *TorrentFile)Announce() string {
	return tf.announce
}

func (tf *TorrentFile)InfoHash() []byte {
	return tf.infohash
}

func (tf *TorrentFile)AnnounceList() []string {
	list := make([]string,0,4)
	for i := 0;i < len(tf.announceList);i++ {
		for j := 0;j < len(tf.announceList[i]);j++ {
			list = append(list,tf.announceList[i][j])
		}
	}
	return list
}

type FileInfo struct {
	Name 		string
	Length		int
	Md5CheckSum	string
}

var dict_start = byte('d')
var int_start = byte('i')
var list_start = byte('l')
var end_char = byte('e')
var separator = byte(':')
var digits = "0123456789"


func New(filename string) (*TorrentFile,error) {
	bt := new(TorrentFile)
	bt.startIndex = -1
	if err := bt.parse(filename);err != nil {
		return nil,err
	}
	return bt,nil
}

func (bt *TorrentFile) parse(filename string) error {
	var err error
	fp,err := os.Open(filename)
	if err != nil {
		return err
	}
	data,err := ioutil.ReadAll(fp)
	if err != nil {
		return err
	}

	parsedData,_,err := bt.decode(data,0)
	mp,ok := parsedData.(map[string]interface{})
	if !ok {
		return ErrUnrecognizedTorrent
	}

	bt.creationDate = parseCreationDate(mp)
	bt.createdBy = parseCreatedBy(mp)
	bt.announce = parseAnnounce(mp)
	bt.announceList = parseAnnounceList(mp)
	bt.comment = parseComment(mp)
	bt.files = parseFiles(mp)

	return nil
}

func parseCreationDate(mp map[string]interface{}) string {
	creationData,ok := mp["creation date"]
	if !ok {
		return ""
	}
	cd,ok := creationData.(string)
	if !ok {
		return ""
	}
	return cd
}

func parseCreatedBy(mp map[string]interface{}) string {
	cb,ok := mp["created by"]
	if !ok {
		return ""
	}

	createdBy,ok := cb.(string)
	if !ok {
		return ""
	}
	return createdBy
}

func parseAnnounce(mp map[string]interface{}) string {
	a,ok := mp["announce"]
	if !ok {
		return ""
	}
	announce,ok := a.(string)
	if !ok {
		return ""
	}
	return announce
}

func parseAnnounceList(mp map[string]interface{}) [][]string {
	v,ok := mp["announce-list"]
	if !ok {
		return nil
	}
	_,ok = v.([]interface{})
	if !ok {
		return nil
	}

	return nil
}

func parseComment(mp map[string]interface{}) string {
	v,ok := mp["comment"]
	if !ok {
		return ""
	}
	comment,ok := v.(string)
	if !ok {
		return ""
	}
	return comment
}

func parseFiles(mp map[string]interface{}) []FileInfo {
	i,ok := mp["info"]
	if !ok {
		return nil
	}

	info,ok := i.(map[string]interface{})
	if !ok {
		return nil
	}
	fs,ok := info["files"]
	if !ok {
		n,ok := info["name"]
		if !ok {
			return nil
		}
		name,ok := n.(string)
		if !ok {
			return nil
		}
		l,ok := info["length"]
		if !ok {
			return nil
		}
		length,ok := l.(int)
		if !ok {
			return nil
		}
		checkSum,ok := info["md5sum"]
		if !ok {
			return nil
		}
		md5CheckSum,ok := checkSum.(string)
		if !ok {
			return nil
		}
		return []FileInfo{FileInfo{name,length,md5CheckSum}}
	} else {
		fileInfos := make([]FileInfo,0)

		dn,ok := info["name"]
		dirName,_ := dn.(string)

		files,ok := fs.([]interface{})
		if !ok {
			return nil
		}
		for _,f := range files {
			file,ok := f.(map[string]interface{})
			if !ok {
				return nil
			}

			p,ok := file["path"]
			if !ok {
				continue
			}
			path,ok := p.([]interface{})
			pathList := make([]string,1)
			pathList[0] = dirName
			for _,p := range path {
				name,_ := p.(string)
				pathList = append(pathList,name)
			}
			l,ok := file["length"]
			if !ok {
				continue
			}
			length,ok := l.(int)
			if !ok {
				continue
			}
			checkSum,ok := file["md5sum"]
			md5CheckSum,ok := checkSum.(string)
			fileInfos = append(fileInfos,FileInfo{strings.Join(pathList,string(os.PathSeparator)),length,md5CheckSum})
		}
		return fileInfos
	}
}


func (bt *TorrentFile) decode(data []byte,index int) (interface{},int,error) {
	if data[index] == dict_start {
		return bt.decode_dict(data,index)
	} else if data[index] == int_start {
		return bt.decode_int(data,index)
	} else if data[index] == list_start {
		return bt.decode_list(data,index)
	} else if strings.IndexByte(digits,data[index]) >= 0 {
		return bt.decode_string(data,index)
	} else {
		return nil,0,ErrUnrecognizedTorrent
	}
}


func (bt *TorrentFile) decode_int(data []byte,index int) (interface{},int,error) {

	int_start := index + 1
	for index < len(data) && data[index] != end_char {
		index++
	}
	if index >= len(data) {
		return nil,0,ErrUnrecognizedTorrent
	}
	intData,err := strconv.Atoi(string(data[int_start:index]))
	if err != nil {
		return nil,0,ErrUnrecognizedTorrent
	}
	return intData,index + 1,nil
}

func (bt *TorrentFile) decode_list(data []byte,index int) (interface{},int,error){

	index++
	list := make([]interface{},0,4)
	var (
		v interface{}
		err error
	)
	for index < len(data) && data[index] != end_char {
		v,index,err = bt.decode(data,index)
		if err != nil {
			return nil,0,err
		}
		list = append(list,v)
	}

	if index >= len(data) {
		return nil,0,ErrUnrecognizedTorrent
	}
	return list,index + 1,nil
}

func (bt *TorrentFile) decode_dict(data []byte,index int) (interface{},int,error) {
	index++
	dict := make(map[string]interface{})

	var (
		key,value interface{}
		err error
	)
	for index < len(data) && data[index] != end_char {
		key,index,err = bt.decode(data,index)

		if err != nil {
			return nil,0,err
		}
		value,index,err = bt.decode(data,index)
		if err != nil {
			return nil,0,err
		}
		if k,ok := key.(string);!ok {
			return nil,0,ErrUnrecognizedTorrent
		} else {
			if k == "info" && bt.startIndex >= 0{
				h := sha1.New()
				io.WriteString(h,string(data[bt.startIndex:index]))
				bt.infohash = h.Sum(nil)
			}
			dict[k] = value
		}
	}
	return dict,index + 1,nil
}

func (bt *TorrentFile) decode_string(data []byte,index int) (interface{},int,error) {
	lengthd := make([]byte,0,4)
	lengthd = append(lengthd,data[index])
	index++
	for index < len(data) && (data[index] >= byte('0') && data[index] <= byte('9')) {
		lengthd = append(lengthd,data[index])
		index++
	}

	if index >= len(data) || data[index] != separator {
		return nil,0,ErrUnrecognizedTorrent
	}

	length,err := strconv.Atoi(string(lengthd))
	if err != nil {
		return nil,0,ErrUnrecognizedTorrent
	}
	index++

	if string(data[index:index + length]) == "info" {
		bt.startIndex = index + length
	}
	return string(data[index:index + length]),index + length,nil
}

