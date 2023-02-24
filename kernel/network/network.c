#include "network.h"
#include "nic/rtl8139.h"
#include "../io/print.h"

void initNetworking(){
    kprint("\n\tInitializing NIC:\n");
    initRTL8139();
    kprint("\tInitializing ARP table...\n");
    initARP(getMac());
    kprint("\tConnecting to router...\n");
    DHCPState = NONE;
    connectToRouter();

    kprint("ARP table:");
    printARPTable(2);
}
