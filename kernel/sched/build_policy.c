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
#അന്തര്ഭാവ <linux/sched/clock.h>
#അന്തര്ഭാവ <linux/sched/cputime.h>
#അന്തര്ഭാവ <linux/sched/hotplug.h>
#അന്തര്ഭാവ <linux/sched/isolation.h>
#അന്തര്ഭാവ <linux/sched/posix-timers.h>
#അന്തര്ഭാവ <linux/sched/rt.h>

#അന്തര്ഭാവ <linux/cpuidle.h>
#അന്തര്ഭാവ <linux/jiffies.h>
#അന്തര്ഭാവ <linux/kobject.h>
#അന്തര്ഭാവ <linux/livepatch.h>
#അന്തര്ഭാവ <linux/pm.h>
#അന്തര്ഭാവ <linux/psi.h>
#അന്തര്ഭാവ <linux/rhashtable.h>
#അന്തര്ഭാവ <linux/seq_buf.h>
#അന്തര്ഭാവ <linux/seqlock_api.h>
#അന്തര്ഭാവ <linux/slab.h>
#അന്തര്ഭാവ <linux/suspend.h>
#അന്തര്ഭാവ <linux/tsacct_kern.h>
#അന്തര്ഭാവ <linux/vtime.h>
#അന്തര്ഭാവ <linux/sysrq.h>
#അന്തര്ഭാവ <linux/percpu-rwsem.h>

#അന്തര്ഭാവ <uapi/linux/sched/types.h>

#അന്തര്ഭാവ "sched.h"
#അന്തര്ഭാവ "smp.h"

#അന്തര്ഭാവ "autogroup.h"
#അന്തര്ഭാവ "stats.h"
#അന്തര്ഭാവ "pelt.h"

/* Source code modules: */

#അന്തര്ഭാവ "idle.c"

#അന്തര്ഭാവ "rt.c"
#അന്തര്ഭാവ "cpudeadline.c"

#അന്തര്ഭാവ "pelt.c"

#അന്തര്ഭാവ "cputime.c"
#അന്തര്ഭാവ "deadline.c"

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

#അന്തര്ഭാവ "syscalls.c"
