#pragma once

#include <stdint.h>

#define IPv4_LENGTH 4

static uint8_t defaultGatewayIPv4[IPv4_LENGTH] = {0xFF, 0xFF, 0xFF, 0xFF};

uint8_t* getDefaultGatewayIPv4();

typedef struct {

} IpHeaders;

void ipRecv(void* data, uint32_t size);