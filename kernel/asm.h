#pragma once

#include <stdint.h>

//no operation - do nothing
void nop();
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

uint32_t in32bit(uint16_t port);

void out32bit(uint16_t port, uint32_t buffer);

uint16_t in16bit(uint16_t port);

void out16bit(uint16_t port, uint16_t buffer);



typedef struct
{
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
} PushaRegisters;
