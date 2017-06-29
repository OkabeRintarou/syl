package bitcask

import (
	"fmt"
	"testing"
)

func mistachValue(t *testing.T,key,expected,actual string) {
	t.Errorf("Get(%s),expect (%s),actual (%s)",key,expected,actual)
}

func TestOpen(t *testing.T) {
	fmt.Println("Test Open...")
	bc,err := Open("/tmp/mydb")

	if err != nil || bc == nil{
		t.Fatal(err)
	}
	var key,value []byte
	key,value = []byte("hello"),[]byte("value")

	bc.Put(key,value)
	v,err := bc.Get(key)
	if err != nil {
		t.Errorf("Get(%s),expect (%s),actual raise error:%s",string(key),string(value),err.Error())
	}
	if string(value) != string(v) {
		mistachValue(t,string(key),string(value),string(v))
	}

	bc.Delete([]byte("hello"))
	_,err = bc.Get([]byte("hello"))
	if err != ErrKeyNotExist {
		t.Errorf("Get(%s),expect error:%s,actual raise error:%s",string(key),ErrKeyNotExist.Error(),err.Error())

	}
	fmt.Println("PASS")
}

func TestPut(t *testing.T) {
	fmt.Println("Test Put...")

	bc,err := Open("/tmp/mydb_testput")
	if err != nil || bc == nil {
		t.Fatal(err)
	}
	for i := 0;i < 10000;i++ {
		key := fmt.Sprintf("key%d",i)
		value := fmt.Sprintf("value%d",i)
		bc.Put([]byte(key),[]byte(value))
		v,err := bc.Get([]byte(key))
		if err != nil {
			t.Errorf("Get(%s),expect (%s),actual raise error:%s",string(key),string(value),err.Error())
		}
		if string(v) != string(value) {
			mistachValue(t,string(key),string(value),string(v))
		}
	}
	fmt.Println("PASS")
}

func TestGet(t *testing.T) {
	fmt.Println("Test Get...")

	bc,err := Open("/tmp/mydb_testput")
	if err != nil {
		t.Fatal(err)
	}

	for i := 0;i < 10000;i++ {
		key := fmt.Sprintf("key%d",i)
		value := fmt.Sprintf("value%d",i)
		v,err := bc.Get([]byte(key))
		if err != nil {
			t.Errorf("Get(%s),expect (%s),actual raise error:%s",string(key),string(value),err.Error())
		}
		if string(v) != string(value) {
			mistachValue(t,string(key),string(value),string(v))
		}
	}
	fmt.Println("PASS")
}
