HelenOS
=======

The sources in this directory are from the HelenOS project.
See: http://www.helenos.org

These sources are from release 0.4.2 
See: http://www.helenos.org/releases/HelenOS-0.4.2.tar.bz2

Unless otherwise indicated, these files are licensed under a BSD-like 
license without advertising clause.
See: http://www.helenos.org/license
Or the file LICENSE in this directory.

Source modifications are mostly documented in the individual source code files.
Source that is removed is discarded by #ifdef 0 ... #endif blocks.

The subdirectories here correspond to the following subdirectories in the 
HelenOS source tree (as of release 0.4.2):

boot
HelenOS-0.4.2/boot
Various subdirectories related to booting the sparc64

boot/genarch
HelenOS-0.4.2/boot/genarch

boot/generic
HelenOS-0.4.2/boot/generic

boot/include
Various .h files from the following directories:
  HelenOS-0.4.2/boot/genarch
  HelenOS-0.4.2/boot/generic
  HelenOS-0.4.2/boot/arch/sparc64/loader

boot/sparc64
HelenOS-0.4.2/boot/arch/sparc64

kernel
HelenOS-0.4.2/kernel
Various subdirectories related to kernel code and services for the sparc64

kernel/genarch
HelenOS-0.4.2/kernel/genarch

kernel/generic
HelenOS-0.4.2/kernel/generic

kernel/sparc64
HelenOS-0.4.2/kernel/arch/sparc64/

