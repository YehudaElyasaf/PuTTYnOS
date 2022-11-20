#pragma once
#include <stdint.h>
#define PDT_SIZE 1024

extern startVirtualMode(uint32_t address);

typedef struct __attribute__((__packed__)) { // packed so it won't pad the struct
    unsigned pageSize : 1;       // page size, 0 is for 4 Kib (32-bit), 1 is for 4 Mib (64-bit)
    unsigned available : 1;      // empty bit for kernel use if we need
    // if page was read by mmu during virtual address finding. CPU will not take care of this, but we don't need it
    unsigned accessed : 1;
    unsigned cacheDisabled : 1;  // if page could be cached
    unsigned writeThrough : 1;   // if set, write-through caching is enabled. if not, write-back is enabled
    unsigned userSupervisor : 1; // if set, can be accessed by user, otherwise user cannot access
    // if set, can write. if not, can only read. WP in cr0 controls whether the kernel can write even though this is not set.
    unsigned readWrite : 1;      
    unsigned present : 1;        // if the page is in the memory at the moment
} PageDirectoryFlags;

typedef struct {
    unsigned address : 20;
    unsigned available : 4; // available bits for kernel use if we need
    PageDirectoryFlags flags;
} PageDirectoryEntry;

typedef struct {
    PageDirectoryEntry entries[PDT_SIZE];
} PDT;

void initPaging(uint32_t address) {
    //PDT* table = (PDT*)address;

    //PageDirectoryEntry firstEntry = {0x00001, 0, (PageDirectoryFlags){0, 0, 1, 1, 1, 1, 1, 1}};
    //table->entries[1] = firstEntry;

    // last entry points to the pdt
    //PageDirectoryEntry lastEntry = {address<<12, 0, (PageDirectoryFlags){0, 0, 0, 1, 1, 0, 1, 1}};
    //table->entries[PDT_SIZE-1] = lastEntry;

    startVirtualMode(address);
}