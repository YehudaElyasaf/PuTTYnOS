#pragma once

#include <stdbool.h>
#include "ip.h"
#include "arp.h"
#include "ethernet.h"

#define DHCP_MAGIC_ID       0x41524945 //random
#define DHCP_PADDING_SIZE   202
#define DHCP_MAGIC_COOKIE   0x63825363
#define MAC_LENGTH          6

#define DHCP_TYMEOUT_MS     3000

#define IPv4_LENGTH 4 // for some reason it doesnt work with ip.h

typedef enum{
    NONE,
    DHCP_DISCOVER_TYPE,
    DHCP_OFFER_TYPE,
    DHCP_REQUEST_TYPE,
    DHCP_DECLINE_TYPE,
    DHCP_ACK_TYPE
} DhcpPacketType;

typedef enum{
    DHCP_OPTION_SUBNET_MASK =       1,
    DHCP_OPTION_ROUTER =            3,
    DHCP_OPTION_DNS_SERVERS =       6,
    DHCP_OPTION_DOMAIN_NAME =       15,
    DHCP_OPTION_IP_LEASE_TIME_SEC = 51,
    DHCP_OPTION_MESSSAGE_TYPE =     53,
} DhcpOptions;

enum DHCP_REQUSETS_OPTIONAL_CODES{
    DHCP_MESSAGE_TYPE =             DHCP_OPTION_MESSSAGE_TYPE,
    DHCP_MESSAGE_TYPE_LENGTH =      1,

    DHCP_REQUEST_LIST_CODE =        55,
    DHCP_REQUEST_LIST_LENGTH =      4,
    //01 - subnet mask, 03 - list of routers, 0f - ask to get my domain name, 06 - list of DNS servers
    //TODO: get more info (time servers, etc.)
    DHCP_REQUEST_LIST =             0x01030f06,
    DHCP_END =                      0xff,
};

typedef struct{
    uint8_t packetTypeCode;
    uint8_t packetTypeLength;
    uint8_t packetType;

    uint8_t requestListCode;
    uint8_t requestListLength;
    uint32_t requestList;

    uint8_t endmark;
} __attribute__((packed)) DHCPRequestOptions;

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

    //only for Discover and Request!
    DHCPRequestOptions options;
} __attribute__((packed)) DHCPPacket;

//return: is successful
bool connectToRouter();

//return: is successful
static bool DHCPWaitForReply(DhcpPacketType replyType);
//return: is successful
static bool DHCPDiscover();
static void DHCPGetOffer(DHCPPacket* packet, uint8_t* options);
//return: is successful
static bool DHCPRequest();
static void DHCPGetAck(DHCPPacket* packet, uint8_t* pOptions);
void DHCPRecv();

static DhcpPacketType DHCPState;
