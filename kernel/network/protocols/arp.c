#include "arp.h"
#include "../../memory/pagingHandler.h"
#include "../../io/print.h"
#include "../../../lib/string.h"
#include "../../../lib/convert.h"
#include "../../../lib/printf.h"

const uint8_t BROADCAST_MAC[MAC_LENGTH] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const uint8_t BROADCAST_IP[IPv4_LENGTH] = {0xFF, 0xFF, 0xFF, 0xFF};

static ArpTableEntry* arpTable;

void initARP(uint8_t MACAddr[MAC_LENGTH]){
    arpTable = NULL;

    addToArpTable(BROADCAST_IP, HW_TYPE_ETHERNET, BROADCAST_MAC);
}

void addToArpTable(uint8_t IPv4[IPv4_LENGTH], uint16_t HWType, uint8_t MACAdress[MAC_LENGTH]){
    ArpTableEntry* newEntry = allocPage();
    for(int i=0; i < IPv4_LENGTH; i++)
        newEntry->IPv4[i] = IPv4[i];
    newEntry->HWType = HWType;
    for(int i=0; i < MAC_LENGTH; i++)
        newEntry->MACAdress[i] = MACAdress[i];
    newEntry->next = NULL;

    if(!arpTable)
        arpTable = newEntry;

    else{
        ArpTableEntry* mov = arpTable;
        while(mov->next)
            mov = mov->next;
        mov->next = newEntry;
    }
}

void static printARPEntry(int offset, char* IPAddress, char* hardwareType, char* hardwareAdress){
    setCursorCol(offset);
    if(IPAddress && hardwareType && hardwareAdress){
        printf("%c %s",186, IPAddress);
        offset += strlen("| 255.255.255.255 ");
        setCursorCol(offset);

        printf("%c %s", 179, hardwareType);
        offset += strlen("| Hardware Type ");
        setCursorCol(offset);

        printf("%c %s", 179, hardwareAdress);
        offset += strlen("| FF:FF:FF:FF:FF:FF ");
        setCursorCol(offset);
        printf("%c\n", 186);
    }
    else{
        for(int i = 0; i < 55; i++)
            kprintc(196);
        kprint("\n");
    }
}
void printARPTable(int offset){ 
    kprint("\n");
    printARPEntry(offset, NULL, NULL, NULL);
    printARPEntry(offset, "IPv4 Adsress", "Hardware Type", "MAC Address");
    printARPEntry(offset, NULL, NULL, NULL);
    
    ArpTableEntry* mov = arpTable;
    while(mov){
        char* IPAddress[20] = {0};
        IPv4tos(mov->IPv4, IPAddress);
        char* MACAdress[20] = {0};
        MACtos(mov->MACAdress, MACAdress);
        
        if(mov->HWType == HW_TYPE_ETHERNET)
            printARPEntry(offset, IPAddress, "Ethernet", MACAdress);
        else
            printARPEntry(offset, IPAddress, "Unknown", MACAdress);
        
        mov = mov->next;
    }

    printARPEntry(offset, NULL, NULL, NULL);
    kprint("\n");
}

uint8_t* IPv4toMAC(uint8_t IP[IPv4_LENGTH]){

}
