#include "network.h"
#include "nic/rtl8139.h"
#include "../io/print.h"

void initNetworking(){
    kprint("\n\tInitializing NIC:\n");
    initRTL8139();
    kprint("\tInitializing ARP table...");
    initARP(getMac());
}
