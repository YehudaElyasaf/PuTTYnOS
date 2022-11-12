#pragma once

#include <stdint.h>

#define NUM_OF_IDT_ENTRIES      256

/*
    FLAGS (MSB to LSB):
        present                 1 bit   always 1
        DPL (privilege level)   2 bits  
        zero bit                1 bit   always zero (beimash'lahem lama tsarich et ze?)
        gate type               4 bits  0101 - task gate, 1110 - 32bit interrupt gate, 1111 - 32bit trap gate
*/
#define IDT_FLAGS_TASK_GATE             0b10000101
#define IDT_FLAGS_INTERRUPT_GATE_RING0  0b10001110
#define IDT_FLAGS_TRAP_GATE_RING0       0b10001111
#define IDT_FLAGS_INTERRUPT_GATE_RING3  0b11101110
#define IDT_FLAGS_TRAP_GATE_RING3       0b11101111

typedef struct
{
    uint16_t offsetLow; //bits 0-15
    uint16_t segmentSelector;
    uint8_t reserved; //always 0x0
    uint8_t flags; //detailed structure above
    uint16_t offsetHigh; //bits 16-31
} __attribute__((packed)) IDTEntry;

typedef struct
{
    uint16_t idtSize;
    IDTEntry* idtAdress;
} __attribute__((packed)) IDTR;


void initIdt();
void initIdtEntry(uint8_t entryNum, uint32_t isrAdress, uint8_t flags);