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
#ifndef __ALINIX_KERNEL_IP_ADDR_NET_H
#define __ALINIX_KERNEL_IP_ADDR_NET_H


#include <alinix/types.h>
#include <alinix/ip.h>
#include <alinix/compiler.h>

struct ip_addr {
  uint32_t addr;
};

PACK_STRUCT_BEGIN
struct ip_addr_packed {
  PACK_STRUCT_FIELD(uint32_t addr);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END


// This should be defined on the top
typedef struct ip_addr ip_addr_t;
typedef struct ip_addr_packed ip_addr_p_t;



/** 255.255.255.255 */
#define IPADDR_NONE         ((uint32_t)0xffffffffUL)
/** 127.0.0.1 */
#define IPADDR_LOOPBACK     ((uint32_t)0x7f000001UL)
/** 0.0.0.0 */
#define IPADDR_ANY          ((uint32_t)0x00000000UL)
/** 255.255.255.255 */


/* Get one byte from the 4-byte address */
#define ip4_addr1(ipaddr) (((uint8_t*)(ipaddr))[0])
#define ip4_addr2(ipaddr) (((uint8_t*)(ipaddr))[1])
#define ip4_addr3(ipaddr) (((uint8_t*)(ipaddr))[2])
#define ip4_addr4(ipaddr) (((uint8_t*)(ipaddr))[3])
#define IPADDR_BROADCAST    ((uint32_t)0xffffffffUL)
#define ip4_addr1_16(ipaddr) ((uint16_t)ip4_addr1(ipaddr))
#define ip4_addr2_16(ipaddr) ((uint16_t)ip4_addr2(ipaddr))
#define ip4_addr3_16(ipaddr) ((uint16_t)ip4_addr3(ipaddr))
#define ip4_addr4_16(ipaddr) ((uint16_t)ip4_addr4(ipaddr))


#define ip_addr_copy(dest, src) ((dest).addr = (src).addr)

#define IP_PCB \
  /* ip addresses in network byte order */ \
  ip_addr_t local_ip; \
  ip_addr_t remote_ip; \
   /* Socket options */  \
  uint8_t so_options;      \
   /* Type Of Service */ \
  uint8_t tos;              \
  /* Time To Live */     \
  uint8_t ttl               \
  /* link layer address resolution hint */ \
  IP_PCB_ADDRHINT

struct ip_pcb {
/* Common members of all PCB types */
  IP_PCB;
};









extern const ip_addr_t ip_addr_any;
extern const ip_addr_t ip_addr_broadcast
#define IP_ADDR_ANY         ((ip_addr_t *)&ip_addr_any)
#define IP_ADDR_BROADCAST   ((ip_addr_t *)&ip_addr_broadcast)
;

#define ip4_addr_set_u32(dest_ipaddr, src_u32) ((dest_ipaddr)->addr = (src_u32))


PACK_STRUCT_BEGIN
struct ip_addr2 {
  PACK_STRUCT_FIELD(uint16_t addrw[2]);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END


#define ip_addr_set_zero(ipaddr)      ((ipaddr)->addr = 0)


#ifndef IPADDR2_COPY
#define IPADDR2_COPY(dest, src) SMEMCPY(dest, src, sizeof(ip_addr_t))
#endif



#define PP_HTONS(x) ((((x) & 0xff) << 8) | (((x) & 0xff00) >> 8))
#define PP_NTOHS(x) PP_HTONS(x)
#define PP_HTONL(x) ((((x) & 0xff) << 24) | \
                     (((x) & 0xff00) << 8) | \
                     (((x) & 0xff0000UL) >> 8) | \
                     (((x) & 0xff000000UL) >> 24))
#define PP_NTOHL(x) PP_HTONL(x)

#endif /*__ALINIX_KERNEL_IP_ADDR_NET_H*/