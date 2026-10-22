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
#અન્તર્ભાવ <linux/sched/clock.h>
#અન્તર્ભાવ <linux/sched/cputime.h>
#અન્તર્ભાવ <linux/sched/debug.h>
#અન્તર્ભાવ <linux/sched/isolation.h>
#અન્તર્ભાવ <linux/sched/loadavg.h>
#અન્તર્ભાવ <linux/sched/nohz.h>
#અન્તર્ભાવ <linux/sched/mm.h>
#અન્તર્ભાવ <linux/sched/rseq_api.h>
#અન્તર્ભાવ <linux/sched/task_stack.h>

#અન્તર્ભાવ <linux/cpufreq.h>
#અન્તર્ભાવ <linux/cpumask_api.h>
#અન્તર્ભાવ <linux/cpuset.h>
#અન્તર્ભાવ <linux/ctype.h>
#અન્તર્ભાવ <linux/debugfs.h>
#અન્તર્ભાવ <linux/energy_model.h>
#અન્તર્ભાવ <linux/hashtable_api.h>
#અન્તર્ભાવ <linux/irq.h>
#અન્તર્ભાવ <linux/kobject_api.h>
#અન્તર્ભાવ <linux/membarrier.h>
#અન્તર્ભાવ <linux/mempolicy.h>
#અન્તર્ભાવ <linux/nmi.h>
#અન્તર્ભાવ <linux/nospec.h>
#અન્તર્ભાવ <linux/proc_fs.h>
#અન્તર્ભાવ <linux/psi.h>
#અન્તર્ભાવ <linux/ptrace_api.h>
#અન્તર્ભાવ <linux/sched_clock.h>
#અન્તર્ભાવ <linux/security.h>
#અન્તર્ભાવ <linux/spinlock_api.h>
#અન્તર્ભાવ <linux/swait_api.h>
#અન્તર્ભાવ <linux/timex.h>
#અન્તર્ભાવ <linux/utsname.h>
#અન્તર્ભાવ <linux/wait_api.h>
#અન્તર્ભાવ <linux/workqueue_api.h>

#અન્તર્ભાવ <uapi/linux/prctl.h>
#અન્તર્ભાવ <uapi/linux/sched/types.h>

#અન્તર્ભાવ <asm/switch_to.h>

#અન્તર્ભાવ "sched.h"
#અન્તર્ભાવ "sched-pelt.h"
#અન્તર્ભાવ "stats.h"
#અન્તર્ભાવ "autogroup.h"

#અન્તર્ભાવ "clock.c"

#ifdef CONFIG_CGROUP_CPUACCT
# include "cpuacct.c"
#endif

#ifdef CONFIG_CPU_FREQ
# include "cpufreq.c"
#endif

#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL
# include "cpufreq_schedutil.c"
#endif

#અન્તર્ભાવ "debug.c"

#ifdef CONFIG_SCHEDSTATS
# include "stats.c"
#endif

#અન્તર્ભાવ "loadavg.c"
#અન્તર્ભાવ "completion.c"
#અન્તર્ભાવ "swait.c"
#અન્તર્ભાવ "wait_bit.c"
#અન્તર્ભાવ "wait.c"

#અન્તર્ભાવ "cpupri.c"
#અન્તર્ભાવ "stop_task.c"

#અન્તર્ભાવ "topology.c"

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
