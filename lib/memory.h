#pragma once

#include<stdint.h>

void memcpy(uint8_t* src, uint8_t* dst, int count);

void memset(uint8_t byte, uint8_t* dst, int count);

uint8_t memeq(uint8_t* a, uint8_t* b, uint32_t len);