Intec SS555
===========
The SS555 port was sponsored by Defence Research and Development 
Canada - Suffield, and is Copyright (C) 2004, Real-Time Systems Inc.

Please send any comments, improvements, or bug reports to:

David Querbach
querbach@realtime.bc.ca


Summary
-------
```
BSP NAME:           ss555
BOARD:              Intec Automation Inc. SS555
BUS:                None 
CPU FAMILY:         PowerPC
CPU:                PowerPC MPC555
COPROCESSORS:       Built-in Motorola TPU
MODE:               32 bit mode

DEBUG MONITOR:      None
```

PERIPHERALS
-----------
```
TIMERS:             PIT / Timebase
  RESOLUTION:       1 microsecond (4 MHz crystal / 4)
SERIAL PORTS:       2 SCI
REAL-TIME CLOCK:    On-chip.
DMA:                None.
VIDEO:              None.
SCSI:               None.
NETWORKING:         None.
```

DRIVER INFORMATION
------------------
```
CLOCK DRIVER:       yes
CONSOLE DRIVER:     yes
SHMSUPP:            N/A
TIMER DRIVER:       yes
NETWORK DRIVER:	    no
```

NOTES
-----
```
On-chip resources:
	SCI1		serial port
	SCI2		serial port (console)
	PIT		clock
	TB		timing test support
	DEC
	SWT		watchdog timer -- enable in startup/iss555.c
	*CS0		external 512k 2-1-1-1 Burst SRAM
	*CS1
	*CS2
	*CS3		CPLD
	IRQ0
	IRQ1
	IRQ2		
	IRQ3		
	IRQ4		
	IRQ5	
	IRQ6
	IRQ7
	IRQ_LVL0	
	IRQ_LVL1
	IRQ_LVL2
	IRQ_LVL3
	IRQ_LVL4
	IRQ_LVL5	SCI
	IRQ_LVL6	PIT
	IRQ_LVL7
```

Board description
-----------------
``` 
Clock rate:     40 MHz development/embeddable board
Bus width:      32-bit on-board RAM, 16-bit off-board I/O
FLASH:	        512k on-chip
RAM:            512k 2-1-1-1 burst SRAM
```


Installation
------------
The ss555 port uses the Intec SS555's external RAM in two different ways,
depending on whether or not it is built for debugging by giving the
VARIANT=DEBUG switch to make:

```shell
	make VARIANT=DEBUG all
```

1.  In the debugging case, the linker script concanttenates the .text,
.data, and .bss sections starting at location zero, so they all can be
loaded into the external RAM for debugging.  We assume that the debugger
disables the internal Flash ROM and enables the external RAM before loading
the code into the external RAM.

2.  In the normal (non-debugging) case, the linker script still places the
.text section near zero, but places the start of the .data and .bss sections
at the start location of the MPC555's internal RAM.  The system startup code
then configures the external RAM just after the internal RAM in memory,
forming one large block from the two RAM devices.


Console driver
--------------
This BSP includes an termios-capable asynchronous serial line driver that
supports SCI1 and SCI2. The RTEMS console is selected at configuration time
with the CONSOLE_MINOR variable (see .../ss555/configure.ac).  We default to
SCI2 for the console, since SCI1 has some extra features which may be
desired for application use.

The BSP console supports three different modes of operation:

  1. polled I/O done with termios support,
  2. polled I/O done without termios support,
  3. interrupt-driven I/O with termios support.
     
The mode of operation of the serial driver is determined at configure time in
part by the value of the UARTS_IO_MODE variable (see .../ss555/configure.ac).

  0 - polled I/O.
  1 - interrupt-driven I/O.
  
Also, set the value of UARTS_USE_TERMIOS to select whether termios should be
used to perform buffering and input/output processing. Without termios
support, input processing is limited to the substitution of LF for a
received CR, and output processing is limited to the transmission of a CR
following the transmission of a LF. The choices for UARTS_USE_TERMIOS are:

  0 - do not use termios
  1 - use termios

In most real-time applications, the driver should be configured to use
termios and interrupt-driven I/O. Special requirements may dictate otherwise.

