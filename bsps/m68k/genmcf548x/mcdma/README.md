Multi-channel DMA API
=====================
Release Notes

Version 0.3

* MCD_INCLUDE_EU functionality supported(microcode changes for all tasks
except ethernet).
* Fixed bug when using MCD_END_FRAME which would cause the DMA to transfer
zero bytes and then complete.
* Code cleanup.


Version 0.2 (Slight Update)

* Modified casts and task table implementations that were causing
warnings (and even errors on certain compilers)
* Cosmetic changes to clean up MCD_dmaApi.c and MCD_dma.h
* Fixed table declarations so that MCD_tasks.c will compile if
  MCD_INCLUDE_EU is defined (Note: EU functionality still not supported)
  
Version 0.1 (Initial release)

Alpha version
MCD_INCLUDE_EU functionality not supported.
MCD_INCLUDE_JBIG not supported.
