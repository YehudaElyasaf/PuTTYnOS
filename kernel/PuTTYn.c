#include "PuTTYn.h"
#include "io/print.h"
#include "io/screen.h"
#include "memory/pagingHandler.h"
#include "idt.h"

void initialize(){
    //initPaging(0x0FF00000);
    //initIdt();
    initScreen(WHITE, BLACK);
}

void main(){
    initialize();
    
    //special divide by zero
    //volatile int i = 1 / 0;
    //asm __volatile__("int $3");

    int d=  102;
    for(int i=1; i< d; i++){
        kprinti(i);
        kprinti(i);
        kprinti(i);
        kprinti(i);
        kprintc('\n');
    }
}
