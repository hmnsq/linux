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
#অন্তর্ভাৱ <linux/sched/clock.h>
#অন্তর্ভাৱ <linux/sched/cputime.h>
#অন্তর্ভাৱ <linux/sched/hotplug.h>
#অন্তর্ভাৱ <linux/sched/isolation.h>
#অন্তর্ভাৱ <linux/sched/posix-timers.h>
#অন্তর্ভাৱ <linux/sched/rt.h>

#অন্তর্ভাৱ <linux/cpuidle.h>
#অন্তর্ভাৱ <linux/jiffies.h>
#অন্তর্ভাৱ <linux/kobject.h>
#অন্তর্ভাৱ <linux/livepatch.h>
#অন্তর্ভাৱ <linux/pm.h>
#অন্তর্ভাৱ <linux/psi.h>
#অন্তর্ভাৱ <linux/rhashtable.h>
#অন্তর্ভাৱ <linux/seq_buf.h>
#অন্তর্ভাৱ <linux/seqlock_api.h>
#অন্তর্ভাৱ <linux/slab.h>
#অন্তর্ভাৱ <linux/suspend.h>
#অন্তর্ভাৱ <linux/tsacct_kern.h>
#অন্তর্ভাৱ <linux/vtime.h>
#অন্তর্ভাৱ <linux/sysrq.h>
#অন্তর্ভাৱ <linux/percpu-rwsem.h>

#অন্তর্ভাৱ <uapi/linux/sched/types.h>

#অন্তর্ভাৱ "sched.h"
#অন্তর্ভাৱ "smp.h"

#অন্তর্ভাৱ "autogroup.h"
#অন্তর্ভাৱ "stats.h"
#অন্তর্ভাৱ "pelt.h"

/* Source code modules: */

#অন্তর্ভাৱ "idle.c"

#অন্তর্ভাৱ "rt.c"
#অন্তর্ভাৱ "cpudeadline.c"

#অন্তর্ভাৱ "pelt.c"

#অন্তর্ভাৱ "cputime.c"
#অন্তর্ভাৱ "deadline.c"

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

#অন্তর্ভাৱ "syscalls.c"
