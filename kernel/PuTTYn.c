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
#include "../user/shell.h"
#include "../lib/string.h"
#include "../lib/syscall.h"
#include "../lib/printf.h"
#include "../lib/scanf.h"
#include "../lib/heap.h"
#include "../lib/tasking.h"

#define _DEBUG

static void printDone(){
    setCursorCol(NUMBER_OF_COLS / 2);
    kcprint("Done!\n", GREEN, getBackgroundColor());
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

    kprint("Initializing PIT...");
    initTimer();
    printDone();

    kprint("Initializing tasking...");
    initTasking();
    printDone();

    kprint("Initializing networking...");
    initNetworking();
    printDone();
    
    setColor(WHITE);
    sti();
}

#ifndef _DEBUG
void main(){
    initialize();
    
    createProcess(&shellMain);

    killProcess(CURRENT_TASK);
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

    clearScreen();
    createProcess(&shellMain);
    
    killProcess(CURRENT_TASK);
}

#endif
