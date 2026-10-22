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
#ଅନ୍ତର୍ଭାଵ <linux/sched/clock.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/cputime.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/debug.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/isolation.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/loadavg.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/nohz.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/mm.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/rseq_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/task_stack.h>

#ଅନ୍ତର୍ଭାଵ <linux/cpufreq.h>
#ଅନ୍ତର୍ଭାଵ <linux/cpumask_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/cpuset.h>
#ଅନ୍ତର୍ଭାଵ <linux/ctype.h>
#ଅନ୍ତର୍ଭାଵ <linux/debugfs.h>
#ଅନ୍ତର୍ଭାଵ <linux/energy_model.h>
#ଅନ୍ତର୍ଭାଵ <linux/hashtable_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/irq.h>
#ଅନ୍ତର୍ଭାଵ <linux/kobject_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/membarrier.h>
#ଅନ୍ତର୍ଭାଵ <linux/mempolicy.h>
#ଅନ୍ତର୍ଭାଵ <linux/nmi.h>
#ଅନ୍ତର୍ଭାଵ <linux/nospec.h>
#ଅନ୍ତର୍ଭାଵ <linux/proc_fs.h>
#ଅନ୍ତର୍ଭାଵ <linux/psi.h>
#ଅନ୍ତର୍ଭାଵ <linux/ptrace_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched_clock.h>
#ଅନ୍ତର୍ଭାଵ <linux/security.h>
#ଅନ୍ତର୍ଭାଵ <linux/spinlock_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/swait_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/timex.h>
#ଅନ୍ତର୍ଭାଵ <linux/utsname.h>
#ଅନ୍ତର୍ଭାଵ <linux/wait_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/workqueue_api.h>

#ଅନ୍ତର୍ଭାଵ <uapi/linux/prctl.h>
#ଅନ୍ତର୍ଭାଵ <uapi/linux/sched/types.h>

#ଅନ୍ତର୍ଭାଵ <asm/switch_to.h>

#ଅନ୍ତର୍ଭାଵ "sched.h"
#ଅନ୍ତର୍ଭାଵ "sched-pelt.h"
#ଅନ୍ତର୍ଭାଵ "stats.h"
#ଅନ୍ତର୍ଭାଵ "autogroup.h"

#ଅନ୍ତର୍ଭାଵ "clock.c"

#ifdef CONFIG_CGROUP_CPUACCT
# include "cpuacct.c"
#endif

#ifdef CONFIG_CPU_FREQ
# include "cpufreq.c"
#endif

#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL
# include "cpufreq_schedutil.c"
#endif

#ଅନ୍ତର୍ଭାଵ "debug.c"

#ifdef CONFIG_SCHEDSTATS
# include "stats.c"
#endif

#ଅନ୍ତର୍ଭାଵ "loadavg.c"
#ଅନ୍ତର୍ଭାଵ "completion.c"
#ଅନ୍ତର୍ଭାଵ "swait.c"
#ଅନ୍ତର୍ଭାଵ "wait_bit.c"
#ଅନ୍ତର୍ଭାଵ "wait.c"

#ଅନ୍ତର୍ଭାଵ "cpupri.c"
#ଅନ୍ତର୍ଭାଵ "stop_task.c"

#ଅନ୍ତର୍ଭାଵ "topology.c"

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
