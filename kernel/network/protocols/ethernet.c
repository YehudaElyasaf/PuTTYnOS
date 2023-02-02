#include "ethernet.h"
#include "../../../lib/memory.h"

const uint8_t MAC_BROADCAST[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

uint32_t calcFCS(void* ptr, uint32_t size) { return 0;} // for now

void etherSend(void* packet, uint32_t size) {
    EtherPacket toSend;
    toSend.preamble1 = PREAMBLE_BYTE;
    toSend.preamble2AndSFD = PREAMBLE_BYTE|1;
    memcpy(MAC_BROADCAST, toSend.dstMAC, MAC_LENGTH);
    memcpy(currentNIC.MAC, toSend.srcMAC, MAC_LENGTH);

    toSend.type = ET_ARP;

    memcpy(&packet, toSend.dataAndFCS, size);

    uint32_t FCS = calcFCS(&packet, size);
    memcpy(&FCS, toSend.dataAndFCS + size, 4);

    currentNIC.send((NICPacket){&toSend, size+4});
}

void etherRecv(void* data) {

}