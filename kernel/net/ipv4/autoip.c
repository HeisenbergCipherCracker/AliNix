/**
 * @author Ali Mirmohammad
 * @file autoip.c
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
 * @abstract:
 *  - Implements auto ip for the kernel drivers.
 * @todo:
 *  - Implement for C++.
*/


#include <alinix/types.h>
#include <alinix/compiler.h>
#include <net/ip.h>
#include <net/udp.h>
#include <net/netif.h>
#include <net/auto_ip.h>
#include <net/opt.h>
#include <net/arp.h>
#include <net/ip_addr.h>
#include <net/etharp.h>
#include <alinix/memory.h>
#include <net/err.h>
#include <net/def.h>
#include <net/inet.h>
#include <alinix/stdarg.h>
#include <alinix/module.h>


MODULE_AUTHOR("Ali Mirmohammad")
MODULE_DESCRIPTION("Auto IP for AliNix")
MODULE_LICENSE("AGPLv3")
MODULE_VERSION("1.0")



/* 169.254.0.0 */
#define AUTOIP_NET         0xA9FE0000
/* 169.254.1.0 */
#define AUTOIP_RANGE_START (AUTOIP_NET | 0x0100)
/* 169.254.254.255 */
#define AUTOIP_RANGE_END   (AUTOIP_NET | 0xFEFF)


/** Pseudo random macro based on netif informations.
 * You could use "rand()" from the C Library if you define LWIP_AUTOIP_RAND in lwipopts.h */
#ifndef LWIP_AUTOIP_RAND
#define LWIP_AUTOIP_RAND(netif) ( (((uint32_t)((netif->hwaddr[5]) & 0xff) << 24) | \
                                   ((uint32_t)((netif->hwaddr[3]) & 0xff) << 16) | \
                                   ((uint32_t)((netif->hwaddr[2]) & 0xff) << 8) | \
                                   ((uint32_t)((netif->hwaddr[4]) & 0xff))) + \
                                   (netif->autoip?netif->autoip->tried_llipaddr:0))
#endif /* LWIP_AUTOIP_RAND */



#ifndef LWIP_AUTOIP_CREATE_SEED_ADDR
#define LWIP_AUTOIP_CREATE_SEED_ADDR(netif) \
  htonl(AUTOIP_RANGE_START + ((uint32_t)(((uint8_t)(netif->hwaddr[4])) | \
                 ((uint32_t)((uint8_t)(netif->hwaddr[5]))) << 8)))
#endif /* LWIP_AUTOIP_CREATE_SEED_ADDR */




/* static functions */
static void autoip_handle_arp_conflict(struct netif *netif);

/* creates a pseudo random LL IP-Address for a network interface */
static void autoip_create_addr(struct netif *netif, ip_addr_t *ipaddr);

/* sends an ARP probe */
static err_t autoip_arp_probe(struct netif *netif);

/* sends an ARP announce */
static err_t autoip_arp_announce(struct netif *netif);

/* configure interface for use with current LL IP-Address */
static err_t autoip_bind(struct netif *netif);

/* start sending probes for llipaddr */
static void autoip_start_probing(struct netif *netif);

/**
 * Announce the AutoIP address using ARP.
 *
 * @param netif Pointer to the network interface.
 * @return The error code indicating the result of the operation.
 *
 * @note This function announces the AutoIP address using ARP.
 *       It takes a pointer to the `netif` structure as a parameter, which represents the network interface.
 *       The function calls the `etharp_raw` function to send an ARP request to announce the AutoIP address.
 *       It passes the necessary parameters to the `etharp_raw` function, including the network interface,
 *       the hardware address of the network interface, the broadcast address, the hardware address of the network interface,
 *       the AutoIP local link IP address, the zero address, and the AutoIP local link IP address.
 *       The function returns the error code indicating the result of the operation.
 *
 * @see etharp_raw()
 * @see netif
 * @see AutoIP
 */
static err_t
autoip_arp_announce(struct netif *netif)
{
  return etharp_raw(netif, (struct eth_addr *)netif->hwaddr, &ethbroadcast,
    (struct eth_addr *)netif->hwaddr, &netif->autoip->llipaddr, &ethzero,
    &netif->autoip->llipaddr, ARP_REQUEST);
}



