#include "isr.h"
#include "asm.h"
#include "io/print.h"

void isrHandler(){
    //tmpPrint('b');
    cli();
    hlt();
}
