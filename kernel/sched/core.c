// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/core.c
 *
 *  Core kernel CPU scheduler code
 *
 *  Copyright (C) 1991-2002  Linus Torvalds
 *  Copyright (C) 1998-2024  Ingo Molnar, Red Hat
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INSTANTIATE_EXPORTED_MIGRATE_DISABLE
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/highmem.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/hrtimer_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/ktime_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/signal.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/syscalls_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/debug_locks.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/prefetch.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/capability.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/pgtable_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/wait_bit.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/jiffies.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/spinlock_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/cpumask_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/lockdep_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/hardirq.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/softirq.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/refcount_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/topology.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/clock.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/cond_resched.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/cputime.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/debug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/hotplug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/init.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/isolation.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/loadavg.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/mm.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/nohz.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/rseq_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/rt.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <linux/context_tracking.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/cpuset.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/delayacct.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/init_task.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/interrupt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/ioprio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/kallsyms.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/kcov.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/kprobes.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/llist_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/mmu_context.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/mmzone.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/mutex_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/nmi.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/nospec.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/perf_event_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/profile.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/psi.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/rcuwait_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/rseq.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/wake_q.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/scs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/slab.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/syscalls.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/vtime.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/wait_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/workqueue_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/livepatch_sched.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PREEMPT_DYNAMIC
# ifdef CONFIG_GENERIC_IRQ_ਇੰਦਰਾਜ
#  include <linux/irq-entry-common.h>
# endif
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <uapi/linux/sched/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <asm/irq_regs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <asm/switch_to.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <asm/tlb.h>

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CREATE_TRACE_POINTS
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/rseq_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <trace/events/sched.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <trace/events/ipi.h>
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CREATE_TRACE_POINTS

#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stats.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "autogroup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pelt.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "smp.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "../workqueue_internal.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../io_uring/io-wq.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../smpboot.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../locking/mutex.h"

EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ_GPL(ipi_send_cpu);
EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ_GPL(ipi_send_cpumask);

/*
 * Export tracepoints that act as a bare tracehook (ie: have no trace event
 * associated with them) to allow external modules to probe them.
 */
EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ_GPL(pelt_cfs_tp);
EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ_GPL(pelt_rt_tp);
EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ_GPL(pelt_dl_tp);
EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ_GPL(pelt_irq_tp);
EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ_GPL(pelt_se_tp);
EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ_GPL(pelt_hw_tp);
EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ_GPL(sched_cpu_ਸਮਰੱਥਾ_tp);
EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ_GPL(sched_overutilized_tp);
EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ_GPL(sched_util_est_cfs_tp);
EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ_GPL(sched_util_est_se_tp);
EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ_GPL(sched_update_nr_running_tp);
EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ_GPL(sched_compute_energy_tp);
EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ_GPL(sched_ਇੰਦਰਾਜ_tp);
EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ_GPL(sched_exit_tp);
EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ_GPL(sched_ਸੈੱਟ_need_resched_tp);
EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ_GPL(sched_dl_throttle_tp);
EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ_GPL(sched_dl_replenish_tp);
EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ_GPL(sched_dl_update_tp);
EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ_GPL(sched_dl_server_ਸ਼ੁਰੂ_tp);
EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ_GPL(sched_dl_server_stop_tp);

DEFINE_ਪ੍ਰਤੀ_CPU_SHARED_ALIGNED(ਬਣਤਰ rq, runqueues);
DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ rnd_state, sched_rnd_state);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_PROXY_EXEC
DEFINE_STATIC_ਕੁੰਜੀ_TRUE(__sched_proxy_exec);
ਸਥਿਰ ਪੂਰਨ __init ਤਿਆਰੀ_proxy_exec(ਅੱਖਰ *str)
{
	bool proxy_enable = true;

	ਜੇ (*str && kstrtobool(str + 1, &proxy_enable)) {
		pr_warn("Unable to parse sched_proxy_exec=\n");
		ਵਾਪਸ 0;
	}

	ਜੇ (proxy_enable) {
		pr_info("sched_proxy_exec enabled via boot arg\n");
		static_branch_enable(&__sched_proxy_exec);
	} ਨਹੀਂਤਾਂ {
		pr_info("sched_proxy_exec disabled via boot arg\n");
		static_branch_disable(&__sched_proxy_exec);
	}
	ਵਾਪਸ 1;
}
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਪੂਰਨ __init ਤਿਆਰੀ_proxy_exec(ਅੱਖਰ *str)
{
	pr_warn("CONFIG_SCHED_PROXY_EXEC=n, so it cannot be enabled or disabled at boot time\n");
	ਵਾਪਸ 0;
}
#ਜੇ_ਅੰਤ
__setup("sched_proxy_exec", ਤਿਆਰੀ_proxy_exec);

/*
 * Debugging: various feature bits
 *
 * If SCHED_DEBUG is disabled, each compilation unit has its own copy of
 * sysctl_sched_features, defined in sched.h, to allow constants propagation
 * at compile time and compiler optimization based on features default.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCHED_FEAT(name, enabled)	\
	(1UL << __SCHED_FEAT_##name) * enabled |
__read_mostly ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_sched_features =
#ਅਨ੍ਤਰ੍ਭਾਵ "features.h"
	0;
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ SCHED_FEAT

/*
 * Print a warning if need_resched is set for the given duration (if
 * LATENCY_WARN is enabled).
 *
 * If sysctl_resched_latency_warn_once is set, only one warning will be shown
 * per boot.
 */
__read_mostly ਪੂਰਨ sysctl_resched_latency_warn_ms = 100;
__read_mostly ਪੂਰਨ sysctl_resched_latency_warn_once = 1;

/*
 * Number of tasks to iterate in a single balance run.
 * Limited because this is done with IRQs disabled.
 */
__read_mostly ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_sched_nr_migrate = SCHED_NR_MIGRATE_BREAK;

__read_mostly ਪੂਰਨ scheduler_running;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CORE

DEFINE_STATIC_ਕੁੰਜੀ_FALSE(__sched_core_enabled);

/* kernel prio, less is more */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ __task_prio(ਅਚਲ ਬਣਤਰ task_struct *p)
{
	ਜੇ (p->sched_class == &stop_sched_class) /* trumps deadline */
		ਵਾਪਸ -2;

	ਜੇ (p->dl_server)
		ਵਾਪਸ -1; /* deadline */

	ਜੇ (rt_or_dl_prio(p->prio))
		ਵਾਪਸ p->prio; /* [-1, 99] */

	ਜੇ (p->sched_class == &idle_sched_class)
		ਵਾਪਸ MAX_RT_PRIO + NICE_ਚੌੜਾਈ; /* 140 */

	ਜੇ (task_on_scx(p))
		ਵਾਪਸ MAX_RT_PRIO + MAX_NICE + 1; /* 120, squash ext */

	ਵਾਪਸ MAX_RT_PRIO + MAX_NICE; /* 119, squash fair */
}

/*
 * l(a,b)
 * le(a,b) := !l(b,a)
 * g(a,b)  := l(b,a)
 * ge(a,b) := !l(a,b)
 */

/* real prio, less is less */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool prio_less(ਅਚਲ ਬਣਤਰ task_struct *a,
			     ਅਚਲ ਬਣਤਰ task_struct *b, bool ਵਿੱਚ_fi)
{

	ਪੂਰਨ pa = __task_prio(a), pb = __task_prio(b);

	ਜੇ (-pa < -pb)
		ਵਾਪਸ true;

	ਜੇ (-pb < -pa)
		ਵਾਪਸ false;

	ਜੇ (pa == -1) { /* dl_prio() doesn't work because of stop_class above */
		ਅਚਲ ਬਣਤਰ sched_dl_entity *a_dl, *b_dl;

		a_dl = &a->dl;
		/*
		 * Since,'a' and 'b' can be CFS tasks served by DL server,
		 * __task_prio() can return -1 (for DL) even for those. In that
		 * case, get to the dl_server's DL entity.
		 */
		ਜੇ (a->dl_server)
			a_dl = a->dl_server;

		b_dl = &b->dl;
		ਜੇ (b->dl_server)
			b_dl = b->dl_server;

		ਵਾਪਸ !dl_ਸਮਾਂ_before(a_dl->deadline, b_dl->deadline);
	}

	ਜੇ (pa == MAX_RT_PRIO + MAX_NICE)	/* fair */
		ਵਾਪਸ cfs_prio_less(a, b, ਵਿੱਚ_fi);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CLASS_EXT
	ਜੇ (pa == MAX_RT_PRIO + MAX_NICE + 1)	/* ext */
		ਵਾਪਸ scx_prio_less(a, b, ਵਿੱਚ_fi);
#ਜੇ_ਅੰਤ

	ਵਾਪਸ false;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool __sched_core_less(ਅਚਲ ਬਣਤਰ task_struct *a,
				     ਅਚਲ ਬਣਤਰ task_struct *b)
{
	ਜੇ (a->core_cookie < b->core_cookie)
		ਵਾਪਸ true;

	ਜੇ (a->core_cookie > b->core_cookie)
		ਵਾਪਸ false;

	/* flip prio, so high prio is leftmost */
	ਜੇ (prio_less(b, a, !!task_rq(a)->core->core_forceidle_ਗਿਣਤੀ))
		ਵਾਪਸ true;

	ਵਾਪਸ false;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __node_2_sc(ਗੰਢ) rb_ਇੰਦਰਾਜ((ਗੰਢ), ਬਣਤਰ task_struct, core_ਗੰਢ)

ਸਥਿਰ ਸਤਰਅੰਦਰ bool rb_sched_core_less(ਬਣਤਰ rb_ਗੰਢ *a, ਅਚਲ ਬਣਤਰ rb_ਗੰਢ *b)
{
	ਵਾਪਸ __sched_core_less(__node_2_sc(a), __node_2_sc(b));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ rb_sched_core_cmp(ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, ਅਚਲ ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ)
{
	ਅਚਲ ਬਣਤਰ task_struct *p = __node_2_sc(ਗੰਢ);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cookie = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)ਕੁੰਜੀ;

	ਜੇ (cookie < p->core_cookie)
		ਵਾਪਸ -1;

	ਜੇ (cookie > p->core_cookie)
		ਵਾਪਸ 1;

	ਵਾਪਸ 0;
}

ਖਾਲੀ sched_core_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਜੇ (p->se.sched_delayed)
		ਵਾਪਸ;

	rq->core->core_task_seq++;

	ਜੇ (!p->core_cookie)
		ਵਾਪਸ;

	rb_ਜੋੜੋ(&p->core_ਗੰਢ, &rq->core_ਰੁੱਖ, rb_sched_core_less);
}

ਖਾਲੀ sched_core_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	ਜੇ (p->se.sched_delayed)
		ਵਾਪਸ;

	rq->core->core_task_seq++;

	ਜੇ (sched_core_enqueued(p)) {
		rb_erase(&p->core_ਗੰਢ, &rq->core_ਰੁੱਖ);
		RB_CLEAR_ਗੰਢ(&p->core_ਗੰਢ);
	}

	/*
	 * Migrating the last task off the cpu, with the cpu in forced idle
	 * state. Reschedule to create an accounting edge for forced idle,
	 * and re-examine whether the core is still in forced idle state.
	 */
	ਜੇ (!(flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SAVE) && rq->nr_running == 1 &&
	    rq->core->core_forceidle_ਗਿਣਤੀ && rq->curr == rq->idle)
		resched_curr(rq);
}

ਸਥਿਰ ਪੂਰਨ sched_task_is_throttled(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu)
{
	ਜੇ (p->sched_class->task_is_throttled)
		ਵਾਪਸ p->sched_class->task_is_throttled(p, cpu);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਣਤਰ task_struct *sched_core_ਅਗਲਾ(ਬਣਤਰ task_struct *p, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cookie)
{
	ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ = &p->core_ਗੰਢ;
	ਪੂਰਨ cpu = task_cpu(p);

	ਕਰੋ {
		ਗੰਢ = rb_ਅਗਲਾ(ਗੰਢ);
		ਜੇ (!ਗੰਢ)
			ਵਾਪਸ ਨਿਰਾ;

		p = __node_2_sc(ਗੰਢ);
		ਜੇ (p->core_cookie != cookie)
			ਵਾਪਸ ਨਿਰਾ;

	} ਜਦੋਂ (sched_task_is_throttled(p, cpu));

	ਵਾਪਸ p;
}

/*
 * Find left-most (aka, highest priority) and unthrottled task matching @cookie.
 * If no suitable task is found, NULL will be returned.
 */
ਸਥਿਰ ਬਣਤਰ task_struct *sched_core_ਲੱਭੋ(ਬਣਤਰ rq *rq, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cookie)
{
	ਬਣਤਰ task_struct *p;
	ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ;

	ਗੰਢ = rb_ਲੱਭੋ_ਪਹਿਲਾ((ਖਾਲੀ *)cookie, &rq->core_ਰੁੱਖ, rb_sched_core_cmp);
	ਜੇ (!ਗੰਢ)
		ਵਾਪਸ ਨਿਰਾ;

	p = __node_2_sc(ਗੰਢ);
	ਜੇ (!sched_task_is_throttled(p, rq->cpu))
		ਵਾਪਸ p;

	ਵਾਪਸ sched_core_ਅਗਲਾ(p, cookie);
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

ਸਥਿਰ DEFINE_MUTEX(sched_core_mutex);
ਸਥਿਰ atomic_t sched_core_ਗਿਣਤੀ;
ਸਥਿਰ ਬਣਤਰ cpumask sched_core_ਨਕਾਬ;

ਸਥਿਰ ਖਾਲੀ sched_core_ਤਾਲਾ(ਪੂਰਨ cpu, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *flags)
	__context_unsafe(/* acquires multiple */)
	__acquires(&runqueues.__lock) /* overapproximation */
{
	ਅਚਲ ਬਣਤਰ cpumask *smt_ਨਕਾਬ = cpu_smt_ਨਕਾਬ(cpu);
	ਪੂਰਨ t, i = 0;

	local_irq_save(*flags);
	for_each_cpu(t, smt_ਨਕਾਬ)
		raw_spin_ਤਾਲਾ_nested(&cpu_rq(t)->__lock, i++);
}

ਸਥਿਰ ਖਾਲੀ sched_core_unlock(ਪੂਰਨ cpu, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *flags)
	__context_unsafe(/* releases multiple */)
	__releases(&runqueues.__lock) /* overapproximation */
{
	ਅਚਲ ਬਣਤਰ cpumask *smt_ਨਕਾਬ = cpu_smt_ਨਕਾਬ(cpu);
	ਪੂਰਨ t;

	for_each_cpu(t, smt_ਨਕਾਬ)
		raw_spin_unlock(&cpu_rq(t)->__lock);
	local_irq_restore(*flags);
}

ਸਥਿਰ ਖਾਲੀ __sched_core_flip(bool enabled)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;
	ਪੂਰਨ cpu, t;

	cpus_read_ਤਾਲਾ();

	/*
	 * Toggle the online cores, one by one.
	 */
	cpumask_copy(&sched_core_ਨਕਾਬ, cpu_online_ਨਕਾਬ);
	for_each_cpu(cpu, &sched_core_ਨਕਾਬ) {
		ਅਚਲ ਬਣਤਰ cpumask *smt_ਨਕਾਬ = cpu_smt_ਨਕਾਬ(cpu);

		sched_core_ਤਾਲਾ(cpu, &flags);

		/*
		 * A core-wide selection may have the shared rq lock temporarily
		 * released by a lock-dropping ->pick_task(). Flipping would
		 * rebind rq_lockp() under it. Wait it out.
		 */
		ਜਦੋਂ (cpu_rq(cpu)->core->core_pick_ਵਿੱਚ_flight) {
			sched_core_unlock(cpu, &flags);
			cpu_relax();
			sched_core_ਤਾਲਾ(cpu, &flags);
		}

		for_each_cpu(t, smt_ਨਕਾਬ)
			cpu_rq(t)->core_enabled = enabled;

		cpu_rq(cpu)->core->core_forceidle_ਸ਼ੁਰੂ = 0;

		sched_core_unlock(cpu, &flags);

		cpumask_andnot(&sched_core_ਨਕਾਬ, &sched_core_ਨਕਾਬ, smt_ਨਕਾਬ);
	}

	/*
	 * Toggle the offline CPUs.
	 */
	for_each_cpu_andnot(cpu, cpu_possible_ਨਕਾਬ, cpu_online_ਨਕਾਬ)
		cpu_rq(cpu)->core_enabled = enabled;

	cpus_read_unlock();
}

ਸਥਿਰ ਖਾਲੀ sched_core_assert_ਖਾਲੀ_ਥਾਵਾਂ(ਖਾਲੀ)
{
	ਪੂਰਨ cpu;

	for_each_possible_cpu(cpu)
		WARN_ON_ONCE(!RB_ਖਾਲੀ_ਥਾਵਾਂ_ਜੜ੍ਹ(&cpu_rq(cpu)->core_ਰੁੱਖ));
}

ਸਥਿਰ ਖਾਲੀ __sched_core_enable(ਖਾਲੀ)
{
	static_branch_enable(&__sched_core_enabled);
	/*
	 * Ensure all previous instances of raw_spin_rq_*lock() have finished
	 * and future ones will observe !sched_core_disabled().
	 */
	synchronize_rcu();
	__sched_core_flip(true);
	sched_core_assert_ਖਾਲੀ_ਥਾਵਾਂ();
}

ਸਥਿਰ ਖਾਲੀ __sched_core_disable(ਖਾਲੀ)
{
	sched_core_assert_ਖਾਲੀ_ਥਾਵਾਂ();
	__sched_core_flip(false);
	static_branch_disable(&__sched_core_enabled);
}

ਖਾਲੀ sched_core_get(ਖਾਲੀ)
{
	ਜੇ (atomic_inc_not_zero(&sched_core_ਗਿਣਤੀ))
		ਵਾਪਸ;

	mutex_ਤਾਲਾ(&sched_core_mutex);
	ਜੇ (!atomic_read(&sched_core_ਗਿਣਤੀ))
		__sched_core_enable();

	smp_mb__before_atomic();
	atomic_inc(&sched_core_ਗਿਣਤੀ);
	mutex_unlock(&sched_core_mutex);
}

ਸਥਿਰ ਖਾਲੀ __sched_core_put(ਬਣਤਰ work_struct *work)
{
	ਜੇ (atomic_dec_and_mutex_ਤਾਲਾ(&sched_core_ਗਿਣਤੀ, &sched_core_mutex)) {
		__sched_core_disable();
		mutex_unlock(&sched_core_mutex);
	}
}

ਖਾਲੀ sched_core_put(ਖਾਲੀ)
{
	ਸਥਿਰ DECLARE_WORK(_work, __sched_core_put);

	/*
	 * "There can be only one"
	 *
	 * Either this is the last one, or we don't actually need to do any
	 * 'work'. If it is the last *again*, we rely on
	 * WORK_STRUCT_PENDING_BIT.
	 */
	ਜੇ (!atomic_ਜੋੜੋ_unless(&sched_core_ਗਿਣਤੀ, -1, 1))
		ਸਮਾਂ_ਸਾਰਣੀ_work(&_work);
}

#ਨਹੀਂਤਾਂ /* !CONFIG_SCHED_CORE: */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_core_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
sched_core_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags) { }

#ਜੇ_ਅੰਤ /* !CONFIG_SCHED_CORE */

/* need a wrapper since we may need to trace from modules */
EXPORT_TRACEPOINT_ਚਿੰਨ੍ਹ(sched_ਸੈੱਟ_state_tp);

/*
 * Call via the helper macro trace_set_current_state.
 * Calls to this function MUST be guarded by a
 * tracepoint_enabled(sched_set_state_tp)
 */
ਖਾਲੀ __trace_set_current_state(ਪੂਰਨ state_ਮੁੱਲ)
{
	trace_call_sched_ਸੈੱਟ_state_tp(current, state_ਮੁੱਲ);
}
EXPORT_ਚਿੰਨ੍ਹ(__trace_set_current_state);

ਪੂਰਨ task_llc(ਅਚਲ ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ ਪ੍ਰਤੀ_cpu(sd_llc_id, task_cpu(p));
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

ਖਾਲੀ raw_spin_rq_ਤਾਲਾ_nested(ਬਣਤਰ rq *rq, ਪੂਰਨ subclass)
	__context_unsafe()
{
	raw_spinlock_t *ਤਾਲਾ;

	/* Matches synchronize_rcu() in __sched_core_enable() */
	preempt_disable();
	ਜੇ (sched_core_disabled()) {
		raw_spin_ਤਾਲਾ_nested(&rq->__lock, subclass);
		/* preempt_count *MUST* be > 1 */
		preempt_enable_no_resched();
		ਵਾਪਸ;
	}

	ਲਈ (;;) {
		ਤਾਲਾ = __rq_lockp(rq);
		raw_spin_ਤਾਲਾ_nested(ਤਾਲਾ, subclass);
		ਜੇ (likely(ਤਾਲਾ == __rq_lockp(rq))) {
			/* preempt_count *MUST* be > 1 */
			preempt_enable_no_resched();
			ਵਾਪਸ;
		}
		raw_spin_unlock(ਤਾਲਾ);
	}
}

bool raw_spin_rq_trylock(ਬਣਤਰ rq *rq)
	__context_unsafe()
{
	raw_spinlock_t *ਤਾਲਾ;
	bool ret;

	/* Matches synchronize_rcu() in __sched_core_enable() */
	preempt_disable();
	ਜੇ (sched_core_disabled()) {
		ret = raw_spin_trylock(&rq->__lock);
		preempt_enable();
		ਵਾਪਸ ret;
	}

	ਲਈ (;;) {
		ਤਾਲਾ = __rq_lockp(rq);
		ret = raw_spin_trylock(ਤਾਲਾ);
		ਜੇ (!ret || (likely(ਤਾਲਾ == __rq_lockp(rq)))) {
			preempt_enable();
			ਵਾਪਸ ret;
		}
		raw_spin_unlock(ਤਾਲਾ);
	}
}

/*
 * double_rq_lock - safely lock two runqueues
 */
ਖਾਲੀ double_rq_ਤਾਲਾ(ਬਣਤਰ rq *rq1, ਬਣਤਰ rq *rq2)
{
	lockdep_assert_irqs_disabled();

	ਜੇ (rq_ਕ੍ਰਮ_less(rq2, rq1))
		ਅਦਲਾ_ਬਦਲੀ(rq1, rq2);

	raw_spin_rq_ਤਾਲਾ(rq1);
	ਜੇ (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_ਤਾਲਾ_nested(rq2, SINGLE_ਡੂੰਘਾਈ_NESTING);
	ਨਹੀਂਤਾਂ
		__acquire_ctx_lock(__rq_lockp(rq2)); /* fake acquire */

	double_rq_clock_clear_update(rq1, rq2);
}

/*
 * ___task_rq_lock - lock the rq @p resides on.
 */
ਬਣਤਰ rq *___task_rq_lock(ਬਣਤਰ task_struct *p, ਬਣਤਰ rq_flags *rf)
{
	ਬਣਤਰ rq *rq;

	lockdep_assert_held(&p->pi_ਤਾਲਾ);

	ਲਈ (;;) {
		rq = task_rq(p);
		raw_spin_rq_ਤਾਲਾ(rq);
		ਜੇ (likely(rq == task_rq(p) && !task_on_rq_migrating(p))) {
			rq_pin_ਤਾਲਾ(rq, rf);
			ਵਾਪਸ rq;
		}
		raw_spin_rq_unlock(rq);

		ਜਦੋਂ (unlikely(task_on_rq_migrating(p)))
			cpu_relax();
	}
}

/*
 * task_rq_lock - lock p->pi_lock and lock the rq @p resides on.
 */
ਬਣਤਰ rq *task_rq_ਤਾਲਾ(ਬਣਤਰ task_struct *p, ਬਣਤਰ rq_flags *rf)
{
	ਬਣਤਰ rq *rq;

	ਲਈ (;;) {
		raw_spin_ਤਾਲਾ_irqsave(&p->pi_ਤਾਲਾ, rf->flags);
		rq = task_rq(p);
		raw_spin_rq_ਤਾਲਾ(rq);
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
		ਜੇ (likely(rq == task_rq(p) && !task_on_rq_migrating(p))) {
			rq_pin_ਤਾਲਾ(rq, rf);
			ਵਾਪਸ rq;
		}
		raw_spin_rq_unlock(rq);
		raw_spin_unlock_irqrestore(&p->pi_ਤਾਲਾ, rf->flags);

		ਜਦੋਂ (unlikely(task_on_rq_migrating(p)))
			cpu_relax();
	}
}

/*
 * RQ-clock updating methods:
 */

/* Use CONFIG_PARAVIRT as this will avoid more #ifdef in arch code. */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PARAVIRT
DEFINE_STATIC_ਕੁੰਜੀ_FALSE(paravirt_steal_rq_enabled);
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਖਾਲੀ update_rq_clock_task(ਬਣਤਰ rq *rq, s64 delta)
{
/*
 * In theory, the compile should just see 0 here, and optimize out the call
 * to sched_rt_avg_update. But I don't trust it...
 */
	s64 __maybe_unused steal = 0, irq_delta = 0;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_IRQ_ਸਮਾਂ_ACCOUNTING
	ਜੇ (irqtime_enabled()) {
		irq_delta = irq_ਸਮਾਂ_read(cpu_ਦਾ(rq)) - rq->prev_irq_ਸਮਾਂ;

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
		ਜੇ (irq_delta > delta)
			irq_delta = delta;

		rq->prev_irq_ਸਮਾਂ += irq_delta;
		delta -= irq_delta;
		delayacct_irq(rq->curr, irq_delta);
	}
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PARAVIRT_ਸਮਾਂ_ACCOUNTING
	ਜੇ (static_branch_unlikely(&paravirt_steal_rq_enabled)) {
		u64 prev_steal;

		steal = prev_steal = paravirt_steal_clock(cpu_ਦਾ(rq));
		steal -= rq->prev_steal_ਸਮਾਂ_rq;

		ਜੇ (unlikely(steal > delta))
			steal = delta;

		rq->prev_steal_ਸਮਾਂ_rq = prev_steal;
		delta -= steal;
	}
#ਜੇ_ਅੰਤ

	rq->clock_task += delta;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_HAVE_SCHED_AVG_IRQ
	ਜੇ ((irq_delta + steal) && sched_feat(NONTASK_ਸਮਰੱਥਾ))
		update_irq_ਲੱਦੋ_avg(rq, irq_delta + steal);
#ਜੇ_ਅੰਤ
	update_rq_clock_pelt(rq, delta);
}

ਖਾਲੀ update_rq_clock(ਬਣਤਰ rq *rq)
{
	s64 delta;
	u64 clock;

	lockdep_assert_rq_held(rq);

	ਜੇ (rq->clock_update_flags & RQCF_ACT_SKIP)
		ਵਾਪਸ;

	ਜੇ (sched_feat(WARN_DOUBLE_CLOCK))
		WARN_ON_ONCE(rq->clock_update_flags & RQCF_UPDATED);
	rq->clock_update_flags |= RQCF_UPDATED;

	clock = sched_clock_cpu(cpu_ਦਾ(rq));
	scx_rq_clock_update(rq, clock);

	delta = clock - rq->clock;
	ਜੇ (delta < 0)
		ਵਾਪਸ;
	rq->clock += delta;

	update_rq_clock_task(rq, delta);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_HRTICK
/*
 * Use HR-timers to deliver accurate preemption points.
 */

ਗਿਣਤੀ {
	HRTICK_SCHED_NONE		= 0,
	HRTICK_SCHED_DEFER		= ਦੋਆਧਾਰੀ_ਅੰਕ(1),
	HRTICK_SCHED_ਸ਼ੁਰੂ		= ਦੋਆਧਾਰੀ_ਅੰਕ(2),
	HRTICK_SCHED_REARM_HRTIMER	= ਦੋਆਧਾਰੀ_ਅੰਕ(3)
};

ਸਥਿਰ ਖਾਲੀ __used hrtick_clear(ਬਣਤਰ rq *rq)
{
	ਜੇ (hrtimer_ਸਰਗਰਮ(&rq->hrtick_timer))
		hrtimer_cancel(&rq->hrtick_timer);
}

/*
 * High-resolution timer tick.
 * Runs from hardirq context with interrupts disabled.
 */
ਸਥਿਰ ਗਿਣਤੀ hrtimer_restart hrtick(ਬਣਤਰ hrtimer *timer)
{
	ਬਣਤਰ rq *rq = container_ਦਾ(timer, ਬਣਤਰ rq, hrtick_timer);
	ਬਣਤਰ rq_flags rf;

	WARN_ON_ONCE(cpu_ਦਾ(rq) != smp_processor_id());

	rq_ਤਾਲਾ(rq, &rf);
	update_rq_clock(rq);
	rq->donor->sched_class->task_tick(rq, rq->donor, 1);
	rq_unlock(rq, &rf);

	ਵਾਪਸ HRTIMER_NORESTART;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool hrtick_needs_rearm(ਬਣਤਰ hrtimer *timer, ktime_t expires)
{
	/*
	 * Queued is false when the timer is not started or currently
	 * running the callback. In both cases, restart. If queued check
	 * whether the expiry time actually changes substantially.
	 */
	ਵਾਪਸ !hrtimer_is_queued(timer) ||
		ਨਿਰਪੇਖ(expires - hrtimer_get_expires(timer)) > 5000;
}

ਸਥਿਰ ਖਾਲੀ hrtick_cond_restart(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ hrtimer *timer = &rq->hrtick_timer;
	ktime_t ਸਮਾਂ = rq->hrtick_ਸਮਾਂ;

	ਜੇ (hrtick_needs_rearm(timer, ਸਮਾਂ))
		hrtimer_ਸ਼ੁਰੂ(timer, ਸਮਾਂ, HRTIMER_ਬਹੁਲਕ_ABS_PINNED_HARD);
}

/*
 * called from hardirq (IPI) context
 */
ਸਥਿਰ ਖਾਲੀ __hrtick_start(ਖਾਲੀ *arg)
{
	ਬਣਤਰ rq *rq = arg;
	ਬਣਤਰ rq_flags rf;

	rq_ਤਾਲਾ(rq, &rf);
	hrtick_cond_restart(rq);
	rq_unlock(rq, &rf);
}

/*
 * Called to set the hrtick timer state.
 *
 * called with rq->lock held and IRQs disabled
 */
ਖਾਲੀ hrtick_ਸ਼ੁਰੂ(ਬਣਤਰ rq *rq, u64 delay)
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
	ਜੇ (rq->hrtick_sched) {
		rq->hrtick_sched |= HRTICK_SCHED_ਸ਼ੁਰੂ;
		rq->hrtick_delay = delta;
		ਵਾਪਸ;
	}

	rq->hrtick_ਸਮਾਂ = ktime_ਜੋੜੋ_ns(ktime_get(), delta);
	ਜੇ (!hrtick_needs_rearm(&rq->hrtick_timer, rq->hrtick_ਸਮਾਂ))
		ਵਾਪਸ;

	ਜੇ (rq == this_rq())
		hrtimer_ਸ਼ੁਰੂ(&rq->hrtick_timer, rq->hrtick_ਸਮਾਂ, HRTIMER_ਬਹੁਲਕ_ABS_PINNED_HARD);
	ਨਹੀਂਤਾਂ
		smp_call_function_single_async(cpu_ਦਾ(rq), &rq->hrtick_csd);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ hrtick_ਸਮਾਂ_ਸਾਰਣੀ_enter(ਬਣਤਰ rq *rq)
{
	rq->hrtick_sched = HRTICK_SCHED_DEFER;
	ਜੇ (hrtimer_test_and_clear_rearm_deferred())
		rq->hrtick_sched |= HRTICK_SCHED_REARM_HRTIMER;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ hrtick_ਸਮਾਂ_ਸਾਰਣੀ_exit(ਬਣਤਰ rq *rq)
{
	ਜੇ (rq->hrtick_sched & HRTICK_SCHED_ਸ਼ੁਰੂ) {
		rq->hrtick_ਸਮਾਂ = ktime_ਜੋੜੋ_ns(ktime_get(), rq->hrtick_delay);
		hrtick_cond_restart(rq);
	} ਨਹੀਂਤਾਂ ਜੇ (idle_rq(rq)) {
		/*
		 * No need for using hrtimer_is_active(). The timer is CPU local
		 * and interrupts are disabled, so the callback cannot be
		 * running and the queued state is valid.
		 */
		ਜੇ (hrtimer_is_queued(&rq->hrtick_timer))
			hrtimer_cancel(&rq->hrtick_timer);
	}

	ਜੇ (rq->hrtick_sched & HRTICK_SCHED_REARM_HRTIMER)
		__hrtimer_rearm_deferred();

	rq->hrtick_sched = HRTICK_SCHED_NONE;
}

ਸਥਿਰ ਖਾਲੀ hrtick_rq_init(ਬਣਤਰ rq *rq)
{
	INIT_CSD(&rq->hrtick_csd, __hrtick_start, rq);
	rq->hrtick_sched = HRTICK_SCHED_NONE;
	hrtimer_ਤਿਆਰੀ(&rq->hrtick_timer, hrtick, CLOCK_MONOTONIC,
		      HRTIMER_ਬਹੁਲਕ_REL_HARD | HRTIMER_ਬਹੁਲਕ_LAZY_REARM);
}
#ਨਹੀਂਤਾਂ /* !CONFIG_SCHED_HRTICK: */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ hrtick_clear(ਬਣਤਰ rq *rq) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ hrtick_rq_init(ਬਣਤਰ rq *rq) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ hrtick_ਸਮਾਂ_ਸਾਰਣੀ_enter(ਬਣਤਰ rq *rq) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ hrtick_ਸਮਾਂ_ਸਾਰਣੀ_exit(ਬਣਤਰ rq *rq) { }
#ਜੇ_ਅੰਤ /* !CONFIG_SCHED_HRTICK */

/*
 * try_cmpxchg based fetch_or() macro so it works for different integer types:
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ fetch_or(ptr, ਨਕਾਬ)						\
	({								\
		typeof(ptr) _ptr = (ptr);				\
		typeof(ਨਕਾਬ) ਨਕਾਬ_2 = (ਨਕਾਬ);				\
		typeof(*_ptr) _val = *_ptr;				\
									\
		ਕਰੋ {							\
		} ਜਦੋਂ (!try_cmpxchg(_ptr, &_val, _val | ਨਕਾਬ_2));	\
	_val;								\
})

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TIF_POLLING_NRFLAG
/*
 * Atomically set TIF_NEED_RESCHED and test for TIF_POLLING_NRFLAG,
 * this avoids any races wrt polling state changes and thereby avoids
 * spurious IPIs.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool ਸੈੱਟ_nr_and_not_polling(ਬਣਤਰ thread_info *ti, ਪੂਰਨ tif)
{
	ਵਾਪਸ !(fetch_or(&ti->flags, 1 << tif) & _TIF_POLLING_NRFLAG);
}

/*
 * Atomically set TIF_NEED_RESCHED if TIF_POLLING_NRFLAG is set.
 *
 * If this returns true, then the idle task promises to call
 * sched_ttwu_pending() and reschedule soon.
 */
ਸਥਿਰ bool ਸੈੱਟ_nr_if_polling(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ thread_info *ti = task_thread_info(p);
	typeof(ti->flags) val = READ_ONCE(ti->flags);

	ਕਰੋ {
		ਜੇ (!(val & _TIF_POLLING_NRFLAG))
			ਵਾਪਸ false;
		ਜੇ (val & _TIF_NEED_RESCHED)
			ਵਾਪਸ true;
	} ਜਦੋਂ (!try_cmpxchg(&ti->flags, &val, val | _TIF_NEED_RESCHED));

	ਵਾਪਸ true;
}

#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਸਤਰਅੰਦਰ bool ਸੈੱਟ_nr_and_not_polling(ਬਣਤਰ thread_info *ti, ਪੂਰਨ tif)
{
	ਸੈੱਟ_ti_thread_flag(ti, tif);
	ਵਾਪਸ true;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool ਸੈੱਟ_nr_if_polling(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ false;
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ bool __wake_q_add(ਬਣਤਰ wake_q_ਸਿਰ *ਸਿਰ, ਬਣਤਰ task_struct *task)
{
	ਬਣਤਰ wake_q_ਗੰਢ *ਗੰਢ = &task->wake_q;

	/*
	 * Atomically grab the task, if ->wake_q is !nil already it means
	 * it's already queued (either by us or someone else) and will get the
	 * wakeup due to that.
	 *
	 * In order to ensure that a pending wakeup will observe our pending
	 * state, even in the failed case, an explicit smp_mb() must be used.
	 */
	smp_mb__before_atomic();
	ਜੇ (unlikely(cmpxchg_relaxed(&ਗੰਢ->ਅਗਲਾ, ਨਿਰਾ, WAKE_Q_ਪੂਛ)))
		ਵਾਪਸ false;

	/*
	 * The head is context local, there can be no concurrency.
	 */
	*ਸਿਰ->lastp = ਗੰਢ;
	ਸਿਰ->lastp = &ਗੰਢ->ਅਗਲਾ;
	ਵਾਪਸ true;
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
ਖਾਲੀ wake_q_ਜੋੜੋ(ਬਣਤਰ wake_q_ਸਿਰ *ਸਿਰ, ਬਣਤਰ task_struct *task)
{
	ਜੇ (__wake_q_add(ਸਿਰ, task))
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
ਖਾਲੀ wake_q_ਜੋੜੋ_ਸੁਰੱਖਿਅਤ(ਬਣਤਰ wake_q_ਸਿਰ *ਸਿਰ, ਬਣਤਰ task_struct *task)
{
	ਜੇ (!__wake_q_add(ਸਿਰ, task))
		put_task_struct(task);
}

ਖਾਲੀ wake_ਉੱਪਰ_q(ਬਣਤਰ wake_q_ਸਿਰ *ਸਿਰ)
{
	ਬਣਤਰ wake_q_ਗੰਢ *ਗੰਢ = ਸਿਰ->ਪਹਿਲਾ;

	ਜਦੋਂ (ਗੰਢ != WAKE_Q_ਪੂਛ) {
		ਬਣਤਰ task_struct *task;

		task = container_ਦਾ(ਗੰਢ, ਬਣਤਰ task_struct, wake_q);
		ਗੰਢ = ਗੰਢ->ਅਗਲਾ;
		/* pairs with cmpxchg_relaxed() in __wake_q_add() */
		WRITE_ONCE(task->wake_q.ਅਗਲਾ, ਨਿਰਾ);
		/* Task can safely be re-inserted now. */

		/*
		 * wake_up_process() executes a full barrier, which pairs with
		 * the queueing in wake_q_add() so as not to miss wakeups.
		 */
		wake_ਉੱਪਰ_process(task);
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
ਸਥਿਰ ਖਾਲੀ __resched_curr(ਬਣਤਰ rq *rq, ਪੂਰਨ tif)
{
	ਬਣਤਰ task_struct *curr = rq->curr;
	ਬਣਤਰ thread_info *cti = task_thread_info(curr);
	bool need_ipi;
	ਪੂਰਨ cpu;

	lockdep_assert_rq_held(rq);

	/*
	 * Always immediately preempt the idle task; no point in delaying doing
	 * actual work.
	 */
	ਜੇ (is_idle_task(curr) && tif == TIF_NEED_RESCHED_LAZY)
		tif = TIF_NEED_RESCHED;

	ਜੇ (cti->flags & ((1 << tif) | _TIF_NEED_RESCHED))
		ਵਾਪਸ;

	cpu = cpu_ਦਾ(rq);

	ਜੇ (cpu == smp_processor_id()) {
		ਸੈੱਟ_ti_thread_flag(cti, tif);
		ਜੇ (tif == TIF_NEED_RESCHED)
			ਸੈੱਟ_preempt_need_resched();
		trace_sched_ਸੈੱਟ_need_resched_tp(curr, cpu, tif);
		ਵਾਪਸ;
	}

	need_ipi = ਸੈੱਟ_nr_and_not_polling(cti, tif);
	trace_sched_ਸੈੱਟ_need_resched_tp(curr, cpu, tif);
	ਜੇ (need_ipi) {
		ਜੇ (tif == TIF_NEED_RESCHED)
			smp_send_reschedule(cpu);
	} ਨਹੀਂਤਾਂ {
		trace_sched_wake_idle_without_ipi(cpu);
	}
}

/*
 * Calls to this function MUST be guarded by a
 * tracepoint_enabled(sched_set_need_resched_tp)
 */
ਖਾਲੀ __trace_set_need_resched(ਬਣਤਰ task_struct *curr, ਪੂਰਨ tif)
{
	trace_call_sched_ਸੈੱਟ_need_resched_tp(curr, smp_processor_id(), tif);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(__trace_set_need_resched);

ਖਾਲੀ resched_curr(ਬਣਤਰ rq *rq)
{
	__resched_curr(rq, TIF_NEED_RESCHED);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PREEMPT_DYNAMIC
ਸਥਿਰ DEFINE_STATIC_ਕੁੰਜੀ_FALSE(sk_dynamic_preempt_lazy);
ਸਥਿਰ __always_inline bool dynamic_preempt_lazy(ਖਾਲੀ)
{
	ਵਾਪਸ static_branch_unlikely(&sk_dynamic_preempt_lazy);
}
#ਨਹੀਂਤਾਂ
ਸਥਿਰ __always_inline bool dynamic_preempt_lazy(ਖਾਲੀ)
{
	ਵਾਪਸ IS_ENABLED(CONFIG_PREEMPT_LAZY);
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ __always_inline ਪੂਰਨ get_lazy_tif_ਦੋਆਧਾਰੀ_ਅੰਕ(ਖਾਲੀ)
{
	ਜੇ (dynamic_preempt_lazy())
		ਵਾਪਸ TIF_NEED_RESCHED_LAZY;

	ਵਾਪਸ TIF_NEED_RESCHED;
}

ਖਾਲੀ resched_curr_lazy(ਬਣਤਰ rq *rq)
{
	__resched_curr(rq, get_lazy_tif_ਦੋਆਧਾਰੀ_ਅੰਕ());
}

ਖਾਲੀ resched_cpu(ਪੂਰਨ cpu)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	raw_spin_rq_ਤਾਲਾ_irqsave(rq, flags);
	ਜੇ (cpu_online(cpu) || cpu == smp_processor_id())
		resched_curr(rq);
	raw_spin_rq_unlock_irqrestore(rq, flags);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NO_HZ_COMMON
/*
 * In the semi idle case, use the nearest busy CPU for migrating timers
 * from an idle CPU.  This is good for power-savings.
 *
 * We don't do similar optimization for completely idle system, as
 * selecting an idle CPU will add more delays to the timers than intended
 * (as that CPU's timer base may not be up to date wrt jiffies etc).
 */
ਪੂਰਨ get_nohz_timer_ਨਿਸ਼ਾਨਾ(ਖਾਲੀ)
{
	ਪੂਰਨ i, cpu = smp_processor_id(), default_cpu = -1;
	ਬਣਤਰ sched_domain *sd;
	ਅਚਲ ਬਣਤਰ cpumask *hk_ਨਕਾਬ;

	ਜੇ (housekeeping_cpu(cpu, HK_TYPE_KERNEL_NOISE)) {
		ਜੇ (!idle_cpu(cpu))
			ਵਾਪਸ cpu;
		default_cpu = cpu;
	}

	hk_ਨਕਾਬ = housekeeping_cpumask(HK_TYPE_KERNEL_NOISE);

	guard(rcu)();

	for_each_domain(cpu, sd) {
		for_each_cpu_and(i, sched_domain_ਵਿਸਤਾਰ(sd), hk_ਨਕਾਬ) {
			ਜੇ (cpu == i)
				ਜਾਰੀ;

			ਜੇ (!idle_cpu(i))
				ਵਾਪਸ i;
		}
	}

	ਜੇ (default_cpu == -1)
		default_cpu = housekeeping_any_cpu(HK_TYPE_KERNEL_NOISE);

	ਵਾਪਸ default_cpu;
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
ਸਥਿਰ ਖਾਲੀ wake_ਉੱਪਰ_idle_cpu(ਪੂਰਨ cpu)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);

	ਜੇ (cpu == smp_processor_id())
		ਵਾਪਸ;

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
	ਜੇ (ਸੈੱਟ_nr_and_not_polling(task_thread_info(rq->idle), TIF_NEED_RESCHED))
		smp_send_reschedule(cpu);
	ਨਹੀਂਤਾਂ
		trace_sched_wake_idle_without_ipi(cpu);
}

ਸਥਿਰ bool wake_ਉੱਪਰ_ਭਰਿਆ_nohz_cpu(ਪੂਰਨ cpu)
{
	/*
	 * We just need the target to call irq_exit() and re-evaluate
	 * the next tick. The nohz full kick at least implies that.
	 * If needed we can still optimize that later with an
	 * empty IRQ.
	 */
	ਜੇ (cpu_is_offline(cpu))
		ਵਾਪਸ true;  /* Don't try to wake offline CPUs. */
	ਜੇ (tick_nohz_ਭਰਿਆ_cpu(cpu)) {
		ਜੇ (cpu != smp_processor_id() ||
		    tick_nohz_tick_stopped())
			tick_nohz_ਭਰਿਆ_kick_cpu(cpu);
		ਵਾਪਸ true;
	}

	ਵਾਪਸ false;
}

/*
 * Wake up the specified CPU.  If the CPU is going offline, it is the
 * caller's responsibility to deal with the lost wakeup, for example,
 * by hooking into the CPU_DEAD notifier like timers and hrtimers do.
 */
ਖਾਲੀ wake_ਉੱਪਰ_nohz_cpu(ਪੂਰਨ cpu)
{
	ਜੇ (!wake_ਉੱਪਰ_ਭਰਿਆ_nohz_cpu(cpu))
		wake_ਉੱਪਰ_idle_cpu(cpu);
}

ਸਥਿਰ ਖਾਲੀ nohz_csd_func(ਖਾਲੀ *info)
{
	ਬਣਤਰ rq *rq = info;
	ਪੂਰਨ cpu = cpu_ਦਾ(rq);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags;

	/*
	 * Release the rq::nohz_csd.
	 */
	flags = atomic_fetch_andnot(NOHZ_KICK_ਨਕਾਬ | NOHZ_NEWILB_KICK, nohz_flags(cpu));
	WARN_ON(!(flags & NOHZ_KICK_ਨਕਾਬ));

	rq->idle_balance = idle_cpu(cpu);
	ਜੇ (rq->idle_balance) {
		rq->nohz_idle_balance = flags;
		__raise_softirq_irqoff(SCHED_SOFTIRQ);
	}
}

#ਜੇ_ਅੰਤ /* CONFIG_NO_HZ_COMMON */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NO_HZ_ਭਰਿਆ
ਸਥਿਰ ਸਤਰਅੰਦਰ bool __need_bw_check(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਜੇ (rq->nr_running != 1)
		ਵਾਪਸ false;

	ਜੇ (p->sched_class != &fair_sched_class)
		ਵਾਪਸ false;

	ਜੇ (!task_on_rq_queued(p))
		ਵਾਪਸ false;

	ਵਾਪਸ true;
}

bool sched_can_stop_tick(ਬਣਤਰ rq *rq)
{
	ਪੂਰਨ fifo_nr_running;

	/* Deadline tasks, even if single, need the tick */
	ਜੇ (rq->dl.dl_nr_running)
		ਵਾਪਸ false;

	/*
	 * If there are more than one RR tasks, we need the tick to affect the
	 * actual RR behaviour.
	 */
	ਜੇ (rq->rt.rr_nr_running) {
		ਜੇ (rq->rt.rr_nr_running == 1)
			ਵਾਪਸ true;
		ਨਹੀਂਤਾਂ
			ਵਾਪਸ false;
	}

	/*
	 * If there's no RR tasks, but FIFO tasks, we can skip the tick, no
	 * forced preemption between FIFO tasks.
	 */
	fifo_nr_running = rq->rt.rt_nr_running - rq->rt.rr_nr_running;
	ਜੇ (fifo_nr_running)
		ਵਾਪਸ true;

	/*
	 * If there are no DL,RR/FIFO tasks, there must only be CFS or SCX tasks
	 * left. For CFS, if there's more than one we need the tick for
	 * involuntary preemption. For SCX, ask.
	 */
	ਜੇ (scx_enabled() && !scx_can_stop_tick(rq))
		ਵਾਪਸ false;

	ਜੇ (rq->cfs.h_nr_queued > 1)
		ਵਾਪਸ false;

	/*
	 * If there is one task and it has CFS runtime bandwidth constraints
	 * and it's on the cpu now we don't want to stop the tick.
	 * This check prevents clearing the bit if a newly enqueued task here is
	 * dequeued by migrating while the constrained task continues to run.
	 * E.g. going from 2->1 without going through pick_next_task().
	 */
	ਜੇ (__need_bw_check(rq, rq->curr)) {
		ਜੇ (cfs_task_bw_constrained(rq->curr))
			ਵਾਪਸ false;
	}

	ਵਾਪਸ true;
}
#ਜੇ_ਅੰਤ /* CONFIG_NO_HZ_FULL */

#ਜੇ defined(CONFIG_RT_ਸਮੂਹ_SCHED) || defined(CONFIG_FAIR_ਸਮੂਹ_SCHED)
/*
 * Iterate task_group tree rooted at *from, calling @down when first entering a
 * node and @up when leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
ਪੂਰਨ walk_tg_ਰੁੱਖ_from(ਬਣਤਰ task_ਸਮੂਹ *from,
			     tg_visitor ਥੱਲੇ, tg_visitor ਉੱਪਰ, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ task_ਸਮੂਹ *ਮਾਪਾ, *ਬੱਚਾ;
	ਪੂਰਨ ret;

	ਮਾਪਾ = from;

ਥੱਲੇ:
	ret = (*ਥੱਲੇ)(ਮਾਪਾ, ਅੰਕੜੇ);
	ਜੇ (ret)
		ਜਾਓ ਬਾਹਰਲਾ;
	list_for_each_ਇੰਦਰਾਜ_rcu(ਬੱਚਾ, &ਮਾਪਾ->children, siblings) {
		ਮਾਪਾ = ਬੱਚਾ;
		ਜਾਓ ਥੱਲੇ;

ਉੱਪਰ:
		ਜਾਰੀ;
	}
	ret = (*ਉੱਪਰ)(ਮਾਪਾ, ਅੰਕੜੇ);
	ਜੇ (ret || ਮਾਪਾ == from)
		ਜਾਓ ਬਾਹਰਲਾ;

	ਬੱਚਾ = ਮਾਪਾ;
	ਮਾਪਾ = ਮਾਪਾ->ਮਾਪਾ;
	ਜੇ (ਮਾਪਾ)
		ਜਾਓ ਉੱਪਰ;
ਬਾਹਰਲਾ:
	ਵਾਪਸ ret;
}

ਪੂਰਨ tg_nop(ਬਣਤਰ task_ਸਮੂਹ *tg, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਵਾਪਸ 0;
}
#ਜੇ_ਅੰਤ

ਖਾਲੀ ਸੈੱਟ_ਲੱਦੋ_ਭਾਰ(ਬਣਤਰ task_struct *p, bool update_ਲੱਦੋ)
{
	ਪੂਰਨ prio = p->static_prio - MAX_RT_PRIO;
	ਬਣਤਰ ਲੱਦੋ_ਭਾਰ lw;

	ਜੇ (task_has_idle_policy(p)) {
		lw.ਭਾਰ = scale_ਲੱਦੋ(ਭਾਰ_IDLEPRIO);
		lw.inv_ਭਾਰ = WMULT_IDLEPRIO;
	} ਨਹੀਂਤਾਂ {
		lw.ਭਾਰ = scale_ਲੱਦੋ(sched_prio_ਮੰਜ਼ਿਲ_ਭਾਰ[prio]);
		lw.inv_ਭਾਰ = sched_prio_ਮੰਜ਼ਿਲ_wmult[prio];
	}

	/*
	 * SCHED_OTHER tasks have to update their load when changing their
	 * weight
	 */
	ਜੇ (update_ਲੱਦੋ && p->sched_class->reweight_task)
		p->sched_class->reweight_task(task_rq(p), p, &lw);
	ਨਹੀਂਤਾਂ
		p->se.ਲੱਦੋ = lw;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK
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
ਸਥਿਰ __maybe_unused DEFINE_MUTEX(uclamp_mutex);

/* Max allowed minimum utilization */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ __maybe_unused sysctl_sched_uclamp_util_min = SCHED_ਸਮਰੱਥਾ_SCALE;

/* Max allowed maximum utilization */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ __maybe_unused sysctl_sched_uclamp_util_max = SCHED_ਸਮਰੱਥਾ_SCALE;

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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_sched_uclamp_util_min_rt_default = SCHED_ਸਮਰੱਥਾ_SCALE;

/* All clamps are required to be less or equal than these values */
ਸਥਿਰ ਬਣਤਰ uclamp_se uclamp_default[UCLAMP_CNT];

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
DEFINE_STATIC_ਕੁੰਜੀ_FALSE(sched_uclamp_ਵਰਤਿਆ);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
uclamp_idle_ਮੁੱਲ(ਬਣਤਰ rq *rq, ਗਿਣਤੀ uclamp_id clamp_id,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ clamp_ਮੁੱਲ)
{
	/*
	 * Avoid blocked utilization pushing up the frequency when we go
	 * idle (which drops the max-clamp) by retaining the last known
	 * max-clamp.
	 */
	ਜੇ (clamp_id == UCLAMP_MAX) {
		rq->uclamp_flags |= UCLAMP_FLAG_IDLE;
		ਵਾਪਸ clamp_ਮੁੱਲ;
	}

	ਵਾਪਸ uclamp_none(UCLAMP_MIN);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ uclamp_idle_reset(ਬਣਤਰ rq *rq, ਗਿਣਤੀ uclamp_id clamp_id,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ clamp_ਮੁੱਲ)
{
	/* Reset max-clamp retention only on idle exit */
	ਜੇ (!(rq->uclamp_flags & UCLAMP_FLAG_IDLE))
		ਵਾਪਸ;

	uclamp_rq_ਸੈੱਟ(rq, clamp_id, clamp_ਮੁੱਲ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ uclamp_rq_max_ਮੁੱਲ(ਬਣਤਰ rq *rq, ਗਿਣਤੀ uclamp_id clamp_id,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ clamp_ਮੁੱਲ)
{
	ਬਣਤਰ uclamp_ਬਾਲਟੀ *ਬਾਲਟੀ = rq->uclamp[clamp_id].ਬਾਲਟੀ;
	ਪੂਰਨ ਬਾਲਟੀ_id = UCLAMP_BUCKETS - 1;

	/*
	 * Since both min and max clamps are max aggregated, find the
	 * top most bucket with tasks in.
	 */
	ਲਈ ( ; ਬਾਲਟੀ_id >= 0; ਬਾਲਟੀ_id--) {
		ਜੇ (!ਬਾਲਟੀ[ਬਾਲਟੀ_id].tasks)
			ਜਾਰੀ;
		ਵਾਪਸ ਬਾਲਟੀ[ਬਾਲਟੀ_id].ਮੁੱਲ;
	}

	/* No tasks -- default clamp values */
	ਵਾਪਸ uclamp_idle_ਮੁੱਲ(rq, clamp_id, clamp_ਮੁੱਲ);
}

ਸਥਿਰ ਖਾਲੀ __uclamp_update_util_min_rt_default(ਬਣਤਰ task_struct *p)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ default_util_min;
	ਬਣਤਰ uclamp_se *uc_se;

	lockdep_assert_held(&p->pi_ਤਾਲਾ);

	uc_se = &p->uclamp_req[UCLAMP_MIN];

	/* Only sync if user didn't override the default */
	ਜੇ (uc_se->user_defined)
		ਵਾਪਸ;

	default_util_min = sysctl_sched_uclamp_util_min_rt_default;
	uclamp_se_ਸੈੱਟ(uc_se, default_util_min, false);
}

ਸਥਿਰ ਖਾਲੀ uclamp_update_util_min_rt_default(ਬਣਤਰ task_struct *p)
{
	ਜੇ (!rt_task(p))
		ਵਾਪਸ;

	/* Protect updates to p->uclamp_* */
	guard(task_rq_ਤਾਲਾ_2)(p);
	__uclamp_update_util_min_rt_default(p);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ uclamp_se
uclamp_tg_restrict(ਬਣਤਰ task_struct *p, ਗਿਣਤੀ uclamp_id clamp_id)
{
	/* Copy by value as we could modify it */
	ਬਣਤਰ uclamp_se uc_req = p->uclamp_req[clamp_id];
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK_ਸਮੂਹ
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ tg_min, tg_max, ਮੁੱਲ;

	/*
	 * Tasks in autogroups or root task group will be
	 * restricted by system defaults.
	 */
	ਜੇ (task_ਸਮੂਹ_is_autogroup(task_ਸਮੂਹ(p)))
		ਵਾਪਸ uc_req;
	ਜੇ (task_ਸਮੂਹ(p) == &ਜੜ੍ਹ_task_ਸਮੂਹ)
		ਵਾਪਸ uc_req;

	tg_min = task_ਸਮੂਹ(p)->uclamp[UCLAMP_MIN].ਮੁੱਲ;
	tg_max = task_ਸਮੂਹ(p)->uclamp[UCLAMP_MAX].ਮੁੱਲ;
	ਮੁੱਲ = uc_req.ਮੁੱਲ;
	ਮੁੱਲ = clamp(ਮੁੱਲ, tg_min, tg_max);
	uclamp_se_ਸੈੱਟ(&uc_req, ਮੁੱਲ, false);
#ਜੇ_ਅੰਤ

	ਵਾਪਸ uc_req;
}

/*
 * The effective clamp bucket index of a task depends on, by increasing
 * priority:
 * - the task specific clamp value, when explicitly requested from userspace
 * - the task group effective clamp value, for tasks not either in the root
 *   group or in an autogroup
 * - the system default clamp value, defined by the sysadmin
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ uclamp_se
uclamp_eff_get(ਬਣਤਰ task_struct *p, ਗਿਣਤੀ uclamp_id clamp_id)
{
	ਬਣਤਰ uclamp_se uc_req = uclamp_tg_restrict(p, clamp_id);
	ਬਣਤਰ uclamp_se uc_max = uclamp_default[clamp_id];

	/* System default restrictions always apply */
	ਜੇ (unlikely(uc_req.ਮੁੱਲ > uc_max.ਮੁੱਲ))
		ਵਾਪਸ uc_max;

	ਵਾਪਸ uc_req;
}

ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ uclamp_eff_ਮੁੱਲ(ਬਣਤਰ task_struct *p, ਗਿਣਤੀ uclamp_id clamp_id)
{
	ਬਣਤਰ uclamp_se uc_eff;

	/* Task currently refcounted: use back-annotated (effective) value */
	ਜੇ (p->uclamp[clamp_id].ਸਰਗਰਮ)
		ਵਾਪਸ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)p->uclamp[clamp_id].ਮੁੱਲ;

	uc_eff = uclamp_eff_get(p, clamp_id);

	ਵਾਪਸ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)uc_eff.ਮੁੱਲ;
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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ uclamp_rq_inc_id(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p,
				    ਗਿਣਤੀ uclamp_id clamp_id)
{
	ਬਣਤਰ uclamp_rq *uc_rq = &rq->uclamp[clamp_id];
	ਬਣਤਰ uclamp_se *uc_se = &p->uclamp[clamp_id];
	ਬਣਤਰ uclamp_ਬਾਲਟੀ *ਬਾਲਟੀ;

	lockdep_assert_rq_held(rq);

	/* Update task effective clamp */
	p->uclamp[clamp_id] = uclamp_eff_get(p, clamp_id);

	ਬਾਲਟੀ = &uc_rq->ਬਾਲਟੀ[uc_se->ਬਾਲਟੀ_id];
	ਬਾਲਟੀ->tasks++;
	uc_se->ਸਰਗਰਮ = true;

	uclamp_idle_reset(rq, clamp_id, uc_se->ਮੁੱਲ);

	/*
	 * Local max aggregation: rq buckets always track the max
	 * "requested" clamp value of its RUNNABLE tasks.
	 */
	ਜੇ (ਬਾਲਟੀ->tasks == 1 || uc_se->ਮੁੱਲ > ਬਾਲਟੀ->ਮੁੱਲ)
		ਬਾਲਟੀ->ਮੁੱਲ = uc_se->ਮੁੱਲ;

	ਜੇ (uc_se->ਮੁੱਲ > uclamp_rq_get(rq, clamp_id))
		uclamp_rq_ਸੈੱਟ(rq, clamp_id, uc_se->ਮੁੱਲ);
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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ uclamp_rq_dec_id(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p,
				    ਗਿਣਤੀ uclamp_id clamp_id)
{
	ਬਣਤਰ uclamp_rq *uc_rq = &rq->uclamp[clamp_id];
	ਬਣਤਰ uclamp_se *uc_se = &p->uclamp[clamp_id];
	ਬਣਤਰ uclamp_ਬਾਲਟੀ *ਬਾਲਟੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ bkt_clamp;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ rq_clamp;

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
	ਜੇ (unlikely(!uc_se->ਸਰਗਰਮ))
		ਵਾਪਸ;

	ਬਾਲਟੀ = &uc_rq->ਬਾਲਟੀ[uc_se->ਬਾਲਟੀ_id];

	WARN_ON_ONCE(!ਬਾਲਟੀ->tasks);
	ਜੇ (likely(ਬਾਲਟੀ->tasks))
		ਬਾਲਟੀ->tasks--;

	uc_se->ਸਰਗਰਮ = false;

	/*
	 * Keep "local max aggregation" simple and accept to (possibly)
	 * overboost some RUNNABLE tasks in the same bucket.
	 * The rq clamp bucket value is reset to its base value whenever
	 * there are no more RUNNABLE tasks refcounting it.
	 */
	ਜੇ (likely(ਬਾਲਟੀ->tasks))
		ਵਾਪਸ;

	rq_clamp = uclamp_rq_get(rq, clamp_id);
	/*
	 * Defensive programming: this should never happen. If it happens,
	 * e.g. due to future modification, warn and fix up the expected value.
	 */
	WARN_ON_ONCE(ਬਾਲਟੀ->ਮੁੱਲ > rq_clamp);
	ਜੇ (ਬਾਲਟੀ->ਮੁੱਲ >= rq_clamp) {
		bkt_clamp = uclamp_rq_max_ਮੁੱਲ(rq, clamp_id, uc_se->ਮੁੱਲ);
		uclamp_rq_ਸੈੱਟ(rq, clamp_id, bkt_clamp);
	}
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ uclamp_rq_inc(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	ਗਿਣਤੀ uclamp_id clamp_id;

	/*
	 * Avoid any overhead until uclamp is actually used by the userspace.
	 *
	 * The condition is constructed such that a NOP is generated when
	 * sched_uclamp_used is disabled.
	 */
	ਜੇ (!uclamp_is_ਵਰਤਿਆ())
		ਵਾਪਸ;

	ਜੇ (unlikely(!p->sched_class->uclamp_enabled))
		ਵਾਪਸ;

	/* Only inc the delayed task which being woken up. */
	ਜੇ (p->se.sched_delayed && !(flags & ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_DELAYED))
		ਵਾਪਸ;

	for_each_clamp_id(clamp_id)
		uclamp_rq_inc_id(rq, p, clamp_id);

	/* Reset clamp idle holding when there is one RUNNABLE task */
	ਜੇ (rq->uclamp_flags & UCLAMP_FLAG_IDLE)
		rq->uclamp_flags &= ~UCLAMP_FLAG_IDLE;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ uclamp_rq_dec(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਗਿਣਤੀ uclamp_id clamp_id;

	/*
	 * Avoid any overhead until uclamp is actually used by the userspace.
	 *
	 * The condition is constructed such that a NOP is generated when
	 * sched_uclamp_used is disabled.
	 */
	ਜੇ (!uclamp_is_ਵਰਤਿਆ())
		ਵਾਪਸ;

	ਜੇ (unlikely(!p->sched_class->uclamp_enabled))
		ਵਾਪਸ;

	ਜੇ (p->se.sched_delayed)
		ਵਾਪਸ;

	for_each_clamp_id(clamp_id)
		uclamp_rq_dec_id(rq, p, clamp_id);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ uclamp_rq_reinc_id(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p,
				      ਗਿਣਤੀ uclamp_id clamp_id)
{
	ਜੇ (!p->uclamp[clamp_id].ਸਰਗਰਮ)
		ਵਾਪਸ;

	uclamp_rq_dec_id(rq, p, clamp_id);
	uclamp_rq_inc_id(rq, p, clamp_id);

	/*
	 * Make sure to clear the idle flag if we've transiently reached 0
	 * active tasks on rq.
	 */
	ਜੇ (clamp_id == UCLAMP_MAX && (rq->uclamp_flags & UCLAMP_FLAG_IDLE))
		rq->uclamp_flags &= ~UCLAMP_FLAG_IDLE;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
uclamp_update_ਸਰਗਰਮ(ਬਣਤਰ task_struct *p)
{
	ਗਿਣਤੀ uclamp_id clamp_id;
	ਬਣਤਰ rq_flags rf;
	ਬਣਤਰ rq *rq;

	/*
	 * Lock the task and the rq where the task is (or was) queued.
	 *
	 * We might lock the (previous) rq of a !RUNNABLE task, but that's the
	 * price to pay to safely serialize util_{min,max} updates with
	 * enqueues, dequeues and migration operations.
	 * This is the same locking schema used by __set_cpus_allowed_ptr().
	 */
	rq = task_rq_ਤਾਲਾ_2(p, &rf);

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

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK_ਸਮੂਹ
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
uclamp_update_ਸਰਗਰਮ_tasks(ਬਣਤਰ cgroup_subsys_state *css)
{
	ਬਣਤਰ css_task_iter it;
	ਬਣਤਰ task_struct *p;

	css_task_iter_ਸ਼ੁਰੂ(css, 0, &it);
	ਜਦੋਂ ((p = css_task_iter_ਅਗਲਾ(&it)))
		uclamp_update_ਸਰਗਰਮ(p);
	css_task_iter_ਸਿਰਾ(&it);
}

ਸਥਿਰ ਖਾਲੀ cpu_util_update_eff(ਬਣਤਰ cgroup_subsys_state *css);
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SYSCTL
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK_ਸਮੂਹ
ਸਥਿਰ ਖਾਲੀ uclamp_update_ਜੜ੍ਹ_tg(ਖਾਲੀ)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg = &ਜੜ੍ਹ_task_ਸਮੂਹ;

	uclamp_se_ਸੈੱਟ(&tg->uclamp_req[UCLAMP_MIN],
		      sysctl_sched_uclamp_util_min, false);
	uclamp_se_ਸੈੱਟ(&tg->uclamp_req[UCLAMP_MAX],
		      sysctl_sched_uclamp_util_max, false);

	guard(rcu)();
	cpu_util_update_eff(&ਜੜ੍ਹ_task_ਸਮੂਹ.css);
}
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਖਾਲੀ uclamp_update_ਜੜ੍ਹ_tg(ਖਾਲੀ) { }
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਖਾਲੀ uclamp_sync_util_min_rt_default(ਖਾਲੀ)
{
	ਬਣਤਰ task_struct *g, *p;

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
	read_ਤਾਲਾ(&tasklist_ਤਾਲਾ);
	smp_mb__after_spinlock();
	read_unlock(&tasklist_ਤਾਲਾ);

	guard(rcu)();
	for_each_process_thread(g, p)
		uclamp_update_util_min_rt_default(p);
}

ਸਥਿਰ ਪੂਰਨ sysctl_sched_uclamp_handler(ਅਚਲ ਬਣਤਰ ctl_ਸਾਰਣੀ *ਸਾਰਣੀ, ਪੂਰਨ ਲਿਖ,
				ਖਾਲੀ *ਬਫਰ, ਮਾਪ_t *lenp, loff_t *ppos)
{
	bool update_ਜੜ੍ਹ_tg = false;
	ਪੂਰਨ old_min, old_max, old_min_rt;
	ਪੂਰਨ ਨਤੀਜਾ;

	guard(mutex)(&uclamp_mutex);

	old_min = sysctl_sched_uclamp_util_min;
	old_max = sysctl_sched_uclamp_util_max;
	old_min_rt = sysctl_sched_uclamp_util_min_rt_default;

	ਨਤੀਜਾ = proc_dointvec(ਸਾਰਣੀ, ਲਿਖ, ਬਫਰ, lenp, ppos);
	ਜੇ (ਨਤੀਜਾ)
		ਜਾਓ undo;
	ਜੇ (!ਲਿਖ)
		ਵਾਪਸ 0;

	ਜੇ (sysctl_sched_uclamp_util_min > sysctl_sched_uclamp_util_max ||
	    sysctl_sched_uclamp_util_max > SCHED_ਸਮਰੱਥਾ_SCALE	||
	    sysctl_sched_uclamp_util_min_rt_default > SCHED_ਸਮਰੱਥਾ_SCALE) {

		ਨਤੀਜਾ = -EINVAL;
		ਜਾਓ undo;
	}

	ਜੇ (old_min != sysctl_sched_uclamp_util_min) {
		uclamp_se_ਸੈੱਟ(&uclamp_default[UCLAMP_MIN],
			      sysctl_sched_uclamp_util_min, false);
		update_ਜੜ੍ਹ_tg = true;
	}
	ਜੇ (old_max != sysctl_sched_uclamp_util_max) {
		uclamp_se_ਸੈੱਟ(&uclamp_default[UCLAMP_MAX],
			      sysctl_sched_uclamp_util_max, false);
		update_ਜੜ੍ਹ_tg = true;
	}

	ਜੇ (update_ਜੜ੍ਹ_tg) {
		sched_uclamp_enable();
		uclamp_update_ਜੜ੍ਹ_tg();
	}

	ਜੇ (old_min_rt != sysctl_sched_uclamp_util_min_rt_default) {
		sched_uclamp_enable();
		uclamp_sync_util_min_rt_default();
	}

	/*
	 * We update all RUNNABLE tasks only when task groups are in use.
	 * Otherwise, keep it simple and do just a lazy update at each next
	 * task enqueue time.
	 */
	ਵਾਪਸ 0;

undo:
	sysctl_sched_uclamp_util_min = old_min;
	sysctl_sched_uclamp_util_max = old_max;
	sysctl_sched_uclamp_util_min_rt_default = old_min_rt;
	ਵਾਪਸ ਨਤੀਜਾ;
}
#ਜੇ_ਅੰਤ /* CONFIG_SYSCTL */

ਸਥਿਰ ਖਾਲੀ uclamp_fork(ਬਣਤਰ task_struct *p)
{
	ਗਿਣਤੀ uclamp_id clamp_id;

	/*
	 * We don't need to hold task_rq_lock() when updating p->uclamp_* here
	 * as the task is still at its early fork stages.
	 */
	for_each_clamp_id(clamp_id)
		p->uclamp[clamp_id].ਸਰਗਰਮ = false;

	ਜੇ (likely(!p->sched_reset_on_fork))
		ਵਾਪਸ;

	for_each_clamp_id(clamp_id) {
		uclamp_se_ਸੈੱਟ(&p->uclamp_req[clamp_id],
			      uclamp_none(clamp_id), false);
	}
}

ਸਥਿਰ ਖਾਲੀ uclamp_post_fork(ਬਣਤਰ task_struct *p)
{
	uclamp_update_util_min_rt_default(p);
}

ਸਥਿਰ ਖਾਲੀ __init init_uclamp_rq(ਬਣਤਰ rq *rq)
{
	ਗਿਣਤੀ uclamp_id clamp_id;
	ਬਣਤਰ uclamp_rq *uc_rq = rq->uclamp;

	for_each_clamp_id(clamp_id) {
		uc_rq[clamp_id] = (ਬਣਤਰ uclamp_rq) {
			.ਮੁੱਲ = uclamp_none(clamp_id)
		};
	}

	rq->uclamp_flags = UCLAMP_FLAG_IDLE;
}

ਸਥਿਰ ਖਾਲੀ __init init_uclamp(ਖਾਲੀ)
{
	ਬਣਤਰ uclamp_se uc_max = {};
	ਗਿਣਤੀ uclamp_id clamp_id;
	ਪੂਰਨ cpu;

	for_each_possible_cpu(cpu)
		init_uclamp_rq(cpu_rq(cpu));

	for_each_clamp_id(clamp_id) {
		uclamp_se_ਸੈੱਟ(&init_task.uclamp_req[clamp_id],
			      uclamp_none(clamp_id), false);
	}

	/* System defaults allow max clamp values for both indexes */
	uclamp_se_ਸੈੱਟ(&uc_max, uclamp_none(UCLAMP_MAX), false);
	for_each_clamp_id(clamp_id) {
		uclamp_default[clamp_id] = uc_max;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK_ਸਮੂਹ
		ਜੜ੍ਹ_task_ਸਮੂਹ.uclamp_req[clamp_id] = uc_max;
		ਜੜ੍ਹ_task_ਸਮੂਹ.uclamp[clamp_id] = uc_max;
#ਜੇ_ਅੰਤ
	}
}

#ਨਹੀਂਤਾਂ /* !CONFIG_UCLAMP_TASK: */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ uclamp_rq_inc(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ uclamp_rq_dec(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ uclamp_fork(ਬਣਤਰ task_struct *p) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ uclamp_post_fork(ਬਣਤਰ task_struct *p) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ init_uclamp(ਖਾਲੀ) { }
#ਜੇ_ਅੰਤ /* !CONFIG_UCLAMP_TASK */

bool sched_task_on_rq(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ task_on_rq_queued(p);
}

ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ get_wchan(ਬਣਤਰ task_struct *p)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ip = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ state;

	ਜੇ (!p || p == current)
		ਵਾਪਸ 0;

	/* Only get wchan if task is blocked and we can keep it that way. */
	raw_spin_ਤਾਲਾ_irq(&p->pi_ਤਾਲਾ);
	state = READ_ONCE(p->__state);
	smp_rmb(); /* see try_to_wake_up() */
	ਜੇ (state != TASK_RUNNING && state != TASK_WAKING && !p->on_rq)
		ip = __get_wchan(p);
	raw_spin_unlock_irq(&p->pi_ਤਾਲਾ);

	ਵਾਪਸ ip;
}

ਖਾਲੀ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	ਜੇ (!(flags & ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_NOCLOCK))
		update_rq_clock(rq);

	/*
	 * Can be before ->enqueue_task() because uclamp considers the
	 * ENQUEUE_DELAYED task before its ->sched_delayed gets cleared
	 * in ->enqueue_task().
	 */
	uclamp_rq_inc(rq, p, flags);

	p->sched_class->ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task(rq, p, flags);

	psi_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(p, flags);

	ਜੇ (!(flags & ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_RESTORE))
		sched_info_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(rq, p);

	ਜੇ (sched_core_enabled(rq))
		sched_core_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(rq, p);
}

/*
 * Must only return false when DEQUEUE_SLEEP.
 */
ਸਤਰਅੰਦਰ bool ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	ਜੇ (sched_core_enabled(rq))
		sched_core_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(rq, p, flags);

	ਜੇ (!(flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_NOCLOCK))
		update_rq_clock(rq);

	ਜੇ (!(flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SAVE))
		sched_info_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(rq, p);

	psi_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(p, flags);

	/*
	 * Must be before ->dequeue_task() because ->dequeue_task() can 'fail'
	 * and mark the task ->sched_delayed.
	 */
	uclamp_rq_dec(rq, p);
	ਵਾਪਸ p->sched_class->ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task(rq, p, flags);
}

ਖਾਲੀ activate_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	ਜੇ (task_on_rq_migrating(p))
		flags |= ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_MIGRATED;

	ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task(rq, p, flags);

	WRITE_ONCE(p->on_rq, TASK_ON_RQ_QUEUED);
	ASSERT_EXCLUSIVE_WRITER(p->on_rq);
}

ਖਾਲੀ deactivate_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	WARN_ON_ONCE(flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SLEEP);

	WRITE_ONCE(p->on_rq, TASK_ON_RQ_MIGRATING);
	ASSERT_EXCLUSIVE_WRITER(p->on_rq);

	/*
	 * Code explicitly relies on TASK_ON_RQ_MIGRATING begin set *before*
	 * dequeue_task() and cleared *after* enqueue_task().
	 */

	ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task(rq, p, flags);
}

ਸਥਿਰ bool ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_ਖੰਡ_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ task_state)
{
	ਪੂਰਨ flags = ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_NOCLOCK;

	p->sched_contributes_ਮੰਜ਼ਿਲ_ਲੱਦੋ =
		(task_state & TASK_UNINTERRUPTIBLE) &&
		!(task_state & TASK_NOLOAD) &&
		!(task_state & TASK_FROZEN);

	ਜੇ (unlikely(is_special_task_state(task_state)))
		flags |= ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SPECIAL;

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
	ਵਾਪਸ ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task(rq, p, ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SLEEP | flags);
}

ਸਥਿਰ ਖਾਲੀ ਖੰਡ_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ task_state)
{
	ਜੇ (ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_ਖੰਡ_task(rq, p, task_state))
		__block_task(rq, p);
}

/**
 * task_curr - is this task currently executing on a CPU?
 * @p: the task in question.
 *
 * Return: 1 if the task is currently executing. 0 otherwise.
 */
ਸਤਰਅੰਦਰ ਪੂਰਨ task_curr(ਅਚਲ ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ cpu_curr(task_cpu(p)) == p;
}

ਖਾਲੀ wakeup_preempt(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	ਬਣਤਰ task_struct *donor = rq->donor;

	ਜੇ (p->sched_class == rq->ਅਗਲਾ_class) {
		rq->ਅਗਲਾ_class->wakeup_preempt(rq, p, flags);

	} ਨਹੀਂਤਾਂ ਜੇ (sched_class_above(p->sched_class, rq->ਅਗਲਾ_class)) {
		rq->ਅਗਲਾ_class->wakeup_preempt(rq, p, flags);
		resched_curr(rq);
		rq->ਅਗਲਾ_class = p->sched_class;
	}

	/*
	 * A queue event has occurred, and we're going to schedule.  In
	 * this case, we can save a useless back to back clock update.
	 */
	ਜੇ (task_on_rq_queued(donor) && test_tsk_need_resched(rq->curr))
		rq_clock_skip_update(rq);
}

ਸਥਿਰ __always_inline
ਪੂਰਨ __task_state_match(ਬਣਤਰ task_struct *p, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ state)
{
	ਜੇ (READ_ONCE(p->__state) & state)
		ਵਾਪਸ 1;

	ਜੇ (READ_ONCE(p->saved_state) & state)
		ਵਾਪਸ -1;

	ਵਾਪਸ 0;
}

ਸਥਿਰ __always_inline
ਪੂਰਨ task_state_ਮੇਲ(ਬਣਤਰ task_struct *p, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ state)
{
	/*
	 * Serialize against current_save_and_set_rtlock_wait_state(),
	 * current_restore_rtlock_saved_state(), and __refrigerator().
	 */
	guard(raw_spinlock_irq)(&p->pi_ਤਾਲਾ);
	ਵਾਪਸ __task_state_match(p, state);
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ wait_task_inactive(ਬਣਤਰ task_struct *p, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੇਲ_state)
{
	ਪੂਰਨ running, queued, ਮੇਲ;
	ਬਣਤਰ rq_flags rf;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ncsw;
	ਬਣਤਰ rq *rq;

	ਲਈ (;;) {
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
		ਜਦੋਂ (task_on_cpu(rq, p)) {
			ਜੇ (!task_state_ਮੇਲ(p, ਮੇਲ_state))
				ਵਾਪਸ 0;
			cpu_relax();
		}

		/*
		 * Ok, time to look more closely! We need the rq
		 * lock now, to be *sure*. If we're wrong, we'll
		 * just go back and repeat.
		 */
		rq = task_rq_ਤਾਲਾ_2(p, &rf);
		/*
		 * If task is sched_delayed, force dequeue it, to avoid always
		 * hitting the tick timeout in the queued case
		 */
		ਜੇ (p->se.sched_delayed)
			ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task(rq, p, ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SLEEP | ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_DELAYED);
		trace_sched_wait_task(p);
		running = task_on_cpu(rq, p);
		queued = task_on_rq_queued(p);
		ncsw = 0;
		ਜੇ ((ਮੇਲ = __task_state_match(p, ਮੇਲ_state))) {
			/*
			 * When matching on p->saved_state, consider this task
			 * still queued so it will wait.
			 */
			ਜੇ (ਮੇਲ < 0)
				queued = 1;
			ncsw = p->nvcsw | LONG_MIN; /* sets MSB */
		}
		task_rq_unlock(rq, p, &rf);

		/*
		 * If it changed from the expected state, bail out now.
		 */
		ਜੇ (unlikely(!ncsw))
			ਰੋਕੋ;

		/*
		 * Was it really running after all now that we
		 * checked with the proper locks actually held?
		 *
		 * Oops. Go back and try again..
		 */
		ਜੇ (unlikely(running)) {
			cpu_relax();
			ਜਾਰੀ;
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
		ਜੇ (unlikely(queued)) {
			ktime_t ਮੰਜ਼ਿਲ = NSEC_ਪ੍ਰਤੀ_SEC / HZ;

			ਸੈੱਟ_current_state(TASK_UNINTERRUPTIBLE);
			ਸਮਾਂ_ਸਾਰਣੀ_hrtimeout(&ਮੰਜ਼ਿਲ, HRTIMER_ਬਹੁਲਕ_REL_HARD);
			ਜਾਰੀ;
		}

		/*
		 * Ahh, all good. It wasn't running, and it wasn't
		 * runnable, which means that it will never become
		 * running in the future either. We're all done!
		 */
		ਰੋਕੋ;
	}

	ਵਾਪਸ ncsw;
}

ਸਥਿਰ ਖਾਲੀ
do_ਸੈੱਟ_cpus_allowed(ਬਣਤਰ task_struct *p, ਬਣਤਰ affinity_context *ctx);

ਸਥਿਰ ਖਾਲੀ migrate_disable_switch(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ affinity_context ac = {
		.new_ਨਕਾਬ  = cpumask_ਦਾ(rq->cpu),
		.flags     = SCA_MIGRATE_DISABLE,
	};

	ਜੇ (likely(!p->migration_disabled))
		ਵਾਪਸ;

	ਜੇ (p->cpus_ptr != &p->cpus_ਨਕਾਬ)
		ਵਾਪਸ;

	scoped_guard (task_rq_ਤਾਲਾ_2, p)
		do_ਸੈੱਟ_cpus_allowed(p, &ac);
}

ਖਾਲੀ ___migrate_enable(ਖਾਲੀ)
{
	ਬਣਤਰ task_struct *p = current;
	ਬਣਤਰ affinity_context ac = {
		.new_ਨਕਾਬ  = &p->cpus_ਨਕਾਬ,
		.flags     = SCA_MIGRATE_ENABLE,
	};

	__set_cpus_allowed_ptr(p, &ac);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(___migrate_enable);

ਖਾਲੀ migrate_disable(ਖਾਲੀ)
{
	__migrate_disable();
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(migrate_disable);

ਖਾਲੀ migrate_enable(ਖਾਲੀ)
{
	__migrate_enable();
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(migrate_enable);

ਸਥਿਰ ਸਤਰਅੰਦਰ bool rq_has_pinned_tasks(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ rq->nr_pinned;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool task_can_migrate_ਮੰਜ਼ਿਲ_preferred(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu)
{
	/* No need to migrate from a preferred CPU */
	ਜੇ (cpu_preferred(cpu))
		ਵਾਪਸ false;

	/* Only FAIR tasks honor preferred CPU state */
	ਜੇ (unlikely(p->sched_class != &fair_sched_class))
		ਵਾਪਸ false;

	/* Ignore preferred state if task affinity is changing */
	ਜੇ (unlikely(!cpumask_test_cpu(task_cpu(p), p->cpus_ptr)))
		ਵਾਪਸ false;

	ਵਾਪਸ cpumask_intersects_and(p->cpus_ptr, cpu_preferred_ਨਕਾਬ,
				      task_cpu_possible_ਨਕਾਬ(p));
}

/*
 * Per-CPU kthreads are allowed to run on !active && online CPUs, see
 * __set_cpus_allowed_ptr() and select_fallback_rq().
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool is_cpu_allowed(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu)
{
	/* When not in the task's cpumask, no point in looking further. */
	ਜੇ (!task_allowed_on_cpu(p, cpu))
		ਵਾਪਸ false;

	/* migrate_disabled() must be allowed to finish. */
	ਜੇ (is_migration_disabled(p))
		ਵਾਪਸ cpu_online(cpu);

	/* Non kernel threads are not allowed during either online or offline. */
	ਜੇ (!(p->flags & PF_KTHREAD)) {
		/* Try to use preferred CPU if task's affinity allows */
		ਜੇ (task_can_migrate_ਮੰਜ਼ਿਲ_preferred(p, cpu))
			ਵਾਪਸ false;
		ਵਾਪਸ cpu_ਸਰਗਰਮ(cpu);
	}

	/* KTHREAD_IS_PER_CPU is always allowed. */
	ਜੇ (kthread_is_ਪ੍ਰਤੀ_cpu(p))
		ਵਾਪਸ cpu_online(cpu);

	/* Regular kernel threads don't get to stay during offline. */
	ਜੇ (cpu_dying(cpu))
		ਵਾਪਸ false;

	/* Try to keep unbound kthreads on a preferred CPU if possible. */
	ਜੇ (task_can_migrate_ਮੰਜ਼ਿਲ_preferred(p, cpu))
		ਵਾਪਸ false;

	/* Otherwise, they are allowed to run on online CPU. */
	ਵਾਪਸ cpu_online(cpu);
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
ਸਥਿਰ ਬਣਤਰ rq *move_queued_task(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf,
				   ਬਣਤਰ task_struct *p, ਪੂਰਨ new_cpu)
	__must_hold(__rq_lockp(rq))
{
	lockdep_assert_rq_held(rq);

	deactivate_task(rq, p, ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_NOCLOCK);
	ਸੈੱਟ_task_cpu(p, new_cpu);
	rq_unlock(rq, rf);

	rq = cpu_rq(new_cpu);

	rq_ਤਾਲਾ(rq, rf);
	WARN_ON_ONCE(task_cpu(p) != new_cpu);
	activate_task(rq, p, 0);
	wakeup_preempt(rq, p, 0);

	ਵਾਪਸ rq;
}

ਬਣਤਰ migration_arg {
	ਬਣਤਰ task_struct		*task;
	ਪੂਰਨ				dest_cpu;
	ਬਣਤਰ ਸੈੱਟ_affinity_pending	*pending;
};

/*
 * @refs: number of wait_for_completion()
 * @stop_pending: is @stop_work in use
 */
ਬਣਤਰ ਸੈੱਟ_affinity_pending {
	refcount_t		refs;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		stop_pending;
	ਬਣਤਰ completion	ਪੂਰਾ;
	ਬਣਤਰ cpu_stop_work	stop_work;
	ਬਣਤਰ migration_arg	arg;
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
ਸਥਿਰ ਬਣਤਰ rq *__migrate_task(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf,
				 ਬਣਤਰ task_struct *p, ਪੂਰਨ dest_cpu)
	__must_hold(__rq_lockp(rq))
{
	/* Affinity changed (again). */
	ਜੇ (!is_cpu_allowed(p, dest_cpu))
		ਵਾਪਸ rq;

	rq = move_queued_task(rq, rf, p, dest_cpu);

	ਵਾਪਸ rq;
}

/*
 * migration_cpu_stop - this will be executed by a high-prio stopper thread
 * and performs thread migration by bumping thread off CPU then
 * 'pushing' onto another runqueue.
 */
ਸਥਿਰ ਪੂਰਨ migration_cpu_stop(ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ migration_arg *arg = ਅੰਕੜੇ;
	ਬਣਤਰ ਸੈੱਟ_affinity_pending *pending = arg->pending;
	ਬਣਤਰ task_struct *p = arg->task;
	ਬਣਤਰ rq *rq = this_rq();
	bool complete = false;
	ਬਣਤਰ rq_flags rf;

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
	flush_smp_call_function_ਕਤਾਰ();

	/*
	 * We may change the underlying rq, but the locks held will
	 * appropriately be "transferred" when switching.
	 */
	context_unsafe_alias(rq);

	raw_spin_ਤਾਲਾ(&p->pi_ਤਾਲਾ);
	rq_ਤਾਲਾ(rq, &rf);

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
	ਜੇ (task_rq(p) == rq) {
		ਜੇ (is_migration_disabled(p))
			ਜਾਓ ਬਾਹਰਲਾ;

		ਜੇ (pending) {
			p->migration_pending = ਨਿਰਾ;
			complete = true;

			ਜੇ (cpumask_test_cpu(task_cpu(p), &p->cpus_ਨਕਾਬ))
				ਜਾਓ ਬਾਹਰਲਾ;
		}

		ਜੇ (task_on_rq_queued(p)) {
			update_rq_clock(rq);
			rq = __migrate_task(rq, &rf, p, arg->dest_cpu);
		} ਨਹੀਂਤਾਂ {
			p->wake_cpu = arg->dest_cpu;
		}

		/*
		 * XXX __migrate_task() can fail, at which point we might end
		 * up running on a dodgy CPU, AFAICT this can only happen
		 * during CPU hotplug, at which point we'll get pushed out
		 * anyway, so it's probably not a big deal.
		 */

	} ਨਹੀਂਤਾਂ ਜੇ (pending) {
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
		ਜੇ (cpumask_test_cpu(task_cpu(p), p->cpus_ptr)) {
			p->migration_pending = ਨਿਰਾ;
			complete = true;
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		/*
		 * When migrate_enable() hits a rq mis-match we can't reliably
		 * determine is_migration_disabled() and so have to chase after
		 * it.
		 */
		WARN_ON_ONCE(!pending->stop_pending);
		preempt_disable();
		rq_unlock(rq, &rf);
		raw_spin_unlock_irqrestore(&p->pi_ਤਾਲਾ, rf.flags);
		stop_one_cpu_nowait(task_cpu(p), migration_cpu_stop,
				    &pending->arg, &pending->stop_work);
		preempt_enable();
		ਵਾਪਸ 0;
	}
ਬਾਹਰਲਾ:
	ਜੇ (pending)
		pending->stop_pending = false;
	rq_unlock(rq, &rf);
	raw_spin_unlock_irqrestore(&p->pi_ਤਾਲਾ, rf.flags);

	ਜੇ (complete)
		complete_all(&pending->ਪੂਰਾ);

	ਵਾਪਸ 0;
}

ਪੂਰਨ ਧੱਕੋ_cpu_stop(ਖਾਲੀ *arg)
{
	ਬਣਤਰ rq *ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq = ਨਿਰਾ, *rq = this_rq();
	ਬਣਤਰ task_struct *p = arg;

	raw_spin_ਤਾਲਾ_irq(&p->pi_ਤਾਲਾ);
	raw_spin_rq_ਤਾਲਾ(rq);

	ਜੇ (task_rq(p) != rq)
		ਜਾਓ ਬਾਹਰਲਾ_unlock;

	ਜੇ (is_migration_disabled(p)) {
		p->migration_flags |= MDF_ਧੱਕੋ;
		ਜਾਓ ਬਾਹਰਲਾ_unlock;
	}

	p->migration_flags &= ~MDF_ਧੱਕੋ;

	ਜੇ (p->sched_class->ਲੱਭੋ_ਤਾਲਾ_rq)
		ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq = p->sched_class->ਲੱਭੋ_ਤਾਲਾ_rq(p, rq);

	ਜੇ (!ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq)
		ਜਾਓ ਬਾਹਰਲਾ_unlock;

	lockdep_assert_rq_held(ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq);

	// XXX validate p is still the highest prio task
	ਜੇ (task_rq(p) == rq) {
		move_queued_task_locked(rq, ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq, p);
		resched_curr(ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq);
	}

	double_unlock_balance(rq, ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq);

ਬਾਹਰਲਾ_unlock:
	rq->ਧੱਕੋ_busy = false;
	raw_spin_rq_unlock(rq);
	raw_spin_unlock_irq(&p->pi_ਤਾਲਾ);

	put_task_struct(p);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ mm_update_cpus_allowed(ਬਣਤਰ mm_struct *mm, ਅਚਲ cpumask_t *affmask);

/*
 * sched_class::set_cpus_allowed must do the below, but is not required to
 * actually call this function.
 */
ਖਾਲੀ ਸੈੱਟ_cpus_allowed_common(ਬਣਤਰ task_struct *p, ਬਣਤਰ affinity_context *ctx)
{
	ਜੇ (ctx->flags & (SCA_MIGRATE_ENABLE | SCA_MIGRATE_DISABLE)) {
		p->cpus_ptr = ctx->new_ਨਕਾਬ;
		ਵਾਪਸ;
	}

	cpumask_copy(&p->cpus_ਨਕਾਬ, ctx->new_ਨਕਾਬ);
	p->nr_cpus_allowed = cpumask_ਭਾਰ(ctx->new_ਨਕਾਬ);
	mm_update_cpus_allowed(p->mm, ctx->new_ਨਕਾਬ);

	/*
	 * Swap in a new user_cpus_ptr if SCA_USER flag set
	 */
	ਜੇ (ctx->flags & SCA_USER)
		ਅਦਲਾ_ਬਦਲੀ(p->user_cpus_ptr, ctx->user_ਨਕਾਬ);
}

ਸਥਿਰ ਖਾਲੀ
do_ਸੈੱਟ_cpus_allowed(ਬਣਤਰ task_struct *p, ਬਣਤਰ affinity_context *ctx)
{
	scoped_guard (sched_change, p, ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SAVE)
		p->sched_class->ਸੈੱਟ_cpus_allowed(p, ctx);
}

/*
 * Used for kthread_bind() and select_fallback_rq(), in both cases the user
 * affinity (if any) should be destroyed too.
 */
ਖਾਲੀ ਸੈੱਟ_cpus_allowed_force(ਬਣਤਰ task_struct *p, ਅਚਲ ਬਣਤਰ cpumask *new_ਨਕਾਬ)
{
	ਬਣਤਰ affinity_context ac = {
		.new_ਨਕਾਬ  = new_ਨਕਾਬ,
		.user_ਨਕਾਬ = ਨਿਰਾ,
		.flags     = SCA_USER,	/* clear the user requested mask */
	};
	ਸੰਘ cpumask_rcuhead {
		cpumask_t cpumask;
		ਬਣਤਰ rcu_ਸਿਰ rcu;
	};

	scoped_guard (__task_rq_lock, p)
		do_ਸੈੱਟ_cpus_allowed(p, &ac);

	/*
	 * Because this is called with p->pi_lock held, it is not possible
	 * to use kfree() here (when PREEMPT_RT=y), therefore punt to using
	 * kfree_rcu().
	 */
	kfree_rcu((ਸੰਘ cpumask_rcuhead *)ac.user_ਨਕਾਬ, rcu);
}

ਪੂਰਨ dup_user_cpus_ptr(ਬਣਤਰ task_struct *dst, ਬਣਤਰ task_struct *src,
		      ਪੂਰਨ ਗੰਢ)
{
	cpumask_t *user_ਨਕਾਬ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	/*
	 * Always clear dst->user_cpus_ptr first as their user_cpus_ptr's
	 * may differ by now due to racing.
	 */
	dst->user_cpus_ptr = ਨਿਰਾ;

	/*
	 * This check is racy and losing the race is a valid situation.
	 * It is not worth the extra overhead of taking the pi_lock on
	 * every fork/clone.
	 */
	ਜੇ (ਅੰਕੜੇ_race(!src->user_cpus_ptr))
		ਵਾਪਸ 0;

	user_ਨਕਾਬ = alloc_user_cpus_ptr(ਗੰਢ);
	ਜੇ (!user_ਨਕਾਬ)
		ਵਾਪਸ -ENOMEM;

	/*
	 * Use pi_lock to protect content of user_cpus_ptr
	 *
	 * Though unlikely, user_cpus_ptr can be reset to NULL by a concurrent
	 * set_cpus_allowed_force().
	 */
	raw_spin_ਤਾਲਾ_irqsave(&src->pi_ਤਾਲਾ, flags);
	ਜੇ (src->user_cpus_ptr) {
		ਅਦਲਾ_ਬਦਲੀ(dst->user_cpus_ptr, user_ਨਕਾਬ);
		cpumask_copy(dst->user_cpus_ptr, src->user_cpus_ptr);
	}
	raw_spin_unlock_irqrestore(&src->pi_ਤਾਲਾ, flags);

	ਜੇ (unlikely(user_ਨਕਾਬ))
		kfree(user_ਨਕਾਬ);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ cpumask *clear_user_cpus_ptr(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ cpumask *user_ਨਕਾਬ = ਨਿਰਾ;

	ਅਦਲਾ_ਬਦਲੀ(p->user_cpus_ptr, user_ਨਕਾਬ);

	ਵਾਪਸ user_ਨਕਾਬ;
}

ਖਾਲੀ release_user_cpus_ptr(ਬਣਤਰ task_struct *p)
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
ਸਥਿਰ ਪੂਰਨ affine_move_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਬਣਤਰ rq_flags *rf,
			    ਪੂਰਨ dest_cpu, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
	__releases(__rq_lockp(rq), &p->pi_ਤਾਲਾ)
{
	ਬਣਤਰ ਸੈੱਟ_affinity_pending my_pending = { }, *pending = ਨਿਰਾ;
	bool stop_pending, complete = false;

	/*
	 * Can the task run on the task's current CPU? If so, we're done
	 *
	 * We are also done if the task is the current donor, boosting a lock-
	 * holding proxy, (and potentially has been migrated outside its
	 * current or previous affinity mask)
	 */
	ਜੇ (cpumask_test_cpu(task_cpu(p), &p->cpus_ਨਕਾਬ) ||
	    (task_current_donor(rq, p) && !task_current(rq, p))) {
		ਬਣਤਰ task_struct *ਧੱਕੋ_task = ਨਿਰਾ;

		ਜੇ ((flags & SCA_MIGRATE_ENABLE) &&
		    (p->migration_flags & MDF_ਧੱਕੋ) && !rq->ਧੱਕੋ_busy) {
			rq->ਧੱਕੋ_busy = true;
			ਧੱਕੋ_task = get_task_struct(p);
		}

		/*
		 * If there are pending waiters, but no pending stop_work,
		 * then complete now.
		 */
		pending = p->migration_pending;
		ਜੇ (pending && !pending->stop_pending) {
			p->migration_pending = ਨਿਰਾ;
			complete = true;
		}

		preempt_disable();
		task_rq_unlock(rq, p, rf);
		ਜੇ (ਧੱਕੋ_task) {
			stop_one_cpu_nowait(rq->cpu, ਧੱਕੋ_cpu_stop,
					    p, &rq->ਧੱਕੋ_work);
		}
		preempt_enable();

		ਜੇ (complete)
			complete_all(&pending->ਪੂਰਾ);

		ਵਾਪਸ 0;
	}

	ਜੇ (!(flags & SCA_MIGRATE_ENABLE)) {
		/* serialized by p->pi_lock */
		ਜੇ (!p->migration_pending) {
			/* Install the request */
			refcount_ਸੈੱਟ(&my_pending.refs, 1);
			init_completion(&my_pending.ਪੂਰਾ);
			my_pending.arg = (ਬਣਤਰ migration_arg) {
				.task = p,
				.dest_cpu = dest_cpu,
				.pending = &my_pending,
			};

			p->migration_pending = &my_pending;
		} ਨਹੀਂਤਾਂ {
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
	ਜੇ (WARN_ON_ONCE(!pending)) {
		task_rq_unlock(rq, p, rf);
		ਵਾਪਸ -EINVAL;
	}

	ਜੇ (task_on_cpu(rq, p) || READ_ONCE(p->__state) == TASK_WAKING) {
		/*
		 * MIGRATE_ENABLE gets here because 'p == current', but for
		 * anything else we cannot do is_migration_disabled(), punt
		 * and have the stopper function handle it all race-free.
		 */
		stop_pending = pending->stop_pending;
		ਜੇ (!stop_pending)
			pending->stop_pending = true;

		ਜੇ (flags & SCA_MIGRATE_ENABLE)
			p->migration_flags &= ~MDF_ਧੱਕੋ;

		preempt_disable();
		task_rq_unlock(rq, p, rf);
		ਜੇ (!stop_pending) {
			stop_one_cpu_nowait(cpu_ਦਾ(rq), migration_cpu_stop,
					    &pending->arg, &pending->stop_work);
		}
		preempt_enable();

		ਜੇ (flags & SCA_MIGRATE_ENABLE)
			ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ {

		ਜੇ (!is_migration_disabled(p)) {
			ਜੇ (task_on_rq_queued(p))
				rq = move_queued_task(rq, rf, p, dest_cpu);

			ਜੇ (!pending->stop_pending) {
				p->migration_pending = ਨਿਰਾ;
				complete = true;
			}
		}
		task_rq_unlock(rq, p, rf);

		ਜੇ (complete)
			complete_all(&pending->ਪੂਰਾ);
	}

	wait_for_completion(&pending->ਪੂਰਾ);

	ਜੇ (refcount_dec_and_test(&pending->refs))
		wake_ਉੱਪਰ_var(&pending->refs); /* No UaF, just an address */

	/*
	 * Block the original owner of &pending until all subsequent callers
	 * have seen the completion and decremented the refcount
	 */
	wait_var_event(&my_pending.refs, !refcount_read(&my_pending.refs));

	/* ARGH */
	WARN_ON_ONCE(my_pending.stop_pending);

	ਵਾਪਸ 0;
}

/*
 * Called with both p->pi_lock and rq->lock held; drops both before returning.
 */
ਸਥਿਰ ਪੂਰਨ __set_cpus_allowed_ptr_locked(ਬਣਤਰ task_struct *p,
					 ਬਣਤਰ affinity_context *ctx,
					 ਬਣਤਰ rq *rq,
					 ਬਣਤਰ rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_ਤਾਲਾ)
{
	ਅਚਲ ਬਣਤਰ cpumask *cpu_allowed_ਨਕਾਬ = task_cpu_possible_ਨਕਾਬ(p);
	ਅਚਲ ਬਣਤਰ cpumask *cpu_valid_ਨਕਾਬ = cpu_ਸਰਗਰਮ_ਨਕਾਬ;
	bool kthread = p->flags & PF_KTHREAD;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ dest_cpu;
	ਪੂਰਨ ret = 0;

	ਜੇ (kthread || is_migration_disabled(p)) {
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
		cpu_valid_ਨਕਾਬ = cpu_online_ਨਕਾਬ;
	}

	ਜੇ (!kthread && !cpumask_subset(ctx->new_ਨਕਾਬ, cpu_allowed_ਨਕਾਬ)) {
		ret = -EINVAL;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/*
	 * Must re-check here, to close a race against __kthread_bind(),
	 * sched_setaffinity() is not guaranteed to observe the flag.
	 */
	ਜੇ ((ctx->flags & SCA_CHECK) && (p->flags & PF_NO_SETAFFINITY)) {
		ret = -EINVAL;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (!(ctx->flags & SCA_MIGRATE_ENABLE)) {
		ਜੇ (cpumask_equal(&p->cpus_ਨਕਾਬ, ctx->new_ਨਕਾਬ)) {
			ਜੇ (ctx->flags & SCA_USER)
				ਅਦਲਾ_ਬਦਲੀ(p->user_cpus_ptr, ctx->user_ਨਕਾਬ);
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		ਜੇ (WARN_ON_ONCE(p == current &&
				 is_migration_disabled(p) &&
				 !cpumask_test_cpu(task_cpu(p), ctx->new_ਨਕਾਬ))) {
			ret = -EBUSY;
			ਜਾਓ ਬਾਹਰਲਾ;
		}
	}

	/*
	 * Picking a ~random cpu helps in cases where we are changing affinity
	 * for groups of tasks (ie. cpuset), so that load balancing is not
	 * immediately required to distribute the tasks within their new mask.
	 */
	dest_cpu = cpumask_any_and_distribute(cpu_valid_ਨਕਾਬ, ctx->new_ਨਕਾਬ);
	ਜੇ (dest_cpu >= nr_cpu_ids) {
		ret = -EINVAL;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	do_ਸੈੱਟ_cpus_allowed(p, ctx);

	ਵਾਪਸ affine_move_task(rq, p, rf, dest_cpu, ctx->flags);

ਬਾਹਰਲਾ:
	task_rq_unlock(rq, p, rf);

	ਵਾਪਸ ret;
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
ਪੂਰਨ __set_cpus_allowed_ptr(ਬਣਤਰ task_struct *p, ਬਣਤਰ affinity_context *ctx)
{
	ਬਣਤਰ rq_flags rf;
	ਬਣਤਰ rq *rq;

	rq = task_rq_ਤਾਲਾ_2(p, &rf);
	/*
	 * Masking should be skipped if SCA_USER or any of the SCA_MIGRATE_*
	 * flags are set.
	 */
	ਜੇ (p->user_cpus_ptr &&
	    !(ctx->flags & (SCA_USER | SCA_MIGRATE_ENABLE | SCA_MIGRATE_DISABLE)) &&
	    cpumask_and(rq->scratch_ਨਕਾਬ, ctx->new_ਨਕਾਬ, p->user_cpus_ptr))
		ctx->new_ਨਕਾਬ = rq->scratch_ਨਕਾਬ;

	ਵਾਪਸ __set_cpus_allowed_ptr_locked(p, ctx, rq, &rf);
}

ਪੂਰਨ ਸੈੱਟ_cpus_allowed_ptr(ਬਣਤਰ task_struct *p, ਅਚਲ ਬਣਤਰ cpumask *new_ਨਕਾਬ)
{
	ਬਣਤਰ affinity_context ac = {
		.new_ਨਕਾਬ  = new_ਨਕਾਬ,
		.flags     = 0,
	};

	ਵਾਪਸ __set_cpus_allowed_ptr(p, &ac);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(ਸੈੱਟ_cpus_allowed_ptr);

/*
 * Change a given task's CPU affinity to the intersection of its current
 * affinity mask and @subset_mask, writing the resulting mask to @new_mask.
 * If user_cpus_ptr is defined, use it as the basis for restricting CPU
 * affinity or use cpu_online_mask instead.
 *
 * If the resulting mask is empty, leave the affinity unchanged and return
 * -EINVAL.
 */
ਸਥਿਰ ਪੂਰਨ restrict_cpus_allowed_ptr(ਬਣਤਰ task_struct *p,
				     ਬਣਤਰ cpumask *new_ਨਕਾਬ,
				     ਅਚਲ ਬਣਤਰ cpumask *subset_ਨਕਾਬ)
{
	ਬਣਤਰ affinity_context ac = {
		.new_ਨਕਾਬ  = new_ਨਕਾਬ,
		.flags     = 0,
	};
	ਬਣਤਰ rq_flags rf;
	ਬਣਤਰ rq *rq;
	ਪੂਰਨ err;

	rq = task_rq_ਤਾਲਾ_2(p, &rf);

	/*
	 * Forcefully restricting the affinity of a deadline task is
	 * likely to cause problems, so fail and noisily override the
	 * mask entirely.
	 */
	ਜੇ (task_has_dl_policy(p) && dl_bandwidth_enabled()) {
		err = -EPERM;
		ਜਾਓ err_unlock;
	}

	ਜੇ (!cpumask_and(new_ਨਕਾਬ, task_user_cpus(p), subset_ਨਕਾਬ)) {
		err = -EINVAL;
		ਜਾਓ err_unlock;
	}

	ਵਾਪਸ __set_cpus_allowed_ptr_locked(p, &ac, rq, &rf);

err_unlock:
	task_rq_unlock(rq, p, &rf);
	ਵਾਪਸ err;
}

/*
 * Restrict the CPU affinity of task @p so that it is a subset of
 * task_cpu_possible_mask() and point @p->user_cpus_ptr to a copy of the
 * old affinity mask. If the resulting mask is empty, we warn and walk
 * up the cpuset hierarchy until we find a suitable mask.
 */
ਖਾਲੀ force_compatible_cpus_allowed_ptr(ਬਣਤਰ task_struct *p)
{
	cpumask_var_t new_ਨਕਾਬ;
	ਅਚਲ ਬਣਤਰ cpumask *override_ਨਕਾਬ = task_cpu_possible_ਨਕਾਬ(p);

	alloc_cpumask_var(&new_ਨਕਾਬ, GFP_KERNEL);

	/*
	 * __migrate_task() can fail silently in the face of concurrent
	 * offlining of the chosen destination CPU, so take the hotplug
	 * lock to ensure that the migration succeeds.
	 */
	cpus_read_ਤਾਲਾ();
	ਜੇ (!cpumask_ਉਪਲਬਧ(new_ਨਕਾਬ))
		ਜਾਓ ਬਾਹਰਲਾ_ਸੈੱਟ_ਨਕਾਬ;

	ਜੇ (!restrict_cpus_allowed_ptr(p, new_ਨਕਾਬ, override_ਨਕਾਬ))
		ਜਾਓ ਬਾਹਰਲਾ_free_ਨਕਾਬ;

	/*
	 * We failed to find a valid subset of the affinity mask for the
	 * task, so override it based on its cpuset hierarchy.
	 */
	cpuset_cpus_allowed(p, new_ਨਕਾਬ);
	override_ਨਕਾਬ = new_ਨਕਾਬ;

ਬਾਹਰਲਾ_ਸੈੱਟ_ਨਕਾਬ:
	ਜੇ (printk_ratelimit()) {
		printk_deferred("Overriding affinity for process %d (%s) to CPUs %*pbl\n",
				task_pid_nr(p), p->comm,
				cpumask_pr_ਦਲੀਲਾਂ(override_ਨਕਾਬ));
	}

	WARN_ON(ਸੈੱਟ_cpus_allowed_ptr(p, override_ਨਕਾਬ));
ਬਾਹਰਲਾ_free_ਨਕਾਬ:
	cpus_read_unlock();
	free_cpumask_var(new_ਨਕਾਬ);
}

/*
 * Restore the affinity of a task @p which was previously restricted by a
 * call to force_compatible_cpus_allowed_ptr().
 *
 * It is the caller's responsibility to serialise this with any calls to
 * force_compatible_cpus_allowed_ptr(@p).
 */
ਖਾਲੀ relax_compatible_cpus_allowed_ptr(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ affinity_context ac = {
		.new_ਨਕਾਬ  = task_user_cpus(p),
		.flags     = 0,
	};
	ਪੂਰਨ ret;

	/*
	 * Try to restore the old affinity mask with __sched_setaffinity().
	 * Cpuset masking will be done there too.
	 */
	ret = __sched_setaffinity(p, &ac);
	WARN_ON_ONCE(ret);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SMP

ਖਾਲੀ ਸੈੱਟ_task_cpu(ਬਣਤਰ task_struct *p, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_cpu)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ state = READ_ONCE(p->__state);
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

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_LOCKDEP
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
	WARN_ON_ONCE(debug_locks && !(lockdep_is_held(&p->pi_ਤਾਲਾ) ||
				      lockdep_is_held(__rq_lockp(task_rq(p)))));
#ਜੇ_ਅੰਤ
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

	ਜੇ (task_cpu(p) != new_cpu) {
		ਜੇ (p->sched_class->migrate_task_rq)
			p->sched_class->migrate_task_rq(p, new_cpu);
		p->se.nr_migrations++;
		perf_event_task_migrate(p);
	}

	__set_task_cpu(p, new_cpu);
}
#ਜੇ_ਅੰਤ /* CONFIG_SMP */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
ਸਥਿਰ ਖਾਲੀ __migrate_swap_task(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu)
{
	ਜੇ (task_on_rq_queued(p)) {
		ਬਣਤਰ rq *src_rq, *dst_rq;
		ਬਣਤਰ rq_flags srf, drf;

		src_rq = task_rq(p);
		dst_rq = cpu_rq(cpu);

		rq_pin_ਤਾਲਾ(src_rq, &srf);
		rq_pin_ਤਾਲਾ(dst_rq, &drf);

		move_queued_task_locked(src_rq, dst_rq, p);
		wakeup_preempt(dst_rq, p, 0);

		rq_unpin_ਤਾਲਾ(dst_rq, &drf);
		rq_unpin_ਤਾਲਾ(src_rq, &srf);

	} ਨਹੀਂਤਾਂ {
		/*
		 * Task isn't running anymore; make it appear like we migrated
		 * it before it went to sleep. This means on wakeup we make the
		 * previous CPU our target instead of where it really is.
		 */
		p->wake_cpu = cpu;
	}
}

ਬਣਤਰ migration_ਅਦਲਾ_ਬਦਲੀ_arg {
	ਬਣਤਰ task_struct *src_task, *dst_task;
	ਪੂਰਨ src_cpu, dst_cpu;
};

ਸਥਿਰ ਪੂਰਨ migrate_ਅਦਲਾ_ਬਦਲੀ_stop(ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ migration_ਅਦਲਾ_ਬਦਲੀ_arg *arg = ਅੰਕੜੇ;
	ਬਣਤਰ rq *src_rq, *dst_rq;

	ਜੇ (!cpu_ਸਰਗਰਮ(arg->src_cpu) || !cpu_ਸਰਗਰਮ(arg->dst_cpu))
		ਵਾਪਸ -EAGAIN;

	src_rq = cpu_rq(arg->src_cpu);
	dst_rq = cpu_rq(arg->dst_cpu);

	guard(double_raw_spinlock)(&arg->src_task->pi_ਤਾਲਾ, &arg->dst_task->pi_ਤਾਲਾ);
	guard(double_rq_ਤਾਲਾ)(src_rq, dst_rq);

	ਜੇ (task_cpu(arg->dst_task) != arg->dst_cpu)
		ਵਾਪਸ -EAGAIN;

	ਜੇ (task_cpu(arg->src_task) != arg->src_cpu)
		ਵਾਪਸ -EAGAIN;

	ਜੇ (!cpumask_test_cpu(arg->dst_cpu, arg->src_task->cpus_ptr))
		ਵਾਪਸ -EAGAIN;

	ਜੇ (!cpumask_test_cpu(arg->src_cpu, arg->dst_task->cpus_ptr))
		ਵਾਪਸ -EAGAIN;

	__migrate_swap_task(arg->src_task, arg->dst_cpu);
	__migrate_swap_task(arg->dst_task, arg->src_cpu);

	ਵਾਪਸ 0;
}

/*
 * Cross migrate two tasks
 */
ਪੂਰਨ migrate_ਅਦਲਾ_ਬਦਲੀ(ਬਣਤਰ task_struct *cur, ਬਣਤਰ task_struct *p,
		ਪੂਰਨ ਨਿਸ਼ਾਨਾ_cpu, ਪੂਰਨ curr_cpu)
{
	ਬਣਤਰ migration_ਅਦਲਾ_ਬਦਲੀ_arg arg;
	ਪੂਰਨ ret = -EINVAL;

	arg = (ਬਣਤਰ migration_ਅਦਲਾ_ਬਦਲੀ_arg){
		.src_task = cur,
		.src_cpu = curr_cpu,
		.dst_task = p,
		.dst_cpu = ਨਿਸ਼ਾਨਾ_cpu,
	};

	ਜੇ (arg.src_cpu == arg.dst_cpu)
		ਜਾਓ ਬਾਹਰਲਾ;

	/*
	 * These three tests are all lockless; this is OK since all of them
	 * will be re-checked with proper locks held further down the line.
	 */
	ਜੇ (!cpu_ਸਰਗਰਮ(arg.src_cpu) || !cpu_ਸਰਗਰਮ(arg.dst_cpu))
		ਜਾਓ ਬਾਹਰਲਾ;

	ਜੇ (!cpumask_test_cpu(arg.dst_cpu, arg.src_task->cpus_ptr))
		ਜਾਓ ਬਾਹਰਲਾ;

	ਜੇ (!cpumask_test_cpu(arg.src_cpu, arg.dst_task->cpus_ptr))
		ਜਾਓ ਬਾਹਰਲਾ;

	trace_sched_ਅਦਲਾ_ਬਦਲੀ_numa(cur, arg.src_cpu, p, arg.dst_cpu);
	ret = stop_two_cpus(arg.dst_cpu, arg.src_cpu, migrate_ਅਦਲਾ_ਬਦਲੀ_stop, &arg);

ਬਾਹਰਲਾ:
	ਵਾਪਸ ret;
}
#ਜੇ_ਅੰਤ /* CONFIG_NUMA_BALANCING */

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
ਖਾਲੀ kick_process(ਬਣਤਰ task_struct *p)
{
	guard(preempt)();
	ਪੂਰਨ cpu = task_cpu(p);

	ਜੇ ((cpu != smp_processor_id()) && task_curr(p))
		smp_send_reschedule(cpu);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(kick_process);

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
ਸਥਿਰ ਪੂਰਨ select_fallback_rq(ਪੂਰਨ cpu, ਬਣਤਰ task_struct *p)
{
	ਪੂਰਨ nid = cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(cpu);
	ਅਚਲ ਬਣਤਰ cpumask *nodemask = ਨਿਰਾ;
	ਗਿਣਤੀ { cpuset, possible, fail } state = cpuset;
	ਪੂਰਨ dest_cpu;

	/*
	 * If the node that the CPU is on has been offlined, cpu_to_node()
	 * will return -1. There is no CPU on the node, and we should
	 * select the CPU on the other node.
	 */
	ਜੇ (nid != -1) {
		nodemask = cpumask_ਦਾ_ਗੰਢ(nid);

		/* Look for allowed, online CPU in same node. */
		for_each_cpu(dest_cpu, nodemask) {
			ਜੇ (is_cpu_allowed(p, dest_cpu))
				ਵਾਪਸ dest_cpu;
		}
	}

	ਲਈ (;;) {
		/* Any allowed, online CPU? */
		for_each_cpu(dest_cpu, p->cpus_ptr) {
			ਜੇ (!is_cpu_allowed(p, dest_cpu))
				ਜਾਰੀ;

			ਜਾਓ ਬਾਹਰਲਾ;
		}

		/* No more Mr. Nice Guy. */
		ਚੋਣ (state) {
		ਹਾਲਤ cpuset:
			ਜੇ (cpuset_cpus_allowed_fallback(p)) {
				state = possible;
				ਰੋਕੋ;
			}
			fallthrough;
		ਹਾਲਤ possible:
			ਸੈੱਟ_cpus_allowed_force(p, task_cpu_fallback_ਨਕਾਬ(p));
			state = fail;
			ਰੋਕੋ;
		ਹਾਲਤ fail:
			BUG();
			ਰੋਕੋ;
		}
	}

ਬਾਹਰਲਾ:
	ਜੇ (state != cpuset) {
		/*
		 * Don't tell them about moving exiting tasks or
		 * kernel threads (both mm NULL), since they never
		 * leave kernel.
		 */
		ਜੇ (p->mm && printk_ratelimit()) {
			printk_deferred("process %d (%s) no longer affine to cpu%d\n",
					task_pid_nr(p), p->comm, cpu);
		}
	}

	ਵਾਪਸ dest_cpu;
}

/*
 * The caller (fork, wakeup) owns p->pi_lock, ->cpus_ptr is stable.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ
ਪੂਰਨ select_task_rq(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu, ਪੂਰਨ *wake_flags)
{
	lockdep_assert_held(&p->pi_ਤਾਲਾ);

	ਜੇ (p->nr_cpus_allowed > 1 && !is_migration_disabled(p)) {
		cpu = p->sched_class->select_task_rq(p, cpu, *wake_flags);
		*wake_flags |= WF_RQ_SELECTED;
	} ਨਹੀਂਤਾਂ {
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
	ਜੇ (unlikely(!is_cpu_allowed(p, cpu)))
		cpu = select_fallback_rq(task_cpu(p), p);

	ਵਾਪਸ cpu;
}

ਖਾਲੀ sched_ਸੈੱਟ_stop_task(ਪੂਰਨ cpu, ਬਣਤਰ task_struct *stop)
{
	ਸਥਿਰ ਬਣਤਰ ਤਾਲਾ_class_ਕੁੰਜੀ stop_pi_ਤਾਲਾ;
	ਬਣਤਰ sched_param param = { .sched_ਤਰਜੀਹ = MAX_RT_PRIO - 1 };
	ਬਣਤਰ task_struct *old_stop = cpu_rq(cpu)->stop;

	ਜੇ (stop) {
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
		lockdep_ਸੈੱਟ_class(&stop->pi_ਤਾਲਾ, &stop_pi_ਤਾਲਾ);
	}

	cpu_rq(cpu)->stop = stop;

	ਜੇ (old_stop) {
		/*
		 * Reset it back to a normal scheduling class so that
		 * it can die in pieces.
		 */
		old_stop->sched_class = &rt_sched_class;
	}
}

ਸਥਿਰ ਖਾਲੀ
ttwu_stat(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu, ਪੂਰਨ wake_flags)
{
	ਬਣਤਰ rq *rq;

	ਜੇ (!schedstat_enabled())
		ਵਾਪਸ;

	rq = this_rq();

	ਜੇ (cpu == rq->cpu) {
		__schedstat_inc(rq->ttwu_local);
		__schedstat_inc(p->stats.nr_wakeups_local);
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ sched_domain *sd;

		__schedstat_inc(p->stats.nr_wakeups_remote);

		guard(rcu)();
		for_each_domain(rq->cpu, sd) {
			ਜੇ (cpumask_test_cpu(cpu, sched_domain_ਵਿਸਤਾਰ(sd))) {
				__schedstat_inc(sd->ttwu_wake_remote);
				ਰੋਕੋ;
			}
		}
	}

	ਜੇ (wake_flags & WF_MIGRATED)
		__schedstat_inc(p->stats.nr_wakeups_migrate);

	__schedstat_inc(rq->ttwu_ਗਿਣਤੀ);
	__schedstat_inc(p->stats.nr_wakeups);

	ਜੇ (wake_flags & WF_SYNC)
		__schedstat_inc(p->stats.nr_wakeups_sync);
}

/*
 * Mark the task runnable.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ttwu_do_wakeup(ਬਣਤਰ task_struct *p)
{
	p->is_blocked = 0;
	WRITE_ONCE(p->__state, TASK_RUNNING);
	trace_sched_wakeup(p);
}

ਖਾਲੀ update_rq_avg_idle(ਬਣਤਰ rq *rq)
{
	u64 idle_stamp = rq->idle_stamp;
	u64 delta, max;

	ਜੇ (!idle_stamp)
		ਵਾਪਸ;

	delta = rq_clock(rq) - idle_stamp;

	update_avg(&rq->avg_idle, delta);

	max = 2 * rq->max_idle_balance_ਲਾਗਤ;
	ਜੇ (rq->avg_idle > max)
		rq->avg_idle = max;
	rq->idle_stamp = 0;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_PROXY_EXEC
ਸਥਿਰ ਖਾਲੀ zap_balance_callbacks(ਬਣਤਰ rq *rq);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ proxy_reset_donor(ਬਣਤਰ rq *rq)
{
	WARN_ON_ONCE(rq->donor == rq->curr);

	put_prev_ਸੈੱਟ_ਅਗਲਾ_task(rq, rq->donor, rq->curr);
	rq->ਅਗਲਾ_class = rq->curr->sched_class;
	rq_ਸੈੱਟ_donor(rq, rq->curr);
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
ਸਥਿਰ ਸਤਰਅੰਦਰ bool proxy_needs_return(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
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
	ਜੇ (task_cpu(p) == p->wake_cpu)
		ਵਾਪਸ false;

	scoped_guard(raw_spinlock, &p->blocked_ਤਾਲਾ) {
		/* Task is waking up; clear any blocked_on relationship */
		__clear_task_blocked_on(p, ਨਿਰਾ);

		/* If already current, don't need to return migrate */
		ਜੇ (task_current(rq, p))
			ਵਾਪਸ false;
	}

	dequeued = ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_ਖੰਡ_task(rq, p, TASK_WAKING);

	/*
	 * Dequeue @p from its scheduling class before resetting rq->donor.
	 * In particular, sched_ext needs to end the donor's running session
	 * and clear SCX_TASK_QUEUED before put_prev_task_scx() is called by
	 * proxy_reset_donor(); otherwise it would reenqueue the blocked donor.
	 *
	 * Keep on_rq set until all donor references have been replaced.
	 */
	ਜੇ (task_current_donor(rq, p))
		proxy_reset_donor(rq);

	ਜੇ (dequeued)
		__block_task(rq, p);
	ਵਾਪਸ true;
}
#ਨਹੀਂਤਾਂ /* !CONFIG_SCHED_PROXY_EXEC */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool proxy_needs_return(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ false;
}
#ਜੇ_ਅੰਤ /* CONFIG_SCHED_PROXY_EXEC */

ਸਥਿਰ ਖਾਲੀ
ttwu_do_activate(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ wake_flags,
		 ਬਣਤਰ rq_flags *rf)
{
	ਪੂਰਨ en_flags = ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_WAKEUP | ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_NOCLOCK;

	lockdep_assert_rq_held(rq);

	ਜੇ (p->sched_contributes_ਮੰਜ਼ਿਲ_ਲੱਦੋ)
		rq->nr_uninterruptible--;

	ਜੇ (wake_flags & WF_RQ_SELECTED)
		en_flags |= ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_RQ_SELECTED;
	ਜੇ (wake_flags & WF_MIGRATED)
		en_flags |= ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_MIGRATED;
	ਨਹੀਂਤਾਂ ਜੇ (p->ਵਿੱਚ_iowait) {
		delayacct_blkio_ਸਿਰਾ(p);
		atomic_dec(&task_rq(p)->nr_iowait);
	}

	activate_task(rq, p, en_flags);
	wakeup_preempt(rq, p, wake_flags);

	ttwu_do_wakeup(p);

	ਜੇ (p->sched_class->task_woken) {
		/*
		 * Our task @p is fully woken up and running; so it's safe to
		 * drop the rq->lock, hereafter rq is only used for statistics.
		 */
		rq_unpin_ਤਾਲਾ(rq, rf);
		p->sched_class->task_woken(rq, p);
		rq_repin_ਤਾਲਾ(rq, rf);
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
ਸਥਿਰ ਪੂਰਨ ttwu_runnable(ਬਣਤਰ task_struct *p, ਪੂਰਨ wake_flags)
{
	ACQUIRE(__task_rq_lock, guard)(p);
	ਬਣਤਰ rq *rq = guard.rq;

	ਜੇ (!task_on_rq_queued(p))
		ਵਾਪਸ 0;

	update_rq_clock(rq);
	ਜੇ (p->is_blocked) {
		ਜੇ (p->se.sched_delayed)
			ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task(rq, p, ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_NOCLOCK | ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_DELAYED);
		ਜੇ (proxy_needs_return(rq, p))
			ਵਾਪਸ 0;
	}
	ਜੇ (!task_on_cpu(rq, p)) {
		/*
		 * When on_rq && !on_cpu the task is preempted, see if
		 * it should preempt the task that is current now.
		 */
		wakeup_preempt(rq, p, wake_flags | WF_TTWU_RQ);
	}
	ttwu_do_wakeup(p);
	ਵਾਪਸ 1;
}

ਖਾਲੀ sched_ttwu_pending(ਖਾਲੀ *arg)
{
	ਬਣਤਰ llist_ਗੰਢ *llist = arg;
	ਬਣਤਰ rq *rq = this_rq();
	ਬਣਤਰ task_struct *p, *t;
	ਬਣਤਰ rq_flags rf;

	ਜੇ (!llist)
		ਵਾਪਸ;

	rq_ਤਾਲਾ_irqsave(rq, &rf);
	update_rq_clock(rq);

	llist_for_each_ਇੰਦਰਾਜ_ਸੁਰੱਖਿਅਤ(p, t, llist, wake_ਇੰਦਰਾਜ.llist) {
		ਜੇ (WARN_ON_ONCE(p->on_cpu))
			smp_cond_ਲੱਦੋ_acquire(&p->on_cpu, !VAL);

		ਜੇ (WARN_ON_ONCE(task_cpu(p) != cpu_ਦਾ(rq)))
			ਸੈੱਟ_task_cpu(p, cpu_ਦਾ(rq));

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
bool call_function_single_prep_ipi(ਪੂਰਨ cpu)
{
	ਜੇ (ਸੈੱਟ_nr_if_polling(cpu_rq(cpu)->idle)) {
		trace_sched_wake_idle_without_ipi(cpu);
		ਵਾਪਸ false;
	}

	ਵਾਪਸ true;
}

/*
 * Queue a task on the target CPUs wake_list and wake the CPU via IPI if
 * necessary. The wakee CPU on receipt of the IPI will queue the task
 * via sched_ttwu_wakeup() for activation so the wakee incurs the cost
 * of the wakeup instead of the waker.
 */
ਸਥਿਰ ਖਾਲੀ __ttwu_queue_wakelist(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu, ਪੂਰਨ wake_flags)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);

	p->sched_remote_wakeup = !!(wake_flags & WF_MIGRATED);

	WRITE_ONCE(rq->ttwu_pending, 1);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SMP
	__smp_call_single_queue(cpu, &p->wake_ਇੰਦਰਾਜ.llist);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ wake_ਉੱਪਰ_if_idle(ਪੂਰਨ cpu)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);

	guard(rcu)();
	ਜੇ (is_idle_task(rcu_dereference(rq->curr))) {
		guard(rq_ਤਾਲਾ_irqsave)(rq);
		ਜੇ (is_idle_task(rq->curr))
			resched_curr(rq);
	}
}

bool cpus_equal_ਸਮਰੱਥਾ(ਪੂਰਨ this_cpu, ਪੂਰਨ that_cpu)
{
	ਜੇ (!sched_asym_cpucap_ਸਰਗਰਮ())
		ਵਾਪਸ true;

	ਜੇ (this_cpu == that_cpu)
		ਵਾਪਸ true;

	ਵਾਪਸ arch_scale_cpu_ਸਮਰੱਥਾ(this_cpu) == arch_scale_cpu_ਸਮਰੱਥਾ(that_cpu);
}

bool cpus_share_cache(ਪੂਰਨ this_cpu, ਪੂਰਨ that_cpu)
{
	ਜੇ (this_cpu == that_cpu)
		ਵਾਪਸ true;

	ਵਾਪਸ ਪ੍ਰਤੀ_cpu(sd_llc_id, this_cpu) == ਪ੍ਰਤੀ_cpu(sd_llc_id, that_cpu);
}

/*
 * Whether CPUs are share cache resources, which means LLC on non-cluster
 * machines and LLC tag or L2 on machines with clusters.
 */
bool cpus_share_resources(ਪੂਰਨ this_cpu, ਪੂਰਨ that_cpu)
{
	ਜੇ (this_cpu == that_cpu)
		ਵਾਪਸ true;

	ਵਾਪਸ ਪ੍ਰਤੀ_cpu(sd_share_id, this_cpu) == ਪ੍ਰਤੀ_cpu(sd_share_id, that_cpu);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool ttwu_ਕਤਾਰ_cond(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu)
{
	ਪੂਰਨ this_cpu = smp_processor_id();

	/* See SCX_OPS_ALLOW_QUEUED_WAKEUP. */
	ਜੇ (!scx_allow_ttwu_ਕਤਾਰ(p))
		ਵਾਪਸ false;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SMP
	ਜੇ (p->sched_class == &stop_sched_class)
		ਵਾਪਸ false;
#ਜੇ_ਅੰਤ

	/*
	 * Do not complicate things with the async wake_list while the CPU is
	 * in hotplug state.
	 */
	ਜੇ (!cpu_ਸਰਗਰਮ(cpu))
		ਵਾਪਸ false;

	/* Ensure the task will still be allowed to run on the CPU. */
	ਜੇ (!cpumask_test_cpu(cpu, p->cpus_ptr))
		ਵਾਪਸ false;

	/*
	 * If the CPU does not share cache, then queue the task on the
	 * remote rqs wakelist to avoid accessing remote data.
	 */
	ਜੇ (!cpus_share_cache(this_cpu, cpu))
		ਵਾਪਸ true;

	ਜੇ (cpu == this_cpu)
		ਵਾਪਸ false;

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
	ਜੇ (!cpu_rq(cpu)->nr_running)
		ਵਾਪਸ true;

	ਵਾਪਸ false;
}

ਸਥਿਰ bool ttwu_ਕਤਾਰ_wakelist(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu, ਪੂਰਨ wake_flags)
{
	ਜੇ (sched_feat(TTWU_ਕਤਾਰ) && ttwu_ਕਤਾਰ_cond(p, cpu)) {
		sched_clock_cpu(cpu); /* Sync clocks across CPUs */
		__ttwu_queue_wakelist(p, cpu, wake_flags);
		ਵਾਪਸ true;
	}

	ਵਾਪਸ false;
}

ਸਥਿਰ ਖਾਲੀ ttwu_ਕਤਾਰ(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu, ਪੂਰਨ wake_flags)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);
	ਬਣਤਰ rq_flags rf;

	ਜੇ (ttwu_ਕਤਾਰ_wakelist(p, cpu, wake_flags))
		ਵਾਪਸ;

	rq_ਤਾਲਾ(rq, &rf);
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
ਸਥਿਰ __always_inline
bool ttwu_state_ਮੇਲ(ਬਣਤਰ task_struct *p, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ state, ਪੂਰਨ *success)
{
	ਪੂਰਨ ਮੇਲ;

	ਜੇ (IS_ENABLED(CONFIG_DEBUG_PREEMPT)) {
		WARN_ON_ONCE((state & TASK_RTLOCK_WAIT) &&
			     state != TASK_RTLOCK_WAIT);
	}

	*success = !!(ਮੇਲ = __task_state_match(p, state));

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
	ਜੇ (ਮੇਲ < 0)
		p->saved_state = TASK_RUNNING;

	ਵਾਪਸ ਮੇਲ > 0;
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
ਪੂਰਨ try_ਮੰਜ਼ਿਲ_wake_ਉੱਪਰ(ਬਣਤਰ task_struct *p, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ state, ਪੂਰਨ wake_flags)
{
	guard(preempt)();
	ਪੂਰਨ cpu, success = 0;

	wake_flags |= WF_TTWU;

	ਜੇ (p == current) {
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
		clear_task_blocked_on(p, ਨਿਰਾ);
		ਜੇ (!ttwu_state_ਮੇਲ(p, state, &success))
			ਜਾਓ ਬਾਹਰਲਾ;

		trace_sched_waking(p);
		ttwu_do_wakeup(p);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/*
	 * If we are going to wake up a thread waiting for CONDITION we
	 * need to ensure that CONDITION=1 done by the caller can not be
	 * reordered with p->state check below. This pairs with smp_store_mb()
	 * in set_current_state() that the waiting thread does.
	 */
	scoped_guard (raw_spinlock_irqsave, &p->pi_ਤਾਲਾ) {
		smp_mb__after_spinlock();

		ਜੇ (!ttwu_state_ਮੇਲ(p, state, &success))
			ਰੋਕੋ;

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
		ਜੇ (READ_ONCE(p->on_rq) && ttwu_runnable(p, wake_flags))
			ਰੋਕੋ;

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
		ਜੇ (smp_ਲੱਦੋ_acquire(&p->on_cpu) &&
		    ttwu_ਕਤਾਰ_wakelist(p, task_cpu(p), wake_flags))
			ਰੋਕੋ;

		/*
		 * If the owning (remote) CPU is still in the middle of schedule() with
		 * this task as prev, wait until it's done referencing the task.
		 *
		 * Pairs with the smp_store_release() in finish_task().
		 *
		 * This ensures that tasks getting woken will be fully ordered against
		 * their previous state and preserve Program Order.
		 */
		smp_cond_ਲੱਦੋ_acquire(&p->on_cpu, !VAL);

		cpu = select_task_rq(p, p->wake_cpu, &wake_flags);
		ਜੇ (task_cpu(p) != cpu) {
			ਜੇ (p->ਵਿੱਚ_iowait) {
				delayacct_blkio_ਸਿਰਾ(p);
				atomic_dec(&task_rq(p)->nr_iowait);
			}

			wake_flags |= WF_MIGRATED;
			psi_ttwu_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(p);
			ਸੈੱਟ_task_cpu(p, cpu);
		} ਨਹੀਂਤਾਂ ਜੇ (cpu != p->wake_cpu) {
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

		ttwu_ਕਤਾਰ(p, cpu, wake_flags);
	}
ਬਾਹਰਲਾ:
	ਜੇ (success)
		ttwu_stat(p, task_cpu(p), wake_flags);

	ਵਾਪਸ success;
}

ਸਥਿਰ bool __task_needs_rq_lock(ਬਣਤਰ task_struct *p)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ state = READ_ONCE(p->__state);

	/*
	 * Since pi->lock blocks try_to_wake_up(), we don't need rq->lock when
	 * the task is blocked. Make sure to check @state since ttwu() can drop
	 * locks at the end, see ttwu_queue_wakelist().
	 */
	ਜੇ (state == TASK_RUNNING || state == TASK_WAKING)
		ਵਾਪਸ true;

	/*
	 * Ensure we load p->on_rq after p->__state, otherwise it would be
	 * possible to, falsely, observe p->on_rq == 0.
	 *
	 * See try_to_wake_up() for a longer comment.
	 */
	smp_rmb();
	ਜੇ (p->on_rq)
		ਵਾਪਸ true;

	/*
	 * Ensure the task has finished __schedule() and will not be referenced
	 * anymore. Again, see try_to_wake_up() for a longer comment.
	 */
	smp_rmb();
	smp_cond_ਲੱਦੋ_acquire(&p->on_cpu, !VAL);

	ਵਾਪਸ false;
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
ਪੂਰਨ task_call_func(ਬਣਤਰ task_struct *p, task_call_f func, ਖਾਲੀ *arg)
{
	ਬਣਤਰ rq_flags rf;
	ਪੂਰਨ ret;

	raw_spin_ਤਾਲਾ_irqsave(&p->pi_ਤਾਲਾ, rf.flags);

	ਜੇ (__task_needs_rq_lock(p)) {
		ਬਣਤਰ rq *rq = __task_rq_lock(p, &rf);

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
	} ਨਹੀਂਤਾਂ {
		ret = func(p, arg);
	}

	raw_spin_unlock_irqrestore(&p->pi_ਤਾਲਾ, rf.flags);
	ਵਾਪਸ ret;
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
ਬਣਤਰ task_struct *cpu_curr_snapshot(ਪੂਰਨ cpu)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);
	ਬਣਤਰ task_struct *t;
	ਬਣਤਰ rq_flags rf;

	rq_ਤਾਲਾ_irqsave(rq, &rf);
	smp_mb__after_spinlock(); /* Pairing determined by caller's synchronization design. */
	t = rcu_dereference(cpu_curr(cpu));
	rq_unlock_irqrestore(rq, &rf);
	smp_mb(); /* Pairing determined by caller's synchronization design. */

	ਵਾਪਸ t;
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
ਪੂਰਨ wake_ਉੱਪਰ_process(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ try_ਮੰਜ਼ਿਲ_wake_ਉੱਪਰ(p, TASK_ਸਾਧਾਰਨ, 0);
}
EXPORT_ਚਿੰਨ੍ਹ(wake_ਉੱਪਰ_process);

ਪੂਰਨ wake_ਉੱਪਰ_state(ਬਣਤਰ task_struct *p, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ state)
{
	ਵਾਪਸ try_ਮੰਜ਼ਿਲ_wake_ਉੱਪਰ(p, state, 0);
}

/*
 * Perform scheduler related setup for a newly forked process p.
 * p is forked by current.
 *
 * __sched_fork() is basic setup which is also used by sched_init() to
 * initialize the boot CPU's idle task.
 */
ਸਥਿਰ ਖਾਲੀ __sched_fork(u64 clone_flags, ਬਣਤਰ task_struct *p)
{
	p->on_rq			= 0;

	p->se.on_rq			= 0;
	p->se.exec_ਸ਼ੁਰੂ		= 0;
	p->se.ਜੋੜ_exec_runtime		= 0;
	p->se.prev_ਜੋੜ_exec_runtime	= 0;
	p->se.nr_migrations		= 0;
	p->se.vruntime			= 0;
	p->se.vlag			= 0;
	p->se.rel_deadline		= 0;
	INIT_LIST_ਸਿਰ(&p->se.ਸਮੂਹ_ਗੰਢ);

	/* A delayed task cannot be in clone(). */
	WARN_ON_ONCE(p->se.sched_delayed);
	WARN_ON_ONCE(p->is_blocked);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
	p->se.cfs_rq			= ਨਿਰਾ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CFS_BANDWIDTH
	init_cfs_throttle_work(p);
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHEDSTATS
	/* Even if schedstat is disabled, there should not be garbage */
	memset(&p->stats, 0, ਆਕਾਰ(p->stats));
#ਜੇ_ਅੰਤ

	init_dl_entity(&p->dl);

	INIT_LIST_ਸਿਰ(&p->rt.ਲੜੀਵਾਰ_list);
	p->rt.timeout		= 0;
	p->rt.ਸਮਾਂ_ਟੁਕੜਾ	= sched_rr_timeslice;
	p->rt.on_rq		= 0;
	p->rt.on_list		= 0;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CLASS_EXT
	init_scx_entity(&p->scx);
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PREEMPT_NOTIFIERS
	INIT_HLIST_ਸਿਰ(&p->preempt_notifiers);
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_COMPACTION
	p->capture_control = ਨਿਰਾ;
#ਜੇ_ਅੰਤ
	init_numa_balancing(clone_flags, p);
	p->wake_ਇੰਦਰਾਜ.u_flags = CSD_TYPE_TTWU;
	p->migration_pending = ਨਿਰਾ;
	init_sched_mm(p);
}

DEFINE_STATIC_ਕੁੰਜੀ_FALSE(sched_numa_balancing);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING

ਪੂਰਨ sysctl_numa_balancing_ਬਹੁਲਕ;

ਸਥਿਰ ਖਾਲੀ __set_numabalancing_state(bool enabled)
{
	ਜੇ (enabled)
		static_branch_enable(&sched_numa_balancing);
	ਨਹੀਂਤਾਂ
		static_branch_disable(&sched_numa_balancing);
}

ਖਾਲੀ ਸੈੱਟ_numabalancing_state(bool enabled)
{
	ਜੇ (enabled)
		sysctl_numa_balancing_ਬਹੁਲਕ = NUMA_BALANCING_ਸਾਧਾਰਨ;
	ਨਹੀਂਤਾਂ
		sysctl_numa_balancing_ਬਹੁਲਕ = NUMA_BALANCING_DISABLED;
	__set_numabalancing_state(enabled);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SYSCTL
ਸਥਿਰ ਖਾਲੀ reset_ਸਮ੍ਰਿਤੀ_tiering(ਖਾਲੀ)
{
	ਬਣਤਰ pglist_ਅੰਕੜੇ *pgdat;

	for_each_online_pgdat(pgdat) {
		pgdat->nbp_ਹੱਦ = 0;
		pgdat->nbp_th_nr_cand = ਗੰਢ_ਪੰਨਾ_state(pgdat, PGPROMOTE_CANDIDATE);
		pgdat->nbp_th_ਸ਼ੁਰੂ = jiffies_ਮੰਜ਼ਿਲ_msecs(jiffies);
	}
}

ਸਥਿਰ ਪੂਰਨ sysctl_numa_balancing(ਅਚਲ ਬਣਤਰ ctl_ਸਾਰਣੀ *ਸਾਰਣੀ, ਪੂਰਨ ਲਿਖ,
			  ਖਾਲੀ *ਬਫਰ, ਮਾਪ_t *lenp, loff_t *ppos)
{
	ਬਣਤਰ ctl_ਸਾਰਣੀ t;
	ਪੂਰਨ err;
	ਪੂਰਨ state = sysctl_numa_balancing_ਬਹੁਲਕ;

	ਜੇ (ਲਿਖ && !capable(CAP_SYS_ADMIN))
		ਵਾਪਸ -EPERM;

	t = *ਸਾਰਣੀ;
	t.ਅੰਕੜੇ = &state;
	err = proc_dointvec_minmax(&t, ਲਿਖ, ਬਫਰ, lenp, ppos);
	ਜੇ (err < 0)
		ਵਾਪਸ err;
	ਜੇ (ਲਿਖ) {
		ਜੇ (!(sysctl_numa_balancing_ਬਹੁਲਕ & NUMA_BALANCING_ਸਮ੍ਰਿਤੀ_TIERING) &&
		    (state & NUMA_BALANCING_ਸਮ੍ਰਿਤੀ_TIERING))
			reset_ਸਮ੍ਰਿਤੀ_tiering();
		sysctl_numa_balancing_ਬਹੁਲਕ = state;
		__set_numabalancing_state(state);
	}
	ਵਾਪਸ err;
}
#ਜੇ_ਅੰਤ /* CONFIG_SYSCTL */
#ਜੇ_ਅੰਤ /* CONFIG_NUMA_BALANCING */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHEDSTATS

DEFINE_STATIC_ਕੁੰਜੀ_FALSE(sched_schedstats);

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_schedstats(bool enabled)
{
	ਜੇ (enabled)
		static_branch_enable(&sched_schedstats);
	ਨਹੀਂਤਾਂ
		static_branch_disable(&sched_schedstats);
}

ਖਾਲੀ force_schedstat_enabled(ਖਾਲੀ)
{
	ਜੇ (!schedstat_enabled()) {
		pr_info("kernel profiling enabled schedstats, disable via kernel.sched_schedstats.\n");
		static_branch_enable(&sched_schedstats);
	}
}

ਸਥਿਰ ਪੂਰਨ __init ਤਿਆਰੀ_schedstats(ਅੱਖਰ *str)
{
	ਪੂਰਨ ret = 0;
	ਜੇ (!str)
		ਜਾਓ ਬਾਹਰਲਾ;

	ਜੇ (!strcmp(str, "enable")) {
		ਸੈੱਟ_schedstats(true);
		ret = 1;
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(str, "disable")) {
		ਸੈੱਟ_schedstats(false);
		ret = 1;
	}
ਬਾਹਰਲਾ:
	ਜੇ (!ret)
		pr_warn("Unable to parse schedstats=\n");

	ਵਾਪਸ ret;
}
__setup("schedstats=", ਤਿਆਰੀ_schedstats);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SYSCTL
ਸਥਿਰ ਪੂਰਨ sysctl_schedstats(ਅਚਲ ਬਣਤਰ ctl_ਸਾਰਣੀ *ਸਾਰਣੀ, ਪੂਰਨ ਲਿਖ, ਖਾਲੀ *ਬਫਰ,
		ਮਾਪ_t *lenp, loff_t *ppos)
{
	ਬਣਤਰ ctl_ਸਾਰਣੀ t;
	ਪੂਰਨ err;
	ਪੂਰਨ state = static_branch_likely(&sched_schedstats);

	ਜੇ (ਲਿਖ && !capable(CAP_SYS_ADMIN))
		ਵਾਪਸ -EPERM;

	t = *ਸਾਰਣੀ;
	t.ਅੰਕੜੇ = &state;
	err = proc_dointvec_minmax(&t, ਲਿਖ, ਬਫਰ, lenp, ppos);
	ਜੇ (err < 0)
		ਵਾਪਸ err;
	ਜੇ (ਲਿਖ)
		ਸੈੱਟ_schedstats(state);
	ਵਾਪਸ err;
}
#ਜੇ_ਅੰਤ /* CONFIG_SYSCTL */
#ਜੇ_ਅੰਤ /* CONFIG_SCHEDSTATS */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SYSCTL
ਸਥਿਰ ਅਚਲ ਬਣਤਰ ctl_ਸਾਰਣੀ sched_core_sysctls[] = {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHEDSTATS
	{
		.procname       = "sched_schedstats",
		.ਅੰਕੜੇ           = ਨਿਰਾ,
		.maxlen         = ਆਕਾਰ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ),
		.ਬਹੁਲਕ           = 0644,
		.proc_handler   = sysctl_schedstats,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
#ਜੇ_ਅੰਤ /* CONFIG_SCHEDSTATS */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK
	{
		.procname       = "sched_util_clamp_min",
		.ਅੰਕੜੇ           = &sysctl_sched_uclamp_util_min,
		.maxlen         = ਆਕਾਰ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ),
		.ਬਹੁਲਕ           = 0644,
		.proc_handler   = sysctl_sched_uclamp_handler,
	},
	{
		.procname       = "sched_util_clamp_max",
		.ਅੰਕੜੇ           = &sysctl_sched_uclamp_util_max,
		.maxlen         = ਆਕਾਰ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ),
		.ਬਹੁਲਕ           = 0644,
		.proc_handler   = sysctl_sched_uclamp_handler,
	},
	{
		.procname       = "sched_util_clamp_min_rt_default",
		.ਅੰਕੜੇ           = &sysctl_sched_uclamp_util_min_rt_default,
		.maxlen         = ਆਕਾਰ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ),
		.ਬਹੁਲਕ           = 0644,
		.proc_handler   = sysctl_sched_uclamp_handler,
	},
#ਜੇ_ਅੰਤ /* CONFIG_UCLAMP_TASK */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
	{
		.procname	= "numa_balancing",
		.ਅੰਕੜੇ		= ਨਿਰਾ, /* filled in by handler */
		.maxlen		= ਆਕਾਰ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ),
		.ਬਹੁਲਕ		= 0644,
		.proc_handler	= sysctl_numa_balancing,
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_FOUR,
	},
#ਜੇ_ਅੰਤ /* CONFIG_NUMA_BALANCING */
};
ਸਥਿਰ ਪੂਰਨ __init sched_core_sysctl_init(ਖਾਲੀ)
{
	ਰਜਿਸਟਰ_sysctl_init("kernel", sched_core_sysctls);
	ਵਾਪਸ 0;
}
late_initcall(sched_core_sysctl_init);
#ਜੇ_ਅੰਤ /* CONFIG_SYSCTL */

/*
 * fork()/clone()-time setup:
 */
ਪੂਰਨ sched_fork(u64 clone_flags, ਬਣਤਰ task_struct *p)
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
	p->prio = current->ਸਾਧਾਰਨ_prio;

	uclamp_fork(p);

	/*
	 * Revert to default priority/policy on fork if requested.
	 */
	ਜੇ (unlikely(p->sched_reset_on_fork)) {
		ਜੇ (task_has_dl_policy(p) || task_has_rt_policy(p)) {
			p->policy = SCHED_ਸਾਧਾਰਨ;
			p->static_prio = NICE_ਮੰਜ਼ਿਲ_PRIO(0);
			p->rt_ਤਰਜੀਹ = 0;
			p->timer_slack_ns = p->default_timer_slack_ns;
		} ਨਹੀਂਤਾਂ ਜੇ (PRIO_ਮੰਜ਼ਿਲ_NICE(p->static_prio) < 0)
			p->static_prio = NICE_ਮੰਜ਼ਿਲ_PRIO(0);

		p->prio = p->ਸਾਧਾਰਨ_prio = p->static_prio;
		ਸੈੱਟ_ਲੱਦੋ_ਭਾਰ(p, false);
		p->se.custom_ਟੁਕੜਾ = 0;
		p->se.ਟੁਕੜਾ = sysctl_sched_ਅਧਾਰ_ਟੁਕੜਾ;

		/*
		 * We don't need the reset flag anymore after the fork. It has
		 * fulfilled its duty:
		 */
		p->sched_reset_on_fork = 0;
	}

	ਜੇ (dl_prio(p->prio))
		ਵਾਪਸ -EAGAIN;

	scx_pre_fork(p);

	ਜੇ (rt_prio(p->prio)) {
		p->sched_class = &rt_sched_class;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CLASS_EXT
	} ਨਹੀਂਤਾਂ ਜੇ (task_should_scx(p->policy)) {
		p->sched_class = &ext_sched_class;
#ਜੇ_ਅੰਤ
	} ਨਹੀਂਤਾਂ {
		p->sched_class = &fair_sched_class;
	}

	init_entity_runnable_ਔਸਤਨ(&p->se);


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_INFO
	ਜੇ (likely(sched_info_on()))
		memset(&p->sched_info, 0, ਆਕਾਰ(p->sched_info));
#ਜੇ_ਅੰਤ
	p->on_cpu = 0;
	init_task_preempt_ਗਿਣਤੀ(p);
	plist_ਗੰਢ_init(&p->pushable_tasks, MAX_PRIO);
	RB_CLEAR_ਗੰਢ(&p->pushable_dl_tasks);

	ਵਾਪਸ 0;
}

ਪੂਰਨ sched_cgroup_fork(ਬਣਤਰ task_struct *p, ਬਣਤਰ kernel_clone_ਦਲੀਲਾਂ *kargs)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	/*
	 * Because we're not yet on the pid-hash, p->pi_lock isn't strictly
	 * required yet, but lockdep gets upset if rules are violated.
	 */
	raw_spin_ਤਾਲਾ_irqsave(&p->pi_ਤਾਲਾ, flags);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CGROUP_SCHED
	ਜੇ (1) {
		ਬਣਤਰ task_ਸਮੂਹ *tg;
		tg = container_ਦਾ(kargs->cset->subsys[cpu_cgrp_id],
				  ਬਣਤਰ task_ਸਮੂਹ, css);
		tg = autogroup_task_ਸਮੂਹ(p, tg);
		p->sched_task_ਸਮੂਹ = tg;
	}
#ਜੇ_ਅੰਤ
	/*
	 * We're setting the CPU for the first time, we don't migrate,
	 * so use __set_task_cpu().
	 */
	__set_task_cpu(p, smp_processor_id());
	ਜੇ (p->sched_class->task_fork)
		p->sched_class->task_fork(p);
	raw_spin_unlock_irqrestore(&p->pi_ਤਾਲਾ, flags);

	ਵਾਪਸ scx_fork(p, kargs);
}

ਖਾਲੀ sched_cancel_fork(ਬਣਤਰ task_struct *p)
{
	scx_cancel_fork(p);
}

ਸਥਿਰ ਖਾਲੀ sched_mm_cid_fork(ਬਣਤਰ task_struct *t);

ਖਾਲੀ sched_post_fork(ਬਣਤਰ task_struct *p)
{
	sched_mm_cid_fork(p);
	uclamp_post_fork(p);
	scx_post_fork(p);
}

u64 ਮੰਜ਼ਿਲ_ਅਨੁਪਾਤ(u64 period, u64 runtime)
{
	ਜੇ (runtime == RUNTIME_INF)
		ਵਾਪਸ BW_UNIT;

	/*
	 * Doing this here saves a lot of checks in all
	 * the calling paths, and returning zero seems
	 * safe for them anyway.
	 */
	ਜੇ (period == 0)
		ਵਾਪਸ 0;

	ਵਾਪਸ div64_u64(runtime << BW_ਖਿਸਕਾਓ, period);
}

/*
 * wake_up_new_task - wake up a newly created task for the first time.
 *
 * This function will do some initial scheduler statistics housekeeping
 * that must be done for every newly created context, then puts the task
 * on the runqueue and wakes it.
 */
ਖਾਲੀ wake_ਉੱਪਰ_new_task(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ rq_flags rf;
	ਬਣਤਰ rq *rq;
	ਪੂਰਨ wake_flags = WF_FORK;

	raw_spin_ਤਾਲਾ_irqsave(&p->pi_ਤਾਲਾ, rf.flags);
	WRITE_ONCE(p->__state, TASK_RUNNING);
	/*
	 * Fork balancing, do it here and not earlier because:
	 *  - cpus_ptr can change in the fork path
	 *  - any previously selected CPU might disappear through hotplug
	 *
	 * Use __set_task_cpu() to avoid calling sched_class::migrate_task_rq,
	 * as we're not fully set-up yet.
	 */
	p->recent_ਵਰਤਿਆ_cpu = task_cpu(p);
	__set_task_cpu(p, select_task_rq(p, task_cpu(p), &wake_flags));
	rq = __task_rq_lock(p, &rf);
	update_rq_clock(rq);
	post_init_entity_util_avg(p);

	activate_task(rq, p, ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_NOCLOCK | ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_INITIAL);
	trace_sched_wakeup_new(p);
	wakeup_preempt(rq, p, wake_flags);
	ਜੇ (p->sched_class->task_woken) {
		/*
		 * Nothing relies on rq->lock after this, so it's fine to
		 * drop it.
		 */
		rq_unpin_ਤਾਲਾ(rq, &rf);
		p->sched_class->task_woken(rq, p);
		rq_repin_ਤਾਲਾ(rq, &rf);
	}
	task_rq_unlock(rq, p, &rf);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PREEMPT_NOTIFIERS

ਸਥਿਰ DEFINE_STATIC_ਕੁੰਜੀ_FALSE(preempt_notifier_ਕੁੰਜੀ);

ਖਾਲੀ preempt_notifier_inc(ਖਾਲੀ)
{
	static_branch_inc(&preempt_notifier_ਕੁੰਜੀ);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(preempt_notifier_inc);

ਖਾਲੀ preempt_notifier_dec(ਖਾਲੀ)
{
	static_branch_dec(&preempt_notifier_ਕੁੰਜੀ);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(preempt_notifier_dec);

/**
 * preempt_notifier_register - tell me when current is being preempted & rescheduled
 * @notifier: notifier struct to register
 */
ਖਾਲੀ preempt_notifier_ਰਜਿਸਟਰ(ਬਣਤਰ preempt_notifier *notifier)
{
	ਜੇ (!static_branch_unlikely(&preempt_notifier_ਕੁੰਜੀ))
		WARN(1, "registering preempt_notifier while notifiers disabled\n");

	hlist_ਜੋੜੋ_ਸਿਰ(&notifier->link, &current->preempt_notifiers);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(preempt_notifier_ਰਜਿਸਟਰ);

/**
 * preempt_notifier_unregister - no longer interested in preemption notifications
 * @notifier: notifier struct to unregister
 *
 * This is *not* safe to call from within a preemption notifier.
 */
ਖਾਲੀ preempt_notifier_unregister(ਬਣਤਰ preempt_notifier *notifier)
{
	hlist_del(&notifier->link);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(preempt_notifier_unregister);

ਸਥਿਰ ਖਾਲੀ __fire_sched_in_preempt_notifiers(ਬਣਤਰ task_struct *curr)
{
	ਬਣਤਰ preempt_notifier *notifier;

	hlist_for_each_ਇੰਦਰਾਜ(notifier, &curr->preempt_notifiers, link)
		notifier->ops->sched_ਵਿੱਚ(notifier, raw_smp_processor_id());
}

ਸਥਿਰ __always_inline ਖਾਲੀ fire_sched_ਵਿੱਚ_preempt_notifiers(ਬਣਤਰ task_struct *curr)
{
	ਜੇ (static_branch_unlikely(&preempt_notifier_ਕੁੰਜੀ))
		__fire_sched_in_preempt_notifiers(curr);
}

ਸਥਿਰ ਖਾਲੀ
__fire_sched_out_preempt_notifiers(ਬਣਤਰ task_struct *curr,
				   ਬਣਤਰ task_struct *ਅਗਲਾ)
{
	ਬਣਤਰ preempt_notifier *notifier;

	hlist_for_each_ਇੰਦਰਾਜ(notifier, &curr->preempt_notifiers, link)
		notifier->ops->sched_ਬਾਹਰਲਾ(notifier, ਅਗਲਾ);
}

ਸਥਿਰ __always_inline ਖਾਲੀ
fire_sched_ਬਾਹਰਲਾ_preempt_notifiers(ਬਣਤਰ task_struct *curr,
				 ਬਣਤਰ task_struct *ਅਗਲਾ)
{
	ਜੇ (static_branch_unlikely(&preempt_notifier_ਕੁੰਜੀ))
		__fire_sched_out_preempt_notifiers(curr, ਅਗਲਾ);
}

#ਨਹੀਂਤਾਂ /* !CONFIG_PREEMPT_NOTIFIERS: */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ fire_sched_ਵਿੱਚ_preempt_notifiers(ਬਣਤਰ task_struct *curr)
{
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
fire_sched_ਬਾਹਰਲਾ_preempt_notifiers(ਬਣਤਰ task_struct *curr,
				 ਬਣਤਰ task_struct *ਅਗਲਾ)
{
}

#ਜੇ_ਅੰਤ /* !CONFIG_PREEMPT_NOTIFIERS */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ prepare_task(ਬਣਤਰ task_struct *ਅਗਲਾ)
{
	/*
	 * Claim the task as running, we do this before switching to it
	 * such that any running task will have this set.
	 *
	 * See the smp_load_acquire(&p->on_cpu) case in ttwu() and
	 * its ordering comment.
	 */
	WRITE_ONCE(ਅਗਲਾ->on_cpu, 1);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਅੰਤ_task(ਬਣਤਰ task_struct *prev)
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
ਸਥਿਰ ਖਾਲੀ zap_balance_callbacks(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ balance_callback *ਅਗਲਾ, *ਸਿਰ;
	bool ਮਿਲਿਆ = false;

	lockdep_assert_rq_held(rq);

	ਸਿਰ = rq->balance_callback;
	ਜਦੋਂ (ਸਿਰ) {
		ਜੇ (ਸਿਰ == &balance_ਧੱਕੋ_callback)
			ਮਿਲਿਆ = true;
		ਅਗਲਾ = ਸਿਰ->ਅਗਲਾ;
		ਸਿਰ->ਅਗਲਾ = ਨਿਰਾ;
		ਸਿਰ = ਅਗਲਾ;
	}
	rq->balance_callback = ਮਿਲਿਆ ? &balance_ਧੱਕੋ_callback : ਨਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ do_balance_callbacks(ਬਣਤਰ rq *rq, ਬਣਤਰ balance_callback *ਸਿਰ)
{
	ਖਾਲੀ (*func)(ਬਣਤਰ rq *rq);
	ਬਣਤਰ balance_callback *ਅਗਲਾ;

	lockdep_assert_rq_held(rq);

	ਜਦੋਂ (ਸਿਰ) {
		func = ਸਿਰ->func;
		ਅਗਲਾ = ਸਿਰ->ਅਗਲਾ;
		ਸਿਰ->ਅਗਲਾ = ਨਿਰਾ;
		ਸਿਰ = ਅਗਲਾ;

		func(rq);
	}
}

ਸਥਿਰ ਖਾਲੀ balance_ਧੱਕੋ(ਬਣਤਰ rq *rq);

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
ਬਣਤਰ balance_callback balance_ਧੱਕੋ_callback = {
	.ਅਗਲਾ = ਨਿਰਾ,
	.func = balance_ਧੱਕੋ,
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ balance_callback *
__splice_balance_callbacks(ਬਣਤਰ rq *rq, bool ਵੰਡੋ)
{
	ਬਣਤਰ balance_callback *ਸਿਰ = rq->balance_callback;

	ਜੇ (likely(!ਸਿਰ))
		ਵਾਪਸ ਨਿਰਾ;

	lockdep_assert_rq_held(rq);
	/*
	 * Must not take balance_push_callback off the list when
	 * splice_balance_callbacks() and balance_callbacks() are not
	 * in the same rq->lock section.
	 *
	 * In that case it would be possible for __schedule() to interleave
	 * and observe the list empty.
	 */
	ਜੇ (ਵੰਡੋ && ਸਿਰ == &balance_ਧੱਕੋ_callback)
		ਸਿਰ = ਨਿਰਾ;
	ਨਹੀਂਤਾਂ
		rq->balance_callback = ਨਿਰਾ;

	ਵਾਪਸ ਸਿਰ;
}

ਬਣਤਰ balance_callback *splice_balance_callbacks(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ __splice_balance_callbacks(rq, true);
}

ਖਾਲੀ __balance_callbacks(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
{
	ਜੇ (rf)
		rq_unpin_ਤਾਲਾ(rq, rf);
	do_balance_callbacks(rq, __splice_balance_callbacks(rq, false));
	ਜੇ (rf)
		rq_repin_ਤਾਲਾ(rq, rf);
}

ਖਾਲੀ balance_callbacks(ਬਣਤਰ rq *rq, ਬਣਤਰ balance_callback *ਸਿਰ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	ਜੇ (unlikely(ਸਿਰ)) {
		raw_spin_rq_ਤਾਲਾ_irqsave(rq, flags);
		do_balance_callbacks(rq, ਸਿਰ);
		raw_spin_rq_unlock_irqrestore(rq, flags);
	}
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
prepare_ਤਾਲਾ_switch(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *ਅਗਲਾ, ਬਣਤਰ rq_flags *rf)
	__releases(__rq_lockp(rq))
	__acquires(__rq_lockp(this_rq()))
{
	/*
	 * Since the runqueue lock will be released by the next
	 * task (which is an invalid locking op but in the case
	 * of the scheduler it's an obvious special-case), so we
	 * do an early lockdep release here:
	 */
	rq_unpin_ਤਾਲਾ(rq, rf);
	spin_release(&__rq_lockp(rq)->dep_map, _THIS_IP_);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_DEBUG_SPINLOCK
	/* this is a valid case when another task releases the spinlock */
	rq_lockp(rq)->owner = ਅਗਲਾ;
#ਜੇ_ਅੰਤ
	/*
	 * Model the rq reference switcheroo.
	 */
	__release(__rq_lockp(rq));
	__acquire(__rq_lockp(this_rq()));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਅੰਤ_ਤਾਲਾ_switch(ਬਣਤਰ rq *rq)
	__releases(__rq_lockp(rq))
{
	/*
	 * If we are tracking spinlock dependencies then we have to
	 * fix up the runqueue lock - which gets 'carried over' from
	 * prev into current:
	 */
	spin_acquire(&__rq_lockp(rq)->dep_map, 0, 0, _THIS_IP_);
	__balance_callbacks(rq, ਨਿਰਾ);
	hrtick_ਸਮਾਂ_ਸਾਰਣੀ_exit(rq);
	raw_spin_rq_unlock_irq(rq);
}

/*
 * NOP if the arch has not defined these:
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ prepare_arch_switch
# define prepare_arch_switch(ਅਗਲਾ)	ਕਰੋ { } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਅੰਤ_arch_post_ਤਾਲਾ_switch
# define ਅੰਤ_arch_post_ਤਾਲਾ_switch()	ਕਰੋ { } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ kmap_local_sched_ਬਾਹਰਲਾ(ਖਾਲੀ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_KMAP_LOCAL
	ਜੇ (unlikely(current->kmap_ctrl.idx))
		__kmap_local_sched_out();
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ kmap_local_sched_ਵਿੱਚ(ਖਾਲੀ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_KMAP_LOCAL
	ਜੇ (unlikely(current->kmap_ctrl.idx))
		__kmap_local_sched_in();
#ਜੇ_ਅੰਤ
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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
prepare_task_switch(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *prev,
		    ਬਣਤਰ task_struct *ਅਗਲਾ)
	__must_hold(__rq_lockp(rq))
{
	kcov_prepare_switch(prev);
	sched_info_switch(rq, prev, ਅਗਲਾ);
	perf_event_task_sched_ਬਾਹਰਲਾ(prev, ਅਗਲਾ);
	fire_sched_ਬਾਹਰਲਾ_preempt_notifiers(prev, ਅਗਲਾ);
	kmap_local_sched_ਬਾਹਰਲਾ();
	prepare_task(ਅਗਲਾ);
	prepare_arch_switch(ਅਗਲਾ);
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
ਸਥਿਰ ਬਣਤਰ rq *ਅੰਤ_task_switch(ਬਣਤਰ task_struct *prev)
	__releases(__rq_lockp(this_rq()))
{
	ਬਣਤਰ rq *rq = this_rq();
	ਬਣਤਰ mm_struct *mm = rq->prev_mm;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ prev_state;

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
	ਜੇ (WARN_ONCE(preempt_ਗਿਣਤੀ() != 2*PREEMPT_DISABLE_ਵਿੱਥ,
		      "corrupted preempt_count: %s/%d/0x%x\n",
		      current->comm, current->pid, preempt_ਗਿਣਤੀ()))
		preempt_ਗਿਣਤੀ_ਸੈੱਟ(FORK_PREEMPT_ਗਿਣਤੀ);

	rq->prev_mm = ਨਿਰਾ;

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
	perf_event_task_sched_ਵਿੱਚ(prev, current);
	ਅੰਤ_task(prev);
	tick_nohz_task_switch();
	ਅੰਤ_ਤਾਲਾ_switch(rq);
	ਅੰਤ_arch_post_ਤਾਲਾ_switch();
	kcov_ਅੰਤ_switch(current);
	/*
	 * kmap_local_sched_out() is invoked with rq::lock held and
	 * interrupts disabled. There is no requirement for that, but the
	 * sched out code does not have an interrupt enabled section.
	 * Restoring the maps on sched in does not require interrupts being
	 * disabled either.
	 */
	kmap_local_sched_ਵਿੱਚ();

	/*
	 * Any cached block-layer timestamp (plug->cur_ktime) is stale now,
	 * invalidate it.
	 */
	blk_plug_invalidate_ts();

	fire_sched_ਵਿੱਚ_preempt_notifiers(current);
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
	ਜੇ (mm) {
		membarrier_mm_sync_core_before_usermode(mm);
		mmdrop_lazy_tlb_sched(mm);
	}

	ਜੇ (unlikely(prev_state == TASK_DEAD)) {
		ਜੇ (prev->sched_class->task_dead)
			prev->sched_class->task_dead(prev);

		/*
		 * sched_ext_dead() must come before cgroup_task_dead() to
		 * prevent cgroups from being removed while its member tasks are
		 * visible to SCX schedulers.
		 */
		sched_ext_dead(prev);
		cgroup_task_dead(prev);

		/* Task is done with its stack. */
		put_task_ਢੇਰ(prev);

		put_task_struct_rcu_user(prev);
	}

	ਵਾਪਸ rq;
}

/**
 * schedule_tail - first thing a freshly forked thread must call.
 * @prev: the thread we just switched away from.
 */
asmlinkage __visible ਖਾਲੀ ਸਮਾਂ_ਸਾਰਣੀ_ਪੂਛ(ਬਣਤਰ task_struct *prev)
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

	ਅੰਤ_task_switch(prev);
	/*
	 * This is a special case: the newly created task has just
	 * switched the context for the first time. It is returning from
	 * schedule for the first time in this path.
	 */
	trace_sched_exit_tp(true);
	preempt_enable();

	ਜੇ (current->ਸੈੱਟ_ਬੱਚਾ_tid)
		put_user(task_pid_vnr(current), current->ਸੈੱਟ_ਬੱਚਾ_tid);

	calculate_sigpending();
}

/*
 * context_switch - switch to the new MM and the new thread's register state.
 */
ਸਥਿਰ __always_inline ਬਣਤਰ rq *
context_switch(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *prev,
	       ਬਣਤਰ task_struct *ਅਗਲਾ, ਬਣਤਰ rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	prepare_task_switch(rq, prev, ਅਗਲਾ);

	/*
	 * For paravirt, this is coupled with an exit in switch_to to
	 * combine the page table reload and the switch backend into
	 * one hypercall.
	 */
	arch_ਸ਼ੁਰੂ_context_switch(prev);

	/*
	 * kernel -> kernel   lazy + transfer active
	 *   user -> kernel   lazy + mmgrab_lazy_tlb() active
	 *
	 * kernel ->   user   switch + mmdrop_lazy_tlb() active
	 *   user ->   user   switch
	 */
	ਜੇ (!ਅਗਲਾ->mm) {				// to kernel
		enter_lazy_tlb(prev->ਸਰਗਰਮ_mm, ਅਗਲਾ);

		ਅਗਲਾ->ਸਰਗਰਮ_mm = prev->ਸਰਗਰਮ_mm;
		ਜੇ (prev->mm)				// from user
			mmgrab_lazy_tlb(prev->ਸਰਗਰਮ_mm);
		ਨਹੀਂਤਾਂ
			prev->ਸਰਗਰਮ_mm = ਨਿਰਾ;
	} ਨਹੀਂਤਾਂ {					// to user
		membarrier_switch_mm(rq, prev->ਸਰਗਰਮ_mm, ਅਗਲਾ->mm);
		/*
		 * sys_membarrier() requires an smp_mb() between setting
		 * rq->curr / membarrier_switch_mm() and returning to userspace.
		 *
		 * The below provides this either through switch_mm(), or in
		 * case 'prev->active_mm == next->mm' through
		 * finish_task_switch()'s mmdrop().
		 */
		switch_mm_irqs_off(prev->ਸਰਗਰਮ_mm, ਅਗਲਾ->mm, ਅਗਲਾ);
		lru_gen_use_mm(ਅਗਲਾ->mm);

		ਜੇ (!prev->mm) {			// from kernel
			/* will mmdrop_lazy_tlb() in finish_task_switch(). */
			rq->prev_mm = prev->ਸਰਗਰਮ_mm;
			prev->ਸਰਗਰਮ_mm = ਨਿਰਾ;
		}
	}

	mm_cid_switch_ਮੰਜ਼ਿਲ(prev, ਅਗਲਾ);

	/*
	 * Tell rseq that the task was scheduled in. Must be after
	 * switch_mm_cid() to get the TIF flag set.
	 */
	rseq_sched_switch_event(ਅਗਲਾ);

	prepare_ਤਾਲਾ_switch(rq, ਅਗਲਾ, rf);

	/* Here we just switch the register state and the stack. */
	switch_ਮੰਜ਼ਿਲ(prev, ਅਗਲਾ, prev);
	barrier();

	ਵਾਪਸ ਅੰਤ_task_switch(prev);
}

/*
 * nr_running and nr_context_switches:
 *
 * externally visible scheduler statistics: current number of runnable
 * threads, total number of context switches performed since bootup.
 */
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr_running(ਖਾਲੀ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i, ਜੋੜ = 0;

	for_each_online_cpu(i)
		ਜੋੜ += cpu_rq(i)->nr_running;

	ਵਾਪਸ ਜੋੜ;
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
bool single_task_running(ਖਾਲੀ)
{
	ਵਾਪਸ raw_rq()->nr_running == 1;
}
EXPORT_ਚਿੰਨ੍ਹ(single_task_running);

ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ nr_context_switches_cpu(ਪੂਰਨ cpu)
{
	ਵਾਪਸ cpu_rq(cpu)->nr_switches;
}

ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ nr_context_switches(ਖਾਲੀ)
{
	ਪੂਰਨ i;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਜੋੜ = 0;

	for_each_possible_cpu(i)
		ਜੋੜ += cpu_rq(i)->nr_switches;

	ਵਾਪਸ ਜੋੜ;
}

/*
 * Return whether any tasks are accounted as I/O waiters on this CPU.
 * They may wake up on a different CPU.
 */
bool cpu_has_iowait(ਪੂਰਨ cpu)
{
	ਵਾਪਸ atomic_read(&cpu_rq(cpu)->nr_iowait) != 0;
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

ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr_iowait(ਖਾਲੀ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i, ਜੋੜ = 0;

	for_each_possible_cpu(i)
		ਜੋੜ += atomic_read(&cpu_rq(i)->nr_iowait);

	ਵਾਪਸ ਜੋੜ;
}

/*
 * sched_exec - execve() is a valuable balancing opportunity, because at
 * this point the task has the smallest effective memory and cache footprint.
 */
ਖਾਲੀ sched_exec(ਖਾਲੀ)
{
	ਬਣਤਰ task_struct *p = current;
	ਬਣਤਰ migration_arg arg;
	ਪੂਰਨ dest_cpu;

	scoped_guard (raw_spinlock_irqsave, &p->pi_ਤਾਲਾ) {
		dest_cpu = p->sched_class->select_task_rq(p, task_cpu(p), WF_EXEC);
		ਜੇ (dest_cpu == smp_processor_id())
			ਵਾਪਸ;

		ਜੇ (unlikely(!cpu_ਸਰਗਰਮ(dest_cpu)))
			ਵਾਪਸ;

		arg = (ਬਣਤਰ migration_arg){ p, dest_cpu };
	}
	stop_one_cpu(task_cpu(p), migration_cpu_stop, &arg);
}

DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ kernel_stat, kstat);
DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ kernel_cpustat, kernel_cpustat) = {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NO_HZ_COMMON
	.idle_sleeptime_seq = SEQCNT_ZERO(kernel_cpustat.idle_sleeptime_seq)
#ਜੇ_ਅੰਤ
};

EXPORT_ਪ੍ਰਤੀ_CPU_ਚਿੰਨ੍ਹ(kstat);
EXPORT_ਪ੍ਰਤੀ_CPU_ਚਿੰਨ੍ਹ(kernel_cpustat);

/*
 * The function fair_sched_class.update_curr accesses the struct curr
 * and its field curr->exec_start; when called from task_sched_runtime(),
 * we observe a high rate of cache misses in practice.
 * Prefetching this data results in improved performance.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ prefetch_curr_exec_ਸ਼ੁਰੂ(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ sched_entity *curr = task_rq(p)->cfs.curr;

	prefetch(curr);
	prefetch(&curr->exec_ਸ਼ੁਰੂ);
}

/*
 * Return accounted runtime for the task.
 * In case the task is currently running, return the runtime plus current's
 * pending runtime that have not been accounted yet.
 */
ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ task_sched_runtime(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ rq_flags rf;
	ਬਣਤਰ rq *rq;
	u64 ns;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_64_ਦੋਆਧਾਰੀ_ਅੰਕ
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
	ਜੇ (!p->on_cpu || !task_on_rq_queued(p))
		ਵਾਪਸ p->se.ਜੋੜ_exec_runtime;
#ਜੇ_ਅੰਤ

	rq = task_rq_ਤਾਲਾ_2(p, &rf);
	/*
	 * Must be ->curr _and_ ->on_rq.  If dequeued, we would
	 * project cycles that may never be accounted to this
	 * thread, breaking clock_gettime().
	 */
	ਜੇ (task_current_donor(rq, p) && task_on_rq_queued(p)) {
		prefetch_curr_exec_ਸ਼ੁਰੂ(p);
		update_rq_clock(rq);
		p->sched_class->update_curr(rq);
	}
	ns = p->se.ਜੋੜ_exec_runtime;
	task_rq_unlock(rq, p, &rf);

	ਵਾਪਸ ns;
}

ਸਥਿਰ u64 cpu_resched_latency(ਬਣਤਰ rq *rq)
{
	ਪੂਰਨ latency_warn_ms = READ_ONCE(sysctl_resched_latency_warn_ms);
	u64 resched_latency, now = rq_clock(rq);
	ਸਥਿਰ bool warned_once;

	ਜੇ (sysctl_resched_latency_warn_once && warned_once)
		ਵਾਪਸ 0;

	ਜੇ (!need_resched() || !latency_warn_ms)
		ਵਾਪਸ 0;

	ਜੇ (system_state == SYSTEM_BOOTING)
		ਵਾਪਸ 0;

	ਜੇ (!rq->ਆਖਰੀ_ਵੇਖਿਆ_need_resched_ns) {
		rq->ਆਖਰੀ_ਵੇਖਿਆ_need_resched_ns = now;
		rq->ticks_without_resched = 0;
		ਵਾਪਸ 0;
	}

	rq->ticks_without_resched++;
	resched_latency = now - rq->ਆਖਰੀ_ਵੇਖਿਆ_need_resched_ns;
	ਜੇ (resched_latency <= latency_warn_ms * NSEC_ਪ੍ਰਤੀ_MSEC)
		ਵਾਪਸ 0;

	warned_once = true;

	ਵਾਪਸ resched_latency;
}

ਸਥਿਰ ਪੂਰਨ __init ਤਿਆਰੀ_resched_latency_warn_ms(ਅੱਖਰ *str)
{
	ਲੰਮਾ val;

	ਜੇ ((kstrtol(str, 0, &val))) {
		pr_warn("Unable to set resched_latency_warn_ms\n");
		ਵਾਪਸ 1;
	}

	sysctl_resched_latency_warn_ms = val;
	ਵਾਪਸ 1;
}
__setup("resched_latency_warn_ms=", ਤਿਆਰੀ_resched_latency_warn_ms);

/*
 * This function gets called by the timer code, with HZ frequency.
 * We call it with interrupts disabled.
 */
ਖਾਲੀ sched_tick(ਖਾਲੀ)
{
	ਪੂਰਨ cpu = smp_processor_id();
	ਬਣਤਰ rq *rq = cpu_rq(cpu);
	/* scheduler accounting goes to the donor task */
	ਬਣਤਰ task_struct *curr, *donor;
	ਬਣਤਰ rq_flags rf;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ hw_pressure;
	u64 resched_latency;

	ਜੇ (!cpu_preferred(cpu))
		sched_ਧੱਕੋ_current_non_preferred_cpu(rq);

	ਜੇ (housekeeping_cpu(cpu, HK_TYPE_KERNEL_NOISE))
		arch_scale_freq_tick();

	sched_clock_tick();

	rq_ਤਾਲਾ(rq, &rf);
	curr = rq->curr;
	donor = rq->donor;

	psi_account_irqtime(rq, curr, ਨਿਰਾ);

	update_rq_clock(rq);
	hw_pressure = arch_scale_hw_pressure(cpu_ਦਾ(rq));
	update_hw_ਲੱਦੋ_avg(rq_clock_task(rq), rq, hw_pressure);

	ਜੇ (dynamic_preempt_lazy() && tif_test_ਦੋਆਧਾਰੀ_ਅੰਕ(TIF_NEED_RESCHED_LAZY))
		resched_curr(rq);

	donor->sched_class->task_tick(rq, donor, 0);
	ਜੇ (sched_feat(LATENCY_WARN))
		resched_latency = cpu_resched_latency(rq);
	calc_global_ਲੱਦੋ_tick(rq);
	sched_core_tick(rq);
	scx_tick(rq);

	rq_unlock(rq, &rf);

	ਜੇ (sched_feat(LATENCY_WARN) && resched_latency)
		resched_latency_warn(cpu, resched_latency);

	perf_event_task_tick();

	ਜੇ (curr->flags & PF_WQ_ਕਾਮਾ)
		wq_ਕਾਮਾ_tick(curr);

	ਜੇ (!scx_switched_all()) {
		rq->idle_balance = idle_cpu(cpu);
		sched_balance_trigger(rq);
	}
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NO_HZ_ਭਰਿਆ

ਬਣਤਰ tick_work {
	ਪੂਰਨ			cpu;
	atomic_t		state;
	ਬਣਤਰ delayed_work	work;
};
/* Values for ->state, see diagram below. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TICK_SCHED_REMOTE_OFFLINE	0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TICK_SCHED_REMOTE_OFFLINING	1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TICK_SCHED_REMOTE_RUNNING	2

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

ਸਥਿਰ ਬਣਤਰ tick_work __percpu *tick_work_cpu;

ਸਥਿਰ ਖਾਲੀ sched_tick_remote(ਬਣਤਰ work_struct *work)
{
	ਬਣਤਰ delayed_work *dwork = ਮੰਜ਼ਿਲ_delayed_work(work);
	ਬਣਤਰ tick_work *twork = container_ਦਾ(dwork, ਬਣਤਰ tick_work, work);
	ਪੂਰਨ cpu = twork->cpu;
	ਬਣਤਰ rq *rq = cpu_rq(cpu);
	ਪੂਰਨ os;

	/*
	 * Handle the tick only if it appears the remote CPU is running in full
	 * dynticks mode. The check is racy by nature, but missing a tick or
	 * having one too much is no big deal because the scheduler tick updates
	 * statistics and checks timeslices in a time-independent way, regardless
	 * of when exactly it is running.
	 */
	ਜੇ (tick_nohz_tick_stopped_cpu(cpu)) {
		guard(rq_ਤਾਲਾ_irq)(rq);
		ਬਣਤਰ task_struct *curr = rq->curr;

		ਜੇ (cpu_online(cpu)) {
			/*
			 * Since this is a remote tick for full dynticks mode,
			 * we are always sure that there is no proxy (only a
			 * single task is running).
			 */
			WARN_ON_ONCE(rq->curr != rq->donor);
			update_rq_clock(rq);

			ਜੇ (!is_idle_task(curr)) {
				/*
				 * Make sure the next tick runs within a
				 * reasonable amount of time.
				 */
				u64 delta = rq_clock_task(rq) - curr->se.exec_ਸ਼ੁਰੂ;
				WARN_ON_ONCE(delta > (u64)NSEC_ਪ੍ਰਤੀ_SEC * 30);
			}
			curr->sched_class->task_tick(rq, curr, 0);

			calc_ਲੱਦੋ_nohz_remote(rq);
		}
	}

	/*
	 * Run the remote tick once per second (1Hz). This arbitrary
	 * frequency is large enough to avoid overload but short enough
	 * to keep scheduler internal stats reasonably up to date.  But
	 * first update state to reflect hotplug activity if required.
	 */
	os = atomic_fetch_ਜੋੜੋ_unless(&twork->state, -1, TICK_SCHED_REMOTE_RUNNING);
	WARN_ON_ONCE(os == TICK_SCHED_REMOTE_OFFLINE);
	ਜੇ (os == TICK_SCHED_REMOTE_RUNNING)
		ਕਤਾਰ_delayed_work(system_dfl_wq, dwork, HZ);
}

ਸਥਿਰ ਖਾਲੀ sched_tick_ਸ਼ੁਰੂ(ਪੂਰਨ cpu)
{
	ਪੂਰਨ os;
	ਬਣਤਰ tick_work *twork;

	ਜੇ (housekeeping_cpu(cpu, HK_TYPE_KERNEL_NOISE))
		ਵਾਪਸ;

	WARN_ON_ONCE(!tick_work_cpu);

	twork = ਪ੍ਰਤੀ_cpu_ptr(tick_work_cpu, cpu);
	os = atomic_xchg(&twork->state, TICK_SCHED_REMOTE_RUNNING);
	WARN_ON_ONCE(os == TICK_SCHED_REMOTE_RUNNING);
	ਜੇ (os == TICK_SCHED_REMOTE_OFFLINE) {
		twork->cpu = cpu;
		INIT_DELAYED_WORK(&twork->work, sched_tick_remote);
		ਕਤਾਰ_delayed_work(system_dfl_wq, &twork->work, HZ);
	}
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_HOTPLUG_CPU
ਸਥਿਰ ਖਾਲੀ sched_tick_stop(ਪੂਰਨ cpu)
{
	ਬਣਤਰ tick_work *twork;
	ਪੂਰਨ os;

	ਜੇ (housekeeping_cpu(cpu, HK_TYPE_KERNEL_NOISE))
		ਵਾਪਸ;

	WARN_ON_ONCE(!tick_work_cpu);

	twork = ਪ੍ਰਤੀ_cpu_ptr(tick_work_cpu, cpu);
	/* There cannot be competing actions, but don't rely on stop-machine. */
	os = atomic_xchg(&twork->state, TICK_SCHED_REMOTE_OFFLINING);
	WARN_ON_ONCE(os != TICK_SCHED_REMOTE_RUNNING);
	/* Don't cancel, as this would mess up the state machine. */
}
#ਜੇ_ਅੰਤ /* CONFIG_HOTPLUG_CPU */

ਪੂਰਨ __init sched_tick_offload_init(ਖਾਲੀ)
{
	tick_work_cpu = alloc_percpu(ਬਣਤਰ tick_work);
	BUG_ON(!tick_work_cpu);
	ਵਾਪਸ 0;
}

#ਨਹੀਂਤਾਂ /* !CONFIG_NO_HZ_FULL: */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_tick_ਸ਼ੁਰੂ(ਪੂਰਨ cpu) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_tick_stop(ਪੂਰਨ cpu) { }
#ਜੇ_ਅੰਤ /* !CONFIG_NO_HZ_FULL */

#ਜੇ defined(CONFIG_PREEMPTION) && (defined(CONFIG_DEBUG_PREEMPT) || \
				defined(CONFIG_TRACE_PREEMPT_TOGGLE))
/*
 * If the value passed in is equal to the current preempt count
 * then we just disabled preemption. Start timing the latency.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ preempt_latency_ਸ਼ੁਰੂ(ਪੂਰਨ val)
{
	ਜੇ (preempt_ਗਿਣਤੀ() == val) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ip = get_ਤਾਲਾ_ਮਾਪਾ_ip();
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_DEBUG_PREEMPT
		current->preempt_disable_ip = ip;
#ਜੇ_ਅੰਤ
		trace_preempt_off(CALLER_ADDR0, ip);
	}
}

ਖਾਲੀ preempt_ਗਿਣਤੀ_ਜੋੜੋ(ਪੂਰਨ val)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_DEBUG_PREEMPT
	/*
	 * Underflow?
	 *
	 * Cannot detect underflow based on the current preempt_count() value
	 * if using HAS_SEPARATE_PREEMPT_RESCHED_BITS because preempt count takes all 32
	 * bits.
	 */
	ਜੇ (!IS_ENABLED(CONFIG_HAS_SEPARATE_PREEMPT_RESCHED_ਬਿੱਟ) &&
	    DEBUG_LOCKS_WARN_ON((preempt_ਗਿਣਤੀ() < 0)))
		ਵਾਪਸ;
#ਜੇ_ਅੰਤ
	__preempt_count_add(val);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_DEBUG_PREEMPT
	/*
	 * Spinlock count overflowing soon?
	 */
	DEBUG_LOCKS_WARN_ON((preempt_ਗਿਣਤੀ() & PREEMPT_ਨਕਾਬ) >=
				PREEMPT_ਨਕਾਬ - 10);
#ਜੇ_ਅੰਤ
	preempt_latency_ਸ਼ੁਰੂ(val);
}
EXPORT_ਚਿੰਨ੍ਹ(preempt_ਗਿਣਤੀ_ਜੋੜੋ);
NOKPROBE_ਚਿੰਨ੍ਹ(preempt_ਗਿਣਤੀ_ਜੋੜੋ);

/*
 * If the value passed in equals to the current preempt count
 * then we just enabled preemption. Stop timing the latency.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ preempt_latency_stop(ਪੂਰਨ val)
{
	ਜੇ (preempt_ਗਿਣਤੀ() == val)
		trace_preempt_on(CALLER_ADDR0, get_ਤਾਲਾ_ਮਾਪਾ_ip());
}

ਖਾਲੀ preempt_ਗਿਣਤੀ_sub(ਪੂਰਨ val)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_DEBUG_PREEMPT
	/*
	 * Underflow?
	 */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ uval = val;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pc = preempt_ਗਿਣਤੀ();

	ਜੇ (DEBUG_LOCKS_WARN_ON(pc - uval > pc))
		ਵਾਪਸ;
	/*
	 * Is the spinlock portion underflowing?
	 */
	ਜੇ (DEBUG_LOCKS_WARN_ON((val < PREEMPT_ਨਕਾਬ) &&
			!(preempt_ਗਿਣਤੀ() & PREEMPT_ਨਕਾਬ)))
		ਵਾਪਸ;
#ਜੇ_ਅੰਤ

	preempt_latency_stop(val);
	__preempt_count_sub(val);
}
EXPORT_ਚਿੰਨ੍ਹ(preempt_ਗਿਣਤੀ_sub);
NOKPROBE_ਚਿੰਨ੍ਹ(preempt_ਗਿਣਤੀ_sub);

#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ preempt_latency_ਸ਼ੁਰੂ(ਪੂਰਨ val) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ preempt_latency_stop(ਪੂਰਨ val) { }
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ get_preempt_disable_ip(ਬਣਤਰ task_struct *p)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_DEBUG_PREEMPT
	ਵਾਪਸ p->preempt_disable_ip;
#ਨਹੀਂਤਾਂ
	ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ
}

/*
 * Print scheduling while atomic bug:
 */
ਸਥਿਰ noinline ਖਾਲੀ __schedule_bug(ਬਣਤਰ task_struct *prev)
{
	/* Save this before calling printk(), since that will clobber it */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ preempt_disable_ip = get_preempt_disable_ip(current);

	ਜੇ (oops_ਵਿੱਚ_ਤਰੱਕੀ)
		ਵਾਪਸ;

	printk(KERN_ERR "BUG: scheduling while atomic: %s/%d/0x%08x\n",
		prev->comm, prev->pid, preempt_ਗਿਣਤੀ());

	debug_show_held_locks(prev);
	print_modules();
	ਜੇ (irqs_disabled())
		print_irqtrace_events(prev);
	ਜੇ (IS_ENABLED(CONFIG_DEBUG_PREEMPT)) {
		pr_err("Preemption disabled at:");
		print_ip_sym(KERN_ERR, preempt_disable_ip);
	}
	check_panic_on_warn("scheduling while atomic");

	dump_ਢੇਰ();
	ਜੋੜੋ_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}

/*
 * Various schedule()-time debugging checks and statistics:
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਸਮਾਂ_ਸਾਰਣੀ_debug(ਬਣਤਰ task_struct *prev, bool preempt)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_ਢੇਰ_ਸਿਰਾ_CHECK
	ਜੇ (task_ਢੇਰ_ਸਿਰਾ_corrupted(prev))
		panic("corrupted stack end detected inside scheduler\n");

	ਜੇ (task_scs_ਸਿਰਾ_corrupted(prev))
		panic("corrupted shadow stack detected inside scheduler\n");
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_DEBUG_ATOMIC_SLEEP
	ਜੇ (!preempt && READ_ONCE(prev->__state) && prev->non_ਖੰਡ_ਗਿਣਤੀ) {
		printk(KERN_ERR "BUG: scheduling in a non-blocking section: %s/%d/%i\n",
			prev->comm, prev->pid, prev->non_ਖੰਡ_ਗਿਣਤੀ);
		dump_ਢੇਰ();
		ਜੋੜੋ_taint(TAINT_WARN, LOCKDEP_STILL_OK);
	}
#ਜੇ_ਅੰਤ

	ਜੇ (unlikely(ਵਿੱਚ_atomic_preempt_off())) {
		__schedule_bug(prev);
		preempt_ਗਿਣਤੀ_ਸੈੱਟ(PREEMPT_DISABLED);
	}
	rcu_sleep_check();
	WARN_ON_ONCE(ct_state() == CT_STATE_USER);

	profile_ਪ੍ਰਾਪਤੀ(SCHED_PROFILING, __builtin_return_address(0));

	schedstat_inc(this_rq()->sched_ਗਿਣਤੀ);
}

ਸਥਿਰ ਖਾਲੀ prev_balance(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
{
	ਅਚਲ ਬਣਤਰ sched_class *ਸ਼ੁਰੂ_class = rq->donor->sched_class;
	ਅਚਲ ਬਣਤਰ sched_class *class;

	/*
	 * We must do the balancing pass before put_prev_task(), such
	 * that when we release the rq->lock the task is in the same
	 * state as before we took rq->lock.
	 *
	 * We can terminate the balance pass as soon as we know there is
	 * a runnable task of @class priority or higher.
	 */
	for_ਸਰਗਰਮ_class_range(class, ਸ਼ੁਰੂ_class, &idle_sched_class) {
		ਜੇ (class->balance && class->balance(rq, rf))
			ਰੋਕੋ;
	}
}

/*
 * Pick up the highest-prio task:
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ task_struct *
__pick_next_task(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	ਅਚਲ ਬਣਤਰ sched_class *class;
	ਬਣਤਰ task_struct *p;

	rq->dl_server = ਨਿਰਾ;

	ਜੇ (scx_enabled())
		ਜਾਓ restart;

	/*
	 * Optimization: we know that if all tasks are in the fair class we can
	 * call that function directly, but only if the @prev task wasn't of a
	 * higher scheduling class, because otherwise those lose the
	 * opportunity to pull in more work from other CPUs.
	 */
	ਜੇ (likely(!sched_class_above(rq->donor->sched_class, &fair_sched_class) &&
		   rq->nr_running == rq->cfs.h_nr_queued)) {

		p = pick_task_fair(rq, rf);
		ਜੇ (unlikely(p == RETRY_TASK))
			ਜਾਓ restart;

		/* Assume the next prioritized class is idle_sched_class */
		ਜੇ (!p)
			p = pick_task_idle(rq, rf);

		put_prev_ਸੈੱਟ_ਅਗਲਾ_task(rq, rq->donor, p);
		ਵਾਪਸ p;
	}

restart:
	prev_balance(rq, rf);

	for_each_ਸਰਗਰਮ_class(class) {
		p = class->pick_task(rq, rf);
		ਜੇ (unlikely(p == RETRY_TASK))
			ਜਾਓ restart;
		ਜੇ (p) {
			put_prev_ਸੈੱਟ_ਅਗਲਾ_task(rq, rq->donor, p);
			ਵਾਪਸ p;
		}
	}

	BUG(); /* The idle class should always have a runnable task. */
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CORE
ਸਥਿਰ ਸਤਰਅੰਦਰ bool is_task_rq_idle(ਬਣਤਰ task_struct *t)
{
	ਵਾਪਸ (task_rq(t)->idle == t);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool cookie_equals(ਬਣਤਰ task_struct *a, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cookie)
{
	ਵਾਪਸ is_task_rq_idle(a) || (a->core_cookie == cookie);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool cookie_ਮੇਲ(ਬਣਤਰ task_struct *a, ਬਣਤਰ task_struct *b)
{
	ਜੇ (is_task_rq_idle(a) || is_task_rq_idle(b))
		ਵਾਪਸ true;

	ਵਾਪਸ a->core_cookie == b->core_cookie;
}

/*
 * Careful; this can return RETRY_TASK, it does not include the retry-loop
 * itself due to the whole SMT pick retry thing below.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ task_struct *pick_task(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
{
	ਅਚਲ ਬਣਤਰ sched_class *class;
	ਬਣਤਰ task_struct *p;

	rq->dl_server = ਨਿਰਾ;

	for_each_ਸਰਗਰਮ_class(class) {
		p = class->pick_task(rq, rf);
		ਜੇ (p)
			ਵਾਪਸ p;
	}

	BUG(); /* The idle class should always have a runnable task. */
}

ਬਾਹਰੀ ਖਾਲੀ task_vruntime_update(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, bool ਵਿੱਚ_fi);

ਸਥਿਰ ਖਾਲੀ ਕਤਾਰ_core_balance(ਬਣਤਰ rq *rq);

ਸਥਿਰ ਬਣਤਰ task_struct *
pick_ਅਗਲਾ_task(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	ਬਣਤਰ task_struct *ਅਗਲਾ, *p, *max;
	ਅਚਲ ਬਣਤਰ cpumask *smt_ਨਕਾਬ;
	bool fi_before = false;
	bool core_clock_updated = (rq == rq->core);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cookie;
	ਪੂਰਨ i, cpu, occ = 0;
	ਬਣਤਰ rq *rq_i;
	bool need_sync = false;

	ਜੇ (!sched_core_enabled(rq))
		ਵਾਪਸ __pick_next_task(rq, rf);

	cpu = cpu_ਦਾ(rq);

	/* Stopper task is switching into idle, no need core-wide selection. */
	ਜੇ (cpu_is_offline(cpu)) {
		/*
		 * Reset core_pick so that we don't enter the fastpath when
		 * coming online. core_pick would already be migrated to
		 * another cpu during offline.
		 */
		rq->core_pick = ਨਿਰਾ;
		rq->core_dl_server = ਨਿਰਾ;
		ਵਾਪਸ __pick_next_task(rq, rf);
	}

	rq->core->core_pick_ਵਿੱਚ_flight++;

	/*
	 * If there were no {en,de}queues since we picked (IOW, the task
	 * pointers are all still valid), and we haven't scheduled the last
	 * pick yet, do so now.
	 *
	 * rq->core_pick can be NULL if no selection was made for a CPU because
	 * it was either offline or went offline during a sibling's core-wide
	 * selection. In this case, do a core-wide selection.
	 */
	ਜੇ (rq->core->core_pick_seq == rq->core->core_task_seq &&
	    rq->core_pick) {
		ਅਗਲਾ = rq->core_pick;
		rq->dl_server = rq->core_dl_server;
		rq->core_pick = ਨਿਰਾ;
		rq->core_dl_server = ਨਿਰਾ;
		ਜਾਓ ਬਾਹਰਲਾ_ਸੈੱਟ_ਅਗਲਾ;
	}

	prev_balance(rq, rf);

	smt_ਨਕਾਬ = cpu_smt_ਨਕਾਬ(cpu);

restart:
	need_sync |= !!rq->core->core_cookie;

	/* reset state */
	rq->core->core_cookie = 0UL;
	ਜੇ (rq->core->core_forceidle_ਗਿਣਤੀ) {
		ਜੇ (!core_clock_updated) {
			update_rq_clock(rq->core);
			core_clock_updated = true;
		}
		sched_core_account_forceidle(rq);
		/* reset after accounting force idle */
		rq->core->core_forceidle_ਸ਼ੁਰੂ = 0;
		rq->core->core_forceidle_ਗਿਣਤੀ = 0;
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
	ਜੇ (!need_sync) {
		ਅਗਲਾ = pick_task(rq, rf);
		ਜੇ (unlikely(ਅਗਲਾ == RETRY_TASK)) {
			/* rq lock may have been dropped, clocks invalidated */
			core_clock_updated = false;
			ਜੇ (!(rq->clock_update_flags & RQCF_UPDATED))
				update_rq_clock(rq);
			ਜਾਓ restart;
		}

		ਜੇ (!ਅਗਲਾ->core_cookie) {
			rq->core_pick = ਨਿਰਾ;
			rq->core_dl_server = ਨਿਰਾ;
			/*
			 * For robustness, update the min_vruntime_fi for
			 * unconstrained picks as well.
			 */
			WARN_ON_ONCE(fi_before);
			task_vruntime_update(rq, ਅਗਲਾ, false);
			ਜਾਓ ਬਾਹਰਲਾ_ਸੈੱਟ_ਅਗਲਾ;
		}
	}

	/*
	 * For each thread: do the regular task pick and find the max prio task
	 * amongst them.
	 *
	 * Tie-break prio towards the current CPU
	 */
	max = ਨਿਰਾ;
	for_each_cpu_wrap(i, smt_ਨਕਾਬ, cpu) {
		rq_i = cpu_rq(i);

		/*
		 * Current cpu always has its clock updated on entrance to
		 * pick_next_task(). If the current cpu is not the core,
		 * the core may also have been updated above.
		 */
		ਜੇ (i != cpu && (rq_i != rq->core || !core_clock_updated))
			update_rq_clock(rq_i);

		p = pick_task(rq_i, rf);
		ਜੇ (unlikely(p == RETRY_TASK)) {
			/* rq lock may have been dropped, clocks invalidated */
			core_clock_updated = false;
			ਜੇ (!(rq->clock_update_flags & RQCF_UPDATED))
				update_rq_clock(rq);
			ਜਾਓ restart;
		}

		rq_i->core_pick = p;
		rq_i->core_dl_server = rq_i->dl_server;

		ਜੇ (!max || prio_less(max, p, fi_before))
			max = p;
	}

	cookie = rq->core->core_cookie = max->core_cookie;

	/*
	 * For each thread: try and find a runnable task that matches @max or
	 * force idle.
	 */
	for_each_cpu(i, smt_ਨਕਾਬ) {
		rq_i = cpu_rq(i);
		p = rq_i->core_pick;

		ਜੇ (!cookie_equals(p, cookie)) {
			p = ਨਿਰਾ;
			ਜੇ (cookie)
				p = sched_core_ਲੱਭੋ(rq_i, cookie);
			ਜੇ (!p)
				p = idle_sched_class.pick_task(rq_i, rf);
		}

		rq_i->core_pick = p;
		rq_i->core_dl_server = ਨਿਰਾ;

		ਜੇ (p == rq_i->idle) {
			ਜੇ (rq_i->nr_running) {
				rq->core->core_forceidle_ਗਿਣਤੀ++;
				ਜੇ (!fi_before)
					rq->core->core_forceidle_seq++;
			}
		} ਨਹੀਂਤਾਂ {
			occ++;
		}
	}

	ਜੇ (schedstat_enabled() && rq->core->core_forceidle_ਗਿਣਤੀ) {
		rq->core->core_forceidle_ਸ਼ੁਰੂ = rq_clock(rq->core);
		rq->core->core_forceidle_occupation = occ;
	}

	rq->core->core_pick_seq = rq->core->core_task_seq;
	ਅਗਲਾ = rq->core_pick;

	/* Something should have been selected for current CPU */
	WARN_ON_ONCE(!ਅਗਲਾ);

	/*
	 * Reschedule siblings
	 *
	 * NOTE: L1TF -- at this point we're no longer running the old task and
	 * sending an IPI (below) ensures the sibling will no longer be running
	 * their task. This ensures there is no inter-sibling overlap between
	 * non-matching user state.
	 */
	for_each_cpu(i, smt_ਨਕਾਬ) {
		rq_i = cpu_rq(i);

		/*
		 * An online sibling might have gone offline before a task
		 * could be picked for it, or it might be offline but later
		 * happen to come online, but its too late and nothing was
		 * picked for it.  That's Ok - it will pick tasks for itself,
		 * so ignore it.
		 */
		ਜੇ (!rq_i->core_pick)
			ਜਾਰੀ;

		/*
		 * Update for new !FI->FI transitions, or if continuing to be in !FI:
		 * fi_before     fi      update?
		 *  0            0       1
		 *  0            1       1
		 *  1            0       1
		 *  1            1       0
		 */
		ਜੇ (!(fi_before && rq->core->core_forceidle_ਗਿਣਤੀ))
			task_vruntime_update(rq_i, rq_i->core_pick, !!rq->core->core_forceidle_ਗਿਣਤੀ);

		rq_i->core_pick->core_occupation = occ;

		ਜੇ (i == cpu) {
			rq_i->core_pick = ਨਿਰਾ;
			rq_i->core_dl_server = ਨਿਰਾ;
			ਜਾਰੀ;
		}

		/* Did we break L1TF mitigation requirements? */
		WARN_ON_ONCE(!cookie_ਮੇਲ(ਅਗਲਾ, rq_i->core_pick));

		ਜੇ (rq_i->curr == rq_i->core_pick) {
			rq_i->core_pick = ਨਿਰਾ;
			rq_i->core_dl_server = ਨਿਰਾ;
			ਜਾਰੀ;
		}

		resched_curr(rq_i);
	}

ਬਾਹਰਲਾ_ਸੈੱਟ_ਅਗਲਾ:
	rq->core->core_pick_ਵਿੱਚ_flight--;
	put_prev_ਸੈੱਟ_ਅਗਲਾ_task(rq, rq->donor, ਅਗਲਾ);
	ਜੇ (rq->core->core_forceidle_ਗਿਣਤੀ && ਅਗਲਾ == rq->idle)
		ਕਤਾਰ_core_balance(rq);

	ਵਾਪਸ ਅਗਲਾ;
}

ਸਥਿਰ bool try_steal_cookie(ਪੂਰਨ this, ਪੂਰਨ that)
{
	ਬਣਤਰ rq *dst = cpu_rq(this), *src = cpu_rq(that);
	ਬਣਤਰ task_struct *p;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cookie;
	bool success = false;

	guard(irq)();
	guard(double_rq_ਤਾਲਾ)(dst, src);

	cookie = dst->core->core_cookie;
	ਜੇ (!cookie)
		ਵਾਪਸ false;

	ਜੇ (dst->curr != dst->idle)
		ਵਾਪਸ false;

	p = sched_core_ਲੱਭੋ(src, cookie);
	ਜੇ (!p)
		ਵਾਪਸ false;

	ਕਰੋ {
		ਜੇ (p == src->core_pick || p == src->curr || p == src->donor)
			ਜਾਓ ਅਗਲਾ;

		ਜੇ (task_is_blocked(p))
			ਜਾਓ ਅਗਲਾ;

		ਜੇ (!is_cpu_allowed(p, this))
			ਜਾਓ ਅਗਲਾ;

		ਜੇ (p->core_occupation > dst->idle->core_occupation)
			ਜਾਓ ਅਗਲਾ;
		/*
		 * sched_core_find() and sched_core_next() will ensure
		 * that task @p is not throttled now, we also need to
		 * check whether the runqueue of the destination CPU is
		 * being throttled.
		 */
		ਜੇ (sched_task_is_throttled(p, this))
			ਜਾਓ ਅਗਲਾ;

		move_queued_task_locked(src, dst, p);
		resched_curr(dst);

		success = true;
		ਰੋਕੋ;

ਅਗਲਾ:
		p = sched_core_ਅਗਲਾ(p, cookie);
	} ਜਦੋਂ (p);

	ਵਾਪਸ success;
}

ਸਥਿਰ bool steal_cookie_task(ਪੂਰਨ cpu, ਬਣਤਰ sched_domain *sd)
{
	ਪੂਰਨ i;

	for_each_cpu_wrap(i, sched_domain_ਵਿਸਤਾਰ(sd), cpu + 1) {
		ਜੇ (i == cpu)
			ਜਾਰੀ;

		ਜੇ (need_resched())
			ਰੋਕੋ;

		ਜੇ (try_steal_cookie(cpu, i))
			ਵਾਪਸ true;
	}

	ਵਾਪਸ false;
}

ਸਥਿਰ ਖਾਲੀ sched_core_balance(ਬਣਤਰ rq *rq)
	__must_hold(__rq_lockp(rq))
{
	ਬਣਤਰ sched_domain *sd;
	ਪੂਰਨ cpu = cpu_ਦਾ(rq);

	guard(preempt)();
	guard(rcu)();

	raw_spin_rq_unlock_irq(rq);
	for_each_domain(cpu, sd) {
		ਜੇ (need_resched())
			ਰੋਕੋ;

		ਜੇ (steal_cookie_task(cpu, sd))
			ਰੋਕੋ;
	}
	raw_spin_rq_ਤਾਲਾ_irq(rq);
}

ਸਥਿਰ DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ balance_callback, core_balance_ਸਿਰ);

ਸਥਿਰ ਖਾਲੀ ਕਤਾਰ_core_balance(ਬਣਤਰ rq *rq)
{
	ਜੇ (!sched_core_enabled(rq))
		ਵਾਪਸ;

	ਜੇ (!rq->core->core_cookie)
		ਵਾਪਸ;

	ਜੇ (!rq->nr_running) /* not forced idle */
		ਵਾਪਸ;

	ਕਤਾਰ_balance_callback(rq, &ਪ੍ਰਤੀ_cpu(core_balance_ਸਿਰ, rq->cpu), sched_core_balance);
}

DEFINE_ਤਾਲਾ_GUARD_1(core_ਤਾਲਾ, ਪੂਰਨ,
		    sched_core_ਤਾਲਾ(*_T->ਤਾਲਾ, &_T->flags),
		    sched_core_unlock(*_T->ਤਾਲਾ, &_T->flags),
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags)

ਸਥਿਰ ਖਾਲੀ sched_core_cpu_starting(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu)
{
	ਅਚਲ ਬਣਤਰ cpumask *smt_ਨਕਾਬ = cpu_smt_ਨਕਾਬ(cpu);
	ਬਣਤਰ rq *rq = cpu_rq(cpu), *core_rq = ਨਿਰਾ;
	ਪੂਰਨ t;

	guard(core_ਤਾਲਾ)(&cpu);

	WARN_ON_ONCE(rq->core != rq);

	/* if we're the first, we'll be our own leader */
	ਜੇ (cpumask_ਭਾਰ(smt_ਨਕਾਬ) == 1)
		ਵਾਪਸ;

	/* find the leader */
	for_each_cpu(t, smt_ਨਕਾਬ) {
		ਜੇ (t == cpu)
			ਜਾਰੀ;
		rq = cpu_rq(t);
		ਜੇ (rq->core == rq) {
			core_rq = rq;
			ਰੋਕੋ;
		}
	}

	ਜੇ (WARN_ON_ONCE(!core_rq)) /* whoopsie */
		ਵਾਪਸ;

	/* install and validate core_rq */
	for_each_cpu(t, smt_ਨਕਾਬ) {
		rq = cpu_rq(t);

		ਜੇ (t == cpu)
			rq->core = core_rq;

		WARN_ON_ONCE(rq->core != core_rq);
	}
}

ਸਥਿਰ ਖਾਲੀ sched_core_cpu_deactivate(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu)
{
	ਅਚਲ ਬਣਤਰ cpumask *smt_ਨਕਾਬ = cpu_smt_ਨਕਾਬ(cpu);
	ਬਣਤਰ rq *rq = cpu_rq(cpu), *core_rq = ਨਿਰਾ;
	ਪੂਰਨ t;

	guard(core_ਤਾਲਾ)(&cpu);

	/* if we're the last man standing, nothing to do */
	ਜੇ (cpumask_ਭਾਰ(smt_ਨਕਾਬ) == 1) {
		WARN_ON_ONCE(rq->core != rq);
		ਵਾਪਸ;
	}

	/* if we're not the leader, nothing to do */
	ਜੇ (rq->core != rq)
		ਵਾਪਸ;

	/* find a new leader */
	for_each_cpu(t, smt_ਨਕਾਬ) {
		ਜੇ (t == cpu)
			ਜਾਰੀ;
		core_rq = cpu_rq(t);
		ਰੋਕੋ;
	}

	ਜੇ (WARN_ON_ONCE(!core_rq)) /* impossible */
		ਵਾਪਸ;

	/* copy the shared state to the new leader */
	core_rq->core_task_seq             = rq->core_task_seq;
	core_rq->core_pick_seq             = rq->core_pick_seq;
	core_rq->core_cookie               = rq->core_cookie;
	core_rq->core_forceidle_ਗਿਣਤੀ      = rq->core_forceidle_ਗਿਣਤੀ;
	core_rq->core_forceidle_seq        = rq->core_forceidle_seq;
	core_rq->core_forceidle_occupation = rq->core_forceidle_occupation;

	/*
	 * A stale leftover would bias the count forever if this CPU later
	 * returns as its own leader. Move, don't copy.
	 */
	core_rq->core_pick_ਵਿੱਚ_flight       = rq->core_pick_ਵਿੱਚ_flight;
	rq->core_pick_ਵਿੱਚ_flight            = 0;

	/*
	 * Accounting edge for forced idle is handled in pick_next_task().
	 * Don't need another one here, since the hotplug thread shouldn't
	 * have a cookie.
	 */
	core_rq->core_forceidle_ਸ਼ੁਰੂ = 0;

	/* install new leader */
	for_each_cpu(t, smt_ਨਕਾਬ) {
		rq = cpu_rq(t);
		rq->core = core_rq;
	}
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_core_cpu_dying(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);

	ਜੇ (rq->core != rq)
		rq->core = rq;
}

#ਨਹੀਂਤਾਂ /* !CONFIG_SCHED_CORE: */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_core_cpu_starting(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_core_cpu_deactivate(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_core_cpu_dying(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu) {}

ਸਥਿਰ ਬਣਤਰ task_struct *
pick_ਅਗਲਾ_task(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	ਵਾਪਸ __pick_next_task(rq, rf);
}

#ਜੇ_ਅੰਤ /* !CONFIG_SCHED_CORE */

/*
 * Constants for the sched_mode argument of __schedule().
 *
 * The mode argument allows RT enabled kernels to differentiate a
 * preemption from blocking on an 'sleeping' spin/rwlock.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SM_IDLE			(-1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SM_NONE			0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SM_PREEMPT		1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SM_RTLOCK_WAIT		2

/*
 * Helper function for __schedule()
 *
 * Tries to deactivate the task, unless the should_block arg
 * is false or if a signal is pending. In the case a signal
 * is pending, marks the task's __state as RUNNING (and clear
 * blocked_on).
 */
ਸਥਿਰ bool try_ਮੰਜ਼ਿਲ_ਖੰਡ_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *task_state_p, bool should_ਖੰਡ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ task_state = *task_state_p;

	WARN_ON_ONCE(p->is_blocked);

	ਜੇ (signal_pending_state(task_state, p)) {
		WRITE_ONCE(p->__state, TASK_RUNNING);
		*task_state_p = TASK_RUNNING;
		clear_task_blocked_on(p, ਨਿਰਾ);

		ਵਾਪਸ false;
	}

	p->is_blocked = 1;

	/*
	 * We check should_block after signal_pending because we
	 * will want to wake the task in that case. But if
	 * should_block is false, its likely due to the task being
	 * blocked on a mutex, and we want to keep it on the runqueue
	 * to be selectable for proxy-execution.
	 */
	ਜੇ (!should_ਖੰਡ)
		ਵਾਪਸ false;

	ਖੰਡ_task(rq, p, task_state);
	ਵਾਪਸ true;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_PROXY_EXEC
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ proxy_ਸੈੱਟ_task_cpu(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ wake_cpu;

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

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ task_struct *proxy_resched_idle(ਬਣਤਰ rq *rq)
{
	put_prev_ਸੈੱਟ_ਅਗਲਾ_task(rq, rq->donor, rq->idle);
	rq->ਅਗਲਾ_class = &idle_sched_class;
	rq_ਸੈੱਟ_donor(rq, rq->idle);
	ਸੈੱਟ_tsk_need_resched(rq->idle);
	ਵਾਪਸ rq->idle;
}

ਸਥਿਰ ਖਾਲੀ proxy_deactivate(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *donor)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ state = READ_ONCE(donor->__state);

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
	ਖੰਡ_task(rq, donor, state);
}

/*
 * Remove a retained proxy donor before changing its scheduler ownership.
 * The caller holds p->pi_lock, so p cannot wake and migrate if block_task()
 * drops it from the runqueue. If DELAY_DEQUEUE keeps a blocked fair task
 * queued, switching_from_fair() completes the dequeue in the immediately
 * following sched_change_begin().
 */
ਖਾਲੀ sched_proxy_ਖੰਡ_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ state = READ_ONCE(p->__state);

	lockdep_assert_held(&p->pi_ਤਾਲਾ);
	lockdep_assert_rq_held(rq);

	ਜੇ (!p->is_blocked || !task_on_rq_queued(p))
		ਵਾਪਸ;
	ਜੇ (WARN_ON_ONCE(state == TASK_RUNNING))
		ਵਾਪਸ;

	ਜੇ (task_current_donor(rq, p))
		proxy_reset_donor(rq);

	ਜੇ (!p->se.sched_delayed)
		ਖੰਡ_task(rq, p, state);

	WARN_ON_ONCE(task_on_rq_queued(p) && !p->se.sched_delayed);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ proxy_release_rq_ਤਾਲਾ(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
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
	rq_unpin_ਤਾਲਾ(rq, rf);
	raw_spin_rq_unlock(rq);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ proxy_reacquire_rq_ਤਾਲਾ(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_ਤਾਲਾ(rq);
	rq_repin_ਤਾਲਾ(rq, rf);
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
ਸਥਿਰ ਖਾਲੀ proxy_migrate_task(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf,
			       ਬਣਤਰ task_struct *p, ਪੂਰਨ ਨਿਸ਼ਾਨਾ_cpu)
	__must_hold(__rq_lockp(rq))
{
	ਬਣਤਰ rq *ਨਿਸ਼ਾਨਾ_rq = cpu_rq(ਨਿਸ਼ਾਨਾ_cpu);
	LIST_ਸਿਰ(migrate_list);

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
	ਲਈ (; p; p = p->blocked_donor) {
		WARN_ON(p == rq->curr);
		deactivate_task(rq, p, ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_NOCLOCK);
		proxy_ਸੈੱਟ_task_cpu(p, ਨਿਸ਼ਾਨਾ_cpu);
		/*
		 * We can re-use se.group_node to migrate the thing,
		 * because @p is deactivated (won't be balanced) and
		 * we hold the rq_lock.
		 */
		list_ਜੋੜੋ(&p->se.ਸਮੂਹ_ਗੰਢ, &migrate_list);
	}
	proxy_release_rq_ਤਾਲਾ(rq, rf);

	__attach_tasks(ਨਿਸ਼ਾਨਾ_rq, &migrate_list);

	proxy_reacquire_rq_ਤਾਲਾ(rq, rf);
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
ਸਥਿਰ ਬਣਤਰ task_struct *
ਲੱਭੋ_proxy_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *donor, ਬਣਤਰ rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	ਬਣਤਰ task_struct *owner = ਨਿਰਾ;
	bool curr_ਵਿੱਚ_chain = false;
	ਪੂਰਨ this_cpu = cpu_ਦਾ(rq);
	ਬਣਤਰ task_struct *p;
	ਪੂਰਨ owner_cpu;

	/* Follow blocked_on chain. */
	ਲਈ (p = donor; p->is_blocked; p = owner) {
		/* if its PROXY_WAKING, do return migration or run if current */
		ਬਣਤਰ mutex *mutex = p->blocked_on;
		ਜੇ (!mutex) {
			clear_task_blocked_on(p, mutex);
			ਜੇ (task_current(rq, p)) {
				p->is_blocked = 0;
				ਵਾਪਸ p;
			}
			ਜਾਓ deactivate;
		}

		/*
		 * By taking mutex->wait_lock we hold off concurrent mutex_unlock()
		 * and ensure @owner sticks around.
		 */
		guard(raw_spinlock)(&mutex->wait_ਤਾਲਾ);
		guard(raw_spinlock)(&p->blocked_ਤਾਲਾ);

		/* Check again that p is blocked with blocked_lock held */
		ਜੇ (mutex != __get_task_blocked_on(p)) {
			/*
			 * Something changed in the blocked_on chain and
			 * we don't know if only at this level. So, let's
			 * just bail out completely and let __schedule()
			 * figure things out (pick_again loop).
			 */
			ਵਾਪਸ ਨਿਰਾ;
		}

		ਜੇ (task_current(rq, p))
			curr_ਵਿੱਚ_chain = true;

		owner = __mutex_owner(mutex);
		ਜੇ (!owner) {
			/*
			 * If there is no owner, either clear blocked_on
			 * and return p (if it is current and safe to
			 * just run on this rq), or return-migrate the task.
			 */
			__clear_task_blocked_on(p, ਨਿਰਾ);
			ਜੇ (task_current(rq, p)) {
				p->is_blocked = 0;
				ਵਾਪਸ p;
			}
			ਜਾਓ deactivate;
		}

		ਜੇ (!READ_ONCE(owner->on_rq) || owner->se.sched_delayed) {
			/* XXX Don't handle blocked owners/delayed dequeue yet */
			ਜੇ (curr_ਵਿੱਚ_chain)
				ਜਾਓ resched_idle;
			__clear_task_blocked_on(p, ਨਿਰਾ);
			ਜਾਓ deactivate;
		}

		owner_cpu = task_cpu(owner);
		ਜੇ (owner_cpu != this_cpu) {
			/*
			 * @owner can disappear, simply migrate to @owner_cpu
			 * and leave that CPU to sort things out.
			 */
			ਜੇ (curr_ਵਿੱਚ_chain)
				ਜਾਓ resched_idle;
			ਜਾਓ migrate_task;
		}

		ਜੇ (task_on_rq_migrating(owner)) {
			/*
			 * One of the chain of mutex owners is currently migrating to this
			 * CPU, but has not yet been enqueued because we are holding the
			 * rq lock. As a simple solution, just schedule rq->idle to give
			 * the migration a chance to complete. Much like the migrate_task
			 * case we should end up back in find_proxy_task(), this time
			 * hopefully with all relevant tasks already enqueued.
			 */
			ਜਾਓ resched_idle;
		}

		/*
		 * Its possible to race where after we check owner->on_rq
		 * but before we check (owner_cpu != this_cpu) that the
		 * task on another cpu was migrated back to this cpu. In
		 * that case it could slip by our  checks. So double check
		 * we are still on this cpu and not migrating. If we get
		 * inconsistent results, try again.
		 */
		ਜੇ (!task_on_rq_queued(owner) || task_cpu(owner) != this_cpu)
			ਵਾਪਸ ਨਿਰਾ;

		ਜੇ (owner == p) {
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
			ਜਾਓ resched_idle;
		}
		/*
		 * OK, now we're absolutely sure @owner is on this
		 * rq, therefore holding @rq->lock is sufficient to
		 * guarantee its existence, as per ttwu_remote().
		 */
		owner->blocked_donor = p;
	}
	WARN_ON_ONCE(owner && !owner->on_rq);

	ਜੇ (owner && !sched_cpu_cookie_ਮੇਲ(rq, owner)) {
		ਜੇ (curr_ਵਿੱਚ_chain)
			ਵਾਪਸ proxy_resched_idle(rq);
		p = donor; /* Deactivate the donor, not the runnable owner */
		clear_task_blocked_on(p, ਨਿਰਾ);
		ਜਾਓ deactivate;
	}
	ਵਾਪਸ owner;

resched_idle:
	ਵਾਪਸ proxy_resched_idle(rq);
deactivate:
	proxy_deactivate(rq, p);
	ਵਾਪਸ ਨਿਰਾ;
migrate_task:
	proxy_migrate_task(rq, rf, p, owner_cpu);
	ਵਾਪਸ ਨਿਰਾ;
}
#ਨਹੀਂਤਾਂ /* SCHED_PROXY_EXEC */
ਸਥਿਰ ਬਣਤਰ task_struct *
ਲੱਭੋ_proxy_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *donor, ਬਣਤਰ rq_flags *rf)
{
	WARN_ONCE(1, "This should never be called in the !SCHED_PROXY_EXEC case\n");
	ਵਾਪਸ donor;
}
#ਜੇ_ਅੰਤ /* SCHED_PROXY_EXEC */

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
ਸਥਿਰ ਖਾਲੀ __sched notrace __schedule(ਪੂਰਨ sched_ਬਹੁਲਕ)
{
	ਬਣਤਰ task_struct *prev, *ਅਗਲਾ;
	/*
	 * On PREEMPT_RT kernel, SM_RTLOCK_WAIT is noted
	 * as a preemption by schedule_debug() and RCU.
	 */
	bool preempt = sched_ਬਹੁਲਕ > SM_NONE;
	bool is_switch = false;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *switch_ਗਿਣਤੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ prev_state;
	ਬਣਤਰ rq_flags rf;
	ਬਣਤਰ rq *rq;
	ਪੂਰਨ cpu;

	/* Trace preemptions consistently with task switches */
	trace_sched_ਇੰਦਰਾਜ_tp(sched_ਬਹੁਲਕ == SM_PREEMPT);

	cpu = smp_processor_id();
	rq = cpu_rq(cpu);
	prev = rq->curr;

	ਸਮਾਂ_ਸਾਰਣੀ_debug(prev, preempt);

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
	rq_ਤਾਲਾ(rq, &rf);
	smp_mb__after_spinlock();

	hrtick_ਸਮਾਂ_ਸਾਰਣੀ_enter(rq);

	/* Promote REQ to ACT */
	rq->clock_update_flags <<= 1;
	update_rq_clock(rq);
	rq->clock_update_flags = RQCF_UPDATED;

	switch_ਗਿਣਤੀ = &prev->nivcsw;

	/* Task state changes only considers SM_PREEMPT as preemption */
	preempt = sched_ਬਹੁਲਕ == SM_PREEMPT;

	/*
	 * We must load prev->state once (task_struct::state is volatile), such
	 * that we form a control dependency vs deactivate_task() below.
	 */
	prev_state = READ_ONCE(prev->__state);
	ਜੇ (sched_ਬਹੁਲਕ == SM_IDLE) {
		/* SCX must consult the BPF scheduler to tell if rq is empty */
		ਜੇ (!rq->nr_running && !scx_enabled()) {
			ਅਗਲਾ = prev;
			rq->ਅਗਲਾ_class = &idle_sched_class;
			ਜਾਓ picked;
		}
	} ਨਹੀਂਤਾਂ ਜੇ (!preempt && prev_state) {
		/*
		 * Keep mutex-blocked tasks on the runqueue for proxy execution
		 * only when their scheduling class allows it. Without proxy
		 * execution, task_is_blocked() always returns false.
		 */
		try_ਮੰਜ਼ਿਲ_ਖੰਡ_task(rq, prev, &prev_state,
				  !task_is_blocked(prev) || !scx_allow_proxy_exec(prev));
		switch_ਗਿਣਤੀ = &prev->nvcsw;
	}

pick_again:
	assert_balance_callbacks_ਖਾਲੀ_ਥਾਵਾਂ(rq);
	ਅਗਲਾ = pick_ਅਗਲਾ_task(rq, &rf);
	rq->ਅਗਲਾ_class = ਅਗਲਾ->sched_class;
	ਜੇ (sched_proxy_exec()) {
		ਬਣਤਰ task_struct *prev_donor = rq->donor;

		rq_ਸੈੱਟ_donor(rq, ਅਗਲਾ);
		ਅਗਲਾ->blocked_donor = ਨਿਰਾ;
		ਜੇ (unlikely(ਅਗਲਾ->is_blocked)) {
			ਅਗਲਾ = ਲੱਭੋ_proxy_task(rq, ਅਗਲਾ, &rf);
			ਜੇ (!ਅਗਲਾ) {
				zap_balance_callbacks(rq);
				ਜਾਓ pick_again;
			}
			ਜੇ (ਅਗਲਾ == rq->idle) {
				zap_balance_callbacks(rq);
				scx_proxy_reenqueue_retry(rq, ਅਗਲਾ);
				ਜਾਓ keep_resched;
			}
		}
		ਜੇ (rq->donor == prev_donor && prev != ਅਗਲਾ) {
			ਬਣਤਰ task_struct *donor = rq->donor;
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
			donor->sched_class->ਸੈੱਟ_ਅਗਲਾ_task(rq, donor, SNT_PICK);
		}
		scx_proxy_donor_ਸ਼ੁਰੂ(rq);
		scx_proxy_reenqueue_retry(rq, ਅਗਲਾ);
	} ਨਹੀਂਤਾਂ {
		rq_ਸੈੱਟ_donor(rq, ਅਗਲਾ);
	}

picked:
	clear_tsk_need_resched(prev);
	clear_preempt_need_resched();
keep_resched:
	rq->ਆਖਰੀ_ਵੇਖਿਆ_need_resched_ns = 0;

	is_switch = prev != ਅਗਲਾ;
	ਜੇ (likely(is_switch)) {
		rq->nr_switches++;
		/*
		 * RCU users of rcu_dereference(rq->curr) may not see
		 * changes to task_struct made by pick_next_task().
		 */
		RCU_INIT_ਸੂਚਕ(rq->curr, ਅਗਲਾ);

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
		++*switch_ਗਿਣਤੀ;

		psi_account_irqtime(rq, prev, ਅਗਲਾ);
		psi_sched_switch(prev, ਅਗਲਾ, !task_on_rq_queued(prev) ||
					     prev->se.sched_delayed);

		trace_sched_switch(preempt, prev, ਅਗਲਾ, prev_state);

		/* Also unlocks the rq: */
		rq = context_switch(rq, prev, ਅਗਲਾ, &rf);
	} ਨਹੀਂਤਾਂ {
		rq_unpin_ਤਾਲਾ(rq, &rf);
		__balance_callbacks(rq, ਨਿਰਾ);
		hrtick_ਸਮਾਂ_ਸਾਰਣੀ_exit(rq);
		raw_spin_rq_unlock_irq(rq);
	}
	trace_sched_exit_tp(is_switch);
}

ਖਾਲੀ __noreturn do_task_dead(ਖਾਲੀ)
{
	/* Causes final put_task_struct in finish_task_switch(): */
	ਸੈੱਟ_special_state(TASK_DEAD);

	/* Tell freezer to ignore us: */
	current->flags |= PF_NOFREEZE;

	__schedule(SM_NONE);
	BUG();

	/* Avoid "noreturn function does return" - but don't continue if BUG() is a NOP: */
	ਲਈ (;;)
		cpu_relax();
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_submit_work(ਬਣਤਰ task_struct *tsk)
{
	ਸਥਿਰ DEFINE_WAIT_OVERRIDE_MAP(sched_map, LD_WAIT_CONFIG);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ task_flags;

	/*
	 * Establish LD_WAIT_CONFIG context to ensure none of the code called
	 * will use a blocking primitive -- which would lead to recursion.
	 */
	ਤਾਲਾ_map_acquire_try(&sched_map);

	task_flags = tsk->flags;
	/*
	 * If a worker goes to sleep, notify and ask workqueue whether it
	 * wants to wake up a task to maintain concurrency.
	 */
	ਜੇ (task_flags & PF_WQ_ਕਾਮਾ)
		wq_ਕਾਮਾ_sleeping(tsk);
	ਨਹੀਂਤਾਂ ਜੇ (task_flags & PF_IO_ਕਾਮਾ)
		io_wq_ਕਾਮਾ_sleeping(tsk);

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

	ਤਾਲਾ_map_release(&sched_map);
}

ਸਥਿਰ ਖਾਲੀ sched_update_ਕਾਮਾ(ਬਣਤਰ task_struct *tsk)
{
	ਜੇ (tsk->flags & (PF_WQ_ਕਾਮਾ | PF_IO_ਕਾਮਾ)) {
		ਜੇ (tsk->flags & PF_WQ_ਕਾਮਾ)
			wq_ਕਾਮਾ_running(tsk);
		ਨਹੀਂਤਾਂ
			io_wq_ਕਾਮਾ_running(tsk);
	}
}

ਸਥਿਰ __always_inline ਖਾਲੀ __schedule_loop(ਪੂਰਨ sched_ਬਹੁਲਕ)
{
	ਕਰੋ {
		preempt_disable();
		__schedule(sched_ਬਹੁਲਕ);
		sched_preempt_enable_no_resched();
	} ਜਦੋਂ (need_resched());
}

asmlinkage __visible ਖਾਲੀ __sched ਸਮਾਂ_ਸਾਰਣੀ(ਖਾਲੀ)
{
	ਬਣਤਰ task_struct *tsk = current;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_MUTEXES
	lockdep_assert(!tsk->sched_rt_mutex);
#ਜੇ_ਅੰਤ

	ਜੇ (!task_is_running(tsk))
		sched_submit_work(tsk);
	__schedule_loop(SM_NONE);
	sched_update_ਕਾਮਾ(tsk);
}
EXPORT_ਚਿੰਨ੍ਹ(ਸਮਾਂ_ਸਾਰਣੀ);

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
ਖਾਲੀ __sched ਸਮਾਂ_ਸਾਰਣੀ_idle(ਖਾਲੀ)
{
	/*
	 * As this skips calling sched_submit_work(), which the idle task does
	 * regardless because that function is a NOP when the task is in a
	 * TASK_RUNNING state, make sure this isn't used someplace that the
	 * current task can be in any other state. Note, idle is always in the
	 * TASK_RUNNING state.
	 */
	WARN_ON_ONCE(current->__state);
	ਕਰੋ {
		__schedule(SM_IDLE);
	} ਜਦੋਂ (need_resched());
}

#ਜੇ defined(CONFIG_CONTEXT_TRACKING_USER) && !defined(CONFIG_HAVE_CONTEXT_TRACKING_USER_OFFSTACK)
asmlinkage __visible ਖਾਲੀ __sched ਸਮਾਂ_ਸਾਰਣੀ_user(ਖਾਲੀ)
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
	ਗਿਣਤੀ ctx_state prev_state = exception_enter();
	ਸਮਾਂ_ਸਾਰਣੀ();
	exception_exit(prev_state);
}
#ਜੇ_ਅੰਤ

/**
 * schedule_preempt_disabled - called with preemption disabled
 *
 * Returns with preemption disabled. Note: preempt_count must be 1
 */
ਖਾਲੀ __sched ਸਮਾਂ_ਸਾਰਣੀ_preempt_disabled(ਖਾਲੀ)
{
	sched_preempt_enable_no_resched();
	ਸਮਾਂ_ਸਾਰਣੀ();
	preempt_disable();
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PREEMPT_RT
ਖਾਲੀ __sched notrace ਸਮਾਂ_ਸਾਰਣੀ_rtlock(ਖਾਲੀ)
{
	__schedule_loop(SM_RTLOCK_WAIT);
}
NOKPROBE_ਚਿੰਨ੍ਹ(ਸਮਾਂ_ਸਾਰਣੀ_rtlock);
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਖਾਲੀ __sched notrace preempt_ਸਮਾਂ_ਸਾਰਣੀ_common(ਖਾਲੀ)
{
	ਕਰੋ {
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
		preempt_latency_ਸ਼ੁਰੂ(1);
		__schedule(SM_PREEMPT);
		preempt_latency_stop(1);
		preempt_enable_no_resched_notrace();

		/*
		 * Check again in case we missed a preemption opportunity
		 * between schedule and now.
		 */
	} ਜਦੋਂ (need_resched());
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PREEMPTION
/*
 * This is the entry point to schedule() from in-kernel preemption
 * off of preempt_enable.
 */
asmlinkage __visible ਖਾਲੀ __sched notrace preempt_ਸਮਾਂ_ਸਾਰਣੀ(ਖਾਲੀ)
{
	/*
	 * If there is a non-zero preempt_count or interrupts are disabled,
	 * we do not want to preempt the current task. Just return..
	 */
	ਜੇ (likely(!preemptible()))
		ਵਾਪਸ;
	preempt_ਸਮਾਂ_ਸਾਰਣੀ_common();
}
NOKPROBE_ਚਿੰਨ੍ਹ(preempt_ਸਮਾਂ_ਸਾਰਣੀ);
EXPORT_ਚਿੰਨ੍ਹ(preempt_ਸਮਾਂ_ਸਾਰਣੀ);

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
asmlinkage __visible ਖਾਲੀ __sched notrace preempt_ਸਮਾਂ_ਸਾਰਣੀ_notrace(ਖਾਲੀ)
{
	ਗਿਣਤੀ ctx_state prev_ctx;

	ਜੇ (likely(!preemptible()))
		ਵਾਪਸ;

	ਕਰੋ {
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
		preempt_latency_ਸ਼ੁਰੂ(1);
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
	} ਜਦੋਂ (need_resched());
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(preempt_ਸਮਾਂ_ਸਾਰਣੀ_notrace);

#ਜੇ_ਅੰਤ /* CONFIG_PREEMPTION */

/*
 * This is the entry point to schedule() from kernel preemption
 * off of IRQ context.
 * Note, that this is called and return with IRQs disabled. This will
 * protect us against recursive calling from IRQ contexts.
 */
asmlinkage __visible ਖਾਲੀ __sched preempt_ਸਮਾਂ_ਸਾਰਣੀ_irq(ਖਾਲੀ)
{
	ਗਿਣਤੀ ctx_state prev_state;

	/* Catch callers which need to be fixed */
	BUG_ON(preempt_ਗਿਣਤੀ() || !irqs_disabled());

	prev_state = exception_enter();

	ਕਰੋ {
		preempt_disable();
		local_irq_enable();
		__schedule(SM_PREEMPT);
		local_irq_disable();
		sched_preempt_enable_no_resched();
	} ਜਦੋਂ (need_resched());

	exception_exit(prev_state);
}

ਪੂਰਨ default_wake_function(wait_ਕਤਾਰ_ਇੰਦਰਾਜ_t *curr, ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ, ਪੂਰਨ wake_flags,
			  ਖਾਲੀ *ਕੁੰਜੀ)
{
	WARN_ON_ONCE(wake_flags & ~(WF_SYNC|WF_CURRENT_CPU));
	ਵਾਪਸ try_ਮੰਜ਼ਿਲ_wake_ਉੱਪਰ(curr->ਨਿਜੀ, ਬਹੁਲਕ, wake_flags);
}
EXPORT_ਚਿੰਨ੍ਹ(default_wake_function);

ਅਚਲ ਬਣਤਰ sched_class *__setscheduler_class(ਪੂਰਨ policy, ਪੂਰਨ prio)
{
	ਜੇ (dl_prio(prio))
		ਵਾਪਸ &dl_sched_class;

	ਜੇ (rt_prio(prio))
		ਵਾਪਸ &rt_sched_class;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CLASS_EXT
	ਜੇ (task_should_scx(policy))
		ਵਾਪਸ &ext_sched_class;
#ਜੇ_ਅੰਤ

	ਵਾਪਸ &fair_sched_class;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_MUTEXES

/*
 * Would be more useful with typeof()/auto_type but they don't mix with
 * bit-fields. Since it's a local thing, use int. Keep the generic sounding
 * name such that if someone were to implement this function we get to compare
 * notes.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ fetch_and_ਸੈੱਟ(x, v) ({ ਪੂਰਨ _x = (x); (x) = (v); _x; })

ਖਾਲੀ rt_mutex_pre_ਸਮਾਂ_ਸਾਰਣੀ(ਖਾਲੀ)
{
	lockdep_assert(!fetch_and_ਸੈੱਟ(current->sched_rt_mutex, 1));
	sched_submit_work(current);
}

/*
 * Used within the futex syscall context, skips sched_submit_work() because none
 * its work will be done. Asserts ensure that it is indeed the case.
 */
ਖਾਲੀ rt_mutex_futex_pre_ਸਮਾਂ_ਸਾਰਣੀ(ਖਾਲੀ)
{
	lockdep_assert(!(current->flags & (PF_WQ_ਕਾਮਾ | PF_IO_ਕਾਮਾ)));
	lockdep_assert(!current->plug);
	lockdep_assert(!fetch_and_ਸੈੱਟ(current->sched_rt_mutex, 1));
}

ਖਾਲੀ rt_mutex_ਸਮਾਂ_ਸਾਰਣੀ(ਖਾਲੀ)
{
	lockdep_assert(current->sched_rt_mutex);
	__schedule_loop(SM_NONE);
}

ਖਾਲੀ rt_mutex_post_ਸਮਾਂ_ਸਾਰਣੀ(ਖਾਲੀ)
{
	sched_update_ਕਾਮਾ(current);
	lockdep_assert(fetch_and_ਸੈੱਟ(current->sched_rt_mutex, 0));
}

ਖਾਲੀ rt_mutex_futex_post_ਸਮਾਂ_ਸਾਰਣੀ(ਖਾਲੀ)
{
	lockdep_assert(fetch_and_ਸੈੱਟ(current->sched_rt_mutex, 0));
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
ਖਾਲੀ rt_mutex_setprio(ਬਣਤਰ task_struct *p, ਬਣਤਰ task_struct *pi_task)
{
	ਪੂਰਨ prio, oldprio, ਕਤਾਰ_flag =
		ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SAVE | ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_MOVE | ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_NOCLOCK;
	ਅਚਲ ਬਣਤਰ sched_class *prev_class, *ਅਗਲਾ_class;
	ਬਣਤਰ rq_flags rf;
	ਬਣਤਰ rq *rq;

	/* XXX used to be waiter->prio, not waiter->task->prio */
	prio = __rt_effective_prio(pi_task, p->ਸਾਧਾਰਨ_prio);

	/*
	 * If nothing changed; bail early.
	 */
	ਜੇ (p->pi_ਸਿਖਰ_task == pi_task && prio == p->prio && !dl_prio(prio))
		ਵਾਪਸ;

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
	p->pi_ਸਿਖਰ_task = pi_task;

	/*
	 * For FIFO/RR we only need to set prio, if that matches we're done.
	 */
	ਜੇ (prio == p->prio && !dl_prio(prio))
		ਜਾਓ ਬਾਹਰਲਾ_unlock;

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
	ਜੇ (unlikely(p == rq->idle)) {
		WARN_ON(p != rq->curr);
		WARN_ON(p->pi_blocked_on);
		ਜਾਓ ਬਾਹਰਲਾ_unlock;
	}

	trace_sched_pi_setprio(p, pi_task);
	oldprio = p->prio;

	ਜੇ (oldprio == prio && !dl_prio(prio))
		ਕਤਾਰ_flag &= ~ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_MOVE;

	prev_class = p->sched_class;
	ਅਗਲਾ_class = __setscheduler_class(p->policy, prio);

	ਜੇ (prev_class != ਅਗਲਾ_class)
		ਕਤਾਰ_flag |= ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_CLASS;

	scoped_guard (sched_change, p, ਕਤਾਰ_flag) {
		/*
		 * Boosting condition are:
		 * 1. -rt task is running and holds mutex A
		 *      --> -dl task blocks on mutex A
		 *
		 * 2. -dl task is running and holds mutex A
		 *      --> -dl task blocks on mutex A and could preempt the
		 *          running task
		 */
		ਜੇ (dl_prio(prio)) {
			ਜੇ (!dl_prio(p->ਸਾਧਾਰਨ_prio) ||
			    (pi_task && dl_prio(pi_task->prio) &&
			     dl_entity_preempt(&pi_task->dl, &p->dl))) {
				p->dl.pi_se = pi_task->dl.pi_se;
				scope->flags |= ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_REPLENISH;
			} ਨਹੀਂਤਾਂ {
				p->dl.pi_se = &p->dl;
			}
		} ਨਹੀਂਤਾਂ ਜੇ (rt_prio(prio)) {
			ਜੇ (dl_prio(oldprio))
				p->dl.pi_se = &p->dl;
			ਜੇ (oldprio < prio)
				scope->flags |= ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_ਸਿਰ;
		} ਨਹੀਂਤਾਂ {
			ਜੇ (dl_prio(oldprio))
				p->dl.pi_se = &p->dl;
			ਜੇ (rt_prio(oldprio))
				p->rt.timeout = 0;
		}

		p->sched_class = ਅਗਲਾ_class;
		p->prio = prio;
	}
ਬਾਹਰਲਾ_unlock:
	/* Caller holds task_struct::pi_lock, IRQs are still disabled */

	__balance_callbacks(rq, &rf);
	__task_rq_unlock(rq, p, &rf);
}
#ਜੇ_ਅੰਤ /* CONFIG_RT_MUTEXES */

#ਜੇ !defined(CONFIG_PREEMPTION)
ਪੂਰਨ __sched __cond_resched(ਖਾਲੀ)
{
	ਜੇ (should_resched(0) && !irqs_disabled()) {
		preempt_ਸਮਾਂ_ਸਾਰਣੀ_common();
		ਵਾਪਸ 1;
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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CONFIG_PREEMPT_RCU
	rcu_all_qs();
#ਜੇ_ਅੰਤ
	ਵਾਪਸ 0;
}
EXPORT_ਚਿੰਨ੍ਹ(__cond_resched);
#ਜੇ_ਅੰਤ

/*
 * __cond_resched_lock() - if a reschedule is pending, drop the given lock,
 * call schedule, and on return reacquire the lock.
 *
 * This works OK both with and without CONFIG_PREEMPTION. We do strange low-level
 * operations here to prevent schedule() from being called twice (once via
 * spin_unlock(), once by hand).
 */
ਪੂਰਨ __cond_resched_lock(spinlock_t *ਤਾਲਾ)
{
	ਪੂਰਨ resched = should_resched(PREEMPT_ਤਾਲਾ_ਵਿੱਥ);
	ਪੂਰਨ ret = 0;

	lockdep_assert_held(ਤਾਲਾ);

	ਜੇ (spin_needbreak(ਤਾਲਾ) || resched) {
		spin_unlock(ਤਾਲਾ);
		ਜੇ (!_cond_resched())
			cpu_relax();
		ret = 1;
		spin_ਤਾਲਾ(ਤਾਲਾ);
	}
	ਵਾਪਸ ret;
}
EXPORT_ਚਿੰਨ੍ਹ(__cond_resched_lock);

ਪੂਰਨ __cond_resched_rwlock_read(rwlock_t *ਤਾਲਾ)
{
	ਪੂਰਨ resched = should_resched(PREEMPT_ਤਾਲਾ_ਵਿੱਥ);
	ਪੂਰਨ ret = 0;

	lockdep_assert_held_read(ਤਾਲਾ);

	ਜੇ (rwlock_needbreak(ਤਾਲਾ) || resched) {
		read_unlock(ਤਾਲਾ);
		ਜੇ (!_cond_resched())
			cpu_relax();
		ret = 1;
		read_ਤਾਲਾ(ਤਾਲਾ);
	}
	ਵਾਪਸ ret;
}
EXPORT_ਚਿੰਨ੍ਹ(__cond_resched_rwlock_read);

ਪੂਰਨ __cond_resched_rwlock_write(rwlock_t *ਤਾਲਾ)
{
	ਪੂਰਨ resched = should_resched(PREEMPT_ਤਾਲਾ_ਵਿੱਥ);
	ਪੂਰਨ ret = 0;

	lockdep_assert_held_write(ਤਾਲਾ);

	ਜੇ (rwlock_needbreak(ਤਾਲਾ) || resched) {
		write_unlock(ਤਾਲਾ);
		ਜੇ (!_cond_resched())
			cpu_relax();
		ret = 1;
		write_ਤਾਲਾ(ਤਾਲਾ);
	}
	ਵਾਪਸ ret;
}
EXPORT_ਚਿੰਨ੍ਹ(__cond_resched_rwlock_write);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PREEMPT_DYNAMIC

# ifdef CONFIG_GENERIC_IRQ_ਇੰਦਰਾਜ
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

ਗਿਣਤੀ {
	preempt_dynamic_undefined = -1,
	preempt_dynamic_ਭਰਿਆ,
	preempt_dynamic_lazy,
};

ਪੂਰਨ preempt_dynamic_ਬਹੁਲਕ = preempt_dynamic_undefined;

ਪੂਰਨ sched_dynamic_ਬਹੁਲਕ(ਅਚਲ ਅੱਖਰ *str)
{
	ਜੇ (!strcmp(str, "full"))
		ਵਾਪਸ preempt_dynamic_ਭਰਿਆ;

	ਜੇ (!strcmp(str, "lazy"))
		ਵਾਪਸ preempt_dynamic_lazy;

	ਵਾਪਸ -EINVAL;
}

# define preempt_dynamic_ਕੁੰਜੀ_enable(f)	static_ਕੁੰਜੀ_enable(&sk_dynamic_##f.ਕੁੰਜੀ)
# define preempt_dynamic_ਕੁੰਜੀ_disable(f)	static_ਕੁੰਜੀ_disable(&sk_dynamic_##f.ਕੁੰਜੀ)

ਸਥਿਰ DEFINE_MUTEX(sched_dynamic_mutex);

ਸਥਿਰ ਖਾਲੀ __sched_dynamic_update(ਪੂਰਨ ਬਹੁਲਕ)
{
	ਚੋਣ (ਬਹੁਲਕ) {
	ਹਾਲਤ preempt_dynamic_ਭਰਿਆ:
		preempt_dynamic_ਕੁੰਜੀ_disable(preempt_lazy);
		ਜੇ (ਬਹੁਲਕ != preempt_dynamic_ਬਹੁਲਕ)
			pr_info("Dynamic Preempt: full\n");
		ਰੋਕੋ;

	ਹਾਲਤ preempt_dynamic_lazy:
		preempt_dynamic_ਕੁੰਜੀ_enable(preempt_lazy);
		ਜੇ (ਬਹੁਲਕ != preempt_dynamic_ਬਹੁਲਕ)
			pr_info("Dynamic Preempt: lazy\n");
		ਰੋਕੋ;
	}

	WRITE_ONCE(preempt_dynamic_ਬਹੁਲਕ, ਬਹੁਲਕ);
}

ਖਾਲੀ sched_dynamic_update(ਪੂਰਨ ਬਹੁਲਕ)
{
	mutex_ਤਾਲਾ(&sched_dynamic_mutex);
	__sched_dynamic_update(ਬਹੁਲਕ);
	mutex_unlock(&sched_dynamic_mutex);
}

ਸਥਿਰ ਪੂਰਨ __init ਤਿਆਰੀ_preempt_ਬਹੁਲਕ(ਅੱਖਰ *str)
{
	ਪੂਰਨ ਬਹੁਲਕ = sched_dynamic_ਬਹੁਲਕ(str);
	ਜੇ (ਬਹੁਲਕ < 0) {
		pr_warn("Dynamic Preempt: unsupported mode: %s\n", str);
		ਵਾਪਸ 0;
	}

	sched_dynamic_update(ਬਹੁਲਕ);
	ਵਾਪਸ 1;
}
__setup("preempt=", ਤਿਆਰੀ_preempt_ਬਹੁਲਕ);

ਸਥਿਰ ਖਾਲੀ __init preempt_dynamic_init(ਖਾਲੀ)
{
	ਜੇ (preempt_dynamic_ਬਹੁਲਕ == preempt_dynamic_undefined) {
		ਜੇ (IS_ENABLED(CONFIG_PREEMPT_LAZY)) {
			sched_dynamic_update(preempt_dynamic_lazy);
		} ਨਹੀਂਤਾਂ {
			/* Default static call setting, nothing to do */
			WARN_ON_ONCE(!IS_ENABLED(CONFIG_PREEMPT));
			preempt_dynamic_ਬਹੁਲਕ = preempt_dynamic_ਭਰਿਆ;
			pr_info("Dynamic Preempt: full\n");
		}
	}
}

# define PREEMPT_MODEL_ACCESSOR(ਬਹੁਲਕ)					\
	bool preempt_model_##mode(ਖਾਲੀ)					\
	{								\
		ਪੂਰਨ ਬਹੁਲਕ = READ_ONCE(preempt_dynamic_ਬਹੁਲਕ);		\
		WARN_ON_ONCE(ਬਹੁਲਕ == preempt_dynamic_undefined);	\
		ਵਾਪਸ ਬਹੁਲਕ == preempt_dynamic_##mode;			\
	}								\
	EXPORT_ਚਿੰਨ੍ਹ_GPL(preempt_model_##mode)

PREEMPT_MODEL_ACCESSOR(ਭਰਿਆ);
PREEMPT_MODEL_ACCESSOR(lazy);

#ਨਹੀਂਤਾਂ /* !CONFIG_PREEMPT_DYNAMIC: */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ preempt_dynamic_ਬਹੁਲਕ -1

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ preempt_dynamic_init(ਖਾਲੀ) { }

#ਜੇ_ਅੰਤ /* CONFIG_PREEMPT_DYNAMIC */

ਅਚਲ ਅੱਖਰ *preempt_modes[] = {
	"full", "lazy", ਨਿਰਾ,
};

ਅਚਲ ਅੱਖਰ *preempt_model_str(ਖਾਲੀ)
{
	bool brace = IS_ENABLED(CONFIG_PREEMPT_RT) &&
		(IS_ENABLED(CONFIG_PREEMPT_DYNAMIC) ||
		 IS_ENABLED(CONFIG_PREEMPT_LAZY));
	ਸਥਿਰ ਅੱਖਰ buf[128];

	ਜੇ (IS_ENABLED(CONFIG_PREEMPT_BUILD)) {
		ਬਣਤਰ seq_buf s;

		seq_buf_init(&s, buf, ਆਕਾਰ(buf));
		seq_buf_puts(&s, "PREEMPT");

		ਜੇ (IS_ENABLED(CONFIG_PREEMPT_RT))
			seq_buf_printf(&s, "%sRT%s",
				       brace ? "_{" : "_",
				       brace ? "," : "");

		ਜੇ (IS_ENABLED(CONFIG_PREEMPT_DYNAMIC)) {
			seq_buf_printf(&s, "(%s)%s",
				       preempt_dynamic_ਬਹੁਲਕ >= 0 ?
				       preempt_modes[preempt_dynamic_ਬਹੁਲਕ] : "undef",
				       brace ? "}" : "");
			ਵਾਪਸ seq_buf_str(&s);
		}

		ਜੇ (IS_ENABLED(CONFIG_PREEMPT_LAZY)) {
			seq_buf_printf(&s, "LAZY%s",
				       brace ? "}" : "");
			ਵਾਪਸ seq_buf_str(&s);
		}

		ਵਾਪਸ seq_buf_str(&s);
	}

	ਜੇ (IS_ENABLED(CONFIG_PREEMPT_VOLUNTARY_BUILD))
		ਵਾਪਸ "VOLUNTARY";

	ਵਾਪਸ "NONE";
}

ਪੂਰਨ io_ਸਮਾਂ_ਸਾਰਣੀ_prepare(ਖਾਲੀ)
{
	ਪੂਰਨ old_iowait = current->ਵਿੱਚ_iowait;

	current->ਵਿੱਚ_iowait = 1;
	blk_flush_plug(current->plug, true);
	ਵਾਪਸ old_iowait;
}

ਖਾਲੀ io_ਸਮਾਂ_ਸਾਰਣੀ_ਅੰਤ(ਪੂਰਨ token)
{
	current->ਵਿੱਚ_iowait = token;
}

/*
 * This task is about to go to sleep on IO. Increment rq->nr_iowait so
 * that process accounting knows that this is a task in IO wait state.
 */
ਲੰਮਾ __sched io_ਸਮਾਂ_ਸਾਰਣੀ_timeout(ਲੰਮਾ timeout)
{
	ਪੂਰਨ token;
	ਲੰਮਾ ret;

	token = io_ਸਮਾਂ_ਸਾਰਣੀ_prepare();
	ret = ਸਮਾਂ_ਸਾਰਣੀ_timeout(timeout);
	io_ਸਮਾਂ_ਸਾਰਣੀ_ਅੰਤ(token);

	ਵਾਪਸ ret;
}
EXPORT_ਚਿੰਨ੍ਹ(io_ਸਮਾਂ_ਸਾਰਣੀ_timeout);

ਖਾਲੀ __sched io_ਸਮਾਂ_ਸਾਰਣੀ(ਖਾਲੀ)
{
	ਪੂਰਨ token;

	token = io_ਸਮਾਂ_ਸਾਰਣੀ_prepare();
	ਸਮਾਂ_ਸਾਰਣੀ();
	io_ਸਮਾਂ_ਸਾਰਣੀ_ਅੰਤ(token);
}
EXPORT_ਚਿੰਨ੍ਹ(io_ਸਮਾਂ_ਸਾਰਣੀ);

ਖਾਲੀ sched_show_task(ਬਣਤਰ task_struct *p)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮੁਕਤ;
	ਪੂਰਨ ppid;

	ਜੇ (!try_get_task_ਢੇਰ(p))
		ਵਾਪਸ;

	pr_info("task:%-15.15s state:%c", p->comm, task_state_ਮੰਜ਼ਿਲ_char(p));

	ਜੇ (task_is_running(p))
		pr_cont("  running task    ");
	ਮੁਕਤ = ਢੇਰ_not_ਵਰਤਿਆ(p);
	ppid = 0;
	rcu_read_ਤਾਲਾ();
	ਜੇ (pid_alive(p))
		ppid = task_pid_nr(rcu_dereference(p->real_ਮਾਪਾ));
	rcu_read_unlock();
	pr_cont(" stack:%-5lu pid:%-5d tgid:%-5d ppid:%-6d task_flags:0x%04x flags:0x%08lx\n",
		ਮੁਕਤ, task_pid_nr(p), task_tgid_nr(p),
		ppid, p->flags, read_task_thread_flags(p));

	print_ਕਾਮਾ_info(KERN_INFO, p);
	print_stop_info(KERN_INFO, p);
	print_scx_info(KERN_INFO, p);
	show_ਢੇਰ(p, ਨਿਰਾ, KERN_INFO);
	put_task_ਢੇਰ(p);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(sched_show_task);

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
state_filter_ਮੇਲ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ state_filter, ਬਣਤਰ task_struct *p)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ state = READ_ONCE(p->__state);

	/* no filter, everything matches */
	ਜੇ (!state_filter)
		ਵਾਪਸ true;

	/* filter, but doesn't match */
	ਜੇ (!(state & state_filter))
		ਵਾਪਸ false;

	/*
	 * When looking for TASK_UNINTERRUPTIBLE skip TASK_IDLE (allows
	 * TASK_KILLABLE).
	 */
	ਜੇ (state_filter == TASK_UNINTERRUPTIBLE && (state & TASK_NOLOAD))
		ਵਾਪਸ false;

	ਵਾਪਸ true;
}


ਖਾਲੀ show_state_filter(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ state_filter)
{
	ਬਣਤਰ task_struct *g, *p;

	rcu_read_ਤਾਲਾ();
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
		ਜੇ (state_filter_ਮੇਲ(state_filter, p))
			sched_show_task(p);
	}

	ਜੇ (!state_filter)
		sysrq_sched_debug_show();

	rcu_read_unlock();
	/*
	 * Only show locks if all tasks are dumped:
	 */
	ਜੇ (!state_filter)
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
ਖਾਲੀ __init init_idle(ਬਣਤਰ task_struct *idle, ਪੂਰਨ cpu)
{
	ਬਣਤਰ affinity_context ac = (ਬਣਤਰ affinity_context) {
		.new_ਨਕਾਬ  = cpumask_ਦਾ(cpu),
		.flags     = 0,
	};
	ਬਣਤਰ rq *rq = cpu_rq(cpu);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	raw_spin_ਤਾਲਾ_irqsave(&idle->pi_ਤਾਲਾ, flags);
	raw_spin_rq_ਤਾਲਾ(rq);

	idle->__state = TASK_RUNNING;
	idle->se.exec_ਸ਼ੁਰੂ = sched_clock();
	/*
	 * PF_KTHREAD should already be set at this point; regardless, make it
	 * look like a proper per-CPU kthread.
	 */
	idle->flags |= PF_KTHREAD | PF_NO_SETAFFINITY;
	kthread_ਸੈੱਟ_ਪ੍ਰਤੀ_cpu(idle, cpu);

	/*
	 * No validation and serialization required at boot time and for
	 * setting up the idle tasks of not yet online CPUs.
	 */
	ਸੈੱਟ_cpus_allowed_common(idle, &ac);
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
	rcu_read_ਤਾਲਾ();
	__set_task_cpu(idle, cpu);
	rcu_read_unlock();

	rq->idle = idle;
	rq_ਸੈੱਟ_donor(rq, idle);
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(rq->curr, idle);
	idle->on_rq = TASK_ON_RQ_QUEUED;
	idle->on_cpu = 1;
	raw_spin_rq_unlock(rq);
	raw_spin_unlock_irqrestore(&idle->pi_ਤਾਲਾ, flags);

	/* Set the preempt count _outside_ the spinlocks! */
	init_idle_preempt_ਗਿਣਤੀ(idle, cpu);

	/*
	 * The idle tasks have their own, simple scheduling class:
	 */
	idle->sched_class = &idle_sched_class;
	ftrace_ਗ੍ਰਾਫ_init_idle_task(idle, cpu);
	vtime_init_idle(idle, cpu);
	sprintf(idle->comm, "%s/%d", INIT_TASK_COMM, cpu);
}

ਪੂਰਨ cpuset_cpumask_can_shrink(ਅਚਲ ਬਣਤਰ cpumask *cur,
			      ਅਚਲ ਬਣਤਰ cpumask *trial)
{
	ਪੂਰਨ ret = 1;

	ਜੇ (cpumask_ਖਾਲੀ_ਥਾਵਾਂ(cur))
		ਵਾਪਸ ret;

	ret = dl_cpuset_cpumask_can_shrink(cur, trial);

	ਵਾਪਸ ret;
}

ਪੂਰਨ task_can_attach(ਬਣਤਰ task_struct *p)
{
	ਪੂਰਨ ret = 0;

	/*
	 * Kthreads which disallow setaffinity shouldn't be moved
	 * to a new cpuset; we don't want to change their CPU
	 * affinity and isolating such threads by their set of
	 * allowed nodes is unnecessary.  Thus, cpusets are not
	 * applicable for such threads.  This prevents checking for
	 * success of set_cpus_allowed_ptr() on all attached tasks
	 * before cpus_mask may be changed.
	 */
	ਜੇ (p->flags & PF_NO_SETAFFINITY)
		ret = -EINVAL;

	ਵਾਪਸ ret;
}

bool sched_smp_initialized __read_mostly;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
/* Migrate current task p to target_cpu */
ਪੂਰਨ migrate_task_ਮੰਜ਼ਿਲ(ਬਣਤਰ task_struct *p, ਪੂਰਨ ਨਿਸ਼ਾਨਾ_cpu)
{
	ਬਣਤਰ migration_arg arg = { p, ਨਿਸ਼ਾਨਾ_cpu };
	ਪੂਰਨ curr_cpu = task_cpu(p);

	ਜੇ (curr_cpu == ਨਿਸ਼ਾਨਾ_cpu)
		ਵਾਪਸ 0;

	ਜੇ (!cpumask_test_cpu(ਨਿਸ਼ਾਨਾ_cpu, p->cpus_ptr))
		ਵਾਪਸ -EINVAL;

	/* TODO: This is not properly updating schedstats */

	trace_sched_move_numa(p, curr_cpu, ਨਿਸ਼ਾਨਾ_cpu);
	ਵਾਪਸ stop_one_cpu(curr_cpu, migration_cpu_stop, &arg);
}

/*
 * Requeue a task on a given node and accurately track the number of NUMA
 * tasks on the runqueues
 */
ਖਾਲੀ sched_setnuma(ਬਣਤਰ task_struct *p, ਪੂਰਨ nid)
{
	guard(task_rq_ਤਾਲਾ_2)(p);
	scoped_guard (sched_change, p, ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SAVE)
		p->numa_preferred_nid = nid;
}
#ਜੇ_ਅੰਤ /* CONFIG_NUMA_BALANCING */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_HOTPLUG_CPU
/*
 * Invoked on the outgoing CPU in context of the CPU hotplug thread
 * after ensuring that there are no user space tasks left on the CPU.
 *
 * If there is a lazy mm in use on the hotplug thread, drop it and
 * switch to init_mm.
 *
 * The reference count on init_mm is dropped in finish_cpu().
 */
ਸਥਿਰ ਖਾਲੀ sched_force_init_mm(ਖਾਲੀ)
{
	ਬਣਤਰ mm_struct *mm = current->ਸਰਗਰਮ_mm;

	ਜੇ (mm != &init_mm) {
		mmgrab_lazy_tlb(&init_mm);
		local_irq_disable();
		current->ਸਰਗਰਮ_mm = &init_mm;
		switch_mm_irqs_off(mm, &init_mm, current);
		local_irq_enable();
		ਅੰਤ_arch_post_ਤਾਲਾ_switch();
		mmdrop_lazy_tlb(mm);
	}

	/* finish_cpu(), as ran on the BP, will clean up the active_mm state */
}

ਸਥਿਰ ਪੂਰਨ __balance_push_cpu_stop(ਖਾਲੀ *arg)
{
	ਬਣਤਰ task_struct *p = arg;
	ਬਣਤਰ rq *rq = this_rq();
	ਬਣਤਰ rq_flags rf;
	ਪੂਰਨ cpu;

	scoped_guard (raw_spinlock_irq, &p->pi_ਤਾਲਾ) {
		/*
		 * We may change the underlying rq, but the locks held will
		 * appropriately be "transferred" when switching.
		 */
		context_unsafe_alias(rq);

		cpu = select_fallback_rq(rq->cpu, p);

		rq_ਤਾਲਾ(rq, &rf);
		update_rq_clock(rq);
		ਜੇ (task_rq(p) == rq && task_on_rq_queued(p))
			rq = __migrate_task(rq, &rf, p, cpu);
		rq_unlock(rq, &rf);
	}

	put_task_struct(p);

	ਵਾਪਸ 0;
}

ਸਥਿਰ DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ cpu_stop_work, ਧੱਕੋ_work);

/*
 * Ensure we only run per-cpu kthreads once the CPU goes !active.
 *
 * This is enabled below SCHED_AP_ACTIVE; when !cpu_active(), but only
 * effective when the hotplug motion is down.
 */
ਸਥਿਰ ਖਾਲੀ balance_ਧੱਕੋ(ਬਣਤਰ rq *rq)
	__must_hold(__rq_lockp(rq))
{
	ਬਣਤਰ task_struct *ਧੱਕੋ_task = rq->curr;

	lockdep_assert_rq_held(rq);

	/*
	 * Ensure the thing is persistent until balance_push_set(.on = false);
	 */
	rq->balance_callback = &balance_ਧੱਕੋ_callback;

	/*
	 * Only active while going offline and when invoked on the outgoing
	 * CPU.
	 */
	ਜੇ (!cpu_dying(rq->cpu) || rq != this_rq())
		ਵਾਪਸ;

	/*
	 * Both the cpu-hotplug and stop task are in this case and are
	 * required to complete the hotplug process.
	 */
	ਜੇ (kthread_is_ਪ੍ਰਤੀ_cpu(ਧੱਕੋ_task) ||
	    is_migration_disabled(ਧੱਕੋ_task)) {

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
		ਜੇ (!rq->nr_running && !rq_has_pinned_tasks(rq) &&
		    rcuwait_ਸਰਗਰਮ(&rq->hotplug_wait)) {
			raw_spin_rq_unlock(rq);
			rcuwait_wake_ਉੱਪਰ(&rq->hotplug_wait);
			raw_spin_rq_ਤਾਲਾ(rq);
		}
		ਵਾਪਸ;
	}

	get_task_struct(ਧੱਕੋ_task);
	/*
	 * Temporarily drop rq->lock such that we can wake-up the stop task.
	 * Both preemption and IRQs are still disabled.
	 */
	preempt_disable();
	raw_spin_rq_unlock(rq);
	stop_one_cpu_nowait(rq->cpu, __balance_push_cpu_stop, ਧੱਕੋ_task,
			    this_cpu_ptr(&ਧੱਕੋ_work));
	preempt_enable();
	/*
	 * At this point need_resched() is true and we'll take the loop in
	 * schedule(). The next pick is obviously going to be the stop task
	 * which kthread_is_per_cpu() and will push this task away.
	 */
	raw_spin_rq_ਤਾਲਾ(rq);
}

ਸਥਿਰ ਖਾਲੀ balance_ਧੱਕੋ_ਸੈੱਟ(ਪੂਰਨ cpu, bool on)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);
	ਬਣਤਰ rq_flags rf;

	rq_ਤਾਲਾ_irqsave(rq, &rf);
	ਜੇ (on) {
		WARN_ON_ONCE(rq->balance_callback);
		rq->balance_callback = &balance_ਧੱਕੋ_callback;
	} ਨਹੀਂਤਾਂ ਜੇ (rq->balance_callback == &balance_ਧੱਕੋ_callback) {
		rq->balance_callback = ਨਿਰਾ;
	}
	rq_unlock_irqrestore(rq, &rf);
}

/*
 * Invoked from a CPUs hotplug control thread after the CPU has been marked
 * inactive. All tasks which are not per CPU kernel threads are either
 * pushed off this CPU now via balance_push() or placed on a different CPU
 * during wakeup. Wait until the CPU is quiescent.
 */
ਸਥਿਰ ਖਾਲੀ balance_hotplug_wait(ਖਾਲੀ)
{
	ਬਣਤਰ rq *rq = this_rq();

	rcuwait_wait_event(&rq->hotplug_wait,
			   rq->nr_running == 1 && !rq_has_pinned_tasks(rq),
			   TASK_UNINTERRUPTIBLE);
}

#ਨਹੀਂਤਾਂ /* !CONFIG_HOTPLUG_CPU: */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ balance_ਧੱਕੋ(ਬਣਤਰ rq *rq)
{
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ balance_ਧੱਕੋ_ਸੈੱਟ(ਪੂਰਨ cpu, bool on)
{
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ balance_hotplug_wait(ਖਾਲੀ)
{
}

#ਜੇ_ਅੰਤ /* !CONFIG_HOTPLUG_CPU */

ਖਾਲੀ ਸੈੱਟ_rq_online(ਬਣਤਰ rq *rq)
{
	ਜੇ (!rq->online) {
		ਅਚਲ ਬਣਤਰ sched_class *class;

		cpumask_ਸੈੱਟ_cpu(rq->cpu, rq->rd->online);
		rq->online = 1;

		for_each_class(class) {
			ਜੇ (class->rq_online)
				class->rq_online(rq);
		}
	}
}

ਖਾਲੀ ਸੈੱਟ_rq_offline(ਬਣਤਰ rq *rq)
{
	ਜੇ (rq->online) {
		ਅਚਲ ਬਣਤਰ sched_class *class;

		update_rq_clock(rq);
		for_each_class(class) {
			ਜੇ (class->rq_offline)
				class->rq_offline(rq);
		}

		cpumask_clear_cpu(rq->cpu, rq->rd->online);
		rq->online = 0;
	}
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_ਸੈੱਟ_rq_online(ਬਣਤਰ rq *rq, ਪੂਰਨ cpu)
{
	ਬਣਤਰ rq_flags rf;

	rq_ਤਾਲਾ_irqsave(rq, &rf);
	ਜੇ (rq->rd) {
		BUG_ON(!cpumask_test_cpu(cpu, rq->rd->ਵਿਸਤਾਰ));
		ਸੈੱਟ_rq_online(rq);
	}
	rq_unlock_irqrestore(rq, &rf);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_ਸੈੱਟ_rq_offline(ਬਣਤਰ rq *rq, ਪੂਰਨ cpu)
{
	ਬਣਤਰ rq_flags rf;

	rq_ਤਾਲਾ_irqsave(rq, &rf);
	ਜੇ (rq->rd) {
		BUG_ON(!cpumask_test_cpu(cpu, rq->rd->ਵਿਸਤਾਰ));
		ਸੈੱਟ_rq_offline(rq);
	}
	rq_unlock_irqrestore(rq, &rf);
}

/*
 * used to mark begin/end of suspend/resume:
 */
ਸਥਿਰ ਪੂਰਨ ਅੰਕ_cpus_frozen;

/*
 * Update cpusets according to cpu_active mask.  If cpusets are
 * disabled, cpuset_update_active_cpus() becomes a simple wrapper
 * around partition_sched_domains().
 *
 * If we come here as part of a suspend/resume, don't touch cpusets because we
 * want to restore it back to its original state upon resume anyway.
 */
ਸਥਿਰ ਖਾਲੀ cpuset_cpu_ਸਰਗਰਮ(ਖਾਲੀ)
{
	ਜੇ (cpuhp_tasks_frozen) {
		/*
		 * num_cpus_frozen tracks how many CPUs are involved in suspend
		 * resume sequence. As long as this is not the last online
		 * operation in the resume sequence, just build a single sched
		 * domain, ignoring cpusets.
		 */
		cpuset_reset_sched_domains();
		ਜੇ (--ਅੰਕ_cpus_frozen)
			ਵਾਪਸ;
		/*
		 * This is the last CPU online operation. So fall through and
		 * restore the original sched domains by considering the
		 * cpuset configurations.
		 */
		cpuset_force_rebuild();
	}
	cpuset_update_ਸਰਗਰਮ_cpus();
}

ਸਥਿਰ ਖਾਲੀ cpuset_cpu_inactive(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu)
{
	ਜੇ (!cpuhp_tasks_frozen) {
		cpuset_update_ਸਰਗਰਮ_cpus();
	} ਨਹੀਂਤਾਂ {
		ਅੰਕ_cpus_frozen++;
		cpuset_reset_sched_domains();
	}
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_smt_present_inc(ਪੂਰਨ cpu)
{
	ਜੇ (cpumask_ਭਾਰ(cpu_smt_ਨਕਾਬ(cpu)) == 2)
		static_branch_inc_cpuslocked(&sched_smt_present);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_smt_present_dec(ਪੂਰਨ cpu)
{
	ਜੇ (cpumask_ਭਾਰ(cpu_smt_ਨਕਾਬ(cpu)) == 2)
		static_branch_dec_cpuslocked(&sched_smt_present);
}

ਪੂਰਨ sched_cpu_activate(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);

	/*
	 * Clear the balance_push callback and prepare to schedule
	 * regular tasks.
	 */
	balance_ਧੱਕੋ_ਸੈੱਟ(cpu, false);

	/*
	 * When going up, increment the number of cores with SMT present.
	 */
	sched_smt_present_inc(cpu);
	ਸੈੱਟ_cpu_ਸਰਗਰਮ(cpu, true);

	ਜੇ (sched_smp_initialized) {
		sched_update_numa(cpu, true);
		sched_domains_numa_masks_ਸੈੱਟ(cpu);
		cpuset_cpu_ਸਰਗਰਮ();
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
	sched_ਸੈੱਟ_rq_online(rq, cpu);

	/* preferred is subset of active and follows its state */
	ਸੈੱਟ_cpu_preferred(cpu, true);

	ਵਾਪਸ 0;
}

ਪੂਰਨ sched_cpu_deactivate(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);
	ਪੂਰਨ ret;

	ret = dl_bw_deactivate(cpu);

	ਜੇ (ret)
		ਵਾਪਸ ret;

	ਸੈੱਟ_cpu_preferred(cpu, false);

	/*
	 * Remove CPU from nohz.idle_cpus_mask to prevent participating in
	 * load balancing when not active
	 */
	scoped_guard (rcu)
		nohz_balance_exit_idle(rq);

	ਸੈੱਟ_cpu_ਸਰਗਰਮ(cpu, false);

	/*
	 * From this point forward, this CPU will refuse to run any task that
	 * is not: migrate_disable() or KTHREAD_IS_PER_CPU, and will actively
	 * push those tasks away until this gets cleared, see
	 * sched_cpu_dying().
	 */
	balance_ਧੱਕੋ_ਸੈੱਟ(cpu, true);

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

	sched_ਸੈੱਟ_rq_offline(rq, cpu);

	scx_rq_deactivate(rq);

	/*
	 * When going down, decrement the number of cores with SMT present.
	 */
	sched_smt_present_dec(cpu);

	sched_core_cpu_deactivate(cpu);

	ਜੇ (!sched_smp_initialized)
		ਵਾਪਸ 0;

	sched_update_numa(cpu, false);
	cpuset_cpu_inactive(cpu);
	sched_domains_numa_masks_clear(cpu);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ sched_rq_cpu_starting(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);

	rq->calc_ਲੱਦੋ_update = calc_ਲੱਦੋ_update;
	update_max_interval();
}

ਪੂਰਨ sched_cpu_starting(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu)
{
	sched_core_cpu_starting(cpu);
	sched_rq_cpu_starting(cpu);
	sched_tick_ਸ਼ੁਰੂ(cpu);
	ਵਾਪਸ 0;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_HOTPLUG_CPU

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
ਪੂਰਨ sched_cpu_wait_ਖਾਲੀ_ਥਾਵਾਂ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu)
{
	balance_hotplug_wait();
	sched_force_init_mm();
	ਵਾਪਸ 0;
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
ਸਥਿਰ ਖਾਲੀ calc_ਲੱਦੋ_migrate(ਬਣਤਰ rq *rq)
{
	ਲੰਮਾ delta = calc_ਲੱਦੋ_fold_ਸਰਗਰਮ(rq, 1);

	ਜੇ (delta)
		atomic_long_ਜੋੜੋ(delta, &calc_ਲੱਦੋ_tasks);
}

ਸਥਿਰ ਖਾਲੀ dump_rq_tasks(ਬਣਤਰ rq *rq, ਅਚਲ ਅੱਖਰ *loglvl)
{
	ਬਣਤਰ task_struct *g, *p;
	ਪੂਰਨ cpu = cpu_ਦਾ(rq);

	lockdep_assert_rq_held(rq);

	printk("%sCPU%d enqueued tasks (%u total):\n", loglvl, cpu, rq->nr_running);
	for_each_process_thread(g, p) {
		ਜੇ (task_cpu(p) != cpu)
			ਜਾਰੀ;

		ਜੇ (!task_on_rq_queued(p))
			ਜਾਰੀ;

		printk("%s\tpid: %d, name: %s\n", loglvl, p->pid, p->comm);
	}
}

ਪੂਰਨ sched_cpu_dying(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);
	ਬਣਤਰ rq_flags rf;

	/* Handle pending wakeups and then migrate everything off */
	sched_tick_stop(cpu);

	rq_ਤਾਲਾ_irqsave(rq, &rf);
	update_rq_clock(rq);
	ਜੇ (rq->nr_running != 1 || rq_has_pinned_tasks(rq)) {
		WARN(true, "Dying CPU not properly vacated!");
		dump_rq_tasks(rq, KERN_WARNING);
	}
	dl_server_stop(&rq->fair_server);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CLASS_EXT
	dl_server_stop(&rq->ext_server);
#ਜੇ_ਅੰਤ
	rq_unlock_irqrestore(rq, &rf);

	calc_ਲੱਦੋ_migrate(rq);
	update_max_interval();
	hrtick_clear(rq);
	sched_core_cpu_dying(cpu);
	ਵਾਪਸ 0;
}
#ਜੇ_ਅੰਤ /* CONFIG_HOTPLUG_CPU */

ਖਾਲੀ __init sched_init_smp(ਖਾਲੀ)
{
	sched_init_numa(NUMA_NO_ਗੰਢ);

	prandom_init_once(&sched_rnd_state);

	/*
	 * There's no userspace yet to cause hotplug operations; hence all the
	 * CPU masks are stable and all blatant races in the below code cannot
	 * happen.
	 */
	sched_domains_mutex_ਤਾਲਾ();
	sched_init_domains(cpu_ਸਰਗਰਮ_ਨਕਾਬ);
	sched_domains_mutex_unlock();

	/* Move init over to a non-isolated CPU */
	ਜੇ (ਸੈੱਟ_cpus_allowed_ptr(current, housekeeping_cpumask(HK_TYPE_DOMAIN)) < 0)
		BUG();
	current->flags &= ~PF_NO_SETAFFINITY;
	sched_init_granularity();

	init_sched_rt_class();
	init_sched_dl_class();

	sched_init_dl_servers();

	sched_smp_initialized = true;
}

ਸਥਿਰ ਪੂਰਨ __init migration_init(ਖਾਲੀ)
{
	sched_cpu_starting(smp_processor_id());
	ਵਾਪਸ 0;
}
early_initcall(migration_init);

ਪੂਰਨ ਵਿੱਚ_sched_functions(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ addr)
{
	ਵਾਪਸ ਵਿੱਚ_ਤਾਲਾ_functions(addr) ||
		(addr >= (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)__sched_text_start
		&& addr < (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)__sched_text_end);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CGROUP_SCHED
/*
 * Default task group.
 * Every task in system belongs to this group at bootup.
 */
ਬਣਤਰ task_ਸਮੂਹ ਜੜ੍ਹ_task_ਸਮੂਹ;
LIST_ਸਿਰ(task_groups);

/* Cacheline aligned slab cache for task_group */
ਸਥਿਰ ਬਣਤਰ kmem_cache *task_ਸਮੂਹ_cache __ro_after_init;
#ਜੇ_ਅੰਤ

ਖਾਲੀ __init sched_init(ਖਾਲੀ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ __maybe_unused ptr = 0;
	ਪੂਰਨ i;

	/* Make sure the linker didn't screw up */
	BUG_ON(!sched_class_above(&stop_sched_class, &dl_sched_class));
	BUG_ON(!sched_class_above(&dl_sched_class, &rt_sched_class));
	BUG_ON(!sched_class_above(&rt_sched_class, &fair_sched_class));
	BUG_ON(!sched_class_above(&fair_sched_class, &idle_sched_class));
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CLASS_EXT
	BUG_ON(!sched_class_above(&fair_sched_class, &ext_sched_class));
	BUG_ON(!sched_class_above(&ext_sched_class, &idle_sched_class));
#ਜੇ_ਅੰਤ

	wait_ਦੋਆਧਾਰੀ_ਅੰਕ_init();

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
	ਜੜ੍ਹ_task_ਸਮੂਹ.cfs_rq = &runqueues.cfs;

	ਜੜ੍ਹ_task_ਸਮੂਹ.shares = ਜੜ੍ਹ_TASK_ਸਮੂਹ_ਲੱਦੋ;
	init_cfs_bandwidth(&ਜੜ੍ਹ_task_ਸਮੂਹ.cfs_bandwidth, ਨਿਰਾ);
#ਜੇ_ਅੰਤ /* CONFIG_FAIR_GROUP_SCHED */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_ਸਮੂਹ_SCHED
	scx_tg_init(&ਜੜ੍ਹ_task_ਸਮੂਹ);
#ਜੇ_ਅੰਤ /* CONFIG_EXT_GROUP_SCHED */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED
	ptr += 2 * nr_cpu_ids * ਆਕਾਰ(ਖਾਲੀ **);
	ptr = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)kzalloc(ptr, GFP_NOWAIT);
	ਜੜ੍ਹ_task_ਸਮੂਹ.rt_se = (ਬਣਤਰ sched_rt_entity **)ptr;
	ptr += nr_cpu_ids * ਆਕਾਰ(ਖਾਲੀ **);

	ਜੜ੍ਹ_task_ਸਮੂਹ.rt_rq = (ਬਣਤਰ rt_rq **)ptr;
	ptr += nr_cpu_ids * ਆਕਾਰ(ਖਾਲੀ **);

#ਜੇ_ਅੰਤ /* CONFIG_RT_GROUP_SCHED */

	init_defrootdomain();

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED
	init_rt_bandwidth(&ਜੜ੍ਹ_task_ਸਮੂਹ.rt_bandwidth,
			global_rt_period(), global_rt_runtime());
#ਜੇ_ਅੰਤ /* CONFIG_RT_GROUP_SCHED */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CGROUP_SCHED
	task_ਸਮੂਹ_cache = KMEM_CACHE(task_ਸਮੂਹ, 0);

	list_ਜੋੜੋ(&ਜੜ੍ਹ_task_ਸਮੂਹ.list, &task_groups);
	INIT_LIST_ਸਿਰ(&ਜੜ੍ਹ_task_ਸਮੂਹ.children);
	INIT_LIST_ਸਿਰ(&ਜੜ੍ਹ_task_ਸਮੂਹ.siblings);
	autogroup_init(&init_task);
#ਜੇ_ਅੰਤ /* CONFIG_CGROUP_SCHED */

	for_each_possible_cpu(i) {
		ਬਣਤਰ rq *rq;

		rq = cpu_rq(i);
		raw_spin_ਤਾਲਾ_init(&rq->__lock);
		rq->nr_running = 0;
		rq->calc_ਲੱਦੋ_ਸਰਗਰਮ = 0;
		rq->calc_ਲੱਦੋ_update = jiffies + ਲੱਦੋ_FREQ;
		init_cfs_rq(&rq->cfs);
		init_rt_rq(&rq->rt);
		init_dl_rq(&rq->dl);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
		INIT_LIST_ਸਿਰ(&rq->ਪੱਤੀ_cfs_rq_list);
		rq->tmp_alone_branch = &rq->ਪੱਤੀ_cfs_rq_list;
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
		init_tg_cfs_ਇੰਦਰਾਜ(&ਜੜ੍ਹ_task_ਸਮੂਹ, &rq->cfs, ਨਿਰਾ, i, ਨਿਰਾ);
#ਜੇ_ਅੰਤ /* CONFIG_FAIR_GROUP_SCHED */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED
		/*
		 * This is required for init cpu because rt.c:__enable_runtime()
		 * starts working after scheduler_running, which is not the case
		 * yet.
		 */
		rq->rt.rt_runtime = global_rt_runtime();
		init_tg_rt_ਇੰਦਰਾਜ(&ਜੜ੍ਹ_task_ਸਮੂਹ, &rq->rt, ਨਿਰਾ, i, ਨਿਰਾ);
#ਜੇ_ਅੰਤ
		rq->ਅਗਲਾ_class = &idle_sched_class;

		rq->sd = ਨਿਰਾ;
		rq->rd = ਨਿਰਾ;
		rq->cpu_ਸਮਰੱਥਾ = SCHED_ਸਮਰੱਥਾ_SCALE;
		rq->balance_callback = &balance_ਧੱਕੋ_callback;
		rq->ਸਰਗਰਮ_balance = 0;
		rq->ਅਗਲਾ_balance = jiffies;
		rq->ਧੱਕੋ_cpu = 0;
		rq->cpu = i;
		rq->online = 0;
		rq->idle_stamp = 0;
		rq->avg_idle = 2*sysctl_sched_migration_ਲਾਗਤ;
		rq->max_idle_balance_ਲਾਗਤ = sysctl_sched_migration_ਲਾਗਤ;

		INIT_LIST_ਸਿਰ(&rq->cfs_tasks);

		rq_attach_ਜੜ੍ਹ(rq, &def_ਜੜ੍ਹ_domain);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NO_HZ_COMMON
		rq->ਆਖਰੀ_blocked_ਲੱਦੋ_update_tick = jiffies;
		atomic_ਸੈੱਟ(&rq->nohz_flags, 0);

		INIT_CSD(&rq->nohz_csd, nohz_csd_func, rq);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_HOTPLUG_CPU
		rcuwait_init(&rq->hotplug_wait);
#ਜੇ_ਅੰਤ
		hrtick_rq_init(rq);
		atomic_ਸੈੱਟ(&rq->nr_iowait, 0);
		fair_server_init(rq);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CLASS_EXT
		ext_server_init(rq);
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CORE
		rq->core = rq;
		rq->core_pick = ਨਿਰਾ;
		rq->core_dl_server = ਨਿਰਾ;
		rq->core_enabled = 0;
		rq->core_ਰੁੱਖ = RB_ਜੜ੍ਹ;
		rq->core_forceidle_ਗਿਣਤੀ = 0;
		rq->core_forceidle_occupation = 0;
		rq->core_forceidle_ਸ਼ੁਰੂ = 0;
		rq->core_pick_ਵਿੱਚ_flight = 0;

		rq->core_cookie = 0UL;
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CACHE
		raw_spin_ਤਾਲਾ_init(&rq->cpu_epoch_ਤਾਲਾ);
		rq->cpu_epoch_ਅਗਲਾ = jiffies;
#ਜੇ_ਅੰਤ

		zalloc_cpumask_var_ਗੰਢ(&rq->scratch_ਨਕਾਬ, GFP_KERNEL, cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(i));
	}

	ਸੈੱਟ_ਲੱਦੋ_ਭਾਰ(&init_task, false);
	init_task.se.ਟੁਕੜਾ = sysctl_sched_ਅਧਾਰ_ਟੁਕੜਾ,

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
	WARN_ON(!ਸੈੱਟ_kthread_struct(current));

	/*
	 * Make us the idle thread. Technically, schedule() should not be
	 * called from this thread, however somewhere below it might be,
	 * but because we are the idle thread, we just pick up running again
	 * when this runqueue becomes "idle".
	 */
	__sched_fork(0, current);
	init_idle(current, smp_processor_id());

	calc_ਲੱਦੋ_update = jiffies + ਲੱਦੋ_FREQ;

	idle_thread_ਸੈੱਟ_boot_cpu();

	balance_ਧੱਕੋ_ਸੈੱਟ(smp_processor_id(), false);
	init_sched_fair_class();
	init_sched_ext_class();

	psi_init();

	init_uclamp();

	preempt_dynamic_init();

	scheduler_running = 1;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_DEBUG_ATOMIC_SLEEP

ਖਾਲੀ __might_sleep(ਅਚਲ ਅੱਖਰ *file, ਪੂਰਨ ਸਤਰ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ state = get_current_state();
	/*
	 * Blocking primitives will set (and therefore destroy) current->state,
	 * since we will exit with TASK_RUNNING make sure we enter with it,
	 * otherwise we will destroy state.
	 */
	WARN_ONCE(state != TASK_RUNNING && current->task_state_change,
			"do not call blocking ops when !TASK_RUNNING; "
			"state=%x set at [<%p>] %pS\n", state,
			(ਖਾਲੀ *)current->task_state_change,
			(ਖਾਲੀ *)current->task_state_change);

	__might_resched(file, ਸਤਰ, 0);
}
EXPORT_ਚਿੰਨ੍ਹ(__might_sleep);

ਸਥਿਰ ਖਾਲੀ print_preempt_disable_ip(ਪੂਰਨ preempt_ਵਿੱਥ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ip)
{
	ਜੇ (!IS_ENABLED(CONFIG_DEBUG_PREEMPT))
		ਵਾਪਸ;

	ਜੇ (preempt_ਗਿਣਤੀ() == preempt_ਵਿੱਥ)
		ਵਾਪਸ;

	pr_err("Preemption disabled at:");
	print_ip_sym(KERN_ERR, ip);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool resched_offsets_ok(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ offsets)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nested = preempt_ਗਿਣਤੀ();

	nested += rcu_preempt_ਡੂੰਘਾਈ() << MIGHT_RESCHED_RCU_ਖਿਸਕਾਓ;

	ਵਾਪਸ nested == offsets;
}

ਖਾਲੀ __might_resched(ਅਚਲ ਅੱਖਰ *file, ਪੂਰਨ ਸਤਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ offsets)
{
	/* Ratelimiting timestamp: */
	ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ prev_jiffy;

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ preempt_disable_ip;

	/* WARN_ON_ONCE() by default, no rate limit required: */
	rcu_sleep_check();

	ਜੇ ((resched_offsets_ok(offsets) && !irqs_disabled() &&
	     !is_idle_task(current) && !current->non_ਖੰਡ_ਗਿਣਤੀ) ||
	    system_state == SYSTEM_BOOTING || system_state > SYSTEM_RUNNING ||
	    oops_ਵਿੱਚ_ਤਰੱਕੀ)
		ਵਾਪਸ;

	ਜੇ (ਸਮਾਂ_before(jiffies, prev_jiffy + HZ) && prev_jiffy)
		ਵਾਪਸ;
	prev_jiffy = jiffies;

	/* Save this before calling printk(), since that will clobber it: */
	preempt_disable_ip = get_preempt_disable_ip(current);

	pr_err("BUG: sleeping function called from invalid context at %s:%d\n",
	       file, ਸਤਰ);
	pr_err("in_atomic(): %d, irqs_disabled(): %d, non_block: %d, pid: %d, name: %s\n",
	       ਵਿੱਚ_atomic(), irqs_disabled(), current->non_ਖੰਡ_ਗਿਣਤੀ,
	       current->pid, current->comm);
	pr_err("preempt_count: %x, expected: %x\n", preempt_ਗਿਣਤੀ(),
	       offsets & MIGHT_RESCHED_PREEMPT_ਨਕਾਬ);

	ਜੇ (IS_ENABLED(CONFIG_PREEMPT_RCU)) {
		pr_err("RCU nest depth: %d, expected: %u\n",
		       rcu_preempt_ਡੂੰਘਾਈ(), offsets >> MIGHT_RESCHED_RCU_ਖਿਸਕਾਓ);
	}

	ਜੇ (task_ਢੇਰ_ਸਿਰਾ_corrupted(current))
		pr_emerg("Thread overran stack, or stack corrupted\n");

	debug_show_held_locks(current);
	ਜੇ (irqs_disabled())
		print_irqtrace_events(current);

	print_preempt_disable_ip(offsets & MIGHT_RESCHED_PREEMPT_ਨਕਾਬ,
				 preempt_disable_ip);

	dump_ਢੇਰ();
	ਜੋੜੋ_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}
EXPORT_ਚਿੰਨ੍ਹ(__might_resched);

ਖਾਲੀ __cant_sleep(ਅਚਲ ਅੱਖਰ *file, ਪੂਰਨ ਸਤਰ)
{
	ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ prev_jiffy;

	ਜੇ (irqs_disabled())
		ਵਾਪਸ;

	ਜੇ (!IS_ENABLED(CONFIG_PREEMPT_ਗਿਣਤੀ))
		ਵਾਪਸ;

	ਜੇ (preempt_ਗਿਣਤੀ())
		ਵਾਪਸ;

	ਜੇ (ਸਮਾਂ_before(jiffies, prev_jiffy + HZ) && prev_jiffy)
		ਵਾਪਸ;
	prev_jiffy = jiffies;

	printk(KERN_ERR "BUG: assuming atomic context at %s:%d\n", file, ਸਤਰ);
	printk(KERN_ERR "in_atomic(): %d, irqs_disabled(): %d, pid: %d, name: %s\n",
			ਵਿੱਚ_atomic(), irqs_disabled(),
			current->pid, current->comm);

	debug_show_held_locks(current);
	dump_ਢੇਰ();
	ਜੋੜੋ_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(__cant_sleep);

# ifdef CONFIG_SMP
ਖਾਲੀ __cant_migrate(ਅਚਲ ਅੱਖਰ *file, ਪੂਰਨ ਸਤਰ)
{
	ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ prev_jiffy;

	ਜੇ (irqs_disabled())
		ਵਾਪਸ;

	ਜੇ (is_migration_disabled(current))
		ਵਾਪਸ;

	ਜੇ (!IS_ENABLED(CONFIG_PREEMPT_ਗਿਣਤੀ))
		ਵਾਪਸ;

	ਜੇ (preempt_ਗਿਣਤੀ())
		ਵਾਪਸ;

	ਜੇ (ਸਮਾਂ_before(jiffies, prev_jiffy + HZ) && prev_jiffy)
		ਵਾਪਸ;
	prev_jiffy = jiffies;

	pr_err("BUG: assuming non migratable context at %s:%d\n", file, ਸਤਰ);
	pr_err("in_atomic(): %d, irqs_disabled(): %d, migration_disabled() %u pid: %d, name: %s\n",
	       ਵਿੱਚ_atomic(), irqs_disabled(), is_migration_disabled(current),
	       current->pid, current->comm);

	debug_show_held_locks(current);
	dump_ਢੇਰ();
	ਜੋੜੋ_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(__cant_migrate);
# endif /* CONFIG_SMP */
#ਜੇ_ਅੰਤ /* CONFIG_DEBUG_ATOMIC_SLEEP */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_MAGIC_SYSRQ
ਖਾਲੀ normalize_rt_tasks(ਖਾਲੀ)
{
	ਬਣਤਰ task_struct *g, *p;
	ਬਣਤਰ sched_attr attr = {
		.sched_policy = SCHED_ਸਾਧਾਰਨ,
	};

	read_ਤਾਲਾ(&tasklist_ਤਾਲਾ);
	for_each_process_thread(g, p) {
		/*
		 * Only normalize user tasks:
		 */
		ਜੇ (p->flags & PF_KTHREAD)
			ਜਾਰੀ;

		p->se.exec_ਸ਼ੁਰੂ = 0;
		schedstat_ਸੈੱਟ(p->stats.wait_ਸ਼ੁਰੂ,  0);
		schedstat_ਸੈੱਟ(p->stats.sleep_ਸ਼ੁਰੂ, 0);
		schedstat_ਸੈੱਟ(p->stats.ਖੰਡ_ਸ਼ੁਰੂ, 0);

		ਜੇ (!rt_or_dl_task(p)) {
			/*
			 * Renice negative nice level userspace
			 * tasks back to 0:
			 */
			ਜੇ (task_nice(p) < 0)
				ਸੈੱਟ_user_nice(p, 0);
			ਜਾਰੀ;
		}

		__sched_setscheduler(p, &attr, false, false);
	}
	read_unlock(&tasklist_ਤਾਲਾ);
}

#ਜੇ_ਅੰਤ /* CONFIG_MAGIC_SYSRQ */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_KGDB_KDB
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
ਬਣਤਰ task_struct *curr_task(ਪੂਰਨ cpu)
{
	ਵਾਪਸ cpu_curr(cpu);
}

#ਜੇ_ਅੰਤ /* CONFIG_KGDB_KDB */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CGROUP_SCHED
/* task_group_lock serializes the addition/removal of task groups */
ਸਥਿਰ DEFINE_SPINLOCK(task_ਸਮੂਹ_ਤਾਲਾ);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ alloc_uclamp_sched_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg,
					    ਬਣਤਰ task_ਸਮੂਹ *ਮਾਪਾ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK_ਸਮੂਹ
	ਗਿਣਤੀ uclamp_id clamp_id;

	for_each_clamp_id(clamp_id) {
		uclamp_se_ਸੈੱਟ(&tg->uclamp_req[clamp_id],
			      uclamp_none(clamp_id), false);
		tg->uclamp[clamp_id] = ਮਾਪਾ->uclamp[clamp_id];
	}
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਖਾਲੀ sched_free_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	free_fair_sched_ਸਮੂਹ(tg);
	free_rt_sched_ਸਮੂਹ(tg);
	autogroup_free(tg);
	kmem_cache_free(task_ਸਮੂਹ_cache, tg);
}

ਸਥਿਰ ਖਾਲੀ sched_free_ਸਮੂਹ_rcu(ਬਣਤਰ rcu_ਸਿਰ *rcu)
{
	sched_free_ਸਮੂਹ(container_ਦਾ(rcu, ਬਣਤਰ task_ਸਮੂਹ, rcu));
}

ਸਥਿਰ ਖਾਲੀ sched_unregister_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	unregister_fair_sched_ਸਮੂਹ(tg);
	unregister_rt_sched_ਸਮੂਹ(tg);
	/*
	 * We have to wait for yet another RCU grace period to expire, as
	 * print_cfs_stats() might run concurrently.
	 */
	call_rcu(&tg->rcu, sched_free_ਸਮੂਹ_rcu);
}

/* allocate runqueue etc for a new task group */
ਬਣਤਰ task_ਸਮੂਹ *sched_create_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *ਮਾਪਾ)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg;

	tg = kmem_cache_alloc(task_ਸਮੂਹ_cache, GFP_KERNEL | __GFP_ZERO);
	ਜੇ (!tg)
		ਵਾਪਸ ERR_PTR(-ENOMEM);

	ਜੇ (!alloc_fair_sched_ਸਮੂਹ(tg, ਮਾਪਾ))
		ਜਾਓ err;

	ਜੇ (!alloc_rt_sched_ਸਮੂਹ(tg, ਮਾਪਾ))
		ਜਾਓ err;

	scx_tg_init(tg);
	alloc_uclamp_sched_ਸਮੂਹ(tg, ਮਾਪਾ);

	ਵਾਪਸ tg;

err:
	sched_free_ਸਮੂਹ(tg);
	ਵਾਪਸ ERR_PTR(-ENOMEM);
}

ਖਾਲੀ sched_online_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg, ਬਣਤਰ task_ਸਮੂਹ *ਮਾਪਾ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	spin_ਤਾਲਾ_irqsave(&task_ਸਮੂਹ_ਤਾਲਾ, flags);
	list_ਜੋੜੋ_ਪੂਛ_rcu(&tg->list, &task_groups);

	/* Root should already exist: */
	WARN_ON(!ਮਾਪਾ);

	tg->ਮਾਪਾ = ਮਾਪਾ;
	INIT_LIST_ਸਿਰ(&tg->children);
	list_ਜੋੜੋ_rcu(&tg->siblings, &ਮਾਪਾ->children);
	spin_unlock_irqrestore(&task_ਸਮੂਹ_ਤਾਲਾ, flags);

	online_fair_sched_ਸਮੂਹ(tg);
}

/* RCU callback to free various structures associated with a task group */
ਸਥਿਰ ਖਾਲੀ sched_unregister_ਸਮੂਹ_rcu(ਬਣਤਰ rcu_ਸਿਰ *rhp)
{
	/* Now it should be safe to free those cfs_rqs: */
	sched_unregister_ਸਮੂਹ(container_ਦਾ(rhp, ਬਣਤਰ task_ਸਮੂਹ, rcu));
}

ਖਾਲੀ sched_destroy_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	/* Wait for possible concurrent references to cfs_rqs complete: */
	call_rcu(&tg->rcu, sched_unregister_ਸਮੂਹ_rcu);
}

ਖਾਲੀ sched_release_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

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
	spin_ਤਾਲਾ_irqsave(&task_ਸਮੂਹ_ਤਾਲਾ, flags);
	list_del_rcu(&tg->list);
	list_del_rcu(&tg->siblings);
	spin_unlock_irqrestore(&task_ਸਮੂਹ_ਤਾਲਾ, flags);
}

ਸਥਿਰ ਖਾਲੀ sched_change_ਸਮੂਹ(ਬਣਤਰ task_struct *tsk)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg;

	/*
	 * All callers are synchronized by task_rq_lock(); we do not use RCU
	 * which is pointless here. Thus, we pass "true" to task_css_check()
	 * to prevent lockdep warnings.
	 */
	tg = container_ਦਾ(task_css_check(tsk, cpu_cgrp_id, true),
			  ਬਣਤਰ task_ਸਮੂਹ, css);
	tg = autogroup_task_ਸਮੂਹ(tsk, tg);
	tsk->sched_task_ਸਮੂਹ = tg;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
	ਜੇ (tsk->sched_class->task_change_ਸਮੂਹ)
		tsk->sched_class->task_change_ਸਮੂਹ(tsk);
	ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
		ਸੈੱਟ_task_rq(tsk, task_cpu(tsk));
}

/*
 * Change task's runqueue when it moves between groups.
 *
 * The caller of this function should have put the task in its new group by
 * now. This function just updates tsk->se.cfs_rq and tsk->se.parent to reflect
 * its new group.
 */
ਖਾਲੀ sched_move_task(ਬਣਤਰ task_struct *tsk, bool for_autogroup)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਕਤਾਰ_flags = ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SAVE | ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_MOVE;
	bool resched = false;
	bool queued = false;
	ਬਣਤਰ rq *rq;

	CLASS(task_rq_ਤਾਲਾ_2, rq_guard)(tsk);
	rq = rq_guard.rq;

	scoped_guard (sched_change, tsk, ਕਤਾਰ_flags) {
		sched_change_ਸਮੂਹ(tsk);
		ਜੇ (!for_autogroup)
			scx_cgroup_move_task(tsk);
		ਜੇ (scope->running)
			resched = true;
		queued = scope->queued;
	}

	ਜੇ (resched)
		resched_curr(rq);
	ਨਹੀਂਤਾਂ ਜੇ (queued)
		wakeup_preempt(rq, tsk, 0);

	__balance_callbacks(rq, &rq_guard.rf);
}

ਸਥਿਰ ਬਣਤਰ cgroup_subsys_state *
cpu_cgroup_css_alloc(ਬਣਤਰ cgroup_subsys_state *ਮਾਪਾ_css)
{
	ਬਣਤਰ task_ਸਮੂਹ *ਮਾਪਾ = css_tg(ਮਾਪਾ_css);
	ਬਣਤਰ task_ਸਮੂਹ *tg;

	ਜੇ (!ਮਾਪਾ) {
		/* This is early initialization for the top cgroup */
		ਵਾਪਸ &ਜੜ੍ਹ_task_ਸਮੂਹ.css;
	}

	tg = sched_create_ਸਮੂਹ(ਮਾਪਾ);
	ਜੇ (IS_ERR(tg))
		ਵਾਪਸ ERR_PTR(-ENOMEM);

	ਵਾਪਸ &tg->css;
}

/* Expose task group only after completing cgroup initialization */
ਸਥਿਰ ਪੂਰਨ cpu_cgroup_css_online(ਬਣਤਰ cgroup_subsys_state *css)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg = css_tg(css);
	ਬਣਤਰ task_ਸਮੂਹ *ਮਾਪਾ = css_tg(css->ਮਾਪਾ);
	ਪੂਰਨ ret;

	ret = scx_tg_online(tg);
	ਜੇ (ret)
		ਵਾਪਸ ret;

	ਜੇ (ਮਾਪਾ)
		sched_online_ਸਮੂਹ(tg, ਮਾਪਾ);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK_ਸਮੂਹ
	/* Propagate the effective uclamp value for the new group */
	guard(mutex)(&uclamp_mutex);
	guard(rcu)();
	cpu_util_update_eff(css);
#ਜੇ_ਅੰਤ

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ cpu_cgroup_css_offline(ਬਣਤਰ cgroup_subsys_state *css)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg = css_tg(css);

	scx_tg_offline(tg);
}

ਸਥਿਰ ਖਾਲੀ cpu_cgroup_css_released(ਬਣਤਰ cgroup_subsys_state *css)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg = css_tg(css);

	sched_release_ਸਮੂਹ(tg);
}

ਸਥਿਰ ਖਾਲੀ cpu_cgroup_css_free(ਬਣਤਰ cgroup_subsys_state *css)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg = css_tg(css);

	/*
	 * Relies on the RCU grace period between css_released() and this.
	 */
	sched_unregister_ਸਮੂਹ(tg);
}

ਸਥਿਰ ਪੂਰਨ cpu_cgroup_can_attach(ਬਣਤਰ cgroup_taskset *tset)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED
	ਬਣਤਰ task_struct *task;
	ਬਣਤਰ cgroup_subsys_state *css;

	ਜੇ (!rt_ਸਮੂਹ_sched_enabled())
		ਜਾਓ scx_check;

	cgroup_taskset_for_each(task, css, tset) {
		ਜੇ (!sched_rt_can_attach(css_tg(css), task))
			ਵਾਪਸ -EINVAL;
	}
scx_check:
#ਜੇ_ਅੰਤ /* CONFIG_RT_GROUP_SCHED */
	ਵਾਪਸ scx_cgroup_can_attach(tset);
}

ਸਥਿਰ ਖਾਲੀ cpu_cgroup_attach(ਬਣਤਰ cgroup_taskset *tset)
{
	ਬਣਤਰ task_struct *task;
	ਬਣਤਰ cgroup_subsys_state *css;

	cgroup_taskset_for_each(task, css, tset)
		sched_move_task(task, false);
}

ਸਥਿਰ ਖਾਲੀ cpu_cgroup_cancel_attach(ਬਣਤਰ cgroup_taskset *tset)
{
	scx_cgroup_cancel_attach(tset);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK_ਸਮੂਹ
ਸਥਿਰ ਖਾਲੀ cpu_util_update_eff(ਬਣਤਰ cgroup_subsys_state *css)
{
	ਬਣਤਰ cgroup_subsys_state *ਸਿਖਰ_css = css;
	ਬਣਤਰ uclamp_se *uc_ਮਾਪਾ = ਨਿਰਾ;
	ਬਣਤਰ uclamp_se *uc_se = ਨਿਰਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ eff[UCLAMP_CNT];
	ਗਿਣਤੀ uclamp_id clamp_id;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ clamps;

	lockdep_assert_held(&uclamp_mutex);
	WARN_ON_ONCE(!rcu_read_ਤਾਲਾ_held());

	css_for_each_descendant_pre(css, ਸਿਖਰ_css) {
		uc_ਮਾਪਾ = css_tg(css)->ਮਾਪਾ
			? css_tg(css)->ਮਾਪਾ->uclamp : ਨਿਰਾ;

		for_each_clamp_id(clamp_id) {
			/* Assume effective clamps matches requested clamps */
			eff[clamp_id] = css_tg(css)->uclamp_req[clamp_id].ਮੁੱਲ;
			/* Cap effective clamps with parent's effective clamps */
			ਜੇ (uc_ਮਾਪਾ &&
			    eff[clamp_id] > uc_ਮਾਪਾ[clamp_id].ਮੁੱਲ) {
				eff[clamp_id] = uc_ਮਾਪਾ[clamp_id].ਮੁੱਲ;
			}
		}
		/* Ensure protection is always capped by limit */
		eff[UCLAMP_MIN] = min(eff[UCLAMP_MIN], eff[UCLAMP_MAX]);

		/* Propagate most restrictive effective clamps */
		clamps = 0x0;
		uc_se = css_tg(css)->uclamp;
		for_each_clamp_id(clamp_id) {
			ਜੇ (eff[clamp_id] == uc_se[clamp_id].ਮੁੱਲ)
				ਜਾਰੀ;
			uc_se[clamp_id].ਮੁੱਲ = eff[clamp_id];
			uc_se[clamp_id].ਬਾਲਟੀ_id = uclamp_ਬਾਲਟੀ_id(eff[clamp_id]);
			clamps |= (0x1 << clamp_id);
		}
		ਜੇ (!clamps) {
			css = css_rightmost_descendant(css);
			ਜਾਰੀ;
		}

		/* Immediately update descendants RUNNABLE tasks */
		uclamp_update_ਸਰਗਰਮ_tasks(css);
	}
}

/*
 * Integer 10^N with a given N exponent by casting to integer the literal "1eN"
 * C expression. Since there is no way to convert a macro argument (N) into a
 * character constant, use two levels of macros.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _POW10(ਘਾਤਾਂਕ) ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)1e##exp)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ POW10(ਘਾਤਾਂਕ) _POW10(ਘਾਤਾਂਕ)

ਬਣਤਰ uclamp_request {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UCLAMP_PERCENT_ਖਿਸਕਾਓ	2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UCLAMP_PERCENT_SCALE	(100 * POW10(UCLAMP_PERCENT_ਖਿਸਕਾਓ))
	s64 percent;
	u64 util;
	ਪੂਰਨ ret;
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ uclamp_request
ਸਮਰੱਥਾ_from_percent(ਅੱਖਰ *buf)
{
	ਬਣਤਰ uclamp_request req = {
		.percent = UCLAMP_PERCENT_SCALE,
		.util = SCHED_ਸਮਰੱਥਾ_SCALE,
		.ret = 0,
	};

	buf = strim(buf);
	ਜੇ (strcmp(buf, "max")) {
		req.ret = cgroup_parse_float(buf, UCLAMP_PERCENT_ਖਿਸਕਾਓ,
					     &req.percent);
		ਜੇ (req.ret)
			ਵਾਪਸ req;
		ਜੇ ((u64)req.percent > UCLAMP_PERCENT_SCALE) {
			req.ret = -ERANGE;
			ਵਾਪਸ req;
		}

		req.util = req.percent << SCHED_ਸਮਰੱਥਾ_ਖਿਸਕਾਓ;
		req.util = DIV_ਗੇੜਾ_CLOSEST_ULL(req.util, UCLAMP_PERCENT_SCALE);
	}

	ਵਾਪਸ req;
}

ਸਥਿਰ ssize_t cpu_uclamp_write(ਬਣਤਰ kernfs_open_file *ਦਾ, ਅੱਖਰ *buf,
				ਮਾਪ_t nbytes, loff_t off,
				ਗਿਣਤੀ uclamp_id clamp_id)
{
	ਬਣਤਰ uclamp_request req;
	ਬਣਤਰ task_ਸਮੂਹ *tg;

	req = ਸਮਰੱਥਾ_from_percent(buf);
	ਜੇ (req.ret)
		ਵਾਪਸ req.ret;

	sched_uclamp_enable();

	guard(mutex)(&uclamp_mutex);
	guard(rcu)();

	tg = css_tg(ਦਾ_css(ਦਾ));
	ਜੇ (tg->uclamp_req[clamp_id].ਮੁੱਲ != req.util)
		uclamp_se_ਸੈੱਟ(&tg->uclamp_req[clamp_id], req.util, false);

	/*
	 * Because of not recoverable conversion rounding we keep track of the
	 * exact requested value
	 */
	tg->uclamp_pct[clamp_id] = req.percent;

	/* Update effective clamps to track the most restrictive value */
	cpu_util_update_eff(ਦਾ_css(ਦਾ));

	ਵਾਪਸ nbytes;
}

ਸਥਿਰ ssize_t cpu_uclamp_min_write(ਬਣਤਰ kernfs_open_file *ਦਾ,
				    ਅੱਖਰ *buf, ਮਾਪ_t nbytes,
				    loff_t off)
{
	ਵਾਪਸ cpu_uclamp_write(ਦਾ, buf, nbytes, off, UCLAMP_MIN);
}

ਸਥਿਰ ssize_t cpu_uclamp_max_write(ਬਣਤਰ kernfs_open_file *ਦਾ,
				    ਅੱਖਰ *buf, ਮਾਪ_t nbytes,
				    loff_t off)
{
	ਵਾਪਸ cpu_uclamp_write(ਦਾ, buf, nbytes, off, UCLAMP_MAX);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ cpu_uclamp_print(ਬਣਤਰ seq_file *sf,
				    ਗਿਣਤੀ uclamp_id clamp_id)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg;
	u64 util_clamp;
	u64 percent;
	u32 rem;

	scoped_guard (rcu) {
		tg = css_tg(seq_css(sf));
		util_clamp = tg->uclamp_req[clamp_id].ਮੁੱਲ;
	}

	ਜੇ (util_clamp == SCHED_ਸਮਰੱਥਾ_SCALE) {
		seq_puts(sf, "max\n");
		ਵਾਪਸ;
	}

	percent = tg->uclamp_pct[clamp_id];
	percent = div_u64_rem(percent, POW10(UCLAMP_PERCENT_ਖਿਸਕਾਓ), &rem);
	seq_printf(sf, "%llu.%0*u\n", percent, UCLAMP_PERCENT_ਖਿਸਕਾਓ, rem);
}

ਸਥਿਰ ਪੂਰਨ cpu_uclamp_min_show(ਬਣਤਰ seq_file *sf, ਖਾਲੀ *v)
{
	cpu_uclamp_print(sf, UCLAMP_MIN);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ cpu_uclamp_max_show(ਬਣਤਰ seq_file *sf, ਖਾਲੀ *v)
{
	cpu_uclamp_print(sf, UCLAMP_MAX);
	ਵਾਪਸ 0;
}
#ਜੇ_ਅੰਤ /* CONFIG_UCLAMP_TASK_GROUP */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_ਸਮੂਹ_SCHED_ਭਾਰ
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ tg_ਭਾਰ(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
	ਵਾਪਸ scale_ਲੱਦੋ_ਥੱਲੇ(tg->shares);
#ਨਹੀਂਤਾਂ
	ਵਾਪਸ sched_ਭਾਰ_from_cgroup(tg->scx.ਭਾਰ);
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਪੂਰਨ cpu_shares_write_u64(ਬਣਤਰ cgroup_subsys_state *css,
				ਬਣਤਰ cftype *cftype, u64 shareval)
{
	ਪੂਰਨ ret;

	ਜੇ (shareval > scale_ਲੱਦੋ_ਥੱਲੇ(ULONG_MAX))
		shareval = MAX_SHARES;
	ret = sched_ਸਮੂਹ_ਸੈੱਟ_shares(css_tg(css), scale_ਲੱਦੋ(shareval));
	ਜੇ (!ret)
		scx_ਸਮੂਹ_ਸੈੱਟ_ਭਾਰ(css_tg(css),
				     sched_ਭਾਰ_ਮੰਜ਼ਿਲ_cgroup(shareval));
	ਵਾਪਸ ret;
}

ਸਥਿਰ u64 cpu_shares_read_u64(ਬਣਤਰ cgroup_subsys_state *css,
			       ਬਣਤਰ cftype *cft)
{
	ਵਾਪਸ tg_ਭਾਰ(css_tg(css));
}
#ਜੇ_ਅੰਤ /* CONFIG_GROUP_SCHED_WEIGHT */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CFS_BANDWIDTH
ਸਥਿਰ DEFINE_MUTEX(cfs_constraints_mutex);

ਸਥਿਰ ਪੂਰਨ __cfs_schedulable(ਬਣਤਰ task_ਸਮੂਹ *tg, u64 period, u64 runtime);

ਸਥਿਰ ਪੂਰਨ tg_ਸੈੱਟ_cfs_bandwidth(ਬਣਤਰ task_ਸਮੂਹ *tg,
				u64 period_us, u64 quota_us, u64 ਮਿਆਦ_us)
{
	ਪੂਰਨ i, ret = 0, runtime_enabled, runtime_was_enabled;
	ਬਣਤਰ cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
	u64 period, quota, ਮਿਆਦ;

	period = (u64)period_us * NSEC_ਪ੍ਰਤੀ_USEC;

	ਜੇ (quota_us == RUNTIME_INF)
		quota = RUNTIME_INF;
	ਨਹੀਂਤਾਂ
		quota = (u64)quota_us * NSEC_ਪ੍ਰਤੀ_USEC;

	ਮਿਆਦ = (u64)ਮਿਆਦ_us * NSEC_ਪ੍ਰਤੀ_USEC;

	/*
	 * Prevent race between setting of cfs_rq->runtime_enabled and
	 * unthrottle_offline_cfs_rqs().
	 */
	guard(cpus_read_ਤਾਲਾ)();
	guard(mutex)(&cfs_constraints_mutex);

	ret = __cfs_schedulable(tg, period, quota);
	ਜੇ (ret)
		ਵਾਪਸ ret;

	runtime_enabled = quota != RUNTIME_INF;
	runtime_was_enabled = cfs_b->quota != RUNTIME_INF;
	/*
	 * If we need to toggle cfs_bandwidth_used, off->on must occur
	 * before making related changes, and on->off must occur afterwards
	 */
	ਜੇ (runtime_enabled && !runtime_was_enabled)
		cfs_bandwidth_usage_inc();

	scoped_guard (raw_spinlock_irq, &cfs_b->ਤਾਲਾ) {
		cfs_b->period = ns_ਮੰਜ਼ਿਲ_ktime(period);
		cfs_b->quota = quota;
		cfs_b->ਮਿਆਦ = ਮਿਆਦ;

		__refill_cfs_bandwidth_runtime(cfs_b);

		/*
		 * Restart the period timer (if active) to handle new
		 * period expiry:
		 */
		ਜੇ (runtime_enabled)
			ਸ਼ੁਰੂ_cfs_bandwidth(cfs_b);
	}

	for_each_online_cpu(i) {
		ਬਣਤਰ cfs_rq *cfs_rq = tg_cfs_rq(tg, i);
		ਬਣਤਰ rq *rq = cfs_rq->rq;

		guard(rq_ਤਾਲਾ_irq)(rq);

		cfs_rq->runtime_enabled = runtime_enabled;
		cfs_rq->runtime_ਬਚਿਆ = 1;

		ਜੇ (cfs_rq->throttled) {
			update_rq_clock(rq);
			unthrottle_cfs_rq(cfs_rq);
		}
	}

	ਜੇ (runtime_was_enabled && !runtime_enabled)
		cfs_bandwidth_usage_dec();

	ਵਾਪਸ 0;
}

ਸਥਿਰ u64 tg_get_cfs_period(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	u64 cfs_period_us;

	cfs_period_us = ktime_ਮੰਜ਼ਿਲ_ns(tg->cfs_bandwidth.period);
	do_div(cfs_period_us, NSEC_ਪ੍ਰਤੀ_USEC);

	ਵਾਪਸ cfs_period_us;
}

ਸਥਿਰ u64 tg_get_cfs_quota(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	u64 quota_us;

	ਜੇ (tg->cfs_bandwidth.quota == RUNTIME_INF)
		ਵਾਪਸ RUNTIME_INF;

	quota_us = tg->cfs_bandwidth.quota;
	do_div(quota_us, NSEC_ਪ੍ਰਤੀ_USEC);

	ਵਾਪਸ quota_us;
}

ਸਥਿਰ u64 tg_get_cfs_ਮਿਆਦ(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	u64 ਮਿਆਦ_us;

	ਮਿਆਦ_us = tg->cfs_bandwidth.ਮਿਆਦ;
	do_div(ਮਿਆਦ_us, NSEC_ਪ੍ਰਤੀ_USEC);

	ਵਾਪਸ ਮਿਆਦ_us;
}

ਬਣਤਰ cfs_schedulable_ਅੰਕੜੇ {
	ਬਣਤਰ task_ਸਮੂਹ *tg;
	u64 period, quota;
};

/*
 * normalize group quota/period to be quota/max_period
 * note: units are usecs
 */
ਸਥਿਰ u64 normalize_cfs_quota(ਬਣਤਰ task_ਸਮੂਹ *tg,
			       ਬਣਤਰ cfs_schedulable_ਅੰਕੜੇ *d)
{
	u64 quota, period;

	ਜੇ (tg == d->tg) {
		period = d->period;
		quota = d->quota;
	} ਨਹੀਂਤਾਂ {
		period = tg_get_cfs_period(tg);
		quota = tg_get_cfs_quota(tg);
	}

	/* note: these should typically be equivalent */
	ਜੇ (quota == RUNTIME_INF || quota == -1)
		ਵਾਪਸ RUNTIME_INF;

	ਵਾਪਸ ਮੰਜ਼ਿਲ_ਅਨੁਪਾਤ(period, quota);
}

ਸਥਿਰ ਪੂਰਨ tg_cfs_schedulable_ਥੱਲੇ(ਬਣਤਰ task_ਸਮੂਹ *tg, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ cfs_schedulable_ਅੰਕੜੇ *d = ਅੰਕੜੇ;
	ਬਣਤਰ cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
	s64 quota = 0, ਮਾਪਾ_quota = -1;

	ਜੇ (!tg->ਮਾਪਾ) {
		quota = RUNTIME_INF;
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ cfs_bandwidth *ਮਾਪਾ_b = &tg->ਮਾਪਾ->cfs_bandwidth;

		quota = normalize_cfs_quota(tg, d);
		ਮਾਪਾ_quota = ਮਾਪਾ_b->ਦਰਜਾਵਾਰ_quota;

		/*
		 * Ensure max(child_quota) <= parent_quota.  On cgroup2,
		 * always take the non-RUNTIME_INF min.  On cgroup1, only
		 * inherit when no limit is set. In both cases this is used
		 * by the scheduler to determine if a given CFS task has a
		 * bandwidth constraint at some higher level.
		 */
		ਜੇ (cgroup_subsys_on_dfl(cpu_cgrp_subsys)) {
			ਜੇ (quota == RUNTIME_INF)
				quota = ਮਾਪਾ_quota;
			ਨਹੀਂਤਾਂ ਜੇ (ਮਾਪਾ_quota != RUNTIME_INF)
				quota = min(quota, ਮਾਪਾ_quota);
		} ਨਹੀਂਤਾਂ {
			ਜੇ (quota == RUNTIME_INF)
				quota = ਮਾਪਾ_quota;
			ਨਹੀਂਤਾਂ ਜੇ (ਮਾਪਾ_quota != RUNTIME_INF && quota > ਮਾਪਾ_quota)
				ਵਾਪਸ -EINVAL;
		}
	}
	cfs_b->ਦਰਜਾਵਾਰ_quota = quota;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ __cfs_schedulable(ਬਣਤਰ task_ਸਮੂਹ *tg, u64 period, u64 quota)
{
	ਬਣਤਰ cfs_schedulable_ਅੰਕੜੇ ਅੰਕੜੇ = {
		.tg = tg,
		.period = period,
		.quota = quota,
	};

	ਜੇ (quota != RUNTIME_INF) {
		do_div(ਅੰਕੜੇ.period, NSEC_ਪ੍ਰਤੀ_USEC);
		do_div(ਅੰਕੜੇ.quota, NSEC_ਪ੍ਰਤੀ_USEC);
	}

	guard(rcu)();
	ਵਾਪਸ walk_tg_ਰੁੱਖ(tg_cfs_schedulable_ਥੱਲੇ, tg_nop, &ਅੰਕੜੇ);
}

ਸਥਿਰ ਪੂਰਨ cpu_cfs_stat_show(ਬਣਤਰ seq_file *sf, ਖਾਲੀ *v)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg = css_tg(seq_css(sf));
	ਬਣਤਰ cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;

	seq_printf(sf, "nr_periods %d\n", cfs_b->nr_periods);
	seq_printf(sf, "nr_throttled %d\n", cfs_b->nr_throttled);
	seq_printf(sf, "throttled_time %llu\n", cfs_b->throttled_ਸਮਾਂ);

	ਜੇ (schedstat_enabled() && tg != &ਜੜ੍ਹ_task_ਸਮੂਹ) {
		ਬਣਤਰ sched_statistics *stats;
		u64 ws = 0;
		ਪੂਰਨ i;

		for_each_possible_cpu(i) {
			stats = __schedstats_from_se(tg_se(tg, i));
			ws += schedstat_val(stats->wait_ਜੋੜ);
		}

		seq_printf(sf, "wait_sum %llu\n", ws);
	}

	seq_printf(sf, "nr_bursts %d\n", cfs_b->nr_ਮਿਆਦ);
	seq_printf(sf, "burst_time %llu\n", cfs_b->ਮਿਆਦ_ਸਮਾਂ);

	ਵਾਪਸ 0;
}

ਸਥਿਰ u64 throttled_ਸਮਾਂ_self(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	ਪੂਰਨ i;
	u64 ਕੁੱਲ = 0;

	for_each_possible_cpu(i) {
		ਕੁੱਲ += READ_ONCE(tg_cfs_rq(tg, i)->throttled_clock_self_ਸਮਾਂ);
	}

	ਵਾਪਸ ਕੁੱਲ;
}

ਸਥਿਰ ਪੂਰਨ cpu_cfs_local_stat_show(ਬਣਤਰ seq_file *sf, ਖਾਲੀ *v)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg = css_tg(seq_css(sf));

	seq_printf(sf, "throttled_time %llu\n", throttled_ਸਮਾਂ_self(tg));

	ਵਾਪਸ 0;
}
#ਜੇ_ਅੰਤ /* CONFIG_CFS_BANDWIDTH */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_ਸਮੂਹ_SCHED_BANDWIDTH
ਅਚਲ u64 max_bw_quota_period_us = 1 * USEC_ਪ੍ਰਤੀ_SEC; /* 1s */
ਸਥਿਰ ਅਚਲ u64 min_bw_quota_period_us = 1 * USEC_ਪ੍ਰਤੀ_MSEC; /* 1ms */
/* More than 203 days if BW_SHIFT equals 20. */
ਸਥਿਰ ਅਚਲ u64 max_bw_runtime_us = MAX_BW;

ਸਥਿਰ ਖਾਲੀ tg_bandwidth(ਬਣਤਰ task_ਸਮੂਹ *tg,
			 u64 *period_us_p, u64 *quota_us_p, u64 *ਮਿਆਦ_us_p)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CFS_BANDWIDTH
	ਜੇ (period_us_p)
		*period_us_p = tg_get_cfs_period(tg);
	ਜੇ (quota_us_p)
		*quota_us_p = tg_get_cfs_quota(tg);
	ਜੇ (ਮਿਆਦ_us_p)
		*ਮਿਆਦ_us_p = tg_get_cfs_ਮਿਆਦ(tg);
#ਨਹੀਂਤਾਂ /* !CONFIG_CFS_BANDWIDTH */
	ਜੇ (period_us_p)
		*period_us_p = tg->scx.bw_period_us;
	ਜੇ (quota_us_p)
		*quota_us_p = tg->scx.bw_quota_us;
	ਜੇ (ਮਿਆਦ_us_p)
		*ਮਿਆਦ_us_p = tg->scx.bw_ਮਿਆਦ_us;
#ਜੇ_ਅੰਤ /* CONFIG_CFS_BANDWIDTH */
}

ਸਥਿਰ u64 cpu_period_read_u64(ਬਣਤਰ cgroup_subsys_state *css,
			       ਬਣਤਰ cftype *cft)
{
	u64 period_us;

	tg_bandwidth(css_tg(css), &period_us, ਨਿਰਾ, ਨਿਰਾ);
	ਵਾਪਸ period_us;
}

ਸਥਿਰ ਪੂਰਨ tg_ਸੈੱਟ_bandwidth(ਬਣਤਰ task_ਸਮੂਹ *tg,
			    u64 period_us, u64 quota_us, u64 ਮਿਆਦ_us)
{
	ਅਚਲ u64 max_usec = U64_MAX / NSEC_ਪ੍ਰਤੀ_USEC;
	ਪੂਰਨ ret = 0;

	ਜੇ (tg == &ਜੜ੍ਹ_task_ਸਮੂਹ)
		ਵਾਪਸ -EINVAL;

	/* Values should survive translation to nsec */
	ਜੇ (period_us > max_usec ||
	    (quota_us != RUNTIME_INF && quota_us > max_usec) ||
	    ਮਿਆਦ_us > max_usec)
		ਵਾਪਸ -EINVAL;

	/*
	 * Ensure we have some amount of bandwidth every period. This is to
	 * prevent reaching a state of large arrears when throttled via
	 * entity_tick() resulting in prolonged exit starvation.
	 */
	ਜੇ (quota_us < min_bw_quota_period_us ||
	    period_us < min_bw_quota_period_us)
		ਵਾਪਸ -EINVAL;

	/*
	 * Likewise, bound things on the other side by preventing insane quota
	 * periods.  This also allows us to normalize in computing quota
	 * feasibility.
	 */
	ਜੇ (period_us > max_bw_quota_period_us)
		ਵਾਪਸ -EINVAL;

	/*
	 * Bound quota to defend quota against overflow during bandwidth shift.
	 */
	ਜੇ (quota_us != RUNTIME_INF && quota_us > max_bw_runtime_us)
		ਵਾਪਸ -EINVAL;

	ਜੇ (quota_us != RUNTIME_INF && (ਮਿਆਦ_us > quota_us ||
					ਮਿਆਦ_us + quota_us > max_bw_runtime_us))
		ਵਾਪਸ -EINVAL;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CFS_BANDWIDTH
	ret = tg_ਸੈੱਟ_cfs_bandwidth(tg, period_us, quota_us, ਮਿਆਦ_us);
#ਜੇ_ਅੰਤ /* CONFIG_CFS_BANDWIDTH */
	ਜੇ (!ret)
		scx_ਸਮੂਹ_ਸੈੱਟ_bandwidth(tg, period_us, quota_us, ਮਿਆਦ_us);
	ਵਾਪਸ ret;
}

ਸਥਿਰ s64 cpu_quota_read_s64(ਬਣਤਰ cgroup_subsys_state *css,
			      ਬਣਤਰ cftype *cft)
{
	u64 quota_us;

	tg_bandwidth(css_tg(css), ਨਿਰਾ, &quota_us, ਨਿਰਾ);
	ਵਾਪਸ quota_us;	/* (s64)RUNTIME_INF becomes -1 */
}

ਸਥਿਰ u64 cpu_ਮਿਆਦ_read_u64(ਬਣਤਰ cgroup_subsys_state *css,
			      ਬਣਤਰ cftype *cft)
{
	u64 ਮਿਆਦ_us;

	tg_bandwidth(css_tg(css), ਨਿਰਾ, ਨਿਰਾ, &ਮਿਆਦ_us);
	ਵਾਪਸ ਮਿਆਦ_us;
}

ਸਥਿਰ ਪੂਰਨ cpu_period_write_u64(ਬਣਤਰ cgroup_subsys_state *css,
				ਬਣਤਰ cftype *cftype, u64 period_us)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg = css_tg(css);
	u64 quota_us, ਮਿਆਦ_us;

	tg_bandwidth(tg, ਨਿਰਾ, &quota_us, &ਮਿਆਦ_us);
	ਵਾਪਸ tg_ਸੈੱਟ_bandwidth(tg, period_us, quota_us, ਮਿਆਦ_us);
}

ਸਥਿਰ ਪੂਰਨ cpu_quota_write_s64(ਬਣਤਰ cgroup_subsys_state *css,
			       ਬਣਤਰ cftype *cftype, s64 quota_us)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg = css_tg(css);
	u64 period_us, ਮਿਆਦ_us;

	ਜੇ (quota_us < 0)
		quota_us = RUNTIME_INF;

	tg_bandwidth(tg, &period_us, ਨਿਰਾ, &ਮਿਆਦ_us);
	ਵਾਪਸ tg_ਸੈੱਟ_bandwidth(tg, period_us, quota_us, ਮਿਆਦ_us);
}

ਸਥਿਰ ਪੂਰਨ cpu_ਮਿਆਦ_write_u64(ਬਣਤਰ cgroup_subsys_state *css,
			       ਬਣਤਰ cftype *cftype, u64 ਮਿਆਦ_us)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg = css_tg(css);
	u64 period_us, quota_us;

	tg_bandwidth(tg, &period_us, &quota_us, ਨਿਰਾ);
	ਵਾਪਸ tg_ਸੈੱਟ_bandwidth(tg, period_us, quota_us, ਮਿਆਦ_us);
}
#ਜੇ_ਅੰਤ /* CONFIG_GROUP_SCHED_BANDWIDTH */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED
ਸਥਿਰ ਪੂਰਨ cpu_rt_runtime_write(ਬਣਤਰ cgroup_subsys_state *css,
				ਬਣਤਰ cftype *cft, s64 val)
{
	ਵਾਪਸ sched_ਸਮੂਹ_ਸੈੱਟ_rt_runtime(css_tg(css), val);
}

ਸਥਿਰ s64 cpu_rt_runtime_read(ਬਣਤਰ cgroup_subsys_state *css,
			       ਬਣਤਰ cftype *cft)
{
	ਵਾਪਸ sched_ਸਮੂਹ_rt_runtime(css_tg(css));
}

ਸਥਿਰ ਪੂਰਨ cpu_rt_period_write_uint(ਬਣਤਰ cgroup_subsys_state *css,
				    ਬਣਤਰ cftype *cftype, u64 rt_period_us)
{
	ਵਾਪਸ sched_ਸਮੂਹ_ਸੈੱਟ_rt_period(css_tg(css), rt_period_us);
}

ਸਥਿਰ u64 cpu_rt_period_read_uint(ਬਣਤਰ cgroup_subsys_state *css,
				   ਬਣਤਰ cftype *cft)
{
	ਵਾਪਸ sched_ਸਮੂਹ_rt_period(css_tg(css));
}
#ਜੇ_ਅੰਤ /* CONFIG_RT_GROUP_SCHED */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_ਸਮੂਹ_SCHED_ਭਾਰ
ਸਥਿਰ s64 cpu_idle_read_s64(ਬਣਤਰ cgroup_subsys_state *css,
			       ਬਣਤਰ cftype *cft)
{
	ਵਾਪਸ css_tg(css)->idle;
}

ਸਥਿਰ ਪੂਰਨ cpu_idle_write_s64(ਬਣਤਰ cgroup_subsys_state *css,
				ਬਣਤਰ cftype *cft, s64 idle)
{
	ਪੂਰਨ ret;

	ret = sched_ਸਮੂਹ_ਸੈੱਟ_idle(css_tg(css), idle);
	ਜੇ (!ret)
		scx_ਸਮੂਹ_ਸੈੱਟ_idle(css_tg(css), idle);
	ਵਾਪਸ ret;
}
#ਜੇ_ਅੰਤ /* CONFIG_GROUP_SCHED_WEIGHT */

ਸਥਿਰ ਬਣਤਰ cftype cpu_legacy_files[] = {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_ਸਮੂਹ_SCHED_ਭਾਰ
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
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_ਸਮੂਹ_SCHED_BANDWIDTH
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
		.read_u64 = cpu_ਮਿਆਦ_read_u64,
		.write_u64 = cpu_ਮਿਆਦ_write_u64,
	},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CFS_BANDWIDTH
	{
		.name = "stat",
		.seq_show = cpu_cfs_stat_show,
	},
	{
		.name = "stat.local",
		.seq_show = cpu_cfs_local_stat_show,
	},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK_ਸਮੂਹ
	{
		.name = "uclamp.min",
		.flags = CFTYPE_NOT_ON_ਜੜ੍ਹ,
		.seq_show = cpu_uclamp_min_show,
		.ਲਿਖ = cpu_uclamp_min_write,
	},
	{
		.name = "uclamp.max",
		.flags = CFTYPE_NOT_ON_ਜੜ੍ਹ,
		.seq_show = cpu_uclamp_max_show,
		.ਲਿਖ = cpu_uclamp_max_write,
	},
#ਜੇ_ਅੰਤ
	{ }	/* Terminate */
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED
ਸਥਿਰ ਬਣਤਰ cftype rt_ਸਮੂਹ_files[] = {
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

# ifdef CONFIG_RT_ਸਮੂਹ_SCHED_DEFAULT_DISABLED
DEFINE_STATIC_ਕੁੰਜੀ_FALSE(rt_ਸਮੂਹ_sched);
# ਨਹੀਂਤਾਂ
DEFINE_STATIC_ਕੁੰਜੀ_TRUE(rt_ਸਮੂਹ_sched);
# endif

ਸਥਿਰ ਪੂਰਨ __init ਤਿਆਰੀ_rt_ਸਮੂਹ_sched(ਅੱਖਰ *str)
{
	ਲੰਮਾ val;

	ਜੇ (kstrtol(str, 0, &val) || val < 0 || val > 1) {
		pr_warn("Unable to set rt_group_sched\n");
		ਵਾਪਸ 1;
	}
	ਜੇ (val)
		static_branch_enable(&rt_ਸਮੂਹ_sched);
	ਨਹੀਂਤਾਂ
		static_branch_disable(&rt_ਸਮੂਹ_sched);

	ਵਾਪਸ 1;
}
__setup("rt_group_sched=", ਤਿਆਰੀ_rt_ਸਮੂਹ_sched);

ਸਥਿਰ ਪੂਰਨ __init cpu_rt_ਸਮੂਹ_init(ਖਾਲੀ)
{
	ਜੇ (!rt_ਸਮੂਹ_sched_enabled())
		ਵਾਪਸ 0;

	WARN_ON(cgroup_ਜੋੜੋ_legacy_cftypes(&cpu_cgrp_subsys, rt_ਸਮੂਹ_files));
	ਵਾਪਸ 0;
}
subsys_initcall(cpu_rt_ਸਮੂਹ_init);
#ਜੇ_ਅੰਤ /* CONFIG_RT_GROUP_SCHED */

ਸਥਿਰ ਪੂਰਨ cpu_extra_stat_show(ਬਣਤਰ seq_file *sf,
			       ਬਣਤਰ cgroup_subsys_state *css)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CFS_BANDWIDTH
	{
		ਬਣਤਰ task_ਸਮੂਹ *tg = css_tg(css);
		ਬਣਤਰ cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
		u64 throttled_usec, ਮਿਆਦ_usec;

		throttled_usec = cfs_b->throttled_ਸਮਾਂ;
		do_div(throttled_usec, NSEC_ਪ੍ਰਤੀ_USEC);
		ਮਿਆਦ_usec = cfs_b->ਮਿਆਦ_ਸਮਾਂ;
		do_div(ਮਿਆਦ_usec, NSEC_ਪ੍ਰਤੀ_USEC);

		seq_printf(sf, "nr_periods %d\n"
			   "nr_throttled %d\n"
			   "throttled_usec %llu\n"
			   "nr_bursts %d\n"
			   "burst_usec %llu\n",
			   cfs_b->nr_periods, cfs_b->nr_throttled,
			   throttled_usec, cfs_b->nr_ਮਿਆਦ, ਮਿਆਦ_usec);
	}
#ਜੇ_ਅੰਤ /* CONFIG_CFS_BANDWIDTH */
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ cpu_local_stat_show(ਬਣਤਰ seq_file *sf,
			       ਬਣਤਰ cgroup_subsys_state *css)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CFS_BANDWIDTH
	{
		ਬਣਤਰ task_ਸਮੂਹ *tg = css_tg(css);
		u64 throttled_self_usec;

		throttled_self_usec = throttled_ਸਮਾਂ_self(tg);
		do_div(throttled_self_usec, NSEC_ਪ੍ਰਤੀ_USEC);

		seq_printf(sf, "throttled_usec %llu\n",
			   throttled_self_usec);
	}
#ਜੇ_ਅੰਤ
	ਵਾਪਸ 0;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_ਸਮੂਹ_SCHED_ਭਾਰ

ਸਥਿਰ u64 cpu_ਭਾਰ_read_u64(ਬਣਤਰ cgroup_subsys_state *css,
			       ਬਣਤਰ cftype *cft)
{
	ਵਾਪਸ sched_ਭਾਰ_ਮੰਜ਼ਿਲ_cgroup(tg_ਭਾਰ(css_tg(css)));
}

ਸਥਿਰ ਪੂਰਨ cpu_ਭਾਰ_write_u64(ਬਣਤਰ cgroup_subsys_state *css,
				ਬਣਤਰ cftype *cft, u64 cgrp_ਭਾਰ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਭਾਰ;
	ਪੂਰਨ ret;

	ਜੇ (cgrp_ਭਾਰ < CGROUP_ਭਾਰ_MIN || cgrp_ਭਾਰ > CGROUP_ਭਾਰ_MAX)
		ਵਾਪਸ -ERANGE;

	ਭਾਰ = sched_ਭਾਰ_from_cgroup(cgrp_ਭਾਰ);

	ret = sched_ਸਮੂਹ_ਸੈੱਟ_shares(css_tg(css), scale_ਲੱਦੋ(ਭਾਰ));
	ਜੇ (!ret)
		scx_ਸਮੂਹ_ਸੈੱਟ_ਭਾਰ(css_tg(css), cgrp_ਭਾਰ);
	ਵਾਪਸ ret;
}

ਸਥਿਰ s64 cpu_ਭਾਰ_nice_read_s64(ਬਣਤਰ cgroup_subsys_state *css,
				    ਬਣਤਰ cftype *cft)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਭਾਰ = tg_ਭਾਰ(css_tg(css));
	ਪੂਰਨ ਆਖਰੀ_delta = ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ;
	ਪੂਰਨ prio, delta;

	/* find the closest nice value to the current weight */
	ਲਈ (prio = 0; prio < ARRAY_ਮਾਪ(sched_prio_ਮੰਜ਼ਿਲ_ਭਾਰ); prio++) {
		delta = ਨਿਰਪੇਖ(sched_prio_ਮੰਜ਼ਿਲ_ਭਾਰ[prio] - ਭਾਰ);
		ਜੇ (delta >= ਆਖਰੀ_delta)
			ਰੋਕੋ;
		ਆਖਰੀ_delta = delta;
	}

	ਵਾਪਸ PRIO_ਮੰਜ਼ਿਲ_NICE(prio - 1 + MAX_RT_PRIO);
}

ਸਥਿਰ ਪੂਰਨ cpu_ਭਾਰ_nice_write_s64(ਬਣਤਰ cgroup_subsys_state *css,
				     ਬਣਤਰ cftype *cft, s64 nice)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਭਾਰ;
	ਪੂਰਨ idx, ret;

	ਜੇ (nice < MIN_NICE || nice > MAX_NICE)
		ਵਾਪਸ -ERANGE;

	idx = NICE_ਮੰਜ਼ਿਲ_PRIO(nice) - MAX_RT_PRIO;
	idx = array_ਸੂਚਕਾਂਕ_nospec(idx, 40);
	ਭਾਰ = sched_prio_ਮੰਜ਼ਿਲ_ਭਾਰ[idx];

	ret = sched_ਸਮੂਹ_ਸੈੱਟ_shares(css_tg(css), scale_ਲੱਦੋ(ਭਾਰ));
	ਜੇ (!ret)
		scx_ਸਮੂਹ_ਸੈੱਟ_ਭਾਰ(css_tg(css),
				     sched_ਭਾਰ_ਮੰਜ਼ਿਲ_cgroup(ਭਾਰ));
	ਵਾਪਸ ret;
}
#ਜੇ_ਅੰਤ /* CONFIG_GROUP_SCHED_WEIGHT */

ਸਥਿਰ ਖਾਲੀ __maybe_unused cpu_period_quota_print(ਬਣਤਰ seq_file *sf,
						  ਲੰਮਾ period, ਲੰਮਾ quota)
{
	ਜੇ (quota < 0)
		seq_puts(sf, "max");
	ਨਹੀਂਤਾਂ
		seq_printf(sf, "%ld", quota);

	seq_printf(sf, " %ld\n", period);
}

/* caller should put the current value in *@periodp before calling */
ਸਥਿਰ ਪੂਰਨ __maybe_unused cpu_period_quota_parse(ਅੱਖਰ *buf, u64 *period_us_p,
						 u64 *quota_us_p)
{
	ਅੱਖਰ tok[21];	/* U64_MAX */

	ਜੇ (ਲੜੀ_ਤੋਂ_ਪੜ੍ਹੋ(buf, "%20s %llu", tok, period_us_p) < 1)
		ਵਾਪਸ -EINVAL;

	ਜੇ (ਲੜੀ_ਤੋਂ_ਪੜ੍ਹੋ(tok, "%llu", quota_us_p) < 1) {
		ਜੇ (!strcmp(tok, "max"))
			*quota_us_p = RUNTIME_INF;
		ਨਹੀਂਤਾਂ
			ਵਾਪਸ -EINVAL;
	}

	ਵਾਪਸ 0;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_ਸਮੂਹ_SCHED_BANDWIDTH
ਸਥਿਰ ਪੂਰਨ cpu_max_show(ਬਣਤਰ seq_file *sf, ਖਾਲੀ *v)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg = css_tg(seq_css(sf));
	u64 period_us, quota_us;

	tg_bandwidth(tg, &period_us, &quota_us, ਨਿਰਾ);
	cpu_period_quota_print(sf, period_us, quota_us);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ssize_t cpu_max_write(ਬਣਤਰ kernfs_open_file *ਦਾ,
			     ਅੱਖਰ *buf, ਮਾਪ_t nbytes, loff_t off)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg = css_tg(ਦਾ_css(ਦਾ));
	u64 period_us, quota_us, ਮਿਆਦ_us;
	ਪੂਰਨ ret;

	tg_bandwidth(tg, &period_us, ਨਿਰਾ, &ਮਿਆਦ_us);
	ret = cpu_period_quota_parse(buf, &period_us, &quota_us);
	ਜੇ (!ret)
		ret = tg_ਸੈੱਟ_bandwidth(tg, period_us, quota_us, ਮਿਆਦ_us);
	ਵਾਪਸ ret ?: nbytes;
}
#ਜੇ_ਅੰਤ /* CONFIG_CFS_BANDWIDTH */

ਸਥਿਰ ਬਣਤਰ cftype cpu_files[] = {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_ਸਮੂਹ_SCHED_ਭਾਰ
	{
		.name = "weight",
		.flags = CFTYPE_NOT_ON_ਜੜ੍ਹ,
		.read_u64 = cpu_ਭਾਰ_read_u64,
		.write_u64 = cpu_ਭਾਰ_write_u64,
	},
	{
		.name = "weight.nice",
		.flags = CFTYPE_NOT_ON_ਜੜ੍ਹ,
		.read_s64 = cpu_ਭਾਰ_nice_read_s64,
		.write_s64 = cpu_ਭਾਰ_nice_write_s64,
	},
	{
		.name = "idle",
		.flags = CFTYPE_NOT_ON_ਜੜ੍ਹ,
		.read_s64 = cpu_idle_read_s64,
		.write_s64 = cpu_idle_write_s64,
	},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_ਸਮੂਹ_SCHED_BANDWIDTH
	{
		.name = "max",
		.flags = CFTYPE_NOT_ON_ਜੜ੍ਹ,
		.seq_show = cpu_max_show,
		.ਲਿਖ = cpu_max_write,
	},
	{
		.name = "max.burst",
		.flags = CFTYPE_NOT_ON_ਜੜ੍ਹ,
		.read_u64 = cpu_ਮਿਆਦ_read_u64,
		.write_u64 = cpu_ਮਿਆਦ_write_u64,
	},
#ਜੇ_ਅੰਤ /* CONFIG_CFS_BANDWIDTH */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK_ਸਮੂਹ
	{
		.name = "uclamp.min",
		.flags = CFTYPE_NOT_ON_ਜੜ੍ਹ,
		.seq_show = cpu_uclamp_min_show,
		.ਲਿਖ = cpu_uclamp_min_write,
	},
	{
		.name = "uclamp.max",
		.flags = CFTYPE_NOT_ON_ਜੜ੍ਹ,
		.seq_show = cpu_uclamp_max_show,
		.ਲਿਖ = cpu_uclamp_max_write,
	},
#ਜੇ_ਅੰਤ /* CONFIG_UCLAMP_TASK_GROUP */
	{ }	/* terminate */
};

ਬਣਤਰ cgroup_subsys cpu_cgrp_subsys = {
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

#ਜੇ_ਅੰਤ /* CONFIG_CGROUP_SCHED */

ਖਾਲੀ dump_cpu_task(ਪੂਰਨ cpu)
{
	ਜੇ (ਵਿੱਚ_hardirq() && cpu == smp_processor_id()) {
		ਬਣਤਰ pt_regs *regs;

		regs = get_irq_regs();
		ਜੇ (regs) {
			show_regs(regs);
			ਵਾਪਸ;
		}
	}

	ਜੇ (trigger_single_cpu_backtrace(cpu))
		ਵਾਪਸ;

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
ਅਚਲ ਪੂਰਨ sched_prio_ਮੰਜ਼ਿਲ_ਭਾਰ[40] = {
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
ਅਚਲ u32 sched_prio_ਮੰਜ਼ਿਲ_wmult[40] = {
 /* -20 */     48388,     59856,     76040,     92818,    118348,
 /* -15 */    147320,    184698,    229616,    287308,    360437,
 /* -10 */    449829,    563644,    704093,    875809,   1099582,
 /*  -5 */   1376151,   1717300,   2157191,   2708050,   3363326,
 /*   0 */   4194304,   5237765,   6557202,   8165337,  10153587,
 /*   5 */  12820798,  15790321,  19976592,  24970740,  31350126,
 /*  10 */  39045157,  49367440,  61356676,  76695844,  95443717,
 /*  15 */ 119304647, 148102320, 186737708, 238609294, 286331153,
};

ਖਾਲੀ call_trace_sched_update_nr_running(ਬਣਤਰ rq *rq, ਪੂਰਨ ਗਿਣਤੀ_2)
{
        trace_sched_update_nr_running_tp(rq, ਗਿਣਤੀ_2);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_MM_CID
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
ਸਥਿਰ ਖਾਲੀ __mm_update_max_cids(ਬਣਤਰ mm_mm_cid *mc)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ opt_cids, max_cids;

	/* Calculate the new optimal constraint */
	opt_cids = min(mc->nr_cpus_allowed, mc->users);

	/* Adjust the maximum CIDs to +25% limited by the number of possible CPUs */
	max_cids = min(opt_cids + (opt_cids / 4), ਅੰਕ_possible_cpus());
	WRITE_ONCE(mc->max_cids, max_cids);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mm_cid_calc_pcpu_thrs(ਬਣਤਰ mm_mm_cid *mc)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ opt_cids;

	opt_cids = min(mc->nr_cpus_allowed, mc->users);
	/* Has to be at least 1 because 0 indicates PCPU mode off */
	ਵਾਪਸ max(min(opt_cids - opt_cids / 4, ਅੰਕ_possible_cpus() / 2), 1);
}

ਸਥਿਰ bool mm_update_max_cids(ਬਣਤਰ mm_struct *mm)
{
	ਬਣਤਰ mm_mm_cid *mc = &mm->mm_cid;
	bool percpu = cid_on_cpu(mc->ਬਹੁਲਕ);

	lockdep_assert_held(&mm->mm_cid.ਤਾਲਾ);

	/* Clear deferred mode switch flag. A change is handled by the caller */
	mc->update_deferred = false;
	__mm_update_max_cids(mc);

	/* Check whether owner mode must be changed */
	ਜੇ (!percpu) {
		/* Enable per CPU mode when the number of users is above max_cids */
		ਜੇ (mc->users > mc->max_cids)
			mc->pcpu_thrs = mm_cid_calc_pcpu_thrs(mc);
	} ਨਹੀਂਤਾਂ {
		/* Switch back to per task if user count under threshold */
		ਜੇ (mc->users < mc->pcpu_thrs)
			mc->pcpu_thrs = 0;
	}

	/* Mode change required? */
	ਜੇ (percpu == !!mc->pcpu_thrs)
		ਵਾਪਸ false;

	/* Flip the mode and set the transition flag to bridge the transfer */
	WRITE_ONCE(mc->ਬਹੁਲਕ, mc->ਬਹੁਲਕ ^ (MM_CID_TRANSIT | MM_CID_ONCPU));
	/*
	 * Order the store against the subsequent fixups so that
	 * acquire(rq::lock) cannot be reordered by the CPU before the
	 * store.
	 */
	smp_mb();
	ਵਾਪਸ true;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ mm_update_cpus_allowed(ਬਣਤਰ mm_struct *mm, ਅਚਲ ਬਣਤਰ cpumask *affmsk)
{
	ਬਣਤਰ cpumask *mm_allowed;
	ਬਣਤਰ mm_mm_cid *mc;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਭਾਰ;

	ਜੇ (!mm || !READ_ONCE(mm->mm_cid.users))
		ਵਾਪਸ;
	/*
	 * mm::mm_cid::mm_cpus_allowed is the superset of each threads
	 * allowed CPUs mask which means it can only grow.
	 */
	mc = &mm->mm_cid;
	guard(raw_spinlock)(&mc->ਤਾਲਾ);
	mm_allowed = mm_cpus_allowed(mm);
	ਭਾਰ = cpumask_weighted_or(mm_allowed, mm_allowed, affmsk);
	ਜੇ (ਭਾਰ == mc->nr_cpus_allowed)
		ਵਾਪਸ;

	WRITE_ONCE(mc->nr_cpus_allowed, ਭਾਰ);
	__mm_update_max_cids(mc);
	ਜੇ (!cid_on_cpu(mc->ਬਹੁਲਕ))
		ਵਾਪਸ;

	/* Adjust the threshold to the wider set */
	mc->pcpu_thrs = mm_cid_calc_pcpu_thrs(mc);
	/* Switch back to per task mode? */
	ਜੇ (mc->users >= mc->pcpu_thrs)
		ਵਾਪਸ;

	/* Don't queue twice */
	ਜੇ (mc->update_deferred)
		ਵਾਪਸ;

	/* Queue the irq work, which schedules the real work */
	mc->update_deferred = true;
	irq_work_ਕਤਾਰ(&mc->irq_work);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ mm_cid_complete_transit(ਬਣਤਰ mm_struct *mm, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਹੁਲਕ)
{
	/*
	 * Ensure that the store removing the TRANSIT bit cannot be
	 * reordered by the CPU before the fixups have been completed.
	 */
	smp_mb();
	WRITE_ONCE(mm->mm_cid.ਬਹੁਲਕ, ਬਹੁਲਕ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ mm_cid_transit_ਮੰਜ਼ਿਲ_task(ਬਣਤਰ task_struct *t, ਬਣਤਰ mm_cid_pcpu *pcp)
{
	ਜੇ (cid_on_cpu(t->mm_cid.cid)) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cid = cpu_cid_ਮੰਜ਼ਿਲ_cid(t->mm_cid.cid);

		t->mm_cid.cid = cid_ਮੰਜ਼ਿਲ_transit_cid(cid);
		pcp->cid = t->mm_cid.cid;
	}
}

ਸਥਿਰ ਖਾਲੀ mm_cid_fixup_cpus_ਮੰਜ਼ਿਲ_tasks(ਬਣਤਰ mm_struct *mm)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu;

	/* Walk the CPUs and fixup all stale CIDs */
	for_each_possible_cpu(cpu) {
		ਬਣਤਰ mm_cid_pcpu *pcp = ਪ੍ਰਤੀ_cpu_ptr(mm->mm_cid.pcpu, cpu);
		ਬਣਤਰ rq *rq = cpu_rq(cpu);

		/* Remote access to mm::mm_cid::pcpu requires rq_lock */
		guard(rq_ਤਾਲਾ_irq)(rq);
		/* Is the CID still owned by the CPU? */
		ਜੇ (cid_on_cpu(pcp->cid)) {
			/*
			 * If rq->curr has @mm, transfer it with the
			 * transition bit set. Otherwise drop it.
			 */
			ਜੇ (rq->curr->mm == mm && rq->curr->mm_cid.ਸਰਗਰਮ)
				mm_cid_transit_ਮੰਜ਼ਿਲ_task(rq->curr, pcp);
			ਨਹੀਂਤਾਂ
				mm_drop_cid_on_cpu(mm, pcp);

		} ਨਹੀਂਤਾਂ ਜੇ (rq->curr->mm == mm && rq->curr->mm_cid.ਸਰਗਰਮ) {
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cid = rq->curr->mm_cid.cid;

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
			ਜੇ (cid != MM_CID_UNSET && !cid_ਵਿੱਚ_transit(cid)) {
				cid = cid_ਮੰਜ਼ਿਲ_transit_cid(cid);
				rq->curr->mm_cid.cid = cid;
				pcp->cid = cid;
			}
		}
	}
	mm_cid_complete_transit(mm, 0);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ mm_cid_transit_ਮੰਜ਼ਿਲ_cpu(ਬਣਤਰ task_struct *t, ਬਣਤਰ mm_cid_pcpu *pcp)
{
	ਜੇ (cid_on_task(t->mm_cid.cid)) {
		t->mm_cid.cid = cid_ਮੰਜ਼ਿਲ_transit_cid(t->mm_cid.cid);
		pcp->cid = t->mm_cid.cid;
	}
}

ਸਥਿਰ ਖਾਲੀ mm_cid_fixup_task_ਮੰਜ਼ਿਲ_cpu(ਬਣਤਰ task_struct *t, ਬਣਤਰ mm_struct *mm)
{
	/* Remote access to mm::mm_cid::pcpu requires rq_lock */
	guard(task_rq_ਤਾਲਾ_2)(t);
	ਜੇ (cid_on_task(t->mm_cid.cid)) {
		/* If running on the CPU, put the CID in transit mode, otherwise drop it */
		ਜੇ (task_rq(t)->curr == t)
			mm_cid_transit_ਮੰਜ਼ਿਲ_cpu(t, ਪ੍ਰਤੀ_cpu_ptr(mm->mm_cid.pcpu, task_cpu(t)));
		ਨਹੀਂਤਾਂ
			mm_unset_cid_on_task(t);
	}
}

ਸਥਿਰ ਖਾਲੀ mm_cid_fixup_tasks_ਮੰਜ਼ਿਲ_cpus(ਖਾਲੀ)
{
	ਬਣਤਰ mm_struct *mm = current->mm;
	ਬਣਤਰ task_struct *t;

	lockdep_assert_held(&mm->mm_cid.mutex);

	hlist_for_each_ਇੰਦਰਾਜ(t, &mm->mm_cid.user_list, mm_cid.ਗੰਢ) {
		/* Current has already transferred before invoking the fixup. */
		ਜੇ (t != current)
			mm_cid_fixup_task_ਮੰਜ਼ਿਲ_cpu(t, mm);
	}

	mm_cid_complete_transit(mm, MM_CID_ONCPU);
}

ਸਥਿਰ bool sched_mm_cid_ਜੋੜੋ_user(ਬਣਤਰ task_struct *t, ਬਣਤਰ mm_struct *mm)
{
	lockdep_assert_held(&mm->mm_cid.ਤਾਲਾ);

	t->mm_cid.ਸਰਗਰਮ = 1;
	hlist_ਜੋੜੋ_ਸਿਰ(&t->mm_cid.ਗੰਢ, &mm->mm_cid.user_list);
	mm->mm_cid.users++;
	ਵਾਪਸ mm_update_max_cids(mm);
}

ਸਥਿਰ ਖਾਲੀ sched_mm_cid_fork(ਬਣਤਰ task_struct *t)
{
	ਬਣਤਰ mm_struct *mm = t->mm;
	bool percpu;

	ਜੇ (!mm)
		ਵਾਪਸ;

	WARN_ON_ONCE(t->mm_cid.cid != MM_CID_UNSET);

	guard(mutex)(&mm->mm_cid.mutex);
	scoped_guard(raw_spinlock_irq, &mm->mm_cid.ਤਾਲਾ) {
		ਬਣਤਰ mm_cid_pcpu *pcp = this_cpu_ptr(mm->mm_cid.pcpu);

		/* First user ? */
		ਜੇ (!mm->mm_cid.users) {
			sched_mm_cid_ਜੋੜੋ_user(t, mm);
			t->mm_cid.cid = mm_get_cid(mm);
			/* Required for execve() */
			pcp->cid = t->mm_cid.cid;
			ਵਾਪਸ;
		}

		ਜੇ (!sched_mm_cid_ਜੋੜੋ_user(t, mm)) {
			ਜੇ (!cid_on_cpu(mm->mm_cid.ਬਹੁਲਕ))
				t->mm_cid.cid = mm_get_cid(mm);
			ਵਾਪਸ;
		}

		/* Handle the mode change and transfer current's CID */
		percpu = cid_on_cpu(mm->mm_cid.ਬਹੁਲਕ);
		ਜੇ (!percpu)
			mm_cid_transit_ਮੰਜ਼ਿਲ_task(current, pcp);
		ਨਹੀਂਤਾਂ
			mm_cid_transit_ਮੰਜ਼ਿਲ_cpu(current, pcp);
	}

	ਜੇ (percpu) {
		mm_cid_fixup_tasks_ਮੰਜ਼ਿਲ_cpus();
	} ਨਹੀਂਤਾਂ {
		mm_cid_fixup_cpus_ਮੰਜ਼ਿਲ_tasks(mm);
		t->mm_cid.cid = mm_get_cid(mm);
	}
}

ਸਥਿਰ bool sched_mm_cid_remove_user(ਬਣਤਰ task_struct *t)
{
	lockdep_assert_held(&t->mm->mm_cid.ਤਾਲਾ);

	t->mm_cid.ਸਰਗਰਮ = 0;
	/* Clear the transition bit */
	t->mm_cid.cid = cid_from_transit_cid(t->mm_cid.cid);
	mm_unset_cid_on_task(t);
	hlist_del_init(&t->mm_cid.ਗੰਢ);
	t->mm->mm_cid.users--;
	ਵਾਪਸ mm_update_max_cids(t->mm);
}

ਸਥਿਰ bool __sched_mm_cid_exit(ਬਣਤਰ task_struct *t)
{
	ਬਣਤਰ mm_struct *mm = t->mm;

	ਜੇ (!sched_mm_cid_remove_user(t))
		ਵਾਪਸ false;
	/*
	 * Contrary to fork() this only deals with a switch back to per
	 * task mode either because the above decreased users or an
	 * affinity change increased the number of allowed CPUs and the
	 * deferred fixup did not run yet.
	 */
	ਜੇ (WARN_ON_ONCE(cid_on_cpu(mm->mm_cid.ਬਹੁਲਕ)))
		ਵਾਪਸ false;
	/*
	 * A failed fork(2) cleanup never gets here, so @current must have
	 * the same MM as @t. That's true for exit() and the failed
	 * pthread_create() cleanup case.
	 */
	ਜੇ (WARN_ON_ONCE(current->mm != mm))
		ਵਾਪਸ false;
	ਵਾਪਸ true;
}

/*
 * When a task exits, the MM CID held by the task is not longer required as
 * the task cannot return to user space.
 */
ਖਾਲੀ sched_mm_cid_exit(ਬਣਤਰ task_struct *t)
{
	ਬਣਤਰ mm_struct *mm = t->mm;

	ਜੇ (!mm || !t->mm_cid.ਸਰਗਰਮ)
		ਵਾਪਸ;
	/*
	 * Ensure that only one instance is doing MM CID operations within
	 * a MM. The common case is uncontended. The rare fixup case adds
	 * some overhead.
	 */
	scoped_guard(mutex, &mm->mm_cid.mutex) {
		/* mm_cid::mutex is sufficient to protect mm_cid::users */
		ਜੇ (likely(mm->mm_cid.users > 1)) {
			scoped_guard(raw_spinlock_irq, &mm->mm_cid.ਤਾਲਾ) {
				ਜੇ (!__sched_mm_cid_exit(t))
					ਵਾਪਸ;
				/*
				 * Mode change. The task has the CID unset
				 * already and dealt with an eventually set
				 * TRANSIT bit. If the CID is owned by the CPU
				 * then drop it.
				 */
				mm_drop_cid_on_cpu(mm, this_cpu_ptr(mm->mm_cid.pcpu));
			}
			mm_cid_fixup_cpus_ਮੰਜ਼ਿਲ_tasks(mm);
			ਵਾਪਸ;
		}
		/* Last user */
		scoped_guard(raw_spinlock_irq, &mm->mm_cid.ਤਾਲਾ) {
			/* Required across execve() */
			ਜੇ (t == current)
				mm_cid_transit_ਮੰਜ਼ਿਲ_task(t, this_cpu_ptr(mm->mm_cid.pcpu));
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
ਖਾਲੀ sched_mm_cid_before_execve(ਬਣਤਰ task_struct *t)
{
	sched_mm_cid_exit(t);
}

/* Reactivate MM CID after execve() */
ਖਾਲੀ sched_mm_cid_after_execve(ਬਣਤਰ task_struct *t)
{
	ਜੇ (t->mm)
		sched_mm_cid_fork(t);
}

ਸਥਿਰ ਖਾਲੀ mm_cid_work_fn(ਬਣਤਰ work_struct *work)
{
	ਬਣਤਰ mm_struct *mm = container_ਦਾ(work, ਬਣਤਰ mm_struct, mm_cid.work);

	guard(mutex)(&mm->mm_cid.mutex);
	/* Did the last user task exit already? */
	ਜੇ (!mm->mm_cid.users)
		ਵਾਪਸ;

	scoped_guard(raw_spinlock_irq, &mm->mm_cid.ਤਾਲਾ) {
		/* Have fork() or exit() handled it already? */
		ਜੇ (!mm->mm_cid.update_deferred)
			ਵਾਪਸ;
		/* This clears mm_cid::update_deferred */
		ਜੇ (!mm_update_max_cids(mm))
			ਵਾਪਸ;
		/* Affinity changes can only switch back to task mode */
		ਜੇ (WARN_ON_ONCE(cid_on_cpu(mm->mm_cid.ਬਹੁਲਕ)))
			ਵਾਪਸ;
	}
	mm_cid_fixup_cpus_ਮੰਜ਼ਿਲ_tasks(mm);
}

ਸਥਿਰ ਖਾਲੀ mm_cid_irq_work(ਬਣਤਰ irq_work *work)
{
	ਬਣਤਰ mm_struct *mm = container_ਦਾ(work, ਬਣਤਰ mm_struct, mm_cid.irq_work);

	/*
	 * Needs to be unconditional because mm_cid::lock cannot be held
	 * when scheduling work as mm_update_cpus_allowed() nests inside
	 * rq::lock and schedule_work() might end up in wakeup...
	 */
	ਸਮਾਂ_ਸਾਰਣੀ_work(&mm->mm_cid.work);
}

ਖਾਲੀ mm_init_cid(ਬਣਤਰ mm_struct *mm, ਬਣਤਰ task_struct *p)
{
	mm->mm_cid.max_cids = 0;
	mm->mm_cid.ਬਹੁਲਕ = 0;
	mm->mm_cid.nr_cpus_allowed = p->nr_cpus_allowed;
	mm->mm_cid.users = 0;
	mm->mm_cid.pcpu_thrs = 0;
	mm->mm_cid.update_deferred = 0;
	raw_spin_ਤਾਲਾ_init(&mm->mm_cid.ਤਾਲਾ);
	mutex_init(&mm->mm_cid.mutex);
	mm->mm_cid.irq_work = IRQ_WORK_INIT_HARD(mm_cid_irq_work);
	INIT_WORK(&mm->mm_cid.work, mm_cid_work_fn);
	INIT_HLIST_ਸਿਰ(&mm->mm_cid.user_list);
	cpumask_copy(mm_cpus_allowed(mm), &p->cpus_ਨਕਾਬ);
	bitmap_zero(mm_cidmask(mm), ਅੰਕ_possible_cpus());
}
#ਨਹੀਂਤਾਂ /* CONFIG_SCHED_MM_CID */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ mm_update_cpus_allowed(ਬਣਤਰ mm_struct *mm, ਅਚਲ ਬਣਤਰ cpumask *affmsk) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_mm_cid_fork(ਬਣਤਰ task_struct *t) { }
#ਜੇ_ਅੰਤ /* !CONFIG_SCHED_MM_CID */

ਸਥਿਰ DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ sched_change_ctx, sched_change_ctx);

ਬਣਤਰ sched_change_ctx *sched_change_begin(ਬਣਤਰ task_struct *p, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਬਣਤਰ sched_change_ctx *ctx = this_cpu_ptr(&sched_change_ctx);
	ਬਣਤਰ rq *rq = task_rq(p);

	/*
	 * Must exclusively use matched flags since this is both dequeue and
	 * enqueue.
	 */
	WARN_ON_ONCE(flags & 0xFFFF0000);

	lockdep_assert_rq_held(rq);

	ਜੇ (!(flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_NOCLOCK)) {
		update_rq_clock(rq);
		flags |= ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_NOCLOCK;
	}

	ਜੇ ((flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_CLASS) && p->sched_class->switching_from)
		p->sched_class->switching_from(rq, p);

	*ctx = (ਬਣਤਰ sched_change_ctx){
		.p = p,
		.class = p->sched_class,
		.flags = flags,
		.queued = task_on_rq_queued(p),
		.running = task_current_donor(rq, p),
	};

	ਜੇ (!(flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_CLASS)) {
		ਜੇ (p->sched_class->get_prio)
			ctx->prio = p->sched_class->get_prio(rq, p);
		ਨਹੀਂਤਾਂ
			ctx->prio = p->prio;
	}

	ਜੇ (ctx->queued)
		ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task(rq, p, flags);
	ਜੇ (ctx->running)
		put_prev_task(rq, p);

	ਜੇ ((flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_CLASS) && p->sched_class->switched_from)
		p->sched_class->switched_from(rq, p);

	ਵਾਪਸ ctx;
}

ਖਾਲੀ sched_change_ਸਿਰਾ(ਬਣਤਰ sched_change_ctx *ctx)
{
	ਬਣਤਰ task_struct *p = ctx->p;
	ਬਣਤਰ rq *rq = task_rq(p);

	lockdep_assert_rq_held(rq);

	/*
	 * Changing class without *QUEUE_CLASS is bad.
	 */
	WARN_ON_ONCE(p->sched_class != ctx->class && !(ctx->flags & ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_CLASS));

	ਜੇ ((ctx->flags & ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_CLASS) && p->sched_class->switching_ਮੰਜ਼ਿਲ)
		p->sched_class->switching_ਮੰਜ਼ਿਲ(rq, p);

	ਜੇ (ctx->queued)
		ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task(rq, p, ctx->flags);
	ਜੇ (ctx->running)
		ਸੈੱਟ_ਅਗਲਾ_task(rq, p);

	ਜੇ (ctx->flags & ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_CLASS) {
		ਜੇ (p->sched_class->switched_ਮੰਜ਼ਿਲ)
			p->sched_class->switched_ਮੰਜ਼ਿਲ(rq, p);

		/*
		 * Check if the task can preempt the current in case the class
		 * was upgraded. Note that none of the switch*_from() methods
		 * know the new class and none of the switch*_to() methods know
		 * the old class so we must resched here.
		 */
		ਜੇ (ctx->queued)
			wakeup_preempt(rq, p, 0);

		ਜੇ (ctx->running) {
			/*
			 * If this was a degradation in class; make sure to
			 * reschedule.
			 */
			ਜੇ (sched_class_above(ctx->class, p->sched_class))
				resched_curr(rq);
		}
	} ਨਹੀਂਤਾਂ {
		p->sched_class->prio_ਬਦਲਿਆ(rq, p, ctx->prio);
	}
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PREFERRED_CPU
ਸਥਿਰ DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ cpu_stop_work, npc_ਧੱਕੋ_task_work);

ਸਥਿਰ ਪੂਰਨ sched_non_preferred_cpu_ਧੱਕੋ_stop(ਖਾਲੀ *arg)
{
	ਬਣਤਰ task_struct *p = arg;
	ਬਣਤਰ rq *rq = this_rq();
	ਬਣਤਰ rq_flags rf;
	ਪੂਰਨ cpu;

	ਜੇ (cpu_preferred(rq->cpu)) {
		scoped_guard(rq_ਤਾਲਾ_irqsave, rq)
			rq->npc_ਧੱਕੋ_work_pending = false;
		put_task_struct(p);
		ਵਾਪਸ 0;
	}

	scoped_guard (raw_spinlock_irq, &p->pi_ਤਾਲਾ) {
		/*
		 * select_fallback_rq() may acquire the rq lock in case of
		 * fallback. So call it before grabbing rq lock. If the task
		 * migrates to another CPU before the rq lock is acquired,
		 * subsequent validation of task's current rq will help to
		 * safely bail out.
		 */
		cpu = select_fallback_rq(rq->cpu, p);
		context_unsafe_alias(rq);
		rq_ਤਾਲਾ(rq, &rf);
		rq->npc_ਧੱਕੋ_work_pending = false;
		update_rq_clock(rq);

		ਜੇ (task_rq(p) == rq && task_on_rq_queued(p)) {
			ਬਣਤਰ rq *dest_rq = __migrate_task(rq, &rf, p, cpu);

			ਜੇ (rq != dest_rq)
				schedstat_inc(p->stats.nr_migrations_cpu_non_preferred);
			rq = dest_rq;
		}
		rq_unlock(rq, &rf);
	}

	put_task_struct(p);
	ਵਾਪਸ 0;
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
ਖਾਲੀ sched_ਧੱਕੋ_current_non_preferred_cpu(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ task_struct *ਧੱਕੋ_task = rq->curr;

	scoped_guard(rq_ਤਾਲਾ, rq) {
		/* Push the task if its explicit affinity allows */
		ਜੇ (!task_can_migrate_ਮੰਜ਼ਿਲ_preferred(ਧੱਕੋ_task, rq->cpu))
			ਵਾਪਸ;

		/* There is already a stopper thread. Don't race with it. */
		ਜੇ (rq->npc_ਧੱਕੋ_work_pending)
			ਵਾਪਸ;

		ਜੇ (is_migration_disabled(ਧੱਕੋ_task))
			ਵਾਪਸ;

		rq->npc_ਧੱਕੋ_work_pending = true;
	}

	/* sched_tick runs with interrupts disabled. */
	get_task_struct(ਧੱਕੋ_task);
	stop_one_cpu_nowait(rq->cpu, sched_non_preferred_cpu_ਧੱਕੋ_stop,
			    ਧੱਕੋ_task, this_cpu_ptr(&npc_ਧੱਕੋ_task_work));
}
#ਜੇ_ਅੰਤ
