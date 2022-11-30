#include "isr.h"
#include "idt.h"
#include "../asm.h"
#include "../io/print.h"
#include "isrs.h" //auto generated

char* isrNames[] = {
   //0
   "Divide By Zero",
   "Debug",
   "Non Maskable Interrupt",
   "Breakpoint",
   "Overflow",
   "Bound Range Exceeded",
   "Invalid Opcode",
   "Device Not Avaliable",
   "Double Fault",
   "Compressor Segment Overrun",
   //10
   "Invalid TSS",
   "Segment Not Present",
   "Stack Segment Fault",
   "General Protection Fault",
   "Page Fault",
   "Reserved",
   "x87 Floation Point Exception",
   "Alignment Check",
   "Machine Check",
   "SIMD Floating Point Exception",
   //20
   "Virtualization Exception",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Hypervisor Injection Exception",
   "VMM Communication Exception",
   //30
   "Security Exception",
   "Reserved"
};

static inline void exit(){
    cli();
    hlt();
}
void isrHandler(IsrFrame isrFrame){
    kcprint("\nERROR!\n", LIGHT_RED, getBackgroundColor());
    kprint("\tIRQ ");
    kprinti(isrFrame.isrNumber);
    kprint(": ");
    kprint(isrNames[isrFrame.isrNumber]);
    kprint(" Error");
    if(isrFrame.errorCode != 0){
        //print error code
        kcprint("\n\tError Code: " , GRAY, getBackgroundColor());
        kcprinti(isrFrame.errorCode, GRAY, getBackgroundColor());
    }
    kprintc('\n');
    
    exit();
}

void initIsr(){
    for(uint8_t isrNumber = FIRST_ISR_ENTRY_INDEX; isrNumber <= LAST_ISR_ENTRY_INDEX; isrNumber++)
        initIdtEntry(isrNumber, getIsr(isrNumber), IDT_FLAGS_INTERRUPT_GATE_RING3);
}
