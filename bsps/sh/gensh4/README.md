gensh4
======

 Author: Alexandra Kossovsky <sasha@oktet.ru>
         Victor Vengerov     <vvv@oktet.ru>
  OKTET Ltd, http://www.oktet.ru


```
BSP NAME:           generic SH4 (gensh4)
BOARD:              n/a
BUS:                n/a
CPU FAMILY:         Hitachi SH
CPU:                SH 7750
COPROCESSORS:	    none
MODE:               n/a

DEBUG MONITOR:      gdb (sh-ipl-g+ loader/stub)
```

PERIPHERALS
-----------
```
TIMERS:             on-chip
SERIAL PORTS:       on-chip (with 2 ports)
REAL-TIME CLOCK:    none
DMA:                not used
VIDEO:              none
SCSI:               none
NETWORKING:         none
```

DRIVER INFORMATION
------------------
```
CLOCK DRIVER:       on-chip timer
IOSUPP DRIVER:      default
SHMSUPP:            n/a
TIMER DRIVER:       on-chip timer
TTY DRIVER:         /dev/console
```

STDIO
-----
```
PORT:               /dev/console
ELECTRICAL:         n/a
BAUD:               n/a
BITS PER CHARACTER: n/a
PARITY:             n/a
STOP BITS:          n/a
```

NOTES
-----
(1) Driver for the on-chip serial devices is tested only with 1st serial
    port. We cannot test it on serial port with FIFO.

    Console driver has 4 modes -- 2 with termios (interrupt-driven & 
    poll-driven modes), one raw mode working with serial port directly,
    without termios, and one mode working with gdb stub (using 'trapa'
    handled by sh-ipl-g+).

(2) The present 'hw_init.c' file provides 'early_hw_init'(void) which
    is normally called from 'start.S' to provide such minimal HW setup.
    It is written in C, but it should be noted that any accesses to memory
    (except hardware registers) are prohibited until hardware not
    initialized. To avoid access to stack, hw_init.c should be compiled with
    -fomit-frame-pointer.

    hw_init.c also provides 'bsp_cache_on'(void) normally called from
    'start.S' after copying all data from rom to ram.

(3) In 'configure.ac' you should properly set 'CPU_CLOCK_RATE_HZ'.
    It is frequency fed to the CPU core (external clock frequency can be
    multiplied by on-chip PLLs). Please note that it is not a frequency of
    external oscillator! See Hardware Manual, section 10, for details.
    Global variable 'SH4_CPU_HZ_Frequency' is declared in 'bsp.h' and 
    initilized in 'bspstart.c' to ${HZ}. It is used by sci driver, 
    which exists in 'libcpu/sh/sh7750'. 

(4) There is SH4_WITH_IPL macro in console driver 'sh4_uart.h'. 
    When it is defined, the application works under 
    gdb-stub (it is able to turn cache on by 'trapa', use gdb mode in console
    driver and get out from gdb to use other console modes).

(5) There are 3 likcmds: 
    - linkcmds: code and data loaded to RAM. No code/data moving required.
    - linkcmds.rom: code executed from the ROM; .data section moved to the
    RAM on initialization.
    - linkcmds.rom2ram: execution started from the ROM (after reset); code
    and data moved to the RAM and execution continued from RAM.

    The same 'start.S' is used for all cases.

(6) You can get gdb stub from http://www.oktet.ru/download/sh4/sh-ipl.tar.gz.
    It is based on 'sh-ipl-g+' package used in sh-linux project.

(7) This project was done in cooperation with Transas company
    http://www.transas.com
