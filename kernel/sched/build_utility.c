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
#അന്തര്ഭാവ <linux/sched/clock.h>
#അന്തര്ഭാവ <linux/sched/cputime.h>
#അന്തര്ഭാവ <linux/sched/debug.h>
#അന്തര്ഭാവ <linux/sched/isolation.h>
#അന്തര്ഭാവ <linux/sched/loadavg.h>
#അന്തര്ഭാവ <linux/sched/nohz.h>
#അന്തര്ഭാവ <linux/sched/mm.h>
#അന്തര്ഭാവ <linux/sched/rseq_api.h>
#അന്തര്ഭാവ <linux/sched/task_stack.h>

#അന്തര്ഭാവ <linux/cpufreq.h>
#അന്തര്ഭാവ <linux/cpumask_api.h>
#അന്തര്ഭാവ <linux/cpuset.h>
#അന്തര്ഭാവ <linux/ctype.h>
#അന്തര്ഭാവ <linux/debugfs.h>
#അന്തര്ഭാവ <linux/energy_model.h>
#അന്തര്ഭാവ <linux/hashtable_api.h>
#അന്തര്ഭാവ <linux/irq.h>
#അന്തര്ഭാവ <linux/kobject_api.h>
#അന്തര്ഭാവ <linux/membarrier.h>
#അന്തര്ഭാവ <linux/mempolicy.h>
#അന്തര്ഭാവ <linux/nmi.h>
#അന്തര്ഭാവ <linux/nospec.h>
#അന്തര്ഭാവ <linux/proc_fs.h>
#അന്തര്ഭാവ <linux/psi.h>
#അന്തര്ഭാവ <linux/ptrace_api.h>
#അന്തര്ഭാവ <linux/sched_clock.h>
#അന്തര്ഭാവ <linux/security.h>
#അന്തര്ഭാവ <linux/spinlock_api.h>
#അന്തര്ഭാവ <linux/swait_api.h>
#അന്തര്ഭാവ <linux/timex.h>
#അന്തര്ഭാവ <linux/utsname.h>
#അന്തര്ഭാവ <linux/wait_api.h>
#അന്തര്ഭാവ <linux/workqueue_api.h>

#അന്തര്ഭാവ <uapi/linux/prctl.h>
#അന്തര്ഭാവ <uapi/linux/sched/types.h>

#അന്തര്ഭാവ <asm/switch_to.h>

#അന്തര്ഭാവ "sched.h"
#അന്തര്ഭാവ "sched-pelt.h"
#അന്തര്ഭാവ "stats.h"
#അന്തര്ഭാവ "autogroup.h"

#അന്തര്ഭാവ "clock.c"

#ifdef CONFIG_CGROUP_CPUACCT
# include "cpuacct.c"
#endif

#ifdef CONFIG_CPU_FREQ
# include "cpufreq.c"
#endif

#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL
# include "cpufreq_schedutil.c"
#endif

#അന്തര്ഭാവ "debug.c"

#ifdef CONFIG_SCHEDSTATS
# include "stats.c"
#endif

#അന്തര്ഭാവ "loadavg.c"
#അന്തര്ഭാവ "completion.c"
#അന്തര്ഭാവ "swait.c"
#അന്തര്ഭാവ "wait_bit.c"
#അന്തര്ഭാവ "wait.c"

#അന്തര്ഭാവ "cpupri.c"
#അന്തര്ഭാവ "stop_task.c"

#അന്തര്ഭാവ "topology.c"

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
