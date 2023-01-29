#pragma once

#include <stdint.h>
#include "../../cpu/isr.h"

#define MAC_LENGTH      4
#define IPv4_LENGTH     4
#define IPv6_LENGTH     8

#define MAC_ADDRES_GROUPS 6

typedef struct
{
    //Physical layer
    uint32_t IOBase;

    //Data link layer
    uint8_t MAC[MAC_LENGTH];

    //Network layer
    uint8_t IPv4[IPv4_LENGTH];
    uint16_t IPv6[IPv6_LENGTH];


} NetwotkAdapter;

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

typedef struct {
    uint8_t* address;
    uint16_t size;
} RTLPacket;


// initiates RTL device, returns true if successful.
uint8_t initRTL8139();

// this function is the way for software to send packets
void RTLSendPacket(RTLPacket packet);


/*
    this is how it actually works, behind the scenes.
    SendPacket is pushing packets to queue, and this function checks
    which transmit register is available, and sends the next packet in the queue in it.
    if nothing is available, or other error occured, it returns false and dont send. else, it returns true.
*/
uint8_t RTLSendNextPacketInQueue();

// This is the IRQ handler that handles recieving packets
void RTLIrqHandler(IsrFrame registers);