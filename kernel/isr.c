#include "isr.h"
#include "asm.h"
#include "io/output.h"

void isrHandler(void){
    tmpPrint('b');
    cli();
    hlt();
}
