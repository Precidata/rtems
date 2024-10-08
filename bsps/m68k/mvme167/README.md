MVME167
=======
This is a README file for the MVME167 port of RTEMS 4.5.0.

Please send any comments, improvements, or bug reports to:

Charles-Antoine Gauthier
charles.gauthier@nrc.ca

or

Darlene Stewart
Darlene.Stewart@nrc.ca
  
Software Engineering Group
Institute for Information Technology
National Research Council of Canada
Ottawa, ON, K1A 0R6
Canada


Disclaimer
----------

The National Research Council of Canada is distributing this RTEMS
board support package for the Motorola MVME167 as free software; you
can redistribute it and/or modify it under terms of the GNU General
Public License as published by the Free Software Foundation; either
version 2, or (at your option) any later version.  This software is
distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
for more details. You should have received a copy of the GNU General
Public License along with RTEMS; see file COPYING. If not, write to
the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

Under no circumstances will the National Research Council of Canada
nor Her Majesty the Queen in right of Canada assume any liablility
for the use this software, nor any responsibility for its quality or
its support.


Installation
------------

Nothing unique to the MVME167. It uses the standard build process for
m68k targets. You will need to edit linkcmds to put in the start address
of your board. We do TFTP transfers to our target. The mvme167.cfg file
builds only the ELF images, which we download to the target, skipping
over the first 0x54 bytes; Motorola S-records are not generated. Edit
this file if you want S-records.


Port Description

Console driver
---------------

This BSP includes an termios-capable interrupt-driven I/O console driver
that supports all four serial ports on the MVME167 model. The port labelled
Serial Port 1/Console on the MVME712 is normally used by 167Bug; do not open
/dev/tty00 if you are debugging using 167Bug.

Limited support is provided for polled terminal I/O. This is used when
running the timing tests, and by the printk() debug output function.
Polled I/O may use termios, or it may bypass those services. The printk()
function does not use termios. When polled I/O is used, the terminal settings
must be set through 167-Bug; trying to change the line settings through RTEMS
has no effect.

Three is no support for using interrupt-driven I/O without termios support.

The default configuration is to use polled I/O and to bypass termios. This
is done so the test can be built at the same time as the rest of the system.
It is highly recommended that the defaults be changed in the mvme167.cfg file
to reflect the desired defaults, or that the appropriate parameters be set up
in NVRAM to select the appropriate I/O modes at boot time.

When configured for interrupt-driven I/O, the console is initialized with
whatever parameters are set up in termios before it calls the firtOpen driver
callback, EXCEPT THAT HARDWARE HANDSHAKING IS TURNED OFF, i.e. CLOCAL is set
in the struct termios c_cflag field. We use 3-wire cables for I/O, and find
hardware handshaking a pain. If you enable hardware handshaking, you must drive
CTS* low on the CD2401 for output to occur. If the port is in the DTE
configuration, you must drive the RS-232 CTS line to space; if the port is
in the DCE configuration, you must drive the RS-232 RTS line to space.

To use interrupt-driven I/O, set the CD2401_IO_MODE manifest constant to 1 in
rtems/make/custom/mvme167.cfg, or configure the appropriate parameter in
User Area Non-volatile RAM. See the Configuration Parameters section below
for instructions on setting up NVRAM.

To use termios, set the CD2401_USE_TERMIOS manifest constant to 1 in
rtems/make/custom/mvme167.cfg, or configure the appropriate parameter in
User Area Non-volatile RAM. See the Configuration Parameters section
below for instructions on setting up NVRAM.

The RTEMS console, i.e. the port used by stdin, stdout and stderr (do not
confuse it with the port labelled Console on the MVME712), must be
specified in the rtems/make/custom/mvme167.cfg file, or in the NVRAM
parameters. Set the value of CONSOLE_MINOR appropriately. See below for a
list of choices. See the Configuration Parameters section below for
instructions on setting up NVRAM.

The RTEMS printk port, i.e. the port where printk sends it debugging output
text, must be specified in the rtems/make/custom/mvme167.cfg file, or in the
NVRAM parameters. Set the value of PRINTK_MINOR appropriately. See below for a
list of choices. See the Configuration Parameters section below for
instructions on setting up NVRAM.

