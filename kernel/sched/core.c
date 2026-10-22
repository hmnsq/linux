// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/core.c
 *
 *  Core kernel CPU scheduler code
 *
 *  Copyright (C) 1991-2002  Linus Torvalds
 *  Copyright (C) 1998-2024  Ingo Molnar, Red Hat
 */
#ವ್ಯಾಖ್ಯಾನಿಸು INSTANTIATE_EXPORTED_MIGRATE_DISABLE
#ಅನ್ತರ್ಭಾವ <linux/sched.h>
#ಅನ್ತರ್ಭಾವ <linux/highmem.h>
#ಅನ್ತರ್ಭಾವ <linux/hrtimer_api.h>
#ಅನ್ತರ್ಭಾವ <linux/ktime_api.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/signal.h>
#ಅನ್ತರ್ಭಾವ <linux/syscalls_api.h>
#ಅನ್ತರ್ಭಾವ <linux/debug_locks.h>
#ಅನ್ತರ್ಭಾವ <linux/prefetch.h>
#ಅನ್ತರ್ಭಾವ <linux/capability.h>
#ಅನ್ತರ್ಭಾವ <linux/pgtable_api.h>
#ಅನ್ತರ್ಭಾವ <linux/wait_bit.h>
#ಅನ್ತರ್ಭಾವ <linux/jiffies.h>
#ಅನ್ತರ್ಭಾವ <linux/spinlock_api.h>
#ಅನ್ತರ್ಭಾವ <linux/cpumask_api.h>
#ಅನ್ತರ್ಭಾವ <linux/lockdep_api.h>
#ಅನ್ತರ್ಭಾವ <linux/hardirq.h>
#ಅನ್ತರ್ಭಾವ <linux/softirq.h>
#ಅನ್ತರ್ಭಾವ <linux/refcount_api.h>
#ಅನ್ತರ್ಭಾವ <linux/topology.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/clock.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/cond_resched.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/cputime.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/debug.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/hotplug.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/init.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/isolation.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/loadavg.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/mm.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/nohz.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/rseq_api.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/rt.h>

#ಅನ್ತರ್ಭಾವ <linux/context_tracking.h>
#ಅನ್ತರ್ಭಾವ <linux/cpuset.h>
#ಅನ್ತರ್ಭಾವ <linux/delayacct.h>
#ಅನ್ತರ್ಭಾವ <linux/init_task.h>
#ಅನ್ತರ್ಭಾವ <linux/interrupt.h>
#ಅನ್ತರ್ಭಾವ <linux/ioprio.h>
#ಅನ್ತರ್ಭಾವ <linux/kallsyms.h>
#ಅನ್ತರ್ಭಾವ <linux/kcov.h>
#ಅನ್ತರ್ಭಾವ <linux/kprobes.h>
#ಅನ್ತರ್ಭಾವ <linux/llist_api.h>
#ಅನ್ತರ್ಭಾವ <linux/mmu_context.h>
#ಅನ್ತರ್ಭಾವ <linux/mmzone.h>
#ಅನ್ತರ್ಭಾವ <linux/mutex_api.h>
#ಅನ್ತರ್ಭಾವ <linux/nmi.h>
#ಅನ್ತರ್ಭಾವ <linux/nospec.h>
#ಅನ್ತರ್ಭಾವ <linux/perf_event_api.h>
#ಅನ್ತರ್ಭಾವ <linux/profile.h>
#ಅನ್ತರ್ಭಾವ <linux/psi.h>
#ಅನ್ತರ್ಭಾವ <linux/rcuwait_api.h>
#ಅನ್ತರ್ಭಾವ <linux/rseq.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/wake_q.h>
#ಅನ್ತರ್ಭಾವ <linux/scs.h>
#ಅನ್ತರ್ಭಾವ <linux/slab.h>
#ಅನ್ತರ್ಭಾವ <linux/syscalls.h>
#ಅನ್ತರ್ಭಾವ <linux/vtime.h>
#ಅನ್ತರ್ಭಾವ <linux/wait_api.h>
#ಅನ್ತರ್ಭಾವ <linux/workqueue_api.h>
#ಅನ್ತರ್ಭಾವ <linux/livepatch_sched.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PREEMPT_DYNAMIC
# ifdef CONFIG_GENERIC_IRQ_ನಮೂದು
#  include <linux/irq-entry-common.h>
# endif
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <uapi/linux/sched/types.h>

#ಅನ್ತರ್ಭಾವ <asm/irq_regs.h>
#ಅನ್ತರ್ಭಾವ <asm/switch_to.h>
#ಅನ್ತರ್ಭಾವ <asm/tlb.h>

#ವ್ಯಾಖ್ಯಾನಿಸು CREATE_TRACE_POINTS
#ಅನ್ತರ್ಭಾವ <linux/sched/rseq_api.h>
#ಅನ್ತರ್ಭಾವ <trace/events/sched.h>
#ಅನ್ತರ್ಭಾವ <trace/events/ipi.h>
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CREATE_TRACE_POINTS

#ಅನ್ತರ್ಭಾವ "sched.h"
#ಅನ್ತರ್ಭಾವ "stats.h"

#ಅನ್ತರ್ಭಾವ "autogroup.h"
#ಅನ್ತರ್ಭಾವ "pelt.h"
#ಅನ್ತರ್ಭಾವ "smp.h"

#ಅನ್ತರ್ಭಾವ "../workqueue_internal.h"
#ಅನ್ತರ್ಭಾವ "../../io_uring/io-wq.h"
#ಅನ್ತರ್ಭಾವ "../smpboot.h"
#ಅನ್ತರ್ಭಾವ "../locking/mutex.h"

EXPORT_TRACEPOINT_ಚಿಹ್ನೆ_GPL(ipi_send_cpu);
EXPORT_TRACEPOINT_ಚಿಹ್ನೆ_GPL(ipi_send_cpumask);

/*
 * Export tracepoints that act as a bare tracehook (ie: have no trace event
 * associated with them) to allow external modules to probe them.
 */
EXPORT_TRACEPOINT_ಚಿಹ್ನೆ_GPL(pelt_cfs_tp);
EXPORT_TRACEPOINT_ಚಿಹ್ನೆ_GPL(pelt_rt_tp);
EXPORT_TRACEPOINT_ಚಿಹ್ನೆ_GPL(pelt_dl_tp);
EXPORT_TRACEPOINT_ಚಿಹ್ನೆ_GPL(pelt_irq_tp);
EXPORT_TRACEPOINT_ಚಿಹ್ನೆ_GPL(pelt_se_tp);
EXPORT_TRACEPOINT_ಚಿಹ್ನೆ_GPL(pelt_hw_tp);
EXPORT_TRACEPOINT_ಚಿಹ್ನೆ_GPL(sched_cpu_ಸಾಮರ್ಥ್ಯ_tp);
EXPORT_TRACEPOINT_ಚಿಹ್ನೆ_GPL(sched_overutilized_tp);
EXPORT_TRACEPOINT_ಚಿಹ್ನೆ_GPL(sched_util_est_cfs_tp);
EXPORT_TRACEPOINT_ಚಿಹ್ನೆ_GPL(sched_util_est_se_tp);
EXPORT_TRACEPOINT_ಚಿಹ್ನೆ_GPL(sched_update_nr_running_tp);
EXPORT_TRACEPOINT_ಚಿಹ್ನೆ_GPL(sched_compute_energy_tp);
EXPORT_TRACEPOINT_ಚಿಹ್ನೆ_GPL(sched_ನಮೂದು_tp);
EXPORT_TRACEPOINT_ಚಿಹ್ನೆ_GPL(sched_exit_tp);
EXPORT_TRACEPOINT_ಚಿಹ್ನೆ_GPL(sched_ಗಣ_need_resched_tp);
EXPORT_TRACEPOINT_ಚಿಹ್ನೆ_GPL(sched_dl_throttle_tp);
EXPORT_TRACEPOINT_ಚಿಹ್ನೆ_GPL(sched_dl_replenish_tp);
EXPORT_TRACEPOINT_ಚಿಹ್ನೆ_GPL(sched_dl_update_tp);
EXPORT_TRACEPOINT_ಚಿಹ್ನೆ_GPL(sched_dl_server_ಆರಂಭ_tp);
EXPORT_TRACEPOINT_ಚಿಹ್ನೆ_GPL(sched_dl_server_stop_tp);

DEFINE_ಪ್ರತಿ_CPU_SHARED_ALIGNED(ರಚನೆ rq, runqueues);
DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ rnd_state, sched_rnd_state);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_PROXY_EXEC
DEFINE_STATIC_ಕೀಲಿ_TRUE(__sched_proxy_exec);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init ಸಿದ್ಧತೆ_proxy_exec(ಅಕ್ಷರ *str)
{
	bool proxy_enable = true;

	ಆದರೆ (*str && kstrtobool(str + 1, &proxy_enable)) {
		pr_warn("Unable to parse sched_proxy_exec=\n");
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (proxy_enable) {
		pr_info("sched_proxy_exec enabled via boot arg\n");
		static_branch_enable(&__sched_proxy_exec);
	} ಇಲ್ಲದಿದ್ದರೆ {
		pr_info("sched_proxy_exec disabled via boot arg\n");
		static_branch_disable(&__sched_proxy_exec);
	}
	ಹಿಂತಿರುಗಿ 1;
}
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init ಸಿದ್ಧತೆ_proxy_exec(ಅಕ್ಷರ *str)
{
	pr_warn("CONFIG_SCHED_PROXY_EXEC=n, so it cannot be enabled or disabled at boot time\n");
	ಹಿಂತಿರುಗಿ 0;
}
#ಆದರೆ_ಅಂತ್ಯ
__setup("sched_proxy_exec", ಸಿದ್ಧತೆ_proxy_exec);

/*
 * Debugging: various feature bits
 *
 * If SCHED_DEBUG is disabled, each compilation unit has its own copy of
 * sysctl_sched_features, defined in sched.h, to allow constants propagation
 * at compile time and compiler optimization based on features default.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು SCHED_FEAT(name, enabled)	\
	(1UL << __SCHED_FEAT_##name) * enabled |
__read_mostly ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_sched_features =
#ಅನ್ತರ್ಭಾವ "features.h"
	0;
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು SCHED_FEAT

/*
 * Print a warning if need_resched is set for the given duration (if
 * LATENCY_WARN is enabled).
 *
 * If sysctl_resched_latency_warn_once is set, only one warning will be shown
 * per boot.
 */
__read_mostly ಪೂರ್ಣಾಂಕ sysctl_resched_latency_warn_ms = 100;
__read_mostly ಪೂರ್ಣಾಂಕ sysctl_resched_latency_warn_once = 1;

/*
 * Number of tasks to iterate in a single balance run.
 * Limited because this is done with IRQs disabled.
 */
__read_mostly ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_sched_nr_migrate = SCHED_NR_MIGRATE_BREAK;

__read_mostly ಪೂರ್ಣಾಂಕ scheduler_running;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CORE

DEFINE_STATIC_ಕೀಲಿ_FALSE(__sched_core_enabled);

/* kernel prio, less is more */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ __task_prio(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p)
{
	ಆದರೆ (p->sched_class == &stop_sched_class) /* trumps deadline */
		ಹಿಂತಿರುಗಿ -2;

	ಆದರೆ (p->dl_server)
		ಹಿಂತಿರುಗಿ -1; /* deadline */

	ಆದರೆ (rt_or_dl_prio(p->prio))
		ಹಿಂತಿರುಗಿ p->prio; /* [-1, 99] */

	ಆದರೆ (p->sched_class == &idle_sched_class)
		ಹಿಂತಿರುಗಿ MAX_RT_PRIO + NICE_ಅಗಲ; /* 140 */

	ಆದರೆ (task_on_scx(p))
		ಹಿಂತಿರುಗಿ MAX_RT_PRIO + MAX_NICE + 1; /* 120, squash ext */

	ಹಿಂತಿರುಗಿ MAX_RT_PRIO + MAX_NICE; /* 119, squash fair */
}

/*
 * l(a,b)
 * le(a,b) := !l(b,a)
 * g(a,b)  := l(b,a)
 * ge(a,b) := !l(a,b)
 */

/* real prio, less is less */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool prio_less(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *a,
			     ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *b, bool ಒಳ_fi)
{

	ಪೂರ್ಣಾಂಕ pa = __task_prio(a), pb = __task_prio(b);

	ಆದರೆ (-pa < -pb)
		ಹಿಂತಿರುಗಿ true;

	ಆದರೆ (-pb < -pa)
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (pa == -1) { /* dl_prio() doesn't work because of stop_class above */
		ಸ್ಥಿರಾಂಕ ರಚನೆ sched_dl_entity *a_dl, *b_dl;

		a_dl = &a->dl;
		/*
		 * Since,'a' and 'b' can be CFS tasks served by DL server,
		 * __task_prio() can return -1 (for DL) even for those. In that
		 * case, get to the dl_server's DL entity.
		 */
		ಆದರೆ (a->dl_server)
			a_dl = a->dl_server;

		b_dl = &b->dl;
		ಆದರೆ (b->dl_server)
			b_dl = b->dl_server;

		ಹಿಂತಿರುಗಿ !dl_ಸಮಯ_before(a_dl->deadline, b_dl->deadline);
	}

	ಆದರೆ (pa == MAX_RT_PRIO + MAX_NICE)	/* fair */
		ಹಿಂತಿರುಗಿ cfs_prio_less(a, b, ಒಳ_fi);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CLASS_EXT
	ಆದರೆ (pa == MAX_RT_PRIO + MAX_NICE + 1)	/* ext */
		ಹಿಂತಿರುಗಿ scx_prio_less(a, b, ಒಳ_fi);
#ಆದರೆ_ಅಂತ್ಯ

	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool __sched_core_less(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *a,
				     ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *b)
{
	ಆದರೆ (a->core_cookie < b->core_cookie)
		ಹಿಂತಿರುಗಿ true;

	ಆದರೆ (a->core_cookie > b->core_cookie)
		ಹಿಂತಿರುಗಿ false;

	/* flip prio, so high prio is leftmost */
	ಆದರೆ (prio_less(b, a, !!task_rq(a)->core->core_forceidle_ಎಣಿಕೆ))
		ಹಿಂತಿರುಗಿ true;

	ಹಿಂತಿರುಗಿ false;
}

#ವ್ಯಾಖ್ಯಾನಿಸು __node_2_sc(ಗಂಟು) rb_ನಮೂದು((ಗಂಟು), ರಚನೆ task_struct, core_ಗಂಟು)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool rb_sched_core_less(ರಚನೆ rb_ಗಂಟು *a, ಸ್ಥಿರಾಂಕ ರಚನೆ rb_ಗಂಟು *b)
{
	ಹಿಂತಿರುಗಿ __sched_core_less(__node_2_sc(a), __node_2_sc(b));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ rb_sched_core_cmp(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ರಚನೆ rb_ಗಂಟು *ಗಂಟು)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p = __node_2_sc(ಗಂಟು);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cookie = (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)ಕೀಲಿ;

	ಆದರೆ (cookie < p->core_cookie)
		ಹಿಂತಿರುಗಿ -1;

	ಆದರೆ (cookie > p->core_cookie)
		ಹಿಂತಿರುಗಿ 1;

	ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ sched_core_ಸರತಿಗೆ_ಸೇರಿಸು(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಆದರೆ (p->se.sched_delayed)
		ಹಿಂತಿರುಗಿ;

	rq->core->core_task_seq++;

	ಆದರೆ (!p->core_cookie)
		ಹಿಂತಿರುಗಿ;

	rb_ಕೂಡಿಸು(&p->core_ಗಂಟು, &rq->core_ವೃಕ್ಷ, rb_sched_core_less);
}

ಶೂನ್ಯ sched_core_ಸರತಿಯಿಂದ_ತೆಗೆ(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	ಆದರೆ (p->se.sched_delayed)
		ಹಿಂತಿರುಗಿ;

	rq->core->core_task_seq++;

	ಆದರೆ (sched_core_enqueued(p)) {
		rb_erase(&p->core_ಗಂಟು, &rq->core_ವೃಕ್ಷ);
		RB_CLEAR_ಗಂಟು(&p->core_ಗಂಟು);
	}

	/*
	 * Migrating the last task off the cpu, with the cpu in forced idle
	 * state. Reschedule to create an accounting edge for forced idle,
	 * and re-examine whether the core is still in forced idle state.
	 */
	ಆದರೆ (!(flags & ಸರತಿಯಿಂದ_ತೆಗೆ_SAVE) && rq->nr_running == 1 &&
	    rq->core->core_forceidle_ಎಣಿಕೆ && rq->curr == rq->idle)
		resched_curr(rq);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_task_is_throttled(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu)
{
	ಆದರೆ (p->sched_class->task_is_throttled)
		ಹಿಂತಿರುಗಿ p->sched_class->task_is_throttled(p, cpu);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ರಚನೆ task_struct *sched_core_ಮುಂದಿನ(ರಚನೆ task_struct *p, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cookie)
{
	ರಚನೆ rb_ಗಂಟು *ಗಂಟು = &p->core_ಗಂಟು;
	ಪೂರ್ಣಾಂಕ cpu = task_cpu(p);

	ಮಾಡು {
		ಗಂಟು = rb_ಮುಂದಿನ(ಗಂಟು);
		ಆದರೆ (!ಗಂಟು)
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;

		p = __node_2_sc(ಗಂಟು);
		ಆದರೆ (p->core_cookie != cookie)
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	} ಆಗ (sched_task_is_throttled(p, cpu));

	ಹಿಂತಿರುಗಿ p;
}

/*
 * Find left-most (aka, highest priority) and unthrottled task matching @cookie.
 * If no suitable task is found, NULL will be returned.
 */
ಸ್ಥಿರ ರಚನೆ task_struct *sched_core_ಕಂಡುಹಿಡಿ(ರಚನೆ rq *rq, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cookie)
{
	ರಚನೆ task_struct *p;
	ರಚನೆ rb_ಗಂಟು *ಗಂಟು;

	ಗಂಟು = rb_ಕಂಡುಹಿಡಿ_ಮೊದಲ((ಶೂನ್ಯ *)cookie, &rq->core_ವೃಕ್ಷ, rb_sched_core_cmp);
	ಆದರೆ (!ಗಂಟು)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	p = __node_2_sc(ಗಂಟು);
	ಆದರೆ (!sched_task_is_throttled(p, rq->cpu))
		ಹಿಂತಿರುಗಿ p;

	ಹಿಂತಿರುಗಿ sched_core_ಮುಂದಿನ(p, cookie);
}

/*
 * Magic required such that:
 *
 *	raw_spin_rq_lock(rq);
 *	...
 *	raw_spin_rq_unlock(rq);
 *
 * ends up locking and unlocking the _same_ lock, and all CPUs
 * always agree on what rq has what lock.
 *
 * XXX entirely possible to selectively enable cores, don't bother for now.
 */

ಸ್ಥಿರ DEFINE_MUTEX(sched_core_mutex);
ಸ್ಥಿರ atomic_t sched_core_ಎಣಿಕೆ;
ಸ್ಥಿರ ರಚನೆ cpumask sched_core_ಮುಖವಾಡ;

ಸ್ಥಿರ ಶೂನ್ಯ sched_core_ಬೀಗ(ಪೂರ್ಣಾಂಕ cpu, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *flags)
	__context_unsafe(/* acquires multiple */)
	__acquires(&runqueues.__lock) /* overapproximation */
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *smt_ಮುಖವಾಡ = cpu_smt_ಮುಖವಾಡ(cpu);
	ಪೂರ್ಣಾಂಕ t, i = 0;

	local_irq_save(*flags);
	for_each_cpu(t, smt_ಮುಖವಾಡ)
		raw_spin_ಬೀಗ_nested(&cpu_rq(t)->__lock, i++);
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_core_unlock(ಪೂರ್ಣಾಂಕ cpu, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *flags)
	__context_unsafe(/* releases multiple */)
	__releases(&runqueues.__lock) /* overapproximation */
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *smt_ಮುಖವಾಡ = cpu_smt_ಮುಖವಾಡ(cpu);
	ಪೂರ್ಣಾಂಕ t;

	for_each_cpu(t, smt_ಮುಖವಾಡ)
		raw_spin_unlock(&cpu_rq(t)->__lock);
	local_irq_restore(*flags);
}

ಸ್ಥಿರ ಶೂನ್ಯ __sched_core_flip(bool enabled)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;
	ಪೂರ್ಣಾಂಕ cpu, t;

	cpus_read_ಬೀಗ();

	/*
	 * Toggle the online cores, one by one.
	 */
	cpumask_copy(&sched_core_ಮುಖವಾಡ, cpu_online_ಮುಖವಾಡ);
	for_each_cpu(cpu, &sched_core_ಮುಖವಾಡ) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *smt_ಮುಖವಾಡ = cpu_smt_ಮುಖವಾಡ(cpu);

		sched_core_ಬೀಗ(cpu, &flags);

		/*
		 * A core-wide selection may have the shared rq lock temporarily
		 * released by a lock-dropping ->pick_task(). Flipping would
		 * rebind rq_lockp() under it. Wait it out.
		 */
		ಆಗ (cpu_rq(cpu)->core->core_pick_ಒಳ_flight) {
			sched_core_unlock(cpu, &flags);
			cpu_relax();
			sched_core_ಬೀಗ(cpu, &flags);
		}

		for_each_cpu(t, smt_ಮುಖವಾಡ)
			cpu_rq(t)->core_enabled = enabled;

		cpu_rq(cpu)->core->core_forceidle_ಆರಂಭ = 0;

		sched_core_unlock(cpu, &flags);

		cpumask_andnot(&sched_core_ಮುಖವಾಡ, &sched_core_ಮುಖವಾಡ, smt_ಮುಖವಾಡ);
	}

	/*
	 * Toggle the offline CPUs.
	 */
	for_each_cpu_andnot(cpu, cpu_possible_ಮುಖವಾಡ, cpu_online_ಮುಖವಾಡ)
		cpu_rq(cpu)->core_enabled = enabled;

	cpus_read_unlock();
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_core_assert_ಖಾಲಿಗಳು(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ cpu;

	for_each_possible_cpu(cpu)
		WARN_ON_ONCE(!RB_ಖಾಲಿಗಳು_ಬೇರು(&cpu_rq(cpu)->core_ವೃಕ್ಷ));
}

ಸ್ಥಿರ ಶೂನ್ಯ __sched_core_enable(ಶೂನ್ಯ)
{
	static_branch_enable(&__sched_core_enabled);
	/*
	 * Ensure all previous instances of raw_spin_rq_*lock() have finished
	 * and future ones will observe !sched_core_disabled().
	 */
	synchronize_rcu();
	__sched_core_flip(true);
	sched_core_assert_ಖಾಲಿಗಳು();
}

ಸ್ಥಿರ ಶೂನ್ಯ __sched_core_disable(ಶೂನ್ಯ)
{
	sched_core_assert_ಖಾಲಿಗಳು();
	__sched_core_flip(false);
	static_branch_disable(&__sched_core_enabled);
}

ಶೂನ್ಯ sched_core_get(ಶೂನ್ಯ)
{
	ಆದರೆ (atomic_inc_not_zero(&sched_core_ಎಣಿಕೆ))
		ಹಿಂತಿರುಗಿ;

	mutex_ಬೀಗ(&sched_core_mutex);
	ಆದರೆ (!atomic_read(&sched_core_ಎಣಿಕೆ))
		__sched_core_enable();

	smp_mb__before_atomic();
	atomic_inc(&sched_core_ಎಣಿಕೆ);
	mutex_unlock(&sched_core_mutex);
}

ಸ್ಥಿರ ಶೂನ್ಯ __sched_core_put(ರಚನೆ work_struct *work)
{
	ಆದರೆ (atomic_dec_and_mutex_ಬೀಗ(&sched_core_ಎಣಿಕೆ, &sched_core_mutex)) {
		__sched_core_disable();
		mutex_unlock(&sched_core_mutex);
	}
}

ಶೂನ್ಯ sched_core_put(ಶೂನ್ಯ)
{
	ಸ್ಥಿರ DECLARE_WORK(_work, __sched_core_put);

	/*
	 * "There can be only one"
	 *
	 * Either this is the last one, or we don't actually need to do any
	 * 'work'. If it is the last *again*, we rely on
	 * WORK_STRUCT_PENDING_BIT.
	 */
	ಆದರೆ (!atomic_ಕೂಡಿಸು_unless(&sched_core_ಎಣಿಕೆ, -1, 1))
		ವೇಳಾಪಟ್ಟಿ_work(&_work);
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_SCHED_CORE: */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_core_ಸರತಿಗೆ_ಸೇರಿಸು(ರಚನೆ rq *rq, ರಚನೆ task_struct *p) { }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
sched_core_ಸರತಿಯಿಂದ_ತೆಗೆ(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags) { }

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_SCHED_CORE */

/* need a wrapper since we may need to trace from modules */
EXPORT_TRACEPOINT_ಚಿಹ್ನೆ(sched_ಗಣ_state_tp);

/*
 * Call via the helper macro trace_set_current_state.
 * Calls to this function MUST be guarded by a
 * tracepoint_enabled(sched_set_state_tp)
 */
ಶೂನ್ಯ __trace_set_current_state(ಪೂರ್ಣಾಂಕ state_ಮೌಲ್ಯ)
{
	trace_call_sched_ಗಣ_state_tp(current, state_ಮೌಲ್ಯ);
}
EXPORT_ಚಿಹ್ನೆ(__trace_set_current_state);

ಪೂರ್ಣಾಂಕ task_llc(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ ಪ್ರತಿ_cpu(sd_llc_id, task_cpu(p));
}

/*
 * Serialization rules:
 *
 * Lock order:
 *
 *   p->pi_lock
 *     rq->lock
 *       hrtimer_cpu_base->lock (hrtimer_start() for bandwidth controls)
 *
 *  rq1->lock
 *    rq2->lock  where: rq1 < rq2
 *
 * Regular state:
 *
 * Normal scheduling state is serialized by rq->lock. __schedule() takes the
 * local CPU's rq->lock, it optionally removes the task from the runqueue and
 * always looks at the local rq data structures to find the most eligible task
 * to run next.
 *
 * Task enqueue is also under rq->lock, possibly taken from another CPU.
 * Wakeups from another LLC domain might use an IPI to transfer the enqueue to
 * the local CPU to avoid bouncing the runqueue state around [ see
 * ttwu_queue_wakelist() ]
 *
 * Task wakeup, specifically wakeups that involve migration, are horribly
 * complicated to avoid having to take two rq->locks.
 *
 * Special state:
 *
 * System-calls and anything external will use task_rq_lock() which acquires
 * both p->pi_lock and rq->lock. As a consequence the state they change is
 * stable while holding either lock:
 *
 *  - sched_setaffinity()/
 *    set_cpus_allowed_ptr():	p->cpus_ptr, p->nr_cpus_allowed
 *  - set_user_nice():		p->se.load, p->*prio
 *  - __sched_setscheduler():	p->sched_class, p->policy, p->*prio,
 *				p->se.load, p->rt_priority,
 *				p->dl.dl_{runtime, deadline, period, flags, bw, density}
 *  - sched_setnuma():		p->numa_preferred_nid
 *  - sched_move_task():	p->sched_task_group
 *  - uclamp_update_active()	p->uclamp*
 *
 * p->state <- TASK_*:
 *
 *   is changed locklessly using set_current_state(), __set_current_state() or
 *   set_special_state(), see their respective comments, or by
 *   try_to_wake_up(). This latter uses p->pi_lock to serialize against
 *   concurrent self.
 *
 * p->on_rq <- { 0, 1 = TASK_ON_RQ_QUEUED, 2 = TASK_ON_RQ_MIGRATING }:
 *
 *   is set by activate_task() and cleared by deactivate_task()/block_task(),
 *   under rq->lock. Non-zero indicates the task is runnable, the special
 *   ON_RQ_MIGRATING state is used for migration without holding both
 *   rq->locks. It indicates task_cpu() is not stable, see task_rq_lock().
 *
 *   Additionally it is possible to be ->on_rq but still be considered not
 *   runnable when p->se.sched_delayed is true. These tasks are on the runqueue
 *   but will be dequeued as soon as they get picked again. See the
 *   task_is_runnable() helper.
 *
 * p->on_cpu <- { 0, 1 }:
 *
 *   is set by prepare_task() and cleared by finish_task() such that it will be
 *   set before p is scheduled-in and cleared after p is scheduled-out, both
 *   under rq->lock. Non-zero indicates the task is running on its CPU.
 *
 *   [ The astute reader will observe that it is possible for two tasks on one
 *     CPU to have ->on_cpu = 1 at the same time. ]
 *
 * p->is_blocked <- { 0, 1 }:
 *
 *   is set by try_to_block_task() and cleared by ttwu_do_wakeup() and tracks
 *   if the task is blocked. Traditionally this would mirror p->on_rq, however
 *   due things like DELAY_DEQUEUE and PROXY_EXEC, this can diverge.
 *
 * task_cpu(p): is changed by set_task_cpu(), the rules are:
 *
 *  - Don't call set_task_cpu() on a blocked task:
 *
 *    We don't care what CPU we're not running on, this simplifies hotplug,
 *    the CPU assignment of blocked tasks isn't required to be valid.
 *
 *  - for try_to_wake_up(), called under p->pi_lock:
 *
 *    This allows try_to_wake_up() to only take one rq->lock, see its comment.
 *
 *  - for migration called under rq->lock:
 *    [ see task_on_rq_migrating() in task_rq_lock() ]
 *
 *    o move_queued_task()
 *    o detach_task()
 *
 *  - for migration called under double_rq_lock():
 *
 *    o __migrate_swap_task()
 *    o push_rt_task() / pull_rt_task()
 *    o push_dl_task() / pull_dl_task()
 *    o dl_task_offline_migration()
 *
 */

ಶೂನ್ಯ raw_spin_rq_ಬೀಗ_nested(ರಚನೆ rq *rq, ಪೂರ್ಣಾಂಕ subclass)
	__context_unsafe()
{
	raw_spinlock_t *ಬೀಗ;

	/* Matches synchronize_rcu() in __sched_core_enable() */
	preempt_disable();
	ಆದರೆ (sched_core_disabled()) {
		raw_spin_ಬೀಗ_nested(&rq->__lock, subclass);
		/* preempt_count *MUST* be > 1 */
		preempt_enable_no_resched();
		ಹಿಂತಿರುಗಿ;
	}

	ಗಾಗಿ (;;) {
		ಬೀಗ = __rq_lockp(rq);
		raw_spin_ಬೀಗ_nested(ಬೀಗ, subclass);
		ಆದರೆ (likely(ಬೀಗ == __rq_lockp(rq))) {
			/* preempt_count *MUST* be > 1 */
			preempt_enable_no_resched();
			ಹಿಂತಿರುಗಿ;
		}
		raw_spin_unlock(ಬೀಗ);
	}
}

bool raw_spin_rq_trylock(ರಚನೆ rq *rq)
	__context_unsafe()
{
	raw_spinlock_t *ಬೀಗ;
	bool ret;

	/* Matches synchronize_rcu() in __sched_core_enable() */
	preempt_disable();
	ಆದರೆ (sched_core_disabled()) {
		ret = raw_spin_trylock(&rq->__lock);
		preempt_enable();
		ಹಿಂತಿರುಗಿ ret;
	}

	ಗಾಗಿ (;;) {
		ಬೀಗ = __rq_lockp(rq);
		ret = raw_spin_trylock(ಬೀಗ);
		ಆದರೆ (!ret || (likely(ಬೀಗ == __rq_lockp(rq)))) {
			preempt_enable();
			ಹಿಂತಿರುಗಿ ret;
		}
		raw_spin_unlock(ಬೀಗ);
	}
}

/*
 * double_rq_lock - safely lock two runqueues
 */
ಶೂನ್ಯ double_rq_ಬೀಗ(ರಚನೆ rq *rq1, ರಚನೆ rq *rq2)
{
	lockdep_assert_irqs_disabled();

	ಆದರೆ (rq_ಕ್ರಮ_less(rq2, rq1))
		ಅದಲುಬದಲು(rq1, rq2);

	raw_spin_rq_ಬೀಗ(rq1);
	ಆದರೆ (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_ಬೀಗ_nested(rq2, SINGLE_ಆಳ_NESTING);
	ಇಲ್ಲದಿದ್ದರೆ
		__acquire_ctx_lock(__rq_lockp(rq2)); /* fake acquire */

	double_rq_clock_clear_update(rq1, rq2);
}

/*
 * ___task_rq_lock - lock the rq @p resides on.
 */
ರಚನೆ rq *___task_rq_lock(ರಚನೆ task_struct *p, ರಚನೆ rq_flags *rf)
{
	ರಚನೆ rq *rq;

	lockdep_assert_held(&p->pi_ಬೀಗ);

	ಗಾಗಿ (;;) {
		rq = task_rq(p);
		raw_spin_rq_ಬೀಗ(rq);
		ಆದರೆ (likely(rq == task_rq(p) && !task_on_rq_migrating(p))) {
			rq_pin_ಬೀಗ(rq, rf);
			ಹಿಂತಿರುಗಿ rq;
		}
		raw_spin_rq_unlock(rq);

		ಆಗ (unlikely(task_on_rq_migrating(p)))
			cpu_relax();
	}
}

/*
 * task_rq_lock - lock p->pi_lock and lock the rq @p resides on.
 */
ರಚನೆ rq *task_rq_ಬೀಗ(ರಚನೆ task_struct *p, ರಚನೆ rq_flags *rf)
{
	ರಚನೆ rq *rq;

	ಗಾಗಿ (;;) {
		raw_spin_ಬೀಗ_irqsave(&p->pi_ಬೀಗ, rf->flags);
		rq = task_rq(p);
		raw_spin_rq_ಬೀಗ(rq);
		/*
		 *	move_queued_task()		task_rq_lock()
		 *
		 *	ACQUIRE (rq->lock)
		 *	[S] ->on_rq = MIGRATING		[L] rq = task_rq()
		 *	WMB (__set_task_cpu())		ACQUIRE (rq->lock);
		 *	[S] ->cpu = new_cpu		[L] task_rq()
		 *					[L] ->on_rq
		 *	RELEASE (rq->lock)
		 *
		 * If we observe the old CPU in task_rq_lock(), the acquire of
		 * the old rq->lock will fully serialize against the stores.
		 *
		 * If we observe the new CPU in task_rq_lock(), the address
		 * dependency headed by '[L] rq = task_rq()' and the acquire
		 * will pair with the WMB to ensure we then also see migrating.
		 */
		ಆದರೆ (likely(rq == task_rq(p) && !task_on_rq_migrating(p))) {
			rq_pin_ಬೀಗ(rq, rf);
			ಹಿಂತಿರುಗಿ rq;
		}
		raw_spin_rq_unlock(rq);
		raw_spin_unlock_irqrestore(&p->pi_ಬೀಗ, rf->flags);

		ಆಗ (unlikely(task_on_rq_migrating(p)))
			cpu_relax();
	}
}

/*
 * RQ-clock updating methods:
 */

/* Use CONFIG_PARAVIRT as this will avoid more #ifdef in arch code. */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PARAVIRT
DEFINE_STATIC_ಕೀಲಿ_FALSE(paravirt_steal_rq_enabled);
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಶೂನ್ಯ update_rq_clock_task(ರಚನೆ rq *rq, s64 delta)
{
/*
 * In theory, the compile should just see 0 here, and optimize out the call
 * to sched_rt_avg_update. But I don't trust it...
 */
	s64 __maybe_unused steal = 0, irq_delta = 0;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_IRQ_ಸಮಯ_ACCOUNTING
	ಆದರೆ (irqtime_enabled()) {
		irq_delta = irq_ಸಮಯ_read(cpu_ದ(rq)) - rq->prev_irq_ಸಮಯ;

		/*
		 * Since irq_time is only updated on {soft,}irq_exit, we might run into
		 * this case when a previous update_rq_clock() happened inside a
		 * {soft,}IRQ region.
		 *
		 * When this happens, we stop ->clock_task and only update the
		 * prev_irq_time stamp to account for the part that fit, so that a next
		 * update will consume the rest. This ensures ->clock_task is
		 * monotonic.
		 *
		 * It does however cause some slight miss-attribution of {soft,}IRQ
		 * time, a more accurate solution would be to update the irq_time using
		 * the current rq->clock timestamp, except that would require using
		 * atomic ops.
		 */
		ಆದರೆ (irq_delta > delta)
			irq_delta = delta;

		rq->prev_irq_ಸಮಯ += irq_delta;
		delta -= irq_delta;
		delayacct_irq(rq->curr, irq_delta);
	}
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PARAVIRT_ಸಮಯ_ACCOUNTING
	ಆದರೆ (static_branch_unlikely(&paravirt_steal_rq_enabled)) {
		u64 prev_steal;

		steal = prev_steal = paravirt_steal_clock(cpu_ದ(rq));
		steal -= rq->prev_steal_ಸಮಯ_rq;

		ಆದರೆ (unlikely(steal > delta))
			steal = delta;

		rq->prev_steal_ಸಮಯ_rq = prev_steal;
		delta -= steal;
	}
#ಆದರೆ_ಅಂತ್ಯ

	rq->clock_task += delta;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_HAVE_SCHED_AVG_IRQ
	ಆದರೆ ((irq_delta + steal) && sched_feat(NONTASK_ಸಾಮರ್ಥ್ಯ))
		update_irq_ತುಂಬು_avg(rq, irq_delta + steal);
#ಆದರೆ_ಅಂತ್ಯ
	update_rq_clock_pelt(rq, delta);
}

ಶೂನ್ಯ update_rq_clock(ರಚನೆ rq *rq)
{
	s64 delta;
	u64 clock;

	lockdep_assert_rq_held(rq);

	ಆದರೆ (rq->clock_update_flags & RQCF_ACT_SKIP)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (sched_feat(WARN_DOUBLE_CLOCK))
		WARN_ON_ONCE(rq->clock_update_flags & RQCF_UPDATED);
	rq->clock_update_flags |= RQCF_UPDATED;

	clock = sched_clock_cpu(cpu_ದ(rq));
	scx_rq_clock_update(rq, clock);

	delta = clock - rq->clock;
	ಆದರೆ (delta < 0)
		ಹಿಂತಿರುಗಿ;
	rq->clock += delta;

	update_rq_clock_task(rq, delta);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_HRTICK
/*
 * Use HR-timers to deliver accurate preemption points.
 */

ಎಣಿಕೆ {
	HRTICK_SCHED_NONE		= 0,
	HRTICK_SCHED_DEFER		= ಬಿಟ್(1),
	HRTICK_SCHED_ಆರಂಭ		= ಬಿಟ್(2),
	HRTICK_SCHED_REARM_HRTIMER	= ಬಿಟ್(3)
};

ಸ್ಥಿರ ಶೂನ್ಯ __used hrtick_clear(ರಚನೆ rq *rq)
{
	ಆದರೆ (hrtimer_ಸಕ್ರಿಯ(&rq->hrtick_timer))
		hrtimer_cancel(&rq->hrtick_timer);
}

/*
 * High-resolution timer tick.
 * Runs from hardirq context with interrupts disabled.
 */
ಸ್ಥಿರ ಎಣಿಕೆ hrtimer_restart hrtick(ರಚನೆ hrtimer *timer)
{
	ರಚನೆ rq *rq = container_ದ(timer, ರಚನೆ rq, hrtick_timer);
	ರಚನೆ rq_flags rf;

	WARN_ON_ONCE(cpu_ದ(rq) != smp_processor_id());

	rq_ಬೀಗ(rq, &rf);
	update_rq_clock(rq);
	rq->donor->sched_class->task_tick(rq, rq->donor, 1);
	rq_unlock(rq, &rf);

	ಹಿಂತಿರುಗಿ HRTIMER_NORESTART;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool hrtick_needs_rearm(ರಚನೆ hrtimer *timer, ktime_t expires)
{
	/*
	 * Queued is false when the timer is not started or currently
	 * running the callback. In both cases, restart. If queued check
	 * whether the expiry time actually changes substantially.
	 */
	ಹಿಂತಿರುಗಿ !hrtimer_is_queued(timer) ||
		ನಿರಪೇಕ್ಷ(expires - hrtimer_get_expires(timer)) > 5000;
}

ಸ್ಥಿರ ಶೂನ್ಯ hrtick_cond_restart(ರಚನೆ rq *rq)
{
	ರಚನೆ hrtimer *timer = &rq->hrtick_timer;
	ktime_t ಸಮಯ = rq->hrtick_ಸಮಯ;

	ಆದರೆ (hrtick_needs_rearm(timer, ಸಮಯ))
		hrtimer_ಆರಂಭ(timer, ಸಮಯ, HRTIMER_ಬಹುಲಕ_ABS_PINNED_HARD);
}

/*
 * called from hardirq (IPI) context
 */
ಸ್ಥಿರ ಶೂನ್ಯ __hrtick_start(ಶೂನ್ಯ *arg)
{
	ರಚನೆ rq *rq = arg;
	ರಚನೆ rq_flags rf;

	rq_ಬೀಗ(rq, &rf);
	hrtick_cond_restart(rq);
	rq_unlock(rq, &rf);
}

/*
 * Called to set the hrtick timer state.
 *
 * called with rq->lock held and IRQs disabled
 */
ಶೂನ್ಯ hrtick_ಆರಂಭ(ರಚನೆ rq *rq, u64 delay)
{
	s64 delta;

	/*
	 * Don't schedule slices shorter than 10000ns, that just
	 * doesn't make sense and can cause timer DoS.
	 */
	delta = max_t(s64, delay, 10000LL);

	/*
	 * If this is in the middle of schedule() only note the delay
	 * and let hrtick_schedule_exit() deal with it.
	 */
	ಆದರೆ (rq->hrtick_sched) {
		rq->hrtick_sched |= HRTICK_SCHED_ಆರಂಭ;
		rq->hrtick_delay = delta;
		ಹಿಂತಿರುಗಿ;
	}

	rq->hrtick_ಸಮಯ = ktime_ಕೂಡಿಸು_ns(ktime_get(), delta);
	ಆದರೆ (!hrtick_needs_rearm(&rq->hrtick_timer, rq->hrtick_ಸಮಯ))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (rq == this_rq())
		hrtimer_ಆರಂಭ(&rq->hrtick_timer, rq->hrtick_ಸಮಯ, HRTIMER_ಬಹುಲಕ_ABS_PINNED_HARD);
	ಇಲ್ಲದಿದ್ದರೆ
		smp_call_function_single_async(cpu_ದ(rq), &rq->hrtick_csd);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ hrtick_ವೇಳಾಪಟ್ಟಿ_enter(ರಚನೆ rq *rq)
{
	rq->hrtick_sched = HRTICK_SCHED_DEFER;
	ಆದರೆ (hrtimer_test_and_clear_rearm_deferred())
		rq->hrtick_sched |= HRTICK_SCHED_REARM_HRTIMER;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ hrtick_ವೇಳಾಪಟ್ಟಿ_exit(ರಚನೆ rq *rq)
{
	ಆದರೆ (rq->hrtick_sched & HRTICK_SCHED_ಆರಂಭ) {
		rq->hrtick_ಸಮಯ = ktime_ಕೂಡಿಸು_ns(ktime_get(), rq->hrtick_delay);
		hrtick_cond_restart(rq);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (idle_rq(rq)) {
		/*
		 * No need for using hrtimer_is_active(). The timer is CPU local
		 * and interrupts are disabled, so the callback cannot be
		 * running and the queued state is valid.
		 */
		ಆದರೆ (hrtimer_is_queued(&rq->hrtick_timer))
			hrtimer_cancel(&rq->hrtick_timer);
	}

	ಆದರೆ (rq->hrtick_sched & HRTICK_SCHED_REARM_HRTIMER)
		__hrtimer_rearm_deferred();

	rq->hrtick_sched = HRTICK_SCHED_NONE;
}

ಸ್ಥಿರ ಶೂನ್ಯ hrtick_rq_init(ರಚನೆ rq *rq)
{
	INIT_CSD(&rq->hrtick_csd, __hrtick_start, rq);
	rq->hrtick_sched = HRTICK_SCHED_NONE;
	hrtimer_ಸಿದ್ಧತೆ(&rq->hrtick_timer, hrtick, CLOCK_MONOTONIC,
		      HRTIMER_ಬಹುಲಕ_REL_HARD | HRTIMER_ಬಹುಲಕ_LAZY_REARM);
}
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_SCHED_HRTICK: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ hrtick_clear(ರಚನೆ rq *rq) { }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ hrtick_rq_init(ರಚನೆ rq *rq) { }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ hrtick_ವೇಳಾಪಟ್ಟಿ_enter(ರಚನೆ rq *rq) { }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ hrtick_ವೇಳಾಪಟ್ಟಿ_exit(ರಚನೆ rq *rq) { }
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_SCHED_HRTICK */

/*
 * try_cmpxchg based fetch_or() macro so it works for different integer types:
 */
#ವ್ಯಾಖ್ಯಾನಿಸು fetch_or(ptr, ಮುಖವಾಡ)						\
	({								\
		typeof(ptr) _ptr = (ptr);				\
		typeof(ಮುಖವಾಡ) ಮುಖವಾಡ_2 = (ಮುಖವಾಡ);				\
		typeof(*_ptr) _val = *_ptr;				\
									\
		ಮಾಡು {							\
		} ಆಗ (!try_cmpxchg(_ptr, &_val, _val | ಮುಖವಾಡ_2));	\
	_val;								\
})

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TIF_POLLING_NRFLAG
/*
 * Atomically set TIF_NEED_RESCHED and test for TIF_POLLING_NRFLAG,
 * this avoids any races wrt polling state changes and thereby avoids
 * spurious IPIs.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool ಗಣ_nr_and_not_polling(ರಚನೆ thread_info *ti, ಪೂರ್ಣಾಂಕ tif)
{
	ಹಿಂತಿರುಗಿ !(fetch_or(&ti->flags, 1 << tif) & _TIF_POLLING_NRFLAG);
}

/*
 * Atomically set TIF_NEED_RESCHED if TIF_POLLING_NRFLAG is set.
 *
 * If this returns true, then the idle task promises to call
 * sched_ttwu_pending() and reschedule soon.
 */
ಸ್ಥಿರ bool ಗಣ_nr_if_polling(ರಚನೆ task_struct *p)
{
	ರಚನೆ thread_info *ti = task_thread_info(p);
	typeof(ti->flags) val = READ_ONCE(ti->flags);

	ಮಾಡು {
		ಆದರೆ (!(val & _TIF_POLLING_NRFLAG))
			ಹಿಂತಿರುಗಿ false;
		ಆದರೆ (val & _TIF_NEED_RESCHED)
			ಹಿಂತಿರುಗಿ true;
	} ಆಗ (!try_cmpxchg(&ti->flags, &val, val | _TIF_NEED_RESCHED));

	ಹಿಂತಿರುಗಿ true;
}

#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool ಗಣ_nr_and_not_polling(ರಚನೆ thread_info *ti, ಪೂರ್ಣಾಂಕ tif)
{
	ಗಣ_ti_thread_flag(ti, tif);
	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool ಗಣ_nr_if_polling(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ false;
}
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ bool __wake_q_add(ರಚನೆ wake_q_ತಲೆ *ತಲೆ, ರಚನೆ task_struct *task)
{
	ರಚನೆ wake_q_ಗಂಟು *ಗಂಟು = &task->wake_q;

	/*
	 * Atomically grab the task, if ->wake_q is !nil already it means
	 * it's already queued (either by us or someone else) and will get the
	 * wakeup due to that.
	 *
	 * In order to ensure that a pending wakeup will observe our pending
	 * state, even in the failed case, an explicit smp_mb() must be used.
	 */
	smp_mb__before_atomic();
	ಆದರೆ (unlikely(cmpxchg_relaxed(&ಗಂಟು->ಮುಂದಿನ, ಖಾಲಿ, WAKE_Q_ಬಾಲ)))
		ಹಿಂತಿರುಗಿ false;

	/*
	 * The head is context local, there can be no concurrency.
	 */
	*ತಲೆ->lastp = ಗಂಟು;
	ತಲೆ->lastp = &ಗಂಟು->ಮುಂದಿನ;
	ಹಿಂತಿರುಗಿ true;
}

/**
 * wake_q_add() - queue a wakeup for 'later' waking.
 * @head: the wake_q_head to add @task to
 * @task: the task to queue for 'later' wakeup
 *
 * Queue a task for later wakeup, most likely by the wake_up_q() call in the
 * same context, _HOWEVER_ this is not guaranteed, the wakeup can come
 * instantly.
 *
 * This function must be used as-if it were wake_up_process(); IOW the task
 * must be ready to be woken at this location.
 */
ಶೂನ್ಯ wake_q_ಕೂಡಿಸು(ರಚನೆ wake_q_ತಲೆ *ತಲೆ, ರಚನೆ task_struct *task)
{
	ಆದರೆ (__wake_q_add(ತಲೆ, task))
		get_task_struct(task);
}

/**
 * wake_q_add_safe() - safely queue a wakeup for 'later' waking.
 * @head: the wake_q_head to add @task to
 * @task: the task to queue for 'later' wakeup
 *
 * Queue a task for later wakeup, most likely by the wake_up_q() call in the
 * same context, _HOWEVER_ this is not guaranteed, the wakeup can come
 * instantly.
 *
 * This function must be used as-if it were wake_up_process(); IOW the task
 * must be ready to be woken at this location.
 *
 * This function is essentially a task-safe equivalent to wake_q_add(). Callers
 * that already hold reference to @task can call the 'safe' version and trust
 * wake_q to do the right thing depending whether or not the @task is already
 * queued for wakeup.
 */
ಶೂನ್ಯ wake_q_ಕೂಡಿಸು_ಸುರಕ್ಷಿತ(ರಚನೆ wake_q_ತಲೆ *ತಲೆ, ರಚನೆ task_struct *task)
{
	ಆದರೆ (!__wake_q_add(ತಲೆ, task))
		put_task_struct(task);
}

ಶೂನ್ಯ wake_ಮೇಲೆ_q(ರಚನೆ wake_q_ತಲೆ *ತಲೆ)
{
	ರಚನೆ wake_q_ಗಂಟು *ಗಂಟು = ತಲೆ->ಮೊದಲ;

	ಆಗ (ಗಂಟು != WAKE_Q_ಬಾಲ) {
		ರಚನೆ task_struct *task;

		task = container_ದ(ಗಂಟು, ರಚನೆ task_struct, wake_q);
		ಗಂಟು = ಗಂಟು->ಮುಂದಿನ;
		/* pairs with cmpxchg_relaxed() in __wake_q_add() */
		WRITE_ONCE(task->wake_q.ಮುಂದಿನ, ಖಾಲಿ);
		/* Task can safely be re-inserted now. */

		/*
		 * wake_up_process() executes a full barrier, which pairs with
		 * the queueing in wake_q_add() so as not to miss wakeups.
		 */
		wake_ಮೇಲೆ_process(task);
		put_task_struct(task);
	}
}

/*
 * resched_curr - mark rq's current task 'to be rescheduled now'.
 *
 * On UP this means the setting of the need_resched flag, on SMP it
 * might also involve a cross-CPU call to trigger the scheduler on
 * the target CPU.
 */
ಸ್ಥಿರ ಶೂನ್ಯ __resched_curr(ರಚನೆ rq *rq, ಪೂರ್ಣಾಂಕ tif)
{
	ರಚನೆ task_struct *curr = rq->curr;
	ರಚನೆ thread_info *cti = task_thread_info(curr);
	bool need_ipi;
	ಪೂರ್ಣಾಂಕ cpu;

	lockdep_assert_rq_held(rq);

	/*
	 * Always immediately preempt the idle task; no point in delaying doing
	 * actual work.
	 */
	ಆದರೆ (is_idle_task(curr) && tif == TIF_NEED_RESCHED_LAZY)
		tif = TIF_NEED_RESCHED;

	ಆದರೆ (cti->flags & ((1 << tif) | _TIF_NEED_RESCHED))
		ಹಿಂತಿರುಗಿ;

	cpu = cpu_ದ(rq);

	ಆದರೆ (cpu == smp_processor_id()) {
		ಗಣ_ti_thread_flag(cti, tif);
		ಆದರೆ (tif == TIF_NEED_RESCHED)
			ಗಣ_preempt_need_resched();
		trace_sched_ಗಣ_need_resched_tp(curr, cpu, tif);
		ಹಿಂತಿರುಗಿ;
	}

	need_ipi = ಗಣ_nr_and_not_polling(cti, tif);
	trace_sched_ಗಣ_need_resched_tp(curr, cpu, tif);
	ಆದರೆ (need_ipi) {
		ಆದರೆ (tif == TIF_NEED_RESCHED)
			smp_send_reschedule(cpu);
	} ಇಲ್ಲದಿದ್ದರೆ {
		trace_sched_wake_idle_without_ipi(cpu);
	}
}

/*
 * Calls to this function MUST be guarded by a
 * tracepoint_enabled(sched_set_need_resched_tp)
 */
ಶೂನ್ಯ __trace_set_need_resched(ರಚನೆ task_struct *curr, ಪೂರ್ಣಾಂಕ tif)
{
	trace_call_sched_ಗಣ_need_resched_tp(curr, smp_processor_id(), tif);
}
EXPORT_ಚಿಹ್ನೆ_GPL(__trace_set_need_resched);

ಶೂನ್ಯ resched_curr(ರಚನೆ rq *rq)
{
	__resched_curr(rq, TIF_NEED_RESCHED);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PREEMPT_DYNAMIC
ಸ್ಥಿರ DEFINE_STATIC_ಕೀಲಿ_FALSE(sk_dynamic_preempt_lazy);
ಸ್ಥಿರ __always_inline bool dynamic_preempt_lazy(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ static_branch_unlikely(&sk_dynamic_preempt_lazy);
}
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ __always_inline bool dynamic_preempt_lazy(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ IS_ENABLED(CONFIG_PREEMPT_LAZY);
}
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ __always_inline ಪೂರ್ಣಾಂಕ get_lazy_tif_ಬಿಟ್(ಶೂನ್ಯ)
{
	ಆದರೆ (dynamic_preempt_lazy())
		ಹಿಂತಿರುಗಿ TIF_NEED_RESCHED_LAZY;

	ಹಿಂತಿರುಗಿ TIF_NEED_RESCHED;
}

ಶೂನ್ಯ resched_curr_lazy(ರಚನೆ rq *rq)
{
	__resched_curr(rq, get_lazy_tif_ಬಿಟ್());
}

ಶೂನ್ಯ resched_cpu(ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	raw_spin_rq_ಬೀಗ_irqsave(rq, flags);
	ಆದರೆ (cpu_online(cpu) || cpu == smp_processor_id())
		resched_curr(rq);
	raw_spin_rq_unlock_irqrestore(rq, flags);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NO_HZ_COMMON
/*
 * In the semi idle case, use the nearest busy CPU for migrating timers
 * from an idle CPU.  This is good for power-savings.
 *
 * We don't do similar optimization for completely idle system, as
 * selecting an idle CPU will add more delays to the timers than intended
 * (as that CPU's timer base may not be up to date wrt jiffies etc).
 */
ಪೂರ್ಣಾಂಕ get_nohz_timer_ಗುರಿ(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ i, cpu = smp_processor_id(), default_cpu = -1;
	ರಚನೆ sched_domain *sd;
	ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *hk_ಮುಖವಾಡ;

	ಆದರೆ (housekeeping_cpu(cpu, HK_TYPE_KERNEL_NOISE)) {
		ಆದರೆ (!idle_cpu(cpu))
			ಹಿಂತಿರುಗಿ cpu;
		default_cpu = cpu;
	}

	hk_ಮುಖವಾಡ = housekeeping_cpumask(HK_TYPE_KERNEL_NOISE);

	guard(rcu)();

	for_each_domain(cpu, sd) {
		for_each_cpu_and(i, sched_domain_ಹರವು(sd), hk_ಮುಖವಾಡ) {
			ಆದರೆ (cpu == i)
				ಮುಂದುವರಿ;

			ಆದರೆ (!idle_cpu(i))
				ಹಿಂತಿರುಗಿ i;
		}
	}

	ಆದರೆ (default_cpu == -1)
		default_cpu = housekeeping_any_cpu(HK_TYPE_KERNEL_NOISE);

	ಹಿಂತಿರುಗಿ default_cpu;
}

/*
 * When add_timer_on() enqueues a timer into the timer wheel of an
 * idle CPU then this timer might expire before the next timer event
 * which is scheduled to wake up that CPU. In case of a completely
 * idle system the next event might even be infinite time into the
 * future. wake_up_idle_cpu() ensures that the CPU is woken up and
 * leaves the inner idle loop so the newly added timer is taken into
 * account when the CPU goes back to idle and evaluates the timer
 * wheel for the next timer event.
 */
ಸ್ಥಿರ ಶೂನ್ಯ wake_ಮೇಲೆ_idle_cpu(ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);

	ಆದರೆ (cpu == smp_processor_id())
		ಹಿಂತಿರುಗಿ;

	/*
	 * Set TIF_NEED_RESCHED and send an IPI if in the non-polling
	 * part of the idle loop. This forces an exit from the idle loop
	 * and a round trip to schedule(). Now this could be optimized
	 * because a simple new idle loop iteration is enough to
	 * re-evaluate the next tick. Provided some re-ordering of tick
	 * nohz functions that would need to follow TIF_NR_POLLING
	 * clearing:
	 *
	 * - On most architectures, a simple fetch_or on ti::flags with a
	 *   "0" value would be enough to know if an IPI needs to be sent.
	 *
	 * - x86 needs to perform a last need_resched() check between
	 *   monitor and mwait which doesn't take timers into account.
	 *   There a dedicated TIF_TIMER flag would be required to
	 *   fetch_or here and be checked along with TIF_NEED_RESCHED
	 *   before mwait().
	 *
	 * However, remote timer enqueue is not such a frequent event
	 * and testing of the above solutions didn't appear to report
	 * much benefits.
	 */
	ಆದರೆ (ಗಣ_nr_and_not_polling(task_thread_info(rq->idle), TIF_NEED_RESCHED))
		smp_send_reschedule(cpu);
	ಇಲ್ಲದಿದ್ದರೆ
		trace_sched_wake_idle_without_ipi(cpu);
}

ಸ್ಥಿರ bool wake_ಮೇಲೆ_ತುಂಬಿದ_nohz_cpu(ಪೂರ್ಣಾಂಕ cpu)
{
	/*
	 * We just need the target to call irq_exit() and re-evaluate
	 * the next tick. The nohz full kick at least implies that.
	 * If needed we can still optimize that later with an
	 * empty IRQ.
	 */
	ಆದರೆ (cpu_is_offline(cpu))
		ಹಿಂತಿರುಗಿ true;  /* Don't try to wake offline CPUs. */
	ಆದರೆ (tick_nohz_ತುಂಬಿದ_cpu(cpu)) {
		ಆದರೆ (cpu != smp_processor_id() ||
		    tick_nohz_tick_stopped())
			tick_nohz_ತುಂಬಿದ_kick_cpu(cpu);
		ಹಿಂತಿರುಗಿ true;
	}

	ಹಿಂತಿರುಗಿ false;
}

/*
 * Wake up the specified CPU.  If the CPU is going offline, it is the
 * caller's responsibility to deal with the lost wakeup, for example,
 * by hooking into the CPU_DEAD notifier like timers and hrtimers do.
 */
ಶೂನ್ಯ wake_ಮೇಲೆ_nohz_cpu(ಪೂರ್ಣಾಂಕ cpu)
{
	ಆದರೆ (!wake_ಮೇಲೆ_ತುಂಬಿದ_nohz_cpu(cpu))
		wake_ಮೇಲೆ_idle_cpu(cpu);
}

ಸ್ಥಿರ ಶೂನ್ಯ nohz_csd_func(ಶೂನ್ಯ *info)
{
	ರಚನೆ rq *rq = info;
	ಪೂರ್ಣಾಂಕ cpu = cpu_ದ(rq);
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags;

	/*
	 * Release the rq::nohz_csd.
	 */
	flags = atomic_fetch_andnot(NOHZ_KICK_ಮುಖವಾಡ | NOHZ_NEWILB_KICK, nohz_flags(cpu));
	WARN_ON(!(flags & NOHZ_KICK_ಮುಖವಾಡ));

	rq->idle_balance = idle_cpu(cpu);
	ಆದರೆ (rq->idle_balance) {
		rq->nohz_idle_balance = flags;
		__raise_softirq_irqoff(SCHED_SOFTIRQ);
	}
}

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_NO_HZ_COMMON */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NO_HZ_ತುಂಬಿದ
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool __need_bw_check(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಆದರೆ (rq->nr_running != 1)
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (p->sched_class != &fair_sched_class)
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (!task_on_rq_queued(p))
		ಹಿಂತಿರುಗಿ false;

	ಹಿಂತಿರುಗಿ true;
}

bool sched_can_stop_tick(ರಚನೆ rq *rq)
{
	ಪೂರ್ಣಾಂಕ fifo_nr_running;

	/* Deadline tasks, even if single, need the tick */
	ಆದರೆ (rq->dl.dl_nr_running)
		ಹಿಂತಿರುಗಿ false;

	/*
	 * If there are more than one RR tasks, we need the tick to affect the
	 * actual RR behaviour.
	 */
	ಆದರೆ (rq->rt.rr_nr_running) {
		ಆದರೆ (rq->rt.rr_nr_running == 1)
			ಹಿಂತಿರುಗಿ true;
		ಇಲ್ಲದಿದ್ದರೆ
			ಹಿಂತಿರುಗಿ false;
	}

	/*
	 * If there's no RR tasks, but FIFO tasks, we can skip the tick, no
	 * forced preemption between FIFO tasks.
	 */
	fifo_nr_running = rq->rt.rt_nr_running - rq->rt.rr_nr_running;
	ಆದರೆ (fifo_nr_running)
		ಹಿಂತಿರುಗಿ true;

	/*
	 * If there are no DL,RR/FIFO tasks, there must only be CFS or SCX tasks
	 * left. For CFS, if there's more than one we need the tick for
	 * involuntary preemption. For SCX, ask.
	 */
	ಆದರೆ (scx_enabled() && !scx_can_stop_tick(rq))
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (rq->cfs.h_nr_queued > 1)
		ಹಿಂತಿರುಗಿ false;

	/*
	 * If there is one task and it has CFS runtime bandwidth constraints
	 * and it's on the cpu now we don't want to stop the tick.
	 * This check prevents clearing the bit if a newly enqueued task here is
	 * dequeued by migrating while the constrained task continues to run.
	 * E.g. going from 2->1 without going through pick_next_task().
	 */
	ಆದರೆ (__need_bw_check(rq, rq->curr)) {
		ಆದರೆ (cfs_task_bw_constrained(rq->curr))
			ಹಿಂತಿರುಗಿ false;
	}

	ಹಿಂತಿರುಗಿ true;
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_NO_HZ_FULL */

#ಆದರೆ defined(CONFIG_RT_ಗುಂಪು_SCHED) || defined(CONFIG_FAIR_ಗುಂಪು_SCHED)
/*
 * Iterate task_group tree rooted at *from, calling @down when first entering a
 * node and @up when leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
ಪೂರ್ಣಾಂಕ walk_tg_ವೃಕ್ಷ_from(ರಚನೆ task_ಗುಂಪು *from,
			     tg_visitor ಕೆಳಕ್ಕೆ, tg_visitor ಮೇಲೆ, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ task_ಗುಂಪು *ಪೋಷಕ, *ಮಗು;
	ಪೂರ್ಣಾಂಕ ret;

	ಪೋಷಕ = from;

ಕೆಳಕ್ಕೆ:
	ret = (*ಕೆಳಕ್ಕೆ)(ಪೋಷಕ, ದತ್ತಾಂಶ);
	ಆದರೆ (ret)
		ಹೋಗು ಹೊರಹೋಗು;
	list_for_each_ನಮೂದು_rcu(ಮಗು, &ಪೋಷಕ->children, siblings) {
		ಪೋಷಕ = ಮಗು;
		ಹೋಗು ಕೆಳಕ್ಕೆ;

ಮೇಲೆ:
		ಮುಂದುವರಿ;
	}
	ret = (*ಮೇಲೆ)(ಪೋಷಕ, ದತ್ತಾಂಶ);
	ಆದರೆ (ret || ಪೋಷಕ == from)
		ಹೋಗು ಹೊರಹೋಗು;

	ಮಗು = ಪೋಷಕ;
	ಪೋಷಕ = ಪೋಷಕ->ಪೋಷಕ;
	ಆದರೆ (ಪೋಷಕ)
		ಹೋಗು ಮೇಲೆ;
ಹೊರಹೋಗು:
	ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ tg_nop(ರಚನೆ task_ಗುಂಪು *tg, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ಹಿಂತಿರುಗಿ 0;
}
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ ಗಣ_ತುಂಬು_ತೂಕ(ರಚನೆ task_struct *p, bool update_ತುಂಬು)
{
	ಪೂರ್ಣಾಂಕ prio = p->static_prio - MAX_RT_PRIO;
	ರಚನೆ ತುಂಬು_ತೂಕ lw;

	ಆದರೆ (task_has_idle_policy(p)) {
		lw.ತೂಕ = scale_ತುಂಬು(ತೂಕ_IDLEPRIO);
		lw.inv_ತೂಕ = WMULT_IDLEPRIO;
	} ಇಲ್ಲದಿದ್ದರೆ {
		lw.ತೂಕ = scale_ತುಂಬು(sched_prio_ಗಮ್ಯ_ತೂಕ[prio]);
		lw.inv_ತೂಕ = sched_prio_ಗಮ್ಯ_wmult[prio];
	}

	/*
	 * SCHED_OTHER tasks have to update their load when changing their
	 * weight
	 */
	ಆದರೆ (update_ತುಂಬು && p->sched_class->reweight_task)
		p->sched_class->reweight_task(task_rq(p), p, &lw);
	ಇಲ್ಲದಿದ್ದರೆ
		p->se.ತುಂಬು = lw;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK
/*
 * Serializes updates of utilization clamp values
 *
 * The (slow-path) user-space triggers utilization clamp value updates which
 * can require updates on (fast-path) scheduler's data structures used to
 * support enqueue/dequeue operations.
 * While the per-CPU rq lock protects fast-path update operations, user-space
 * requests are serialized using a mutex to reduce the risk of conflicting
 * updates or API abuses.
 */
ಸ್ಥಿರ __maybe_unused DEFINE_MUTEX(uclamp_mutex);

/* Max allowed minimum utilization */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ __maybe_unused sysctl_sched_uclamp_util_min = SCHED_ಸಾಮರ್ಥ್ಯ_SCALE;

/* Max allowed maximum utilization */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ __maybe_unused sysctl_sched_uclamp_util_max = SCHED_ಸಾಮರ್ಥ್ಯ_SCALE;

/*
 * By default RT tasks run at the maximum performance point/capacity of the
 * system. Uclamp enforces this by always setting UCLAMP_MIN of RT tasks to
 * SCHED_CAPACITY_SCALE.
 *
 * This knob allows admins to change the default behavior when uclamp is being
 * used. In battery powered devices, particularly, running at the maximum
 * capacity and frequency will increase energy consumption and shorten the
 * battery life.
 *
 * This knob only affects RT tasks that their uclamp_se->user_defined == false.
 *
 * This knob will not override the system default sched_util_clamp_min defined
 * above.
 */
ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_sched_uclamp_util_min_rt_default = SCHED_ಸಾಮರ್ಥ್ಯ_SCALE;

/* All clamps are required to be less or equal than these values */
ಸ್ಥಿರ ರಚನೆ uclamp_se uclamp_default[UCLAMP_CNT];

/*
 * This static key is used to reduce the uclamp overhead in the fast path. It
 * primarily disables the call to uclamp_rq_{inc, dec}() in
 * enqueue/dequeue_task().
 *
 * This allows users to continue to enable uclamp in their kernel config with
 * minimum uclamp overhead in the fast path.
 *
 * As soon as userspace modifies any of the uclamp knobs, the static key is
 * enabled, since we have an actual users that make use of uclamp
 * functionality.
 *
 * The knobs that would enable this static key are:
 *
 *   * A task modifying its uclamp value with sched_setattr().
 *   * An admin modifying the sysctl_sched_uclamp_{min, max} via procfs.
 *   * An admin modifying the cgroup cpu.uclamp.{min, max}
 */
DEFINE_STATIC_ಕೀಲಿ_FALSE(sched_uclamp_ಬಳಸಿದ);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ
uclamp_idle_ಮೌಲ್ಯ(ರಚನೆ rq *rq, ಎಣಿಕೆ uclamp_id clamp_id,
		  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ clamp_ಮೌಲ್ಯ)
{
	/*
	 * Avoid blocked utilization pushing up the frequency when we go
	 * idle (which drops the max-clamp) by retaining the last known
	 * max-clamp.
	 */
	ಆದರೆ (clamp_id == UCLAMP_MAX) {
		rq->uclamp_flags |= UCLAMP_FLAG_IDLE;
		ಹಿಂತಿರುಗಿ clamp_ಮೌಲ್ಯ;
	}

	ಹಿಂತಿರುಗಿ uclamp_none(UCLAMP_MIN);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ uclamp_idle_reset(ರಚನೆ rq *rq, ಎಣಿಕೆ uclamp_id clamp_id,
				     ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ clamp_ಮೌಲ್ಯ)
{
	/* Reset max-clamp retention only on idle exit */
	ಆದರೆ (!(rq->uclamp_flags & UCLAMP_FLAG_IDLE))
		ಹಿಂತಿರುಗಿ;

	uclamp_rq_ಗಣ(rq, clamp_id, clamp_ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ uclamp_rq_max_ಮೌಲ್ಯ(ರಚನೆ rq *rq, ಎಣಿಕೆ uclamp_id clamp_id,
				   ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ clamp_ಮೌಲ್ಯ)
{
	ರಚನೆ uclamp_ಪಾತ್ರೆ *ಪಾತ್ರೆ = rq->uclamp[clamp_id].ಪಾತ್ರೆ;
	ಪೂರ್ಣಾಂಕ ಪಾತ್ರೆ_id = UCLAMP_BUCKETS - 1;

	/*
	 * Since both min and max clamps are max aggregated, find the
	 * top most bucket with tasks in.
	 */
	ಗಾಗಿ ( ; ಪಾತ್ರೆ_id >= 0; ಪಾತ್ರೆ_id--) {
		ಆದರೆ (!ಪಾತ್ರೆ[ಪಾತ್ರೆ_id].tasks)
			ಮುಂದುವರಿ;
		ಹಿಂತಿರುಗಿ ಪಾತ್ರೆ[ಪಾತ್ರೆ_id].ಮೌಲ್ಯ;
	}

	/* No tasks -- default clamp values */
	ಹಿಂತಿರುಗಿ uclamp_idle_ಮೌಲ್ಯ(rq, clamp_id, clamp_ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಶೂನ್ಯ __uclamp_update_util_min_rt_default(ರಚನೆ task_struct *p)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ default_util_min;
	ರಚನೆ uclamp_se *uc_se;

	lockdep_assert_held(&p->pi_ಬೀಗ);

	uc_se = &p->uclamp_req[UCLAMP_MIN];

	/* Only sync if user didn't override the default */
	ಆದರೆ (uc_se->user_defined)
		ಹಿಂತಿರುಗಿ;

	default_util_min = sysctl_sched_uclamp_util_min_rt_default;
	uclamp_se_ಗಣ(uc_se, default_util_min, false);
}

ಸ್ಥಿರ ಶೂನ್ಯ uclamp_update_util_min_rt_default(ರಚನೆ task_struct *p)
{
	ಆದರೆ (!rt_task(p))
		ಹಿಂತಿರುಗಿ;

	/* Protect updates to p->uclamp_* */
	guard(task_rq_ಬೀಗ_2)(p);
	__uclamp_update_util_min_rt_default(p);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ uclamp_se
uclamp_tg_restrict(ರಚನೆ task_struct *p, ಎಣಿಕೆ uclamp_id clamp_id)
{
	/* Copy by value as we could modify it */
	ರಚನೆ uclamp_se uc_req = p->uclamp_req[clamp_id];
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK_ಗುಂಪು
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ tg_min, tg_max, ಮೌಲ್ಯ;

	/*
	 * Tasks in autogroups or root task group will be
	 * restricted by system defaults.
	 */
	ಆದರೆ (task_ಗುಂಪು_is_autogroup(task_ಗುಂಪು(p)))
		ಹಿಂತಿರುಗಿ uc_req;
	ಆದರೆ (task_ಗುಂಪು(p) == &ಬೇರು_task_ಗುಂಪು)
		ಹಿಂತಿರುಗಿ uc_req;

	tg_min = task_ಗುಂಪು(p)->uclamp[UCLAMP_MIN].ಮೌಲ್ಯ;
	tg_max = task_ಗುಂಪು(p)->uclamp[UCLAMP_MAX].ಮೌಲ್ಯ;
	ಮೌಲ್ಯ = uc_req.ಮೌಲ್ಯ;
	ಮೌಲ್ಯ = clamp(ಮೌಲ್ಯ, tg_min, tg_max);
	uclamp_se_ಗಣ(&uc_req, ಮೌಲ್ಯ, false);
#ಆದರೆ_ಅಂತ್ಯ

	ಹಿಂತಿರುಗಿ uc_req;
}

/*
 * The effective clamp bucket index of a task depends on, by increasing
 * priority:
 * - the task specific clamp value, when explicitly requested from userspace
 * - the task group effective clamp value, for tasks not either in the root
 *   group or in an autogroup
 * - the system default clamp value, defined by the sysadmin
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ uclamp_se
uclamp_eff_get(ರಚನೆ task_struct *p, ಎಣಿಕೆ uclamp_id clamp_id)
{
	ರಚನೆ uclamp_se uc_req = uclamp_tg_restrict(p, clamp_id);
	ರಚನೆ uclamp_se uc_max = uclamp_default[clamp_id];

	/* System default restrictions always apply */
	ಆದರೆ (unlikely(uc_req.ಮೌಲ್ಯ > uc_max.ಮೌಲ್ಯ))
		ಹಿಂತಿರುಗಿ uc_max;

	ಹಿಂತಿರುಗಿ uc_req;
}

ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ uclamp_eff_ಮೌಲ್ಯ(ರಚನೆ task_struct *p, ಎಣಿಕೆ uclamp_id clamp_id)
{
	ರಚನೆ uclamp_se uc_eff;

	/* Task currently refcounted: use back-annotated (effective) value */
	ಆದರೆ (p->uclamp[clamp_id].ಸಕ್ರಿಯ)
		ಹಿಂತಿರುಗಿ (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)p->uclamp[clamp_id].ಮೌಲ್ಯ;

	uc_eff = uclamp_eff_get(p, clamp_id);

	ಹಿಂತಿರುಗಿ (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)uc_eff.ಮೌಲ್ಯ;
}

/*
 * When a task is enqueued on a rq, the clamp bucket currently defined by the
 * task's uclamp::bucket_id is refcounted on that rq. This also immediately
 * updates the rq's clamp value if required.
 *
 * Tasks can have a task-specific value requested from user-space, track
 * within each bucket the maximum value for tasks refcounted in it.
 * This "local max aggregation" allows to track the exact "requested" value
 * for each bucket when all its RUNNABLE tasks require the same clamp.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ uclamp_rq_inc_id(ರಚನೆ rq *rq, ರಚನೆ task_struct *p,
				    ಎಣಿಕೆ uclamp_id clamp_id)
{
	ರಚನೆ uclamp_rq *uc_rq = &rq->uclamp[clamp_id];
	ರಚನೆ uclamp_se *uc_se = &p->uclamp[clamp_id];
	ರಚನೆ uclamp_ಪಾತ್ರೆ *ಪಾತ್ರೆ;

	lockdep_assert_rq_held(rq);

	/* Update task effective clamp */
	p->uclamp[clamp_id] = uclamp_eff_get(p, clamp_id);

	ಪಾತ್ರೆ = &uc_rq->ಪಾತ್ರೆ[uc_se->ಪಾತ್ರೆ_id];
	ಪಾತ್ರೆ->tasks++;
	uc_se->ಸಕ್ರಿಯ = true;

	uclamp_idle_reset(rq, clamp_id, uc_se->ಮೌಲ್ಯ);

	/*
	 * Local max aggregation: rq buckets always track the max
	 * "requested" clamp value of its RUNNABLE tasks.
	 */
	ಆದರೆ (ಪಾತ್ರೆ->tasks == 1 || uc_se->ಮೌಲ್ಯ > ಪಾತ್ರೆ->ಮೌಲ್ಯ)
		ಪಾತ್ರೆ->ಮೌಲ್ಯ = uc_se->ಮೌಲ್ಯ;

	ಆದರೆ (uc_se->ಮೌಲ್ಯ > uclamp_rq_get(rq, clamp_id))
		uclamp_rq_ಗಣ(rq, clamp_id, uc_se->ಮೌಲ್ಯ);
}

/*
 * When a task is dequeued from a rq, the clamp bucket refcounted by the task
 * is released. If this is the last task reference counting the rq's max
 * active clamp value, then the rq's clamp value is updated.
 *
 * Both refcounted tasks and rq's cached clamp values are expected to be
 * always valid. If it's detected they are not, as defensive programming,
 * enforce the expected state and warn.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ uclamp_rq_dec_id(ರಚನೆ rq *rq, ರಚನೆ task_struct *p,
				    ಎಣಿಕೆ uclamp_id clamp_id)
{
	ರಚನೆ uclamp_rq *uc_rq = &rq->uclamp[clamp_id];
	ರಚನೆ uclamp_se *uc_se = &p->uclamp[clamp_id];
	ರಚನೆ uclamp_ಪಾತ್ರೆ *ಪಾತ್ರೆ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ bkt_clamp;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ rq_clamp;

	lockdep_assert_rq_held(rq);

	/*
	 * If sched_uclamp_used was enabled after task @p was enqueued,
	 * we could end up with unbalanced call to uclamp_rq_dec_id().
	 *
	 * In this case the uc_se->active flag should be false since no uclamp
	 * accounting was performed at enqueue time and we can just return
	 * here.
	 *
	 * Need to be careful of the following enqueue/dequeue ordering
	 * problem too
	 *
	 *	enqueue(taskA)
	 *	// sched_uclamp_used gets enabled
	 *	enqueue(taskB)
	 *	dequeue(taskA)
	 *	// Must not decrement bucket->tasks here
	 *	dequeue(taskB)
	 *
	 * where we could end up with stale data in uc_se and
	 * bucket[uc_se->bucket_id].
	 *
	 * The following check here eliminates the possibility of such race.
	 */
	ಆದರೆ (unlikely(!uc_se->ಸಕ್ರಿಯ))
		ಹಿಂತಿರುಗಿ;

	ಪಾತ್ರೆ = &uc_rq->ಪಾತ್ರೆ[uc_se->ಪಾತ್ರೆ_id];

	WARN_ON_ONCE(!ಪಾತ್ರೆ->tasks);
	ಆದರೆ (likely(ಪಾತ್ರೆ->tasks))
		ಪಾತ್ರೆ->tasks--;

	uc_se->ಸಕ್ರಿಯ = false;

	/*
	 * Keep "local max aggregation" simple and accept to (possibly)
	 * overboost some RUNNABLE tasks in the same bucket.
	 * The rq clamp bucket value is reset to its base value whenever
	 * there are no more RUNNABLE tasks refcounting it.
	 */
	ಆದರೆ (likely(ಪಾತ್ರೆ->tasks))
		ಹಿಂತಿರುಗಿ;

	rq_clamp = uclamp_rq_get(rq, clamp_id);
	/*
	 * Defensive programming: this should never happen. If it happens,
	 * e.g. due to future modification, warn and fix up the expected value.
	 */
	WARN_ON_ONCE(ಪಾತ್ರೆ->ಮೌಲ್ಯ > rq_clamp);
	ಆದರೆ (ಪಾತ್ರೆ->ಮೌಲ್ಯ >= rq_clamp) {
		bkt_clamp = uclamp_rq_max_ಮೌಲ್ಯ(rq, clamp_id, uc_se->ಮೌಲ್ಯ);
		uclamp_rq_ಗಣ(rq, clamp_id, bkt_clamp);
	}
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ uclamp_rq_inc(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	ಎಣಿಕೆ uclamp_id clamp_id;

	/*
	 * Avoid any overhead until uclamp is actually used by the userspace.
	 *
	 * The condition is constructed such that a NOP is generated when
	 * sched_uclamp_used is disabled.
	 */
	ಆದರೆ (!uclamp_is_ಬಳಸಿದ())
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (unlikely(!p->sched_class->uclamp_enabled))
		ಹಿಂತಿರುಗಿ;

	/* Only inc the delayed task which being woken up. */
	ಆದರೆ (p->se.sched_delayed && !(flags & ಸರತಿಗೆ_ಸೇರಿಸು_DELAYED))
		ಹಿಂತಿರುಗಿ;

	for_each_clamp_id(clamp_id)
		uclamp_rq_inc_id(rq, p, clamp_id);

	/* Reset clamp idle holding when there is one RUNNABLE task */
	ಆದರೆ (rq->uclamp_flags & UCLAMP_FLAG_IDLE)
		rq->uclamp_flags &= ~UCLAMP_FLAG_IDLE;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ uclamp_rq_dec(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಎಣಿಕೆ uclamp_id clamp_id;

	/*
	 * Avoid any overhead until uclamp is actually used by the userspace.
	 *
	 * The condition is constructed such that a NOP is generated when
	 * sched_uclamp_used is disabled.
	 */
	ಆದರೆ (!uclamp_is_ಬಳಸಿದ())
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (unlikely(!p->sched_class->uclamp_enabled))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (p->se.sched_delayed)
		ಹಿಂತಿರುಗಿ;

	for_each_clamp_id(clamp_id)
		uclamp_rq_dec_id(rq, p, clamp_id);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ uclamp_rq_reinc_id(ರಚನೆ rq *rq, ರಚನೆ task_struct *p,
				      ಎಣಿಕೆ uclamp_id clamp_id)
{
	ಆದರೆ (!p->uclamp[clamp_id].ಸಕ್ರಿಯ)
		ಹಿಂತಿರುಗಿ;

	uclamp_rq_dec_id(rq, p, clamp_id);
	uclamp_rq_inc_id(rq, p, clamp_id);

	/*
	 * Make sure to clear the idle flag if we've transiently reached 0
	 * active tasks on rq.
	 */
	ಆದರೆ (clamp_id == UCLAMP_MAX && (rq->uclamp_flags & UCLAMP_FLAG_IDLE))
		rq->uclamp_flags &= ~UCLAMP_FLAG_IDLE;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
uclamp_update_ಸಕ್ರಿಯ(ರಚನೆ task_struct *p)
{
	ಎಣಿಕೆ uclamp_id clamp_id;
	ರಚನೆ rq_flags rf;
	ರಚನೆ rq *rq;

	/*
	 * Lock the task and the rq where the task is (or was) queued.
	 *
	 * We might lock the (previous) rq of a !RUNNABLE task, but that's the
	 * price to pay to safely serialize util_{min,max} updates with
	 * enqueues, dequeues and migration operations.
	 * This is the same locking schema used by __set_cpus_allowed_ptr().
	 */
	rq = task_rq_ಬೀಗ_2(p, &rf);

	/*
	 * Setting the clamp bucket is serialized by task_rq_lock().
	 * If the task is not yet RUNNABLE and its task_struct is not
	 * affecting a valid clamp bucket, the next time it's enqueued,
	 * it will already see the updated clamp bucket value.
	 */
	for_each_clamp_id(clamp_id)
		uclamp_rq_reinc_id(rq, p, clamp_id);

	task_rq_unlock(rq, p, &rf);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK_ಗುಂಪು
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
uclamp_update_ಸಕ್ರಿಯ_tasks(ರಚನೆ cgroup_subsys_state *css)
{
	ರಚನೆ css_task_iter it;
	ರಚನೆ task_struct *p;

	css_task_iter_ಆರಂಭ(css, 0, &it);
	ಆಗ ((p = css_task_iter_ಮುಂದಿನ(&it)))
		uclamp_update_ಸಕ್ರಿಯ(p);
	css_task_iter_ಕೊನೆ(&it);
}

ಸ್ಥಿರ ಶೂನ್ಯ cpu_util_update_eff(ರಚನೆ cgroup_subsys_state *css);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SYSCTL
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK_ಗುಂಪು
ಸ್ಥಿರ ಶೂನ್ಯ uclamp_update_ಬೇರು_tg(ಶೂನ್ಯ)
{
	ರಚನೆ task_ಗುಂಪು *tg = &ಬೇರು_task_ಗುಂಪು;

	uclamp_se_ಗಣ(&tg->uclamp_req[UCLAMP_MIN],
		      sysctl_sched_uclamp_util_min, false);
	uclamp_se_ಗಣ(&tg->uclamp_req[UCLAMP_MAX],
		      sysctl_sched_uclamp_util_max, false);

	guard(rcu)();
	cpu_util_update_eff(&ಬೇರು_task_ಗುಂಪು.css);
}
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಶೂನ್ಯ uclamp_update_ಬೇರು_tg(ಶೂನ್ಯ) { }
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಶೂನ್ಯ uclamp_sync_util_min_rt_default(ಶೂನ್ಯ)
{
	ರಚನೆ task_struct *g, *p;

	/*
	 * copy_process()			sysctl_uclamp
	 *					  uclamp_min_rt = X;
	 *   write_lock(&tasklist_lock)		  read_lock(&tasklist_lock)
	 *   // link thread			  smp_mb__after_spinlock()
	 *   write_unlock(&tasklist_lock)	  read_unlock(&tasklist_lock);
	 *   sched_post_fork()			  for_each_process_thread()
	 *     __uclamp_sync_rt()		    __uclamp_sync_rt()
	 *
	 * Ensures that either sched_post_fork() will observe the new
	 * uclamp_min_rt or for_each_process_thread() will observe the new
	 * task.
	 */
	read_ಬೀಗ(&tasklist_ಬೀಗ);
	smp_mb__after_spinlock();
	read_unlock(&tasklist_ಬೀಗ);

	guard(rcu)();
	for_each_process_thread(g, p)
		uclamp_update_util_min_rt_default(p);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sysctl_sched_uclamp_handler(ಸ್ಥಿರಾಂಕ ರಚನೆ ctl_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ, ಪೂರ್ಣಾಂಕ ಲಿಖ,
				ಶೂನ್ಯ *ಬಫರ್, ಗಾತ್ರ_t *lenp, loff_t *ppos)
{
	bool update_ಬೇರು_tg = false;
	ಪೂರ್ಣಾಂಕ old_min, old_max, old_min_rt;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;

	guard(mutex)(&uclamp_mutex);

	old_min = sysctl_sched_uclamp_util_min;
	old_max = sysctl_sched_uclamp_util_max;
	old_min_rt = sysctl_sched_uclamp_util_min_rt_default;

	ಫಲಿತಾಂಶ = proc_dointvec(ಕೋಷ್ಟಕ, ಲಿಖ, ಬಫರ್, lenp, ppos);
	ಆದರೆ (ಫಲಿತಾಂಶ)
		ಹೋಗು undo;
	ಆದರೆ (!ಲಿಖ)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (sysctl_sched_uclamp_util_min > sysctl_sched_uclamp_util_max ||
	    sysctl_sched_uclamp_util_max > SCHED_ಸಾಮರ್ಥ್ಯ_SCALE	||
	    sysctl_sched_uclamp_util_min_rt_default > SCHED_ಸಾಮರ್ಥ್ಯ_SCALE) {

		ಫಲಿತಾಂಶ = -EINVAL;
		ಹೋಗು undo;
	}

	ಆದರೆ (old_min != sysctl_sched_uclamp_util_min) {
		uclamp_se_ಗಣ(&uclamp_default[UCLAMP_MIN],
			      sysctl_sched_uclamp_util_min, false);
		update_ಬೇರು_tg = true;
	}
	ಆದರೆ (old_max != sysctl_sched_uclamp_util_max) {
		uclamp_se_ಗಣ(&uclamp_default[UCLAMP_MAX],
			      sysctl_sched_uclamp_util_max, false);
		update_ಬೇರು_tg = true;
	}

	ಆದರೆ (update_ಬೇರು_tg) {
		sched_uclamp_enable();
		uclamp_update_ಬೇರು_tg();
	}

	ಆದರೆ (old_min_rt != sysctl_sched_uclamp_util_min_rt_default) {
		sched_uclamp_enable();
		uclamp_sync_util_min_rt_default();
	}

	/*
	 * We update all RUNNABLE tasks only when task groups are in use.
	 * Otherwise, keep it simple and do just a lazy update at each next
	 * task enqueue time.
	 */
	ಹಿಂತಿರುಗಿ 0;

undo:
	sysctl_sched_uclamp_util_min = old_min;
	sysctl_sched_uclamp_util_max = old_max;
	sysctl_sched_uclamp_util_min_rt_default = old_min_rt;
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SYSCTL */

ಸ್ಥಿರ ಶೂನ್ಯ uclamp_fork(ರಚನೆ task_struct *p)
{
	ಎಣಿಕೆ uclamp_id clamp_id;

	/*
	 * We don't need to hold task_rq_lock() when updating p->uclamp_* here
	 * as the task is still at its early fork stages.
	 */
	for_each_clamp_id(clamp_id)
		p->uclamp[clamp_id].ಸಕ್ರಿಯ = false;

	ಆದರೆ (likely(!p->sched_reset_on_fork))
		ಹಿಂತಿರುಗಿ;

	for_each_clamp_id(clamp_id) {
		uclamp_se_ಗಣ(&p->uclamp_req[clamp_id],
			      uclamp_none(clamp_id), false);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ uclamp_post_fork(ರಚನೆ task_struct *p)
{
	uclamp_update_util_min_rt_default(p);
}

ಸ್ಥಿರ ಶೂನ್ಯ __init init_uclamp_rq(ರಚನೆ rq *rq)
{
	ಎಣಿಕೆ uclamp_id clamp_id;
	ರಚನೆ uclamp_rq *uc_rq = rq->uclamp;

	for_each_clamp_id(clamp_id) {
		uc_rq[clamp_id] = (ರಚನೆ uclamp_rq) {
			.ಮೌಲ್ಯ = uclamp_none(clamp_id)
		};
	}

	rq->uclamp_flags = UCLAMP_FLAG_IDLE;
}

ಸ್ಥಿರ ಶೂನ್ಯ __init init_uclamp(ಶೂನ್ಯ)
{
	ರಚನೆ uclamp_se uc_max = {};
	ಎಣಿಕೆ uclamp_id clamp_id;
	ಪೂರ್ಣಾಂಕ cpu;

	for_each_possible_cpu(cpu)
		init_uclamp_rq(cpu_rq(cpu));

	for_each_clamp_id(clamp_id) {
		uclamp_se_ಗಣ(&init_task.uclamp_req[clamp_id],
			      uclamp_none(clamp_id), false);
	}

	/* System defaults allow max clamp values for both indexes */
	uclamp_se_ಗಣ(&uc_max, uclamp_none(UCLAMP_MAX), false);
	for_each_clamp_id(clamp_id) {
		uclamp_default[clamp_id] = uc_max;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK_ಗುಂಪು
		ಬೇರು_task_ಗುಂಪು.uclamp_req[clamp_id] = uc_max;
		ಬೇರು_task_ಗುಂಪು.uclamp[clamp_id] = uc_max;
#ಆದರೆ_ಅಂತ್ಯ
	}
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_UCLAMP_TASK: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ uclamp_rq_inc(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags) { }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ uclamp_rq_dec(ರಚನೆ rq *rq, ರಚನೆ task_struct *p) { }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ uclamp_fork(ರಚನೆ task_struct *p) { }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ uclamp_post_fork(ರಚನೆ task_struct *p) { }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ init_uclamp(ಶೂನ್ಯ) { }
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_UCLAMP_TASK */

bool sched_task_on_rq(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ task_on_rq_queued(p);
}

ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ get_wchan(ರಚನೆ task_struct *p)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ip = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ state;

	ಆದರೆ (!p || p == current)
		ಹಿಂತಿರುಗಿ 0;

	/* Only get wchan if task is blocked and we can keep it that way. */
	raw_spin_ಬೀಗ_irq(&p->pi_ಬೀಗ);
	state = READ_ONCE(p->__state);
	smp_rmb(); /* see try_to_wake_up() */
	ಆದರೆ (state != TASK_RUNNING && state != TASK_WAKING && !p->on_rq)
		ip = __get_wchan(p);
	raw_spin_unlock_irq(&p->pi_ಬೀಗ);

	ಹಿಂತಿರುಗಿ ip;
}

ಶೂನ್ಯ ಸರತಿಗೆ_ಸೇರಿಸು_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	ಆದರೆ (!(flags & ಸರತಿಗೆ_ಸೇರಿಸು_NOCLOCK))
		update_rq_clock(rq);

	/*
	 * Can be before ->enqueue_task() because uclamp considers the
	 * ENQUEUE_DELAYED task before its ->sched_delayed gets cleared
	 * in ->enqueue_task().
	 */
	uclamp_rq_inc(rq, p, flags);

	p->sched_class->ಸರತಿಗೆ_ಸೇರಿಸು_task(rq, p, flags);

	psi_ಸರತಿಗೆ_ಸೇರಿಸು(p, flags);

	ಆದರೆ (!(flags & ಸರತಿಗೆ_ಸೇರಿಸು_RESTORE))
		sched_info_ಸರತಿಗೆ_ಸೇರಿಸು(rq, p);

	ಆದರೆ (sched_core_enabled(rq))
		sched_core_ಸರತಿಗೆ_ಸೇರಿಸು(rq, p);
}

/*
 * Must only return false when DEQUEUE_SLEEP.
 */
ಸಾಲೊಳಗೆ bool ಸರತಿಯಿಂದ_ತೆಗೆ_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	ಆದರೆ (sched_core_enabled(rq))
		sched_core_ಸರತಿಯಿಂದ_ತೆಗೆ(rq, p, flags);

	ಆದರೆ (!(flags & ಸರತಿಯಿಂದ_ತೆಗೆ_NOCLOCK))
		update_rq_clock(rq);

	ಆದರೆ (!(flags & ಸರತಿಯಿಂದ_ತೆಗೆ_SAVE))
		sched_info_ಸರತಿಯಿಂದ_ತೆಗೆ(rq, p);

	psi_ಸರತಿಯಿಂದ_ತೆಗೆ(p, flags);

	/*
	 * Must be before ->dequeue_task() because ->dequeue_task() can 'fail'
	 * and mark the task ->sched_delayed.
	 */
	uclamp_rq_dec(rq, p);
	ಹಿಂತಿರುಗಿ p->sched_class->ಸರತಿಯಿಂದ_ತೆಗೆ_task(rq, p, flags);
}

ಶೂನ್ಯ activate_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	ಆದರೆ (task_on_rq_migrating(p))
		flags |= ಸರತಿಗೆ_ಸೇರಿಸು_MIGRATED;

	ಸರತಿಗೆ_ಸೇರಿಸು_task(rq, p, flags);

	WRITE_ONCE(p->on_rq, TASK_ON_RQ_QUEUED);
	ASSERT_EXCLUSIVE_WRITER(p->on_rq);
}

ಶೂನ್ಯ deactivate_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	WARN_ON_ONCE(flags & ಸರತಿಯಿಂದ_ತೆಗೆ_SLEEP);

	WRITE_ONCE(p->on_rq, TASK_ON_RQ_MIGRATING);
	ASSERT_EXCLUSIVE_WRITER(p->on_rq);

	/*
	 * Code explicitly relies on TASK_ON_RQ_MIGRATING begin set *before*
	 * dequeue_task() and cleared *after* enqueue_task().
	 */

	ಸರತಿಯಿಂದ_ತೆಗೆ_task(rq, p, flags);
}

ಸ್ಥಿರ bool ಸರತಿಯಿಂದ_ತೆಗೆ_ಖಂಡ_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p,
			       ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ task_state)
{
	ಪೂರ್ಣಾಂಕ flags = ಸರತಿಯಿಂದ_ತೆಗೆ_NOCLOCK;

	p->sched_contributes_ಗಮ್ಯ_ತುಂಬು =
		(task_state & TASK_UNINTERRUPTIBLE) &&
		!(task_state & TASK_NOLOAD) &&
		!(task_state & TASK_FROZEN);

	ಆದರೆ (unlikely(is_special_task_state(task_state)))
		flags |= ಸರತಿಯಿಂದ_ತೆಗೆ_SPECIAL;

	/*
	 * __schedule()			ttwu()
	 *   prev_state = prev->state;    if (p->on_rq && ...)
	 *   if (prev_state)		    goto out;
	 *     p->on_rq = 0;		  smp_acquire__after_ctrl_dep();
	 *				  p->state = TASK_WAKING
	 *
	 * Where __schedule() and ttwu() have matching control dependencies.
	 *
	 * Once the caller invokes __block_task(), schedule() must not care about
	 * p->state any more.
	 */
	ಹಿಂತಿರುಗಿ ಸರತಿಯಿಂದ_ತೆಗೆ_task(rq, p, ಸರತಿಯಿಂದ_ತೆಗೆ_SLEEP | flags);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಖಂಡ_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ task_state)
{
	ಆದರೆ (ಸರತಿಯಿಂದ_ತೆಗೆ_ಖಂಡ_task(rq, p, task_state))
		__block_task(rq, p);
}

/**
 * task_curr - is this task currently executing on a CPU?
 * @p: the task in question.
 *
 * Return: 1 if the task is currently executing. 0 otherwise.
 */
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ task_curr(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ cpu_curr(task_cpu(p)) == p;
}

ಶೂನ್ಯ wakeup_preempt(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ task_struct *donor = rq->donor;

	ಆದರೆ (p->sched_class == rq->ಮುಂದಿನ_class) {
		rq->ಮುಂದಿನ_class->wakeup_preempt(rq, p, flags);

	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (sched_class_above(p->sched_class, rq->ಮುಂದಿನ_class)) {
		rq->ಮುಂದಿನ_class->wakeup_preempt(rq, p, flags);
		resched_curr(rq);
		rq->ಮುಂದಿನ_class = p->sched_class;
	}

	/*
	 * A queue event has occurred, and we're going to schedule.  In
	 * this case, we can save a useless back to back clock update.
	 */
	ಆದರೆ (task_on_rq_queued(donor) && test_tsk_need_resched(rq->curr))
		rq_clock_skip_update(rq);
}

ಸ್ಥಿರ __always_inline
ಪೂರ್ಣಾಂಕ __task_state_match(ರಚನೆ task_struct *p, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ state)
{
	ಆದರೆ (READ_ONCE(p->__state) & state)
		ಹಿಂತಿರುಗಿ 1;

	ಆದರೆ (READ_ONCE(p->saved_state) & state)
		ಹಿಂತಿರುಗಿ -1;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ __always_inline
ಪೂರ್ಣಾಂಕ task_state_ಹೊಂದಿಕೆ(ರಚನೆ task_struct *p, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ state)
{
	/*
	 * Serialize against current_save_and_set_rtlock_wait_state(),
	 * current_restore_rtlock_saved_state(), and __refrigerator().
	 */
	guard(raw_spinlock_irq)(&p->pi_ಬೀಗ);
	ಹಿಂತಿರುಗಿ __task_state_match(p, state);
}

/*
 * wait_task_inactive - wait for a thread to unschedule.
 *
 * Wait for the thread to block in any of the states set in @match_state.
 * If it changes, i.e. @p might have woken up, then return zero.  When we
 * succeed in waiting for @p to be off its CPU, we return a positive number
 * (its total switch count).  If a second call a short while later returns the
 * same number, the caller can be sure that @p has remained unscheduled the
 * whole time.
 *
 * The caller must ensure that the task *will* unschedule sometime soon,
 * else this function might spin for a *long* time. This function can't
 * be called with interrupts off, or it may introduce deadlock with
 * smp_call_function() if an IPI is sent by the same process we are
 * waiting to become inactive.
 */
ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ wait_task_inactive(ರಚನೆ task_struct *p, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಹೊಂದಿಕೆ_state)
{
	ಪೂರ್ಣಾಂಕ running, queued, ಹೊಂದಿಕೆ;
	ರಚನೆ rq_flags rf;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ncsw;
	ರಚನೆ rq *rq;

	ಗಾಗಿ (;;) {
		/*
		 * We do the initial early heuristics without holding
		 * any task-queue locks at all. We'll only try to get
		 * the runqueue lock when things look like they will
		 * work out!
		 */
		rq = task_rq(p);

		/*
		 * If the task is actively running on another CPU
		 * still, just relax and busy-wait without holding
		 * any locks.
		 *
		 * NOTE! Since we don't hold any locks, it's not
		 * even sure that "rq" stays as the right runqueue!
		 * But we don't care, since "task_on_cpu()" will
		 * return false if the runqueue has changed and p
		 * is actually now running somewhere else!
		 */
		ಆಗ (task_on_cpu(rq, p)) {
			ಆದರೆ (!task_state_ಹೊಂದಿಕೆ(p, ಹೊಂದಿಕೆ_state))
				ಹಿಂತಿರುಗಿ 0;
			cpu_relax();
		}

		/*
		 * Ok, time to look more closely! We need the rq
		 * lock now, to be *sure*. If we're wrong, we'll
		 * just go back and repeat.
		 */
		rq = task_rq_ಬೀಗ_2(p, &rf);
		/*
		 * If task is sched_delayed, force dequeue it, to avoid always
		 * hitting the tick timeout in the queued case
		 */
		ಆದರೆ (p->se.sched_delayed)
			ಸರತಿಯಿಂದ_ತೆಗೆ_task(rq, p, ಸರತಿಯಿಂದ_ತೆಗೆ_SLEEP | ಸರತಿಯಿಂದ_ತೆಗೆ_DELAYED);
		trace_sched_wait_task(p);
		running = task_on_cpu(rq, p);
		queued = task_on_rq_queued(p);
		ncsw = 0;
		ಆದರೆ ((ಹೊಂದಿಕೆ = __task_state_match(p, ಹೊಂದಿಕೆ_state))) {
			/*
			 * When matching on p->saved_state, consider this task
			 * still queued so it will wait.
			 */
			ಆದರೆ (ಹೊಂದಿಕೆ < 0)
				queued = 1;
			ncsw = p->nvcsw | LONG_MIN; /* sets MSB */
		}
		task_rq_unlock(rq, p, &rf);

		/*
		 * If it changed from the expected state, bail out now.
		 */
		ಆದರೆ (unlikely(!ncsw))
			ನಿಲ್ಲಿಸು;

		/*
		 * Was it really running after all now that we
		 * checked with the proper locks actually held?
		 *
		 * Oops. Go back and try again..
		 */
		ಆದರೆ (unlikely(running)) {
			cpu_relax();
			ಮುಂದುವರಿ;
		}

		/*
		 * It's not enough that it's not actively running,
		 * it must be off the runqueue _entirely_, and not
		 * preempted!
		 *
		 * So if it was still runnable (but just not actively
		 * running right now), it's preempted, and we should
		 * yield - it could be a while.
		 */
		ಆದರೆ (unlikely(queued)) {
			ktime_t ಗಮ್ಯ = NSEC_ಪ್ರತಿ_SEC / HZ;

			ಗಣ_current_state(TASK_UNINTERRUPTIBLE);
			ವೇಳಾಪಟ್ಟಿ_hrtimeout(&ಗಮ್ಯ, HRTIMER_ಬಹುಲಕ_REL_HARD);
			ಮುಂದುವರಿ;
		}

		/*
		 * Ahh, all good. It wasn't running, and it wasn't
		 * runnable, which means that it will never become
		 * running in the future either. We're all done!
		 */
		ನಿಲ್ಲಿಸು;
	}

	ಹಿಂತಿರುಗಿ ncsw;
}

ಸ್ಥಿರ ಶೂನ್ಯ
do_ಗಣ_cpus_allowed(ರಚನೆ task_struct *p, ರಚನೆ affinity_context *ctx);

ಸ್ಥಿರ ಶೂನ್ಯ migrate_disable_switch(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ರಚನೆ affinity_context ac = {
		.new_ಮುಖವಾಡ  = cpumask_ದ(rq->cpu),
		.flags     = SCA_MIGRATE_DISABLE,
	};

	ಆದರೆ (likely(!p->migration_disabled))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (p->cpus_ptr != &p->cpus_ಮುಖವಾಡ)
		ಹಿಂತಿರುಗಿ;

	scoped_guard (task_rq_ಬೀಗ_2, p)
		do_ಗಣ_cpus_allowed(p, &ac);
}

ಶೂನ್ಯ ___migrate_enable(ಶೂನ್ಯ)
{
	ರಚನೆ task_struct *p = current;
	ರಚನೆ affinity_context ac = {
		.new_ಮುಖವಾಡ  = &p->cpus_ಮುಖವಾಡ,
		.flags     = SCA_MIGRATE_ENABLE,
	};

	__set_cpus_allowed_ptr(p, &ac);
}
EXPORT_ಚಿಹ್ನೆ_GPL(___migrate_enable);

ಶೂನ್ಯ migrate_disable(ಶೂನ್ಯ)
{
	__migrate_disable();
}
EXPORT_ಚಿಹ್ನೆ_GPL(migrate_disable);

ಶೂನ್ಯ migrate_enable(ಶೂನ್ಯ)
{
	__migrate_enable();
}
EXPORT_ಚಿಹ್ನೆ_GPL(migrate_enable);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool rq_has_pinned_tasks(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ rq->nr_pinned;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool task_can_migrate_ಗಮ್ಯ_preferred(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu)
{
	/* No need to migrate from a preferred CPU */
	ಆದರೆ (cpu_preferred(cpu))
		ಹಿಂತಿರುಗಿ false;

	/* Only FAIR tasks honor preferred CPU state */
	ಆದರೆ (unlikely(p->sched_class != &fair_sched_class))
		ಹಿಂತಿರುಗಿ false;

	/* Ignore preferred state if task affinity is changing */
	ಆದರೆ (unlikely(!cpumask_test_cpu(task_cpu(p), p->cpus_ptr)))
		ಹಿಂತಿರುಗಿ false;

	ಹಿಂತಿರುಗಿ cpumask_intersects_and(p->cpus_ptr, cpu_preferred_ಮುಖವಾಡ,
				      task_cpu_possible_ಮುಖವಾಡ(p));
}

/*
 * Per-CPU kthreads are allowed to run on !active && online CPUs, see
 * __set_cpus_allowed_ptr() and select_fallback_rq().
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool is_cpu_allowed(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu)
{
	/* When not in the task's cpumask, no point in looking further. */
	ಆದರೆ (!task_allowed_on_cpu(p, cpu))
		ಹಿಂತಿರುಗಿ false;

	/* migrate_disabled() must be allowed to finish. */
	ಆದರೆ (is_migration_disabled(p))
		ಹಿಂತಿರುಗಿ cpu_online(cpu);

	/* Non kernel threads are not allowed during either online or offline. */
	ಆದರೆ (!(p->flags & PF_KTHREAD)) {
		/* Try to use preferred CPU if task's affinity allows */
		ಆದರೆ (task_can_migrate_ಗಮ್ಯ_preferred(p, cpu))
			ಹಿಂತಿರುಗಿ false;
		ಹಿಂತಿರುಗಿ cpu_ಸಕ್ರಿಯ(cpu);
	}

	/* KTHREAD_IS_PER_CPU is always allowed. */
	ಆದರೆ (kthread_is_ಪ್ರತಿ_cpu(p))
		ಹಿಂತಿರುಗಿ cpu_online(cpu);

	/* Regular kernel threads don't get to stay during offline. */
	ಆದರೆ (cpu_dying(cpu))
		ಹಿಂತಿರುಗಿ false;

	/* Try to keep unbound kthreads on a preferred CPU if possible. */
	ಆದರೆ (task_can_migrate_ಗಮ್ಯ_preferred(p, cpu))
		ಹಿಂತಿರುಗಿ false;

	/* Otherwise, they are allowed to run on online CPU. */
	ಹಿಂತಿರುಗಿ cpu_online(cpu);
}

/*
 * This is how migration works:
 *
 * 1) we invoke migration_cpu_stop() on the target CPU using
 *    stop_one_cpu().
 * 2) stopper starts to run (implicitly forcing the migrated thread
 *    off the CPU)
 * 3) it checks whether the migrated task is still in the wrong runqueue.
 * 4) if it's in the wrong runqueue then the migration thread removes
 *    it and puts it into the right queue.
 * 5) stopper completes and stop_one_cpu() returns and the migration
 *    is done.
 */

/*
 * move_queued_task - move a queued task to new rq.
 *
 * Returns (locked) new rq. Old rq's lock is released.
 */
ಸ್ಥಿರ ರಚನೆ rq *move_queued_task(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf,
				   ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ new_cpu)
	__must_hold(__rq_lockp(rq))
{
	lockdep_assert_rq_held(rq);

	deactivate_task(rq, p, ಸರತಿಯಿಂದ_ತೆಗೆ_NOCLOCK);
	ಗಣ_task_cpu(p, new_cpu);
	rq_unlock(rq, rf);

	rq = cpu_rq(new_cpu);

	rq_ಬೀಗ(rq, rf);
	WARN_ON_ONCE(task_cpu(p) != new_cpu);
	activate_task(rq, p, 0);
	wakeup_preempt(rq, p, 0);

	ಹಿಂತಿರುಗಿ rq;
}

ರಚನೆ migration_arg {
	ರಚನೆ task_struct		*task;
	ಪೂರ್ಣಾಂಕ				dest_cpu;
	ರಚನೆ ಗಣ_affinity_pending	*pending;
};

/*
 * @refs: number of wait_for_completion()
 * @stop_pending: is @stop_work in use
 */
ರಚನೆ ಗಣ_affinity_pending {
	refcount_t		refs;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		stop_pending;
	ರಚನೆ completion	ಮುಗಿದ;
	ರಚನೆ cpu_stop_work	stop_work;
	ರಚನೆ migration_arg	arg;
};

/*
 * Move (not current) task off this CPU, onto the destination CPU. We're doing
 * this because either it can't run here any more (set_cpus_allowed()
 * away from this CPU, or CPU going down), or because we're
 * attempting to rebalance this task on exec (sched_exec).
 *
 * So we race with normal scheduler movements, but that's OK, as long
 * as the task is no longer on this CPU.
 */
ಸ್ಥಿರ ರಚನೆ rq *__migrate_task(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf,
				 ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ dest_cpu)
	__must_hold(__rq_lockp(rq))
{
	/* Affinity changed (again). */
	ಆದರೆ (!is_cpu_allowed(p, dest_cpu))
		ಹಿಂತಿರುಗಿ rq;

	rq = move_queued_task(rq, rf, p, dest_cpu);

	ಹಿಂತಿರುಗಿ rq;
}

/*
 * migration_cpu_stop - this will be executed by a high-prio stopper thread
 * and performs thread migration by bumping thread off CPU then
 * 'pushing' onto another runqueue.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ migration_cpu_stop(ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ migration_arg *arg = ದತ್ತಾಂಶ;
	ರಚನೆ ಗಣ_affinity_pending *pending = arg->pending;
	ರಚನೆ task_struct *p = arg->task;
	ರಚನೆ rq *rq = this_rq();
	bool complete = false;
	ರಚನೆ rq_flags rf;

	/*
	 * The original target CPU might have gone down and we might
	 * be on another CPU but it doesn't matter.
	 */
	local_irq_save(rf.flags);
	/*
	 * We need to explicitly wake pending tasks before running
	 * __migrate_task() such that we will not miss enforcing cpus_ptr
	 * during wakeups, see set_cpus_allowed_ptr()'s TASK_WAKING test.
	 */
	flush_smp_call_function_ಸರತಿ();

	/*
	 * We may change the underlying rq, but the locks held will
	 * appropriately be "transferred" when switching.
	 */
	context_unsafe_alias(rq);

	raw_spin_ಬೀಗ(&p->pi_ಬೀಗ);
	rq_ಬೀಗ(rq, &rf);

	/*
	 * If we were passed a pending, then ->stop_pending was set, thus
	 * p->migration_pending must have remained stable.
	 */
	WARN_ON_ONCE(pending && pending != p->migration_pending);

	/*
	 * If task_rq(p) != rq, it cannot be migrated here, because we're
	 * holding rq->lock, if p->on_rq == 0 it cannot get enqueued because
	 * we're holding p->pi_lock.
	 */
	ಆದರೆ (task_rq(p) == rq) {
		ಆದರೆ (is_migration_disabled(p))
			ಹೋಗು ಹೊರಹೋಗು;

		ಆದರೆ (pending) {
			p->migration_pending = ಖಾಲಿ;
			complete = true;

			ಆದರೆ (cpumask_test_cpu(task_cpu(p), &p->cpus_ಮುಖವಾಡ))
				ಹೋಗು ಹೊರಹೋಗು;
		}

		ಆದರೆ (task_on_rq_queued(p)) {
			update_rq_clock(rq);
			rq = __migrate_task(rq, &rf, p, arg->dest_cpu);
		} ಇಲ್ಲದಿದ್ದರೆ {
			p->wake_cpu = arg->dest_cpu;
		}

		/*
		 * XXX __migrate_task() can fail, at which point we might end
		 * up running on a dodgy CPU, AFAICT this can only happen
		 * during CPU hotplug, at which point we'll get pushed out
		 * anyway, so it's probably not a big deal.
		 */

	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (pending) {
		/*
		 * This happens when we get migrated between migrate_enable()'s
		 * preempt_enable() and scheduling the stopper task. At that
		 * point we're a regular task again and not current anymore.
		 *
		 * A !PREEMPT kernel has a giant hole here, which makes it far
		 * more likely.
		 */

		/*
		 * The task moved before the stopper got to run. We're holding
		 * ->pi_lock, so the allowed mask is stable - if it got
		 * somewhere allowed, we're done.
		 */
		ಆದರೆ (cpumask_test_cpu(task_cpu(p), p->cpus_ptr)) {
			p->migration_pending = ಖಾಲಿ;
			complete = true;
			ಹೋಗು ಹೊರಹೋಗು;
		}

		/*
		 * When migrate_enable() hits a rq mis-match we can't reliably
		 * determine is_migration_disabled() and so have to chase after
		 * it.
		 */
		WARN_ON_ONCE(!pending->stop_pending);
		preempt_disable();
		rq_unlock(rq, &rf);
		raw_spin_unlock_irqrestore(&p->pi_ಬೀಗ, rf.flags);
		stop_one_cpu_nowait(task_cpu(p), migration_cpu_stop,
				    &pending->arg, &pending->stop_work);
		preempt_enable();
		ಹಿಂತಿರುಗಿ 0;
	}
ಹೊರಹೋಗು:
	ಆದರೆ (pending)
		pending->stop_pending = false;
	rq_unlock(rq, &rf);
	raw_spin_unlock_irqrestore(&p->pi_ಬೀಗ, rf.flags);

	ಆದರೆ (complete)
		complete_all(&pending->ಮುಗಿದ);

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ ತಳ್ಳು_cpu_stop(ಶೂನ್ಯ *arg)
{
	ರಚನೆ rq *ಅತಿಕಡಿಮೆ_rq = ಖಾಲಿ, *rq = this_rq();
	ರಚನೆ task_struct *p = arg;

	raw_spin_ಬೀಗ_irq(&p->pi_ಬೀಗ);
	raw_spin_rq_ಬೀಗ(rq);

	ಆದರೆ (task_rq(p) != rq)
		ಹೋಗು ಹೊರಹೋಗು_unlock;

	ಆದರೆ (is_migration_disabled(p)) {
		p->migration_flags |= MDF_ತಳ್ಳು;
		ಹೋಗು ಹೊರಹೋಗು_unlock;
	}

	p->migration_flags &= ~MDF_ತಳ್ಳು;

	ಆದರೆ (p->sched_class->ಕಂಡುಹಿಡಿ_ಬೀಗ_rq)
		ಅತಿಕಡಿಮೆ_rq = p->sched_class->ಕಂಡುಹಿಡಿ_ಬೀಗ_rq(p, rq);

	ಆದರೆ (!ಅತಿಕಡಿಮೆ_rq)
		ಹೋಗು ಹೊರಹೋಗು_unlock;

	lockdep_assert_rq_held(ಅತಿಕಡಿಮೆ_rq);

	// XXX validate p is still the highest prio task
	ಆದರೆ (task_rq(p) == rq) {
		move_queued_task_locked(rq, ಅತಿಕಡಿಮೆ_rq, p);
		resched_curr(ಅತಿಕಡಿಮೆ_rq);
	}

	double_unlock_balance(rq, ಅತಿಕಡಿಮೆ_rq);

ಹೊರಹೋಗು_unlock:
	rq->ತಳ್ಳು_busy = false;
	raw_spin_rq_unlock(rq);
	raw_spin_unlock_irq(&p->pi_ಬೀಗ);

	put_task_struct(p);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ mm_update_cpus_allowed(ರಚನೆ mm_struct *mm, ಸ್ಥಿರಾಂಕ cpumask_t *affmask);

/*
 * sched_class::set_cpus_allowed must do the below, but is not required to
 * actually call this function.
 */
ಶೂನ್ಯ ಗಣ_cpus_allowed_common(ರಚನೆ task_struct *p, ರಚನೆ affinity_context *ctx)
{
	ಆದರೆ (ctx->flags & (SCA_MIGRATE_ENABLE | SCA_MIGRATE_DISABLE)) {
		p->cpus_ptr = ctx->new_ಮುಖವಾಡ;
		ಹಿಂತಿರುಗಿ;
	}

	cpumask_copy(&p->cpus_ಮುಖವಾಡ, ctx->new_ಮುಖವಾಡ);
	p->nr_cpus_allowed = cpumask_ತೂಕ(ctx->new_ಮುಖವಾಡ);
	mm_update_cpus_allowed(p->mm, ctx->new_ಮುಖವಾಡ);

	/*
	 * Swap in a new user_cpus_ptr if SCA_USER flag set
	 */
	ಆದರೆ (ctx->flags & SCA_USER)
		ಅದಲುಬದಲು(p->user_cpus_ptr, ctx->user_ಮುಖವಾಡ);
}

ಸ್ಥಿರ ಶೂನ್ಯ
do_ಗಣ_cpus_allowed(ರಚನೆ task_struct *p, ರಚನೆ affinity_context *ctx)
{
	scoped_guard (sched_change, p, ಸರತಿಯಿಂದ_ತೆಗೆ_SAVE)
		p->sched_class->ಗಣ_cpus_allowed(p, ctx);
}

/*
 * Used for kthread_bind() and select_fallback_rq(), in both cases the user
 * affinity (if any) should be destroyed too.
 */
ಶೂನ್ಯ ಗಣ_cpus_allowed_force(ರಚನೆ task_struct *p, ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *new_ಮುಖವಾಡ)
{
	ರಚನೆ affinity_context ac = {
		.new_ಮುಖವಾಡ  = new_ಮುಖವಾಡ,
		.user_ಮುಖವಾಡ = ಖಾಲಿ,
		.flags     = SCA_USER,	/* clear the user requested mask */
	};
	ಒಕ್ಕೂಟ cpumask_rcuhead {
		cpumask_t cpumask;
		ರಚನೆ rcu_ತಲೆ rcu;
	};

	scoped_guard (__task_rq_lock, p)
		do_ಗಣ_cpus_allowed(p, &ac);

	/*
	 * Because this is called with p->pi_lock held, it is not possible
	 * to use kfree() here (when PREEMPT_RT=y), therefore punt to using
	 * kfree_rcu().
	 */
	kfree_rcu((ಒಕ್ಕೂಟ cpumask_rcuhead *)ac.user_ಮುಖವಾಡ, rcu);
}

ಪೂರ್ಣಾಂಕ dup_user_cpus_ptr(ರಚನೆ task_struct *dst, ರಚನೆ task_struct *src,
		      ಪೂರ್ಣಾಂಕ ಗಂಟು)
{
	cpumask_t *user_ಮುಖವಾಡ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	/*
	 * Always clear dst->user_cpus_ptr first as their user_cpus_ptr's
	 * may differ by now due to racing.
	 */
	dst->user_cpus_ptr = ಖಾಲಿ;

	/*
	 * This check is racy and losing the race is a valid situation.
	 * It is not worth the extra overhead of taking the pi_lock on
	 * every fork/clone.
	 */
	ಆದರೆ (ದತ್ತಾಂಶ_race(!src->user_cpus_ptr))
		ಹಿಂತಿರುಗಿ 0;

	user_ಮುಖವಾಡ = alloc_user_cpus_ptr(ಗಂಟು);
	ಆದರೆ (!user_ಮುಖವಾಡ)
		ಹಿಂತಿರುಗಿ -ENOMEM;

	/*
	 * Use pi_lock to protect content of user_cpus_ptr
	 *
	 * Though unlikely, user_cpus_ptr can be reset to NULL by a concurrent
	 * set_cpus_allowed_force().
	 */
	raw_spin_ಬೀಗ_irqsave(&src->pi_ಬೀಗ, flags);
	ಆದರೆ (src->user_cpus_ptr) {
		ಅದಲುಬದಲು(dst->user_cpus_ptr, user_ಮುಖವಾಡ);
		cpumask_copy(dst->user_cpus_ptr, src->user_cpus_ptr);
	}
	raw_spin_unlock_irqrestore(&src->pi_ಬೀಗ, flags);

	ಆದರೆ (unlikely(user_ಮುಖವಾಡ))
		kfree(user_ಮುಖವಾಡ);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ cpumask *clear_user_cpus_ptr(ರಚನೆ task_struct *p)
{
	ರಚನೆ cpumask *user_ಮುಖವಾಡ = ಖಾಲಿ;

	ಅದಲುಬದಲು(p->user_cpus_ptr, user_ಮುಖವಾಡ);

	ಹಿಂತಿರುಗಿ user_ಮುಖವಾಡ;
}

ಶೂನ್ಯ release_user_cpus_ptr(ರಚನೆ task_struct *p)
{
	kfree(clear_user_cpus_ptr(p));
}

/*
 * This function is wildly self concurrent; here be dragons.
 *
 *
 * When given a valid mask, __set_cpus_allowed_ptr() must block until the
 * designated task is enqueued on an allowed CPU. If that task is currently
 * running, we have to kick it out using the CPU stopper.
 *
 * Migrate-Disable comes along and tramples all over our nice sandcastle.
 * Consider:
 *
 *     Initial conditions: P0->cpus_mask = [0, 1]
 *
 *     P0@CPU0                  P1
 *
 *     migrate_disable();
 *     <preempted>
 *                              set_cpus_allowed_ptr(P0, [1]);
 *
 * P1 *cannot* return from this set_cpus_allowed_ptr() call until P0 executes
 * its outermost migrate_enable() (i.e. it exits its Migrate-Disable region).
 * This means we need the following scheme:
 *
 *     P0@CPU0                  P1
 *
 *     migrate_disable();
 *     <preempted>
 *                              set_cpus_allowed_ptr(P0, [1]);
 *                                <blocks>
 *     <resumes>
 *     migrate_enable();
 *       __set_cpus_allowed_ptr();
 *       <wakes local stopper>
 *                         `--> <woken on migration completion>
 *
 * Now the fun stuff: there may be several P1-like tasks, i.e. multiple
 * concurrent set_cpus_allowed_ptr(P0, [*]) calls. CPU affinity changes of any
 * task p are serialized by p->pi_lock, which we can leverage: the one that
 * should come into effect at the end of the Migrate-Disable region is the last
 * one. This means we only need to track a single cpumask (i.e. p->cpus_mask),
 * but we still need to properly signal those waiting tasks at the appropriate
 * moment.
 *
 * This is implemented using struct set_affinity_pending. The first
 * __set_cpus_allowed_ptr() caller within a given Migrate-Disable region will
 * setup an instance of that struct and install it on the targeted task_struct.
 * Any and all further callers will reuse that instance. Those then wait for
 * a completion signaled at the tail of the CPU stopper callback (1), triggered
 * on the end of the Migrate-Disable region (i.e. outermost migrate_enable()).
 *
 *
 * (1) In the cases covered above. There is one more where the completion is
 * signaled within affine_move_task() itself: when a subsequent affinity request
 * occurs after the stopper bailed out due to the targeted task still being
 * Migrate-Disable. Consider:
 *
 *     Initial conditions: P0->cpus_mask = [0, 1]
 *
 *     CPU0		  P1				P2
 *     <P0>
 *       migrate_disable();
 *       <preempted>
 *                        set_cpus_allowed_ptr(P0, [1]);
 *                          <blocks>
 *     <migration/0>
 *       migration_cpu_stop()
 *         is_migration_disabled()
 *           <bails>
 *                                                       set_cpus_allowed_ptr(P0, [0, 1]);
 *                                                         <signal completion>
 *                          <awakes>
 *
 * Note that the above is safe vs a concurrent migrate_enable(), as any
 * pending affinity completion is preceded by an uninstallation of
 * p->migration_pending done with p->pi_lock held.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ affine_move_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ರಚನೆ rq_flags *rf,
			    ಪೂರ್ಣಾಂಕ dest_cpu, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
	__releases(__rq_lockp(rq), &p->pi_ಬೀಗ)
{
	ರಚನೆ ಗಣ_affinity_pending my_pending = { }, *pending = ಖಾಲಿ;
	bool stop_pending, complete = false;

	/*
	 * Can the task run on the task's current CPU? If so, we're done
	 *
	 * We are also done if the task is the current donor, boosting a lock-
	 * holding proxy, (and potentially has been migrated outside its
	 * current or previous affinity mask)
	 */
	ಆದರೆ (cpumask_test_cpu(task_cpu(p), &p->cpus_ಮುಖವಾಡ) ||
	    (task_current_donor(rq, p) && !task_current(rq, p))) {
		ರಚನೆ task_struct *ತಳ್ಳು_task = ಖಾಲಿ;

		ಆದರೆ ((flags & SCA_MIGRATE_ENABLE) &&
		    (p->migration_flags & MDF_ತಳ್ಳು) && !rq->ತಳ್ಳು_busy) {
			rq->ತಳ್ಳು_busy = true;
			ತಳ್ಳು_task = get_task_struct(p);
		}

		/*
		 * If there are pending waiters, but no pending stop_work,
		 * then complete now.
		 */
		pending = p->migration_pending;
		ಆದರೆ (pending && !pending->stop_pending) {
			p->migration_pending = ಖಾಲಿ;
			complete = true;
		}

		preempt_disable();
		task_rq_unlock(rq, p, rf);
		ಆದರೆ (ತಳ್ಳು_task) {
			stop_one_cpu_nowait(rq->cpu, ತಳ್ಳು_cpu_stop,
					    p, &rq->ತಳ್ಳು_work);
		}
		preempt_enable();

		ಆದರೆ (complete)
			complete_all(&pending->ಮುಗಿದ);

		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!(flags & SCA_MIGRATE_ENABLE)) {
		/* serialized by p->pi_lock */
		ಆದರೆ (!p->migration_pending) {
			/* Install the request */
			refcount_ಗಣ(&my_pending.refs, 1);
			init_completion(&my_pending.ಮುಗಿದ);
			my_pending.arg = (ರಚನೆ migration_arg) {
				.task = p,
				.dest_cpu = dest_cpu,
				.pending = &my_pending,
			};

			p->migration_pending = &my_pending;
		} ಇಲ್ಲದಿದ್ದರೆ {
			pending = p->migration_pending;
			refcount_inc(&pending->refs);
			/*
			 * Affinity has changed, but we've already installed a
			 * pending. migration_cpu_stop() *must* see this, else
			 * we risk a completion of the pending despite having a
			 * task on a disallowed CPU.
			 *
			 * Serialized by p->pi_lock, so this is safe.
			 */
			pending->arg.dest_cpu = dest_cpu;
		}
	}
	pending = p->migration_pending;
	/*
	 * - !MIGRATE_ENABLE:
	 *   we'll have installed a pending if there wasn't one already.
	 *
	 * - MIGRATE_ENABLE:
	 *   we're here because the current CPU isn't matching anymore,
	 *   the only way that can happen is because of a concurrent
	 *   set_cpus_allowed_ptr() call, which should then still be
	 *   pending completion.
	 *
	 * Either way, we really should have a @pending here.
	 */
	ಆದರೆ (WARN_ON_ONCE(!pending)) {
		task_rq_unlock(rq, p, rf);
		ಹಿಂತಿರುಗಿ -EINVAL;
	}

	ಆದರೆ (task_on_cpu(rq, p) || READ_ONCE(p->__state) == TASK_WAKING) {
		/*
		 * MIGRATE_ENABLE gets here because 'p == current', but for
		 * anything else we cannot do is_migration_disabled(), punt
		 * and have the stopper function handle it all race-free.
		 */
		stop_pending = pending->stop_pending;
		ಆದರೆ (!stop_pending)
			pending->stop_pending = true;

		ಆದರೆ (flags & SCA_MIGRATE_ENABLE)
			p->migration_flags &= ~MDF_ತಳ್ಳು;

		preempt_disable();
		task_rq_unlock(rq, p, rf);
		ಆದರೆ (!stop_pending) {
			stop_one_cpu_nowait(cpu_ದ(rq), migration_cpu_stop,
					    &pending->arg, &pending->stop_work);
		}
		preempt_enable();

		ಆದರೆ (flags & SCA_MIGRATE_ENABLE)
			ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ {

		ಆದರೆ (!is_migration_disabled(p)) {
			ಆದರೆ (task_on_rq_queued(p))
				rq = move_queued_task(rq, rf, p, dest_cpu);

			ಆದರೆ (!pending->stop_pending) {
				p->migration_pending = ಖಾಲಿ;
				complete = true;
			}
		}
		task_rq_unlock(rq, p, rf);

		ಆದರೆ (complete)
			complete_all(&pending->ಮುಗಿದ);
	}

	wait_for_completion(&pending->ಮುಗಿದ);

	ಆದರೆ (refcount_dec_and_test(&pending->refs))
		wake_ಮೇಲೆ_var(&pending->refs); /* No UaF, just an address */

	/*
	 * Block the original owner of &pending until all subsequent callers
	 * have seen the completion and decremented the refcount
	 */
	wait_var_event(&my_pending.refs, !refcount_read(&my_pending.refs));

	/* ARGH */
	WARN_ON_ONCE(my_pending.stop_pending);

	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Called with both p->pi_lock and rq->lock held; drops both before returning.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __set_cpus_allowed_ptr_locked(ರಚನೆ task_struct *p,
					 ರಚನೆ affinity_context *ctx,
					 ರಚನೆ rq *rq,
					 ರಚನೆ rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_ಬೀಗ)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_allowed_ಮುಖವಾಡ = task_cpu_possible_ಮುಖವಾಡ(p);
	ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_valid_ಮುಖವಾಡ = cpu_ಸಕ್ರಿಯ_ಮುಖವಾಡ;
	bool kthread = p->flags & PF_KTHREAD;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ dest_cpu;
	ಪೂರ್ಣಾಂಕ ret = 0;

	ಆದರೆ (kthread || is_migration_disabled(p)) {
		/*
		 * Kernel threads are allowed on online && !active CPUs,
		 * however, during cpu-hot-unplug, even these might get pushed
		 * away if not KTHREAD_IS_PER_CPU.
		 *
		 * Specifically, migration_disabled() tasks must not fail the
		 * cpumask_any_and_distribute() pick below, esp. so on
		 * SCA_MIGRATE_ENABLE, otherwise we'll not call
		 * set_cpus_allowed_common() and actually reset p->cpus_ptr.
		 */
		cpu_valid_ಮುಖವಾಡ = cpu_online_ಮುಖವಾಡ;
	}

	ಆದರೆ (!kthread && !cpumask_subset(ctx->new_ಮುಖವಾಡ, cpu_allowed_ಮುಖವಾಡ)) {
		ret = -EINVAL;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/*
	 * Must re-check here, to close a race against __kthread_bind(),
	 * sched_setaffinity() is not guaranteed to observe the flag.
	 */
	ಆದರೆ ((ctx->flags & SCA_CHECK) && (p->flags & PF_NO_SETAFFINITY)) {
		ret = -EINVAL;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (!(ctx->flags & SCA_MIGRATE_ENABLE)) {
		ಆದರೆ (cpumask_equal(&p->cpus_ಮುಖವಾಡ, ctx->new_ಮುಖವಾಡ)) {
			ಆದರೆ (ctx->flags & SCA_USER)
				ಅದಲುಬದಲು(p->user_cpus_ptr, ctx->user_ಮುಖವಾಡ);
			ಹೋಗು ಹೊರಹೋಗು;
		}

		ಆದರೆ (WARN_ON_ONCE(p == current &&
				 is_migration_disabled(p) &&
				 !cpumask_test_cpu(task_cpu(p), ctx->new_ಮುಖವಾಡ))) {
			ret = -EBUSY;
			ಹೋಗು ಹೊರಹೋಗು;
		}
	}

	/*
	 * Picking a ~random cpu helps in cases where we are changing affinity
	 * for groups of tasks (ie. cpuset), so that load balancing is not
	 * immediately required to distribute the tasks within their new mask.
	 */
	dest_cpu = cpumask_any_and_distribute(cpu_valid_ಮುಖವಾಡ, ctx->new_ಮುಖವಾಡ);
	ಆದರೆ (dest_cpu >= nr_cpu_ids) {
		ret = -EINVAL;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	do_ಗಣ_cpus_allowed(p, ctx);

	ಹಿಂತಿರುಗಿ affine_move_task(rq, p, rf, dest_cpu, ctx->flags);

ಹೊರಹೋಗು:
	task_rq_unlock(rq, p, rf);

	ಹಿಂತಿರುಗಿ ret;
}

/*
 * Change a given task's CPU affinity. Migrate the thread to a
 * proper CPU and schedule it away if the CPU it's executing on
 * is removed from the allowed bitmask.
 *
 * NOTE: the caller must have a valid reference to the task, the
 * task must not exit() & deallocate itself prematurely. The
 * call is not atomic; no spinlocks may be held.
 */
ಪೂರ್ಣಾಂಕ __set_cpus_allowed_ptr(ರಚನೆ task_struct *p, ರಚನೆ affinity_context *ctx)
{
	ರಚನೆ rq_flags rf;
	ರಚನೆ rq *rq;

	rq = task_rq_ಬೀಗ_2(p, &rf);
	/*
	 * Masking should be skipped if SCA_USER or any of the SCA_MIGRATE_*
	 * flags are set.
	 */
	ಆದರೆ (p->user_cpus_ptr &&
	    !(ctx->flags & (SCA_USER | SCA_MIGRATE_ENABLE | SCA_MIGRATE_DISABLE)) &&
	    cpumask_and(rq->scratch_ಮುಖವಾಡ, ctx->new_ಮುಖವಾಡ, p->user_cpus_ptr))
		ctx->new_ಮುಖವಾಡ = rq->scratch_ಮುಖವಾಡ;

	ಹಿಂತಿರುಗಿ __set_cpus_allowed_ptr_locked(p, ctx, rq, &rf);
}

ಪೂರ್ಣಾಂಕ ಗಣ_cpus_allowed_ptr(ರಚನೆ task_struct *p, ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *new_ಮುಖವಾಡ)
{
	ರಚನೆ affinity_context ac = {
		.new_ಮುಖವಾಡ  = new_ಮುಖವಾಡ,
		.flags     = 0,
	};

	ಹಿಂತಿರುಗಿ __set_cpus_allowed_ptr(p, &ac);
}
EXPORT_ಚಿಹ್ನೆ_GPL(ಗಣ_cpus_allowed_ptr);

/*
 * Change a given task's CPU affinity to the intersection of its current
 * affinity mask and @subset_mask, writing the resulting mask to @new_mask.
 * If user_cpus_ptr is defined, use it as the basis for restricting CPU
 * affinity or use cpu_online_mask instead.
 *
 * If the resulting mask is empty, leave the affinity unchanged and return
 * -EINVAL.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ restrict_cpus_allowed_ptr(ರಚನೆ task_struct *p,
				     ರಚನೆ cpumask *new_ಮುಖವಾಡ,
				     ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *subset_ಮುಖವಾಡ)
{
	ರಚನೆ affinity_context ac = {
		.new_ಮುಖವಾಡ  = new_ಮುಖವಾಡ,
		.flags     = 0,
	};
	ರಚನೆ rq_flags rf;
	ರಚನೆ rq *rq;
	ಪೂರ್ಣಾಂಕ err;

	rq = task_rq_ಬೀಗ_2(p, &rf);

	/*
	 * Forcefully restricting the affinity of a deadline task is
	 * likely to cause problems, so fail and noisily override the
	 * mask entirely.
	 */
	ಆದರೆ (task_has_dl_policy(p) && dl_bandwidth_enabled()) {
		err = -EPERM;
		ಹೋಗು err_unlock;
	}

	ಆದರೆ (!cpumask_and(new_ಮುಖವಾಡ, task_user_cpus(p), subset_ಮುಖವಾಡ)) {
		err = -EINVAL;
		ಹೋಗು err_unlock;
	}

	ಹಿಂತಿರುಗಿ __set_cpus_allowed_ptr_locked(p, &ac, rq, &rf);

err_unlock:
	task_rq_unlock(rq, p, &rf);
	ಹಿಂತಿರುಗಿ err;
}

/*
 * Restrict the CPU affinity of task @p so that it is a subset of
 * task_cpu_possible_mask() and point @p->user_cpus_ptr to a copy of the
 * old affinity mask. If the resulting mask is empty, we warn and walk
 * up the cpuset hierarchy until we find a suitable mask.
 */
ಶೂನ್ಯ force_compatible_cpus_allowed_ptr(ರಚನೆ task_struct *p)
{
	cpumask_var_t new_ಮುಖವಾಡ;
	ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *override_ಮುಖವಾಡ = task_cpu_possible_ಮುಖವಾಡ(p);

	alloc_cpumask_var(&new_ಮುಖವಾಡ, GFP_KERNEL);

	/*
	 * __migrate_task() can fail silently in the face of concurrent
	 * offlining of the chosen destination CPU, so take the hotplug
	 * lock to ensure that the migration succeeds.
	 */
	cpus_read_ಬೀಗ();
	ಆದರೆ (!cpumask_ಲಭ್ಯ(new_ಮುಖವಾಡ))
		ಹೋಗು ಹೊರಹೋಗು_ಗಣ_ಮುಖವಾಡ;

	ಆದರೆ (!restrict_cpus_allowed_ptr(p, new_ಮುಖವಾಡ, override_ಮುಖವಾಡ))
		ಹೋಗು ಹೊರಹೋಗು_free_ಮುಖವಾಡ;

	/*
	 * We failed to find a valid subset of the affinity mask for the
	 * task, so override it based on its cpuset hierarchy.
	 */
	cpuset_cpus_allowed(p, new_ಮುಖವಾಡ);
	override_ಮುಖವಾಡ = new_ಮುಖವಾಡ;

ಹೊರಹೋಗು_ಗಣ_ಮುಖವಾಡ:
	ಆದರೆ (printk_ratelimit()) {
		printk_deferred("Overriding affinity for process %d (%s) to CPUs %*pbl\n",
				task_pid_nr(p), p->comm,
				cpumask_pr_ವಾದಗಳು(override_ಮುಖವಾಡ));
	}

	WARN_ON(ಗಣ_cpus_allowed_ptr(p, override_ಮುಖವಾಡ));
ಹೊರಹೋಗು_free_ಮುಖವಾಡ:
	cpus_read_unlock();
	free_cpumask_var(new_ಮುಖವಾಡ);
}

/*
 * Restore the affinity of a task @p which was previously restricted by a
 * call to force_compatible_cpus_allowed_ptr().
 *
 * It is the caller's responsibility to serialise this with any calls to
 * force_compatible_cpus_allowed_ptr(@p).
 */
ಶೂನ್ಯ relax_compatible_cpus_allowed_ptr(ರಚನೆ task_struct *p)
{
	ರಚನೆ affinity_context ac = {
		.new_ಮುಖವಾಡ  = task_user_cpus(p),
		.flags     = 0,
	};
	ಪೂರ್ಣಾಂಕ ret;

	/*
	 * Try to restore the old affinity mask with __sched_setaffinity().
	 * Cpuset masking will be done there too.
	 */
	ret = __sched_setaffinity(p, &ac);
	WARN_ON_ONCE(ret);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SMP

ಶೂನ್ಯ ಗಣ_task_cpu(ರಚನೆ task_struct *p, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ new_cpu)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ state = READ_ONCE(p->__state);
	bool proxy_migrated = sched_proxy_exec() && p->is_blocked &&
			      task_cpu(p) != p->wake_cpu;

	/*
	 * We should never call set_task_cpu() on a blocked task,
	 * ttwu() will sort out the placement.
	 */
	WARN_ON_ONCE(state != TASK_RUNNING && state != TASK_WAKING && !p->on_rq);

	/*
	 * Migrating fair class task must have p->on_rq = TASK_ON_RQ_MIGRATING,
	 * because schedstat_wait_{start,end} rebase migrating task's wait_start
	 * time relying on p->on_rq.
	 */
	WARN_ON_ONCE(state == TASK_RUNNING &&
		     p->sched_class == &fair_sched_class &&
		     (p->on_rq && !task_on_rq_migrating(p)));

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_LOCKDEP
	/*
	 * The caller should hold either p->pi_lock or rq->lock, when changing
	 * a task's CPU. ->pi_lock for waking tasks, rq->lock for runnable tasks.
	 *
	 * sched_move_task() holds both and thus holding either pins the cgroup,
	 * see task_group().
	 *
	 * Furthermore, all task_rq users should acquire both locks, see
	 * task_rq_lock().
	 */
	WARN_ON_ONCE(debug_locks && !(lockdep_is_held(&p->pi_ಬೀಗ) ||
				      lockdep_is_held(__rq_lockp(task_rq(p)))));
#ಆದರೆ_ಅಂತ್ಯ
	/*
	 * Clearly, migrating tasks to offline CPUs is a fairly daft thing.
	 */
	WARN_ON_ONCE(!cpu_online(new_cpu));

	/*
	 * Proxy execution can move a blocked task's scheduling context to any
	 * CPU without moving its migration-disabled execution context. The
	 * wakeup path will return the task to a CPU where it can execute.
	 */
	WARN_ON_ONCE(is_migration_disabled(p) && !proxy_migrated);

	trace_sched_migrate_task(p, new_cpu);

	ಆದರೆ (task_cpu(p) != new_cpu) {
		ಆದರೆ (p->sched_class->migrate_task_rq)
			p->sched_class->migrate_task_rq(p, new_cpu);
		p->se.nr_migrations++;
		perf_event_task_migrate(p);
	}

	__set_task_cpu(p, new_cpu);
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SMP */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
ಸ್ಥಿರ ಶೂನ್ಯ __migrate_swap_task(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu)
{
	ಆದರೆ (task_on_rq_queued(p)) {
		ರಚನೆ rq *src_rq, *dst_rq;
		ರಚನೆ rq_flags srf, drf;

		src_rq = task_rq(p);
		dst_rq = cpu_rq(cpu);

		rq_pin_ಬೀಗ(src_rq, &srf);
		rq_pin_ಬೀಗ(dst_rq, &drf);

		move_queued_task_locked(src_rq, dst_rq, p);
		wakeup_preempt(dst_rq, p, 0);

		rq_unpin_ಬೀಗ(dst_rq, &drf);
		rq_unpin_ಬೀಗ(src_rq, &srf);

	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * Task isn't running anymore; make it appear like we migrated
		 * it before it went to sleep. This means on wakeup we make the
		 * previous CPU our target instead of where it really is.
		 */
		p->wake_cpu = cpu;
	}
}

ರಚನೆ migration_ಅದಲುಬದಲು_arg {
	ರಚನೆ task_struct *src_task, *dst_task;
	ಪೂರ್ಣಾಂಕ src_cpu, dst_cpu;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ migrate_ಅದಲುಬದಲು_stop(ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ migration_ಅದಲುಬದಲು_arg *arg = ದತ್ತಾಂಶ;
	ರಚನೆ rq *src_rq, *dst_rq;

	ಆದರೆ (!cpu_ಸಕ್ರಿಯ(arg->src_cpu) || !cpu_ಸಕ್ರಿಯ(arg->dst_cpu))
		ಹಿಂತಿರುಗಿ -EAGAIN;

	src_rq = cpu_rq(arg->src_cpu);
	dst_rq = cpu_rq(arg->dst_cpu);

	guard(double_raw_spinlock)(&arg->src_task->pi_ಬೀಗ, &arg->dst_task->pi_ಬೀಗ);
	guard(double_rq_ಬೀಗ)(src_rq, dst_rq);

	ಆದರೆ (task_cpu(arg->dst_task) != arg->dst_cpu)
		ಹಿಂತಿರುಗಿ -EAGAIN;

	ಆದರೆ (task_cpu(arg->src_task) != arg->src_cpu)
		ಹಿಂತಿರುಗಿ -EAGAIN;

	ಆದರೆ (!cpumask_test_cpu(arg->dst_cpu, arg->src_task->cpus_ptr))
		ಹಿಂತಿರುಗಿ -EAGAIN;

	ಆದರೆ (!cpumask_test_cpu(arg->src_cpu, arg->dst_task->cpus_ptr))
		ಹಿಂತಿರುಗಿ -EAGAIN;

	__migrate_swap_task(arg->src_task, arg->dst_cpu);
	__migrate_swap_task(arg->dst_task, arg->src_cpu);

	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Cross migrate two tasks
 */
ಪೂರ್ಣಾಂಕ migrate_ಅದಲುಬದಲು(ರಚನೆ task_struct *cur, ರಚನೆ task_struct *p,
		ಪೂರ್ಣಾಂಕ ಗುರಿ_cpu, ಪೂರ್ಣಾಂಕ curr_cpu)
{
	ರಚನೆ migration_ಅದಲುಬದಲು_arg arg;
	ಪೂರ್ಣಾಂಕ ret = -EINVAL;

	arg = (ರಚನೆ migration_ಅದಲುಬದಲು_arg){
		.src_task = cur,
		.src_cpu = curr_cpu,
		.dst_task = p,
		.dst_cpu = ಗುರಿ_cpu,
	};

	ಆದರೆ (arg.src_cpu == arg.dst_cpu)
		ಹೋಗು ಹೊರಹೋಗು;

	/*
	 * These three tests are all lockless; this is OK since all of them
	 * will be re-checked with proper locks held further down the line.
	 */
	ಆದರೆ (!cpu_ಸಕ್ರಿಯ(arg.src_cpu) || !cpu_ಸಕ್ರಿಯ(arg.dst_cpu))
		ಹೋಗು ಹೊರಹೋಗು;

	ಆದರೆ (!cpumask_test_cpu(arg.dst_cpu, arg.src_task->cpus_ptr))
		ಹೋಗು ಹೊರಹೋಗು;

	ಆದರೆ (!cpumask_test_cpu(arg.src_cpu, arg.dst_task->cpus_ptr))
		ಹೋಗು ಹೊರಹೋಗು;

	trace_sched_ಅದಲುಬದಲು_numa(cur, arg.src_cpu, p, arg.dst_cpu);
	ret = stop_two_cpus(arg.dst_cpu, arg.src_cpu, migrate_ಅದಲುಬದಲು_stop, &arg);

ಹೊರಹೋಗು:
	ಹಿಂತಿರುಗಿ ret;
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_NUMA_BALANCING */

/***
 * kick_process - kick a running thread to enter/exit the kernel
 * @p: the to-be-kicked thread
 *
 * Cause a process which is running on another CPU to enter
 * kernel-mode, without any delay. (to get signals handled.)
 *
 * NOTE: this function doesn't have to take the runqueue lock,
 * because all it wants to ensure is that the remote task enters
 * the kernel. If the IPI races and the task has been migrated
 * to another CPU then no harm is done and the purpose has been
 * achieved as well.
 */
ಶೂನ್ಯ kick_process(ರಚನೆ task_struct *p)
{
	guard(preempt)();
	ಪೂರ್ಣಾಂಕ cpu = task_cpu(p);

	ಆದರೆ ((cpu != smp_processor_id()) && task_curr(p))
		smp_send_reschedule(cpu);
}
EXPORT_ಚಿಹ್ನೆ_GPL(kick_process);

/*
 * ->cpus_ptr is protected by both rq->lock and p->pi_lock
 *
 * A few notes on cpu_active vs cpu_online:
 *
 *  - cpu_active must be a subset of cpu_online
 *
 *  - on CPU-up we allow per-CPU kthreads on the online && !active CPU,
 *    see __set_cpus_allowed_ptr(). At this point the newly online
 *    CPU isn't yet part of the sched domains, and balancing will not
 *    see it.
 *
 *  - on CPU-down we clear cpu_active() to mask the sched domains and
 *    avoid the load balancer to place new tasks on the to be removed
 *    CPU. Existing tasks will remain running there and will be taken
 *    off.
 *
 * This means that fallback selection must not select !active CPUs.
 * And can assume that any active CPU must be online. Conversely
 * select_task_rq() below may allow selection of !active CPUs in order
 * to satisfy the above rules.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ select_fallback_rq(ಪೂರ್ಣಾಂಕ cpu, ರಚನೆ task_struct *p)
{
	ಪೂರ್ಣಾಂಕ nid = cpu_ಗಮ್ಯ_ಗಂಟು(cpu);
	ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *nodemask = ಖಾಲಿ;
	ಎಣಿಕೆ { cpuset, possible, fail } state = cpuset;
	ಪೂರ್ಣಾಂಕ dest_cpu;

	/*
	 * If the node that the CPU is on has been offlined, cpu_to_node()
	 * will return -1. There is no CPU on the node, and we should
	 * select the CPU on the other node.
	 */
	ಆದರೆ (nid != -1) {
		nodemask = cpumask_ದ_ಗಂಟು(nid);

		/* Look for allowed, online CPU in same node. */
		for_each_cpu(dest_cpu, nodemask) {
			ಆದರೆ (is_cpu_allowed(p, dest_cpu))
				ಹಿಂತಿರುಗಿ dest_cpu;
		}
	}

	ಗಾಗಿ (;;) {
		/* Any allowed, online CPU? */
		for_each_cpu(dest_cpu, p->cpus_ptr) {
			ಆದರೆ (!is_cpu_allowed(p, dest_cpu))
				ಮುಂದುವರಿ;

			ಹೋಗು ಹೊರಹೋಗು;
		}

		/* No more Mr. Nice Guy. */
		ಆಯ್ಕೆ (state) {
		ಸ್ಥಿತಿ cpuset:
			ಆದರೆ (cpuset_cpus_allowed_fallback(p)) {
				state = possible;
				ನಿಲ್ಲಿಸು;
			}
			fallthrough;
		ಸ್ಥಿತಿ possible:
			ಗಣ_cpus_allowed_force(p, task_cpu_fallback_ಮುಖವಾಡ(p));
			state = fail;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ fail:
			BUG();
			ನಿಲ್ಲಿಸು;
		}
	}

ಹೊರಹೋಗು:
	ಆದರೆ (state != cpuset) {
		/*
		 * Don't tell them about moving exiting tasks or
		 * kernel threads (both mm NULL), since they never
		 * leave kernel.
		 */
		ಆದರೆ (p->mm && printk_ratelimit()) {
			printk_deferred("process %d (%s) no longer affine to cpu%d\n",
					task_pid_nr(p), p->comm, cpu);
		}
	}

	ಹಿಂತಿರುಗಿ dest_cpu;
}

/*
 * The caller (fork, wakeup) owns p->pi_lock, ->cpus_ptr is stable.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಪೂರ್ಣಾಂಕ select_task_rq(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu, ಪೂರ್ಣಾಂಕ *wake_flags)
{
	lockdep_assert_held(&p->pi_ಬೀಗ);

	ಆದರೆ (p->nr_cpus_allowed > 1 && !is_migration_disabled(p)) {
		cpu = p->sched_class->select_task_rq(p, cpu, *wake_flags);
		*wake_flags |= WF_RQ_SELECTED;
	} ಇಲ್ಲದಿದ್ದರೆ {
		cpu = cpumask_any(p->cpus_ptr);
	}

	/*
	 * In order not to call set_task_cpu() on a blocking task we need
	 * to rely on ttwu() to place the task on a valid ->cpus_ptr
	 * CPU.
	 *
	 * Since this is common to all placement strategies, this lives here.
	 *
	 * [ this allows ->select_task() to simply return task_cpu(p) and
	 *   not worry about this generic constraint ]
	 */
	ಆದರೆ (unlikely(!is_cpu_allowed(p, cpu)))
		cpu = select_fallback_rq(task_cpu(p), p);

	ಹಿಂತಿರುಗಿ cpu;
}

ಶೂನ್ಯ sched_ಗಣ_stop_task(ಪೂರ್ಣಾಂಕ cpu, ರಚನೆ task_struct *stop)
{
	ಸ್ಥಿರ ರಚನೆ ಬೀಗ_class_ಕೀಲಿ stop_pi_ಬೀಗ;
	ರಚನೆ sched_param param = { .sched_ಆದ್ಯತೆ = MAX_RT_PRIO - 1 };
	ರಚನೆ task_struct *old_stop = cpu_rq(cpu)->stop;

	ಆದರೆ (stop) {
		/*
		 * Make it appear like a SCHED_FIFO task, its something
		 * userspace knows about and won't get confused about.
		 *
		 * Also, it will make PI more or less work without too
		 * much confusion -- but then, stop work should not
		 * rely on PI working anyway.
		 */
		sched_setscheduler_nocheck(stop, SCHED_FIFO, &param);

		stop->sched_class = &stop_sched_class;

		/*
		 * The PI code calls rt_mutex_setprio() with ->pi_lock held to
		 * adjust the effective priority of a task. As a result,
		 * rt_mutex_setprio() can trigger (RT) balancing operations,
		 * which can then trigger wakeups of the stop thread to push
		 * around the current task.
		 *
		 * The stop task itself will never be part of the PI-chain, it
		 * never blocks, therefore that ->pi_lock recursion is safe.
		 * Tell lockdep about this by placing the stop->pi_lock in its
		 * own class.
		 */
		lockdep_ಗಣ_class(&stop->pi_ಬೀಗ, &stop_pi_ಬೀಗ);
	}

	cpu_rq(cpu)->stop = stop;

	ಆದರೆ (old_stop) {
		/*
		 * Reset it back to a normal scheduling class so that
		 * it can die in pieces.
		 */
		old_stop->sched_class = &rt_sched_class;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ
ttwu_stat(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu, ಪೂರ್ಣಾಂಕ wake_flags)
{
	ರಚನೆ rq *rq;

	ಆದರೆ (!schedstat_enabled())
		ಹಿಂತಿರುಗಿ;

	rq = this_rq();

	ಆದರೆ (cpu == rq->cpu) {
		__schedstat_inc(rq->ttwu_local);
		__schedstat_inc(p->stats.nr_wakeups_local);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ sched_domain *sd;

		__schedstat_inc(p->stats.nr_wakeups_remote);

		guard(rcu)();
		for_each_domain(rq->cpu, sd) {
			ಆದರೆ (cpumask_test_cpu(cpu, sched_domain_ಹರವು(sd))) {
				__schedstat_inc(sd->ttwu_wake_remote);
				ನಿಲ್ಲಿಸು;
			}
		}
	}

	ಆದರೆ (wake_flags & WF_MIGRATED)
		__schedstat_inc(p->stats.nr_wakeups_migrate);

	__schedstat_inc(rq->ttwu_ಎಣಿಕೆ);
	__schedstat_inc(p->stats.nr_wakeups);

	ಆದರೆ (wake_flags & WF_SYNC)
		__schedstat_inc(p->stats.nr_wakeups_sync);
}

/*
 * Mark the task runnable.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ttwu_do_wakeup(ರಚನೆ task_struct *p)
{
	p->is_blocked = 0;
	WRITE_ONCE(p->__state, TASK_RUNNING);
	trace_sched_wakeup(p);
}

ಶೂನ್ಯ update_rq_avg_idle(ರಚನೆ rq *rq)
{
	u64 idle_stamp = rq->idle_stamp;
	u64 delta, max;

	ಆದರೆ (!idle_stamp)
		ಹಿಂತಿರುಗಿ;

	delta = rq_clock(rq) - idle_stamp;

	update_avg(&rq->avg_idle, delta);

	max = 2 * rq->max_idle_balance_ವೆಚ್ಚ;
	ಆದರೆ (rq->avg_idle > max)
		rq->avg_idle = max;
	rq->idle_stamp = 0;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_PROXY_EXEC
ಸ್ಥಿರ ಶೂನ್ಯ zap_balance_callbacks(ರಚನೆ rq *rq);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ proxy_reset_donor(ರಚನೆ rq *rq)
{
	WARN_ON_ONCE(rq->donor == rq->curr);

	put_prev_ಗಣ_ಮುಂದಿನ_task(rq, rq->donor, rq->curr);
	rq->ಮುಂದಿನ_class = rq->curr->sched_class;
	rq_ಗಣ_donor(rq, rq->curr);
	zap_balance_callbacks(rq);
	resched_curr(rq);
}

/*
 * Checks to see if task p has been proxy-migrated to another rq
 * and needs to be returned. If so, we deactivate the task here
 * so that it can be properly woken up on the p->wake_cpu
 * (or whichever cpu select_task_rq() picks at the bottom of
 * try_to_wake_up()
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool proxy_needs_return(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	bool dequeued;

	/*
	 * Typically per __set_task_cpu(), task_cpu(p) == p->wake_cpu.
	 *
	 * However, proxy_set_task_cpu() is such that it preserves the
	 * original cpu in p->wake_cpu while migrating p for proxy reasons
	 * (possibly outside of the allowed p->cpus_ptr).
	 *
	 * Furthermore, migration_cpu_stop() / __migrate_swap_task(), will
	 * only set p->wake_cpu when !p->on_rq, and since here p->on_rq, this
	 * will not apply. But if it did, this check is the safe way around
	 * and would migrate.
	 */
	ಆದರೆ (task_cpu(p) == p->wake_cpu)
		ಹಿಂತಿರುಗಿ false;

	scoped_guard(raw_spinlock, &p->blocked_ಬೀಗ) {
		/* Task is waking up; clear any blocked_on relationship */
		__clear_task_blocked_on(p, ಖಾಲಿ);

		/* If already current, don't need to return migrate */
		ಆದರೆ (task_current(rq, p))
			ಹಿಂತಿರುಗಿ false;
	}

	dequeued = ಸರತಿಯಿಂದ_ತೆಗೆ_ಖಂಡ_task(rq, p, TASK_WAKING);

	/*
	 * Dequeue @p from its scheduling class before resetting rq->donor.
	 * In particular, sched_ext needs to end the donor's running session
	 * and clear SCX_TASK_QUEUED before put_prev_task_scx() is called by
	 * proxy_reset_donor(); otherwise it would reenqueue the blocked donor.
	 *
	 * Keep on_rq set until all donor references have been replaced.
	 */
	ಆದರೆ (task_current_donor(rq, p))
		proxy_reset_donor(rq);

	ಆದರೆ (dequeued)
		__block_task(rq, p);
	ಹಿಂತಿರುಗಿ true;
}
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_SCHED_PROXY_EXEC */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool proxy_needs_return(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ false;
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SCHED_PROXY_EXEC */

ಸ್ಥಿರ ಶೂನ್ಯ
ttwu_do_activate(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ wake_flags,
		 ರಚನೆ rq_flags *rf)
{
	ಪೂರ್ಣಾಂಕ en_flags = ಸರತಿಗೆ_ಸೇರಿಸು_WAKEUP | ಸರತಿಗೆ_ಸೇರಿಸು_NOCLOCK;

	lockdep_assert_rq_held(rq);

	ಆದರೆ (p->sched_contributes_ಗಮ್ಯ_ತುಂಬು)
		rq->nr_uninterruptible--;

	ಆದರೆ (wake_flags & WF_RQ_SELECTED)
		en_flags |= ಸರತಿಗೆ_ಸೇರಿಸು_RQ_SELECTED;
	ಆದರೆ (wake_flags & WF_MIGRATED)
		en_flags |= ಸರತಿಗೆ_ಸೇರಿಸು_MIGRATED;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (p->ಒಳ_iowait) {
		delayacct_blkio_ಕೊನೆ(p);
		atomic_dec(&task_rq(p)->nr_iowait);
	}

	activate_task(rq, p, en_flags);
	wakeup_preempt(rq, p, wake_flags);

	ttwu_do_wakeup(p);

	ಆದರೆ (p->sched_class->task_woken) {
		/*
		 * Our task @p is fully woken up and running; so it's safe to
		 * drop the rq->lock, hereafter rq is only used for statistics.
		 */
		rq_unpin_ಬೀಗ(rq, rf);
		p->sched_class->task_woken(rq, p);
		rq_repin_ಬೀಗ(rq, rf);
	}
}

/*
 * Consider @p being inside a wait loop:
 *
 *   for (;;) {
 *      set_current_state(TASK_UNINTERRUPTIBLE);
 *
 *      if (CONDITION)
 *         break;
 *
 *      schedule();
 *   }
 *   __set_current_state(TASK_RUNNING);
 *
 * between set_current_state() and schedule(). In this case @p is still
 * runnable, so all that needs doing is change p->state back to TASK_RUNNING in
 * an atomic manner.
 *
 * By taking task_rq(p)->lock we serialize against schedule(), if @p->on_rq
 * then schedule() must still happen and p->state can be changed to
 * TASK_RUNNING. Otherwise we lost the race, schedule() has happened, and we
 * need to do a full wakeup with enqueue.
 *
 * Returns: %true when the wakeup is done,
 *          %false otherwise.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ttwu_runnable(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ wake_flags)
{
	ACQUIRE(__task_rq_lock, guard)(p);
	ರಚನೆ rq *rq = guard.rq;

	ಆದರೆ (!task_on_rq_queued(p))
		ಹಿಂತಿರುಗಿ 0;

	update_rq_clock(rq);
	ಆದರೆ (p->is_blocked) {
		ಆದರೆ (p->se.sched_delayed)
			ಸರತಿಗೆ_ಸೇರಿಸು_task(rq, p, ಸರತಿಗೆ_ಸೇರಿಸು_NOCLOCK | ಸರತಿಗೆ_ಸೇರಿಸು_DELAYED);
		ಆದರೆ (proxy_needs_return(rq, p))
			ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!task_on_cpu(rq, p)) {
		/*
		 * When on_rq && !on_cpu the task is preempted, see if
		 * it should preempt the task that is current now.
		 */
		wakeup_preempt(rq, p, wake_flags | WF_TTWU_RQ);
	}
	ttwu_do_wakeup(p);
	ಹಿಂತಿರುಗಿ 1;
}

ಶೂನ್ಯ sched_ttwu_pending(ಶೂನ್ಯ *arg)
{
	ರಚನೆ llist_ಗಂಟು *llist = arg;
	ರಚನೆ rq *rq = this_rq();
	ರಚನೆ task_struct *p, *t;
	ರಚನೆ rq_flags rf;

	ಆದರೆ (!llist)
		ಹಿಂತಿರುಗಿ;

	rq_ಬೀಗ_irqsave(rq, &rf);
	update_rq_clock(rq);

	llist_for_each_ನಮೂದು_ಸುರಕ್ಷಿತ(p, t, llist, wake_ನಮೂದು.llist) {
		ಆದರೆ (WARN_ON_ONCE(p->on_cpu))
			smp_cond_ತುಂಬು_acquire(&p->on_cpu, !VAL);

		ಆದರೆ (WARN_ON_ONCE(task_cpu(p) != cpu_ದ(rq)))
			ಗಣ_task_cpu(p, cpu_ದ(rq));

		ttwu_do_activate(rq, p, p->sched_remote_wakeup ? WF_MIGRATED : 0, &rf);
	}

	/*
	 * Must be after enqueueing at least once task such that
	 * idle_cpu() does not observe a false-negative -- if it does,
	 * it is possible for select_idle_siblings() to stack a number
	 * of tasks on this CPU during that window.
	 *
	 * It is OK to clear ttwu_pending when another task pending.
	 * We will receive IPI after local IRQ enabled and then enqueue it.
	 * Since now nr_running > 0, idle_cpu() will always get correct result.
	 */
	WRITE_ONCE(rq->ttwu_pending, 0);
	rq_unlock_irqrestore(rq, &rf);
}

/*
 * Prepare the scene for sending an IPI for a remote smp_call
 *
 * Returns true if the caller can proceed with sending the IPI.
 * Returns false otherwise.
 */
bool call_function_single_prep_ipi(ಪೂರ್ಣಾಂಕ cpu)
{
	ಆದರೆ (ಗಣ_nr_if_polling(cpu_rq(cpu)->idle)) {
		trace_sched_wake_idle_without_ipi(cpu);
		ಹಿಂತಿರುಗಿ false;
	}

	ಹಿಂತಿರುಗಿ true;
}

/*
 * Queue a task on the target CPUs wake_list and wake the CPU via IPI if
 * necessary. The wakee CPU on receipt of the IPI will queue the task
 * via sched_ttwu_wakeup() for activation so the wakee incurs the cost
 * of the wakeup instead of the waker.
 */
ಸ್ಥಿರ ಶೂನ್ಯ __ttwu_queue_wakelist(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu, ಪೂರ್ಣಾಂಕ wake_flags)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);

	p->sched_remote_wakeup = !!(wake_flags & WF_MIGRATED);

	WRITE_ONCE(rq->ttwu_pending, 1);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SMP
	__smp_call_single_queue(cpu, &p->wake_ನಮೂದು.llist);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ wake_ಮೇಲೆ_if_idle(ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);

	guard(rcu)();
	ಆದರೆ (is_idle_task(rcu_dereference(rq->curr))) {
		guard(rq_ಬೀಗ_irqsave)(rq);
		ಆದರೆ (is_idle_task(rq->curr))
			resched_curr(rq);
	}
}

bool cpus_equal_ಸಾಮರ್ಥ್ಯ(ಪೂರ್ಣಾಂಕ this_cpu, ಪೂರ್ಣಾಂಕ that_cpu)
{
	ಆದರೆ (!sched_asym_cpucap_ಸಕ್ರಿಯ())
		ಹಿಂತಿರುಗಿ true;

	ಆದರೆ (this_cpu == that_cpu)
		ಹಿಂತಿರುಗಿ true;

	ಹಿಂತಿರುಗಿ arch_scale_cpu_ಸಾಮರ್ಥ್ಯ(this_cpu) == arch_scale_cpu_ಸಾಮರ್ಥ್ಯ(that_cpu);
}

bool cpus_share_cache(ಪೂರ್ಣಾಂಕ this_cpu, ಪೂರ್ಣಾಂಕ that_cpu)
{
	ಆದರೆ (this_cpu == that_cpu)
		ಹಿಂತಿರುಗಿ true;

	ಹಿಂತಿರುಗಿ ಪ್ರತಿ_cpu(sd_llc_id, this_cpu) == ಪ್ರತಿ_cpu(sd_llc_id, that_cpu);
}

/*
 * Whether CPUs are share cache resources, which means LLC on non-cluster
 * machines and LLC tag or L2 on machines with clusters.
 */
bool cpus_share_resources(ಪೂರ್ಣಾಂಕ this_cpu, ಪೂರ್ಣಾಂಕ that_cpu)
{
	ಆದರೆ (this_cpu == that_cpu)
		ಹಿಂತಿರುಗಿ true;

	ಹಿಂತಿರುಗಿ ಪ್ರತಿ_cpu(sd_share_id, this_cpu) == ಪ್ರತಿ_cpu(sd_share_id, that_cpu);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool ttwu_ಸರತಿ_cond(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu)
{
	ಪೂರ್ಣಾಂಕ this_cpu = smp_processor_id();

	/* See SCX_OPS_ALLOW_QUEUED_WAKEUP. */
	ಆದರೆ (!scx_allow_ttwu_ಸರತಿ(p))
		ಹಿಂತಿರುಗಿ false;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SMP
	ಆದರೆ (p->sched_class == &stop_sched_class)
		ಹಿಂತಿರುಗಿ false;
#ಆದರೆ_ಅಂತ್ಯ

	/*
	 * Do not complicate things with the async wake_list while the CPU is
	 * in hotplug state.
	 */
	ಆದರೆ (!cpu_ಸಕ್ರಿಯ(cpu))
		ಹಿಂತಿರುಗಿ false;

	/* Ensure the task will still be allowed to run on the CPU. */
	ಆದರೆ (!cpumask_test_cpu(cpu, p->cpus_ptr))
		ಹಿಂತಿರುಗಿ false;

	/*
	 * If the CPU does not share cache, then queue the task on the
	 * remote rqs wakelist to avoid accessing remote data.
	 */
	ಆದರೆ (!cpus_share_cache(this_cpu, cpu))
		ಹಿಂತಿರುಗಿ true;

	ಆದರೆ (cpu == this_cpu)
		ಹಿಂತಿರುಗಿ false;

	/*
	 * If the wakee cpu is idle, or the task is descheduling and the
	 * only running task on the CPU, then use the wakelist to offload
	 * the task activation to the idle (or soon-to-be-idle) CPU as
	 * the current CPU is likely busy. nr_running is checked to
	 * avoid unnecessary task stacking.
	 *
	 * Note that we can only get here with (wakee) p->on_rq=0,
	 * p->on_cpu can be whatever, we've done the dequeue, so
	 * the wakee has been accounted out of ->nr_running.
	 */
	ಆದರೆ (!cpu_rq(cpu)->nr_running)
		ಹಿಂತಿರುಗಿ true;

	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool ttwu_ಸರತಿ_wakelist(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu, ಪೂರ್ಣಾಂಕ wake_flags)
{
	ಆದರೆ (sched_feat(TTWU_ಸರತಿ) && ttwu_ಸರತಿ_cond(p, cpu)) {
		sched_clock_cpu(cpu); /* Sync clocks across CPUs */
		__ttwu_queue_wakelist(p, cpu, wake_flags);
		ಹಿಂತಿರುಗಿ true;
	}

	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ ttwu_ಸರತಿ(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu, ಪೂರ್ಣಾಂಕ wake_flags)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);
	ರಚನೆ rq_flags rf;

	ಆದರೆ (ttwu_ಸರತಿ_wakelist(p, cpu, wake_flags))
		ಹಿಂತಿರುಗಿ;

	rq_ಬೀಗ(rq, &rf);
	update_rq_clock(rq);
	ttwu_do_activate(rq, p, wake_flags, &rf);
	rq_unlock(rq, &rf);
}

/*
 * Invoked from try_to_wake_up() to check whether the task can be woken up.
 *
 * The caller holds p::pi_lock if p != current or has preemption
 * disabled when p == current.
 *
 * The rules of saved_state:
 *
 *   The related locking code always holds p::pi_lock when updating
 *   p::saved_state, which means the code is fully serialized in both cases.
 *
 *   For PREEMPT_RT, the lock wait and lock wakeups happen via TASK_RTLOCK_WAIT.
 *   No other bits set. This allows to distinguish all wakeup scenarios.
 *
 *   For FREEZER, the wakeup happens via TASK_FROZEN. No other bits set. This
 *   allows us to prevent early wakeup of tasks before they can be run on
 *   asymmetric ISA architectures (eg ARMv9).
 */
ಸ್ಥಿರ __always_inline
bool ttwu_state_ಹೊಂದಿಕೆ(ರಚನೆ task_struct *p, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ state, ಪೂರ್ಣಾಂಕ *success)
{
	ಪೂರ್ಣಾಂಕ ಹೊಂದಿಕೆ;

	ಆದರೆ (IS_ENABLED(CONFIG_DEBUG_PREEMPT)) {
		WARN_ON_ONCE((state & TASK_RTLOCK_WAIT) &&
			     state != TASK_RTLOCK_WAIT);
	}

	*success = !!(ಹೊಂದಿಕೆ = __task_state_match(p, state));

	/*
	 * Saved state preserves the task state across blocking on
	 * an RT lock or TASK_FREEZABLE tasks.  If the state matches,
	 * set p::saved_state to TASK_RUNNING, but do not wake the task
	 * because it waits for a lock wakeup or __thaw_task(). Also
	 * indicate success because from the regular waker's point of
	 * view this has succeeded.
	 *
	 * After acquiring the lock the task will restore p::__state
	 * from p::saved_state which ensures that the regular
	 * wakeup is not lost. The restore will also set
	 * p::saved_state to TASK_RUNNING so any further tests will
	 * not result in false positives vs. @success
	 */
	ಆದರೆ (ಹೊಂದಿಕೆ < 0)
		p->saved_state = TASK_RUNNING;

	ಹಿಂತಿರುಗಿ ಹೊಂದಿಕೆ > 0;
}

/*
 * Notes on Program-Order guarantees on SMP systems.
 *
 *  MIGRATION
 *
 * The basic program-order guarantee on SMP systems is that when a task [t]
 * migrates, all its activity on its old CPU [c0] happens-before any subsequent
 * execution on its new CPU [c1].
 *
 * For migration (of runnable tasks) this is provided by the following means:
 *
 *  A) UNLOCK of the rq(c0)->lock scheduling out task t
 *  B) migration for t is required to synchronize *both* rq(c0)->lock and
 *     rq(c1)->lock (if not at the same time, then in that order).
 *  C) LOCK of the rq(c1)->lock scheduling in task
 *
 * Release/acquire chaining guarantees that B happens after A and C after B.
 * Note: the CPU doing B need not be c0 or c1
 *
 * Example:
 *
 *   CPU0            CPU1            CPU2
 *
 *   LOCK rq(0)->lock
 *   sched-out X
 *   sched-in Y
 *   UNLOCK rq(0)->lock
 *
 *                                   LOCK rq(0)->lock // orders against CPU0
 *                                   dequeue X
 *                                   UNLOCK rq(0)->lock
 *
 *                                   LOCK rq(1)->lock
 *                                   enqueue X
 *                                   UNLOCK rq(1)->lock
 *
 *                   LOCK rq(1)->lock // orders against CPU2
 *                   sched-out Z
 *                   sched-in X
 *                   UNLOCK rq(1)->lock
 *
 *
 *  BLOCKING -- aka. SLEEP + WAKEUP
 *
 * For blocking we (obviously) need to provide the same guarantee as for
 * migration. However the means are completely different as there is no lock
 * chain to provide order. Instead we do:
 *
 *   1) smp_store_release(X->on_cpu, 0)   -- finish_task()
 *   2) smp_cond_load_acquire(!X->on_cpu) -- try_to_wake_up()
 *
 * Example:
 *
 *   CPU0 (schedule)  CPU1 (try_to_wake_up) CPU2 (schedule)
 *
 *   LOCK rq(0)->lock LOCK X->pi_lock
 *   dequeue X
 *   sched-out X
 *   smp_store_release(X->on_cpu, 0);
 *
 *                    smp_cond_load_acquire(&X->on_cpu, !VAL);
 *                    X->state = WAKING
 *                    set_task_cpu(X,2)
 *
 *                    LOCK rq(2)->lock
 *                    enqueue X
 *                    X->state = RUNNING
 *                    UNLOCK rq(2)->lock
 *
 *                                          LOCK rq(2)->lock // orders against CPU1
 *                                          sched-out Z
 *                                          sched-in X
 *                                          UNLOCK rq(2)->lock
 *
 *                    UNLOCK X->pi_lock
 *   UNLOCK rq(0)->lock
 *
 *
 * However, for wakeups there is a second guarantee we must provide, namely we
 * must ensure that CONDITION=1 done by the caller can not be reordered with
 * accesses to the task state; see try_to_wake_up() and set_current_state().
 */

/**
 * try_to_wake_up - wake up a thread
 * @p: the thread to be awakened
 * @state: the mask of task states that can be woken
 * @wake_flags: wake modifier flags (WF_*)
 *
 * Conceptually does:
 *
 *   If (@state & @p->state) @p->state = TASK_RUNNING.
 *
 * If the task was not queued/runnable, also place it back on a runqueue.
 *
 * This function is atomic against schedule() which would dequeue the task.
 *
 * It issues a full memory barrier before accessing @p->state, see the comment
 * with set_current_state().
 *
 * Uses p->pi_lock to serialize against concurrent wake-ups.
 *
 * Relies on p->pi_lock stabilizing:
 *  - p->sched_class
 *  - p->cpus_ptr
 *  - p->sched_task_group
 * in order to do migration, see its use of select_task_rq()/set_task_cpu().
 *
 * Tries really hard to only take one task_rq(p)->lock for performance.
 * Takes rq->lock in:
 *  - ttwu_runnable()    -- old rq, unavoidable, see comment there;
 *  - ttwu_queue()       -- new rq, for enqueue of the task;
 *  - psi_ttwu_dequeue() -- much sadness :-( accounting will kill us.
 *
 * As a consequence we race really badly with just about everything. See the
 * many memory barriers and their comments for details.
 *
 * Return: %true if @p->state changes (an actual wakeup was done),
 *	   %false otherwise.
 */
ಪೂರ್ಣಾಂಕ try_ಗಮ್ಯ_wake_ಮೇಲೆ(ರಚನೆ task_struct *p, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ state, ಪೂರ್ಣಾಂಕ wake_flags)
{
	guard(preempt)();
	ಪೂರ್ಣಾಂಕ cpu, success = 0;

	wake_flags |= WF_TTWU;

	ಆದರೆ (p == current) {
		/*
		 * We're waking current, this means 'p->on_rq' and 'task_cpu(p)
		 * == smp_processor_id()'. Together this means we can special
		 * case the whole 'p->on_rq && ttwu_runnable()' case below
		 * without taking any locks.
		 *
		 * Specifically, given current runs ttwu() we must be before
		 * schedule()'s block_task(), as such this must not observe
		 * sched_delayed.
		 *
		 * In particular:
		 *  - we rely on Program-Order guarantees for all the ordering,
		 *  - we're serialized against set_special_state() by virtue of
		 *    it disabling IRQs (this allows not taking ->pi_lock).
		 */
		WARN_ON_ONCE(p->se.sched_delayed);
		WARN_ON_ONCE(p->is_blocked);
		/* If p is current, we know we can run here, so clear blocked_on */
		clear_task_blocked_on(p, ಖಾಲಿ);
		ಆದರೆ (!ttwu_state_ಹೊಂದಿಕೆ(p, state, &success))
			ಹೋಗು ಹೊರಹೋಗು;

		trace_sched_waking(p);
		ttwu_do_wakeup(p);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/*
	 * If we are going to wake up a thread waiting for CONDITION we
	 * need to ensure that CONDITION=1 done by the caller can not be
	 * reordered with p->state check below. This pairs with smp_store_mb()
	 * in set_current_state() that the waiting thread does.
	 */
	scoped_guard (raw_spinlock_irqsave, &p->pi_ಬೀಗ) {
		smp_mb__after_spinlock();

		ಆದರೆ (!ttwu_state_ಹೊಂದಿಕೆ(p, state, &success))
			ನಿಲ್ಲಿಸು;

		trace_sched_waking(p);

		/*
		 * Ensure we load p->on_rq _after_ p->state, otherwise it would
		 * be possible to, falsely, observe p->on_rq == 0 and get stuck
		 * in smp_cond_load_acquire() below.
		 *
		 * sched_ttwu_pending()			try_to_wake_up()
		 *   STORE p->on_rq = 1			  LOAD p->state
		 *   UNLOCK rq->lock
		 *
		 * __schedule() (switch to task 'p')
		 *   LOCK rq->lock			  smp_rmb();
		 *   smp_mb__after_spinlock();
		 *   UNLOCK rq->lock
		 *
		 * [task p]
		 *   STORE p->state = UNINTERRUPTIBLE	  LOAD p->on_rq
		 *
		 * Pairs with the LOCK+smp_mb__after_spinlock() on rq->lock in
		 * __schedule().  See the comment for smp_mb__after_spinlock().
		 *
		 * A similar smp_rmb() lives in __task_needs_rq_lock().
		 */
		smp_rmb();
		ಆದರೆ (READ_ONCE(p->on_rq) && ttwu_runnable(p, wake_flags))
			ನಿಲ್ಲಿಸು;

		/*
		 * Ensure we load p->on_cpu _after_ p->on_rq, otherwise it would be
		 * possible to, falsely, observe p->on_cpu == 0.
		 *
		 * One must be running (->on_cpu == 1) in order to remove oneself
		 * from the runqueue.
		 *
		 * __schedule() (switch to task 'p')	try_to_wake_up()
		 *   STORE p->on_cpu = 1		  LOAD p->on_rq
		 *   UNLOCK rq->lock
		 *
		 * __schedule() (put 'p' to sleep)
		 *   LOCK rq->lock			  smp_rmb();
		 *   smp_mb__after_spinlock();
		 *   STORE p->on_rq = 0			  LOAD p->on_cpu
		 *
		 * Pairs with the LOCK+smp_mb__after_spinlock() on rq->lock in
		 * __schedule().  See the comment for smp_mb__after_spinlock().
		 *
		 * Form a control-dep-acquire with p->on_rq == 0 above, to ensure
		 * schedule()'s block_task() has 'happened' and p will no longer
		 * care about it's own p->state. See the comment in __schedule().
		 */
		smp_acquire__after_ctrl_dep();

		/*
		 * We're doing the wakeup (@success == 1), they did a dequeue (p->on_rq
		 * == 0), which means we need to do an enqueue, change p->state to
		 * TASK_WAKING such that we can unlock p->pi_lock before doing the
		 * enqueue, such as ttwu_queue_wakelist().
		 */
		WRITE_ONCE(p->__state, TASK_WAKING);

		/*
		 * If the owning (remote) CPU is still in the middle of schedule() with
		 * this task as prev, considering queueing p on the remote CPUs wake_list
		 * which potentially sends an IPI instead of spinning on p->on_cpu to
		 * let the waker make forward progress. This is safe because IRQs are
		 * disabled and the IPI will deliver after on_cpu is cleared.
		 *
		 * Ensure we load task_cpu(p) after p->on_cpu:
		 *
		 * set_task_cpu(p, cpu);
		 *   STORE p->cpu = @cpu
		 * __schedule() (switch to task 'p')
		 *   LOCK rq->lock
		 *   smp_mb__after_spin_lock()		smp_cond_load_acquire(&p->on_cpu)
		 *   STORE p->on_cpu = 1		LOAD p->cpu
		 *
		 * to ensure we observe the correct CPU on which the task is currently
		 * scheduling.
		 */
		ಆದರೆ (smp_ತುಂಬು_acquire(&p->on_cpu) &&
		    ttwu_ಸರತಿ_wakelist(p, task_cpu(p), wake_flags))
			ನಿಲ್ಲಿಸು;

		/*
		 * If the owning (remote) CPU is still in the middle of schedule() with
		 * this task as prev, wait until it's done referencing the task.
		 *
		 * Pairs with the smp_store_release() in finish_task().
		 *
		 * This ensures that tasks getting woken will be fully ordered against
		 * their previous state and preserve Program Order.
		 */
		smp_cond_ತುಂಬು_acquire(&p->on_cpu, !VAL);

		cpu = select_task_rq(p, p->wake_cpu, &wake_flags);
		ಆದರೆ (task_cpu(p) != cpu) {
			ಆದರೆ (p->ಒಳ_iowait) {
				delayacct_blkio_ಕೊನೆ(p);
				atomic_dec(&task_rq(p)->nr_iowait);
			}

			wake_flags |= WF_MIGRATED;
			psi_ttwu_ಸರತಿಯಿಂದ_ತೆಗೆ(p);
			ಗಣ_task_cpu(p, cpu);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cpu != p->wake_cpu) {
			/*
			 * If we were proxy-migrated to cpu, then
			 * select_task_rq() picks cpu instead of wake_cpu
			 * to return to, we won't call set_task_cpu(),
			 * leaving a stale wake_cpu pointing to where we
			 * proxy-migrated from. So just fixup wake_cpu here
			 * if its not correct
			 */
			p->wake_cpu = cpu;
		}

		ttwu_ಸರತಿ(p, cpu, wake_flags);
	}
ಹೊರಹೋಗು:
	ಆದರೆ (success)
		ttwu_stat(p, task_cpu(p), wake_flags);

	ಹಿಂತಿರುಗಿ success;
}

ಸ್ಥಿರ bool __task_needs_rq_lock(ರಚನೆ task_struct *p)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ state = READ_ONCE(p->__state);

	/*
	 * Since pi->lock blocks try_to_wake_up(), we don't need rq->lock when
	 * the task is blocked. Make sure to check @state since ttwu() can drop
	 * locks at the end, see ttwu_queue_wakelist().
	 */
	ಆದರೆ (state == TASK_RUNNING || state == TASK_WAKING)
		ಹಿಂತಿರುಗಿ true;

	/*
	 * Ensure we load p->on_rq after p->__state, otherwise it would be
	 * possible to, falsely, observe p->on_rq == 0.
	 *
	 * See try_to_wake_up() for a longer comment.
	 */
	smp_rmb();
	ಆದರೆ (p->on_rq)
		ಹಿಂತಿರುಗಿ true;

	/*
	 * Ensure the task has finished __schedule() and will not be referenced
	 * anymore. Again, see try_to_wake_up() for a longer comment.
	 */
	smp_rmb();
	smp_cond_ತುಂಬು_acquire(&p->on_cpu, !VAL);

	ಹಿಂತಿರುಗಿ false;
}

/**
 * task_call_func - Invoke a function on task in fixed state
 * @p: Process for which the function is to be invoked, can be @current.
 * @func: Function to invoke.
 * @arg: Argument to function.
 *
 * Fix the task in it's current state by avoiding wakeups and or rq operations
 * and call @func(@arg) on it.  This function can use task_is_runnable() and
 * task_curr() to work out what the state is, if required.  Given that @func
 * can be invoked with a runqueue lock held, it had better be quite
 * lightweight.
 *
 * Returns:
 *   Whatever @func returns
 */
ಪೂರ್ಣಾಂಕ task_call_func(ರಚನೆ task_struct *p, task_call_f func, ಶೂನ್ಯ *arg)
{
	ರಚನೆ rq_flags rf;
	ಪೂರ್ಣಾಂಕ ret;

	raw_spin_ಬೀಗ_irqsave(&p->pi_ಬೀಗ, rf.flags);

	ಆದರೆ (__task_needs_rq_lock(p)) {
		ರಚನೆ rq *rq = __task_rq_lock(p, &rf);

		/*
		 * At this point the task is pinned; either:
		 *  - blocked and we're holding off wakeups	 (pi->lock)
		 *  - woken, and we're holding off enqueue	 (rq->lock)
		 *  - queued, and we're holding off schedule	 (rq->lock)
		 *  - running, and we're holding off de-schedule (rq->lock)
		 *
		 * The called function (@func) can use: task_curr(), p->on_rq and
		 * p->__state to differentiate between these states.
		 */
		ret = func(p, arg);

		__task_rq_unlock(rq, p, &rf);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ret = func(p, arg);
	}

	raw_spin_unlock_irqrestore(&p->pi_ಬೀಗ, rf.flags);
	ಹಿಂತಿರುಗಿ ret;
}

/**
 * cpu_curr_snapshot - Return a snapshot of the currently running task
 * @cpu: The CPU on which to snapshot the task.
 *
 * Returns the task_struct pointer of the task "currently" running on
 * the specified CPU.
 *
 * If the specified CPU was offline, the return value is whatever it
 * is, perhaps a pointer to the task_struct structure of that CPU's idle
 * task, but there is no guarantee.  Callers wishing a useful return
 * value must take some action to ensure that the specified CPU remains
 * online throughout.
 *
 * This function executes full memory barriers before and after fetching
 * the pointer, which permits the caller to confine this function's fetch
 * with respect to the caller's accesses to other shared variables.
 */
ರಚನೆ task_struct *cpu_curr_snapshot(ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);
	ರಚನೆ task_struct *t;
	ರಚನೆ rq_flags rf;

	rq_ಬೀಗ_irqsave(rq, &rf);
	smp_mb__after_spinlock(); /* Pairing determined by caller's synchronization design. */
	t = rcu_dereference(cpu_curr(cpu));
	rq_unlock_irqrestore(rq, &rf);
	smp_mb(); /* Pairing determined by caller's synchronization design. */

	ಹಿಂತಿರುಗಿ t;
}

/**
 * wake_up_process - Wake up a specific process
 * @p: The process to be woken up.
 *
 * Attempt to wake up the nominated process and move it to the set of runnable
 * processes.
 *
 * Return: 1 if the process was woken up, 0 if it was already running.
 *
 * This function executes a full memory barrier before accessing the task state.
 */
ಪೂರ್ಣಾಂಕ wake_ಮೇಲೆ_process(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ try_ಗಮ್ಯ_wake_ಮೇಲೆ(p, TASK_ಸಾಮಾನ್ಯ, 0);
}
EXPORT_ಚಿಹ್ನೆ(wake_ಮೇಲೆ_process);

ಪೂರ್ಣಾಂಕ wake_ಮೇಲೆ_state(ರಚನೆ task_struct *p, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ state)
{
	ಹಿಂತಿರುಗಿ try_ಗಮ್ಯ_wake_ಮೇಲೆ(p, state, 0);
}

/*
 * Perform scheduler related setup for a newly forked process p.
 * p is forked by current.
 *
 * __sched_fork() is basic setup which is also used by sched_init() to
 * initialize the boot CPU's idle task.
 */
ಸ್ಥಿರ ಶೂನ್ಯ __sched_fork(u64 clone_flags, ರಚನೆ task_struct *p)
{
	p->on_rq			= 0;

	p->se.on_rq			= 0;
	p->se.exec_ಆರಂಭ		= 0;
	p->se.ಮೊತ್ತ_exec_runtime		= 0;
	p->se.prev_ಮೊತ್ತ_exec_runtime	= 0;
	p->se.nr_migrations		= 0;
	p->se.vruntime			= 0;
	p->se.vlag			= 0;
	p->se.rel_deadline		= 0;
	INIT_LIST_ತಲೆ(&p->se.ಗುಂಪು_ಗಂಟು);

	/* A delayed task cannot be in clone(). */
	WARN_ON_ONCE(p->se.sched_delayed);
	WARN_ON_ONCE(p->is_blocked);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
	p->se.cfs_rq			= ಖಾಲಿ;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CFS_BANDWIDTH
	init_cfs_throttle_work(p);
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHEDSTATS
	/* Even if schedstat is disabled, there should not be garbage */
	memset(&p->stats, 0, ಗಾತ್ರ(p->stats));
#ಆದರೆ_ಅಂತ್ಯ

	init_dl_entity(&p->dl);

	INIT_LIST_ತಲೆ(&p->rt.ಸತತ_list);
	p->rt.timeout		= 0;
	p->rt.ಸಮಯ_ತುಂಡು	= sched_rr_timeslice;
	p->rt.on_rq		= 0;
	p->rt.on_list		= 0;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CLASS_EXT
	init_scx_entity(&p->scx);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PREEMPT_NOTIFIERS
	INIT_HLIST_ತಲೆ(&p->preempt_notifiers);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_COMPACTION
	p->capture_control = ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ
	init_numa_balancing(clone_flags, p);
	p->wake_ನಮೂದು.u_flags = CSD_TYPE_TTWU;
	p->migration_pending = ಖಾಲಿ;
	init_sched_mm(p);
}

DEFINE_STATIC_ಕೀಲಿ_FALSE(sched_numa_balancing);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING

ಪೂರ್ಣಾಂಕ sysctl_numa_balancing_ಬಹುಲಕ;

ಸ್ಥಿರ ಶೂನ್ಯ __set_numabalancing_state(bool enabled)
{
	ಆದರೆ (enabled)
		static_branch_enable(&sched_numa_balancing);
	ಇಲ್ಲದಿದ್ದರೆ
		static_branch_disable(&sched_numa_balancing);
}

ಶೂನ್ಯ ಗಣ_numabalancing_state(bool enabled)
{
	ಆದರೆ (enabled)
		sysctl_numa_balancing_ಬಹುಲಕ = NUMA_BALANCING_ಸಾಮಾನ್ಯ;
	ಇಲ್ಲದಿದ್ದರೆ
		sysctl_numa_balancing_ಬಹುಲಕ = NUMA_BALANCING_DISABLED;
	__set_numabalancing_state(enabled);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SYSCTL
ಸ್ಥಿರ ಶೂನ್ಯ reset_ಸ್ಮರಣೆ_tiering(ಶೂನ್ಯ)
{
	ರಚನೆ pglist_ದತ್ತಾಂಶ *pgdat;

	for_each_online_pgdat(pgdat) {
		pgdat->nbp_ಮಿತಿ = 0;
		pgdat->nbp_th_nr_cand = ಗಂಟು_ಪುಟ_state(pgdat, PGPROMOTE_CANDIDATE);
		pgdat->nbp_th_ಆರಂಭ = jiffies_ಗಮ್ಯ_msecs(jiffies);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sysctl_numa_balancing(ಸ್ಥಿರಾಂಕ ರಚನೆ ctl_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ, ಪೂರ್ಣಾಂಕ ಲಿಖ,
			  ಶೂನ್ಯ *ಬಫರ್, ಗಾತ್ರ_t *lenp, loff_t *ppos)
{
	ರಚನೆ ctl_ಕೋಷ್ಟಕ t;
	ಪೂರ್ಣಾಂಕ err;
	ಪೂರ್ಣಾಂಕ state = sysctl_numa_balancing_ಬಹುಲಕ;

	ಆದರೆ (ಲಿಖ && !capable(CAP_SYS_ADMIN))
		ಹಿಂತಿರುಗಿ -EPERM;

	t = *ಕೋಷ್ಟಕ;
	t.ದತ್ತಾಂಶ = &state;
	err = proc_dointvec_minmax(&t, ಲಿಖ, ಬಫರ್, lenp, ppos);
	ಆದರೆ (err < 0)
		ಹಿಂತಿರುಗಿ err;
	ಆದರೆ (ಲಿಖ) {
		ಆದರೆ (!(sysctl_numa_balancing_ಬಹುಲಕ & NUMA_BALANCING_ಸ್ಮರಣೆ_TIERING) &&
		    (state & NUMA_BALANCING_ಸ್ಮರಣೆ_TIERING))
			reset_ಸ್ಮರಣೆ_tiering();
		sysctl_numa_balancing_ಬಹುಲಕ = state;
		__set_numabalancing_state(state);
	}
	ಹಿಂತಿರುಗಿ err;
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SYSCTL */
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_NUMA_BALANCING */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHEDSTATS

DEFINE_STATIC_ಕೀಲಿ_FALSE(sched_schedstats);

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_schedstats(bool enabled)
{
	ಆದರೆ (enabled)
		static_branch_enable(&sched_schedstats);
	ಇಲ್ಲದಿದ್ದರೆ
		static_branch_disable(&sched_schedstats);
}

ಶೂನ್ಯ force_schedstat_enabled(ಶೂನ್ಯ)
{
	ಆದರೆ (!schedstat_enabled()) {
		pr_info("kernel profiling enabled schedstats, disable via kernel.sched_schedstats.\n");
		static_branch_enable(&sched_schedstats);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init ಸಿದ್ಧತೆ_schedstats(ಅಕ್ಷರ *str)
{
	ಪೂರ್ಣಾಂಕ ret = 0;
	ಆದರೆ (!str)
		ಹೋಗು ಹೊರಹೋಗು;

	ಆದರೆ (!strcmp(str, "enable")) {
		ಗಣ_schedstats(true);
		ret = 1;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(str, "disable")) {
		ಗಣ_schedstats(false);
		ret = 1;
	}
ಹೊರಹೋಗು:
	ಆದರೆ (!ret)
		pr_warn("Unable to parse schedstats=\n");

	ಹಿಂತಿರುಗಿ ret;
}
__setup("schedstats=", ಸಿದ್ಧತೆ_schedstats);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SYSCTL
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sysctl_schedstats(ಸ್ಥಿರಾಂಕ ರಚನೆ ctl_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ, ಪೂರ್ಣಾಂಕ ಲಿಖ, ಶೂನ್ಯ *ಬಫರ್,
		ಗಾತ್ರ_t *lenp, loff_t *ppos)
{
	ರಚನೆ ctl_ಕೋಷ್ಟಕ t;
	ಪೂರ್ಣಾಂಕ err;
	ಪೂರ್ಣಾಂಕ state = static_branch_likely(&sched_schedstats);

	ಆದರೆ (ಲಿಖ && !capable(CAP_SYS_ADMIN))
		ಹಿಂತಿರುಗಿ -EPERM;

	t = *ಕೋಷ್ಟಕ;
	t.ದತ್ತಾಂಶ = &state;
	err = proc_dointvec_minmax(&t, ಲಿಖ, ಬಫರ್, lenp, ppos);
	ಆದರೆ (err < 0)
		ಹಿಂತಿರುಗಿ err;
	ಆದರೆ (ಲಿಖ)
		ಗಣ_schedstats(state);
	ಹಿಂತಿರುಗಿ err;
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SYSCTL */
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SCHEDSTATS */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SYSCTL
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ ctl_ಕೋಷ್ಟಕ sched_core_sysctls[] = {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHEDSTATS
	{
		.procname       = "sched_schedstats",
		.ದತ್ತಾಂಶ           = ಖಾಲಿ,
		.maxlen         = ಗಾತ್ರ(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ),
		.ಬಹುಲಕ           = 0644,
		.proc_handler   = sysctl_schedstats,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SCHEDSTATS */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK
	{
		.procname       = "sched_util_clamp_min",
		.ದತ್ತಾಂಶ           = &sysctl_sched_uclamp_util_min,
		.maxlen         = ಗಾತ್ರ(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ),
		.ಬಹುಲಕ           = 0644,
		.proc_handler   = sysctl_sched_uclamp_handler,
	},
	{
		.procname       = "sched_util_clamp_max",
		.ದತ್ತಾಂಶ           = &sysctl_sched_uclamp_util_max,
		.maxlen         = ಗಾತ್ರ(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ),
		.ಬಹುಲಕ           = 0644,
		.proc_handler   = sysctl_sched_uclamp_handler,
	},
	{
		.procname       = "sched_util_clamp_min_rt_default",
		.ದತ್ತಾಂಶ           = &sysctl_sched_uclamp_util_min_rt_default,
		.maxlen         = ಗಾತ್ರ(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ),
		.ಬಹುಲಕ           = 0644,
		.proc_handler   = sysctl_sched_uclamp_handler,
	},
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_UCLAMP_TASK */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
	{
		.procname	= "numa_balancing",
		.ದತ್ತಾಂಶ		= ಖಾಲಿ, /* filled in by handler */
		.maxlen		= ಗಾತ್ರ(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ),
		.ಬಹುಲಕ		= 0644,
		.proc_handler	= sysctl_numa_balancing,
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_FOUR,
	},
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_NUMA_BALANCING */
};
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init sched_core_sysctl_init(ಶೂನ್ಯ)
{
	ರಿಜಿಸ್ಟರ್_sysctl_init("kernel", sched_core_sysctls);
	ಹಿಂತಿರುಗಿ 0;
}
late_initcall(sched_core_sysctl_init);
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SYSCTL */

/*
 * fork()/clone()-time setup:
 */
ಪೂರ್ಣಾಂಕ sched_fork(u64 clone_flags, ರಚನೆ task_struct *p)
{
	__sched_fork(clone_flags, p);
	/*
	 * We mark the process as NEW here. This guarantees that
	 * nobody will actually run it, and a signal or other external
	 * event cannot wake it up and insert it on the runqueue either.
	 */
	p->__state = TASK_NEW;

	/*
	 * Make sure we do not leak PI boosting priority to the child.
	 */
	p->prio = current->ಸಾಮಾನ್ಯ_prio;

	uclamp_fork(p);

	/*
	 * Revert to default priority/policy on fork if requested.
	 */
	ಆದರೆ (unlikely(p->sched_reset_on_fork)) {
		ಆದರೆ (task_has_dl_policy(p) || task_has_rt_policy(p)) {
			p->policy = SCHED_ಸಾಮಾನ್ಯ;
			p->static_prio = NICE_ಗಮ್ಯ_PRIO(0);
			p->rt_ಆದ್ಯತೆ = 0;
			p->timer_slack_ns = p->default_timer_slack_ns;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (PRIO_ಗಮ್ಯ_NICE(p->static_prio) < 0)
			p->static_prio = NICE_ಗಮ್ಯ_PRIO(0);

		p->prio = p->ಸಾಮಾನ್ಯ_prio = p->static_prio;
		ಗಣ_ತುಂಬು_ತೂಕ(p, false);
		p->se.custom_ತುಂಡು = 0;
		p->se.ತುಂಡು = sysctl_sched_ತಳ_ತುಂಡು;

		/*
		 * We don't need the reset flag anymore after the fork. It has
		 * fulfilled its duty:
		 */
		p->sched_reset_on_fork = 0;
	}

	ಆದರೆ (dl_prio(p->prio))
		ಹಿಂತಿರುಗಿ -EAGAIN;

	scx_pre_fork(p);

	ಆದರೆ (rt_prio(p->prio)) {
		p->sched_class = &rt_sched_class;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CLASS_EXT
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (task_should_scx(p->policy)) {
		p->sched_class = &ext_sched_class;
#ಆದರೆ_ಅಂತ್ಯ
	} ಇಲ್ಲದಿದ್ದರೆ {
		p->sched_class = &fair_sched_class;
	}

	init_entity_runnable_ಸರಾಸರಿ_ಮೌಲ್ಯ(&p->se);


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_INFO
	ಆದರೆ (likely(sched_info_on()))
		memset(&p->sched_info, 0, ಗಾತ್ರ(p->sched_info));
#ಆದರೆ_ಅಂತ್ಯ
	p->on_cpu = 0;
	init_task_preempt_ಎಣಿಕೆ(p);
	plist_ಗಂಟು_init(&p->pushable_tasks, MAX_PRIO);
	RB_CLEAR_ಗಂಟು(&p->pushable_dl_tasks);

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ sched_cgroup_fork(ರಚನೆ task_struct *p, ರಚನೆ kernel_clone_ವಾದಗಳು *kargs)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	/*
	 * Because we're not yet on the pid-hash, p->pi_lock isn't strictly
	 * required yet, but lockdep gets upset if rules are violated.
	 */
	raw_spin_ಬೀಗ_irqsave(&p->pi_ಬೀಗ, flags);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CGROUP_SCHED
	ಆದರೆ (1) {
		ರಚನೆ task_ಗುಂಪು *tg;
		tg = container_ದ(kargs->cset->subsys[cpu_cgrp_id],
				  ರಚನೆ task_ಗುಂಪು, css);
		tg = autogroup_task_ಗುಂಪು(p, tg);
		p->sched_task_ಗುಂಪು = tg;
	}
#ಆದರೆ_ಅಂತ್ಯ
	/*
	 * We're setting the CPU for the first time, we don't migrate,
	 * so use __set_task_cpu().
	 */
	__set_task_cpu(p, smp_processor_id());
	ಆದರೆ (p->sched_class->task_fork)
		p->sched_class->task_fork(p);
	raw_spin_unlock_irqrestore(&p->pi_ಬೀಗ, flags);

	ಹಿಂತಿರುಗಿ scx_fork(p, kargs);
}

ಶೂನ್ಯ sched_cancel_fork(ರಚನೆ task_struct *p)
{
	scx_cancel_fork(p);
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_mm_cid_fork(ರಚನೆ task_struct *t);

ಶೂನ್ಯ sched_post_fork(ರಚನೆ task_struct *p)
{
	sched_mm_cid_fork(p);
	uclamp_post_fork(p);
	scx_post_fork(p);
}

u64 ಗಮ್ಯ_ಅನುಪಾತ(u64 period, u64 runtime)
{
	ಆದರೆ (runtime == RUNTIME_INF)
		ಹಿಂತಿರುಗಿ BW_UNIT;

	/*
	 * Doing this here saves a lot of checks in all
	 * the calling paths, and returning zero seems
	 * safe for them anyway.
	 */
	ಆದರೆ (period == 0)
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ div64_u64(runtime << BW_ಸರಿಸು, period);
}

/*
 * wake_up_new_task - wake up a newly created task for the first time.
 *
 * This function will do some initial scheduler statistics housekeeping
 * that must be done for every newly created context, then puts the task
 * on the runqueue and wakes it.
 */
ಶೂನ್ಯ wake_ಮೇಲೆ_new_task(ರಚನೆ task_struct *p)
{
	ರಚನೆ rq_flags rf;
	ರಚನೆ rq *rq;
	ಪೂರ್ಣಾಂಕ wake_flags = WF_FORK;

	raw_spin_ಬೀಗ_irqsave(&p->pi_ಬೀಗ, rf.flags);
	WRITE_ONCE(p->__state, TASK_RUNNING);
	/*
	 * Fork balancing, do it here and not earlier because:
	 *  - cpus_ptr can change in the fork path
	 *  - any previously selected CPU might disappear through hotplug
	 *
	 * Use __set_task_cpu() to avoid calling sched_class::migrate_task_rq,
	 * as we're not fully set-up yet.
	 */
	p->recent_ಬಳಸಿದ_cpu = task_cpu(p);
	__set_task_cpu(p, select_task_rq(p, task_cpu(p), &wake_flags));
	rq = __task_rq_lock(p, &rf);
	update_rq_clock(rq);
	post_init_entity_util_avg(p);

	activate_task(rq, p, ಸರತಿಗೆ_ಸೇರಿಸು_NOCLOCK | ಸರತಿಗೆ_ಸೇರಿಸು_INITIAL);
	trace_sched_wakeup_new(p);
	wakeup_preempt(rq, p, wake_flags);
	ಆದರೆ (p->sched_class->task_woken) {
		/*
		 * Nothing relies on rq->lock after this, so it's fine to
		 * drop it.
		 */
		rq_unpin_ಬೀಗ(rq, &rf);
		p->sched_class->task_woken(rq, p);
		rq_repin_ಬೀಗ(rq, &rf);
	}
	task_rq_unlock(rq, p, &rf);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PREEMPT_NOTIFIERS

ಸ್ಥಿರ DEFINE_STATIC_ಕೀಲಿ_FALSE(preempt_notifier_ಕೀಲಿ);

ಶೂನ್ಯ preempt_notifier_inc(ಶೂನ್ಯ)
{
	static_branch_inc(&preempt_notifier_ಕೀಲಿ);
}
EXPORT_ಚಿಹ್ನೆ_GPL(preempt_notifier_inc);

ಶೂನ್ಯ preempt_notifier_dec(ಶೂನ್ಯ)
{
	static_branch_dec(&preempt_notifier_ಕೀಲಿ);
}
EXPORT_ಚಿಹ್ನೆ_GPL(preempt_notifier_dec);

/**
 * preempt_notifier_register - tell me when current is being preempted & rescheduled
 * @notifier: notifier struct to register
 */
ಶೂನ್ಯ preempt_notifier_ರಿಜಿಸ್ಟರ್(ರಚನೆ preempt_notifier *notifier)
{
	ಆದರೆ (!static_branch_unlikely(&preempt_notifier_ಕೀಲಿ))
		WARN(1, "registering preempt_notifier while notifiers disabled\n");

	hlist_ಕೂಡಿಸು_ತಲೆ(&notifier->link, &current->preempt_notifiers);
}
EXPORT_ಚಿಹ್ನೆ_GPL(preempt_notifier_ರಿಜಿಸ್ಟರ್);

/**
 * preempt_notifier_unregister - no longer interested in preemption notifications
 * @notifier: notifier struct to unregister
 *
 * This is *not* safe to call from within a preemption notifier.
 */
ಶೂನ್ಯ preempt_notifier_unregister(ರಚನೆ preempt_notifier *notifier)
{
	hlist_del(&notifier->link);
}
EXPORT_ಚಿಹ್ನೆ_GPL(preempt_notifier_unregister);

ಸ್ಥಿರ ಶೂನ್ಯ __fire_sched_in_preempt_notifiers(ರಚನೆ task_struct *curr)
{
	ರಚನೆ preempt_notifier *notifier;

	hlist_for_each_ನಮೂದು(notifier, &curr->preempt_notifiers, link)
		notifier->ops->sched_ಒಳ(notifier, raw_smp_processor_id());
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ fire_sched_ಒಳ_preempt_notifiers(ರಚನೆ task_struct *curr)
{
	ಆದರೆ (static_branch_unlikely(&preempt_notifier_ಕೀಲಿ))
		__fire_sched_in_preempt_notifiers(curr);
}

ಸ್ಥಿರ ಶೂನ್ಯ
__fire_sched_out_preempt_notifiers(ರಚನೆ task_struct *curr,
				   ರಚನೆ task_struct *ಮುಂದಿನ)
{
	ರಚನೆ preempt_notifier *notifier;

	hlist_for_each_ನಮೂದು(notifier, &curr->preempt_notifiers, link)
		notifier->ops->sched_ಹೊರಹೋಗು(notifier, ಮುಂದಿನ);
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ
fire_sched_ಹೊರಹೋಗು_preempt_notifiers(ರಚನೆ task_struct *curr,
				 ರಚನೆ task_struct *ಮುಂದಿನ)
{
	ಆದರೆ (static_branch_unlikely(&preempt_notifier_ಕೀಲಿ))
		__fire_sched_out_preempt_notifiers(curr, ಮುಂದಿನ);
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_PREEMPT_NOTIFIERS: */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ fire_sched_ಒಳ_preempt_notifiers(ರಚನೆ task_struct *curr)
{
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
fire_sched_ಹೊರಹೋಗು_preempt_notifiers(ರಚನೆ task_struct *curr,
				 ರಚನೆ task_struct *ಮುಂದಿನ)
{
}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_PREEMPT_NOTIFIERS */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ prepare_task(ರಚನೆ task_struct *ಮುಂದಿನ)
{
	/*
	 * Claim the task as running, we do this before switching to it
	 * such that any running task will have this set.
	 *
	 * See the smp_load_acquire(&p->on_cpu) case in ttwu() and
	 * its ordering comment.
	 */
	WRITE_ONCE(ಮುಂದಿನ->on_cpu, 1);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಮುಕ್ತಾಯ_task(ರಚನೆ task_struct *prev)
{
	/*
	 * This must be the very last reference to @prev from this CPU. After
	 * p->on_cpu is cleared, the task can be moved to a different CPU. We
	 * must ensure this doesn't happen until the switch is completely
	 * finished.
	 *
	 * In particular, the load of prev->state in finish_task_switch() must
	 * happen before this.
	 *
	 * Pairs with the smp_cond_load_acquire() in try_to_wake_up().
	 */
	smp_store_release(&prev->on_cpu, 0);
}

/*
 * Only called from __schedule context
 *
 * There are some cases where we are going to re-do the action
 * that added the balance callbacks. We may not be in a state
 * where we can run them, so just zap them so they can be
 * properly re-added on the next time around. This is similar
 * handling to running the callbacks, except we just don't call
 * them.
 */
ಸ್ಥಿರ ಶೂನ್ಯ zap_balance_callbacks(ರಚನೆ rq *rq)
{
	ರಚನೆ balance_callback *ಮುಂದಿನ, *ತಲೆ;
	bool ಸಿಕ್ಕಿತು = false;

	lockdep_assert_rq_held(rq);

	ತಲೆ = rq->balance_callback;
	ಆಗ (ತಲೆ) {
		ಆದರೆ (ತಲೆ == &balance_ತಳ್ಳು_callback)
			ಸಿಕ್ಕಿತು = true;
		ಮುಂದಿನ = ತಲೆ->ಮುಂದಿನ;
		ತಲೆ->ಮುಂದಿನ = ಖಾಲಿ;
		ತಲೆ = ಮುಂದಿನ;
	}
	rq->balance_callback = ಸಿಕ್ಕಿತು ? &balance_ತಳ್ಳು_callback : ಖಾಲಿ;
}

ಸ್ಥಿರ ಶೂನ್ಯ do_balance_callbacks(ರಚನೆ rq *rq, ರಚನೆ balance_callback *ತಲೆ)
{
	ಶೂನ್ಯ (*func)(ರಚನೆ rq *rq);
	ರಚನೆ balance_callback *ಮುಂದಿನ;

	lockdep_assert_rq_held(rq);

	ಆಗ (ತಲೆ) {
		func = ತಲೆ->func;
		ಮುಂದಿನ = ತಲೆ->ಮುಂದಿನ;
		ತಲೆ->ಮುಂದಿನ = ಖಾಲಿ;
		ತಲೆ = ಮುಂದಿನ;

		func(rq);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ balance_ತಳ್ಳು(ರಚನೆ rq *rq);

/*
 * balance_push_callback is a right abuse of the callback interface and plays
 * by significantly different rules.
 *
 * Where the normal balance_callback's purpose is to be ran in the same context
 * that queued it (only later, when it's safe to drop rq->lock again),
 * balance_push_callback is specifically targeted at __schedule().
 *
 * This abuse is tolerated because it places all the unlikely/odd cases behind
 * a single test, namely: rq->balance_callback == NULL.
 */
ರಚನೆ balance_callback balance_ತಳ್ಳು_callback = {
	.ಮುಂದಿನ = ಖಾಲಿ,
	.func = balance_ತಳ್ಳು,
};

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ balance_callback *
__splice_balance_callbacks(ರಚನೆ rq *rq, bool ವಿಭಜಿಸು)
{
	ರಚನೆ balance_callback *ತಲೆ = rq->balance_callback;

	ಆದರೆ (likely(!ತಲೆ))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	lockdep_assert_rq_held(rq);
	/*
	 * Must not take balance_push_callback off the list when
	 * splice_balance_callbacks() and balance_callbacks() are not
	 * in the same rq->lock section.
	 *
	 * In that case it would be possible for __schedule() to interleave
	 * and observe the list empty.
	 */
	ಆದರೆ (ವಿಭಜಿಸು && ತಲೆ == &balance_ತಳ್ಳು_callback)
		ತಲೆ = ಖಾಲಿ;
	ಇಲ್ಲದಿದ್ದರೆ
		rq->balance_callback = ಖಾಲಿ;

	ಹಿಂತಿರುಗಿ ತಲೆ;
}

ರಚನೆ balance_callback *splice_balance_callbacks(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ __splice_balance_callbacks(rq, true);
}

ಶೂನ್ಯ __balance_callbacks(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
{
	ಆದರೆ (rf)
		rq_unpin_ಬೀಗ(rq, rf);
	do_balance_callbacks(rq, __splice_balance_callbacks(rq, false));
	ಆದರೆ (rf)
		rq_repin_ಬೀಗ(rq, rf);
}

ಶೂನ್ಯ balance_callbacks(ರಚನೆ rq *rq, ರಚನೆ balance_callback *ತಲೆ)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	ಆದರೆ (unlikely(ತಲೆ)) {
		raw_spin_rq_ಬೀಗ_irqsave(rq, flags);
		do_balance_callbacks(rq, ತಲೆ);
		raw_spin_rq_unlock_irqrestore(rq, flags);
	}
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
prepare_ಬೀಗ_switch(ರಚನೆ rq *rq, ರಚನೆ task_struct *ಮುಂದಿನ, ರಚನೆ rq_flags *rf)
	__releases(__rq_lockp(rq))
	__acquires(__rq_lockp(this_rq()))
{
	/*
	 * Since the runqueue lock will be released by the next
	 * task (which is an invalid locking op but in the case
	 * of the scheduler it's an obvious special-case), so we
	 * do an early lockdep release here:
	 */
	rq_unpin_ಬೀಗ(rq, rf);
	spin_release(&__rq_lockp(rq)->dep_map, _THIS_IP_);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_DEBUG_SPINLOCK
	/* this is a valid case when another task releases the spinlock */
	rq_lockp(rq)->owner = ಮುಂದಿನ;
#ಆದರೆ_ಅಂತ್ಯ
	/*
	 * Model the rq reference switcheroo.
	 */
	__release(__rq_lockp(rq));
	__acquire(__rq_lockp(this_rq()));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಮುಕ್ತಾಯ_ಬೀಗ_switch(ರಚನೆ rq *rq)
	__releases(__rq_lockp(rq))
{
	/*
	 * If we are tracking spinlock dependencies then we have to
	 * fix up the runqueue lock - which gets 'carried over' from
	 * prev into current:
	 */
	spin_acquire(&__rq_lockp(rq)->dep_map, 0, 0, _THIS_IP_);
	__balance_callbacks(rq, ಖಾಲಿ);
	hrtick_ವೇಳಾಪಟ್ಟಿ_exit(rq);
	raw_spin_rq_unlock_irq(rq);
}

/*
 * NOP if the arch has not defined these:
 */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ prepare_arch_switch
# define prepare_arch_switch(ಮುಂದಿನ)	ಮಾಡು { } ಆಗ (0)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಮುಕ್ತಾಯ_arch_post_ಬೀಗ_switch
# define ಮುಕ್ತಾಯ_arch_post_ಬೀಗ_switch()	ಮಾಡು { } ಆಗ (0)
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ kmap_local_sched_ಹೊರಹೋಗು(ಶೂನ್ಯ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_KMAP_LOCAL
	ಆದರೆ (unlikely(current->kmap_ctrl.idx))
		__kmap_local_sched_out();
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ kmap_local_sched_ಒಳ(ಶೂನ್ಯ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_KMAP_LOCAL
	ಆದರೆ (unlikely(current->kmap_ctrl.idx))
		__kmap_local_sched_in();
#ಆದರೆ_ಅಂತ್ಯ
}

/**
 * prepare_task_switch - prepare to switch tasks
 * @rq: the runqueue preparing to switch
 * @prev: the current task that is being switched out
 * @next: the task we are going to switch to.
 *
 * This is called with the rq lock held and interrupts off. It must
 * be paired with a subsequent finish_task_switch after the context
 * switch.
 *
 * prepare_task_switch sets up locking and calls architecture specific
 * hooks.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
prepare_task_switch(ರಚನೆ rq *rq, ರಚನೆ task_struct *prev,
		    ರಚನೆ task_struct *ಮುಂದಿನ)
	__must_hold(__rq_lockp(rq))
{
	kcov_prepare_switch(prev);
	sched_info_switch(rq, prev, ಮುಂದಿನ);
	perf_event_task_sched_ಹೊರಹೋಗು(prev, ಮುಂದಿನ);
	fire_sched_ಹೊರಹೋಗು_preempt_notifiers(prev, ಮುಂದಿನ);
	kmap_local_sched_ಹೊರಹೋಗು();
	prepare_task(ಮುಂದಿನ);
	prepare_arch_switch(ಮುಂದಿನ);
}

/**
 * finish_task_switch - clean up after a task-switch
 * @prev: the thread we just switched away from.
 *
 * finish_task_switch must be called after the context switch, paired
 * with a prepare_task_switch call before the context switch.
 * finish_task_switch will reconcile locking set up by prepare_task_switch,
 * and do any other architecture-specific cleanup actions.
 *
 * Note that we may have delayed dropping an mm in context_switch(). If
 * so, we finish that here outside of the runqueue lock. (Doing it
 * with the lock held can cause deadlocks; see schedule() for
 * details.)
 *
 * The context switch have flipped the stack from under us and restored the
 * local variables which were saved when this task called schedule() in the
 * past. 'prev == current' is still correct but we need to recalculate this_rq
 * because prev may have moved to another CPU.
 */
ಸ್ಥಿರ ರಚನೆ rq *ಮುಕ್ತಾಯ_task_switch(ರಚನೆ task_struct *prev)
	__releases(__rq_lockp(this_rq()))
{
	ರಚನೆ rq *rq = this_rq();
	ರಚನೆ mm_struct *mm = rq->prev_mm;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ prev_state;

	/*
	 * The previous task will have left us with a preempt_count of 2
	 * because it left us after:
	 *
	 *	schedule()
	 *	  preempt_disable();			// 1
	 *	  __schedule()
	 *	    raw_spin_lock_irq(&rq->lock)	// 2
	 *
	 * Also, see FORK_PREEMPT_COUNT.
	 */
	ಆದರೆ (WARN_ONCE(preempt_ಎಣಿಕೆ() != 2*PREEMPT_DISABLE_ಸ್ಥಾನಾಂತರ,
		      "corrupted preempt_count: %s/%d/0x%x\n",
		      current->comm, current->pid, preempt_ಎಣಿಕೆ()))
		preempt_ಎಣಿಕೆ_ಗಣ(FORK_PREEMPT_ಎಣಿಕೆ);

	rq->prev_mm = ಖಾಲಿ;

	/*
	 * A task struct has one reference for the use as "current".
	 * If a task dies, then it sets TASK_DEAD in tsk->state and calls
	 * schedule one last time. The schedule call will never return, and
	 * the scheduled task must drop that reference.
	 *
	 * We must observe prev->state before clearing prev->on_cpu (in
	 * finish_task), otherwise a concurrent wakeup can get prev
	 * running on another CPU and we could rave with its RUNNING -> DEAD
	 * transition, resulting in a double drop.
	 */
	prev_state = READ_ONCE(prev->__state);
	vtime_task_switch(prev);
	perf_event_task_sched_ಒಳ(prev, current);
	ಮುಕ್ತಾಯ_task(prev);
	tick_nohz_task_switch();
	ಮುಕ್ತಾಯ_ಬೀಗ_switch(rq);
	ಮುಕ್ತಾಯ_arch_post_ಬೀಗ_switch();
	kcov_ಮುಕ್ತಾಯ_switch(current);
	/*
	 * kmap_local_sched_out() is invoked with rq::lock held and
	 * interrupts disabled. There is no requirement for that, but the
	 * sched out code does not have an interrupt enabled section.
	 * Restoring the maps on sched in does not require interrupts being
	 * disabled either.
	 */
	kmap_local_sched_ಒಳ();

	/*
	 * Any cached block-layer timestamp (plug->cur_ktime) is stale now,
	 * invalidate it.
	 */
	blk_plug_invalidate_ts();

	fire_sched_ಒಳ_preempt_notifiers(current);
	/*
	 * When switching through a kernel thread, the loop in
	 * membarrier_{private,global}_expedited() may have observed that
	 * kernel thread and not issued an IPI. It is therefore possible to
	 * schedule between user->kernel->user threads without passing though
	 * switch_mm(). Membarrier requires a barrier after storing to
	 * rq->curr, before returning to userspace, so provide them here:
	 *
	 * - a full memory barrier for {PRIVATE,GLOBAL}_EXPEDITED, implicitly
	 *   provided by mmdrop_lazy_tlb(),
	 * - a sync_core for SYNC_CORE.
	 */
	ಆದರೆ (mm) {
		membarrier_mm_sync_core_before_usermode(mm);
		mmdrop_lazy_tlb_sched(mm);
	}

	ಆದರೆ (unlikely(prev_state == TASK_DEAD)) {
		ಆದರೆ (prev->sched_class->task_dead)
			prev->sched_class->task_dead(prev);

		/*
		 * sched_ext_dead() must come before cgroup_task_dead() to
		 * prevent cgroups from being removed while its member tasks are
		 * visible to SCX schedulers.
		 */
		sched_ext_dead(prev);
		cgroup_task_dead(prev);

		/* Task is done with its stack. */
		put_task_ರಾಶಿ(prev);

		put_task_struct_rcu_user(prev);
	}

	ಹಿಂತಿರುಗಿ rq;
}

/**
 * schedule_tail - first thing a freshly forked thread must call.
 * @prev: the thread we just switched away from.
 */
asmlinkage __visible ಶೂನ್ಯ ವೇಳಾಪಟ್ಟಿ_ಬಾಲ(ರಚನೆ task_struct *prev)
	__releases(__rq_lockp(this_rq()))
{
	/*
	 * New tasks start with FORK_PREEMPT_COUNT, see there and
	 * finish_task_switch() for details.
	 *
	 * finish_task_switch() will drop rq->lock() and lower preempt_count
	 * and the preempt_enable() will end up enabling preemption (on
	 * PREEMPT_COUNT kernels).
	 */

	ಮುಕ್ತಾಯ_task_switch(prev);
	/*
	 * This is a special case: the newly created task has just
	 * switched the context for the first time. It is returning from
	 * schedule for the first time in this path.
	 */
	trace_sched_exit_tp(true);
	preempt_enable();

	ಆದರೆ (current->ಗಣ_ಮಗು_tid)
		put_user(task_pid_vnr(current), current->ಗಣ_ಮಗು_tid);

	calculate_sigpending();
}

/*
 * context_switch - switch to the new MM and the new thread's register state.
 */
ಸ್ಥಿರ __always_inline ರಚನೆ rq *
context_switch(ರಚನೆ rq *rq, ರಚನೆ task_struct *prev,
	       ರಚನೆ task_struct *ಮುಂದಿನ, ರಚನೆ rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	prepare_task_switch(rq, prev, ಮುಂದಿನ);

	/*
	 * For paravirt, this is coupled with an exit in switch_to to
	 * combine the page table reload and the switch backend into
	 * one hypercall.
	 */
	arch_ಆರಂಭ_context_switch(prev);

	/*
	 * kernel -> kernel   lazy + transfer active
	 *   user -> kernel   lazy + mmgrab_lazy_tlb() active
	 *
	 * kernel ->   user   switch + mmdrop_lazy_tlb() active
	 *   user ->   user   switch
	 */
	ಆದರೆ (!ಮುಂದಿನ->mm) {				// to kernel
		enter_lazy_tlb(prev->ಸಕ್ರಿಯ_mm, ಮುಂದಿನ);

		ಮುಂದಿನ->ಸಕ್ರಿಯ_mm = prev->ಸಕ್ರಿಯ_mm;
		ಆದರೆ (prev->mm)				// from user
			mmgrab_lazy_tlb(prev->ಸಕ್ರಿಯ_mm);
		ಇಲ್ಲದಿದ್ದರೆ
			prev->ಸಕ್ರಿಯ_mm = ಖಾಲಿ;
	} ಇಲ್ಲದಿದ್ದರೆ {					// to user
		membarrier_switch_mm(rq, prev->ಸಕ್ರಿಯ_mm, ಮುಂದಿನ->mm);
		/*
		 * sys_membarrier() requires an smp_mb() between setting
		 * rq->curr / membarrier_switch_mm() and returning to userspace.
		 *
		 * The below provides this either through switch_mm(), or in
		 * case 'prev->active_mm == next->mm' through
		 * finish_task_switch()'s mmdrop().
		 */
		switch_mm_irqs_off(prev->ಸಕ್ರಿಯ_mm, ಮುಂದಿನ->mm, ಮುಂದಿನ);
		lru_gen_use_mm(ಮುಂದಿನ->mm);

		ಆದರೆ (!prev->mm) {			// from kernel
			/* will mmdrop_lazy_tlb() in finish_task_switch(). */
			rq->prev_mm = prev->ಸಕ್ರಿಯ_mm;
			prev->ಸಕ್ರಿಯ_mm = ಖಾಲಿ;
		}
	}

	mm_cid_switch_ಗಮ್ಯ(prev, ಮುಂದಿನ);

	/*
	 * Tell rseq that the task was scheduled in. Must be after
	 * switch_mm_cid() to get the TIF flag set.
	 */
	rseq_sched_switch_event(ಮುಂದಿನ);

	prepare_ಬೀಗ_switch(rq, ಮುಂದಿನ, rf);

	/* Here we just switch the register state and the stack. */
	switch_ಗಮ್ಯ(prev, ಮುಂದಿನ, prev);
	barrier();

	ಹಿಂತಿರುಗಿ ಮುಕ್ತಾಯ_task_switch(prev);
}

/*
 * nr_running and nr_context_switches:
 *
 * externally visible scheduler statistics: current number of runnable
 * threads, total number of context switches performed since bootup.
 */
ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr_running(ಶೂನ್ಯ)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i, ಮೊತ್ತ = 0;

	for_each_online_cpu(i)
		ಮೊತ್ತ += cpu_rq(i)->nr_running;

	ಹಿಂತಿರುಗಿ ಮೊತ್ತ;
}

/*
 * Check if only the current task is running on the CPU.
 *
 * Caution: this function does not check that the caller has disabled
 * preemption, thus the result might have a time-of-check-to-time-of-use
 * race.  The caller is responsible to use it correctly, for example:
 *
 * - from a non-preemptible section (of course)
 *
 * - from a thread that is bound to a single CPU
 *
 * - in a loop with very short iterations (e.g. a polling loop)
 */
bool single_task_running(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ raw_rq()->nr_running == 1;
}
EXPORT_ಚಿಹ್ನೆ(single_task_running);

ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ nr_context_switches_cpu(ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ cpu_rq(cpu)->nr_switches;
}

ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ nr_context_switches(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ i;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಮೊತ್ತ = 0;

	for_each_possible_cpu(i)
		ಮೊತ್ತ += cpu_rq(i)->nr_switches;

	ಹಿಂತಿರುಗಿ ಮೊತ್ತ;
}

/*
 * Return whether any tasks are accounted as I/O waiters on this CPU.
 * They may wake up on a different CPU.
 */
bool cpu_has_iowait(ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ atomic_read(&cpu_rq(cpu)->nr_iowait) != 0;
}

/*
 * IO-wait accounting, and how it's mostly bollocks (on SMP).
 *
 * The idea behind IO-wait account is to account the idle time that we could
 * have spend running if it were not for IO. That is, if we were to improve the
 * storage performance, we'd have a proportional reduction in IO-wait time.
 *
 * This all works nicely on UP, where, when a task blocks on IO, we account
 * idle time as IO-wait, because if the storage were faster, it could've been
 * running and we'd not be idle.
 *
 * This has been extended to SMP, by doing the same for each CPU. This however
 * is broken.
 *
 * Imagine for instance the case where two tasks block on one CPU, only the one
 * CPU will have IO-wait accounted, while the other has regular idle. Even
 * though, if the storage were faster, both could've ran at the same time,
 * utilising both CPUs.
 *
 * This means, that when looking globally, the current IO-wait accounting on
 * SMP is a lower bound, by reason of under accounting.
 *
 * Worse, since the numbers are provided per CPU, they are sometimes
 * interpreted per CPU, and that is nonsensical. A blocked task isn't strictly
 * associated with any one particular CPU, it can wake to another CPU than it
 * blocked on. This means the per CPU IO-wait number is meaningless.
 *
 * Task CPU affinities can make all that even more 'interesting'.
 */

ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr_iowait(ಶೂನ್ಯ)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i, ಮೊತ್ತ = 0;

	for_each_possible_cpu(i)
		ಮೊತ್ತ += atomic_read(&cpu_rq(i)->nr_iowait);

	ಹಿಂತಿರುಗಿ ಮೊತ್ತ;
}

/*
 * sched_exec - execve() is a valuable balancing opportunity, because at
 * this point the task has the smallest effective memory and cache footprint.
 */
ಶೂನ್ಯ sched_exec(ಶೂನ್ಯ)
{
	ರಚನೆ task_struct *p = current;
	ರಚನೆ migration_arg arg;
	ಪೂರ್ಣಾಂಕ dest_cpu;

	scoped_guard (raw_spinlock_irqsave, &p->pi_ಬೀಗ) {
		dest_cpu = p->sched_class->select_task_rq(p, task_cpu(p), WF_EXEC);
		ಆದರೆ (dest_cpu == smp_processor_id())
			ಹಿಂತಿರುಗಿ;

		ಆದರೆ (unlikely(!cpu_ಸಕ್ರಿಯ(dest_cpu)))
			ಹಿಂತಿರುಗಿ;

		arg = (ರಚನೆ migration_arg){ p, dest_cpu };
	}
	stop_one_cpu(task_cpu(p), migration_cpu_stop, &arg);
}

DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ kernel_stat, kstat);
DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ kernel_cpustat, kernel_cpustat) = {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NO_HZ_COMMON
	.idle_sleeptime_seq = SEQCNT_ZERO(kernel_cpustat.idle_sleeptime_seq)
#ಆದರೆ_ಅಂತ್ಯ
};

EXPORT_ಪ್ರತಿ_CPU_ಚಿಹ್ನೆ(kstat);
EXPORT_ಪ್ರತಿ_CPU_ಚಿಹ್ನೆ(kernel_cpustat);

/*
 * The function fair_sched_class.update_curr accesses the struct curr
 * and its field curr->exec_start; when called from task_sched_runtime(),
 * we observe a high rate of cache misses in practice.
 * Prefetching this data results in improved performance.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ prefetch_curr_exec_ಆರಂಭ(ರಚನೆ task_struct *p)
{
	ರಚನೆ sched_entity *curr = task_rq(p)->cfs.curr;

	prefetch(curr);
	prefetch(&curr->exec_ಆರಂಭ);
}

/*
 * Return accounted runtime for the task.
 * In case the task is currently running, return the runtime plus current's
 * pending runtime that have not been accounted yet.
 */
ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ task_sched_runtime(ರಚನೆ task_struct *p)
{
	ರಚನೆ rq_flags rf;
	ರಚನೆ rq *rq;
	u64 ns;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_64_ಬಿಟ್
	/*
	 * 64-bit doesn't need locks to atomically read a 64-bit value.
	 * So we have a optimization chance when the task's delta_exec is 0.
	 * Reading ->on_cpu is racy, but this is OK.
	 *
	 * If we race with it leaving CPU, we'll take a lock. So we're correct.
	 * If we race with it entering CPU, unaccounted time is 0. This is
	 * indistinguishable from the read occurring a few cycles earlier.
	 * If we see ->on_cpu without ->on_rq, the task is leaving, and has
	 * been accounted, so we're correct here as well.
	 */
	ಆದರೆ (!p->on_cpu || !task_on_rq_queued(p))
		ಹಿಂತಿರುಗಿ p->se.ಮೊತ್ತ_exec_runtime;
#ಆದರೆ_ಅಂತ್ಯ

	rq = task_rq_ಬೀಗ_2(p, &rf);
	/*
	 * Must be ->curr _and_ ->on_rq.  If dequeued, we would
	 * project cycles that may never be accounted to this
	 * thread, breaking clock_gettime().
	 */
	ಆದರೆ (task_current_donor(rq, p) && task_on_rq_queued(p)) {
		prefetch_curr_exec_ಆರಂಭ(p);
		update_rq_clock(rq);
		p->sched_class->update_curr(rq);
	}
	ns = p->se.ಮೊತ್ತ_exec_runtime;
	task_rq_unlock(rq, p, &rf);

	ಹಿಂತಿರುಗಿ ns;
}

ಸ್ಥಿರ u64 cpu_resched_latency(ರಚನೆ rq *rq)
{
	ಪೂರ್ಣಾಂಕ latency_warn_ms = READ_ONCE(sysctl_resched_latency_warn_ms);
	u64 resched_latency, now = rq_clock(rq);
	ಸ್ಥಿರ bool warned_once;

	ಆದರೆ (sysctl_resched_latency_warn_once && warned_once)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!need_resched() || !latency_warn_ms)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (system_state == SYSTEM_BOOTING)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!rq->ಕೊನೆಯ_ನೋಡಿದ_need_resched_ns) {
		rq->ಕೊನೆಯ_ನೋಡಿದ_need_resched_ns = now;
		rq->ticks_without_resched = 0;
		ಹಿಂತಿರುಗಿ 0;
	}

	rq->ticks_without_resched++;
	resched_latency = now - rq->ಕೊನೆಯ_ನೋಡಿದ_need_resched_ns;
	ಆದರೆ (resched_latency <= latency_warn_ms * NSEC_ಪ್ರತಿ_MSEC)
		ಹಿಂತಿರುಗಿ 0;

	warned_once = true;

	ಹಿಂತಿರುಗಿ resched_latency;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init ಸಿದ್ಧತೆ_resched_latency_warn_ms(ಅಕ್ಷರ *str)
{
	ದೀರ್ಘ val;

	ಆದರೆ ((kstrtol(str, 0, &val))) {
		pr_warn("Unable to set resched_latency_warn_ms\n");
		ಹಿಂತಿರುಗಿ 1;
	}

	sysctl_resched_latency_warn_ms = val;
	ಹಿಂತಿರುಗಿ 1;
}
__setup("resched_latency_warn_ms=", ಸಿದ್ಧತೆ_resched_latency_warn_ms);

/*
 * This function gets called by the timer code, with HZ frequency.
 * We call it with interrupts disabled.
 */
ಶೂನ್ಯ sched_tick(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ cpu = smp_processor_id();
	ರಚನೆ rq *rq = cpu_rq(cpu);
	/* scheduler accounting goes to the donor task */
	ರಚನೆ task_struct *curr, *donor;
	ರಚನೆ rq_flags rf;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ hw_pressure;
	u64 resched_latency;

	ಆದರೆ (!cpu_preferred(cpu))
		sched_ತಳ್ಳು_current_non_preferred_cpu(rq);

	ಆದರೆ (housekeeping_cpu(cpu, HK_TYPE_KERNEL_NOISE))
		arch_scale_freq_tick();

	sched_clock_tick();

	rq_ಬೀಗ(rq, &rf);
	curr = rq->curr;
	donor = rq->donor;

	psi_account_irqtime(rq, curr, ಖಾಲಿ);

	update_rq_clock(rq);
	hw_pressure = arch_scale_hw_pressure(cpu_ದ(rq));
	update_hw_ತುಂಬು_avg(rq_clock_task(rq), rq, hw_pressure);

	ಆದರೆ (dynamic_preempt_lazy() && tif_test_ಬಿಟ್(TIF_NEED_RESCHED_LAZY))
		resched_curr(rq);

	donor->sched_class->task_tick(rq, donor, 0);
	ಆದರೆ (sched_feat(LATENCY_WARN))
		resched_latency = cpu_resched_latency(rq);
	calc_global_ತುಂಬು_tick(rq);
	sched_core_tick(rq);
	scx_tick(rq);

	rq_unlock(rq, &rf);

	ಆದರೆ (sched_feat(LATENCY_WARN) && resched_latency)
		resched_latency_warn(cpu, resched_latency);

	perf_event_task_tick();

	ಆದರೆ (curr->flags & PF_WQ_ಕೆಲಸಗಾರ)
		wq_ಕೆಲಸಗಾರ_tick(curr);

	ಆದರೆ (!scx_switched_all()) {
		rq->idle_balance = idle_cpu(cpu);
		sched_balance_trigger(rq);
	}
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NO_HZ_ತುಂಬಿದ

ರಚನೆ tick_work {
	ಪೂರ್ಣಾಂಕ			cpu;
	atomic_t		state;
	ರಚನೆ delayed_work	work;
};
/* Values for ->state, see diagram below. */
#ವ್ಯಾಖ್ಯಾನಿಸು TICK_SCHED_REMOTE_OFFLINE	0
#ವ್ಯಾಖ್ಯಾನಿಸು TICK_SCHED_REMOTE_OFFLINING	1
#ವ್ಯಾಖ್ಯಾನಿಸು TICK_SCHED_REMOTE_RUNNING	2

/*
 * State diagram for ->state:
 *
 *
 *          TICK_SCHED_REMOTE_OFFLINE
 *                    |   ^
 *                    |   |
 *                    |   | sched_tick_remote()
 *                    |   |
 *                    |   |
 *                    +--TICK_SCHED_REMOTE_OFFLINING
 *                    |   ^
 *                    |   |
 * sched_tick_start() |   | sched_tick_stop()
 *                    |   |
 *                    V   |
 *          TICK_SCHED_REMOTE_RUNNING
 *
 *
 * Other transitions get WARN_ON_ONCE(), except that sched_tick_remote()
 * and sched_tick_start() are happy to leave the state in RUNNING.
 */

ಸ್ಥಿರ ರಚನೆ tick_work __percpu *tick_work_cpu;

ಸ್ಥಿರ ಶೂನ್ಯ sched_tick_remote(ರಚನೆ work_struct *work)
{
	ರಚನೆ delayed_work *dwork = ಗಮ್ಯ_delayed_work(work);
	ರಚನೆ tick_work *twork = container_ದ(dwork, ರಚನೆ tick_work, work);
	ಪೂರ್ಣಾಂಕ cpu = twork->cpu;
	ರಚನೆ rq *rq = cpu_rq(cpu);
	ಪೂರ್ಣಾಂಕ os;

	/*
	 * Handle the tick only if it appears the remote CPU is running in full
	 * dynticks mode. The check is racy by nature, but missing a tick or
	 * having one too much is no big deal because the scheduler tick updates
	 * statistics and checks timeslices in a time-independent way, regardless
	 * of when exactly it is running.
	 */
	ಆದರೆ (tick_nohz_tick_stopped_cpu(cpu)) {
		guard(rq_ಬೀಗ_irq)(rq);
		ರಚನೆ task_struct *curr = rq->curr;

		ಆದರೆ (cpu_online(cpu)) {
			/*
			 * Since this is a remote tick for full dynticks mode,
			 * we are always sure that there is no proxy (only a
			 * single task is running).
			 */
			WARN_ON_ONCE(rq->curr != rq->donor);
			update_rq_clock(rq);

			ಆದರೆ (!is_idle_task(curr)) {
				/*
				 * Make sure the next tick runs within a
				 * reasonable amount of time.
				 */
				u64 delta = rq_clock_task(rq) - curr->se.exec_ಆರಂಭ;
				WARN_ON_ONCE(delta > (u64)NSEC_ಪ್ರತಿ_SEC * 30);
			}
			curr->sched_class->task_tick(rq, curr, 0);

			calc_ತುಂಬು_nohz_remote(rq);
		}
	}

	/*
	 * Run the remote tick once per second (1Hz). This arbitrary
	 * frequency is large enough to avoid overload but short enough
	 * to keep scheduler internal stats reasonably up to date.  But
	 * first update state to reflect hotplug activity if required.
	 */
	os = atomic_fetch_ಕೂಡಿಸು_unless(&twork->state, -1, TICK_SCHED_REMOTE_RUNNING);
	WARN_ON_ONCE(os == TICK_SCHED_REMOTE_OFFLINE);
	ಆದರೆ (os == TICK_SCHED_REMOTE_RUNNING)
		ಸರತಿ_delayed_work(system_dfl_wq, dwork, HZ);
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_tick_ಆರಂಭ(ಪೂರ್ಣಾಂಕ cpu)
{
	ಪೂರ್ಣಾಂಕ os;
	ರಚನೆ tick_work *twork;

	ಆದರೆ (housekeeping_cpu(cpu, HK_TYPE_KERNEL_NOISE))
		ಹಿಂತಿರುಗಿ;

	WARN_ON_ONCE(!tick_work_cpu);

	twork = ಪ್ರತಿ_cpu_ptr(tick_work_cpu, cpu);
	os = atomic_xchg(&twork->state, TICK_SCHED_REMOTE_RUNNING);
	WARN_ON_ONCE(os == TICK_SCHED_REMOTE_RUNNING);
	ಆದರೆ (os == TICK_SCHED_REMOTE_OFFLINE) {
		twork->cpu = cpu;
		INIT_DELAYED_WORK(&twork->work, sched_tick_remote);
		ಸರತಿ_delayed_work(system_dfl_wq, &twork->work, HZ);
	}
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_HOTPLUG_CPU
ಸ್ಥಿರ ಶೂನ್ಯ sched_tick_stop(ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ tick_work *twork;
	ಪೂರ್ಣಾಂಕ os;

	ಆದರೆ (housekeeping_cpu(cpu, HK_TYPE_KERNEL_NOISE))
		ಹಿಂತಿರುಗಿ;

	WARN_ON_ONCE(!tick_work_cpu);

	twork = ಪ್ರತಿ_cpu_ptr(tick_work_cpu, cpu);
	/* There cannot be competing actions, but don't rely on stop-machine. */
	os = atomic_xchg(&twork->state, TICK_SCHED_REMOTE_OFFLINING);
	WARN_ON_ONCE(os != TICK_SCHED_REMOTE_RUNNING);
	/* Don't cancel, as this would mess up the state machine. */
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_HOTPLUG_CPU */

ಪೂರ್ಣಾಂಕ __init sched_tick_offload_init(ಶೂನ್ಯ)
{
	tick_work_cpu = alloc_percpu(ರಚನೆ tick_work);
	BUG_ON(!tick_work_cpu);
	ಹಿಂತಿರುಗಿ 0;
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_NO_HZ_FULL: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_tick_ಆರಂಭ(ಪೂರ್ಣಾಂಕ cpu) { }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_tick_stop(ಪೂರ್ಣಾಂಕ cpu) { }
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_NO_HZ_FULL */

#ಆದರೆ defined(CONFIG_PREEMPTION) && (defined(CONFIG_DEBUG_PREEMPT) || \
				defined(CONFIG_TRACE_PREEMPT_TOGGLE))
/*
 * If the value passed in is equal to the current preempt count
 * then we just disabled preemption. Start timing the latency.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ preempt_latency_ಆರಂಭ(ಪೂರ್ಣಾಂಕ val)
{
	ಆದರೆ (preempt_ಎಣಿಕೆ() == val) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ip = get_ಬೀಗ_ಪೋಷಕ_ip();
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_DEBUG_PREEMPT
		current->preempt_disable_ip = ip;
#ಆದರೆ_ಅಂತ್ಯ
		trace_preempt_off(CALLER_ADDR0, ip);
	}
}

ಶೂನ್ಯ preempt_ಎಣಿಕೆ_ಕೂಡಿಸು(ಪೂರ್ಣಾಂಕ val)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_DEBUG_PREEMPT
	/*
	 * Underflow?
	 *
	 * Cannot detect underflow based on the current preempt_count() value
	 * if using HAS_SEPARATE_PREEMPT_RESCHED_BITS because preempt count takes all 32
	 * bits.
	 */
	ಆದರೆ (!IS_ENABLED(CONFIG_HAS_SEPARATE_PREEMPT_RESCHED_ಬಿಟ್ಟುಗಳು) &&
	    DEBUG_LOCKS_WARN_ON((preempt_ಎಣಿಕೆ() < 0)))
		ಹಿಂತಿರುಗಿ;
#ಆದರೆ_ಅಂತ್ಯ
	__preempt_count_add(val);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_DEBUG_PREEMPT
	/*
	 * Spinlock count overflowing soon?
	 */
	DEBUG_LOCKS_WARN_ON((preempt_ಎಣಿಕೆ() & PREEMPT_ಮುಖವಾಡ) >=
				PREEMPT_ಮುಖವಾಡ - 10);
#ಆದರೆ_ಅಂತ್ಯ
	preempt_latency_ಆರಂಭ(val);
}
EXPORT_ಚಿಹ್ನೆ(preempt_ಎಣಿಕೆ_ಕೂಡಿಸು);
NOKPROBE_ಚಿಹ್ನೆ(preempt_ಎಣಿಕೆ_ಕೂಡಿಸು);

/*
 * If the value passed in equals to the current preempt count
 * then we just enabled preemption. Stop timing the latency.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ preempt_latency_stop(ಪೂರ್ಣಾಂಕ val)
{
	ಆದರೆ (preempt_ಎಣಿಕೆ() == val)
		trace_preempt_on(CALLER_ADDR0, get_ಬೀಗ_ಪೋಷಕ_ip());
}

ಶೂನ್ಯ preempt_ಎಣಿಕೆ_sub(ಪೂರ್ಣಾಂಕ val)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_DEBUG_PREEMPT
	/*
	 * Underflow?
	 */
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ uval = val;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ pc = preempt_ಎಣಿಕೆ();

	ಆದರೆ (DEBUG_LOCKS_WARN_ON(pc - uval > pc))
		ಹಿಂತಿರುಗಿ;
	/*
	 * Is the spinlock portion underflowing?
	 */
	ಆದರೆ (DEBUG_LOCKS_WARN_ON((val < PREEMPT_ಮುಖವಾಡ) &&
			!(preempt_ಎಣಿಕೆ() & PREEMPT_ಮುಖವಾಡ)))
		ಹಿಂತಿರುಗಿ;
#ಆದರೆ_ಅಂತ್ಯ

	preempt_latency_stop(val);
	__preempt_count_sub(val);
}
EXPORT_ಚಿಹ್ನೆ(preempt_ಎಣಿಕೆ_sub);
NOKPROBE_ಚಿಹ್ನೆ(preempt_ಎಣಿಕೆ_sub);

#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ preempt_latency_ಆರಂಭ(ಪೂರ್ಣಾಂಕ val) { }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ preempt_latency_stop(ಪೂರ್ಣಾಂಕ val) { }
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ get_preempt_disable_ip(ರಚನೆ task_struct *p)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_DEBUG_PREEMPT
	ಹಿಂತಿರುಗಿ p->preempt_disable_ip;
#ಇಲ್ಲದಿದ್ದರೆ
	ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ
}

/*
 * Print scheduling while atomic bug:
 */
ಸ್ಥಿರ noinline ಶೂನ್ಯ __schedule_bug(ರಚನೆ task_struct *prev)
{
	/* Save this before calling printk(), since that will clobber it */
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ preempt_disable_ip = get_preempt_disable_ip(current);

	ಆದರೆ (oops_ಒಳ_ಪ್ರಗತಿ)
		ಹಿಂತಿರುಗಿ;

	printk(KERN_ERR "BUG: scheduling while atomic: %s/%d/0x%08x\n",
		prev->comm, prev->pid, preempt_ಎಣಿಕೆ());

	debug_show_held_locks(prev);
	print_modules();
	ಆದರೆ (irqs_disabled())
		print_irqtrace_events(prev);
	ಆದರೆ (IS_ENABLED(CONFIG_DEBUG_PREEMPT)) {
		pr_err("Preemption disabled at:");
		print_ip_sym(KERN_ERR, preempt_disable_ip);
	}
	check_panic_on_warn("scheduling while atomic");

	dump_ರಾಶಿ();
	ಕೂಡಿಸು_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}

/*
 * Various schedule()-time debugging checks and statistics:
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ವೇಳಾಪಟ್ಟಿ_debug(ರಚನೆ task_struct *prev, bool preempt)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_ರಾಶಿ_ಕೊನೆ_CHECK
	ಆದರೆ (task_ರಾಶಿ_ಕೊನೆ_corrupted(prev))
		panic("corrupted stack end detected inside scheduler\n");

	ಆದರೆ (task_scs_ಕೊನೆ_corrupted(prev))
		panic("corrupted shadow stack detected inside scheduler\n");
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_DEBUG_ATOMIC_SLEEP
	ಆದರೆ (!preempt && READ_ONCE(prev->__state) && prev->non_ಖಂಡ_ಎಣಿಕೆ) {
		printk(KERN_ERR "BUG: scheduling in a non-blocking section: %s/%d/%i\n",
			prev->comm, prev->pid, prev->non_ಖಂಡ_ಎಣಿಕೆ);
		dump_ರಾಶಿ();
		ಕೂಡಿಸು_taint(TAINT_WARN, LOCKDEP_STILL_OK);
	}
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (unlikely(ಒಳ_atomic_preempt_off())) {
		__schedule_bug(prev);
		preempt_ಎಣಿಕೆ_ಗಣ(PREEMPT_DISABLED);
	}
	rcu_sleep_check();
	WARN_ON_ONCE(ct_state() == CT_STATE_USER);

	profile_ದೊರಕಿತು(SCHED_PROFILING, __builtin_return_address(0));

	schedstat_inc(this_rq()->sched_ಎಣಿಕೆ);
}

ಸ್ಥಿರ ಶೂನ್ಯ prev_balance(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *ಆರಂಭ_class = rq->donor->sched_class;
	ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *class;

	/*
	 * We must do the balancing pass before put_prev_task(), such
	 * that when we release the rq->lock the task is in the same
	 * state as before we took rq->lock.
	 *
	 * We can terminate the balance pass as soon as we know there is
	 * a runnable task of @class priority or higher.
	 */
	for_ಸಕ್ರಿಯ_class_range(class, ಆರಂಭ_class, &idle_sched_class) {
		ಆದರೆ (class->balance && class->balance(rq, rf))
			ನಿಲ್ಲಿಸು;
	}
}

/*
 * Pick up the highest-prio task:
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ task_struct *
__pick_next_task(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *class;
	ರಚನೆ task_struct *p;

	rq->dl_server = ಖಾಲಿ;

	ಆದರೆ (scx_enabled())
		ಹೋಗು restart;

	/*
	 * Optimization: we know that if all tasks are in the fair class we can
	 * call that function directly, but only if the @prev task wasn't of a
	 * higher scheduling class, because otherwise those lose the
	 * opportunity to pull in more work from other CPUs.
	 */
	ಆದರೆ (likely(!sched_class_above(rq->donor->sched_class, &fair_sched_class) &&
		   rq->nr_running == rq->cfs.h_nr_queued)) {

		p = pick_task_fair(rq, rf);
		ಆದರೆ (unlikely(p == RETRY_TASK))
			ಹೋಗು restart;

		/* Assume the next prioritized class is idle_sched_class */
		ಆದರೆ (!p)
			p = pick_task_idle(rq, rf);

		put_prev_ಗಣ_ಮುಂದಿನ_task(rq, rq->donor, p);
		ಹಿಂತಿರುಗಿ p;
	}

restart:
	prev_balance(rq, rf);

	for_each_ಸಕ್ರಿಯ_class(class) {
		p = class->pick_task(rq, rf);
		ಆದರೆ (unlikely(p == RETRY_TASK))
			ಹೋಗು restart;
		ಆದರೆ (p) {
			put_prev_ಗಣ_ಮುಂದಿನ_task(rq, rq->donor, p);
			ಹಿಂತಿರುಗಿ p;
		}
	}

	BUG(); /* The idle class should always have a runnable task. */
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CORE
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool is_task_rq_idle(ರಚನೆ task_struct *t)
{
	ಹಿಂತಿರುಗಿ (task_rq(t)->idle == t);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool cookie_equals(ರಚನೆ task_struct *a, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cookie)
{
	ಹಿಂತಿರುಗಿ is_task_rq_idle(a) || (a->core_cookie == cookie);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool cookie_ಹೊಂದಿಕೆ(ರಚನೆ task_struct *a, ರಚನೆ task_struct *b)
{
	ಆದರೆ (is_task_rq_idle(a) || is_task_rq_idle(b))
		ಹಿಂತಿರುಗಿ true;

	ಹಿಂತಿರುಗಿ a->core_cookie == b->core_cookie;
}

/*
 * Careful; this can return RETRY_TASK, it does not include the retry-loop
 * itself due to the whole SMT pick retry thing below.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ task_struct *pick_task(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *class;
	ರಚನೆ task_struct *p;

	rq->dl_server = ಖಾಲಿ;

	for_each_ಸಕ್ರಿಯ_class(class) {
		p = class->pick_task(rq, rf);
		ಆದರೆ (p)
			ಹಿಂತಿರುಗಿ p;
	}

	BUG(); /* The idle class should always have a runnable task. */
}

ಬಾಹ್ಯ ಶೂನ್ಯ task_vruntime_update(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, bool ಒಳ_fi);

ಸ್ಥಿರ ಶೂನ್ಯ ಸರತಿ_core_balance(ರಚನೆ rq *rq);

ಸ್ಥಿರ ರಚನೆ task_struct *
pick_ಮುಂದಿನ_task(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	ರಚನೆ task_struct *ಮುಂದಿನ, *p, *max;
	ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *smt_ಮುಖವಾಡ;
	bool fi_before = false;
	bool core_clock_updated = (rq == rq->core);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cookie;
	ಪೂರ್ಣಾಂಕ i, cpu, occ = 0;
	ರಚನೆ rq *rq_i;
	bool need_sync = false;

	ಆದರೆ (!sched_core_enabled(rq))
		ಹಿಂತಿರುಗಿ __pick_next_task(rq, rf);

	cpu = cpu_ದ(rq);

	/* Stopper task is switching into idle, no need core-wide selection. */
	ಆದರೆ (cpu_is_offline(cpu)) {
		/*
		 * Reset core_pick so that we don't enter the fastpath when
		 * coming online. core_pick would already be migrated to
		 * another cpu during offline.
		 */
		rq->core_pick = ಖಾಲಿ;
		rq->core_dl_server = ಖಾಲಿ;
		ಹಿಂತಿರುಗಿ __pick_next_task(rq, rf);
	}

	rq->core->core_pick_ಒಳ_flight++;

	/*
	 * If there were no {en,de}queues since we picked (IOW, the task
	 * pointers are all still valid), and we haven't scheduled the last
	 * pick yet, do so now.
	 *
	 * rq->core_pick can be NULL if no selection was made for a CPU because
	 * it was either offline or went offline during a sibling's core-wide
	 * selection. In this case, do a core-wide selection.
	 */
	ಆದರೆ (rq->core->core_pick_seq == rq->core->core_task_seq &&
	    rq->core_pick) {
		ಮುಂದಿನ = rq->core_pick;
		rq->dl_server = rq->core_dl_server;
		rq->core_pick = ಖಾಲಿ;
		rq->core_dl_server = ಖಾಲಿ;
		ಹೋಗು ಹೊರಹೋಗು_ಗಣ_ಮುಂದಿನ;
	}

	prev_balance(rq, rf);

	smt_ಮುಖವಾಡ = cpu_smt_ಮುಖವಾಡ(cpu);

restart:
	need_sync |= !!rq->core->core_cookie;

	/* reset state */
	rq->core->core_cookie = 0UL;
	ಆದರೆ (rq->core->core_forceidle_ಎಣಿಕೆ) {
		ಆದರೆ (!core_clock_updated) {
			update_rq_clock(rq->core);
			core_clock_updated = true;
		}
		sched_core_account_forceidle(rq);
		/* reset after accounting force idle */
		rq->core->core_forceidle_ಆರಂಭ = 0;
		rq->core->core_forceidle_ಎಣಿಕೆ = 0;
		rq->core->core_forceidle_occupation = 0;
		need_sync = true;
		fi_before = true;
	}

	/*
	 * core->core_task_seq, core->core_pick_seq
	 *
	 * @task_seq guards the task state ({en,de}queues)
	 * @pick_seq is the @task_seq we did a selection on
	 *
	 * Once a core-wide selection is committed, a non-NULL core_pick denotes
	 * a pick which still needs to be consumed on this CPU.
	 *
	 * However, preemptions can cause multiple picks on the same task set.
	 * 'Fix' this by also increasing @task_seq for every pick.
	 */
	rq->core->core_task_seq++;

	/*
	 * Optimize for common case where this CPU has no cookies
	 * and there are no cookied tasks running on siblings.
	 */
	ಆದರೆ (!need_sync) {
		ಮುಂದಿನ = pick_task(rq, rf);
		ಆದರೆ (unlikely(ಮುಂದಿನ == RETRY_TASK)) {
			/* rq lock may have been dropped, clocks invalidated */
			core_clock_updated = false;
			ಆದರೆ (!(rq->clock_update_flags & RQCF_UPDATED))
				update_rq_clock(rq);
			ಹೋಗು restart;
		}

		ಆದರೆ (!ಮುಂದಿನ->core_cookie) {
			rq->core_pick = ಖಾಲಿ;
			rq->core_dl_server = ಖಾಲಿ;
			/*
			 * For robustness, update the min_vruntime_fi for
			 * unconstrained picks as well.
			 */
			WARN_ON_ONCE(fi_before);
			task_vruntime_update(rq, ಮುಂದಿನ, false);
			ಹೋಗು ಹೊರಹೋಗು_ಗಣ_ಮುಂದಿನ;
		}
	}

	/*
	 * For each thread: do the regular task pick and find the max prio task
	 * amongst them.
	 *
	 * Tie-break prio towards the current CPU
	 */
	max = ಖಾಲಿ;
	for_each_cpu_wrap(i, smt_ಮುಖವಾಡ, cpu) {
		rq_i = cpu_rq(i);

		/*
		 * Current cpu always has its clock updated on entrance to
		 * pick_next_task(). If the current cpu is not the core,
		 * the core may also have been updated above.
		 */
		ಆದರೆ (i != cpu && (rq_i != rq->core || !core_clock_updated))
			update_rq_clock(rq_i);

		p = pick_task(rq_i, rf);
		ಆದರೆ (unlikely(p == RETRY_TASK)) {
			/* rq lock may have been dropped, clocks invalidated */
			core_clock_updated = false;
			ಆದರೆ (!(rq->clock_update_flags & RQCF_UPDATED))
				update_rq_clock(rq);
			ಹೋಗು restart;
		}

		rq_i->core_pick = p;
		rq_i->core_dl_server = rq_i->dl_server;

		ಆದರೆ (!max || prio_less(max, p, fi_before))
			max = p;
	}

	cookie = rq->core->core_cookie = max->core_cookie;

	/*
	 * For each thread: try and find a runnable task that matches @max or
	 * force idle.
	 */
	for_each_cpu(i, smt_ಮುಖವಾಡ) {
		rq_i = cpu_rq(i);
		p = rq_i->core_pick;

		ಆದರೆ (!cookie_equals(p, cookie)) {
			p = ಖಾಲಿ;
			ಆದರೆ (cookie)
				p = sched_core_ಕಂಡುಹಿಡಿ(rq_i, cookie);
			ಆದರೆ (!p)
				p = idle_sched_class.pick_task(rq_i, rf);
		}

		rq_i->core_pick = p;
		rq_i->core_dl_server = ಖಾಲಿ;

		ಆದರೆ (p == rq_i->idle) {
			ಆದರೆ (rq_i->nr_running) {
				rq->core->core_forceidle_ಎಣಿಕೆ++;
				ಆದರೆ (!fi_before)
					rq->core->core_forceidle_seq++;
			}
		} ಇಲ್ಲದಿದ್ದರೆ {
			occ++;
		}
	}

	ಆದರೆ (schedstat_enabled() && rq->core->core_forceidle_ಎಣಿಕೆ) {
		rq->core->core_forceidle_ಆರಂಭ = rq_clock(rq->core);
		rq->core->core_forceidle_occupation = occ;
	}

	rq->core->core_pick_seq = rq->core->core_task_seq;
	ಮುಂದಿನ = rq->core_pick;

	/* Something should have been selected for current CPU */
	WARN_ON_ONCE(!ಮುಂದಿನ);

	/*
	 * Reschedule siblings
	 *
	 * NOTE: L1TF -- at this point we're no longer running the old task and
	 * sending an IPI (below) ensures the sibling will no longer be running
	 * their task. This ensures there is no inter-sibling overlap between
	 * non-matching user state.
	 */
	for_each_cpu(i, smt_ಮುಖವಾಡ) {
		rq_i = cpu_rq(i);

		/*
		 * An online sibling might have gone offline before a task
		 * could be picked for it, or it might be offline but later
		 * happen to come online, but its too late and nothing was
		 * picked for it.  That's Ok - it will pick tasks for itself,
		 * so ignore it.
		 */
		ಆದರೆ (!rq_i->core_pick)
			ಮುಂದುವರಿ;

		/*
		 * Update for new !FI->FI transitions, or if continuing to be in !FI:
		 * fi_before     fi      update?
		 *  0            0       1
		 *  0            1       1
		 *  1            0       1
		 *  1            1       0
		 */
		ಆದರೆ (!(fi_before && rq->core->core_forceidle_ಎಣಿಕೆ))
			task_vruntime_update(rq_i, rq_i->core_pick, !!rq->core->core_forceidle_ಎಣಿಕೆ);

		rq_i->core_pick->core_occupation = occ;

		ಆದರೆ (i == cpu) {
			rq_i->core_pick = ಖಾಲಿ;
			rq_i->core_dl_server = ಖಾಲಿ;
			ಮುಂದುವರಿ;
		}

		/* Did we break L1TF mitigation requirements? */
		WARN_ON_ONCE(!cookie_ಹೊಂದಿಕೆ(ಮುಂದಿನ, rq_i->core_pick));

		ಆದರೆ (rq_i->curr == rq_i->core_pick) {
			rq_i->core_pick = ಖಾಲಿ;
			rq_i->core_dl_server = ಖಾಲಿ;
			ಮುಂದುವರಿ;
		}

		resched_curr(rq_i);
	}

ಹೊರಹೋಗು_ಗಣ_ಮುಂದಿನ:
	rq->core->core_pick_ಒಳ_flight--;
	put_prev_ಗಣ_ಮುಂದಿನ_task(rq, rq->donor, ಮುಂದಿನ);
	ಆದರೆ (rq->core->core_forceidle_ಎಣಿಕೆ && ಮುಂದಿನ == rq->idle)
		ಸರತಿ_core_balance(rq);

	ಹಿಂತಿರುಗಿ ಮುಂದಿನ;
}

ಸ್ಥಿರ bool try_steal_cookie(ಪೂರ್ಣಾಂಕ this, ಪೂರ್ಣಾಂಕ that)
{
	ರಚನೆ rq *dst = cpu_rq(this), *src = cpu_rq(that);
	ರಚನೆ task_struct *p;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cookie;
	bool success = false;

	guard(irq)();
	guard(double_rq_ಬೀಗ)(dst, src);

	cookie = dst->core->core_cookie;
	ಆದರೆ (!cookie)
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (dst->curr != dst->idle)
		ಹಿಂತಿರುಗಿ false;

	p = sched_core_ಕಂಡುಹಿಡಿ(src, cookie);
	ಆದರೆ (!p)
		ಹಿಂತಿರುಗಿ false;

	ಮಾಡು {
		ಆದರೆ (p == src->core_pick || p == src->curr || p == src->donor)
			ಹೋಗು ಮುಂದಿನ;

		ಆದರೆ (task_is_blocked(p))
			ಹೋಗು ಮುಂದಿನ;

		ಆದರೆ (!is_cpu_allowed(p, this))
			ಹೋಗು ಮುಂದಿನ;

		ಆದರೆ (p->core_occupation > dst->idle->core_occupation)
			ಹೋಗು ಮುಂದಿನ;
		/*
		 * sched_core_find() and sched_core_next() will ensure
		 * that task @p is not throttled now, we also need to
		 * check whether the runqueue of the destination CPU is
		 * being throttled.
		 */
		ಆದರೆ (sched_task_is_throttled(p, this))
			ಹೋಗು ಮುಂದಿನ;

		move_queued_task_locked(src, dst, p);
		resched_curr(dst);

		success = true;
		ನಿಲ್ಲಿಸು;

ಮುಂದಿನ:
		p = sched_core_ಮುಂದಿನ(p, cookie);
	} ಆಗ (p);

	ಹಿಂತಿರುಗಿ success;
}

ಸ್ಥಿರ bool steal_cookie_task(ಪೂರ್ಣಾಂಕ cpu, ರಚನೆ sched_domain *sd)
{
	ಪೂರ್ಣಾಂಕ i;

	for_each_cpu_wrap(i, sched_domain_ಹರವು(sd), cpu + 1) {
		ಆದರೆ (i == cpu)
			ಮುಂದುವರಿ;

		ಆದರೆ (need_resched())
			ನಿಲ್ಲಿಸು;

		ಆದರೆ (try_steal_cookie(cpu, i))
			ಹಿಂತಿರುಗಿ true;
	}

	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_core_balance(ರಚನೆ rq *rq)
	__must_hold(__rq_lockp(rq))
{
	ರಚನೆ sched_domain *sd;
	ಪೂರ್ಣಾಂಕ cpu = cpu_ದ(rq);

	guard(preempt)();
	guard(rcu)();

	raw_spin_rq_unlock_irq(rq);
	for_each_domain(cpu, sd) {
		ಆದರೆ (need_resched())
			ನಿಲ್ಲಿಸು;

		ಆದರೆ (steal_cookie_task(cpu, sd))
			ನಿಲ್ಲಿಸು;
	}
	raw_spin_rq_ಬೀಗ_irq(rq);
}

ಸ್ಥಿರ DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ balance_callback, core_balance_ತಲೆ);

ಸ್ಥಿರ ಶೂನ್ಯ ಸರತಿ_core_balance(ರಚನೆ rq *rq)
{
	ಆದರೆ (!sched_core_enabled(rq))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!rq->core->core_cookie)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!rq->nr_running) /* not forced idle */
		ಹಿಂತಿರುಗಿ;

	ಸರತಿ_balance_callback(rq, &ಪ್ರತಿ_cpu(core_balance_ತಲೆ, rq->cpu), sched_core_balance);
}

DEFINE_ಬೀಗ_GUARD_1(core_ಬೀಗ, ಪೂರ್ಣಾಂಕ,
		    sched_core_ಬೀಗ(*_T->ಬೀಗ, &_T->flags),
		    sched_core_unlock(*_T->ಬೀಗ, &_T->flags),
		    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags)

ಸ್ಥಿರ ಶೂನ್ಯ sched_core_cpu_starting(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *smt_ಮುಖವಾಡ = cpu_smt_ಮುಖವಾಡ(cpu);
	ರಚನೆ rq *rq = cpu_rq(cpu), *core_rq = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ t;

	guard(core_ಬೀಗ)(&cpu);

	WARN_ON_ONCE(rq->core != rq);

	/* if we're the first, we'll be our own leader */
	ಆದರೆ (cpumask_ತೂಕ(smt_ಮುಖವಾಡ) == 1)
		ಹಿಂತಿರುಗಿ;

	/* find the leader */
	for_each_cpu(t, smt_ಮುಖವಾಡ) {
		ಆದರೆ (t == cpu)
			ಮುಂದುವರಿ;
		rq = cpu_rq(t);
		ಆದರೆ (rq->core == rq) {
			core_rq = rq;
			ನಿಲ್ಲಿಸು;
		}
	}

	ಆದರೆ (WARN_ON_ONCE(!core_rq)) /* whoopsie */
		ಹಿಂತಿರುಗಿ;

	/* install and validate core_rq */
	for_each_cpu(t, smt_ಮುಖವಾಡ) {
		rq = cpu_rq(t);

		ಆದರೆ (t == cpu)
			rq->core = core_rq;

		WARN_ON_ONCE(rq->core != core_rq);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_core_cpu_deactivate(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *smt_ಮುಖವಾಡ = cpu_smt_ಮುಖವಾಡ(cpu);
	ರಚನೆ rq *rq = cpu_rq(cpu), *core_rq = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ t;

	guard(core_ಬೀಗ)(&cpu);

	/* if we're the last man standing, nothing to do */
	ಆದರೆ (cpumask_ತೂಕ(smt_ಮುಖವಾಡ) == 1) {
		WARN_ON_ONCE(rq->core != rq);
		ಹಿಂತಿರುಗಿ;
	}

	/* if we're not the leader, nothing to do */
	ಆದರೆ (rq->core != rq)
		ಹಿಂತಿರುಗಿ;

	/* find a new leader */
	for_each_cpu(t, smt_ಮುಖವಾಡ) {
		ಆದರೆ (t == cpu)
			ಮುಂದುವರಿ;
		core_rq = cpu_rq(t);
		ನಿಲ್ಲಿಸು;
	}

	ಆದರೆ (WARN_ON_ONCE(!core_rq)) /* impossible */
		ಹಿಂತಿರುಗಿ;

	/* copy the shared state to the new leader */
	core_rq->core_task_seq             = rq->core_task_seq;
	core_rq->core_pick_seq             = rq->core_pick_seq;
	core_rq->core_cookie               = rq->core_cookie;
	core_rq->core_forceidle_ಎಣಿಕೆ      = rq->core_forceidle_ಎಣಿಕೆ;
	core_rq->core_forceidle_seq        = rq->core_forceidle_seq;
	core_rq->core_forceidle_occupation = rq->core_forceidle_occupation;

	/*
	 * A stale leftover would bias the count forever if this CPU later
	 * returns as its own leader. Move, don't copy.
	 */
	core_rq->core_pick_ಒಳ_flight       = rq->core_pick_ಒಳ_flight;
	rq->core_pick_ಒಳ_flight            = 0;

	/*
	 * Accounting edge for forced idle is handled in pick_next_task().
	 * Don't need another one here, since the hotplug thread shouldn't
	 * have a cookie.
	 */
	core_rq->core_forceidle_ಆರಂಭ = 0;

	/* install new leader */
	for_each_cpu(t, smt_ಮುಖವಾಡ) {
		rq = cpu_rq(t);
		rq->core = core_rq;
	}
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_core_cpu_dying(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);

	ಆದರೆ (rq->core != rq)
		rq->core = rq;
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_SCHED_CORE: */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_core_cpu_starting(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_core_cpu_deactivate(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_core_cpu_dying(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu) {}

ಸ್ಥಿರ ರಚನೆ task_struct *
pick_ಮುಂದಿನ_task(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	ಹಿಂತಿರುಗಿ __pick_next_task(rq, rf);
}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_SCHED_CORE */

/*
 * Constants for the sched_mode argument of __schedule().
 *
 * The mode argument allows RT enabled kernels to differentiate a
 * preemption from blocking on an 'sleeping' spin/rwlock.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು SM_IDLE			(-1)
#ವ್ಯಾಖ್ಯಾನಿಸು SM_NONE			0
#ವ್ಯಾಖ್ಯಾನಿಸು SM_PREEMPT		1
#ವ್ಯಾಖ್ಯಾನಿಸು SM_RTLOCK_WAIT		2

/*
 * Helper function for __schedule()
 *
 * Tries to deactivate the task, unless the should_block arg
 * is false or if a signal is pending. In the case a signal
 * is pending, marks the task's __state as RUNNING (and clear
 * blocked_on).
 */
ಸ್ಥಿರ bool try_ಗಮ್ಯ_ಖಂಡ_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p,
			      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *task_state_p, bool should_ಖಂಡ)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ task_state = *task_state_p;

	WARN_ON_ONCE(p->is_blocked);

	ಆದರೆ (signal_pending_state(task_state, p)) {
		WRITE_ONCE(p->__state, TASK_RUNNING);
		*task_state_p = TASK_RUNNING;
		clear_task_blocked_on(p, ಖಾಲಿ);

		ಹಿಂತಿರುಗಿ false;
	}

	p->is_blocked = 1;

	/*
	 * We check should_block after signal_pending because we
	 * will want to wake the task in that case. But if
	 * should_block is false, its likely due to the task being
	 * blocked on a mutex, and we want to keep it on the runqueue
	 * to be selectable for proxy-execution.
	 */
	ಆದರೆ (!should_ಖಂಡ)
		ಹಿಂತಿರುಗಿ false;

	ಖಂಡ_task(rq, p, task_state);
	ಹಿಂತಿರುಗಿ true;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_PROXY_EXEC
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ proxy_ಗಣ_task_cpu(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ wake_cpu;

	/*
	 * Since we are enqueuing a blocked task on a cpu it may
	 * not be able to run on, preserve wake_cpu when we
	 * __set_task_cpu so we can return the task to where it
	 * was previously runnable.
	 */
	wake_cpu = p->wake_cpu;
	__set_task_cpu(p, cpu);
	p->wake_cpu = wake_cpu;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ task_struct *proxy_resched_idle(ರಚನೆ rq *rq)
{
	put_prev_ಗಣ_ಮುಂದಿನ_task(rq, rq->donor, rq->idle);
	rq->ಮುಂದಿನ_class = &idle_sched_class;
	rq_ಗಣ_donor(rq, rq->idle);
	ಗಣ_tsk_need_resched(rq->idle);
	ಹಿಂತಿರುಗಿ rq->idle;
}

ಸ್ಥಿರ ಶೂನ್ಯ proxy_deactivate(ರಚನೆ rq *rq, ರಚನೆ task_struct *donor)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ state = READ_ONCE(donor->__state);

	WARN_ON_ONCE(state == TASK_RUNNING);
	WARN_ON_ONCE(donor->blocked_on);
	/*
	 * Because we got donor from pick_next_task(), it is *crucial*
	 * that we call proxy_resched_idle() before we deactivate it.
	 * As once we deactivate donor, donor->on_rq is set to zero,
	 * which allows ttwu() to immediately try to wake the task on
	 * another rq. So we cannot use *any* references to donor
	 * after that point. So things like cfs_rq->curr or rq->donor
	 * need to be changed from next *before* we deactivate.
	 */
	proxy_resched_idle(rq);
	ಖಂಡ_task(rq, donor, state);
}

/*
 * Remove a retained proxy donor before changing its scheduler ownership.
 * The caller holds p->pi_lock, so p cannot wake and migrate if block_task()
 * drops it from the runqueue. If DELAY_DEQUEUE keeps a blocked fair task
 * queued, switching_from_fair() completes the dequeue in the immediately
 * following sched_change_begin().
 */
ಶೂನ್ಯ sched_proxy_ಖಂಡ_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ state = READ_ONCE(p->__state);

	lockdep_assert_held(&p->pi_ಬೀಗ);
	lockdep_assert_rq_held(rq);

	ಆದರೆ (!p->is_blocked || !task_on_rq_queued(p))
		ಹಿಂತಿರುಗಿ;
	ಆದರೆ (WARN_ON_ONCE(state == TASK_RUNNING))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (task_current_donor(rq, p))
		proxy_reset_donor(rq);

	ಆದರೆ (!p->se.sched_delayed)
		ಖಂಡ_task(rq, p, state);

	WARN_ON_ONCE(task_on_rq_queued(p) && !p->se.sched_delayed);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ proxy_release_rq_ಬೀಗ(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	/*
	 * The class scheduler may have queued a balance callback
	 * from pick_next_task() called earlier.
	 *
	 * So here we have to zap callbacks before unlocking the rq
	 * as another CPU may jump in and call sched_balance_rq
	 * which can trip the warning in rq_pin_lock() if we
	 * leave callbacks set.
	 *
	 * After we later reaquire the rq lock, we will force __schedule()
	 * to pick_again, so the callbacks will get re-established.
	 */
	zap_balance_callbacks(rq);
	rq_unpin_ಬೀಗ(rq, rf);
	raw_spin_rq_unlock(rq);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ proxy_reacquire_rq_ಬೀಗ(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_ಬೀಗ(rq);
	rq_repin_ಬೀಗ(rq, rf);
	update_rq_clock(rq);
}

/*
 * If the blocked-on relationship crosses CPUs, migrate @p to the
 * owner's CPU.
 *
 * This is because we must respect the CPU affinity of execution
 * contexts (owner) but we can ignore affinity for scheduling
 * contexts (@p). So we have to move scheduling contexts towards
 * potential execution contexts.
 *
 * Note: The owner can disappear, but simply migrate to @target_cpu
 * and leave that CPU to sort things out.
 */
ಸ್ಥಿರ ಶೂನ್ಯ proxy_migrate_task(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf,
			       ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ ಗುರಿ_cpu)
	__must_hold(__rq_lockp(rq))
{
	ರಚನೆ rq *ಗುರಿ_rq = cpu_rq(ಗುರಿ_cpu);
	LIST_ತಲೆ(migrate_list);

	lockdep_assert_rq_held(rq);
	/*
	 * Since we are migrating a blocked donor, it could be rq->donor,
	 * and we want to make sure there aren't any references from this
	 * rq to it before we drop the lock. This avoids another cpu
	 * jumping in and grabbing the rq lock and referencing rq->donor
	 * or cfs_rq->curr, etc after we have migrated it to another cpu,
	 * and before we pick_again in __schedule.
	 *
	 * So call proxy_resched_idle() to drop the rq->donor references
	 * before we release the lock.
	 */
	proxy_resched_idle(rq);
	ಗಾಗಿ (; p; p = p->blocked_donor) {
		WARN_ON(p == rq->curr);
		deactivate_task(rq, p, ಸರತಿಯಿಂದ_ತೆಗೆ_NOCLOCK);
		proxy_ಗಣ_task_cpu(p, ಗುರಿ_cpu);
		/*
		 * We can re-use se.group_node to migrate the thing,
		 * because @p is deactivated (won't be balanced) and
		 * we hold the rq_lock.
		 */
		list_ಕೂಡಿಸು(&p->se.ಗುಂಪು_ಗಂಟು, &migrate_list);
	}
	proxy_release_rq_ಬೀಗ(rq, rf);

	__attach_tasks(ಗುರಿ_rq, &migrate_list);

	proxy_reacquire_rq_ಬೀಗ(rq, rf);
}

/*
 * Find runnable lock owner to proxy for mutex blocked donor
 *
 * Follow the blocked-on relation:
 *
 *                ,-> task
 *                |     | blocked-on
 *                |     v
 *  blocked_donor |   mutex
 *                |     | owner
 *                |     v
 *                `-- task
 *
 * and set the blocked_donor relation, this latter is used by the mutex
 * code to find which (blocked) task to hand-off to.
 *
 * Lock order:
 *
 *   p->pi_lock
 *     rq->lock
 *       mutex->wait_lock
 *         p->blocked_lock
 *
 * Returns the task that is going to be used as execution context (the one
 * that is actually going to be run on cpu_of(rq)).
 */
ಸ್ಥಿರ ರಚನೆ task_struct *
ಕಂಡುಹಿಡಿ_proxy_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *donor, ರಚನೆ rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	ರಚನೆ task_struct *owner = ಖಾಲಿ;
	bool curr_ಒಳ_chain = false;
	ಪೂರ್ಣಾಂಕ this_cpu = cpu_ದ(rq);
	ರಚನೆ task_struct *p;
	ಪೂರ್ಣಾಂಕ owner_cpu;

	/* Follow blocked_on chain. */
	ಗಾಗಿ (p = donor; p->is_blocked; p = owner) {
		/* if its PROXY_WAKING, do return migration or run if current */
		ರಚನೆ mutex *mutex = p->blocked_on;
		ಆದರೆ (!mutex) {
			clear_task_blocked_on(p, mutex);
			ಆದರೆ (task_current(rq, p)) {
				p->is_blocked = 0;
				ಹಿಂತಿರುಗಿ p;
			}
			ಹೋಗು deactivate;
		}

		/*
		 * By taking mutex->wait_lock we hold off concurrent mutex_unlock()
		 * and ensure @owner sticks around.
		 */
		guard(raw_spinlock)(&mutex->wait_ಬೀಗ);
		guard(raw_spinlock)(&p->blocked_ಬೀಗ);

		/* Check again that p is blocked with blocked_lock held */
		ಆದರೆ (mutex != __get_task_blocked_on(p)) {
			/*
			 * Something changed in the blocked_on chain and
			 * we don't know if only at this level. So, let's
			 * just bail out completely and let __schedule()
			 * figure things out (pick_again loop).
			 */
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;
		}

		ಆದರೆ (task_current(rq, p))
			curr_ಒಳ_chain = true;

		owner = __mutex_owner(mutex);
		ಆದರೆ (!owner) {
			/*
			 * If there is no owner, either clear blocked_on
			 * and return p (if it is current and safe to
			 * just run on this rq), or return-migrate the task.
			 */
			__clear_task_blocked_on(p, ಖಾಲಿ);
			ಆದರೆ (task_current(rq, p)) {
				p->is_blocked = 0;
				ಹಿಂತಿರುಗಿ p;
			}
			ಹೋಗು deactivate;
		}

		ಆದರೆ (!READ_ONCE(owner->on_rq) || owner->se.sched_delayed) {
			/* XXX Don't handle blocked owners/delayed dequeue yet */
			ಆದರೆ (curr_ಒಳ_chain)
				ಹೋಗು resched_idle;
			__clear_task_blocked_on(p, ಖಾಲಿ);
			ಹೋಗು deactivate;
		}

		owner_cpu = task_cpu(owner);
		ಆದರೆ (owner_cpu != this_cpu) {
			/*
			 * @owner can disappear, simply migrate to @owner_cpu
			 * and leave that CPU to sort things out.
			 */
			ಆದರೆ (curr_ಒಳ_chain)
				ಹೋಗು resched_idle;
			ಹೋಗು migrate_task;
		}

		ಆದರೆ (task_on_rq_migrating(owner)) {
			/*
			 * One of the chain of mutex owners is currently migrating to this
			 * CPU, but has not yet been enqueued because we are holding the
			 * rq lock. As a simple solution, just schedule rq->idle to give
			 * the migration a chance to complete. Much like the migrate_task
			 * case we should end up back in find_proxy_task(), this time
			 * hopefully with all relevant tasks already enqueued.
			 */
			ಹೋಗು resched_idle;
		}

		/*
		 * Its possible to race where after we check owner->on_rq
		 * but before we check (owner_cpu != this_cpu) that the
		 * task on another cpu was migrated back to this cpu. In
		 * that case it could slip by our  checks. So double check
		 * we are still on this cpu and not migrating. If we get
		 * inconsistent results, try again.
		 */
		ಆದರೆ (!task_on_rq_queued(owner) || task_cpu(owner) != this_cpu)
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;

		ಆದರೆ (owner == p) {
			/*
			 * It's possible we interleave with mutex_unlock like:
			 *
			 *				lock(&rq->lock);
			 *				  find_proxy_task()
			 * mutex_unlock()
			 *   lock(&wait_lock);
			 *   donor(owner) = current->blocked_donor;
			 *   unlock(&wait_lock);
			 *
			 *   wake_up_q();
			 *     ...
			 *       ttwu_runnable()
			 *         __task_rq_lock()
			 *				  lock(&wait_lock);
			 *				  owner == p
			 *
			 * Which leaves us to finish the ttwu_runnable() and make it go.
			 *
			 * So schedule rq->idle so that ttwu_runnable() can get the rq
			 * lock and mark owner as running.
			 */
			ಹೋಗು resched_idle;
		}
		/*
		 * OK, now we're absolutely sure @owner is on this
		 * rq, therefore holding @rq->lock is sufficient to
		 * guarantee its existence, as per ttwu_remote().
		 */
		owner->blocked_donor = p;
	}
	WARN_ON_ONCE(owner && !owner->on_rq);

	ಆದರೆ (owner && !sched_cpu_cookie_ಹೊಂದಿಕೆ(rq, owner)) {
		ಆದರೆ (curr_ಒಳ_chain)
			ಹಿಂತಿರುಗಿ proxy_resched_idle(rq);
		p = donor; /* Deactivate the donor, not the runnable owner */
		clear_task_blocked_on(p, ಖಾಲಿ);
		ಹೋಗು deactivate;
	}
	ಹಿಂತಿರುಗಿ owner;

resched_idle:
	ಹಿಂತಿರುಗಿ proxy_resched_idle(rq);
deactivate:
	proxy_deactivate(rq, p);
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
migrate_task:
	proxy_migrate_task(rq, rf, p, owner_cpu);
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}
#ಇಲ್ಲದಿದ್ದರೆ /* SCHED_PROXY_EXEC */
ಸ್ಥಿರ ರಚನೆ task_struct *
ಕಂಡುಹಿಡಿ_proxy_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *donor, ರಚನೆ rq_flags *rf)
{
	WARN_ONCE(1, "This should never be called in the !SCHED_PROXY_EXEC case\n");
	ಹಿಂತಿರುಗಿ donor;
}
#ಆದರೆ_ಅಂತ್ಯ /* SCHED_PROXY_EXEC */

/*
 * __schedule() is the main scheduler function.
 *
 * The main means of driving the scheduler and thus entering this function are:
 *
 *   1. Explicit blocking: mutex, semaphore, waitqueue, etc.
 *
 *   2. TIF_NEED_RESCHED flag is checked on interrupt and userspace return
 *      paths. For example, see arch/x86/entry_64.S.
 *
 *      To drive preemption between tasks, the scheduler sets the flag in timer
 *      interrupt handler sched_tick().
 *
 *   3. Wakeups don't really cause entry into schedule(). They add a
 *      task to the run-queue and that's it.
 *
 *      Now, if the new task added to the run-queue preempts the current
 *      task, then the wakeup sets TIF_NEED_RESCHED and schedule() gets
 *      called on the nearest possible occasion:
 *
 *       - If the kernel is preemptible (CONFIG_PREEMPTION=y):
 *
 *         - in syscall or exception context, at the next outmost
 *           preempt_enable(). (this might be as soon as the wake_up()'s
 *           spin_unlock()!)
 *
 *         - in IRQ context, return from interrupt-handler to
 *           preemptible context
 *
 *       - If the kernel is not preemptible (CONFIG_PREEMPTION is not set)
 *         then at the next:
 *
 *          - cond_resched() call
 *          - explicit schedule() call
 *          - return from syscall or exception to user-space
 *          - return from interrupt-handler to user-space
 *
 * WARNING: must be called with preemption disabled!
 */
ಸ್ಥಿರ ಶೂನ್ಯ __sched notrace __schedule(ಪೂರ್ಣಾಂಕ sched_ಬಹುಲಕ)
{
	ರಚನೆ task_struct *prev, *ಮುಂದಿನ;
	/*
	 * On PREEMPT_RT kernel, SM_RTLOCK_WAIT is noted
	 * as a preemption by schedule_debug() and RCU.
	 */
	bool preempt = sched_ಬಹುಲಕ > SM_NONE;
	bool is_switch = false;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *switch_ಎಣಿಕೆ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ prev_state;
	ರಚನೆ rq_flags rf;
	ರಚನೆ rq *rq;
	ಪೂರ್ಣಾಂಕ cpu;

	/* Trace preemptions consistently with task switches */
	trace_sched_ನಮೂದು_tp(sched_ಬಹುಲಕ == SM_PREEMPT);

	cpu = smp_processor_id();
	rq = cpu_rq(cpu);
	prev = rq->curr;

	ವೇಳಾಪಟ್ಟಿ_debug(prev, preempt);

	klp_sched_try_switch(prev);

	local_irq_disable();
	rcu_note_context_switch(preempt);
	migrate_disable_switch(rq, prev);

	/*
	 * Make sure that signal_pending_state()->signal_pending() below
	 * can't be reordered with __set_current_state(TASK_INTERRUPTIBLE)
	 * done by the caller to avoid the race with signal_wake_up():
	 *
	 * __set_current_state(@state)		signal_wake_up()
	 * schedule()				  set_tsk_thread_flag(p, TIF_SIGPENDING)
	 *					  wake_up_state(p, state)
	 *   LOCK rq->lock			    LOCK p->pi_state
	 *   smp_mb__after_spinlock()		    smp_mb__after_spinlock()
	 *     if (signal_pending_state())	    if (p->state & @state)
	 *
	 * Also, the membarrier system call requires a full memory barrier
	 * after coming from user-space, before storing to rq->curr; this
	 * barrier matches a full barrier in the proximity of the membarrier
	 * system call exit.
	 */
	rq_ಬೀಗ(rq, &rf);
	smp_mb__after_spinlock();

	hrtick_ವೇಳಾಪಟ್ಟಿ_enter(rq);

	/* Promote REQ to ACT */
	rq->clock_update_flags <<= 1;
	update_rq_clock(rq);
	rq->clock_update_flags = RQCF_UPDATED;

	switch_ಎಣಿಕೆ = &prev->nivcsw;

	/* Task state changes only considers SM_PREEMPT as preemption */
	preempt = sched_ಬಹುಲಕ == SM_PREEMPT;

	/*
	 * We must load prev->state once (task_struct::state is volatile), such
	 * that we form a control dependency vs deactivate_task() below.
	 */
	prev_state = READ_ONCE(prev->__state);
	ಆದರೆ (sched_ಬಹುಲಕ == SM_IDLE) {
		/* SCX must consult the BPF scheduler to tell if rq is empty */
		ಆದರೆ (!rq->nr_running && !scx_enabled()) {
			ಮುಂದಿನ = prev;
			rq->ಮುಂದಿನ_class = &idle_sched_class;
			ಹೋಗು picked;
		}
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!preempt && prev_state) {
		/*
		 * Keep mutex-blocked tasks on the runqueue for proxy execution
		 * only when their scheduling class allows it. Without proxy
		 * execution, task_is_blocked() always returns false.
		 */
		try_ಗಮ್ಯ_ಖಂಡ_task(rq, prev, &prev_state,
				  !task_is_blocked(prev) || !scx_allow_proxy_exec(prev));
		switch_ಎಣಿಕೆ = &prev->nvcsw;
	}

pick_again:
	assert_balance_callbacks_ಖಾಲಿಗಳು(rq);
	ಮುಂದಿನ = pick_ಮುಂದಿನ_task(rq, &rf);
	rq->ಮುಂದಿನ_class = ಮುಂದಿನ->sched_class;
	ಆದರೆ (sched_proxy_exec()) {
		ರಚನೆ task_struct *prev_donor = rq->donor;

		rq_ಗಣ_donor(rq, ಮುಂದಿನ);
		ಮುಂದಿನ->blocked_donor = ಖಾಲಿ;
		ಆದರೆ (unlikely(ಮುಂದಿನ->is_blocked)) {
			ಮುಂದಿನ = ಕಂಡುಹಿಡಿ_proxy_task(rq, ಮುಂದಿನ, &rf);
			ಆದರೆ (!ಮುಂದಿನ) {
				zap_balance_callbacks(rq);
				ಹೋಗು pick_again;
			}
			ಆದರೆ (ಮುಂದಿನ == rq->idle) {
				zap_balance_callbacks(rq);
				scx_proxy_reenqueue_retry(rq, ಮುಂದಿನ);
				ಹೋಗು keep_resched;
			}
		}
		ಆದರೆ (rq->donor == prev_donor && prev != ಮುಂದಿನ) {
			ರಚನೆ task_struct *donor = rq->donor;
			/*
			 * When transitioning like:
			 *
			 *         prev         next
			 * donor:    B            B
			 * curr:     A          B or C
			 *
			 * then put_prev_set_next_task() will not have done
			 * anything, since B == B. However, A might have
			 * missed a RT/DL balance opportunity due to being
			 * on_cpu.
			 */
			donor->sched_class->put_prev_task(rq, donor, donor);
			donor->sched_class->ಗಣ_ಮುಂದಿನ_task(rq, donor, SNT_PICK);
		}
		scx_proxy_donor_ಆರಂಭ(rq);
		scx_proxy_reenqueue_retry(rq, ಮುಂದಿನ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		rq_ಗಣ_donor(rq, ಮುಂದಿನ);
	}

picked:
	clear_tsk_need_resched(prev);
	clear_preempt_need_resched();
keep_resched:
	rq->ಕೊನೆಯ_ನೋಡಿದ_need_resched_ns = 0;

	is_switch = prev != ಮುಂದಿನ;
	ಆದರೆ (likely(is_switch)) {
		rq->nr_switches++;
		/*
		 * RCU users of rcu_dereference(rq->curr) may not see
		 * changes to task_struct made by pick_next_task().
		 */
		RCU_INIT_ಸೂಚಕ(rq->curr, ಮುಂದಿನ);

		/*
		 * The membarrier system call requires each architecture
		 * to have a full memory barrier after updating
		 * rq->curr, before returning to user-space.
		 *
		 * Here are the schemes providing that barrier on the
		 * various architectures:
		 * - mm ? switch_mm() : mmdrop() for x86, s390, sparc, PowerPC,
		 *   RISC-V.  switch_mm() relies on membarrier_arch_switch_mm()
		 *   on PowerPC and on RISC-V.
		 * - finish_lock_switch() for weakly-ordered
		 *   architectures where spin_unlock is a full barrier,
		 * - switch_to() for arm64 (weakly-ordered, spin_unlock
		 *   is a RELEASE barrier),
		 *
		 * The barrier matches a full barrier in the proximity of
		 * the membarrier system call entry.
		 *
		 * On RISC-V, this barrier pairing is also needed for the
		 * SYNC_CORE command when switching between processes, cf.
		 * the inline comments in membarrier_arch_switch_mm().
		 */
		++*switch_ಎಣಿಕೆ;

		psi_account_irqtime(rq, prev, ಮುಂದಿನ);
		psi_sched_switch(prev, ಮುಂದಿನ, !task_on_rq_queued(prev) ||
					     prev->se.sched_delayed);

		trace_sched_switch(preempt, prev, ಮುಂದಿನ, prev_state);

		/* Also unlocks the rq: */
		rq = context_switch(rq, prev, ಮುಂದಿನ, &rf);
	} ಇಲ್ಲದಿದ್ದರೆ {
		rq_unpin_ಬೀಗ(rq, &rf);
		__balance_callbacks(rq, ಖಾಲಿ);
		hrtick_ವೇಳಾಪಟ್ಟಿ_exit(rq);
		raw_spin_rq_unlock_irq(rq);
	}
	trace_sched_exit_tp(is_switch);
}

ಶೂನ್ಯ __noreturn do_task_dead(ಶೂನ್ಯ)
{
	/* Causes final put_task_struct in finish_task_switch(): */
	ಗಣ_special_state(TASK_DEAD);

	/* Tell freezer to ignore us: */
	current->flags |= PF_NOFREEZE;

	__schedule(SM_NONE);
	BUG();

	/* Avoid "noreturn function does return" - but don't continue if BUG() is a NOP: */
	ಗಾಗಿ (;;)
		cpu_relax();
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_submit_work(ರಚನೆ task_struct *tsk)
{
	ಸ್ಥಿರ DEFINE_WAIT_OVERRIDE_MAP(sched_map, LD_WAIT_CONFIG);
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ task_flags;

	/*
	 * Establish LD_WAIT_CONFIG context to ensure none of the code called
	 * will use a blocking primitive -- which would lead to recursion.
	 */
	ಬೀಗ_map_acquire_try(&sched_map);

	task_flags = tsk->flags;
	/*
	 * If a worker goes to sleep, notify and ask workqueue whether it
	 * wants to wake up a task to maintain concurrency.
	 */
	ಆದರೆ (task_flags & PF_WQ_ಕೆಲಸಗಾರ)
		wq_ಕೆಲಸಗಾರ_sleeping(tsk);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (task_flags & PF_IO_ಕೆಲಸಗಾರ)
		io_wq_ಕೆಲಸಗಾರ_sleeping(tsk);

	/*
	 * spinlock and rwlock must not flush block requests.  This will
	 * deadlock if the callback attempts to acquire a lock which is
	 * already acquired.
	 */
	WARN_ON_ONCE(current->__state & TASK_RTLOCK_WAIT);

	/*
	 * If we are going to sleep and we have plugged IO queued,
	 * make sure to submit it to avoid deadlocks.
	 */
	blk_flush_plug(tsk->plug, true);

	ಬೀಗ_map_release(&sched_map);
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_update_ಕೆಲಸಗಾರ(ರಚನೆ task_struct *tsk)
{
	ಆದರೆ (tsk->flags & (PF_WQ_ಕೆಲಸಗಾರ | PF_IO_ಕೆಲಸಗಾರ)) {
		ಆದರೆ (tsk->flags & PF_WQ_ಕೆಲಸಗಾರ)
			wq_ಕೆಲಸಗಾರ_running(tsk);
		ಇಲ್ಲದಿದ್ದರೆ
			io_wq_ಕೆಲಸಗಾರ_running(tsk);
	}
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ __schedule_loop(ಪೂರ್ಣಾಂಕ sched_ಬಹುಲಕ)
{
	ಮಾಡು {
		preempt_disable();
		__schedule(sched_ಬಹುಲಕ);
		sched_preempt_enable_no_resched();
	} ಆಗ (need_resched());
}

asmlinkage __visible ಶೂನ್ಯ __sched ವೇಳಾಪಟ್ಟಿ(ಶೂನ್ಯ)
{
	ರಚನೆ task_struct *tsk = current;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_MUTEXES
	lockdep_assert(!tsk->sched_rt_mutex);
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (!task_is_running(tsk))
		sched_submit_work(tsk);
	__schedule_loop(SM_NONE);
	sched_update_ಕೆಲಸಗಾರ(tsk);
}
EXPORT_ಚಿಹ್ನೆ(ವೇಳಾಪಟ್ಟಿ);

/*
 * synchronize_rcu_tasks() makes sure that no task is stuck in preempted
 * state (have scheduled out non-voluntarily) by making sure that all
 * tasks have either left the run queue or have gone into user space.
 * As idle tasks do not do either, they must not ever be preempted
 * (schedule out non-voluntarily).
 *
 * schedule_idle() is similar to schedule_preempt_disable() except that it
 * never enables preemption because it does not call sched_submit_work().
 */
ಶೂನ್ಯ __sched ವೇಳಾಪಟ್ಟಿ_idle(ಶೂನ್ಯ)
{
	/*
	 * As this skips calling sched_submit_work(), which the idle task does
	 * regardless because that function is a NOP when the task is in a
	 * TASK_RUNNING state, make sure this isn't used someplace that the
	 * current task can be in any other state. Note, idle is always in the
	 * TASK_RUNNING state.
	 */
	WARN_ON_ONCE(current->__state);
	ಮಾಡು {
		__schedule(SM_IDLE);
	} ಆಗ (need_resched());
}

#ಆದರೆ defined(CONFIG_CONTEXT_TRACKING_USER) && !defined(CONFIG_HAVE_CONTEXT_TRACKING_USER_OFFSTACK)
asmlinkage __visible ಶೂನ್ಯ __sched ವೇಳಾಪಟ್ಟಿ_user(ಶೂನ್ಯ)
{
	/*
	 * If we come here after a random call to set_need_resched(),
	 * or we have been woken up remotely but the IPI has not yet arrived,
	 * we haven't yet exited the RCU idle mode. Do it here manually until
	 * we find a better solution.
	 *
	 * NB: There are buggy callers of this function.  Ideally we
	 * should warn if prev_state != CT_STATE_USER, but that will trigger
	 * too frequently to make sense yet.
	 */
	ಎಣಿಕೆ ctx_state prev_state = exception_enter();
	ವೇಳಾಪಟ್ಟಿ();
	exception_exit(prev_state);
}
#ಆದರೆ_ಅಂತ್ಯ

/**
 * schedule_preempt_disabled - called with preemption disabled
 *
 * Returns with preemption disabled. Note: preempt_count must be 1
 */
ಶೂನ್ಯ __sched ವೇಳಾಪಟ್ಟಿ_preempt_disabled(ಶೂನ್ಯ)
{
	sched_preempt_enable_no_resched();
	ವೇಳಾಪಟ್ಟಿ();
	preempt_disable();
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PREEMPT_RT
ಶೂನ್ಯ __sched notrace ವೇಳಾಪಟ್ಟಿ_rtlock(ಶೂನ್ಯ)
{
	__schedule_loop(SM_RTLOCK_WAIT);
}
NOKPROBE_ಚಿಹ್ನೆ(ವೇಳಾಪಟ್ಟಿ_rtlock);
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಶೂನ್ಯ __sched notrace preempt_ವೇಳಾಪಟ್ಟಿ_common(ಶೂನ್ಯ)
{
	ಮಾಡು {
		/*
		 * Because the function tracer can trace preempt_count_sub()
		 * and it also uses preempt_enable/disable_notrace(), if
		 * NEED_RESCHED is set, the preempt_enable_notrace() called
		 * by the function tracer will call this function again and
		 * cause infinite recursion.
		 *
		 * Preemption must be disabled here before the function
		 * tracer can trace. Break up preempt_disable() into two
		 * calls. One to disable preemption without fear of being
		 * traced. The other to still record the preemption latency,
		 * which can also be traced by the function tracer.
		 */
		preempt_disable_notrace();
		preempt_latency_ಆರಂಭ(1);
		__schedule(SM_PREEMPT);
		preempt_latency_stop(1);
		preempt_enable_no_resched_notrace();

		/*
		 * Check again in case we missed a preemption opportunity
		 * between schedule and now.
		 */
	} ಆಗ (need_resched());
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PREEMPTION
/*
 * This is the entry point to schedule() from in-kernel preemption
 * off of preempt_enable.
 */
asmlinkage __visible ಶೂನ್ಯ __sched notrace preempt_ವೇಳಾಪಟ್ಟಿ(ಶೂನ್ಯ)
{
	/*
	 * If there is a non-zero preempt_count or interrupts are disabled,
	 * we do not want to preempt the current task. Just return..
	 */
	ಆದರೆ (likely(!preemptible()))
		ಹಿಂತಿರುಗಿ;
	preempt_ವೇಳಾಪಟ್ಟಿ_common();
}
NOKPROBE_ಚಿಹ್ನೆ(preempt_ವೇಳಾಪಟ್ಟಿ);
EXPORT_ಚಿಹ್ನೆ(preempt_ವೇಳಾಪಟ್ಟಿ);

/**
 * preempt_schedule_notrace - preempt_schedule called by tracing
 *
 * The tracing infrastructure uses preempt_enable_notrace to prevent
 * recursion and tracing preempt enabling caused by the tracing
 * infrastructure itself. But as tracing can happen in areas coming
 * from userspace or just about to enter userspace, a preempt enable
 * can occur before user_exit() is called. This will cause the scheduler
 * to be called when the system is still in usermode.
 *
 * To prevent this, the preempt_enable_notrace will use this function
 * instead of preempt_schedule() to exit user context if needed before
 * calling the scheduler.
 */
asmlinkage __visible ಶೂನ್ಯ __sched notrace preempt_ವೇಳಾಪಟ್ಟಿ_notrace(ಶೂನ್ಯ)
{
	ಎಣಿಕೆ ctx_state prev_ctx;

	ಆದರೆ (likely(!preemptible()))
		ಹಿಂತಿರುಗಿ;

	ಮಾಡು {
		/*
		 * Because the function tracer can trace preempt_count_sub()
		 * and it also uses preempt_enable/disable_notrace(), if
		 * NEED_RESCHED is set, the preempt_enable_notrace() called
		 * by the function tracer will call this function again and
		 * cause infinite recursion.
		 *
		 * Preemption must be disabled here before the function
		 * tracer can trace. Break up preempt_disable() into two
		 * calls. One to disable preemption without fear of being
		 * traced. The other to still record the preemption latency,
		 * which can also be traced by the function tracer.
		 */
		preempt_disable_notrace();
		preempt_latency_ಆರಂಭ(1);
		/*
		 * Needs preempt disabled in case user_exit() is traced
		 * and the tracer calls preempt_enable_notrace() causing
		 * an infinite recursion.
		 */
		prev_ctx = exception_enter();
		__schedule(SM_PREEMPT);
		exception_exit(prev_ctx);

		preempt_latency_stop(1);
		preempt_enable_no_resched_notrace();
	} ಆಗ (need_resched());
}
EXPORT_ಚಿಹ್ನೆ_GPL(preempt_ವೇಳಾಪಟ್ಟಿ_notrace);

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_PREEMPTION */

/*
 * This is the entry point to schedule() from kernel preemption
 * off of IRQ context.
 * Note, that this is called and return with IRQs disabled. This will
 * protect us against recursive calling from IRQ contexts.
 */
asmlinkage __visible ಶೂನ್ಯ __sched preempt_ವೇಳಾಪಟ್ಟಿ_irq(ಶೂನ್ಯ)
{
	ಎಣಿಕೆ ctx_state prev_state;

	/* Catch callers which need to be fixed */
	BUG_ON(preempt_ಎಣಿಕೆ() || !irqs_disabled());

	prev_state = exception_enter();

	ಮಾಡು {
		preempt_disable();
		local_irq_enable();
		__schedule(SM_PREEMPT);
		local_irq_disable();
		sched_preempt_enable_no_resched();
	} ಆಗ (need_resched());

	exception_exit(prev_state);
}

ಪೂರ್ಣಾಂಕ default_wake_function(wait_ಸರತಿ_ನಮೂದು_t *curr, ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ, ಪೂರ್ಣಾಂಕ wake_flags,
			  ಶೂನ್ಯ *ಕೀಲಿ)
{
	WARN_ON_ONCE(wake_flags & ~(WF_SYNC|WF_CURRENT_CPU));
	ಹಿಂತಿರುಗಿ try_ಗಮ್ಯ_wake_ಮೇಲೆ(curr->ಖಾಸಗಿ, ಬಹುಲಕ, wake_flags);
}
EXPORT_ಚಿಹ್ನೆ(default_wake_function);

ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *__setscheduler_class(ಪೂರ್ಣಾಂಕ policy, ಪೂರ್ಣಾಂಕ prio)
{
	ಆದರೆ (dl_prio(prio))
		ಹಿಂತಿರುಗಿ &dl_sched_class;

	ಆದರೆ (rt_prio(prio))
		ಹಿಂತಿರುಗಿ &rt_sched_class;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CLASS_EXT
	ಆದರೆ (task_should_scx(policy))
		ಹಿಂತಿರುಗಿ &ext_sched_class;
#ಆದರೆ_ಅಂತ್ಯ

	ಹಿಂತಿರುಗಿ &fair_sched_class;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_MUTEXES

/*
 * Would be more useful with typeof()/auto_type but they don't mix with
 * bit-fields. Since it's a local thing, use int. Keep the generic sounding
 * name such that if someone were to implement this function we get to compare
 * notes.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು fetch_and_ಗಣ(x, v) ({ ಪೂರ್ಣಾಂಕ _x = (x); (x) = (v); _x; })

ಶೂನ್ಯ rt_mutex_pre_ವೇಳಾಪಟ್ಟಿ(ಶೂನ್ಯ)
{
	lockdep_assert(!fetch_and_ಗಣ(current->sched_rt_mutex, 1));
	sched_submit_work(current);
}

/*
 * Used within the futex syscall context, skips sched_submit_work() because none
 * its work will be done. Asserts ensure that it is indeed the case.
 */
ಶೂನ್ಯ rt_mutex_futex_pre_ವೇಳಾಪಟ್ಟಿ(ಶೂನ್ಯ)
{
	lockdep_assert(!(current->flags & (PF_WQ_ಕೆಲಸಗಾರ | PF_IO_ಕೆಲಸಗಾರ)));
	lockdep_assert(!current->plug);
	lockdep_assert(!fetch_and_ಗಣ(current->sched_rt_mutex, 1));
}

ಶೂನ್ಯ rt_mutex_ವೇಳಾಪಟ್ಟಿ(ಶೂನ್ಯ)
{
	lockdep_assert(current->sched_rt_mutex);
	__schedule_loop(SM_NONE);
}

ಶೂನ್ಯ rt_mutex_post_ವೇಳಾಪಟ್ಟಿ(ಶೂನ್ಯ)
{
	sched_update_ಕೆಲಸಗಾರ(current);
	lockdep_assert(fetch_and_ಗಣ(current->sched_rt_mutex, 0));
}

ಶೂನ್ಯ rt_mutex_futex_post_ವೇಳಾಪಟ್ಟಿ(ಶೂನ್ಯ)
{
	lockdep_assert(fetch_and_ಗಣ(current->sched_rt_mutex, 0));
}

/*
 * rt_mutex_setprio - set the current priority of a task
 * @p: task to boost
 * @pi_task: donor task
 *
 * This function changes the 'effective' priority of a task. It does
 * not touch ->normal_prio like __setscheduler().
 *
 * Used by the rt_mutex code to implement priority inheritance
 * logic. Call site only calls if the priority of the task changed.
 */
ಶೂನ್ಯ rt_mutex_setprio(ರಚನೆ task_struct *p, ರಚನೆ task_struct *pi_task)
{
	ಪೂರ್ಣಾಂಕ prio, oldprio, ಸರತಿ_flag =
		ಸರತಿಯಿಂದ_ತೆಗೆ_SAVE | ಸರತಿಯಿಂದ_ತೆಗೆ_MOVE | ಸರತಿಯಿಂದ_ತೆಗೆ_NOCLOCK;
	ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *prev_class, *ಮುಂದಿನ_class;
	ರಚನೆ rq_flags rf;
	ರಚನೆ rq *rq;

	/* XXX used to be waiter->prio, not waiter->task->prio */
	prio = __rt_effective_prio(pi_task, p->ಸಾಮಾನ್ಯ_prio);

	/*
	 * If nothing changed; bail early.
	 */
	ಆದರೆ (p->pi_ತುದಿ_task == pi_task && prio == p->prio && !dl_prio(prio))
		ಹಿಂತಿರುಗಿ;

	rq = __task_rq_lock(p, &rf);
	update_rq_clock(rq);
	/*
	 * Set under pi_lock && rq->lock, such that the value can be used under
	 * either lock.
	 *
	 * Note that there is loads of tricky to make this pointer cache work
	 * right. rt_mutex_slowunlock()+rt_mutex_postunlock() work together to
	 * ensure a task is de-boosted (pi_task is set to NULL) before the
	 * task is allowed to run again (and can exit). This ensures the pointer
	 * points to a blocked task -- which guarantees the task is present.
	 */
	p->pi_ತುದಿ_task = pi_task;

	/*
	 * For FIFO/RR we only need to set prio, if that matches we're done.
	 */
	ಆದರೆ (prio == p->prio && !dl_prio(prio))
		ಹೋಗು ಹೊರಹೋಗು_unlock;

	/*
	 * Idle task boosting is a no-no in general. There is one
	 * exception, when PREEMPT_RT and NOHZ is active:
	 *
	 * The idle task calls get_next_timer_interrupt() and holds
	 * the timer wheel base->lock on the CPU and another CPU wants
	 * to access the timer (probably to cancel it). We can safely
	 * ignore the boosting request, as the idle CPU runs this code
	 * with interrupts disabled and will complete the lock
	 * protected section without being interrupted. So there is no
	 * real need to boost.
	 */
	ಆದರೆ (unlikely(p == rq->idle)) {
		WARN_ON(p != rq->curr);
		WARN_ON(p->pi_blocked_on);
		ಹೋಗು ಹೊರಹೋಗು_unlock;
	}

	trace_sched_pi_setprio(p, pi_task);
	oldprio = p->prio;

	ಆದರೆ (oldprio == prio && !dl_prio(prio))
		ಸರತಿ_flag &= ~ಸರತಿಯಿಂದ_ತೆಗೆ_MOVE;

	prev_class = p->sched_class;
	ಮುಂದಿನ_class = __setscheduler_class(p->policy, prio);

	ಆದರೆ (prev_class != ಮುಂದಿನ_class)
		ಸರತಿ_flag |= ಸರತಿಯಿಂದ_ತೆಗೆ_CLASS;

	scoped_guard (sched_change, p, ಸರತಿ_flag) {
		/*
		 * Boosting condition are:
		 * 1. -rt task is running and holds mutex A
		 *      --> -dl task blocks on mutex A
		 *
		 * 2. -dl task is running and holds mutex A
		 *      --> -dl task blocks on mutex A and could preempt the
		 *          running task
		 */
		ಆದರೆ (dl_prio(prio)) {
			ಆದರೆ (!dl_prio(p->ಸಾಮಾನ್ಯ_prio) ||
			    (pi_task && dl_prio(pi_task->prio) &&
			     dl_entity_preempt(&pi_task->dl, &p->dl))) {
				p->dl.pi_se = pi_task->dl.pi_se;
				scope->flags |= ಸರತಿಗೆ_ಸೇರಿಸು_REPLENISH;
			} ಇಲ್ಲದಿದ್ದರೆ {
				p->dl.pi_se = &p->dl;
			}
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (rt_prio(prio)) {
			ಆದರೆ (dl_prio(oldprio))
				p->dl.pi_se = &p->dl;
			ಆದರೆ (oldprio < prio)
				scope->flags |= ಸರತಿಗೆ_ಸೇರಿಸು_ತಲೆ;
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಆದರೆ (dl_prio(oldprio))
				p->dl.pi_se = &p->dl;
			ಆದರೆ (rt_prio(oldprio))
				p->rt.timeout = 0;
		}

		p->sched_class = ಮುಂದಿನ_class;
		p->prio = prio;
	}
ಹೊರಹೋಗು_unlock:
	/* Caller holds task_struct::pi_lock, IRQs are still disabled */

	__balance_callbacks(rq, &rf);
	__task_rq_unlock(rq, p, &rf);
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_RT_MUTEXES */

#ಆದರೆ !defined(CONFIG_PREEMPTION)
ಪೂರ್ಣಾಂಕ __sched __cond_resched(ಶೂನ್ಯ)
{
	ಆದರೆ (should_resched(0) && !irqs_disabled()) {
		preempt_ವೇಳಾಪಟ್ಟಿ_common();
		ಹಿಂತಿರುಗಿ 1;
	}
	/*
	 * In PREEMPT_RCU kernels, ->rcu_read_lock_nesting tells the tick
	 * whether the current CPU is in an RCU read-side critical section,
	 * so the tick can report quiescent states even for CPUs looping
	 * in kernel context.  In contrast, in non-preemptible kernels,
	 * RCU readers leave no in-memory hints, which means that CPU-bound
	 * processes executing in kernel context might never report an
	 * RCU quiescent state.  Therefore, the following code causes
	 * cond_resched() to report a quiescent state, but only when RCU
	 * is in urgent need of one.
	 * A third case, preemptible, but non-PREEMPT_RCU provides for
	 * urgently needed quiescent states via rcu_flavor_sched_clock_irq().
	 */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CONFIG_PREEMPT_RCU
	rcu_all_qs();
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ 0;
}
EXPORT_ಚಿಹ್ನೆ(__cond_resched);
#ಆದರೆ_ಅಂತ್ಯ

/*
 * __cond_resched_lock() - if a reschedule is pending, drop the given lock,
 * call schedule, and on return reacquire the lock.
 *
 * This works OK both with and without CONFIG_PREEMPTION. We do strange low-level
 * operations here to prevent schedule() from being called twice (once via
 * spin_unlock(), once by hand).
 */
ಪೂರ್ಣಾಂಕ __cond_resched_lock(spinlock_t *ಬೀಗ)
{
	ಪೂರ್ಣಾಂಕ resched = should_resched(PREEMPT_ಬೀಗ_ಸ್ಥಾನಾಂತರ);
	ಪೂರ್ಣಾಂಕ ret = 0;

	lockdep_assert_held(ಬೀಗ);

	ಆದರೆ (spin_needbreak(ಬೀಗ) || resched) {
		spin_unlock(ಬೀಗ);
		ಆದರೆ (!_cond_resched())
			cpu_relax();
		ret = 1;
		spin_ಬೀಗ(ಬೀಗ);
	}
	ಹಿಂತಿರುಗಿ ret;
}
EXPORT_ಚಿಹ್ನೆ(__cond_resched_lock);

ಪೂರ್ಣಾಂಕ __cond_resched_rwlock_read(rwlock_t *ಬೀಗ)
{
	ಪೂರ್ಣಾಂಕ resched = should_resched(PREEMPT_ಬೀಗ_ಸ್ಥಾನಾಂತರ);
	ಪೂರ್ಣಾಂಕ ret = 0;

	lockdep_assert_held_read(ಬೀಗ);

	ಆದರೆ (rwlock_needbreak(ಬೀಗ) || resched) {
		read_unlock(ಬೀಗ);
		ಆದರೆ (!_cond_resched())
			cpu_relax();
		ret = 1;
		read_ಬೀಗ(ಬೀಗ);
	}
	ಹಿಂತಿರುಗಿ ret;
}
EXPORT_ಚಿಹ್ನೆ(__cond_resched_rwlock_read);

ಪೂರ್ಣಾಂಕ __cond_resched_rwlock_write(rwlock_t *ಬೀಗ)
{
	ಪೂರ್ಣಾಂಕ resched = should_resched(PREEMPT_ಬೀಗ_ಸ್ಥಾನಾಂತರ);
	ಪೂರ್ಣಾಂಕ ret = 0;

	lockdep_assert_held_write(ಬೀಗ);

	ಆದರೆ (rwlock_needbreak(ಬೀಗ) || resched) {
		write_unlock(ಬೀಗ);
		ಆದರೆ (!_cond_resched())
			cpu_relax();
		ret = 1;
		write_ಬೀಗ(ಬೀಗ);
	}
	ಹಿಂತಿರುಗಿ ret;
}
EXPORT_ಚಿಹ್ನೆ(__cond_resched_rwlock_write);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PREEMPT_DYNAMIC

# ifdef CONFIG_GENERIC_IRQ_ನಮೂದು
#  include <linux/irq-entry-common.h>
# endif

/*
 * NONE:
 *   (unselectable)
 *
 * VOLUNTARY:
 *   (unselectable)
 *
 * FULL:
 *   dynamic_preempt_lazy       <- false
 *
 * LAZY:
 *   dynamic_preempt_lazy       <- true
 */

ಎಣಿಕೆ {
	preempt_dynamic_undefined = -1,
	preempt_dynamic_ತುಂಬಿದ,
	preempt_dynamic_lazy,
};

ಪೂರ್ಣಾಂಕ preempt_dynamic_ಬಹುಲಕ = preempt_dynamic_undefined;

ಪೂರ್ಣಾಂಕ sched_dynamic_ಬಹುಲಕ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str)
{
	ಆದರೆ (!strcmp(str, "full"))
		ಹಿಂತಿರುಗಿ preempt_dynamic_ತುಂಬಿದ;

	ಆದರೆ (!strcmp(str, "lazy"))
		ಹಿಂತಿರುಗಿ preempt_dynamic_lazy;

	ಹಿಂತಿರುಗಿ -EINVAL;
}

# define preempt_dynamic_ಕೀಲಿ_enable(f)	static_ಕೀಲಿ_enable(&sk_dynamic_##f.ಕೀಲಿ)
# define preempt_dynamic_ಕೀಲಿ_disable(f)	static_ಕೀಲಿ_disable(&sk_dynamic_##f.ಕೀಲಿ)

ಸ್ಥಿರ DEFINE_MUTEX(sched_dynamic_mutex);

ಸ್ಥಿರ ಶೂನ್ಯ __sched_dynamic_update(ಪೂರ್ಣಾಂಕ ಬಹುಲಕ)
{
	ಆಯ್ಕೆ (ಬಹುಲಕ) {
	ಸ್ಥಿತಿ preempt_dynamic_ತುಂಬಿದ:
		preempt_dynamic_ಕೀಲಿ_disable(preempt_lazy);
		ಆದರೆ (ಬಹುಲಕ != preempt_dynamic_ಬಹುಲಕ)
			pr_info("Dynamic Preempt: full\n");
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ preempt_dynamic_lazy:
		preempt_dynamic_ಕೀಲಿ_enable(preempt_lazy);
		ಆದರೆ (ಬಹುಲಕ != preempt_dynamic_ಬಹುಲಕ)
			pr_info("Dynamic Preempt: lazy\n");
		ನಿಲ್ಲಿಸು;
	}

	WRITE_ONCE(preempt_dynamic_ಬಹುಲಕ, ಬಹುಲಕ);
}

ಶೂನ್ಯ sched_dynamic_update(ಪೂರ್ಣಾಂಕ ಬಹುಲಕ)
{
	mutex_ಬೀಗ(&sched_dynamic_mutex);
	__sched_dynamic_update(ಬಹುಲಕ);
	mutex_unlock(&sched_dynamic_mutex);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init ಸಿದ್ಧತೆ_preempt_ಬಹುಲಕ(ಅಕ್ಷರ *str)
{
	ಪೂರ್ಣಾಂಕ ಬಹುಲಕ = sched_dynamic_ಬಹುಲಕ(str);
	ಆದರೆ (ಬಹುಲಕ < 0) {
		pr_warn("Dynamic Preempt: unsupported mode: %s\n", str);
		ಹಿಂತಿರುಗಿ 0;
	}

	sched_dynamic_update(ಬಹುಲಕ);
	ಹಿಂತಿರುಗಿ 1;
}
__setup("preempt=", ಸಿದ್ಧತೆ_preempt_ಬಹುಲಕ);

ಸ್ಥಿರ ಶೂನ್ಯ __init preempt_dynamic_init(ಶೂನ್ಯ)
{
	ಆದರೆ (preempt_dynamic_ಬಹುಲಕ == preempt_dynamic_undefined) {
		ಆದರೆ (IS_ENABLED(CONFIG_PREEMPT_LAZY)) {
			sched_dynamic_update(preempt_dynamic_lazy);
		} ಇಲ್ಲದಿದ್ದರೆ {
			/* Default static call setting, nothing to do */
			WARN_ON_ONCE(!IS_ENABLED(CONFIG_PREEMPT));
			preempt_dynamic_ಬಹುಲಕ = preempt_dynamic_ತುಂಬಿದ;
			pr_info("Dynamic Preempt: full\n");
		}
	}
}

# define PREEMPT_MODEL_ACCESSOR(ಬಹುಲಕ)					\
	bool preempt_model_##mode(ಶೂನ್ಯ)					\
	{								\
		ಪೂರ್ಣಾಂಕ ಬಹುಲಕ = READ_ONCE(preempt_dynamic_ಬಹುಲಕ);		\
		WARN_ON_ONCE(ಬಹುಲಕ == preempt_dynamic_undefined);	\
		ಹಿಂತಿರುಗಿ ಬಹುಲಕ == preempt_dynamic_##mode;			\
	}								\
	EXPORT_ಚಿಹ್ನೆ_GPL(preempt_model_##mode)

PREEMPT_MODEL_ACCESSOR(ತುಂಬಿದ);
PREEMPT_MODEL_ACCESSOR(lazy);

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_PREEMPT_DYNAMIC: */

#ವ್ಯಾಖ್ಯಾನಿಸು preempt_dynamic_ಬಹುಲಕ -1

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ preempt_dynamic_init(ಶೂನ್ಯ) { }

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_PREEMPT_DYNAMIC */

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *preempt_modes[] = {
	"full", "lazy", ಖಾಲಿ,
};

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *preempt_model_str(ಶೂನ್ಯ)
{
	bool brace = IS_ENABLED(CONFIG_PREEMPT_RT) &&
		(IS_ENABLED(CONFIG_PREEMPT_DYNAMIC) ||
		 IS_ENABLED(CONFIG_PREEMPT_LAZY));
	ಸ್ಥಿರ ಅಕ್ಷರ buf[128];

	ಆದರೆ (IS_ENABLED(CONFIG_PREEMPT_BUILD)) {
		ರಚನೆ seq_buf s;

		seq_buf_init(&s, buf, ಗಾತ್ರ(buf));
		seq_buf_puts(&s, "PREEMPT");

		ಆದರೆ (IS_ENABLED(CONFIG_PREEMPT_RT))
			seq_buf_printf(&s, "%sRT%s",
				       brace ? "_{" : "_",
				       brace ? "," : "");

		ಆದರೆ (IS_ENABLED(CONFIG_PREEMPT_DYNAMIC)) {
			seq_buf_printf(&s, "(%s)%s",
				       preempt_dynamic_ಬಹುಲಕ >= 0 ?
				       preempt_modes[preempt_dynamic_ಬಹುಲಕ] : "undef",
				       brace ? "}" : "");
			ಹಿಂತಿರುಗಿ seq_buf_str(&s);
		}

		ಆದರೆ (IS_ENABLED(CONFIG_PREEMPT_LAZY)) {
			seq_buf_printf(&s, "LAZY%s",
				       brace ? "}" : "");
			ಹಿಂತಿರುಗಿ seq_buf_str(&s);
		}

		ಹಿಂತಿರುಗಿ seq_buf_str(&s);
	}

	ಆದರೆ (IS_ENABLED(CONFIG_PREEMPT_VOLUNTARY_BUILD))
		ಹಿಂತಿರುಗಿ "VOLUNTARY";

	ಹಿಂತಿರುಗಿ "NONE";
}

ಪೂರ್ಣಾಂಕ io_ವೇಳಾಪಟ್ಟಿ_prepare(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ old_iowait = current->ಒಳ_iowait;

	current->ಒಳ_iowait = 1;
	blk_flush_plug(current->plug, true);
	ಹಿಂತಿರುಗಿ old_iowait;
}

ಶೂನ್ಯ io_ವೇಳಾಪಟ್ಟಿ_ಮುಕ್ತಾಯ(ಪೂರ್ಣಾಂಕ token)
{
	current->ಒಳ_iowait = token;
}

/*
 * This task is about to go to sleep on IO. Increment rq->nr_iowait so
 * that process accounting knows that this is a task in IO wait state.
 */
ದೀರ್ಘ __sched io_ವೇಳಾಪಟ್ಟಿ_timeout(ದೀರ್ಘ timeout)
{
	ಪೂರ್ಣಾಂಕ token;
	ದೀರ್ಘ ret;

	token = io_ವೇಳಾಪಟ್ಟಿ_prepare();
	ret = ವೇಳಾಪಟ್ಟಿ_timeout(timeout);
	io_ವೇಳಾಪಟ್ಟಿ_ಮುಕ್ತಾಯ(token);

	ಹಿಂತಿರುಗಿ ret;
}
EXPORT_ಚಿಹ್ನೆ(io_ವೇಳಾಪಟ್ಟಿ_timeout);

ಶೂನ್ಯ __sched io_ವೇಳಾಪಟ್ಟಿ(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ token;

	token = io_ವೇಳಾಪಟ್ಟಿ_prepare();
	ವೇಳಾಪಟ್ಟಿ();
	io_ವೇಳಾಪಟ್ಟಿ_ಮುಕ್ತಾಯ(token);
}
EXPORT_ಚಿಹ್ನೆ(io_ವೇಳಾಪಟ್ಟಿ);

ಶೂನ್ಯ sched_show_task(ರಚನೆ task_struct *p)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಬಿಡುಗಡೆ;
	ಪೂರ್ಣಾಂಕ ppid;

	ಆದರೆ (!try_get_task_ರಾಶಿ(p))
		ಹಿಂತಿರುಗಿ;

	pr_info("task:%-15.15s state:%c", p->comm, task_state_ಗಮ್ಯ_char(p));

	ಆದರೆ (task_is_running(p))
		pr_cont("  running task    ");
	ಬಿಡುಗಡೆ = ರಾಶಿ_not_ಬಳಸಿದ(p);
	ppid = 0;
	rcu_read_ಬೀಗ();
	ಆದರೆ (pid_alive(p))
		ppid = task_pid_nr(rcu_dereference(p->real_ಪೋಷಕ));
	rcu_read_unlock();
	pr_cont(" stack:%-5lu pid:%-5d tgid:%-5d ppid:%-6d task_flags:0x%04x flags:0x%08lx\n",
		ಬಿಡುಗಡೆ, task_pid_nr(p), task_tgid_nr(p),
		ppid, p->flags, read_task_thread_flags(p));

	print_ಕೆಲಸಗಾರ_info(KERN_INFO, p);
	print_stop_info(KERN_INFO, p);
	print_scx_info(KERN_INFO, p);
	show_ರಾಶಿ(p, ಖಾಲಿ, KERN_INFO);
	put_task_ರಾಶಿ(p);
}
EXPORT_ಚಿಹ್ನೆ_GPL(sched_show_task);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
state_filter_ಹೊಂದಿಕೆ(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ state_filter, ರಚನೆ task_struct *p)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ state = READ_ONCE(p->__state);

	/* no filter, everything matches */
	ಆದರೆ (!state_filter)
		ಹಿಂತಿರುಗಿ true;

	/* filter, but doesn't match */
	ಆದರೆ (!(state & state_filter))
		ಹಿಂತಿರುಗಿ false;

	/*
	 * When looking for TASK_UNINTERRUPTIBLE skip TASK_IDLE (allows
	 * TASK_KILLABLE).
	 */
	ಆದರೆ (state_filter == TASK_UNINTERRUPTIBLE && (state & TASK_NOLOAD))
		ಹಿಂತಿರುಗಿ false;

	ಹಿಂತಿರುಗಿ true;
}


ಶೂನ್ಯ show_state_filter(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ state_filter)
{
	ರಚನೆ task_struct *g, *p;

	rcu_read_ಬೀಗ();
	for_each_process_thread(g, p) {
		/*
		 * reset the NMI-timeout, listing all files on a slow
		 * console might take a lot of time:
		 * Also, reset softlockup watchdogs on all CPUs, because
		 * another CPU might be blocked waiting for us to process
		 * an IPI.
		 */
		touch_nmi_watchdog();
		touch_all_softlockup_watchdogs();
		ಆದರೆ (state_filter_ಹೊಂದಿಕೆ(state_filter, p))
			sched_show_task(p);
	}

	ಆದರೆ (!state_filter)
		sysrq_sched_debug_show();

	rcu_read_unlock();
	/*
	 * Only show locks if all tasks are dumped:
	 */
	ಆದರೆ (!state_filter)
		debug_show_all_locks();
}

/**
 * init_idle - set up an idle thread for a given CPU
 * @idle: task in question
 * @cpu: CPU the idle task belongs to
 *
 * NOTE: this function does not set the idle thread's NEED_RESCHED
 * flag, to make booting more robust.
 */
ಶೂನ್ಯ __init init_idle(ರಚನೆ task_struct *idle, ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ affinity_context ac = (ರಚನೆ affinity_context) {
		.new_ಮುಖವಾಡ  = cpumask_ದ(cpu),
		.flags     = 0,
	};
	ರಚನೆ rq *rq = cpu_rq(cpu);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	raw_spin_ಬೀಗ_irqsave(&idle->pi_ಬೀಗ, flags);
	raw_spin_rq_ಬೀಗ(rq);

	idle->__state = TASK_RUNNING;
	idle->se.exec_ಆರಂಭ = sched_clock();
	/*
	 * PF_KTHREAD should already be set at this point; regardless, make it
	 * look like a proper per-CPU kthread.
	 */
	idle->flags |= PF_KTHREAD | PF_NO_SETAFFINITY;
	kthread_ಗಣ_ಪ್ರತಿ_cpu(idle, cpu);

	/*
	 * No validation and serialization required at boot time and for
	 * setting up the idle tasks of not yet online CPUs.
	 */
	ಗಣ_cpus_allowed_common(idle, &ac);
	/*
	 * We're having a chicken and egg problem, even though we are
	 * holding rq->lock, the CPU isn't yet set to this CPU so the
	 * lockdep check in task_group() will fail.
	 *
	 * Similar case to sched_fork(). / Alternatively we could
	 * use task_rq_lock() here and obtain the other rq->lock.
	 *
	 * Silence PROVE_RCU
	 */
	rcu_read_ಬೀಗ();
	__set_task_cpu(idle, cpu);
	rcu_read_unlock();

	rq->idle = idle;
	rq_ಗಣ_donor(rq, idle);
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(rq->curr, idle);
	idle->on_rq = TASK_ON_RQ_QUEUED;
	idle->on_cpu = 1;
	raw_spin_rq_unlock(rq);
	raw_spin_unlock_irqrestore(&idle->pi_ಬೀಗ, flags);

	/* Set the preempt count _outside_ the spinlocks! */
	init_idle_preempt_ಎಣಿಕೆ(idle, cpu);

	/*
	 * The idle tasks have their own, simple scheduling class:
	 */
	idle->sched_class = &idle_sched_class;
	ftrace_ನಕ್ಷೆ_init_idle_task(idle, cpu);
	vtime_init_idle(idle, cpu);
	sprintf(idle->comm, "%s/%d", INIT_TASK_COMM, cpu);
}

ಪೂರ್ಣಾಂಕ cpuset_cpumask_can_shrink(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cur,
			      ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *trial)
{
	ಪೂರ್ಣಾಂಕ ret = 1;

	ಆದರೆ (cpumask_ಖಾಲಿಗಳು(cur))
		ಹಿಂತಿರುಗಿ ret;

	ret = dl_cpuset_cpumask_can_shrink(cur, trial);

	ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ task_can_attach(ರಚನೆ task_struct *p)
{
	ಪೂರ್ಣಾಂಕ ret = 0;

	/*
	 * Kthreads which disallow setaffinity shouldn't be moved
	 * to a new cpuset; we don't want to change their CPU
	 * affinity and isolating such threads by their set of
	 * allowed nodes is unnecessary.  Thus, cpusets are not
	 * applicable for such threads.  This prevents checking for
	 * success of set_cpus_allowed_ptr() on all attached tasks
	 * before cpus_mask may be changed.
	 */
	ಆದರೆ (p->flags & PF_NO_SETAFFINITY)
		ret = -EINVAL;

	ಹಿಂತಿರುಗಿ ret;
}

bool sched_smp_initialized __read_mostly;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
/* Migrate current task p to target_cpu */
ಪೂರ್ಣಾಂಕ migrate_task_ಗಮ್ಯ(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ ಗುರಿ_cpu)
{
	ರಚನೆ migration_arg arg = { p, ಗುರಿ_cpu };
	ಪೂರ್ಣಾಂಕ curr_cpu = task_cpu(p);

	ಆದರೆ (curr_cpu == ಗುರಿ_cpu)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!cpumask_test_cpu(ಗುರಿ_cpu, p->cpus_ptr))
		ಹಿಂತಿರುಗಿ -EINVAL;

	/* TODO: This is not properly updating schedstats */

	trace_sched_move_numa(p, curr_cpu, ಗುರಿ_cpu);
	ಹಿಂತಿರುಗಿ stop_one_cpu(curr_cpu, migration_cpu_stop, &arg);
}

/*
 * Requeue a task on a given node and accurately track the number of NUMA
 * tasks on the runqueues
 */
ಶೂನ್ಯ sched_setnuma(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ nid)
{
	guard(task_rq_ಬೀಗ_2)(p);
	scoped_guard (sched_change, p, ಸರತಿಯಿಂದ_ತೆಗೆ_SAVE)
		p->numa_preferred_nid = nid;
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_NUMA_BALANCING */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_HOTPLUG_CPU
/*
 * Invoked on the outgoing CPU in context of the CPU hotplug thread
 * after ensuring that there are no user space tasks left on the CPU.
 *
 * If there is a lazy mm in use on the hotplug thread, drop it and
 * switch to init_mm.
 *
 * The reference count on init_mm is dropped in finish_cpu().
 */
ಸ್ಥಿರ ಶೂನ್ಯ sched_force_init_mm(ಶೂನ್ಯ)
{
	ರಚನೆ mm_struct *mm = current->ಸಕ್ರಿಯ_mm;

	ಆದರೆ (mm != &init_mm) {
		mmgrab_lazy_tlb(&init_mm);
		local_irq_disable();
		current->ಸಕ್ರಿಯ_mm = &init_mm;
		switch_mm_irqs_off(mm, &init_mm, current);
		local_irq_enable();
		ಮುಕ್ತಾಯ_arch_post_ಬೀಗ_switch();
		mmdrop_lazy_tlb(mm);
	}

	/* finish_cpu(), as ran on the BP, will clean up the active_mm state */
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __balance_push_cpu_stop(ಶೂನ್ಯ *arg)
{
	ರಚನೆ task_struct *p = arg;
	ರಚನೆ rq *rq = this_rq();
	ರಚನೆ rq_flags rf;
	ಪೂರ್ಣಾಂಕ cpu;

	scoped_guard (raw_spinlock_irq, &p->pi_ಬೀಗ) {
		/*
		 * We may change the underlying rq, but the locks held will
		 * appropriately be "transferred" when switching.
		 */
		context_unsafe_alias(rq);

		cpu = select_fallback_rq(rq->cpu, p);

		rq_ಬೀಗ(rq, &rf);
		update_rq_clock(rq);
		ಆದರೆ (task_rq(p) == rq && task_on_rq_queued(p))
			rq = __migrate_task(rq, &rf, p, cpu);
		rq_unlock(rq, &rf);
	}

	put_task_struct(p);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ cpu_stop_work, ತಳ್ಳು_work);

/*
 * Ensure we only run per-cpu kthreads once the CPU goes !active.
 *
 * This is enabled below SCHED_AP_ACTIVE; when !cpu_active(), but only
 * effective when the hotplug motion is down.
 */
ಸ್ಥಿರ ಶೂನ್ಯ balance_ತಳ್ಳು(ರಚನೆ rq *rq)
	__must_hold(__rq_lockp(rq))
{
	ರಚನೆ task_struct *ತಳ್ಳು_task = rq->curr;

	lockdep_assert_rq_held(rq);

	/*
	 * Ensure the thing is persistent until balance_push_set(.on = false);
	 */
	rq->balance_callback = &balance_ತಳ್ಳು_callback;

	/*
	 * Only active while going offline and when invoked on the outgoing
	 * CPU.
	 */
	ಆದರೆ (!cpu_dying(rq->cpu) || rq != this_rq())
		ಹಿಂತಿರುಗಿ;

	/*
	 * Both the cpu-hotplug and stop task are in this case and are
	 * required to complete the hotplug process.
	 */
	ಆದರೆ (kthread_is_ಪ್ರತಿ_cpu(ತಳ್ಳು_task) ||
	    is_migration_disabled(ತಳ್ಳು_task)) {

		/*
		 * If this is the idle task on the outgoing CPU try to wake
		 * up the hotplug control thread which might wait for the
		 * last task to vanish. The rcuwait_active() check is
		 * accurate here because the waiter is pinned on this CPU
		 * and can't obviously be running in parallel.
		 *
		 * On RT kernels this also has to check whether there are
		 * pinned and scheduled out tasks on the runqueue. They
		 * need to leave the migrate disabled section first.
		 */
		ಆದರೆ (!rq->nr_running && !rq_has_pinned_tasks(rq) &&
		    rcuwait_ಸಕ್ರಿಯ(&rq->hotplug_wait)) {
			raw_spin_rq_unlock(rq);
			rcuwait_wake_ಮೇಲೆ(&rq->hotplug_wait);
			raw_spin_rq_ಬೀಗ(rq);
		}
		ಹಿಂತಿರುಗಿ;
	}

	get_task_struct(ತಳ್ಳು_task);
	/*
	 * Temporarily drop rq->lock such that we can wake-up the stop task.
	 * Both preemption and IRQs are still disabled.
	 */
	preempt_disable();
	raw_spin_rq_unlock(rq);
	stop_one_cpu_nowait(rq->cpu, __balance_push_cpu_stop, ತಳ್ಳು_task,
			    this_cpu_ptr(&ತಳ್ಳು_work));
	preempt_enable();
	/*
	 * At this point need_resched() is true and we'll take the loop in
	 * schedule(). The next pick is obviously going to be the stop task
	 * which kthread_is_per_cpu() and will push this task away.
	 */
	raw_spin_rq_ಬೀಗ(rq);
}

ಸ್ಥಿರ ಶೂನ್ಯ balance_ತಳ್ಳು_ಗಣ(ಪೂರ್ಣಾಂಕ cpu, bool on)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);
	ರಚನೆ rq_flags rf;

	rq_ಬೀಗ_irqsave(rq, &rf);
	ಆದರೆ (on) {
		WARN_ON_ONCE(rq->balance_callback);
		rq->balance_callback = &balance_ತಳ್ಳು_callback;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (rq->balance_callback == &balance_ತಳ್ಳು_callback) {
		rq->balance_callback = ಖಾಲಿ;
	}
	rq_unlock_irqrestore(rq, &rf);
}

/*
 * Invoked from a CPUs hotplug control thread after the CPU has been marked
 * inactive. All tasks which are not per CPU kernel threads are either
 * pushed off this CPU now via balance_push() or placed on a different CPU
 * during wakeup. Wait until the CPU is quiescent.
 */
ಸ್ಥಿರ ಶೂನ್ಯ balance_hotplug_wait(ಶೂನ್ಯ)
{
	ರಚನೆ rq *rq = this_rq();

	rcuwait_wait_event(&rq->hotplug_wait,
			   rq->nr_running == 1 && !rq_has_pinned_tasks(rq),
			   TASK_UNINTERRUPTIBLE);
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_HOTPLUG_CPU: */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ balance_ತಳ್ಳು(ರಚನೆ rq *rq)
{
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ balance_ತಳ್ಳು_ಗಣ(ಪೂರ್ಣಾಂಕ cpu, bool on)
{
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ balance_hotplug_wait(ಶೂನ್ಯ)
{
}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_HOTPLUG_CPU */

ಶೂನ್ಯ ಗಣ_rq_online(ರಚನೆ rq *rq)
{
	ಆದರೆ (!rq->online) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *class;

		cpumask_ಗಣ_cpu(rq->cpu, rq->rd->online);
		rq->online = 1;

		for_each_class(class) {
			ಆದರೆ (class->rq_online)
				class->rq_online(rq);
		}
	}
}

ಶೂನ್ಯ ಗಣ_rq_offline(ರಚನೆ rq *rq)
{
	ಆದರೆ (rq->online) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *class;

		update_rq_clock(rq);
		for_each_class(class) {
			ಆದರೆ (class->rq_offline)
				class->rq_offline(rq);
		}

		cpumask_clear_cpu(rq->cpu, rq->rd->online);
		rq->online = 0;
	}
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_ಗಣ_rq_online(ರಚನೆ rq *rq, ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ rq_flags rf;

	rq_ಬೀಗ_irqsave(rq, &rf);
	ಆದರೆ (rq->rd) {
		BUG_ON(!cpumask_test_cpu(cpu, rq->rd->ಹರವು));
		ಗಣ_rq_online(rq);
	}
	rq_unlock_irqrestore(rq, &rf);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_ಗಣ_rq_offline(ರಚನೆ rq *rq, ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ rq_flags rf;

	rq_ಬೀಗ_irqsave(rq, &rf);
	ಆದರೆ (rq->rd) {
		BUG_ON(!cpumask_test_cpu(cpu, rq->rd->ಹರವು));
		ಗಣ_rq_offline(rq);
	}
	rq_unlock_irqrestore(rq, &rf);
}

/*
 * used to mark begin/end of suspend/resume:
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಅಂಕೆ_cpus_frozen;

/*
 * Update cpusets according to cpu_active mask.  If cpusets are
 * disabled, cpuset_update_active_cpus() becomes a simple wrapper
 * around partition_sched_domains().
 *
 * If we come here as part of a suspend/resume, don't touch cpusets because we
 * want to restore it back to its original state upon resume anyway.
 */
ಸ್ಥಿರ ಶೂನ್ಯ cpuset_cpu_ಸಕ್ರಿಯ(ಶೂನ್ಯ)
{
	ಆದರೆ (cpuhp_tasks_frozen) {
		/*
		 * num_cpus_frozen tracks how many CPUs are involved in suspend
		 * resume sequence. As long as this is not the last online
		 * operation in the resume sequence, just build a single sched
		 * domain, ignoring cpusets.
		 */
		cpuset_reset_sched_domains();
		ಆದರೆ (--ಅಂಕೆ_cpus_frozen)
			ಹಿಂತಿರುಗಿ;
		/*
		 * This is the last CPU online operation. So fall through and
		 * restore the original sched domains by considering the
		 * cpuset configurations.
		 */
		cpuset_force_rebuild();
	}
	cpuset_update_ಸಕ್ರಿಯ_cpus();
}

ಸ್ಥಿರ ಶೂನ್ಯ cpuset_cpu_inactive(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu)
{
	ಆದರೆ (!cpuhp_tasks_frozen) {
		cpuset_update_ಸಕ್ರಿಯ_cpus();
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಅಂಕೆ_cpus_frozen++;
		cpuset_reset_sched_domains();
	}
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_smt_present_inc(ಪೂರ್ಣಾಂಕ cpu)
{
	ಆದರೆ (cpumask_ತೂಕ(cpu_smt_ಮುಖವಾಡ(cpu)) == 2)
		static_branch_inc_cpuslocked(&sched_smt_present);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_smt_present_dec(ಪೂರ್ಣಾಂಕ cpu)
{
	ಆದರೆ (cpumask_ತೂಕ(cpu_smt_ಮುಖವಾಡ(cpu)) == 2)
		static_branch_dec_cpuslocked(&sched_smt_present);
}

ಪೂರ್ಣಾಂಕ sched_cpu_activate(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);

	/*
	 * Clear the balance_push callback and prepare to schedule
	 * regular tasks.
	 */
	balance_ತಳ್ಳು_ಗಣ(cpu, false);

	/*
	 * When going up, increment the number of cores with SMT present.
	 */
	sched_smt_present_inc(cpu);
	ಗಣ_cpu_ಸಕ್ರಿಯ(cpu, true);

	ಆದರೆ (sched_smp_initialized) {
		sched_update_numa(cpu, true);
		sched_domains_numa_masks_ಗಣ(cpu);
		cpuset_cpu_ಸಕ್ರಿಯ();
	}

	scx_rq_activate(rq);

	/*
	 * Put the rq online, if not already. This happens:
	 *
	 * 1) In the early boot process, because we build the real domains
	 *    after all CPUs have been brought up.
	 *
	 * 2) At runtime, if cpuset_cpu_active() fails to rebuild the
	 *    domains.
	 */
	sched_ಗಣ_rq_online(rq, cpu);

	/* preferred is subset of active and follows its state */
	ಗಣ_cpu_preferred(cpu, true);

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ sched_cpu_deactivate(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);
	ಪೂರ್ಣಾಂಕ ret;

	ret = dl_bw_deactivate(cpu);

	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;

	ಗಣ_cpu_preferred(cpu, false);

	/*
	 * Remove CPU from nohz.idle_cpus_mask to prevent participating in
	 * load balancing when not active
	 */
	scoped_guard (rcu)
		nohz_balance_exit_idle(rq);

	ಗಣ_cpu_ಸಕ್ರಿಯ(cpu, false);

	/*
	 * From this point forward, this CPU will refuse to run any task that
	 * is not: migrate_disable() or KTHREAD_IS_PER_CPU, and will actively
	 * push those tasks away until this gets cleared, see
	 * sched_cpu_dying().
	 */
	balance_ತಳ್ಳು_ಗಣ(cpu, true);

	/*
	 * We've cleared cpu_active_mask / set balance_push, wait for all
	 * preempt-disabled and RCU users of this state to go away such that
	 * all new such users will observe it.
	 *
	 * Specifically, we rely on ttwu to no longer target this CPU, see
	 * ttwu_queue_cond() and is_cpu_allowed().
	 *
	 * Do sync before park smpboot threads to take care the RCU boost case.
	 */
	synchronize_rcu();

	sched_domains_free_llc_id(cpu);

	sched_ಗಣ_rq_offline(rq, cpu);

	scx_rq_deactivate(rq);

	/*
	 * When going down, decrement the number of cores with SMT present.
	 */
	sched_smt_present_dec(cpu);

	sched_core_cpu_deactivate(cpu);

	ಆದರೆ (!sched_smp_initialized)
		ಹಿಂತಿರುಗಿ 0;

	sched_update_numa(cpu, false);
	cpuset_cpu_inactive(cpu);
	sched_domains_numa_masks_clear(cpu);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_rq_cpu_starting(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);

	rq->calc_ತುಂಬು_update = calc_ತುಂಬು_update;
	update_max_interval();
}

ಪೂರ್ಣಾಂಕ sched_cpu_starting(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu)
{
	sched_core_cpu_starting(cpu);
	sched_rq_cpu_starting(cpu);
	sched_tick_ಆರಂಭ(cpu);
	ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_HOTPLUG_CPU

/*
 * Invoked immediately before the stopper thread is invoked to bring the
 * CPU down completely. At this point all per CPU kthreads except the
 * hotplug thread (current) and the stopper thread (inactive) have been
 * either parked or have been unbound from the outgoing CPU. Ensure that
 * any of those which might be on the way out are gone.
 *
 * If after this point a bound task is being woken on this CPU then the
 * responsible hotplug callback has failed to do it's job.
 * sched_cpu_dying() will catch it with the appropriate fireworks.
 */
ಪೂರ್ಣಾಂಕ sched_cpu_wait_ಖಾಲಿಗಳು(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu)
{
	balance_hotplug_wait();
	sched_force_init_mm();
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Since this CPU is going 'away' for a while, fold any nr_active delta we
 * might have. Called from the CPU stopper task after ensuring that the
 * stopper is the last running task on the CPU, so nr_active count is
 * stable. We need to take the tear-down thread which is calling this into
 * account, so we hand in adjust = 1 to the load calculation.
 *
 * Also see the comment "Global load-average calculations".
 */
ಸ್ಥಿರ ಶೂನ್ಯ calc_ತುಂಬು_migrate(ರಚನೆ rq *rq)
{
	ದೀರ್ಘ delta = calc_ತುಂಬು_fold_ಸಕ್ರಿಯ(rq, 1);

	ಆದರೆ (delta)
		atomic_long_ಕೂಡಿಸು(delta, &calc_ತುಂಬು_tasks);
}

ಸ್ಥಿರ ಶೂನ್ಯ dump_rq_tasks(ರಚನೆ rq *rq, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *loglvl)
{
	ರಚನೆ task_struct *g, *p;
	ಪೂರ್ಣಾಂಕ cpu = cpu_ದ(rq);

	lockdep_assert_rq_held(rq);

	printk("%sCPU%d enqueued tasks (%u total):\n", loglvl, cpu, rq->nr_running);
	for_each_process_thread(g, p) {
		ಆದರೆ (task_cpu(p) != cpu)
			ಮುಂದುವರಿ;

		ಆದರೆ (!task_on_rq_queued(p))
			ಮುಂದುವರಿ;

		printk("%s\tpid: %d, name: %s\n", loglvl, p->pid, p->comm);
	}
}

ಪೂರ್ಣಾಂಕ sched_cpu_dying(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);
	ರಚನೆ rq_flags rf;

	/* Handle pending wakeups and then migrate everything off */
	sched_tick_stop(cpu);

	rq_ಬೀಗ_irqsave(rq, &rf);
	update_rq_clock(rq);
	ಆದರೆ (rq->nr_running != 1 || rq_has_pinned_tasks(rq)) {
		WARN(true, "Dying CPU not properly vacated!");
		dump_rq_tasks(rq, KERN_WARNING);
	}
	dl_server_stop(&rq->fair_server);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CLASS_EXT
	dl_server_stop(&rq->ext_server);
#ಆದರೆ_ಅಂತ್ಯ
	rq_unlock_irqrestore(rq, &rf);

	calc_ತುಂಬು_migrate(rq);
	update_max_interval();
	hrtick_clear(rq);
	sched_core_cpu_dying(cpu);
	ಹಿಂತಿರುಗಿ 0;
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_HOTPLUG_CPU */

ಶೂನ್ಯ __init sched_init_smp(ಶೂನ್ಯ)
{
	sched_init_numa(NUMA_NO_ಗಂಟು);

	prandom_init_once(&sched_rnd_state);

	/*
	 * There's no userspace yet to cause hotplug operations; hence all the
	 * CPU masks are stable and all blatant races in the below code cannot
	 * happen.
	 */
	sched_domains_mutex_ಬೀಗ();
	sched_init_domains(cpu_ಸಕ್ರಿಯ_ಮುಖವಾಡ);
	sched_domains_mutex_unlock();

	/* Move init over to a non-isolated CPU */
	ಆದರೆ (ಗಣ_cpus_allowed_ptr(current, housekeeping_cpumask(HK_TYPE_DOMAIN)) < 0)
		BUG();
	current->flags &= ~PF_NO_SETAFFINITY;
	sched_init_granularity();

	init_sched_rt_class();
	init_sched_dl_class();

	sched_init_dl_servers();

	sched_smp_initialized = true;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init migration_init(ಶೂನ್ಯ)
{
	sched_cpu_starting(smp_processor_id());
	ಹಿಂತಿರುಗಿ 0;
}
early_initcall(migration_init);

ಪೂರ್ಣಾಂಕ ಒಳ_sched_functions(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ addr)
{
	ಹಿಂತಿರುಗಿ ಒಳ_ಬೀಗ_functions(addr) ||
		(addr >= (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)__sched_text_start
		&& addr < (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)__sched_text_end);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CGROUP_SCHED
/*
 * Default task group.
 * Every task in system belongs to this group at bootup.
 */
ರಚನೆ task_ಗುಂಪು ಬೇರು_task_ಗುಂಪು;
LIST_ತಲೆ(task_groups);

/* Cacheline aligned slab cache for task_group */
ಸ್ಥಿರ ರಚನೆ kmem_cache *task_ಗುಂಪು_cache __ro_after_init;
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ __init sched_init(ಶೂನ್ಯ)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ __maybe_unused ptr = 0;
	ಪೂರ್ಣಾಂಕ i;

	/* Make sure the linker didn't screw up */
	BUG_ON(!sched_class_above(&stop_sched_class, &dl_sched_class));
	BUG_ON(!sched_class_above(&dl_sched_class, &rt_sched_class));
	BUG_ON(!sched_class_above(&rt_sched_class, &fair_sched_class));
	BUG_ON(!sched_class_above(&fair_sched_class, &idle_sched_class));
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CLASS_EXT
	BUG_ON(!sched_class_above(&fair_sched_class, &ext_sched_class));
	BUG_ON(!sched_class_above(&ext_sched_class, &idle_sched_class));
#ಆದರೆ_ಅಂತ್ಯ

	wait_ಬಿಟ್_init();

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
	ಬೇರು_task_ಗುಂಪು.cfs_rq = &runqueues.cfs;

	ಬೇರು_task_ಗುಂಪು.shares = ಬೇರು_TASK_ಗುಂಪು_ತುಂಬು;
	init_cfs_bandwidth(&ಬೇರು_task_ಗುಂಪು.cfs_bandwidth, ಖಾಲಿ);
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FAIR_GROUP_SCHED */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_ಗುಂಪು_SCHED
	scx_tg_init(&ಬೇರು_task_ಗುಂಪು);
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_EXT_GROUP_SCHED */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED
	ptr += 2 * nr_cpu_ids * ಗಾತ್ರ(ಶೂನ್ಯ **);
	ptr = (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)kzalloc(ptr, GFP_NOWAIT);
	ಬೇರು_task_ಗುಂಪು.rt_se = (ರಚನೆ sched_rt_entity **)ptr;
	ptr += nr_cpu_ids * ಗಾತ್ರ(ಶೂನ್ಯ **);

	ಬೇರು_task_ಗುಂಪು.rt_rq = (ರಚನೆ rt_rq **)ptr;
	ptr += nr_cpu_ids * ಗಾತ್ರ(ಶೂನ್ಯ **);

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_RT_GROUP_SCHED */

	init_defrootdomain();

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED
	init_rt_bandwidth(&ಬೇರು_task_ಗುಂಪು.rt_bandwidth,
			global_rt_period(), global_rt_runtime());
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_RT_GROUP_SCHED */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CGROUP_SCHED
	task_ಗುಂಪು_cache = KMEM_CACHE(task_ಗುಂಪು, 0);

	list_ಕೂಡಿಸು(&ಬೇರು_task_ಗುಂಪು.list, &task_groups);
	INIT_LIST_ತಲೆ(&ಬೇರು_task_ಗುಂಪು.children);
	INIT_LIST_ತಲೆ(&ಬೇರು_task_ಗುಂಪು.siblings);
	autogroup_init(&init_task);
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_CGROUP_SCHED */

	for_each_possible_cpu(i) {
		ರಚನೆ rq *rq;

		rq = cpu_rq(i);
		raw_spin_ಬೀಗ_init(&rq->__lock);
		rq->nr_running = 0;
		rq->calc_ತುಂಬು_ಸಕ್ರಿಯ = 0;
		rq->calc_ತುಂಬು_update = jiffies + ತುಂಬು_FREQ;
		init_cfs_rq(&rq->cfs);
		init_rt_rq(&rq->rt);
		init_dl_rq(&rq->dl);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
		INIT_LIST_ತಲೆ(&rq->ಎಲೆ_cfs_rq_list);
		rq->tmp_alone_branch = &rq->ಎಲೆ_cfs_rq_list;
		/*
		 * How much CPU bandwidth does root_task_group get?
		 *
		 * In case of task-groups formed through the cgroup filesystem, it
		 * gets 100% of the CPU resources in the system. This overall
		 * system CPU resource is divided among the tasks of
		 * root_task_group and its child task-groups in a fair manner,
		 * based on each entity's (task or task-group's) weight
		 * (se->load.weight).
		 *
		 * In other words, if root_task_group has 10 tasks of weight
		 * 1024) and two child groups A0 and A1 (of weight 1024 each),
		 * then A0's share of the CPU resource is:
		 *
		 *	A0's bandwidth = 1024 / (10*1024 + 1024 + 1024) = 8.33%
		 *
		 * We achieve this by letting root_task_group's tasks sit
		 * directly in rq->cfs (i.e root_task_group->se[] = NULL).
		 */
		init_tg_cfs_ನಮೂದು(&ಬೇರು_task_ಗುಂಪು, &rq->cfs, ಖಾಲಿ, i, ಖಾಲಿ);
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FAIR_GROUP_SCHED */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED
		/*
		 * This is required for init cpu because rt.c:__enable_runtime()
		 * starts working after scheduler_running, which is not the case
		 * yet.
		 */
		rq->rt.rt_runtime = global_rt_runtime();
		init_tg_rt_ನಮೂದು(&ಬೇರು_task_ಗುಂಪು, &rq->rt, ಖಾಲಿ, i, ಖಾಲಿ);
#ಆದರೆ_ಅಂತ್ಯ
		rq->ಮುಂದಿನ_class = &idle_sched_class;

		rq->sd = ಖಾಲಿ;
		rq->rd = ಖಾಲಿ;
		rq->cpu_ಸಾಮರ್ಥ್ಯ = SCHED_ಸಾಮರ್ಥ್ಯ_SCALE;
		rq->balance_callback = &balance_ತಳ್ಳು_callback;
		rq->ಸಕ್ರಿಯ_balance = 0;
		rq->ಮುಂದಿನ_balance = jiffies;
		rq->ತಳ್ಳು_cpu = 0;
		rq->cpu = i;
		rq->online = 0;
		rq->idle_stamp = 0;
		rq->avg_idle = 2*sysctl_sched_migration_ವೆಚ್ಚ;
		rq->max_idle_balance_ವೆಚ್ಚ = sysctl_sched_migration_ವೆಚ್ಚ;

		INIT_LIST_ತಲೆ(&rq->cfs_tasks);

		rq_attach_ಬೇರು(rq, &def_ಬೇರು_domain);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NO_HZ_COMMON
		rq->ಕೊನೆಯ_blocked_ತುಂಬು_update_tick = jiffies;
		atomic_ಗಣ(&rq->nohz_flags, 0);

		INIT_CSD(&rq->nohz_csd, nohz_csd_func, rq);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_HOTPLUG_CPU
		rcuwait_init(&rq->hotplug_wait);
#ಆದರೆ_ಅಂತ್ಯ
		hrtick_rq_init(rq);
		atomic_ಗಣ(&rq->nr_iowait, 0);
		fair_server_init(rq);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CLASS_EXT
		ext_server_init(rq);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CORE
		rq->core = rq;
		rq->core_pick = ಖಾಲಿ;
		rq->core_dl_server = ಖಾಲಿ;
		rq->core_enabled = 0;
		rq->core_ವೃಕ್ಷ = RB_ಬೇರು;
		rq->core_forceidle_ಎಣಿಕೆ = 0;
		rq->core_forceidle_occupation = 0;
		rq->core_forceidle_ಆರಂಭ = 0;
		rq->core_pick_ಒಳ_flight = 0;

		rq->core_cookie = 0UL;
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CACHE
		raw_spin_ಬೀಗ_init(&rq->cpu_epoch_ಬೀಗ);
		rq->cpu_epoch_ಮುಂದಿನ = jiffies;
#ಆದರೆ_ಅಂತ್ಯ

		zalloc_cpumask_var_ಗಂಟು(&rq->scratch_ಮುಖವಾಡ, GFP_KERNEL, cpu_ಗಮ್ಯ_ಗಂಟು(i));
	}

	ಗಣ_ತುಂಬು_ತೂಕ(&init_task, false);
	init_task.se.ತುಂಡು = sysctl_sched_ತಳ_ತುಂಡು,

	/*
	 * The boot idle thread does lazy MMU switching as well:
	 */
	mmgrab_lazy_tlb(&init_mm);
	enter_lazy_tlb(&init_mm, current);

	/*
	 * The idle task doesn't need the kthread struct to function, but it
	 * is dressed up as a per-CPU kthread and thus needs to play the part
	 * if we want to avoid special-casing it in code that deals with per-CPU
	 * kthreads.
	 */
	WARN_ON(!ಗಣ_kthread_struct(current));

	/*
	 * Make us the idle thread. Technically, schedule() should not be
	 * called from this thread, however somewhere below it might be,
	 * but because we are the idle thread, we just pick up running again
	 * when this runqueue becomes "idle".
	 */
	__sched_fork(0, current);
	init_idle(current, smp_processor_id());

	calc_ತುಂಬು_update = jiffies + ತುಂಬು_FREQ;

	idle_thread_ಗಣ_boot_cpu();

	balance_ತಳ್ಳು_ಗಣ(smp_processor_id(), false);
	init_sched_fair_class();
	init_sched_ext_class();

	psi_init();

	init_uclamp();

	preempt_dynamic_init();

	scheduler_running = 1;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_DEBUG_ATOMIC_SLEEP

ಶೂನ್ಯ __might_sleep(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, ಪೂರ್ಣಾಂಕ ಸಾಲು)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ state = get_current_state();
	/*
	 * Blocking primitives will set (and therefore destroy) current->state,
	 * since we will exit with TASK_RUNNING make sure we enter with it,
	 * otherwise we will destroy state.
	 */
	WARN_ONCE(state != TASK_RUNNING && current->task_state_change,
			"do not call blocking ops when !TASK_RUNNING; "
			"state=%x set at [<%p>] %pS\n", state,
			(ಶೂನ್ಯ *)current->task_state_change,
			(ಶೂನ್ಯ *)current->task_state_change);

	__might_resched(file, ಸಾಲು, 0);
}
EXPORT_ಚಿಹ್ನೆ(__might_sleep);

ಸ್ಥಿರ ಶೂನ್ಯ print_preempt_disable_ip(ಪೂರ್ಣಾಂಕ preempt_ಸ್ಥಾನಾಂತರ, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ip)
{
	ಆದರೆ (!IS_ENABLED(CONFIG_DEBUG_PREEMPT))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (preempt_ಎಣಿಕೆ() == preempt_ಸ್ಥಾನಾಂತರ)
		ಹಿಂತಿರುಗಿ;

	pr_err("Preemption disabled at:");
	print_ip_sym(KERN_ERR, ip);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool resched_offsets_ok(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ offsets)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nested = preempt_ಎಣಿಕೆ();

	nested += rcu_preempt_ಆಳ() << MIGHT_RESCHED_RCU_ಸರಿಸು;

	ಹಿಂತಿರುಗಿ nested == offsets;
}

ಶೂನ್ಯ __might_resched(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, ಪೂರ್ಣಾಂಕ ಸಾಲು, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ offsets)
{
	/* Ratelimiting timestamp: */
	ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ prev_jiffy;

	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ preempt_disable_ip;

	/* WARN_ON_ONCE() by default, no rate limit required: */
	rcu_sleep_check();

	ಆದರೆ ((resched_offsets_ok(offsets) && !irqs_disabled() &&
	     !is_idle_task(current) && !current->non_ಖಂಡ_ಎಣಿಕೆ) ||
	    system_state == SYSTEM_BOOTING || system_state > SYSTEM_RUNNING ||
	    oops_ಒಳ_ಪ್ರಗತಿ)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (ಸಮಯ_before(jiffies, prev_jiffy + HZ) && prev_jiffy)
		ಹಿಂತಿರುಗಿ;
	prev_jiffy = jiffies;

	/* Save this before calling printk(), since that will clobber it: */
	preempt_disable_ip = get_preempt_disable_ip(current);

	pr_err("BUG: sleeping function called from invalid context at %s:%d\n",
	       file, ಸಾಲು);
	pr_err("in_atomic(): %d, irqs_disabled(): %d, non_block: %d, pid: %d, name: %s\n",
	       ಒಳ_atomic(), irqs_disabled(), current->non_ಖಂಡ_ಎಣಿಕೆ,
	       current->pid, current->comm);
	pr_err("preempt_count: %x, expected: %x\n", preempt_ಎಣಿಕೆ(),
	       offsets & MIGHT_RESCHED_PREEMPT_ಮುಖವಾಡ);

	ಆದರೆ (IS_ENABLED(CONFIG_PREEMPT_RCU)) {
		pr_err("RCU nest depth: %d, expected: %u\n",
		       rcu_preempt_ಆಳ(), offsets >> MIGHT_RESCHED_RCU_ಸರಿಸು);
	}

	ಆದರೆ (task_ರಾಶಿ_ಕೊನೆ_corrupted(current))
		pr_emerg("Thread overran stack, or stack corrupted\n");

	debug_show_held_locks(current);
	ಆದರೆ (irqs_disabled())
		print_irqtrace_events(current);

	print_preempt_disable_ip(offsets & MIGHT_RESCHED_PREEMPT_ಮುಖವಾಡ,
				 preempt_disable_ip);

	dump_ರಾಶಿ();
	ಕೂಡಿಸು_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}
EXPORT_ಚಿಹ್ನೆ(__might_resched);

ಶೂನ್ಯ __cant_sleep(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, ಪೂರ್ಣಾಂಕ ಸಾಲು)
{
	ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ prev_jiffy;

	ಆದರೆ (irqs_disabled())
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!IS_ENABLED(CONFIG_PREEMPT_ಎಣಿಕೆ))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (preempt_ಎಣಿಕೆ())
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (ಸಮಯ_before(jiffies, prev_jiffy + HZ) && prev_jiffy)
		ಹಿಂತಿರುಗಿ;
	prev_jiffy = jiffies;

	printk(KERN_ERR "BUG: assuming atomic context at %s:%d\n", file, ಸಾಲು);
	printk(KERN_ERR "in_atomic(): %d, irqs_disabled(): %d, pid: %d, name: %s\n",
			ಒಳ_atomic(), irqs_disabled(),
			current->pid, current->comm);

	debug_show_held_locks(current);
	dump_ರಾಶಿ();
	ಕೂಡಿಸು_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}
EXPORT_ಚಿಹ್ನೆ_GPL(__cant_sleep);

# ifdef CONFIG_SMP
ಶೂನ್ಯ __cant_migrate(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, ಪೂರ್ಣಾಂಕ ಸಾಲು)
{
	ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ prev_jiffy;

	ಆದರೆ (irqs_disabled())
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (is_migration_disabled(current))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!IS_ENABLED(CONFIG_PREEMPT_ಎಣಿಕೆ))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (preempt_ಎಣಿಕೆ())
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (ಸಮಯ_before(jiffies, prev_jiffy + HZ) && prev_jiffy)
		ಹಿಂತಿರುಗಿ;
	prev_jiffy = jiffies;

	pr_err("BUG: assuming non migratable context at %s:%d\n", file, ಸಾಲು);
	pr_err("in_atomic(): %d, irqs_disabled(): %d, migration_disabled() %u pid: %d, name: %s\n",
	       ಒಳ_atomic(), irqs_disabled(), is_migration_disabled(current),
	       current->pid, current->comm);

	debug_show_held_locks(current);
	dump_ರಾಶಿ();
	ಕೂಡಿಸು_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}
EXPORT_ಚಿಹ್ನೆ_GPL(__cant_migrate);
# endif /* CONFIG_SMP */
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_DEBUG_ATOMIC_SLEEP */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_MAGIC_SYSRQ
ಶೂನ್ಯ normalize_rt_tasks(ಶೂನ್ಯ)
{
	ರಚನೆ task_struct *g, *p;
	ರಚನೆ sched_attr attr = {
		.sched_policy = SCHED_ಸಾಮಾನ್ಯ,
	};

	read_ಬೀಗ(&tasklist_ಬೀಗ);
	for_each_process_thread(g, p) {
		/*
		 * Only normalize user tasks:
		 */
		ಆದರೆ (p->flags & PF_KTHREAD)
			ಮುಂದುವರಿ;

		p->se.exec_ಆರಂಭ = 0;
		schedstat_ಗಣ(p->stats.wait_ಆರಂಭ,  0);
		schedstat_ಗಣ(p->stats.sleep_ಆರಂಭ, 0);
		schedstat_ಗಣ(p->stats.ಖಂಡ_ಆರಂಭ, 0);

		ಆದರೆ (!rt_or_dl_task(p)) {
			/*
			 * Renice negative nice level userspace
			 * tasks back to 0:
			 */
			ಆದರೆ (task_nice(p) < 0)
				ಗಣ_user_nice(p, 0);
			ಮುಂದುವರಿ;
		}

		__sched_setscheduler(p, &attr, false, false);
	}
	read_unlock(&tasklist_ಬೀಗ);
}

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_MAGIC_SYSRQ */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_KGDB_KDB
/*
 * These functions are only useful for KDB.
 *
 * They can only be called when the whole system has been
 * stopped - every CPU needs to be quiescent, and no scheduling
 * activity can take place. Using them for anything else would
 * be a serious bug, and as a result, they aren't even visible
 * under any other configuration.
 */

/**
 * curr_task - return the current task for a given CPU.
 * @cpu: the processor in question.
 *
 * ONLY VALID WHEN THE WHOLE SYSTEM IS STOPPED!
 *
 * Return: The current task for @cpu.
 */
ರಚನೆ task_struct *curr_task(ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ cpu_curr(cpu);
}

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_KGDB_KDB */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CGROUP_SCHED
/* task_group_lock serializes the addition/removal of task groups */
ಸ್ಥಿರ DEFINE_SPINLOCK(task_ಗುಂಪು_ಬೀಗ);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ alloc_uclamp_sched_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg,
					    ರಚನೆ task_ಗುಂಪು *ಪೋಷಕ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK_ಗುಂಪು
	ಎಣಿಕೆ uclamp_id clamp_id;

	for_each_clamp_id(clamp_id) {
		uclamp_se_ಗಣ(&tg->uclamp_req[clamp_id],
			      uclamp_none(clamp_id), false);
		tg->uclamp[clamp_id] = ಪೋಷಕ->uclamp[clamp_id];
	}
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_free_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg)
{
	free_fair_sched_ಗುಂಪು(tg);
	free_rt_sched_ಗುಂಪು(tg);
	autogroup_free(tg);
	kmem_cache_free(task_ಗುಂಪು_cache, tg);
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_free_ಗುಂಪು_rcu(ರಚನೆ rcu_ತಲೆ *rcu)
{
	sched_free_ಗುಂಪು(container_ದ(rcu, ರಚನೆ task_ಗುಂಪು, rcu));
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_unregister_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg)
{
	unregister_fair_sched_ಗುಂಪು(tg);
	unregister_rt_sched_ಗುಂಪು(tg);
	/*
	 * We have to wait for yet another RCU grace period to expire, as
	 * print_cfs_stats() might run concurrently.
	 */
	call_rcu(&tg->rcu, sched_free_ಗುಂಪು_rcu);
}

/* allocate runqueue etc for a new task group */
ರಚನೆ task_ಗುಂಪು *sched_create_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *ಪೋಷಕ)
{
	ರಚನೆ task_ಗುಂಪು *tg;

	tg = kmem_cache_alloc(task_ಗುಂಪು_cache, GFP_KERNEL | __GFP_ZERO);
	ಆದರೆ (!tg)
		ಹಿಂತಿರುಗಿ ERR_PTR(-ENOMEM);

	ಆದರೆ (!alloc_fair_sched_ಗುಂಪು(tg, ಪೋಷಕ))
		ಹೋಗು err;

	ಆದರೆ (!alloc_rt_sched_ಗುಂಪು(tg, ಪೋಷಕ))
		ಹೋಗು err;

	scx_tg_init(tg);
	alloc_uclamp_sched_ಗುಂಪು(tg, ಪೋಷಕ);

	ಹಿಂತಿರುಗಿ tg;

err:
	sched_free_ಗುಂಪು(tg);
	ಹಿಂತಿರುಗಿ ERR_PTR(-ENOMEM);
}

ಶೂನ್ಯ sched_online_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg, ರಚನೆ task_ಗುಂಪು *ಪೋಷಕ)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	spin_ಬೀಗ_irqsave(&task_ಗುಂಪು_ಬೀಗ, flags);
	list_ಕೂಡಿಸು_ಬಾಲ_rcu(&tg->list, &task_groups);

	/* Root should already exist: */
	WARN_ON(!ಪೋಷಕ);

	tg->ಪೋಷಕ = ಪೋಷಕ;
	INIT_LIST_ತಲೆ(&tg->children);
	list_ಕೂಡಿಸು_rcu(&tg->siblings, &ಪೋಷಕ->children);
	spin_unlock_irqrestore(&task_ಗುಂಪು_ಬೀಗ, flags);

	online_fair_sched_ಗುಂಪು(tg);
}

/* RCU callback to free various structures associated with a task group */
ಸ್ಥಿರ ಶೂನ್ಯ sched_unregister_ಗುಂಪು_rcu(ರಚನೆ rcu_ತಲೆ *rhp)
{
	/* Now it should be safe to free those cfs_rqs: */
	sched_unregister_ಗುಂಪು(container_ದ(rhp, ರಚನೆ task_ಗುಂಪು, rcu));
}

ಶೂನ್ಯ sched_destroy_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg)
{
	/* Wait for possible concurrent references to cfs_rqs complete: */
	call_rcu(&tg->rcu, sched_unregister_ಗುಂಪು_rcu);
}

ಶೂನ್ಯ sched_release_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	/*
	 * Unlink first, to avoid walk_tg_tree_from() from finding us (via
	 * sched_cfs_period_timer()).
	 *
	 * For this to be effective, we have to wait for all pending users of
	 * this task group to leave their RCU critical section to ensure no new
	 * user will see our dying task group any more. Specifically ensure
	 * that tg_unthrottle_up() won't add decayed cfs_rq's to it.
	 *
	 * We therefore defer calling unregister_fair_sched_group() to
	 * sched_unregister_group() which is guarantied to get called only after the
	 * current RCU grace period has expired.
	 */
	spin_ಬೀಗ_irqsave(&task_ಗುಂಪು_ಬೀಗ, flags);
	list_del_rcu(&tg->list);
	list_del_rcu(&tg->siblings);
	spin_unlock_irqrestore(&task_ಗುಂಪು_ಬೀಗ, flags);
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_change_ಗುಂಪು(ರಚನೆ task_struct *tsk)
{
	ರಚನೆ task_ಗುಂಪು *tg;

	/*
	 * All callers are synchronized by task_rq_lock(); we do not use RCU
	 * which is pointless here. Thus, we pass "true" to task_css_check()
	 * to prevent lockdep warnings.
	 */
	tg = container_ದ(task_css_check(tsk, cpu_cgrp_id, true),
			  ರಚನೆ task_ಗುಂಪು, css);
	tg = autogroup_task_ಗುಂಪು(tsk, tg);
	tsk->sched_task_ಗುಂಪು = tg;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
	ಆದರೆ (tsk->sched_class->task_change_ಗುಂಪು)
		tsk->sched_class->task_change_ಗುಂಪು(tsk);
	ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
		ಗಣ_task_rq(tsk, task_cpu(tsk));
}

/*
 * Change task's runqueue when it moves between groups.
 *
 * The caller of this function should have put the task in its new group by
 * now. This function just updates tsk->se.cfs_rq and tsk->se.parent to reflect
 * its new group.
 */
ಶೂನ್ಯ sched_move_task(ರಚನೆ task_struct *tsk, bool for_autogroup)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸರತಿ_flags = ಸರತಿಯಿಂದ_ತೆಗೆ_SAVE | ಸರತಿಯಿಂದ_ತೆಗೆ_MOVE;
	bool resched = false;
	bool queued = false;
	ರಚನೆ rq *rq;

	CLASS(task_rq_ಬೀಗ_2, rq_guard)(tsk);
	rq = rq_guard.rq;

	scoped_guard (sched_change, tsk, ಸರತಿ_flags) {
		sched_change_ಗುಂಪು(tsk);
		ಆದರೆ (!for_autogroup)
			scx_cgroup_move_task(tsk);
		ಆದರೆ (scope->running)
			resched = true;
		queued = scope->queued;
	}

	ಆದರೆ (resched)
		resched_curr(rq);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (queued)
		wakeup_preempt(rq, tsk, 0);

	__balance_callbacks(rq, &rq_guard.rf);
}

ಸ್ಥಿರ ರಚನೆ cgroup_subsys_state *
cpu_cgroup_css_alloc(ರಚನೆ cgroup_subsys_state *ಪೋಷಕ_css)
{
	ರಚನೆ task_ಗುಂಪು *ಪೋಷಕ = css_tg(ಪೋಷಕ_css);
	ರಚನೆ task_ಗುಂಪು *tg;

	ಆದರೆ (!ಪೋಷಕ) {
		/* This is early initialization for the top cgroup */
		ಹಿಂತಿರುಗಿ &ಬೇರು_task_ಗುಂಪು.css;
	}

	tg = sched_create_ಗುಂಪು(ಪೋಷಕ);
	ಆದರೆ (IS_ERR(tg))
		ಹಿಂತಿರುಗಿ ERR_PTR(-ENOMEM);

	ಹಿಂತಿರುಗಿ &tg->css;
}

/* Expose task group only after completing cgroup initialization */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpu_cgroup_css_online(ರಚನೆ cgroup_subsys_state *css)
{
	ರಚನೆ task_ಗುಂಪು *tg = css_tg(css);
	ರಚನೆ task_ಗುಂಪು *ಪೋಷಕ = css_tg(css->ಪೋಷಕ);
	ಪೂರ್ಣಾಂಕ ret;

	ret = scx_tg_online(tg);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;

	ಆದರೆ (ಪೋಷಕ)
		sched_online_ಗುಂಪು(tg, ಪೋಷಕ);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK_ಗುಂಪು
	/* Propagate the effective uclamp value for the new group */
	guard(mutex)(&uclamp_mutex);
	guard(rcu)();
	cpu_util_update_eff(css);
#ಆದರೆ_ಅಂತ್ಯ

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ cpu_cgroup_css_offline(ರಚನೆ cgroup_subsys_state *css)
{
	ರಚನೆ task_ಗುಂಪು *tg = css_tg(css);

	scx_tg_offline(tg);
}

ಸ್ಥಿರ ಶೂನ್ಯ cpu_cgroup_css_released(ರಚನೆ cgroup_subsys_state *css)
{
	ರಚನೆ task_ಗುಂಪು *tg = css_tg(css);

	sched_release_ಗುಂಪು(tg);
}

ಸ್ಥಿರ ಶೂನ್ಯ cpu_cgroup_css_free(ರಚನೆ cgroup_subsys_state *css)
{
	ರಚನೆ task_ಗುಂಪು *tg = css_tg(css);

	/*
	 * Relies on the RCU grace period between css_released() and this.
	 */
	sched_unregister_ಗುಂಪು(tg);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpu_cgroup_can_attach(ರಚನೆ cgroup_taskset *tset)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED
	ರಚನೆ task_struct *task;
	ರಚನೆ cgroup_subsys_state *css;

	ಆದರೆ (!rt_ಗುಂಪು_sched_enabled())
		ಹೋಗು scx_check;

	cgroup_taskset_for_each(task, css, tset) {
		ಆದರೆ (!sched_rt_can_attach(css_tg(css), task))
			ಹಿಂತಿರುಗಿ -EINVAL;
	}
scx_check:
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_RT_GROUP_SCHED */
	ಹಿಂತಿರುಗಿ scx_cgroup_can_attach(tset);
}

ಸ್ಥಿರ ಶೂನ್ಯ cpu_cgroup_attach(ರಚನೆ cgroup_taskset *tset)
{
	ರಚನೆ task_struct *task;
	ರಚನೆ cgroup_subsys_state *css;

	cgroup_taskset_for_each(task, css, tset)
		sched_move_task(task, false);
}

ಸ್ಥಿರ ಶೂನ್ಯ cpu_cgroup_cancel_attach(ರಚನೆ cgroup_taskset *tset)
{
	scx_cgroup_cancel_attach(tset);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK_ಗುಂಪು
ಸ್ಥಿರ ಶೂನ್ಯ cpu_util_update_eff(ರಚನೆ cgroup_subsys_state *css)
{
	ರಚನೆ cgroup_subsys_state *ತುದಿ_css = css;
	ರಚನೆ uclamp_se *uc_ಪೋಷಕ = ಖಾಲಿ;
	ರಚನೆ uclamp_se *uc_se = ಖಾಲಿ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ eff[UCLAMP_CNT];
	ಎಣಿಕೆ uclamp_id clamp_id;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ clamps;

	lockdep_assert_held(&uclamp_mutex);
	WARN_ON_ONCE(!rcu_read_ಬೀಗ_held());

	css_for_each_descendant_pre(css, ತುದಿ_css) {
		uc_ಪೋಷಕ = css_tg(css)->ಪೋಷಕ
			? css_tg(css)->ಪೋಷಕ->uclamp : ಖಾಲಿ;

		for_each_clamp_id(clamp_id) {
			/* Assume effective clamps matches requested clamps */
			eff[clamp_id] = css_tg(css)->uclamp_req[clamp_id].ಮೌಲ್ಯ;
			/* Cap effective clamps with parent's effective clamps */
			ಆದರೆ (uc_ಪೋಷಕ &&
			    eff[clamp_id] > uc_ಪೋಷಕ[clamp_id].ಮೌಲ್ಯ) {
				eff[clamp_id] = uc_ಪೋಷಕ[clamp_id].ಮೌಲ್ಯ;
			}
		}
		/* Ensure protection is always capped by limit */
		eff[UCLAMP_MIN] = min(eff[UCLAMP_MIN], eff[UCLAMP_MAX]);

		/* Propagate most restrictive effective clamps */
		clamps = 0x0;
		uc_se = css_tg(css)->uclamp;
		for_each_clamp_id(clamp_id) {
			ಆದರೆ (eff[clamp_id] == uc_se[clamp_id].ಮೌಲ್ಯ)
				ಮುಂದುವರಿ;
			uc_se[clamp_id].ಮೌಲ್ಯ = eff[clamp_id];
			uc_se[clamp_id].ಪಾತ್ರೆ_id = uclamp_ಪಾತ್ರೆ_id(eff[clamp_id]);
			clamps |= (0x1 << clamp_id);
		}
		ಆದರೆ (!clamps) {
			css = css_rightmost_descendant(css);
			ಮುಂದುವರಿ;
		}

		/* Immediately update descendants RUNNABLE tasks */
		uclamp_update_ಸಕ್ರಿಯ_tasks(css);
	}
}

/*
 * Integer 10^N with a given N exponent by casting to integer the literal "1eN"
 * C expression. Since there is no way to convert a macro argument (N) into a
 * character constant, use two levels of macros.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು _POW10(ಘಾತಾಂಕ) ((ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)1e##exp)
#ವ್ಯಾಖ್ಯಾನಿಸು POW10(ಘಾತಾಂಕ) _POW10(ಘಾತಾಂಕ)

ರಚನೆ uclamp_request {
#ವ್ಯಾಖ್ಯಾನಿಸು UCLAMP_PERCENT_ಸರಿಸು	2
#ವ್ಯಾಖ್ಯಾನಿಸು UCLAMP_PERCENT_SCALE	(100 * POW10(UCLAMP_PERCENT_ಸರಿಸು))
	s64 percent;
	u64 util;
	ಪೂರ್ಣಾಂಕ ret;
};

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ uclamp_request
ಸಾಮರ್ಥ್ಯ_from_percent(ಅಕ್ಷರ *buf)
{
	ರಚನೆ uclamp_request req = {
		.percent = UCLAMP_PERCENT_SCALE,
		.util = SCHED_ಸಾಮರ್ಥ್ಯ_SCALE,
		.ret = 0,
	};

	buf = strim(buf);
	ಆದರೆ (strcmp(buf, "max")) {
		req.ret = cgroup_parse_float(buf, UCLAMP_PERCENT_ಸರಿಸು,
					     &req.percent);
		ಆದರೆ (req.ret)
			ಹಿಂತಿರುಗಿ req;
		ಆದರೆ ((u64)req.percent > UCLAMP_PERCENT_SCALE) {
			req.ret = -ERANGE;
			ಹಿಂತಿರುಗಿ req;
		}

		req.util = req.percent << SCHED_ಸಾಮರ್ಥ್ಯ_ಸರಿಸು;
		req.util = DIV_ಸುತ್ತು_CLOSEST_ULL(req.util, UCLAMP_PERCENT_SCALE);
	}

	ಹಿಂತಿರುಗಿ req;
}

ಸ್ಥಿರ ssize_t cpu_uclamp_write(ರಚನೆ kernfs_open_file *ದ, ಅಕ್ಷರ *buf,
				ಗಾತ್ರ_t nbytes, loff_t off,
				ಎಣಿಕೆ uclamp_id clamp_id)
{
	ರಚನೆ uclamp_request req;
	ರಚನೆ task_ಗುಂಪು *tg;

	req = ಸಾಮರ್ಥ್ಯ_from_percent(buf);
	ಆದರೆ (req.ret)
		ಹಿಂತಿರುಗಿ req.ret;

	sched_uclamp_enable();

	guard(mutex)(&uclamp_mutex);
	guard(rcu)();

	tg = css_tg(ದ_css(ದ));
	ಆದರೆ (tg->uclamp_req[clamp_id].ಮೌಲ್ಯ != req.util)
		uclamp_se_ಗಣ(&tg->uclamp_req[clamp_id], req.util, false);

	/*
	 * Because of not recoverable conversion rounding we keep track of the
	 * exact requested value
	 */
	tg->uclamp_pct[clamp_id] = req.percent;

	/* Update effective clamps to track the most restrictive value */
	cpu_util_update_eff(ದ_css(ದ));

	ಹಿಂತಿರುಗಿ nbytes;
}

ಸ್ಥಿರ ssize_t cpu_uclamp_min_write(ರಚನೆ kernfs_open_file *ದ,
				    ಅಕ್ಷರ *buf, ಗಾತ್ರ_t nbytes,
				    loff_t off)
{
	ಹಿಂತಿರುಗಿ cpu_uclamp_write(ದ, buf, nbytes, off, UCLAMP_MIN);
}

ಸ್ಥಿರ ssize_t cpu_uclamp_max_write(ರಚನೆ kernfs_open_file *ದ,
				    ಅಕ್ಷರ *buf, ಗಾತ್ರ_t nbytes,
				    loff_t off)
{
	ಹಿಂತಿರುಗಿ cpu_uclamp_write(ದ, buf, nbytes, off, UCLAMP_MAX);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ cpu_uclamp_print(ರಚನೆ seq_file *sf,
				    ಎಣಿಕೆ uclamp_id clamp_id)
{
	ರಚನೆ task_ಗುಂಪು *tg;
	u64 util_clamp;
	u64 percent;
	u32 rem;

	scoped_guard (rcu) {
		tg = css_tg(seq_css(sf));
		util_clamp = tg->uclamp_req[clamp_id].ಮೌಲ್ಯ;
	}

	ಆದರೆ (util_clamp == SCHED_ಸಾಮರ್ಥ್ಯ_SCALE) {
		seq_puts(sf, "max\n");
		ಹಿಂತಿರುಗಿ;
	}

	percent = tg->uclamp_pct[clamp_id];
	percent = div_u64_rem(percent, POW10(UCLAMP_PERCENT_ಸರಿಸು), &rem);
	seq_printf(sf, "%llu.%0*u\n", percent, UCLAMP_PERCENT_ಸರಿಸು, rem);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpu_uclamp_min_show(ರಚನೆ seq_file *sf, ಶೂನ್ಯ *v)
{
	cpu_uclamp_print(sf, UCLAMP_MIN);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpu_uclamp_max_show(ರಚನೆ seq_file *sf, ಶೂನ್ಯ *v)
{
	cpu_uclamp_print(sf, UCLAMP_MAX);
	ಹಿಂತಿರುಗಿ 0;
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_UCLAMP_TASK_GROUP */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_ಗುಂಪು_SCHED_ತೂಕ
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ tg_ತೂಕ(ರಚನೆ task_ಗುಂಪು *tg)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
	ಹಿಂತಿರುಗಿ scale_ತುಂಬು_ಕೆಳಕ್ಕೆ(tg->shares);
#ಇಲ್ಲದಿದ್ದರೆ
	ಹಿಂತಿರುಗಿ sched_ತೂಕ_from_cgroup(tg->scx.ತೂಕ);
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpu_shares_write_u64(ರಚನೆ cgroup_subsys_state *css,
				ರಚನೆ cftype *cftype, u64 shareval)
{
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (shareval > scale_ತುಂಬು_ಕೆಳಕ್ಕೆ(ULONG_MAX))
		shareval = MAX_SHARES;
	ret = sched_ಗುಂಪು_ಗಣ_shares(css_tg(css), scale_ತುಂಬು(shareval));
	ಆದರೆ (!ret)
		scx_ಗುಂಪು_ಗಣ_ತೂಕ(css_tg(css),
				     sched_ತೂಕ_ಗಮ್ಯ_cgroup(shareval));
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ u64 cpu_shares_read_u64(ರಚನೆ cgroup_subsys_state *css,
			       ರಚನೆ cftype *cft)
{
	ಹಿಂತಿರುಗಿ tg_ತೂಕ(css_tg(css));
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_GROUP_SCHED_WEIGHT */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CFS_BANDWIDTH
ಸ್ಥಿರ DEFINE_MUTEX(cfs_constraints_mutex);

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __cfs_schedulable(ರಚನೆ task_ಗುಂಪು *tg, u64 period, u64 runtime);

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tg_ಗಣ_cfs_bandwidth(ರಚನೆ task_ಗುಂಪು *tg,
				u64 period_us, u64 quota_us, u64 ಅವಧಿ_us)
{
	ಪೂರ್ಣಾಂಕ i, ret = 0, runtime_enabled, runtime_was_enabled;
	ರಚನೆ cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
	u64 period, quota, ಅವಧಿ;

	period = (u64)period_us * NSEC_ಪ್ರತಿ_USEC;

	ಆದರೆ (quota_us == RUNTIME_INF)
		quota = RUNTIME_INF;
	ಇಲ್ಲದಿದ್ದರೆ
		quota = (u64)quota_us * NSEC_ಪ್ರತಿ_USEC;

	ಅವಧಿ = (u64)ಅವಧಿ_us * NSEC_ಪ್ರತಿ_USEC;

	/*
	 * Prevent race between setting of cfs_rq->runtime_enabled and
	 * unthrottle_offline_cfs_rqs().
	 */
	guard(cpus_read_ಬೀಗ)();
	guard(mutex)(&cfs_constraints_mutex);

	ret = __cfs_schedulable(tg, period, quota);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;

	runtime_enabled = quota != RUNTIME_INF;
	runtime_was_enabled = cfs_b->quota != RUNTIME_INF;
	/*
	 * If we need to toggle cfs_bandwidth_used, off->on must occur
	 * before making related changes, and on->off must occur afterwards
	 */
	ಆದರೆ (runtime_enabled && !runtime_was_enabled)
		cfs_bandwidth_usage_inc();

	scoped_guard (raw_spinlock_irq, &cfs_b->ಬೀಗ) {
		cfs_b->period = ns_ಗಮ್ಯ_ktime(period);
		cfs_b->quota = quota;
		cfs_b->ಅವಧಿ = ಅವಧಿ;

		__refill_cfs_bandwidth_runtime(cfs_b);

		/*
		 * Restart the period timer (if active) to handle new
		 * period expiry:
		 */
		ಆದರೆ (runtime_enabled)
			ಆರಂಭ_cfs_bandwidth(cfs_b);
	}

	for_each_online_cpu(i) {
		ರಚನೆ cfs_rq *cfs_rq = tg_cfs_rq(tg, i);
		ರಚನೆ rq *rq = cfs_rq->rq;

		guard(rq_ಬೀಗ_irq)(rq);

		cfs_rq->runtime_enabled = runtime_enabled;
		cfs_rq->runtime_ಉಳಿದ = 1;

		ಆದರೆ (cfs_rq->throttled) {
			update_rq_clock(rq);
			unthrottle_cfs_rq(cfs_rq);
		}
	}

	ಆದರೆ (runtime_was_enabled && !runtime_enabled)
		cfs_bandwidth_usage_dec();

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ u64 tg_get_cfs_period(ರಚನೆ task_ಗುಂಪು *tg)
{
	u64 cfs_period_us;

	cfs_period_us = ktime_ಗಮ್ಯ_ns(tg->cfs_bandwidth.period);
	do_div(cfs_period_us, NSEC_ಪ್ರತಿ_USEC);

	ಹಿಂತಿರುಗಿ cfs_period_us;
}

ಸ್ಥಿರ u64 tg_get_cfs_quota(ರಚನೆ task_ಗುಂಪು *tg)
{
	u64 quota_us;

	ಆದರೆ (tg->cfs_bandwidth.quota == RUNTIME_INF)
		ಹಿಂತಿರುಗಿ RUNTIME_INF;

	quota_us = tg->cfs_bandwidth.quota;
	do_div(quota_us, NSEC_ಪ್ರತಿ_USEC);

	ಹಿಂತಿರುಗಿ quota_us;
}

ಸ್ಥಿರ u64 tg_get_cfs_ಅವಧಿ(ರಚನೆ task_ಗುಂಪು *tg)
{
	u64 ಅವಧಿ_us;

	ಅವಧಿ_us = tg->cfs_bandwidth.ಅವಧಿ;
	do_div(ಅವಧಿ_us, NSEC_ಪ್ರತಿ_USEC);

	ಹಿಂತಿರುಗಿ ಅವಧಿ_us;
}

ರಚನೆ cfs_schedulable_ದತ್ತಾಂಶ {
	ರಚನೆ task_ಗುಂಪು *tg;
	u64 period, quota;
};

/*
 * normalize group quota/period to be quota/max_period
 * note: units are usecs
 */
ಸ್ಥಿರ u64 normalize_cfs_quota(ರಚನೆ task_ಗುಂಪು *tg,
			       ರಚನೆ cfs_schedulable_ದತ್ತಾಂಶ *d)
{
	u64 quota, period;

	ಆದರೆ (tg == d->tg) {
		period = d->period;
		quota = d->quota;
	} ಇಲ್ಲದಿದ್ದರೆ {
		period = tg_get_cfs_period(tg);
		quota = tg_get_cfs_quota(tg);
	}

	/* note: these should typically be equivalent */
	ಆದರೆ (quota == RUNTIME_INF || quota == -1)
		ಹಿಂತಿರುಗಿ RUNTIME_INF;

	ಹಿಂತಿರುಗಿ ಗಮ್ಯ_ಅನುಪಾತ(period, quota);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tg_cfs_schedulable_ಕೆಳಕ್ಕೆ(ರಚನೆ task_ಗುಂಪು *tg, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ cfs_schedulable_ದತ್ತಾಂಶ *d = ದತ್ತಾಂಶ;
	ರಚನೆ cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
	s64 quota = 0, ಪೋಷಕ_quota = -1;

	ಆದರೆ (!tg->ಪೋಷಕ) {
		quota = RUNTIME_INF;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ cfs_bandwidth *ಪೋಷಕ_b = &tg->ಪೋಷಕ->cfs_bandwidth;

		quota = normalize_cfs_quota(tg, d);
		ಪೋಷಕ_quota = ಪೋಷಕ_b->ಶ್ರೇಣೀಕೃತ_quota;

		/*
		 * Ensure max(child_quota) <= parent_quota.  On cgroup2,
		 * always take the non-RUNTIME_INF min.  On cgroup1, only
		 * inherit when no limit is set. In both cases this is used
		 * by the scheduler to determine if a given CFS task has a
		 * bandwidth constraint at some higher level.
		 */
		ಆದರೆ (cgroup_subsys_on_dfl(cpu_cgrp_subsys)) {
			ಆದರೆ (quota == RUNTIME_INF)
				quota = ಪೋಷಕ_quota;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಪೋಷಕ_quota != RUNTIME_INF)
				quota = min(quota, ಪೋಷಕ_quota);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಆದರೆ (quota == RUNTIME_INF)
				quota = ಪೋಷಕ_quota;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಪೋಷಕ_quota != RUNTIME_INF && quota > ಪೋಷಕ_quota)
				ಹಿಂತಿರುಗಿ -EINVAL;
		}
	}
	cfs_b->ಶ್ರೇಣೀಕೃತ_quota = quota;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __cfs_schedulable(ರಚನೆ task_ಗುಂಪು *tg, u64 period, u64 quota)
{
	ರಚನೆ cfs_schedulable_ದತ್ತಾಂಶ ದತ್ತಾಂಶ = {
		.tg = tg,
		.period = period,
		.quota = quota,
	};

	ಆದರೆ (quota != RUNTIME_INF) {
		do_div(ದತ್ತಾಂಶ.period, NSEC_ಪ್ರತಿ_USEC);
		do_div(ದತ್ತಾಂಶ.quota, NSEC_ಪ್ರತಿ_USEC);
	}

	guard(rcu)();
	ಹಿಂತಿರುಗಿ walk_tg_ವೃಕ್ಷ(tg_cfs_schedulable_ಕೆಳಕ್ಕೆ, tg_nop, &ದತ್ತಾಂಶ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpu_cfs_stat_show(ರಚನೆ seq_file *sf, ಶೂನ್ಯ *v)
{
	ರಚನೆ task_ಗುಂಪು *tg = css_tg(seq_css(sf));
	ರಚನೆ cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;

	seq_printf(sf, "nr_periods %d\n", cfs_b->nr_periods);
	seq_printf(sf, "nr_throttled %d\n", cfs_b->nr_throttled);
	seq_printf(sf, "throttled_time %llu\n", cfs_b->throttled_ಸಮಯ);

	ಆದರೆ (schedstat_enabled() && tg != &ಬೇರು_task_ಗುಂಪು) {
		ರಚನೆ sched_statistics *stats;
		u64 ws = 0;
		ಪೂರ್ಣಾಂಕ i;

		for_each_possible_cpu(i) {
			stats = __schedstats_from_se(tg_se(tg, i));
			ws += schedstat_val(stats->wait_ಮೊತ್ತ);
		}

		seq_printf(sf, "wait_sum %llu\n", ws);
	}

	seq_printf(sf, "nr_bursts %d\n", cfs_b->nr_ಅವಧಿ);
	seq_printf(sf, "burst_time %llu\n", cfs_b->ಅವಧಿ_ಸಮಯ);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ u64 throttled_ಸಮಯ_self(ರಚನೆ task_ಗುಂಪು *tg)
{
	ಪೂರ್ಣಾಂಕ i;
	u64 ಒಟ್ಟು = 0;

	for_each_possible_cpu(i) {
		ಒಟ್ಟು += READ_ONCE(tg_cfs_rq(tg, i)->throttled_clock_self_ಸಮಯ);
	}

	ಹಿಂತಿರುಗಿ ಒಟ್ಟು;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpu_cfs_local_stat_show(ರಚನೆ seq_file *sf, ಶೂನ್ಯ *v)
{
	ರಚನೆ task_ಗುಂಪು *tg = css_tg(seq_css(sf));

	seq_printf(sf, "throttled_time %llu\n", throttled_ಸಮಯ_self(tg));

	ಹಿಂತಿರುಗಿ 0;
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_CFS_BANDWIDTH */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_ಗುಂಪು_SCHED_BANDWIDTH
ಸ್ಥಿರಾಂಕ u64 max_bw_quota_period_us = 1 * USEC_ಪ್ರತಿ_SEC; /* 1s */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ u64 min_bw_quota_period_us = 1 * USEC_ಪ್ರತಿ_MSEC; /* 1ms */
/* More than 203 days if BW_SHIFT equals 20. */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ u64 max_bw_runtime_us = MAX_BW;

ಸ್ಥಿರ ಶೂನ್ಯ tg_bandwidth(ರಚನೆ task_ಗುಂಪು *tg,
			 u64 *period_us_p, u64 *quota_us_p, u64 *ಅವಧಿ_us_p)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CFS_BANDWIDTH
	ಆದರೆ (period_us_p)
		*period_us_p = tg_get_cfs_period(tg);
	ಆದರೆ (quota_us_p)
		*quota_us_p = tg_get_cfs_quota(tg);
	ಆದರೆ (ಅವಧಿ_us_p)
		*ಅವಧಿ_us_p = tg_get_cfs_ಅವಧಿ(tg);
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_CFS_BANDWIDTH */
	ಆದರೆ (period_us_p)
		*period_us_p = tg->scx.bw_period_us;
	ಆದರೆ (quota_us_p)
		*quota_us_p = tg->scx.bw_quota_us;
	ಆದರೆ (ಅವಧಿ_us_p)
		*ಅವಧಿ_us_p = tg->scx.bw_ಅವಧಿ_us;
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_CFS_BANDWIDTH */
}

ಸ್ಥಿರ u64 cpu_period_read_u64(ರಚನೆ cgroup_subsys_state *css,
			       ರಚನೆ cftype *cft)
{
	u64 period_us;

	tg_bandwidth(css_tg(css), &period_us, ಖಾಲಿ, ಖಾಲಿ);
	ಹಿಂತಿರುಗಿ period_us;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tg_ಗಣ_bandwidth(ರಚನೆ task_ಗುಂಪು *tg,
			    u64 period_us, u64 quota_us, u64 ಅವಧಿ_us)
{
	ಸ್ಥಿರಾಂಕ u64 max_usec = U64_MAX / NSEC_ಪ್ರತಿ_USEC;
	ಪೂರ್ಣಾಂಕ ret = 0;

	ಆದರೆ (tg == &ಬೇರು_task_ಗುಂಪು)
		ಹಿಂತಿರುಗಿ -EINVAL;

	/* Values should survive translation to nsec */
	ಆದರೆ (period_us > max_usec ||
	    (quota_us != RUNTIME_INF && quota_us > max_usec) ||
	    ಅವಧಿ_us > max_usec)
		ಹಿಂತಿರುಗಿ -EINVAL;

	/*
	 * Ensure we have some amount of bandwidth every period. This is to
	 * prevent reaching a state of large arrears when throttled via
	 * entity_tick() resulting in prolonged exit starvation.
	 */
	ಆದರೆ (quota_us < min_bw_quota_period_us ||
	    period_us < min_bw_quota_period_us)
		ಹಿಂತಿರುಗಿ -EINVAL;

	/*
	 * Likewise, bound things on the other side by preventing insane quota
	 * periods.  This also allows us to normalize in computing quota
	 * feasibility.
	 */
	ಆದರೆ (period_us > max_bw_quota_period_us)
		ಹಿಂತಿರುಗಿ -EINVAL;

	/*
	 * Bound quota to defend quota against overflow during bandwidth shift.
	 */
	ಆದರೆ (quota_us != RUNTIME_INF && quota_us > max_bw_runtime_us)
		ಹಿಂತಿರುಗಿ -EINVAL;

	ಆದರೆ (quota_us != RUNTIME_INF && (ಅವಧಿ_us > quota_us ||
					ಅವಧಿ_us + quota_us > max_bw_runtime_us))
		ಹಿಂತಿರುಗಿ -EINVAL;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CFS_BANDWIDTH
	ret = tg_ಗಣ_cfs_bandwidth(tg, period_us, quota_us, ಅವಧಿ_us);
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_CFS_BANDWIDTH */
	ಆದರೆ (!ret)
		scx_ಗುಂಪು_ಗಣ_bandwidth(tg, period_us, quota_us, ಅವಧಿ_us);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ s64 cpu_quota_read_s64(ರಚನೆ cgroup_subsys_state *css,
			      ರಚನೆ cftype *cft)
{
	u64 quota_us;

	tg_bandwidth(css_tg(css), ಖಾಲಿ, &quota_us, ಖಾಲಿ);
	ಹಿಂತಿರುಗಿ quota_us;	/* (s64)RUNTIME_INF becomes -1 */
}

ಸ್ಥಿರ u64 cpu_ಅವಧಿ_read_u64(ರಚನೆ cgroup_subsys_state *css,
			      ರಚನೆ cftype *cft)
{
	u64 ಅವಧಿ_us;

	tg_bandwidth(css_tg(css), ಖಾಲಿ, ಖಾಲಿ, &ಅವಧಿ_us);
	ಹಿಂತಿರುಗಿ ಅವಧಿ_us;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpu_period_write_u64(ರಚನೆ cgroup_subsys_state *css,
				ರಚನೆ cftype *cftype, u64 period_us)
{
	ರಚನೆ task_ಗುಂಪು *tg = css_tg(css);
	u64 quota_us, ಅವಧಿ_us;

	tg_bandwidth(tg, ಖಾಲಿ, &quota_us, &ಅವಧಿ_us);
	ಹಿಂತಿರುಗಿ tg_ಗಣ_bandwidth(tg, period_us, quota_us, ಅವಧಿ_us);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpu_quota_write_s64(ರಚನೆ cgroup_subsys_state *css,
			       ರಚನೆ cftype *cftype, s64 quota_us)
{
	ರಚನೆ task_ಗುಂಪು *tg = css_tg(css);
	u64 period_us, ಅವಧಿ_us;

	ಆದರೆ (quota_us < 0)
		quota_us = RUNTIME_INF;

	tg_bandwidth(tg, &period_us, ಖಾಲಿ, &ಅವಧಿ_us);
	ಹಿಂತಿರುಗಿ tg_ಗಣ_bandwidth(tg, period_us, quota_us, ಅವಧಿ_us);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpu_ಅವಧಿ_write_u64(ರಚನೆ cgroup_subsys_state *css,
			       ರಚನೆ cftype *cftype, u64 ಅವಧಿ_us)
{
	ರಚನೆ task_ಗುಂಪು *tg = css_tg(css);
	u64 period_us, quota_us;

	tg_bandwidth(tg, &period_us, &quota_us, ಖಾಲಿ);
	ಹಿಂತಿರುಗಿ tg_ಗಣ_bandwidth(tg, period_us, quota_us, ಅವಧಿ_us);
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_GROUP_SCHED_BANDWIDTH */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpu_rt_runtime_write(ರಚನೆ cgroup_subsys_state *css,
				ರಚನೆ cftype *cft, s64 val)
{
	ಹಿಂತಿರುಗಿ sched_ಗುಂಪು_ಗಣ_rt_runtime(css_tg(css), val);
}

ಸ್ಥಿರ s64 cpu_rt_runtime_read(ರಚನೆ cgroup_subsys_state *css,
			       ರಚನೆ cftype *cft)
{
	ಹಿಂತಿರುಗಿ sched_ಗುಂಪು_rt_runtime(css_tg(css));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpu_rt_period_write_uint(ರಚನೆ cgroup_subsys_state *css,
				    ರಚನೆ cftype *cftype, u64 rt_period_us)
{
	ಹಿಂತಿರುಗಿ sched_ಗುಂಪು_ಗಣ_rt_period(css_tg(css), rt_period_us);
}

ಸ್ಥಿರ u64 cpu_rt_period_read_uint(ರಚನೆ cgroup_subsys_state *css,
				   ರಚನೆ cftype *cft)
{
	ಹಿಂತಿರುಗಿ sched_ಗುಂಪು_rt_period(css_tg(css));
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_RT_GROUP_SCHED */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_ಗುಂಪು_SCHED_ತೂಕ
ಸ್ಥಿರ s64 cpu_idle_read_s64(ರಚನೆ cgroup_subsys_state *css,
			       ರಚನೆ cftype *cft)
{
	ಹಿಂತಿರುಗಿ css_tg(css)->idle;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpu_idle_write_s64(ರಚನೆ cgroup_subsys_state *css,
				ರಚನೆ cftype *cft, s64 idle)
{
	ಪೂರ್ಣಾಂಕ ret;

	ret = sched_ಗುಂಪು_ಗಣ_idle(css_tg(css), idle);
	ಆದರೆ (!ret)
		scx_ಗುಂಪು_ಗಣ_idle(css_tg(css), idle);
	ಹಿಂತಿರುಗಿ ret;
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_GROUP_SCHED_WEIGHT */

ಸ್ಥಿರ ರಚನೆ cftype cpu_legacy_files[] = {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_ಗುಂಪು_SCHED_ತೂಕ
	{
		.name = "shares",
		.read_u64 = cpu_shares_read_u64,
		.write_u64 = cpu_shares_write_u64,
	},
	{
		.name = "idle",
		.read_s64 = cpu_idle_read_s64,
		.write_s64 = cpu_idle_write_s64,
	},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_ಗುಂಪು_SCHED_BANDWIDTH
	{
		.name = "cfs_period_us",
		.read_u64 = cpu_period_read_u64,
		.write_u64 = cpu_period_write_u64,
	},
	{
		.name = "cfs_quota_us",
		.read_s64 = cpu_quota_read_s64,
		.write_s64 = cpu_quota_write_s64,
	},
	{
		.name = "cfs_burst_us",
		.read_u64 = cpu_ಅವಧಿ_read_u64,
		.write_u64 = cpu_ಅವಧಿ_write_u64,
	},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CFS_BANDWIDTH
	{
		.name = "stat",
		.seq_show = cpu_cfs_stat_show,
	},
	{
		.name = "stat.local",
		.seq_show = cpu_cfs_local_stat_show,
	},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK_ಗುಂಪು
	{
		.name = "uclamp.min",
		.flags = CFTYPE_NOT_ON_ಬೇರು,
		.seq_show = cpu_uclamp_min_show,
		.ಲಿಖ = cpu_uclamp_min_write,
	},
	{
		.name = "uclamp.max",
		.flags = CFTYPE_NOT_ON_ಬೇರು,
		.seq_show = cpu_uclamp_max_show,
		.ಲಿಖ = cpu_uclamp_max_write,
	},
#ಆದರೆ_ಅಂತ್ಯ
	{ }	/* Terminate */
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED
ಸ್ಥಿರ ರಚನೆ cftype rt_ಗುಂಪು_files[] = {
	{
		.name = "rt_runtime_us",
		.read_s64 = cpu_rt_runtime_read,
		.write_s64 = cpu_rt_runtime_write,
	},
	{
		.name = "rt_period_us",
		.read_u64 = cpu_rt_period_read_uint,
		.write_u64 = cpu_rt_period_write_uint,
	},
	{ }	/* Terminate */
};

# ifdef CONFIG_RT_ಗುಂಪು_SCHED_DEFAULT_DISABLED
DEFINE_STATIC_ಕೀಲಿ_FALSE(rt_ಗುಂಪು_sched);
# ಇಲ್ಲದಿದ್ದರೆ
DEFINE_STATIC_ಕೀಲಿ_TRUE(rt_ಗುಂಪು_sched);
# endif

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init ಸಿದ್ಧತೆ_rt_ಗುಂಪು_sched(ಅಕ್ಷರ *str)
{
	ದೀರ್ಘ val;

	ಆದರೆ (kstrtol(str, 0, &val) || val < 0 || val > 1) {
		pr_warn("Unable to set rt_group_sched\n");
		ಹಿಂತಿರುಗಿ 1;
	}
	ಆದರೆ (val)
		static_branch_enable(&rt_ಗುಂಪು_sched);
	ಇಲ್ಲದಿದ್ದರೆ
		static_branch_disable(&rt_ಗುಂಪು_sched);

	ಹಿಂತಿರುಗಿ 1;
}
__setup("rt_group_sched=", ಸಿದ್ಧತೆ_rt_ಗುಂಪು_sched);

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init cpu_rt_ಗುಂಪು_init(ಶೂನ್ಯ)
{
	ಆದರೆ (!rt_ಗುಂಪು_sched_enabled())
		ಹಿಂತಿರುಗಿ 0;

	WARN_ON(cgroup_ಕೂಡಿಸು_legacy_cftypes(&cpu_cgrp_subsys, rt_ಗುಂಪು_files));
	ಹಿಂತಿರುಗಿ 0;
}
subsys_initcall(cpu_rt_ಗುಂಪು_init);
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_RT_GROUP_SCHED */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpu_extra_stat_show(ರಚನೆ seq_file *sf,
			       ರಚನೆ cgroup_subsys_state *css)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CFS_BANDWIDTH
	{
		ರಚನೆ task_ಗುಂಪು *tg = css_tg(css);
		ರಚನೆ cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
		u64 throttled_usec, ಅವಧಿ_usec;

		throttled_usec = cfs_b->throttled_ಸಮಯ;
		do_div(throttled_usec, NSEC_ಪ್ರತಿ_USEC);
		ಅವಧಿ_usec = cfs_b->ಅವಧಿ_ಸಮಯ;
		do_div(ಅವಧಿ_usec, NSEC_ಪ್ರತಿ_USEC);

		seq_printf(sf, "nr_periods %d\n"
			   "nr_throttled %d\n"
			   "throttled_usec %llu\n"
			   "nr_bursts %d\n"
			   "burst_usec %llu\n",
			   cfs_b->nr_periods, cfs_b->nr_throttled,
			   throttled_usec, cfs_b->nr_ಅವಧಿ, ಅವಧಿ_usec);
	}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_CFS_BANDWIDTH */
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpu_local_stat_show(ರಚನೆ seq_file *sf,
			       ರಚನೆ cgroup_subsys_state *css)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CFS_BANDWIDTH
	{
		ರಚನೆ task_ಗುಂಪು *tg = css_tg(css);
		u64 throttled_self_usec;

		throttled_self_usec = throttled_ಸಮಯ_self(tg);
		do_div(throttled_self_usec, NSEC_ಪ್ರತಿ_USEC);

		seq_printf(sf, "throttled_usec %llu\n",
			   throttled_self_usec);
	}
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_ಗುಂಪು_SCHED_ತೂಕ

ಸ್ಥಿರ u64 cpu_ತೂಕ_read_u64(ರಚನೆ cgroup_subsys_state *css,
			       ರಚನೆ cftype *cft)
{
	ಹಿಂತಿರುಗಿ sched_ತೂಕ_ಗಮ್ಯ_cgroup(tg_ತೂಕ(css_tg(css)));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpu_ತೂಕ_write_u64(ರಚನೆ cgroup_subsys_state *css,
				ರಚನೆ cftype *cft, u64 cgrp_ತೂಕ)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತೂಕ;
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (cgrp_ತೂಕ < CGROUP_ತೂಕ_MIN || cgrp_ತೂಕ > CGROUP_ತೂಕ_MAX)
		ಹಿಂತಿರುಗಿ -ERANGE;

	ತೂಕ = sched_ತೂಕ_from_cgroup(cgrp_ತೂಕ);

	ret = sched_ಗುಂಪು_ಗಣ_shares(css_tg(css), scale_ತುಂಬು(ತೂಕ));
	ಆದರೆ (!ret)
		scx_ಗುಂಪು_ಗಣ_ತೂಕ(css_tg(css), cgrp_ತೂಕ);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ s64 cpu_ತೂಕ_nice_read_s64(ರಚನೆ cgroup_subsys_state *css,
				    ರಚನೆ cftype *cft)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತೂಕ = tg_ತೂಕ(css_tg(css));
	ಪೂರ್ಣಾಂಕ ಕೊನೆಯ_delta = ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;
	ಪೂರ್ಣಾಂಕ prio, delta;

	/* find the closest nice value to the current weight */
	ಗಾಗಿ (prio = 0; prio < ARRAY_ಗಾತ್ರ(sched_prio_ಗಮ್ಯ_ತೂಕ); prio++) {
		delta = ನಿರಪೇಕ್ಷ(sched_prio_ಗಮ್ಯ_ತೂಕ[prio] - ತೂಕ);
		ಆದರೆ (delta >= ಕೊನೆಯ_delta)
			ನಿಲ್ಲಿಸು;
		ಕೊನೆಯ_delta = delta;
	}

	ಹಿಂತಿರುಗಿ PRIO_ಗಮ್ಯ_NICE(prio - 1 + MAX_RT_PRIO);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpu_ತೂಕ_nice_write_s64(ರಚನೆ cgroup_subsys_state *css,
				     ರಚನೆ cftype *cft, s64 nice)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತೂಕ;
	ಪೂರ್ಣಾಂಕ idx, ret;

	ಆದರೆ (nice < MIN_NICE || nice > MAX_NICE)
		ಹಿಂತಿರುಗಿ -ERANGE;

	idx = NICE_ಗಮ್ಯ_PRIO(nice) - MAX_RT_PRIO;
	idx = array_ಸೂಚ್ಯಂಕ_nospec(idx, 40);
	ತೂಕ = sched_prio_ಗಮ್ಯ_ತೂಕ[idx];

	ret = sched_ಗುಂಪು_ಗಣ_shares(css_tg(css), scale_ತುಂಬು(ತೂಕ));
	ಆದರೆ (!ret)
		scx_ಗುಂಪು_ಗಣ_ತೂಕ(css_tg(css),
				     sched_ತೂಕ_ಗಮ್ಯ_cgroup(ತೂಕ));
	ಹಿಂತಿರುಗಿ ret;
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_GROUP_SCHED_WEIGHT */

ಸ್ಥಿರ ಶೂನ್ಯ __maybe_unused cpu_period_quota_print(ರಚನೆ seq_file *sf,
						  ದೀರ್ಘ period, ದೀರ್ಘ quota)
{
	ಆದರೆ (quota < 0)
		seq_puts(sf, "max");
	ಇಲ್ಲದಿದ್ದರೆ
		seq_printf(sf, "%ld", quota);

	seq_printf(sf, " %ld\n", period);
}

/* caller should put the current value in *@periodp before calling */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __maybe_unused cpu_period_quota_parse(ಅಕ್ಷರ *buf, u64 *period_us_p,
						 u64 *quota_us_p)
{
	ಅಕ್ಷರ tok[21];	/* U64_MAX */

	ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಓದು(buf, "%20s %llu", tok, period_us_p) < 1)
		ಹಿಂತಿರುಗಿ -EINVAL;

	ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಓದು(tok, "%llu", quota_us_p) < 1) {
		ಆದರೆ (!strcmp(tok, "max"))
			*quota_us_p = RUNTIME_INF;
		ಇಲ್ಲದಿದ್ದರೆ
			ಹಿಂತಿರುಗಿ -EINVAL;
	}

	ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_ಗುಂಪು_SCHED_BANDWIDTH
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpu_max_show(ರಚನೆ seq_file *sf, ಶೂನ್ಯ *v)
{
	ರಚನೆ task_ಗುಂಪು *tg = css_tg(seq_css(sf));
	u64 period_us, quota_us;

	tg_bandwidth(tg, &period_us, &quota_us, ಖಾಲಿ);
	cpu_period_quota_print(sf, period_us, quota_us);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ssize_t cpu_max_write(ರಚನೆ kernfs_open_file *ದ,
			     ಅಕ್ಷರ *buf, ಗಾತ್ರ_t nbytes, loff_t off)
{
	ರಚನೆ task_ಗುಂಪು *tg = css_tg(ದ_css(ದ));
	u64 period_us, quota_us, ಅವಧಿ_us;
	ಪೂರ್ಣಾಂಕ ret;

	tg_bandwidth(tg, &period_us, ಖಾಲಿ, &ಅವಧಿ_us);
	ret = cpu_period_quota_parse(buf, &period_us, &quota_us);
	ಆದರೆ (!ret)
		ret = tg_ಗಣ_bandwidth(tg, period_us, quota_us, ಅವಧಿ_us);
	ಹಿಂತಿರುಗಿ ret ?: nbytes;
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_CFS_BANDWIDTH */

ಸ್ಥಿರ ರಚನೆ cftype cpu_files[] = {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_ಗುಂಪು_SCHED_ತೂಕ
	{
		.name = "weight",
		.flags = CFTYPE_NOT_ON_ಬೇರು,
		.read_u64 = cpu_ತೂಕ_read_u64,
		.write_u64 = cpu_ತೂಕ_write_u64,
	},
	{
		.name = "weight.nice",
		.flags = CFTYPE_NOT_ON_ಬೇರು,
		.read_s64 = cpu_ತೂಕ_nice_read_s64,
		.write_s64 = cpu_ತೂಕ_nice_write_s64,
	},
	{
		.name = "idle",
		.flags = CFTYPE_NOT_ON_ಬೇರು,
		.read_s64 = cpu_idle_read_s64,
		.write_s64 = cpu_idle_write_s64,
	},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_ಗುಂಪು_SCHED_BANDWIDTH
	{
		.name = "max",
		.flags = CFTYPE_NOT_ON_ಬೇರು,
		.seq_show = cpu_max_show,
		.ಲಿಖ = cpu_max_write,
	},
	{
		.name = "max.burst",
		.flags = CFTYPE_NOT_ON_ಬೇರು,
		.read_u64 = cpu_ಅವಧಿ_read_u64,
		.write_u64 = cpu_ಅವಧಿ_write_u64,
	},
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_CFS_BANDWIDTH */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK_ಗುಂಪು
	{
		.name = "uclamp.min",
		.flags = CFTYPE_NOT_ON_ಬೇರು,
		.seq_show = cpu_uclamp_min_show,
		.ಲಿಖ = cpu_uclamp_min_write,
	},
	{
		.name = "uclamp.max",
		.flags = CFTYPE_NOT_ON_ಬೇರು,
		.seq_show = cpu_uclamp_max_show,
		.ಲಿಖ = cpu_uclamp_max_write,
	},
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_UCLAMP_TASK_GROUP */
	{ }	/* terminate */
};

ರಚನೆ cgroup_subsys cpu_cgrp_subsys = {
	.css_alloc	= cpu_cgroup_css_alloc,
	.css_online	= cpu_cgroup_css_online,
	.css_offline	= cpu_cgroup_css_offline,
	.css_released	= cpu_cgroup_css_released,
	.css_free	= cpu_cgroup_css_free,
	.css_extra_stat_show = cpu_extra_stat_show,
	.css_local_stat_show = cpu_local_stat_show,
	.can_attach	= cpu_cgroup_can_attach,
	.attach		= cpu_cgroup_attach,
	.cancel_attach	= cpu_cgroup_cancel_attach,
	.legacy_cftypes	= cpu_legacy_files,
	.dfl_cftypes	= cpu_files,
	.early_init	= true,
	.threaded	= true,
};

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_CGROUP_SCHED */

ಶೂನ್ಯ dump_cpu_task(ಪೂರ್ಣಾಂಕ cpu)
{
	ಆದರೆ (ಒಳ_hardirq() && cpu == smp_processor_id()) {
		ರಚನೆ pt_regs *regs;

		regs = get_irq_regs();
		ಆದರೆ (regs) {
			show_regs(regs);
			ಹಿಂತಿರುಗಿ;
		}
	}

	ಆದರೆ (trigger_single_cpu_backtrace(cpu))
		ಹಿಂತಿರುಗಿ;

	pr_info("Task dump for CPU %d:\n", cpu);
	sched_show_task(cpu_curr(cpu));
}

/*
 * Nice levels are multiplicative, with a gentle 10% change for every
 * nice level changed. I.e. when a CPU-bound task goes from nice 0 to
 * nice 1, it will get ~10% less CPU time than another CPU-bound task
 * that remained on nice 0.
 *
 * The "10% effect" is relative and cumulative: from _any_ nice level,
 * if you go up 1 level, it's -10% CPU usage, if you go down 1 level
 * it's +10% CPU usage. (to achieve that we use a multiplier of 1.25.
 * If a task goes up by ~10% and another task goes down by ~10% then
 * the relative distance between them is ~25%.)
 */
ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ sched_prio_ಗಮ್ಯ_ತೂಕ[40] = {
 /* -20 */     88761,     71755,     56483,     46273,     36291,
 /* -15 */     29154,     23254,     18705,     14949,     11916,
 /* -10 */      9548,      7620,      6100,      4904,      3906,
 /*  -5 */      3121,      2501,      1991,      1586,      1277,
 /*   0 */      1024,       820,       655,       526,       423,
 /*   5 */       335,       272,       215,       172,       137,
 /*  10 */       110,        87,        70,        56,        45,
 /*  15 */        36,        29,        23,        18,        15,
};

/*
 * Inverse (2^32/x) values of the sched_prio_to_weight[] array, pre-calculated.
 *
 * In cases where the weight does not change often, we can use the
 * pre-calculated inverse to speed up arithmetics by turning divisions
 * into multiplications:
 */
ಸ್ಥಿರಾಂಕ u32 sched_prio_ಗಮ್ಯ_wmult[40] = {
 /* -20 */     48388,     59856,     76040,     92818,    118348,
 /* -15 */    147320,    184698,    229616,    287308,    360437,
 /* -10 */    449829,    563644,    704093,    875809,   1099582,
 /*  -5 */   1376151,   1717300,   2157191,   2708050,   3363326,
 /*   0 */   4194304,   5237765,   6557202,   8165337,  10153587,
 /*   5 */  12820798,  15790321,  19976592,  24970740,  31350126,
 /*  10 */  39045157,  49367440,  61356676,  76695844,  95443717,
 /*  15 */ 119304647, 148102320, 186737708, 238609294, 286331153,
};

ಶೂನ್ಯ call_trace_sched_update_nr_running(ರಚನೆ rq *rq, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2)
{
        trace_sched_update_nr_running_tp(rq, ಎಣಿಕೆ_2);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_MM_CID
/*
 * Concurrency IDentifier management
 *
 * Serialization rules:
 *
 * mm::mm_cid::mutex:	Serializes fork() and exit() and therefore
 *			protects mm::mm_cid::users and mode switch
 *			transitions
 *
 * mm::mm_cid::lock:	Serializes mm_update_max_cids() and
 *			mm_update_cpus_allowed(). Nests in mm_cid::mutex
 *			and runqueue lock.
 *
 * The mm_cidmask bitmap is not protected by any of the mm::mm_cid locks
 * and can only be modified with atomic operations.
 *
 * The mm::mm_cid:pcpu per CPU storage is protected by the CPUs runqueue
 * lock.
 *
 * CID ownership:
 *
 * A CID is either owned by a task (stored in task_struct::mm_cid.cid) or
 * by a CPU (stored in mm::mm_cid.pcpu::cid). CIDs owned by CPUs have the
 * MM_CID_ONCPU bit set.
 *
 * During the transition of ownership mode, the MM_CID_TRANSIT bit is set
 * on the CIDs. When this bit is set the tasks drop the CID back into the
 * pool when scheduling out.
 *
 * Both bits (ONCPU and TRANSIT) are filtered out by task_cid() when the
 * CID is actually handed over to user space in the RSEQ memory.
 *
 * Mode switching:
 *
 * The ownership mode is per process and stored in mm:mm_cid::mode with the
 * following possible states:
 *
 *	0:				Per task ownership
 *	0 | MM_CID_TRANSIT:		Transition from per CPU to per task
 *	MM_CID_ONCPU:			Per CPU ownership
 *	MM_CID_ONCPU | MM_CID_TRANSIT:	Transition from per task to per CPU
 *
 * All transitions of ownership mode happen in two phases:
 *
 *  1) mm:mm_cid::mode has the MM_CID_TRANSIT bit set. This is OR'ed on the
 *     CIDs and denotes that the CID is only temporarily owned by a
 *     task. When the task schedules out it drops the CID back into the
 *     pool if this bit is set.
 *
 *  2) The initiating context walks the per CPU space or the tasks to fixup
 *     or drop the CIDs and after completion it clears MM_CID_TRANSIT in
 *     mm:mm_cid::mode. After that point the CIDs are strictly task or CPU
 *     owned again.
 *
 * This two phase transition is required to prevent CID space exhaustion
 * during the transition as a direct transfer of ownership would fail:
 *
 *   - On task to CPU mode switch if a task is scheduled in on one CPU and
 *     then migrated to another CPU before the fixup freed enough per task
 *     CIDs.
 *
 *   - On CPU to task mode switch if two tasks are scheduled in on the same
 *     CPU before the fixup freed per CPU CIDs.
 *
 *   Both scenarios can result in a live lock because sched_in() is invoked
 *   with runqueue lock held and loops in search of a CID and the fixup
 *   thread can't make progress freeing them up because it is stuck on the
 *   same runqueue lock.
 *
 * While MM_CID_TRANSIT is active during the transition phase the MM_CID
 * bitmap can be contended, but that's a temporary contention bound to the
 * transition period. After that everything goes back into steady state and
 * nothing except fork() and exit() will touch the bitmap. This is an
 * acceptable tradeoff as it completely avoids complex serialization,
 * memory barriers and atomic operations for the common case.
 *
 * Aside of that this mechanism also ensures RT compability:
 *
 *   - The task which runs the fixup is fully preemptible except for the
 *     short runqueue lock held sections.
 *
 *   - The transient impact of the bitmap contention is only problematic
 *     when there is a thundering herd scenario of tasks scheduling in and
 *     out concurrently. There is not much which can be done about that
 *     except for avoiding mode switching by a proper overall system
 *     configuration.
 *
 * Switching to per CPU mode happens when the user count becomes greater
 * than the maximum number of CIDs, which is calculated by:
 *
 *	opt_cids = min(mm_cid::nr_cpus_allowed, mm_cid::users);
 *	max_cids = min(1.25 * opt_cids, num_possible_cpus());
 *
 * The +25% allowance is useful for tight CPU masks in scenarios where only
 * a few threads are created and destroyed to avoid frequent mode
 * switches. Though this allowance shrinks, the closer opt_cids becomes to
 * num_possible_cpus(), which is the (unfortunate) hard ABI limit.
 *
 * At the point of switching to per CPU mode the new user is not yet
 * visible in the system, so the task which initiated the fork() runs the
 * fixup function. mm_cid_fixup_tasks_to_cpu() walks the thread list and
 * either marks each task owned CID with MM_CID_TRANSIT if the task is
 * running on a CPU or drops it into the CID pool if a task is not on a
 * CPU. Tasks which schedule in before the task walk reaches them do the
 * handover in mm_cid_schedin(). When mm_cid_fixup_tasks_to_cpus()
 * completes it is guaranteed that no task related to that MM owns a CID
 * anymore.
 *
 * Switching back to task mode happens when the user count goes below the
 * threshold which was recorded on the per CPU mode switch:
 *
 *	pcpu_thrs = min(opt_cids - (opt_cids / 4), num_possible_cpus() / 2);
 *
 * This threshold is updated when a affinity change increases the number of
 * allowed CPUs for the MM, which might cause a switch back to per task
 * mode.
 *
 * If the switch back was initiated by a exiting task, then that task runs
 * the fixup function. If it was initiated by a affinity change, then it's
 * run either in the deferred update function in context of a workqueue or
 * by a task which forks a new one or by a task which exits. Whatever
 * happens first. mm_cid_fixup_cpus_to_task() walks through the possible
 * CPUs and either marks the CPU owned CIDs with MM_CID_TRANSIT if a
 * related task is running on the CPU or drops it into the pool. Tasks
 * which are scheduled in before the fixup covered them do the handover
 * themself. When mm_cid_fixup_cpus_to_tasks() completes it is guaranteed
 * that no CID related to that MM is owned by a CPU anymore.
 */

/*
 * Update the CID range properties when the constraints change. Invoked via
 * fork(), exit() and affinity changes
 */
ಸ್ಥಿರ ಶೂನ್ಯ __mm_update_max_cids(ರಚನೆ mm_mm_cid *mc)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ opt_cids, max_cids;

	/* Calculate the new optimal constraint */
	opt_cids = min(mc->nr_cpus_allowed, mc->users);

	/* Adjust the maximum CIDs to +25% limited by the number of possible CPUs */
	max_cids = min(opt_cids + (opt_cids / 4), ಅಂಕೆ_possible_cpus());
	WRITE_ONCE(mc->max_cids, max_cids);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ mm_cid_calc_pcpu_thrs(ರಚನೆ mm_mm_cid *mc)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ opt_cids;

	opt_cids = min(mc->nr_cpus_allowed, mc->users);
	/* Has to be at least 1 because 0 indicates PCPU mode off */
	ಹಿಂತಿರುಗಿ max(min(opt_cids - opt_cids / 4, ಅಂಕೆ_possible_cpus() / 2), 1);
}

ಸ್ಥಿರ bool mm_update_max_cids(ರಚನೆ mm_struct *mm)
{
	ರಚನೆ mm_mm_cid *mc = &mm->mm_cid;
	bool percpu = cid_on_cpu(mc->ಬಹುಲಕ);

	lockdep_assert_held(&mm->mm_cid.ಬೀಗ);

	/* Clear deferred mode switch flag. A change is handled by the caller */
	mc->update_deferred = false;
	__mm_update_max_cids(mc);

	/* Check whether owner mode must be changed */
	ಆದರೆ (!percpu) {
		/* Enable per CPU mode when the number of users is above max_cids */
		ಆದರೆ (mc->users > mc->max_cids)
			mc->pcpu_thrs = mm_cid_calc_pcpu_thrs(mc);
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* Switch back to per task if user count under threshold */
		ಆದರೆ (mc->users < mc->pcpu_thrs)
			mc->pcpu_thrs = 0;
	}

	/* Mode change required? */
	ಆದರೆ (percpu == !!mc->pcpu_thrs)
		ಹಿಂತಿರುಗಿ false;

	/* Flip the mode and set the transition flag to bridge the transfer */
	WRITE_ONCE(mc->ಬಹುಲಕ, mc->ಬಹುಲಕ ^ (MM_CID_TRANSIT | MM_CID_ONCPU));
	/*
	 * Order the store against the subsequent fixups so that
	 * acquire(rq::lock) cannot be reordered by the CPU before the
	 * store.
	 */
	smp_mb();
	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ mm_update_cpus_allowed(ರಚನೆ mm_struct *mm, ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *affmsk)
{
	ರಚನೆ cpumask *mm_allowed;
	ರಚನೆ mm_mm_cid *mc;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ತೂಕ;

	ಆದರೆ (!mm || !READ_ONCE(mm->mm_cid.users))
		ಹಿಂತಿರುಗಿ;
	/*
	 * mm::mm_cid::mm_cpus_allowed is the superset of each threads
	 * allowed CPUs mask which means it can only grow.
	 */
	mc = &mm->mm_cid;
	guard(raw_spinlock)(&mc->ಬೀಗ);
	mm_allowed = mm_cpus_allowed(mm);
	ತೂಕ = cpumask_weighted_or(mm_allowed, mm_allowed, affmsk);
	ಆದರೆ (ತೂಕ == mc->nr_cpus_allowed)
		ಹಿಂತಿರುಗಿ;

	WRITE_ONCE(mc->nr_cpus_allowed, ತೂಕ);
	__mm_update_max_cids(mc);
	ಆದರೆ (!cid_on_cpu(mc->ಬಹುಲಕ))
		ಹಿಂತಿರುಗಿ;

	/* Adjust the threshold to the wider set */
	mc->pcpu_thrs = mm_cid_calc_pcpu_thrs(mc);
	/* Switch back to per task mode? */
	ಆದರೆ (mc->users >= mc->pcpu_thrs)
		ಹಿಂತಿರುಗಿ;

	/* Don't queue twice */
	ಆದರೆ (mc->update_deferred)
		ಹಿಂತಿರುಗಿ;

	/* Queue the irq work, which schedules the real work */
	mc->update_deferred = true;
	irq_work_ಸರತಿ(&mc->irq_work);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ mm_cid_complete_transit(ರಚನೆ mm_struct *mm, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬಹುಲಕ)
{
	/*
	 * Ensure that the store removing the TRANSIT bit cannot be
	 * reordered by the CPU before the fixups have been completed.
	 */
	smp_mb();
	WRITE_ONCE(mm->mm_cid.ಬಹುಲಕ, ಬಹುಲಕ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ mm_cid_transit_ಗಮ್ಯ_task(ರಚನೆ task_struct *t, ರಚನೆ mm_cid_pcpu *pcp)
{
	ಆದರೆ (cid_on_cpu(t->mm_cid.cid)) {
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cid = cpu_cid_ಗಮ್ಯ_cid(t->mm_cid.cid);

		t->mm_cid.cid = cid_ಗಮ್ಯ_transit_cid(cid);
		pcp->cid = t->mm_cid.cid;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ mm_cid_fixup_cpus_ಗಮ್ಯ_tasks(ರಚನೆ mm_struct *mm)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu;

	/* Walk the CPUs and fixup all stale CIDs */
	for_each_possible_cpu(cpu) {
		ರಚನೆ mm_cid_pcpu *pcp = ಪ್ರತಿ_cpu_ptr(mm->mm_cid.pcpu, cpu);
		ರಚನೆ rq *rq = cpu_rq(cpu);

		/* Remote access to mm::mm_cid::pcpu requires rq_lock */
		guard(rq_ಬೀಗ_irq)(rq);
		/* Is the CID still owned by the CPU? */
		ಆದರೆ (cid_on_cpu(pcp->cid)) {
			/*
			 * If rq->curr has @mm, transfer it with the
			 * transition bit set. Otherwise drop it.
			 */
			ಆದರೆ (rq->curr->mm == mm && rq->curr->mm_cid.ಸಕ್ರಿಯ)
				mm_cid_transit_ಗಮ್ಯ_task(rq->curr, pcp);
			ಇಲ್ಲದಿದ್ದರೆ
				mm_drop_cid_on_cpu(mm, pcp);

		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (rq->curr->mm == mm && rq->curr->mm_cid.ಸಕ್ರಿಯ) {
			ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cid = rq->curr->mm_cid.cid;

			/*
			 * Set the transition bit only on a genuine task-owned
			 * CID. A running active task can legitimately have
			 * MM_CID_UNSET here: in per-CPU mode CIDs are assigned
			 * lazily on schedule-in, so the fork()/execve() window
			 * leaves the task active with no owned CID. Setting the
			 * transition bit on MM_CID_UNSET would later feed
			 * clear_bit() an out-of-bounds bit number via
			 * mm_cid_schedout(), so exclude it. A CPU-owned
			 * (MM_CID_ONCPU) CID is handled by the cid_on_cpu()
			 * branch above and never reaches here.
			 */
			ಆದರೆ (cid != MM_CID_UNSET && !cid_ಒಳ_transit(cid)) {
				cid = cid_ಗಮ್ಯ_transit_cid(cid);
				rq->curr->mm_cid.cid = cid;
				pcp->cid = cid;
			}
		}
	}
	mm_cid_complete_transit(mm, 0);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ mm_cid_transit_ಗಮ್ಯ_cpu(ರಚನೆ task_struct *t, ರಚನೆ mm_cid_pcpu *pcp)
{
	ಆದರೆ (cid_on_task(t->mm_cid.cid)) {
		t->mm_cid.cid = cid_ಗಮ್ಯ_transit_cid(t->mm_cid.cid);
		pcp->cid = t->mm_cid.cid;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ mm_cid_fixup_task_ಗಮ್ಯ_cpu(ರಚನೆ task_struct *t, ರಚನೆ mm_struct *mm)
{
	/* Remote access to mm::mm_cid::pcpu requires rq_lock */
	guard(task_rq_ಬೀಗ_2)(t);
	ಆದರೆ (cid_on_task(t->mm_cid.cid)) {
		/* If running on the CPU, put the CID in transit mode, otherwise drop it */
		ಆದರೆ (task_rq(t)->curr == t)
			mm_cid_transit_ಗಮ್ಯ_cpu(t, ಪ್ರತಿ_cpu_ptr(mm->mm_cid.pcpu, task_cpu(t)));
		ಇಲ್ಲದಿದ್ದರೆ
			mm_unset_cid_on_task(t);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ mm_cid_fixup_tasks_ಗಮ್ಯ_cpus(ಶೂನ್ಯ)
{
	ರಚನೆ mm_struct *mm = current->mm;
	ರಚನೆ task_struct *t;

	lockdep_assert_held(&mm->mm_cid.mutex);

	hlist_for_each_ನಮೂದು(t, &mm->mm_cid.user_list, mm_cid.ಗಂಟು) {
		/* Current has already transferred before invoking the fixup. */
		ಆದರೆ (t != current)
			mm_cid_fixup_task_ಗಮ್ಯ_cpu(t, mm);
	}

	mm_cid_complete_transit(mm, MM_CID_ONCPU);
}

ಸ್ಥಿರ bool sched_mm_cid_ಕೂಡಿಸು_user(ರಚನೆ task_struct *t, ರಚನೆ mm_struct *mm)
{
	lockdep_assert_held(&mm->mm_cid.ಬೀಗ);

	t->mm_cid.ಸಕ್ರಿಯ = 1;
	hlist_ಕೂಡಿಸು_ತಲೆ(&t->mm_cid.ಗಂಟು, &mm->mm_cid.user_list);
	mm->mm_cid.users++;
	ಹಿಂತಿರುಗಿ mm_update_max_cids(mm);
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_mm_cid_fork(ರಚನೆ task_struct *t)
{
	ರಚನೆ mm_struct *mm = t->mm;
	bool percpu;

	ಆದರೆ (!mm)
		ಹಿಂತಿರುಗಿ;

	WARN_ON_ONCE(t->mm_cid.cid != MM_CID_UNSET);

	guard(mutex)(&mm->mm_cid.mutex);
	scoped_guard(raw_spinlock_irq, &mm->mm_cid.ಬೀಗ) {
		ರಚನೆ mm_cid_pcpu *pcp = this_cpu_ptr(mm->mm_cid.pcpu);

		/* First user ? */
		ಆದರೆ (!mm->mm_cid.users) {
			sched_mm_cid_ಕೂಡಿಸು_user(t, mm);
			t->mm_cid.cid = mm_get_cid(mm);
			/* Required for execve() */
			pcp->cid = t->mm_cid.cid;
			ಹಿಂತಿರುಗಿ;
		}

		ಆದರೆ (!sched_mm_cid_ಕೂಡಿಸು_user(t, mm)) {
			ಆದರೆ (!cid_on_cpu(mm->mm_cid.ಬಹುಲಕ))
				t->mm_cid.cid = mm_get_cid(mm);
			ಹಿಂತಿರುಗಿ;
		}

		/* Handle the mode change and transfer current's CID */
		percpu = cid_on_cpu(mm->mm_cid.ಬಹುಲಕ);
		ಆದರೆ (!percpu)
			mm_cid_transit_ಗಮ್ಯ_task(current, pcp);
		ಇಲ್ಲದಿದ್ದರೆ
			mm_cid_transit_ಗಮ್ಯ_cpu(current, pcp);
	}

	ಆದರೆ (percpu) {
		mm_cid_fixup_tasks_ಗಮ್ಯ_cpus();
	} ಇಲ್ಲದಿದ್ದರೆ {
		mm_cid_fixup_cpus_ಗಮ್ಯ_tasks(mm);
		t->mm_cid.cid = mm_get_cid(mm);
	}
}

ಸ್ಥಿರ bool sched_mm_cid_remove_user(ರಚನೆ task_struct *t)
{
	lockdep_assert_held(&t->mm->mm_cid.ಬೀಗ);

	t->mm_cid.ಸಕ್ರಿಯ = 0;
	/* Clear the transition bit */
	t->mm_cid.cid = cid_from_transit_cid(t->mm_cid.cid);
	mm_unset_cid_on_task(t);
	hlist_del_init(&t->mm_cid.ಗಂಟು);
	t->mm->mm_cid.users--;
	ಹಿಂತಿರುಗಿ mm_update_max_cids(t->mm);
}

ಸ್ಥಿರ bool __sched_mm_cid_exit(ರಚನೆ task_struct *t)
{
	ರಚನೆ mm_struct *mm = t->mm;

	ಆದರೆ (!sched_mm_cid_remove_user(t))
		ಹಿಂತಿರುಗಿ false;
	/*
	 * Contrary to fork() this only deals with a switch back to per
	 * task mode either because the above decreased users or an
	 * affinity change increased the number of allowed CPUs and the
	 * deferred fixup did not run yet.
	 */
	ಆದರೆ (WARN_ON_ONCE(cid_on_cpu(mm->mm_cid.ಬಹುಲಕ)))
		ಹಿಂತಿರುಗಿ false;
	/*
	 * A failed fork(2) cleanup never gets here, so @current must have
	 * the same MM as @t. That's true for exit() and the failed
	 * pthread_create() cleanup case.
	 */
	ಆದರೆ (WARN_ON_ONCE(current->mm != mm))
		ಹಿಂತಿರುಗಿ false;
	ಹಿಂತಿರುಗಿ true;
}

/*
 * When a task exits, the MM CID held by the task is not longer required as
 * the task cannot return to user space.
 */
ಶೂನ್ಯ sched_mm_cid_exit(ರಚನೆ task_struct *t)
{
	ರಚನೆ mm_struct *mm = t->mm;

	ಆದರೆ (!mm || !t->mm_cid.ಸಕ್ರಿಯ)
		ಹಿಂತಿರುಗಿ;
	/*
	 * Ensure that only one instance is doing MM CID operations within
	 * a MM. The common case is uncontended. The rare fixup case adds
	 * some overhead.
	 */
	scoped_guard(mutex, &mm->mm_cid.mutex) {
		/* mm_cid::mutex is sufficient to protect mm_cid::users */
		ಆದರೆ (likely(mm->mm_cid.users > 1)) {
			scoped_guard(raw_spinlock_irq, &mm->mm_cid.ಬೀಗ) {
				ಆದರೆ (!__sched_mm_cid_exit(t))
					ಹಿಂತಿರುಗಿ;
				/*
				 * Mode change. The task has the CID unset
				 * already and dealt with an eventually set
				 * TRANSIT bit. If the CID is owned by the CPU
				 * then drop it.
				 */
				mm_drop_cid_on_cpu(mm, this_cpu_ptr(mm->mm_cid.pcpu));
			}
			mm_cid_fixup_cpus_ಗಮ್ಯ_tasks(mm);
			ಹಿಂತಿರುಗಿ;
		}
		/* Last user */
		scoped_guard(raw_spinlock_irq, &mm->mm_cid.ಬೀಗ) {
			/* Required across execve() */
			ಆದರೆ (t == current)
				mm_cid_transit_ಗಮ್ಯ_task(t, this_cpu_ptr(mm->mm_cid.pcpu));
			/* Ignore mode change. There is nothing to do. */
			sched_mm_cid_remove_user(t);
		}
	}

	/*
	 * As this is the last user (execve(), process exit or failed
	 * fork(2)) there is no concurrency anymore.
	 *
	 * Synchronize eventually pending work to ensure that there are no
	 * dangling references left. @t->mm_cid.users is zero so nothing
	 * can queue this work anymore.
	 */
	irq_work_sync(&mm->mm_cid.irq_work);
	cancel_work_sync(&mm->mm_cid.work);
}

/* Deactivate MM CID allocation across execve() */
ಶೂನ್ಯ sched_mm_cid_before_execve(ರಚನೆ task_struct *t)
{
	sched_mm_cid_exit(t);
}

/* Reactivate MM CID after execve() */
ಶೂನ್ಯ sched_mm_cid_after_execve(ರಚನೆ task_struct *t)
{
	ಆದರೆ (t->mm)
		sched_mm_cid_fork(t);
}

ಸ್ಥಿರ ಶೂನ್ಯ mm_cid_work_fn(ರಚನೆ work_struct *work)
{
	ರಚನೆ mm_struct *mm = container_ದ(work, ರಚನೆ mm_struct, mm_cid.work);

	guard(mutex)(&mm->mm_cid.mutex);
	/* Did the last user task exit already? */
	ಆದರೆ (!mm->mm_cid.users)
		ಹಿಂತಿರುಗಿ;

	scoped_guard(raw_spinlock_irq, &mm->mm_cid.ಬೀಗ) {
		/* Have fork() or exit() handled it already? */
		ಆದರೆ (!mm->mm_cid.update_deferred)
			ಹಿಂತಿರುಗಿ;
		/* This clears mm_cid::update_deferred */
		ಆದರೆ (!mm_update_max_cids(mm))
			ಹಿಂತಿರುಗಿ;
		/* Affinity changes can only switch back to task mode */
		ಆದರೆ (WARN_ON_ONCE(cid_on_cpu(mm->mm_cid.ಬಹುಲಕ)))
			ಹಿಂತಿರುಗಿ;
	}
	mm_cid_fixup_cpus_ಗಮ್ಯ_tasks(mm);
}

ಸ್ಥಿರ ಶೂನ್ಯ mm_cid_irq_work(ರಚನೆ irq_work *work)
{
	ರಚನೆ mm_struct *mm = container_ದ(work, ರಚನೆ mm_struct, mm_cid.irq_work);

	/*
	 * Needs to be unconditional because mm_cid::lock cannot be held
	 * when scheduling work as mm_update_cpus_allowed() nests inside
	 * rq::lock and schedule_work() might end up in wakeup...
	 */
	ವೇಳಾಪಟ್ಟಿ_work(&mm->mm_cid.work);
}

ಶೂನ್ಯ mm_init_cid(ರಚನೆ mm_struct *mm, ರಚನೆ task_struct *p)
{
	mm->mm_cid.max_cids = 0;
	mm->mm_cid.ಬಹುಲಕ = 0;
	mm->mm_cid.nr_cpus_allowed = p->nr_cpus_allowed;
	mm->mm_cid.users = 0;
	mm->mm_cid.pcpu_thrs = 0;
	mm->mm_cid.update_deferred = 0;
	raw_spin_ಬೀಗ_init(&mm->mm_cid.ಬೀಗ);
	mutex_init(&mm->mm_cid.mutex);
	mm->mm_cid.irq_work = IRQ_WORK_INIT_HARD(mm_cid_irq_work);
	INIT_WORK(&mm->mm_cid.work, mm_cid_work_fn);
	INIT_HLIST_ತಲೆ(&mm->mm_cid.user_list);
	cpumask_copy(mm_cpus_allowed(mm), &p->cpus_ಮುಖವಾಡ);
	bitmap_zero(mm_cidmask(mm), ಅಂಕೆ_possible_cpus());
}
#ಇಲ್ಲದಿದ್ದರೆ /* CONFIG_SCHED_MM_CID */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ mm_update_cpus_allowed(ರಚನೆ mm_struct *mm, ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *affmsk) { }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_mm_cid_fork(ರಚನೆ task_struct *t) { }
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_SCHED_MM_CID */

ಸ್ಥಿರ DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ sched_change_ctx, sched_change_ctx);

ರಚನೆ sched_change_ctx *sched_change_begin(ರಚನೆ task_struct *p, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ sched_change_ctx *ctx = this_cpu_ptr(&sched_change_ctx);
	ರಚನೆ rq *rq = task_rq(p);

	/*
	 * Must exclusively use matched flags since this is both dequeue and
	 * enqueue.
	 */
	WARN_ON_ONCE(flags & 0xFFFF0000);

	lockdep_assert_rq_held(rq);

	ಆದರೆ (!(flags & ಸರತಿಯಿಂದ_ತೆಗೆ_NOCLOCK)) {
		update_rq_clock(rq);
		flags |= ಸರತಿಯಿಂದ_ತೆಗೆ_NOCLOCK;
	}

	ಆದರೆ ((flags & ಸರತಿಯಿಂದ_ತೆಗೆ_CLASS) && p->sched_class->switching_from)
		p->sched_class->switching_from(rq, p);

	*ctx = (ರಚನೆ sched_change_ctx){
		.p = p,
		.class = p->sched_class,
		.flags = flags,
		.queued = task_on_rq_queued(p),
		.running = task_current_donor(rq, p),
	};

	ಆದರೆ (!(flags & ಸರತಿಯಿಂದ_ತೆಗೆ_CLASS)) {
		ಆದರೆ (p->sched_class->get_prio)
			ctx->prio = p->sched_class->get_prio(rq, p);
		ಇಲ್ಲದಿದ್ದರೆ
			ctx->prio = p->prio;
	}

	ಆದರೆ (ctx->queued)
		ಸರತಿಯಿಂದ_ತೆಗೆ_task(rq, p, flags);
	ಆದರೆ (ctx->running)
		put_prev_task(rq, p);

	ಆದರೆ ((flags & ಸರತಿಯಿಂದ_ತೆಗೆ_CLASS) && p->sched_class->switched_from)
		p->sched_class->switched_from(rq, p);

	ಹಿಂತಿರುಗಿ ctx;
}

ಶೂನ್ಯ sched_change_ಕೊನೆ(ರಚನೆ sched_change_ctx *ctx)
{
	ರಚನೆ task_struct *p = ctx->p;
	ರಚನೆ rq *rq = task_rq(p);

	lockdep_assert_rq_held(rq);

	/*
	 * Changing class without *QUEUE_CLASS is bad.
	 */
	WARN_ON_ONCE(p->sched_class != ctx->class && !(ctx->flags & ಸರತಿಗೆ_ಸೇರಿಸು_CLASS));

	ಆದರೆ ((ctx->flags & ಸರತಿಗೆ_ಸೇರಿಸು_CLASS) && p->sched_class->switching_ಗಮ್ಯ)
		p->sched_class->switching_ಗಮ್ಯ(rq, p);

	ಆದರೆ (ctx->queued)
		ಸರತಿಗೆ_ಸೇರಿಸು_task(rq, p, ctx->flags);
	ಆದರೆ (ctx->running)
		ಗಣ_ಮುಂದಿನ_task(rq, p);

	ಆದರೆ (ctx->flags & ಸರತಿಗೆ_ಸೇರಿಸು_CLASS) {
		ಆದರೆ (p->sched_class->switched_ಗಮ್ಯ)
			p->sched_class->switched_ಗಮ್ಯ(rq, p);

		/*
		 * Check if the task can preempt the current in case the class
		 * was upgraded. Note that none of the switch*_from() methods
		 * know the new class and none of the switch*_to() methods know
		 * the old class so we must resched here.
		 */
		ಆದರೆ (ctx->queued)
			wakeup_preempt(rq, p, 0);

		ಆದರೆ (ctx->running) {
			/*
			 * If this was a degradation in class; make sure to
			 * reschedule.
			 */
			ಆದರೆ (sched_class_above(ctx->class, p->sched_class))
				resched_curr(rq);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		p->sched_class->prio_ಬದಲಾಯಿತು(rq, p, ctx->prio);
	}
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PREFERRED_CPU
ಸ್ಥಿರ DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ cpu_stop_work, npc_ತಳ್ಳು_task_work);

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_non_preferred_cpu_ತಳ್ಳು_stop(ಶೂನ್ಯ *arg)
{
	ರಚನೆ task_struct *p = arg;
	ರಚನೆ rq *rq = this_rq();
	ರಚನೆ rq_flags rf;
	ಪೂರ್ಣಾಂಕ cpu;

	ಆದರೆ (cpu_preferred(rq->cpu)) {
		scoped_guard(rq_ಬೀಗ_irqsave, rq)
			rq->npc_ತಳ್ಳು_work_pending = false;
		put_task_struct(p);
		ಹಿಂತಿರುಗಿ 0;
	}

	scoped_guard (raw_spinlock_irq, &p->pi_ಬೀಗ) {
		/*
		 * select_fallback_rq() may acquire the rq lock in case of
		 * fallback. So call it before grabbing rq lock. If the task
		 * migrates to another CPU before the rq lock is acquired,
		 * subsequent validation of task's current rq will help to
		 * safely bail out.
		 */
		cpu = select_fallback_rq(rq->cpu, p);
		context_unsafe_alias(rq);
		rq_ಬೀಗ(rq, &rf);
		rq->npc_ತಳ್ಳು_work_pending = false;
		update_rq_clock(rq);

		ಆದರೆ (task_rq(p) == rq && task_on_rq_queued(p)) {
			ರಚನೆ rq *dest_rq = __migrate_task(rq, &rf, p, cpu);

			ಆದರೆ (rq != dest_rq)
				schedstat_inc(p->stats.nr_migrations_cpu_non_preferred);
			rq = dest_rq;
		}
		rq_unlock(rq, &rf);
	}

	put_task_struct(p);
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Push the current task running on non-preferred CPU(npc).
 * Using this non preferred CPU will lead to more contention
 * in the host. So it is better not to use this CPU.
 *
 * Since task is running, call a stopper to push the task out. This is
 * similar to how task moves during hotplug. In select_fallback_rq() a
 * preferred CPU will be chosen and henceforth task shouldn't come back to
 * this CPU again.
 *
 * Works for FAIR class only.
 *
 * If task is affined only on non-preferred CPUs, no point in moving it out.
 */
ಶೂನ್ಯ sched_ತಳ್ಳು_current_non_preferred_cpu(ರಚನೆ rq *rq)
{
	ರಚನೆ task_struct *ತಳ್ಳು_task = rq->curr;

	scoped_guard(rq_ಬೀಗ, rq) {
		/* Push the task if its explicit affinity allows */
		ಆದರೆ (!task_can_migrate_ಗಮ್ಯ_preferred(ತಳ್ಳು_task, rq->cpu))
			ಹಿಂತಿರುಗಿ;

		/* There is already a stopper thread. Don't race with it. */
		ಆದರೆ (rq->npc_ತಳ್ಳು_work_pending)
			ಹಿಂತಿರುಗಿ;

		ಆದರೆ (is_migration_disabled(ತಳ್ಳು_task))
			ಹಿಂತಿರುಗಿ;

		rq->npc_ತಳ್ಳು_work_pending = true;
	}

	/* sched_tick runs with interrupts disabled. */
	get_task_struct(ತಳ್ಳು_task);
	stop_one_cpu_nowait(rq->cpu, sched_non_preferred_cpu_ತಳ್ಳು_stop,
			    ತಳ್ಳು_task, this_cpu_ptr(&npc_ತಳ್ಳು_task_work));
}
#ಆದರೆ_ಅಂತ್ಯ
