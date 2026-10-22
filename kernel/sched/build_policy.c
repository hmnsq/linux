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
#समावेश <linux/sched/clock.h>
#समावेश <linux/sched/cputime.h>
#समावेश <linux/sched/hotplug.h>
#समावेश <linux/sched/isolation.h>
#समावेश <linux/sched/posix-timers.h>
#समावेश <linux/sched/rt.h>

#समावेश <linux/cpuidle.h>
#समावेश <linux/jiffies.h>
#समावेश <linux/kobject.h>
#समावेश <linux/livepatch.h>
#समावेश <linux/pm.h>
#समावेश <linux/psi.h>
#समावेश <linux/rhashtable.h>
#समावेश <linux/seq_buf.h>
#समावेश <linux/seqlock_api.h>
#समावेश <linux/slab.h>
#समावेश <linux/suspend.h>
#समावेश <linux/tsacct_kern.h>
#समावेश <linux/vtime.h>
#समावेश <linux/sysrq.h>
#समावेश <linux/percpu-rwsem.h>

#समावेश <uapi/linux/sched/types.h>

#समावेश "sched.h"
#समावेश "smp.h"

#समावेश "autogroup.h"
#समावेश "stats.h"
#समावेश "pelt.h"

/* Source code modules: */

#समावेश "idle.c"

#समावेश "rt.c"
#समावेश "cpudeadline.c"

#समावेश "pelt.c"

#समावेश "cputime.c"
#समावेश "deadline.c"

#अगर_घोषित CONFIG_SCHED_CLASS_EXT
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
#पूर्ण_अगर

#समावेश "syscalls.c"
