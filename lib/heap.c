#include "heap.h"
#include "memory.h"
#include "linkedList.h"

#define SIZE(addr) ((HeapHeader*)(addr))->size
#define NEXT(addr) ((HeapHeader*)(addr))->next

static uint32_t firstHole = LLEMPTY;
static uint32_t headOfHeap = HEAP_START;

uint32_t alloc(uint32_t size) {
    uint8_t* ptr = headOfHeap;
    HeapHeader* i = (HeapHeader*)firstHole, *last_header = i;
    size += sizeof(HeapHeader)/2; // no need for the "next" in the header for a used block
    if (size < sizeof(HeapHeader))
        size = sizeof(HeapHeader);

    while (i != (HeapHeader*)LLEMPTY) {
        // if the size is exactly the size of the hole, or if the hole is 
        // big enough to store the new block and a new one after that.
        if (i->size == size || i->size >= size + sizeof(HeapHeader)) {
            ptr = i;
            removeNode(&firstHole, ptr, sizeof(HeapHeader)/2);
            if (i->size >= size + sizeof(HeapHeader)) {
                SIZE(ptr + size) = i->size - size;
                NEXT(ptr + size) = i->next;
                insertNode(&firstHole, ptr + size, sizeof(HeapHeader)/2);
            }
            break;
        }
        last_header = i;
        i = i->next;
    }
    
    // if didn't find a hole that is good for this memory block
    if (ptr != (uint8_t*)i) {
        headOfHeap += size;
    }
    SIZE(ptr) = size;
    return ptr + sizeof(HeapHeader)/2;
}

void free(uint32_t addr) {
    addr -= sizeof(HeapHeader)/2; // include the header too
    if (addr + SIZE(addr) == headOfHeap) {
        headOfHeap = addr;
        memset(0, addr, SIZE(addr));
    }
    else {
        memset(0, addr + sizeof(HeapHeader)/2, SIZE(addr) - sizeof(HeapHeader)/2);
        insertNode(&firstHole, addr, sizeof(HeapHeader)/2);
        if (addr + SIZE(addr) == NEXT(addr)) { // if there is a hole next to it
            uint32_t tmp = addr + SIZE(addr);
            SIZE(addr) += SIZE(tmp);
            removeNode(&firstHole, tmp, sizeof(HeapHeader)/2);
            memset(0, tmp, SIZE(tmp));
        }

        uint32_t beforePtr = nodeByIndex(firstHole, placeInList(firstHole, addr, sizeof(HeapHeader)/2)-1, sizeof(HeapHeader)/2);
        if (beforePtr != LLEMPTY && beforePtr + SIZE(beforePtr) == addr) { // if there is a hole next to it
            SIZE(beforePtr) += SIZE(addr);
            SIZE(addr) = 0;
            removeNode(&firstHole, addr, sizeof(HeapHeader)/2);
        }
    }
}

uint32_t realloc(uint32_t addr, uint32_t size) {
    uint32_t newAddr = alloc(size);
    memcpy(addr, newAddr, SIZE(addr-sizeof(HeapHeader)/2));
    free(addr);
}

/*
uint32_t realloc(uint32_t addr, uint32_t size) {
    size += sizeof(HeapHeader)/2;
    addr -= sizeof(HeapHeader)/2;

    if (size == SIZE(addr)) {
        return addr;
    }
    if (size < SIZE(addr)) {
        if (SIZE(addr) - size > sizeof(HeapHeader)) {
            SIZE(addr) = size;
            insertNode(&firstHole, addr + size, sizeof(HeapHeader)/2);
            return addr;
        }
    }

    if (size > SIZE(addr)) {
        if (headOfHeap == addr + SIZE(addr)) {
            headOfHeap = addr + size;
            SIZE(addr) = size;
            return addr;
        }
        int checkSize = size - SIZE(addr) + sizeof(HeapHeader)/2;
        checkSize = checkSize >= sizeof(HeapHeader) ? checkSize : sizeof(HeapHeader);
        
        if (placeInList(firstHole, addr + SIZE(addr), sizeof(HeapHeader)/2) != -1) {
            if (SIZE(addr + SIZE(addr)) >= checkSize) {
                removeNode(&firstHole, SIZE(addr + SIZE(addr)), sizeof(HeapHeader)/2);
                memset(0, addr + SIZE(addr), SIZE(addr + SIZE(addr)));
                insertNode(&firstHole, addr + size, sizeof(HeapHeader)/2);
                SIZE(addr + SIZE(addr)) = size;
                return addr + sizeof(HeapHeader)/2;
            }
        }
        for (uint32_t ptr = firstHole; ptr != -1 && ptr < addr; ptr = NEXT(ptr)) {
            if (SIZE(ptr) + ptr == addr && SIZE(ptr) >= checkSize) {
                removeNode(&firstHole, ptr, sizeof(HeapHeader)/2);
                int addr_size = SIZE(addr);
                SIZE(ptr) += SIZE(addr);
                memcpy(addr+sizeof(HeapHeader)/2, ptr+sizeof(HeapHeader)/2, SIZE(addr)-sizeof(HeapHeader)/2);
                memset(0, addr, addr_size);

            }
        }
    }
    
    uint32_t newPtr = alloc(size-sizeof(HeapHeader)/2);
    memcpy(addr, newPtr, size-sizeof(HeapHeader)/2);
    free(addr);
    return newPtr;
}*/