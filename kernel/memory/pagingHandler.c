#include "pagingHandler.h"
#include "../io/print.h"
#include "../asm.h"
#include "../cpu/idt.h"
#include "../../lib/memory.h"

PTEntry* kernelPTAddr = 0;

uint32_t firstHole = KERNEL_END, headOfPages = KERNEL_END;

int initPT(int size) {
    uint32_t pt = allocPage();
    int i = 0, cr0 = 0;
    for (; i < PDT_SIZE && !(*(uint32_t*)(0xfffff000+i) & PRESENT); i++);
    if (i >= PDT_SIZE) // PDT full
        return;
    
    *(uint32_t*)(0xfffff000+i) = PRESENT | USER | READWRITE | pt;

    size = size > 0 ? size : DEFAULT_PAGE_NUM;
    size *= PAGE_SIZE;

    kmalloc(size, i);

    return i*0x400000;
}

void kmalloc(uint32_t size, uint32_t pageTable) {
    PTEntry *pt = *(PTEntry*)(0xfffff000+pageTable);
    uint8_t* ptr = 0xffc00000 + pageTable*PAGE_SIZE;

    if (!(*pt & DIRTY) || !(*pt & PRESENT)) {
        return;
    }

    size = size / PAGE_SIZE + !(!(size % PAGE_SIZE)); // size in pages. the remainder counts as another page.
    
    for (; *ptr & DIRTY && ptr < (uint32_t)pt + PAGE_SIZE; ptr++); // wait until you find not used 
    if (ptr >= (uint32_t)pt + PAGE_SIZE)
        return; // no more pages to allocate

    for (int i = 0; i < size; i++) {
        if (*(ptr+i) & PRESENT)
            continue;
        *(ptr+i) = allocPage() | DIRTY | USER | PRESENT | READWRITE;
    }
}

uint32_t allocPage() {
    int cr0 = 0;
    __asm__("mov %%cr0, %0":"=r"(cr0));
    cr0 ^= 0x80000000; // turn off paging mode
    __asm__("mov %0, %%cr0"::"r"(cr0));

    uint32_t addr = firstHole;

    if (*((uint32_t*)firstHole) != 0) { // this is a hole which is not in the head of the pageHeap
        firstHole = *((uint32_t*)firstHole); // goto next hole.
        *((uint32_t*)addr) = 0; // clear the next segment in this hole.
    } else {
        firstHole += PAGE_SIZE;
        headOfPages = firstHole;
    }
    __asm__("mov %%cr0, %0":"=r"(cr0));
    cr0 |= 0x80000000; // turn on paging mode
    __asm__("mov %0, %%cr0"::"r"(cr0));
    return addr;
}

void deallocPage(uint32_t page) {
    int cr0 = 0;

    if (page >= headOfPages)
        return;

    __asm__("mov %%cr0, %0":"=r"(cr0));
    cr0 ^= 0x80000000; // turn off paging mode
    __asm__("mov %0, %%cr0"::"r"(cr0));
    
    memset(0, page, PAGE_SIZE);

    if (page == headOfPages - PAGE_SIZE) {
        if (firstHole == headOfPages)
            firstHole = page;
        headOfPages = page;
    }

    if (page < firstHole) {
        *((uint32_t*)page) = firstHole;
        firstHole = page;
        return;
    }
    uint32_t* ptr = firstHole;
    for (; *ptr < page; ptr = *ptr);
    if (page != headOfPages)
        *((uint32_t*)page) = ptr;
    *ptr = page;

    __asm__("mov %%cr0, %0":"=r"(cr0));
    cr0 |= 0x80000000; // turn on paging mode
    __asm__("mov %0, %%cr0"::"r"(cr0));
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

    *table = READWRITE | PRESENT | DIRTY | (uint32_t)kernelPTAddr;

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
    *(table+(PDT_SIZE-1)) = READWRITE | PRESENT | DIRTY | USER | (uint32_t)table;


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