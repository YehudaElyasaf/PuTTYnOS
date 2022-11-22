#include "idt.h"
#include "asm.h"
#include "io/output.h"

#define KERNEL_CODE_SEGMENT_START     0x8

static IDTEntry idt[NUM_OF_IDT_ENTRIES];

void initIdtEntry(uint8_t entryNum, uint32_t isrAdress, uint8_t flags){
    IDTEntry* entry = &idt[entryNum];
    
    entry->offsetLow = (uint16_t) isrAdress;              //casting uses 16 LSBs
    entry->offsetHigh = (uint16_t)(isrAdress >> 16);      //>> 16 = casting uses 16 MSB
    entry->reserved = 0x0;
    entry->segmentSelector = KERNEL_CODE_SEGMENT_START;
    entry->flags = flags;
}

//__attribute__((interrupt))
//void exceptionHandler(IsrFrame* isrFrame){
//    tmpPrint('a');
//    cli();
//    hlt();
//}
//
//__attribute__((interrupt))
//void exceptionHandlerWithErrorCode(IsrFrame* isrFrame, uint32_t errCode){
//    tmpPrint('e');
//    cli();
//    hlt();
//}

extern uint32_t* isrTable;
void initIdt(){
    tmpPrint('0' + isrTable[5]);
    IDTRegister idtr = {sizeof(idt) - 1, idt};

    //init all IDT entries
    //ISR 0-31: CPU exceptions
    for(uint8_t i = FIRST_EXCEPTION_ENTRY_INDEX; i <= LAST_EXCEPTION_ENTRY_INDEX; i++)
        initIdtEntry(i, isrTable[i], IDT_FLAGS_TRAP_GATE_RING0);

    //ISR 32-255: Interrupts
    //used uint16_t becuase in uint8_t the value after 255 is 0
    //for(uint16_t i = FIRST_INTERRUPT_ENTRY_INDEX; i <= LAST_INTERRUPT_ENTRY_INDEX; i++)
    //    initIdtEntry(i, exceptionHandler, IDT_FLAGS_INTERRUPT_GATE_RING3);

    //load IDT
    asm __volatile__ (
        "lidt (%0);"
        : //no output
        : "r" (&idtr)
    );
    //enable some interrupts???
    //sti();
}
