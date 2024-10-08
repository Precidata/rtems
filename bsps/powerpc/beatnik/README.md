Beatnik
=======

 Valuable information was obtained from the following drivers
   netbsd: Allegro Networks Inc; Wasabi Systems Inc.
    linux: MontaVista, Software, Inc; Chris Zankel, Mark A. Greer.
	       Matthew Dharm, rabeeh, Manish Lachwani, Ralf Baechle.  
    rtems: Brookhaven National Laboratory; Shuchen Kate Feng
 This BSP also builds on top of the work of others who have contributed
 to similar RTEMS (powerpc) BSPs, most notably Eric Valette, Eric Norum
 and others.

 In particular, the Author wishes to thank Shuchen Kate Feng (BNL) for many
 inspiring discussions and Dayle Kotturi (SLAC) for her contributions, support
 and extensive testing.


LICENSE
-------
See ./LICENSE file.

Note that not all files that are part of this BSP were written by
me (most notably, the ethernet drivers if_gfe [netbsd port] and
if_em [freebsd port]). Consult individual file headers for copyright
and authorship information.


BUILD INFO
----------
(relevant only if you received this BSP unbundled from the RTEMS distribution)

  prepare:
  - get up-to date RTEMS release
  - untar beatnik.tgz into c/src/lib/libbsp/powerpc
  - copy beatnik.cfg into make/custom
  - patch c/src/lib/libsp/powerpc/acinclude.ac
  - run 'bootstrap' from top directory; make sure RTEMS
    autoXXX are found first in your PATH
  configure:
  - configure with your favorite options. BSP name is 'beatnik'
    I recommend passing RTEMS_CFLAGS=-g to 'configure'


TARGET
------
Even though this BSP is binary compatible with the MVME5500 it's primary
target was and is the MVME6100 board which in some respects is quite different.
In particular, the discovery chip and the VME bridge exhibit significant
differences.
I am sometimes asked why this BSP provides yet another port of the gfe
and em BSD drivers (which had previously been ported for the mvme5500
BSP by Shuchen Kate Feng [BNL]). The answer is simply a matter of time:
Once support for the 6100 board was completed I found it easier to use
the set of 'quick-and-dirty' wrappers (found in network/porting) that I had
developed for other projects and to do a new port from scratch using that
framework rather than modifying the mvme5500 BSP's drivers. mvme5500 support was
added to this BSP because we own a few of those boards we occasionally
play with but we don't want to build and support an additional BSP for them.
An important detail -- hardware cache snooping -- was borrowed from
Shuchen Kate Feng's gfe driver port, though.


HARDWARE SUPPORT
---------------
(some of the headers mentioned below contain more
detailed information)

NOTE:  The BSP supports both, the mvme6100 and the mvme5500 boards.
       It detects relevant hardware at run-time.

WARNING: It is extremely important that a MOTLoad "waitProbe", "netShut"
         sequence be executed before booting RTEMS. Otherwise, network
         interface interrupt handlers installed by MOTLoad may cause memory
         corruption

CONSOLE: 2 serial devices, UART driver from 'shared' - no surprises
       ("/dev/ttyS0", [="/dev/console"], "/dev/ttyS1"). (Only
       /dev/ttyS0 is accessible from the front panel.)

CLOCK: Decrementer, same as other PPC BSPs. (FIXME: a discovery timer
       could be used.)

PIC (interrupt controller) (bsp/irq.h): Marvell hostbridge
       does not implement interrupt priorities. The driver supports
       priorities in software (masking lower priority lines during
       execution of higher priority ISR). I believe the design of the
       IRQ subsystem is as efficient as possible with focus on low
       latencies.
       In addition to the rtems IRQ API, calls are available to
       change IRQ priority and to enable/disable interrupts at the PIC.

EXCEPTIONS: (bspException.h) Routines to install a user callback
       for (PPC) exception handling.

PCI (bsp/pci.h): The BSP hides the fact that there are effectively
       two 'root' busses (AKA 'hoses') behind the discovery bridge.
	   Devices are addressed by bus/slot/function-triples and the PCI
	   subsystem transparently figures out what hose to use.
       In addition to rtems' PCI API, a call is available to scan
       all devices executing a user callback on each device.
       BSP_pciConfigDump() is a convenience wrapper dumping essential
       information (IDs, BAs, IRQ pin/line) to the console or a file.

