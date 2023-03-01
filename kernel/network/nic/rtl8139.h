#pragma once

#include <stdint.h>
#include "../../cpu/isr.h"
#include "../network.h"

#define MAC_ADDRES_GROUPS 6

enum RTL8139{
    MAC0                        = 0x0,
    INIT_RTL_CONTROL_REGISTER   = 0x52,
    RBSTART                     = 0x30,
    RTL_CONTROL_REGISTER        = 0x37,
    IMR_ISR_FLAGS               = 0x3C,
    RCR                         = 0x44, // what packets to accept. Broadcast, Multicast, Physical match, All packets
 
    POWER_ON_CODE               = 0x0,
    RESET_CODE                  = 0X10
};

// initiates RTL device, returns true if successful.
bool initRTL8139(NetwotkAdapter* nic);

// this function is the way for software to send packets
void RTLSendPacket(NICPacket packet);


/*
    this is how it actually works, behind the scenes.
    SendPacket is pushing packets to queue, and this function checks
    which transmit register is available, and sends the next packet in the queue in it.
    if nothing is available, or other error occured, it returns false and dont send. else, it returns true.
*/
bool RTLSendNextPacketInQueue();

// This is the IRQ handler that handles recieving packets
void RTLIrqHandler(IsrFrame registers);

uint8_t* getMac();