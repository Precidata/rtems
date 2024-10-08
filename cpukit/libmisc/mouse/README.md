Mouse
=====

SOURCE: http://www.kryslix.com/nsfaq/Q.12.html

Subject: What protocol do mice use?
Date: 09/16/97
Updated: 11/16/00 This document was adapated from a web page produced by
Tomi Engdahl <then@delta.hut.fi>
Microsoft serial mouse
Description

The Microsoft serial mouse is the most popular 2 button mouse. It is
supported by all major operating systems. The maximum tracking rate for
a Microsoft mouse is 40 reports/second * 127 counts per report, in other
words, 5080 counts per second. The most common range for mice is is 100
to 400 CPI (counts per inch) but can be up to 1000 CPI. A 100CPI mouse
can discriminate motion up to 50.8 inches/second while a 400 CPI mouse
can only discriminate motion up to 12.7 inches/second.
Pinout

```
9 pin  25 pin    Line    Comments
shell  1         GND
3      2         TD      Serial data from host to mouse (only for power)
2      3         RD      Serial data from mouse to host
7      4         RTS     Positive voltage to mouse
8      5         CTS
6      6         DSR
5      7         SGND
4      20        DTR     Positive voltage to mouse and reset/detection
```

RTS = Request to Send   CTS  = Clear to Send
DSR = Data Set Ready    DTR  = Data Terminal Ready
GND = Protective Ground SGND = Signal Ground

To function correctly, both the RTS and DTR lines must be
positive. DTR/DSR and RTS/CTS must NOT be shorted. RTS may be toggled
negative for at least 100ms to reset the mouse. (After a cold boot, the
RTS line is usually negative. This provides an automatic toggle when
RTS is brought positive). When DTR is toggled the mouse should send a
single byte 0x45 (ASCII 'M').

Serial data parameters:
1200bps, 7 databits, 1 stop-bit

Data packet format:

Data is sent in 3 byte packets for each event (a button is pressed or
released or the mouse moves):

```
        D7      D6      D5      D4      D3      D2      D1      D0
 
Byte 1  X       1       LB      RB      Y7      Y6      X7      X6
Byte 2  X       0       X5      X4      X3      X2      X1      X0      
Byte 3  X       0       Y5      Y4      Y3      Y2      Y1      Y0
```

LB is the state of the left button (1 means down)
RB is the state of the right button (1 means down)
X7-X0 movement in X direction since last packet (signed byte)
Y7-Y0 movement in Y direction since last packet (signed byte)

The high order bit of each byte (D7) is ignored. Bit D6 indicates the start of an event, which allows the software to synchronize with the mouse.

Graphical representation of a packet

```
              1st byte        2nd byte         3rd byte
          ================  ===============  ================
           - 1 ? ? Y Y X X  - 0 X X X X X X  - 0 Y Y Y Y Y Y
          ================  ===============  ================
               | | \ / \ /      \---------/      \---------/
               | |  |   |            |                |
               | |  |   \----\       |                |
               | |  \--------|-------|--------\       |
               | |          / \ /---------\  / \ /---------\
               | |         ================ =================
               | |          0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
 Left Button --/ |         ================ =================
Right Button ----/            X increment      Y increment
```

3 Button Logitech extension

Logitech extended the 2 button mouse protocol to support 3 button mice
by adding a 4th byte when the middle button is pressed (and the first
packet after it is released). If a 4th byte is encountered (i.e., an
extra byte with D6 set to 0) then D5 of that byte (0x20) indicates the
status of the middle mouse button.


Mouse systems mouse
-------------------

Serial data parameters:
1200bps, 8 databits, 1 stop-bit

5 byte Mouse Systems packet

```
        D7      D6      D5      D4      D3      D2      D1      D0

Byte 1  1       0       0       0       0       LB      CB      RB
Byte 2  X7      X6      X5      X4      X3      X2      X1      X0
Byte 3  Y7      Y6      Y5      Y4      Y3      Y4      Y1      Y0
Byte 4  X7'     X6'     X5'     X4'     X3'     X2'     X1'     X0'
Byte 5  Y7'     Y6'     Y5'     Y4'     Y3'     Y4'     Y1'     Y0'
```

