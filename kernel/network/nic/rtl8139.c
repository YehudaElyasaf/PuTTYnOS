#include "rtl8139.h"

#include "../network.h"
#include "../../io/pci.h"
#include "../../asm.h"
#include "../../../lib/printf.h"
#include "../../../lib/convert.h"
#include "../../../lib/queue.h"
#include "../../../lib/memory.h"

#define VENDOR_ID  0x10EC
#define DEVICE_ID  0x8139

#define BUFFER_LEN 8192 + 16
#define QUEUE_BUFFER_LEN 128
#define SEND_MAX_SIZE   0x700

const int RTL_TRANSMIT_COMMAND[] = {0x10, 0x14, 0x18, 0x1C};
const int RTL_TRANSMIT_START[]   = {0x20, 0x24, 0x28, 0x2C};

NICPacket RTLQueueBuffer[QUEUE_BUFFER_LEN] = {0};

char rx_buffer[BUFFER_LEN] = {0};

Queue RTLQueue;

uint8_t RTL8139IrqNumber = 0;
uint32_t ioAddr = 0;

bool initRTL8139(NetwotkAdapter* nic){
    kprint("\tScanning for NIC...\n");
    uint32_t pciAddr = PCI_ScanForDevice(VENDOR_ID, DEVICE_ID);
    printf("\npcidev: %x\n", pciAddr);
    if (pciAddr == -1){
        kprint("\tCouldn't find NIC address on PCI!");
        return false;
    }

    ioAddr = PCI_Read(pciAddr + 0x10);
    //two last bits reperesent address type
    ioAddr &= (~0x3);
    RTL8139IrqNumber = PCI_Read(pciAddr + 0x3C);

    if(ioAddr == -1){
        kprint("\tCouldn't find NIC!");
        return false;
    }
    else{
        kprint("\tFound device: RTL8139\n");
    }

    uint16_t pciCommand = PCI_Read(pciAddr + 0x6);
    pciCommand |= 1 << 2; 
    PCI_Write(pciAddr, pciCommand);

    //power on
    out8bit(ioAddr + INIT_RTL_CONTROL_REGISTER, POWER_ON_CODE);
    //reset card
    out8bit( ioAddr + RTL_CONTROL_REGISTER, RESET_CODE);
    while( (in8bit(ioAddr + RTL_CONTROL_REGISTER) & RESET_CODE));

    out32bit(ioAddr + RBSTART, rx_buffer); // send uint32_t memory location to RBSTART (0x30)
    memset(0, rx_buffer, BUFFER_LEN);
    
    out16bit(ioAddr + IMR_ISR_FLAGS, 0x0005); // Sets the TOK and ROK bits high

    out32bit(ioAddr + RCR, 0xf);

    irqInstallHandler(RTL8139IrqNumber, RTLIrqHandler);

    out8bit(ioAddr + RTL_CONTROL_REGISTER, 0x0C); // Sets the RE and TE bits high, start recieving packets
    
    for (int i = 0; i < 6; i++)
        nic->MAC[i] = in8bit(ioAddr+i);

    //print MAC adress
    char MACStr[20];
    MACtos(nic->MAC, MACStr);
    printf("\tMAC: %s\n", MACStr);

    nic->IOBase = ioAddr;
    nic->send = RTLSendPacket;
    nic->sendMaxLen = SEND_MAX_SIZE;

    RTLQueue = (Queue){RTLQueueBuffer, 0, QUEUE_BUFFER_LEN, sizeof(NICPacket)};

    return true;
}

void RTLIrqHandler(IsrFrame registers) {
    kcprint("I GOT A MESSAGE\n", GREEN, DEFAULT_COLOR);
    //printPacket("MSG", rx_buffer, 100);
    for(int i=0; i<BUFFER_LEN; i++)
        if(rx_buffer[i]!=0)
            kprinti(i);
    out8bit(ioAddr + IMR_ISR_FLAGS + 2, 0);
    out8bit(ioAddr + IMR_ISR_FLAGS + 3, 0x5);
}

void RTLSendPacket(NICPacket packet) {
    // TODO: implement locking task switch (cli and sti?)
    queuePush(&RTLQueue, &packet);
    NICPacket* packetAddr = queueHead(RTLQueue);
    memcpy(packet.data, packetAddr->data, packet.size); // deep copy
    RTLSendNextPacketInQueue();
}

bool RTLSendNextPacketInQueue() {
    int i = 0;
    for (; i < 4 && in16bit(ioAddr + RTL_TRANSMIT_COMMAND[i]) & (1 << 15); i++);
    if (i == 4) // no pairs which arent used
        return false; // return false, it couldn't send the next packet.

    NICPacket *packet = queueHead(RTLQueue);
    if (!packet) // no packet in queue
        return false;

    printPacket("aaaaa", packet->data, packet->size);

    out32bit(ioAddr + RTL_TRANSMIT_START[i], (uint32_t)packet->data);

    //out32bit(ioAddr + RTL_TRANSMIT_COMMAND[i], ((uint32_t)packet->size) | (48 << 16));
    //while(true) kprint("a");
    

    queuePop(&RTLQueue, 0);
    return true;
}
