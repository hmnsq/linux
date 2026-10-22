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
#ಅನ್ತರ್ಭಾವ <linux/sched/clock.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/cputime.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/debug.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/isolation.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/loadavg.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/nohz.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/mm.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/rseq_api.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/task_stack.h>

#ಅನ್ತರ್ಭಾವ <linux/cpufreq.h>
#ಅನ್ತರ್ಭಾವ <linux/cpumask_api.h>
#ಅನ್ತರ್ಭಾವ <linux/cpuset.h>
#ಅನ್ತರ್ಭಾವ <linux/ctype.h>
#ಅನ್ತರ್ಭಾವ <linux/debugfs.h>
#ಅನ್ತರ್ಭಾವ <linux/energy_model.h>
#ಅನ್ತರ್ಭಾವ <linux/hashtable_api.h>
#ಅನ್ತರ್ಭಾವ <linux/irq.h>
#ಅನ್ತರ್ಭಾವ <linux/kobject_api.h>
#ಅನ್ತರ್ಭಾವ <linux/membarrier.h>
#ಅನ್ತರ್ಭಾವ <linux/mempolicy.h>
#ಅನ್ತರ್ಭಾವ <linux/nmi.h>
#ಅನ್ತರ್ಭಾವ <linux/nospec.h>
#ಅನ್ತರ್ಭಾವ <linux/proc_fs.h>
#ಅನ್ತರ್ಭಾವ <linux/psi.h>
#ಅನ್ತರ್ಭಾವ <linux/ptrace_api.h>
#ಅನ್ತರ್ಭಾವ <linux/sched_clock.h>
#ಅನ್ತರ್ಭಾವ <linux/security.h>
#ಅನ್ತರ್ಭಾವ <linux/spinlock_api.h>
#ಅನ್ತರ್ಭಾವ <linux/swait_api.h>
#ಅನ್ತರ್ಭಾವ <linux/timex.h>
#ಅನ್ತರ್ಭಾವ <linux/utsname.h>
#ಅನ್ತರ್ಭಾವ <linux/wait_api.h>
#ಅನ್ತರ್ಭಾವ <linux/workqueue_api.h>

#ಅನ್ತರ್ಭಾವ <uapi/linux/prctl.h>
#ಅನ್ತರ್ಭಾವ <uapi/linux/sched/types.h>

#ಅನ್ತರ್ಭಾವ <asm/switch_to.h>

#ಅನ್ತರ್ಭಾವ "sched.h"
#ಅನ್ತರ್ಭಾವ "sched-pelt.h"
#ಅನ್ತರ್ಭಾವ "stats.h"
#ಅನ್ತರ್ಭಾವ "autogroup.h"

#ಅನ್ತರ್ಭಾವ "clock.c"

#ifdef CONFIG_CGROUP_CPUACCT
# include "cpuacct.c"
#endif

#ifdef CONFIG_CPU_FREQ
# include "cpufreq.c"
#endif

#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL
# include "cpufreq_schedutil.c"
#endif

#ಅನ್ತರ್ಭಾವ "debug.c"

#ifdef CONFIG_SCHEDSTATS
# include "stats.c"
#endif

#ಅನ್ತರ್ಭಾವ "loadavg.c"
#ಅನ್ತರ್ಭಾವ "completion.c"
#ಅನ್ತರ್ಭಾವ "swait.c"
#ಅನ್ತರ್ಭಾವ "wait_bit.c"
#ಅನ್ತರ್ಭಾವ "wait.c"

#ಅನ್ತರ್ಭಾವ "cpupri.c"
#ಅನ್ತರ್ಭಾವ "stop_task.c"

#ಅನ್ತರ್ಭಾವ "topology.c"

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
