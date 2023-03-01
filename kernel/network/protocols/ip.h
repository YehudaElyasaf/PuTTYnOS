#pragma once
#include "stdint.h"
#include "ethernet.h"
#include "udp.h"

#define IPv4_LENGTH 4
#define IPv4_MAX_DATA_SIZE 1980 // ETHER_MAX - 20
#define MIN_IHL 5

static uint8_t defaultGatewayIPv4[IPv4_LENGTH] = {0xFF, 0xFF, 0xFF, 0xFF};

uint8_t* getDefaultGatewayIPv4();

enum IP_PROTOCOL_NUMBER {
    ICMP = 1,
    UDP  = 17
};

typedef struct __attribute__((__packed__)){
    unsigned version:4; // version number, in ipv4 format is always 4
    unsigned IHL    :4; // Headers length (muultiply by 32bits), in ipv4 format without options is 5
    uint8_t DSCP_AND_ECN; // optional.
    uint16_t totalLength; // total length of the packet from IP headers and after
    uint16_t identification; // we don't use it
    uint16_t flagsAndFragOffset; // flags and fragment offset. we dont need them because we dont use fragmentation.
    uint8_t TTL;
    uint8_t protocol;
    uint16_t headerChecksum;
    uint8_t srcAddr[IPv4_LENGTH];
    uint8_t dstAddr[IPv4_LENGTH];

    uint8_t data[IPv4_MAX_DATA_SIZE];
} IpHeaders;

void ipRecv(void* data);

// the function recieves a pointer to an IP packet, and whether or not you should verify the checksum

// if you want to receive a checksum for your packet, toVerify = 0, it adds up the headers in 16-bit
// and without the checksum field, and returns the one's complement to that.

// if you want to verify the checksum, toVerify = 1, it adds WITH the checksum field,
// and returns 0 if it's valid and something else if it's not valid.
uint16_t ipChecksum(IpHeaders* packet, uint8_t toVerify);