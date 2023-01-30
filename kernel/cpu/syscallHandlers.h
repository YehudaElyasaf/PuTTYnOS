#pragma once

#include <stdint.h>

enum SyscallsIndexes{
    //10 - 19: I/O Syscalls
    SYSCALL_PUTCHAR     = 10,
    SYSCALL_GETCHAR     = 11,
    SYSCALL_SEEK        = 12,

    //20 - 29: Tasking Syscalls
    SYSCALL_CREATE_TASK = 20,
    SYSCALL_KILL_TASK   = 21,
    SYSCALL_SLEEP       = 22
};

/*
How to add syscall handler:
1.
Add syscall index to Syscall Indexes enum.
2.
Create handler here.
3.
Add handler to /kernel/cpu/syscalls.c, in function initSyscalls();
4.
Create library function to use this syscall in lib/ folder.

all syscalls handlers MUST be in this structure:
uint32_t <NAME>SyscallHandler(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4);
*/

uint32_t notImplementedSyscallHandler(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4);

uint32_t putcharSyscallHandler(uint32_t ch, uint32_t color, uint32_t backgroundColor, uint32_t param4);

uint32_t getcharSyscallHandler(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4);

uint32_t seekSyscallHandler(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4);
//return: pid
uint32_t createTaskSyscallHandler(uint32_t startAddress, uint32_t param2, uint32_t param3, uint32_t param4);
//return: is successfull
uint32_t killTaskSyscallHandler(uint32_t pid, uint32_t param2, uint32_t param3, uint32_t param4);

uint32_t sleepSyscallHandler(uint32_t ms, uint32_t param2, uint32_t param3, uint32_t param4);
