#pragma once

#include <stdint.h>

//int to ascii
void itoa(long n, char* buffer);
//int to hex ascii
void itoh(unsigned long n, char* buffer);

// hex ascii to int
int stoh(char* buffer);
// ascii to int
int stoi(char* buffer);


long reverseNum(long n, int base);