LB is left button state (0=pressed, 1=released)
CB is center button state (0=pressed, 1=released)
RB is right button state (0=pressed, 1=released)
X7-X0 movement in X direction since last packet in signed byte 
      format (-128..+127), positive direction right
Y7-Y0 movement in Y direction since last packet in signed byte 
      format (-128..+127), positive direction up
X7'-X0' movement in X direction since sending of X7-X0 packet in signed byte 
      format (-128..+127), positive direction right
Y7'-Y0' movement in Y direction since sending of Y7-Y0 in signed byte 
      format (-128..+127), positive direction up

The last two bytes in the packet (bytes 4 and 5) contain information
about movement data changes which have occured after data bytes 2 and
3 have been sent.

PS/2 mouse

The standard PS/2 mouse (such as the Logitech mouse) defaults to 160 CPI
and can be switched to 40, 80, 160 or 320 CPI via software. The Microsoft
mouse driver for Windows 3.x and Windows 95 defaults to 160 counts per
inch. The maximum tracking rate for PS/2 mouse is 40 reports/second *
255 counts per report, or 10200 counts per second. A 100 CPI mouse could
discriminate motion up to 102 inches per second while a 400 CPI mouse
could discriminate motion up to 25.2 inches per second.


Connector pinout
----------------
```
Pin     Wire Name
1       DATA
2       Reserved
3       Ground
4       +5V Supply
5       CLK
6       Reserved
Shield  Chassis
```

Packet Format
-------------
```
        D7      D6      D5      D4      D3      D2      D1      D0
Byte 1  XV      XV      YS      XS       1       M       R       L
Byte 2  X7      X6      X5      X4      X3      X2      X1      X0
Byte 3  Y7      Y6      Y5      Y4      Y3      Y2      Y1      Y0

L       Left button state (1 = pressed down)
M	Middle button state (1 = pressed down)
R       Right button state (1 = pressed down)
X0-X7   Movement in X direction
Y0-Y7   Movement in Y direction
XS,YS   Movement data sign bits (1 = negative)
XV,YV   Movement data overflow bits (1 = overflow has occured)
```

Physical connector
------------------

The PS/2 mouse connector has the following pinout when looking at the
connector on the back of the computer:

```
        4 u 6
       1  .  2
        3   5

1.      GND
2.      +5V
3.      DATA
4.      CLOCK
5.      Not used
6.      Not used
```

Bi-directional transmission is controlled by the CLK and DATA lines. Both
are fed by an open collector device which lets either host or mouse force
the line to "0". During non-transmission, CLK is at "1" and DATA can be at
"0" or "1".

The host can inhibit mouse transmission by forcing CLK to "0". If the
host inhibits the mouse while it is transmitting, the byte must be
retransmitted (if the inhibit state arrived before the 11th clock).

Receiving data: Check 'clock'. If inactive, there is a bit on the 'data'
line. Each transmission unit is one start bit, eight data bits, odd
parity and one stop bit. Start bits are low, stop bits high. Each clock
active or inactive period is 30 to 50 microseconds. Data transition to
falling edge of clock is 5 to 25 microseconds.

Sending: Check that both clock and data are high. Pull down data for
start bit, and start clocking.

NOTE: Logitech has made the extension to this protocol to allow three
buttons (the M bit is always 0 on 2 button mice).

References

* Original PC mouse info page by Tomi Engdahl
  http://www.hut.fi/~then/mytexts/mouse.html

* PC Magazine May 28, 1991

* Programmer's Reference to Genius Mouse
  ftp://x2ftp.oulu.fi/pub/msdos/programming/docs/gmouse.doc

* Logitech Pointing Device Hardware Information Product Support Document # 1410
  ftp://ftp.logitech.com/pub/TechSupport/MOUSE/HELP/1410.txt

* Mice: How do they work? by Richard Torrens
  http://box.argonet.co.uk/users/4qd/meece.html

* In Pursuit Of The Perfect Portable Pointer by Intelink Electronics
from EDN Products Edition April 16, 1977 pages 43-45

* Programming the Microsoft Mouse 
  http://www.geocities.com/SiliconValley/2151/mouse.html
