#include "PuTTYn.h"
#include "io/print.h"
#include "io/screen.h"
#include "memory/pagingHandler.h"
#include "cpu/idt.h"
#include "io/keyboard.h"
#include "../lib/string.h"

static inline void printDone(){
    setCursorCol(NUMBER_OF_COLS / 2);
    kcprint("Done!\n", GREEN, getBackgroundColor());
}
void initialize(){
    initScreen(GRAY, BLACK);
    kprint("Initializing IDT...");
    initIdt();
    printDone();
    
    kprint("Initializing PDT...    ");
    //initPDT(/*params*/);
    printDone();
    
    kprint("Initializing keyboard...    ");
    initKeyboard();
    printDone();
    

    setColor(WHITE);
    kprintc('\n');
}

void main(){
    initialize();
}
