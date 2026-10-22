// SPDX-License-Identifier: GPL-2.0-only
/*
 * These are various utility functions of the scheduler,
 * built in a single compilation unit for build efficiency reasons.
 *
 * ( Incidentally, the size of the compilation unit is roughly
 *   comparable to core.c, fair.c, smp.c and policy.c, the other
 *   big compilation units. This helps balance build time, while
 *   coalescing source files to amortize header inclusion
 *   cost. )
 */
#অন্তর্ভাৱ <linux/sched/clock.h>
#অন্তর্ভাৱ <linux/sched/cputime.h>
#অন্তর্ভাৱ <linux/sched/debug.h>
#অন্তর্ভাৱ <linux/sched/isolation.h>
#অন্তর্ভাৱ <linux/sched/loadavg.h>
#অন্তর্ভাৱ <linux/sched/nohz.h>
#অন্তর্ভাৱ <linux/sched/mm.h>
#অন্তর্ভাৱ <linux/sched/rseq_api.h>
#অন্তর্ভাৱ <linux/sched/task_stack.h>

#অন্তর্ভাৱ <linux/cpufreq.h>
#অন্তর্ভাৱ <linux/cpumask_api.h>
#অন্তর্ভাৱ <linux/cpuset.h>
#অন্তর্ভাৱ <linux/ctype.h>
#অন্তর্ভাৱ <linux/debugfs.h>
#অন্তর্ভাৱ <linux/energy_model.h>
#অন্তর্ভাৱ <linux/hashtable_api.h>
#অন্তর্ভাৱ <linux/irq.h>
#অন্তর্ভাৱ <linux/kobject_api.h>
#অন্তর্ভাৱ <linux/membarrier.h>
#অন্তর্ভাৱ <linux/mempolicy.h>
#অন্তর্ভাৱ <linux/nmi.h>
#অন্তর্ভাৱ <linux/nospec.h>
#অন্তর্ভাৱ <linux/proc_fs.h>
#অন্তর্ভাৱ <linux/psi.h>
#অন্তর্ভাৱ <linux/ptrace_api.h>
#অন্তর্ভাৱ <linux/sched_clock.h>
#অন্তর্ভাৱ <linux/security.h>
#অন্তর্ভাৱ <linux/spinlock_api.h>
#অন্তর্ভাৱ <linux/swait_api.h>
#অন্তর্ভাৱ <linux/timex.h>
#অন্তর্ভাৱ <linux/utsname.h>
#অন্তর্ভাৱ <linux/wait_api.h>
#অন্তর্ভাৱ <linux/workqueue_api.h>

#অন্তর্ভাৱ <uapi/linux/prctl.h>
#অন্তর্ভাৱ <uapi/linux/sched/types.h>

#অন্তর্ভাৱ <asm/switch_to.h>

#অন্তর্ভাৱ "sched.h"
#অন্তর্ভাৱ "sched-pelt.h"
#অন্তর্ভাৱ "stats.h"
#অন্তর্ভাৱ "autogroup.h"

#অন্তর্ভাৱ "clock.c"

#ifdef CONFIG_CGROUP_CPUACCT
# include "cpuacct.c"
#endif

#ifdef CONFIG_CPU_FREQ
# include "cpufreq.c"
#endif

#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL
# include "cpufreq_schedutil.c"
#endif

#অন্তর্ভাৱ "debug.c"

#ifdef CONFIG_SCHEDSTATS
# include "stats.c"
#endif

#অন্তর্ভাৱ "loadavg.c"
#অন্তর্ভাৱ "completion.c"
#অন্তর্ভাৱ "swait.c"
#অন্তর্ভাৱ "wait_bit.c"
#অন্তর্ভাৱ "wait.c"

#অন্তর্ভাৱ "cpupri.c"
#অন্তর্ভাৱ "stop_task.c"

#অন্তর্ভাৱ "topology.c"

#ifdef CONFIG_SCHED_CORE
# include "core_sched.c"
#endif

#ifdef CONFIG_PSI
# include "psi.c"
#endif

#ifdef CONFIG_MEMBARRIER
# include "membarrier.c"
#endif

#ifdef CONFIG_CPU_ISOLATION
# include "isolation.c"
#endif

#ifdef CONFIG_SCHED_AUTOGROUP
# include "autogroup.c"
#endif
