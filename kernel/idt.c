#include "idt.h"
#include "asm.h"
#include "io/output.h"

#define KERNEL_CODE_SEGMENT_START     0x8

IDTEntry idt[NUM_OF_IDT_ENTRIES];

bool hasErrorCode(uint8_t entryNum){
    //all the following entries have an error code
    return entryNum == 8 ||     //
        entryNum == 10 ||       //
        entryNum == 11 ||       //
        entryNum == 12 ||       //
        entryNum == 13 ||       //
        entryNum == 14 ||       //
        entryNum == 17 ||       //
        entryNum == 21;         //
}
void initIdtEntry(uint8_t entryNum, void* isrAdress, uint8_t flags){
    IDTEntry* entry = &idt[entryNum];
    
    entry->offsetLow = (uint16_t)isrAdress;             //casting uses 16 LSBs
    entry->offsetHigh = (uint16_t)((uint16_t)isrAdress >> 16);    //>> 16 = casting uses 16 MSB
    entry->reserved = 0x0;
    entry->segmentSelector = KERNEL_CODE_SEGMENT_START;
    entry->flags = flags;
}

__attribute__((interrupt))
void exceptionHandler(IsrFrame* isrFrame){
    while (true);
}

__attribute__((interrupt))
void exceptionHandlerWithErrorCode(IsrFrame* isrFrame, uint32_t errCode){
    
}

void initIdt(){
    IDTR idtr = {sizeof(idt) - 1, idt};

    //init all IDT entries
    //ISR 0-31: CPU exceptions
    for(uint8_t i = FIRST_EXCEPTION_ENTRY_INDEX; i <= LAST_EXCEPTION_ENTRY_INDEX; i++)
        //only specific exceptions have an error code
        if(hasErrorCode(i))
            initIdtEntry(i, exceptionHandlerWithErrorCode, IDT_FLAGS_TRAP_GATE_RING0);
        else
            initIdtEntry(i, exceptionHandler, IDT_FLAGS_TRAP_GATE_RING0);

    //ISR 32-255: Interrupts
    //used uint16_t to avoidbecuase in uint8_t the value after 255 is 0
    for(uint16_t i = FIRST_INTERRUPT_ENTRY_INDEX; i <= LAST_INTERRUPT_ENTRY_INDEX; i++)
        initIdtEntry(i, exceptionHandler, IDT_FLAGS_INTERRUPT_GATE_RING3);

    //load IDT
    asm __volatile__ (
        "lidt (%0);"
        : //no output
        : "r" (&idtr)
    );
    //enable some interrupts???
    sti();
}
