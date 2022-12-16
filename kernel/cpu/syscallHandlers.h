#pragma once

#include <stdint.h>

enum SyscallsIndexes{
    SYSCALL_PUTCHAR     = 10,
    SYSCALL_GETCHAR     = 11,
    SYSCALL_SEEK        = 12
};

/*
How to add syscal handler:
1.
Add syscall index to Syscall Indexes enum.
2.
Create handler here.
3.
Add handler to /kernel/cpu/syscalls.c, in function initSyscalls();

all syscalls handlers MUST be in this structure:
uint32_t __NAME__SyscallHandler(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4);
*/

uint32_t notImplementedSyscallHandler(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4);
uint32_t putcharSyscallHandler(uint32_t ch, uint32_t color, uint32_t backgroundColor, uint32_t param4);
uint32_t getcharSyscallHandler(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4);
uint32_t seekSyscallHandler(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4);