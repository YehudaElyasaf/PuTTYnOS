#pragma once

#include <stdint.h>

//Set interrup flag
void sti();
//Clear interrup flag
void cli();
//Do nothing until next interrupt
void hlt();

typedef struct
{
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
} PushaRegisters;