Interrupt-driven and polled I/O cannot be mixed in the MVME167, except that
printk always used polled I/O without termios. If interrupt-driven I/O is
used and printk is used, do not open the device that printk uses from an
RTEMS application.

Console and printk port choices:

```
  0 - /dev/tty0, Serial Port 1/Console on the MVME712M.
  1 - /dev/tty1, Serial Port 2/TTY01 on the MVME712M.
  2 - /dev/tty2, Serial Port 3 on the MVME712M.
  3 - /dev/tty3, Serial Port 4 on the MVME712M.
```

Setting the RTEMS console to port 0 when interrupt-driven I/O is specified
will prevent 167-Bug from using that port.

To use polled I/O on port 2 or 3, the port must be configured in 167-Bug. See
the "PF" command in the "Debugging Package for Motorola 68K CISC CPUs User's
Manual", part number 68KBUG.


Floating-point
--------------

The MC68040 has a built-in FPU. This FPU does not implement all the
instruction of the MC68881/MC68882 floating-point coprocessors in
hardware. The -m68040 compilation options instructs gcc to not generate
the missing instructions. All of the RTEMS code is built this way. Some
of the missing functionality must be supplied by external libraries. The
required functions are part of libgcc.a.

The issue gets complicated because libc, libm and libgcc do not come as
m68040-specific variants. The default variants of these libraries are for the
MC68020 and MC68030. There are specific variants for the MC68000 (which has
limited addressing modes with respect to later family members), and specific
variants for systems without a floating-point unit, either a built-in FPU or
a coprocessor. These latter variants will be referred to as the msoft-float
variants. There is a msoft-float variant for the MC68000, and one for the
other family members.

The default variants of libc, libm and libgcc appear to work just fine for the
MC68040, AS LONG AS NO FLOATING POINT FUNCTIONS ARE CALLED. In particular,
printf() and scanf() raise unimplemented floating-point instruction exceptions
at run time. Expect almost every function that must compute a floating-point
result to also raise unimplemented floating-point instruction exceptions. Do
not use these variants if your application does any floating-point operations,
unless you use the Motorola FPSP package (described further down).

The msoft-float variants do print out floating-point numbers properly, but we
have not tested them extensively, so use them with caution. In particular,
the Paranoia test fails when linked with the msoft-float variants of the
libraries; it goes into an infinite loop after milestone 40.

MSOFT_FLOAT VARIANTS MUST BE USED TOGETHER. If you use the msoft-float variant
of libc and libm, you must also linked with the msoft-float variant of libgcc,
otherwise calls such as printf() print out floating-point values incorrectly.

RTEMS comes with the Motorola FPSP (Floating-Point Support Package) for the
MC68040 (rtems/c/src/lib/libcp/m68k/m68040/fpsp). This package emulates the
missing floating-point instructions. It is built automatically for the
MVME167 and installed in bsp_start().

The FPSP allows the use of the default variants of libc, libm and libgcc.
It also runs the paranoia test properly, and prints out the correct results.
It should probably be used in preference to the msoft-float libraries, as it
appears to work better. The disadvantage of the FPSP is that it increases the
size of the executable by about 60KB and that it relies on run time
exceptions.

If your application does not do any floating-point operations at all, you
should consider disabling the FPSP. In bsp_start(), emove the call to
M68KFPSPInstallExceptionHandlers(), and uncomment the three lines in
mvme167.cfg that redefine which variants of libc, libm and libgcc to link
against.


Configuration Parameters
------------------------

