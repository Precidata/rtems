realview-pbx-a9
===============

Tested only on Qemu.

```shell
git clone git://git.qemu.org/qemu.git qemu
cd qemu
git co a1bff71c56f2d1048244c829b63797940dd4ba0e
mkdir build
cd build
../configure --prefix=/opt/qemu --interp-prefix=/opt/qemu
make
make install
export PATH="$PATH:/opt/qemu/bin"

qemu-system-arm -S -s -no-reboot -net none -nographic -M realview-pbx-a9 -m 256M -kernel ticker.exe

qemu-system-arm -S -s -no-reboot -net none -nographic -smp 2 -icount auto -M realview-pbx-a9 -m 256M -kernel ticker.exe
```
