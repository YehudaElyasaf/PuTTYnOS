.PHONY: all clean run build debug

OS_NAME=PuTTYnOS
OS_VERSION=$(OS_NAME)-i386
OS_MAC_ADDR=
#32m = green
#37m = white
#35m = purple
#31m = orange
SUCESS_COLOR=\033[0;32m
DEFAULT_COLOR=\033[0;37m
LOG_COLOR=\033[0;35m
DEBUG_COLOR=\033[0;31m

GCC=/usr/local/i386elfgcc/bin/i386-elf-gcc
GCCFLAGS=-c -ffreestanding -g
GCCWARNINGS=-Wno-incompatible-pointer-types -Wno-int-conversion -Wno-int-to-pointer-cast -Wno-overflow -Wno-discarded-qualifiers
LD=/usr/local/i386elfgcc/bin/i386-elf-ld
LDFLAGS= -Ttext 0x1000
QEMU=qemu-system-i386
QEMUFLAGS= -fda $(OS_VERSION).img  -boot c -nic model=rtl8139,mac=$(OS_MAC_ADDR) -m 4G $(QAF) --no-reboot
QEMUFLAGS_DEBUG=$(QEMUFLAGS) -s -S
QUIET_RUN= > /dev/null 2>&1
NASM=nasm
PY=python3
GDB=gdb
GDBFLAGS=--quiet
GDBCMDS=-ex "target remote localhost:1234" -ex "symbol-file $(OS_VERSION)-symbols.elf"

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
	@echo "${SUCESS_COLOR}RUNNING $(OS_NAME)!${DEFAULT_COLOR}"
	@ $(QEMU) $(QEMUFLAGS) $(QUIET_RUN)

	@echo "${SUCESS_COLOR}\nПока-пока!${DEFAULT_COLOR}"

debug: build $(OS_VERSION)-symbols.elf
	@ echo "${DEBUG_COLOR}RUNNING ${OS_NAME} IN DEBUG MODE!${DEFAULT_COLOR}"
	@ $(QEMU) $(QEMUFLAGS_DEBUG) $(QUIET_RUN) &
	@ ${GDB} $(GDBFLAGS) $(GDBCMDS)

	@echo "${DEBUG_COLOR}\nПока-пока!${DEFAULT_COLOR}"

build: $(OS_VERSION).img

$(OS_VERSION).img: boot/bootloader.bin $(OS_VERSION).bin
	@echo "${LOG_COLOR}\nCREATING DISK IMAGE...${DEFAULT_COLOR}"
	@cat $^ > $@
	@echo

$(OS_VERSION).bin: boot/kernelCaller.o ${ASM_OBJECT_FILES_EXCLUDING_KERNEL_CALLER} ${C_OBJECT_FILES}
	@echo "${LOG_COLOR}\nLINKING...${DEFAULT_COLOR}"
	@ $(LD) $^ $(LDFLAGS) -o $@ --oformat binary

$(OS_VERSION)-symbols.elf: boot/kernelCaller.o ${ASM_OBJECT_FILES_EXCLUDING_KERNEL_CALLER} ${C_OBJECT_FILES}
	@echo "${DEBUG_COLOR}CREATING SYMBOL TABLE...${DEFAULT_COLOR}"
	@ $(LD) $^ $(LDFLAGS) -o $@

%.o: %.c
	@ $(GCC) $< $(GCCFLAGS) -o $@ $(GCCWARNINGS)

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
	@rm -f -r $(shell find -name "*.o")
	@rm -f -r $(shell find -name "*.bin")
	@rm -f -r $(shell find -name "*.elf")
	@rm -f -r $(shell find -name "*.img")
	@rm -f -r $(shell find -name "*.iso")
	@rm -f -r $(shell find -name "*tempCodeRunnerFile.c") #VSCode's auto generated file
	@rm -f -r $(shell find -name "*.vscode")
	@rm -f -r $(shell find -name "*__pycache__")
	@rm -f -r $(AUTO_GENERATED_ASM_FILES) $(AUTO_GENERATED_H_FILES)

	@echo "${SUCESS_COLOR}\c"
	@echo "Successfully cleaned!\n${DEFAULT_COLOR}"
