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
#அந்தர்பாவ <linux/sched/clock.h>
#அந்தர்பாவ <linux/sched/cputime.h>
#அந்தர்பாவ <linux/sched/hotplug.h>
#அந்தர்பாவ <linux/sched/isolation.h>
#அந்தர்பாவ <linux/sched/posix-timers.h>
#அந்தர்பாவ <linux/sched/rt.h>

#அந்தர்பாவ <linux/cpuidle.h>
#அந்தர்பாவ <linux/jiffies.h>
#அந்தர்பாவ <linux/kobject.h>
#அந்தர்பாவ <linux/livepatch.h>
#அந்தர்பாவ <linux/pm.h>
#அந்தர்பாவ <linux/psi.h>
#அந்தர்பாவ <linux/rhashtable.h>
#அந்தர்பாவ <linux/seq_buf.h>
#அந்தர்பாவ <linux/seqlock_api.h>
#அந்தர்பாவ <linux/slab.h>
#அந்தர்பாவ <linux/suspend.h>
#அந்தர்பாவ <linux/tsacct_kern.h>
#அந்தர்பாவ <linux/vtime.h>
#அந்தர்பாவ <linux/sysrq.h>
#அந்தர்பாவ <linux/percpu-rwsem.h>

#அந்தர்பாவ <uapi/linux/sched/types.h>

#அந்தர்பாவ "sched.h"
#அந்தர்பாவ "smp.h"

#அந்தர்பாவ "autogroup.h"
#அந்தர்பாவ "stats.h"
#அந்தர்பாவ "pelt.h"

/* Source code modules: */

#அந்தர்பாவ "idle.c"

#அந்தர்பாவ "rt.c"
#அந்தர்பாவ "cpudeadline.c"

#அந்தர்பாவ "pelt.c"

#அந்தர்பாவ "cputime.c"
#அந்தர்பாவ "deadline.c"

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

#அந்தர்பாவ "syscalls.c"
