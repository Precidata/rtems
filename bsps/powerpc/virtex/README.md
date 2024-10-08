virtex
======
```
BSP NAME:           Virtex
BOARD:              Xilinx ML-403 and (hopefully) any vitex/PPC based board
BUS:                N/A
CPU FAMILY:         ppc
CPU:                PowerPC 405GP
COPROCESSORS:       N/A
MODE:               32 bit mode

DEBUG MONITOR:
```

PERIPHERALS
-----------
``` 
TIMERS:             405GP internal
SERIAL PORTS:       Xilinx consolelite
REAL-TIME CLOCK:    none
DMA:                Xilinx vitex internal
VIDEO:              none
SCSI:               none
NETWORKING:         Xilinx TEMAC
``` 

DRIVER INFORMATION
------------------
``` 
CLOCK DRIVER:       PPC Decrementer
IOSUPP DRIVER:      N/A
SHMSUPP:            N/A
TIMER DRIVER:       N/A
TTY DRIVER:         consoleelite
``` 

STDIO
-----
``` 
PORT:               Console port 0
ELECTRICAL:         RS-232
BAUD:               as defined in FPGA design
BITS PER CHARACTER: 8
PARITY:             None
STOP BITS:          1
``` 

Board description
-----------------
``` 
clock rate:	234 MHz
ROM:		16MByte FLASH
RAM:		64MByte DRAM
``` 

Virtex only supports single processor operations.

Configuration
-------------
This board support package is written for a typical virtex/PPC FPGA
system. The rough features of such a board are described above.

When a new virtex FPGA system is created (using the Xilinx design
software), a parameter file "xparameters.h" is also created, which
describes the basic features of the hardware (like peripherals
included, interrupt routing etc.).

This BSP normally takes its basic HW description for the file
"xparameters_dflt.h", which describes my FPGA system. When this BSP
should run on a different hardware, a path to the proper
"xparameters.h" can be provided on the "configure" command line.

For adapting this BSP to other boards, you can specify several configuration
options at the configure command line (use "./configure --help" in this
directory).  Here is an example for the top-level configure invocation:

```shell
/path/to/rtems/sources/configure \
  --target=powerpc-rtems4.11 \
  --enable-rtemsbsp=virtex \
  --enable-maintainer-mode \
  --enable-posix \
  --enable-tests \
  --disable-networking \
  'RTEMS_XPARAMETERS_H="/path/to/xparameters.h"' \
  VIRTEX_RAM_ORIGIN=0xfffc0000 \
  VIRTEX_RAM_LENGTH=0x3ffec \
  VIRTEX_RESET_ORIGIN=0xffffffec
```
