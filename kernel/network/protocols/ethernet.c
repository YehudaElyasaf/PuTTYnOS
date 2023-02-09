#include "ethernet.h"
#include "../../../lib/memory.h"
#include "../network.h"
#include "ip.h"

const uint8_t MAC_BROADCAST[MAC_LENGTH] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

uint32_t calcFCS(void* ptr, uint32_t size) { return 0;} // for now

void etherSend(void* packet, uint32_t size) {
    EtherPacket toSend;
    NICPacket p;
    toSend.preamble1 = PREAMBLE_BYTE;
    toSend.preamble2AndSFD = PREAMBLE_BYTE|1;
    // TODO: change MAC address to 
    memcpy(MAC_BROADCAST, toSend.dstMAC, MAC_LENGTH);
    memcpy(currentNIC.MAC, toSend.srcMAC, MAC_LENGTH);
    toSend.type = ET_ARP;
    
    while (size > ETHER_MAX_LENGTH) {
        memcpy(&packet, toSend.dataAndFCS, ETHER_MAX_LENGTH);

        uint32_t FCS = calcFCS(&packet, ETHER_MAX_LENGTH);
        memcpy(&FCS, toSend.dataAndFCS + ETHER_MAX_LENGTH, 4);

        memcpy(&toSend, &p.data, sizeof(toSend));
        p.size = ETHER_MAX_LENGTH;
        currentNIC.send(p);
        packet += ETHER_MAX_LENGTH;
        size -= ETHER_MAX_LENGTH;
    }

    memcpy(&packet, toSend.dataAndFCS, size);

    uint32_t FCS = calcFCS(&packet, size);
    memcpy(&FCS, toSend.dataAndFCS + size, 4);
    memcpy(&toSend, &p.data, sizeof(toSend));
    p.size = size;
    currentNIC.send(p);
}

void etherRecv(void* data) {
    EtherPacket packet;
    memcpy(data, &packet, sizeof(EtherPacket));
    
    if (packet.type == ET_IPV4) {
        ipRecv(packet.dataAndFCS, sizeof(packet.dataAndFCS)-4);
    }
}