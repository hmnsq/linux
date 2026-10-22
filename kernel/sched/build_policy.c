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
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/clock.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/cputime.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/hotplug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/isolation.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/posix-timers.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/rt.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <linux/cpuidle.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/jiffies.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/kobject.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/livepatch.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/pm.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/psi.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/rhashtable.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/seq_buf.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/seqlock_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/slab.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/suspend.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/tsacct_kern.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/vtime.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sysrq.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/percpu-rwsem.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <uapi/linux/sched/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "smp.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "autogroup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stats.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pelt.h"

/* Source code modules: */

#ਅਨ੍ਤਰ੍ਭਾਵ "idle.c"

#ਅਨ੍ਤਰ੍ਭਾਵ "rt.c"
#ਅਨ੍ਤਰ੍ਭਾਵ "cpudeadline.c"

#ਅਨ੍ਤਰ੍ਭਾਵ "pelt.c"

#ਅਨ੍ਤਰ੍ਭਾਵ "cputime.c"
#ਅਨ੍ਤਰ੍ਭਾਵ "deadline.c"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CLASS_EXT
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
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "syscalls.c"