If Jumper J1-4 is installed, certain configuration parameters may be read from
the first 31 bytes of User Area NVRAM starting at 0xFFFC0000. In this case, the
remaining J1-[5-7] jumpers are ignored, and the user is responsible for writing
the appropriate values in NVRAM (via 167-Bug) in order to alter the default
behaviour. A zero value in NVRAM results in the default behaviour. The paramaters
that are configurable and their default settings are described below. 
		
	Cache Mode (0xFFFC0000 - 1 byte)
	  Set the following bits in the byte to set the desired cache mode:
	    bit 0
	      0 - data cache disable
	      1 - data cache enable
	    bit 1
	      0 - instruction cache disable
	      1 - instruction cache enable
	    bits 2 & 3:
		    00 = cachable, write-through
		    01 = cachable, copyback
		    10 = noncachable, serialized
		    11 = noncachable

	Console driver I/O mode (0xFFFC0001 - 1 byte)
	  Set the following bits in the byte to set the desired I/O mode:
	    bit 0
	      0 - do not use termios
	      1 - use termios
	    bit 1
	      0 - polled I/O
	      1 - interrupt-driven I/O

	Console driver ports (0xFFFC0002 - 1 byte)
	  Set the following bits in the byte to select the console and printk ports:
	    bit 0 & 1 select the RTEMS console port
        00 - /dev/tty0, Serial Port 1/Console on the MVME712M.
        01 - /dev/tty1, Serial Port 2/TTY01 on the MVME712M.
        10 - /dev/tty2, Serial Port 3 on the MVME712M.
        11 - /dev/tty3, Serial Port 4 on the MVME712M.
	    bit 4 & 5 select the RTEMS printk port
        00 - /dev/tty0, Serial Port 1/Console on the MVME712M.
        01 - /dev/tty1, Serial Port 2/TTY01 on the MVME712M.
        10 - /dev/tty2, Serial Port 3 on the MVME712M.
        11 - /dev/tty3, Serial Port 4 on the MVME712M.
    If the printk port is the same as some other port that will be opened by an
    RTEMS application, then the driver must use polled I/O, or the printk port
    must not be used.
    
	IP Address (0xFFFC0004 - 4 bytes)
		write the hexadecimal representation of the IP address of the board in this
		locatio, e.g. 192.168.1.2 = 0xC0A80102
		default: obtain the IP address from an rtems_bsdnet_ifconfig structure

	Netmask (0xFFFC0008 - 4 bytes)
		write the hexadecimal representation of the netmask in this location
		for example, 255.255.255.0 = 0xFFFFFF00
		default: obtain the netmask from an rtems_bsdnet_ifconfig structure
		
	Ethernet Address (0xFFFC000C - 6 bytes)
		write the Ethernet address of the board in this location
		default: obtain the hardware address from an rtems_bsdnet_ifconfig
		         structure

	Processor ID (0xFFFC0012 - 2 bytes)
		reserved for future use
		
	RMA start (0xFFFC0014 - 4 bytes)
		reserved for future use
		
	VMA start (0xFFFC0018 - 4 bytes)
		reserved for future use
		
	RamSize (0xFFFC001C - 4 bytes)
		reserved for future use


Cache Control and Memory Mapping
--------------------------------

If configuration is not obtained from non-volatile RAM (ie. J1-4 is off),
cache control is done through the remaining J1 jumpers as follows:

If Jumper J1-7 is installed, the data cache will be turned on. If Jumper
J1-6 is installed, the instruction cache will be turned on. (If a jumper
is off, its corresponding cache will remain disabled).

If Jumper J1-5 is installed, the data cache will be placed in copyback
mode. If it is removed, it will be placed in writethrough mode.

Currently, block address translation is set up to map the virtual
0x00000000--0x7FFFFFFF to the physical range 0x00000000--0x7FFFFFFF. The
port relies on the hardware to raise exceptions when addressing
non-existent memory. Caching is not controllable on a finer grain.


Networking
----------

If configuration is not obtained from non-volatile RAM (ie. J1-4 is off),
the networking parameters shown above must be specified in an initialized
rtems_bsdnet_ifconfig struct. This structure is declared and initialized to
specify any network devices and includes entries for ip_address, ip_netmask
and hardware_address. See the Network Device Configuration section of the
RTEMS Networking Supplement.

When non-default (non-zero) networking paramaters are provided in NVRAM (ie.
j1-4 is on), the user MUST ensure that the corresponding entries in the
ifconfig struct are NULL. Failing to do so is an error, because it causes
the memory allocated for the initialized struct values to be lost.


Miscellaneous
-------------

The timer and clock drivers were patterned after the MVME162 and MVME152
ports.

At this time, we do not have an MPCI layer for the MVME167. We are planning
to write one.

This port supplies its own fatal_error_handler, which attempts to print some
error message through 167Bug (on the Serial Port 1/Console on the MVME712M).


Host System
-----------

The port was initially developed on an RS-6000 running AIX 4.2. The following
tools were used:

    - GNU gcc 2.8.1 configured for a powerpc-ibm-aix4.2.0.0 host and
      m68k-rtems target;
    - GNU binutils 2.9.1 configured for a powerpc-ibm-aix4.2.0.0 host and
      m68k-rtems target;

