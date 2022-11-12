#include "idt.h"

#define KERNEL_CODE_SEGMENT     0x8

IDTEntry idt[NUM_OF_IDT_ENTRIES];
IDTR idtr = {sizeof(idt) - 1, idt};

void initIdt(){
    asm __volatile__ (
        "lidt (%0);"
        : //no output
        : "r" (&idtr)
    );
}

void initIdtEntry(uint8_t entryNum, uint32_t isrAdress, uint8_t flags){
    IDTEntry* entry = &idt[entryNum];
    
    entry->offsetLow = (uint16_t)isrAdress;             //casting uses 16 LSBs
    entry->offsetHigh = (uint16_t)(isrAdress >> 16);    //>> 16 = casting uses 16 MSB
    entry->reserved = 0x0;
    entry->segmentSelector = KERNEL_CODE_SEGMENT;
    entry->flags = flags;
}