#include "network.h"
#include "nic/rtl8139.h"
#include "../PuTTYn.h"
#include "../io/print.h"
#include "../../lib/memory.h"
#include "../../lib/string.h"
#include "../../lib/convert.h"
#include "../../lib/printf.h"

#define BYTES_PER_LINE 16

void initNetworking(){
    kprint("\n\tInitializing NIC:\n");

    if(initRTL8139(&currentNIC)){
        
        kprint("\tInitializing ARP table...\n");
        initARP(getMac());
        kprint("\tConnecting to router...\n");
        DHCPState = NONE;
        connectToRouter();

        kprint("ARP table:");
        printARPTable(2);
    }
}


uint8_t* getMac(){
    return currentNIC.MAC;
}

uint8_t* getIPv4(){
    return currentNIC.IPv4;
}
void setIPv4(uint8_t ip[IPv4_LENGTH]){
    memcpy(ip, currentNIC.IPv4, IPv4_LENGTH);
}

uint16_t* getIPv6(){
    return currentNIC.IPv6;
}
void setIPv6(uint8_t ip[IPv6_LENGTH]){
    memcpy(ip, currentNIC.IPv6, IPv6_LENGTH);
}

void printPacket(char* title, uint8_t* pData, int size){
    uint8_t saveColor = getColor();
    setColor(YELLOW);

    printf("%C\n\t%s\tSize: %d\n\t", LIGHT_PURPLE, DEFAULT_COLOR, title, size);
    for(int i=0; i<72; i++)
        kcprintc('-', PURPLE, DEFAULT_COLOR);

    for(int i=0; i<size; i += BYTES_PER_LINE){
        //print line
        int bytesToPrint = BYTES_PER_LINE;

        kcprint("\n\t| ", PURPLE, DEFAULT_COLOR);

        
        for(int j = 0; j < BYTES_PER_LINE;j++){
            char converBuffer[10];
            itoh(pData[j], converBuffer);
            
            //seperator space
            if(j == BYTES_PER_LINE / 2)
                kprintc(' ');

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
            //seperator space
            if(j == BYTES_PER_LINE / 2)
                kprintc(' ');

            //check if packet ended
            if(i + j >= size){
                kprintc(' ');
                continue;
            }
            
            if( pData[j] == 0 ||
                pData[j] == '\t' ||
                pData[j] == '\n' ||
                pData[j] == 0xff)
                kcprintc('.', GRAY, getBackgroundColor());
            else
                kprintc(pData[j]);
        }

        kcprint(" |", PURPLE, DEFAULT_COLOR);
        pData += BYTES_PER_LINE;
    }

    kprint("\n\t");
    for(int i=0; i<72; i++)
        kcprintc('-', PURPLE, DEFAULT_COLOR);
    
    kprintc('\n');
    setColor(saveColor);
}

void NicSend(NICPacket packet){
    currentNIC.send(packet);

    #ifdef _NETWORKING_DEBUG_LOG
    printPacket("Sent", packet.data, packet.size);
    #endif
}
