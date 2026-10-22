// SPDX-License-Identifier: GPL-2.0-only
/*
 * These are the scheduling policy related scheduler files, built
 * in a single compilation unit for build efficiency reasons.
 *
 * ( Incidentally, the size of the compilation unit is roughly
 *   comparable to core.c and fair.c, the other two big
 *   compilation units. This helps balance build time, while
 *   coalescing source files to amortize header inclusion
 *   cost. )
 *
 * core.c and fair.c are built separately.
 */

/* Headers: */
#شامل <linux/sched/clock.h>
#شامل <linux/sched/cputime.h>
#شامل <linux/sched/hotplug.h>
#شامل <linux/sched/isolation.h>
#شامل <linux/sched/posix-timers.h>
#شامل <linux/sched/rt.h>

#شامل <linux/cpuidle.h>
#شامل <linux/jiffies.h>
#شامل <linux/kobject.h>
#شامل <linux/livepatch.h>
#شامل <linux/pm.h>
#شامل <linux/psi.h>
#شامل <linux/rhashtable.h>
#شامل <linux/seq_buf.h>
#شامل <linux/seqlock_api.h>
#شامل <linux/slab.h>
#شامل <linux/suspend.h>
#شامل <linux/tsacct_kern.h>
#شامل <linux/vtime.h>
#شامل <linux/sysrq.h>
#شامل <linux/percpu-rwsem.h>

#شامل <uapi/linux/sched/types.h>

#شامل "sched.h"
#شامل "smp.h"

#شامل "autogroup.h"
#شامل "stats.h"
#شامل "pelt.h"

/* Source code modules: */

#شامل "idle.c"

#شامل "rt.c"
#شامل "cpudeadline.c"

#شامل "pelt.c"

#شامل "cputime.c"
#شامل "deadline.c"

#ifdef CONFIG_SCHED_CLASS_EXT
# include <linux/btf_ids.h>
# include <linux/find.h>
# include <linux/genalloc.h>
# include "ext/types.h"
# include "ext/internal.h"
# include "ext/cid.h"
# include "ext/arena.h"
# include "ext/idle.h"
# include "ext/sub.h"
# include "ext/inlines.h"
# include "ext/ext.c"
# include "ext/cid.c"
# include "ext/arena.c"
# include "ext/idle.c"
# include "ext/sub.c"
#endif

#شامل "syscalls.c"
