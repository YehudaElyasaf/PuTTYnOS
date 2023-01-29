#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "syscall.h"

void createProcess(void(*startAddress)(void));
//return: is successful
bool blockProcess(uint32_t pid);
//return: is successful
bool killProcess(uint32_t pid);
//ms = sleep time [miliseconds]
void sleep(int ms);
