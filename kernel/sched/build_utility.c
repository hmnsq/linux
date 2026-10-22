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
#شامل <linux/sched/clock.h>
#شامل <linux/sched/cputime.h>
#شامل <linux/sched/debug.h>
#شامل <linux/sched/isolation.h>
#شامل <linux/sched/loadavg.h>
#شامل <linux/sched/nohz.h>
#شامل <linux/sched/mm.h>
#شامل <linux/sched/rseq_api.h>
#شامل <linux/sched/task_stack.h>

#شامل <linux/cpufreq.h>
#شامل <linux/cpumask_api.h>
#شامل <linux/cpuset.h>
#شامل <linux/ctype.h>
#شامل <linux/debugfs.h>
#شامل <linux/energy_model.h>
#شامل <linux/hashtable_api.h>
#شامل <linux/irq.h>
#شامل <linux/kobject_api.h>
#شامل <linux/membarrier.h>
#شامل <linux/mempolicy.h>
#شامل <linux/nmi.h>
#شامل <linux/nospec.h>
#شامل <linux/proc_fs.h>
#شامل <linux/psi.h>
#شامل <linux/ptrace_api.h>
#شامل <linux/sched_clock.h>
#شامل <linux/security.h>
#شامل <linux/spinlock_api.h>
#شامل <linux/swait_api.h>
#شامل <linux/timex.h>
#شامل <linux/utsname.h>
#شامل <linux/wait_api.h>
#شامل <linux/workqueue_api.h>

#شامل <uapi/linux/prctl.h>
#شامل <uapi/linux/sched/types.h>

#شامل <asm/switch_to.h>

#شامل "sched.h"
#شامل "sched-pelt.h"
#شامل "stats.h"
#شامل "autogroup.h"

#شامل "clock.c"

#ifdef CONFIG_CGROUP_CPUACCT
# include "cpuacct.c"
#endif

#ifdef CONFIG_CPU_FREQ
# include "cpufreq.c"
#endif

#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL
# include "cpufreq_schedutil.c"
#endif

#شامل "debug.c"

#ifdef CONFIG_SCHEDSTATS
# include "stats.c"
#endif

#شامل "loadavg.c"
#شامل "completion.c"
#شامل "swait.c"
#شامل "wait_bit.c"
#شامل "wait.c"

#شامل "cpupri.c"
#شامل "stop_task.c"

#شامل "topology.c"

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
