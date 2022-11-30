#include "PuTTYn.h"
#include "io/print.h"
#include "io/screen.h"
#include "memory/pagingHandler.h"
#include "cpu/idt.h"

void initialize(){
    initIdt();
    initScreen(WHITE, BLACK);
}

void main(){
    initialize();
    //call 3 ints
    //special divide by zero
    //int i = 1 / 0;
    
    kprint("text\nhere!!!");
    asm __volatile__("int $4");
    
    kprint("\n\nend!");
}
