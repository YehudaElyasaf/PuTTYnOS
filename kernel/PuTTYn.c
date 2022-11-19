#include "PuTTYn.h"
#include "io/output.h"
#include "PagingHandler.h"
#include "idt.h"

void initialize(){
    //initPaging(0x00000000);
    initIdt();
}

void main(){
    tmpPrint('a');

    initialize();

    //special divide by zero
    //volatile int i = 1 / 0;
    //asm __volatile__("int $3");
    //tmpPrint('c');
}
