#pragma once

#include "irq.h"
#include "syscallHandlers.h"

#define SYSCALL_IDT_INDEX   0x42
#define SYSCALL_IRQ_NUMBER  SYSCALL_IDT_INDEX - FIRST_IRQ_MASTER_ENTRY_INDEX

#define NUMBER_OF_SYSCALLS 256
#define NUMBER_OF_SYSCALLS_PARAMETERS 4

void initSyscalls();
void syscallIrqHandler(IrqFrame irqFrame);
