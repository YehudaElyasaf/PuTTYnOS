.PHONY: all clean run

OS-VERSION=PuTTYnOS-i386

GCC=/usr/local/i386elfgcc/bin/i386-elf-gcc
GCCFLAGS=-c -ffreestanding
LD=/usr/local/i386elfgcc/bin/i386-elf-ld
LDFLAGS= -Ttext 0x1000 --oformat binary
QEMU=qemu-system-i386
QEMUFLAGS=-boot c -nic model=rtl8139
NASM=nasm

C_SOURCES=$(wildcard kernel/*.c)
HEADERS=$(wildcard kernel/*.h)
OBJECTS=${C_SOURCES:.c=.o}

all: $(OS-VERSION).img

run: all
	truncate -s 144k $(OS-VERSION).img
	$(QEMU) $(OS-VERSION).img $(QEMUFLAGS)

$(OS-VERSION).img: bootloader.bin PuTTYn.bin
	cat $^ > $@

	#auto run image
	make run

PuTTYn.bin: boot/kernelCaller.o ${OBJECTS}
	@echo $^
	$(LD) $^ $(LDFLAGS) -o $@

%.o: %.c ${HEADERS}
	$(GCC) $< $(GCCFLAGS) -o $@

%.o: %.asm
	$(NASM) $^ -f elf -o $@
bootloader.bin: boot/bootloader.asm
	@clear
	$(NASM) $^ -f bin -o $@

clean:
	@rm -rf *.bin *.o *.img *.iso kernel/*.bin kernel/*.o boot/*.o  boot/*.bin
