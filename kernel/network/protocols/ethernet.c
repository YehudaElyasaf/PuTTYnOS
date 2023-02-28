#include "ethernet.h"
#include "../nic/rtl8139.h"
#include "ip.h"
#include "../../PuTTYn.h"
#include "../../io/print.h"
#include "../../../lib/memory.h"
#include "../../../lib/printf.h"
#include "../../../lib/convert.h"
#include "../../../lib/string.h"

#define BYTES_PER_LINE 4

static void printPacket(char* title, uint8_t* pData, int size);

uint32_t calcFCS(void* ptr, uint32_t size) { return 0;} // for now

void etherSend(uint8_t* data, uint32_t size, uint8_t* dstMAC) {
    EtherPacket toSend;
    NICPacket p;
    toSend.preamble1 = PREAMBLE_BYTE;
    toSend.preamble2AndSFD = PREAMBLE_BYTE|1;
    
    memcpy(dstMAC, toSend.dstMAC, MAC_LENGTH);
    memcpy(currentNIC.MAC, toSend.srcMAC, MAC_LENGTH);
    toSend.type = ET_ARP;
    
    while (size > ETHER_MAX_LENGTH) {
        memcpy(&data, toSend.dataAndFCS, ETHER_MAX_LENGTH);

        uint32_t FCS = calcFCS(&data, ETHER_MAX_LENGTH);
        memcpy(&FCS, toSend.dataAndFCS + ETHER_MAX_LENGTH, 4);

        memcpy(&toSend, &p.data, sizeof(toSend));
        p.size = ETHER_MAX_LENGTH;
        currentNIC.send(p);
        data += ETHER_MAX_LENGTH;
        size -= ETHER_MAX_LENGTH;
    }

    memcpy(&data, toSend.dataAndFCS, size);

    uint32_t FCS = calcFCS(&data, size);
    memcpy(&FCS, toSend.dataAndFCS + size, 4);
    memcpy(&toSend, &p.data, sizeof(toSend));
    p.size = size;
    
    //TODO: polymorphism
    //currentNIC.send(p);
    RTLSendPacket(p);

    #ifdef _NETWORKING_DEBUG_LOG
    printPacket("Sent", data, sizeof(ArpPacket));
    #endif
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

void printPacket(char* title, uint8_t* pData, int size){
    printf("%C\n\t%s\tSize: %d\n\t", LIGHT_PURPLE, DEFAULT_COLOR, title, size);
    for(int i=0; i<60; i++)
        kcprintc('-', PURPLE, DEFAULT_COLOR);

    for(int i=0; i<size; i += BYTES_PER_LINE){
        //print line
        int bytesToPrint = BYTES_PER_LINE;

        kcprint("\n\t| ", PURPLE, DEFAULT_COLOR);

        for(int j = 0; j < BYTES_PER_LINE;j++){
            char convertBuffer[10];
            itob(pData[j], convertBuffer);

            //check if packet ended
            if(i + j >= size){
                kprint("         ");
                continue;
            }

            //print padding
            for(int k=strlen(convertBuffer); k < 8; k++)
                kprintc('0');
            printf("%s ", convertBuffer);
        }
        
        kcprint("| ", PURPLE, DEFAULT_COLOR);
        
        for(int j = 0; j < BYTES_PER_LINE;j++){
            char converBuffer[10];
            itoh(pData[j], converBuffer);
            
            //check if packet ended
            if(i + j >= size){
                kprint("   ");
                continue;
            }

            //print padding
            for(int k=strlen(converBuffer); k < 2; k++)
                kprintc('0');
            printf("%s ", converBuffer);
        }

        kcprint("| ", PURPLE, DEFAULT_COLOR);
        
        for(int j = 0; j < BYTES_PER_LINE;j++){
            //check if packet ended
            if(i + j >= size){
                kprintc(' ');
                continue;
            }

            kcprintc(pData[j], getBackgroundColor(), GRAY);
        }

        kcprint(" |", PURPLE, DEFAULT_COLOR);
        pData+=4;
    }

    kprint("\n\t");
    for(int i=0; i<60; i++)
        kcprintc('-', PURPLE, DEFAULT_COLOR);
    
    kprintc('\n');
}
