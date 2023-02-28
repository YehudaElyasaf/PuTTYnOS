#pragma once
#include "stdint.h"
#include "../../../lib/queue.h"

typedef struct UDPSocketDescriptor {
    uint16_t port;
    Queue *packets;
} UDPSocketDescriptor;

void udpRecv(void* data, uint32_t size);