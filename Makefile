.PHONY: all clean run

OS_VERSION=PuTTYnOS-i386
#32m = green
#35m = purple
#37m = white
SUCESS_COLOR=\033[0;32m
DEFAULT_COLOR=\033[0;37m
LOG_COLOR=\033[0;35m

GCC=/usr/local/i386elfgcc/bin/i386-elf-gcc
GCCFLAGS=-c -ffreestanding
LD=/usr/local/i386elfgcc/bin/i386-elf-ld
LDFLAGS= --oformat binary
# LDFLAGS= -Ttext 0x1000 --oformat binary
QEMU=qemu-system-i386
QEMUFLAGS=-boot c -nic model=rtl8139
NASM=nasm

C_FILES=$(shell find -name "*.c")
# C_HEADER_FILES=$(C_FILES:.c=.h)
C_OBJECT_FILES=${C_FILES:.c=.o}

ASM_FILES=$(shell find -name "*.asm")
ASM_FILES_EXCLUDING_BOOTLOADER=${ASM_FILES:./boot/bootloader.asm=}

ASM_OBJECT_FILES=${ASM_FILES_EXCLUDING_BOOTLOADER:.asm=.o}
#kernelCaller.o MUST be linked first, so it's added before all dependencies separately
ASM_OBJECT_FILES_EXCLUDING_KERNEL_CALLER=${ASM_OBJECT_FILES:./boot/kernelCaller.o=}


all: $(OS_VERSION).img

run: all
	@truncate -s 144k $(OS_VERSION).img

	@# auto clean build files
	@make --no-print-directory clean

	@echo "${SUCESS_COLOR}\nRUNNING PuTTYnOS!${DEFAULT_COLOR}"
	@ $(QEMU) $(OS_VERSION).img $(QEMUFLAGS)

	@echo "${SUCESS_COLOR}\nПока-пока!${DEFAULT_COLOR}"

$(OS_VERSION).img: bootloader.bin PuTTYn.bin
	@echo "${LOG_COLOR}\nCREATING DISK IMAGE...${DEFAULT_COLOR}"
	@cat $^ > $@
	@echo

	@# auto run image
	@make --no-print-directory run

PuTTYn.bin: boot/kernelCaller.o ${ASM_OBJECT_FILES_EXCLUDING_KERNEL_CALLER} ${C_OBJECT_FILES}
	@echo "${LOG_COLOR}\nLINKING...${DEFAULT_COLOR}"
	@ $(LD) $^ $(LDFLAGS) -o $@

%.o: %.c
	@ $(GCC) $< $(GCCFLAGS) -o $@

%.o: %.asm
	@ $(NASM) $^ -f elf -o $@

bootloader.bin: boot/bootloader.asm
	@echo "${LOG_COLOR}\nCOMPILING...${DEFAULT_COLOR}"
 	# @clear
	@ $(NASM) $^ -f bin -o $@

clean:
	@echo "${LOG_COLOR}\nCLEANING BUILD FILES...${DEFAULT_COLOR}"
	@rm -f $(shell find -name "*.o")
	@rm -f $(shell find -name "*.bin")

	@echo "${SUCESS_COLOR}\c"
	@echo "Successfully cleaned!\n${DEFAULT_COLOR}"
