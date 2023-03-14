#include "asm.h"

#define GARBAGE_PORT 0x80
#define GARBAGE_VALUE 0
//write to garbage port, to wait a bit
#define PORT_WAIT() //asm("out %%al, %%dx" : : "a"(GARBAGE_VALUE), "d"(GARBAGE_PORT))

void nop(){
    asm("nop");
}

void sti(){
    asm volatile("sti");
}
void cli(){
    asm volatile("cli");
}
void hlt(){
    asm volatile("hlt");
}


uint8_t in8bit(uint16_t port){
    uint8_t buffer;
    //syntax: IN    port, accumulator
    //read from port stored in dx to al
    asm("in %%dx, %%al" : "=a"(buffer) : "d"(port));
    PORT_WAIT();

    return buffer;
}

void out8bit(uint16_t port, uint8_t buffer){
    //syntax: OUT   accumulator. port
    //write from al to port stored in dx
    asm("out %%al, %%dx" : : "a"(buffer), "d"(port));
    PORT_WAIT();
}

uint16_t in16bit(uint16_t port){
    uint16_t buffer;
    //syntax: IN    port, accumulator
    //read from port stored in dx to al
    asm("in %%dx, %%ax" : "=a"(buffer) : "d"(port));
    PORT_WAIT();

    return buffer;
}

void out16bit(uint16_t port, uint16_t buffer){
    //syntax: OUT   accumulator. port
    //write from al to port stored in dx
    asm("out %%ax, %%dx" : : "a"(buffer), "d"(port));
    PORT_WAIT();
}

uint32_t in32bit(uint16_t port){
    uint32_t buffer;
    //syntax: IN    port, accumulator
    //read from port stored in dx to al
    asm("inl %%dx, %%eax" : "=a"(buffer) : "d"(port));
    PORT_WAIT();

    return buffer;
}

void out32bit(uint16_t port, uint32_t buffer){
    //syntax: OUT   accumulator. port
    //write from al to port stored in dx
    asm("outl %%eax, %%dx" : : "a"(buffer), "d"(port));
    PORT_WAIT();
}