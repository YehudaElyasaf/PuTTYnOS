#include "isr.h"
#include "asm.h"
#include "../io/print.h"

#include "isrs.h"

void isrHandler(registers_t r){
    kcprint("An isr was called!\n", RED, getDefaultBackgroundColor());
    kprinti(r.int_no);
    kprintc('\n');
}
