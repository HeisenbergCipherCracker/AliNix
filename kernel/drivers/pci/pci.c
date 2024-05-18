/**
 ** This file is part of AliNix.

**AliNix is free software: you can redistribute it and/or modify
**it under the terms of the GNU Affero General Public License as published by
**the Free Software Foundation, either version 3 of the License, or
**(at your option) any later version.

**AliNix is distributed in the hope that it will be useful,
**but WITHOUT ANY WARRANTY; without even the implied warranty of
**MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**GNU Affero General Public License for more details.

**You should have received a copy of the GNU Affero General Public License
**along with AliNix. If not, see <https://www.gnu.org/licenses/>.
*/

#include <alinix/init.h>
#include <alinix/module.h>
#include <alinix/kernel.h>
#include <alinix/pci.h>
#include <alinix/types.h>

uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset){
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;

    // Create configuration address as per Figure 1
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
    
    outportl(0xCF8,address);

    tmp = (uint16_t)((inportl(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;
}

uint16_t pciCheckVendor(uint8_t bus, uint8_t slot) {
    uint16_t vendor,device;
    if ((vendor == pciConfigReadWord(bus,slot,0,0)) != 0xFFFF){
        device = pciConfigReadWord(bus, slot, 0, 2);
    }
    return (vendor);
}

void checkDevice(uint8_t bus, uint8_t device){
    uint16_t vendor;
    vendor = pciCheckVendor(bus,device);
    if (vendor == 0x8086){
        RET;
    }
 }
 
 void checkFunction(uint8_t bus, uint8_t device, uint8_t function) {
 }

void checkAllBuses(void){
    uint16_t bus;
    uint16_t device;
    for (bus = 0;bus < 256;bus++){
        for (device = 0; device < 32;device++){
            checkDevice(bus,device);
        }
    }
}

void checkBus(uint8_t bus){
    uint8_t device;
    for (device = 0;device < 32;device++){
        checkDevice(bus,device);
    }
}

uint64_t arch_msi_address(uint64_t *data, size_t vector, uint32_t processor, uint8_t edgetrigger, uint8_t deassert) {
	*data = (vector & 0xFF) | (edgetrigger == 1 ? 0 : (1 << 15)) | (deassert == 1 ? 0 : (1 << 14));
	return (0xFEE00000 | (processor << 12));
}