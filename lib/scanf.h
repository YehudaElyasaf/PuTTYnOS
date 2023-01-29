#pragma once

#include <stdint.h>
#include "syscall.h"
#include "../kernel/io/print.h"

#define BUFFER_LEN 1024

// return: kernel system call return
char getchar();
// add offset to cursor position
void seek(uint32_t offset);

__attribute__((__cdecl__)) int scanf(char* format, /* <type>* <ptrName> ...*/ ...);
