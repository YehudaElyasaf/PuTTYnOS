#include "PuTTYn.h"
#include "io/print.h"
#include "io/screen.h"
#include "memory/pagingHandler.h"
#include "idt.h"

void initialize(){
    initPDT(0x0FF00000);
    //initIdt();
    initScreen(WHITE, BLACK);
}

void main(){
    initialize();
    //special divide by zero
    //volatile int i = 1 / 0;
    //asm __volatile__("int $3");

    int d=  100;
    for(int i=1; i< d; i++){
        kcprintc('\t', BLACK, LIGHT_GREEN);
        kprinti(i);
        kprintc('\n');
    }
    char str[] = "1234";
    kprint(str);
startVirtualMode(0x0FF00000);
}
