#pragma once
#include "stdint.h"

typedef struct {
    uint8_t* ptr;
    uint16_t curPtr;
    
    uint16_t bufferSize;
    uint8_t itemSize;
} Queue;

void queuePush(Queue* q, void* item);

void queuePop(Queue* q, void* item);
