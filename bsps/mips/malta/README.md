Status
======

17 Februrary 2011

XXX

This is a BSP for the MIPS Malta board with a 24K CPU on it.
It has ONLY been tested on Qemu.

Anything not mentioned has not been touched at all and will
most likely not be in the first release of the BSP.

Working
-------
+ Board initialization and shutdown
+ tty0 working polled
+ tty1 working polled (see note in issues)
+ tty2 working polled (see notes in issues)
+ Clock Tick


Issues
------
+ We have small hack to Qemu so reset will exit.  This needs to be
  fixed to follow the PC386 Qemu model where a command line argument
  selects reset or exit on reset.

+ tty2 is generating an interrupt which causes a TLB fault. We have
  disabled the interrupt in the CPU interrupt mask for now.

+ tty1 and tty2 are not showing any data on the screen.  This is
  most likely an issue with qemu since the status bit is changing
  as the characters are polled out.  

TBD
---
+ Conversion to Programmable Interrupt Controller IRQ model
  using shared infrastructure
+ tty0 working interrupt driver
+ tty1 working interrupt driver
+ tty2 working interrupt driver
+ PCI Bus Support
+ AMD AM79C973 NIC
+ Consider moving mips_interrupt_mask() into BSP.
