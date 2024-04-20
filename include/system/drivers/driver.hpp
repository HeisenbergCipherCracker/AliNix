#ifndef __ALINIX_KERNEL_SYSTEM_DRIVERS_HPP
#define __ALINIX_KERNEL_SYSTEM_DRIVERS_HPP

#ifndef __cplusplus
# error "This file must be compiled with C++"

#endif


#include <system/drivers/usb/usbdevice.h>

class Driver
    {
    private:
        char* Name;
        char* Description;
    public:
        Driver(char* name = 0, char* description = 0);

        char* GetDriverName();
        char* GetDriverDescription();

        virtual bool Initialize();
    };


static char* GetDriverName();
static char* GetDriverDescription();

static bool Initialize();


#endif //__ALINIX_KERNEL_SYSTEM_DRIVERS_HPP