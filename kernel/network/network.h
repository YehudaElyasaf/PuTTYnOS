#pragma once

#define MAC_LENGTH      4
#define IPv4_LENGTH     4
#define IPv6_LENGTH     8

typedef struct {
    uint8_t* address;
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
} NetwotkAdapter;

static NetwotkAdapter currentNIC;

void initNetworking();
