#include "ethernet.h"
#include "ip.h"
#include "../nic/rtl8139.h"
#include "../../io/print.h"
#include "../../../lib/memory.h"
#include "../../../lib/convert.h"
#include "../../../lib/string.h"
#include "../../../lib/heap.h"

#define MIN_PACKET_SIZE 60

void etherSend(uint8_t* data, uint32_t size, uint8_t* dstMAC, EtherType type) {
    EtherPacket etherPacket;
    NICPacket nicPacket;
    //etherPacket.preamble1 = switchEndian32bit(PREAMBLE_BYTE);
    //etherPacket.preamble2AndSFD = switchEndian32bit(PREAMBLE_BYTE|1);
    
    memcpy(dstMAC, etherPacket.dstMAC, MAC_LENGTH);
    memcpy(getMac(), etherPacket.srcMAC, MAC_LENGTH);
    etherPacket.type = switchEndian16bit(type);

    nicPacket.size = size + sizeof(etherPacket);
    if(nicPacket.size < MIN_PACKET_SIZE)
        nicPacket.size = MIN_PACKET_SIZE;

    nicPacket.data = alloc(size);
    memset(0, nicPacket.data, nicPacket.size);

    memcpy(&etherPacket, nicPacket.data, sizeof(etherPacket));
    memcpy(data, nicPacket.data + sizeof(etherPacket), size);
    
    NicSend(nicPacket);
    
    free(nicPacket.data);
}

void etherRecv(uint8_t* data) {
    EtherPacket packet;
    memcpy(data, &packet, sizeof(EtherPacket));
    
    if (packet.type == ET_IPV4) {
        ipRecv(data + sizeof(EtherPacket));
    }
    if (packet.type == ET_ARP){
        ARPRecieve(data + sizeof(EtherPacket));
    }
}
