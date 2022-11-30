#pragma once

#include <stdint.h>
#include "../asm.h"

typedef struct {
   uint32_t ds; //pushed from ax
   PushaRegisters regs;
   uint32_t isrNumber, errorCode;
   uint32_t eip, cs, eflags, useresp, ss; //isr registers
} IsrFrame;

void isrHandler(IsrFrame isrFrame);
void exit();
void initIsr();
