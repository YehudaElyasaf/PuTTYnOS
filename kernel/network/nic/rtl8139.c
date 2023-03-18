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

#define RX_BUFFER_LEN 0x10000
#define TX_BUFFER_LEN 4096
#define QUEUE_BUFFER_LEN 128
#define SEND_MAX_SIZE   0x700

const int RTL_TRANSMIT_COMMAND[] = {0x10, 0x14, 0x18, 0x1C};
const int RTL_TRANSMIT_START[]   = {0x20, 0x24, 0x28, 0x2C};

NICPacket RTLQueueBuffer[QUEUE_BUFFER_LEN] = {0};

char buffer[RX_BUFFER_LEN + TX_BUFFER_LEN];
char *rx_buffer, *tx_buffer;
Queue RTLQueue;

uint8_t RTL8139IrqNumber = 0;
uint32_t ioAddr = 0;

bool initRTL8139(NetwotkAdapter* nic){
    kprint("\tScanning for NIC...\n");
    uint32_t pciAddr = PCI_ScanForDevice(VENDOR_ID, DEVICE_ID);
    
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

    uint32_t pciCommand = PCI_Read(pciAddr + 0x4);
    pciCommand |= (1 << 2);
    PCI_Write(pciAddr + 0x4, pciCommand);
    printf("PCICMD: %x\n", pciCommand);

    rx_buffer = buffer;
    tx_buffer = buffer + RX_BUFFER_LEN;

    //power on
    out8bit(ioAddr + INIT_RTL_CONTROL_REGISTER, POWER_ON_CODE);
    //reset card
    out8bit( ioAddr + RTL_CONTROL_REGISTER, RESET_CODE);
    while( (in8bit(ioAddr + RTL_CONTROL_REGISTER) & RESET_CODE));

    memset(0, rx_buffer, RX_BUFFER_LEN);
    

    out8bit(ioAddr + RTL_CONTROL_REGISTER, 0x0C); // Sets the RE and TE bits high, start recieving packets
    out32bit(ioAddr + TX_CONFIG, 0x03000700);
    out32bit(ioAddr + RX_CONFIG, /*0xf*/0x01001e3e); //TODO: why?

    irqInstallHandler(RTL8139IrqNumber, RTLIrqHandler);
    out32bit(ioAddr + RBSTART, rx_buffer); // send uint32_t memory location to RBSTART (0x30)
    out16bit(ioAddr + IMR_ISR_FLAGS, 0xFFFF); // Sets the TOK and ROK bits high

    
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
    while(1)
    printf("GOT!\n");
    for(int i=0; i<RX_BUFFER_LEN; i++)
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

    memcpy(packet->data, tx_buffer, packet->size);

    printPacket("aaaaa", tx_buffer, packet->size);

    out32bit(ioAddr + RTL_TRANSMIT_START[i], tx_buffer);
    out32bit(ioAddr + RTL_TRANSMIT_COMMAND[i], ((uint32_t)packet->size) | (48 << 16));
    while(true) kprint("a");

    queuePop(&RTLQueue, 0);
    return true;
}