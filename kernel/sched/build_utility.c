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
#अन्तर्भाव <linux/sched/clock.h>
#अन्तर्भाव <linux/sched/cputime.h>
#अन्तर्भाव <linux/sched/debug.h>
#अन्तर्भाव <linux/sched/isolation.h>
#अन्तर्भाव <linux/sched/loadavg.h>
#अन्तर्भाव <linux/sched/nohz.h>
#अन्तर्भाव <linux/sched/mm.h>
#अन्तर्भाव <linux/sched/rseq_api.h>
#अन्तर्भाव <linux/sched/task_stack.h>

#अन्तर्भाव <linux/cpufreq.h>
#अन्तर्भाव <linux/cpumask_api.h>
#अन्तर्भाव <linux/cpuset.h>
#अन्तर्भाव <linux/ctype.h>
#अन्तर्भाव <linux/debugfs.h>
#अन्तर्भाव <linux/energy_model.h>
#अन्तर्भाव <linux/hashtable_api.h>
#अन्तर्भाव <linux/irq.h>
#अन्तर्भाव <linux/kobject_api.h>
#अन्तर्भाव <linux/membarrier.h>
#अन्तर्भाव <linux/mempolicy.h>
#अन्तर्भाव <linux/nmi.h>
#अन्तर्भाव <linux/nospec.h>
#अन्तर्भाव <linux/proc_fs.h>
#अन्तर्भाव <linux/psi.h>
#अन्तर्भाव <linux/ptrace_api.h>
#अन्तर्भाव <linux/sched_clock.h>
#अन्तर्भाव <linux/security.h>
#अन्तर्भाव <linux/spinlock_api.h>
#अन्तर्भाव <linux/swait_api.h>
#अन्तर्भाव <linux/timex.h>
#अन्तर्भाव <linux/utsname.h>
#अन्तर्भाव <linux/wait_api.h>
#अन्तर्भाव <linux/workqueue_api.h>

#अन्तर्भाव <uapi/linux/prctl.h>
#अन्तर्भाव <uapi/linux/sched/types.h>

#अन्तर्भाव <asm/switch_to.h>

#अन्तर्भाव "sched.h"
#अन्तर्भाव "sched-pelt.h"
#अन्तर्भाव "stats.h"
#अन्तर्भाव "autogroup.h"

#अन्तर्भाव "clock.c"

#यदि_परिभाषितम् CONFIG_CGROUP_CPUACCT
# include "cpuacct.c"
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_CPU_FREQ
# include "cpufreq.c"
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_CPU_FREQ_GOV_SCHEDUTIL
# include "cpufreq_schedutil.c"
#यदि_अन्तः

#अन्तर्भाव "debug.c"

#यदि_परिभाषितम् CONFIG_SCHEDSTATS
# include "stats.c"
#यदि_अन्तः

#अन्तर्भाव "loadavg.c"
#अन्तर्भाव "completion.c"
#अन्तर्भाव "swait.c"
#अन्तर्भाव "wait_bit.c"
#अन्तर्भाव "wait.c"

#अन्तर्भाव "cpupri.c"
#अन्तर्भाव "stop_task.c"

#अन्तर्भाव "topology.c"

#यदि_परिभाषितम् CONFIG_SCHED_CORE
# include "core_sched.c"
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_PSI
# include "psi.c"
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_MEMBARRIER
# include "membarrier.c"
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_CPU_ISOLATION
# include "isolation.c"
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_SCHED_AUTOGROUP
# include "autogroup.c"
#यदि_अन्तः
