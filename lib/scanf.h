#pragma once

#include <stdint.h>
#include "syscall.h"
#include "../kernel/io/print.h"

// return: kernel system call return
char getchar();

__attribute__((__cdecl__)) int scanf(char* format, /* <type>* <ptrName> ...*/ ...);
