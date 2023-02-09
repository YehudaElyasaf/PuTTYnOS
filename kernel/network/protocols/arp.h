#pragma once

#include <stdint.h>
#include "ip.h"
#include "ethernet.h"

#define MAC_LENGTH 6
#define HW_TYPE_ETHERNET 0x1

enum OPERATION_CODES{
    ARP_REQUEST = 0x1,
    ARP_REPLY   = 0x2
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
    uint16_t    operationCode;

    uint8_t     srcHWAdress[MAC_LENGTH]; //source MAC
    uint8_t     srcIP[IPv4_LENGTH];
    uint8_t     dstHWAdress[MAC_LENGTH]; //destination MAC
    uint8_t     dstIP[IPv4_LENGTH];
} __attribute__((packed)) ArpPacket;

void initARP(uint8_t MACAddr[MAC_LENGTH]);
void addToArpTable(uint8_t IPv4[IPv4_LENGTH], uint16_t HWType, uint8_t HWAdress[MAC_LENGTH]);
void printARPTable(int offset);
uint8_t* IPv4toMAC(uint8_t IP[IPv4_LENGTH]);
