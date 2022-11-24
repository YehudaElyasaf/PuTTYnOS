#pragma once

#include <stdint.h>

//int to ascii
void itoa(long n, char* buffer);
//int to hex ascii
void itoh(unsigned long n, char* buffer);

long reverseNum(long n, int base);