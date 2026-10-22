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
#અન્તર્ભાવ <linux/sched/clock.h>
#અન્તર્ભાવ <linux/sched/cputime.h>
#અન્તર્ભાવ <linux/sched/hotplug.h>
#અન્તર્ભાવ <linux/sched/isolation.h>
#અન્તર્ભાવ <linux/sched/posix-timers.h>
#અન્તર્ભાવ <linux/sched/rt.h>

#અન્તર્ભાવ <linux/cpuidle.h>
#અન્તર્ભાવ <linux/jiffies.h>
#અન્તર્ભાવ <linux/kobject.h>
#અન્તર્ભાવ <linux/livepatch.h>
#અન્તર્ભાવ <linux/pm.h>
#અન્તર્ભાવ <linux/psi.h>
#અન્તર્ભાવ <linux/rhashtable.h>
#અન્તર્ભાવ <linux/seq_buf.h>
#અન્તર્ભાવ <linux/seqlock_api.h>
#અન્તર્ભાવ <linux/slab.h>
#અન્તર્ભાવ <linux/suspend.h>
#અન્તર્ભાવ <linux/tsacct_kern.h>
#અન્તર્ભાવ <linux/vtime.h>
#અન્તર્ભાવ <linux/sysrq.h>
#અન્તર્ભાવ <linux/percpu-rwsem.h>

#અન્તર્ભાવ <uapi/linux/sched/types.h>

#અન્તર્ભાવ "sched.h"
#અન્તર્ભાવ "smp.h"

#અન્તર્ભાવ "autogroup.h"
#અન્તર્ભાવ "stats.h"
#અન્તર્ભાવ "pelt.h"

/* Source code modules: */

#અન્તર્ભાવ "idle.c"

#અન્તર્ભાવ "rt.c"
#અન્તર્ભાવ "cpudeadline.c"

#અન્તર્ભાવ "pelt.c"

#અન્તર્ભાવ "cputime.c"
#અન્તર્ભાવ "deadline.c"

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

#અન્તર્ભાવ "syscalls.c"
