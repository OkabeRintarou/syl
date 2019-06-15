package main

import (
	"net"
	"proxy"
)

func main() {
	conn, err := net.Dial("tcp", "127.0.0.1:3306")
	if err != nil {
		panic("connect mysql error")
	}
	defer conn.Close()

	greeting := &proxy.GreetingPacket{}
	if err := greeting.Encode(conn); err != nil {
		panic("client receive broken greeting packet")
	}
}
