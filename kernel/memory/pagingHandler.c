#include "pagingHandler.h"
#include "../io/print.h"
#include "../asm.h"
#include "../cpu/idt.h"

PTEntry* kernelPTAddr = 0;

uint32_t firstFree;

uint32_t allocPage() {
    if (firstFree == 0)
        firstFree = KERNEL_END;
    firstFree += PAGE_SIZE;
    return firstFree - PAGE_SIZE;
}

void startVirtualMode(uint32_t address) {    
    __asm__("mov %0, %%cr3"::"r"(address));
    uint32_t cr0 = 0;
    __asm__("mov %%cr0, %0":"=r"(cr0));
    cr0 |= 0x80000000;
    __asm__("mov %0, %%cr0"::"r"(cr0));
}

void initPDT() {
    PDEntry* table = allocPage();
    kernelPTAddr = allocPage();

    *table = READWRITE | PRESENT | (uint32_t)kernelPTAddr;

    // identity mapping kernel code physical address to virtual address
    for (int i = 1; i <= KERNEL_SIZE; i++) {
        *(kernelPTAddr+i) = PRESENT | READWRITE | DIRTY | KERNEL_START + (i-1)*PAGE_SIZE;
    }

    // identity mapping stack physical address to virtual address
    *(kernelPTAddr + 0x8f) = PRESENT | READWRITE | DIRTY | 0x8f000;

    // identity mapping video memory 
    *(kernelPTAddr + 0xb8) = PRESENT | READWRITE | DIRTY | 0xb8000;

    for (int i = 0; i <= getIDTR().idtSize/0x1000; i++) {
        *(kernelPTAddr + (uint32_t)getIDTR().idtAdress + i) = PRESENT | READWRITE | DIRTY |(uint32_t)getIDTR().idtAdress + i;
    }

    // last entry points to the pdt itself
    *(table+(PDT_SIZE-1)) = READWRITE | PRESENT | (uint32_t)table;


    irqInstallHandler(14, pagefault);

    startVirtualMode((uint32_t)table);
}

void pagefault(IsrFrame regs) {
    // A page fault has occurred.
   // The faulting address is stored in the CR2 register.
   uint32_t faulting_address;
   asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

   // The error code gives us details of what happened.
   int present   = !(regs.errorCode & 0x1); // Page not present
   int rw = regs.errorCode & 0x2;           // Write operation?
   int us = regs.errorCode & 0x4;           // Processor was in user-mode?
   int reserved = regs.errorCode & 0x8;     // Overwritten CPU-reserved bits of page entry?
   int id = regs.errorCode & 0x10;          // Caused by an instruction fetch?

   // Output an error message.
   setColor(RED);
   kprint("Page fault! ( ");
   if (present) {kprint("present ");}
   if (rw) {kprint("read-only ");}
   if (us) {kprint("user-mode ");}
   if (reserved) {kprint("reserved ");}
   kprint(") at ");
   kprinth(faulting_address);
   kprint("\n");
   setColor(WHITE);
}