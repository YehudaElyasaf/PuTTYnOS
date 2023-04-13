#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "syscall.h"
#include "../kernel/tasking/task.h"

//return: pid
uint32_t createProcess(int(*startAddress)(int, char**), int argc, char** argv);
void exit(int returnValue);
//return: is successful
bool killProcess(uint32_t pid);
//ms = sleep time [miliseconds]
void sleep(int ms);
//wait to end of process
void join(uint32_t pid);
