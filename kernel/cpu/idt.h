#pragma once

#include <stdint.h>
#include <stdbool.h>

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


#define NUM_OF_IDT_ENTRIES 256
#define FIRST_ISR_ENTRY_INDEX 0
#define LAST_ISR_ENTRY_INDEX FIRST_IRQ_MASTER_ENTRY_INDEX - 1

#define FIRST_IRQ_MASTER_ENTRY_INDEX 32
#define LAST_IRQ_MASTER_ENTRY_INDEX FIRST_IRQ_SLAVE_ENTRY_INDEX - 1
#define FIRST_IRQ_SLAVE_ENTRY_INDEX 40
#define LAST_IRQ_SLAVE_ENTRY_INDEX 47

#define NUM_OF_IRQS LAST_IRQ_SLAVE_ENTRY_INDEX - FIRST_IRQ_MASTER_ENTRY_INDEX

#define PIC_MASTER_REMAP_CMD1 0x4
#define PIC_SLAVE_REMAP_CMD1 0x2
#define PICS_REMAP_CMD2 0x1
#define PICS_REMAP_CMD3 0x0

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
} __attribute__((packed)) IDTRegister;


void initIdtEntry(uint8_t entryNum, uint32_t* isrAdress, uint8_t flags);
void initIdt();

IDTRegister getIDTR();