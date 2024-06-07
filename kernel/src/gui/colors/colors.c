/**
 * @author Ali Mirmohammad
 * @file canvas.c
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
 * @abstraction:
 *  - Kernel Colors for GUI.
*/


#include <alinix/types.h>
#include <alinix/gui/colors.h>
#include <alinix/module.h>

MODULE_AUTHOR("Ali Mirmohammad")
MODULE_DESCRIPTION("Kernel Colors for GUI")
MODULE_LICENSE("AGPL-3.0")
MODULE_VERSION("0.1")

static const int AMASK = 0xFF000000;
static const int RBMASK = 0x00FF00FF;
static const int GMASK = 0x0000FF00;
static const int AGMASK = AMASK | GMASK;
static const int ONEALPHA = 0x01000000;

/**
 * Performs alpha blending of two colors.
 *
 * @param color1 The first color in ARGB format (e.g., 0xFF000000 for black).
 * @param color2 The second color in ARGB format (e.g., 0xFFFF0000 for red).
 *
 * @return The resulting blended color in ARGB format.
 *
 * @throws None
 */
const uint32_t AlphaBlend(uint32_t color1, uint32_t color2)
{
    uint32_t a = (color2 & AMASK) >> 24;

    if(a == 0)
        return color1;
    else if(a == 255)
        return color2;
    else
    {
        uint32_t na = 255 - a;
        uint32_t rb = ((na * (color1 & RBMASK)) + (a * (color2 & RBMASK))) >> 8;
        uint32_t ag = (na * ((color1 & AGMASK) >> 8)) + (a * (ONEALPHA | ((color2 & GMASK) >> 8)));

        return ((rb & RBMASK) | (ag & AGMASK));
    }
}