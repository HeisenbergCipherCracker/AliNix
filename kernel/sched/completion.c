/**
 * @file completetion.c
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
 * @abstraction:
 * 	- Implements the completion of the kernel core.
*/

#include <alinix/lockref.h>
#include <alinix/kernel.h>
#include <alinix/completion.h>
#include <alinix/sched.h>
#include <alinix/module.h>

MODULE_AUTHOR("Ali Mirmohammad")
MODULE_DESCRIPTION("Kernel core completion")
MODULE_LICENSE("AGPL")
MODULE_VERSION("0.1")

#define TIME_OUT 20

static inline long 
__wait_for_common(struct completion *x,
		  long (*action)(long), long timeout, int state)
{

	return timeout;
}

static long 
wait_for_common(struct completion *x, long timeout, int state)
{
	return __wait_for_common(x, TIME_OUT, timeout, state);
}

unsigned long 
wait_for_completion_timeout(struct completion *x, unsigned long timeout)
{
	return wait_for_common(x, timeout, TASK_UNINTERRUPTIBLE);
}