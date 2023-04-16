#include "memory.h"

void memcpy(uint8_t* src, uint8_t* dst, int count){
    for(int i = 0; i < count; i++)
        dst[i] = src[i];
}

void memset(uint8_t byte, uint8_t* dst, int count) {
    for (int i = 0; i < count; i++)
        dst[i] = byte;
}

uint8_t memeq(uint8_t* a, uint8_t* b, uint32_t len) {
    for(int i = 0; i < len; i++)
        if (a[i] != b[i])
            return 0;
    return 1;
}