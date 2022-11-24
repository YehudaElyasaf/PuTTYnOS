#include "ports.h"

uint8_t in8bit(uint16_t port){
    uint8_t buffer;
    //syntax: IN    port, accumulator
    //read from port stored in dx to al
    asm("in %%dx, %%al" : "=a"(buffer) : "d"(port));

    return buffer;
}

void out8bit(uint16_t port, uint8_t buffer){
    //syntax: OUT   accumulator. port
    //write from al to port stored in dx
    asm("out %%al, %%dx" : : "a"(buffer), "d"(port));
}
