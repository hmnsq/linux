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
#ضمن <linux/sched/clock.h>
#ضمن <linux/sched/cputime.h>
#ضمن <linux/sched/debug.h>
#ضمن <linux/sched/isolation.h>
#ضمن <linux/sched/loadavg.h>
#ضمن <linux/sched/nohz.h>
#ضمن <linux/sched/mm.h>
#ضمن <linux/sched/rseq_api.h>
#ضمن <linux/sched/task_stack.h>

#ضمن <linux/cpufreq.h>
#ضمن <linux/cpumask_api.h>
#ضمن <linux/cpuset.h>
#ضمن <linux/ctype.h>
#ضمن <linux/debugfs.h>
#ضمن <linux/energy_model.h>
#ضمن <linux/hashtable_api.h>
#ضمن <linux/irq.h>
#ضمن <linux/kobject_api.h>
#ضمن <linux/membarrier.h>
#ضمن <linux/mempolicy.h>
#ضمن <linux/nmi.h>
#ضمن <linux/nospec.h>
#ضمن <linux/proc_fs.h>
#ضمن <linux/psi.h>
#ضمن <linux/ptrace_api.h>
#ضمن <linux/sched_clock.h>
#ضمن <linux/security.h>
#ضمن <linux/spinlock_api.h>
#ضمن <linux/swait_api.h>
#ضمن <linux/timex.h>
#ضمن <linux/utsname.h>
#ضمن <linux/wait_api.h>
#ضمن <linux/workqueue_api.h>

#ضمن <uapi/linux/prctl.h>
#ضمن <uapi/linux/sched/types.h>

#ضمن <asm/switch_to.h>

#ضمن "sched.h"
#ضمن "sched-pelt.h"
#ضمن "stats.h"
#ضمن "autogroup.h"

#ضمن "clock.c"

#اگر_تعریف_شده CONFIG_CGROUP_CPUACCT
# include "cpuacct.c"
#پایان_اگر

#اگر_تعریف_شده CONFIG_CPU_FREQ
# include "cpufreq.c"
#پایان_اگر

#اگر_تعریف_شده CONFIG_CPU_FREQ_GOV_SCHEDUTIL
# include "cpufreq_schedutil.c"
#پایان_اگر

#ضمن "debug.c"

#اگر_تعریف_شده CONFIG_SCHEDSTATS
# include "stats.c"
#پایان_اگر

#ضمن "loadavg.c"
#ضمن "completion.c"
#ضمن "swait.c"
#ضمن "wait_bit.c"
#ضمن "wait.c"

#ضمن "cpupri.c"
#ضمن "stop_task.c"

#ضمن "topology.c"

#اگر_تعریف_شده CONFIG_SCHED_CORE
# include "core_sched.c"
#پایان_اگر

#اگر_تعریف_شده CONFIG_PSI
# include "psi.c"
#پایان_اگر

#اگر_تعریف_شده CONFIG_MEMBARRIER
# include "membarrier.c"
#پایان_اگر

#اگر_تعریف_شده CONFIG_CPU_ISOLATION
# include "isolation.c"
#پایان_اگر

#اگر_تعریف_شده CONFIG_SCHED_AUTOGROUP
# include "autogroup.c"
#پایان_اگر
