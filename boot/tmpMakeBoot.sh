nasm bootloader.asm -o bootloader.img
truncate -s 1440k bootloader.img
qemu-system-i386 bootloader.img