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
#অন্তর্ভাব <linux/sched/clock.h>
#অন্তর্ভাব <linux/sched/cputime.h>
#অন্তর্ভাব <linux/sched/debug.h>
#অন্তর্ভাব <linux/sched/isolation.h>
#অন্তর্ভাব <linux/sched/loadavg.h>
#অন্তর্ভাব <linux/sched/nohz.h>
#অন্তর্ভাব <linux/sched/mm.h>
#অন্তর্ভাব <linux/sched/rseq_api.h>
#অন্তর্ভাব <linux/sched/task_stack.h>

#অন্তর্ভাব <linux/cpufreq.h>
#অন্তর্ভাব <linux/cpumask_api.h>
#অন্তর্ভাব <linux/cpuset.h>
#অন্তর্ভাব <linux/ctype.h>
#অন্তর্ভাব <linux/debugfs.h>
#অন্তর্ভাব <linux/energy_model.h>
#অন্তর্ভাব <linux/hashtable_api.h>
#অন্তর্ভাব <linux/irq.h>
#অন্তর্ভাব <linux/kobject_api.h>
#অন্তর্ভাব <linux/membarrier.h>
#অন্তর্ভাব <linux/mempolicy.h>
#অন্তর্ভাব <linux/nmi.h>
#অন্তর্ভাব <linux/nospec.h>
#অন্তর্ভাব <linux/proc_fs.h>
#অন্তর্ভাব <linux/psi.h>
#অন্তর্ভাব <linux/ptrace_api.h>
#অন্তর্ভাব <linux/sched_clock.h>
#অন্তর্ভাব <linux/security.h>
#অন্তর্ভাব <linux/spinlock_api.h>
#অন্তর্ভাব <linux/swait_api.h>
#অন্তর্ভাব <linux/timex.h>
#অন্তর্ভাব <linux/utsname.h>
#অন্তর্ভাব <linux/wait_api.h>
#অন্তর্ভাব <linux/workqueue_api.h>

#অন্তর্ভাব <uapi/linux/prctl.h>
#অন্তর্ভাব <uapi/linux/sched/types.h>

#অন্তর্ভাব <asm/switch_to.h>

#অন্তর্ভাব "sched.h"
#অন্তর্ভাব "sched-pelt.h"
#অন্তর্ভাব "stats.h"
#অন্তর্ভাব "autogroup.h"

#অন্তর্ভাব "clock.c"

#ifdef CONFIG_CGROUP_CPUACCT
# include "cpuacct.c"
#endif

#ifdef CONFIG_CPU_FREQ
# include "cpufreq.c"
#endif

#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL
# include "cpufreq_schedutil.c"
#endif

#অন্তর্ভাব "debug.c"

#ifdef CONFIG_SCHEDSTATS
# include "stats.c"
#endif

#অন্তর্ভাব "loadavg.c"
#অন্তর্ভাব "completion.c"
#অন্তর্ভাব "swait.c"
#অন্তর্ভাব "wait_bit.c"
#অন্তর্ভাব "wait.c"

#অন্তর্ভাব "cpupri.c"
#অন্তর্ভাব "stop_task.c"

#অন্তর্ভাব "topology.c"

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
