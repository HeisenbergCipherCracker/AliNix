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
#ifndef __ALINIX_KERNEL_GAME_PORT_H
#define __ALINIX_KERNEL_GAME_PORT_H


/**
 * @ref https://github.com/torvalds/linux/blob/master/include/linux/gameport.h#L16
 * @ref https://wiki.osdev.org/Game_port
*/

// Joy stick code goes here

/* Joystick 200h - 201h. */
#define JOYSTICK_PORT 0x201
#define GARBAGE_DATA       0xAA55
#define MAX_TIME_ATTEMPTS  1000

#endif 