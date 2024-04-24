#ifndef __ALINIX_KERNEL_NET_TCP_IMPL_H
#define __ALINIX_KERNEL_NET_TCP_IMPL_H

#include <alinix/types.h>

extern uint8_t tcp_active_pcbs_changed;
extern struct tcp_pcb *tcp_active_pcbs;  /* List of all TCP PCBs that are in a
              state in which they accept or send
              data. */

#endif