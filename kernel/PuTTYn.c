#include "PuTTYn.h"
#include "io/print.h"
#include "io/screen.h"
#include "memory/pagingHandler.h"
#include "cpu/idt.h"
#include "cpu/syscall.h"
#include "io/keyboard.h"
#include "tasking/timer.h"
#include "tasking/task.h"
#include "../lib/string.h"
#include "../user/shell.h"
#include "../lib/syscall.h"
#include "../lib/printf.h"
#include "../lib/scanf.h"
#include "../lib/heap.h"


#define _DEBUG

static void printDone(){
    #ifndef _DEBUG
    for(long i=0;i<50000000;i++); //wait
    #endif
    setCursorCol(NUMBER_OF_COLS / 2);
    kcprint("Done!\n", GREEN, getBackgroundColor());
    #ifndef _DEBUG
    for(long i=0;i<20000000;i++){} //wait
    #endif
}
void initialize(){
    sti();

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
    //initPDT(); //TODO: enable
    printDone();

    kprint("Initializing PIT...");
    initTimer();
    printDone();

    kprint("Initializing tasking...");
    initTasking();
    printDone();
    
    setColor(WHITE);
    sti();
}

#ifndef _DEBUG
void main(){
    initialize();
    
    createTask(&shellMain);

    //do nothing
    while (true)
    {
        hlt();
    }
}
#else

void t(){
    while (true)
    {
        kprint("a");
        for(long i=0;i<5000000;i++);
    }
}
void tt(){
    while (true)
    {
        kprint("bc");
        for(long i=0;i<5000000;i++);
    }
}
void main(){
    initialize();

    createTask(&shellMain);
    //for(long i=0;i<5000000;i++);
    createTask(&t);
    createTask(&tt);
}

#endif
