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

#include <alinix/gui/canvas.h>
#include <alinix/types.h>
#include <alinix/gui/directgui.h>
#include <alinix/log.h>
#include <alinix/heap.h>
#include <alinix/enums.h>
#include <alinix/syscall.h>
#include <alinix/memory.h>
#include <alinix/ulib.h>
#include <alinix/gui/fonts/font.h>


Canvas* base = 0;

uint8_t directGUIFont[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x24, 0x00, 0x00, 0x00, 0x00, 0x10, 0x08, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x24, 0x00, 0x08, 0x00, 0x00, 0x10, 0x10, 0x10, 0x54, 0x00, 0x00, 0x00, 0x00, 0x04, 0x38, 0x08, 0x3c, 0x7e, 0x04, 0x3e, 0x3c, 0x7e, 0x18, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x18, 0x70, 0x3c, 0x78, 0x7e, 0x7e, 0x3c, 0x42, 0x7c, 0x7e, 0x44, 0x40, 0x41, 0x42, 0x3c, 0x7c, 0x3c, 0x7c, 0x3c, 0x7f, 0x42, 0x42, 0x41, 0x42, 0x44, 0x7e, 0x1c, 0x40, 0x38, 0x10, 0x00, 0x30, 0x00, 0x40, 0x00, 0x02, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x24, 0x12, 0x1c, 0x40, 0x18, 0x10, 0x10, 0x10, 0x38, 0x00, 0x00, 0x00, 0x00, 0x04, 0x4c, 0x18, 0x42, 0x02, 0x0c, 0x20, 0x42, 0x02, 0x24, 0x42, 0x10, 0x00, 0x00, 0x00, 0x00, 0x24, 0x38, 0x24, 0x48, 0x42, 0x44, 0x40, 0x40, 0x42, 0x42, 0x10, 0x04, 0x44, 0x40, 0x63, 0x62, 0x42, 0x42, 0x42, 0x42, 0x42, 0x08, 0x42, 0x42, 0x41, 0x42, 0x44, 0x02, 0x10, 0x40, 0x08, 0x28, 0x00, 0x10, 0x00, 0x40, 0x00, 0x02, 0x00, 0x0c, 0x00, 0x40, 0x00, 0x00, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x48, 0x12, 0x2a, 0xa2, 0x20, 0x10, 0x20, 0x08, 0x28, 0x10, 0x00, 0x00, 0x00, 0x08, 0x4c, 0x28, 0x42, 0x04, 0x14, 0x40, 0x40, 0x04, 0x24, 0x42, 0x10, 0x10, 0x02, 0x00, 0x40, 0x42, 0x44, 0x42, 0x44, 0x40, 0x42, 0x40, 0x40, 0x40, 0x42, 0x10, 0x04, 0x48, 0x40, 0x63, 0x62, 0x42, 0x42, 0x42, 0x42, 0x40, 0x08, 0x42, 0x42, 0x41, 0x24, 0x44, 0x02, 0x10, 0x20, 0x08, 0x44, 0x00, 0x00, 0x00, 0x40, 0x00, 0x02, 0x00, 0x12, 0x00, 0x40, 0x10, 0x04, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x7f, 0x28, 0xa4, 0x10, 0x00, 0x20, 0x08, 0x28, 0x10, 0x00, 0x00, 0x00, 0x08, 0x4c, 0x08, 0x02, 0x18, 0x24, 0x40, 0x40, 0x08, 0x18, 0x42, 0x00, 0x00, 0x0c, 0x7e, 0x30, 0x02, 0x82, 0x42, 0x44, 0x40, 0x42, 0x40, 0x40, 0x40, 0x42, 0x10, 0x04, 0x50, 0x40, 0x55, 0x52, 0x42, 0x42, 0x42, 0x42, 0x20, 0x08, 0x42, 0x42, 0x41, 0x24, 0x44, 0x04, 0x10, 0x20, 0x08, 0x00, 0x00, 0x00, 0x38, 0x40, 0x00, 0x02, 0x00, 0x10, 0x1e, 0x40, 0x00, 0x00, 0x20, 0x10, 0x54, 0x2c, 0x3c, 0x5c, 0x3c, 0x2c, 0x1c, 0x3e, 0x24, 0x22, 0x44, 0x42, 0x24, 0x3e, 0x10, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x24, 0x18, 0x48, 0x20, 0x00, 0x20, 0x08, 0x00, 0x7c, 0x00, 0x7e, 0x00, 0x10, 0x54, 0x08, 0x02, 0x04, 0x44, 0x7c, 0x5c, 0x08, 0x24, 0x3e, 0x00, 0x00, 0x30, 0x00, 0x0c, 0x04, 0x92, 0x42, 0x78, 0x40, 0x42, 0x7c, 0x7c, 0x40, 0x42, 0x10, 0x04, 0x60, 0x40, 0x55, 0x52, 0x42, 0x7c, 0x42, 0x7c, 0x18, 0x08, 0x42, 0x42, 0x49, 0x18, 0x28, 0x08, 0x10, 0x10, 0x08, 0x00, 0x00, 0x00, 0x04, 0x5c, 0x3c, 0x3a, 0x3c, 0x10, 0x22, 0x40, 0x00, 0x04, 0x22, 0x10, 0x2a, 0x12, 0x42, 0x22, 0x44, 0x12, 0x22, 0x10, 0x24, 0x22, 0x44, 0x22, 0x24, 0x02, 0x20, 0x08, 0x04, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x24, 0x0c, 0x10, 0x52, 0x00, 0x20, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x54, 0x08, 0x04, 0x02, 0x44, 0x02, 0x62, 0x10, 0x42, 0x02, 0x00, 0x00, 0x40, 0x00, 0x02, 0x08, 0xaa, 0x7e, 0x44, 0x40, 0x42, 0x40, 0x40, 0x4e, 0x7e, 0x10, 0x04, 0x50, 0x40, 0x49, 0x4a, 0x42, 0x40, 0x42, 0x50, 0x04, 0x08, 0x42, 0x42, 0x49, 0x18, 0x10, 0x10, 0x10, 0x10, 0x08, 0x00, 0x00, 0x00, 0x3c, 0x62, 0x42, 0x46, 0x42, 0x3c, 0x22, 0x5c, 0x10, 0x04, 0x2c, 0x10, 0x2a, 0x12, 0x42, 0x22, 0x44, 0x10, 0x20, 0x10, 0x24, 0x22, 0x54, 0x24, 0x24, 0x04, 0x10, 0x08, 0x08, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0xfe, 0x0a, 0x22, 0x4a, 0x00, 0x20, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x54, 0x08, 0x08, 0x02, 0x7e, 0x02, 0x42, 0x10, 0x42, 0x02, 0x10, 0x10, 0x30, 0x7e, 0x0c, 0x10, 0xaa, 0x42, 0x42, 0x40, 0x42, 0x40, 0x40, 0x42, 0x42, 0x10, 0x44, 0x48, 0x40, 0x49, 0x4a, 0x42, 0x40, 0x42, 0x4c, 0x02, 0x08, 0x42, 0x24, 0x49, 0x24, 0x10, 0x20, 0x10, 0x08, 0x08, 0x00, 0x00, 0x00, 0x44, 0x42, 0x40, 0x42, 0x7e, 0x10, 0x1e, 0x62, 0x10, 0x04, 0x30, 0x10, 0x2a, 0x12, 0x42, 0x32, 0x3c, 0x10, 0x1c, 0x10, 0x24, 0x22, 0x54, 0x18, 0x1c, 0x08, 0x08, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x0a, 0x45, 0x44, 0x00, 0x20, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x64, 0x08, 0x10, 0x02, 0x04, 0x42, 0x42, 0x10, 0x42, 0x02, 0x10, 0x10, 0x0c, 0x00, 0x30, 0x10, 0x94, 0x42, 0x42, 0x40, 0x42, 0x40, 0x40, 0x42, 0x42, 0x10, 0x44, 0x44, 0x40, 0x41, 0x46, 0x42, 0x40, 0x4a, 0x42, 0x42, 0x08, 0x42, 0x24, 0x49, 0x24, 0x10, 0x40, 0x10, 0x08, 0x08, 0x00, 0x00, 0x00, 0x44, 0x42, 0x40, 0x42, 0x40, 0x10, 0x02, 0x42, 0x10, 0x04, 0x28, 0x10, 0x2a, 0x12, 0x42, 0x2c, 0x04, 0x10, 0x02, 0x10, 0x24, 0x22, 0x54, 0x24, 0x04, 0x10, 0x08, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x2a, 0x05, 0x3a, 0x00, 0x10, 0x10, 0x00, 0x00, 0x20, 0x00, 0x00, 0x40, 0x64, 0x08, 0x20, 0x44, 0x04, 0x42, 0x42, 0x10, 0x42, 0x04, 0x00, 0x20, 0x02, 0x00, 0x40, 0x00, 0x40, 0x42, 0x42, 0x42, 0x44, 0x40, 0x40, 0x42, 0x42, 0x10, 0x44, 0x42, 0x40, 0x41, 0x46, 0x42, 0x40, 0x44, 0x42, 0x42, 0x08, 0x42, 0x24, 0x49, 0x42, 0x10, 0x40, 0x10, 0x04, 0x08, 0x00, 0x00, 0x00, 0x44, 0x62, 0x42, 0x46, 0x42, 0x10, 0x22, 0x42, 0x10, 0x24, 0x24, 0x10, 0x2a, 0x12, 0x42, 0x20, 0x04, 0x10, 0x22, 0x10, 0x24, 0x14, 0x54, 0x44, 0x04, 0x20, 0x08, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x1c, 0x02, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x20, 0x00, 0x00, 0x40, 0x38, 0x08, 0x7e, 0x38, 0x04, 0x3c, 0x3c, 0x10, 0x3c, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x42, 0x7c, 0x3c, 0x78, 0x7e, 0x40, 0x3c, 0x42, 0x7c, 0x38, 0x42, 0x7e, 0x41, 0x42, 0x3c, 0x40, 0x3a, 0x42, 0x3c, 0x08, 0x3c, 0x18, 0x36, 0x42, 0x10, 0x7e, 0x1c, 0x04, 0x38, 0x00, 0x7f, 0x00, 0x3a, 0x5c, 0x3c, 0x3a, 0x3c, 0x10, 0x1c, 0x42, 0x08, 0x18, 0x22, 0x08, 0x2a, 0x12, 0x3c, 0x20, 0x02, 0x10, 0x1c, 0x0e, 0x1a, 0x08, 0x2a, 0x42, 0x38, 0x3e, 0x08, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x20, 0x00, 0x00, 0x40, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x60, 0x00, 0x00};

bool RequestFramebuffer(){
    Log(Info,"Requesting the new frame buffer...");
    bool ret = DoSyscall(SYSCALL_GUI_GETLFB, addr);

    if (ret){
        base = malloc(sizeof(Canvas));
    }
    return ret;
}