#include "arp.h"
#include "../nic/rtl8139.h"
#include "../../memory/pagingHandler.h"
#include "../../io/print.h"
#include "../../../lib/string.h"
#include "../../../lib/convert.h"
#include "../../../lib/printf.h"
#include "../../../lib/memory.h"

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

uint8_t* findInArpTable(uint8_t IP[IPv4_LENGTH]){
    ArpTableEntry* mov = arpTable;

    while (mov)
    {
        if(mov->IPv4[0] == IP[0] && mov->IPv4[1] == IP[1] &&
        mov->IPv4[2] == IP[2] && mov->IPv4[3] == IP[3])
            return mov->MACAdress;

        mov = mov->next;
    }
    
    return NULL;
}

void ARPSend(uint8_t IP[IPv4_LENGTH]){
    ArpPacket* packet = allocPage();

    packet->HWType = HW_TYPE_ETHERNET;
    packet->protocolType = ET_IPV4;
    packet->HWAddrLen = MAC_LENGTH;
    packet->protocolAddrLen = IPv4_LENGTH;

    packet->opcode = ARP_REQUEST;
    
    memcpy(getMac(), packet->srcMAC, MAC_LENGTH);
    
    //TODO: my ip from NIC, setted with DHCP
    memcpy(getIPv4(), packet->srcIP, IPv4_LENGTH);
    memcpy(BROADCAST_MAC, packet->dstMAC, MAC_LENGTH);
    //TODO: router ip with DHCP
    memcpy(getDefaultGatewayIPv4(), packet->dstIP, IPv4_LENGTH);

    etherSend(packet, sizeof(packet), BROADCAST_MAC);
}

void ARPRecieve(uint8_t IP[IPv4_LENGTH]){

}
