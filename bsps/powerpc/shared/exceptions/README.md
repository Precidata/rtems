Exceptions
==========

BSP support middleware for 'new-exception' style PPC.

T. Straumann, 12/2007


EXPLANATION OF SOME TERMS
-------------------------

In this README we refer to exceptions and sometimes
to 'interrupts'. Interrupts simply are asynchronous
exceptions such as 'external' exceptions or 'decrementer'
/'timer' exceptions.

Traditionally (in the libbsp/powerpc/shared implementation),
synchronous exceptions are handled entirely in the context
of the interrupted task, i.e., the exception handlers use
the task's stack and leave thread-dispatching enabled,
i.e., scheduling is allowed to happen 'in the middle'
of an exception handler.

Asynchronous exceptions/interrupts, OTOH, use a dedicated
interrupt stack and defer scheduling until after the last
nested ISR has finished.


RATIONALE
---------
The 'new-exception' processing API works at a rather
low level. It provides functions for
installing low-level code (which must be written in
assembly code) directly into the PPC vector area.
It is entirely left to the BSP to implement low-level
exception handlers and to implement an API for
C-level exception handlers and to implement the
RTEMS interrupt API defined in cpukit/include/rtems/irq.h.

The result has been a Darwinian evolution of variants
of this code which is very hard to maintain. Mostly,
the four files

