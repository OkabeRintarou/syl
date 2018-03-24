package snowflake

import "time"

var (
	startTime = time.Date(2017, 12, 31, 24, 0, 0, 0, time.UTC)
)

const (
	timestampsShift   = 22
	datacenterIDShift = 17
	machineIDShift    = 12
)

type SnowFlake struct {
	startTime    time.Time
	elapsed      time.Duration
	datacenterID int
	machineID    int
	idMilli      int
}

func New(datacenterID, machineID int) *SnowFlake {
	return &SnowFlake{
		startTime:    startTime,
		datacenterID: datacenterID,
		machineID:    machineID,
	}
}

func (sf *SnowFlake) id() int64 {
	return (int64(sf.idMilli) & int64(0x0fff)) |
		((int64(sf.machineID) << machineIDShift) & int64(0x1f000)) |
		((int64(sf.datacenterID) << datacenterIDShift) & int64(0x3e0000)) |
		((int64(sf.elapsed) << timestampsShift) & int64(0x7fffffffffc00000))
}

func (sf *SnowFlake) ID() int64 {
	elapsed := time.Now().Sub(sf.startTime)
	if elapsed == sf.elapsed {
                if sf.idMilli == (1 << machineIDShift) {
                        sf.elapsed += time.Duration(1)
                        sf.idMilli = 0
                } else {
		        sf.idMilli++
                }
	} else {
		sf.elapsed = elapsed
		sf.idMilli = 0
	}
	return sf.id()
}
