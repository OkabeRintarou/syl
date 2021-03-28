package torrent

import (
    "errors"
    common2 "github.com/OkabeRintarou/iTorrent/pkg/common"
    "time"
)

var InvalidTorrent = errors.New("invalid torrent file")

type FileInfo struct {
    Name   string
    Length uint64
    Md5Sum string
}

type FileMeta struct {
    PieceLength uint64
    Pieces      [][]byte
    Private     bool
    Infos       []FileInfo
}

type Torrent struct {
    Info      FileMeta
    Announces []string
    CreateData time.Time
    Comment    string
    CreatedBy  string
}

func parseSingleFileMeta(data interface{}, name string, meta *FileMeta) error {
    length, err := common2.BencodeGetInt(data, "length")
    if err != nil {
        return err
    }

    meta.Infos = make([]FileInfo, 1, 1)
    meta.Infos[0].Name = name
    meta.Infos[0].Length = uint64(length)

    if md5sum, err := common2.BencodeGetString(data, "md5sum"); err != nil && len(md5sum) == 32 {
        meta.Infos[0].Md5Sum = md5sum
    }

    return nil
}

func parseMultiFileMeta(data, files interface{}, dirname string, meta *FileMeta) error {
    return nil
}

func parseFileMeta(data interface{}, meta *FileMeta) error {
    pieceLen, err := common2.BencodeGetInt(data, "piece length")
    if err != nil {
        return err
    }
    name, err := common2.BencodeGetString(data, "name")
    if err != nil {
        return err
    }

    pieces, err := common2.BencodeGetString(data, "pieces")
    if err != nil {
        return err
    }
    if (len([]byte(pieces)) % 20) != 0 {
        return errors.New("pieces length must be a multiple of 20")
    }
    meta.Pieces = make([][]byte, 0, len([]byte(pieces))/20)
    b := []byte(pieces)
    for len(b) > 0 {
        meta.Pieces = append(meta.Pieces, b[:20:20])
        b = b[20:]
    }

    meta.PieceLength = uint64(pieceLen)
    if files, err := common2.BencodeGet(data, "files"); err != nil {
        return parseSingleFileMeta(data, name, meta)
    } else {
        return parseMultiFileMeta(data, files, name, meta)
    }
}

func NewTorrentFromInterface(info interface{}) (*Torrent, error) {
    if d, ok := info.(map[string]interface{}); ok {
        for k := range d {
            println(k)
        }
    }
    n := make([]string, 0, 1)
    announce, err := common2.BencodeGetString(info, "announce")
    if err != nil {
        return nil, InvalidTorrent
    }
    n = append(n, announce)

    meta, err := common2.BencodeGet(info, "info")
    if err != nil {
        return nil, InvalidTorrent
    }
    t := &Torrent{
        Announces: n,
    }

    if err = parseFileMeta(meta, &t.Info); err != nil {
        return nil, InvalidTorrent
    }
    comment, _ := common2.BencodeGetString(info, "comment")
    create, _ := common2.BencodeGetInt(info, "creation date")
    _, _ = comment, create
    return nil, nil
}

func NewTorrent(content string) (*Torrent, error) {
    encode, err := common2.Decode(content)
    if err != nil {
        return nil, err
    }
    return NewTorrentFromInterface(encode)
}
