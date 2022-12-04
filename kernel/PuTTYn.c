#include "PuTTYn.h"
#include "io/print.h"
#include "io/screen.h"
#include "memory/pagingHandler.h"
#include "cpu/idt.h"
#include "cpu/syscall.h"
#include "io/keyboard.h"
#include "../lib/string.h"
#include "../user/shell.h"
#include "../lib/syscall.h"

static inline void printDone(){
    #ifndef _DEBUG
    for(long i=0;i<50000000;i++){} //wait
    #endif
    setCursorCol(NUMBER_OF_COLS / 2);
    kcprint("Done!\n", GREEN, getBackgroundColor());
    #ifndef _DEBUG
    for(long i=0;i<20000000;i++){} //wait
    #endif
}
void initialize(){
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
    //initPDT(/*params*/);
    printDone();
    
    setColor(WHITE);
}

#ifndef _DEBUG
void main(){
    initialize();
   
    shellMain();
}
#endif


#ifdef _DEBUG
void main(){
    initialize();

    kprinti(syscall(SYSCALL_PRINT, 1, 2, 3, 4));

    shellMain();
}
#endif