#pragma once
#include "stdint.h"

#define HEAP_START 0xf000; // to not interrupt with kernel code
#define SIZE(addr) ((HeapHeader*)(addr))->size
#define NEXT(addr) ((HeapHeader*)(addr))->next

typedef struct {
    uint32_t size;
    uint32_t next;
} HeapHeader;

uint32_t alloc(uint32_t size);
void free(uint32_t addr);
//uint32_t realloc(uint32_t addr, uint32_t size);