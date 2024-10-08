tmtests
=======

This directory contains the RTEMS Classic API Timing Test Suite.
The tests in this directory are used to measure the execution
time of RTEMS directive and some critical internal functions.
The results of these test are useful for comparison of RTEMS
performance 

+ between versions of RTEMS
+ between different boards 
+ across different architectures

These tests attempt to benchmark RTEMS as the user would.  They measure
the execution time for a directive (under various circumstances)
from the time RTEMS is entered until it is executed.  Where possible,
the time is the average of at least 100 invocations of the directive.

The accuracy of the times reported by these benchmarks is very dependent
on the resolution of the timer being used.  It is also important to
insure that all sources of hardware interrupts are disabled during
execution of the tests.  This insures that the directive time reported
does not include any interrupt time.

