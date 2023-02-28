#pragma once

#include <stdint.h>
#include "../kernel/network/protocols/ip.h"
#include "../kernel/network/protocols/ethernet.h"

//int to ascii
void itoa(long n, char* buffer);
//int to hex string
void itoh(unsigned long n, char* buffer);
//int to binary string
void itob(unsigned long n, char* buffer);

// hex ascii to int
int stoh(char* buffer);
// ascii to int
int stoi(char* buffer);

long reverseNum(long n, int base);

//IPv4 to string
void IPv4tos(uint8_t IPv4[IPv4_LENGTH], char* buffer);
//MAC to string
void MACtos(uint8_t MAC[MAC_LENGTH], char* buffer);
