package bitcask


type dir struct {
	fileId		uint32
	valueSize	uint32
	valuePos	int64
	timeStamp	uint64
}

// in-memory hash table
type keyDirs struct {
	dir			map[string]*dir
}

func NewKeyDir() *keyDirs {
	kd := &keyDirs{}
	kd.dir = make(map[string]*dir)
	return kd
}

func NewDir(fileId int,valueSize uint32,valuePos int64,timeStamp uint64) *dir {
	return &dir{
			fileId:uint32(fileId),
			valueSize:valueSize,
			valuePos:valuePos,
			timeStamp:timeStamp,
		}
}

// lookup up the key in keydir,return nil if key does not exist
func (kd *keyDirs)get(key string)*dir {
	d,ok := kd.dir[key]
	if ok {
		return d
	} else {
		return nil
	}
}

// update keydir
func (kd *keyDirs)set(key string,d *dir) {
	kd.dir[key] = d
}

func (kd *keyDirs)del(key string) {
	delete(kd.dir,key)
}

