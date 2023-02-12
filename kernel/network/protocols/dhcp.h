#pragma once

#include "ip.h"
#include "arp.h"
#include "ethernet.h"

#define DHCP_MAGIC_ID       0x41524945 //random
#define DHCP_PADDING_SIZE   202
#define DHCP_MAGIC_COOKIE   0x63825363
#define MAC_LENGTH          6

enum DHCP_PACKET_TYPES{
    DHCP_DISCOVER_TYPE =    0x1,
    DHCP_OFFER_TYPE =       0x2,
    DHCP_REQUEST_TYPE =     0x3,
    DHCP_ACK_TYPE =         0x5
};

enum DHCP_EXTENSION_CODES{
    DHCP_MESSAGE_TYPE =             53,
    DHCP_MESSAGE_TYPE_LENGTH =      1,

    DHCP_REQUEST_LIST_CODE =        55,
    DHCP_REQUEST_LIST_LENGTH =      4,
    //01 - subnet mask, 03 - list of routers, 0f - ask to get my domain name, 06 - list of DNS servers
    DHCP_REQUEST_LIST =             0x01030f06,
    DHCP_END =                      0xff,

    DHCP_SERVER_IDENTIFIER_CODE =   54,
    DHCP_SERVER_IDENTIFIER_LENGTH = 4
};

typedef struct
{
    uint8_t operation;
    uint8_t HWType;
    uint8_t HWAddrLen;
    uint8_t hops;
    uint32_t id; //random id
    uint16_t seconds; //seconds since request. reserved.
    uint16_t flags; //reserved too.
    uint8_t clientIP[IPv4_LENGTH];
    uint8_t yourIP[IPv4_LENGTH];
    uint8_t serverIP[IPv4_LENGTH];
    uint8_t gatewayIP[IPv4_LENGTH]; //not used
    uint8_t clientMAC[MAC_LENGTH];
    //originally used for MAC padding,
    //server name and boot filename
    uint8_t reserverd[DHCP_PADDING_SIZE];
    uint32_t magicCookie;

    //options inserted here
} __attribute__((packed)) DHCPPacket;

typedef struct{
    uint8_t packetTypeCode;
    uint8_t packetTypeLength;
    uint8_t packetType;

    uint8_t requestListCode;
    uint8_t requestListLength;
    uint32_t requestList;

    uint8_t endmark;
} __attribute__((packed)) DHCPDiscoverOptions;

typedef struct{
    uint8_t packetTypeCode;
    uint8_t packetTypeLength;
    uint8_t packetType;

    uint8_t serverIdentifierCode;
    uint8_t serverIdentifierLength;
    uint32_t serverIdentifier;

    uint8_t endmark;
} __attribute__((packed)) DHCPRequestOptions;

static uint8_t defaultGatewayIPv4[IPv4_LENGTH] = {0xFF, 0xFF, 0xFF, 0xFF};

uint8_t* getDefaultGatewayIPv4();

void DHCPDiscover(uint8_t MAC[MAC_LENGTH]);
void DHCPGetOffer();
void DHCPRequest(uint8_t MAC[MAC_LENGTH]);
void DHCPGetAck();
void DHCPRecv();
