#include "rtl8139.h"
#include "../network.h"
#include "../../io/pci.h"
#include "../../asm.h"
#include "../../../lib/printf.h"
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

Queue RTLQueue = {RTLQueueBuffer, 0, QUEUE_BUFFER_LEN, sizeof(NICPacket)};

uint32_t ioAddr = 0;

uint8_t initRTL8139() {
    kprint("\n\tScanning for NIC...\n");
    int pciAddr = PCI_ScanForDevice(VENDOR_ID, DEVICE_ID);
    if (pciAddr == -1){
        kprint("\tCouldn't find NIC address on PCI!");
        return -1;
    }

    ioAddr = PCI_Read(pciAddr + 0x10);

    if(ioAddr == 0xFFFFFFFF){
        kprint("\tCouldn't find NIC!");
        return -1;
    }
    else{
        printf("\tFound device: RTL8139\n");
    }

    //power on
    out8bit(ioAddr + INIT_RTL_CONTROL_REGISTER, POWER_ON_CODE);
    //reset card
    out8bit( ioAddr + RTL_CONTROL_REGISTER, RESET_CODE);
    while( (in8bit(ioAddr + RTL_CONTROL_REGISTER) & RESET_CODE) != 0) { }

    out32bit(ioAddr + RBSTART, rx_buffer); // send uint32_t memory location to RBSTART (0x30)
    
    out32bit(ioAddr + IMR_ISR_FLAGS, 0x0005); // Sets the TOK and ROK bits high

    out8bit(ioAddr + RCR, 0xf); // accept all packets

    irqInstallHandler(IRQ10_NETWORK_ADAPTER, RTLIrqHandler);   

    out8bit(ioAddr + RTL_CONTROL_REGISTER, 0x0C); // Sets the RE and TE bits high, start recieving packets

    // mac finding doesnt work for now.
    for (int i = 0; i < 6; i++) {
        currentNIC.MAC[i] = in8bit(ioAddr+i);
    }

    //print MAC adress
    printf("\tMAC: ");
    for (int i = 0; i < MAC_LENGTH; i++) {
        printf("%x", currentNIC.MAC[i]);
        if (i != MAC_LENGTH-1)
            printf(":");
        else
            printf("\n");
    }

    currentNIC.IOBase = ioAddr;
    currentNIC.recv = RTLIrqHandler;
    currentNIC.send = RTLSendPacket;
    currentNIC.sendMaxLen = SEND_MAX_SIZE;
}

void RTLIrqHandler(IsrFrame registers) {
    printf("MSG: %s\n", rx_buffer);
    out8bit(ioAddr + IMR_ISR_FLAGS + 2, 0);
    out8bit(ioAddr + IMR_ISR_FLAGS + 3, 0x5);
}

void RTLSendPacket(NICPacket packet) {
    // TODO: implement locking task switch (cli and sti?)
    queuePush(&RTLQueue, &packet);
    NICPacket* packetAddr = queueHead(RTLQueue);
    memcpy(packet.data.dataAndFCS, packetAddr->data.dataAndFCS, packet.size); // deep copy
}

uint8_t RTLSendNextPacketInQueue() {
    int i = 0;
    for (; i < 4 && !(in16bit(ioAddr + RTL_TRANSMIT_COMMAND[i]) & 1 << 15); i++);
    if (i == 4) // no pairs which arent used
        return 0; // return false, it couldn't send the next packet.

    NICPacket *packet = queueHead(RTLQueue);
    if (!packet) // no packet in queue
        return 0;

    out16bit(ioAddr + RTL_TRANSMIT_COMMAND[i], packet->size | 1 << 13 | 1 << 15);

    out32bit(ioAddr + RTL_TRANSMIT_START[i], (uint32_t)&packet->data);

    out16bit(ioAddr + RTL_TRANSMIT_COMMAND[i], packet->size | 1 << 13); // clear 1 from send bit, it will start to send the packet

    queuePop(&RTLQueue, 0);

    return 1;
}
