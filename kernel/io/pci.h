#pragma once
#include "stdint.h"

uint32_t PCI_Read(uint32_t pciAddress);

uint32_t PCI_Write(uint32_t pciAddress, uint32_t value);

// used to make bus, slot, func and offset into an address
uint32_t PCI_Address(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

// used to find and return address of a PCI device with a given vendor and device ids
uint32_t PCI_ScanForDevice(uint16_t vendor, uint16_t dev);

// used to call function for every device that has this vendor and device id
void PCI_ScanAndCallback(uint16_t vendor, uint16_t dev, void (*callback)(uint32_t));