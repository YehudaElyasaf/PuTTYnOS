#include "PuTTYn.h"
#include "io/print.h"
#include "network/network.h"
#include "io/screen.h"
#include "memory/pagingHandler.h"
#include "cpu/idt.h"
#include "cpu/syscall.h"
#include "io/keyboard.h"
#include "tasking/timer.h"
#include "tasking/task.h"
#include "tasking/scheduler.h"
#include "../lib/string.h"
#include "../user/shell.h"
#include "../lib/syscall.h"
#include "../lib/printf.h"
#include "../lib/scanf.h"
#include "../lib/heap.h"


//#define _DEBUG

static void printDone(){
    setCursorCol(NUMBER_OF_COLS / 2);
    kcprint("Done!\n", GREEN, getBackgroundColor());
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
    initPDT(/*params*/);
    printDone();

    kprint("Finding NIC...");
    initNetworking();
    kprint("Initializing PIT...");
    initTimer();
    printDone();

    kprint("Initializing tasking...");
    initTasking();
    printDone();
    
    setColor(WHITE);
}

#ifndef _DEBUG
void main(){
    initialize();
    
    createTask(&shellMain);

    killTask(CURRENT_TASK);
}
#else

void t(){
    while (true)
    {
        printf("a");
        for(long i=0;i<50000;i++);
    }
}
void main(){
    initialize();

    createTask(&shellMain);
    for(long i=0;i<50000000;i++);
    createTask(&t);
}

#endif
