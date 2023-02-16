#pragma once

#include <stdint.h>
#include "ip.h"
#include "ethernet.h"
#include "dhcp.h"

#define MAC_LENGTH 6
#define HW_TYPE_ETHERNET 0x1

#define IPv4_LENGTH 4

enum OPERATION_CODES{
    REQUEST_OPCODE = 0x1,
    REPLY_OPCODE   = 0x2
};

typedef struct
{
    //works with IPv4 only
    uint8_t     IPv4[IPv4_LENGTH];
    uint16_t    HWType;
    uint8_t     MACAdress[MAC_LENGTH];

    struct ArpTableEntry* next;
} ArpTableEntry;


typedef struct {
    uint16_t    HWType;
    uint16_t    protocolType; //type is always IPv4
    uint8_t     HWAddrLen;
    uint8_t     protocolAddrLen;

    uint16_t    opcode;

    //FIXME: is src-dst IP's actually client-server IP's?
    uint8_t     srcMAC[MAC_LENGTH];
    uint8_t     srcIP[IPv4_LENGTH];
    uint8_t     dstMAC[MAC_LENGTH];
    uint8_t     dstIP[IPv4_LENGTH];
} __attribute__((packed)) ArpPacket;

void initARP(uint8_t MACAddr[MAC_LENGTH]);
void addToArpTable(uint8_t IPv4[IPv4_LENGTH], uint16_t HWType, uint8_t HWAdress[MAC_LENGTH]);
void printARPTable(int offset);
uint8_t* findInArpTable(uint8_t IP[IPv4_LENGTH]);

void ARPSend(uint8_t IP[IPv4_LENGTH]);
void ARPRecieve(ArpPacket* packet);
