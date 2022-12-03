#pragma once

#include <stdint.h>

enum SyscallsIndexes{
    SYSCALL_PRINT   = 10,
    SYSCALL_GETCHAR = 11
};

/*
How to add syscal handler:
1.
Add syscall index to Syscall Indexes enum.
2.
Create handler here.
3.



all syscalls handlers MUST be in this structure:
uint32_t __NAME__SyscallHandler(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4);
*/

uint32_t notImplementedSyscallHandler(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4);
uint32_t printSyscallHandler(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4);
