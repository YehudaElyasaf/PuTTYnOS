#pragma once

#include <stdint.h>

enum SyscallsIndexes{
    PRINT = 0
};

//all syscalls handlers MUST be in this form:
//uint32_t __NAME__SyscallHandler(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4);

uint32_t printSyscallHandler(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4);
