#include "syscall.h"
#include "../kernel/cpu/syscall.h"
#include "../kernel/io/screen.h"

uint32_t syscall(uint32_t syscallIndex, uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4){
    uint32_t ret = 32323;
    asm("\
        mov %0, %%esi; \
        mov %1, %%eax; \
        mov %2, %%ebx; \
        mov %3, %%ecx; \
        mov %4, %%edx; \
        int %5;"
        : 
        : "r"(syscallIndex), "a"(param1), "b"(param2), "c"(param3), "d"(param4),
            "g"(SYSCALL_IDT_INDEX)
        );
    asm("mov %%eax, %0" : "=a"(ret):);
    kprinti(ret);

    return ret;
}
