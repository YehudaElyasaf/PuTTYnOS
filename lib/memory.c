#include "memory.h"

void memcpy(uint8_t* src, uint8_t* dst, int count){
    for(int i = 0; i < count; i++)
        dst[i] = src[i];
}

void memset(uint8_t byte, uint8_t* dst, int count) {
    for (int i = 0; i < count; i++)
        dst[i] = byte;
}
