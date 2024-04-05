#ifndef _HEISEN_KERNEL_XHCI__
#define _HEISEN_KERNEL_XHCI__

#if defined(__cplusplus)
#include "types.h"
using namespace LIBHeisenKernel;

#else 
#include <stdint.h>

#endif 



#pragma pack(push,1)
typedef struct _xhci_doorbell_ {
	unsigned doorbell[256];
}xhci_doorbell_regs_t;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct _xhci_cap_regs_ {
	uint8_t cap_caplen_version;
	uint8_t reserved;
	uint16_t hci_version;
	uint32_t cap_hcsparams1;
	uint32_t cap_hcsparams2;
	uint32_t cap_hcsparams3;
	uint32_t cap_hccparams1;
	uint32_t cap_dboff;
	uint32_t cap_hccparams2;
}xhci_cap_regs_t;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct _xhci_cap_regs_ {
	uint8_t cap_caplen_version;
	uint8_t reserved;
	uint16_t hci_version;
	uint32_t cap_hcsparams1;
	uint32_t cap_hcsparams2;
	uint32_t cap_hcsparams3;
	uint32_t cap_hccparams1;
	uint32_t cap_dboff;
	uint32_t cap_hccparams2;
}xhci_cap_regs_t;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct _xhci_ex_cap_ {
	uint32_t id : 8;
	uint32_t next : 8;
	uint32_t controller_bios_semaphore : 1;
	uint32_t rsvd : 7;
	uint32_t controller_os_sem : 1;
	uint32_t rsvd2 : 7;
}xhci_ext_cap_t;
#pragma pack(pop)
#endif