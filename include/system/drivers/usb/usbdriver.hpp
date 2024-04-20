#ifndef __ALINIX_KERNEL_USB_DRIVER_SYSTEM_HPP
#define __ALINIX_KERNEL_USB_DRIVER_SYSTEM_HPP


#include <system/drivers/driver.h>
#include <system/drivers/driver.hpp>
#include <system/drivers/usb/usbdevice.hpp>


        typedef struct InterruptTransfer InterruptTransfer_t;
class USBDevice;

class USBDriver : public Driver
{
public:
    // Constructor
    USBDriver(USBDevice* dev, char* driverName);

    // Destructor
    virtual ~USBDriver();

    // Other member functions
    virtual void DeInitialize();
    virtual bool HandleInterruptPacket(InterruptTransfer_t* transfer);

private:
    USBDevice* device; // Pointer to USBDevice
};




#endif /*__ALINIX_KERNEL_USB_DRIVER_SYSTEM_HPP*/