/**
 * @brief Initiates the probing phase for AutoIP address assignment.
 * 
 * This function changes the state of the AutoIP instance associated with the given
 * network interface to `PROBING` and sets the parameters required to begin the probing
 * process. The probing phase is responsible for ensuring that the chosen link-local IP
 * address is not already in use by another device on the network.
 * 
 * @param netif A pointer to the network interface structure on which the AutoIP process is being performed.
 * 
 * @return void This function does not return a value.
 */
static void
autoip_start_probing(struct netif *netif)
{
  struct autoip *autoip = netif->autoip;

  autoip->state = AUTOIP_STATE_PROBING;
  autoip->sent_num = 0;
  LWIP_DEBUGF(AUTOIP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE,
     ("autoip_start_probing(): changing state to PROBING: %"U16_F".%"U16_F".%"U16_F".%"U16_F"\n",
      ip4_addr1_16(&netif->autoip->llipaddr), ip4_addr2_16(&netif->autoip->llipaddr),
      ip4_addr3_16(&netif->autoip->llipaddr), ip4_addr4_16(&netif->autoip->llipaddr)));

  /* time to wait to first probe, this is randomly
   * choosen out of 0 to PROBE_WAIT seconds.
   * compliant to RFC 3927 Section 2.2.1
   */
  autoip->ttw = (uint16_t)(LWIP_AUTOIP_RAND(netif) % (PROBE_WAIT * AUTOIP_TICKS_PER_SECOND));

  /*
   * if we tried more then MAX_CONFLICTS we must limit our rate for
   * accquiring and probing address
   * compliant to RFC 3927 Section 2.2.1
   */
  if (autoip->tried_llipaddr > MAX_CONFLICTS) {
    autoip->ttw = RATE_LIMIT_INTERVAL * AUTOIP_TICKS_PER_SECOND;
  }
}

/**
 * Start probing for an AutoIP address.
 *
 * @param netif Pointer to the network interface.
 *
 * @note This function starts probing for an AutoIP address.
 *       It takes a pointer to the `netif` structure as a parameter, which represents the network interface.
 *       The function initializes the `autoip` structure of the `netif` with the appropriate values.
 *       It sets the `state` of the `autoip` structure to `AUTOIP_STATE_PROBING` to indicate that probing is in progress.
 *       It sets the `sent_num` member of the `autoip` structure to 0.
 *       The function then logs a debug message indicating the change in state and the AutoIP address being probed.
 *       The function generates a random time-to-wait value (`ttw`) for the first probe, which is chosen randomly from 0 to `PROBE_WAIT` seconds.
 *       The function also checks if the number of conflicts (`tried_llipaddr`) exceeds the maximum allowed conflicts (`MAX_CONFLICTS`).
 *       If it does, the `ttw` value is set to `RATE_LIMIT_INTERVAL` times `AUTOIP_TICKS_PER_SECOND` to limit the rate of acquiring and probing the address.
 *       The function does not return any value.
 *
 * @see autoip_state_t
 * @see AUTOIP_STATE_PROBING
* */
err_t 
__autoip_start(struct netif *netif){
  struct autoip *autoip = netif->autoip;
  err_t result = ERR_OK;

  if (netif_is_up(netif)) {
    netif_set_down(netif);
  }

  /* Set IP-Address, Netmask and Gateway to 0 to make sure that
   * ARP Packets are formed correctly
   */
  ip_addr_set_zero(&netif->ip_addr);
  ip_addr_set_zero(&netif->netmask);
  ip_addr_set_zero(&netif->gw);

  LWIP_DEBUGF(AUTOIP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE,
    ("autoip_start(netif=%p) %c%c%"U16_F"\n", (void*)netif, netif->name[0],
    netif->name[1], (uint16_t)netif->num));
  if (autoip == NULL) {
    /* no AutoIP client attached yet? */
    LWIP_DEBUGF(AUTOIP_DEBUG | LWIP_DBG_TRACE,
      ("autoip_start(): starting new AUTOIP client\n"));
    autoip = (struct autoip *)mem_malloc(sizeof(struct autoip));
    if (autoip == NULL) {
      LWIP_DEBUGF(AUTOIP_DEBUG | LWIP_DBG_TRACE,
        ("autoip_start(): could not allocate autoip\n"));
      return ERR_MEM;
    }
    memset(autoip, 0, sizeof(struct autoip));
    /* store this AutoIP client in the netif */
    netif->autoip = autoip;
    LWIP_DEBUGF(AUTOIP_DEBUG | LWIP_DBG_TRACE, ("autoip_start(): allocated autoip"));
  } else {
    autoip->state = AUTOIP_STATE_OFF;
    autoip->ttw = 0;
    autoip->sent_num = 0;
    ip_addr_set_zero(&autoip->llipaddr);
    autoip->lastconflict = 0;
  }

  autoip_create_addr(netif, &(autoip->llipaddr));
  autoip_start_probing(netif);

  return result;
}