libbsp/powerpc/shared/vectors/vectors.S
  (low-level handlers for 'normal' or 'synchronous'
  exceptions. This code saves all registers on
  the interrupted task's stack and calls a
  'global' C (high-level) exception handler.

libbsp/powerpc/shared/vectors/vectors_init.c
  (default implementation of the 'global' C
  exception handler and initialization of the
  vector table with trampoline code that ends up
  calling the 'global' handler.

libbsp/powerpc/shared/irq/irq_asm.S
  (low-level handlers for 'IRQ'-type or 'asynchronous'
  exceptions. This code is very similar to vectors.S
  but does slightly more: after saving (only
  the minimal set of) registers on the interrupted
  task's stack it disables thread-dispatching, switches
  to a dedicated ISR stack (if not already there which is
  possible for nested interrupts) and then executes the high
  level (C) interrupt dispatcher 'C_dispatch_irq_handler()'.
  After 'C_dispatch_irq_handler()' returns the stack
  is switched back (if not a nested IRQ), thread-dispatching
  is re-enabled, signals are delivered and a context
  switch is initiated if necessary.

libbsp/powerpc/shared/irq/irq.c
  implementation of the RTEMS ('new') IRQ API defined
  in cpukit/include/rtems/irq.h.

have been copied and modified by a myriad of BSPs leading
to many slightly different variants.

THE BSP-SUPORT MIDDLEWARE
=========================

The code in this directory is an attempt to provide the
functionality implemented by the aforementioned files
in a more generic way so that it can be shared by more
BSPs rather than being copied and modified.

Another important goal was eliminating all conditional
compilation which tested for specific CPU models by means
of C-preprocessor symbols (#ifdef ppcXYZ).
Instead, appropriate run-time checks for features defined
in cpuIdent.h are used.

The assembly code has been (almost completely) rewritten
and it tries to address a few problems while deliberately
trying to live with the existing APIs and semantics
(how these could be improved is beyond the scope but
that they could is beyond doubt...):

 - some PPCs don't fit into the classic scheme where
   the exception vector addresses all were multiples of
   0x100 (some vectors are spaced as closely as 0x10).
   The API should not expose vector offsets but only
   vector numbers which can be considered an abstract
   entity. The mapping from vector numbers to actual
   address offsets is performed inside 'raw_exception.c'
 - having to provide assembly prologue code in order to
   hook an exception is cumbersome. The middleware
   tries to free users and BSP writers from this issue
   by dealing with assembly prologues entirely inside
   the middleware. The user can hook ordinary C routines.
 - the advent of BookE CPUs brought interrupts with
   multiple priorities: non-critical and critical 
   interrupts. Unfortunately, these are not entirely
   trivial to deal with (unless critical interrupts
   are permanently disabled [which is still the case:
   ATM rtems_interrupt_enable()/rtems_interrupt_disable()
   only deal with EE]). See separate section titled
   'race condition...' below for a detailed explanation.


STRUCTURE
---------
The middleware uses exception 'categories' or
'flavors' as defined in raw_exception.h.

The middleware consists of the following parts:

   1 small 'prologue' snippets that encode the
     vector information and jump to appropriate
	 'flavored-wrapper' code for further handling.
	 Some PPC exceptions are spaced only
	 16-bytes apart, so the generic
	 prologue snippets are only 16-bytes long.
	 Prologues for synchronuos and asynchronous
	 exceptions differ.

   2 flavored-wrappers which sets up a stack frame
     and do things that are specific for
	 different 'flavors' of exceptions which
	 currently are
	   - classic PPC exception
	   - ppc405 critical exception
	   - bookE critical exception
	   - e500 machine check exception

   Assembler macros are provided and they can be
   expanded to generate prologue templates and
   flavored-wrappers for different flavors
   of exceptions. Currently, there are two prologues
   for all aforementioned flavors. One for synchronous
   exceptions, the other for interrupts.

   3 generic assembly-level code that does the bulk
     of saving register context and calling C-code.

   4 C-code (ppc_exc_hdl.c) for dispatching BSP/user
     handlers.

   5 Initialization code (vectors_init.c). All valid
     exceptions for the detected CPU are determined
	 and a fitting prologue snippet for the exception
	 category (classic, critical, synchronous or IRQ, ...)
	 is generated from a template and the vector number
	 and then installed in the vector area.

	 The user/BSP only has to deal with installing
	 high-level handlers but by default, the standard
	 'C_dispatch_irq_handler' routine is hooked to
	 the external and 'decrementer' exceptions.

   6 RTEMS IRQ API is implemented by 'irq.c'. It
     relies on a few routines to be provided by
	 the BSP.

USAGE
-----
	BSP writers must provide the following routines 
	(declared in irq_supp.h):
	Interrupt controller (PIC) support:
```c
		BSP_setup_the_pic()        - initialize PIC hardware
		BSP_enable_irq_at_pic()    - enable/disable given irq at PIC; IGNORE if
		BSP_disable_irq_at_pic()     irq number out of range!
		C_dispatch_irq_handler()   - handle irqs and dispatch user handlers
		                             this routine SHOULD use the inline
									 fragment

									   bsp_irq_dispatch_list()

									 provided by irq_supp.h
									 for calling user handlers.

	BSP initialization; call

	rtems_status_code sc = ppc_exc_initialize(
	  PPC_INTERRUPT_DISABLE_MASK_DEFAULT,
	  interrupt_stack_begin,
	  interrupt_stack_size
	);
	if (sc != RTEMS_SUCCESSFUL) {
	  BSP_panic("cannot initialize exceptions");
	}
	BSP_rtems_irq_mngt_set();
```

	Note that BSP_rtems_irq_mngt_set() hooks the C_dispatch_irq_handler()
	to the external and decrementer (PIT exception for bookE; a decrementer
	emulation is activated) exceptions for backwards compatibility reasons.
	C_dispatch_irq_handler() must therefore be able to support these two
	exceptions.
	However, the BSP implementor is free to either disconnect
	C_dispatch_irq_handler() from either of these exceptions, to connect
	other handlers (e.g., for SYSMGMT exceptions) or to hook
	C_dispatch_irq_handler() to yet more exceptions etc. *after*
	BSP_rtems_irq_mngt_set() executed.

	Hooking exceptions:

	The API defined in vectors.h declares routines for connecting
	a C-handler to any exception. Note that the execution environment
	of the C-handler depends on the exception being synchronous or
	asynchronous:

		- synchronous exceptions use the task stack and do not
		  disable thread dispatching scheduling.
		- asynchronous exceptions use a dedicated stack and do
		  defer thread dispatching until handling has (almost) finished.

	By inspecting the vector number stored in the exception frame
	the nature of the exception can be determined: asynchronous 
	exceptions have the most significant bit(s) set.

	Any exception for which no dedicated handler is registered
	ends up being handled by the routine addressed by the
	(traditional) 'globalExcHdl' function pointer.

	Makefile.am:
		- make sure the Makefile.am does NOT use any of the files
			vectors.S, vectors.h, vectors_init.c, irq_asm.S, irq.c
		  from 'libbsp/powerpc/shared' NOR must the BSP implement
		  any functionality that is provided by those files (and
		  now the middleware).

		- (probably) remove 'vectors.rel' and anything related

		- add

```shell
		    ../../../libcpu/@RTEMS_CPU@/@exceptions@/bspsupport/vectors.h
		    ../../../libcpu/@RTEMS_CPU@/@exceptions@/bspsupport/irq_supp.h
```
		  to 'include_bsp_HEADERS'

		- add
```shell
		    ../../../libcpu/@RTEMS_CPU@/@exceptions@/exc_bspsupport.rel
		    ../../../libcpu/@RTEMS_CPU@/@exceptions@/irq_bspsupport.rel
```
		  to 'libbsp_a_LIBADD'

		  (irq.c is in a separate '.rel' so that you can get support
		  for exceptions only).

CAVEATS
-------
On classic PPCs, early (and late) parts of the low-level
exception handling code run with the MMU disabled which mean
that the default caching attributes (write-back) are in effect
(thanks to Thomas Doerfler for bringing this up).
The code currently assumes that the MMU translations
for the task and interrupt stacks as well as some
variables in the data-area MATCH THE DEFAULT CACHING
ATTRIBUTES (this assumption also holds for the old code
in libbsp/powepc/shared/vectors ../irq).

During initialization of exception handling, a crude test
is performed to check if memory seems to have the write-back
attribute. The 'dcbz' instruction should - on most PPCs - cause
an alignment exception if the tested cache-line does not
have this attribute.

BSPs which entirely disable caching (e.g., by physically
disabling the cache(s)) should set the variable
  ppc_exc_cache_wb_check = 0
prior to calling initialize_exceptions(). 
Note that this check does not catch all possible
misconfigurations (e.g., on the 860, the default attribute
is AFAIK [libcpu/powerpc/mpc8xx/mmu/mmu_init.c] set to
'caching-disabled' which is potentially harmful but
this situation is not detected).


RACE CONDITION WHEN DEALING WITH CRITICAL INTERRUPTS
----------------------------------------------------

   The problematic race condition is as follows:

   Usually, ISRs are allowed to use certain OS
   primitives such as e.g., releasing a semaphore.
   In order to prevent a context switch from happening
   immediately (this would result in the ISR being
   suspended), thread-dispatching must be disabled
   around execution of the ISR. However, on the
   PPC architecture it is neither possible to 
   atomically disable ALL interrupts nor is it
   possible to atomically increment a variable
   (the thread-dispatch-disable level).
   Hence, the following sequence of events could
   occur:
    1) low-priority interrupt (LPI) is taken
    2) before the LPI can increase the 
	   thread-dispatch-disable level or disable
	   high-priority interupts, a high-priority
	   interrupt (HPI) happens
	3) HPI increases dispatch-disable level
	4) HPI executes high-priority ISR which e.g.,
	   posts a semaphore
	5) HPI decreases dispatch-disable level and
	   realizes that a context switch is necessary
	6) context switch is performed since LPI had
	   not gotten to the point where it could
	   increase the dispatch-disable level.
   At this point, the LPI has been effectively
   suspended which means that the low-priority
   ISR will not be executed until the task 
   interupted in 1) is scheduled again!

   The solution to this problem is letting the
   first machine instruction of the low-priority
   exception handler write a non-zero value to 
   a variable in memory:

	ee_vector_offset:  

         stw r1, ee_lock@sdarel(r13)	
         .. save some registers etc..
		 .. increase thread-dispatch-disable-level
		 .. clear 'ee_lock' variable

	After the HPI decrements the dispatch-disable level
	it checks 'ee_lock' and refrains from performing
	a context switch if 'ee_lock' is nonzero. Since
	the LPI will complete execution subsequently it
	will eventually do the context switch.

	For the single-instruction write operation we must
	  a) write a register that is guaranteed to be
	     non-zero (e.g., R1 (stack pointer) or R13
		 (SVR4 short-data area).
	  b) use an addressing mode that doesn't require
	     loading any registers. The short-data area
		 pointer R13 is appropriate.

    CAVEAT: unfortunately, this method by itself
	is *NOT* enough because raising a low-priority
	exception and executing the first instruction
	of the handler is *NOT* atomic. Hence, the following
	could occur:

	 1) LPI is taken
	 2) PC is saved in SRR0, PC is loaded with 
	    address of 'locking instruction'
		  stw r1, ee_lock@sdarel(r13)
     3) ==> critical interrupt happens
	 4) PC (containing address of locking instruction)
	    is saved in CSRR0
     5) HPI is dispatched

	For the HPI to correctly handle this situation
	it does the following:

	
		a) increase thread-dispatch disable level
		b) do interrupt work
		c) decrease thread-dispatch disable level
	    d) if ( dispatch-disable level == 0 )
		 d1) check ee_lock
		 d2) check instruction at *CSRR0
		 d3) do a context switch if necessary ONLY IF
		     ee_lock is NOT set AND *CSRR0 is NOT the
			 'locking instruction'

	this works because the address of 'ee_lock'
	is embedded in the locking instruction
	'stw r1, ee_lock@sdarel(r13)' and because the
	registers r1/r13 have a special purpose
	(stack-pointer, SDA-pointer). Hence it is safe
	to assume that the particular instruction
	'stw r1,ee_lock&sdarel(r13)' never occurs
	anywhere else.

	Another note: this algorithm also makes sure
	that ONLY nested ASYNCHRONOUS interrupts which
	enable/disable thread-dispatching and check if
	thread-dispatching is required before returning
	control engage in this locking protocol. It is
	important that when a critical, asynchronous
	interrupt interrupts a 'synchronous' exception
	(which does not disable thread-dispatching)
	the thread-dispatching operation upon return of
	the HPI is NOT deferred (because the synchronous
	handler would not, eventually, check for a
	dispatch requirement).

	And one more note: We never want to disable
	machine-check exceptions to avoid a checkstop.
	This means that we cannot use enabling/disabling
	this type of exception for protection of critical
	OS data structures.
	Therefore, calling OS primitives from a asynchronous
	machine-check handler is ILLEGAL and not supported.
	Since machine-checks can happen anytime it is not
	legal to test if a deferred context switch should
	be performed when the asynchronous machine-check
	handler returns (since _Context_Switch_is_necessary
	could have been set by a IRQ-protected section of
	code that was hit by the machine-check).
	Note that synchronous machine-checks can legally
	use OS primitives and currently there are no
	asynchronous machine-checks defined.

Epilogue
--------
   You have to disable all asynchronous exceptions which may cause a context
   switch before the restoring of the SRRs and the RFI.  Reason:
   
      Suppose we are in the epilogue code of an EE between the move to SRRs and
      the RFI. Here EE is disabled but CE is enabled. Now a CE happens.  The
      handler decides that a thread dispatch is necessary. The CE checks if
      this is possible:
   
         o The thread dispatch disable level is 0, because the EE has already
           decremented it.
         o The EE lock variable is cleared.
         o The EE executes not the first instruction.
   
      Hence a thread dispatch is allowed. The CE issues a context switch to a
      task with EE enabled (for example a task waiting for a semaphore). Now a
      EE happens and the current content of the SRRs is lost.
