#pragma once

typedef struct {
    unsigned present : 1;        // if the page is in the memory at the moment
    // if set, can write. if not, can only read. WP in cr0 controls whether the kernel can write even though this is not set.
    unsigned readWrite : 1;      
    unsigned userSupervisor : 1; // if set, can be accessed by user, otherwise user cannot access
    unsigned writeThrough : 1;   // if set, write-through caching is enabled. if not, write-back is enabled
    unsigned cacheDisabled : 1;  // if page could be cached
    // if page was read by mmu during virtual address finding. CPU will not take care of this, but we don't need it
    unsigned accessed : 1;      
    unsigned available : 1;      // empty bit for kernel use if we need
    unsigned pageSize : 1;       // page size, 0 is for 4 Kib (32-bit), 1 is for 4 Mib (64-bit)
} PageDirectoryFlags;

typedef struct {
    PageDirectoryFlags flags;
    unsigned available : 4; // available bits for kernel use if we need
    unsigned address : 12;
} PageDirectoryEntry;