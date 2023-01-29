#include "idt.h"
#include "../asm.h"
#include "../io/print.h"
#include "isr.h"
#include "irq.h"

#define KERNEL_CODE_SEGMENT_START 0x8

static IDTEntry idt[NUM_OF_IDT_ENTRIES];

void initIdtEntry(uint8_t entryNum, uint32_t* isrAdress, uint8_t flags){
    IDTEntry* entry = idt + entryNum;
    
    entry->offsetLow = (uint16_t) (uint32_t)isrAdress;              //casting uses 16 LSBs
    entry->offsetHigh = (uint16_t)((uint32_t)isrAdress >> 16);      //>> 16 = casting uses 16 MSB
    entry->reserved = 0x0;
    entry->segmentSelector = KERNEL_CODE_SEGMENT_START;
    entry->flags = flags;
}

void initIdt(){
    //disable irqs
    cli();
    
    IDTRegister idtr = {sizeof(idt) - 1, idt};

    //create IDT entries
    //0-31: CPU exceptions
    initIsr();

    //32-47: Hardware interrupts
    initIrq();

    //load IDT
    asm __volatile__ (
        "lidt (%0)"
        : //no output
        : "r" (&idtr)
    );
}

IDTRegister getIDTR() {
    return (IDTRegister){sizeof(idt) - 1, idt};
}