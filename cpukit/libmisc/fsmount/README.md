fsmount
=======

Author: Thomas Doerfler 02/07/2003

fsmount.c contains the function fsmount. It processes the 
fs table given as an parameter to create the listed mount points
and mount the corresponding file systems to their mount points.
See "c/src/tests/samples/fileio" for a sample on how to use this
function.

The field "report_reasons" specifies, what results of the mount point
creation/mount operations should print to the console.

The field "abort_reasons" specifies, what results of the mount point
creation/mount operations should abort the function. Do not set the
"abort_reasons" bit, unless you want to stop the processing of the
fsmount table after the first successful mount.