It was also tested on a Pentium II-based PC running Windows NT Workstation 4.0
and the Cygnus Cygwin32 release b20.1 environment, with the following tools:

    - EGCS 1.1.1 configured for a i586-cygwin32 host and m68k-rtems target;
    - GNU binutils 2.9.4 configured for a i586-cygwin32 host and m68k-rtems
      target;

With the latter environment, be patient; builds take a very looong time...

Current development is done on a Pentium III PC running RedHat Linux 6.1.
At the time this README was composed, the latest working compiler that was
used successfully was gcc version 2.96 20000213 (experimental). Both the C
and C++ compilers were working. Binutils 2.10 are used.


Known Problems
--------------

Polled I/O without termios may not work very well on input. The problem
is that input processing is not done: applications may get characters too
early, and may get characters that they normally would not get, such as
backspace or delete. Furthermore, input is not buffered at all. The latest
versions of rtems seem to set the count field in the rtems_libio_rw_args_t
argument to the buffer size, not to the number of characters expected on
input. Rather than wait for 1024 characters on each call, the driver
returns each character when it is received.

The cdtest will not run with interrupt-driven I/O. The reason is that the
constructors for the static objects are called at boot time when the
interrupts are still disabled. The output buffer fills up, but never empties,
and the application goes into an infinite loop waiting for buffer space. This
should have been documented in the rtems/c/src/tests/PROBLEMS file. The moral
of this story is: do not do I/O from the constructors or destructors of static
objects.

Output stops prematurely in the termios test when the console is operating in
interrupt-driven mode because the serial port is re-initialized before all
characters in the last raw output buffer are sent. Adding calls to tcdrain()
in the test task helps, but it does not solve the problem. What happens is
that the CD2401 raises a transmit interrupt when the last character in the
DMA buffer is written into the transmit FIFO, not when the last character
has been transmitted. When tcdrain() returns, there might be up to 16
characters in the output FIFO. The call to tcsetattr() causes the serial port
to re-initialize, at which point the output FIFO is cleared. We could not find
a way to detect whether characters are still in the FIFO and to wait for them
to be transmitted.

The first raw buffer to be transmitted after the console is re-initialized
with tcsetattr() is garbled. At this time, it does not seem worth while to
track this problem down.

In the stackchk test, an access fault exception is raised after the stack is
blown. This is one case were overwritting the first or last 16 bytes of the
stack does cause problems (but hey, an exception occurred, which is better
than propagating the error).

In the stackchk test, an access fault exception is raised after the stack is
blown. This is one case were overwritting the first or last 16 bytes of the
stack does cause problems (but hey, an exception occurred, which is better
than propagating the error).

When using interrupt-driven I/O, psx08 produces all the expected output, but
it does not return control to 167Bug. Is this test supposed to work with
interrupt-driven console I/O?


What is new
-----------

Support for Java is being actively worked on.


Thanks
------

- to On-Line Applications Research Corporation (OAR) for developing
RTEMS and making it available on a Technology Transfer basis;

- to FSF and Cygnus Support for great free software;


Test Configuration
------------------

Board:                Motorola MVME167
CPU:                  Motorola MC68040
Clock Speed:          25 MHz
RAM:                  4 MBytes of 32-bit DRAM with parity
Cache Configuration:  Instruction cache on; data cache on, copyback mode.
Times Reported in:    microseconds
Timer Source:         VMEchip2 Tick Timer 1
GCC Flags:            -m68040 -g -O4 -fomit-frame-pointer
Console:              Operate in polled mode. Set CD2401_POLLED_IO to 1 in
                      rtems/c/src/lib/libbsp/m68k/mvme167/console/console.c.     


Test Results
------------

Single processor tests:  All tests passed, except the following ones:

  - paranoia required the FPSP and the default variants of libm (and libc and
    libgcc) for us. It may work with the msoft-float variants for you, but it
    does require the FPSP.
  
  - cpuuse and malloctest did not work.
  
  - The stackchk test got an access fault exception before the RTEMS stack
    checker had had a chance to detect the corrupted stack.
                         
                         
Multi-processort tests:  not applicable -- No MPCI layer yet.

Timing tests: See `rtems/c/src/lib/libbsp/m68k/mvme167/times`
