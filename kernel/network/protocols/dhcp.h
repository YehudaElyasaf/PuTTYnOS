#pragma once

#include "ip.h"

typedef struct
{
    
} __attribute__((packed)) DHCPPacket;

static uint8_t defaultGatewayIPv4[IPv4_LENGTH];

uint8_t* getDefaultGatewayIPv4();
