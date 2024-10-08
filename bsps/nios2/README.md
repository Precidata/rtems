nios2
=====

 Goal is to have BSPs build almost completely automatically from a template
 and information that comes from SOPC Builder as a .PTF file. Most of the
 code will go to a shared/ BSP directory.

 Ideally, updates to the PTF shouldn't cause any pain for the maintainer
 of a specific BSP (possibly with enhancements not covered by the
 automatic BSP creation).

 Some first steps toward utilizing SOPC Builder PTF output can be found
 in top level /tools/cpu/nios2. Also see the README there.

  Implemented (in shared/ subdirectory)
    Clock driver
    Timer driver
    Console via JTAG UART

  Todo;
    Support more peripherals. My priorities:
      - (improve) Altera Avalon JTAG UART
      - Altera Avalon UART
      - OpenCores.org I2C Master
      - Altera SPI Core / EPCS Configuration Device
      - OpenCores.org 10/100 Ethernet MAC (use existing driver)
      - (more) Altera Avalon Timer

    Put all drivers aside in a shared/ subdirectory.
    Update the "times" file for NIOS2 with and without icache.

  Missing (although it looks like it's there)
    Data cache handling (for now, don't use the "fast" NIOS2)
    SHM support (just taken over the code from no_cpu/no_bsp)

```
BSP NAME:           nios2_eb2_1
BOARD:              Altera Instruction Set Simulator Default plus second timer
BUS:                Avalon
CPU FAMILY:         nios2
CPU:                small
COPROCESSORS:       none
MODE:               32 bit mode

DEBUG MONITOR:      none
```

PERIPHERALS
-----------
```
TIMERS:             Altera Avalon Timer
  RESOLUTION:         .0001 microseconds
SERIAL PORTS:       Altera Avalon JTAG UART
REAL-TIME CLOCK:    none
DMA:                none
VIDEO:              none
SCSI:               none
NETWORKING:         none
```

DRIVER INFORMATION
------------------
```
CLOCK DRIVER:       Altera Avalon Timer
IOSUPP DRIVER:      none
SHMSUPP:            polled
TIMER DRIVER:       Altera Avalon Timer
TTY DRIVER:         none
```

STDIO
-----
```
PORT:               Console port 0
ELECTRICAL:         JTAG
BAUD:               115200
BITS PER CHARACTER: 8
PARITY:             None
STOP BITS:          1
```
