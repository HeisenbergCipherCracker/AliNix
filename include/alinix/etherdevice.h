/**
 * @author Ali Mirmohammad
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
 * @file etherdevice.h
 * Purpose:
 *      - Provide utility functions for Ethernet devices.
*/
#ifndef __ALINIX_KERNEL_ETHER_DEVICE_H
#define __ALINIX_KERNEL_ETHER_DEVICE_H

#include <alinix/types.h>


static inline bool is_zero_ether_addr(const uint8_t *addr)
{
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS)
	return ((*(const u32 *)addr) | (*(const uint16_t *)(addr + 4))) == 0;
#else
	return (*(const uint16_t *)(addr + 0) |
		*(const uint16_t *)(addr + 2) |
		*(const uint16_t *)(addr + 4)) == 0;
#endif
}

static inline bool is_multicast_ether_addr(const uint8_t *addr)
{
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS)
	u32 a = *(const u32 *)addr;
#else
	uint16_t a = *(const uint16_t *)addr;
#endif
#ifdef __BIG_ENDIAN
	return 0x01 & (a >> ((sizeof(a) * 8) - 8));
#else
	return 0x01 & a;
#endif
}



static inline bool is_multicast_ether_addr_64bits(const uint8_t *addr)
{
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS) && BITS_PER_LONG == 64
#ifdef __BIG_ENDIAN
	return 0x01 & ((*(const u64 *)addr) >> 56);
#else
	return 0x01 & (*(const u64 *)addr);
#endif
#else
	return is_multicast_ether_addr(addr);
#endif
}



static inline bool is_local_ether_addr(const uint8_t *addr){
    return 0x02 & addr[0];
}


static inline bool is_broadcast_ether_addr(const uint8_t *addr)
{
	return (*(const uint16_t *)(addr + 0) &
		*(const uint16_t *)(addr + 2) &
		*(const uint16_t *)(addr + 4)) == 0xffff;
}

static inline bool is_unicast_ether_addr(const uint8_t *addr)
{
	return !is_multicast_ether_addr(addr);
}


static inline bool is_valid_ether_addr(const uint8_t *addr)
{
	/* FF:FF:FF:FF:FF:FF is a multicast address so we don't need to
	 * explicitly check for it here. */
	return !is_multicast_ether_addr(addr) && !is_zero_ether_addr(addr);
}




#endif // __ALINIX_KERNEL_ETHER_DEVICE_H