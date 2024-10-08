MVME147
=======

MVME147 port for TNI - Telecom Bretagne
by Dominique LE CAMPION (Dominique.LECAMPION@enst-bretagne.fr)
May 1996


This bsp is essentially based on the mvme136 bsp.

Summary
-------
 * include
 
   - bsp.h 
      Peripheral Channel Controller memory mapping
      Z8530 memory mapping
 
 * startup
 
   - bspstart.c
      main () setup for VME roundrobin mode
	      setup for the PCC interrupt vector base 
   - bspclean.c
      bsp_cleanup () disable timer 1 & 2 interruptions
   - linkcmds set the RAM start (0x5000) and size (4Meg - 0x5000)
   - setvec.c unchanged
   - sbrk.c unchanged
 
 * console
 
   - console.c taken from the dmv152 bsp (Zilog Z8530)
               with no modification
 
 * clock
 
   - ckinit.c entirely rewritten for the PCC tick timer 2
 
 * timer
 
   - timerisr.s and timer.c 
     entirely rewritten for the PCC tick timer 1
     now gives results un 6.25 us units (mininum timer delay,
     suprising big grain)
 
 * times
 
   - updated results for the mvme147 (beware of the 6.25 us grain)

 * Makefiles
   
   - compilation of shmsupp simply removed


To be done:

 * add VMEchip memory mapping to include/bsp.h

 * update the overheads in coverhead.h 

 * add support for serila ports 2,3 and 4.

Other notes:

 * There is no MP support (no more shmsupp) because I have no
   experience of the VME bus. The mvme136 shared memory support
   does not seem applicable on the VMEchip of the mvme147, so
   I don't know where to start. Suggestions are welcome.

 * All the timing tests and sp tests have been run.


Future work
-----------
 * Add gdb serial remote support.

 * Shared memory support (I don't really need it, but I can do
   it if it's simple).

 * Message passing on VME bus, with Ada 95 annex E (distributed
   systems) in mind.
