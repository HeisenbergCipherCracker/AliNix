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


/**
 * Abstraction:
 *      - Device  (CPU / GPU) utility
*/


#ifndef __ALINIX_KERNEL_DEVICE_H
#define __ALINIX_KERNEL_DEVICE_H

#include <alinix/drivers/usb/driver.hpp>
#include <alinix/types.h>

struct device_private;

struct device{
    CONSTANT STRING *name;        
    uint64_t base;
    CONSTANT STRING vendor;   /* Vendor name */
    CONSTANT STRING product;   /* Product name */
    struct dev_pm_domain	*pm_domain;
    struct device_private	*p;
};

#endif /*__ALINIX_KERNEL_DEVICE_H*/