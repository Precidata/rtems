shsim
=====

Simple BSP for the SH simulator built into gdb.

Simulator Invocation
--------------------
```shell
sh-rtems[elf|]-gdb <executable>
(gdb) target sim
(gdb) set archi [sh|sh2]
(gdb) load <executable>
(gdb) run
```

Status
------
* The simulator invocation procedure outlined above produces error messages
with gdb-5.0, nevertheless seems to work. With gdb versions > 5.0 these
error messages are gone. I.e. if you plan to seriously work with the gdb
simulator better use gdb versions > 5.0.

* gdb's simulator is not able to correctly emulate memory areas esp. shadowing
and non-consecutive memory. I.e. access to memory areas besides area 0 will 
(bogusly) generate SIGBUS exceptions.  This includes access to area 5 
(On-chip peripherials) and prevents simulation of configuration and access
to on-chip peripherials.

* Due to limitations of the simulator you will only be able to run
applications which do not try to access any SH control registers.

Currently, this excludes all applications, which apply timers and serial 
devices, i.e. almost any real world application.

* This BSP supports 3 different console devices (cf. configure --help):
- trap34, an interface base on gdb's trap34 emulation. Known to work with
  gdb-5.0.
- gdbsci1, a stripped down sci device driver adapted to apply gdb's sci1
emulation. This is known to fail with gdb-5.0, because of a bug in gdb-5.0's 
sh-sim, a patch is submitted, but .. ~==
- devnull, redirection of console io to /dev/null. Try to single step this,
if you want to understand the details on how SH-RTEMS console redirection
works.

NOTE: the trap34 interface is incomplete and is temporarily disabled
inside of configure.in.
