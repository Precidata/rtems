sp04
====
This test is broken as it is currently implemented.  It is illegal for
a task to do Ada IO during a extension since the Ada run-time uses
locks to safeguard data structures.  Dispatching is disabled and the
call to Text_IO in the Switch extension will lock the system up.

The proper solution is to have the Switch extension lock task and
time when it a switch occurs and have a single task print this
information as necessary.

