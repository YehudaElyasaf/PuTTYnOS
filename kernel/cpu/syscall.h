#pragma once

#include "irq.h"

#define SYSCALL_IRQ 0x42

void initSyscalls();
void syscallIrqHandler(IrqFrame irqFrame);
