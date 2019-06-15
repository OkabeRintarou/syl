// supported MySQL version: 5.7.25

package proxy

import (
	"bufio"
	"bytes"
	"encoding/binary"
	"errors"
	"io"
)

type PacketHeader struct {
	BodyLen   uint
	SeqNumber uint
}

func mustPacketHeader(data []byte) PacketHeader {
	len0, len1, len2 := uint(data[0]), uint(data[1]), uint(data[2])
	bodyLen := len0 | (len1 << 8) | (len2 << 16)
	return PacketHeader{BodyLen: bodyLen, SeqNumber: uint(data[3])}
}

const packetHeaderLen = 4

type Packet interface {
	Decode() []byte
	Encode(r io.Reader) error
}

const (
	CLIENT_SECURE_CONNECTION = 0x00008000
	CLIENT_PLUGIN_AUTH       = 0x00080000
)

type GreetingPacket struct {
	PacketHeader
	VersionNum        uint8
	Version           string
	InternalID        uint32
	AuthData          string
	CapabilityFlag    uint32
	CharacterSet      uint8
	StatusFlag        uint16
	AuthPluginDataLen uint8
	AuthPluginName    string
}

func (pk *GreetingPacket) Decode() []byte {
	dataLen := packetHeaderLen + len(pk.Version) + 45
	data := make([]byte, dataLen)

	data[0] = byte(pk.PacketHeader.BodyLen & 0xff)
	data[1] = byte((pk.PacketHeader.BodyLen >> 8) & 0xff)
	data[2] = byte((pk.PacketHeader.BodyLen >> 16) & 0xff)
	data[3] = byte(pk.PacketHeader.SeqNumber)

	w := bufio.NewWriter(bytes.NewBuffer(data[packetHeaderLen:]))
	_ = binary.Write(w, binary.LittleEndian, pk.VersionNum)
	_ = binary.Write(w, binary.LittleEndian, pk.Version)
	return data
}

func (pk *GreetingPacket) Encode(r io.Reader) error {
	header := make([]byte, packetHeaderLen)
	br := bufio.NewReader(r)
	if n, err := br.Read(header); err != nil || n != packetHeaderLen {
		return errors.New("broken packet header")
	}

	pk.PacketHeader = mustPacketHeader(header)

	var err error

	if err = binary.Read(br, binary.LittleEndian, &pk.VersionNum); err != nil {
		return err
	}

	if pk.Version, err = br.ReadString(0); err != nil {
		return err
	}

	if err = binary.Read(br, binary.LittleEndian, &pk.InternalID); err != nil {
		return err
	}

	if pk.AuthData, err = br.ReadString(0); err != nil {
		return err
	}

	var capabilityFlag uint16
	if err = binary.Read(br, binary.LittleEndian, &capabilityFlag); err != nil {
		if err == io.EOF {
			return nil
		}
		return err
	}
	pk.CapabilityFlag = uint32(capabilityFlag)

	if err = binary.Read(br, binary.LittleEndian, &pk.CharacterSet); err != nil {
		return err
	}

	if err = binary.Read(br, binary.LittleEndian, &pk.StatusFlag); err != nil {
		return err
	}

	if err = binary.Read(br, binary.LittleEndian, &capabilityFlag); err != nil {
		return err
	}
	pk.CapabilityFlag |= uint32(capabilityFlag) << 16

	if err = binary.Read(br, binary.LittleEndian, &pk.AuthPluginDataLen); err != nil {
		return err
	}

	var reserved [10]byte
	if err = binary.Read(br, binary.LittleEndian, &reserved); err != nil {
		return err
	}

	if (pk.CapabilityFlag & uint32(CLIENT_SECURE_CONNECTION)) != 0 {
		dataLen := pk.AuthPluginDataLen - 8
		if dataLen < 13 {
			dataLen = 13
		}
		authData := make([]byte, dataLen)
		if n, err := br.Read(authData); err != nil || n != int(dataLen) {
			return err
		}
		pk.AuthData += string(authData)
	}

	if (pk.CapabilityFlag & uint32(CLIENT_PLUGIN_AUTH)) != 0 {
		if pk.AuthPluginName, err = br.ReadString(0); err != nil {
			return err
		}
	}
	return nil
}
