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
#ଅନ୍ତର୍ଭାଵ <linux/sched/clock.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/cputime.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/hotplug.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/isolation.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/posix-timers.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/rt.h>

#ଅନ୍ତର୍ଭାଵ <linux/cpuidle.h>
#ଅନ୍ତର୍ଭାଵ <linux/jiffies.h>
#ଅନ୍ତର୍ଭାଵ <linux/kobject.h>
#ଅନ୍ତର୍ଭାଵ <linux/livepatch.h>
#ଅନ୍ତର୍ଭାଵ <linux/pm.h>
#ଅନ୍ତର୍ଭାଵ <linux/psi.h>
#ଅନ୍ତର୍ଭାଵ <linux/rhashtable.h>
#ଅନ୍ତର୍ଭାଵ <linux/seq_buf.h>
#ଅନ୍ତର୍ଭାଵ <linux/seqlock_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/slab.h>
#ଅନ୍ତର୍ଭାଵ <linux/suspend.h>
#ଅନ୍ତର୍ଭାଵ <linux/tsacct_kern.h>
#ଅନ୍ତର୍ଭାଵ <linux/vtime.h>
#ଅନ୍ତର୍ଭାଵ <linux/sysrq.h>
#ଅନ୍ତର୍ଭାଵ <linux/percpu-rwsem.h>

#ଅନ୍ତର୍ଭାଵ <uapi/linux/sched/types.h>

#ଅନ୍ତର୍ଭାଵ "sched.h"
#ଅନ୍ତର୍ଭାଵ "smp.h"

#ଅନ୍ତର୍ଭାଵ "autogroup.h"
#ଅନ୍ତର୍ଭାଵ "stats.h"
#ଅନ୍ତର୍ଭାଵ "pelt.h"

/* Source code modules: */

#ଅନ୍ତର୍ଭାଵ "idle.c"

#ଅନ୍ତର୍ଭାଵ "rt.c"
#ଅନ୍ତର୍ଭାଵ "cpudeadline.c"

#ଅନ୍ତର୍ଭାଵ "pelt.c"

#ଅନ୍ତର୍ଭାଵ "cputime.c"
#ଅନ୍ତର୍ଭାଵ "deadline.c"

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

#ଅନ୍ତର୍ଭାଵ "syscalls.c"
