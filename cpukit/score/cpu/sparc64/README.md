SPARC64
=======

A lot of explanation needed, will do when known.

The score/cpu/sparc64 contains only code that can execute without accessing 
privileged registers or using privileged instructions. This was done because
the privileged registers differ between the sun4u and sun4v models.

The model specific and privileged code that would normally be found in 
score/cpu/sparc64 resides in libcpu/sparc64/@RTEMS_CPU_MODEL@/score or in
libcpu/sparc64/shared/score directory. This is primarily the interrupt 
handling code.

