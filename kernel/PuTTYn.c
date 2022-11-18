#include "PuTTYn.h"
#include "io/output.h"
#include "PagingHandler.h"
#include "idt.h"

void initialize(){
    initPaging(0x00000000);
    initIdt();
}

void main(){

    initialize();

    tmpPrint('a');
    //special divide by zero
    volatile int i = 1 / 0;
    tmpPrint('b');
    tmpPrint42();
}
