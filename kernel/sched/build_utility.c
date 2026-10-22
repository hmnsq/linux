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
#அந்தர்பாவ <linux/sched/clock.h>
#அந்தர்பாவ <linux/sched/cputime.h>
#அந்தர்பாவ <linux/sched/debug.h>
#அந்தர்பாவ <linux/sched/isolation.h>
#அந்தர்பாவ <linux/sched/loadavg.h>
#அந்தர்பாவ <linux/sched/nohz.h>
#அந்தர்பாவ <linux/sched/mm.h>
#அந்தர்பாவ <linux/sched/rseq_api.h>
#அந்தர்பாவ <linux/sched/task_stack.h>

#அந்தர்பாவ <linux/cpufreq.h>
#அந்தர்பாவ <linux/cpumask_api.h>
#அந்தர்பாவ <linux/cpuset.h>
#அந்தர்பாவ <linux/ctype.h>
#அந்தர்பாவ <linux/debugfs.h>
#அந்தர்பாவ <linux/energy_model.h>
#அந்தர்பாவ <linux/hashtable_api.h>
#அந்தர்பாவ <linux/irq.h>
#அந்தர்பாவ <linux/kobject_api.h>
#அந்தர்பாவ <linux/membarrier.h>
#அந்தர்பாவ <linux/mempolicy.h>
#அந்தர்பாவ <linux/nmi.h>
#அந்தர்பாவ <linux/nospec.h>
#அந்தர்பாவ <linux/proc_fs.h>
#அந்தர்பாவ <linux/psi.h>
#அந்தர்பாவ <linux/ptrace_api.h>
#அந்தர்பாவ <linux/sched_clock.h>
#அந்தர்பாவ <linux/security.h>
#அந்தர்பாவ <linux/spinlock_api.h>
#அந்தர்பாவ <linux/swait_api.h>
#அந்தர்பாவ <linux/timex.h>
#அந்தர்பாவ <linux/utsname.h>
#அந்தர்பாவ <linux/wait_api.h>
#அந்தர்பாவ <linux/workqueue_api.h>

#அந்தர்பாவ <uapi/linux/prctl.h>
#அந்தர்பாவ <uapi/linux/sched/types.h>

#அந்தர்பாவ <asm/switch_to.h>

#அந்தர்பாவ "sched.h"
#அந்தர்பாவ "sched-pelt.h"
#அந்தர்பாவ "stats.h"
#அந்தர்பாவ "autogroup.h"

#அந்தர்பாவ "clock.c"

#ifdef CONFIG_CGROUP_CPUACCT
# include "cpuacct.c"
#endif

#ifdef CONFIG_CPU_FREQ
# include "cpufreq.c"
#endif

#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL
# include "cpufreq_schedutil.c"
#endif

#அந்தர்பாவ "debug.c"

#ifdef CONFIG_SCHEDSTATS
# include "stats.c"
#endif

#அந்தர்பாவ "loadavg.c"
#அந்தர்பாவ "completion.c"
#அந்தர்பாவ "swait.c"
#அந்தர்பாவ "wait_bit.c"
#அந்தர்பாவ "wait.c"

#அந்தர்பாவ "cpupri.c"
#அந்தர்பாவ "stop_task.c"

#அந்தர்பாவ "topology.c"

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
