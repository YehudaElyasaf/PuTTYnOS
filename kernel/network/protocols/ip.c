#include "ip.h"
#include "../../../lib/memory.h"
#include "../network.h"

#define SEND_TTL 1024

const uint8_t IP_BROADCAST[IPv4_LENGTH] = {0xFF, 0xFF, 0xFF, 0xFF};

uint8_t* getDefaultGatewayIPv4(){
    return defaultGatewayIPv4;
}

uint16_t ipChecksum(IpHeaders* packet, uint8_t toVerify) {
    uint32_t output = 0;
    for (uint16_t* ptr = packet; ptr < (uint16_t*)packet + packet->IHL*2; ptr++) {
        // dont add the checksum field, unless you want to verify whether the checksum is correct
        // when you verify whether the checksum is correct, if the output is not 0 it's incorrect
        if (toVerify && ptr == &packet->headerChecksum)
            continue;
        
        output += *ptr;
    }

    while (output << 16) {
        // after adding up the bytes, the carry of summing all of the bytes is
        // in the 3rd byte of the output (bit 17 and after).
        // while there is carry, add the carry to the output.
        output += output << 16;
    }

    return ~(uint16_t)output; // make it a one's complement of the sum
}

void ipRecv(void* data, uint32_t size) {
    IpHeaders packet;
    
    memcpy((uint8_t*)data, &packet, MIN_IHL*4);

    if (packet.totalLength > ETHER_MAX_LENGTH-MIN_IHL*4) {
        // fragmentation. we dont use it, so totalLength should be no more than ETHER_MAX_LENGTH
        packet.totalLength = ETHER_MAX_LENGTH-MIN_IHL*4;
    }

    // verify packetChecksum. if it returns 0 it's fine, otherwise the packet is not fully correct.
    if (ipChecksum(&packet, 1)) {
        printf("Error! packet checksum mismatch.\n");
    }

    memcpy((uint8_t*)data, &packet.data, packet.totalLength-MIN_IHL*4);

    if (memeq(packet.dstAddr, currentNIC.IPv4, IPv4_LENGTH) || memeq(packet.dstAddr, IP_BROADCAST, IPv4_LENGTH)) {
        if (packet.protocol == UDP)
            udpRecv(packet.data, packet.totalLength-packet.IHL*4);
    } else {
        // reroute packet to default gateway 
        etherSend(&packet, packet.totalLength, findInArpTable(getDefaultGatewayIPv4()));
    }
}

void ipSend(void* data, uint32_t size, uint8_t dst[IPv4_LENGTH], uint8_t protocol) {
    if (size > IPv4_MAX_DATA_SIZE) {
        // with fragmentation enabled this part will change to fragmentize
        printf("Error! Packet too large.\n");
        return;
    }
    IpHeaders packet;
    packet.version = 4;
    packet.IHL = MIN_IHL;


    packet.totalLength = size+MIN_IHL;
    packet.TTL = SEND_TTL;
    packet.protocol = protocol;

    memcpy(currentNIC.IPv4, packet.srcAddr, IPv4_LENGTH);
    memcpy(dst, packet.dstAddr, IPv4_LENGTH);
    
    packet.headerChecksum = ipChecksum(&packet);

    memcpy(data, packet.data, size);

    if (findInArpTable(dst))
        etherSend(&packet, packet.totalLength, findInArpTable(dst));
    else
        etherSend(&packet, packet.totalLength, findInArpTable(getDefaultGatewayIPv4()));
}