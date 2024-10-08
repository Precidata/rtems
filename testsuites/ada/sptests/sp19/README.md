sp19
====
This is a "problem" test in Ada versus the C equivalent.  In C,
we used macros to :

  + declare a LOT of local integer and floating point variables
  + load them with values
  + check them at certain times
  + to avoid floating point operations on CPUs w/o hardware support

The macros are not equivalent to subprograms doing the same thing.
We are trying to insure that the register set is fully utilized and
checking that it survives context switches.  If the routines are
subprograms, then calling conventions and scoping rules apply.  Thus
the variables do not exist throughout the entire life of the task
as in the C test.

It seems to point out that although the conditional compilation
and macro capabilities of C are often abused, they do provide
capabilities which are difficult if not impossible to mimic entirely
with subprograms.  
