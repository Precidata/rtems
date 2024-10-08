GDB stub
========

Debugged this stub against the MongooseV bsp.  Relies on putting break
instructions on breakpoints and step targets- normal stuff, and does not
employ hardware breakpoint support at this time.  As written, a single
breakpoint in a loop will not be reasserted unless the user steps or has
a 2nd one, since breakpoints are only reset when the gdb stub is
re-entered.  A useful enhancement would be to fix the break instruction
management so the stub could invisibly put a 2nd break after the 1st
"official" one so it can silently reset breakpoints.  Shouldn't be too
hard, mostly a matter of working it out.

This was tested only against an R3000 MIPS.  It should work OK on a
R4000.  Needs to be tested at some point.

This stub supports threads as implemented by gdb 5 and doesn't have any
bugs I'm aware of.

Greg Menke
3/5/2002

The contents of this directory are based upon the "r46kstub.tar.gz" package
released to the net by

	C. M. Heard
	VVNET, Inc.                           phone:  +1 408 247 9376
	4040 Moorpark Ave. Suite 206          fax:    +1 408 244 3651
	San Jose, CA 95117 USA                e-mail: heard@vvnet.com

This package was released in the September 1996 time frame for use
with gdb 4.16 and an IDT R4600 Orion.   The stub was modified to support
R3000 class CPUs and to work within the mips-rtems exeception processing
framework.

The file memlimits.h could end up being target board dependent.  If
this is the case, copy it to your BSP directory and modify as necessary.


Original README 
---------------

The r46kstub directory and its compressed archive (r46kstub.tar.gz) contain
the 9/29/96 source code snapshot for a ROM-resident gdb-4.16 debug agent
(aka stub) for the IDT R4600 Orion processor.  It is based on the stub for
the Hitachi SH processor written by Ben Lee and Steve Chamberlain and
supplied with the gdb-4.16 distribution;  that stub in turn was "originally
based on an m68k software stub written by Glenn Engel at HP, but has changed
quite a bit".  The modifications for the R4600 were contributed by C. M.
Heard of VVNET, Inc. and were based in part on the Algorithmics R4000 version
of Phil Bunce's PMON program.

The distribution consists of the following files:

```
-rw-r--r--   1    1178 Sep 29 16:34 ChangeLog
-rw-r--r--   1     748 Jul 26 01:18 Makefile
-rw-r--r--   1    6652 Sep 29 16:34 README
-rw-r--r--   1    1829 May 21 02:02 gdb_if.h
-rw-r--r--   1    3745 Sep 29 14:03 ioaddr.h
-rw-r--r--   1    2906 Sep 29 14:39 limits.h
-rw-r--r--   1    6552 May 23 00:17 mips_opcode.h
-rw-r--r--   1   14017 May 21 02:04 r4600.h
-rw-r--r--   1   23874 Jul 21 20:31 r46kstub.c
-rw-r--r--   1    1064 Jul  3 12:35 r46kstub.ld
-rw-r--r--   1   13299 Sep 29 16:24 stubinit.S
```

With the exception of mips_opcode.h, which is a slightly modified version
of a header file contributed by Ralph Campbell to 4.4 BSD and is therefore
copyrighted by the UC Regents, all of the source files have been dedicated
by their authors to the public domain.  Use them as you wish, but do so
at your own risk!  The authors accept _no_ responsibility for any errors.

The debug agent contained herein is at this writing in active use at VVNET
supporting initial hardware debug and board bring-up of an OC-12 ATM probe
board.  It uses polled I/O on a 16C450 UART.  We had originally intended to
add support for interrupts to allow gdb to break in on a running program,
but we have found that this is not really necessary since the reset button
will accomplish the same purpose (thanks to the MIPS feature of saving the
program counter in the ErrorEPC register when a reset exception occurs).

Be aware that this stub handles ALL interrupts and exceptions except for
reset (or NMI) in the same way -- by passing control to the debug command
loop.  It of course uses the ROM exception vectors to do so.  In order to
support code that actally needs to use interrupts we use use a more elaborate
stub that is linked with the downloaded program.  It hooks the RAM exception
vectors and clears the BEV status bit to gain control.  The ROM-based stub
is still used in this case for initial program loading.

In order to port this stub to a different platform you will at a minimum
need to customize the macros in limits.h (which define the limits of readable,
writeable, and steppable address space) and the I/O addresses in ioaddr.h
(which define the 16C450 MMIO addresses).  If you use something other than
a 16C450 UART you will probably also need to modify the portions of stubinit.S
which deal with the serial port.  I've tried to be careful to respect all the
architecturally-defined hazards as described in Appendix F of Kane and
Heinrich, MIPS RISC Architecture, in order to minimize the work in porting
to 4000-series processors other than the R4600, but no guarantees are offered.
Support is presently restricted to big-endian addressing, and I've not even
considered what changes would be needed for little-endian support.

When this stub is built with gcc-2.7.2 and binutils-2.6 you will see a few
warning messages from the single-step support routine where a cast is used
to sign-extend a pointer (the next instruction address) into a long long
(the PC image).  Those warnings are expected;  I've checked the generated
code and it is doing what I had intended.  But you should not see any other
warnings or errors.  Here is a log of the build:

```shell
mips64orion-idt-elf-gcc -g -Wa,-ahld -Wall -membedded-data \
	-O3 -c r46kstub.c >r46kstub.L
r46kstub.c: In function `doSStep':
r46kstub.c:537: warning: cast to pointer from integer of different size
r46kstub.c:539: warning: cast to pointer from integer of different size
r46kstub.c:547: warning: cast to pointer from integer of different size
r46kstub.c:561: warning: cast to pointer from integer of different size
r46kstub.c:563: warning: cast to pointer from integer of different size
r46kstub.c:572: warning: cast to pointer from integer of different size
r46kstub.c:574: warning: cast to pointer from integer of different size
r46kstub.c:582: warning: cast to pointer from integer of different size
r46kstub.c:589: warning: cast to pointer from integer of different size
r46kstub.c:591: warning: cast to pointer from integer of different size
r46kstub.c:597: warning: cast to pointer from integer of different size
r46kstub.c:599: warning: cast to pointer from integer of different size
r46kstub.c:605: warning: cast to pointer from integer of different size
r46kstub.c:607: warning: cast to pointer from integer of different size
r46kstub.c:613: warning: cast to pointer from integer of different size
r46kstub.c:615: warning: cast to pointer from integer of different size
r46kstub.c:624: warning: cast to pointer from integer of different size
r46kstub.c:628: warning: cast to pointer from integer of different size
r46kstub.c:635: warning: cast to pointer from integer of different size
r46kstub.c:637: warning: cast to pointer from integer of different size
mips64orion-idt-elf-gcc -g -Wa,-ahld -Wall -membedded-data \
	-O3 -c stubinit.S >stubinit.L
mips64orion-idt-elf-ld -t -s -T r46kstub.ld -Map r46kstub.map -o r46kstub.out
mips64orion-idt-elf-ld: mode elf32bmip
stubinit.o
r46kstub.o
mips64orion-idt-elf-objcopy -S -R .bss -R .data -R .reginfo \
                                -O srec r46kstub.out r46kstub.hex
```

Limitations: stubinit.S deliberately forces the PC (which is a 64-bit
register) to contain a legitimate sign-extended 32-bit value.  This was
done to cope with a bug in gdb-4.16, which does _not_ properly sign-extend
the initial PC when it loads a program.  This means that you cannot use
the "set" command to load an unmapped sixty-four bit virtual address into
the PC, as you can for all other registers.

Please send bug reports, comments, or suggestions for improvement to:
