#pragma once

#include "../kernel/cpu/syscall.h"

uint32_t syscall(uint32_t syscallIndex, uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4);
