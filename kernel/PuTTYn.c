#include "PuTTYn.h"
#include "io/print.h"
#include "io/screen.h"
#include "memory/pagingHandler.h"
#include "cpu/idt.h"

void initialize(){
    initScreen(GRAY, BLACK);
    kprint("Initializing IDT...    ");
    initIdt();
    kcprint("Done!\n", GREEN, getBackgroundColor());
    
    kprint("Initializing PDT...    ");
    //initPDT();
    kcprint("Done!\n", GREEN, getBackgroundColor());
    

    setColor(WHITE);
    kprintc('\n');
}

void main(){
    initialize();
}
