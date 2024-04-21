#ifndef __ALINIX_KERNEL_USB_DRIVER_SYSTEM_HPP
#define __ALINIX_KERNEL_USB_DRIVER_SYSTEM_HPP


// #include <system/drivers/driver.h>
#include <system/drivers/driver.hpp>
#include <system/drivers/usb/usbdevice.hpp>
#include <alinix/list.hpp>
#include <system/drivers/usb/usbcontroller.hpp>


// typedef  InterruptTransfer InterruptTransfer ;

class USBDriver : public Driver
{
public:
    // Which device is this driver for
    USBDevice* device;
public:
    USBDriver(USBDevice* dev, char* driverName);
    virtual ~USBDriver();

    // De-Active this driver from the system
    // Called when device is unplugged
    virtual void DeInitialize();

    // Called from USB Controller when a interrupt packet is received
    virtual bool HandleInterruptPacket(InterruptTransfer_t* transfer);
};





#endif /*__ALINIX_KERNEL_USB_DRIVER_SYSTEM_HPP*/