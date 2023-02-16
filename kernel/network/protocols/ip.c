#include "ip.h"
#include "../../../lib/memory.h"
#include "../network.h"

const uint8_t IP_BROADCAST[IPv4_LENGTH] = {0xFF, 0xFF, 0xFF, 0xFF};

uint8_t* getDefaultGatewayIPv4(){
    return defaultGatewayIPv4;
}

void ipRecv(void* data, uint32_t size) {
    IpHeaders packet;
    
    memcpy((uint8_t*)data, &packet, MIN_IHL*4);

    if (packet.totalLength > ETHER_MAX_LENGTH-MIN_IHL*4) {
        // fragmentation. we dont use it, so totalLength should be no more than ETHER_MAX_LENGTH
        packet.totalLength = ETHER_MAX_LENGTH-MIN_IHL*4;
    }

    memcpy((uint8_t*)data, &packet.data, packet.totalLength-MIN_IHL*4);

    if (memeq(packet.dstAddr, currentNIC.IPv4, IPv4_LENGTH) || memeq(packet.dstAddr, IP_BROADCAST, IPv4_LENGTH)) {
        if (packet.protocol == UDP)
            udpRecv(packet.data, packet.totalLength-packet.IHL*4);
    } else {
        etherSend(&packet, packet.totalLength, getDefaultGatewayIPv4());
    }
}