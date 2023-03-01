#include "network.h"
#include "nic/rtl8139.h"
#include "../PuTTYn.h"
#include "../io/print.h"
#include "../../lib/memory.h"
#include "../../lib/string.h"
#include "../../lib/convert.h"
#include "../../lib/printf.h"

#define BYTES_PER_LINE 4

static NetwotkAdapter currentNIC;

void initNetworking(){
    kprint("\n\tInitializing NIC:\n");
    initRTL8139(&currentNIC);
    kprint("\tInitializing ARP table...\n");
    initARP(getMac());
    kprint("\tConnecting to router...\n");
    DHCPState = NONE;
    connectToRouter();

    kprint("ARP table:");
    printARPTable(2);
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

static void printPacket(char* title, uint8_t* pData, int size){
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

void NicSend(NICPacket packet){
    currentNIC.send(packet);

    #ifdef _NETWORKING_DEBUG_LOG
    printPacket("Sent", &packet.data, packet.size + 14+8);
    #endif
}
