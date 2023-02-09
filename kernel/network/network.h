#pragma once

#include "protocols/ethernet.h"
#define MAC_LENGTH      6
#define IPv4_LENGTH     4
#define IPv6_LENGTH     8

struct _EtherPacket;

typedef struct {
    struct _EtherPacket data;
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
    void (*recv)(NICPacket);

    uint32_t sendMaxLen;
} NetwotkAdapter;

static NetwotkAdapter currentNIC;

void initNetworking();
uint8_t* getMac();
uint8_t* getIPv4();
uint16_t* getIPv6();
