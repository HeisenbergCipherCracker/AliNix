/**
 * @author Ali Mirmohammad
 * @file kernel.h
 * *************************************IMPORTANT ALINIX LICENSE TERM********************************************
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
#ifndef __ALINIX_KERNEL_FS_H
#define __ALINIX_KERNEL_FS_H

#include <alinix/types.h>
#include <alinix/kernel.h>
#include <alinix/spinlock_types.h>
#include <alinix/errorseq.h>
#include <alinix/mutex_types.h>
#include <alinix/list_bl.h>
#include <alinix/rwsem.h>
#include <alinix/time64.h>
#include <alinix/percpu-rwsem.h>
#include <alinix/uuid.h>



/* Possible states of 'frozen' field */
enum {
	SB_UNFROZEN = 0,		/* FS is unfrozen */
	SB_FREEZE_WRITE	= 1,		/* Writes, dir ops, ioctls frozen */
	SB_FREEZE_PAGEFAULT = 2,	/* Page faults stopped as well */
	SB_FREEZE_FS = 3,		/* For internal FS use (e.g. to stop
					 * internal threads if needed) */
	SB_FREEZE_COMPLETE = 4,		/* ->freeze_fs finished successfully */
};

#define SB_FREEZE_LEVELS (SB_FREEZE_COMPLETE - 1)


struct sb_writers {
	unsigned short			frozen;		/* Is sb frozen? */
	int				freeze_kcount;	/* How many kernel freeze requests? */
	int				freeze_ucount;	/* How many userspace freeze requests? */
	struct percpu_rw_semaphore	rw_sem[SB_FREEZE_LEVELS];
};

struct file_ra_state {
	pgoff_t start;
	unsigned int size;
	unsigned int async_size;
	unsigned int ra_pages;
	unsigned int mmap_miss;
	loff_t prev_pos;
};


struct super_block {
	struct list_head	s_list;		/* Keep this first */
	dev_t			s_dev;		/* search index; _not_ kdev_t */
	unsigned char		s_blocksize_bits;
	unsigned long		s_blocksize;
	loff_t			s_maxbytes;	/* Max file size */
	struct file_system_type	*s_type;
	const struct super_operations	*s_op;
	const struct dquot_operations	*dq_op;
	const struct quotactl_ops	*s_qcop;
	const struct export_operations *s_export_op;
	unsigned long		s_flags;
	unsigned long		s_iflags;	/* internal SB_I_* flags */
	unsigned long		s_magic;
	struct dentry		*s_root;
	struct rw_semaphore	s_umount;
	int			s_count;
	atomic_t		s_active;
#ifdef CONFIG_SECURITY
	void                    *s_security;
#endif
	const struct xattr_handler * const *s_xattr;
#ifdef CONFIG_FS_ENCRYPTION
	const struct fscrypt_operations	*s_cop;
	struct fscrypt_keyring	*s_master_keys; /* master crypto keys in use */
#endif
#ifdef CONFIG_FS_VERITY
	const struct fsverity_operations *s_vop;
#endif
#if IS_ENABLED(CONFIG_UNICODE)
	struct unicode_map *s_encoding;
	__u16 s_encoding_flags;
#endif
	struct hlist_bl_head	s_roots;	/* alternate root dentries for NFS */
	struct list_head	s_mounts;	/* list of mounts; _not_ for fs use */
	struct block_device	*s_bdev;	/* can go away once we use an accessor for @s_bdev_file */
	struct file		*s_bdev_file;
	struct backing_dev_info *s_bdi;
	struct mtd_info		*s_mtd;
	struct hlist_node	s_instances;
	unsigned int		s_quota_types;	/* Bitmask of supported quota types */
	// struct quota_info	s_dquot;	/* Diskquota specific options */

	struct sb_writers	s_writers;

	/*
	 * Keep s_fs_info, s_time_gran, s_fsnotify_mask, and
	 * s_fsnotify_marks together for cache efficiency. They are frequently
	 * accessed and rarely modified.
	 */
	void			*s_fs_info;	/* Filesystem private info */

	/* Granularity of c/m/atime in ns (cannot be worse than a second) */
	u32			s_time_gran;
	/* Time limits for c/m/atime in seconds */
	time64_t		   s_time_min;
	time64_t		   s_time_max;
#ifdef CONFIG_FSNOTIFY
	__u32			s_fsnotify_mask;
	struct fsnotify_mark_connector __rcu	*s_fsnotify_marks;
#endif

