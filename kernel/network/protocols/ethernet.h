#pragma once
#include <stdint.h>
#include "arp.h"
#include "ip.h"

#define PREAMBLE_BYTE 0xAAAA
#define ETHER_MAX_LENGTH 1500
#define MAC_LENGTH 6

typedef enum {
    ET_IPV4 = 0x0800,
    ET_IPV6 = 0x86DD,
    ET_ARP  = 0x0806
} EtherType;

typedef struct _EtherPacket{
    //TODO: preambles and FSC
    //uint32_t preamble1;
    //uint32_t preamble2AndSFD;
    uint8_t dstMAC[MAC_LENGTH];
    uint8_t srcMAC[MAC_LENGTH];
    uint16_t type;
    //uint8_t dataAndFCS[ETHER_MAX_LENGTH + 4];
} __attribute__((packed)) EtherPacket;

void etherSend(uint8_t* data, uint32_t size, uint8_t* dstMAC, EtherType type);
void etherRecv(uint8_t* data);
