#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "syscall.h"

void createProcess(void(*startAddress)(int, char**), int argc, char** argv);
//return: is successful
bool killProcess(uint32_t pid);
//ms = sleep time [miliseconds]
void sleep(int ms);
