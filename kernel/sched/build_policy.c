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
#అన్తర్భావ <linux/sched/clock.h>
#అన్తర్భావ <linux/sched/cputime.h>
#అన్తర్భావ <linux/sched/hotplug.h>
#అన్తర్భావ <linux/sched/isolation.h>
#అన్తర్భావ <linux/sched/posix-timers.h>
#అన్తర్భావ <linux/sched/rt.h>

#అన్తర్భావ <linux/cpuidle.h>
#అన్తర్భావ <linux/jiffies.h>
#అన్తర్భావ <linux/kobject.h>
#అన్తర్భావ <linux/livepatch.h>
#అన్తర్భావ <linux/pm.h>
#అన్తర్భావ <linux/psi.h>
#అన్తర్భావ <linux/rhashtable.h>
#అన్తర్భావ <linux/seq_buf.h>
#అన్తర్భావ <linux/seqlock_api.h>
#అన్తర్భావ <linux/slab.h>
#అన్తర్భావ <linux/suspend.h>
#అన్తర్భావ <linux/tsacct_kern.h>
#అన్తర్భావ <linux/vtime.h>
#అన్తర్భావ <linux/sysrq.h>
#అన్తర్భావ <linux/percpu-rwsem.h>

#అన్తర్భావ <uapi/linux/sched/types.h>

#అన్తర్భావ "sched.h"
#అన్తర్భావ "smp.h"

#అన్తర్భావ "autogroup.h"
#అన్తర్భావ "stats.h"
#అన్తర్భావ "pelt.h"

/* Source code modules: */

#అన్తర్భావ "idle.c"

#అన్తర్భావ "rt.c"
#అన్తర్భావ "cpudeadline.c"

#అన్తర్భావ "pelt.c"

#అన్తర్భావ "cputime.c"
#అన్తర్భావ "deadline.c"

#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
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
#అయితే_ముగింపు

#అన్తర్భావ "syscalls.c"
