// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/core.c
 *
 *  Core kernel CPU scheduler code
 *
 *  Copyright (C) 1991-2002  Linus Torvalds
 *  Copyright (C) 1998-2024  Ingo Molnar, Red Hat
 */
#define INSTANTIATE_EXPORTED_MIGRATE_DISABLE
#ଅନ୍ତର୍ଭାଵ <linux/sched.h>
#ଅନ୍ତର୍ଭାଵ <linux/highmem.h>
#ଅନ୍ତର୍ଭାଵ <linux/hrtimer_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/ktime_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/signal.h>
#ଅନ୍ତର୍ଭାଵ <linux/syscalls_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/debug_locks.h>
#ଅନ୍ତର୍ଭାଵ <linux/prefetch.h>
#ଅନ୍ତର୍ଭାଵ <linux/capability.h>
#ଅନ୍ତର୍ଭାଵ <linux/pgtable_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/wait_bit.h>
#ଅନ୍ତର୍ଭାଵ <linux/jiffies.h>
#ଅନ୍ତର୍ଭାଵ <linux/spinlock_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/cpumask_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/lockdep_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/hardirq.h>
#ଅନ୍ତର୍ଭାଵ <linux/softirq.h>
#ଅନ୍ତର୍ଭାଵ <linux/refcount_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/topology.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/clock.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/cond_resched.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/cputime.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/debug.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/hotplug.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/init.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/isolation.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/loadavg.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/mm.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/nohz.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/rseq_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/rt.h>

#ଅନ୍ତର୍ଭାଵ <linux/context_tracking.h>
#ଅନ୍ତର୍ଭାଵ <linux/cpuset.h>
#ଅନ୍ତର୍ଭାଵ <linux/delayacct.h>
#ଅନ୍ତର୍ଭାଵ <linux/init_task.h>
#ଅନ୍ତର୍ଭାଵ <linux/interrupt.h>
#ଅନ୍ତର୍ଭାଵ <linux/ioprio.h>
#ଅନ୍ତର୍ଭାଵ <linux/kallsyms.h>
#ଅନ୍ତର୍ଭାଵ <linux/kcov.h>
#ଅନ୍ତର୍ଭାଵ <linux/kprobes.h>
#ଅନ୍ତର୍ଭାଵ <linux/llist_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/mmu_context.h>
#ଅନ୍ତର୍ଭାଵ <linux/mmzone.h>
#ଅନ୍ତର୍ଭାଵ <linux/mutex_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/nmi.h>
#ଅନ୍ତର୍ଭାଵ <linux/nospec.h>
#ଅନ୍ତର୍ଭାଵ <linux/perf_event_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/profile.h>
#ଅନ୍ତର୍ଭାଵ <linux/psi.h>
#ଅନ୍ତର୍ଭାଵ <linux/rcuwait_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/rseq.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/wake_q.h>
#ଅନ୍ତର୍ଭାଵ <linux/scs.h>
#ଅନ୍ତର୍ଭାଵ <linux/slab.h>
#ଅନ୍ତର୍ଭାଵ <linux/syscalls.h>
#ଅନ୍ତର୍ଭାଵ <linux/vtime.h>
#ଅନ୍ତର୍ଭାଵ <linux/wait_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/workqueue_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/livepatch_sched.h>

#ifdef CONFIG_PREEMPT_DYNAMIC
# ifdef CONFIG_GENERIC_IRQ_ENTRY
#  include <linux/irq-entry-common.h>
# endif
#endif

#ଅନ୍ତର୍ଭାଵ <uapi/linux/sched/types.h>

#ଅନ୍ତର୍ଭାଵ <asm/irq_regs.h>
#ଅନ୍ତର୍ଭାଵ <asm/switch_to.h>
#ଅନ୍ତର୍ଭାଵ <asm/tlb.h>

#define CREATE_TRACE_POINTS
#ଅନ୍ତର୍ଭାଵ <linux/sched/rseq_api.h>
#ଅନ୍ତର୍ଭାଵ <trace/events/sched.h>
#ଅନ୍ତର୍ଭାଵ <trace/events/ipi.h>
#undef CREATE_TRACE_POINTS

#ଅନ୍ତର୍ଭାଵ "sched.h"
#ଅନ୍ତର୍ଭାଵ "stats.h"

#ଅନ୍ତର୍ଭାଵ "autogroup.h"
#ଅନ୍ତର୍ଭାଵ "pelt.h"
#ଅନ୍ତର୍ଭାଵ "smp.h"

#ଅନ୍ତର୍ଭାଵ "../workqueue_internal.h"
#ଅନ୍ତର୍ଭାଵ "../../io_uring/io-wq.h"
#ଅନ୍ତର୍ଭାଵ "../smpboot.h"
#ଅନ୍ତର୍ଭାଵ "../locking/mutex.h"

EXPORT_TRACEPOINT_ପ୍ରତୀକ_GPL(ipi_send_cpu);
EXPORT_TRACEPOINT_ପ୍ରତୀକ_GPL(ipi_send_cpumask);

/*
 * Export tracepoints that act as a bare tracehook (ie: have no trace event
 * associated with them) to allow external modules to probe them.
 */
EXPORT_TRACEPOINT_ପ୍ରତୀକ_GPL(pelt_cfs_tp);
EXPORT_TRACEPOINT_ପ୍ରତୀକ_GPL(pelt_rt_tp);
EXPORT_TRACEPOINT_ପ୍ରତୀକ_GPL(pelt_dl_tp);
EXPORT_TRACEPOINT_ପ୍ରତୀକ_GPL(pelt_irq_tp);
EXPORT_TRACEPOINT_ପ୍ରତୀକ_GPL(pelt_se_tp);
EXPORT_TRACEPOINT_ପ୍ରତୀକ_GPL(pelt_hw_tp);
EXPORT_TRACEPOINT_ପ୍ରତୀକ_GPL(sched_cpu_capacity_tp);
EXPORT_TRACEPOINT_ପ୍ରତୀକ_GPL(sched_overutilized_tp);
EXPORT_TRACEPOINT_ପ୍ରତୀକ_GPL(sched_util_est_cfs_tp);
EXPORT_TRACEPOINT_ପ୍ରତୀକ_GPL(sched_util_est_se_tp);
EXPORT_TRACEPOINT_ପ୍ରତୀକ_GPL(sched_update_nr_running_tp);
EXPORT_TRACEPOINT_ପ୍ରତୀକ_GPL(sched_compute_energy_tp);
EXPORT_TRACEPOINT_ପ୍ରତୀକ_GPL(sched_entry_tp);
EXPORT_TRACEPOINT_ପ୍ରତୀକ_GPL(sched_exit_tp);
EXPORT_TRACEPOINT_ପ୍ରତୀକ_GPL(sched_set_need_resched_tp);
EXPORT_TRACEPOINT_ପ୍ରତୀକ_GPL(sched_dl_throttle_tp);
EXPORT_TRACEPOINT_ପ୍ରତୀକ_GPL(sched_dl_replenish_tp);
EXPORT_TRACEPOINT_ପ୍ରତୀକ_GPL(sched_dl_update_tp);
EXPORT_TRACEPOINT_ପ୍ରତୀକ_GPL(sched_dl_server_start_tp);
EXPORT_TRACEPOINT_ପ୍ରତୀକ_GPL(sched_dl_server_stop_tp);

DEFINE_PER_CPU_SHARED_ALIGNED(ସଂରଚନା rq, runqueues);
DEFINE_PER_CPU(ସଂରଚନା rnd_state, sched_rnd_state);

#ifdef CONFIG_SCHED_PROXY_EXEC
DEFINE_STATIC_ଚାବି_TRUE(__sched_proxy_exec);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init setup_proxy_exec(ଅକ୍ଷର *str)
{
	bool proxy_enable = true;

	ଯଦି (*str && kstrtobool(str + 1, &proxy_enable)) {
		pr_warn("Unable to parse sched_proxy_exec=\n");
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (proxy_enable) {
		pr_info("sched_proxy_exec enabled via boot arg\n");
		static_branch_enable(&__sched_proxy_exec);
	} ଅନ୍ୟଥା {
		pr_info("sched_proxy_exec disabled via boot arg\n");
		static_branch_disable(&__sched_proxy_exec);
	}
	ଫେରନ୍ତୁ 1;
}
#ଅନ୍ୟଥା
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init setup_proxy_exec(ଅକ୍ଷର *str)
{
	pr_warn("CONFIG_SCHED_PROXY_EXEC=n, so it cannot be enabled or disabled at boot time\n");
	ଫେରନ୍ତୁ 0;
}
#endif
__setup("sched_proxy_exec", setup_proxy_exec);

/*
 * Debugging: various feature bits
 *
 * If SCHED_DEBUG is disabled, each compilation unit has its own copy of
 * sysctl_sched_features, defined in sched.h, to allow constants propagation
 * at compile time and compiler optimization based on features default.
 */
#define SCHED_FEAT(name, enabled)	\
	(1UL << __SCHED_FEAT_##name) * enabled |
__read_mostly unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_sched_features =
#ଅନ୍ତର୍ଭାଵ "features.h"
	0;
#undef SCHED_FEAT

/*
 * Print a warning if need_resched is set for the given duration (if
 * LATENCY_WARN is enabled).
 *
 * If sysctl_resched_latency_warn_once is set, only one warning will be shown
 * per boot.
 */
__read_mostly ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_resched_latency_warn_ms = 100;
__read_mostly ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_resched_latency_warn_once = 1;

/*
 * Number of tasks to iterate in a single balance run.
 * Limited because this is done with IRQs disabled.
 */
__read_mostly unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_sched_nr_migrate = SCHED_NR_MIGRATE_BREAK;

__read_mostly ପୂର୍ଣ୍ଣସଂଖ୍ୟା scheduler_running;

#ifdef CONFIG_SCHED_CORE

DEFINE_STATIC_ଚାବି_FALSE(__sched_core_enabled);

/* kernel prio, less is more */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା __task_prio(ଅଚଳ ସଂରଚନା task_struct *p)
{
	ଯଦି (p->sched_class == &stop_sched_class) /* trumps deadline */
		ଫେରନ୍ତୁ -2;

	ଯଦି (p->dl_server)
		ଫେରନ୍ତୁ -1; /* deadline */

	ଯଦି (rt_or_dl_prio(p->prio))
		ଫେରନ୍ତୁ p->prio; /* [-1, 99] */

	ଯଦି (p->sched_class == &idle_sched_class)
		ଫେରନ୍ତୁ MAX_RT_PRIO + NICE_WIDTH; /* 140 */

	ଯଦି (task_on_scx(p))
		ଫେରନ୍ତୁ MAX_RT_PRIO + MAX_NICE + 1; /* 120, squash ext */

	ଫେରନ୍ତୁ MAX_RT_PRIO + MAX_NICE; /* 119, squash fair */
}

/*
 * l(a,b)
 * le(a,b) := !l(b,a)
 * g(a,b)  := l(b,a)
 * ge(a,b) := !l(a,b)
 */

/* real prio, less is less */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool prio_less(ଅଚଳ ସଂରଚନା task_struct *a,
			     ଅଚଳ ସଂରଚନା task_struct *b, bool in_fi)
{

	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pa = __task_prio(a), pb = __task_prio(b);

	ଯଦି (-pa < -pb)
		ଫେରନ୍ତୁ true;

	ଯଦି (-pb < -pa)
		ଫେରନ୍ତୁ false;

	ଯଦି (pa == -1) { /* dl_prio() doesn't work because of stop_class above */
		ଅଚଳ ସଂରଚନା sched_dl_entity *a_dl, *b_dl;

		a_dl = &a->dl;
		/*
		 * Since,'a' and 'b' can be CFS tasks served by DL server,
		 * __task_prio() can return -1 (for DL) even for those. In that
		 * case, get to the dl_server's DL entity.
		 */
		ଯଦି (a->dl_server)
			a_dl = a->dl_server;

		b_dl = &b->dl;
		ଯଦି (b->dl_server)
			b_dl = b->dl_server;

		ଫେରନ୍ତୁ !dl_time_before(a_dl->deadline, b_dl->deadline);
	}

	ଯଦି (pa == MAX_RT_PRIO + MAX_NICE)	/* fair */
		ଫେରନ୍ତୁ cfs_prio_less(a, b, in_fi);

#ifdef CONFIG_SCHED_CLASS_EXT
	ଯଦି (pa == MAX_RT_PRIO + MAX_NICE + 1)	/* ext */
		ଫେରନ୍ତୁ scx_prio_less(a, b, in_fi);
#endif

	ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool __sched_core_less(ଅଚଳ ସଂରଚନା task_struct *a,
				     ଅଚଳ ସଂରଚନା task_struct *b)
{
	ଯଦି (a->core_cookie < b->core_cookie)
		ଫେରନ୍ତୁ true;

	ଯଦି (a->core_cookie > b->core_cookie)
		ଫେରନ୍ତୁ false;

	/* flip prio, so high prio is leftmost */
	ଯଦି (prio_less(b, a, !!task_rq(a)->core->core_forceidle_ଗଣନା))
		ଫେରନ୍ତୁ true;

	ଫେରନ୍ତୁ false;
}

#define __node_2_sc(ଗଣ୍ଠି) rb_entry((ଗଣ୍ଠି), ସଂରଚନା task_struct, core_ଗଣ୍ଠି)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool rb_sched_core_less(ସଂରଚନା rb_ଗଣ୍ଠି *a, ଅଚଳ ସଂରଚନା rb_ଗଣ୍ଠି *b)
{
	ଫେରନ୍ତୁ __sched_core_less(__node_2_sc(a), __node_2_sc(b));
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା rb_sched_core_cmp(ଅଚଳ ଶୂନ୍ୟ *ଚାବି, ଅଚଳ ସଂରଚନା rb_ଗଣ୍ଠି *ଗଣ୍ଠି)
{
	ଅଚଳ ସଂରଚନା task_struct *p = __node_2_sc(ଗଣ୍ଠି);
	unsigned long cookie = (unsigned long)ଚାବି;

	ଯଦି (cookie < p->core_cookie)
		ଫେରନ୍ତୁ -1;

	ଯଦି (cookie > p->core_cookie)
		ଫେରନ୍ତୁ 1;

	ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ sched_core_enqueue(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	ଯଦି (p->se.sched_delayed)
		ଫେରନ୍ତୁ;

	rq->core->core_task_seq++;

	ଯଦି (!p->core_cookie)
		ଫେରନ୍ତୁ;

	rb_add(&p->core_ଗଣ୍ଠି, &rq->core_ବୃକ୍ଷ, rb_sched_core_less);
}

ଶୂନ୍ୟ sched_core_dequeue(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ଯଦି (p->se.sched_delayed)
		ଫେରନ୍ତୁ;

	rq->core->core_task_seq++;

	ଯଦି (sched_core_enqueued(p)) {
		rb_erase(&p->core_ଗଣ୍ଠି, &rq->core_ବୃକ୍ଷ);
		RB_CLEAR_ଗଣ୍ଠି(&p->core_ଗଣ୍ଠି);
	}

	/*
	 * Migrating the last task off the cpu, with the cpu in forced idle
	 * state. Reschedule to create an accounting edge for forced idle,
	 * and re-examine whether the core is still in forced idle state.
	 */
	ଯଦି (!(flags & DEQUEUE_SAVE) && rq->nr_running == 1 &&
	    rq->core->core_forceidle_ଗଣନା && rq->curr == rq->idle)
		resched_curr(rq);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_task_is_throttled(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଯଦି (p->sched_class->task_is_throttled)
		ଫେରନ୍ତୁ p->sched_class->task_is_throttled(p, cpu);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ସଂରଚନା task_struct *sched_core_next(ସଂରଚନା task_struct *p, unsigned long cookie)
{
	ସଂରଚନା rb_ଗଣ୍ଠି *ଗଣ୍ଠି = &p->core_ଗଣ୍ଠି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = task_cpu(p);

	କର {
		ଗଣ୍ଠି = rb_next(ଗଣ୍ଠି);
		ଯଦି (!ଗଣ୍ଠି)
			ଫେରନ୍ତୁ ଖାଲି;

		p = __node_2_sc(ଗଣ୍ଠି);
		ଯଦି (p->core_cookie != cookie)
			ଫେରନ୍ତୁ ଖାଲି;

	} ଯେତେବେଳେ (sched_task_is_throttled(p, cpu));

	ଫେରନ୍ତୁ p;
}

/*
 * Find left-most (aka, highest priority) and unthrottled task matching @cookie.
 * If no suitable task is found, NULL will be returned.
 */
ସ୍ଥିର ସଂରଚନା task_struct *sched_core_find(ସଂରଚନା rq *rq, unsigned long cookie)
{
	ସଂରଚନା task_struct *p;
	ସଂରଚନା rb_ଗଣ୍ଠି *ଗଣ୍ଠି;

	ଗଣ୍ଠି = rb_find_first((ଶୂନ୍ୟ *)cookie, &rq->core_ବୃକ୍ଷ, rb_sched_core_cmp);
	ଯଦି (!ଗଣ୍ଠି)
		ଫେରନ୍ତୁ ଖାଲି;

	p = __node_2_sc(ଗଣ୍ଠି);
	ଯଦି (!sched_task_is_throttled(p, rq->cpu))
		ଫେରନ୍ତୁ p;

	ଫେରନ୍ତୁ sched_core_next(p, cookie);
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

ସ୍ଥିର DEFINE_MUTEX(sched_core_mutex);
ସ୍ଥିର atomic_t sched_core_ଗଣନା;
ସ୍ଥିର ସଂରଚନା cpumask sched_core_mask;

ସ୍ଥିର ଶୂନ୍ୟ sched_core_lock(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, unsigned long *flags)
	__context_unsafe(/* acquires multiple */)
	__acquires(&runqueues.__lock) /* overapproximation */
{
	ଅଚଳ ସଂରଚନା cpumask *smt_mask = cpu_smt_mask(cpu);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା t, i = 0;

	local_irq_save(*flags);
	for_each_cpu(t, smt_mask)
		raw_spin_lock_nested(&cpu_rq(t)->__lock, i++);
}

ସ୍ଥିର ଶୂନ୍ୟ sched_core_unlock(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, unsigned long *flags)
	__context_unsafe(/* releases multiple */)
	__releases(&runqueues.__lock) /* overapproximation */
{
	ଅଚଳ ସଂରଚନା cpumask *smt_mask = cpu_smt_mask(cpu);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା t;

	for_each_cpu(t, smt_mask)
		raw_spin_unlock(&cpu_rq(t)->__lock);
	local_irq_restore(*flags);
}

ସ୍ଥିର ଶୂନ୍ୟ __sched_core_flip(bool enabled)
{
	unsigned long flags;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, t;

	cpus_read_lock();

	/*
	 * Toggle the online cores, one by one.
	 */
	cpumask_copy(&sched_core_mask, cpu_online_mask);
	for_each_cpu(cpu, &sched_core_mask) {
		ଅଚଳ ସଂରଚନା cpumask *smt_mask = cpu_smt_mask(cpu);

		sched_core_lock(cpu, &flags);

		/*
		 * A core-wide selection may have the shared rq lock temporarily
		 * released by a lock-dropping ->pick_task(). Flipping would
		 * rebind rq_lockp() under it. Wait it out.
		 */
		ଯେତେବେଳେ (cpu_rq(cpu)->core->core_pick_in_flight) {
			sched_core_unlock(cpu, &flags);
			cpu_relax();
			sched_core_lock(cpu, &flags);
		}

		for_each_cpu(t, smt_mask)
			cpu_rq(t)->core_enabled = enabled;

		cpu_rq(cpu)->core->core_forceidle_start = 0;

		sched_core_unlock(cpu, &flags);

		cpumask_andnot(&sched_core_mask, &sched_core_mask, smt_mask);
	}

	/*
	 * Toggle the offline CPUs.
	 */
	for_each_cpu_andnot(cpu, cpu_possible_mask, cpu_online_mask)
		cpu_rq(cpu)->core_enabled = enabled;

	cpus_read_unlock();
}

ସ୍ଥିର ଶୂନ୍ୟ sched_core_assert_empty(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

	for_each_possible_cpu(cpu)
		WARN_ON_ONCE(!RB_EMPTY_ମୂଳ(&cpu_rq(cpu)->core_ବୃକ୍ଷ));
}

ସ୍ଥିର ଶୂନ୍ୟ __sched_core_enable(ଶୂନ୍ୟ)
{
	static_branch_enable(&__sched_core_enabled);
	/*
	 * Ensure all previous instances of raw_spin_rq_*lock() have finished
	 * and future ones will observe !sched_core_disabled().
	 */
	synchronize_rcu();
	__sched_core_flip(true);
	sched_core_assert_empty();
}

ସ୍ଥିର ଶୂନ୍ୟ __sched_core_disable(ଶୂନ୍ୟ)
{
	sched_core_assert_empty();
	__sched_core_flip(false);
	static_branch_disable(&__sched_core_enabled);
}

ଶୂନ୍ୟ sched_core_get(ଶୂନ୍ୟ)
{
	ଯଦି (atomic_inc_not_zero(&sched_core_ଗଣନା))
		ଫେରନ୍ତୁ;

	mutex_lock(&sched_core_mutex);
	ଯଦି (!atomic_read(&sched_core_ଗଣନା))
		__sched_core_enable();

	smp_mb__before_atomic();
	atomic_inc(&sched_core_ଗଣନା);
	mutex_unlock(&sched_core_mutex);
}

ସ୍ଥିର ଶୂନ୍ୟ __sched_core_put(ସଂରଚନା work_struct *work)
{
	ଯଦି (atomic_dec_and_mutex_lock(&sched_core_ଗଣନା, &sched_core_mutex)) {
		__sched_core_disable();
		mutex_unlock(&sched_core_mutex);
	}
}

ଶୂନ୍ୟ sched_core_put(ଶୂନ୍ୟ)
{
	ସ୍ଥିର DECLARE_WORK(_work, __sched_core_put);

	/*
	 * "There can be only one"
	 *
	 * Either this is the last one, or we don't actually need to do any
	 * 'work'. If it is the last *again*, we rely on
	 * WORK_STRUCT_PENDING_BIT.
	 */
	ଯଦି (!atomic_add_unless(&sched_core_ଗଣନା, -1, 1))
		schedule_work(&_work);
}

#ଅନ୍ୟଥା /* !CONFIG_SCHED_CORE: */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_core_enqueue(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p) { }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
sched_core_dequeue(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags) { }

#endif /* !CONFIG_SCHED_CORE */

/* need a wrapper since we may need to trace from modules */
EXPORT_TRACEPOINT_ପ୍ରତୀକ(sched_set_state_tp);

/*
 * Call via the helper macro trace_set_current_state.
 * Calls to this function MUST be guarded by a
 * tracepoint_enabled(sched_set_state_tp)
 */
ଶୂନ୍ୟ __trace_set_current_state(ପୂର୍ଣ୍ଣସଂଖ୍ୟା state_ମୂଲ୍ୟ)
{
	trace_call__sched_set_state_tp(current, state_ମୂଲ୍ୟ);
}
EXPORT_ପ୍ରତୀକ(__trace_set_current_state);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା task_llc(ଅଚଳ ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ per_cpu(sd_llc_id, task_cpu(p));
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

ଶୂନ୍ୟ raw_spin_rq_lock_nested(ସଂରଚନା rq *rq, ପୂର୍ଣ୍ଣସଂଖ୍ୟା subclass)
	__context_unsafe()
{
	raw_spinlock_t *lock;

	/* Matches synchronize_rcu() in __sched_core_enable() */
	preempt_disable();
	ଯଦି (sched_core_disabled()) {
		raw_spin_lock_nested(&rq->__lock, subclass);
		/* preempt_count *MUST* be > 1 */
		preempt_enable_no_resched();
		ଫେରନ୍ତୁ;
	}

	ପାଇଁ (;;) {
		lock = __rq_lockp(rq);
		raw_spin_lock_nested(lock, subclass);
		ଯଦି (likely(lock == __rq_lockp(rq))) {
			/* preempt_count *MUST* be > 1 */
			preempt_enable_no_resched();
			ଫେରନ୍ତୁ;
		}
		raw_spin_unlock(lock);
	}
}

bool raw_spin_rq_trylock(ସଂରଚନା rq *rq)
	__context_unsafe()
{
	raw_spinlock_t *lock;
	bool ret;

	/* Matches synchronize_rcu() in __sched_core_enable() */
	preempt_disable();
	ଯଦି (sched_core_disabled()) {
		ret = raw_spin_trylock(&rq->__lock);
		preempt_enable();
		ଫେରନ୍ତୁ ret;
	}

	ପାଇଁ (;;) {
		lock = __rq_lockp(rq);
		ret = raw_spin_trylock(lock);
		ଯଦି (!ret || (likely(lock == __rq_lockp(rq)))) {
			preempt_enable();
			ଫେରନ୍ତୁ ret;
		}
		raw_spin_unlock(lock);
	}
}

/*
 * double_rq_lock - safely lock two runqueues
 */
ଶୂନ୍ୟ double_rq_lock(ସଂରଚନା rq *rq1, ସଂରଚନା rq *rq2)
{
	lockdep_assert_irqs_disabled();

	ଯଦି (rq_order_less(rq2, rq1))
		swap(rq1, rq2);

	raw_spin_rq_lock(rq1);
	ଯଦି (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_lock_nested(rq2, SINGLE_DEPTH_NESTING);
	ଅନ୍ୟଥା
		__acquire_ctx_lock(__rq_lockp(rq2)); /* fake acquire */

	double_rq_clock_clear_update(rq1, rq2);
}

/*
 * ___task_rq_lock - lock the rq @p resides on.
 */
ସଂରଚନା rq *___task_rq_lock(ସଂରଚନା task_struct *p, ସଂରଚନା rq_flags *rf)
{
	ସଂରଚନା rq *rq;

	lockdep_assert_held(&p->pi_lock);

	ପାଇଁ (;;) {
		rq = task_rq(p);
		raw_spin_rq_lock(rq);
		ଯଦି (likely(rq == task_rq(p) && !task_on_rq_migrating(p))) {
			rq_pin_lock(rq, rf);
			ଫେରନ୍ତୁ rq;
		}
		raw_spin_rq_unlock(rq);

		ଯେତେବେଳେ (unlikely(task_on_rq_migrating(p)))
			cpu_relax();
	}
}

/*
 * task_rq_lock - lock p->pi_lock and lock the rq @p resides on.
 */
ସଂରଚନା rq *_task_rq_lock(ସଂରଚନା task_struct *p, ସଂରଚନା rq_flags *rf)
{
	ସଂରଚନା rq *rq;

	ପାଇଁ (;;) {
		raw_spin_lock_irqsave(&p->pi_lock, rf->flags);
		rq = task_rq(p);
		raw_spin_rq_lock(rq);
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
		ଯଦି (likely(rq == task_rq(p) && !task_on_rq_migrating(p))) {
			rq_pin_lock(rq, rf);
			ଫେରନ୍ତୁ rq;
		}
		raw_spin_rq_unlock(rq);
		raw_spin_unlock_irqrestore(&p->pi_lock, rf->flags);

		ଯେତେବେଳେ (unlikely(task_on_rq_migrating(p)))
			cpu_relax();
	}
}

/*
 * RQ-clock updating methods:
 */

/* Use CONFIG_PARAVIRT as this will avoid more #ifdef in arch code. */
#ifdef CONFIG_PARAVIRT
DEFINE_STATIC_ଚାବି_FALSE(paravirt_steal_rq_enabled);
#endif

ସ୍ଥିର ଶୂନ୍ୟ update_rq_clock_task(ସଂରଚନା rq *rq, s64 delta)
{
/*
 * In theory, the compile should just see 0 here, and optimize out the call
 * to sched_rt_avg_update. But I don't trust it...
 */
	s64 __maybe_unused steal = 0, irq_delta = 0;

#ifdef CONFIG_IRQ_TIME_ACCOUNTING
	ଯଦି (irqtime_enabled()) {
		irq_delta = irq_time_read(cpu_of(rq)) - rq->prev_irq_time;

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
		ଯଦି (irq_delta > delta)
			irq_delta = delta;

		rq->prev_irq_time += irq_delta;
		delta -= irq_delta;
		delayacct_irq(rq->curr, irq_delta);
	}
#endif
#ifdef CONFIG_PARAVIRT_TIME_ACCOUNTING
	ଯଦି (static_branch_unlikely(&paravirt_steal_rq_enabled)) {
		u64 prev_steal;

		steal = prev_steal = paravirt_steal_clock(cpu_of(rq));
		steal -= rq->prev_steal_time_rq;

		ଯଦି (unlikely(steal > delta))
			steal = delta;

		rq->prev_steal_time_rq = prev_steal;
		delta -= steal;
	}
#endif

	rq->clock_task += delta;

#ifdef CONFIG_HAVE_SCHED_AVG_IRQ
	ଯଦି ((irq_delta + steal) && sched_feat(NONTASK_CAPACITY))
		update_irq_load_avg(rq, irq_delta + steal);
#endif
	update_rq_clock_pelt(rq, delta);
}

ଶୂନ୍ୟ update_rq_clock(ସଂରଚନା rq *rq)
{
	s64 delta;
	u64 clock;

	lockdep_assert_rq_held(rq);

	ଯଦି (rq->clock_update_flags & RQCF_ACT_SKIP)
		ଫେରନ୍ତୁ;

	ଯଦି (sched_feat(WARN_DOUBLE_CLOCK))
		WARN_ON_ONCE(rq->clock_update_flags & RQCF_UPDATED);
	rq->clock_update_flags |= RQCF_UPDATED;

	clock = sched_clock_cpu(cpu_of(rq));
	scx_rq_clock_update(rq, clock);

	delta = clock - rq->clock;
	ଯଦି (delta < 0)
		ଫେରନ୍ତୁ;
	rq->clock += delta;

	update_rq_clock_task(rq, delta);
}

#ifdef CONFIG_SCHED_HRTICK
/*
 * Use HR-timers to deliver accurate preemption points.
 */

ଗଣନା {
	HRTICK_SCHED_NONE		= 0,
	HRTICK_SCHED_DEFER		= BIT(1),
	HRTICK_SCHED_START		= BIT(2),
	HRTICK_SCHED_REARM_HRTIMER	= BIT(3)
};

ସ୍ଥିର ଶୂନ୍ୟ __used hrtick_clear(ସଂରଚନା rq *rq)
{
	ଯଦି (hrtimer_active(&rq->hrtick_timer))
		hrtimer_cancel(&rq->hrtick_timer);
}

/*
 * High-resolution timer tick.
 * Runs from hardirq context with interrupts disabled.
 */
ସ୍ଥିର ଗଣନା hrtimer_restart hrtick(ସଂରଚନା hrtimer *timer)
{
	ସଂରଚନା rq *rq = container_of(timer, ସଂରଚନା rq, hrtick_timer);
	ସଂରଚନା rq_flags rf;

	WARN_ON_ONCE(cpu_of(rq) != smp_processor_id());

	rq_lock(rq, &rf);
	update_rq_clock(rq);
	rq->donor->sched_class->task_tick(rq, rq->donor, 1);
	rq_unlock(rq, &rf);

	ଫେରନ୍ତୁ HRTIMER_NORESTART;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool hrtick_needs_rearm(ସଂରଚନା hrtimer *timer, ktime_t expires)
{
	/*
	 * Queued is false when the timer is not started or currently
	 * running the callback. In both cases, restart. If queued check
	 * whether the expiry time actually changes substantially.
	 */
	ଫେରନ୍ତୁ !hrtimer_is_queued(timer) ||
		abs(expires - hrtimer_get_expires(timer)) > 5000;
}

ସ୍ଥିର ଶୂନ୍ୟ hrtick_cond_restart(ସଂରଚନା rq *rq)
{
	ସଂରଚନା hrtimer *timer = &rq->hrtick_timer;
	ktime_t time = rq->hrtick_time;

	ଯଦି (hrtick_needs_rearm(timer, time))
		hrtimer_start(timer, time, HRTIMER_MODE_ABS_PINNED_HARD);
}

/*
 * called from hardirq (IPI) context
 */
ସ୍ଥିର ଶୂନ୍ୟ __hrtick_start(ଶୂନ୍ୟ *arg)
{
	ସଂରଚନା rq *rq = arg;
	ସଂରଚନା rq_flags rf;

	rq_lock(rq, &rf);
	hrtick_cond_restart(rq);
	rq_unlock(rq, &rf);
}

/*
 * Called to set the hrtick timer state.
 *
 * called with rq->lock held and IRQs disabled
 */
ଶୂନ୍ୟ hrtick_start(ସଂରଚନା rq *rq, u64 delay)
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
	ଯଦି (rq->hrtick_sched) {
		rq->hrtick_sched |= HRTICK_SCHED_START;
		rq->hrtick_delay = delta;
		ଫେରନ୍ତୁ;
	}

	rq->hrtick_time = ktime_add_ns(ktime_get(), delta);
	ଯଦି (!hrtick_needs_rearm(&rq->hrtick_timer, rq->hrtick_time))
		ଫେରନ୍ତୁ;

	ଯଦି (rq == this_rq())
		hrtimer_start(&rq->hrtick_timer, rq->hrtick_time, HRTIMER_MODE_ABS_PINNED_HARD);
	ଅନ୍ୟଥା
		smp_call_function_single_async(cpu_of(rq), &rq->hrtick_csd);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ hrtick_schedule_enter(ସଂରଚନା rq *rq)
{
	rq->hrtick_sched = HRTICK_SCHED_DEFER;
	ଯଦି (hrtimer_test_and_clear_rearm_deferred())
		rq->hrtick_sched |= HRTICK_SCHED_REARM_HRTIMER;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ hrtick_schedule_exit(ସଂରଚନା rq *rq)
{
	ଯଦି (rq->hrtick_sched & HRTICK_SCHED_START) {
		rq->hrtick_time = ktime_add_ns(ktime_get(), rq->hrtick_delay);
		hrtick_cond_restart(rq);
	} ଅନ୍ୟଥା ଯଦି (idle_rq(rq)) {
		/*
		 * No need for using hrtimer_is_active(). The timer is CPU local
		 * and interrupts are disabled, so the callback cannot be
		 * running and the queued state is valid.
		 */
		ଯଦି (hrtimer_is_queued(&rq->hrtick_timer))
			hrtimer_cancel(&rq->hrtick_timer);
	}

	ଯଦି (rq->hrtick_sched & HRTICK_SCHED_REARM_HRTIMER)
		__hrtimer_rearm_deferred();

	rq->hrtick_sched = HRTICK_SCHED_NONE;
}

ସ୍ଥିର ଶୂନ୍ୟ hrtick_rq_init(ସଂରଚନା rq *rq)
{
	INIT_CSD(&rq->hrtick_csd, __hrtick_start, rq);
	rq->hrtick_sched = HRTICK_SCHED_NONE;
	hrtimer_setup(&rq->hrtick_timer, hrtick, CLOCK_MONOTONIC,
		      HRTIMER_MODE_REL_HARD | HRTIMER_MODE_LAZY_REARM);
}
#ଅନ୍ୟଥା /* !CONFIG_SCHED_HRTICK: */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ hrtick_clear(ସଂରଚନା rq *rq) { }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ hrtick_rq_init(ସଂରଚନା rq *rq) { }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ hrtick_schedule_enter(ସଂରଚନା rq *rq) { }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ hrtick_schedule_exit(ସଂରଚନା rq *rq) { }
#endif /* !CONFIG_SCHED_HRTICK */

/*
 * try_cmpxchg based fetch_or() macro so it works for different integer types:
 */
#define fetch_or(ptr, mask)						\
	({								\
		typeof(ptr) _ptr = (ptr);				\
		typeof(mask) _mask = (mask);				\
		typeof(*_ptr) _val = *_ptr;				\
									\
		କର {							\
		} ଯେତେବେଳେ (!try_cmpxchg(_ptr, &_val, _val | _mask));	\
	_val;								\
})

#ifdef TIF_POLLING_NRFLAG
/*
 * Atomically set TIF_NEED_RESCHED and test for TIF_POLLING_NRFLAG,
 * this avoids any races wrt polling state changes and thereby avoids
 * spurious IPIs.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool set_nr_and_not_polling(ସଂରଚନା thread_info *ti, ପୂର୍ଣ୍ଣସଂଖ୍ୟା tif)
{
	ଫେରନ୍ତୁ !(fetch_or(&ti->flags, 1 << tif) & _TIF_POLLING_NRFLAG);
}

/*
 * Atomically set TIF_NEED_RESCHED if TIF_POLLING_NRFLAG is set.
 *
 * If this returns true, then the idle task promises to call
 * sched_ttwu_pending() and reschedule soon.
 */
ସ୍ଥିର bool set_nr_if_polling(ସଂରଚନା task_struct *p)
{
	ସଂରଚନା thread_info *ti = task_thread_info(p);
	typeof(ti->flags) val = READ_ONCE(ti->flags);

	କର {
		ଯଦି (!(val & _TIF_POLLING_NRFLAG))
			ଫେରନ୍ତୁ false;
		ଯଦି (val & _TIF_NEED_RESCHED)
			ଫେରନ୍ତୁ true;
	} ଯେତେବେଳେ (!try_cmpxchg(&ti->flags, &val, val | _TIF_NEED_RESCHED));

	ଫେରନ୍ତୁ true;
}

#ଅନ୍ୟଥା
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool set_nr_and_not_polling(ସଂରଚନା thread_info *ti, ପୂର୍ଣ୍ଣସଂଖ୍ୟା tif)
{
	set_ti_thread_flag(ti, tif);
	ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool set_nr_if_polling(ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ false;
}
#endif

ସ୍ଥିର bool __wake_q_add(ସଂରଚନା wake_q_head *head, ସଂରଚନା task_struct *task)
{
	ସଂରଚନା wake_q_ଗଣ୍ଠି *ଗଣ୍ଠି = &task->wake_q;

	/*
	 * Atomically grab the task, if ->wake_q is !nil already it means
	 * it's already queued (either by us or someone else) and will get the
	 * wakeup due to that.
	 *
	 * In order to ensure that a pending wakeup will observe our pending
	 * state, even in the failed case, an explicit smp_mb() must be used.
	 */
	smp_mb__before_atomic();
	ଯଦି (unlikely(cmpxchg_relaxed(&ଗଣ୍ଠି->next, ଖାଲି, WAKE_Q_TAIL)))
		ଫେରନ୍ତୁ false;

	/*
	 * The head is context local, there can be no concurrency.
	 */
	*head->lastp = ଗଣ୍ଠି;
	head->lastp = &ଗଣ୍ଠି->next;
	ଫେରନ୍ତୁ true;
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
ଶୂନ୍ୟ wake_q_add(ସଂରଚନା wake_q_head *head, ସଂରଚନା task_struct *task)
{
	ଯଦି (__wake_q_add(head, task))
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
ଶୂନ୍ୟ wake_q_add_safe(ସଂରଚନା wake_q_head *head, ସଂରଚନା task_struct *task)
{
	ଯଦି (!__wake_q_add(head, task))
		put_task_struct(task);
}

ଶୂନ୍ୟ wake_up_q(ସଂରଚନା wake_q_head *head)
{
	ସଂରଚନା wake_q_ଗଣ୍ଠି *ଗଣ୍ଠି = head->first;

	ଯେତେବେଳେ (ଗଣ୍ଠି != WAKE_Q_TAIL) {
		ସଂରଚନା task_struct *task;

		task = container_of(ଗଣ୍ଠି, ସଂରଚନା task_struct, wake_q);
		ଗଣ୍ଠି = ଗଣ୍ଠି->next;
		/* pairs with cmpxchg_relaxed() in __wake_q_add() */
		WRITE_ONCE(task->wake_q.next, ଖାଲି);
		/* Task can safely be re-inserted now. */

		/*
		 * wake_up_process() executes a full barrier, which pairs with
		 * the queueing in wake_q_add() so as not to miss wakeups.
		 */
		wake_up_process(task);
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
ସ୍ଥିର ଶୂନ୍ୟ __resched_curr(ସଂରଚନା rq *rq, ପୂର୍ଣ୍ଣସଂଖ୍ୟା tif)
{
	ସଂରଚନା task_struct *curr = rq->curr;
	ସଂରଚନା thread_info *cti = task_thread_info(curr);
	bool need_ipi;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

	lockdep_assert_rq_held(rq);

	/*
	 * Always immediately preempt the idle task; no point in delaying doing
	 * actual work.
	 */
	ଯଦି (is_idle_task(curr) && tif == TIF_NEED_RESCHED_LAZY)
		tif = TIF_NEED_RESCHED;

	ଯଦି (cti->flags & ((1 << tif) | _TIF_NEED_RESCHED))
		ଫେରନ୍ତୁ;

	cpu = cpu_of(rq);

	ଯଦି (cpu == smp_processor_id()) {
		set_ti_thread_flag(cti, tif);
		ଯଦି (tif == TIF_NEED_RESCHED)
			set_preempt_need_resched();
		trace_sched_set_need_resched_tp(curr, cpu, tif);
		ଫେରନ୍ତୁ;
	}

	need_ipi = set_nr_and_not_polling(cti, tif);
	trace_sched_set_need_resched_tp(curr, cpu, tif);
	ଯଦି (need_ipi) {
		ଯଦି (tif == TIF_NEED_RESCHED)
			smp_send_reschedule(cpu);
	} ଅନ୍ୟଥା {
		trace_sched_wake_idle_without_ipi(cpu);
	}
}

/*
 * Calls to this function MUST be guarded by a
 * tracepoint_enabled(sched_set_need_resched_tp)
 */
ଶୂନ୍ୟ __trace_set_need_resched(ସଂରଚନା task_struct *curr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା tif)
{
	trace_call__sched_set_need_resched_tp(curr, smp_processor_id(), tif);
}
EXPORT_ପ୍ରତୀକ_GPL(__trace_set_need_resched);

ଶୂନ୍ୟ resched_curr(ସଂରଚନା rq *rq)
{
	__resched_curr(rq, TIF_NEED_RESCHED);
}

#ifdef CONFIG_PREEMPT_DYNAMIC
ସ୍ଥିର DEFINE_STATIC_ଚାବି_FALSE(sk_dynamic_preempt_lazy);
ସ୍ଥିର __always_inline bool dynamic_preempt_lazy(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ static_branch_unlikely(&sk_dynamic_preempt_lazy);
}
#ଅନ୍ୟଥା
ସ୍ଥିର __always_inline bool dynamic_preempt_lazy(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ IS_ENABLED(CONFIG_PREEMPT_LAZY);
}
#endif

ସ୍ଥିର __always_inline ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_lazy_tif_bit(ଶୂନ୍ୟ)
{
	ଯଦି (dynamic_preempt_lazy())
		ଫେରନ୍ତୁ TIF_NEED_RESCHED_LAZY;

	ଫେରନ୍ତୁ TIF_NEED_RESCHED;
}

ଶୂନ୍ୟ resched_curr_lazy(ସଂରଚନା rq *rq)
{
	__resched_curr(rq, get_lazy_tif_bit());
}

ଶୂନ୍ୟ resched_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା rq *rq = cpu_rq(cpu);
	unsigned long flags;

	raw_spin_rq_lock_irqsave(rq, flags);
	ଯଦି (cpu_online(cpu) || cpu == smp_processor_id())
		resched_curr(rq);
	raw_spin_rq_unlock_irqrestore(rq, flags);
}

#ifdef CONFIG_NO_HZ_COMMON
/*
 * In the semi idle case, use the nearest busy CPU for migrating timers
 * from an idle CPU.  This is good for power-savings.
 *
 * We don't do similar optimization for completely idle system, as
 * selecting an idle CPU will add more delays to the timers than intended
 * (as that CPU's timer base may not be up to date wrt jiffies etc).
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_nohz_timer_target(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, cpu = smp_processor_id(), default_cpu = -1;
	ସଂରଚନା sched_domain *sd;
	ଅଚଳ ସଂରଚନା cpumask *hk_mask;

	ଯଦି (housekeeping_cpu(cpu, HK_TYPE_KERNEL_NOISE)) {
		ଯଦି (!idle_cpu(cpu))
			ଫେରନ୍ତୁ cpu;
		default_cpu = cpu;
	}

	hk_mask = housekeeping_cpumask(HK_TYPE_KERNEL_NOISE);

	guard(rcu)();

	for_each_domain(cpu, sd) {
		for_each_cpu_and(i, sched_domain_span(sd), hk_mask) {
			ଯଦି (cpu == i)
				ଜାରି;

			ଯଦି (!idle_cpu(i))
				ଫେରନ୍ତୁ i;
		}
	}

	ଯଦି (default_cpu == -1)
		default_cpu = housekeeping_any_cpu(HK_TYPE_KERNEL_NOISE);

	ଫେରନ୍ତୁ default_cpu;
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
ସ୍ଥିର ଶୂନ୍ୟ wake_up_idle_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା rq *rq = cpu_rq(cpu);

	ଯଦି (cpu == smp_processor_id())
		ଫେରନ୍ତୁ;

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
	ଯଦି (set_nr_and_not_polling(task_thread_info(rq->idle), TIF_NEED_RESCHED))
		smp_send_reschedule(cpu);
	ଅନ୍ୟଥା
		trace_sched_wake_idle_without_ipi(cpu);
}

ସ୍ଥିର bool wake_up_full_nohz_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	/*
	 * We just need the target to call irq_exit() and re-evaluate
	 * the next tick. The nohz full kick at least implies that.
	 * If needed we can still optimize that later with an
	 * empty IRQ.
	 */
	ଯଦି (cpu_is_offline(cpu))
		ଫେରନ୍ତୁ true;  /* Don't try to wake offline CPUs. */
	ଯଦି (tick_nohz_full_cpu(cpu)) {
		ଯଦି (cpu != smp_processor_id() ||
		    tick_nohz_tick_stopped())
			tick_nohz_full_kick_cpu(cpu);
		ଫେରନ୍ତୁ true;
	}

	ଫେରନ୍ତୁ false;
}

/*
 * Wake up the specified CPU.  If the CPU is going offline, it is the
 * caller's responsibility to deal with the lost wakeup, for example,
 * by hooking into the CPU_DEAD notifier like timers and hrtimers do.
 */
ଶୂନ୍ୟ wake_up_nohz_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଯଦି (!wake_up_full_nohz_cpu(cpu))
		wake_up_idle_cpu(cpu);
}

ସ୍ଥିର ଶୂନ୍ୟ nohz_csd_func(ଶୂନ୍ୟ *info)
{
	ସଂରଚନା rq *rq = info;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = cpu_of(rq);
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags;

	/*
	 * Release the rq::nohz_csd.
	 */
	flags = atomic_fetch_andnot(NOHZ_KICK_MASK | NOHZ_NEWILB_KICK, nohz_flags(cpu));
	WARN_ON(!(flags & NOHZ_KICK_MASK));

	rq->idle_balance = idle_cpu(cpu);
	ଯଦି (rq->idle_balance) {
		rq->nohz_idle_balance = flags;
		__raise_softirq_irqoff(SCHED_SOFTIRQ);
	}
}

#endif /* CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_NO_HZ_FULL
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool __need_bw_check(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	ଯଦି (rq->nr_running != 1)
		ଫେରନ୍ତୁ false;

	ଯଦି (p->sched_class != &fair_sched_class)
		ଫେରନ୍ତୁ false;

	ଯଦି (!task_on_rq_queued(p))
		ଫେରନ୍ତୁ false;

	ଫେରନ୍ତୁ true;
}

bool sched_can_stop_tick(ସଂରଚନା rq *rq)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fifo_nr_running;

	/* Deadline tasks, even if single, need the tick */
	ଯଦି (rq->dl.dl_nr_running)
		ଫେରନ୍ତୁ false;

	/*
	 * If there are more than one RR tasks, we need the tick to affect the
	 * actual RR behaviour.
	 */
	ଯଦି (rq->rt.rr_nr_running) {
		ଯଦି (rq->rt.rr_nr_running == 1)
			ଫେରନ୍ତୁ true;
		ଅନ୍ୟଥା
			ଫେରନ୍ତୁ false;
	}

	/*
	 * If there's no RR tasks, but FIFO tasks, we can skip the tick, no
	 * forced preemption between FIFO tasks.
	 */
	fifo_nr_running = rq->rt.rt_nr_running - rq->rt.rr_nr_running;
	ଯଦି (fifo_nr_running)
		ଫେରନ୍ତୁ true;

	/*
	 * If there are no DL,RR/FIFO tasks, there must only be CFS or SCX tasks
	 * left. For CFS, if there's more than one we need the tick for
	 * involuntary preemption. For SCX, ask.
	 */
	ଯଦି (scx_enabled() && !scx_can_stop_tick(rq))
		ଫେରନ୍ତୁ false;

	ଯଦି (rq->cfs.h_nr_queued > 1)
		ଫେରନ୍ତୁ false;

	/*
	 * If there is one task and it has CFS runtime bandwidth constraints
	 * and it's on the cpu now we don't want to stop the tick.
	 * This check prevents clearing the bit if a newly enqueued task here is
	 * dequeued by migrating while the constrained task continues to run.
	 * E.g. going from 2->1 without going through pick_next_task().
	 */
	ଯଦି (__need_bw_check(rq, rq->curr)) {
		ଯଦି (cfs_task_bw_constrained(rq->curr))
			ଫେରନ୍ତୁ false;
	}

	ଫେରନ୍ତୁ true;
}
#endif /* CONFIG_NO_HZ_FULL */

#ଯଦି defined(CONFIG_RT_GROUP_SCHED) || defined(CONFIG_FAIR_GROUP_SCHED)
/*
 * Iterate task_group tree rooted at *from, calling @down when first entering a
 * node and @up when leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା walk_tg_ବୃକ୍ଷ_from(ସଂରଚନା task_group *from,
			     tg_visitor down, tg_visitor up, ଶୂନ୍ୟ *data)
{
	ସଂରଚନା task_group *parent, *child;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	parent = from;

down:
	ret = (*down)(parent, data);
	ଯଦି (ret)
		ଯାଅ out;
	list_for_each_entry_rcu(child, &parent->children, siblings) {
		parent = child;
		ଯାଅ down;

up:
		ଜାରି;
	}
	ret = (*up)(parent, data);
	ଯଦି (ret || parent == from)
		ଯାଅ out;

	child = parent;
	parent = parent->parent;
	ଯଦି (parent)
		ଯାଅ up;
out:
	ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା tg_nop(ସଂରଚନା task_group *tg, ଶୂନ୍ୟ *data)
{
	ଫେରନ୍ତୁ 0;
}
#endif

ଶୂନ୍ୟ set_load_weight(ସଂରଚନା task_struct *p, bool update_load)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା prio = p->static_prio - MAX_RT_PRIO;
	ସଂରଚନା load_weight lw;

	ଯଦି (task_has_idle_policy(p)) {
		lw.weight = scale_load(WEIGHT_IDLEPRIO);
		lw.inv_weight = WMULT_IDLEPRIO;
	} ଅନ୍ୟଥା {
		lw.weight = scale_load(sched_prio_to_weight[prio]);
		lw.inv_weight = sched_prio_to_wmult[prio];
	}

	/*
	 * SCHED_OTHER tasks have to update their load when changing their
	 * weight
	 */
	ଯଦି (update_load && p->sched_class->reweight_task)
		p->sched_class->reweight_task(task_rq(p), p, &lw);
	ଅନ୍ୟଥା
		p->se.load = lw;
}

#ifdef CONFIG_UCLAMP_TASK
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
ସ୍ଥିର __maybe_unused DEFINE_MUTEX(uclamp_mutex);

/* Max allowed minimum utilization */
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା __maybe_unused sysctl_sched_uclamp_util_min = SCHED_CAPACITY_SCALE;

/* Max allowed maximum utilization */
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା __maybe_unused sysctl_sched_uclamp_util_max = SCHED_CAPACITY_SCALE;

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
unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_sched_uclamp_util_min_rt_default = SCHED_CAPACITY_SCALE;

/* All clamps are required to be less or equal than these values */
ସ୍ଥିର ସଂରଚନା uclamp_se uclamp_default[UCLAMP_CNT];

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
DEFINE_STATIC_ଚାବି_FALSE(sched_uclamp_used);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
uclamp_idle_ମୂଲ୍ୟ(ସଂରଚନା rq *rq, ଗଣନା uclamp_id clamp_id,
		  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା clamp_ମୂଲ୍ୟ)
{
	/*
	 * Avoid blocked utilization pushing up the frequency when we go
	 * idle (which drops the max-clamp) by retaining the last known
	 * max-clamp.
	 */
	ଯଦି (clamp_id == UCLAMP_MAX) {
		rq->uclamp_flags |= UCLAMP_FLAG_IDLE;
		ଫେରନ୍ତୁ clamp_ମୂଲ୍ୟ;
	}

	ଫେରନ୍ତୁ uclamp_none(UCLAMP_MIN);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ uclamp_idle_reset(ସଂରଚନା rq *rq, ଗଣନା uclamp_id clamp_id,
				     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା clamp_ମୂଲ୍ୟ)
{
	/* Reset max-clamp retention only on idle exit */
	ଯଦି (!(rq->uclamp_flags & UCLAMP_FLAG_IDLE))
		ଫେରନ୍ତୁ;

	uclamp_rq_set(rq, clamp_id, clamp_ମୂଲ୍ୟ);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା uclamp_rq_max_ମୂଲ୍ୟ(ସଂରଚନା rq *rq, ଗଣନା uclamp_id clamp_id,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା clamp_ମୂଲ୍ୟ)
{
	ସଂରଚନା uclamp_bucket *bucket = rq->uclamp[clamp_id].bucket;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା bucket_id = UCLAMP_BUCKETS - 1;

	/*
	 * Since both min and max clamps are max aggregated, find the
	 * top most bucket with tasks in.
	 */
	ପାଇଁ ( ; bucket_id >= 0; bucket_id--) {
		ଯଦି (!bucket[bucket_id].tasks)
			ଜାରି;
		ଫେରନ୍ତୁ bucket[bucket_id].ମୂଲ୍ୟ;
	}

	/* No tasks -- default clamp values */
	ଫେରନ୍ତୁ uclamp_idle_ମୂଲ୍ୟ(rq, clamp_id, clamp_ମୂଲ୍ୟ);
}

ସ୍ଥିର ଶୂନ୍ୟ __uclamp_update_util_min_rt_default(ସଂରଚନା task_struct *p)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_util_min;
	ସଂରଚନା uclamp_se *uc_se;

	lockdep_assert_held(&p->pi_lock);

	uc_se = &p->uclamp_req[UCLAMP_MIN];

	/* Only sync if user didn't override the default */
	ଯଦି (uc_se->user_defined)
		ଫେରନ୍ତୁ;

	default_util_min = sysctl_sched_uclamp_util_min_rt_default;
	uclamp_se_set(uc_se, default_util_min, false);
}

ସ୍ଥିର ଶୂନ୍ୟ uclamp_update_util_min_rt_default(ସଂରଚନା task_struct *p)
{
	ଯଦି (!rt_task(p))
		ଫେରନ୍ତୁ;

	/* Protect updates to p->uclamp_* */
	guard(task_rq_lock)(p);
	__uclamp_update_util_min_rt_default(p);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା uclamp_se
uclamp_tg_restrict(ସଂରଚନା task_struct *p, ଗଣନା uclamp_id clamp_id)
{
	/* Copy by value as we could modify it */
	ସଂରଚନା uclamp_se uc_req = p->uclamp_req[clamp_id];
#ifdef CONFIG_UCLAMP_TASK_GROUP
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା tg_min, tg_max, ମୂଲ୍ୟ;

	/*
	 * Tasks in autogroups or root task group will be
	 * restricted by system defaults.
	 */
	ଯଦି (task_group_is_autogroup(task_group(p)))
		ଫେରନ୍ତୁ uc_req;
	ଯଦି (task_group(p) == &ମୂଳ_task_group)
		ଫେରନ୍ତୁ uc_req;

	tg_min = task_group(p)->uclamp[UCLAMP_MIN].ମୂଲ୍ୟ;
	tg_max = task_group(p)->uclamp[UCLAMP_MAX].ମୂଲ୍ୟ;
	ମୂଲ୍ୟ = uc_req.ମୂଲ୍ୟ;
	ମୂଲ୍ୟ = clamp(ମୂଲ୍ୟ, tg_min, tg_max);
	uclamp_se_set(&uc_req, ମୂଲ୍ୟ, false);
#endif

	ଫେରନ୍ତୁ uc_req;
}

/*
 * The effective clamp bucket index of a task depends on, by increasing
 * priority:
 * - the task specific clamp value, when explicitly requested from userspace
 * - the task group effective clamp value, for tasks not either in the root
 *   group or in an autogroup
 * - the system default clamp value, defined by the sysadmin
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା uclamp_se
uclamp_eff_get(ସଂରଚନା task_struct *p, ଗଣନା uclamp_id clamp_id)
{
	ସଂରଚନା uclamp_se uc_req = uclamp_tg_restrict(p, clamp_id);
	ସଂରଚନା uclamp_se uc_max = uclamp_default[clamp_id];

	/* System default restrictions always apply */
	ଯଦି (unlikely(uc_req.ମୂଲ୍ୟ > uc_max.ମୂଲ୍ୟ))
		ଫେରନ୍ତୁ uc_max;

	ଫେରନ୍ତୁ uc_req;
}

unsigned long uclamp_eff_ମୂଲ୍ୟ(ସଂରଚନା task_struct *p, ଗଣନା uclamp_id clamp_id)
{
	ସଂରଚନା uclamp_se uc_eff;

	/* Task currently refcounted: use back-annotated (effective) value */
	ଯଦି (p->uclamp[clamp_id].active)
		ଫେରନ୍ତୁ (unsigned long)p->uclamp[clamp_id].ମୂଲ୍ୟ;

	uc_eff = uclamp_eff_get(p, clamp_id);

	ଫେରନ୍ତୁ (unsigned long)uc_eff.ମୂଲ୍ୟ;
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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ uclamp_rq_inc_id(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p,
				    ଗଣନା uclamp_id clamp_id)
{
	ସଂରଚନା uclamp_rq *uc_rq = &rq->uclamp[clamp_id];
	ସଂରଚନା uclamp_se *uc_se = &p->uclamp[clamp_id];
	ସଂରଚନା uclamp_bucket *bucket;

	lockdep_assert_rq_held(rq);

	/* Update task effective clamp */
	p->uclamp[clamp_id] = uclamp_eff_get(p, clamp_id);

	bucket = &uc_rq->bucket[uc_se->bucket_id];
	bucket->tasks++;
	uc_se->active = true;

	uclamp_idle_reset(rq, clamp_id, uc_se->ମୂଲ୍ୟ);

	/*
	 * Local max aggregation: rq buckets always track the max
	 * "requested" clamp value of its RUNNABLE tasks.
	 */
	ଯଦି (bucket->tasks == 1 || uc_se->ମୂଲ୍ୟ > bucket->ମୂଲ୍ୟ)
		bucket->ମୂଲ୍ୟ = uc_se->ମୂଲ୍ୟ;

	ଯଦି (uc_se->ମୂଲ୍ୟ > uclamp_rq_get(rq, clamp_id))
		uclamp_rq_set(rq, clamp_id, uc_se->ମୂଲ୍ୟ);
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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ uclamp_rq_dec_id(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p,
				    ଗଣନା uclamp_id clamp_id)
{
	ସଂରଚନା uclamp_rq *uc_rq = &rq->uclamp[clamp_id];
	ସଂରଚନା uclamp_se *uc_se = &p->uclamp[clamp_id];
	ସଂରଚନା uclamp_bucket *bucket;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା bkt_clamp;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା rq_clamp;

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
	ଯଦି (unlikely(!uc_se->active))
		ଫେରନ୍ତୁ;

	bucket = &uc_rq->bucket[uc_se->bucket_id];

	WARN_ON_ONCE(!bucket->tasks);
	ଯଦି (likely(bucket->tasks))
		bucket->tasks--;

	uc_se->active = false;

	/*
	 * Keep "local max aggregation" simple and accept to (possibly)
	 * overboost some RUNNABLE tasks in the same bucket.
	 * The rq clamp bucket value is reset to its base value whenever
	 * there are no more RUNNABLE tasks refcounting it.
	 */
	ଯଦି (likely(bucket->tasks))
		ଫେରନ୍ତୁ;

	rq_clamp = uclamp_rq_get(rq, clamp_id);
	/*
	 * Defensive programming: this should never happen. If it happens,
	 * e.g. due to future modification, warn and fix up the expected value.
	 */
	WARN_ON_ONCE(bucket->ମୂଲ୍ୟ > rq_clamp);
	ଯଦି (bucket->ମୂଲ୍ୟ >= rq_clamp) {
		bkt_clamp = uclamp_rq_max_ମୂଲ୍ୟ(rq, clamp_id, uc_se->ମୂଲ୍ୟ);
		uclamp_rq_set(rq, clamp_id, bkt_clamp);
	}
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ uclamp_rq_inc(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ଗଣନା uclamp_id clamp_id;

	/*
	 * Avoid any overhead until uclamp is actually used by the userspace.
	 *
	 * The condition is constructed such that a NOP is generated when
	 * sched_uclamp_used is disabled.
	 */
	ଯଦି (!uclamp_is_used())
		ଫେରନ୍ତୁ;

	ଯଦି (unlikely(!p->sched_class->uclamp_enabled))
		ଫେରନ୍ତୁ;

	/* Only inc the delayed task which being woken up. */
	ଯଦି (p->se.sched_delayed && !(flags & ENQUEUE_DELAYED))
		ଫେରନ୍ତୁ;

	for_each_clamp_id(clamp_id)
		uclamp_rq_inc_id(rq, p, clamp_id);

	/* Reset clamp idle holding when there is one RUNNABLE task */
	ଯଦି (rq->uclamp_flags & UCLAMP_FLAG_IDLE)
		rq->uclamp_flags &= ~UCLAMP_FLAG_IDLE;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ uclamp_rq_dec(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	ଗଣନା uclamp_id clamp_id;

	/*
	 * Avoid any overhead until uclamp is actually used by the userspace.
	 *
	 * The condition is constructed such that a NOP is generated when
	 * sched_uclamp_used is disabled.
	 */
	ଯଦି (!uclamp_is_used())
		ଫେରନ୍ତୁ;

	ଯଦି (unlikely(!p->sched_class->uclamp_enabled))
		ଫେରନ୍ତୁ;

	ଯଦି (p->se.sched_delayed)
		ଫେରନ୍ତୁ;

	for_each_clamp_id(clamp_id)
		uclamp_rq_dec_id(rq, p, clamp_id);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ uclamp_rq_reinc_id(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p,
				      ଗଣନା uclamp_id clamp_id)
{
	ଯଦି (!p->uclamp[clamp_id].active)
		ଫେରନ୍ତୁ;

	uclamp_rq_dec_id(rq, p, clamp_id);
	uclamp_rq_inc_id(rq, p, clamp_id);

	/*
	 * Make sure to clear the idle flag if we've transiently reached 0
	 * active tasks on rq.
	 */
	ଯଦି (clamp_id == UCLAMP_MAX && (rq->uclamp_flags & UCLAMP_FLAG_IDLE))
		rq->uclamp_flags &= ~UCLAMP_FLAG_IDLE;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
uclamp_update_active(ସଂରଚନା task_struct *p)
{
	ଗଣନା uclamp_id clamp_id;
	ସଂରଚନା rq_flags rf;
	ସଂରଚନା rq *rq;

	/*
	 * Lock the task and the rq where the task is (or was) queued.
	 *
	 * We might lock the (previous) rq of a !RUNNABLE task, but that's the
	 * price to pay to safely serialize util_{min,max} updates with
	 * enqueues, dequeues and migration operations.
	 * This is the same locking schema used by __set_cpus_allowed_ptr().
	 */
	rq = task_rq_lock(p, &rf);

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

#ifdef CONFIG_UCLAMP_TASK_GROUP
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
uclamp_update_active_tasks(ସଂରଚନା cgroup_subsys_state *css)
{
	ସଂରଚନା css_task_iter it;
	ସଂରଚନା task_struct *p;

	css_task_iter_start(css, 0, &it);
	ଯେତେବେଳେ ((p = css_task_iter_next(&it)))
		uclamp_update_active(p);
	css_task_iter_end(&it);
}

ସ୍ଥିର ଶୂନ୍ୟ cpu_util_update_eff(ସଂରଚନା cgroup_subsys_state *css);
#endif

#ifdef CONFIG_SYSCTL
#ifdef CONFIG_UCLAMP_TASK_GROUP
ସ୍ଥିର ଶୂନ୍ୟ uclamp_update_ମୂଳ_tg(ଶୂନ୍ୟ)
{
	ସଂରଚନା task_group *tg = &ମୂଳ_task_group;

	uclamp_se_set(&tg->uclamp_req[UCLAMP_MIN],
		      sysctl_sched_uclamp_util_min, false);
	uclamp_se_set(&tg->uclamp_req[UCLAMP_MAX],
		      sysctl_sched_uclamp_util_max, false);

	guard(rcu)();
	cpu_util_update_eff(&ମୂଳ_task_group.css);
}
#ଅନ୍ୟଥା
ସ୍ଥିର ଶୂନ୍ୟ uclamp_update_ମୂଳ_tg(ଶୂନ୍ୟ) { }
#endif

ସ୍ଥିର ଶୂନ୍ୟ uclamp_sync_util_min_rt_default(ଶୂନ୍ୟ)
{
	ସଂରଚନା task_struct *g, *p;

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
	read_lock(&tasklist_lock);
	smp_mb__after_spinlock();
	read_unlock(&tasklist_lock);

	guard(rcu)();
	for_each_process_thread(g, p)
		uclamp_update_util_min_rt_default(p);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_sched_uclamp_handler(ଅଚଳ ସଂରଚନା ctl_table *table, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଲିଖ,
				ଶୂନ୍ୟ *buffer, size_t *lenp, loff_t *ppos)
{
	bool update_ମୂଳ_tg = false;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_min, old_max, old_min_rt;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result;

	guard(mutex)(&uclamp_mutex);

	old_min = sysctl_sched_uclamp_util_min;
	old_max = sysctl_sched_uclamp_util_max;
	old_min_rt = sysctl_sched_uclamp_util_min_rt_default;

	result = proc_dointvec(table, ଲିଖ, buffer, lenp, ppos);
	ଯଦି (result)
		ଯାଅ undo;
	ଯଦି (!ଲିଖ)
		ଫେରନ୍ତୁ 0;

	ଯଦି (sysctl_sched_uclamp_util_min > sysctl_sched_uclamp_util_max ||
	    sysctl_sched_uclamp_util_max > SCHED_CAPACITY_SCALE	||
	    sysctl_sched_uclamp_util_min_rt_default > SCHED_CAPACITY_SCALE) {

		result = -EINVAL;
		ଯାଅ undo;
	}

	ଯଦି (old_min != sysctl_sched_uclamp_util_min) {
		uclamp_se_set(&uclamp_default[UCLAMP_MIN],
			      sysctl_sched_uclamp_util_min, false);
		update_ମୂଳ_tg = true;
	}
	ଯଦି (old_max != sysctl_sched_uclamp_util_max) {
		uclamp_se_set(&uclamp_default[UCLAMP_MAX],
			      sysctl_sched_uclamp_util_max, false);
		update_ମୂଳ_tg = true;
	}

	ଯଦି (update_ମୂଳ_tg) {
		sched_uclamp_enable();
		uclamp_update_ମୂଳ_tg();
	}

	ଯଦି (old_min_rt != sysctl_sched_uclamp_util_min_rt_default) {
		sched_uclamp_enable();
		uclamp_sync_util_min_rt_default();
	}

	/*
	 * We update all RUNNABLE tasks only when task groups are in use.
	 * Otherwise, keep it simple and do just a lazy update at each next
	 * task enqueue time.
	 */
	ଫେରନ୍ତୁ 0;

undo:
	sysctl_sched_uclamp_util_min = old_min;
	sysctl_sched_uclamp_util_max = old_max;
	sysctl_sched_uclamp_util_min_rt_default = old_min_rt;
	ଫେରନ୍ତୁ result;
}
#endif /* CONFIG_SYSCTL */

ସ୍ଥିର ଶୂନ୍ୟ uclamp_fork(ସଂରଚନା task_struct *p)
{
	ଗଣନା uclamp_id clamp_id;

	/*
	 * We don't need to hold task_rq_lock() when updating p->uclamp_* here
	 * as the task is still at its early fork stages.
	 */
	for_each_clamp_id(clamp_id)
		p->uclamp[clamp_id].active = false;

	ଯଦି (likely(!p->sched_reset_on_fork))
		ଫେରନ୍ତୁ;

	for_each_clamp_id(clamp_id) {
		uclamp_se_set(&p->uclamp_req[clamp_id],
			      uclamp_none(clamp_id), false);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ uclamp_post_fork(ସଂରଚନା task_struct *p)
{
	uclamp_update_util_min_rt_default(p);
}

ସ୍ଥିର ଶୂନ୍ୟ __init init_uclamp_rq(ସଂରଚନା rq *rq)
{
	ଗଣନା uclamp_id clamp_id;
	ସଂରଚନା uclamp_rq *uc_rq = rq->uclamp;

	for_each_clamp_id(clamp_id) {
		uc_rq[clamp_id] = (ସଂରଚନା uclamp_rq) {
			.ମୂଲ୍ୟ = uclamp_none(clamp_id)
		};
	}

	rq->uclamp_flags = UCLAMP_FLAG_IDLE;
}

ସ୍ଥିର ଶୂନ୍ୟ __init init_uclamp(ଶୂନ୍ୟ)
{
	ସଂରଚନା uclamp_se uc_max = {};
	ଗଣନା uclamp_id clamp_id;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

	for_each_possible_cpu(cpu)
		init_uclamp_rq(cpu_rq(cpu));

	for_each_clamp_id(clamp_id) {
		uclamp_se_set(&init_task.uclamp_req[clamp_id],
			      uclamp_none(clamp_id), false);
	}

	/* System defaults allow max clamp values for both indexes */
	uclamp_se_set(&uc_max, uclamp_none(UCLAMP_MAX), false);
	for_each_clamp_id(clamp_id) {
		uclamp_default[clamp_id] = uc_max;
#ifdef CONFIG_UCLAMP_TASK_GROUP
		ମୂଳ_task_group.uclamp_req[clamp_id] = uc_max;
		ମୂଳ_task_group.uclamp[clamp_id] = uc_max;
#endif
	}
}

#ଅନ୍ୟଥା /* !CONFIG_UCLAMP_TASK: */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ uclamp_rq_inc(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags) { }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ uclamp_rq_dec(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p) { }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ uclamp_fork(ସଂରଚନା task_struct *p) { }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ uclamp_post_fork(ସଂରଚନା task_struct *p) { }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ init_uclamp(ଶୂନ୍ୟ) { }
#endif /* !CONFIG_UCLAMP_TASK */

bool sched_task_on_rq(ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ task_on_rq_queued(p);
}

unsigned long get_wchan(ସଂରଚନା task_struct *p)
{
	unsigned long ip = 0;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା state;

	ଯଦି (!p || p == current)
		ଫେରନ୍ତୁ 0;

	/* Only get wchan if task is blocked and we can keep it that way. */
	raw_spin_lock_irq(&p->pi_lock);
	state = READ_ONCE(p->__state);
	smp_rmb(); /* see try_to_wake_up() */
	ଯଦି (state != TASK_RUNNING && state != TASK_WAKING && !p->on_rq)
		ip = __get_wchan(p);
	raw_spin_unlock_irq(&p->pi_lock);

	ଫେରନ୍ତୁ ip;
}

ଶୂନ୍ୟ enqueue_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ଯଦି (!(flags & ENQUEUE_NOCLOCK))
		update_rq_clock(rq);

	/*
	 * Can be before ->enqueue_task() because uclamp considers the
	 * ENQUEUE_DELAYED task before its ->sched_delayed gets cleared
	 * in ->enqueue_task().
	 */
	uclamp_rq_inc(rq, p, flags);

	p->sched_class->enqueue_task(rq, p, flags);

	psi_enqueue(p, flags);

	ଯଦି (!(flags & ENQUEUE_RESTORE))
		sched_info_enqueue(rq, p);

	ଯଦି (sched_core_enabled(rq))
		sched_core_enqueue(rq, p);
}

/*
 * Must only return false when DEQUEUE_SLEEP.
 */
ଧାଡ଼ିଭିତରେ bool dequeue_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ଯଦି (sched_core_enabled(rq))
		sched_core_dequeue(rq, p, flags);

	ଯଦି (!(flags & DEQUEUE_NOCLOCK))
		update_rq_clock(rq);

	ଯଦି (!(flags & DEQUEUE_SAVE))
		sched_info_dequeue(rq, p);

	psi_dequeue(p, flags);

	/*
	 * Must be before ->dequeue_task() because ->dequeue_task() can 'fail'
	 * and mark the task ->sched_delayed.
	 */
	uclamp_rq_dec(rq, p);
	ଫେରନ୍ତୁ p->sched_class->dequeue_task(rq, p, flags);
}

ଶୂନ୍ୟ activate_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ଯଦି (task_on_rq_migrating(p))
		flags |= ENQUEUE_MIGRATED;

	enqueue_task(rq, p, flags);

	WRITE_ONCE(p->on_rq, TASK_ON_RQ_QUEUED);
	ASSERT_EXCLUSIVE_WRITER(p->on_rq);
}

ଶୂନ୍ୟ deactivate_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	WARN_ON_ONCE(flags & DEQUEUE_SLEEP);

	WRITE_ONCE(p->on_rq, TASK_ON_RQ_MIGRATING);
	ASSERT_EXCLUSIVE_WRITER(p->on_rq);

	/*
	 * Code explicitly relies on TASK_ON_RQ_MIGRATING begin set *before*
	 * dequeue_task() and cleared *after* enqueue_task().
	 */

	dequeue_task(rq, p, flags);
}

ସ୍ଥିର bool dequeue_block_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p,
			       unsigned long task_state)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = DEQUEUE_NOCLOCK;

	p->sched_contributes_to_load =
		(task_state & TASK_UNINTERRUPTIBLE) &&
		!(task_state & TASK_NOLOAD) &&
		!(task_state & TASK_FROZEN);

	ଯଦି (unlikely(is_special_task_state(task_state)))
		flags |= DEQUEUE_SPECIAL;

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
	ଫେରନ୍ତୁ dequeue_task(rq, p, DEQUEUE_SLEEP | flags);
}

ସ୍ଥିର ଶୂନ୍ୟ block_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, unsigned long task_state)
{
	ଯଦି (dequeue_block_task(rq, p, task_state))
		__block_task(rq, p);
}

/**
 * task_curr - is this task currently executing on a CPU?
 * @p: the task in question.
 *
 * Return: 1 if the task is currently executing. 0 otherwise.
 */
ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା task_curr(ଅଚଳ ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ cpu_curr(task_cpu(p)) == p;
}

ଶୂନ୍ୟ wakeup_preempt(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା task_struct *donor = rq->donor;

	ଯଦି (p->sched_class == rq->next_class) {
		rq->next_class->wakeup_preempt(rq, p, flags);

	} ଅନ୍ୟଥା ଯଦି (sched_class_above(p->sched_class, rq->next_class)) {
		rq->next_class->wakeup_preempt(rq, p, flags);
		resched_curr(rq);
		rq->next_class = p->sched_class;
	}

	/*
	 * A queue event has occurred, and we're going to schedule.  In
	 * this case, we can save a useless back to back clock update.
	 */
	ଯଦି (task_on_rq_queued(donor) && test_tsk_need_resched(rq->curr))
		rq_clock_skip_update(rq);
}

ସ୍ଥିର __always_inline
ପୂର୍ଣ୍ଣସଂଖ୍ୟା __task_state_match(ସଂରଚନା task_struct *p, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା state)
{
	ଯଦି (READ_ONCE(p->__state) & state)
		ଫେରନ୍ତୁ 1;

	ଯଦି (READ_ONCE(p->saved_state) & state)
		ଫେରନ୍ତୁ -1;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର __always_inline
ପୂର୍ଣ୍ଣସଂଖ୍ୟା task_state_match(ସଂରଚନା task_struct *p, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା state)
{
	/*
	 * Serialize against current_save_and_set_rtlock_wait_state(),
	 * current_restore_rtlock_saved_state(), and __refrigerator().
	 */
	guard(raw_spinlock_irq)(&p->pi_lock);
	ଫେରନ୍ତୁ __task_state_match(p, state);
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
unsigned long wait_task_inactive(ସଂରଚନା task_struct *p, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା match_state)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା running, queued, match;
	ସଂରଚନା rq_flags rf;
	unsigned long ncsw;
	ସଂରଚନା rq *rq;

	ପାଇଁ (;;) {
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
		ଯେତେବେଳେ (task_on_cpu(rq, p)) {
			ଯଦି (!task_state_match(p, match_state))
				ଫେରନ୍ତୁ 0;
			cpu_relax();
		}

		/*
		 * Ok, time to look more closely! We need the rq
		 * lock now, to be *sure*. If we're wrong, we'll
		 * just go back and repeat.
		 */
		rq = task_rq_lock(p, &rf);
		/*
		 * If task is sched_delayed, force dequeue it, to avoid always
		 * hitting the tick timeout in the queued case
		 */
		ଯଦି (p->se.sched_delayed)
			dequeue_task(rq, p, DEQUEUE_SLEEP | DEQUEUE_DELAYED);
		trace_sched_wait_task(p);
		running = task_on_cpu(rq, p);
		queued = task_on_rq_queued(p);
		ncsw = 0;
		ଯଦି ((match = __task_state_match(p, match_state))) {
			/*
			 * When matching on p->saved_state, consider this task
			 * still queued so it will wait.
			 */
			ଯଦି (match < 0)
				queued = 1;
			ncsw = p->nvcsw | LONG_MIN; /* sets MSB */
		}
		task_rq_unlock(rq, p, &rf);

		/*
		 * If it changed from the expected state, bail out now.
		 */
		ଯଦି (unlikely(!ncsw))
			ଭାଙ୍ଗ;

		/*
		 * Was it really running after all now that we
		 * checked with the proper locks actually held?
		 *
		 * Oops. Go back and try again..
		 */
		ଯଦି (unlikely(running)) {
			cpu_relax();
			ଜାରି;
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
		ଯଦି (unlikely(queued)) {
			ktime_t to = NSEC_PER_SEC / HZ;

			set_current_state(TASK_UNINTERRUPTIBLE);
			schedule_hrtimeout(&to, HRTIMER_MODE_REL_HARD);
			ଜାରି;
		}

		/*
		 * Ahh, all good. It wasn't running, and it wasn't
		 * runnable, which means that it will never become
		 * running in the future either. We're all done!
		 */
		ଭାଙ୍ଗ;
	}

	ଫେରନ୍ତୁ ncsw;
}

ସ୍ଥିର ଶୂନ୍ୟ
do_set_cpus_allowed(ସଂରଚନା task_struct *p, ସଂରଚନା affinity_context *ctx);

ସ୍ଥିର ଶୂନ୍ୟ migrate_disable_switch(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	ସଂରଚନା affinity_context ac = {
		.new_mask  = cpumask_of(rq->cpu),
		.flags     = SCA_MIGRATE_DISABLE,
	};

	ଯଦି (likely(!p->migration_disabled))
		ଫେରନ୍ତୁ;

	ଯଦି (p->cpus_ptr != &p->cpus_mask)
		ଫେରନ୍ତୁ;

	scoped_guard (task_rq_lock, p)
		do_set_cpus_allowed(p, &ac);
}

ଶୂନ୍ୟ ___migrate_enable(ଶୂନ୍ୟ)
{
	ସଂରଚନା task_struct *p = current;
	ସଂରଚନା affinity_context ac = {
		.new_mask  = &p->cpus_mask,
		.flags     = SCA_MIGRATE_ENABLE,
	};

	__set_cpus_allowed_ptr(p, &ac);
}
EXPORT_ପ୍ରତୀକ_GPL(___migrate_enable);

ଶୂନ୍ୟ migrate_disable(ଶୂନ୍ୟ)
{
	__migrate_disable();
}
EXPORT_ପ୍ରତୀକ_GPL(migrate_disable);

ଶୂନ୍ୟ migrate_enable(ଶୂନ୍ୟ)
{
	__migrate_enable();
}
EXPORT_ପ୍ରତୀକ_GPL(migrate_enable);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool rq_has_pinned_tasks(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ rq->nr_pinned;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool task_can_migrate_to_preferred(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	/* No need to migrate from a preferred CPU */
	ଯଦି (cpu_preferred(cpu))
		ଫେରନ୍ତୁ false;

	/* Only FAIR tasks honor preferred CPU state */
	ଯଦି (unlikely(p->sched_class != &fair_sched_class))
		ଫେରନ୍ତୁ false;

	/* Ignore preferred state if task affinity is changing */
	ଯଦି (unlikely(!cpumask_test_cpu(task_cpu(p), p->cpus_ptr)))
		ଫେରନ୍ତୁ false;

	ଫେରନ୍ତୁ cpumask_intersects_and(p->cpus_ptr, cpu_preferred_mask,
				      task_cpu_possible_mask(p));
}

/*
 * Per-CPU kthreads are allowed to run on !active && online CPUs, see
 * __set_cpus_allowed_ptr() and select_fallback_rq().
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool is_cpu_allowed(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	/* When not in the task's cpumask, no point in looking further. */
	ଯଦି (!task_allowed_on_cpu(p, cpu))
		ଫେରନ୍ତୁ false;

	/* migrate_disabled() must be allowed to finish. */
	ଯଦି (is_migration_disabled(p))
		ଫେରନ୍ତୁ cpu_online(cpu);

	/* Non kernel threads are not allowed during either online or offline. */
	ଯଦି (!(p->flags & PF_KTHREAD)) {
		/* Try to use preferred CPU if task's affinity allows */
		ଯଦି (task_can_migrate_to_preferred(p, cpu))
			ଫେରନ୍ତୁ false;
		ଫେରନ୍ତୁ cpu_active(cpu);
	}

	/* KTHREAD_IS_PER_CPU is always allowed. */
	ଯଦି (kthread_is_per_cpu(p))
		ଫେରନ୍ତୁ cpu_online(cpu);

	/* Regular kernel threads don't get to stay during offline. */
	ଯଦି (cpu_dying(cpu))
		ଫେରନ୍ତୁ false;

	/* Try to keep unbound kthreads on a preferred CPU if possible. */
	ଯଦି (task_can_migrate_to_preferred(p, cpu))
		ଫେରନ୍ତୁ false;

	/* Otherwise, they are allowed to run on online CPU. */
	ଫେରନ୍ତୁ cpu_online(cpu);
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
ସ୍ଥିର ସଂରଚନା rq *move_queued_task(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf,
				   ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_cpu)
	__must_hold(__rq_lockp(rq))
{
	lockdep_assert_rq_held(rq);

	deactivate_task(rq, p, DEQUEUE_NOCLOCK);
	set_task_cpu(p, new_cpu);
	rq_unlock(rq, rf);

	rq = cpu_rq(new_cpu);

	rq_lock(rq, rf);
	WARN_ON_ONCE(task_cpu(p) != new_cpu);
	activate_task(rq, p, 0);
	wakeup_preempt(rq, p, 0);

	ଫେରନ୍ତୁ rq;
}

ସଂରଚନା migration_arg {
	ସଂରଚନା task_struct		*task;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା				dest_cpu;
	ସଂରଚନା set_affinity_pending	*pending;
};

/*
 * @refs: number of wait_for_completion()
 * @stop_pending: is @stop_work in use
 */
ସଂରଚନା set_affinity_pending {
	refcount_t		refs;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		stop_pending;
	ସଂରଚନା completion	done;
	ସଂରଚନା cpu_stop_work	stop_work;
	ସଂରଚନା migration_arg	arg;
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
ସ୍ଥିର ସଂରଚନା rq *__migrate_task(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf,
				 ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dest_cpu)
	__must_hold(__rq_lockp(rq))
{
	/* Affinity changed (again). */
	ଯଦି (!is_cpu_allowed(p, dest_cpu))
		ଫେରନ୍ତୁ rq;

	rq = move_queued_task(rq, rf, p, dest_cpu);

	ଫେରନ୍ତୁ rq;
}

/*
 * migration_cpu_stop - this will be executed by a high-prio stopper thread
 * and performs thread migration by bumping thread off CPU then
 * 'pushing' onto another runqueue.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା migration_cpu_stop(ଶୂନ୍ୟ *data)
{
	ସଂରଚନା migration_arg *arg = data;
	ସଂରଚନା set_affinity_pending *pending = arg->pending;
	ସଂରଚନା task_struct *p = arg->task;
	ସଂରଚନା rq *rq = this_rq();
	bool complete = false;
	ସଂରଚନା rq_flags rf;

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
	flush_smp_call_function_queue();

	/*
	 * We may change the underlying rq, but the locks held will
	 * appropriately be "transferred" when switching.
	 */
	context_unsafe_alias(rq);

	raw_spin_lock(&p->pi_lock);
	rq_lock(rq, &rf);

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
	ଯଦି (task_rq(p) == rq) {
		ଯଦି (is_migration_disabled(p))
			ଯାଅ out;

		ଯଦି (pending) {
			p->migration_pending = ଖାଲି;
			complete = true;

			ଯଦି (cpumask_test_cpu(task_cpu(p), &p->cpus_mask))
				ଯାଅ out;
		}

		ଯଦି (task_on_rq_queued(p)) {
			update_rq_clock(rq);
			rq = __migrate_task(rq, &rf, p, arg->dest_cpu);
		} ଅନ୍ୟଥା {
			p->wake_cpu = arg->dest_cpu;
		}

		/*
		 * XXX __migrate_task() can fail, at which point we might end
		 * up running on a dodgy CPU, AFAICT this can only happen
		 * during CPU hotplug, at which point we'll get pushed out
		 * anyway, so it's probably not a big deal.
		 */

	} ଅନ୍ୟଥା ଯଦି (pending) {
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
		ଯଦି (cpumask_test_cpu(task_cpu(p), p->cpus_ptr)) {
			p->migration_pending = ଖାଲି;
			complete = true;
			ଯାଅ out;
		}

		/*
		 * When migrate_enable() hits a rq mis-match we can't reliably
		 * determine is_migration_disabled() and so have to chase after
		 * it.
		 */
		WARN_ON_ONCE(!pending->stop_pending);
		preempt_disable();
		rq_unlock(rq, &rf);
		raw_spin_unlock_irqrestore(&p->pi_lock, rf.flags);
		stop_one_cpu_nowait(task_cpu(p), migration_cpu_stop,
				    &pending->arg, &pending->stop_work);
		preempt_enable();
		ଫେରନ୍ତୁ 0;
	}
out:
	ଯଦି (pending)
		pending->stop_pending = false;
	rq_unlock(rq, &rf);
	raw_spin_unlock_irqrestore(&p->pi_lock, rf.flags);

	ଯଦି (complete)
		complete_all(&pending->done);

	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଠେଲ_cpu_stop(ଶୂନ୍ୟ *arg)
{
	ସଂରଚନା rq *lowest_rq = ଖାଲି, *rq = this_rq();
	ସଂରଚନା task_struct *p = arg;

	raw_spin_lock_irq(&p->pi_lock);
	raw_spin_rq_lock(rq);

	ଯଦି (task_rq(p) != rq)
		ଯାଅ out_unlock;

	ଯଦି (is_migration_disabled(p)) {
		p->migration_flags |= MDF_ଠେଲ;
		ଯାଅ out_unlock;
	}

	p->migration_flags &= ~MDF_ଠେଲ;

	ଯଦି (p->sched_class->find_lock_rq)
		lowest_rq = p->sched_class->find_lock_rq(p, rq);

	ଯଦି (!lowest_rq)
		ଯାଅ out_unlock;

	lockdep_assert_rq_held(lowest_rq);

	// XXX validate p is still the highest prio task
	ଯଦି (task_rq(p) == rq) {
		move_queued_task_locked(rq, lowest_rq, p);
		resched_curr(lowest_rq);
	}

	double_unlock_balance(rq, lowest_rq);

out_unlock:
	rq->ଠେଲ_busy = false;
	raw_spin_rq_unlock(rq);
	raw_spin_unlock_irq(&p->pi_lock);

	put_task_struct(p);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ mm_update_cpus_allowed(ସଂରଚନା mm_struct *mm, ଅଚଳ cpumask_t *affmask);

/*
 * sched_class::set_cpus_allowed must do the below, but is not required to
 * actually call this function.
 */
ଶୂନ୍ୟ set_cpus_allowed_common(ସଂରଚନା task_struct *p, ସଂରଚନା affinity_context *ctx)
{
	ଯଦି (ctx->flags & (SCA_MIGRATE_ENABLE | SCA_MIGRATE_DISABLE)) {
		p->cpus_ptr = ctx->new_mask;
		ଫେରନ୍ତୁ;
	}

	cpumask_copy(&p->cpus_mask, ctx->new_mask);
	p->nr_cpus_allowed = cpumask_weight(ctx->new_mask);
	mm_update_cpus_allowed(p->mm, ctx->new_mask);

	/*
	 * Swap in a new user_cpus_ptr if SCA_USER flag set
	 */
	ଯଦି (ctx->flags & SCA_USER)
		swap(p->user_cpus_ptr, ctx->user_mask);
}

ସ୍ଥିର ଶୂନ୍ୟ
do_set_cpus_allowed(ସଂରଚନା task_struct *p, ସଂରଚନା affinity_context *ctx)
{
	scoped_guard (sched_change, p, DEQUEUE_SAVE)
		p->sched_class->set_cpus_allowed(p, ctx);
}

/*
 * Used for kthread_bind() and select_fallback_rq(), in both cases the user
 * affinity (if any) should be destroyed too.
 */
ଶୂନ୍ୟ set_cpus_allowed_force(ସଂରଚନା task_struct *p, ଅଚଳ ସଂରଚନା cpumask *new_mask)
{
	ସଂରଚନା affinity_context ac = {
		.new_mask  = new_mask,
		.user_mask = ଖାଲି,
		.flags     = SCA_USER,	/* clear the user requested mask */
	};
	ସଂଘ cpumask_rcuhead {
		cpumask_t cpumask;
		ସଂରଚନା rcu_head rcu;
	};

	scoped_guard (__task_rq_lock, p)
		do_set_cpus_allowed(p, &ac);

	/*
	 * Because this is called with p->pi_lock held, it is not possible
	 * to use kfree() here (when PREEMPT_RT=y), therefore punt to using
	 * kfree_rcu().
	 */
	kfree_rcu((ସଂଘ cpumask_rcuhead *)ac.user_mask, rcu);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା dup_user_cpus_ptr(ସଂରଚନା task_struct *dst, ସଂରଚନା task_struct *src,
		      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣ୍ଠି)
{
	cpumask_t *user_mask;
	unsigned long flags;

	/*
	 * Always clear dst->user_cpus_ptr first as their user_cpus_ptr's
	 * may differ by now due to racing.
	 */
	dst->user_cpus_ptr = ଖାଲି;

	/*
	 * This check is racy and losing the race is a valid situation.
	 * It is not worth the extra overhead of taking the pi_lock on
	 * every fork/clone.
	 */
	ଯଦି (data_race(!src->user_cpus_ptr))
		ଫେରନ୍ତୁ 0;

	user_mask = alloc_user_cpus_ptr(ଗଣ୍ଠି);
	ଯଦି (!user_mask)
		ଫେରନ୍ତୁ -ENOMEM;

	/*
	 * Use pi_lock to protect content of user_cpus_ptr
	 *
	 * Though unlikely, user_cpus_ptr can be reset to NULL by a concurrent
	 * set_cpus_allowed_force().
	 */
	raw_spin_lock_irqsave(&src->pi_lock, flags);
	ଯଦି (src->user_cpus_ptr) {
		swap(dst->user_cpus_ptr, user_mask);
		cpumask_copy(dst->user_cpus_ptr, src->user_cpus_ptr);
	}
	raw_spin_unlock_irqrestore(&src->pi_lock, flags);

	ଯଦି (unlikely(user_mask))
		kfree(user_mask);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା cpumask *clear_user_cpus_ptr(ସଂରଚନା task_struct *p)
{
	ସଂରଚନା cpumask *user_mask = ଖାଲି;

	swap(p->user_cpus_ptr, user_mask);

	ଫେରନ୍ତୁ user_mask;
}

ଶୂନ୍ୟ release_user_cpus_ptr(ସଂରଚନା task_struct *p)
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା affine_move_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ସଂରଚନା rq_flags *rf,
			    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dest_cpu, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
	__releases(__rq_lockp(rq), &p->pi_lock)
{
	ସଂରଚନା set_affinity_pending my_pending = { }, *pending = ଖାଲି;
	bool stop_pending, complete = false;

	/*
	 * Can the task run on the task's current CPU? If so, we're done
	 *
	 * We are also done if the task is the current donor, boosting a lock-
	 * holding proxy, (and potentially has been migrated outside its
	 * current or previous affinity mask)
	 */
	ଯଦି (cpumask_test_cpu(task_cpu(p), &p->cpus_mask) ||
	    (task_current_donor(rq, p) && !task_current(rq, p))) {
		ସଂରଚନା task_struct *ଠେଲ_task = ଖାଲି;

		ଯଦି ((flags & SCA_MIGRATE_ENABLE) &&
		    (p->migration_flags & MDF_ଠେଲ) && !rq->ଠେଲ_busy) {
			rq->ଠେଲ_busy = true;
			ଠେଲ_task = get_task_struct(p);
		}

		/*
		 * If there are pending waiters, but no pending stop_work,
		 * then complete now.
		 */
		pending = p->migration_pending;
		ଯଦି (pending && !pending->stop_pending) {
			p->migration_pending = ଖାଲି;
			complete = true;
		}

		preempt_disable();
		task_rq_unlock(rq, p, rf);
		ଯଦି (ଠେଲ_task) {
			stop_one_cpu_nowait(rq->cpu, ଠେଲ_cpu_stop,
					    p, &rq->ଠେଲ_work);
		}
		preempt_enable();

		ଯଦି (complete)
			complete_all(&pending->done);

		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!(flags & SCA_MIGRATE_ENABLE)) {
		/* serialized by p->pi_lock */
		ଯଦି (!p->migration_pending) {
			/* Install the request */
			refcount_set(&my_pending.refs, 1);
			init_completion(&my_pending.done);
			my_pending.arg = (ସଂରଚନା migration_arg) {
				.task = p,
				.dest_cpu = dest_cpu,
				.pending = &my_pending,
			};

			p->migration_pending = &my_pending;
		} ଅନ୍ୟଥା {
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
	ଯଦି (WARN_ON_ONCE(!pending)) {
		task_rq_unlock(rq, p, rf);
		ଫେରନ୍ତୁ -EINVAL;
	}

	ଯଦି (task_on_cpu(rq, p) || READ_ONCE(p->__state) == TASK_WAKING) {
		/*
		 * MIGRATE_ENABLE gets here because 'p == current', but for
		 * anything else we cannot do is_migration_disabled(), punt
		 * and have the stopper function handle it all race-free.
		 */
		stop_pending = pending->stop_pending;
		ଯଦି (!stop_pending)
			pending->stop_pending = true;

		ଯଦି (flags & SCA_MIGRATE_ENABLE)
			p->migration_flags &= ~MDF_ଠେଲ;

		preempt_disable();
		task_rq_unlock(rq, p, rf);
		ଯଦି (!stop_pending) {
			stop_one_cpu_nowait(cpu_of(rq), migration_cpu_stop,
					    &pending->arg, &pending->stop_work);
		}
		preempt_enable();

		ଯଦି (flags & SCA_MIGRATE_ENABLE)
			ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା {

		ଯଦି (!is_migration_disabled(p)) {
			ଯଦି (task_on_rq_queued(p))
				rq = move_queued_task(rq, rf, p, dest_cpu);

			ଯଦି (!pending->stop_pending) {
				p->migration_pending = ଖାଲି;
				complete = true;
			}
		}
		task_rq_unlock(rq, p, rf);

		ଯଦି (complete)
			complete_all(&pending->done);
	}

	wait_for_completion(&pending->done);

	ଯଦି (refcount_dec_and_test(&pending->refs))
		wake_up_var(&pending->refs); /* No UaF, just an address */

	/*
	 * Block the original owner of &pending until all subsequent callers
	 * have seen the completion and decremented the refcount
	 */
	wait_var_event(&my_pending.refs, !refcount_read(&my_pending.refs));

	/* ARGH */
	WARN_ON_ONCE(my_pending.stop_pending);

	ଫେରନ୍ତୁ 0;
}

/*
 * Called with both p->pi_lock and rq->lock held; drops both before returning.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __set_cpus_allowed_ptr_locked(ସଂରଚନା task_struct *p,
					 ସଂରଚନା affinity_context *ctx,
					 ସଂରଚନା rq *rq,
					 ସଂରଚନା rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_lock)
{
	ଅଚଳ ସଂରଚନା cpumask *cpu_allowed_mask = task_cpu_possible_mask(p);
	ଅଚଳ ସଂରଚନା cpumask *cpu_valid_mask = cpu_active_mask;
	bool kthread = p->flags & PF_KTHREAD;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା dest_cpu;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	ଯଦି (kthread || is_migration_disabled(p)) {
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
		cpu_valid_mask = cpu_online_mask;
	}

	ଯଦି (!kthread && !cpumask_subset(ctx->new_mask, cpu_allowed_mask)) {
		ret = -EINVAL;
		ଯାଅ out;
	}

	/*
	 * Must re-check here, to close a race against __kthread_bind(),
	 * sched_setaffinity() is not guaranteed to observe the flag.
	 */
	ଯଦି ((ctx->flags & SCA_CHECK) && (p->flags & PF_NO_SETAFFINITY)) {
		ret = -EINVAL;
		ଯାଅ out;
	}

	ଯଦି (!(ctx->flags & SCA_MIGRATE_ENABLE)) {
		ଯଦି (cpumask_equal(&p->cpus_mask, ctx->new_mask)) {
			ଯଦି (ctx->flags & SCA_USER)
				swap(p->user_cpus_ptr, ctx->user_mask);
			ଯାଅ out;
		}

		ଯଦି (WARN_ON_ONCE(p == current &&
				 is_migration_disabled(p) &&
				 !cpumask_test_cpu(task_cpu(p), ctx->new_mask))) {
			ret = -EBUSY;
			ଯାଅ out;
		}
	}

	/*
	 * Picking a ~random cpu helps in cases where we are changing affinity
	 * for groups of tasks (ie. cpuset), so that load balancing is not
	 * immediately required to distribute the tasks within their new mask.
	 */
	dest_cpu = cpumask_any_and_distribute(cpu_valid_mask, ctx->new_mask);
	ଯଦି (dest_cpu >= nr_cpu_ids) {
		ret = -EINVAL;
		ଯାଅ out;
	}

	do_set_cpus_allowed(p, ctx);

	ଫେରନ୍ତୁ affine_move_task(rq, p, rf, dest_cpu, ctx->flags);

out:
	task_rq_unlock(rq, p, rf);

	ଫେରନ୍ତୁ ret;
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
ପୂର୍ଣ୍ଣସଂଖ୍ୟା __set_cpus_allowed_ptr(ସଂରଚନା task_struct *p, ସଂରଚନା affinity_context *ctx)
{
	ସଂରଚନା rq_flags rf;
	ସଂରଚନା rq *rq;

	rq = task_rq_lock(p, &rf);
	/*
	 * Masking should be skipped if SCA_USER or any of the SCA_MIGRATE_*
	 * flags are set.
	 */
	ଯଦି (p->user_cpus_ptr &&
	    !(ctx->flags & (SCA_USER | SCA_MIGRATE_ENABLE | SCA_MIGRATE_DISABLE)) &&
	    cpumask_and(rq->scratch_mask, ctx->new_mask, p->user_cpus_ptr))
		ctx->new_mask = rq->scratch_mask;

	ଫେରନ୍ତୁ __set_cpus_allowed_ptr_locked(p, ctx, rq, &rf);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା set_cpus_allowed_ptr(ସଂରଚନା task_struct *p, ଅଚଳ ସଂରଚନା cpumask *new_mask)
{
	ସଂରଚନା affinity_context ac = {
		.new_mask  = new_mask,
		.flags     = 0,
	};

	ଫେରନ୍ତୁ __set_cpus_allowed_ptr(p, &ac);
}
EXPORT_ପ୍ରତୀକ_GPL(set_cpus_allowed_ptr);

/*
 * Change a given task's CPU affinity to the intersection of its current
 * affinity mask and @subset_mask, writing the resulting mask to @new_mask.
 * If user_cpus_ptr is defined, use it as the basis for restricting CPU
 * affinity or use cpu_online_mask instead.
 *
 * If the resulting mask is empty, leave the affinity unchanged and return
 * -EINVAL.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା restrict_cpus_allowed_ptr(ସଂରଚନା task_struct *p,
				     ସଂରଚନା cpumask *new_mask,
				     ଅଚଳ ସଂରଚନା cpumask *subset_mask)
{
	ସଂରଚନା affinity_context ac = {
		.new_mask  = new_mask,
		.flags     = 0,
	};
	ସଂରଚନା rq_flags rf;
	ସଂରଚନା rq *rq;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;

	rq = task_rq_lock(p, &rf);

	/*
	 * Forcefully restricting the affinity of a deadline task is
	 * likely to cause problems, so fail and noisily override the
	 * mask entirely.
	 */
	ଯଦି (task_has_dl_policy(p) && dl_bandwidth_enabled()) {
		err = -EPERM;
		ଯାଅ err_unlock;
	}

	ଯଦି (!cpumask_and(new_mask, task_user_cpus(p), subset_mask)) {
		err = -EINVAL;
		ଯାଅ err_unlock;
	}

	ଫେରନ୍ତୁ __set_cpus_allowed_ptr_locked(p, &ac, rq, &rf);

err_unlock:
	task_rq_unlock(rq, p, &rf);
	ଫେରନ୍ତୁ err;
}

/*
 * Restrict the CPU affinity of task @p so that it is a subset of
 * task_cpu_possible_mask() and point @p->user_cpus_ptr to a copy of the
 * old affinity mask. If the resulting mask is empty, we warn and walk
 * up the cpuset hierarchy until we find a suitable mask.
 */
ଶୂନ୍ୟ force_compatible_cpus_allowed_ptr(ସଂରଚନା task_struct *p)
{
	cpumask_var_t new_mask;
	ଅଚଳ ସଂରଚନା cpumask *override_mask = task_cpu_possible_mask(p);

	alloc_cpumask_var(&new_mask, GFP_KERNEL);

	/*
	 * __migrate_task() can fail silently in the face of concurrent
	 * offlining of the chosen destination CPU, so take the hotplug
	 * lock to ensure that the migration succeeds.
	 */
	cpus_read_lock();
	ଯଦି (!cpumask_available(new_mask))
		ଯାଅ out_set_mask;

	ଯଦି (!restrict_cpus_allowed_ptr(p, new_mask, override_mask))
		ଯାଅ out_free_mask;

	/*
	 * We failed to find a valid subset of the affinity mask for the
	 * task, so override it based on its cpuset hierarchy.
	 */
	cpuset_cpus_allowed(p, new_mask);
	override_mask = new_mask;

out_set_mask:
	ଯଦି (printk_ratelimit()) {
		printk_deferred("Overriding affinity for process %d (%s) to CPUs %*pbl\n",
				task_pid_nr(p), p->comm,
				cpumask_pr_args(override_mask));
	}

	WARN_ON(set_cpus_allowed_ptr(p, override_mask));
out_free_mask:
	cpus_read_unlock();
	free_cpumask_var(new_mask);
}

/*
 * Restore the affinity of a task @p which was previously restricted by a
 * call to force_compatible_cpus_allowed_ptr().
 *
 * It is the caller's responsibility to serialise this with any calls to
 * force_compatible_cpus_allowed_ptr(@p).
 */
ଶୂନ୍ୟ relax_compatible_cpus_allowed_ptr(ସଂରଚନା task_struct *p)
{
	ସଂରଚନା affinity_context ac = {
		.new_mask  = task_user_cpus(p),
		.flags     = 0,
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	/*
	 * Try to restore the old affinity mask with __sched_setaffinity().
	 * Cpuset masking will be done there too.
	 */
	ret = __sched_setaffinity(p, &ac);
	WARN_ON_ONCE(ret);
}

#ifdef CONFIG_SMP

ଶୂନ୍ୟ set_task_cpu(ସଂରଚନା task_struct *p, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_cpu)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା state = READ_ONCE(p->__state);
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

#ifdef CONFIG_LOCKDEP
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
	WARN_ON_ONCE(debug_locks && !(lockdep_is_held(&p->pi_lock) ||
				      lockdep_is_held(__rq_lockp(task_rq(p)))));
#endif
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

	ଯଦି (task_cpu(p) != new_cpu) {
		ଯଦି (p->sched_class->migrate_task_rq)
			p->sched_class->migrate_task_rq(p, new_cpu);
		p->se.nr_migrations++;
		perf_event_task_migrate(p);
	}

	__set_task_cpu(p, new_cpu);
}
#endif /* CONFIG_SMP */

#ifdef CONFIG_NUMA_BALANCING
ସ୍ଥିର ଶୂନ୍ୟ __migrate_swap_task(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଯଦି (task_on_rq_queued(p)) {
		ସଂରଚନା rq *src_rq, *dst_rq;
		ସଂରଚନା rq_flags srf, drf;

		src_rq = task_rq(p);
		dst_rq = cpu_rq(cpu);

		rq_pin_lock(src_rq, &srf);
		rq_pin_lock(dst_rq, &drf);

		move_queued_task_locked(src_rq, dst_rq, p);
		wakeup_preempt(dst_rq, p, 0);

		rq_unpin_lock(dst_rq, &drf);
		rq_unpin_lock(src_rq, &srf);

	} ଅନ୍ୟଥା {
		/*
		 * Task isn't running anymore; make it appear like we migrated
		 * it before it went to sleep. This means on wakeup we make the
		 * previous CPU our target instead of where it really is.
		 */
		p->wake_cpu = cpu;
	}
}

ସଂରଚନା migration_swap_arg {
	ସଂରଚନା task_struct *src_task, *dst_task;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା src_cpu, dst_cpu;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା migrate_swap_stop(ଶୂନ୍ୟ *data)
{
	ସଂରଚନା migration_swap_arg *arg = data;
	ସଂରଚନା rq *src_rq, *dst_rq;

	ଯଦି (!cpu_active(arg->src_cpu) || !cpu_active(arg->dst_cpu))
		ଫେରନ୍ତୁ -EAGAIN;

	src_rq = cpu_rq(arg->src_cpu);
	dst_rq = cpu_rq(arg->dst_cpu);

	guard(double_raw_spinlock)(&arg->src_task->pi_lock, &arg->dst_task->pi_lock);
	guard(double_rq_lock)(src_rq, dst_rq);

	ଯଦି (task_cpu(arg->dst_task) != arg->dst_cpu)
		ଫେରନ୍ତୁ -EAGAIN;

	ଯଦି (task_cpu(arg->src_task) != arg->src_cpu)
		ଫେରନ୍ତୁ -EAGAIN;

	ଯଦି (!cpumask_test_cpu(arg->dst_cpu, arg->src_task->cpus_ptr))
		ଫେରନ୍ତୁ -EAGAIN;

	ଯଦି (!cpumask_test_cpu(arg->src_cpu, arg->dst_task->cpus_ptr))
		ଫେରନ୍ତୁ -EAGAIN;

	__migrate_swap_task(arg->src_task, arg->dst_cpu);
	__migrate_swap_task(arg->dst_task, arg->src_cpu);

	ଫେରନ୍ତୁ 0;
}

/*
 * Cross migrate two tasks
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା migrate_swap(ସଂରଚନା task_struct *cur, ସଂରଚନା task_struct *p,
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା target_cpu, ପୂର୍ଣ୍ଣସଂଖ୍ୟା curr_cpu)
{
	ସଂରଚନା migration_swap_arg arg;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -EINVAL;

	arg = (ସଂରଚନା migration_swap_arg){
		.src_task = cur,
		.src_cpu = curr_cpu,
		.dst_task = p,
		.dst_cpu = target_cpu,
	};

	ଯଦି (arg.src_cpu == arg.dst_cpu)
		ଯାଅ out;

	/*
	 * These three tests are all lockless; this is OK since all of them
	 * will be re-checked with proper locks held further down the line.
	 */
	ଯଦି (!cpu_active(arg.src_cpu) || !cpu_active(arg.dst_cpu))
		ଯାଅ out;

	ଯଦି (!cpumask_test_cpu(arg.dst_cpu, arg.src_task->cpus_ptr))
		ଯାଅ out;

	ଯଦି (!cpumask_test_cpu(arg.src_cpu, arg.dst_task->cpus_ptr))
		ଯାଅ out;

	trace_sched_swap_numa(cur, arg.src_cpu, p, arg.dst_cpu);
	ret = stop_two_cpus(arg.dst_cpu, arg.src_cpu, migrate_swap_stop, &arg);

out:
	ଫେରନ୍ତୁ ret;
}
#endif /* CONFIG_NUMA_BALANCING */

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
ଶୂନ୍ୟ kick_process(ସଂରଚନା task_struct *p)
{
	guard(preempt)();
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = task_cpu(p);

	ଯଦି ((cpu != smp_processor_id()) && task_curr(p))
		smp_send_reschedule(cpu);
}
EXPORT_ପ୍ରତୀକ_GPL(kick_process);

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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା select_fallback_rq(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ସଂରଚନା task_struct *p)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nid = cpu_to_ଗଣ୍ଠି(cpu);
	ଅଚଳ ସଂରଚନା cpumask *nodemask = ଖାଲି;
	ଗଣନା { cpuset, possible, fail } state = cpuset;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା dest_cpu;

	/*
	 * If the node that the CPU is on has been offlined, cpu_to_node()
	 * will return -1. There is no CPU on the node, and we should
	 * select the CPU on the other node.
	 */
	ଯଦି (nid != -1) {
		nodemask = cpumask_of_ଗଣ୍ଠି(nid);

		/* Look for allowed, online CPU in same node. */
		for_each_cpu(dest_cpu, nodemask) {
			ଯଦି (is_cpu_allowed(p, dest_cpu))
				ଫେରନ୍ତୁ dest_cpu;
		}
	}

	ପାଇଁ (;;) {
		/* Any allowed, online CPU? */
		for_each_cpu(dest_cpu, p->cpus_ptr) {
			ଯଦି (!is_cpu_allowed(p, dest_cpu))
				ଜାରି;

			ଯାଅ out;
		}

		/* No more Mr. Nice Guy. */
		ବାଛ (state) {
		ସ୍ଥିତି cpuset:
			ଯଦି (cpuset_cpus_allowed_fallback(p)) {
				state = possible;
				ଭାଙ୍ଗ;
			}
			fallthrough;
		ସ୍ଥିତି possible:
			set_cpus_allowed_force(p, task_cpu_fallback_mask(p));
			state = fail;
			ଭାଙ୍ଗ;
		ସ୍ଥିତି fail:
			BUG();
			ଭାଙ୍ଗ;
		}
	}

out:
	ଯଦି (state != cpuset) {
		/*
		 * Don't tell them about moving exiting tasks or
		 * kernel threads (both mm NULL), since they never
		 * leave kernel.
		 */
		ଯଦି (p->mm && printk_ratelimit()) {
			printk_deferred("process %d (%s) no longer affine to cpu%d\n",
					task_pid_nr(p), p->comm, cpu);
		}
	}

	ଫେରନ୍ତୁ dest_cpu;
}

/*
 * The caller (fork, wakeup) owns p->pi_lock, ->cpus_ptr is stable.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ପୂର୍ଣ୍ଣସଂଖ୍ୟା select_task_rq(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *wake_flags)
{
	lockdep_assert_held(&p->pi_lock);

	ଯଦି (p->nr_cpus_allowed > 1 && !is_migration_disabled(p)) {
		cpu = p->sched_class->select_task_rq(p, cpu, *wake_flags);
		*wake_flags |= WF_RQ_SELECTED;
	} ଅନ୍ୟଥା {
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
	ଯଦି (unlikely(!is_cpu_allowed(p, cpu)))
		cpu = select_fallback_rq(task_cpu(p), p);

	ଫେରନ୍ତୁ cpu;
}

ଶୂନ୍ୟ sched_set_stop_task(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ସଂରଚନା task_struct *stop)
{
	ସ୍ଥିର ସଂରଚନା lock_class_ଚାବି stop_pi_lock;
	ସଂରଚନା sched_param param = { .sched_priority = MAX_RT_PRIO - 1 };
	ସଂରଚନା task_struct *old_stop = cpu_rq(cpu)->stop;

	ଯଦି (stop) {
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
		lockdep_set_class(&stop->pi_lock, &stop_pi_lock);
	}

	cpu_rq(cpu)->stop = stop;

	ଯଦି (old_stop) {
		/*
		 * Reset it back to a normal scheduling class so that
		 * it can die in pieces.
		 */
		old_stop->sched_class = &rt_sched_class;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ
ttwu_stat(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ପୂର୍ଣ୍ଣସଂଖ୍ୟା wake_flags)
{
	ସଂରଚନା rq *rq;

	ଯଦି (!schedstat_enabled())
		ଫେରନ୍ତୁ;

	rq = this_rq();

	ଯଦି (cpu == rq->cpu) {
		__schedstat_inc(rq->ttwu_local);
		__schedstat_inc(p->stats.nr_wakeups_local);
	} ଅନ୍ୟଥା {
		ସଂରଚନା sched_domain *sd;

		__schedstat_inc(p->stats.nr_wakeups_remote);

		guard(rcu)();
		for_each_domain(rq->cpu, sd) {
			ଯଦି (cpumask_test_cpu(cpu, sched_domain_span(sd))) {
				__schedstat_inc(sd->ttwu_wake_remote);
				ଭାଙ୍ଗ;
			}
		}
	}

	ଯଦି (wake_flags & WF_MIGRATED)
		__schedstat_inc(p->stats.nr_wakeups_migrate);

	__schedstat_inc(rq->ttwu_ଗଣନା);
	__schedstat_inc(p->stats.nr_wakeups);

	ଯଦି (wake_flags & WF_SYNC)
		__schedstat_inc(p->stats.nr_wakeups_sync);
}

/*
 * Mark the task runnable.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ ttwu_do_wakeup(ସଂରଚନା task_struct *p)
{
	p->is_blocked = 0;
	WRITE_ONCE(p->__state, TASK_RUNNING);
	trace_sched_wakeup(p);
}

ଶୂନ୍ୟ update_rq_avg_idle(ସଂରଚନା rq *rq)
{
	u64 idle_stamp = rq->idle_stamp;
	u64 delta, max;

	ଯଦି (!idle_stamp)
		ଫେରନ୍ତୁ;

	delta = rq_clock(rq) - idle_stamp;

	update_avg(&rq->avg_idle, delta);

	max = 2 * rq->max_idle_balance_cost;
	ଯଦି (rq->avg_idle > max)
		rq->avg_idle = max;
	rq->idle_stamp = 0;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
ସ୍ଥିର ଶୂନ୍ୟ zap_balance_callbacks(ସଂରଚନା rq *rq);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ proxy_reset_donor(ସଂରଚନା rq *rq)
{
	WARN_ON_ONCE(rq->donor == rq->curr);

	put_prev_set_next_task(rq, rq->donor, rq->curr);
	rq->next_class = rq->curr->sched_class;
	rq_set_donor(rq, rq->curr);
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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool proxy_needs_return(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
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
	ଯଦି (task_cpu(p) == p->wake_cpu)
		ଫେରନ୍ତୁ false;

	scoped_guard(raw_spinlock, &p->blocked_lock) {
		/* Task is waking up; clear any blocked_on relationship */
		__clear_task_blocked_on(p, ଖାଲି);

		/* If already current, don't need to return migrate */
		ଯଦି (task_current(rq, p))
			ଫେରନ୍ତୁ false;
	}

	dequeued = dequeue_block_task(rq, p, TASK_WAKING);

	/*
	 * Dequeue @p from its scheduling class before resetting rq->donor.
	 * In particular, sched_ext needs to end the donor's running session
	 * and clear SCX_TASK_QUEUED before put_prev_task_scx() is called by
	 * proxy_reset_donor(); otherwise it would reenqueue the blocked donor.
	 *
	 * Keep on_rq set until all donor references have been replaced.
	 */
	ଯଦି (task_current_donor(rq, p))
		proxy_reset_donor(rq);

	ଯଦି (dequeued)
		__block_task(rq, p);
	ଫେରନ୍ତୁ true;
}
#ଅନ୍ୟଥା /* !CONFIG_SCHED_PROXY_EXEC */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool proxy_needs_return(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ false;
}
#endif /* CONFIG_SCHED_PROXY_EXEC */

ସ୍ଥିର ଶୂନ୍ୟ
ttwu_do_activate(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା wake_flags,
		 ସଂରଚନା rq_flags *rf)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା en_flags = ENQUEUE_WAKEUP | ENQUEUE_NOCLOCK;

	lockdep_assert_rq_held(rq);

	ଯଦି (p->sched_contributes_to_load)
		rq->nr_uninterruptible--;

	ଯଦି (wake_flags & WF_RQ_SELECTED)
		en_flags |= ENQUEUE_RQ_SELECTED;
	ଯଦି (wake_flags & WF_MIGRATED)
		en_flags |= ENQUEUE_MIGRATED;
	ଅନ୍ୟଥା ଯଦି (p->in_iowait) {
		delayacct_blkio_end(p);
		atomic_dec(&task_rq(p)->nr_iowait);
	}

	activate_task(rq, p, en_flags);
	wakeup_preempt(rq, p, wake_flags);

	ttwu_do_wakeup(p);

	ଯଦି (p->sched_class->task_woken) {
		/*
		 * Our task @p is fully woken up and running; so it's safe to
		 * drop the rq->lock, hereafter rq is only used for statistics.
		 */
		rq_unpin_lock(rq, rf);
		p->sched_class->task_woken(rq, p);
		rq_repin_lock(rq, rf);
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ttwu_runnable(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା wake_flags)
{
	ACQUIRE(__task_rq_lock, guard)(p);
	ସଂରଚନା rq *rq = guard.rq;

	ଯଦି (!task_on_rq_queued(p))
		ଫେରନ୍ତୁ 0;

	update_rq_clock(rq);
	ଯଦି (p->is_blocked) {
		ଯଦି (p->se.sched_delayed)
			enqueue_task(rq, p, ENQUEUE_NOCLOCK | ENQUEUE_DELAYED);
		ଯଦି (proxy_needs_return(rq, p))
			ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!task_on_cpu(rq, p)) {
		/*
		 * When on_rq && !on_cpu the task is preempted, see if
		 * it should preempt the task that is current now.
		 */
		wakeup_preempt(rq, p, wake_flags | WF_TTWU_RQ);
	}
	ttwu_do_wakeup(p);
	ଫେରନ୍ତୁ 1;
}

ଶୂନ୍ୟ sched_ttwu_pending(ଶୂନ୍ୟ *arg)
{
	ସଂରଚନା llist_ଗଣ୍ଠି *llist = arg;
	ସଂରଚନା rq *rq = this_rq();
	ସଂରଚନା task_struct *p, *t;
	ସଂରଚନା rq_flags rf;

	ଯଦି (!llist)
		ଫେରନ୍ତୁ;

	rq_lock_irqsave(rq, &rf);
	update_rq_clock(rq);

	llist_for_each_entry_safe(p, t, llist, wake_entry.llist) {
		ଯଦି (WARN_ON_ONCE(p->on_cpu))
			smp_cond_load_acquire(&p->on_cpu, !VAL);

		ଯଦି (WARN_ON_ONCE(task_cpu(p) != cpu_of(rq)))
			set_task_cpu(p, cpu_of(rq));

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
bool call_function_single_prep_ipi(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଯଦି (set_nr_if_polling(cpu_rq(cpu)->idle)) {
		trace_sched_wake_idle_without_ipi(cpu);
		ଫେରନ୍ତୁ false;
	}

	ଫେରନ୍ତୁ true;
}

/*
 * Queue a task on the target CPUs wake_list and wake the CPU via IPI if
 * necessary. The wakee CPU on receipt of the IPI will queue the task
 * via sched_ttwu_wakeup() for activation so the wakee incurs the cost
 * of the wakeup instead of the waker.
 */
ସ୍ଥିର ଶୂନ୍ୟ __ttwu_queue_wakelist(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ପୂର୍ଣ୍ଣସଂଖ୍ୟା wake_flags)
{
	ସଂରଚନା rq *rq = cpu_rq(cpu);

	p->sched_remote_wakeup = !!(wake_flags & WF_MIGRATED);

	WRITE_ONCE(rq->ttwu_pending, 1);
#ifdef CONFIG_SMP
	__smp_call_single_queue(cpu, &p->wake_entry.llist);
#endif
}

ଶୂନ୍ୟ wake_up_if_idle(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା rq *rq = cpu_rq(cpu);

	guard(rcu)();
	ଯଦି (is_idle_task(rcu_dereference(rq->curr))) {
		guard(rq_lock_irqsave)(rq);
		ଯଦି (is_idle_task(rq->curr))
			resched_curr(rq);
	}
}

bool cpus_equal_capacity(ପୂର୍ଣ୍ଣସଂଖ୍ୟା this_cpu, ପୂର୍ଣ୍ଣସଂଖ୍ୟା that_cpu)
{
	ଯଦି (!sched_asym_cpucap_active())
		ଫେରନ୍ତୁ true;

	ଯଦି (this_cpu == that_cpu)
		ଫେରନ୍ତୁ true;

	ଫେରନ୍ତୁ arch_scale_cpu_capacity(this_cpu) == arch_scale_cpu_capacity(that_cpu);
}

bool cpus_share_cache(ପୂର୍ଣ୍ଣସଂଖ୍ୟା this_cpu, ପୂର୍ଣ୍ଣସଂଖ୍ୟା that_cpu)
{
	ଯଦି (this_cpu == that_cpu)
		ଫେରନ୍ତୁ true;

	ଫେରନ୍ତୁ per_cpu(sd_llc_id, this_cpu) == per_cpu(sd_llc_id, that_cpu);
}

/*
 * Whether CPUs are share cache resources, which means LLC on non-cluster
 * machines and LLC tag or L2 on machines with clusters.
 */
bool cpus_share_resources(ପୂର୍ଣ୍ଣସଂଖ୍ୟା this_cpu, ପୂର୍ଣ୍ଣସଂଖ୍ୟା that_cpu)
{
	ଯଦି (this_cpu == that_cpu)
		ଫେରନ୍ତୁ true;

	ଫେରନ୍ତୁ per_cpu(sd_share_id, this_cpu) == per_cpu(sd_share_id, that_cpu);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool ttwu_queue_cond(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା this_cpu = smp_processor_id();

	/* See SCX_OPS_ALLOW_QUEUED_WAKEUP. */
	ଯଦି (!scx_allow_ttwu_queue(p))
		ଫେରନ୍ତୁ false;

#ifdef CONFIG_SMP
	ଯଦି (p->sched_class == &stop_sched_class)
		ଫେରନ୍ତୁ false;
#endif

	/*
	 * Do not complicate things with the async wake_list while the CPU is
	 * in hotplug state.
	 */
	ଯଦି (!cpu_active(cpu))
		ଫେରନ୍ତୁ false;

	/* Ensure the task will still be allowed to run on the CPU. */
	ଯଦି (!cpumask_test_cpu(cpu, p->cpus_ptr))
		ଫେରନ୍ତୁ false;

	/*
	 * If the CPU does not share cache, then queue the task on the
	 * remote rqs wakelist to avoid accessing remote data.
	 */
	ଯଦି (!cpus_share_cache(this_cpu, cpu))
		ଫେରନ୍ତୁ true;

	ଯଦି (cpu == this_cpu)
		ଫେରନ୍ତୁ false;

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
	ଯଦି (!cpu_rq(cpu)->nr_running)
		ଫେରନ୍ତୁ true;

	ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool ttwu_queue_wakelist(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ପୂର୍ଣ୍ଣସଂଖ୍ୟା wake_flags)
{
	ଯଦି (sched_feat(TTWU_QUEUE) && ttwu_queue_cond(p, cpu)) {
		sched_clock_cpu(cpu); /* Sync clocks across CPUs */
		__ttwu_queue_wakelist(p, cpu, wake_flags);
		ଫେରନ୍ତୁ true;
	}

	ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଶୂନ୍ୟ ttwu_queue(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ପୂର୍ଣ୍ଣସଂଖ୍ୟା wake_flags)
{
	ସଂରଚନା rq *rq = cpu_rq(cpu);
	ସଂରଚନା rq_flags rf;

	ଯଦି (ttwu_queue_wakelist(p, cpu, wake_flags))
		ଫେରନ୍ତୁ;

	rq_lock(rq, &rf);
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
ସ୍ଥିର __always_inline
bool ttwu_state_match(ସଂରଚନା task_struct *p, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା state, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *success)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା match;

	ଯଦି (IS_ENABLED(CONFIG_DEBUG_PREEMPT)) {
		WARN_ON_ONCE((state & TASK_RTLOCK_WAIT) &&
			     state != TASK_RTLOCK_WAIT);
	}

	*success = !!(match = __task_state_match(p, state));

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
	ଯଦି (match < 0)
		p->saved_state = TASK_RUNNING;

	ଫେରନ୍ତୁ match > 0;
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
ପୂର୍ଣ୍ଣସଂଖ୍ୟା try_to_wake_up(ସଂରଚନା task_struct *p, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା state, ପୂର୍ଣ୍ଣସଂଖ୍ୟା wake_flags)
{
	guard(preempt)();
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, success = 0;

	wake_flags |= WF_TTWU;

	ଯଦି (p == current) {
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
		clear_task_blocked_on(p, ଖାଲି);
		ଯଦି (!ttwu_state_match(p, state, &success))
			ଯାଅ out;

		trace_sched_waking(p);
		ttwu_do_wakeup(p);
		ଯାଅ out;
	}

	/*
	 * If we are going to wake up a thread waiting for CONDITION we
	 * need to ensure that CONDITION=1 done by the caller can not be
	 * reordered with p->state check below. This pairs with smp_store_mb()
	 * in set_current_state() that the waiting thread does.
	 */
	scoped_guard (raw_spinlock_irqsave, &p->pi_lock) {
		smp_mb__after_spinlock();

		ଯଦି (!ttwu_state_match(p, state, &success))
			ଭାଙ୍ଗ;

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
		ଯଦି (READ_ONCE(p->on_rq) && ttwu_runnable(p, wake_flags))
			ଭାଙ୍ଗ;

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
		ଯଦି (smp_load_acquire(&p->on_cpu) &&
		    ttwu_queue_wakelist(p, task_cpu(p), wake_flags))
			ଭାଙ୍ଗ;

		/*
		 * If the owning (remote) CPU is still in the middle of schedule() with
		 * this task as prev, wait until it's done referencing the task.
		 *
		 * Pairs with the smp_store_release() in finish_task().
		 *
		 * This ensures that tasks getting woken will be fully ordered against
		 * their previous state and preserve Program Order.
		 */
		smp_cond_load_acquire(&p->on_cpu, !VAL);

		cpu = select_task_rq(p, p->wake_cpu, &wake_flags);
		ଯଦି (task_cpu(p) != cpu) {
			ଯଦି (p->in_iowait) {
				delayacct_blkio_end(p);
				atomic_dec(&task_rq(p)->nr_iowait);
			}

			wake_flags |= WF_MIGRATED;
			psi_ttwu_dequeue(p);
			set_task_cpu(p, cpu);
		} ଅନ୍ୟଥା ଯଦି (cpu != p->wake_cpu) {
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

		ttwu_queue(p, cpu, wake_flags);
	}
out:
	ଯଦି (success)
		ttwu_stat(p, task_cpu(p), wake_flags);

	ଫେରନ୍ତୁ success;
}

ସ୍ଥିର bool __task_needs_rq_lock(ସଂରଚନା task_struct *p)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା state = READ_ONCE(p->__state);

	/*
	 * Since pi->lock blocks try_to_wake_up(), we don't need rq->lock when
	 * the task is blocked. Make sure to check @state since ttwu() can drop
	 * locks at the end, see ttwu_queue_wakelist().
	 */
	ଯଦି (state == TASK_RUNNING || state == TASK_WAKING)
		ଫେରନ୍ତୁ true;

	/*
	 * Ensure we load p->on_rq after p->__state, otherwise it would be
	 * possible to, falsely, observe p->on_rq == 0.
	 *
	 * See try_to_wake_up() for a longer comment.
	 */
	smp_rmb();
	ଯଦି (p->on_rq)
		ଫେରନ୍ତୁ true;

	/*
	 * Ensure the task has finished __schedule() and will not be referenced
	 * anymore. Again, see try_to_wake_up() for a longer comment.
	 */
	smp_rmb();
	smp_cond_load_acquire(&p->on_cpu, !VAL);

	ଫେରନ୍ତୁ false;
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
ପୂର୍ଣ୍ଣସଂଖ୍ୟା task_call_func(ସଂରଚନା task_struct *p, task_call_f func, ଶୂନ୍ୟ *arg)
{
	ସଂରଚନା rq_flags rf;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	raw_spin_lock_irqsave(&p->pi_lock, rf.flags);

	ଯଦି (__task_needs_rq_lock(p)) {
		ସଂରଚନା rq *rq = __task_rq_lock(p, &rf);

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
	} ଅନ୍ୟଥା {
		ret = func(p, arg);
	}

	raw_spin_unlock_irqrestore(&p->pi_lock, rf.flags);
	ଫେରନ୍ତୁ ret;
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
ସଂରଚନା task_struct *cpu_curr_snapshot(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା rq *rq = cpu_rq(cpu);
	ସଂରଚନା task_struct *t;
	ସଂରଚନା rq_flags rf;

	rq_lock_irqsave(rq, &rf);
	smp_mb__after_spinlock(); /* Pairing determined by caller's synchronization design. */
	t = rcu_dereference(cpu_curr(cpu));
	rq_unlock_irqrestore(rq, &rf);
	smp_mb(); /* Pairing determined by caller's synchronization design. */

	ଫେରନ୍ତୁ t;
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
ପୂର୍ଣ୍ଣସଂଖ୍ୟା wake_up_process(ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ try_to_wake_up(p, TASK_NORMAL, 0);
}
EXPORT_ପ୍ରତୀକ(wake_up_process);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା wake_up_state(ସଂରଚନା task_struct *p, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା state)
{
	ଫେରନ୍ତୁ try_to_wake_up(p, state, 0);
}

/*
 * Perform scheduler related setup for a newly forked process p.
 * p is forked by current.
 *
 * __sched_fork() is basic setup which is also used by sched_init() to
 * initialize the boot CPU's idle task.
 */
ସ୍ଥିର ଶୂନ୍ୟ __sched_fork(u64 clone_flags, ସଂରଚନା task_struct *p)
{
	p->on_rq			= 0;

	p->se.on_rq			= 0;
	p->se.exec_start		= 0;
	p->se.sum_exec_runtime		= 0;
	p->se.prev_sum_exec_runtime	= 0;
	p->se.nr_migrations		= 0;
	p->se.vruntime			= 0;
	p->se.vlag			= 0;
	p->se.rel_deadline		= 0;
	INIT_LIST_HEAD(&p->se.group_ଗଣ୍ଠି);

	/* A delayed task cannot be in clone(). */
	WARN_ON_ONCE(p->se.sched_delayed);
	WARN_ON_ONCE(p->is_blocked);

#ifdef CONFIG_FAIR_GROUP_SCHED
	p->se.cfs_rq			= ଖାଲି;
#ifdef CONFIG_CFS_BANDWIDTH
	init_cfs_throttle_work(p);
#endif
#endif

#ifdef CONFIG_SCHEDSTATS
	/* Even if schedstat is disabled, there should not be garbage */
	memset(&p->stats, 0, ଆକାର(p->stats));
#endif

	init_dl_entity(&p->dl);

	INIT_LIST_HEAD(&p->rt.run_list);
	p->rt.timeout		= 0;
	p->rt.time_slice	= sched_rr_timeslice;
	p->rt.on_rq		= 0;
	p->rt.on_list		= 0;

#ifdef CONFIG_SCHED_CLASS_EXT
	init_scx_entity(&p->scx);
#endif

#ifdef CONFIG_PREEMPT_NOTIFIERS
	INIT_HLIST_HEAD(&p->preempt_notifiers);
#endif

#ifdef CONFIG_COMPACTION
	p->capture_control = ଖାଲି;
#endif
	init_numa_balancing(clone_flags, p);
	p->wake_entry.u_flags = CSD_TYPE_TTWU;
	p->migration_pending = ଖାଲି;
	init_sched_mm(p);
}

DEFINE_STATIC_ଚାବି_FALSE(sched_numa_balancing);

#ifdef CONFIG_NUMA_BALANCING

ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_numa_balancing_mode;

ସ୍ଥିର ଶୂନ୍ୟ __set_numabalancing_state(bool enabled)
{
	ଯଦି (enabled)
		static_branch_enable(&sched_numa_balancing);
	ଅନ୍ୟଥା
		static_branch_disable(&sched_numa_balancing);
}

ଶୂନ୍ୟ set_numabalancing_state(bool enabled)
{
	ଯଦି (enabled)
		sysctl_numa_balancing_mode = NUMA_BALANCING_NORMAL;
	ଅନ୍ୟଥା
		sysctl_numa_balancing_mode = NUMA_BALANCING_DISABLED;
	__set_numabalancing_state(enabled);
}

#ifdef CONFIG_SYSCTL
ସ୍ଥିର ଶୂନ୍ୟ reset_memory_tiering(ଶୂନ୍ୟ)
{
	ସଂରଚନା pglist_data *pgdat;

	for_each_online_pgdat(pgdat) {
		pgdat->nbp_threshold = 0;
		pgdat->nbp_th_nr_cand = ଗଣ୍ଠି_page_state(pgdat, PGPROMOTE_CANDIDATE);
		pgdat->nbp_th_start = jiffies_to_msecs(jiffies);
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_numa_balancing(ଅଚଳ ସଂରଚନା ctl_table *table, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଲିଖ,
			  ଶୂନ୍ୟ *buffer, size_t *lenp, loff_t *ppos)
{
	ସଂରଚନା ctl_table t;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା state = sysctl_numa_balancing_mode;

	ଯଦି (ଲିଖ && !capable(CAP_SYS_ADMIN))
		ଫେରନ୍ତୁ -EPERM;

	t = *table;
	t.data = &state;
	err = proc_dointvec_minmax(&t, ଲିଖ, buffer, lenp, ppos);
	ଯଦି (err < 0)
		ଫେରନ୍ତୁ err;
	ଯଦି (ଲିଖ) {
		ଯଦି (!(sysctl_numa_balancing_mode & NUMA_BALANCING_MEMORY_TIERING) &&
		    (state & NUMA_BALANCING_MEMORY_TIERING))
			reset_memory_tiering();
		sysctl_numa_balancing_mode = state;
		__set_numabalancing_state(state);
	}
	ଫେରନ୍ତୁ err;
}
#endif /* CONFIG_SYSCTL */
#endif /* CONFIG_NUMA_BALANCING */

#ifdef CONFIG_SCHEDSTATS

DEFINE_STATIC_ଚାବି_FALSE(sched_schedstats);

ସ୍ଥିର ଶୂନ୍ୟ set_schedstats(bool enabled)
{
	ଯଦି (enabled)
		static_branch_enable(&sched_schedstats);
	ଅନ୍ୟଥା
		static_branch_disable(&sched_schedstats);
}

ଶୂନ୍ୟ force_schedstat_enabled(ଶୂନ୍ୟ)
{
	ଯଦି (!schedstat_enabled()) {
		pr_info("kernel profiling enabled schedstats, disable via kernel.sched_schedstats.\n");
		static_branch_enable(&sched_schedstats);
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init setup_schedstats(ଅକ୍ଷର *str)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ଯଦି (!str)
		ଯାଅ out;

	ଯଦି (!strcmp(str, "enable")) {
		set_schedstats(true);
		ret = 1;
	} ଅନ୍ୟଥା ଯଦି (!strcmp(str, "disable")) {
		set_schedstats(false);
		ret = 1;
	}
out:
	ଯଦି (!ret)
		pr_warn("Unable to parse schedstats=\n");

	ଫେରନ୍ତୁ ret;
}
__setup("schedstats=", setup_schedstats);

#ifdef CONFIG_SYSCTL
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_schedstats(ଅଚଳ ସଂରଚନା ctl_table *table, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଲିଖ, ଶୂନ୍ୟ *buffer,
		size_t *lenp, loff_t *ppos)
{
	ସଂରଚନା ctl_table t;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା state = static_branch_likely(&sched_schedstats);

	ଯଦି (ଲିଖ && !capable(CAP_SYS_ADMIN))
		ଫେରନ୍ତୁ -EPERM;

	t = *table;
	t.data = &state;
	err = proc_dointvec_minmax(&t, ଲିଖ, buffer, lenp, ppos);
	ଯଦି (err < 0)
		ଫେରନ୍ତୁ err;
	ଯଦି (ଲିଖ)
		set_schedstats(state);
	ଫେରନ୍ତୁ err;
}
#endif /* CONFIG_SYSCTL */
#endif /* CONFIG_SCHEDSTATS */

#ifdef CONFIG_SYSCTL
ସ୍ଥିର ଅଚଳ ସଂରଚନା ctl_table sched_core_sysctls[] = {
#ifdef CONFIG_SCHEDSTATS
	{
		.procname       = "sched_schedstats",
		.data           = ଖାଲି,
		.maxlen         = ଆକାର(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା),
		.mode           = 0644,
		.proc_handler   = sysctl_schedstats,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
#endif /* CONFIG_SCHEDSTATS */
#ifdef CONFIG_UCLAMP_TASK
	{
		.procname       = "sched_util_clamp_min",
		.data           = &sysctl_sched_uclamp_util_min,
		.maxlen         = ଆକାର(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା),
		.mode           = 0644,
		.proc_handler   = sysctl_sched_uclamp_handler,
	},
	{
		.procname       = "sched_util_clamp_max",
		.data           = &sysctl_sched_uclamp_util_max,
		.maxlen         = ଆକାର(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା),
		.mode           = 0644,
		.proc_handler   = sysctl_sched_uclamp_handler,
	},
	{
		.procname       = "sched_util_clamp_min_rt_default",
		.data           = &sysctl_sched_uclamp_util_min_rt_default,
		.maxlen         = ଆକାର(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା),
		.mode           = 0644,
		.proc_handler   = sysctl_sched_uclamp_handler,
	},
#endif /* CONFIG_UCLAMP_TASK */
#ifdef CONFIG_NUMA_BALANCING
	{
		.procname	= "numa_balancing",
		.data		= ଖାଲି, /* filled in by handler */
		.maxlen		= ଆକାର(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା),
		.mode		= 0644,
		.proc_handler	= sysctl_numa_balancing,
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_FOUR,
	},
#endif /* CONFIG_NUMA_BALANCING */
};
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init sched_core_sysctl_init(ଶୂନ୍ୟ)
{
	register_sysctl_init("kernel", sched_core_sysctls);
	ଫେରନ୍ତୁ 0;
}
late_initcall(sched_core_sysctl_init);
#endif /* CONFIG_SYSCTL */

/*
 * fork()/clone()-time setup:
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_fork(u64 clone_flags, ସଂରଚନା task_struct *p)
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
	p->prio = current->normal_prio;

	uclamp_fork(p);

	/*
	 * Revert to default priority/policy on fork if requested.
	 */
	ଯଦି (unlikely(p->sched_reset_on_fork)) {
		ଯଦି (task_has_dl_policy(p) || task_has_rt_policy(p)) {
			p->policy = SCHED_NORMAL;
			p->static_prio = NICE_TO_PRIO(0);
			p->rt_priority = 0;
			p->timer_slack_ns = p->default_timer_slack_ns;
		} ଅନ୍ୟଥା ଯଦି (PRIO_TO_NICE(p->static_prio) < 0)
			p->static_prio = NICE_TO_PRIO(0);

		p->prio = p->normal_prio = p->static_prio;
		set_load_weight(p, false);
		p->se.custom_slice = 0;
		p->se.slice = sysctl_sched_base_slice;

		/*
		 * We don't need the reset flag anymore after the fork. It has
		 * fulfilled its duty:
		 */
		p->sched_reset_on_fork = 0;
	}

	ଯଦି (dl_prio(p->prio))
		ଫେରନ୍ତୁ -EAGAIN;

	scx_pre_fork(p);

	ଯଦି (rt_prio(p->prio)) {
		p->sched_class = &rt_sched_class;
#ifdef CONFIG_SCHED_CLASS_EXT
	} ଅନ୍ୟଥା ଯଦି (task_should_scx(p->policy)) {
		p->sched_class = &ext_sched_class;
#endif
	} ଅନ୍ୟଥା {
		p->sched_class = &fair_sched_class;
	}

	init_entity_runnable_average(&p->se);


#ifdef CONFIG_SCHED_INFO
	ଯଦି (likely(sched_info_on()))
		memset(&p->sched_info, 0, ଆକାର(p->sched_info));
#endif
	p->on_cpu = 0;
	init_task_preempt_ଗଣନା(p);
	plist_ଗଣ୍ଠି_init(&p->pushable_tasks, MAX_PRIO);
	RB_CLEAR_ଗଣ୍ଠି(&p->pushable_dl_tasks);

	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_cgroup_fork(ସଂରଚନା task_struct *p, ସଂରଚନା kernel_clone_args *kargs)
{
	unsigned long flags;

	/*
	 * Because we're not yet on the pid-hash, p->pi_lock isn't strictly
	 * required yet, but lockdep gets upset if rules are violated.
	 */
	raw_spin_lock_irqsave(&p->pi_lock, flags);
#ifdef CONFIG_CGROUP_SCHED
	ଯଦି (1) {
		ସଂରଚନା task_group *tg;
		tg = container_of(kargs->cset->subsys[cpu_cgrp_id],
				  ସଂରଚନା task_group, css);
		tg = autogroup_task_group(p, tg);
		p->sched_task_group = tg;
	}
#endif
	/*
	 * We're setting the CPU for the first time, we don't migrate,
	 * so use __set_task_cpu().
	 */
	__set_task_cpu(p, smp_processor_id());
	ଯଦି (p->sched_class->task_fork)
		p->sched_class->task_fork(p);
	raw_spin_unlock_irqrestore(&p->pi_lock, flags);

	ଫେରନ୍ତୁ scx_fork(p, kargs);
}

ଶୂନ୍ୟ sched_cancel_fork(ସଂରଚନା task_struct *p)
{
	scx_cancel_fork(p);
}

ସ୍ଥିର ଶୂନ୍ୟ sched_mm_cid_fork(ସଂରଚନା task_struct *t);

ଶୂନ୍ୟ sched_post_fork(ସଂରଚନା task_struct *p)
{
	sched_mm_cid_fork(p);
	uclamp_post_fork(p);
	scx_post_fork(p);
}

u64 to_ratio(u64 period, u64 runtime)
{
	ଯଦି (runtime == RUNTIME_INF)
		ଫେରନ୍ତୁ BW_UNIT;

	/*
	 * Doing this here saves a lot of checks in all
	 * the calling paths, and returning zero seems
	 * safe for them anyway.
	 */
	ଯଦି (period == 0)
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ div64_u64(runtime << BW_SHIFT, period);
}

/*
 * wake_up_new_task - wake up a newly created task for the first time.
 *
 * This function will do some initial scheduler statistics housekeeping
 * that must be done for every newly created context, then puts the task
 * on the runqueue and wakes it.
 */
ଶୂନ୍ୟ wake_up_new_task(ସଂରଚନା task_struct *p)
{
	ସଂରଚନା rq_flags rf;
	ସଂରଚନା rq *rq;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା wake_flags = WF_FORK;

	raw_spin_lock_irqsave(&p->pi_lock, rf.flags);
	WRITE_ONCE(p->__state, TASK_RUNNING);
	/*
	 * Fork balancing, do it here and not earlier because:
	 *  - cpus_ptr can change in the fork path
	 *  - any previously selected CPU might disappear through hotplug
	 *
	 * Use __set_task_cpu() to avoid calling sched_class::migrate_task_rq,
	 * as we're not fully set-up yet.
	 */
	p->recent_used_cpu = task_cpu(p);
	__set_task_cpu(p, select_task_rq(p, task_cpu(p), &wake_flags));
	rq = __task_rq_lock(p, &rf);
	update_rq_clock(rq);
	post_init_entity_util_avg(p);

	activate_task(rq, p, ENQUEUE_NOCLOCK | ENQUEUE_INITIAL);
	trace_sched_wakeup_new(p);
	wakeup_preempt(rq, p, wake_flags);
	ଯଦି (p->sched_class->task_woken) {
		/*
		 * Nothing relies on rq->lock after this, so it's fine to
		 * drop it.
		 */
		rq_unpin_lock(rq, &rf);
		p->sched_class->task_woken(rq, p);
		rq_repin_lock(rq, &rf);
	}
	task_rq_unlock(rq, p, &rf);
}

#ifdef CONFIG_PREEMPT_NOTIFIERS

ସ୍ଥିର DEFINE_STATIC_ଚାବି_FALSE(preempt_notifier_ଚାବି);

ଶୂନ୍ୟ preempt_notifier_inc(ଶୂନ୍ୟ)
{
	static_branch_inc(&preempt_notifier_ଚାବି);
}
EXPORT_ପ୍ରତୀକ_GPL(preempt_notifier_inc);

ଶୂନ୍ୟ preempt_notifier_dec(ଶୂନ୍ୟ)
{
	static_branch_dec(&preempt_notifier_ଚାବି);
}
EXPORT_ପ୍ରତୀକ_GPL(preempt_notifier_dec);

/**
 * preempt_notifier_register - tell me when current is being preempted & rescheduled
 * @notifier: notifier struct to register
 */
ଶୂନ୍ୟ preempt_notifier_register(ସଂରଚନା preempt_notifier *notifier)
{
	ଯଦି (!static_branch_unlikely(&preempt_notifier_ଚାବି))
		WARN(1, "registering preempt_notifier while notifiers disabled\n");

	hlist_add_head(&notifier->link, &current->preempt_notifiers);
}
EXPORT_ପ୍ରତୀକ_GPL(preempt_notifier_register);

/**
 * preempt_notifier_unregister - no longer interested in preemption notifications
 * @notifier: notifier struct to unregister
 *
 * This is *not* safe to call from within a preemption notifier.
 */
ଶୂନ୍ୟ preempt_notifier_unregister(ସଂରଚନା preempt_notifier *notifier)
{
	hlist_del(&notifier->link);
}
EXPORT_ପ୍ରତୀକ_GPL(preempt_notifier_unregister);

ସ୍ଥିର ଶୂନ୍ୟ __fire_sched_in_preempt_notifiers(ସଂରଚନା task_struct *curr)
{
	ସଂରଚନା preempt_notifier *notifier;

	hlist_for_each_entry(notifier, &curr->preempt_notifiers, link)
		notifier->ops->sched_in(notifier, raw_smp_processor_id());
}

ସ୍ଥିର __always_inline ଶୂନ୍ୟ fire_sched_in_preempt_notifiers(ସଂରଚନା task_struct *curr)
{
	ଯଦି (static_branch_unlikely(&preempt_notifier_ଚାବି))
		__fire_sched_in_preempt_notifiers(curr);
}

ସ୍ଥିର ଶୂନ୍ୟ
__fire_sched_out_preempt_notifiers(ସଂରଚନା task_struct *curr,
				   ସଂରଚନା task_struct *next)
{
	ସଂରଚନା preempt_notifier *notifier;

	hlist_for_each_entry(notifier, &curr->preempt_notifiers, link)
		notifier->ops->sched_out(notifier, next);
}

ସ୍ଥିର __always_inline ଶୂନ୍ୟ
fire_sched_out_preempt_notifiers(ସଂରଚନା task_struct *curr,
				 ସଂରଚନା task_struct *next)
{
	ଯଦି (static_branch_unlikely(&preempt_notifier_ଚାବି))
		__fire_sched_out_preempt_notifiers(curr, next);
}

#ଅନ୍ୟଥା /* !CONFIG_PREEMPT_NOTIFIERS: */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ fire_sched_in_preempt_notifiers(ସଂରଚନା task_struct *curr)
{
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
fire_sched_out_preempt_notifiers(ସଂରଚନା task_struct *curr,
				 ସଂରଚନା task_struct *next)
{
}

#endif /* !CONFIG_PREEMPT_NOTIFIERS */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ prepare_task(ସଂରଚନା task_struct *next)
{
	/*
	 * Claim the task as running, we do this before switching to it
	 * such that any running task will have this set.
	 *
	 * See the smp_load_acquire(&p->on_cpu) case in ttwu() and
	 * its ordering comment.
	 */
	WRITE_ONCE(next->on_cpu, 1);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ finish_task(ସଂରଚନା task_struct *prev)
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
ସ୍ଥିର ଶୂନ୍ୟ zap_balance_callbacks(ସଂରଚନା rq *rq)
{
	ସଂରଚନା balance_callback *next, *head;
	bool found = false;

	lockdep_assert_rq_held(rq);

	head = rq->balance_callback;
	ଯେତେବେଳେ (head) {
		ଯଦି (head == &balance_ଠେଲ_callback)
			found = true;
		next = head->next;
		head->next = ଖାଲି;
		head = next;
	}
	rq->balance_callback = found ? &balance_ଠେଲ_callback : ଖାଲି;
}

ସ୍ଥିର ଶୂନ୍ୟ do_balance_callbacks(ସଂରଚନା rq *rq, ସଂରଚନା balance_callback *head)
{
	ଶୂନ୍ୟ (*func)(ସଂରଚନା rq *rq);
	ସଂରଚନା balance_callback *next;

	lockdep_assert_rq_held(rq);

	ଯେତେବେଳେ (head) {
		func = head->func;
		next = head->next;
		head->next = ଖାଲି;
		head = next;

		func(rq);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ balance_ଠେଲ(ସଂରଚନା rq *rq);

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
ସଂରଚନା balance_callback balance_ଠେଲ_callback = {
	.next = ଖାଲି,
	.func = balance_ଠେଲ,
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା balance_callback *
__splice_balance_callbacks(ସଂରଚନା rq *rq, bool split)
{
	ସଂରଚନା balance_callback *head = rq->balance_callback;

	ଯଦି (likely(!head))
		ଫେରନ୍ତୁ ଖାଲି;

	lockdep_assert_rq_held(rq);
	/*
	 * Must not take balance_push_callback off the list when
	 * splice_balance_callbacks() and balance_callbacks() are not
	 * in the same rq->lock section.
	 *
	 * In that case it would be possible for __schedule() to interleave
	 * and observe the list empty.
	 */
	ଯଦି (split && head == &balance_ଠେଲ_callback)
		head = ଖାଲି;
	ଅନ୍ୟଥା
		rq->balance_callback = ଖାଲି;

	ଫେରନ୍ତୁ head;
}

ସଂରଚନା balance_callback *splice_balance_callbacks(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ __splice_balance_callbacks(rq, true);
}

ଶୂନ୍ୟ __balance_callbacks(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf)
{
	ଯଦି (rf)
		rq_unpin_lock(rq, rf);
	do_balance_callbacks(rq, __splice_balance_callbacks(rq, false));
	ଯଦି (rf)
		rq_repin_lock(rq, rf);
}

ଶୂନ୍ୟ balance_callbacks(ସଂରଚନା rq *rq, ସଂରଚନା balance_callback *head)
{
	unsigned long flags;

	ଯଦି (unlikely(head)) {
		raw_spin_rq_lock_irqsave(rq, flags);
		do_balance_callbacks(rq, head);
		raw_spin_rq_unlock_irqrestore(rq, flags);
	}
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
prepare_lock_switch(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *next, ସଂରଚନା rq_flags *rf)
	__releases(__rq_lockp(rq))
	__acquires(__rq_lockp(this_rq()))
{
	/*
	 * Since the runqueue lock will be released by the next
	 * task (which is an invalid locking op but in the case
	 * of the scheduler it's an obvious special-case), so we
	 * do an early lockdep release here:
	 */
	rq_unpin_lock(rq, rf);
	spin_release(&__rq_lockp(rq)->dep_map, _THIS_IP_);
#ifdef CONFIG_DEBUG_SPINLOCK
	/* this is a valid case when another task releases the spinlock */
	rq_lockp(rq)->owner = next;
#endif
	/*
	 * Model the rq reference switcheroo.
	 */
	__release(__rq_lockp(rq));
	__acquire(__rq_lockp(this_rq()));
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ finish_lock_switch(ସଂରଚନା rq *rq)
	__releases(__rq_lockp(rq))
{
	/*
	 * If we are tracking spinlock dependencies then we have to
	 * fix up the runqueue lock - which gets 'carried over' from
	 * prev into current:
	 */
	spin_acquire(&__rq_lockp(rq)->dep_map, 0, 0, _THIS_IP_);
	__balance_callbacks(rq, ଖାଲି);
	hrtick_schedule_exit(rq);
	raw_spin_rq_unlock_irq(rq);
}

/*
 * NOP if the arch has not defined these:
 */

#ifndef prepare_arch_switch
# define prepare_arch_switch(next)	କର { } ଯେତେବେଳେ (0)
#endif

#ifndef finish_arch_post_lock_switch
# define finish_arch_post_lock_switch()	କର { } ଯେତେବେଳେ (0)
#endif

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ kmap_local_sched_out(ଶୂନ୍ୟ)
{
#ifdef CONFIG_KMAP_LOCAL
	ଯଦି (unlikely(current->kmap_ctrl.idx))
		__kmap_local_sched_out();
#endif
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ kmap_local_sched_in(ଶୂନ୍ୟ)
{
#ifdef CONFIG_KMAP_LOCAL
	ଯଦି (unlikely(current->kmap_ctrl.idx))
		__kmap_local_sched_in();
#endif
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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
prepare_task_switch(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *prev,
		    ସଂରଚନା task_struct *next)
	__must_hold(__rq_lockp(rq))
{
	kcov_prepare_switch(prev);
	sched_info_switch(rq, prev, next);
	perf_event_task_sched_out(prev, next);
	fire_sched_out_preempt_notifiers(prev, next);
	kmap_local_sched_out();
	prepare_task(next);
	prepare_arch_switch(next);
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
ସ୍ଥିର ସଂରଚନା rq *finish_task_switch(ସଂରଚନା task_struct *prev)
	__releases(__rq_lockp(this_rq()))
{
	ସଂରଚନା rq *rq = this_rq();
	ସଂରଚନା mm_struct *mm = rq->prev_mm;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା prev_state;

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
	ଯଦି (WARN_ONCE(preempt_ଗଣନା() != 2*PREEMPT_DISABLE_OFFSET,
		      "corrupted preempt_count: %s/%d/0x%x\n",
		      current->comm, current->pid, preempt_ଗଣନା()))
		preempt_ଗଣନା_set(FORK_PREEMPT_ଗଣନା);

	rq->prev_mm = ଖାଲି;

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
	perf_event_task_sched_in(prev, current);
	finish_task(prev);
	tick_nohz_task_switch();
	finish_lock_switch(rq);
	finish_arch_post_lock_switch();
	kcov_finish_switch(current);
	/*
	 * kmap_local_sched_out() is invoked with rq::lock held and
	 * interrupts disabled. There is no requirement for that, but the
	 * sched out code does not have an interrupt enabled section.
	 * Restoring the maps on sched in does not require interrupts being
	 * disabled either.
	 */
	kmap_local_sched_in();

	/*
	 * Any cached block-layer timestamp (plug->cur_ktime) is stale now,
	 * invalidate it.
	 */
	blk_plug_invalidate_ts();

	fire_sched_in_preempt_notifiers(current);
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
	ଯଦି (mm) {
		membarrier_mm_sync_core_before_usermode(mm);
		mmdrop_lazy_tlb_sched(mm);
	}

	ଯଦି (unlikely(prev_state == TASK_DEAD)) {
		ଯଦି (prev->sched_class->task_dead)
			prev->sched_class->task_dead(prev);

		/*
		 * sched_ext_dead() must come before cgroup_task_dead() to
		 * prevent cgroups from being removed while its member tasks are
		 * visible to SCX schedulers.
		 */
		sched_ext_dead(prev);
		cgroup_task_dead(prev);

		/* Task is done with its stack. */
		put_task_ଗଦା(prev);

		put_task_struct_rcu_user(prev);
	}

	ଫେରନ୍ତୁ rq;
}

/**
 * schedule_tail - first thing a freshly forked thread must call.
 * @prev: the thread we just switched away from.
 */
asmlinkage __visible ଶୂନ୍ୟ schedule_tail(ସଂରଚନା task_struct *prev)
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

	finish_task_switch(prev);
	/*
	 * This is a special case: the newly created task has just
	 * switched the context for the first time. It is returning from
	 * schedule for the first time in this path.
	 */
	trace_sched_exit_tp(true);
	preempt_enable();

	ଯଦି (current->set_child_tid)
		put_user(task_pid_vnr(current), current->set_child_tid);

	calculate_sigpending();
}

/*
 * context_switch - switch to the new MM and the new thread's register state.
 */
ସ୍ଥିର __always_inline ସଂରଚନା rq *
context_switch(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *prev,
	       ସଂରଚନା task_struct *next, ସଂରଚନା rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	prepare_task_switch(rq, prev, next);

	/*
	 * For paravirt, this is coupled with an exit in switch_to to
	 * combine the page table reload and the switch backend into
	 * one hypercall.
	 */
	arch_start_context_switch(prev);

	/*
	 * kernel -> kernel   lazy + transfer active
	 *   user -> kernel   lazy + mmgrab_lazy_tlb() active
	 *
	 * kernel ->   user   switch + mmdrop_lazy_tlb() active
	 *   user ->   user   switch
	 */
	ଯଦି (!next->mm) {				// to kernel
		enter_lazy_tlb(prev->active_mm, next);

		next->active_mm = prev->active_mm;
		ଯଦି (prev->mm)				// from user
			mmgrab_lazy_tlb(prev->active_mm);
		ଅନ୍ୟଥା
			prev->active_mm = ଖାଲି;
	} ଅନ୍ୟଥା {					// to user
		membarrier_switch_mm(rq, prev->active_mm, next->mm);
		/*
		 * sys_membarrier() requires an smp_mb() between setting
		 * rq->curr / membarrier_switch_mm() and returning to userspace.
		 *
		 * The below provides this either through switch_mm(), or in
		 * case 'prev->active_mm == next->mm' through
		 * finish_task_switch()'s mmdrop().
		 */
		switch_mm_irqs_off(prev->active_mm, next->mm, next);
		lru_gen_use_mm(next->mm);

		ଯଦି (!prev->mm) {			// from kernel
			/* will mmdrop_lazy_tlb() in finish_task_switch(). */
			rq->prev_mm = prev->active_mm;
			prev->active_mm = ଖାଲି;
		}
	}

	mm_cid_switch_to(prev, next);

	/*
	 * Tell rseq that the task was scheduled in. Must be after
	 * switch_mm_cid() to get the TIF flag set.
	 */
	rseq_sched_switch_event(next);

	prepare_lock_switch(rq, next, rf);

	/* Here we just switch the register state and the stack. */
	switch_to(prev, next, prev);
	barrier();

	ଫେରନ୍ତୁ finish_task_switch(prev);
}

/*
 * nr_running and nr_context_switches:
 *
 * externally visible scheduler statistics: current number of runnable
 * threads, total number of context switches performed since bootup.
 */
unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_running(ଶୂନ୍ୟ)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, sum = 0;

	for_each_online_cpu(i)
		sum += cpu_rq(i)->nr_running;

	ଫେରନ୍ତୁ sum;
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
bool single_task_running(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ raw_rq()->nr_running == 1;
}
EXPORT_ପ୍ରତୀକ(single_task_running);

unsigned long long nr_context_switches_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଫେରନ୍ତୁ cpu_rq(cpu)->nr_switches;
}

unsigned long long nr_context_switches(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	unsigned long long sum = 0;

	for_each_possible_cpu(i)
		sum += cpu_rq(i)->nr_switches;

	ଫେରନ୍ତୁ sum;
}

/*
 * Return whether any tasks are accounted as I/O waiters on this CPU.
 * They may wake up on a different CPU.
 */
bool cpu_has_iowait(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଫେରନ୍ତୁ atomic_read(&cpu_rq(cpu)->nr_iowait) != 0;
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

unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_iowait(ଶୂନ୍ୟ)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, sum = 0;

	for_each_possible_cpu(i)
		sum += atomic_read(&cpu_rq(i)->nr_iowait);

	ଫେରନ୍ତୁ sum;
}

/*
 * sched_exec - execve() is a valuable balancing opportunity, because at
 * this point the task has the smallest effective memory and cache footprint.
 */
ଶୂନ୍ୟ sched_exec(ଶୂନ୍ୟ)
{
	ସଂରଚନା task_struct *p = current;
	ସଂରଚନା migration_arg arg;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା dest_cpu;

	scoped_guard (raw_spinlock_irqsave, &p->pi_lock) {
		dest_cpu = p->sched_class->select_task_rq(p, task_cpu(p), WF_EXEC);
		ଯଦି (dest_cpu == smp_processor_id())
			ଫେରନ୍ତୁ;

		ଯଦି (unlikely(!cpu_active(dest_cpu)))
			ଫେରନ୍ତୁ;

		arg = (ସଂରଚନା migration_arg){ p, dest_cpu };
	}
	stop_one_cpu(task_cpu(p), migration_cpu_stop, &arg);
}

DEFINE_PER_CPU(ସଂରଚନା kernel_stat, kstat);
DEFINE_PER_CPU(ସଂରଚନା kernel_cpustat, kernel_cpustat) = {
#ifdef CONFIG_NO_HZ_COMMON
	.idle_sleeptime_seq = SEQCNT_ZERO(kernel_cpustat.idle_sleeptime_seq)
#endif
};

EXPORT_PER_CPU_ପ୍ରତୀକ(kstat);
EXPORT_PER_CPU_ପ୍ରତୀକ(kernel_cpustat);

/*
 * The function fair_sched_class.update_curr accesses the struct curr
 * and its field curr->exec_start; when called from task_sched_runtime(),
 * we observe a high rate of cache misses in practice.
 * Prefetching this data results in improved performance.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ prefetch_curr_exec_start(ସଂରଚନା task_struct *p)
{
	ସଂରଚନା sched_entity *curr = task_rq(p)->cfs.curr;

	prefetch(curr);
	prefetch(&curr->exec_start);
}

/*
 * Return accounted runtime for the task.
 * In case the task is currently running, return the runtime plus current's
 * pending runtime that have not been accounted yet.
 */
unsigned long long task_sched_runtime(ସଂରଚନା task_struct *p)
{
	ସଂରଚନା rq_flags rf;
	ସଂରଚନା rq *rq;
	u64 ns;

#ifdef CONFIG_64BIT
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
	ଯଦି (!p->on_cpu || !task_on_rq_queued(p))
		ଫେରନ୍ତୁ p->se.sum_exec_runtime;
#endif

	rq = task_rq_lock(p, &rf);
	/*
	 * Must be ->curr _and_ ->on_rq.  If dequeued, we would
	 * project cycles that may never be accounted to this
	 * thread, breaking clock_gettime().
	 */
	ଯଦି (task_current_donor(rq, p) && task_on_rq_queued(p)) {
		prefetch_curr_exec_start(p);
		update_rq_clock(rq);
		p->sched_class->update_curr(rq);
	}
	ns = p->se.sum_exec_runtime;
	task_rq_unlock(rq, p, &rf);

	ଫେରନ୍ତୁ ns;
}

ସ୍ଥିର u64 cpu_resched_latency(ସଂରଚନା rq *rq)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା latency_warn_ms = READ_ONCE(sysctl_resched_latency_warn_ms);
	u64 resched_latency, now = rq_clock(rq);
	ସ୍ଥିର bool warned_once;

	ଯଦି (sysctl_resched_latency_warn_once && warned_once)
		ଫେରନ୍ତୁ 0;

	ଯଦି (!need_resched() || !latency_warn_ms)
		ଫେରନ୍ତୁ 0;

	ଯଦି (system_state == SYSTEM_BOOTING)
		ଫେରନ୍ତୁ 0;

	ଯଦି (!rq->last_seen_need_resched_ns) {
		rq->last_seen_need_resched_ns = now;
		rq->ticks_without_resched = 0;
		ଫେରନ୍ତୁ 0;
	}

	rq->ticks_without_resched++;
	resched_latency = now - rq->last_seen_need_resched_ns;
	ଯଦି (resched_latency <= latency_warn_ms * NSEC_PER_MSEC)
		ଫେରନ୍ତୁ 0;

	warned_once = true;

	ଫେରନ୍ତୁ resched_latency;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init setup_resched_latency_warn_ms(ଅକ୍ଷର *str)
{
	long val;

	ଯଦି ((kstrtol(str, 0, &val))) {
		pr_warn("Unable to set resched_latency_warn_ms\n");
		ଫେରନ୍ତୁ 1;
	}

	sysctl_resched_latency_warn_ms = val;
	ଫେରନ୍ତୁ 1;
}
__setup("resched_latency_warn_ms=", setup_resched_latency_warn_ms);

/*
 * This function gets called by the timer code, with HZ frequency.
 * We call it with interrupts disabled.
 */
ଶୂନ୍ୟ sched_tick(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = smp_processor_id();
	ସଂରଚନା rq *rq = cpu_rq(cpu);
	/* scheduler accounting goes to the donor task */
	ସଂରଚନା task_struct *curr, *donor;
	ସଂରଚନା rq_flags rf;
	unsigned long hw_pressure;
	u64 resched_latency;

	ଯଦି (!cpu_preferred(cpu))
		sched_ଠେଲ_current_non_preferred_cpu(rq);

	ଯଦି (housekeeping_cpu(cpu, HK_TYPE_KERNEL_NOISE))
		arch_scale_freq_tick();

	sched_clock_tick();

	rq_lock(rq, &rf);
	curr = rq->curr;
	donor = rq->donor;

	psi_account_irqtime(rq, curr, ଖାଲି);

	update_rq_clock(rq);
	hw_pressure = arch_scale_hw_pressure(cpu_of(rq));
	update_hw_load_avg(rq_clock_task(rq), rq, hw_pressure);

	ଯଦି (dynamic_preempt_lazy() && tif_test_bit(TIF_NEED_RESCHED_LAZY))
		resched_curr(rq);

	donor->sched_class->task_tick(rq, donor, 0);
	ଯଦି (sched_feat(LATENCY_WARN))
		resched_latency = cpu_resched_latency(rq);
	calc_global_load_tick(rq);
	sched_core_tick(rq);
	scx_tick(rq);

	rq_unlock(rq, &rf);

	ଯଦି (sched_feat(LATENCY_WARN) && resched_latency)
		resched_latency_warn(cpu, resched_latency);

	perf_event_task_tick();

	ଯଦି (curr->flags & PF_WQ_WORKER)
		wq_worker_tick(curr);

	ଯଦି (!scx_switched_all()) {
		rq->idle_balance = idle_cpu(cpu);
		sched_balance_trigger(rq);
	}
}

#ifdef CONFIG_NO_HZ_FULL

ସଂରଚନା tick_work {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			cpu;
	atomic_t		state;
	ସଂରଚନା delayed_work	work;
};
/* Values for ->state, see diagram below. */
#define TICK_SCHED_REMOTE_OFFLINE	0
#define TICK_SCHED_REMOTE_OFFLINING	1
#define TICK_SCHED_REMOTE_RUNNING	2

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

ସ୍ଥିର ସଂରଚନା tick_work __percpu *tick_work_cpu;

ସ୍ଥିର ଶୂନ୍ୟ sched_tick_remote(ସଂରଚନା work_struct *work)
{
	ସଂରଚନା delayed_work *dwork = to_delayed_work(work);
	ସଂରଚନା tick_work *twork = container_of(dwork, ସଂରଚନା tick_work, work);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = twork->cpu;
	ସଂରଚନା rq *rq = cpu_rq(cpu);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା os;

	/*
	 * Handle the tick only if it appears the remote CPU is running in full
	 * dynticks mode. The check is racy by nature, but missing a tick or
	 * having one too much is no big deal because the scheduler tick updates
	 * statistics and checks timeslices in a time-independent way, regardless
	 * of when exactly it is running.
	 */
	ଯଦି (tick_nohz_tick_stopped_cpu(cpu)) {
		guard(rq_lock_irq)(rq);
		ସଂରଚନା task_struct *curr = rq->curr;

		ଯଦି (cpu_online(cpu)) {
			/*
			 * Since this is a remote tick for full dynticks mode,
			 * we are always sure that there is no proxy (only a
			 * single task is running).
			 */
			WARN_ON_ONCE(rq->curr != rq->donor);
			update_rq_clock(rq);

			ଯଦି (!is_idle_task(curr)) {
				/*
				 * Make sure the next tick runs within a
				 * reasonable amount of time.
				 */
				u64 delta = rq_clock_task(rq) - curr->se.exec_start;
				WARN_ON_ONCE(delta > (u64)NSEC_PER_SEC * 30);
			}
			curr->sched_class->task_tick(rq, curr, 0);

			calc_load_nohz_remote(rq);
		}
	}

	/*
	 * Run the remote tick once per second (1Hz). This arbitrary
	 * frequency is large enough to avoid overload but short enough
	 * to keep scheduler internal stats reasonably up to date.  But
	 * first update state to reflect hotplug activity if required.
	 */
	os = atomic_fetch_add_unless(&twork->state, -1, TICK_SCHED_REMOTE_RUNNING);
	WARN_ON_ONCE(os == TICK_SCHED_REMOTE_OFFLINE);
	ଯଦି (os == TICK_SCHED_REMOTE_RUNNING)
		queue_delayed_work(system_dfl_wq, dwork, HZ);
}

ସ୍ଥିର ଶୂନ୍ୟ sched_tick_start(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା os;
	ସଂରଚନା tick_work *twork;

	ଯଦି (housekeeping_cpu(cpu, HK_TYPE_KERNEL_NOISE))
		ଫେରନ୍ତୁ;

	WARN_ON_ONCE(!tick_work_cpu);

	twork = per_cpu_ptr(tick_work_cpu, cpu);
	os = atomic_xchg(&twork->state, TICK_SCHED_REMOTE_RUNNING);
	WARN_ON_ONCE(os == TICK_SCHED_REMOTE_RUNNING);
	ଯଦି (os == TICK_SCHED_REMOTE_OFFLINE) {
		twork->cpu = cpu;
		INIT_DELAYED_WORK(&twork->work, sched_tick_remote);
		queue_delayed_work(system_dfl_wq, &twork->work, HZ);
	}
}

#ifdef CONFIG_HOTPLUG_CPU
ସ୍ଥିର ଶୂନ୍ୟ sched_tick_stop(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା tick_work *twork;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା os;

	ଯଦି (housekeeping_cpu(cpu, HK_TYPE_KERNEL_NOISE))
		ଫେରନ୍ତୁ;

	WARN_ON_ONCE(!tick_work_cpu);

	twork = per_cpu_ptr(tick_work_cpu, cpu);
	/* There cannot be competing actions, but don't rely on stop-machine. */
	os = atomic_xchg(&twork->state, TICK_SCHED_REMOTE_OFFLINING);
	WARN_ON_ONCE(os != TICK_SCHED_REMOTE_RUNNING);
	/* Don't cancel, as this would mess up the state machine. */
}
#endif /* CONFIG_HOTPLUG_CPU */

ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init sched_tick_offload_init(ଶୂନ୍ୟ)
{
	tick_work_cpu = alloc_percpu(ସଂରଚନା tick_work);
	BUG_ON(!tick_work_cpu);
	ଫେରନ୍ତୁ 0;
}

#ଅନ୍ୟଥା /* !CONFIG_NO_HZ_FULL: */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_tick_start(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu) { }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_tick_stop(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu) { }
#endif /* !CONFIG_NO_HZ_FULL */

#ଯଦି defined(CONFIG_PREEMPTION) && (defined(CONFIG_DEBUG_PREEMPT) || \
				defined(CONFIG_TRACE_PREEMPT_TOGGLE))
/*
 * If the value passed in is equal to the current preempt count
 * then we just disabled preemption. Start timing the latency.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ preempt_latency_start(ପୂର୍ଣ୍ଣସଂଖ୍ୟା val)
{
	ଯଦି (preempt_ଗଣନା() == val) {
		unsigned long ip = get_lock_parent_ip();
#ifdef CONFIG_DEBUG_PREEMPT
		current->preempt_disable_ip = ip;
#endif
		trace_preempt_off(CALLER_ADDR0, ip);
	}
}

ଶୂନ୍ୟ preempt_ଗଣନା_add(ପୂର୍ଣ୍ଣସଂଖ୍ୟା val)
{
#ifdef CONFIG_DEBUG_PREEMPT
	/*
	 * Underflow?
	 *
	 * Cannot detect underflow based on the current preempt_count() value
	 * if using HAS_SEPARATE_PREEMPT_RESCHED_BITS because preempt count takes all 32
	 * bits.
	 */
	ଯଦି (!IS_ENABLED(CONFIG_HAS_SEPARATE_PREEMPT_RESCHED_BITS) &&
	    DEBUG_LOCKS_WARN_ON((preempt_ଗଣନା() < 0)))
		ଫେରନ୍ତୁ;
#endif
	__preempt_count_add(val);
#ifdef CONFIG_DEBUG_PREEMPT
	/*
	 * Spinlock count overflowing soon?
	 */
	DEBUG_LOCKS_WARN_ON((preempt_ଗଣନା() & PREEMPT_MASK) >=
				PREEMPT_MASK - 10);
#endif
	preempt_latency_start(val);
}
EXPORT_ପ୍ରତୀକ(preempt_ଗଣନା_add);
NOKPROBE_ପ୍ରତୀକ(preempt_ଗଣନା_add);

/*
 * If the value passed in equals to the current preempt count
 * then we just enabled preemption. Stop timing the latency.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ preempt_latency_stop(ପୂର୍ଣ୍ଣସଂଖ୍ୟା val)
{
	ଯଦି (preempt_ଗଣନା() == val)
		trace_preempt_on(CALLER_ADDR0, get_lock_parent_ip());
}

ଶୂନ୍ୟ preempt_ଗଣନା_sub(ପୂର୍ଣ୍ଣସଂଖ୍ୟା val)
{
#ifdef CONFIG_DEBUG_PREEMPT
	/*
	 * Underflow?
	 */
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା uval = val;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା pc = preempt_ଗଣନା();

	ଯଦି (DEBUG_LOCKS_WARN_ON(pc - uval > pc))
		ଫେରନ୍ତୁ;
	/*
	 * Is the spinlock portion underflowing?
	 */
	ଯଦି (DEBUG_LOCKS_WARN_ON((val < PREEMPT_MASK) &&
			!(preempt_ଗଣନା() & PREEMPT_MASK)))
		ଫେରନ୍ତୁ;
#endif

	preempt_latency_stop(val);
	__preempt_count_sub(val);
}
EXPORT_ପ୍ରତୀକ(preempt_ଗଣନା_sub);
NOKPROBE_ପ୍ରତୀକ(preempt_ଗଣନା_sub);

#ଅନ୍ୟଥା
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ preempt_latency_start(ପୂର୍ଣ୍ଣସଂଖ୍ୟା val) { }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ preempt_latency_stop(ପୂର୍ଣ୍ଣସଂଖ୍ୟା val) { }
#endif

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned long get_preempt_disable_ip(ସଂରଚନା task_struct *p)
{
#ifdef CONFIG_DEBUG_PREEMPT
	ଫେରନ୍ତୁ p->preempt_disable_ip;
#ଅନ୍ୟଥା
	ଫେରନ୍ତୁ 0;
#endif
}

/*
 * Print scheduling while atomic bug:
 */
ସ୍ଥିର noinline ଶୂନ୍ୟ __schedule_bug(ସଂରଚନା task_struct *prev)
{
	/* Save this before calling printk(), since that will clobber it */
	unsigned long preempt_disable_ip = get_preempt_disable_ip(current);

	ଯଦି (oops_in_progress)
		ଫେରନ୍ତୁ;

	printk(KERN_ERR "BUG: scheduling while atomic: %s/%d/0x%08x\n",
		prev->comm, prev->pid, preempt_ଗଣନା());

	debug_show_held_locks(prev);
	print_modules();
	ଯଦି (irqs_disabled())
		print_irqtrace_events(prev);
	ଯଦି (IS_ENABLED(CONFIG_DEBUG_PREEMPT)) {
		pr_err("Preemption disabled at:");
		print_ip_sym(KERN_ERR, preempt_disable_ip);
	}
	check_panic_on_warn("scheduling while atomic");

	dump_ଗଦା();
	add_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}

/*
 * Various schedule()-time debugging checks and statistics:
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ schedule_debug(ସଂରଚନା task_struct *prev, bool preempt)
{
#ifdef CONFIG_SCHED_ଗଦା_END_CHECK
	ଯଦି (task_ଗଦା_end_corrupted(prev))
		panic("corrupted stack end detected inside scheduler\n");

	ଯଦି (task_scs_end_corrupted(prev))
		panic("corrupted shadow stack detected inside scheduler\n");
#endif

#ifdef CONFIG_DEBUG_ATOMIC_SLEEP
	ଯଦି (!preempt && READ_ONCE(prev->__state) && prev->non_block_ଗଣନା) {
		printk(KERN_ERR "BUG: scheduling in a non-blocking section: %s/%d/%i\n",
			prev->comm, prev->pid, prev->non_block_ଗଣନା);
		dump_ଗଦା();
		add_taint(TAINT_WARN, LOCKDEP_STILL_OK);
	}
#endif

	ଯଦି (unlikely(in_atomic_preempt_off())) {
		__schedule_bug(prev);
		preempt_ଗଣନା_set(PREEMPT_DISABLED);
	}
	rcu_sleep_check();
	WARN_ON_ONCE(ct_state() == CT_STATE_USER);

	profile_hit(SCHED_PROFILING, __builtin_return_address(0));

	schedstat_inc(this_rq()->sched_ଗଣନା);
}

ସ୍ଥିର ଶୂନ୍ୟ prev_balance(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf)
{
	ଅଚଳ ସଂରଚନା sched_class *start_class = rq->donor->sched_class;
	ଅଚଳ ସଂରଚନା sched_class *class;

	/*
	 * We must do the balancing pass before put_prev_task(), such
	 * that when we release the rq->lock the task is in the same
	 * state as before we took rq->lock.
	 *
	 * We can terminate the balance pass as soon as we know there is
	 * a runnable task of @class priority or higher.
	 */
	for_active_class_range(class, start_class, &idle_sched_class) {
		ଯଦି (class->balance && class->balance(rq, rf))
			ଭାଙ୍ଗ;
	}
}

/*
 * Pick up the highest-prio task:
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା task_struct *
__pick_next_task(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	ଅଚଳ ସଂରଚନା sched_class *class;
	ସଂରଚନା task_struct *p;

	rq->dl_server = ଖାଲି;

	ଯଦି (scx_enabled())
		ଯାଅ restart;

	/*
	 * Optimization: we know that if all tasks are in the fair class we can
	 * call that function directly, but only if the @prev task wasn't of a
	 * higher scheduling class, because otherwise those lose the
	 * opportunity to pull in more work from other CPUs.
	 */
	ଯଦି (likely(!sched_class_above(rq->donor->sched_class, &fair_sched_class) &&
		   rq->nr_running == rq->cfs.h_nr_queued)) {

		p = pick_task_fair(rq, rf);
		ଯଦି (unlikely(p == RETRY_TASK))
			ଯାଅ restart;

		/* Assume the next prioritized class is idle_sched_class */
		ଯଦି (!p)
			p = pick_task_idle(rq, rf);

		put_prev_set_next_task(rq, rq->donor, p);
		ଫେରନ୍ତୁ p;
	}

restart:
	prev_balance(rq, rf);

	for_each_active_class(class) {
		p = class->pick_task(rq, rf);
		ଯଦି (unlikely(p == RETRY_TASK))
			ଯାଅ restart;
		ଯଦି (p) {
			put_prev_set_next_task(rq, rq->donor, p);
			ଫେରନ୍ତୁ p;
		}
	}

	BUG(); /* The idle class should always have a runnable task. */
}

#ifdef CONFIG_SCHED_CORE
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool is_task_rq_idle(ସଂରଚନା task_struct *t)
{
	ଫେରନ୍ତୁ (task_rq(t)->idle == t);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool cookie_equals(ସଂରଚନା task_struct *a, unsigned long cookie)
{
	ଫେରନ୍ତୁ is_task_rq_idle(a) || (a->core_cookie == cookie);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool cookie_match(ସଂରଚନା task_struct *a, ସଂରଚନା task_struct *b)
{
	ଯଦି (is_task_rq_idle(a) || is_task_rq_idle(b))
		ଫେରନ୍ତୁ true;

	ଫେରନ୍ତୁ a->core_cookie == b->core_cookie;
}

/*
 * Careful; this can return RETRY_TASK, it does not include the retry-loop
 * itself due to the whole SMT pick retry thing below.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା task_struct *pick_task(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf)
{
	ଅଚଳ ସଂରଚନା sched_class *class;
	ସଂରଚନା task_struct *p;

	rq->dl_server = ଖାଲି;

	for_each_active_class(class) {
		p = class->pick_task(rq, rf);
		ଯଦି (p)
			ଫେରନ୍ତୁ p;
	}

	BUG(); /* The idle class should always have a runnable task. */
}

ବାହ୍ୟ ଶୂନ୍ୟ task_vruntime_update(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, bool in_fi);

ସ୍ଥିର ଶୂନ୍ୟ queue_core_balance(ସଂରଚନା rq *rq);

ସ୍ଥିର ସଂରଚନା task_struct *
pick_next_task(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	ସଂରଚନା task_struct *next, *p, *max;
	ଅଚଳ ସଂରଚନା cpumask *smt_mask;
	bool fi_before = false;
	bool core_clock_updated = (rq == rq->core);
	unsigned long cookie;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, cpu, occ = 0;
	ସଂରଚନା rq *rq_i;
	bool need_sync = false;

	ଯଦି (!sched_core_enabled(rq))
		ଫେରନ୍ତୁ __pick_next_task(rq, rf);

	cpu = cpu_of(rq);

	/* Stopper task is switching into idle, no need core-wide selection. */
	ଯଦି (cpu_is_offline(cpu)) {
		/*
		 * Reset core_pick so that we don't enter the fastpath when
		 * coming online. core_pick would already be migrated to
		 * another cpu during offline.
		 */
		rq->core_pick = ଖାଲି;
		rq->core_dl_server = ଖାଲି;
		ଫେରନ୍ତୁ __pick_next_task(rq, rf);
	}

	rq->core->core_pick_in_flight++;

	/*
	 * If there were no {en,de}queues since we picked (IOW, the task
	 * pointers are all still valid), and we haven't scheduled the last
	 * pick yet, do so now.
	 *
	 * rq->core_pick can be NULL if no selection was made for a CPU because
	 * it was either offline or went offline during a sibling's core-wide
	 * selection. In this case, do a core-wide selection.
	 */
	ଯଦି (rq->core->core_pick_seq == rq->core->core_task_seq &&
	    rq->core_pick) {
		next = rq->core_pick;
		rq->dl_server = rq->core_dl_server;
		rq->core_pick = ଖାଲି;
		rq->core_dl_server = ଖାଲି;
		ଯାଅ out_set_next;
	}

	prev_balance(rq, rf);

	smt_mask = cpu_smt_mask(cpu);

restart:
	need_sync |= !!rq->core->core_cookie;

	/* reset state */
	rq->core->core_cookie = 0UL;
	ଯଦି (rq->core->core_forceidle_ଗଣନା) {
		ଯଦି (!core_clock_updated) {
			update_rq_clock(rq->core);
			core_clock_updated = true;
		}
		sched_core_account_forceidle(rq);
		/* reset after accounting force idle */
		rq->core->core_forceidle_start = 0;
		rq->core->core_forceidle_ଗଣନା = 0;
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
	ଯଦି (!need_sync) {
		next = pick_task(rq, rf);
		ଯଦି (unlikely(next == RETRY_TASK)) {
			/* rq lock may have been dropped, clocks invalidated */
			core_clock_updated = false;
			ଯଦି (!(rq->clock_update_flags & RQCF_UPDATED))
				update_rq_clock(rq);
			ଯାଅ restart;
		}

		ଯଦି (!next->core_cookie) {
			rq->core_pick = ଖାଲି;
			rq->core_dl_server = ଖାଲି;
			/*
			 * For robustness, update the min_vruntime_fi for
			 * unconstrained picks as well.
			 */
			WARN_ON_ONCE(fi_before);
			task_vruntime_update(rq, next, false);
			ଯାଅ out_set_next;
		}
	}

	/*
	 * For each thread: do the regular task pick and find the max prio task
	 * amongst them.
	 *
	 * Tie-break prio towards the current CPU
	 */
	max = ଖାଲି;
	for_each_cpu_wrap(i, smt_mask, cpu) {
		rq_i = cpu_rq(i);

		/*
		 * Current cpu always has its clock updated on entrance to
		 * pick_next_task(). If the current cpu is not the core,
		 * the core may also have been updated above.
		 */
		ଯଦି (i != cpu && (rq_i != rq->core || !core_clock_updated))
			update_rq_clock(rq_i);

		p = pick_task(rq_i, rf);
		ଯଦି (unlikely(p == RETRY_TASK)) {
			/* rq lock may have been dropped, clocks invalidated */
			core_clock_updated = false;
			ଯଦି (!(rq->clock_update_flags & RQCF_UPDATED))
				update_rq_clock(rq);
			ଯାଅ restart;
		}

		rq_i->core_pick = p;
		rq_i->core_dl_server = rq_i->dl_server;

		ଯଦି (!max || prio_less(max, p, fi_before))
			max = p;
	}

	cookie = rq->core->core_cookie = max->core_cookie;

	/*
	 * For each thread: try and find a runnable task that matches @max or
	 * force idle.
	 */
	for_each_cpu(i, smt_mask) {
		rq_i = cpu_rq(i);
		p = rq_i->core_pick;

		ଯଦି (!cookie_equals(p, cookie)) {
			p = ଖାଲି;
			ଯଦି (cookie)
				p = sched_core_find(rq_i, cookie);
			ଯଦି (!p)
				p = idle_sched_class.pick_task(rq_i, rf);
		}

		rq_i->core_pick = p;
		rq_i->core_dl_server = ଖାଲି;

		ଯଦି (p == rq_i->idle) {
			ଯଦି (rq_i->nr_running) {
				rq->core->core_forceidle_ଗଣନା++;
				ଯଦି (!fi_before)
					rq->core->core_forceidle_seq++;
			}
		} ଅନ୍ୟଥା {
			occ++;
		}
	}

	ଯଦି (schedstat_enabled() && rq->core->core_forceidle_ଗଣନା) {
		rq->core->core_forceidle_start = rq_clock(rq->core);
		rq->core->core_forceidle_occupation = occ;
	}

	rq->core->core_pick_seq = rq->core->core_task_seq;
	next = rq->core_pick;

	/* Something should have been selected for current CPU */
	WARN_ON_ONCE(!next);

	/*
	 * Reschedule siblings
	 *
	 * NOTE: L1TF -- at this point we're no longer running the old task and
	 * sending an IPI (below) ensures the sibling will no longer be running
	 * their task. This ensures there is no inter-sibling overlap between
	 * non-matching user state.
	 */
	for_each_cpu(i, smt_mask) {
		rq_i = cpu_rq(i);

		/*
		 * An online sibling might have gone offline before a task
		 * could be picked for it, or it might be offline but later
		 * happen to come online, but its too late and nothing was
		 * picked for it.  That's Ok - it will pick tasks for itself,
		 * so ignore it.
		 */
		ଯଦି (!rq_i->core_pick)
			ଜାରି;

		/*
		 * Update for new !FI->FI transitions, or if continuing to be in !FI:
		 * fi_before     fi      update?
		 *  0            0       1
		 *  0            1       1
		 *  1            0       1
		 *  1            1       0
		 */
		ଯଦି (!(fi_before && rq->core->core_forceidle_ଗଣନା))
			task_vruntime_update(rq_i, rq_i->core_pick, !!rq->core->core_forceidle_ଗଣନା);

		rq_i->core_pick->core_occupation = occ;

		ଯଦି (i == cpu) {
			rq_i->core_pick = ଖାଲି;
			rq_i->core_dl_server = ଖାଲି;
			ଜାରି;
		}

		/* Did we break L1TF mitigation requirements? */
		WARN_ON_ONCE(!cookie_match(next, rq_i->core_pick));

		ଯଦି (rq_i->curr == rq_i->core_pick) {
			rq_i->core_pick = ଖାଲି;
			rq_i->core_dl_server = ଖାଲି;
			ଜାରି;
		}

		resched_curr(rq_i);
	}

out_set_next:
	rq->core->core_pick_in_flight--;
	put_prev_set_next_task(rq, rq->donor, next);
	ଯଦି (rq->core->core_forceidle_ଗଣନା && next == rq->idle)
		queue_core_balance(rq);

	ଫେରନ୍ତୁ next;
}

ସ୍ଥିର bool try_steal_cookie(ପୂର୍ଣ୍ଣସଂଖ୍ୟା this, ପୂର୍ଣ୍ଣସଂଖ୍ୟା that)
{
	ସଂରଚନା rq *dst = cpu_rq(this), *src = cpu_rq(that);
	ସଂରଚନା task_struct *p;
	unsigned long cookie;
	bool success = false;

	guard(irq)();
	guard(double_rq_lock)(dst, src);

	cookie = dst->core->core_cookie;
	ଯଦି (!cookie)
		ଫେରନ୍ତୁ false;

	ଯଦି (dst->curr != dst->idle)
		ଫେରନ୍ତୁ false;

	p = sched_core_find(src, cookie);
	ଯଦି (!p)
		ଫେରନ୍ତୁ false;

	କର {
		ଯଦି (p == src->core_pick || p == src->curr || p == src->donor)
			ଯାଅ next;

		ଯଦି (task_is_blocked(p))
			ଯାଅ next;

		ଯଦି (!is_cpu_allowed(p, this))
			ଯାଅ next;

		ଯଦି (p->core_occupation > dst->idle->core_occupation)
			ଯାଅ next;
		/*
		 * sched_core_find() and sched_core_next() will ensure
		 * that task @p is not throttled now, we also need to
		 * check whether the runqueue of the destination CPU is
		 * being throttled.
		 */
		ଯଦି (sched_task_is_throttled(p, this))
			ଯାଅ next;

		move_queued_task_locked(src, dst, p);
		resched_curr(dst);

		success = true;
		ଭାଙ୍ଗ;

next:
		p = sched_core_next(p, cookie);
	} ଯେତେବେଳେ (p);

	ଫେରନ୍ତୁ success;
}

ସ୍ଥିର bool steal_cookie_task(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ସଂରଚନା sched_domain *sd)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	for_each_cpu_wrap(i, sched_domain_span(sd), cpu + 1) {
		ଯଦି (i == cpu)
			ଜାରି;

		ଯଦି (need_resched())
			ଭାଙ୍ଗ;

		ଯଦି (try_steal_cookie(cpu, i))
			ଫେରନ୍ତୁ true;
	}

	ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଶୂନ୍ୟ sched_core_balance(ସଂରଚନା rq *rq)
	__must_hold(__rq_lockp(rq))
{
	ସଂରଚନା sched_domain *sd;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = cpu_of(rq);

	guard(preempt)();
	guard(rcu)();

	raw_spin_rq_unlock_irq(rq);
	for_each_domain(cpu, sd) {
		ଯଦି (need_resched())
			ଭାଙ୍ଗ;

		ଯଦି (steal_cookie_task(cpu, sd))
			ଭାଙ୍ଗ;
	}
	raw_spin_rq_lock_irq(rq);
}

ସ୍ଥିର DEFINE_PER_CPU(ସଂରଚନା balance_callback, core_balance_head);

ସ୍ଥିର ଶୂନ୍ୟ queue_core_balance(ସଂରଚନା rq *rq)
{
	ଯଦି (!sched_core_enabled(rq))
		ଫେରନ୍ତୁ;

	ଯଦି (!rq->core->core_cookie)
		ଫେରନ୍ତୁ;

	ଯଦି (!rq->nr_running) /* not forced idle */
		ଫେରନ୍ତୁ;

	queue_balance_callback(rq, &per_cpu(core_balance_head, rq->cpu), sched_core_balance);
}

DEFINE_LOCK_GUARD_1(core_lock, ପୂର୍ଣ୍ଣସଂଖ୍ୟା,
		    sched_core_lock(*_T->lock, &_T->flags),
		    sched_core_unlock(*_T->lock, &_T->flags),
		    unsigned long flags)

ସ୍ଥିର ଶୂନ୍ୟ sched_core_cpu_starting(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଅଚଳ ସଂରଚନା cpumask *smt_mask = cpu_smt_mask(cpu);
	ସଂରଚନା rq *rq = cpu_rq(cpu), *core_rq = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା t;

	guard(core_lock)(&cpu);

	WARN_ON_ONCE(rq->core != rq);

	/* if we're the first, we'll be our own leader */
	ଯଦି (cpumask_weight(smt_mask) == 1)
		ଫେରନ୍ତୁ;

	/* find the leader */
	for_each_cpu(t, smt_mask) {
		ଯଦି (t == cpu)
			ଜାରି;
		rq = cpu_rq(t);
		ଯଦି (rq->core == rq) {
			core_rq = rq;
			ଭାଙ୍ଗ;
		}
	}

	ଯଦି (WARN_ON_ONCE(!core_rq)) /* whoopsie */
		ଫେରନ୍ତୁ;

	/* install and validate core_rq */
	for_each_cpu(t, smt_mask) {
		rq = cpu_rq(t);

		ଯଦି (t == cpu)
			rq->core = core_rq;

		WARN_ON_ONCE(rq->core != core_rq);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ sched_core_cpu_deactivate(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଅଚଳ ସଂରଚନା cpumask *smt_mask = cpu_smt_mask(cpu);
	ସଂରଚନା rq *rq = cpu_rq(cpu), *core_rq = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା t;

	guard(core_lock)(&cpu);

	/* if we're the last man standing, nothing to do */
	ଯଦି (cpumask_weight(smt_mask) == 1) {
		WARN_ON_ONCE(rq->core != rq);
		ଫେରନ୍ତୁ;
	}

	/* if we're not the leader, nothing to do */
	ଯଦି (rq->core != rq)
		ଫେରନ୍ତୁ;

	/* find a new leader */
	for_each_cpu(t, smt_mask) {
		ଯଦି (t == cpu)
			ଜାରି;
		core_rq = cpu_rq(t);
		ଭାଙ୍ଗ;
	}

	ଯଦି (WARN_ON_ONCE(!core_rq)) /* impossible */
		ଫେରନ୍ତୁ;

	/* copy the shared state to the new leader */
	core_rq->core_task_seq             = rq->core_task_seq;
	core_rq->core_pick_seq             = rq->core_pick_seq;
	core_rq->core_cookie               = rq->core_cookie;
	core_rq->core_forceidle_ଗଣନା      = rq->core_forceidle_ଗଣନା;
	core_rq->core_forceidle_seq        = rq->core_forceidle_seq;
	core_rq->core_forceidle_occupation = rq->core_forceidle_occupation;

	/*
	 * A stale leftover would bias the count forever if this CPU later
	 * returns as its own leader. Move, don't copy.
	 */
	core_rq->core_pick_in_flight       = rq->core_pick_in_flight;
	rq->core_pick_in_flight            = 0;

	/*
	 * Accounting edge for forced idle is handled in pick_next_task().
	 * Don't need another one here, since the hotplug thread shouldn't
	 * have a cookie.
	 */
	core_rq->core_forceidle_start = 0;

	/* install new leader */
	for_each_cpu(t, smt_mask) {
		rq = cpu_rq(t);
		rq->core = core_rq;
	}
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_core_cpu_dying(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା rq *rq = cpu_rq(cpu);

	ଯଦି (rq->core != rq)
		rq->core = rq;
}

#ଅନ୍ୟଥା /* !CONFIG_SCHED_CORE: */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_core_cpu_starting(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_core_cpu_deactivate(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_core_cpu_dying(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu) {}

ସ୍ଥିର ସଂରଚନା task_struct *
pick_next_task(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	ଫେରନ୍ତୁ __pick_next_task(rq, rf);
}

#endif /* !CONFIG_SCHED_CORE */

/*
 * Constants for the sched_mode argument of __schedule().
 *
 * The mode argument allows RT enabled kernels to differentiate a
 * preemption from blocking on an 'sleeping' spin/rwlock.
 */
#define SM_IDLE			(-1)
#define SM_NONE			0
#define SM_PREEMPT		1
#define SM_RTLOCK_WAIT		2

/*
 * Helper function for __schedule()
 *
 * Tries to deactivate the task, unless the should_block arg
 * is false or if a signal is pending. In the case a signal
 * is pending, marks the task's __state as RUNNING (and clear
 * blocked_on).
 */
ସ୍ଥିର bool try_to_block_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p,
			      unsigned long *task_state_p, bool should_block)
{
	unsigned long task_state = *task_state_p;

	WARN_ON_ONCE(p->is_blocked);

	ଯଦି (signal_pending_state(task_state, p)) {
		WRITE_ONCE(p->__state, TASK_RUNNING);
		*task_state_p = TASK_RUNNING;
		clear_task_blocked_on(p, ଖାଲି);

		ଫେରନ୍ତୁ false;
	}

	p->is_blocked = 1;

	/*
	 * We check should_block after signal_pending because we
	 * will want to wake the task in that case. But if
	 * should_block is false, its likely due to the task being
	 * blocked on a mutex, and we want to keep it on the runqueue
	 * to be selectable for proxy-execution.
	 */
	ଯଦି (!should_block)
		ଫେରନ୍ତୁ false;

	block_task(rq, p, task_state);
	ଫେରନ୍ତୁ true;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ proxy_set_task_cpu(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା wake_cpu;

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

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା task_struct *proxy_resched_idle(ସଂରଚନା rq *rq)
{
	put_prev_set_next_task(rq, rq->donor, rq->idle);
	rq->next_class = &idle_sched_class;
	rq_set_donor(rq, rq->idle);
	set_tsk_need_resched(rq->idle);
	ଫେରନ୍ତୁ rq->idle;
}

ସ୍ଥିର ଶୂନ୍ୟ proxy_deactivate(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *donor)
{
	unsigned long state = READ_ONCE(donor->__state);

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
	block_task(rq, donor, state);
}

/*
 * Remove a retained proxy donor before changing its scheduler ownership.
 * The caller holds p->pi_lock, so p cannot wake and migrate if block_task()
 * drops it from the runqueue. If DELAY_DEQUEUE keeps a blocked fair task
 * queued, switching_from_fair() completes the dequeue in the immediately
 * following sched_change_begin().
 */
ଶୂନ୍ୟ sched_proxy_block_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	unsigned long state = READ_ONCE(p->__state);

	lockdep_assert_held(&p->pi_lock);
	lockdep_assert_rq_held(rq);

	ଯଦି (!p->is_blocked || !task_on_rq_queued(p))
		ଫେରନ୍ତୁ;
	ଯଦି (WARN_ON_ONCE(state == TASK_RUNNING))
		ଫେରନ୍ତୁ;

	ଯଦି (task_current_donor(rq, p))
		proxy_reset_donor(rq);

	ଯଦି (!p->se.sched_delayed)
		block_task(rq, p, state);

	WARN_ON_ONCE(task_on_rq_queued(p) && !p->se.sched_delayed);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ proxy_release_rq_lock(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf)
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
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ proxy_reacquire_rq_lock(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock(rq);
	rq_repin_lock(rq, rf);
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
ସ୍ଥିର ଶୂନ୍ୟ proxy_migrate_task(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf,
			       ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା target_cpu)
	__must_hold(__rq_lockp(rq))
{
	ସଂରଚନା rq *target_rq = cpu_rq(target_cpu);
	LIST_HEAD(migrate_list);

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
	ପାଇଁ (; p; p = p->blocked_donor) {
		WARN_ON(p == rq->curr);
		deactivate_task(rq, p, DEQUEUE_NOCLOCK);
		proxy_set_task_cpu(p, target_cpu);
		/*
		 * We can re-use se.group_node to migrate the thing,
		 * because @p is deactivated (won't be balanced) and
		 * we hold the rq_lock.
		 */
		list_add(&p->se.group_ଗଣ୍ଠି, &migrate_list);
	}
	proxy_release_rq_lock(rq, rf);

	__attach_tasks(target_rq, &migrate_list);

	proxy_reacquire_rq_lock(rq, rf);
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
ସ୍ଥିର ସଂରଚନା task_struct *
find_proxy_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *donor, ସଂରଚନା rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	ସଂରଚନା task_struct *owner = ଖାଲି;
	bool curr_in_chain = false;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା this_cpu = cpu_of(rq);
	ସଂରଚନା task_struct *p;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା owner_cpu;

	/* Follow blocked_on chain. */
	ପାଇଁ (p = donor; p->is_blocked; p = owner) {
		/* if its PROXY_WAKING, do return migration or run if current */
		ସଂରଚନା mutex *mutex = p->blocked_on;
		ଯଦି (!mutex) {
			clear_task_blocked_on(p, mutex);
			ଯଦି (task_current(rq, p)) {
				p->is_blocked = 0;
				ଫେରନ୍ତୁ p;
			}
			ଯାଅ deactivate;
		}

		/*
		 * By taking mutex->wait_lock we hold off concurrent mutex_unlock()
		 * and ensure @owner sticks around.
		 */
		guard(raw_spinlock)(&mutex->wait_lock);
		guard(raw_spinlock)(&p->blocked_lock);

		/* Check again that p is blocked with blocked_lock held */
		ଯଦି (mutex != __get_task_blocked_on(p)) {
			/*
			 * Something changed in the blocked_on chain and
			 * we don't know if only at this level. So, let's
			 * just bail out completely and let __schedule()
			 * figure things out (pick_again loop).
			 */
			ଫେରନ୍ତୁ ଖାଲି;
		}

		ଯଦି (task_current(rq, p))
			curr_in_chain = true;

		owner = __mutex_owner(mutex);
		ଯଦି (!owner) {
			/*
			 * If there is no owner, either clear blocked_on
			 * and return p (if it is current and safe to
			 * just run on this rq), or return-migrate the task.
			 */
			__clear_task_blocked_on(p, ଖାଲି);
			ଯଦି (task_current(rq, p)) {
				p->is_blocked = 0;
				ଫେରନ୍ତୁ p;
			}
			ଯାଅ deactivate;
		}

		ଯଦି (!READ_ONCE(owner->on_rq) || owner->se.sched_delayed) {
			/* XXX Don't handle blocked owners/delayed dequeue yet */
			ଯଦି (curr_in_chain)
				ଯାଅ resched_idle;
			__clear_task_blocked_on(p, ଖାଲି);
			ଯାଅ deactivate;
		}

		owner_cpu = task_cpu(owner);
		ଯଦି (owner_cpu != this_cpu) {
			/*
			 * @owner can disappear, simply migrate to @owner_cpu
			 * and leave that CPU to sort things out.
			 */
			ଯଦି (curr_in_chain)
				ଯାଅ resched_idle;
			ଯାଅ migrate_task;
		}

		ଯଦି (task_on_rq_migrating(owner)) {
			/*
			 * One of the chain of mutex owners is currently migrating to this
			 * CPU, but has not yet been enqueued because we are holding the
			 * rq lock. As a simple solution, just schedule rq->idle to give
			 * the migration a chance to complete. Much like the migrate_task
			 * case we should end up back in find_proxy_task(), this time
			 * hopefully with all relevant tasks already enqueued.
			 */
			ଯାଅ resched_idle;
		}

		/*
		 * Its possible to race where after we check owner->on_rq
		 * but before we check (owner_cpu != this_cpu) that the
		 * task on another cpu was migrated back to this cpu. In
		 * that case it could slip by our  checks. So double check
		 * we are still on this cpu and not migrating. If we get
		 * inconsistent results, try again.
		 */
		ଯଦି (!task_on_rq_queued(owner) || task_cpu(owner) != this_cpu)
			ଫେରନ୍ତୁ ଖାଲି;

		ଯଦି (owner == p) {
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
			ଯାଅ resched_idle;
		}
		/*
		 * OK, now we're absolutely sure @owner is on this
		 * rq, therefore holding @rq->lock is sufficient to
		 * guarantee its existence, as per ttwu_remote().
		 */
		owner->blocked_donor = p;
	}
	WARN_ON_ONCE(owner && !owner->on_rq);

	ଯଦି (owner && !sched_cpu_cookie_match(rq, owner)) {
		ଯଦି (curr_in_chain)
			ଫେରନ୍ତୁ proxy_resched_idle(rq);
		p = donor; /* Deactivate the donor, not the runnable owner */
		clear_task_blocked_on(p, ଖାଲି);
		ଯାଅ deactivate;
	}
	ଫେରନ୍ତୁ owner;

resched_idle:
	ଫେରନ୍ତୁ proxy_resched_idle(rq);
deactivate:
	proxy_deactivate(rq, p);
	ଫେରନ୍ତୁ ଖାଲି;
migrate_task:
	proxy_migrate_task(rq, rf, p, owner_cpu);
	ଫେରନ୍ତୁ ଖାଲି;
}
#ଅନ୍ୟଥା /* SCHED_PROXY_EXEC */
ସ୍ଥିର ସଂରଚନା task_struct *
find_proxy_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *donor, ସଂରଚନା rq_flags *rf)
{
	WARN_ONCE(1, "This should never be called in the !SCHED_PROXY_EXEC case\n");
	ଫେରନ୍ତୁ donor;
}
#endif /* SCHED_PROXY_EXEC */

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
ସ୍ଥିର ଶୂନ୍ୟ __sched notrace __schedule(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_mode)
{
	ସଂରଚନା task_struct *prev, *next;
	/*
	 * On PREEMPT_RT kernel, SM_RTLOCK_WAIT is noted
	 * as a preemption by schedule_debug() and RCU.
	 */
	bool preempt = sched_mode > SM_NONE;
	bool is_switch = false;
	unsigned long *switch_ଗଣନା;
	unsigned long prev_state;
	ସଂରଚନା rq_flags rf;
	ସଂରଚନା rq *rq;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

	/* Trace preemptions consistently with task switches */
	trace_sched_entry_tp(sched_mode == SM_PREEMPT);

	cpu = smp_processor_id();
	rq = cpu_rq(cpu);
	prev = rq->curr;

	schedule_debug(prev, preempt);

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
	rq_lock(rq, &rf);
	smp_mb__after_spinlock();

	hrtick_schedule_enter(rq);

	/* Promote REQ to ACT */
	rq->clock_update_flags <<= 1;
	update_rq_clock(rq);
	rq->clock_update_flags = RQCF_UPDATED;

	switch_ଗଣନା = &prev->nivcsw;

	/* Task state changes only considers SM_PREEMPT as preemption */
	preempt = sched_mode == SM_PREEMPT;

	/*
	 * We must load prev->state once (task_struct::state is volatile), such
	 * that we form a control dependency vs deactivate_task() below.
	 */
	prev_state = READ_ONCE(prev->__state);
	ଯଦି (sched_mode == SM_IDLE) {
		/* SCX must consult the BPF scheduler to tell if rq is empty */
		ଯଦି (!rq->nr_running && !scx_enabled()) {
			next = prev;
			rq->next_class = &idle_sched_class;
			ଯାଅ picked;
		}
	} ଅନ୍ୟଥା ଯଦି (!preempt && prev_state) {
		/*
		 * Keep mutex-blocked tasks on the runqueue for proxy execution
		 * only when their scheduling class allows it. Without proxy
		 * execution, task_is_blocked() always returns false.
		 */
		try_to_block_task(rq, prev, &prev_state,
				  !task_is_blocked(prev) || !scx_allow_proxy_exec(prev));
		switch_ଗଣନା = &prev->nvcsw;
	}

pick_again:
	assert_balance_callbacks_empty(rq);
	next = pick_next_task(rq, &rf);
	rq->next_class = next->sched_class;
	ଯଦି (sched_proxy_exec()) {
		ସଂରଚନା task_struct *prev_donor = rq->donor;

		rq_set_donor(rq, next);
		next->blocked_donor = ଖାଲି;
		ଯଦି (unlikely(next->is_blocked)) {
			next = find_proxy_task(rq, next, &rf);
			ଯଦି (!next) {
				zap_balance_callbacks(rq);
				ଯାଅ pick_again;
			}
			ଯଦି (next == rq->idle) {
				zap_balance_callbacks(rq);
				scx_proxy_reenqueue_retry(rq, next);
				ଯାଅ keep_resched;
			}
		}
		ଯଦି (rq->donor == prev_donor && prev != next) {
			ସଂରଚନା task_struct *donor = rq->donor;
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
			donor->sched_class->set_next_task(rq, donor, SNT_PICK);
		}
		scx_proxy_donor_start(rq);
		scx_proxy_reenqueue_retry(rq, next);
	} ଅନ୍ୟଥା {
		rq_set_donor(rq, next);
	}

picked:
	clear_tsk_need_resched(prev);
	clear_preempt_need_resched();
keep_resched:
	rq->last_seen_need_resched_ns = 0;

	is_switch = prev != next;
	ଯଦି (likely(is_switch)) {
		rq->nr_switches++;
		/*
		 * RCU users of rcu_dereference(rq->curr) may not see
		 * changes to task_struct made by pick_next_task().
		 */
		RCU_INIT_POINTER(rq->curr, next);

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
		++*switch_ଗଣନା;

		psi_account_irqtime(rq, prev, next);
		psi_sched_switch(prev, next, !task_on_rq_queued(prev) ||
					     prev->se.sched_delayed);

		trace_sched_switch(preempt, prev, next, prev_state);

		/* Also unlocks the rq: */
		rq = context_switch(rq, prev, next, &rf);
	} ଅନ୍ୟଥା {
		rq_unpin_lock(rq, &rf);
		__balance_callbacks(rq, ଖାଲି);
		hrtick_schedule_exit(rq);
		raw_spin_rq_unlock_irq(rq);
	}
	trace_sched_exit_tp(is_switch);
}

ଶୂନ୍ୟ __noreturn do_task_dead(ଶୂନ୍ୟ)
{
	/* Causes final put_task_struct in finish_task_switch(): */
	set_special_state(TASK_DEAD);

	/* Tell freezer to ignore us: */
	current->flags |= PF_NOFREEZE;

	__schedule(SM_NONE);
	BUG();

	/* Avoid "noreturn function does return" - but don't continue if BUG() is a NOP: */
	ପାଇଁ (;;)
		cpu_relax();
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_submit_work(ସଂରଚନା task_struct *tsk)
{
	ସ୍ଥିର DEFINE_WAIT_OVERRIDE_MAP(sched_map, LD_WAIT_CONFIG);
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା task_flags;

	/*
	 * Establish LD_WAIT_CONFIG context to ensure none of the code called
	 * will use a blocking primitive -- which would lead to recursion.
	 */
	lock_map_acquire_try(&sched_map);

	task_flags = tsk->flags;
	/*
	 * If a worker goes to sleep, notify and ask workqueue whether it
	 * wants to wake up a task to maintain concurrency.
	 */
	ଯଦି (task_flags & PF_WQ_WORKER)
		wq_worker_sleeping(tsk);
	ଅନ୍ୟଥା ଯଦି (task_flags & PF_IO_WORKER)
		io_wq_worker_sleeping(tsk);

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

	lock_map_release(&sched_map);
}

ସ୍ଥିର ଶୂନ୍ୟ sched_update_worker(ସଂରଚନା task_struct *tsk)
{
	ଯଦି (tsk->flags & (PF_WQ_WORKER | PF_IO_WORKER)) {
		ଯଦି (tsk->flags & PF_WQ_WORKER)
			wq_worker_running(tsk);
		ଅନ୍ୟଥା
			io_wq_worker_running(tsk);
	}
}

ସ୍ଥିର __always_inline ଶୂନ୍ୟ __schedule_loop(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_mode)
{
	କର {
		preempt_disable();
		__schedule(sched_mode);
		sched_preempt_enable_no_resched();
	} ଯେତେବେଳେ (need_resched());
}

asmlinkage __visible ଶୂନ୍ୟ __sched schedule(ଶୂନ୍ୟ)
{
	ସଂରଚନା task_struct *tsk = current;

#ifdef CONFIG_RT_MUTEXES
	lockdep_assert(!tsk->sched_rt_mutex);
#endif

	ଯଦି (!task_is_running(tsk))
		sched_submit_work(tsk);
	__schedule_loop(SM_NONE);
	sched_update_worker(tsk);
}
EXPORT_ପ୍ରତୀକ(schedule);

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
ଶୂନ୍ୟ __sched schedule_idle(ଶୂନ୍ୟ)
{
	/*
	 * As this skips calling sched_submit_work(), which the idle task does
	 * regardless because that function is a NOP when the task is in a
	 * TASK_RUNNING state, make sure this isn't used someplace that the
	 * current task can be in any other state. Note, idle is always in the
	 * TASK_RUNNING state.
	 */
	WARN_ON_ONCE(current->__state);
	କର {
		__schedule(SM_IDLE);
	} ଯେତେବେଳେ (need_resched());
}

#ଯଦି defined(CONFIG_CONTEXT_TRACKING_USER) && !defined(CONFIG_HAVE_CONTEXT_TRACKING_USER_OFFSTACK)
asmlinkage __visible ଶୂନ୍ୟ __sched schedule_user(ଶୂନ୍ୟ)
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
	ଗଣନା ctx_state prev_state = exception_enter();
	schedule();
	exception_exit(prev_state);
}
#endif

/**
 * schedule_preempt_disabled - called with preemption disabled
 *
 * Returns with preemption disabled. Note: preempt_count must be 1
 */
ଶୂନ୍ୟ __sched schedule_preempt_disabled(ଶୂନ୍ୟ)
{
	sched_preempt_enable_no_resched();
	schedule();
	preempt_disable();
}

#ifdef CONFIG_PREEMPT_RT
ଶୂନ୍ୟ __sched notrace schedule_rtlock(ଶୂନ୍ୟ)
{
	__schedule_loop(SM_RTLOCK_WAIT);
}
NOKPROBE_ପ୍ରତୀକ(schedule_rtlock);
#endif

ସ୍ଥିର ଶୂନ୍ୟ __sched notrace preempt_schedule_common(ଶୂନ୍ୟ)
{
	କର {
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
		preempt_latency_start(1);
		__schedule(SM_PREEMPT);
		preempt_latency_stop(1);
		preempt_enable_no_resched_notrace();

		/*
		 * Check again in case we missed a preemption opportunity
		 * between schedule and now.
		 */
	} ଯେତେବେଳେ (need_resched());
}

#ifdef CONFIG_PREEMPTION
/*
 * This is the entry point to schedule() from in-kernel preemption
 * off of preempt_enable.
 */
asmlinkage __visible ଶୂନ୍ୟ __sched notrace preempt_schedule(ଶୂନ୍ୟ)
{
	/*
	 * If there is a non-zero preempt_count or interrupts are disabled,
	 * we do not want to preempt the current task. Just return..
	 */
	ଯଦି (likely(!preemptible()))
		ଫେରନ୍ତୁ;
	preempt_schedule_common();
}
NOKPROBE_ପ୍ରତୀକ(preempt_schedule);
EXPORT_ପ୍ରତୀକ(preempt_schedule);

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
asmlinkage __visible ଶୂନ୍ୟ __sched notrace preempt_schedule_notrace(ଶୂନ୍ୟ)
{
	ଗଣନା ctx_state prev_ctx;

	ଯଦି (likely(!preemptible()))
		ଫେରନ୍ତୁ;

	କର {
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
		preempt_latency_start(1);
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
	} ଯେତେବେଳେ (need_resched());
}
EXPORT_ପ୍ରତୀକ_GPL(preempt_schedule_notrace);

#endif /* CONFIG_PREEMPTION */

/*
 * This is the entry point to schedule() from kernel preemption
 * off of IRQ context.
 * Note, that this is called and return with IRQs disabled. This will
 * protect us against recursive calling from IRQ contexts.
 */
asmlinkage __visible ଶୂନ୍ୟ __sched preempt_schedule_irq(ଶୂନ୍ୟ)
{
	ଗଣନା ctx_state prev_state;

	/* Catch callers which need to be fixed */
	BUG_ON(preempt_ଗଣନା() || !irqs_disabled());

	prev_state = exception_enter();

	କର {
		preempt_disable();
		local_irq_enable();
		__schedule(SM_PREEMPT);
		local_irq_disable();
		sched_preempt_enable_no_resched();
	} ଯେତେବେଳେ (need_resched());

	exception_exit(prev_state);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_wake_function(wait_queue_entry_t *curr, unsigned mode, ପୂର୍ଣ୍ଣସଂଖ୍ୟା wake_flags,
			  ଶୂନ୍ୟ *ଚାବି)
{
	WARN_ON_ONCE(wake_flags & ~(WF_SYNC|WF_CURRENT_CPU));
	ଫେରନ୍ତୁ try_to_wake_up(curr->ବ୍ୟକ୍ତିଗତ, mode, wake_flags);
}
EXPORT_ପ୍ରତୀକ(default_wake_function);

ଅଚଳ ସଂରଚନା sched_class *__setscheduler_class(ପୂର୍ଣ୍ଣସଂଖ୍ୟା policy, ପୂର୍ଣ୍ଣସଂଖ୍ୟା prio)
{
	ଯଦି (dl_prio(prio))
		ଫେରନ୍ତୁ &dl_sched_class;

	ଯଦି (rt_prio(prio))
		ଫେରନ୍ତୁ &rt_sched_class;

#ifdef CONFIG_SCHED_CLASS_EXT
	ଯଦି (task_should_scx(policy))
		ଫେରନ୍ତୁ &ext_sched_class;
#endif

	ଫେରନ୍ତୁ &fair_sched_class;
}

#ifdef CONFIG_RT_MUTEXES

/*
 * Would be more useful with typeof()/auto_type but they don't mix with
 * bit-fields. Since it's a local thing, use int. Keep the generic sounding
 * name such that if someone were to implement this function we get to compare
 * notes.
 */
#define fetch_and_set(x, v) ({ ପୂର୍ଣ୍ଣସଂଖ୍ୟା _x = (x); (x) = (v); _x; })

ଶୂନ୍ୟ rt_mutex_pre_schedule(ଶୂନ୍ୟ)
{
	lockdep_assert(!fetch_and_set(current->sched_rt_mutex, 1));
	sched_submit_work(current);
}

/*
 * Used within the futex syscall context, skips sched_submit_work() because none
 * its work will be done. Asserts ensure that it is indeed the case.
 */
ଶୂନ୍ୟ rt_mutex_futex_pre_schedule(ଶୂନ୍ୟ)
{
	lockdep_assert(!(current->flags & (PF_WQ_WORKER | PF_IO_WORKER)));
	lockdep_assert(!current->plug);
	lockdep_assert(!fetch_and_set(current->sched_rt_mutex, 1));
}

ଶୂନ୍ୟ rt_mutex_schedule(ଶୂନ୍ୟ)
{
	lockdep_assert(current->sched_rt_mutex);
	__schedule_loop(SM_NONE);
}

ଶୂନ୍ୟ rt_mutex_post_schedule(ଶୂନ୍ୟ)
{
	sched_update_worker(current);
	lockdep_assert(fetch_and_set(current->sched_rt_mutex, 0));
}

ଶୂନ୍ୟ rt_mutex_futex_post_schedule(ଶୂନ୍ୟ)
{
	lockdep_assert(fetch_and_set(current->sched_rt_mutex, 0));
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
ଶୂନ୍ୟ rt_mutex_setprio(ସଂରଚନା task_struct *p, ସଂରଚନା task_struct *pi_task)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା prio, oldprio, queue_flag =
		DEQUEUE_SAVE | DEQUEUE_MOVE | DEQUEUE_NOCLOCK;
	ଅଚଳ ସଂରଚନା sched_class *prev_class, *next_class;
	ସଂରଚନା rq_flags rf;
	ସଂରଚନା rq *rq;

	/* XXX used to be waiter->prio, not waiter->task->prio */
	prio = __rt_effective_prio(pi_task, p->normal_prio);

	/*
	 * If nothing changed; bail early.
	 */
	ଯଦି (p->pi_ଶୀର୍ଷ_task == pi_task && prio == p->prio && !dl_prio(prio))
		ଫେରନ୍ତୁ;

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
	p->pi_ଶୀର୍ଷ_task = pi_task;

	/*
	 * For FIFO/RR we only need to set prio, if that matches we're done.
	 */
	ଯଦି (prio == p->prio && !dl_prio(prio))
		ଯାଅ out_unlock;

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
	ଯଦି (unlikely(p == rq->idle)) {
		WARN_ON(p != rq->curr);
		WARN_ON(p->pi_blocked_on);
		ଯାଅ out_unlock;
	}

	trace_sched_pi_setprio(p, pi_task);
	oldprio = p->prio;

	ଯଦି (oldprio == prio && !dl_prio(prio))
		queue_flag &= ~DEQUEUE_MOVE;

	prev_class = p->sched_class;
	next_class = __setscheduler_class(p->policy, prio);

	ଯଦି (prev_class != next_class)
		queue_flag |= DEQUEUE_CLASS;

	scoped_guard (sched_change, p, queue_flag) {
		/*
		 * Boosting condition are:
		 * 1. -rt task is running and holds mutex A
		 *      --> -dl task blocks on mutex A
		 *
		 * 2. -dl task is running and holds mutex A
		 *      --> -dl task blocks on mutex A and could preempt the
		 *          running task
		 */
		ଯଦି (dl_prio(prio)) {
			ଯଦି (!dl_prio(p->normal_prio) ||
			    (pi_task && dl_prio(pi_task->prio) &&
			     dl_entity_preempt(&pi_task->dl, &p->dl))) {
				p->dl.pi_se = pi_task->dl.pi_se;
				scope->flags |= ENQUEUE_REPLENISH;
			} ଅନ୍ୟଥା {
				p->dl.pi_se = &p->dl;
			}
		} ଅନ୍ୟଥା ଯଦି (rt_prio(prio)) {
			ଯଦି (dl_prio(oldprio))
				p->dl.pi_se = &p->dl;
			ଯଦି (oldprio < prio)
				scope->flags |= ENQUEUE_HEAD;
		} ଅନ୍ୟଥା {
			ଯଦି (dl_prio(oldprio))
				p->dl.pi_se = &p->dl;
			ଯଦି (rt_prio(oldprio))
				p->rt.timeout = 0;
		}

		p->sched_class = next_class;
		p->prio = prio;
	}
out_unlock:
	/* Caller holds task_struct::pi_lock, IRQs are still disabled */

	__balance_callbacks(rq, &rf);
	__task_rq_unlock(rq, p, &rf);
}
#endif /* CONFIG_RT_MUTEXES */

#ଯଦି !defined(CONFIG_PREEMPTION)
ପୂର୍ଣ୍ଣସଂଖ୍ୟା __sched __cond_resched(ଶୂନ୍ୟ)
{
	ଯଦି (should_resched(0) && !irqs_disabled()) {
		preempt_schedule_common();
		ଫେରନ୍ତୁ 1;
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
#ifndef CONFIG_PREEMPT_RCU
	rcu_all_qs();
#endif
	ଫେରନ୍ତୁ 0;
}
EXPORT_ପ୍ରତୀକ(__cond_resched);
#endif

/*
 * __cond_resched_lock() - if a reschedule is pending, drop the given lock,
 * call schedule, and on return reacquire the lock.
 *
 * This works OK both with and without CONFIG_PREEMPTION. We do strange low-level
 * operations here to prevent schedule() from being called twice (once via
 * spin_unlock(), once by hand).
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା __cond_resched_lock(spinlock_t *lock)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା resched = should_resched(PREEMPT_LOCK_OFFSET);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	lockdep_assert_held(lock);

	ଯଦି (spin_needbreak(lock) || resched) {
		spin_unlock(lock);
		ଯଦି (!_cond_resched())
			cpu_relax();
		ret = 1;
		spin_lock(lock);
	}
	ଫେରନ୍ତୁ ret;
}
EXPORT_ପ୍ରତୀକ(__cond_resched_lock);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା __cond_resched_rwlock_read(rwlock_t *lock)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା resched = should_resched(PREEMPT_LOCK_OFFSET);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	lockdep_assert_held_read(lock);

	ଯଦି (rwlock_needbreak(lock) || resched) {
		read_unlock(lock);
		ଯଦି (!_cond_resched())
			cpu_relax();
		ret = 1;
		read_lock(lock);
	}
	ଫେରନ୍ତୁ ret;
}
EXPORT_ପ୍ରତୀକ(__cond_resched_rwlock_read);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା __cond_resched_rwlock_write(rwlock_t *lock)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା resched = should_resched(PREEMPT_LOCK_OFFSET);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	lockdep_assert_held_write(lock);

	ଯଦି (rwlock_needbreak(lock) || resched) {
		write_unlock(lock);
		ଯଦି (!_cond_resched())
			cpu_relax();
		ret = 1;
		write_lock(lock);
	}
	ଫେରନ୍ତୁ ret;
}
EXPORT_ପ୍ରତୀକ(__cond_resched_rwlock_write);

#ifdef CONFIG_PREEMPT_DYNAMIC

# ifdef CONFIG_GENERIC_IRQ_ENTRY
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

ଗଣନା {
	preempt_dynamic_undefined = -1,
	preempt_dynamic_full,
	preempt_dynamic_lazy,
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା preempt_dynamic_mode = preempt_dynamic_undefined;

ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_dynamic_mode(ଅଚଳ ଅକ୍ଷର *str)
{
	ଯଦି (!strcmp(str, "full"))
		ଫେରନ୍ତୁ preempt_dynamic_full;

	ଯଦି (!strcmp(str, "lazy"))
		ଫେରନ୍ତୁ preempt_dynamic_lazy;

	ଫେରନ୍ତୁ -EINVAL;
}

# define preempt_dynamic_ଚାବି_enable(f)	static_ଚାବି_enable(&sk_dynamic_##f.ଚାବି)
# define preempt_dynamic_ଚାବି_disable(f)	static_ଚାବି_disable(&sk_dynamic_##f.ଚାବି)

ସ୍ଥିର DEFINE_MUTEX(sched_dynamic_mutex);

ସ୍ଥିର ଶୂନ୍ୟ __sched_dynamic_update(ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode)
{
	ବାଛ (mode) {
	ସ୍ଥିତି preempt_dynamic_full:
		preempt_dynamic_ଚାବି_disable(preempt_lazy);
		ଯଦି (mode != preempt_dynamic_mode)
			pr_info("Dynamic Preempt: full\n");
		ଭାଙ୍ଗ;

	ସ୍ଥିତି preempt_dynamic_lazy:
		preempt_dynamic_ଚାବି_enable(preempt_lazy);
		ଯଦି (mode != preempt_dynamic_mode)
			pr_info("Dynamic Preempt: lazy\n");
		ଭାଙ୍ଗ;
	}

	WRITE_ONCE(preempt_dynamic_mode, mode);
}

ଶୂନ୍ୟ sched_dynamic_update(ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode)
{
	mutex_lock(&sched_dynamic_mutex);
	__sched_dynamic_update(mode);
	mutex_unlock(&sched_dynamic_mutex);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init setup_preempt_mode(ଅକ୍ଷର *str)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode = sched_dynamic_mode(str);
	ଯଦି (mode < 0) {
		pr_warn("Dynamic Preempt: unsupported mode: %s\n", str);
		ଫେରନ୍ତୁ 0;
	}

	sched_dynamic_update(mode);
	ଫେରନ୍ତୁ 1;
}
__setup("preempt=", setup_preempt_mode);

ସ୍ଥିର ଶୂନ୍ୟ __init preempt_dynamic_init(ଶୂନ୍ୟ)
{
	ଯଦି (preempt_dynamic_mode == preempt_dynamic_undefined) {
		ଯଦି (IS_ENABLED(CONFIG_PREEMPT_LAZY)) {
			sched_dynamic_update(preempt_dynamic_lazy);
		} ଅନ୍ୟଥା {
			/* Default static call setting, nothing to do */
			WARN_ON_ONCE(!IS_ENABLED(CONFIG_PREEMPT));
			preempt_dynamic_mode = preempt_dynamic_full;
			pr_info("Dynamic Preempt: full\n");
		}
	}
}

# define PREEMPT_MODEL_ACCESSOR(mode)					\
	bool preempt_model_##mode(ଶୂନ୍ୟ)					\
	{								\
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode = READ_ONCE(preempt_dynamic_mode);		\
		WARN_ON_ONCE(mode == preempt_dynamic_undefined);	\
		ଫେରନ୍ତୁ mode == preempt_dynamic_##mode;			\
	}								\
	EXPORT_ପ୍ରତୀକ_GPL(preempt_model_##mode)

PREEMPT_MODEL_ACCESSOR(full);
PREEMPT_MODEL_ACCESSOR(lazy);

#ଅନ୍ୟଥା /* !CONFIG_PREEMPT_DYNAMIC: */

#define preempt_dynamic_mode -1

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ preempt_dynamic_init(ଶୂନ୍ୟ) { }

#endif /* CONFIG_PREEMPT_DYNAMIC */

ଅଚଳ ଅକ୍ଷର *preempt_modes[] = {
	"full", "lazy", ଖାଲି,
};

ଅଚଳ ଅକ୍ଷର *preempt_model_str(ଶୂନ୍ୟ)
{
	bool brace = IS_ENABLED(CONFIG_PREEMPT_RT) &&
		(IS_ENABLED(CONFIG_PREEMPT_DYNAMIC) ||
		 IS_ENABLED(CONFIG_PREEMPT_LAZY));
	ସ୍ଥିର ଅକ୍ଷର buf[128];

	ଯଦି (IS_ENABLED(CONFIG_PREEMPT_BUILD)) {
		ସଂରଚନା seq_buf s;

		seq_buf_init(&s, buf, ଆକାର(buf));
		seq_buf_puts(&s, "PREEMPT");

		ଯଦି (IS_ENABLED(CONFIG_PREEMPT_RT))
			seq_buf_printf(&s, "%sRT%s",
				       brace ? "_{" : "_",
				       brace ? "," : "");

		ଯଦି (IS_ENABLED(CONFIG_PREEMPT_DYNAMIC)) {
			seq_buf_printf(&s, "(%s)%s",
				       preempt_dynamic_mode >= 0 ?
				       preempt_modes[preempt_dynamic_mode] : "undef",
				       brace ? "}" : "");
			ଫେରନ୍ତୁ seq_buf_str(&s);
		}

		ଯଦି (IS_ENABLED(CONFIG_PREEMPT_LAZY)) {
			seq_buf_printf(&s, "LAZY%s",
				       brace ? "}" : "");
			ଫେରନ୍ତୁ seq_buf_str(&s);
		}

		ଫେରନ୍ତୁ seq_buf_str(&s);
	}

	ଯଦି (IS_ENABLED(CONFIG_PREEMPT_VOLUNTARY_BUILD))
		ଫେରନ୍ତୁ "VOLUNTARY";

	ଫେରନ୍ତୁ "NONE";
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା io_schedule_prepare(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_iowait = current->in_iowait;

	current->in_iowait = 1;
	blk_flush_plug(current->plug, true);
	ଫେରନ୍ତୁ old_iowait;
}

ଶୂନ୍ୟ io_schedule_finish(ପୂର୍ଣ୍ଣସଂଖ୍ୟା token)
{
	current->in_iowait = token;
}

/*
 * This task is about to go to sleep on IO. Increment rq->nr_iowait so
 * that process accounting knows that this is a task in IO wait state.
 */
long __sched io_schedule_timeout(long timeout)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା token;
	long ret;

	token = io_schedule_prepare();
	ret = schedule_timeout(timeout);
	io_schedule_finish(token);

	ଫେରନ୍ତୁ ret;
}
EXPORT_ପ୍ରତୀକ(io_schedule_timeout);

ଶୂନ୍ୟ __sched io_schedule(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା token;

	token = io_schedule_prepare();
	schedule();
	io_schedule_finish(token);
}
EXPORT_ପ୍ରତୀକ(io_schedule);

ଶୂନ୍ୟ sched_show_task(ସଂରଚନା task_struct *p)
{
	unsigned long ମୁକ୍ତ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ppid;

	ଯଦି (!try_get_task_ଗଦା(p))
		ଫେରନ୍ତୁ;

	pr_info("task:%-15.15s state:%c", p->comm, task_state_to_char(p));

	ଯଦି (task_is_running(p))
		pr_cont("  running task    ");
	ମୁକ୍ତ = ଗଦା_not_used(p);
	ppid = 0;
	rcu_read_lock();
	ଯଦି (pid_alive(p))
		ppid = task_pid_nr(rcu_dereference(p->real_parent));
	rcu_read_unlock();
	pr_cont(" stack:%-5lu pid:%-5d tgid:%-5d ppid:%-6d task_flags:0x%04x flags:0x%08lx\n",
		ମୁକ୍ତ, task_pid_nr(p), task_tgid_nr(p),
		ppid, p->flags, read_task_thread_flags(p));

	print_worker_info(KERN_INFO, p);
	print_stop_info(KERN_INFO, p);
	print_scx_info(KERN_INFO, p);
	show_ଗଦା(p, ଖାଲି, KERN_INFO);
	put_task_ଗଦା(p);
}
EXPORT_ପ୍ରତୀକ_GPL(sched_show_task);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
state_filter_match(unsigned long state_filter, ସଂରଚନା task_struct *p)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା state = READ_ONCE(p->__state);

	/* no filter, everything matches */
	ଯଦି (!state_filter)
		ଫେରନ୍ତୁ true;

	/* filter, but doesn't match */
	ଯଦି (!(state & state_filter))
		ଫେରନ୍ତୁ false;

	/*
	 * When looking for TASK_UNINTERRUPTIBLE skip TASK_IDLE (allows
	 * TASK_KILLABLE).
	 */
	ଯଦି (state_filter == TASK_UNINTERRUPTIBLE && (state & TASK_NOLOAD))
		ଫେରନ୍ତୁ false;

	ଫେରନ୍ତୁ true;
}


ଶୂନ୍ୟ show_state_filter(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା state_filter)
{
	ସଂରଚନା task_struct *g, *p;

	rcu_read_lock();
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
		ଯଦି (state_filter_match(state_filter, p))
			sched_show_task(p);
	}

	ଯଦି (!state_filter)
		sysrq_sched_debug_show();

	rcu_read_unlock();
	/*
	 * Only show locks if all tasks are dumped:
	 */
	ଯଦି (!state_filter)
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
ଶୂନ୍ୟ __init init_idle(ସଂରଚନା task_struct *idle, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା affinity_context ac = (ସଂରଚନା affinity_context) {
		.new_mask  = cpumask_of(cpu),
		.flags     = 0,
	};
	ସଂରଚନା rq *rq = cpu_rq(cpu);
	unsigned long flags;

	raw_spin_lock_irqsave(&idle->pi_lock, flags);
	raw_spin_rq_lock(rq);

	idle->__state = TASK_RUNNING;
	idle->se.exec_start = sched_clock();
	/*
	 * PF_KTHREAD should already be set at this point; regardless, make it
	 * look like a proper per-CPU kthread.
	 */
	idle->flags |= PF_KTHREAD | PF_NO_SETAFFINITY;
	kthread_set_per_cpu(idle, cpu);

	/*
	 * No validation and serialization required at boot time and for
	 * setting up the idle tasks of not yet online CPUs.
	 */
	set_cpus_allowed_common(idle, &ac);
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
	rcu_read_lock();
	__set_task_cpu(idle, cpu);
	rcu_read_unlock();

	rq->idle = idle;
	rq_set_donor(rq, idle);
	rcu_assign_pointer(rq->curr, idle);
	idle->on_rq = TASK_ON_RQ_QUEUED;
	idle->on_cpu = 1;
	raw_spin_rq_unlock(rq);
	raw_spin_unlock_irqrestore(&idle->pi_lock, flags);

	/* Set the preempt count _outside_ the spinlocks! */
	init_idle_preempt_ଗଣନା(idle, cpu);

	/*
	 * The idle tasks have their own, simple scheduling class:
	 */
	idle->sched_class = &idle_sched_class;
	ftrace_graph_init_idle_task(idle, cpu);
	vtime_init_idle(idle, cpu);
	sprintf(idle->comm, "%s/%d", INIT_TASK_COMM, cpu);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpuset_cpumask_can_shrink(ଅଚଳ ସଂରଚନା cpumask *cur,
			      ଅଚଳ ସଂରଚନା cpumask *trial)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 1;

	ଯଦି (cpumask_empty(cur))
		ଫେରନ୍ତୁ ret;

	ret = dl_cpuset_cpumask_can_shrink(cur, trial);

	ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା task_can_attach(ସଂରଚନା task_struct *p)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	/*
	 * Kthreads which disallow setaffinity shouldn't be moved
	 * to a new cpuset; we don't want to change their CPU
	 * affinity and isolating such threads by their set of
	 * allowed nodes is unnecessary.  Thus, cpusets are not
	 * applicable for such threads.  This prevents checking for
	 * success of set_cpus_allowed_ptr() on all attached tasks
	 * before cpus_mask may be changed.
	 */
	ଯଦି (p->flags & PF_NO_SETAFFINITY)
		ret = -EINVAL;

	ଫେରନ୍ତୁ ret;
}

bool sched_smp_initialized __read_mostly;

#ifdef CONFIG_NUMA_BALANCING
/* Migrate current task p to target_cpu */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା migrate_task_to(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା target_cpu)
{
	ସଂରଚନା migration_arg arg = { p, target_cpu };
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା curr_cpu = task_cpu(p);

	ଯଦି (curr_cpu == target_cpu)
		ଫେରନ୍ତୁ 0;

	ଯଦି (!cpumask_test_cpu(target_cpu, p->cpus_ptr))
		ଫେରନ୍ତୁ -EINVAL;

	/* TODO: This is not properly updating schedstats */

	trace_sched_move_numa(p, curr_cpu, target_cpu);
	ଫେରନ୍ତୁ stop_one_cpu(curr_cpu, migration_cpu_stop, &arg);
}

/*
 * Requeue a task on a given node and accurately track the number of NUMA
 * tasks on the runqueues
 */
ଶୂନ୍ୟ sched_setnuma(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା nid)
{
	guard(task_rq_lock)(p);
	scoped_guard (sched_change, p, DEQUEUE_SAVE)
		p->numa_preferred_nid = nid;
}
#endif /* CONFIG_NUMA_BALANCING */

#ifdef CONFIG_HOTPLUG_CPU
/*
 * Invoked on the outgoing CPU in context of the CPU hotplug thread
 * after ensuring that there are no user space tasks left on the CPU.
 *
 * If there is a lazy mm in use on the hotplug thread, drop it and
 * switch to init_mm.
 *
 * The reference count on init_mm is dropped in finish_cpu().
 */
ସ୍ଥିର ଶୂନ୍ୟ sched_force_init_mm(ଶୂନ୍ୟ)
{
	ସଂରଚନା mm_struct *mm = current->active_mm;

	ଯଦି (mm != &init_mm) {
		mmgrab_lazy_tlb(&init_mm);
		local_irq_disable();
		current->active_mm = &init_mm;
		switch_mm_irqs_off(mm, &init_mm, current);
		local_irq_enable();
		finish_arch_post_lock_switch();
		mmdrop_lazy_tlb(mm);
	}

	/* finish_cpu(), as ran on the BP, will clean up the active_mm state */
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __balance_push_cpu_stop(ଶୂନ୍ୟ *arg)
{
	ସଂରଚନା task_struct *p = arg;
	ସଂରଚନା rq *rq = this_rq();
	ସଂରଚନା rq_flags rf;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

	scoped_guard (raw_spinlock_irq, &p->pi_lock) {
		/*
		 * We may change the underlying rq, but the locks held will
		 * appropriately be "transferred" when switching.
		 */
		context_unsafe_alias(rq);

		cpu = select_fallback_rq(rq->cpu, p);

		rq_lock(rq, &rf);
		update_rq_clock(rq);
		ଯଦି (task_rq(p) == rq && task_on_rq_queued(p))
			rq = __migrate_task(rq, &rf, p, cpu);
		rq_unlock(rq, &rf);
	}

	put_task_struct(p);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର DEFINE_PER_CPU(ସଂରଚନା cpu_stop_work, ଠେଲ_work);

/*
 * Ensure we only run per-cpu kthreads once the CPU goes !active.
 *
 * This is enabled below SCHED_AP_ACTIVE; when !cpu_active(), but only
 * effective when the hotplug motion is down.
 */
ସ୍ଥିର ଶୂନ୍ୟ balance_ଠେଲ(ସଂରଚନା rq *rq)
	__must_hold(__rq_lockp(rq))
{
	ସଂରଚନା task_struct *ଠେଲ_task = rq->curr;

	lockdep_assert_rq_held(rq);

	/*
	 * Ensure the thing is persistent until balance_push_set(.on = false);
	 */
	rq->balance_callback = &balance_ଠେଲ_callback;

	/*
	 * Only active while going offline and when invoked on the outgoing
	 * CPU.
	 */
	ଯଦି (!cpu_dying(rq->cpu) || rq != this_rq())
		ଫେରନ୍ତୁ;

	/*
	 * Both the cpu-hotplug and stop task are in this case and are
	 * required to complete the hotplug process.
	 */
	ଯଦି (kthread_is_per_cpu(ଠେଲ_task) ||
	    is_migration_disabled(ଠେଲ_task)) {

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
		ଯଦି (!rq->nr_running && !rq_has_pinned_tasks(rq) &&
		    rcuwait_active(&rq->hotplug_wait)) {
			raw_spin_rq_unlock(rq);
			rcuwait_wake_up(&rq->hotplug_wait);
			raw_spin_rq_lock(rq);
		}
		ଫେରନ୍ତୁ;
	}

	get_task_struct(ଠେଲ_task);
	/*
	 * Temporarily drop rq->lock such that we can wake-up the stop task.
	 * Both preemption and IRQs are still disabled.
	 */
	preempt_disable();
	raw_spin_rq_unlock(rq);
	stop_one_cpu_nowait(rq->cpu, __balance_push_cpu_stop, ଠେଲ_task,
			    this_cpu_ptr(&ଠେଲ_work));
	preempt_enable();
	/*
	 * At this point need_resched() is true and we'll take the loop in
	 * schedule(). The next pick is obviously going to be the stop task
	 * which kthread_is_per_cpu() and will push this task away.
	 */
	raw_spin_rq_lock(rq);
}

ସ୍ଥିର ଶୂନ୍ୟ balance_ଠେଲ_set(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, bool on)
{
	ସଂରଚନା rq *rq = cpu_rq(cpu);
	ସଂରଚନା rq_flags rf;

	rq_lock_irqsave(rq, &rf);
	ଯଦି (on) {
		WARN_ON_ONCE(rq->balance_callback);
		rq->balance_callback = &balance_ଠେଲ_callback;
	} ଅନ୍ୟଥା ଯଦି (rq->balance_callback == &balance_ଠେଲ_callback) {
		rq->balance_callback = ଖାଲି;
	}
	rq_unlock_irqrestore(rq, &rf);
}

/*
 * Invoked from a CPUs hotplug control thread after the CPU has been marked
 * inactive. All tasks which are not per CPU kernel threads are either
 * pushed off this CPU now via balance_push() or placed on a different CPU
 * during wakeup. Wait until the CPU is quiescent.
 */
ସ୍ଥିର ଶୂନ୍ୟ balance_hotplug_wait(ଶୂନ୍ୟ)
{
	ସଂରଚନା rq *rq = this_rq();

	rcuwait_wait_event(&rq->hotplug_wait,
			   rq->nr_running == 1 && !rq_has_pinned_tasks(rq),
			   TASK_UNINTERRUPTIBLE);
}

#ଅନ୍ୟଥା /* !CONFIG_HOTPLUG_CPU: */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ balance_ଠେଲ(ସଂରଚନା rq *rq)
{
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ balance_ଠେଲ_set(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, bool on)
{
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ balance_hotplug_wait(ଶୂନ୍ୟ)
{
}

#endif /* !CONFIG_HOTPLUG_CPU */

ଶୂନ୍ୟ set_rq_online(ସଂରଚନା rq *rq)
{
	ଯଦି (!rq->online) {
		ଅଚଳ ସଂରଚନା sched_class *class;

		cpumask_set_cpu(rq->cpu, rq->rd->online);
		rq->online = 1;

		for_each_class(class) {
			ଯଦି (class->rq_online)
				class->rq_online(rq);
		}
	}
}

ଶୂନ୍ୟ set_rq_offline(ସଂରଚନା rq *rq)
{
	ଯଦି (rq->online) {
		ଅଚଳ ସଂରଚନା sched_class *class;

		update_rq_clock(rq);
		for_each_class(class) {
			ଯଦି (class->rq_offline)
				class->rq_offline(rq);
		}

		cpumask_clear_cpu(rq->cpu, rq->rd->online);
		rq->online = 0;
	}
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_set_rq_online(ସଂରଚନା rq *rq, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା rq_flags rf;

	rq_lock_irqsave(rq, &rf);
	ଯଦି (rq->rd) {
		BUG_ON(!cpumask_test_cpu(cpu, rq->rd->span));
		set_rq_online(rq);
	}
	rq_unlock_irqrestore(rq, &rf);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_set_rq_offline(ସଂରଚନା rq *rq, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା rq_flags rf;

	rq_lock_irqsave(rq, &rf);
	ଯଦି (rq->rd) {
		BUG_ON(!cpumask_test_cpu(cpu, rq->rd->span));
		set_rq_offline(rq);
	}
	rq_unlock_irqrestore(rq, &rf);
}

/*
 * used to mark begin/end of suspend/resume:
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_cpus_frozen;

/*
 * Update cpusets according to cpu_active mask.  If cpusets are
 * disabled, cpuset_update_active_cpus() becomes a simple wrapper
 * around partition_sched_domains().
 *
 * If we come here as part of a suspend/resume, don't touch cpusets because we
 * want to restore it back to its original state upon resume anyway.
 */
ସ୍ଥିର ଶୂନ୍ୟ cpuset_cpu_active(ଶୂନ୍ୟ)
{
	ଯଦି (cpuhp_tasks_frozen) {
		/*
		 * num_cpus_frozen tracks how many CPUs are involved in suspend
		 * resume sequence. As long as this is not the last online
		 * operation in the resume sequence, just build a single sched
		 * domain, ignoring cpusets.
		 */
		cpuset_reset_sched_domains();
		ଯଦି (--ଅଙ୍କ_cpus_frozen)
			ଫେରନ୍ତୁ;
		/*
		 * This is the last CPU online operation. So fall through and
		 * restore the original sched domains by considering the
		 * cpuset configurations.
		 */
		cpuset_force_rebuild();
	}
	cpuset_update_active_cpus();
}

ସ୍ଥିର ଶୂନ୍ୟ cpuset_cpu_inactive(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଯଦି (!cpuhp_tasks_frozen) {
		cpuset_update_active_cpus();
	} ଅନ୍ୟଥା {
		ଅଙ୍କ_cpus_frozen++;
		cpuset_reset_sched_domains();
	}
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_smt_present_inc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଯଦି (cpumask_weight(cpu_smt_mask(cpu)) == 2)
		static_branch_inc_cpuslocked(&sched_smt_present);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_smt_present_dec(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଯଦି (cpumask_weight(cpu_smt_mask(cpu)) == 2)
		static_branch_dec_cpuslocked(&sched_smt_present);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_cpu_activate(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା rq *rq = cpu_rq(cpu);

	/*
	 * Clear the balance_push callback and prepare to schedule
	 * regular tasks.
	 */
	balance_ଠେଲ_set(cpu, false);

	/*
	 * When going up, increment the number of cores with SMT present.
	 */
	sched_smt_present_inc(cpu);
	set_cpu_active(cpu, true);

	ଯଦି (sched_smp_initialized) {
		sched_update_numa(cpu, true);
		sched_domains_numa_masks_set(cpu);
		cpuset_cpu_active();
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
	sched_set_rq_online(rq, cpu);

	/* preferred is subset of active and follows its state */
	set_cpu_preferred(cpu, true);

	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_cpu_deactivate(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା rq *rq = cpu_rq(cpu);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ret = dl_bw_deactivate(cpu);

	ଯଦି (ret)
		ଫେରନ୍ତୁ ret;

	set_cpu_preferred(cpu, false);

	/*
	 * Remove CPU from nohz.idle_cpus_mask to prevent participating in
	 * load balancing when not active
	 */
	scoped_guard (rcu)
		nohz_balance_exit_idle(rq);

	set_cpu_active(cpu, false);

	/*
	 * From this point forward, this CPU will refuse to run any task that
	 * is not: migrate_disable() or KTHREAD_IS_PER_CPU, and will actively
	 * push those tasks away until this gets cleared, see
	 * sched_cpu_dying().
	 */
	balance_ଠେଲ_set(cpu, true);

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

	sched_set_rq_offline(rq, cpu);

	scx_rq_deactivate(rq);

	/*
	 * When going down, decrement the number of cores with SMT present.
	 */
	sched_smt_present_dec(cpu);

	sched_core_cpu_deactivate(cpu);

	ଯଦି (!sched_smp_initialized)
		ଫେରନ୍ତୁ 0;

	sched_update_numa(cpu, false);
	cpuset_cpu_inactive(cpu);
	sched_domains_numa_masks_clear(cpu);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ sched_rq_cpu_starting(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା rq *rq = cpu_rq(cpu);

	rq->calc_load_update = calc_load_update;
	update_max_interval();
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_cpu_starting(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	sched_core_cpu_starting(cpu);
	sched_rq_cpu_starting(cpu);
	sched_tick_start(cpu);
	ଫେରନ୍ତୁ 0;
}

#ifdef CONFIG_HOTPLUG_CPU

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
ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_cpu_wait_empty(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	balance_hotplug_wait();
	sched_force_init_mm();
	ଫେରନ୍ତୁ 0;
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
ସ୍ଥିର ଶୂନ୍ୟ calc_load_migrate(ସଂରଚନା rq *rq)
{
	long delta = calc_load_fold_active(rq, 1);

	ଯଦି (delta)
		atomic_long_add(delta, &calc_load_tasks);
}

ସ୍ଥିର ଶୂନ୍ୟ dump_rq_tasks(ସଂରଚନା rq *rq, ଅଚଳ ଅକ୍ଷର *loglvl)
{
	ସଂରଚନା task_struct *g, *p;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = cpu_of(rq);

	lockdep_assert_rq_held(rq);

	printk("%sCPU%d enqueued tasks (%u total):\n", loglvl, cpu, rq->nr_running);
	for_each_process_thread(g, p) {
		ଯଦି (task_cpu(p) != cpu)
			ଜାରି;

		ଯଦି (!task_on_rq_queued(p))
			ଜାରି;

		printk("%s\tpid: %d, name: %s\n", loglvl, p->pid, p->comm);
	}
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_cpu_dying(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା rq *rq = cpu_rq(cpu);
	ସଂରଚନା rq_flags rf;

	/* Handle pending wakeups and then migrate everything off */
	sched_tick_stop(cpu);

	rq_lock_irqsave(rq, &rf);
	update_rq_clock(rq);
	ଯଦି (rq->nr_running != 1 || rq_has_pinned_tasks(rq)) {
		WARN(true, "Dying CPU not properly vacated!");
		dump_rq_tasks(rq, KERN_WARNING);
	}
	dl_server_stop(&rq->fair_server);
#ifdef CONFIG_SCHED_CLASS_EXT
	dl_server_stop(&rq->ext_server);
#endif
	rq_unlock_irqrestore(rq, &rf);

	calc_load_migrate(rq);
	update_max_interval();
	hrtick_clear(rq);
	sched_core_cpu_dying(cpu);
	ଫେରନ୍ତୁ 0;
}
#endif /* CONFIG_HOTPLUG_CPU */

ଶୂନ୍ୟ __init sched_init_smp(ଶୂନ୍ୟ)
{
	sched_init_numa(NUMA_NO_ଗଣ୍ଠି);

	prandom_init_once(&sched_rnd_state);

	/*
	 * There's no userspace yet to cause hotplug operations; hence all the
	 * CPU masks are stable and all blatant races in the below code cannot
	 * happen.
	 */
	sched_domains_mutex_lock();
	sched_init_domains(cpu_active_mask);
	sched_domains_mutex_unlock();

	/* Move init over to a non-isolated CPU */
	ଯଦି (set_cpus_allowed_ptr(current, housekeeping_cpumask(HK_TYPE_DOMAIN)) < 0)
		BUG();
	current->flags &= ~PF_NO_SETAFFINITY;
	sched_init_granularity();

	init_sched_rt_class();
	init_sched_dl_class();

	sched_init_dl_servers();

	sched_smp_initialized = true;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init migration_init(ଶୂନ୍ୟ)
{
	sched_cpu_starting(smp_processor_id());
	ଫେରନ୍ତୁ 0;
}
early_initcall(migration_init);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା in_sched_functions(unsigned long addr)
{
	ଫେରନ୍ତୁ in_lock_functions(addr) ||
		(addr >= (unsigned long)__sched_text_start
		&& addr < (unsigned long)__sched_text_end);
}

#ifdef CONFIG_CGROUP_SCHED
/*
 * Default task group.
 * Every task in system belongs to this group at bootup.
 */
ସଂରଚନା task_group ମୂଳ_task_group;
LIST_HEAD(task_groups);

/* Cacheline aligned slab cache for task_group */
ସ୍ଥିର ସଂରଚନା kmem_cache *task_group_cache __ro_after_init;
#endif

ଶୂନ୍ୟ __init sched_init(ଶୂନ୍ୟ)
{
	unsigned long __maybe_unused ptr = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	/* Make sure the linker didn't screw up */
	BUG_ON(!sched_class_above(&stop_sched_class, &dl_sched_class));
	BUG_ON(!sched_class_above(&dl_sched_class, &rt_sched_class));
	BUG_ON(!sched_class_above(&rt_sched_class, &fair_sched_class));
	BUG_ON(!sched_class_above(&fair_sched_class, &idle_sched_class));
#ifdef CONFIG_SCHED_CLASS_EXT
	BUG_ON(!sched_class_above(&fair_sched_class, &ext_sched_class));
	BUG_ON(!sched_class_above(&ext_sched_class, &idle_sched_class));
#endif

	wait_bit_init();

#ifdef CONFIG_FAIR_GROUP_SCHED
	ମୂଳ_task_group.cfs_rq = &runqueues.cfs;

	ମୂଳ_task_group.shares = ମୂଳ_TASK_GROUP_LOAD;
	init_cfs_bandwidth(&ମୂଳ_task_group.cfs_bandwidth, ଖାଲି);
#endif /* CONFIG_FAIR_GROUP_SCHED */
#ifdef CONFIG_EXT_GROUP_SCHED
	scx_tg_init(&ମୂଳ_task_group);
#endif /* CONFIG_EXT_GROUP_SCHED */
#ifdef CONFIG_RT_GROUP_SCHED
	ptr += 2 * nr_cpu_ids * ଆକାର(ଶୂନ୍ୟ **);
	ptr = (unsigned long)kzalloc(ptr, GFP_NOWAIT);
	ମୂଳ_task_group.rt_se = (ସଂରଚନା sched_rt_entity **)ptr;
	ptr += nr_cpu_ids * ଆକାର(ଶୂନ୍ୟ **);

	ମୂଳ_task_group.rt_rq = (ସଂରଚନା rt_rq **)ptr;
	ptr += nr_cpu_ids * ଆକାର(ଶୂନ୍ୟ **);

#endif /* CONFIG_RT_GROUP_SCHED */

	init_defrootdomain();

#ifdef CONFIG_RT_GROUP_SCHED
	init_rt_bandwidth(&ମୂଳ_task_group.rt_bandwidth,
			global_rt_period(), global_rt_runtime());
#endif /* CONFIG_RT_GROUP_SCHED */

#ifdef CONFIG_CGROUP_SCHED
	task_group_cache = KMEM_CACHE(task_group, 0);

	list_add(&ମୂଳ_task_group.list, &task_groups);
	INIT_LIST_HEAD(&ମୂଳ_task_group.children);
	INIT_LIST_HEAD(&ମୂଳ_task_group.siblings);
	autogroup_init(&init_task);
#endif /* CONFIG_CGROUP_SCHED */

	for_each_possible_cpu(i) {
		ସଂରଚନା rq *rq;

		rq = cpu_rq(i);
		raw_spin_lock_init(&rq->__lock);
		rq->nr_running = 0;
		rq->calc_load_active = 0;
		rq->calc_load_update = jiffies + LOAD_FREQ;
		init_cfs_rq(&rq->cfs);
		init_rt_rq(&rq->rt);
		init_dl_rq(&rq->dl);
#ifdef CONFIG_FAIR_GROUP_SCHED
		INIT_LIST_HEAD(&rq->leaf_cfs_rq_list);
		rq->tmp_alone_branch = &rq->leaf_cfs_rq_list;
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
		init_tg_cfs_entry(&ମୂଳ_task_group, &rq->cfs, ଖାଲି, i, ଖାଲି);
#endif /* CONFIG_FAIR_GROUP_SCHED */

#ifdef CONFIG_RT_GROUP_SCHED
		/*
		 * This is required for init cpu because rt.c:__enable_runtime()
		 * starts working after scheduler_running, which is not the case
		 * yet.
		 */
		rq->rt.rt_runtime = global_rt_runtime();
		init_tg_rt_entry(&ମୂଳ_task_group, &rq->rt, ଖାଲି, i, ଖାଲି);
#endif
		rq->next_class = &idle_sched_class;

		rq->sd = ଖାଲି;
		rq->rd = ଖାଲି;
		rq->cpu_capacity = SCHED_CAPACITY_SCALE;
		rq->balance_callback = &balance_ଠେଲ_callback;
		rq->active_balance = 0;
		rq->next_balance = jiffies;
		rq->ଠେଲ_cpu = 0;
		rq->cpu = i;
		rq->online = 0;
		rq->idle_stamp = 0;
		rq->avg_idle = 2*sysctl_sched_migration_cost;
		rq->max_idle_balance_cost = sysctl_sched_migration_cost;

		INIT_LIST_HEAD(&rq->cfs_tasks);

		rq_attach_ମୂଳ(rq, &def_ମୂଳ_domain);
#ifdef CONFIG_NO_HZ_COMMON
		rq->last_blocked_load_update_tick = jiffies;
		atomic_set(&rq->nohz_flags, 0);

		INIT_CSD(&rq->nohz_csd, nohz_csd_func, rq);
#endif
#ifdef CONFIG_HOTPLUG_CPU
		rcuwait_init(&rq->hotplug_wait);
#endif
		hrtick_rq_init(rq);
		atomic_set(&rq->nr_iowait, 0);
		fair_server_init(rq);
#ifdef CONFIG_SCHED_CLASS_EXT
		ext_server_init(rq);
#endif

#ifdef CONFIG_SCHED_CORE
		rq->core = rq;
		rq->core_pick = ଖାଲି;
		rq->core_dl_server = ଖାଲି;
		rq->core_enabled = 0;
		rq->core_ବୃକ୍ଷ = RB_ମୂଳ;
		rq->core_forceidle_ଗଣନା = 0;
		rq->core_forceidle_occupation = 0;
		rq->core_forceidle_start = 0;
		rq->core_pick_in_flight = 0;

		rq->core_cookie = 0UL;
#endif
#ifdef CONFIG_SCHED_CACHE
		raw_spin_lock_init(&rq->cpu_epoch_lock);
		rq->cpu_epoch_next = jiffies;
#endif

		zalloc_cpumask_var_ଗଣ୍ଠି(&rq->scratch_mask, GFP_KERNEL, cpu_to_ଗଣ୍ଠି(i));
	}

	set_load_weight(&init_task, false);
	init_task.se.slice = sysctl_sched_base_slice,

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
	WARN_ON(!set_kthread_struct(current));

	/*
	 * Make us the idle thread. Technically, schedule() should not be
	 * called from this thread, however somewhere below it might be,
	 * but because we are the idle thread, we just pick up running again
	 * when this runqueue becomes "idle".
	 */
	__sched_fork(0, current);
	init_idle(current, smp_processor_id());

	calc_load_update = jiffies + LOAD_FREQ;

	idle_thread_set_boot_cpu();

	balance_ଠେଲ_set(smp_processor_id(), false);
	init_sched_fair_class();
	init_sched_ext_class();

	psi_init();

	init_uclamp();

	preempt_dynamic_init();

	scheduler_running = 1;
}

#ifdef CONFIG_DEBUG_ATOMIC_SLEEP

ଶୂନ୍ୟ __might_sleep(ଅଚଳ ଅକ୍ଷର *file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା state = get_current_state();
	/*
	 * Blocking primitives will set (and therefore destroy) current->state,
	 * since we will exit with TASK_RUNNING make sure we enter with it,
	 * otherwise we will destroy state.
	 */
	WARN_ONCE(state != TASK_RUNNING && current->task_state_change,
			"do not call blocking ops when !TASK_RUNNING; "
			"state=%x set at [<%p>] %pS\n", state,
			(ଶୂନ୍ୟ *)current->task_state_change,
			(ଶୂନ୍ୟ *)current->task_state_change);

	__might_resched(file, ଧାଡ଼ି, 0);
}
EXPORT_ପ୍ରତୀକ(__might_sleep);

ସ୍ଥିର ଶୂନ୍ୟ print_preempt_disable_ip(ପୂର୍ଣ୍ଣସଂଖ୍ୟା preempt_offset, unsigned long ip)
{
	ଯଦି (!IS_ENABLED(CONFIG_DEBUG_PREEMPT))
		ଫେରନ୍ତୁ;

	ଯଦି (preempt_ଗଣନା() == preempt_offset)
		ଫେରନ୍ତୁ;

	pr_err("Preemption disabled at:");
	print_ip_sym(KERN_ERR, ip);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool resched_offsets_ok(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offsets)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nested = preempt_ଗଣନା();

	nested += rcu_preempt_depth() << MIGHT_RESCHED_RCU_SHIFT;

	ଫେରନ୍ତୁ nested == offsets;
}

ଶୂନ୍ୟ __might_resched(ଅଚଳ ଅକ୍ଷର *file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offsets)
{
	/* Ratelimiting timestamp: */
	ସ୍ଥିର unsigned long prev_jiffy;

	unsigned long preempt_disable_ip;

	/* WARN_ON_ONCE() by default, no rate limit required: */
	rcu_sleep_check();

	ଯଦି ((resched_offsets_ok(offsets) && !irqs_disabled() &&
	     !is_idle_task(current) && !current->non_block_ଗଣନା) ||
	    system_state == SYSTEM_BOOTING || system_state > SYSTEM_RUNNING ||
	    oops_in_progress)
		ଫେରନ୍ତୁ;

	ଯଦି (time_before(jiffies, prev_jiffy + HZ) && prev_jiffy)
		ଫେରନ୍ତୁ;
	prev_jiffy = jiffies;

	/* Save this before calling printk(), since that will clobber it: */
	preempt_disable_ip = get_preempt_disable_ip(current);

	pr_err("BUG: sleeping function called from invalid context at %s:%d\n",
	       file, ଧାଡ଼ି);
	pr_err("in_atomic(): %d, irqs_disabled(): %d, non_block: %d, pid: %d, name: %s\n",
	       in_atomic(), irqs_disabled(), current->non_block_ଗଣନା,
	       current->pid, current->comm);
	pr_err("preempt_count: %x, expected: %x\n", preempt_ଗଣନା(),
	       offsets & MIGHT_RESCHED_PREEMPT_MASK);

	ଯଦି (IS_ENABLED(CONFIG_PREEMPT_RCU)) {
		pr_err("RCU nest depth: %d, expected: %u\n",
		       rcu_preempt_depth(), offsets >> MIGHT_RESCHED_RCU_SHIFT);
	}

	ଯଦି (task_ଗଦା_end_corrupted(current))
		pr_emerg("Thread overran stack, or stack corrupted\n");

	debug_show_held_locks(current);
	ଯଦି (irqs_disabled())
		print_irqtrace_events(current);

	print_preempt_disable_ip(offsets & MIGHT_RESCHED_PREEMPT_MASK,
				 preempt_disable_ip);

	dump_ଗଦା();
	add_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}
EXPORT_ପ୍ରତୀକ(__might_resched);

ଶୂନ୍ୟ __cant_sleep(ଅଚଳ ଅକ୍ଷର *file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି)
{
	ସ୍ଥିର unsigned long prev_jiffy;

	ଯଦି (irqs_disabled())
		ଫେରନ୍ତୁ;

	ଯଦି (!IS_ENABLED(CONFIG_PREEMPT_ଗଣନା))
		ଫେରନ୍ତୁ;

	ଯଦି (preempt_ଗଣନା())
		ଫେରନ୍ତୁ;

	ଯଦି (time_before(jiffies, prev_jiffy + HZ) && prev_jiffy)
		ଫେରନ୍ତୁ;
	prev_jiffy = jiffies;

	printk(KERN_ERR "BUG: assuming atomic context at %s:%d\n", file, ଧାଡ଼ି);
	printk(KERN_ERR "in_atomic(): %d, irqs_disabled(): %d, pid: %d, name: %s\n",
			in_atomic(), irqs_disabled(),
			current->pid, current->comm);

	debug_show_held_locks(current);
	dump_ଗଦା();
	add_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}
EXPORT_ପ୍ରତୀକ_GPL(__cant_sleep);

# ifdef CONFIG_SMP
ଶୂନ୍ୟ __cant_migrate(ଅଚଳ ଅକ୍ଷର *file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି)
{
	ସ୍ଥିର unsigned long prev_jiffy;

	ଯଦି (irqs_disabled())
		ଫେରନ୍ତୁ;

	ଯଦି (is_migration_disabled(current))
		ଫେରନ୍ତୁ;

	ଯଦି (!IS_ENABLED(CONFIG_PREEMPT_ଗଣନା))
		ଫେରନ୍ତୁ;

	ଯଦି (preempt_ଗଣନା())
		ଫେରନ୍ତୁ;

	ଯଦି (time_before(jiffies, prev_jiffy + HZ) && prev_jiffy)
		ଫେରନ୍ତୁ;
	prev_jiffy = jiffies;

	pr_err("BUG: assuming non migratable context at %s:%d\n", file, ଧାଡ଼ି);
	pr_err("in_atomic(): %d, irqs_disabled(): %d, migration_disabled() %u pid: %d, name: %s\n",
	       in_atomic(), irqs_disabled(), is_migration_disabled(current),
	       current->pid, current->comm);

	debug_show_held_locks(current);
	dump_ଗଦା();
	add_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}
EXPORT_ପ୍ରତୀକ_GPL(__cant_migrate);
# endif /* CONFIG_SMP */
#endif /* CONFIG_DEBUG_ATOMIC_SLEEP */

#ifdef CONFIG_MAGIC_SYSRQ
ଶୂନ୍ୟ normalize_rt_tasks(ଶୂନ୍ୟ)
{
	ସଂରଚନା task_struct *g, *p;
	ସଂରଚନା sched_attr attr = {
		.sched_policy = SCHED_NORMAL,
	};

	read_lock(&tasklist_lock);
	for_each_process_thread(g, p) {
		/*
		 * Only normalize user tasks:
		 */
		ଯଦି (p->flags & PF_KTHREAD)
			ଜାରି;

		p->se.exec_start = 0;
		schedstat_set(p->stats.wait_start,  0);
		schedstat_set(p->stats.sleep_start, 0);
		schedstat_set(p->stats.block_start, 0);

		ଯଦି (!rt_or_dl_task(p)) {
			/*
			 * Renice negative nice level userspace
			 * tasks back to 0:
			 */
			ଯଦି (task_nice(p) < 0)
				set_user_nice(p, 0);
			ଜାରି;
		}

		__sched_setscheduler(p, &attr, false, false);
	}
	read_unlock(&tasklist_lock);
}

#endif /* CONFIG_MAGIC_SYSRQ */

#ifdef CONFIG_KGDB_KDB
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
ସଂରଚନା task_struct *curr_task(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଫେରନ୍ତୁ cpu_curr(cpu);
}

#endif /* CONFIG_KGDB_KDB */

#ifdef CONFIG_CGROUP_SCHED
/* task_group_lock serializes the addition/removal of task groups */
ସ୍ଥିର DEFINE_SPINLOCK(task_group_lock);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ alloc_uclamp_sched_group(ସଂରଚନା task_group *tg,
					    ସଂରଚନା task_group *parent)
{
#ifdef CONFIG_UCLAMP_TASK_GROUP
	ଗଣନା uclamp_id clamp_id;

	for_each_clamp_id(clamp_id) {
		uclamp_se_set(&tg->uclamp_req[clamp_id],
			      uclamp_none(clamp_id), false);
		tg->uclamp[clamp_id] = parent->uclamp[clamp_id];
	}
#endif
}

ସ୍ଥିର ଶୂନ୍ୟ sched_free_group(ସଂରଚନା task_group *tg)
{
	free_fair_sched_group(tg);
	free_rt_sched_group(tg);
	autogroup_free(tg);
	kmem_cache_free(task_group_cache, tg);
}

ସ୍ଥିର ଶୂନ୍ୟ sched_free_group_rcu(ସଂରଚନା rcu_head *rcu)
{
	sched_free_group(container_of(rcu, ସଂରଚନା task_group, rcu));
}

ସ୍ଥିର ଶୂନ୍ୟ sched_unregister_group(ସଂରଚନା task_group *tg)
{
	unregister_fair_sched_group(tg);
	unregister_rt_sched_group(tg);
	/*
	 * We have to wait for yet another RCU grace period to expire, as
	 * print_cfs_stats() might run concurrently.
	 */
	call_rcu(&tg->rcu, sched_free_group_rcu);
}

/* allocate runqueue etc for a new task group */
ସଂରଚନା task_group *sched_create_group(ସଂରଚନା task_group *parent)
{
	ସଂରଚନା task_group *tg;

	tg = kmem_cache_alloc(task_group_cache, GFP_KERNEL | __GFP_ZERO);
	ଯଦି (!tg)
		ଫେରନ୍ତୁ ERR_PTR(-ENOMEM);

	ଯଦି (!alloc_fair_sched_group(tg, parent))
		ଯାଅ err;

	ଯଦି (!alloc_rt_sched_group(tg, parent))
		ଯାଅ err;

	scx_tg_init(tg);
	alloc_uclamp_sched_group(tg, parent);

	ଫେରନ୍ତୁ tg;

err:
	sched_free_group(tg);
	ଫେରନ୍ତୁ ERR_PTR(-ENOMEM);
}

ଶୂନ୍ୟ sched_online_group(ସଂରଚନା task_group *tg, ସଂରଚନା task_group *parent)
{
	unsigned long flags;

	spin_lock_irqsave(&task_group_lock, flags);
	list_add_tail_rcu(&tg->list, &task_groups);

	/* Root should already exist: */
	WARN_ON(!parent);

	tg->parent = parent;
	INIT_LIST_HEAD(&tg->children);
	list_add_rcu(&tg->siblings, &parent->children);
	spin_unlock_irqrestore(&task_group_lock, flags);

	online_fair_sched_group(tg);
}

/* RCU callback to free various structures associated with a task group */
ସ୍ଥିର ଶୂନ୍ୟ sched_unregister_group_rcu(ସଂରଚନା rcu_head *rhp)
{
	/* Now it should be safe to free those cfs_rqs: */
	sched_unregister_group(container_of(rhp, ସଂରଚନା task_group, rcu));
}

ଶୂନ୍ୟ sched_destroy_group(ସଂରଚନା task_group *tg)
{
	/* Wait for possible concurrent references to cfs_rqs complete: */
	call_rcu(&tg->rcu, sched_unregister_group_rcu);
}

ଶୂନ୍ୟ sched_release_group(ସଂରଚନା task_group *tg)
{
	unsigned long flags;

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
	spin_lock_irqsave(&task_group_lock, flags);
	list_del_rcu(&tg->list);
	list_del_rcu(&tg->siblings);
	spin_unlock_irqrestore(&task_group_lock, flags);
}

ସ୍ଥିର ଶୂନ୍ୟ sched_change_group(ସଂରଚନା task_struct *tsk)
{
	ସଂରଚନା task_group *tg;

	/*
	 * All callers are synchronized by task_rq_lock(); we do not use RCU
	 * which is pointless here. Thus, we pass "true" to task_css_check()
	 * to prevent lockdep warnings.
	 */
	tg = container_of(task_css_check(tsk, cpu_cgrp_id, true),
			  ସଂରଚନା task_group, css);
	tg = autogroup_task_group(tsk, tg);
	tsk->sched_task_group = tg;

#ifdef CONFIG_FAIR_GROUP_SCHED
	ଯଦି (tsk->sched_class->task_change_group)
		tsk->sched_class->task_change_group(tsk);
	ଅନ୍ୟଥା
#endif
		set_task_rq(tsk, task_cpu(tsk));
}

/*
 * Change task's runqueue when it moves between groups.
 *
 * The caller of this function should have put the task in its new group by
 * now. This function just updates tsk->se.cfs_rq and tsk->se.parent to reflect
 * its new group.
 */
ଶୂନ୍ୟ sched_move_task(ସଂରଚନା task_struct *tsk, bool for_autogroup)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା queue_flags = DEQUEUE_SAVE | DEQUEUE_MOVE;
	bool resched = false;
	bool queued = false;
	ସଂରଚନା rq *rq;

	CLASS(task_rq_lock, rq_guard)(tsk);
	rq = rq_guard.rq;

	scoped_guard (sched_change, tsk, queue_flags) {
		sched_change_group(tsk);
		ଯଦି (!for_autogroup)
			scx_cgroup_move_task(tsk);
		ଯଦି (scope->running)
			resched = true;
		queued = scope->queued;
	}

	ଯଦି (resched)
		resched_curr(rq);
	ଅନ୍ୟଥା ଯଦି (queued)
		wakeup_preempt(rq, tsk, 0);

	__balance_callbacks(rq, &rq_guard.rf);
}

ସ୍ଥିର ସଂରଚନା cgroup_subsys_state *
cpu_cgroup_css_alloc(ସଂରଚନା cgroup_subsys_state *parent_css)
{
	ସଂରଚନା task_group *parent = css_tg(parent_css);
	ସଂରଚନା task_group *tg;

	ଯଦି (!parent) {
		/* This is early initialization for the top cgroup */
		ଫେରନ୍ତୁ &ମୂଳ_task_group.css;
	}

	tg = sched_create_group(parent);
	ଯଦି (IS_ERR(tg))
		ଫେରନ୍ତୁ ERR_PTR(-ENOMEM);

	ଫେରନ୍ତୁ &tg->css;
}

/* Expose task group only after completing cgroup initialization */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_cgroup_css_online(ସଂରଚନା cgroup_subsys_state *css)
{
	ସଂରଚନା task_group *tg = css_tg(css);
	ସଂରଚନା task_group *parent = css_tg(css->parent);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ret = scx_tg_online(tg);
	ଯଦି (ret)
		ଫେରନ୍ତୁ ret;

	ଯଦି (parent)
		sched_online_group(tg, parent);

#ifdef CONFIG_UCLAMP_TASK_GROUP
	/* Propagate the effective uclamp value for the new group */
	guard(mutex)(&uclamp_mutex);
	guard(rcu)();
	cpu_util_update_eff(css);
#endif

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ cpu_cgroup_css_offline(ସଂରଚନା cgroup_subsys_state *css)
{
	ସଂରଚନା task_group *tg = css_tg(css);

	scx_tg_offline(tg);
}

ସ୍ଥିର ଶୂନ୍ୟ cpu_cgroup_css_released(ସଂରଚନା cgroup_subsys_state *css)
{
	ସଂରଚନା task_group *tg = css_tg(css);

	sched_release_group(tg);
}

ସ୍ଥିର ଶୂନ୍ୟ cpu_cgroup_css_free(ସଂରଚନା cgroup_subsys_state *css)
{
	ସଂରଚନା task_group *tg = css_tg(css);

	/*
	 * Relies on the RCU grace period between css_released() and this.
	 */
	sched_unregister_group(tg);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_cgroup_can_attach(ସଂରଚନା cgroup_taskset *tset)
{
#ifdef CONFIG_RT_GROUP_SCHED
	ସଂରଚନା task_struct *task;
	ସଂରଚନା cgroup_subsys_state *css;

	ଯଦି (!rt_group_sched_enabled())
		ଯାଅ scx_check;

	cgroup_taskset_for_each(task, css, tset) {
		ଯଦି (!sched_rt_can_attach(css_tg(css), task))
			ଫେରନ୍ତୁ -EINVAL;
	}
scx_check:
#endif /* CONFIG_RT_GROUP_SCHED */
	ଫେରନ୍ତୁ scx_cgroup_can_attach(tset);
}

ସ୍ଥିର ଶୂନ୍ୟ cpu_cgroup_attach(ସଂରଚନା cgroup_taskset *tset)
{
	ସଂରଚନା task_struct *task;
	ସଂରଚନା cgroup_subsys_state *css;

	cgroup_taskset_for_each(task, css, tset)
		sched_move_task(task, false);
}

ସ୍ଥିର ଶୂନ୍ୟ cpu_cgroup_cancel_attach(ସଂରଚନା cgroup_taskset *tset)
{
	scx_cgroup_cancel_attach(tset);
}

#ifdef CONFIG_UCLAMP_TASK_GROUP
ସ୍ଥିର ଶୂନ୍ୟ cpu_util_update_eff(ସଂରଚନା cgroup_subsys_state *css)
{
	ସଂରଚନା cgroup_subsys_state *ଶୀର୍ଷ_css = css;
	ସଂରଚନା uclamp_se *uc_parent = ଖାଲି;
	ସଂରଚନା uclamp_se *uc_se = ଖାଲି;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା eff[UCLAMP_CNT];
	ଗଣନା uclamp_id clamp_id;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା clamps;

	lockdep_assert_held(&uclamp_mutex);
	WARN_ON_ONCE(!rcu_read_lock_held());

	css_for_each_descendant_pre(css, ଶୀର୍ଷ_css) {
		uc_parent = css_tg(css)->parent
			? css_tg(css)->parent->uclamp : ଖାଲି;

		for_each_clamp_id(clamp_id) {
			/* Assume effective clamps matches requested clamps */
			eff[clamp_id] = css_tg(css)->uclamp_req[clamp_id].ମୂଲ୍ୟ;
			/* Cap effective clamps with parent's effective clamps */
			ଯଦି (uc_parent &&
			    eff[clamp_id] > uc_parent[clamp_id].ମୂଲ୍ୟ) {
				eff[clamp_id] = uc_parent[clamp_id].ମୂଲ୍ୟ;
			}
		}
		/* Ensure protection is always capped by limit */
		eff[UCLAMP_MIN] = min(eff[UCLAMP_MIN], eff[UCLAMP_MAX]);

		/* Propagate most restrictive effective clamps */
		clamps = 0x0;
		uc_se = css_tg(css)->uclamp;
		for_each_clamp_id(clamp_id) {
			ଯଦି (eff[clamp_id] == uc_se[clamp_id].ମୂଲ୍ୟ)
				ଜାରି;
			uc_se[clamp_id].ମୂଲ୍ୟ = eff[clamp_id];
			uc_se[clamp_id].bucket_id = uclamp_bucket_id(eff[clamp_id]);
			clamps |= (0x1 << clamp_id);
		}
		ଯଦି (!clamps) {
			css = css_rightmost_descendant(css);
			ଜାରି;
		}

		/* Immediately update descendants RUNNABLE tasks */
		uclamp_update_active_tasks(css);
	}
}

/*
 * Integer 10^N with a given N exponent by casting to integer the literal "1eN"
 * C expression. Since there is no way to convert a macro argument (N) into a
 * character constant, use two levels of macros.
 */
#define _POW10(exp) ((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)1e##exp)
#define POW10(exp) _POW10(exp)

ସଂରଚନା uclamp_request {
#define UCLAMP_PERCENT_SHIFT	2
#define UCLAMP_PERCENT_SCALE	(100 * POW10(UCLAMP_PERCENT_SHIFT))
	s64 percent;
	u64 util;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା uclamp_request
capacity_from_percent(ଅକ୍ଷର *buf)
{
	ସଂରଚନା uclamp_request req = {
		.percent = UCLAMP_PERCENT_SCALE,
		.util = SCHED_CAPACITY_SCALE,
		.ret = 0,
	};

	buf = strim(buf);
	ଯଦି (strcmp(buf, "max")) {
		req.ret = cgroup_parse_float(buf, UCLAMP_PERCENT_SHIFT,
					     &req.percent);
		ଯଦି (req.ret)
			ଫେରନ୍ତୁ req;
		ଯଦି ((u64)req.percent > UCLAMP_PERCENT_SCALE) {
			req.ret = -ERANGE;
			ଫେରନ୍ତୁ req;
		}

		req.util = req.percent << SCHED_CAPACITY_SHIFT;
		req.util = DIV_ROUND_CLOSEST_ULL(req.util, UCLAMP_PERCENT_SCALE);
	}

	ଫେରନ୍ତୁ req;
}

ସ୍ଥିର ssize_t cpu_uclamp_write(ସଂରଚନା kernfs_open_file *of, ଅକ୍ଷର *buf,
				size_t nbytes, loff_t off,
				ଗଣନା uclamp_id clamp_id)
{
	ସଂରଚନା uclamp_request req;
	ସଂରଚନା task_group *tg;

	req = capacity_from_percent(buf);
	ଯଦି (req.ret)
		ଫେରନ୍ତୁ req.ret;

	sched_uclamp_enable();

	guard(mutex)(&uclamp_mutex);
	guard(rcu)();

	tg = css_tg(of_css(of));
	ଯଦି (tg->uclamp_req[clamp_id].ମୂଲ୍ୟ != req.util)
		uclamp_se_set(&tg->uclamp_req[clamp_id], req.util, false);

	/*
	 * Because of not recoverable conversion rounding we keep track of the
	 * exact requested value
	 */
	tg->uclamp_pct[clamp_id] = req.percent;

	/* Update effective clamps to track the most restrictive value */
	cpu_util_update_eff(of_css(of));

	ଫେରନ୍ତୁ nbytes;
}

ସ୍ଥିର ssize_t cpu_uclamp_min_write(ସଂରଚନା kernfs_open_file *of,
				    ଅକ୍ଷର *buf, size_t nbytes,
				    loff_t off)
{
	ଫେରନ୍ତୁ cpu_uclamp_write(of, buf, nbytes, off, UCLAMP_MIN);
}

ସ୍ଥିର ssize_t cpu_uclamp_max_write(ସଂରଚନା kernfs_open_file *of,
				    ଅକ୍ଷର *buf, size_t nbytes,
				    loff_t off)
{
	ଫେରନ୍ତୁ cpu_uclamp_write(of, buf, nbytes, off, UCLAMP_MAX);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ cpu_uclamp_print(ସଂରଚନା seq_file *sf,
				    ଗଣନା uclamp_id clamp_id)
{
	ସଂରଚନା task_group *tg;
	u64 util_clamp;
	u64 percent;
	u32 rem;

	scoped_guard (rcu) {
		tg = css_tg(seq_css(sf));
		util_clamp = tg->uclamp_req[clamp_id].ମୂଲ୍ୟ;
	}

	ଯଦି (util_clamp == SCHED_CAPACITY_SCALE) {
		seq_puts(sf, "max\n");
		ଫେରନ୍ତୁ;
	}

	percent = tg->uclamp_pct[clamp_id];
	percent = div_u64_rem(percent, POW10(UCLAMP_PERCENT_SHIFT), &rem);
	seq_printf(sf, "%llu.%0*u\n", percent, UCLAMP_PERCENT_SHIFT, rem);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_uclamp_min_show(ସଂରଚନା seq_file *sf, ଶୂନ୍ୟ *v)
{
	cpu_uclamp_print(sf, UCLAMP_MIN);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_uclamp_max_show(ସଂରଚନା seq_file *sf, ଶୂନ୍ୟ *v)
{
	cpu_uclamp_print(sf, UCLAMP_MAX);
	ଫେରନ୍ତୁ 0;
}
#endif /* CONFIG_UCLAMP_TASK_GROUP */

#ifdef CONFIG_GROUP_SCHED_WEIGHT
ସ୍ଥିର unsigned long tg_weight(ସଂରଚନା task_group *tg)
{
#ifdef CONFIG_FAIR_GROUP_SCHED
	ଫେରନ୍ତୁ scale_load_down(tg->shares);
#ଅନ୍ୟଥା
	ଫେରନ୍ତୁ sched_weight_from_cgroup(tg->scx.weight);
#endif
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_shares_write_u64(ସଂରଚନା cgroup_subsys_state *css,
				ସଂରଚନା cftype *cftype, u64 shareval)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ଯଦି (shareval > scale_load_down(ULONG_MAX))
		shareval = MAX_SHARES;
	ret = sched_group_set_shares(css_tg(css), scale_load(shareval));
	ଯଦି (!ret)
		scx_group_set_weight(css_tg(css),
				     sched_weight_to_cgroup(shareval));
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର u64 cpu_shares_read_u64(ସଂରଚନା cgroup_subsys_state *css,
			       ସଂରଚନା cftype *cft)
{
	ଫେରନ୍ତୁ tg_weight(css_tg(css));
}
#endif /* CONFIG_GROUP_SCHED_WEIGHT */

#ifdef CONFIG_CFS_BANDWIDTH
ସ୍ଥିର DEFINE_MUTEX(cfs_constraints_mutex);

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __cfs_schedulable(ସଂରଚନା task_group *tg, u64 period, u64 runtime);

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tg_set_cfs_bandwidth(ସଂରଚନା task_group *tg,
				u64 period_us, u64 quota_us, u64 burst_us)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ret = 0, runtime_enabled, runtime_was_enabled;
	ସଂରଚନା cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
	u64 period, quota, burst;

	period = (u64)period_us * NSEC_PER_USEC;

	ଯଦି (quota_us == RUNTIME_INF)
		quota = RUNTIME_INF;
	ଅନ୍ୟଥା
		quota = (u64)quota_us * NSEC_PER_USEC;

	burst = (u64)burst_us * NSEC_PER_USEC;

	/*
	 * Prevent race between setting of cfs_rq->runtime_enabled and
	 * unthrottle_offline_cfs_rqs().
	 */
	guard(cpus_read_lock)();
	guard(mutex)(&cfs_constraints_mutex);

	ret = __cfs_schedulable(tg, period, quota);
	ଯଦି (ret)
		ଫେରନ୍ତୁ ret;

	runtime_enabled = quota != RUNTIME_INF;
	runtime_was_enabled = cfs_b->quota != RUNTIME_INF;
	/*
	 * If we need to toggle cfs_bandwidth_used, off->on must occur
	 * before making related changes, and on->off must occur afterwards
	 */
	ଯଦି (runtime_enabled && !runtime_was_enabled)
		cfs_bandwidth_usage_inc();

	scoped_guard (raw_spinlock_irq, &cfs_b->lock) {
		cfs_b->period = ns_to_ktime(period);
		cfs_b->quota = quota;
		cfs_b->burst = burst;

		__refill_cfs_bandwidth_runtime(cfs_b);

		/*
		 * Restart the period timer (if active) to handle new
		 * period expiry:
		 */
		ଯଦି (runtime_enabled)
			start_cfs_bandwidth(cfs_b);
	}

	for_each_online_cpu(i) {
		ସଂରଚନା cfs_rq *cfs_rq = tg_cfs_rq(tg, i);
		ସଂରଚନା rq *rq = cfs_rq->rq;

		guard(rq_lock_irq)(rq);

		cfs_rq->runtime_enabled = runtime_enabled;
		cfs_rq->runtime_remaining = 1;

		ଯଦି (cfs_rq->throttled) {
			update_rq_clock(rq);
			unthrottle_cfs_rq(cfs_rq);
		}
	}

	ଯଦି (runtime_was_enabled && !runtime_enabled)
		cfs_bandwidth_usage_dec();

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର u64 tg_get_cfs_period(ସଂରଚନା task_group *tg)
{
	u64 cfs_period_us;

	cfs_period_us = ktime_to_ns(tg->cfs_bandwidth.period);
	do_div(cfs_period_us, NSEC_PER_USEC);

	ଫେରନ୍ତୁ cfs_period_us;
}

ସ୍ଥିର u64 tg_get_cfs_quota(ସଂରଚନା task_group *tg)
{
	u64 quota_us;

	ଯଦି (tg->cfs_bandwidth.quota == RUNTIME_INF)
		ଫେରନ୍ତୁ RUNTIME_INF;

	quota_us = tg->cfs_bandwidth.quota;
	do_div(quota_us, NSEC_PER_USEC);

	ଫେରନ୍ତୁ quota_us;
}

ସ୍ଥିର u64 tg_get_cfs_burst(ସଂରଚନା task_group *tg)
{
	u64 burst_us;

	burst_us = tg->cfs_bandwidth.burst;
	do_div(burst_us, NSEC_PER_USEC);

	ଫେରନ୍ତୁ burst_us;
}

ସଂରଚନା cfs_schedulable_data {
	ସଂରଚନା task_group *tg;
	u64 period, quota;
};

/*
 * normalize group quota/period to be quota/max_period
 * note: units are usecs
 */
ସ୍ଥିର u64 normalize_cfs_quota(ସଂରଚନା task_group *tg,
			       ସଂରଚନା cfs_schedulable_data *d)
{
	u64 quota, period;

	ଯଦି (tg == d->tg) {
		period = d->period;
		quota = d->quota;
	} ଅନ୍ୟଥା {
		period = tg_get_cfs_period(tg);
		quota = tg_get_cfs_quota(tg);
	}

	/* note: these should typically be equivalent */
	ଯଦି (quota == RUNTIME_INF || quota == -1)
		ଫେରନ୍ତୁ RUNTIME_INF;

	ଫେରନ୍ତୁ to_ratio(period, quota);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tg_cfs_schedulable_down(ସଂରଚନା task_group *tg, ଶୂନ୍ୟ *data)
{
	ସଂରଚନା cfs_schedulable_data *d = data;
	ସଂରଚନା cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
	s64 quota = 0, parent_quota = -1;

	ଯଦି (!tg->parent) {
		quota = RUNTIME_INF;
	} ଅନ୍ୟଥା {
		ସଂରଚନା cfs_bandwidth *parent_b = &tg->parent->cfs_bandwidth;

		quota = normalize_cfs_quota(tg, d);
		parent_quota = parent_b->hierarchical_quota;

		/*
		 * Ensure max(child_quota) <= parent_quota.  On cgroup2,
		 * always take the non-RUNTIME_INF min.  On cgroup1, only
		 * inherit when no limit is set. In both cases this is used
		 * by the scheduler to determine if a given CFS task has a
		 * bandwidth constraint at some higher level.
		 */
		ଯଦି (cgroup_subsys_on_dfl(cpu_cgrp_subsys)) {
			ଯଦି (quota == RUNTIME_INF)
				quota = parent_quota;
			ଅନ୍ୟଥା ଯଦି (parent_quota != RUNTIME_INF)
				quota = min(quota, parent_quota);
		} ଅନ୍ୟଥା {
			ଯଦି (quota == RUNTIME_INF)
				quota = parent_quota;
			ଅନ୍ୟଥା ଯଦି (parent_quota != RUNTIME_INF && quota > parent_quota)
				ଫେରନ୍ତୁ -EINVAL;
		}
	}
	cfs_b->hierarchical_quota = quota;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __cfs_schedulable(ସଂରଚନା task_group *tg, u64 period, u64 quota)
{
	ସଂରଚନା cfs_schedulable_data data = {
		.tg = tg,
		.period = period,
		.quota = quota,
	};

	ଯଦି (quota != RUNTIME_INF) {
		do_div(data.period, NSEC_PER_USEC);
		do_div(data.quota, NSEC_PER_USEC);
	}

	guard(rcu)();
	ଫେରନ୍ତୁ walk_tg_ବୃକ୍ଷ(tg_cfs_schedulable_down, tg_nop, &data);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_cfs_stat_show(ସଂରଚନା seq_file *sf, ଶୂନ୍ୟ *v)
{
	ସଂରଚନା task_group *tg = css_tg(seq_css(sf));
	ସଂରଚନା cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;

	seq_printf(sf, "nr_periods %d\n", cfs_b->nr_periods);
	seq_printf(sf, "nr_throttled %d\n", cfs_b->nr_throttled);
	seq_printf(sf, "throttled_time %llu\n", cfs_b->throttled_time);

	ଯଦି (schedstat_enabled() && tg != &ମୂଳ_task_group) {
		ସଂରଚନା sched_statistics *stats;
		u64 ws = 0;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

		for_each_possible_cpu(i) {
			stats = __schedstats_from_se(tg_se(tg, i));
			ws += schedstat_val(stats->wait_sum);
		}

		seq_printf(sf, "wait_sum %llu\n", ws);
	}

	seq_printf(sf, "nr_bursts %d\n", cfs_b->nr_burst);
	seq_printf(sf, "burst_time %llu\n", cfs_b->burst_time);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର u64 throttled_time_self(ସଂରଚନା task_group *tg)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	u64 total = 0;

	for_each_possible_cpu(i) {
		total += READ_ONCE(tg_cfs_rq(tg, i)->throttled_clock_self_time);
	}

	ଫେରନ୍ତୁ total;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_cfs_local_stat_show(ସଂରଚନା seq_file *sf, ଶୂନ୍ୟ *v)
{
	ସଂରଚନା task_group *tg = css_tg(seq_css(sf));

	seq_printf(sf, "throttled_time %llu\n", throttled_time_self(tg));

	ଫେରନ୍ତୁ 0;
}
#endif /* CONFIG_CFS_BANDWIDTH */

#ifdef CONFIG_GROUP_SCHED_BANDWIDTH
ଅଚଳ u64 max_bw_quota_period_us = 1 * USEC_PER_SEC; /* 1s */
ସ୍ଥିର ଅଚଳ u64 min_bw_quota_period_us = 1 * USEC_PER_MSEC; /* 1ms */
/* More than 203 days if BW_SHIFT equals 20. */
ସ୍ଥିର ଅଚଳ u64 max_bw_runtime_us = MAX_BW;

ସ୍ଥିର ଶୂନ୍ୟ tg_bandwidth(ସଂରଚନା task_group *tg,
			 u64 *period_us_p, u64 *quota_us_p, u64 *burst_us_p)
{
#ifdef CONFIG_CFS_BANDWIDTH
	ଯଦି (period_us_p)
		*period_us_p = tg_get_cfs_period(tg);
	ଯଦି (quota_us_p)
		*quota_us_p = tg_get_cfs_quota(tg);
	ଯଦି (burst_us_p)
		*burst_us_p = tg_get_cfs_burst(tg);
#ଅନ୍ୟଥା /* !CONFIG_CFS_BANDWIDTH */
	ଯଦି (period_us_p)
		*period_us_p = tg->scx.bw_period_us;
	ଯଦି (quota_us_p)
		*quota_us_p = tg->scx.bw_quota_us;
	ଯଦି (burst_us_p)
		*burst_us_p = tg->scx.bw_burst_us;
#endif /* CONFIG_CFS_BANDWIDTH */
}

ସ୍ଥିର u64 cpu_period_read_u64(ସଂରଚନା cgroup_subsys_state *css,
			       ସଂରଚନା cftype *cft)
{
	u64 period_us;

	tg_bandwidth(css_tg(css), &period_us, ଖାଲି, ଖାଲି);
	ଫେରନ୍ତୁ period_us;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tg_set_bandwidth(ସଂରଚନା task_group *tg,
			    u64 period_us, u64 quota_us, u64 burst_us)
{
	ଅଚଳ u64 max_usec = U64_MAX / NSEC_PER_USEC;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	ଯଦି (tg == &ମୂଳ_task_group)
		ଫେରନ୍ତୁ -EINVAL;

	/* Values should survive translation to nsec */
	ଯଦି (period_us > max_usec ||
	    (quota_us != RUNTIME_INF && quota_us > max_usec) ||
	    burst_us > max_usec)
		ଫେରନ୍ତୁ -EINVAL;

	/*
	 * Ensure we have some amount of bandwidth every period. This is to
	 * prevent reaching a state of large arrears when throttled via
	 * entity_tick() resulting in prolonged exit starvation.
	 */
	ଯଦି (quota_us < min_bw_quota_period_us ||
	    period_us < min_bw_quota_period_us)
		ଫେରନ୍ତୁ -EINVAL;

	/*
	 * Likewise, bound things on the other side by preventing insane quota
	 * periods.  This also allows us to normalize in computing quota
	 * feasibility.
	 */
	ଯଦି (period_us > max_bw_quota_period_us)
		ଫେରନ୍ତୁ -EINVAL;

	/*
	 * Bound quota to defend quota against overflow during bandwidth shift.
	 */
	ଯଦି (quota_us != RUNTIME_INF && quota_us > max_bw_runtime_us)
		ଫେରନ୍ତୁ -EINVAL;

	ଯଦି (quota_us != RUNTIME_INF && (burst_us > quota_us ||
					burst_us + quota_us > max_bw_runtime_us))
		ଫେରନ୍ତୁ -EINVAL;

#ifdef CONFIG_CFS_BANDWIDTH
	ret = tg_set_cfs_bandwidth(tg, period_us, quota_us, burst_us);
#endif /* CONFIG_CFS_BANDWIDTH */
	ଯଦି (!ret)
		scx_group_set_bandwidth(tg, period_us, quota_us, burst_us);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର s64 cpu_quota_read_s64(ସଂରଚନା cgroup_subsys_state *css,
			      ସଂରଚନା cftype *cft)
{
	u64 quota_us;

	tg_bandwidth(css_tg(css), ଖାଲି, &quota_us, ଖାଲି);
	ଫେରନ୍ତୁ quota_us;	/* (s64)RUNTIME_INF becomes -1 */
}

ସ୍ଥିର u64 cpu_burst_read_u64(ସଂରଚନା cgroup_subsys_state *css,
			      ସଂରଚନା cftype *cft)
{
	u64 burst_us;

	tg_bandwidth(css_tg(css), ଖାଲି, ଖାଲି, &burst_us);
	ଫେରନ୍ତୁ burst_us;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_period_write_u64(ସଂରଚନା cgroup_subsys_state *css,
				ସଂରଚନା cftype *cftype, u64 period_us)
{
	ସଂରଚନା task_group *tg = css_tg(css);
	u64 quota_us, burst_us;

	tg_bandwidth(tg, ଖାଲି, &quota_us, &burst_us);
	ଫେରନ୍ତୁ tg_set_bandwidth(tg, period_us, quota_us, burst_us);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_quota_write_s64(ସଂରଚନା cgroup_subsys_state *css,
			       ସଂରଚନା cftype *cftype, s64 quota_us)
{
	ସଂରଚନା task_group *tg = css_tg(css);
	u64 period_us, burst_us;

	ଯଦି (quota_us < 0)
		quota_us = RUNTIME_INF;

	tg_bandwidth(tg, &period_us, ଖାଲି, &burst_us);
	ଫେରନ୍ତୁ tg_set_bandwidth(tg, period_us, quota_us, burst_us);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_burst_write_u64(ସଂରଚନା cgroup_subsys_state *css,
			       ସଂରଚନା cftype *cftype, u64 burst_us)
{
	ସଂରଚନା task_group *tg = css_tg(css);
	u64 period_us, quota_us;

	tg_bandwidth(tg, &period_us, &quota_us, ଖାଲି);
	ଫେରନ୍ତୁ tg_set_bandwidth(tg, period_us, quota_us, burst_us);
}
#endif /* CONFIG_GROUP_SCHED_BANDWIDTH */

#ifdef CONFIG_RT_GROUP_SCHED
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_rt_runtime_write(ସଂରଚନା cgroup_subsys_state *css,
				ସଂରଚନା cftype *cft, s64 val)
{
	ଫେରନ୍ତୁ sched_group_set_rt_runtime(css_tg(css), val);
}

ସ୍ଥିର s64 cpu_rt_runtime_read(ସଂରଚନା cgroup_subsys_state *css,
			       ସଂରଚନା cftype *cft)
{
	ଫେରନ୍ତୁ sched_group_rt_runtime(css_tg(css));
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_rt_period_write_uint(ସଂରଚନା cgroup_subsys_state *css,
				    ସଂରଚନା cftype *cftype, u64 rt_period_us)
{
	ଫେରନ୍ତୁ sched_group_set_rt_period(css_tg(css), rt_period_us);
}

ସ୍ଥିର u64 cpu_rt_period_read_uint(ସଂରଚନା cgroup_subsys_state *css,
				   ସଂରଚନା cftype *cft)
{
	ଫେରନ୍ତୁ sched_group_rt_period(css_tg(css));
}
#endif /* CONFIG_RT_GROUP_SCHED */

#ifdef CONFIG_GROUP_SCHED_WEIGHT
ସ୍ଥିର s64 cpu_idle_read_s64(ସଂରଚନା cgroup_subsys_state *css,
			       ସଂରଚନା cftype *cft)
{
	ଫେରନ୍ତୁ css_tg(css)->idle;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_idle_write_s64(ସଂରଚନା cgroup_subsys_state *css,
				ସଂରଚନା cftype *cft, s64 idle)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ret = sched_group_set_idle(css_tg(css), idle);
	ଯଦି (!ret)
		scx_group_set_idle(css_tg(css), idle);
	ଫେରନ୍ତୁ ret;
}
#endif /* CONFIG_GROUP_SCHED_WEIGHT */

ସ୍ଥିର ସଂରଚନା cftype cpu_legacy_files[] = {
#ifdef CONFIG_GROUP_SCHED_WEIGHT
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
#endif
#ifdef CONFIG_GROUP_SCHED_BANDWIDTH
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
		.read_u64 = cpu_burst_read_u64,
		.write_u64 = cpu_burst_write_u64,
	},
#endif
#ifdef CONFIG_CFS_BANDWIDTH
	{
		.name = "stat",
		.seq_show = cpu_cfs_stat_show,
	},
	{
		.name = "stat.local",
		.seq_show = cpu_cfs_local_stat_show,
	},
#endif
#ifdef CONFIG_UCLAMP_TASK_GROUP
	{
		.name = "uclamp.min",
		.flags = CFTYPE_NOT_ON_ମୂଳ,
		.seq_show = cpu_uclamp_min_show,
		.ଲିଖ = cpu_uclamp_min_write,
	},
	{
		.name = "uclamp.max",
		.flags = CFTYPE_NOT_ON_ମୂଳ,
		.seq_show = cpu_uclamp_max_show,
		.ଲିଖ = cpu_uclamp_max_write,
	},
#endif
	{ }	/* Terminate */
};

#ifdef CONFIG_RT_GROUP_SCHED
ସ୍ଥିର ସଂରଚନା cftype rt_group_files[] = {
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

# ifdef CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED
DEFINE_STATIC_ଚାବି_FALSE(rt_group_sched);
# ଅନ୍ୟଥା
DEFINE_STATIC_ଚାବି_TRUE(rt_group_sched);
# endif

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init setup_rt_group_sched(ଅକ୍ଷର *str)
{
	long val;

	ଯଦି (kstrtol(str, 0, &val) || val < 0 || val > 1) {
		pr_warn("Unable to set rt_group_sched\n");
		ଫେରନ୍ତୁ 1;
	}
	ଯଦି (val)
		static_branch_enable(&rt_group_sched);
	ଅନ୍ୟଥା
		static_branch_disable(&rt_group_sched);

	ଫେରନ୍ତୁ 1;
}
__setup("rt_group_sched=", setup_rt_group_sched);

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init cpu_rt_group_init(ଶୂନ୍ୟ)
{
	ଯଦି (!rt_group_sched_enabled())
		ଫେରନ୍ତୁ 0;

	WARN_ON(cgroup_add_legacy_cftypes(&cpu_cgrp_subsys, rt_group_files));
	ଫେରନ୍ତୁ 0;
}
subsys_initcall(cpu_rt_group_init);
#endif /* CONFIG_RT_GROUP_SCHED */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_extra_stat_show(ସଂରଚନା seq_file *sf,
			       ସଂରଚନା cgroup_subsys_state *css)
{
#ifdef CONFIG_CFS_BANDWIDTH
	{
		ସଂରଚନା task_group *tg = css_tg(css);
		ସଂରଚନା cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
		u64 throttled_usec, burst_usec;

		throttled_usec = cfs_b->throttled_time;
		do_div(throttled_usec, NSEC_PER_USEC);
		burst_usec = cfs_b->burst_time;
		do_div(burst_usec, NSEC_PER_USEC);

		seq_printf(sf, "nr_periods %d\n"
			   "nr_throttled %d\n"
			   "throttled_usec %llu\n"
			   "nr_bursts %d\n"
			   "burst_usec %llu\n",
			   cfs_b->nr_periods, cfs_b->nr_throttled,
			   throttled_usec, cfs_b->nr_burst, burst_usec);
	}
#endif /* CONFIG_CFS_BANDWIDTH */
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_local_stat_show(ସଂରଚନା seq_file *sf,
			       ସଂରଚନା cgroup_subsys_state *css)
{
#ifdef CONFIG_CFS_BANDWIDTH
	{
		ସଂରଚନା task_group *tg = css_tg(css);
		u64 throttled_self_usec;

		throttled_self_usec = throttled_time_self(tg);
		do_div(throttled_self_usec, NSEC_PER_USEC);

		seq_printf(sf, "throttled_usec %llu\n",
			   throttled_self_usec);
	}
#endif
	ଫେରନ୍ତୁ 0;
}

#ifdef CONFIG_GROUP_SCHED_WEIGHT

ସ୍ଥିର u64 cpu_weight_read_u64(ସଂରଚନା cgroup_subsys_state *css,
			       ସଂରଚନା cftype *cft)
{
	ଫେରନ୍ତୁ sched_weight_to_cgroup(tg_weight(css_tg(css)));
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_weight_write_u64(ସଂରଚନା cgroup_subsys_state *css,
				ସଂରଚନା cftype *cft, u64 cgrp_weight)
{
	unsigned long weight;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ଯଦି (cgrp_weight < CGROUP_WEIGHT_MIN || cgrp_weight > CGROUP_WEIGHT_MAX)
		ଫେରନ୍ତୁ -ERANGE;

	weight = sched_weight_from_cgroup(cgrp_weight);

	ret = sched_group_set_shares(css_tg(css), scale_load(weight));
	ଯଦି (!ret)
		scx_group_set_weight(css_tg(css), cgrp_weight);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର s64 cpu_weight_nice_read_s64(ସଂରଚନା cgroup_subsys_state *css,
				    ସଂରଚନା cftype *cft)
{
	unsigned long weight = tg_weight(css_tg(css));
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_delta = INT_MAX;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା prio, delta;

	/* find the closest nice value to the current weight */
	ପାଇଁ (prio = 0; prio < ARRAY_SIZE(sched_prio_to_weight); prio++) {
		delta = abs(sched_prio_to_weight[prio] - weight);
		ଯଦି (delta >= last_delta)
			ଭାଙ୍ଗ;
		last_delta = delta;
	}

	ଫେରନ୍ତୁ PRIO_TO_NICE(prio - 1 + MAX_RT_PRIO);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_weight_nice_write_s64(ସଂରଚନା cgroup_subsys_state *css,
				     ସଂରଚନା cftype *cft, s64 nice)
{
	unsigned long weight;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx, ret;

	ଯଦି (nice < MIN_NICE || nice > MAX_NICE)
		ଫେରନ୍ତୁ -ERANGE;

	idx = NICE_TO_PRIO(nice) - MAX_RT_PRIO;
	idx = array_index_nospec(idx, 40);
	weight = sched_prio_to_weight[idx];

	ret = sched_group_set_shares(css_tg(css), scale_load(weight));
	ଯଦି (!ret)
		scx_group_set_weight(css_tg(css),
				     sched_weight_to_cgroup(weight));
	ଫେରନ୍ତୁ ret;
}
#endif /* CONFIG_GROUP_SCHED_WEIGHT */

ସ୍ଥିର ଶୂନ୍ୟ __maybe_unused cpu_period_quota_print(ସଂରଚନା seq_file *sf,
						  long period, long quota)
{
	ଯଦି (quota < 0)
		seq_puts(sf, "max");
	ଅନ୍ୟଥା
		seq_printf(sf, "%ld", quota);

	seq_printf(sf, " %ld\n", period);
}

/* caller should put the current value in *@periodp before calling */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __maybe_unused cpu_period_quota_parse(ଅକ୍ଷର *buf, u64 *period_us_p,
						 u64 *quota_us_p)
{
	ଅକ୍ଷର tok[21];	/* U64_MAX */

	ଯଦି (sscanf(buf, "%20s %llu", tok, period_us_p) < 1)
		ଫେରନ୍ତୁ -EINVAL;

	ଯଦି (sscanf(tok, "%llu", quota_us_p) < 1) {
		ଯଦି (!strcmp(tok, "max"))
			*quota_us_p = RUNTIME_INF;
		ଅନ୍ୟଥା
			ଫେରନ୍ତୁ -EINVAL;
	}

	ଫେରନ୍ତୁ 0;
}

#ifdef CONFIG_GROUP_SCHED_BANDWIDTH
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_max_show(ସଂରଚନା seq_file *sf, ଶୂନ୍ୟ *v)
{
	ସଂରଚନା task_group *tg = css_tg(seq_css(sf));
	u64 period_us, quota_us;

	tg_bandwidth(tg, &period_us, &quota_us, ଖାଲି);
	cpu_period_quota_print(sf, period_us, quota_us);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ssize_t cpu_max_write(ସଂରଚନା kernfs_open_file *of,
			     ଅକ୍ଷର *buf, size_t nbytes, loff_t off)
{
	ସଂରଚନା task_group *tg = css_tg(of_css(of));
	u64 period_us, quota_us, burst_us;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	tg_bandwidth(tg, &period_us, ଖାଲି, &burst_us);
	ret = cpu_period_quota_parse(buf, &period_us, &quota_us);
	ଯଦି (!ret)
		ret = tg_set_bandwidth(tg, period_us, quota_us, burst_us);
	ଫେରନ୍ତୁ ret ?: nbytes;
}
#endif /* CONFIG_CFS_BANDWIDTH */

ସ୍ଥିର ସଂରଚନା cftype cpu_files[] = {
#ifdef CONFIG_GROUP_SCHED_WEIGHT
	{
		.name = "weight",
		.flags = CFTYPE_NOT_ON_ମୂଳ,
		.read_u64 = cpu_weight_read_u64,
		.write_u64 = cpu_weight_write_u64,
	},
	{
		.name = "weight.nice",
		.flags = CFTYPE_NOT_ON_ମୂଳ,
		.read_s64 = cpu_weight_nice_read_s64,
		.write_s64 = cpu_weight_nice_write_s64,
	},
	{
		.name = "idle",
		.flags = CFTYPE_NOT_ON_ମୂଳ,
		.read_s64 = cpu_idle_read_s64,
		.write_s64 = cpu_idle_write_s64,
	},
#endif
#ifdef CONFIG_GROUP_SCHED_BANDWIDTH
	{
		.name = "max",
		.flags = CFTYPE_NOT_ON_ମୂଳ,
		.seq_show = cpu_max_show,
		.ଲିଖ = cpu_max_write,
	},
	{
		.name = "max.burst",
		.flags = CFTYPE_NOT_ON_ମୂଳ,
		.read_u64 = cpu_burst_read_u64,
		.write_u64 = cpu_burst_write_u64,
	},
#endif /* CONFIG_CFS_BANDWIDTH */
#ifdef CONFIG_UCLAMP_TASK_GROUP
	{
		.name = "uclamp.min",
		.flags = CFTYPE_NOT_ON_ମୂଳ,
		.seq_show = cpu_uclamp_min_show,
		.ଲିଖ = cpu_uclamp_min_write,
	},
	{
		.name = "uclamp.max",
		.flags = CFTYPE_NOT_ON_ମୂଳ,
		.seq_show = cpu_uclamp_max_show,
		.ଲିଖ = cpu_uclamp_max_write,
	},
#endif /* CONFIG_UCLAMP_TASK_GROUP */
	{ }	/* terminate */
};

ସଂରଚନା cgroup_subsys cpu_cgrp_subsys = {
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

#endif /* CONFIG_CGROUP_SCHED */

ଶୂନ୍ୟ dump_cpu_task(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଯଦି (in_hardirq() && cpu == smp_processor_id()) {
		ସଂରଚନା pt_regs *regs;

		regs = get_irq_regs();
		ଯଦି (regs) {
			show_regs(regs);
			ଫେରନ୍ତୁ;
		}
	}

	ଯଦି (trigger_single_cpu_backtrace(cpu))
		ଫେରନ୍ତୁ;

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
ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_prio_to_weight[40] = {
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
ଅଚଳ u32 sched_prio_to_wmult[40] = {
 /* -20 */     48388,     59856,     76040,     92818,    118348,
 /* -15 */    147320,    184698,    229616,    287308,    360437,
 /* -10 */    449829,    563644,    704093,    875809,   1099582,
 /*  -5 */   1376151,   1717300,   2157191,   2708050,   3363326,
 /*   0 */   4194304,   5237765,   6557202,   8165337,  10153587,
 /*   5 */  12820798,  15790321,  19976592,  24970740,  31350126,
 /*  10 */  39045157,  49367440,  61356676,  76695844,  95443717,
 /*  15 */ 119304647, 148102320, 186737708, 238609294, 286331153,
};

ଶୂନ୍ୟ call_trace_sched_update_nr_running(ସଂରଚନା rq *rq, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2)
{
        trace_sched_update_nr_running_tp(rq, ଗଣନା_2);
}

#ifdef CONFIG_SCHED_MM_CID
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
ସ୍ଥିର ଶୂନ୍ୟ __mm_update_max_cids(ସଂରଚନା mm_mm_cid *mc)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt_cids, max_cids;

	/* Calculate the new optimal constraint */
	opt_cids = min(mc->nr_cpus_allowed, mc->users);

	/* Adjust the maximum CIDs to +25% limited by the number of possible CPUs */
	max_cids = min(opt_cids + (opt_cids / 4), ଅଙ୍କ_possible_cpus());
	WRITE_ONCE(mc->max_cids, max_cids);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mm_cid_calc_pcpu_thrs(ସଂରଚନା mm_mm_cid *mc)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt_cids;

	opt_cids = min(mc->nr_cpus_allowed, mc->users);
	/* Has to be at least 1 because 0 indicates PCPU mode off */
	ଫେରନ୍ତୁ max(min(opt_cids - opt_cids / 4, ଅଙ୍କ_possible_cpus() / 2), 1);
}

ସ୍ଥିର bool mm_update_max_cids(ସଂରଚନା mm_struct *mm)
{
	ସଂରଚନା mm_mm_cid *mc = &mm->mm_cid;
	bool percpu = cid_on_cpu(mc->mode);

	lockdep_assert_held(&mm->mm_cid.lock);

	/* Clear deferred mode switch flag. A change is handled by the caller */
	mc->update_deferred = false;
	__mm_update_max_cids(mc);

	/* Check whether owner mode must be changed */
	ଯଦି (!percpu) {
		/* Enable per CPU mode when the number of users is above max_cids */
		ଯଦି (mc->users > mc->max_cids)
			mc->pcpu_thrs = mm_cid_calc_pcpu_thrs(mc);
	} ଅନ୍ୟଥା {
		/* Switch back to per task if user count under threshold */
		ଯଦି (mc->users < mc->pcpu_thrs)
			mc->pcpu_thrs = 0;
	}

	/* Mode change required? */
	ଯଦି (percpu == !!mc->pcpu_thrs)
		ଫେରନ୍ତୁ false;

	/* Flip the mode and set the transition flag to bridge the transfer */
	WRITE_ONCE(mc->mode, mc->mode ^ (MM_CID_TRANSIT | MM_CID_ONCPU));
	/*
	 * Order the store against the subsequent fixups so that
	 * acquire(rq::lock) cannot be reordered by the CPU before the
	 * store.
	 */
	smp_mb();
	ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ mm_update_cpus_allowed(ସଂରଚନା mm_struct *mm, ଅଚଳ ସଂରଚନା cpumask *affmsk)
{
	ସଂରଚନା cpumask *mm_allowed;
	ସଂରଚନା mm_mm_cid *mc;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା weight;

	ଯଦି (!mm || !READ_ONCE(mm->mm_cid.users))
		ଫେରନ୍ତୁ;
	/*
	 * mm::mm_cid::mm_cpus_allowed is the superset of each threads
	 * allowed CPUs mask which means it can only grow.
	 */
	mc = &mm->mm_cid;
	guard(raw_spinlock)(&mc->lock);
	mm_allowed = mm_cpus_allowed(mm);
	weight = cpumask_weighted_or(mm_allowed, mm_allowed, affmsk);
	ଯଦି (weight == mc->nr_cpus_allowed)
		ଫେରନ୍ତୁ;

	WRITE_ONCE(mc->nr_cpus_allowed, weight);
	__mm_update_max_cids(mc);
	ଯଦି (!cid_on_cpu(mc->mode))
		ଫେରନ୍ତୁ;

	/* Adjust the threshold to the wider set */
	mc->pcpu_thrs = mm_cid_calc_pcpu_thrs(mc);
	/* Switch back to per task mode? */
	ଯଦି (mc->users >= mc->pcpu_thrs)
		ଫେରନ୍ତୁ;

	/* Don't queue twice */
	ଯଦି (mc->update_deferred)
		ଫେରନ୍ତୁ;

	/* Queue the irq work, which schedules the real work */
	mc->update_deferred = true;
	irq_work_queue(&mc->irq_work);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ mm_cid_complete_transit(ସଂରଚନା mm_struct *mm, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode)
{
	/*
	 * Ensure that the store removing the TRANSIT bit cannot be
	 * reordered by the CPU before the fixups have been completed.
	 */
	smp_mb();
	WRITE_ONCE(mm->mm_cid.mode, mode);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ mm_cid_transit_to_task(ସଂରଚନା task_struct *t, ସଂରଚନା mm_cid_pcpu *pcp)
{
	ଯଦି (cid_on_cpu(t->mm_cid.cid)) {
		unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cid = cpu_cid_to_cid(t->mm_cid.cid);

		t->mm_cid.cid = cid_to_transit_cid(cid);
		pcp->cid = t->mm_cid.cid;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ mm_cid_fixup_cpus_to_tasks(ସଂରଚନା mm_struct *mm)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

	/* Walk the CPUs and fixup all stale CIDs */
	for_each_possible_cpu(cpu) {
		ସଂରଚନା mm_cid_pcpu *pcp = per_cpu_ptr(mm->mm_cid.pcpu, cpu);
		ସଂରଚନା rq *rq = cpu_rq(cpu);

		/* Remote access to mm::mm_cid::pcpu requires rq_lock */
		guard(rq_lock_irq)(rq);
		/* Is the CID still owned by the CPU? */
		ଯଦି (cid_on_cpu(pcp->cid)) {
			/*
			 * If rq->curr has @mm, transfer it with the
			 * transition bit set. Otherwise drop it.
			 */
			ଯଦି (rq->curr->mm == mm && rq->curr->mm_cid.active)
				mm_cid_transit_to_task(rq->curr, pcp);
			ଅନ୍ୟଥା
				mm_drop_cid_on_cpu(mm, pcp);

		} ଅନ୍ୟଥା ଯଦି (rq->curr->mm == mm && rq->curr->mm_cid.active) {
			unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cid = rq->curr->mm_cid.cid;

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
			ଯଦି (cid != MM_CID_UNSET && !cid_in_transit(cid)) {
				cid = cid_to_transit_cid(cid);
				rq->curr->mm_cid.cid = cid;
				pcp->cid = cid;
			}
		}
	}
	mm_cid_complete_transit(mm, 0);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ mm_cid_transit_to_cpu(ସଂରଚନା task_struct *t, ସଂରଚନା mm_cid_pcpu *pcp)
{
	ଯଦି (cid_on_task(t->mm_cid.cid)) {
		t->mm_cid.cid = cid_to_transit_cid(t->mm_cid.cid);
		pcp->cid = t->mm_cid.cid;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ mm_cid_fixup_task_to_cpu(ସଂରଚନା task_struct *t, ସଂରଚନା mm_struct *mm)
{
	/* Remote access to mm::mm_cid::pcpu requires rq_lock */
	guard(task_rq_lock)(t);
	ଯଦି (cid_on_task(t->mm_cid.cid)) {
		/* If running on the CPU, put the CID in transit mode, otherwise drop it */
		ଯଦି (task_rq(t)->curr == t)
			mm_cid_transit_to_cpu(t, per_cpu_ptr(mm->mm_cid.pcpu, task_cpu(t)));
		ଅନ୍ୟଥା
			mm_unset_cid_on_task(t);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ mm_cid_fixup_tasks_to_cpus(ଶୂନ୍ୟ)
{
	ସଂରଚନା mm_struct *mm = current->mm;
	ସଂରଚନା task_struct *t;

	lockdep_assert_held(&mm->mm_cid.mutex);

	hlist_for_each_entry(t, &mm->mm_cid.user_list, mm_cid.ଗଣ୍ଠି) {
		/* Current has already transferred before invoking the fixup. */
		ଯଦି (t != current)
			mm_cid_fixup_task_to_cpu(t, mm);
	}

	mm_cid_complete_transit(mm, MM_CID_ONCPU);
}

ସ୍ଥିର bool sched_mm_cid_add_user(ସଂରଚନା task_struct *t, ସଂରଚନା mm_struct *mm)
{
	lockdep_assert_held(&mm->mm_cid.lock);

	t->mm_cid.active = 1;
	hlist_add_head(&t->mm_cid.ଗଣ୍ଠି, &mm->mm_cid.user_list);
	mm->mm_cid.users++;
	ଫେରନ୍ତୁ mm_update_max_cids(mm);
}

ସ୍ଥିର ଶୂନ୍ୟ sched_mm_cid_fork(ସଂରଚନା task_struct *t)
{
	ସଂରଚନା mm_struct *mm = t->mm;
	bool percpu;

	ଯଦି (!mm)
		ଫେରନ୍ତୁ;

	WARN_ON_ONCE(t->mm_cid.cid != MM_CID_UNSET);

	guard(mutex)(&mm->mm_cid.mutex);
	scoped_guard(raw_spinlock_irq, &mm->mm_cid.lock) {
		ସଂରଚନା mm_cid_pcpu *pcp = this_cpu_ptr(mm->mm_cid.pcpu);

		/* First user ? */
		ଯଦି (!mm->mm_cid.users) {
			sched_mm_cid_add_user(t, mm);
			t->mm_cid.cid = mm_get_cid(mm);
			/* Required for execve() */
			pcp->cid = t->mm_cid.cid;
			ଫେରନ୍ତୁ;
		}

		ଯଦି (!sched_mm_cid_add_user(t, mm)) {
			ଯଦି (!cid_on_cpu(mm->mm_cid.mode))
				t->mm_cid.cid = mm_get_cid(mm);
			ଫେରନ୍ତୁ;
		}

		/* Handle the mode change and transfer current's CID */
		percpu = cid_on_cpu(mm->mm_cid.mode);
		ଯଦି (!percpu)
			mm_cid_transit_to_task(current, pcp);
		ଅନ୍ୟଥା
			mm_cid_transit_to_cpu(current, pcp);
	}

	ଯଦି (percpu) {
		mm_cid_fixup_tasks_to_cpus();
	} ଅନ୍ୟଥା {
		mm_cid_fixup_cpus_to_tasks(mm);
		t->mm_cid.cid = mm_get_cid(mm);
	}
}

ସ୍ଥିର bool sched_mm_cid_remove_user(ସଂରଚନା task_struct *t)
{
	lockdep_assert_held(&t->mm->mm_cid.lock);

	t->mm_cid.active = 0;
	/* Clear the transition bit */
	t->mm_cid.cid = cid_from_transit_cid(t->mm_cid.cid);
	mm_unset_cid_on_task(t);
	hlist_del_init(&t->mm_cid.ଗଣ୍ଠି);
	t->mm->mm_cid.users--;
	ଫେରନ୍ତୁ mm_update_max_cids(t->mm);
}

ସ୍ଥିର bool __sched_mm_cid_exit(ସଂରଚନା task_struct *t)
{
	ସଂରଚନା mm_struct *mm = t->mm;

	ଯଦି (!sched_mm_cid_remove_user(t))
		ଫେରନ୍ତୁ false;
	/*
	 * Contrary to fork() this only deals with a switch back to per
	 * task mode either because the above decreased users or an
	 * affinity change increased the number of allowed CPUs and the
	 * deferred fixup did not run yet.
	 */
	ଯଦି (WARN_ON_ONCE(cid_on_cpu(mm->mm_cid.mode)))
		ଫେରନ୍ତୁ false;
	/*
	 * A failed fork(2) cleanup never gets here, so @current must have
	 * the same MM as @t. That's true for exit() and the failed
	 * pthread_create() cleanup case.
	 */
	ଯଦି (WARN_ON_ONCE(current->mm != mm))
		ଫେରନ୍ତୁ false;
	ଫେରନ୍ତୁ true;
}

/*
 * When a task exits, the MM CID held by the task is not longer required as
 * the task cannot return to user space.
 */
ଶୂନ୍ୟ sched_mm_cid_exit(ସଂରଚନା task_struct *t)
{
	ସଂରଚନା mm_struct *mm = t->mm;

	ଯଦି (!mm || !t->mm_cid.active)
		ଫେରନ୍ତୁ;
	/*
	 * Ensure that only one instance is doing MM CID operations within
	 * a MM. The common case is uncontended. The rare fixup case adds
	 * some overhead.
	 */
	scoped_guard(mutex, &mm->mm_cid.mutex) {
		/* mm_cid::mutex is sufficient to protect mm_cid::users */
		ଯଦି (likely(mm->mm_cid.users > 1)) {
			scoped_guard(raw_spinlock_irq, &mm->mm_cid.lock) {
				ଯଦି (!__sched_mm_cid_exit(t))
					ଫେରନ୍ତୁ;
				/*
				 * Mode change. The task has the CID unset
				 * already and dealt with an eventually set
				 * TRANSIT bit. If the CID is owned by the CPU
				 * then drop it.
				 */
				mm_drop_cid_on_cpu(mm, this_cpu_ptr(mm->mm_cid.pcpu));
			}
			mm_cid_fixup_cpus_to_tasks(mm);
			ଫେରନ୍ତୁ;
		}
		/* Last user */
		scoped_guard(raw_spinlock_irq, &mm->mm_cid.lock) {
			/* Required across execve() */
			ଯଦି (t == current)
				mm_cid_transit_to_task(t, this_cpu_ptr(mm->mm_cid.pcpu));
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
ଶୂନ୍ୟ sched_mm_cid_before_execve(ସଂରଚନା task_struct *t)
{
	sched_mm_cid_exit(t);
}

/* Reactivate MM CID after execve() */
ଶୂନ୍ୟ sched_mm_cid_after_execve(ସଂରଚନା task_struct *t)
{
	ଯଦି (t->mm)
		sched_mm_cid_fork(t);
}

ସ୍ଥିର ଶୂନ୍ୟ mm_cid_work_fn(ସଂରଚନା work_struct *work)
{
	ସଂରଚନା mm_struct *mm = container_of(work, ସଂରଚନା mm_struct, mm_cid.work);

	guard(mutex)(&mm->mm_cid.mutex);
	/* Did the last user task exit already? */
	ଯଦି (!mm->mm_cid.users)
		ଫେରନ୍ତୁ;

	scoped_guard(raw_spinlock_irq, &mm->mm_cid.lock) {
		/* Have fork() or exit() handled it already? */
		ଯଦି (!mm->mm_cid.update_deferred)
			ଫେରନ୍ତୁ;
		/* This clears mm_cid::update_deferred */
		ଯଦି (!mm_update_max_cids(mm))
			ଫେରନ୍ତୁ;
		/* Affinity changes can only switch back to task mode */
		ଯଦି (WARN_ON_ONCE(cid_on_cpu(mm->mm_cid.mode)))
			ଫେରନ୍ତୁ;
	}
	mm_cid_fixup_cpus_to_tasks(mm);
}

ସ୍ଥିର ଶୂନ୍ୟ mm_cid_irq_work(ସଂରଚନା irq_work *work)
{
	ସଂରଚନା mm_struct *mm = container_of(work, ସଂରଚନା mm_struct, mm_cid.irq_work);

	/*
	 * Needs to be unconditional because mm_cid::lock cannot be held
	 * when scheduling work as mm_update_cpus_allowed() nests inside
	 * rq::lock and schedule_work() might end up in wakeup...
	 */
	schedule_work(&mm->mm_cid.work);
}

ଶୂନ୍ୟ mm_init_cid(ସଂରଚନା mm_struct *mm, ସଂରଚନା task_struct *p)
{
	mm->mm_cid.max_cids = 0;
	mm->mm_cid.mode = 0;
	mm->mm_cid.nr_cpus_allowed = p->nr_cpus_allowed;
	mm->mm_cid.users = 0;
	mm->mm_cid.pcpu_thrs = 0;
	mm->mm_cid.update_deferred = 0;
	raw_spin_lock_init(&mm->mm_cid.lock);
	mutex_init(&mm->mm_cid.mutex);
	mm->mm_cid.irq_work = IRQ_WORK_INIT_HARD(mm_cid_irq_work);
	INIT_WORK(&mm->mm_cid.work, mm_cid_work_fn);
	INIT_HLIST_HEAD(&mm->mm_cid.user_list);
	cpumask_copy(mm_cpus_allowed(mm), &p->cpus_mask);
	bitmap_zero(mm_cidmask(mm), ଅଙ୍କ_possible_cpus());
}
#ଅନ୍ୟଥା /* CONFIG_SCHED_MM_CID */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ mm_update_cpus_allowed(ସଂରଚନା mm_struct *mm, ଅଚଳ ସଂରଚନା cpumask *affmsk) { }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_mm_cid_fork(ସଂରଚନା task_struct *t) { }
#endif /* !CONFIG_SCHED_MM_CID */

ସ୍ଥିର DEFINE_PER_CPU(ସଂରଚନା sched_change_ctx, sched_change_ctx);

ସଂରଚନା sched_change_ctx *sched_change_begin(ସଂରଚନା task_struct *p, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା sched_change_ctx *ctx = this_cpu_ptr(&sched_change_ctx);
	ସଂରଚନା rq *rq = task_rq(p);

	/*
	 * Must exclusively use matched flags since this is both dequeue and
	 * enqueue.
	 */
	WARN_ON_ONCE(flags & 0xFFFF0000);

	lockdep_assert_rq_held(rq);

	ଯଦି (!(flags & DEQUEUE_NOCLOCK)) {
		update_rq_clock(rq);
		flags |= DEQUEUE_NOCLOCK;
	}

	ଯଦି ((flags & DEQUEUE_CLASS) && p->sched_class->switching_from)
		p->sched_class->switching_from(rq, p);

	*ctx = (ସଂରଚନା sched_change_ctx){
		.p = p,
		.class = p->sched_class,
		.flags = flags,
		.queued = task_on_rq_queued(p),
		.running = task_current_donor(rq, p),
	};

	ଯଦି (!(flags & DEQUEUE_CLASS)) {
		ଯଦି (p->sched_class->get_prio)
			ctx->prio = p->sched_class->get_prio(rq, p);
		ଅନ୍ୟଥା
			ctx->prio = p->prio;
	}

	ଯଦି (ctx->queued)
		dequeue_task(rq, p, flags);
	ଯଦି (ctx->running)
		put_prev_task(rq, p);

	ଯଦି ((flags & DEQUEUE_CLASS) && p->sched_class->switched_from)
		p->sched_class->switched_from(rq, p);

	ଫେରନ୍ତୁ ctx;
}

ଶୂନ୍ୟ sched_change_end(ସଂରଚନା sched_change_ctx *ctx)
{
	ସଂରଚନା task_struct *p = ctx->p;
	ସଂରଚନା rq *rq = task_rq(p);

	lockdep_assert_rq_held(rq);

	/*
	 * Changing class without *QUEUE_CLASS is bad.
	 */
	WARN_ON_ONCE(p->sched_class != ctx->class && !(ctx->flags & ENQUEUE_CLASS));

	ଯଦି ((ctx->flags & ENQUEUE_CLASS) && p->sched_class->switching_to)
		p->sched_class->switching_to(rq, p);

	ଯଦି (ctx->queued)
		enqueue_task(rq, p, ctx->flags);
	ଯଦି (ctx->running)
		set_next_task(rq, p);

	ଯଦି (ctx->flags & ENQUEUE_CLASS) {
		ଯଦି (p->sched_class->switched_to)
			p->sched_class->switched_to(rq, p);

		/*
		 * Check if the task can preempt the current in case the class
		 * was upgraded. Note that none of the switch*_from() methods
		 * know the new class and none of the switch*_to() methods know
		 * the old class so we must resched here.
		 */
		ଯଦି (ctx->queued)
			wakeup_preempt(rq, p, 0);

		ଯଦି (ctx->running) {
			/*
			 * If this was a degradation in class; make sure to
			 * reschedule.
			 */
			ଯଦି (sched_class_above(ctx->class, p->sched_class))
				resched_curr(rq);
		}
	} ଅନ୍ୟଥା {
		p->sched_class->prio_changed(rq, p, ctx->prio);
	}
}

#ifdef CONFIG_PREFERRED_CPU
ସ୍ଥିର DEFINE_PER_CPU(ସଂରଚନା cpu_stop_work, npc_ଠେଲ_task_work);

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_non_preferred_cpu_ଠେଲ_stop(ଶୂନ୍ୟ *arg)
{
	ସଂରଚନା task_struct *p = arg;
	ସଂରଚନା rq *rq = this_rq();
	ସଂରଚନା rq_flags rf;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

	ଯଦି (cpu_preferred(rq->cpu)) {
		scoped_guard(rq_lock_irqsave, rq)
			rq->npc_ଠେଲ_work_pending = false;
		put_task_struct(p);
		ଫେରନ୍ତୁ 0;
	}

	scoped_guard (raw_spinlock_irq, &p->pi_lock) {
		/*
		 * select_fallback_rq() may acquire the rq lock in case of
		 * fallback. So call it before grabbing rq lock. If the task
		 * migrates to another CPU before the rq lock is acquired,
		 * subsequent validation of task's current rq will help to
		 * safely bail out.
		 */
		cpu = select_fallback_rq(rq->cpu, p);
		context_unsafe_alias(rq);
		rq_lock(rq, &rf);
		rq->npc_ଠେଲ_work_pending = false;
		update_rq_clock(rq);

		ଯଦି (task_rq(p) == rq && task_on_rq_queued(p)) {
			ସଂରଚନା rq *dest_rq = __migrate_task(rq, &rf, p, cpu);

			ଯଦି (rq != dest_rq)
				schedstat_inc(p->stats.nr_migrations_cpu_non_preferred);
			rq = dest_rq;
		}
		rq_unlock(rq, &rf);
	}

	put_task_struct(p);
	ଫେରନ୍ତୁ 0;
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
ଶୂନ୍ୟ sched_ଠେଲ_current_non_preferred_cpu(ସଂରଚନା rq *rq)
{
	ସଂରଚନା task_struct *ଠେଲ_task = rq->curr;

	scoped_guard(rq_lock, rq) {
		/* Push the task if its explicit affinity allows */
		ଯଦି (!task_can_migrate_to_preferred(ଠେଲ_task, rq->cpu))
			ଫେରନ୍ତୁ;

		/* There is already a stopper thread. Don't race with it. */
		ଯଦି (rq->npc_ଠେଲ_work_pending)
			ଫେରନ୍ତୁ;

		ଯଦି (is_migration_disabled(ଠେଲ_task))
			ଫେରନ୍ତୁ;

		rq->npc_ଠେଲ_work_pending = true;
	}

	/* sched_tick runs with interrupts disabled. */
	get_task_struct(ଠେଲ_task);
	stop_one_cpu_nowait(rq->cpu, sched_non_preferred_cpu_ଠେଲ_stop,
			    ଠେଲ_task, this_cpu_ptr(&npc_ଠେଲ_task_work));
}
#endif
