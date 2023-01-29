#include "queue.h"
#include "memory.h"

void queuePush(Queue* q, void* item) {
    int len = strlen(q->ptr + q->curPtr*q->itemSize);
    if (len >= q->bufferSize) return;
    if (len == 0) {
        q->curPtr = 0;
    }
    else if (q->ptr + len + 1 > q->bufferSize) {
        memcpy(q->ptr + q->curPtr*q->itemSize, q->ptr, len);
        q->curPtr = 0;
    }

    memcpy(item, q->ptr + q->curPtr*q->itemSize + len, q->itemSize);
    *(q->ptr + q->curPtr*q->itemSize + len + 1) = 0;
}

void queuePop(Queue* q, void* item) {
    int len = strlen(q->ptr + q->curPtr*q->itemSize);
    if (!len) {
        memset(0, item, q->itemSize);
        return;
    }
    memcpy(q->ptr + q->curPtr*q->itemSize, item, q->itemSize);
    memset(0, q->ptr + q->curPtr*q->itemSize, q->itemSize);
    q->curPtr = q->curPtr < q->bufferSize - q->itemSize-1 ? q->curPtr + q->itemSize : 0;
}