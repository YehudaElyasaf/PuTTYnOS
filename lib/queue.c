#include "queue.h"
#include "memory.h"
#include "string.h"

void queuePush(Queue* q, void* item) {
    spin_lock(&q->lock);
    int len = queueLen(*q);
    if (len >= q->bufferSize) return;
    if (len == 0) {
        q->curPtr = 0;
    }
    else if (q->ptr + len + 1 > (uint8_t*)q->bufferSize) {
        memcpy(q->ptr + q->curPtr*q->itemSize, q->ptr, len);
        q->curPtr = 0;
    }

    memcpy(item, q->ptr + q->curPtr*q->itemSize + len, q->itemSize);
    *(q->ptr + q->curPtr*q->itemSize + len + 1) = 0;
    spin_unlock(&q->lock);
}

void queuePop(Queue* q, void* item) {
    spin_lock(&q->lock);
    int len = queueLen(*q);
    if (!len) {
        memset(0, item, q->itemSize);
        return;
    }
    
    if (item)
        memcpy(q->ptr + q->curPtr*q->itemSize, item, q->itemSize);
    memset(0, q->ptr + q->curPtr*q->itemSize, q->itemSize);
    q->curPtr = q->curPtr < q->bufferSize - q->itemSize-1 ? q->curPtr + q->itemSize : 0;
    spin_unlock(&q->lock);
}

void* queueHead(Queue q) {
    spin_lock(&q.lock);
    int len = queueLen(q);
    if (!len) {
        return 0;
    }
    spin_unlock(&q.lock);
    return q.ptr + q.curPtr*q.itemSize;
}

uint32_t queueLen(Queue q) {
    uint32_t len = 0;
    for (uint8_t* ptr = q.ptr + q.curPtr+q.itemSize; ptr < q.ptr + q.bufferSize; ptr += q.itemSize) {
        uint8_t* emptyChecker;
        for (emptyChecker = ptr; *emptyChecker == 0 && emptyChecker < q.ptr + q.itemSize; emptyChecker++);
        
        if (*emptyChecker == 0)
            return len; // all of the bytes in itemSize are empty
        len++;
    }
    return len;
}