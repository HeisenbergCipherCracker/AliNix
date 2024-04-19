/**
 * @author Ali Mirmohammad
 * @file colors.h
 * *************************************IMPORTANT ALINIX LICENSE TERM********************************************
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
#ifndef __ALINIX_KERNEL_GUI_COLORS_H
#define __ALINIX_KERNEL_GUI_COLORS_H

#include <alinix/types.h>


typedef union Color4Tag
{
    uint32_t c;
    struct ColorComponents
    {
        uint8_t b;
        uint8_t g;
        uint8_t r;
        uint8_t a;
    } argb;
} Color4;


static const uint32_t Transparent = 0x00000000;

#endif /*__ALINIX_KERNEL_GUI_COLORS_H*/