#include "isr.h"
#include "asm.h"
#include "io/output.h"

void isrHandler(){
    tmpPrint('b');
    cli();
    hlt();
}