/**
 * Set the AutoIP structure for a network interface.
 *
 * @param netif Pointer to the network interface.
 * @param autoip Pointer to the AutoIP structure.
 *
 * @note This function sets the AutoIP structure for a network interface.
 *       It takes a pointer to the `netif` structure and a pointer to the `autoip` structure as parameters.
 *       The function first clears the `autoip` structure by setting all its members to 0 using the `memset` function.
 *       It then sets the `netif->autoip` member to the `autoip` pointer.
 *       The function does not return any value.
 *
 * @see netif
 * @see autoip
 */
VOID
autoip_set_struct(struct netif *netif, struct autoip *autoip)
{
//   LWIP_ASSERT("netif != NULL", netif != NULL);
//   LWIP_ASSERT("autoip != NULL", autoip != NULL);
//   LWIP_ASSERT("netif already has a struct autoip set", netif->autoip == NULL);

  /* clear data structure */
  memset(autoip, 0, sizeof(struct autoip));
  /* autoip->state = AUTOIP_STATE_OFF; */
  netif->autoip = autoip;
}

/**
 * Restart the AutoIP process.
 *
 * @param netif Pointer to the network interface.
 *
 * @note This function restarts the AutoIP process.
 *       It takes a pointer to the `netif` structure as a parameter, which represents the network interface.
 *       The function increments the `tried_llipaddr` member of the `autoip` structure of the `netif` by 1.
 *       It then calls the `__autoip_start` function to start the AutoIP process.
 *       The function does not return any value.
 *
 * @see netif
 * @see autoip
 * @see __autoip_start
 */
PRIVATE VOID
autoip_restart(struct netif *netif)
{
  netif->autoip->tried_llipaddr++;
  __autoip_start(netif);
}


/**
 * Handle an ARP conflict in the AutoIP process.
 *
 * @param netif Pointer to the network interface.
 *
 * @note This function handles an ARP conflict in the AutoIP process.
 *       It takes a pointer to the `netif` structure as a parameter, which represents the network interface.
 *       The function first determines whether it is defending or retreating based on the value of the `defend` variable.
 *       If it is defending, it checks if there was a conflicting ARP in the last `DEFEND_INTERVAL` seconds.
 *       If there was, it logs a debug message indicating that it is defending but in the `DEFEND_INTERVAL`, and then calls the `autoip_restart` function to restart the AutoIP process.
 *       If there was no conflicting ARP, it logs a debug message indicating that it is defending and sends an ARP announce using the `autoip_arp_announce` function.
 *       It then updates the `lastconflict` member of the `autoip` structure to `DEFEND_INTERVAL` times `AUTOIP_TICKS_PER_SECOND`.
 *       If it is not defending, it logs a debug message indicating that it does not defend and calls the `autoip_restart` function to restart the AutoIP process.
 *       The function does not return any value.
 *
 * @see netif
 * @see autoip
 * @see autoip_restart
 * @see autoip_arp_announce
 */
PRIVATE VOID
autoip_handle_arp_conflict(struct netif *netif)
{
  /* Somehow detect if we are defending or retreating */
  unsigned char defend = 1; /* tbd */

  if (defend) {
    if (netif->autoip->lastconflict > 0) {
      /* retreat, there was a conflicting ARP in the last
       * DEFEND_INTERVAL seconds
       */
      LWIP_DEBUGF(AUTOIP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE,
        ("autoip_handle_arp_conflict(): we are defending, but in DEFEND_INTERVAL, retreating\n"));

      /* TODO: close all TCP sessions */
      autoip_restart(netif);
    } else {
      LWIP_DEBUGF(AUTOIP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE,
        ("autoip_handle_arp_conflict(): we are defend, send ARP Announce\n"));
      autoip_arp_announce(netif);
      netif->autoip->lastconflict = DEFEND_INTERVAL * AUTOIP_TICKS_PER_SECOND;
    }
  } else {
    LWIP_DEBUGF(AUTOIP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE,
      ("autoip_handle_arp_conflict(): we do not defend, retreating\n"));
    /* TODO: close all TCP sessions */
    autoip_restart(netif);
  }
}

