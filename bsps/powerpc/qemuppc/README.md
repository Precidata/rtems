qemuppc
=======
This BSP is designed to operate on the PPC simulator provided by qemu.
We are using the Courverture Project's qemu source tree.

Couverture Project to add tracing/coverage to qemu
  http://libre.adacore.com/libre/tools/coverage/

Their source repository
  http://forge.open-do.org/scm/?group_id=8

That has instructions on checking it out.

If you check it out into ${BASE} directory, then the
directory with their modified qemu is:

  ${BASE}/couverture/trunk/couverture/tools/qemu-r6588

My BASE is /home/joel/qemu-coverage.  I configured like this
from within the qemu source tree.

```shell
  ./configure --prefix=/home/joel/qemu-coverage/install
  make
  make install
```

This gives all simulated targets supported.  

See the Makefile for more details.

  
