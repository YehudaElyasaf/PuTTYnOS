#include "asm.h"

void sti(){
    asm("sti");
}
void cli(){
    asm("cli");
}
void hlt(){
    asm("hlt");
}