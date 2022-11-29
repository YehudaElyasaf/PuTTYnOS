#include "isr.h"
#include "asm.h"
#include "io/print.h"


void isrHandler(registers_t r){
    kcprint("An isr was called!\n", RED, getDefaultBackgroundColor());
}
