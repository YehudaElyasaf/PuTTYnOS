#pragma once
#include <stdint.h>

#define IPv4_LENGTH 4

typedef struct {

} IpHeaders;

void ipRecv(void* data, uint32_t size);