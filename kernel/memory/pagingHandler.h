#pragma once
#include <stdint.h>
#include "../io/print.h"
#include "../cpu/irq.h"

#define PDT_SIZE 1024
#define KERNEL_START 0x1000
#define PAGE_SIZE 0x1000
#define KERNEL_SIZE 0xe
#define KERNEL_END (KERNEL_START + PAGE_SIZE*KERNEL_SIZE)

uint32_t allocPage();

typedef enum {
    PRESENT = 1,
    READWRITE = 2,
    USER = 4,
    WRITETHROUGH = 8,
    CACHE = 16,
    DIRTY = 64 // this is specifically in PTEntry and not in PDEntry
    // you may have noticed i don't mention some other flags, such as PageSize, Global, PageAttributeTable
    // I didn't because we don't need them
} PageFlags;

typedef uint32_t PDEntry;

typedef struct {
    PDEntry entries[PDT_SIZE];
} PD;

typedef uint32_t PTEntry;

typedef struct {
    PTEntry entries[PDT_SIZE];
} PT;

void startVirtualMode(uint32_t address);

void initPDT();

void pagefault(IsrFrame regs);