	/*
	 * q: why are s_id and s_sysfs_name not the same? both are human
	 * readable strings that identify the filesystem
	 * a: s_id is allowed to change at runtime; it's used in log messages,
	 * and we want to when a device starts out as single device (s_id is dev
	 * name) but then a device is hot added and we have to switch to
	 * identifying it by UUID
	 * but s_sysfs_name is a handle for programmatic access, and can't
	 * change at runtime
	 */
	char			s_id[32];	/* Informational name */
	uuid_t			s_uuid;		/* UUID */
	u8			s_uuid_len;	/* Default 16, possibly smaller for weird filesystems */

	/* if set, fs shows up under sysfs at /sys/fs/$FSTYP/s_sysfs_name */
	char			s_sysfs_name[UUID_STRING_LEN + 1];

	unsigned int		s_max_links;

	/*
	 * The next field is for VFS *only*. No filesystems have any business
	 * even looking at it. You had been warned.
	 */
	struct mutex s_vfs_rename_mutex;	/* Kludge */

	/*
	 * Filesystem subtype.  If non-empty the filesystem type field
	 * in /proc/mounts will be "type.subtype"
	 */
	const char *s_subtype;

	const struct dentry_operations *s_d_op; /* default d_op for dentries */

	struct shrinker *s_shrink;	/* per-sb shrinker handle */

	/* Number of inodes with nlink == 0 but still referenced */
	long long s_remove_count;

	/*
	 * Number of inode/mount/sb objects that are being watched, note that
	 * inodes objects are currently double-accounted.
	 */
	long long s_fsnotify_connectors;

	/* Read-only state of the superblock is being changed */
	int s_readonly_remount;

	/* per-sb errseq_t for reporting writeback errors via syncfs */
	errseq_t s_wb_err;

	/* AIO completions deferred from interrupt context */
	struct workqueue_struct *s_dio_done_wq;
	struct hlist_head s_pins;

	/*
	 * Owning user namespace and default context in which to
	 * interpret filesystem uids, gids, quotas, device nodes,
	 * xattrs and security labels.
	 */
	struct user_namespace *s_user_ns;

	/*
	 * The list_lru structure is essentially just a pointer to a table
	 * of per-node lru lists, each of which has its own spinlock.
	 * There is no need to put them into separate cachelines.
	 */
	struct list_lru		s_dentry_lru;
	struct list_lru		s_inode_lru;
	struct rcu_head		rcu;
	struct work_struct	destroy_work;

	struct mutex		s_sync_lock;	/* sync serialisation lock */

	/*
	 * Indicates how deep in a filesystem stack this SB is
	 */
	int s_stack_depth;

	/* s_inode_list_lock protects s_inodes */
	spinlock_t		 ____cacheline_aligned_in_smp;
	struct list_head	s_inodes;	/* all inodes */

	spinlock_t		s_inode_wblist_lock;
	struct list_head	s_inodes_wb;	/* writeback inodes */
} __randomize_layout;
struct file{
    union {
        /* fput() uses task work when closing and freeing file (default). */
		struct callback_head 	f_task_work;
		/* fput() must use workqueue (most kernel threads). */
		struct llist_node	f_llist;
		unsigned int 		f_iocb_flags;
    };
    	spinlock_t		f_lock;
	fmode_t			f_mode;
	long long		f_count;
	struct mutex		f_pos_lock;
	loff_t			f_pos;
	unsigned int		f_flags;
	struct fown_struct	f_owner;
	const struct cred	*f_cred;
	struct file_ra_state	f_ra;
	struct path		f_path;
	struct inode		*f_inode;	/* cached value */
	const struct file_operations	*f_op;

	u64			f_version;
#ifdef CONFIG_SECURITY
	void			*f_security;
#endif
	/* needed for tty driver, and maybe others */
	void			*private_data;

#ifdef CONFIG_EPOLL
	/* Used by fs/eventpoll.c to link all the hooks to this file */
	struct hlist_head	*f_ep;
#endif /* #ifdef CONFIG_EPOLL */
	struct address_space	*f_mapping;
	errseq_t		f_wb_err;
	errseq_t		f_sb_err; /* for syncfs */
};

#endif // __ALINIX_KERNEL_FS_H