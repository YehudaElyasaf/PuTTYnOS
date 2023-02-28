#include "asm.h"

#define GARBAGE_PORT 0x80
#define GARBAGE_VALUE 0

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

//in\out
//write to garbage port, to wait a bit
static inline void portWait(){
    //asm("out %%al, %%dx" : : "a"(GARBAGE_VALUE), "d"(GARBAGE_PORT));
    //out8bit(0x80, 0);
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

uint32_t in32bit(uint16_t port){
    uint32_t buffer;
    //syntax: IN    port, accumulator
    //read from port stored in dx to al
    asm("in %%dx, %%eax" : "=a"(buffer) : "d"(port));
    portWait();

    return buffer;
}

void out32bit(uint16_t port, uint32_t buffer){
    //syntax: OUT   accumulator. port
    //write from al to port stored in dx
    asm("out %%eax, %%dx" : : "a"(buffer), "d"(port));
    portWait();
}

uint16_t in16bit(uint16_t port){
    uint16_t buffer;
    //syntax: IN    port, accumulator
    //read from port stored in dx to al
    asm("in %%dx, %%ax" : "=a"(buffer) : "d"(port));
    portWait();

    return buffer;
}

void out16bit(uint16_t port, uint16_t buffer){
    //syntax: OUT   accumulator. port
    //write from al to port stored in dx
    asm("out %%ax, %%dx" : : "a"(buffer), "d"(port));
    portWait();
}

uint64_t in64bit(uint16_t port){
    unsigned long int buffer;
    //syntax: IN    port, accumulator
    //read from port stored in dx to al
    asm("inl %%dx, %%eax" : "=a" (buffer) : "d" (port));
    portWait();

    return buffer;
}

void out64bit(uint16_t port, uint64_t buffer){
    //syntax: OUT   accumulator. port
    //write from al to port stored in dx
    asm("outl %%eax, %%dx" : : "a"((unsigned long)buffer), "d"(port));
    portWait();
}
