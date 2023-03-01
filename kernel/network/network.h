#pragma once

#include "protocols/ethernet.h"

#define MAC_LENGTH      6
#define IPv4_LENGTH     4
#define IPv6_LENGTH     8

typedef struct {
    uint8_t* data;
    uint16_t size;
} NICPacket;

typedef struct {
    //Physical layer
    uint32_t IOBase;

    //Data link layer
    uint8_t MAC[MAC_LENGTH];

    //Network layer
    uint8_t IPv4[IPv4_LENGTH];
    uint16_t IPv6[IPv6_LENGTH];

    void (*send)(NICPacket);

    uint32_t sendMaxLen;
} NetwotkAdapter;

void initNetworking();
uint8_t* getIPv4();
void setIPv4(uint8_t* ip);
uint16_t* getIPv6();
void setIPv6(uint8_t* ip);

void NicSend(NICPacket packet);