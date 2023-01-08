#include "PuTTYn.h"
#include "io/print.h"
#include "io/screen.h"
#include "memory/pagingHandler.h"
#include "cpu/idt.h"
#include "cpu/syscall.h"
#include "io/keyboard.h"
#include "tasking/timer.h"
#include "../lib/string.h"
#include "../user/shell.h"
#include "../lib/syscall.h"
#include "../lib/printf.h"
#include "../lib/scanf.h"
#include "../lib/heap.h"


#define _DEBUG

static void printDone(){
    #ifndef _DEBUG
    for(long i=0;i<20;i++);//i<50000000;i++){} //wait
    #endif
    setCursorCol(NUMBER_OF_COLS / 2);
    kcprint("Done!\n", GREEN, getBackgroundColor());
    #ifndef _DEBUG
    for(long i=0;i<20;i++);//for(long i=0;i<20000000;i++){} //wait
    #endif
}
void initialize(){
    cli();

    initScreen(GRAY, BLACK);
    
    kprint("Initializing IDT...");
    initIdt();
    printDone();

    kprint("Initializing syscalls...");
    initSyscalls();
    printDone();
    
    kprint("Initializing keyboard...");
    initKeyboard();
    printDone();

    kprint("Initializing PDT...");
    initPDT();
    printDone();

    kprint("Initializing timer...");
    initTimer();
    printDone();
    
    setColor(WHITE);
    sti();
}

#ifndef _DEBUG
void main(){
    initialize();
   
    shellMain();
}
#else

#define PRINTN(x) {putchar(x/100%10+'0'); putchar(x/10%10+'0'); putchar(x%10+'0');}
void main(){
    initialize();
    //shellMain();
}
#endif
