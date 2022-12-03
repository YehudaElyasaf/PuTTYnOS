#include "PuTTYn.h"
#include "io/print.h"
#include "io/screen.h"
#include "memory/pagingHandler.h"
#include "cpu/idt.h"
#include "cpu/syscall.h"
#include "io/keyboard.h"
#include "../lib/string.h"
#include "../user/shell.h"

static inline void printDone(){
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
    //initPDT(/*params*/);
    printDone();
    
    setColor(WHITE);
}

void main(){
    initialize();

    asm("int $9");

    kprint("\nRunning shell!\n");
    shellMain();
}
