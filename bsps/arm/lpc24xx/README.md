lpc24xx
=======

Development Board: QVGA Base Board from Embedded Artists

http://www.embeddedartists.com/products/uclinux/oem_lpc2478.php

Drivers:

	o Console
	o Clock
	o RTC
	o SSP (SPI mode)
	o Network
	o I2C

Howto setup QVGA Base Board?

	o Unpack board.
	o Connect board via USB to your PC.
	o Verify that demo application runs.
	o Disconnect board.
	o Change jumpers to enable ISP.
	o Connect board.
	o Load U-Boot image 'u-boot_v1.1.6_lpc2468oem_v1_8_16bit.hex'
	(available from the EA support page) into the flash (flash tool
	FlashMagic is availabe from NXP).
	o Change jumbers back to disable ISP.
	o Use a terminal program to change the U-Boot settings via the console.
	o U-Boot settings:
		set ethaddr '00:1a:f1:X:X:X'
		set serverip 'X.X.X.X'
		set ipaddr 'X.X.X.X'
		set rtems 'tftp a1000000 lpc2478.img;bootm'
		set bootcmd 'echo Booting RTEMS ...;run rtems'
		saveenv

Howto make a U-Boot image?

mkimage -A arm -O rtems -T kernel -C gzip \
   -a a0000000 -e a0000040 -n "RTEMS Application" -d app.bin.gz app.img

Application Board: NCS (Nurse Control Station)

	Board:      NextGenNCS
	Processor:  NXP LPC2478 or LPC2470
	SDRAM:      8MByte, 16 bit wide
	Ext. Flash: 1MByte, 16 bit wide
	Console:    UART, 115200 Baud
	Network:    100Base-T

Application Board: TLI800
	TLI800 is a network node using four serial ports produced by Thorn
	Security Limited.  It is used by Tyco Fire & Integrated Solutions for a
	fire control network.
