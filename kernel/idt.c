#include "idt.h"

IDTEntry idt[NUM_OF_IDT_ENTRIES];
IDTR idtr = {sizeof(idt) - 1, idt};

void initIdt(){
    asm __volatile__ (
        "lidt (%0);"
        : //no output
        : "r" (&idtr)
    );
}