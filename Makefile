.PHONY: all clean run build

OS_VERSION=PuTTYnOS-i386
#32m = green
#35m = purple
#37m = white
SUCESS_COLOR=\033[0;32m
DEFAULT_COLOR=\033[0;37m
LOG_COLOR=\033[0;35m

GCC=/usr/local/i386elfgcc/bin/i386-elf-gcc
GCCFLAGS=-c -ffreestanding -g
LD=/usr/local/i386elfgcc/bin/i386-elf-ld
LDFLAGS= -Ttext 0x1000 --oformat binary
QEMU=qemu-system-i386 -fda
QEMUFLAGS=-boot c -nic model=rtl8139 -m 4G $(QEMU_ADD_FLAGS)
NASM=nasm
PY=python3

C_FILES=$(shell find -name "*.c")
C_OBJECT_FILES=${C_FILES:.c=.o}

ASM_FILES=$(shell find -name "*.asm") $(AUTO_GENERATED_ASM_FILES)
ASM_FILES_EXCLUDING_BOOTLOADER=${ASM_FILES:./boot/bootloader.asm=}

ASM_OBJECT_FILES=${ASM_FILES_EXCLUDING_BOOTLOADER:.asm=.o}
#kernelCaller.o MUST be linked first, so it's added before all dependencies separately
ASM_OBJECT_FILES_EXCLUDING_KERNEL_CALLER=${ASM_OBJECT_FILES:./boot/kernelCaller.o=}
AUTO_GENERATED_ASM_FILES=kernel/cpu/isrs.asm kernel/cpu/irqs.asm
AUTO_GENERATED_H_FILES=kernel/cpu/isrs.h kernel/cpu/irqs.h

all: build

run: all
	@echo "${SUCESS_COLOR}RUNNING PuTTYnOS!${DEFAULT_COLOR}"
	@ $(QEMU) $(OS_VERSION).img $(QEMUFLAGS)

	@echo "${SUCESS_COLOR}\nПока-пока!${DEFAULT_COLOR}"

build: $(OS_VERSION).img
	@truncate -s 144k $(OS_VERSION).img

$(OS_VERSION).img: boot/bootloader.bin PuTTYn.bin
	@echo "${LOG_COLOR}\nCREATING DISK IMAGE...${DEFAULT_COLOR}"
	@cat $^ > $@
	@echo

PuTTYn.bin: boot/kernelCaller.o ${ASM_OBJECT_FILES_EXCLUDING_KERNEL_CALLER} ${C_OBJECT_FILES}
	@echo "${LOG_COLOR}\nLINKING...${DEFAULT_COLOR}"
	@ $(LD) $^ $(LDFLAGS) -o $@

%.o: %.c
	@ $(GCC) $< $(GCCFLAGS) -o $@

%.o: %.asm
	@ $(NASM) $^ -f elf -o $@

boot/bootloader.bin: boot/bootloader.asm  $(AUTO_GENERATED_ASM_FILES)
	@echo "${LOG_COLOR}\nCOMPILING...${DEFAULT_COLOR}"
	@ $(NASM) $< -f bin -o $@

$(AUTO_GENERATED_ASM_FILES): kernel/cpu/generateIsrsAsm.py kernel/cpu/generateIsrsH.py kernel/cpu/generateIrqsAsm.py kernel/cpu/generateIrqsH.py #auto_generated_files
	@echo "${LOG_COLOR}\nGENERATING FILES...${DEFAULT_COLOR}"
	@ $(foreach pyFile, $^, $(PY) $(pyFile);)

clean:
	@echo "${LOG_COLOR}\nCLEANING BUILD FILES...${DEFAULT_COLOR}"
	@rm -f $(shell find -name "*.o")
	@rm -f $(shell find -name "*.bin")
	@rm -f $(shell find -name "*tempCodeRunnerFile.c") #VSCode's auto gfile
	@rm -f $(AUTO_GENERATED_ASM_FILES) $(AUTO_GENERATED_H_FILES)

	@echo "${SUCESS_COLOR}\c"
	@echo "Successfully cleaned!\n${DEFAULT_COLOR}"
