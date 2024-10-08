virtex5
=======
Adapted from virtex BSP

```
BSP NAME:           virtex5
BOARD:              N/A
BUS:                N/A
CPU FAMILY:         ppc
CPU:                PowerPC 440x5
COPROCESSORS:       N/A
MODE:               32 bit mode

DEBUG MONITOR:
```

PERIPHERALS
-----------
```
TIMERS:             440 internal
SERIAL PORTS:       none
REAL-TIME CLOCK:    none
DMA:                Xilinx virtex internal
VIDEO:              none
SCSI:               none
NETWORKING:         none
```

DRIVER INFORMATION
------------------
```
CLOCK DRIVER:       PPC Decrementer
IOSUPP DRIVER:      N/A
SHMSUPP:            N/A
TIMER DRIVER:       N/A
TTY DRIVER:         N/A
```

STDIO
-----
```
PORT:               N/A
ELECTRICAL:         N/A
BAUD:               N/A
BITS PER CHARACTER: N/A
PARITY:             N/A
STOP BITS:          N/A
```

Board description
-----------------
clock rate:	465 MHz
ROM:		N/A
RAM:		4GByte DRAM

Virtex only supports single processor operations.

Porting
-------
This board support package is written for a naked Virtex 5/PPC FPGA
system. The rough features of such a board are described above.
The BSP itself makes no assumptions on what is loaded in the FPGA,
other than that the CPU has access to some memory, either on-board
or external, from which code can be run.

This BSP has been constructed so that an application of both firmware
and software can be layered on top of it by supplying implementations
for the various 'weak' symbols.  These symbols are prefaced with the
term 'app_'.  Applications can thus be built outside of the RTEMS
directory tree by linking with the appropriate libraries.

The linkcmds file describes the memory layout.  Included in this
definition is a section of memory named MsgArea.  Output sent to
stdout is recorded in this area and can be dumped using the JTAG
interface, for example.

For adapting this BSP to other boards, the following files should be
modified:

- c/src/lib/libbsp/powerpc/virtex5/startup/linkcmds
	for the memory layout required

- c/src/lib/libbsp/powerpc/virtex5/startup/bspstart.c
	Here you can select the clock source for the timers and the
        serial interface (system clock or external clock pin), the
        clock rates, etc.

- c/src/lib/libbsp/powerpc/virtex5/include/bsp.h
	some BSP-related constants

- c/src/lib/libbsp/powerpc/virtex5/*
	well, they should be generic, so there _should_ be no reason
	to mess around there (but who knows...)
