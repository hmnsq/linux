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
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/clock.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/cputime.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/debug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/isolation.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/loadavg.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/nohz.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/mm.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/rseq_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/task_stack.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <linux/cpufreq.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/cpumask_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/cpuset.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/ctype.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/debugfs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/energy_model.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/hashtable_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/irq.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/kobject_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/membarrier.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/mempolicy.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/nmi.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/nospec.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/proc_fs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/psi.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/ptrace_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched_clock.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/security.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/spinlock_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/swait_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/timex.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/utsname.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/wait_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/workqueue_api.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <uapi/linux/prctl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <uapi/linux/sched/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <asm/switch_to.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sched-pelt.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stats.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "autogroup.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "clock.c"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CGROUP_CPUACCT
# include "cpuacct.c"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CPU_FREQ
# include "cpufreq.c"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CPU_FREQ_GOV_SCHEDUTIL
# include "cpufreq_schedutil.c"
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "debug.c"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHEDSTATS
# include "stats.c"
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "loadavg.c"
#ਅਨ੍ਤਰ੍ਭਾਵ "completion.c"
#ਅਨ੍ਤਰ੍ਭਾਵ "swait.c"
#ਅਨ੍ਤਰ੍ਭਾਵ "wait_bit.c"
#ਅਨ੍ਤਰ੍ਭਾਵ "wait.c"

#ਅਨ੍ਤਰ੍ਭਾਵ "cpupri.c"
#ਅਨ੍ਤਰ੍ਭਾਵ "stop_task.c"

#ਅਨ੍ਤਰ੍ਭਾਵ "topology.c"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CORE
# include "core_sched.c"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PSI
# include "psi.c"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_MEMBARRIER
# include "membarrier.c"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CPU_ISOLATION
# include "isolation.c"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_AUTOGROUP
# include "autogroup.c"
#ਜੇ_ਅੰਤ
