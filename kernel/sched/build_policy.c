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
#ಅನ್ತರ್ಭಾವ <linux/sched/clock.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/cputime.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/hotplug.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/isolation.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/posix-timers.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/rt.h>

#ಅನ್ತರ್ಭಾವ <linux/cpuidle.h>
#ಅನ್ತರ್ಭಾವ <linux/jiffies.h>
#ಅನ್ತರ್ಭಾವ <linux/kobject.h>
#ಅನ್ತರ್ಭಾವ <linux/livepatch.h>
#ಅನ್ತರ್ಭಾವ <linux/pm.h>
#ಅನ್ತರ್ಭಾವ <linux/psi.h>
#ಅನ್ತರ್ಭಾವ <linux/rhashtable.h>
#ಅನ್ತರ್ಭಾವ <linux/seq_buf.h>
#ಅನ್ತರ್ಭಾವ <linux/seqlock_api.h>
#ಅನ್ತರ್ಭಾವ <linux/slab.h>
#ಅನ್ತರ್ಭಾವ <linux/suspend.h>
#ಅನ್ತರ್ಭಾವ <linux/tsacct_kern.h>
#ಅನ್ತರ್ಭಾವ <linux/vtime.h>
#ಅನ್ತರ್ಭಾವ <linux/sysrq.h>
#ಅನ್ತರ್ಭಾವ <linux/percpu-rwsem.h>

#ಅನ್ತರ್ಭಾವ <uapi/linux/sched/types.h>

#ಅನ್ತರ್ಭಾವ "sched.h"
#ಅನ್ತರ್ಭಾವ "smp.h"

#ಅನ್ತರ್ಭಾವ "autogroup.h"
#ಅನ್ತರ್ಭಾವ "stats.h"
#ಅನ್ತರ್ಭಾವ "pelt.h"

/* Source code modules: */

#ಅನ್ತರ್ಭಾವ "idle.c"

#ಅನ್ತರ್ಭಾವ "rt.c"
#ಅನ್ತರ್ಭಾವ "cpudeadline.c"

#ಅನ್ತರ್ಭಾವ "pelt.c"

#ಅನ್ತರ್ಭಾವ "cputime.c"
#ಅನ್ತರ್ಭಾವ "deadline.c"

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

#ಅನ್ತರ್ಭಾವ "syscalls.c"
