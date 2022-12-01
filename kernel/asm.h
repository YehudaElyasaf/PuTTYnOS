#pragma once

#include <stdint.h>

//Set interrup flag
void sti();
//Clear interrup flag
void cli();
//Do nothing until next interrupt
void hlt();

//read from port
uint8_t in8bit(uint16_t port);
//write to port
void out8bit(uint16_t port, uint8_t buffer);


typedef struct
{
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
} PushaRegisters;
