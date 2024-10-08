Samples
=======
COPYRIGHT (c) 1989-1999.
On-Line Applications Research Corporation (OAR).

The license and distribution terms for this file may be
found in the file LICENSE in this distribution or at
http://www.rtems.org/license/LICENSE.

This directory contains the RTEMS Sample Application Suite.
The tests in this directory perform two functions:

   + provide simple examples of applications which can be
     used as a starting point for your application.

   + help test a new board support package

The hello and ticker applications are useful when first bringing up
a new board support package.  The base_mp test is useful when
performing initial checkout on a new MPCI layer.

The following describes each of the sample applications:

   base_mp

      This is a very simple two node multiprocessor application.  It consists
      of a single initialization task on each node which print out
      their respective node numbers and task IDs.  This test can be
      used as a simple test of a new MPCI layer because it minimizes
      the number of packets sent by RTEMS.

      This is intended as a starting point for custom developed multiprocessor
      applications.

   base_sp

      This is a simple single processor application which consists of
      an initialization task which creates another task.

      This is intended as a starting point for custom developed single
      processor applications.

   capture

      This simple application starts the monitor and allows you access
      to the capture engine. The capture engine provides a trace of
      RTEMS activity and is used to debug your application.

   cdtest

      A very simple C++ application which demonstrates that it is
      possible to use C++ contructors and destructors in an RTEMS
      application.  Also does a perfunctory iostream test.

   hello

      This is the RTEMS version of the classic hello world program.
      It consists of single initialization task which prints out
      a few messages.

      This test does not include a Clock Tick device driver and can
      be used to test the startup code of the board support package
      as well as console output.

   paranoia

      A public domain test of the floating point and math library
      capabilities of a toolset.  It reports discrepancies between
      actual and expected results.  It is a large test.

   ticker

      This is a simple test of the user's Clock Tick device driver.
      This test has an initialization task create three application
      tasks which sleep and periodically wake up and print the time.

   unlimited

      This is a test for the umlimited object feature of RTEMS. Here
      you can configure RTEMS to give as many objects as memory
      in the Workspace.
