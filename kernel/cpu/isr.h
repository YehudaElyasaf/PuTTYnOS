#pragma once

#include <stdint.h>
#include "../asm.h"
#include "idt.h"

typedef struct {
   uint32_t ds; //pushed from ax
   PushaRegisters regs;
   uint32_t isrIndex, errorCode; //or irqIndex, irqNumber
   uint32_t eip, cs, eflags, useresp, ss; //isr registers
} IsrFrame;
#define irqIndex isrIndex
#define irqNumber errorCode

void isrHandler(IsrFrame isrFrame);
void initIsr();
