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
#అన్తర్భావ <linux/sched/clock.h>
#అన్తర్భావ <linux/sched/cputime.h>
#అన్తర్భావ <linux/sched/debug.h>
#అన్తర్భావ <linux/sched/isolation.h>
#అన్తర్భావ <linux/sched/loadavg.h>
#అన్తర్భావ <linux/sched/nohz.h>
#అన్తర్భావ <linux/sched/mm.h>
#అన్తర్భావ <linux/sched/rseq_api.h>
#అన్తర్భావ <linux/sched/task_stack.h>

#అన్తర్భావ <linux/cpufreq.h>
#అన్తర్భావ <linux/cpumask_api.h>
#అన్తర్భావ <linux/cpuset.h>
#అన్తర్భావ <linux/ctype.h>
#అన్తర్భావ <linux/debugfs.h>
#అన్తర్భావ <linux/energy_model.h>
#అన్తర్భావ <linux/hashtable_api.h>
#అన్తర్భావ <linux/irq.h>
#అన్తర్భావ <linux/kobject_api.h>
#అన్తర్భావ <linux/membarrier.h>
#అన్తర్భావ <linux/mempolicy.h>
#అన్తర్భావ <linux/nmi.h>
#అన్తర్భావ <linux/nospec.h>
#అన్తర్భావ <linux/proc_fs.h>
#అన్తర్భావ <linux/psi.h>
#అన్తర్భావ <linux/ptrace_api.h>
#అన్తర్భావ <linux/sched_clock.h>
#అన్తర్భావ <linux/security.h>
#అన్తర్భావ <linux/spinlock_api.h>
#అన్తర్భావ <linux/swait_api.h>
#అన్తర్భావ <linux/timex.h>
#అన్తర్భావ <linux/utsname.h>
#అన్తర్భావ <linux/wait_api.h>
#అన్తర్భావ <linux/workqueue_api.h>

#అన్తర్భావ <uapi/linux/prctl.h>
#అన్తర్భావ <uapi/linux/sched/types.h>

#అన్తర్భావ <asm/switch_to.h>

#అన్తర్భావ "sched.h"
#అన్తర్భావ "sched-pelt.h"
#అన్తర్భావ "stats.h"
#అన్తర్భావ "autogroup.h"

#అన్తర్భావ "clock.c"

#నిర్వచితమైతే CONFIG_CGROUP_CPUACCT
# include "cpuacct.c"
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_CPU_FREQ
# include "cpufreq.c"
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_CPU_FREQ_GOV_SCHEDUTIL
# include "cpufreq_schedutil.c"
#అయితే_ముగింపు

#అన్తర్భావ "debug.c"

#నిర్వచితమైతే CONFIG_SCHEDSTATS
# include "stats.c"
#అయితే_ముగింపు

#అన్తర్భావ "loadavg.c"
#అన్తర్భావ "completion.c"
#అన్తర్భావ "swait.c"
#అన్తర్భావ "wait_bit.c"
#అన్తర్భావ "wait.c"

#అన్తర్భావ "cpupri.c"
#అన్తర్భావ "stop_task.c"

#అన్తర్భావ "topology.c"

#నిర్వచితమైతే CONFIG_SCHED_CORE
# include "core_sched.c"
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_PSI
# include "psi.c"
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_MEMBARRIER
# include "membarrier.c"
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_CPU_ISOLATION
# include "isolation.c"
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_SCHED_AUTOGROUP
# include "autogroup.c"
#అయితే_ముగింపు
