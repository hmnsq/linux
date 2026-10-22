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
#अन्तर्भाव <linux/sched/clock.h>
#अन्तर्भाव <linux/sched/cputime.h>
#अन्तर्भाव <linux/sched/hotplug.h>
#अन्तर्भाव <linux/sched/isolation.h>
#अन्तर्भाव <linux/sched/posix-timers.h>
#अन्तर्भाव <linux/sched/rt.h>

#अन्तर्भाव <linux/cpuidle.h>
#अन्तर्भाव <linux/jiffies.h>
#अन्तर्भाव <linux/kobject.h>
#अन्तर्भाव <linux/livepatch.h>
#अन्तर्भाव <linux/pm.h>
#अन्तर्भाव <linux/psi.h>
#अन्तर्भाव <linux/rhashtable.h>
#अन्तर्भाव <linux/seq_buf.h>
#अन्तर्भाव <linux/seqlock_api.h>
#अन्तर्भाव <linux/slab.h>
#अन्तर्भाव <linux/suspend.h>
#अन्तर्भाव <linux/tsacct_kern.h>
#अन्तर्भाव <linux/vtime.h>
#अन्तर्भाव <linux/sysrq.h>
#अन्तर्भाव <linux/percpu-rwsem.h>

#अन्तर्भाव <uapi/linux/sched/types.h>

#अन्तर्भाव "sched.h"
#अन्तर्भाव "smp.h"

#अन्तर्भाव "autogroup.h"
#अन्तर्भाव "stats.h"
#अन्तर्भाव "pelt.h"

/* Source code modules: */

#अन्तर्भाव "idle.c"

#अन्तर्भाव "rt.c"
#अन्तर्भाव "cpudeadline.c"

#अन्तर्भाव "pelt.c"

#अन्तर्भाव "cputime.c"
#अन्तर्भाव "deadline.c"

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

#अन्तर्भाव "syscalls.c"
