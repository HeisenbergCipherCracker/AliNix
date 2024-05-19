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
#ifndef __ALINIX_KERNEL_INCLUDED_DRIVER_HPP
#define __ALINIX_KERNEL_INCLUDED_DRIVER_HPP


namespace alinix{
    namespace drivers{
        namespace usb{
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
        }
    }
}


#endif // __ALINIX_KERNEL_INCLUDED_DRIVER_HPP