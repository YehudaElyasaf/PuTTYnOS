#include "ethernet.h"
#include "../nic/rtl8139.h"
#include "ip.h"
#include "../../io/print.h"
#include "../../../lib/memory.h"
#include "../../../lib/convert.h"
#include "../../../lib/string.h"

uint32_t calcFCS(void* ptr, uint32_t size) { return 0;} // for now

void etherSend(uint8_t* data, uint32_t size, uint8_t* dstMAC) {
    EtherPacket toSend;
    NICPacket p;
    toSend.preamble1 = switchEndian32bit(PREAMBLE_BYTE);
    toSend.preamble2AndSFD = switchEndian32bit(PREAMBLE_BYTE|1);
    
    memcpy(dstMAC, toSend.dstMAC, MAC_LENGTH);
    memcpy(getMac(), toSend.srcMAC, MAC_LENGTH);
    toSend.type = switchEndian16bit(ET_ARP);
    
    if (size > ETHER_MAX_LENGTH) {
        memcpy(data, toSend.dataAndFCS, ETHER_MAX_LENGTH);

        uint32_t FCS = calcFCS(&data, ETHER_MAX_LENGTH);
        memcpy(&FCS, toSend.dataAndFCS + ETHER_MAX_LENGTH, 4);

        memcpy(&toSend, &p.data, sizeof(toSend));
        p.size = ETHER_MAX_LENGTH;
        NicSend(p);
        data += ETHER_MAX_LENGTH;
        size -= ETHER_MAX_LENGTH;
    }

    memcpy(data, toSend.dataAndFCS, size);

    uint32_t FCS = calcFCS(&data, size);
    memcpy(&FCS, toSend.dataAndFCS + size, 4);
    memcpy(&toSend, &p.data, sizeof(toSend));
    p.size = size;
    
    NicSend(p);
}

void etherRecv(uint8_t* data) {
    EtherPacket packet;
    memcpy(data, &packet, sizeof(EtherPacket));
    
    if (packet.type == ET_IPV4) {
        ipRecv(packet.dataAndFCS, sizeof(packet.dataAndFCS) - 4);
    }
    if (packet.type == ET_ARP){
        ARPRecieve(packet.dataAndFCS);
    }
}
