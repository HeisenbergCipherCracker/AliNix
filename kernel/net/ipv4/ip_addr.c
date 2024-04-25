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
#include <net/ip_addr.h>


const ip_addr_t ip_addr_any = { IPADDR_ANY };
const ip_addr_t ip_addr_broadcast = { IPADDR_BROADCAST };

uint8_t
ip4_addr_isbroadcast(uint32_t addr, const struct netif *netif)
{
  ip_addr_t ipaddr;
  ip4_addr_set_u32(&ipaddr, addr);

  /* all ones (broadcast) or all zeroes (old skool broadcast) */
  if ((~addr == IPADDR_ANY) ||
      (addr == IPADDR_ANY)) {
    return 1;
  /* no broadcast support on this network interface? */
  } else if ((netif->flags & NETIF_FLAG_BROADCAST) == 0) {
    /* the given address cannot be a broadcast address
     * nor can we check against any broadcast addresses */
    return 0;
  /* address matches network interface address exactly? => no broadcast */
  } else if (addr == ip4_addr_get_u32(&netif->ip_addr)) {
    return 0;
  /*  on the same (sub) network... */
  } else if (ip_addr_netcmp(&ipaddr, &(netif->ip_addr), &(netif->netmask))
         /* ...and host identifier bits are all ones? =>... */
          && ((addr & ~ip4_addr_get_u32(&netif->netmask)) ==
           (IPADDR_BROADCAST & ~ip4_addr_get_u32(&netif->netmask)))) {
    /* => network broadcast address */
    return 1;
  } else {
    return 0;
  }
}