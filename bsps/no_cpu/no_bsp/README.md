no_bsp BSP Example
==================

This is a sample hardware description file for a BSP.  This comment
block does not have to appear in a real one.  The intention of this
file is to provide a central place to look when searching for
information about a board when starting a new BSP.  For example,
you may want to find an existing timer driver for the chip you are
using on your board.  It is easier to grep for the chip name in
all of the HARDWARE files than to peruse the source tree.  Hopefully,
making the HARDDWARE files accurate will also alleviate the common
problem of not knowing anything about a board based on its BSP
name.

NOTE:  If you have a class of peripheral chip on board which
       is not in this list please add it to this file so
       others will also use the same name.

       Timer resolution is the way it is configured in this BSP.
       On a counting timer, this is the length of time which
       corresponds to 1 count.


```
BSP NAME:           fastsbc1
BOARD:              Fasssst Computers, Fast SBC-1
BUS:                SchoolBus
CPU FAMILY:         i386
CPU:                Intel Hexium
COPROCESSORS:       Witch Hex87
MODE:               32 bit mode

DEBUG MONITOR:      HexBug
```

PERIPHERALS
-----------
``` 
TIMERS:             Intel i8254
  RESOLUTION:         .0001 microseconds
SERIAL PORTS:       Zilog Z8530 (with 2 ports)
REAL-TIME CLOCK:    RTC-4
DMA:                Intel i8259
VIDEO:              none
SCSI:               none
NETWORKING:         none
``` 

DRIVER INFORMATION
------------------
``` 
CLOCK DRIVER:       RTC-4
IOSUPP DRIVER:      Zilog Z8530 port A
SHMSUPP:            polled and interrupts
TIMER DRIVER:       Intel i8254
TTY DRIVER:         stub only
``` 

STDIO
-----
``` 
PORT:               Console port 0
ELECTRICAL:         RS-232
BAUD:               9600
BITS PER CHARACTER: 8
PARITY:             None
STOP BITS:          1
``` 

NOTES
-----

(1) 900 Mhz and 950 Mhz versions.

(2) 1 Gb or 2 Gb RAM.

(3) PC compatible if HexBug not enabled.
