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
#অন্তর্ভাব <linux/sched/clock.h>
#অন্তর্ভাব <linux/sched/cputime.h>
#অন্তর্ভাব <linux/sched/hotplug.h>
#অন্তর্ভাব <linux/sched/isolation.h>
#অন্তর্ভাব <linux/sched/posix-timers.h>
#অন্তর্ভাব <linux/sched/rt.h>

#অন্তর্ভাব <linux/cpuidle.h>
#অন্তর্ভাব <linux/jiffies.h>
#অন্তর্ভাব <linux/kobject.h>
#অন্তর্ভাব <linux/livepatch.h>
#অন্তর্ভাব <linux/pm.h>
#অন্তর্ভাব <linux/psi.h>
#অন্তর্ভাব <linux/rhashtable.h>
#অন্তর্ভাব <linux/seq_buf.h>
#অন্তর্ভাব <linux/seqlock_api.h>
#অন্তর্ভাব <linux/slab.h>
#অন্তর্ভাব <linux/suspend.h>
#অন্তর্ভাব <linux/tsacct_kern.h>
#অন্তর্ভাব <linux/vtime.h>
#অন্তর্ভাব <linux/sysrq.h>
#অন্তর্ভাব <linux/percpu-rwsem.h>

#অন্তর্ভাব <uapi/linux/sched/types.h>

#অন্তর্ভাব "sched.h"
#অন্তর্ভাব "smp.h"

#অন্তর্ভাব "autogroup.h"
#অন্তর্ভাব "stats.h"
#অন্তর্ভাব "pelt.h"

/* Source code modules: */

#অন্তর্ভাব "idle.c"

#অন্তর্ভাব "rt.c"
#অন্তর্ভাব "cpudeadline.c"

#অন্তর্ভাব "pelt.c"

#অন্তর্ভাব "cputime.c"
#অন্তর্ভাব "deadline.c"

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

#অন্তর্ভাব "syscalls.c"
