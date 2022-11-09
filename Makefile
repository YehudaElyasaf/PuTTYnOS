#.PHONY: all clean run

CC=i386-elf-gcc
CCFLAGS=
LD=i386-elf-ld
LDFLAGS=
QEMU=qemu-system-i386
QEMUFLAGS =  -fda $< -nic model=rtl8139

#all: run

#PuTTYn.bin: lib.o PuTTYn.o
#	@echo "link:"

#%.o: %.c
#	@echo "compile:"
#	$(CC) $(CCFLAGS)

#%.o: %.asm
#	@echo "asm:"

#run: PuTTYn.bin
#	@echo "run:"
#	@#$(QEMU) $(QEMUFLAGS)


#clean:
#	rm -r *.o *.bin

all: run

# Notice how dependencies are built as needed
kernel.bin: kernelCaller.o PuTTYn.o
	/usr/local/i386elfgcc/bin/i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

kernelCaller.o: boot/kernelCaller.asm
	nasm $< -f elf -o $@

PuTTYn.o: kernel/PuTTYn.c
	/usr/local/i386elfgcc/bin/i386-elf-gcc -ffreestanding -c $< -o $@


bootloader.bin: boot/bootloader.asm
	nasm $< -f bin -o $@

os-image.bin: bootloader.bin kernel.bin
	cat $^ > $@

run: os-image.bin
	truncate -s 1440k $<
	qemu-system-i386 $< -boot c

clean:
	rm *.bin *.o *.dis
