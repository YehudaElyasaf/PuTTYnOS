#include "PuTTYn.h"
#include "io/print.h"
#include "io/screen.h"
#include "memory/pagingHandler.h"
#include "idt.h"

void initialize(){
    initIdt();
    initScreen(WHITE, BLACK);
}

void main(){
    initialize();
    kprint("\n\n\n");
    //special divide by zero
    //volatile int i = 1 / 0;
    asm __volatile__("int $3");
    
    kprint("\n\nend!");
}