MEMORY MAP: CHRP; all addresses (MEM + I/O) read from PCI config. space
       are CPU addresses. For sake of portability, drivers should still
       use the _IO_BASE, PCI_MEM_BASE, PCI_DRAM_OFFSET constants.

NVRAM: Address constants are defined in bsp.h

FLASH (bsp/flashPgm.h): Routines to write flash. Highest level
      wrapper writes a file to flash.
	  NOTE: Writing to flash is disabled by default;
	        call BSP_flashWriteEnable().

I2C (bsp.h, rtems/libi2c.h, libchip/i2c-xxx.h):  temp. sensor and eeprom
      are available as device files (bsp.h); lower-level interface is
      provided by libi2c.h.
	  NOTE: The I2C devices are not registered and the driver is not
	        initialized by default. Call BSP_i2c_initialize() to do that;
			this will create
				/dev/i2c0.vpd-eeprom
				/dev/i2c0.usr-eeprom
				/dev/i2c0.ds1621
	        You can then read the board temperature:
			    fd = open("/dev/i2c0.ds1621",O_RDONLY)
				read(fd,&temp,1)
				close(fd);
				printf("Board Temp. is %idegC\n",(int)temp);

VME: (bsp/VME.h, bsp/vme_am_defs.h, bsp/VMEDMA.h).
      *always* use VME.h API, if possible; do *not* use chip drivers
	  (vmeUniverse.h, vmeTsi148.h) directly unless you know what you are
	  doing (i.e., if you need specific features provided by the particular
	  chip; currently, both of the mentioned chip drivers expose entry points
	  that are designed to be compatible).

      VMEConfig.h should not be used by applications as it makes them
      dependent on BSP internals. VMEConfig.h is intended to be used
	  by BSP designers only.

      VME interrupt priorities: the VME bridge(s) do not implement
      priorities in hardware.
      However, on the 5500/6100 multiple physical interrupt
      lines/wires connect the VME bridge to the PIC. Hence, it is possible
      to assign the different wires different priorities at the PIC
      (see above) and to route VME interrupts to different wires according
      to their priority. You need to call driver specific routines
      for this (vmeXXXIntRoute()), however (for driver-specific API
	  consult bsp/vmeUniverse.h, bsp/vmeTsi148.h).

	  For VME DMA *always* use the bsp/VMEDMA.h API. DO NOT use
	  chip-specific features. Applications written using the bsp/VMEDMA.h
	  API are portable between the UniverseII and the Tsi148.

HARDWARE TIMERS: (bsp/gt_timer.h). Programmable general-purpose (GPT) and
      watchdog timers. Routines are provided to setup, start and stop
	  GPTs. The setup routine allows for specifying single-shot or periodic
	  mode and dispatches a user ISR when the GPT expires.

	  The watchdog timer - when started - issues a hard-reset of the
	  board if not 'petted' within a configurable timeout period.

NETWORK: (bsp/bsp_bsdnet_attach.h). The BSP offers a call to list
      all available interfaces (name, description, 'attach'-method)
      for the application to make a selection.
      Alternatively, there are BSP_auto_network_driver_name and
      BSP_auto_enet_attach(), the latter with the capability to configure
      the first NIC with a 'live' link status.
      All drivers (rewritten 'mve' for the mv64360 NIC (6100) and BSD ports
      'gfe'/'em' (5500)) support the SIOCSIFMEDIA/SIOCGIFMEDIA ioctls
      (rtems/rtems_mii_ioctl.h provides helpers to convert strings from/to
      control words).

VPD: (bsp/vpd.h). The board's VPD (vital-product-data such as S/N,
      MAC addresses and so forth) can be retrieved.

BOOTING: BSP has a relocator-header. Clear MSR and jump to the first
      instruction in the binary. R3 and R4, if non-null, point to the
      start/end of an optional command line string that is copied into
      BSP_commandline_string. The BSP is compatible with 'netboot'.

Have fun.

-- Till Straumann <strauman@slac.stanford.edu>, 2005-2007.
