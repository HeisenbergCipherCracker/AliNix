#ifndef __ALINIX_KERNEL_SK_BUFF_H
#define __ALINIX_KERNEL_SK_BUFF_H

#include <alinix/types.h>
#include <alinix/list.h>
#include <alinix/rbtree_types.h>
#include <alinix/llist.h>
#include <alinix/ktime.h>

/**
 * @brief https://github.com/torvalds/linux/blob/master/include/linux/skbuff.h#L855
*/

struct sk_buff{
    union {
        struct {
            struct sk_buff * next;
            struct sk_buff * prev;
        };
        struct rb_node		rbnode; /* used in netem, ip4 defrag, and tcp stack */
		struct list_head	list;
		struct llist_node	ll_node;
    };

    union {
        ktime_t tstamp;
        u64		skb_mstamp_ns; /* earliest departure time */
    };
    char			cb[48] __attribute__((__aligned__(8)));

    union {
        struct {
            unsigned long _skb_refdst;
            sk_buff *skb;
        };
        struct list_head	tcp_tsorted_anchor;
    };
};

#endif // __ALINIX_KERNEL_SK_BUFF_H