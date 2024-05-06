#ifndef __ALINIX_KERNEL_DRIVERS_E1000_82571_H
#define __ALINIX_KERNEL_DRIVERS_E1000_82571_H


#include <alinix/types.h>


#define ID_LED_RESERVED_F746 0xF746
#define ID_LED_DEFAULT_82573 ((ID_LED_DEF1_DEF2 << 12) | \
                              (ID_LED_OFF1_ON2  <<  8) | \
                              (ID_LED_DEF1_DEF2 <<  4) | \
                              (ID_LED_DEF1_DEF2))

#define E1000_GCR_L1_ACT_WITHOUT_L0S_RX 0x08000000
#define AN_RETRY_COUNT          5 /* Autoneg Retry Count value */

/* Intr Throttling - RW */
#define E1000_EITR_82574(_n)    (0x000E8 + (0x4 * (_n)))

#define E1000_EIAC_82574        0x000DC /* Ext. Interrupt Auto Clear - RW */
#define E1000_EIAC_MASK_82574   0x01F00000

#define E1000_NVM_INIT_CTRL2_MNGM 0x6000 /* Manageability Operation Mode mask */

#define E1000_RXCFGL    0x0B634 /* TimeSync Rx EtherType & Msg Type Reg - RW */

#define E1000_BASE1000T_STATUS 10
#define E1000_IDLE_ERROR_COUNT_MASK 0xFF
#define E1000_RECEIVE_ERROR_COUNTER 21
#define E1000_RECEIVE_ERROR_MAX 0xFFFF
bool e1000_check_phy_82574(struct e1000_hw *hw);
bool e1000e_get_laa_state_82571(struct e1000_hw *hw);
void e1000e_set_laa_state_82571(struct e1000_hw *hw, bool state);

#endif