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
#समावेश <linux/sched/clock.h>
#समावेश <linux/sched/cputime.h>
#समावेश <linux/sched/debug.h>
#समावेश <linux/sched/isolation.h>
#समावेश <linux/sched/loadavg.h>
#समावेश <linux/sched/nohz.h>
#समावेश <linux/sched/mm.h>
#समावेश <linux/sched/rseq_api.h>
#समावेश <linux/sched/task_stack.h>

#समावेश <linux/cpufreq.h>
#समावेश <linux/cpumask_api.h>
#समावेश <linux/cpuset.h>
#समावेश <linux/ctype.h>
#समावेश <linux/debugfs.h>
#समावेश <linux/energy_model.h>
#समावेश <linux/hashtable_api.h>
#समावेश <linux/irq.h>
#समावेश <linux/kobject_api.h>
#समावेश <linux/membarrier.h>
#समावेश <linux/mempolicy.h>
#समावेश <linux/nmi.h>
#समावेश <linux/nospec.h>
#समावेश <linux/proc_fs.h>
#समावेश <linux/psi.h>
#समावेश <linux/ptrace_api.h>
#समावेश <linux/sched_clock.h>
#समावेश <linux/security.h>
#समावेश <linux/spinlock_api.h>
#समावेश <linux/swait_api.h>
#समावेश <linux/timex.h>
#समावेश <linux/utsname.h>
#समावेश <linux/wait_api.h>
#समावेश <linux/workqueue_api.h>

#समावेश <uapi/linux/prctl.h>
#समावेश <uapi/linux/sched/types.h>

#समावेश <asm/switch_to.h>

#समावेश "sched.h"
#समावेश "sched-pelt.h"
#समावेश "stats.h"
#समावेश "autogroup.h"

#समावेश "clock.c"

#अगर_घोषित CONFIG_CGROUP_CPUACCT
# include "cpuacct.c"
#पूर्ण_अगर

#अगर_घोषित CONFIG_CPU_FREQ
# include "cpufreq.c"
#पूर्ण_अगर

#अगर_घोषित CONFIG_CPU_FREQ_GOV_SCHEDUTIL
# include "cpufreq_schedutil.c"
#पूर्ण_अगर

#समावेश "debug.c"

#अगर_घोषित CONFIG_SCHEDSTATS
# include "stats.c"
#पूर्ण_अगर

#समावेश "loadavg.c"
#समावेश "completion.c"
#समावेश "swait.c"
#समावेश "wait_bit.c"
#समावेश "wait.c"

#समावेश "cpupri.c"
#समावेश "stop_task.c"

#समावेश "topology.c"

#अगर_घोषित CONFIG_SCHED_CORE
# include "core_sched.c"
#पूर्ण_अगर

#अगर_घोषित CONFIG_PSI
# include "psi.c"
#पूर्ण_अगर

#अगर_घोषित CONFIG_MEMBARRIER
# include "membarrier.c"
#पूर्ण_अगर

#अगर_घोषित CONFIG_CPU_ISOLATION
# include "isolation.c"
#पूर्ण_अगर

#अगर_घोषित CONFIG_SCHED_AUTOGROUP
# include "autogroup.c"
#पूर्ण_अगर
