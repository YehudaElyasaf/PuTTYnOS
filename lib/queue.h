#pragma once
#include "stdint.h"
#include "spinlock.h"

typedef struct {
    uint8_t* ptr;
    uint16_t curPtr;
    
    uint16_t bufferSize;
    uint8_t itemSize;
    
    Spinlock lock;
} Queue;

void queuePush(Queue* q, void* item);

void queuePop(Queue* q, void* item);

// like pop but without deleting the head of the queue
void* queueHead(Queue q);

// get the length of the queue,
// used because some queues can't use strlen because
// they are binary, might have 0s in them.
uint32_t queueLen(Queue q);