/**
 * Create an IP address for AutoIP.
 *
 * @param netif Pointer to the network interface.
 * @param ipaddr Pointer to the IP address structure to be filled.
 *
 * @note This function creates an IP address for AutoIP within the range 169.254.1.0 to 169.254.254.255,
 *       compliant to RFC 3927 Section 2.1. It has 254 * 256 possibilities.
 *       The function takes a pointer to the `netif` structure and a pointer to the `ipaddr` structure as parameters.
 *       It first creates an IP address by adding the `tried_llipaddr` member of the `autoip` structure of the `netif` to the seed address obtained from `LWIP_AUTOIP_CREATE_SEED_ADDR(netif)`.
 *       It then applies the necessary mask and shifts to ensure the IP address is within the specified range.
 *       If the IP address is less than `AUTOIP_RANGE_START`, it adds the difference between `AUTOIP_RANGE_END` and `AUTOIP_RANGE_START` plus 1 to the IP address.
 *       If the IP address is greater than `AUTOIP_RANGE_END`, it subtracts the difference between `AUTOIP_RANGE_END` and `AUTOIP_RANGE_START` plus 1 from the IP address.
 *       Finally, it sets the `ipaddr` structure with the created IP address and logs a debug message with the `tried_llipaddr` and the created IP address.
 *       The function does not return any value.
 *
 * @see netif
 * @see ipaddr
 */
static void
autoip_create_addr(struct netif *netif, ip_addr_t *ipaddr)
{
  /* Here we create an IP-Address out of range 169.254.1.0 to 169.254.254.255
   * compliant to RFC 3927 Section 2.1
   * We have 254 * 256 possibilities */

  uint32_t addr = ntohl(LWIP_AUTOIP_CREATE_SEED_ADDR(netif));
  addr += netif->autoip->tried_llipaddr;
  addr = AUTOIP_NET | (addr & 0xffff);
  /* Now, 169.254.0.0 <= addr <= 169.254.255.255 */ 

  if (addr < AUTOIP_RANGE_START) {
    addr += AUTOIP_RANGE_END - AUTOIP_RANGE_START + 1;
  }
  if (addr > AUTOIP_RANGE_END) {
    addr -= AUTOIP_RANGE_END - AUTOIP_RANGE_START + 1;
  }
  LWIP_ASSERT("AUTOIP address not in range", (addr >= AUTOIP_RANGE_START) &&
    (addr <= AUTOIP_RANGE_END));
  ip4_addr_set_u32(ipaddr, htonl(addr));
  
  LWIP_DEBUGF(AUTOIP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE,
    ("autoip_create_addr(): tried_llipaddr=%"U16_F", %"U16_F".%"U16_F".%"U16_F".%"U16_F"\n",
    (uint16_t)(netif->autoip->tried_llipaddr), ip4_addr1_16(ipaddr), ip4_addr2_16(ipaddr),
    ip4_addr3_16(ipaddr), ip4_addr4_16(ipaddr)));
}

/**
 * Send an ARP probe for AutoIP.
 *
 * @param netif Pointer to the network interface.
 *
 * @return The result of the ARP probe.
 *
 * @note This function sends an ARP probe for AutoIP.
 *       It takes a pointer to the `netif` structure as a parameter, which represents the network interface.
 *       The function calls the `etharp_raw` function with the following parameters:
 *       - `netif`: The network interface.
 *       - `netif->hwaddr`: The hardware address of the network interface.
 *       - `ethbroadcast`: The broadcast address.
 *       - `netif->hwaddr`: The hardware address of the network interface.
 *       - `IP_ADDR_ANY`: The IP address set to `0.0.0.0`.
 *       - `ethzero`: The zero MAC address.
 *       - `&netif->autoip->llipaddr`: The AutoIP link-local IP address.
 *       - `ARP_REQUEST`: The ARP request type.
 *       The function returns the result of the ARP probe.
 *
 * @see netif
 * @see etharp_raw
 * @see ethbroadcast
 * @see ethzero
 * @see IP_ADDR_ANY
 * @see ARP_REQUEST
 */
static err_t
autoip_arp_probe(struct netif *netif)
{
  return etharp_raw(netif, (struct eth_addr *)netif->hwaddr, &ethbroadcast,
    (struct eth_addr *)netif->hwaddr, IP_ADDR_ANY, &ethzero,
    &netif->autoip->llipaddr, ARP_REQUEST);
}