Polled I/O must be used when running the timing tests. It must also be used
to run some other tests and some samples, such as the cdtest. Some tests
change the interrupt mask and will hang interrupt-driven I/O indefinitely.
Others, such as cdtest, perform console output from the static constructors
before the console is opened, causing the test to hang. Still other tests
produce output that is supposed to be in some specific order. For these
tests, termios should not be used, as termios buffers output and the
transmission of the buffers occur at somewhat unpredictable times.

The real solution is to fix the tests so that they work with interrupt-driven
I/O and termios.


printk() and debug output
-------------------------
The implementation of printk() in RTEMS is mostly independent of most system
services. The printk() function can therefore be used to print messages to a
debug console, particularly when debugging startup code or device drivers,
i.e. code that runs before the console driver is opened or that runs with
interrupts disabled.

Support is provided to send printk output to either port. Specify the
desired port at configure time by setting the value of PRINTK_MINOR to one
of SCI1_MINOR or SCI2_MINOR.

printk() always uses polled I/O, and never uses termios.

If the printk() port is opened by RTEMS, then UARTS_IO_MODE must be set for
polled I/O, otherwise the I/O functions will be in conflict.  Using printk()
before the port is initialized is, of course, not possible.  This
initialization occurs in console_initialize(), which is called by
rtems_initialize_executive_early().


Watchdog Timer
--------------
The MPC555 watchdog timer can be enabled at configuration time by defining
the WATCHDOG_TIMEOUT variable.  This variable sets the watchdog timeout
period in steps of

	  2048          2048
	---------  =  ---------  =  51.2 usec
	 Fsystem       40 MHz

or about 1/20 msec.  When WATCHDOG_TIMEOUT is left undefined, the watchdog
timer is disabled.


Miscellaneous
-------------
Most code came from the mbx8xx port, except for the floating-point handling
which came from the mpc8260ads.


Host System
-----------
The port was developed on an x86 box running Debian 3.0.  The toolchain was
built from the sources at rtems.org, except for the autotools which came
from the Debian distribution.


Test Configuration
------------------
```
Board:                Intec SS555 v1.1
CPU:                  Motorola MPC555LFMZP40, mask 1K83H
Clock Speed:          Crystal 4.0 MHz, CPU 40.0 MHz
RAM:                  512K bytes of 2-1-1-1 Burst SRAM
Times Reported in:    Microseconds
Timer Source:         Timebase clock
GCC Flags:            -O4 -fno-keep-inline-functions -mcpu=(821/860) 
Console:              Operates in polled mode on SMC2. No I/O through EPPC-Bug.    
```

Test Results
------------
Single processor tests:  
  All tests passed, except that:
    - sp09 aborts due to memory shortage
    - sp20 needs to be run with output buffering enabled (see
      buffer_test_io.h)

Multi-processor tests:  
  Not applicable.

Library tests:
  All tests passed.  Note that the termios test only works if the system is
  rebuilt with termios enabled.                         

Posix tests:
  All tests passed, except that:
    - the message queue test failed with "errno (91 - File or path name too
      long)"

Timing tests:
  Due to memory limitations, many of the tests will not run unless you set
  OPERATION_COUNT=20 at configuration time.

  To run tm27 (the interrupt latency timer test), short CN5-48 to CN5-50 on
  the SS555 board.

  All tests run cleanly, except for tm26, which gives a "PANIC 12" after it
  exits.  This doesn't seem to cause a problem otherwise.

  See the times file for the results of the timing tests.

Network tests:  
  Not applicable.

Sample programs:
  These run correctly, except that:
    - The "minimum" sample is not designed to run properly on any system.
    - The loopback, fileio, unilimited, and pppd tests fail due to memory
      limitations.
    - The paranoia program dies on a floating-point assist exception.

Various non-BSP-dependent support routines.

timer -		Support for the RTEMS timer tick, using the Programmable
		Interval Timer (PIT).

console-generic	- Console support via the on-chip dual SCI port in the QSMCM
		module.

exception -	Installation and deinstallation of exception handlers, by
		manipulation of exception vector table.

irq -		Exception handler for all external and decrementer interrupts. 
		Generalized interrupt handler which calls specific handlers
		via entries in the interrupt connection table.  Interrupt
		connection table maintenance routines.  USIU and UIMB
		interrupt masking and level control.

timer -		Support for RTEMS timer tests, using the PowerPC timebase
		(TB) registers.

vectors -	Compressed MPC5XX exception vector table, exception handler
		prologues, default exception handler.  Code to initialize
		table with default handlers.
