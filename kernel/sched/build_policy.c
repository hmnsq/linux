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
#ضمن <linux/sched/clock.h>
#ضمن <linux/sched/cputime.h>
#ضمن <linux/sched/hotplug.h>
#ضمن <linux/sched/isolation.h>
#ضمن <linux/sched/posix-timers.h>
#ضمن <linux/sched/rt.h>

#ضمن <linux/cpuidle.h>
#ضمن <linux/jiffies.h>
#ضمن <linux/kobject.h>
#ضمن <linux/livepatch.h>
#ضمن <linux/pm.h>
#ضمن <linux/psi.h>
#ضمن <linux/rhashtable.h>
#ضمن <linux/seq_buf.h>
#ضمن <linux/seqlock_api.h>
#ضمن <linux/slab.h>
#ضمن <linux/suspend.h>
#ضمن <linux/tsacct_kern.h>
#ضمن <linux/vtime.h>
#ضمن <linux/sysrq.h>
#ضمن <linux/percpu-rwsem.h>

#ضمن <uapi/linux/sched/types.h>

#ضمن "sched.h"
#ضمن "smp.h"

#ضمن "autogroup.h"
#ضمن "stats.h"
#ضمن "pelt.h"

/* Source code modules: */

#ضمن "idle.c"

#ضمن "rt.c"
#ضمن "cpudeadline.c"

#ضمن "pelt.c"

#ضمن "cputime.c"
#ضمن "deadline.c"

#اگر_تعریف_شده CONFIG_SCHED_CLASS_EXT
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
#پایان_اگر

#ضمن "syscalls.c"
