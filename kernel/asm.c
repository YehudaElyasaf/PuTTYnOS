#include "asm.h"

#define GARBAGE_PORT 0x80
#define GARBAGE_VALUE 0

void nop(){
    asm("nop");
}
void sti(){
    asm("sti");
}
void cli(){
    asm("cli");
}
void hlt(){
    asm("hlt");
}

//in\out

//write to garbage port, to wait a bit
static inline void portWait(){
    asm("out %%al, %%dx" : : "a"(GARBAGE_VALUE), "d"(GARBAGE_PORT));
}
uint8_t in8bit(uint16_t port){
    uint8_t buffer;
    //syntax: IN    port, accumulator
    //read from port stored in dx to al
    asm("in %%dx, %%al" : "=a"(buffer) : "d"(port));
    portWait();

    return buffer;
}

void out8bit(uint16_t port, uint8_t buffer){
    //syntax: OUT   accumulator. port
    //write from al to port stored in dx
    asm("out %%al, %%dx" : : "a"(buffer), "d"(port));
    portWait();
}
