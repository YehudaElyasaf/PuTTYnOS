#include "rtl8139.h"
#include "../../io/pci.h"
#include "../../asm.h"
#include "../../../lib/printf.h"
#include "../../../lib/queue.h"

#define VENDOR_ID  0x10EC
#define DEVICE_ID  0x8139

#define BUFFER_LEN 8192 + 16
#define QUEUE_BUFFER_LEN 128
#define SEND_MAX_SIZE   0x700

const int RTL_TRANSMIT_COMMAND[] = {0x10, 0x14, 0x18, 0x1C};
const int RTL_TRANSMIT_START[]   = {0x20, 0x24, 0x28, 0x2C};

RTLPacket RTLQueueBuffer[QUEUE_BUFFER_LEN] = {0};

char rx_buffer[BUFFER_LEN] = {0};

Queue RTLQueue = {RTLQueueBuffer, 0, QUEUE_BUFFER_LEN, sizeof(RTLPacket)};

uint8_t macAddr[6];

uint32_t ioAddr = 0;

uint8_t initRTL8139() {
    kprint("\n\tScanning for NIC...\n");
    int pciAddr = PCI_ScanForDevice(VENDOR_ID, DEVICE_ID);
    if (pciAddr == -1){
        kprint("\tCouldn't find NIC address on PCI!");
        return -1;
    }

    ioAddr = in32bit(pciAddr + 0x10);

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
        macAddr[i] = in8bit(ioAddr+i);
    }

    //print MAC adress
    printf("\tMAC: ", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
    for(int i = 0; i < MAC_ADDRES_GROUPS; i++){
        //print padding
        if(macAddr[i] < 0xA)
            printf("0");

        printf("%x", macAddr[i]);
        if(i != MAC_ADDRES_GROUPS - 1)
            //not last field
            printf("-");
    }
}

void RTLIrqHandler(IsrFrame registers) {
    printf("MSG: %s\n", rx_buffer);
    out8bit(ioAddr + IMR_ISR_FLAGS + 2, 0);
    out8bit(ioAddr + IMR_ISR_FLAGS + 3, 0x5);
}

void RTLSendPacket(RTLPacket packet) {
    while (packet.size > SEND_MAX_SIZE) {
        RTLPacket smallerPacket = {packet.address, SEND_MAX_SIZE};
        queuePush(&RTLQueue, &smallerPacket);
        packet.size -= SEND_MAX_SIZE;
        packet.address += SEND_MAX_SIZE;
    }
    queuePush(&RTLQueue, &packet);
}

uint8_t RTLSendNextPacketInQueue() {
    int i = 0;
    for (; i < 4 && !(in16bit(ioAddr + RTL_TRANSMIT_COMMAND[i]) & 1 << 15); i++);
    if (i == 4) // no pairs which arent used
        return 0; // return false, it couldn't send the next packet.

    RTLPacket packet = {0};
    queuePop(&RTLQueue, &packet);
    if (!packet.address && !packet.size) // no packet in queues
        return 0;

    out16bit(ioAddr + RTL_TRANSMIT_COMMAND[i], packet.size | 1 << 13 | 1 << 15);

    out32bit(ioAddr + RTL_TRANSMIT_START[i], (uint32_t)packet.address);

    out16bit(ioAddr + RTL_TRANSMIT_COMMAND[i], packet.size | 1 << 13); // clear 1 from send bit, it will start to send the packet
    return 1;    
}
