#include "PuTTYn.h"
#include "io/output.h"
#include "memory/pagingHandler.h"
#include "idt.h"

void initialize(){
    //initPaging(0x0FF00000);
    initIdt();
}

void main(){
    initialize();
    
    //special divide by zero
    //volatile int i = 1 / 0;
    asm __volatile__("int $3");


    tmpPrint('c');
}
