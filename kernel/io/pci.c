#include "pci.h"
#include "../asm.h"

#define IO_CONFIG_ADDR 0xCF8
#define IO_DATA_ADDR   0xCFC

uint32_t PCI_Read(uint32_t pciAddress) {
    pciAddress &= 0xFFFFFFFC; // address cannot have not 0s in last two bits
    pciAddress |= 0x80000000; // address needs to have 1 in the second bit
    
    out32bit(IO_CONFIG_ADDR, pciAddress);

    return in32bit(IO_DATA_ADDR);
}

uint32_t PCI_Write(uint32_t pciAddress, uint32_t value) {
    pciAddress &= 0xFFFFFFFC;  // address cannot have not 0s in last two bits 
    pciAddress |= 0x80000000;  // address needs to have 1 in the second bit

    out32bit(IO_CONFIG_ADDR, pciAddress);

    out32bit(IO_DATA_ADDR, value);
}

uint32_t PCI_Address(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    return (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC);
}

uint32_t PCI_ScanForDevice(uint16_t vendor, uint16_t dev) {
    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            for (uint8_t func = 0; func < 8; func++) {
                // go through all buses, slots and functions.
                // check if their register has vendor and device id
                // are what you want. if they are, return the address.
                uint32_t addr = PCI_Address(bus, slot, func, 0);
                uint32_t reg = PCI_Read(addr);
                // if the first register in the bus is empty, the whole bus is empty
                if (slot == 0 && reg == ~0 && func == 0)
                    goto nextBus;
                if (reg == ~0) // empty register
                    continue;
                
                // register's first 16 bits are vendor id, and last bits are device id
                if ((uint16_t)reg == vendor && reg >> 16 == dev)
                    return addr;
            }
        }
        nextBus:
        continue;
    }
    return -1;
}

void PCI_ScanAndCallback(uint16_t vendor, uint16_t dev, void (*callback)(uint32_t)) {
    for (uint8_t bus = 0; bus <= (uint8_t)-1; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            for (uint8_t func = 0; func < 8; func++) {
                // go through all buses, slots and functions.
                // check if their register has vendor and device id
                // are what you want. if they are, return the address.
                uint32_t addr = PCI_Address(bus, slot, func, 0);
                uint32_t reg = PCI_Read(addr);
                // if the first register in the bus is empty, the whole bus is empty
                if (slot == 0 && reg == ~0 && func == 0)
                    goto nextBus;
                if (reg == ~0) // empty register
                    continue;
                
                // register's first 16 bits are vendor id, and last bits are device id
                if (reg & 0xFFFF == vendor && reg >> 16 == dev)
                    callback(addr);
            }
        }
        nextBus:
        continue;
    }
}