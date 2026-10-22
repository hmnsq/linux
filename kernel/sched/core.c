// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/core.c
 *
 *  Core kernel CPU scheduler code
 *
 *  Copyright (C) 1991-2002  Linus Torvalds
 *  Copyright (C) 1998-2024  Ingo Molnar, Red Hat
 */
#నిర్వచించు INSTANTIATE_EXPORTED_MIGRATE_DISABLE
#అన్తర్భావ <linux/sched.h>
#అన్తర్భావ <linux/highmem.h>
#అన్తర్భావ <linux/hrtimer_api.h>
#అన్తర్భావ <linux/ktime_api.h>
#అన్తర్భావ <linux/sched/signal.h>
#అన్తర్భావ <linux/syscalls_api.h>
#అన్తర్భావ <linux/debug_locks.h>
#అన్తర్భావ <linux/prefetch.h>
#అన్తర్భావ <linux/capability.h>
#అన్తర్భావ <linux/pgtable_api.h>
#అన్తర్భావ <linux/wait_bit.h>
#అన్తర్భావ <linux/jiffies.h>
#అన్తర్భావ <linux/spinlock_api.h>
#అన్తర్భావ <linux/cpumask_api.h>
#అన్తర్భావ <linux/lockdep_api.h>
#అన్తర్భావ <linux/hardirq.h>
#అన్తర్భావ <linux/softirq.h>
#అన్తర్భావ <linux/refcount_api.h>
#అన్తర్భావ <linux/topology.h>
#అన్తర్భావ <linux/sched/clock.h>
#అన్తర్భావ <linux/sched/cond_resched.h>
#అన్తర్భావ <linux/sched/cputime.h>
#అన్తర్భావ <linux/sched/debug.h>
#అన్తర్భావ <linux/sched/hotplug.h>
#అన్తర్భావ <linux/sched/init.h>
#అన్తర్భావ <linux/sched/isolation.h>
#అన్తర్భావ <linux/sched/loadavg.h>
#అన్తర్భావ <linux/sched/mm.h>
#అన్తర్భావ <linux/sched/nohz.h>
#అన్తర్భావ <linux/sched/rseq_api.h>
#అన్తర్భావ <linux/sched/rt.h>

#అన్తర్భావ <linux/context_tracking.h>
#అన్తర్భావ <linux/cpuset.h>
#అన్తర్భావ <linux/delayacct.h>
#అన్తర్భావ <linux/init_task.h>
#అన్తర్భావ <linux/interrupt.h>
#అన్తర్భావ <linux/ioprio.h>
#అన్తర్భావ <linux/kallsyms.h>
#అన్తర్భావ <linux/kcov.h>
#అన్తర్భావ <linux/kprobes.h>
#అన్తర్భావ <linux/llist_api.h>
#అన్తర్భావ <linux/mmu_context.h>
#అన్తర్భావ <linux/mmzone.h>
#అన్తర్భావ <linux/mutex_api.h>
#అన్తర్భావ <linux/nmi.h>
#అన్తర్భావ <linux/nospec.h>
#అన్తర్భావ <linux/perf_event_api.h>
#అన్తర్భావ <linux/profile.h>
#అన్తర్భావ <linux/psi.h>
#అన్తర్భావ <linux/rcuwait_api.h>
#అన్తర్భావ <linux/rseq.h>
#అన్తర్భావ <linux/sched/wake_q.h>
#అన్తర్భావ <linux/scs.h>
#అన్తర్భావ <linux/slab.h>
#అన్తర్భావ <linux/syscalls.h>
#అన్తర్భావ <linux/vtime.h>
#అన్తర్భావ <linux/wait_api.h>
#అన్తర్భావ <linux/workqueue_api.h>
#అన్తర్భావ <linux/livepatch_sched.h>

#నిర్వచితమైతే CONFIG_PREEMPT_DYNAMIC
# ifdef CONFIG_GENERIC_IRQ_నమోదు
#  include <linux/irq-entry-common.h>
# endif
#అయితే_ముగింపు

#అన్తర్భావ <uapi/linux/sched/types.h>

#అన్తర్భావ <asm/irq_regs.h>
#అన్తర్భావ <asm/switch_to.h>
#అన్తర్భావ <asm/tlb.h>

#నిర్వచించు CREATE_TRACE_POINTS
#అన్తర్భావ <linux/sched/rseq_api.h>
#అన్తర్భావ <trace/events/sched.h>
#అన్తర్భావ <trace/events/ipi.h>
#నిర్వచనం_రద్దు CREATE_TRACE_POINTS

#అన్తర్భావ "sched.h"
#అన్తర్భావ "stats.h"

#అన్తర్భావ "autogroup.h"
#అన్తర్భావ "pelt.h"
#అన్తర్భావ "smp.h"

#అన్తర్భావ "../workqueue_internal.h"
#అన్తర్భావ "../../io_uring/io-wq.h"
#అన్తర్భావ "../smpboot.h"
#అన్తర్భావ "../locking/mutex.h"

EXPORT_TRACEPOINT_చిహ్నం_GPL(ipi_send_cpu);
EXPORT_TRACEPOINT_చిహ్నం_GPL(ipi_send_cpumask);

/*
 * Export tracepoints that act as a bare tracehook (ie: have no trace event
 * associated with them) to allow external modules to probe them.
 */
EXPORT_TRACEPOINT_చిహ్నం_GPL(pelt_cfs_tp);
EXPORT_TRACEPOINT_చిహ్నం_GPL(pelt_rt_tp);
EXPORT_TRACEPOINT_చిహ్నం_GPL(pelt_dl_tp);
EXPORT_TRACEPOINT_చిహ్నం_GPL(pelt_irq_tp);
EXPORT_TRACEPOINT_చిహ్నం_GPL(pelt_se_tp);
EXPORT_TRACEPOINT_చిహ్నం_GPL(pelt_hw_tp);
EXPORT_TRACEPOINT_చిహ్నం_GPL(sched_cpu_సామర్థ్యం_tp);
EXPORT_TRACEPOINT_చిహ్నం_GPL(sched_overutilized_tp);
EXPORT_TRACEPOINT_చిహ్నం_GPL(sched_util_est_cfs_tp);
EXPORT_TRACEPOINT_చిహ్నం_GPL(sched_util_est_se_tp);
EXPORT_TRACEPOINT_చిహ్నం_GPL(sched_update_nr_running_tp);
EXPORT_TRACEPOINT_చిహ్నం_GPL(sched_compute_energy_tp);
EXPORT_TRACEPOINT_చిహ్నం_GPL(sched_నమోదు_tp);
EXPORT_TRACEPOINT_చిహ్నం_GPL(sched_exit_tp);
EXPORT_TRACEPOINT_చిహ్నం_GPL(sched_సమితి_need_resched_tp);
EXPORT_TRACEPOINT_చిహ్నం_GPL(sched_dl_throttle_tp);
EXPORT_TRACEPOINT_చిహ్నం_GPL(sched_dl_replenish_tp);
EXPORT_TRACEPOINT_చిహ్నం_GPL(sched_dl_update_tp);
EXPORT_TRACEPOINT_చిహ్నం_GPL(sched_dl_server_ప్రారంభం_tp);
EXPORT_TRACEPOINT_చిహ్నం_GPL(sched_dl_server_stop_tp);

DEFINE_చొప్పున_CPU_SHARED_ALIGNED(నిర్మాణం rq, runqueues);
DEFINE_చొప్పున_CPU(నిర్మాణం rnd_state, sched_rnd_state);

#నిర్వచితమైతే CONFIG_SCHED_PROXY_EXEC
DEFINE_STATIC_కీలకం_TRUE(__sched_proxy_exec);
స్థిర పూర్ణాంకం __init అమరిక_proxy_exec(అక్షరం *str)
{
	bool proxy_enable = true;

	ఒకవేళ (*str && kstrtobool(str + 1, &proxy_enable)) {
		pr_warn("Unable to parse sched_proxy_exec=\n");
		తిరిగి 0;
	}

	ఒకవేళ (proxy_enable) {
		pr_info("sched_proxy_exec enabled via boot arg\n");
		static_branch_enable(&__sched_proxy_exec);
	} లేకపోతే {
		pr_info("sched_proxy_exec disabled via boot arg\n");
		static_branch_disable(&__sched_proxy_exec);
	}
	తిరిగి 1;
}
#లేకపోతే
స్థిర పూర్ణాంకం __init అమరిక_proxy_exec(అక్షరం *str)
{
	pr_warn("CONFIG_SCHED_PROXY_EXEC=n, so it cannot be enabled or disabled at boot time\n");
	తిరిగి 0;
}
#అయితే_ముగింపు
__setup("sched_proxy_exec", అమరిక_proxy_exec);

/*
 * Debugging: various feature bits
 *
 * If SCHED_DEBUG is disabled, each compilation unit has its own copy of
 * sysctl_sched_features, defined in sched.h, to allow constants propagation
 * at compile time and compiler optimization based on features default.
 */
#నిర్వచించు SCHED_FEAT(name, enabled)	\
	(1UL << __SCHED_FEAT_##name) * enabled |
__read_mostly గుర్తులేని పూర్ణాంకం sysctl_sched_features =
#అన్తర్భావ "features.h"
	0;
#నిర్వచనం_రద్దు SCHED_FEAT

/*
 * Print a warning if need_resched is set for the given duration (if
 * LATENCY_WARN is enabled).
 *
 * If sysctl_resched_latency_warn_once is set, only one warning will be shown
 * per boot.
 */
__read_mostly పూర్ణాంకం sysctl_resched_latency_warn_ms = 100;
__read_mostly పూర్ణాంకం sysctl_resched_latency_warn_once = 1;

/*
 * Number of tasks to iterate in a single balance run.
 * Limited because this is done with IRQs disabled.
 */
__read_mostly గుర్తులేని పూర్ణాంకం sysctl_sched_nr_migrate = SCHED_NR_MIGRATE_BREAK;

__read_mostly పూర్ణాంకం scheduler_running;

#నిర్వచితమైతే CONFIG_SCHED_CORE

DEFINE_STATIC_కీలకం_FALSE(__sched_core_enabled);

/* kernel prio, less is more */
స్థిర అంతఃవరుస పూర్ణాంకం __task_prio(స్థిరరాశి నిర్మాణం task_struct *p)
{
	ఒకవేళ (p->sched_class == &stop_sched_class) /* trumps deadline */
		తిరిగి -2;

	ఒకవేళ (p->dl_server)
		తిరిగి -1; /* deadline */

	ఒకవేళ (rt_or_dl_prio(p->prio))
		తిరిగి p->prio; /* [-1, 99] */

	ఒకవేళ (p->sched_class == &idle_sched_class)
		తిరిగి MAX_RT_PRIO + NICE_వెడల్పు; /* 140 */

	ఒకవేళ (task_on_scx(p))
		తిరిగి MAX_RT_PRIO + MAX_NICE + 1; /* 120, squash ext */

	తిరిగి MAX_RT_PRIO + MAX_NICE; /* 119, squash fair */
}

/*
 * l(a,b)
 * le(a,b) := !l(b,a)
 * g(a,b)  := l(b,a)
 * ge(a,b) := !l(a,b)
 */

/* real prio, less is less */
స్థిర అంతఃవరుస bool prio_less(స్థిరరాశి నిర్మాణం task_struct *a,
			     స్థిరరాశి నిర్మాణం task_struct *b, bool లోపల_fi)
{

	పూర్ణాంకం pa = __task_prio(a), pb = __task_prio(b);

	ఒకవేళ (-pa < -pb)
		తిరిగి true;

	ఒకవేళ (-pb < -pa)
		తిరిగి false;

	ఒకవేళ (pa == -1) { /* dl_prio() doesn't work because of stop_class above */
		స్థిరరాశి నిర్మాణం sched_dl_entity *a_dl, *b_dl;

		a_dl = &a->dl;
		/*
		 * Since,'a' and 'b' can be CFS tasks served by DL server,
		 * __task_prio() can return -1 (for DL) even for those. In that
		 * case, get to the dl_server's DL entity.
		 */
		ఒకవేళ (a->dl_server)
			a_dl = a->dl_server;

		b_dl = &b->dl;
		ఒకవేళ (b->dl_server)
			b_dl = b->dl_server;

		తిరిగి !dl_సమయం_before(a_dl->deadline, b_dl->deadline);
	}

	ఒకవేళ (pa == MAX_RT_PRIO + MAX_NICE)	/* fair */
		తిరిగి cfs_prio_less(a, b, లోపల_fi);

#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
	ఒకవేళ (pa == MAX_RT_PRIO + MAX_NICE + 1)	/* ext */
		తిరిగి scx_prio_less(a, b, లోపల_fi);
#అయితే_ముగింపు

	తిరిగి false;
}

స్థిర అంతఃవరుస bool __sched_core_less(స్థిరరాశి నిర్మాణం task_struct *a,
				     స్థిరరాశి నిర్మాణం task_struct *b)
{
	ఒకవేళ (a->core_cookie < b->core_cookie)
		తిరిగి true;

	ఒకవేళ (a->core_cookie > b->core_cookie)
		తిరిగి false;

	/* flip prio, so high prio is leftmost */
	ఒకవేళ (prio_less(b, a, !!task_rq(a)->core->core_forceidle_లెక్క))
		తిరిగి true;

	తిరిగి false;
}

#నిర్వచించు __node_2_sc(కణుపు) rb_నమోదు((కణుపు), నిర్మాణం task_struct, core_కణుపు)

స్థిర అంతఃవరుస bool rb_sched_core_less(నిర్మాణం rb_కణుపు *a, స్థిరరాశి నిర్మాణం rb_కణుపు *b)
{
	తిరిగి __sched_core_less(__node_2_sc(a), __node_2_sc(b));
}

స్థిర అంతఃవరుస పూర్ణాంకం rb_sched_core_cmp(స్థిరరాశి శూన్య *కీలకం, స్థిరరాశి నిర్మాణం rb_కణుపు *కణుపు)
{
	స్థిరరాశి నిర్మాణం task_struct *p = __node_2_sc(కణుపు);
	గుర్తులేని దీర్ఘ cookie = (గుర్తులేని దీర్ఘ)కీలకం;

	ఒకవేళ (cookie < p->core_cookie)
		తిరిగి -1;

	ఒకవేళ (cookie > p->core_cookie)
		తిరిగి 1;

	తిరిగి 0;
}

శూన్య sched_core_వరుసలో_చేర్చు(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	ఒకవేళ (p->se.sched_delayed)
		తిరిగి;

	rq->core->core_task_seq++;

	ఒకవేళ (!p->core_cookie)
		తిరిగి;

	rb_కూడు(&p->core_కణుపు, &rq->core_వృక్షం, rb_sched_core_less);
}

శూన్య sched_core_వరుస_నుండి_తీయి(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	ఒకవేళ (p->se.sched_delayed)
		తిరిగి;

	rq->core->core_task_seq++;

	ఒకవేళ (sched_core_enqueued(p)) {
		rb_erase(&p->core_కణుపు, &rq->core_వృక్షం);
		RB_CLEAR_కణుపు(&p->core_కణుపు);
	}

	/*
	 * Migrating the last task off the cpu, with the cpu in forced idle
	 * state. Reschedule to create an accounting edge for forced idle,
	 * and re-examine whether the core is still in forced idle state.
	 */
	ఒకవేళ (!(flags & వరుస_నుండి_తీయి_SAVE) && rq->nr_running == 1 &&
	    rq->core->core_forceidle_లెక్క && rq->curr == rq->idle)
		resched_curr(rq);
}

స్థిర పూర్ణాంకం sched_task_is_throttled(నిర్మాణం task_struct *p, పూర్ణాంకం cpu)
{
	ఒకవేళ (p->sched_class->task_is_throttled)
		తిరిగి p->sched_class->task_is_throttled(p, cpu);

	తిరిగి 0;
}

స్థిర నిర్మాణం task_struct *sched_core_తదుపరి(నిర్మాణం task_struct *p, గుర్తులేని దీర్ఘ cookie)
{
	నిర్మాణం rb_కణుపు *కణుపు = &p->core_కణుపు;
	పూర్ణాంకం cpu = task_cpu(p);

	చేయి {
		కణుపు = rb_తదుపరి(కణుపు);
		ఒకవేళ (!కణుపు)
			తిరిగి ఖాళీ;

		p = __node_2_sc(కణుపు);
		ఒకవేళ (p->core_cookie != cookie)
			తిరిగి ఖాళీ;

	} అయితే (sched_task_is_throttled(p, cpu));

	తిరిగి p;
}

/*
 * Find left-most (aka, highest priority) and unthrottled task matching @cookie.
 * If no suitable task is found, NULL will be returned.
 */
స్థిర నిర్మాణం task_struct *sched_core_కనుగొను(నిర్మాణం rq *rq, గుర్తులేని దీర్ఘ cookie)
{
	నిర్మాణం task_struct *p;
	నిర్మాణం rb_కణుపు *కణుపు;

	కణుపు = rb_కనుగొను_మొదటి((శూన్య *)cookie, &rq->core_వృక్షం, rb_sched_core_cmp);
	ఒకవేళ (!కణుపు)
		తిరిగి ఖాళీ;

	p = __node_2_sc(కణుపు);
	ఒకవేళ (!sched_task_is_throttled(p, rq->cpu))
		తిరిగి p;

	తిరిగి sched_core_తదుపరి(p, cookie);
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

స్థిర DEFINE_MUTEX(sched_core_mutex);
స్థిర atomic_t sched_core_లెక్క;
స్థిర నిర్మాణం cpumask sched_core_ముసుగు;

స్థిర శూన్య sched_core_తాళం(పూర్ణాంకం cpu, గుర్తులేని దీర్ఘ *flags)
	__context_unsafe(/* acquires multiple */)
	__acquires(&runqueues.__lock) /* overapproximation */
{
	స్థిరరాశి నిర్మాణం cpumask *smt_ముసుగు = cpu_smt_ముసుగు(cpu);
	పూర్ణాంకం t, i = 0;

	local_irq_save(*flags);
	for_each_cpu(t, smt_ముసుగు)
		raw_spin_తాళం_nested(&cpu_rq(t)->__lock, i++);
}

స్థిర శూన్య sched_core_unlock(పూర్ణాంకం cpu, గుర్తులేని దీర్ఘ *flags)
	__context_unsafe(/* releases multiple */)
	__releases(&runqueues.__lock) /* overapproximation */
{
	స్థిరరాశి నిర్మాణం cpumask *smt_ముసుగు = cpu_smt_ముసుగు(cpu);
	పూర్ణాంకం t;

	for_each_cpu(t, smt_ముసుగు)
		raw_spin_unlock(&cpu_rq(t)->__lock);
	local_irq_restore(*flags);
}

స్థిర శూన్య __sched_core_flip(bool enabled)
{
	గుర్తులేని దీర్ఘ flags;
	పూర్ణాంకం cpu, t;

	cpus_read_తాళం();

	/*
	 * Toggle the online cores, one by one.
	 */
	cpumask_copy(&sched_core_ముసుగు, cpu_online_ముసుగు);
	for_each_cpu(cpu, &sched_core_ముసుగు) {
		స్థిరరాశి నిర్మాణం cpumask *smt_ముసుగు = cpu_smt_ముసుగు(cpu);

		sched_core_తాళం(cpu, &flags);

		/*
		 * A core-wide selection may have the shared rq lock temporarily
		 * released by a lock-dropping ->pick_task(). Flipping would
		 * rebind rq_lockp() under it. Wait it out.
		 */
		అయితే (cpu_rq(cpu)->core->core_pick_లోపల_flight) {
			sched_core_unlock(cpu, &flags);
			cpu_relax();
			sched_core_తాళం(cpu, &flags);
		}

		for_each_cpu(t, smt_ముసుగు)
			cpu_rq(t)->core_enabled = enabled;

		cpu_rq(cpu)->core->core_forceidle_ప్రారంభం = 0;

		sched_core_unlock(cpu, &flags);

		cpumask_andnot(&sched_core_ముసుగు, &sched_core_ముసుగు, smt_ముసుగు);
	}

	/*
	 * Toggle the offline CPUs.
	 */
	for_each_cpu_andnot(cpu, cpu_possible_ముసుగు, cpu_online_ముసుగు)
		cpu_rq(cpu)->core_enabled = enabled;

	cpus_read_unlock();
}

స్థిర శూన్య sched_core_assert_ఖాళీలు(శూన్య)
{
	పూర్ణాంకం cpu;

	for_each_possible_cpu(cpu)
		WARN_ON_ONCE(!RB_ఖాళీలు_మూలం(&cpu_rq(cpu)->core_వృక్షం));
}

స్థిర శూన్య __sched_core_enable(శూన్య)
{
	static_branch_enable(&__sched_core_enabled);
	/*
	 * Ensure all previous instances of raw_spin_rq_*lock() have finished
	 * and future ones will observe !sched_core_disabled().
	 */
	synchronize_rcu();
	__sched_core_flip(true);
	sched_core_assert_ఖాళీలు();
}

స్థిర శూన్య __sched_core_disable(శూన్య)
{
	sched_core_assert_ఖాళీలు();
	__sched_core_flip(false);
	static_branch_disable(&__sched_core_enabled);
}

శూన్య sched_core_get(శూన్య)
{
	ఒకవేళ (atomic_inc_not_zero(&sched_core_లెక్క))
		తిరిగి;

	mutex_తాళం(&sched_core_mutex);
	ఒకవేళ (!atomic_read(&sched_core_లెక్క))
		__sched_core_enable();

	smp_mb__before_atomic();
	atomic_inc(&sched_core_లెక్క);
	mutex_unlock(&sched_core_mutex);
}

స్థిర శూన్య __sched_core_put(నిర్మాణం work_struct *work)
{
	ఒకవేళ (atomic_dec_and_mutex_తాళం(&sched_core_లెక్క, &sched_core_mutex)) {
		__sched_core_disable();
		mutex_unlock(&sched_core_mutex);
	}
}

శూన్య sched_core_put(శూన్య)
{
	స్థిర DECLARE_WORK(_work, __sched_core_put);

	/*
	 * "There can be only one"
	 *
	 * Either this is the last one, or we don't actually need to do any
	 * 'work'. If it is the last *again*, we rely on
	 * WORK_STRUCT_PENDING_BIT.
	 */
	ఒకవేళ (!atomic_కూడు_unless(&sched_core_లెక్క, -1, 1))
		కాలపట్టిక_work(&_work);
}

#లేకపోతే /* !CONFIG_SCHED_CORE: */

స్థిర అంతఃవరుస శూన్య sched_core_వరుసలో_చేర్చు(నిర్మాణం rq *rq, నిర్మాణం task_struct *p) { }
స్థిర అంతఃవరుస శూన్య
sched_core_వరుస_నుండి_తీయి(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags) { }

#అయితే_ముగింపు /* !CONFIG_SCHED_CORE */

/* need a wrapper since we may need to trace from modules */
EXPORT_TRACEPOINT_చిహ్నం(sched_సమితి_state_tp);

/*
 * Call via the helper macro trace_set_current_state.
 * Calls to this function MUST be guarded by a
 * tracepoint_enabled(sched_set_state_tp)
 */
శూన్య __trace_set_current_state(పూర్ణాంకం state_విలువ)
{
	trace_call_sched_సమితి_state_tp(current, state_విలువ);
}
EXPORT_చిహ్నం(__trace_set_current_state);

పూర్ణాంకం task_llc(స్థిరరాశి నిర్మాణం task_struct *p)
{
	తిరిగి చొప్పున_cpu(sd_llc_id, task_cpu(p));
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

శూన్య raw_spin_rq_తాళం_nested(నిర్మాణం rq *rq, పూర్ణాంకం subclass)
	__context_unsafe()
{
	raw_spinlock_t *తాళం;

	/* Matches synchronize_rcu() in __sched_core_enable() */
	preempt_disable();
	ఒకవేళ (sched_core_disabled()) {
		raw_spin_తాళం_nested(&rq->__lock, subclass);
		/* preempt_count *MUST* be > 1 */
		preempt_enable_no_resched();
		తిరిగి;
	}

	కొరకు (;;) {
		తాళం = __rq_lockp(rq);
		raw_spin_తాళం_nested(తాళం, subclass);
		ఒకవేళ (likely(తాళం == __rq_lockp(rq))) {
			/* preempt_count *MUST* be > 1 */
			preempt_enable_no_resched();
			తిరిగి;
		}
		raw_spin_unlock(తాళం);
	}
}

bool raw_spin_rq_trylock(నిర్మాణం rq *rq)
	__context_unsafe()
{
	raw_spinlock_t *తాళం;
	bool ret;

	/* Matches synchronize_rcu() in __sched_core_enable() */
	preempt_disable();
	ఒకవేళ (sched_core_disabled()) {
		ret = raw_spin_trylock(&rq->__lock);
		preempt_enable();
		తిరిగి ret;
	}

	కొరకు (;;) {
		తాళం = __rq_lockp(rq);
		ret = raw_spin_trylock(తాళం);
		ఒకవేళ (!ret || (likely(తాళం == __rq_lockp(rq)))) {
			preempt_enable();
			తిరిగి ret;
		}
		raw_spin_unlock(తాళం);
	}
}

/*
 * double_rq_lock - safely lock two runqueues
 */
శూన్య double_rq_తాళం(నిర్మాణం rq *rq1, నిర్మాణం rq *rq2)
{
	lockdep_assert_irqs_disabled();

	ఒకవేళ (rq_క్రమం_less(rq2, rq1))
		మార్పిడి(rq1, rq2);

	raw_spin_rq_తాళం(rq1);
	ఒకవేళ (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_తాళం_nested(rq2, SINGLE_లోతు_NESTING);
	లేకపోతే
		__acquire_ctx_lock(__rq_lockp(rq2)); /* fake acquire */

	double_rq_clock_clear_update(rq1, rq2);
}

/*
 * ___task_rq_lock - lock the rq @p resides on.
 */
నిర్మాణం rq *___task_rq_lock(నిర్మాణం task_struct *p, నిర్మాణం rq_flags *rf)
{
	నిర్మాణం rq *rq;

	lockdep_assert_held(&p->pi_తాళం);

	కొరకు (;;) {
		rq = task_rq(p);
		raw_spin_rq_తాళం(rq);
		ఒకవేళ (likely(rq == task_rq(p) && !task_on_rq_migrating(p))) {
			rq_pin_తాళం(rq, rf);
			తిరిగి rq;
		}
		raw_spin_rq_unlock(rq);

		అయితే (unlikely(task_on_rq_migrating(p)))
			cpu_relax();
	}
}

/*
 * task_rq_lock - lock p->pi_lock and lock the rq @p resides on.
 */
నిర్మాణం rq *task_rq_తాళం(నిర్మాణం task_struct *p, నిర్మాణం rq_flags *rf)
{
	నిర్మాణం rq *rq;

	కొరకు (;;) {
		raw_spin_తాళం_irqsave(&p->pi_తాళం, rf->flags);
		rq = task_rq(p);
		raw_spin_rq_తాళం(rq);
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
		ఒకవేళ (likely(rq == task_rq(p) && !task_on_rq_migrating(p))) {
			rq_pin_తాళం(rq, rf);
			తిరిగి rq;
		}
		raw_spin_rq_unlock(rq);
		raw_spin_unlock_irqrestore(&p->pi_తాళం, rf->flags);

		అయితే (unlikely(task_on_rq_migrating(p)))
			cpu_relax();
	}
}

/*
 * RQ-clock updating methods:
 */

/* Use CONFIG_PARAVIRT as this will avoid more #ifdef in arch code. */
#నిర్వచితమైతే CONFIG_PARAVIRT
DEFINE_STATIC_కీలకం_FALSE(paravirt_steal_rq_enabled);
#అయితే_ముగింపు

స్థిర శూన్య update_rq_clock_task(నిర్మాణం rq *rq, s64 delta)
{
/*
 * In theory, the compile should just see 0 here, and optimize out the call
 * to sched_rt_avg_update. But I don't trust it...
 */
	s64 __maybe_unused steal = 0, irq_delta = 0;

#నిర్వచితమైతే CONFIG_IRQ_సమయం_ACCOUNTING
	ఒకవేళ (irqtime_enabled()) {
		irq_delta = irq_సమయం_read(cpu_యొక్క(rq)) - rq->prev_irq_సమయం;

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
		ఒకవేళ (irq_delta > delta)
			irq_delta = delta;

		rq->prev_irq_సమయం += irq_delta;
		delta -= irq_delta;
		delayacct_irq(rq->curr, irq_delta);
	}
#అయితే_ముగింపు
#నిర్వచితమైతే CONFIG_PARAVIRT_సమయం_ACCOUNTING
	ఒకవేళ (static_branch_unlikely(&paravirt_steal_rq_enabled)) {
		u64 prev_steal;

		steal = prev_steal = paravirt_steal_clock(cpu_యొక్క(rq));
		steal -= rq->prev_steal_సమయం_rq;

		ఒకవేళ (unlikely(steal > delta))
			steal = delta;

		rq->prev_steal_సమయం_rq = prev_steal;
		delta -= steal;
	}
#అయితే_ముగింపు

	rq->clock_task += delta;

#నిర్వచితమైతే CONFIG_HAVE_SCHED_AVG_IRQ
	ఒకవేళ ((irq_delta + steal) && sched_feat(NONTASK_సామర్థ్యం))
		update_irq_ఎక్కించు_avg(rq, irq_delta + steal);
#అయితే_ముగింపు
	update_rq_clock_pelt(rq, delta);
}

శూన్య update_rq_clock(నిర్మాణం rq *rq)
{
	s64 delta;
	u64 clock;

	lockdep_assert_rq_held(rq);

	ఒకవేళ (rq->clock_update_flags & RQCF_ACT_SKIP)
		తిరిగి;

	ఒకవేళ (sched_feat(WARN_DOUBLE_CLOCK))
		WARN_ON_ONCE(rq->clock_update_flags & RQCF_UPDATED);
	rq->clock_update_flags |= RQCF_UPDATED;

	clock = sched_clock_cpu(cpu_యొక్క(rq));
	scx_rq_clock_update(rq, clock);

	delta = clock - rq->clock;
	ఒకవేళ (delta < 0)
		తిరిగి;
	rq->clock += delta;

	update_rq_clock_task(rq, delta);
}

#నిర్వచితమైతే CONFIG_SCHED_HRTICK
/*
 * Use HR-timers to deliver accurate preemption points.
 */

గణన {
	HRTICK_SCHED_NONE		= 0,
	HRTICK_SCHED_DEFER		= బిట్టు(1),
	HRTICK_SCHED_ప్రారంభం		= బిట్టు(2),
	HRTICK_SCHED_REARM_HRTIMER	= బిట్టు(3)
};

స్థిర శూన్య __used hrtick_clear(నిర్మాణం rq *rq)
{
	ఒకవేళ (hrtimer_చురుకైన(&rq->hrtick_timer))
		hrtimer_cancel(&rq->hrtick_timer);
}

/*
 * High-resolution timer tick.
 * Runs from hardirq context with interrupts disabled.
 */
స్థిర గణన hrtimer_restart hrtick(నిర్మాణం hrtimer *timer)
{
	నిర్మాణం rq *rq = container_యొక్క(timer, నిర్మాణం rq, hrtick_timer);
	నిర్మాణం rq_flags rf;

	WARN_ON_ONCE(cpu_యొక్క(rq) != smp_processor_id());

	rq_తాళం(rq, &rf);
	update_rq_clock(rq);
	rq->donor->sched_class->task_tick(rq, rq->donor, 1);
	rq_unlock(rq, &rf);

	తిరిగి HRTIMER_NORESTART;
}

స్థిర అంతఃవరుస bool hrtick_needs_rearm(నిర్మాణం hrtimer *timer, ktime_t expires)
{
	/*
	 * Queued is false when the timer is not started or currently
	 * running the callback. In both cases, restart. If queued check
	 * whether the expiry time actually changes substantially.
	 */
	తిరిగి !hrtimer_is_queued(timer) ||
		నిరపేక్ష(expires - hrtimer_get_expires(timer)) > 5000;
}

స్థిర శూన్య hrtick_cond_restart(నిర్మాణం rq *rq)
{
	నిర్మాణం hrtimer *timer = &rq->hrtick_timer;
	ktime_t సమయం = rq->hrtick_సమయం;

	ఒకవేళ (hrtick_needs_rearm(timer, సమయం))
		hrtimer_ప్రారంభం(timer, సమయం, HRTIMER_బహుళకం_ABS_PINNED_HARD);
}

/*
 * called from hardirq (IPI) context
 */
స్థిర శూన్య __hrtick_start(శూన్య *arg)
{
	నిర్మాణం rq *rq = arg;
	నిర్మాణం rq_flags rf;

	rq_తాళం(rq, &rf);
	hrtick_cond_restart(rq);
	rq_unlock(rq, &rf);
}

/*
 * Called to set the hrtick timer state.
 *
 * called with rq->lock held and IRQs disabled
 */
శూన్య hrtick_ప్రారంభం(నిర్మాణం rq *rq, u64 delay)
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
	ఒకవేళ (rq->hrtick_sched) {
		rq->hrtick_sched |= HRTICK_SCHED_ప్రారంభం;
		rq->hrtick_delay = delta;
		తిరిగి;
	}

	rq->hrtick_సమయం = ktime_కూడు_ns(ktime_get(), delta);
	ఒకవేళ (!hrtick_needs_rearm(&rq->hrtick_timer, rq->hrtick_సమయం))
		తిరిగి;

	ఒకవేళ (rq == this_rq())
		hrtimer_ప్రారంభం(&rq->hrtick_timer, rq->hrtick_సమయం, HRTIMER_బహుళకం_ABS_PINNED_HARD);
	లేకపోతే
		smp_call_function_single_async(cpu_యొక్క(rq), &rq->hrtick_csd);
}

స్థిర అంతఃవరుస శూన్య hrtick_కాలపట్టిక_enter(నిర్మాణం rq *rq)
{
	rq->hrtick_sched = HRTICK_SCHED_DEFER;
	ఒకవేళ (hrtimer_test_and_clear_rearm_deferred())
		rq->hrtick_sched |= HRTICK_SCHED_REARM_HRTIMER;
}

స్థిర అంతఃవరుస శూన్య hrtick_కాలపట్టిక_exit(నిర్మాణం rq *rq)
{
	ఒకవేళ (rq->hrtick_sched & HRTICK_SCHED_ప్రారంభం) {
		rq->hrtick_సమయం = ktime_కూడు_ns(ktime_get(), rq->hrtick_delay);
		hrtick_cond_restart(rq);
	} లేకపోతే ఒకవేళ (idle_rq(rq)) {
		/*
		 * No need for using hrtimer_is_active(). The timer is CPU local
		 * and interrupts are disabled, so the callback cannot be
		 * running and the queued state is valid.
		 */
		ఒకవేళ (hrtimer_is_queued(&rq->hrtick_timer))
			hrtimer_cancel(&rq->hrtick_timer);
	}

	ఒకవేళ (rq->hrtick_sched & HRTICK_SCHED_REARM_HRTIMER)
		__hrtimer_rearm_deferred();

	rq->hrtick_sched = HRTICK_SCHED_NONE;
}

స్థిర శూన్య hrtick_rq_init(నిర్మాణం rq *rq)
{
	INIT_CSD(&rq->hrtick_csd, __hrtick_start, rq);
	rq->hrtick_sched = HRTICK_SCHED_NONE;
	hrtimer_అమరిక(&rq->hrtick_timer, hrtick, CLOCK_MONOTONIC,
		      HRTIMER_బహుళకం_REL_HARD | HRTIMER_బహుళకం_LAZY_REARM);
}
#లేకపోతే /* !CONFIG_SCHED_HRTICK: */
స్థిర అంతఃవరుస శూన్య hrtick_clear(నిర్మాణం rq *rq) { }
స్థిర అంతఃవరుస శూన్య hrtick_rq_init(నిర్మాణం rq *rq) { }
స్థిర అంతఃవరుస శూన్య hrtick_కాలపట్టిక_enter(నిర్మాణం rq *rq) { }
స్థిర అంతఃవరుస శూన్య hrtick_కాలపట్టిక_exit(నిర్మాణం rq *rq) { }
#అయితే_ముగింపు /* !CONFIG_SCHED_HRTICK */

/*
 * try_cmpxchg based fetch_or() macro so it works for different integer types:
 */
#నిర్వచించు fetch_or(ptr, ముసుగు)						\
	({								\
		typeof(ptr) _ptr = (ptr);				\
		typeof(ముసుగు) ముసుగు_2 = (ముసుగు);				\
		typeof(*_ptr) _val = *_ptr;				\
									\
		చేయి {							\
		} అయితే (!try_cmpxchg(_ptr, &_val, _val | ముసుగు_2));	\
	_val;								\
})

#నిర్వచితమైతే TIF_POLLING_NRFLAG
/*
 * Atomically set TIF_NEED_RESCHED and test for TIF_POLLING_NRFLAG,
 * this avoids any races wrt polling state changes and thereby avoids
 * spurious IPIs.
 */
స్థిర అంతఃవరుస bool సమితి_nr_and_not_polling(నిర్మాణం thread_info *ti, పూర్ణాంకం tif)
{
	తిరిగి !(fetch_or(&ti->flags, 1 << tif) & _TIF_POLLING_NRFLAG);
}

/*
 * Atomically set TIF_NEED_RESCHED if TIF_POLLING_NRFLAG is set.
 *
 * If this returns true, then the idle task promises to call
 * sched_ttwu_pending() and reschedule soon.
 */
స్థిర bool సమితి_nr_if_polling(నిర్మాణం task_struct *p)
{
	నిర్మాణం thread_info *ti = task_thread_info(p);
	typeof(ti->flags) val = READ_ONCE(ti->flags);

	చేయి {
		ఒకవేళ (!(val & _TIF_POLLING_NRFLAG))
			తిరిగి false;
		ఒకవేళ (val & _TIF_NEED_RESCHED)
			తిరిగి true;
	} అయితే (!try_cmpxchg(&ti->flags, &val, val | _TIF_NEED_RESCHED));

	తిరిగి true;
}

#లేకపోతే
స్థిర అంతఃవరుస bool సమితి_nr_and_not_polling(నిర్మాణం thread_info *ti, పూర్ణాంకం tif)
{
	సమితి_ti_thread_flag(ti, tif);
	తిరిగి true;
}

స్థిర అంతఃవరుస bool సమితి_nr_if_polling(నిర్మాణం task_struct *p)
{
	తిరిగి false;
}
#అయితే_ముగింపు

స్థిర bool __wake_q_add(నిర్మాణం wake_q_తల *తల, నిర్మాణం task_struct *task)
{
	నిర్మాణం wake_q_కణుపు *కణుపు = &task->wake_q;

	/*
	 * Atomically grab the task, if ->wake_q is !nil already it means
	 * it's already queued (either by us or someone else) and will get the
	 * wakeup due to that.
	 *
	 * In order to ensure that a pending wakeup will observe our pending
	 * state, even in the failed case, an explicit smp_mb() must be used.
	 */
	smp_mb__before_atomic();
	ఒకవేళ (unlikely(cmpxchg_relaxed(&కణుపు->తదుపరి, ఖాళీ, WAKE_Q_తోక)))
		తిరిగి false;

	/*
	 * The head is context local, there can be no concurrency.
	 */
	*తల->lastp = కణుపు;
	తల->lastp = &కణుపు->తదుపరి;
	తిరిగి true;
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
శూన్య wake_q_కూడు(నిర్మాణం wake_q_తల *తల, నిర్మాణం task_struct *task)
{
	ఒకవేళ (__wake_q_add(తల, task))
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
శూన్య wake_q_కూడు_సురక్షిత(నిర్మాణం wake_q_తల *తల, నిర్మాణం task_struct *task)
{
	ఒకవేళ (!__wake_q_add(తల, task))
		put_task_struct(task);
}

శూన్య wake_పైకి_q(నిర్మాణం wake_q_తల *తల)
{
	నిర్మాణం wake_q_కణుపు *కణుపు = తల->మొదటి;

	అయితే (కణుపు != WAKE_Q_తోక) {
		నిర్మాణం task_struct *task;

		task = container_యొక్క(కణుపు, నిర్మాణం task_struct, wake_q);
		కణుపు = కణుపు->తదుపరి;
		/* pairs with cmpxchg_relaxed() in __wake_q_add() */
		WRITE_ONCE(task->wake_q.తదుపరి, ఖాళీ);
		/* Task can safely be re-inserted now. */

		/*
		 * wake_up_process() executes a full barrier, which pairs with
		 * the queueing in wake_q_add() so as not to miss wakeups.
		 */
		wake_పైకి_process(task);
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
స్థిర శూన్య __resched_curr(నిర్మాణం rq *rq, పూర్ణాంకం tif)
{
	నిర్మాణం task_struct *curr = rq->curr;
	నిర్మాణం thread_info *cti = task_thread_info(curr);
	bool need_ipi;
	పూర్ణాంకం cpu;

	lockdep_assert_rq_held(rq);

	/*
	 * Always immediately preempt the idle task; no point in delaying doing
	 * actual work.
	 */
	ఒకవేళ (is_idle_task(curr) && tif == TIF_NEED_RESCHED_LAZY)
		tif = TIF_NEED_RESCHED;

	ఒకవేళ (cti->flags & ((1 << tif) | _TIF_NEED_RESCHED))
		తిరిగి;

	cpu = cpu_యొక్క(rq);

	ఒకవేళ (cpu == smp_processor_id()) {
		సమితి_ti_thread_flag(cti, tif);
		ఒకవేళ (tif == TIF_NEED_RESCHED)
			సమితి_preempt_need_resched();
		trace_sched_సమితి_need_resched_tp(curr, cpu, tif);
		తిరిగి;
	}

	need_ipi = సమితి_nr_and_not_polling(cti, tif);
	trace_sched_సమితి_need_resched_tp(curr, cpu, tif);
	ఒకవేళ (need_ipi) {
		ఒకవేళ (tif == TIF_NEED_RESCHED)
			smp_send_reschedule(cpu);
	} లేకపోతే {
		trace_sched_wake_idle_without_ipi(cpu);
	}
}

/*
 * Calls to this function MUST be guarded by a
 * tracepoint_enabled(sched_set_need_resched_tp)
 */
శూన్య __trace_set_need_resched(నిర్మాణం task_struct *curr, పూర్ణాంకం tif)
{
	trace_call_sched_సమితి_need_resched_tp(curr, smp_processor_id(), tif);
}
EXPORT_చిహ్నం_GPL(__trace_set_need_resched);

శూన్య resched_curr(నిర్మాణం rq *rq)
{
	__resched_curr(rq, TIF_NEED_RESCHED);
}

#నిర్వచితమైతే CONFIG_PREEMPT_DYNAMIC
స్థిర DEFINE_STATIC_కీలకం_FALSE(sk_dynamic_preempt_lazy);
స్థిర __always_inline bool dynamic_preempt_lazy(శూన్య)
{
	తిరిగి static_branch_unlikely(&sk_dynamic_preempt_lazy);
}
#లేకపోతే
స్థిర __always_inline bool dynamic_preempt_lazy(శూన్య)
{
	తిరిగి IS_ENABLED(CONFIG_PREEMPT_LAZY);
}
#అయితే_ముగింపు

స్థిర __always_inline పూర్ణాంకం get_lazy_tif_బిట్టు(శూన్య)
{
	ఒకవేళ (dynamic_preempt_lazy())
		తిరిగి TIF_NEED_RESCHED_LAZY;

	తిరిగి TIF_NEED_RESCHED;
}

శూన్య resched_curr_lazy(నిర్మాణం rq *rq)
{
	__resched_curr(rq, get_lazy_tif_బిట్టు());
}

శూన్య resched_cpu(పూర్ణాంకం cpu)
{
	నిర్మాణం rq *rq = cpu_rq(cpu);
	గుర్తులేని దీర్ఘ flags;

	raw_spin_rq_తాళం_irqsave(rq, flags);
	ఒకవేళ (cpu_online(cpu) || cpu == smp_processor_id())
		resched_curr(rq);
	raw_spin_rq_unlock_irqrestore(rq, flags);
}

#నిర్వచితమైతే CONFIG_NO_HZ_COMMON
/*
 * In the semi idle case, use the nearest busy CPU for migrating timers
 * from an idle CPU.  This is good for power-savings.
 *
 * We don't do similar optimization for completely idle system, as
 * selecting an idle CPU will add more delays to the timers than intended
 * (as that CPU's timer base may not be up to date wrt jiffies etc).
 */
పూర్ణాంకం get_nohz_timer_లక్ష్యం(శూన్య)
{
	పూర్ణాంకం i, cpu = smp_processor_id(), default_cpu = -1;
	నిర్మాణం sched_domain *sd;
	స్థిరరాశి నిర్మాణం cpumask *hk_ముసుగు;

	ఒకవేళ (housekeeping_cpu(cpu, HK_TYPE_KERNEL_NOISE)) {
		ఒకవేళ (!idle_cpu(cpu))
			తిరిగి cpu;
		default_cpu = cpu;
	}

	hk_ముసుగు = housekeeping_cpumask(HK_TYPE_KERNEL_NOISE);

	guard(rcu)();

	for_each_domain(cpu, sd) {
		for_each_cpu_and(i, sched_domain_పరిధి(sd), hk_ముసుగు) {
			ఒకవేళ (cpu == i)
				కొనసాగు;

			ఒకవేళ (!idle_cpu(i))
				తిరిగి i;
		}
	}

	ఒకవేళ (default_cpu == -1)
		default_cpu = housekeeping_any_cpu(HK_TYPE_KERNEL_NOISE);

	తిరిగి default_cpu;
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
స్థిర శూన్య wake_పైకి_idle_cpu(పూర్ణాంకం cpu)
{
	నిర్మాణం rq *rq = cpu_rq(cpu);

	ఒకవేళ (cpu == smp_processor_id())
		తిరిగి;

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
	ఒకవేళ (సమితి_nr_and_not_polling(task_thread_info(rq->idle), TIF_NEED_RESCHED))
		smp_send_reschedule(cpu);
	లేకపోతే
		trace_sched_wake_idle_without_ipi(cpu);
}

స్థిర bool wake_పైకి_నిండిన_nohz_cpu(పూర్ణాంకం cpu)
{
	/*
	 * We just need the target to call irq_exit() and re-evaluate
	 * the next tick. The nohz full kick at least implies that.
	 * If needed we can still optimize that later with an
	 * empty IRQ.
	 */
	ఒకవేళ (cpu_is_offline(cpu))
		తిరిగి true;  /* Don't try to wake offline CPUs. */
	ఒకవేళ (tick_nohz_నిండిన_cpu(cpu)) {
		ఒకవేళ (cpu != smp_processor_id() ||
		    tick_nohz_tick_stopped())
			tick_nohz_నిండిన_kick_cpu(cpu);
		తిరిగి true;
	}

	తిరిగి false;
}

/*
 * Wake up the specified CPU.  If the CPU is going offline, it is the
 * caller's responsibility to deal with the lost wakeup, for example,
 * by hooking into the CPU_DEAD notifier like timers and hrtimers do.
 */
శూన్య wake_పైకి_nohz_cpu(పూర్ణాంకం cpu)
{
	ఒకవేళ (!wake_పైకి_నిండిన_nohz_cpu(cpu))
		wake_పైకి_idle_cpu(cpu);
}

స్థిర శూన్య nohz_csd_func(శూన్య *info)
{
	నిర్మాణం rq *rq = info;
	పూర్ణాంకం cpu = cpu_యొక్క(rq);
	గుర్తులేని పూర్ణాంకం flags;

	/*
	 * Release the rq::nohz_csd.
	 */
	flags = atomic_fetch_andnot(NOHZ_KICK_ముసుగు | NOHZ_NEWILB_KICK, nohz_flags(cpu));
	WARN_ON(!(flags & NOHZ_KICK_ముసుగు));

	rq->idle_balance = idle_cpu(cpu);
	ఒకవేళ (rq->idle_balance) {
		rq->nohz_idle_balance = flags;
		__raise_softirq_irqoff(SCHED_SOFTIRQ);
	}
}

#అయితే_ముగింపు /* CONFIG_NO_HZ_COMMON */

#నిర్వచితమైతే CONFIG_NO_HZ_నిండిన
స్థిర అంతఃవరుస bool __need_bw_check(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	ఒకవేళ (rq->nr_running != 1)
		తిరిగి false;

	ఒకవేళ (p->sched_class != &fair_sched_class)
		తిరిగి false;

	ఒకవేళ (!task_on_rq_queued(p))
		తిరిగి false;

	తిరిగి true;
}

bool sched_can_stop_tick(నిర్మాణం rq *rq)
{
	పూర్ణాంకం fifo_nr_running;

	/* Deadline tasks, even if single, need the tick */
	ఒకవేళ (rq->dl.dl_nr_running)
		తిరిగి false;

	/*
	 * If there are more than one RR tasks, we need the tick to affect the
	 * actual RR behaviour.
	 */
	ఒకవేళ (rq->rt.rr_nr_running) {
		ఒకవేళ (rq->rt.rr_nr_running == 1)
			తిరిగి true;
		లేకపోతే
			తిరిగి false;
	}

	/*
	 * If there's no RR tasks, but FIFO tasks, we can skip the tick, no
	 * forced preemption between FIFO tasks.
	 */
	fifo_nr_running = rq->rt.rt_nr_running - rq->rt.rr_nr_running;
	ఒకవేళ (fifo_nr_running)
		తిరిగి true;

	/*
	 * If there are no DL,RR/FIFO tasks, there must only be CFS or SCX tasks
	 * left. For CFS, if there's more than one we need the tick for
	 * involuntary preemption. For SCX, ask.
	 */
	ఒకవేళ (scx_enabled() && !scx_can_stop_tick(rq))
		తిరిగి false;

	ఒకవేళ (rq->cfs.h_nr_queued > 1)
		తిరిగి false;

	/*
	 * If there is one task and it has CFS runtime bandwidth constraints
	 * and it's on the cpu now we don't want to stop the tick.
	 * This check prevents clearing the bit if a newly enqueued task here is
	 * dequeued by migrating while the constrained task continues to run.
	 * E.g. going from 2->1 without going through pick_next_task().
	 */
	ఒకవేళ (__need_bw_check(rq, rq->curr)) {
		ఒకవేళ (cfs_task_bw_constrained(rq->curr))
			తిరిగి false;
	}

	తిరిగి true;
}
#అయితే_ముగింపు /* CONFIG_NO_HZ_FULL */

#ఒకవేళ defined(CONFIG_RT_సమూహం_SCHED) || defined(CONFIG_FAIR_సమూహం_SCHED)
/*
 * Iterate task_group tree rooted at *from, calling @down when first entering a
 * node and @up when leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
పూర్ణాంకం walk_tg_వృక్షం_from(నిర్మాణం task_సమూహం *from,
			     tg_visitor కిందికి, tg_visitor పైకి, శూన్య *దత్తాంశం)
{
	నిర్మాణం task_సమూహం *మాతృ, *బిడ్డ;
	పూర్ణాంకం ret;

	మాతృ = from;

కిందికి:
	ret = (*కిందికి)(మాతృ, దత్తాంశం);
	ఒకవేళ (ret)
		వెళ్ళు బహిర్గతం;
	list_for_each_నమోదు_rcu(బిడ్డ, &మాతృ->children, siblings) {
		మాతృ = బిడ్డ;
		వెళ్ళు కిందికి;

పైకి:
		కొనసాగు;
	}
	ret = (*పైకి)(మాతృ, దత్తాంశం);
	ఒకవేళ (ret || మాతృ == from)
		వెళ్ళు బహిర్గతం;

	బిడ్డ = మాతృ;
	మాతృ = మాతృ->మాతృ;
	ఒకవేళ (మాతృ)
		వెళ్ళు పైకి;
బహిర్గతం:
	తిరిగి ret;
}

పూర్ణాంకం tg_nop(నిర్మాణం task_సమూహం *tg, శూన్య *దత్తాంశం)
{
	తిరిగి 0;
}
#అయితే_ముగింపు

శూన్య సమితి_ఎక్కించు_బరువు(నిర్మాణం task_struct *p, bool update_ఎక్కించు)
{
	పూర్ణాంకం prio = p->static_prio - MAX_RT_PRIO;
	నిర్మాణం ఎక్కించు_బరువు lw;

	ఒకవేళ (task_has_idle_policy(p)) {
		lw.బరువు = scale_ఎక్కించు(బరువు_IDLEPRIO);
		lw.inv_బరువు = WMULT_IDLEPRIO;
	} లేకపోతే {
		lw.బరువు = scale_ఎక్కించు(sched_prio_గమ్యం_బరువు[prio]);
		lw.inv_బరువు = sched_prio_గమ్యం_wmult[prio];
	}

	/*
	 * SCHED_OTHER tasks have to update their load when changing their
	 * weight
	 */
	ఒకవేళ (update_ఎక్కించు && p->sched_class->reweight_task)
		p->sched_class->reweight_task(task_rq(p), p, &lw);
	లేకపోతే
		p->se.ఎక్కించు = lw;
}

#నిర్వచితమైతే CONFIG_UCLAMP_TASK
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
స్థిర __maybe_unused DEFINE_MUTEX(uclamp_mutex);

/* Max allowed minimum utilization */
స్థిర గుర్తులేని పూర్ణాంకం __maybe_unused sysctl_sched_uclamp_util_min = SCHED_సామర్థ్యం_SCALE;

/* Max allowed maximum utilization */
స్థిర గుర్తులేని పూర్ణాంకం __maybe_unused sysctl_sched_uclamp_util_max = SCHED_సామర్థ్యం_SCALE;

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
గుర్తులేని పూర్ణాంకం sysctl_sched_uclamp_util_min_rt_default = SCHED_సామర్థ్యం_SCALE;

/* All clamps are required to be less or equal than these values */
స్థిర నిర్మాణం uclamp_se uclamp_default[UCLAMP_CNT];

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
DEFINE_STATIC_కీలకం_FALSE(sched_uclamp_వాడిన);

స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం
uclamp_idle_విలువ(నిర్మాణం rq *rq, గణన uclamp_id clamp_id,
		  గుర్తులేని పూర్ణాంకం clamp_విలువ)
{
	/*
	 * Avoid blocked utilization pushing up the frequency when we go
	 * idle (which drops the max-clamp) by retaining the last known
	 * max-clamp.
	 */
	ఒకవేళ (clamp_id == UCLAMP_MAX) {
		rq->uclamp_flags |= UCLAMP_FLAG_IDLE;
		తిరిగి clamp_విలువ;
	}

	తిరిగి uclamp_none(UCLAMP_MIN);
}

స్థిర అంతఃవరుస శూన్య uclamp_idle_reset(నిర్మాణం rq *rq, గణన uclamp_id clamp_id,
				     గుర్తులేని పూర్ణాంకం clamp_విలువ)
{
	/* Reset max-clamp retention only on idle exit */
	ఒకవేళ (!(rq->uclamp_flags & UCLAMP_FLAG_IDLE))
		తిరిగి;

	uclamp_rq_సమితి(rq, clamp_id, clamp_విలువ);
}

స్థిర అంతఃవరుస
గుర్తులేని పూర్ణాంకం uclamp_rq_max_విలువ(నిర్మాణం rq *rq, గణన uclamp_id clamp_id,
				   గుర్తులేని పూర్ణాంకం clamp_విలువ)
{
	నిర్మాణం uclamp_పాత్ర *పాత్ర = rq->uclamp[clamp_id].పాత్ర;
	పూర్ణాంకం పాత్ర_id = UCLAMP_BUCKETS - 1;

	/*
	 * Since both min and max clamps are max aggregated, find the
	 * top most bucket with tasks in.
	 */
	కొరకు ( ; పాత్ర_id >= 0; పాత్ర_id--) {
		ఒకవేళ (!పాత్ర[పాత్ర_id].tasks)
			కొనసాగు;
		తిరిగి పాత్ర[పాత్ర_id].విలువ;
	}

	/* No tasks -- default clamp values */
	తిరిగి uclamp_idle_విలువ(rq, clamp_id, clamp_విలువ);
}

స్థిర శూన్య __uclamp_update_util_min_rt_default(నిర్మాణం task_struct *p)
{
	గుర్తులేని పూర్ణాంకం default_util_min;
	నిర్మాణం uclamp_se *uc_se;

	lockdep_assert_held(&p->pi_తాళం);

	uc_se = &p->uclamp_req[UCLAMP_MIN];

	/* Only sync if user didn't override the default */
	ఒకవేళ (uc_se->user_defined)
		తిరిగి;

	default_util_min = sysctl_sched_uclamp_util_min_rt_default;
	uclamp_se_సమితి(uc_se, default_util_min, false);
}

స్థిర శూన్య uclamp_update_util_min_rt_default(నిర్మాణం task_struct *p)
{
	ఒకవేళ (!rt_task(p))
		తిరిగి;

	/* Protect updates to p->uclamp_* */
	guard(task_rq_తాళం_2)(p);
	__uclamp_update_util_min_rt_default(p);
}

స్థిర అంతఃవరుస నిర్మాణం uclamp_se
uclamp_tg_restrict(నిర్మాణం task_struct *p, గణన uclamp_id clamp_id)
{
	/* Copy by value as we could modify it */
	నిర్మాణం uclamp_se uc_req = p->uclamp_req[clamp_id];
#నిర్వచితమైతే CONFIG_UCLAMP_TASK_సమూహం
	గుర్తులేని పూర్ణాంకం tg_min, tg_max, విలువ;

	/*
	 * Tasks in autogroups or root task group will be
	 * restricted by system defaults.
	 */
	ఒకవేళ (task_సమూహం_is_autogroup(task_సమూహం(p)))
		తిరిగి uc_req;
	ఒకవేళ (task_సమూహం(p) == &మూలం_task_సమూహం)
		తిరిగి uc_req;

	tg_min = task_సమూహం(p)->uclamp[UCLAMP_MIN].విలువ;
	tg_max = task_సమూహం(p)->uclamp[UCLAMP_MAX].విలువ;
	విలువ = uc_req.విలువ;
	విలువ = clamp(విలువ, tg_min, tg_max);
	uclamp_se_సమితి(&uc_req, విలువ, false);
#అయితే_ముగింపు

	తిరిగి uc_req;
}

/*
 * The effective clamp bucket index of a task depends on, by increasing
 * priority:
 * - the task specific clamp value, when explicitly requested from userspace
 * - the task group effective clamp value, for tasks not either in the root
 *   group or in an autogroup
 * - the system default clamp value, defined by the sysadmin
 */
స్థిర అంతఃవరుస నిర్మాణం uclamp_se
uclamp_eff_get(నిర్మాణం task_struct *p, గణన uclamp_id clamp_id)
{
	నిర్మాణం uclamp_se uc_req = uclamp_tg_restrict(p, clamp_id);
	నిర్మాణం uclamp_se uc_max = uclamp_default[clamp_id];

	/* System default restrictions always apply */
	ఒకవేళ (unlikely(uc_req.విలువ > uc_max.విలువ))
		తిరిగి uc_max;

	తిరిగి uc_req;
}

గుర్తులేని దీర్ఘ uclamp_eff_విలువ(నిర్మాణం task_struct *p, గణన uclamp_id clamp_id)
{
	నిర్మాణం uclamp_se uc_eff;

	/* Task currently refcounted: use back-annotated (effective) value */
	ఒకవేళ (p->uclamp[clamp_id].చురుకైన)
		తిరిగి (గుర్తులేని దీర్ఘ)p->uclamp[clamp_id].విలువ;

	uc_eff = uclamp_eff_get(p, clamp_id);

	తిరిగి (గుర్తులేని దీర్ఘ)uc_eff.విలువ;
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
స్థిర అంతఃవరుస శూన్య uclamp_rq_inc_id(నిర్మాణం rq *rq, నిర్మాణం task_struct *p,
				    గణన uclamp_id clamp_id)
{
	నిర్మాణం uclamp_rq *uc_rq = &rq->uclamp[clamp_id];
	నిర్మాణం uclamp_se *uc_se = &p->uclamp[clamp_id];
	నిర్మాణం uclamp_పాత్ర *పాత్ర;

	lockdep_assert_rq_held(rq);

	/* Update task effective clamp */
	p->uclamp[clamp_id] = uclamp_eff_get(p, clamp_id);

	పాత్ర = &uc_rq->పాత్ర[uc_se->పాత్ర_id];
	పాత్ర->tasks++;
	uc_se->చురుకైన = true;

	uclamp_idle_reset(rq, clamp_id, uc_se->విలువ);

	/*
	 * Local max aggregation: rq buckets always track the max
	 * "requested" clamp value of its RUNNABLE tasks.
	 */
	ఒకవేళ (పాత్ర->tasks == 1 || uc_se->విలువ > పాత్ర->విలువ)
		పాత్ర->విలువ = uc_se->విలువ;

	ఒకవేళ (uc_se->విలువ > uclamp_rq_get(rq, clamp_id))
		uclamp_rq_సమితి(rq, clamp_id, uc_se->విలువ);
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
స్థిర అంతఃవరుస శూన్య uclamp_rq_dec_id(నిర్మాణం rq *rq, నిర్మాణం task_struct *p,
				    గణన uclamp_id clamp_id)
{
	నిర్మాణం uclamp_rq *uc_rq = &rq->uclamp[clamp_id];
	నిర్మాణం uclamp_se *uc_se = &p->uclamp[clamp_id];
	నిర్మాణం uclamp_పాత్ర *పాత్ర;
	గుర్తులేని పూర్ణాంకం bkt_clamp;
	గుర్తులేని పూర్ణాంకం rq_clamp;

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
	ఒకవేళ (unlikely(!uc_se->చురుకైన))
		తిరిగి;

	పాత్ర = &uc_rq->పాత్ర[uc_se->పాత్ర_id];

	WARN_ON_ONCE(!పాత్ర->tasks);
	ఒకవేళ (likely(పాత్ర->tasks))
		పాత్ర->tasks--;

	uc_se->చురుకైన = false;

	/*
	 * Keep "local max aggregation" simple and accept to (possibly)
	 * overboost some RUNNABLE tasks in the same bucket.
	 * The rq clamp bucket value is reset to its base value whenever
	 * there are no more RUNNABLE tasks refcounting it.
	 */
	ఒకవేళ (likely(పాత్ర->tasks))
		తిరిగి;

	rq_clamp = uclamp_rq_get(rq, clamp_id);
	/*
	 * Defensive programming: this should never happen. If it happens,
	 * e.g. due to future modification, warn and fix up the expected value.
	 */
	WARN_ON_ONCE(పాత్ర->విలువ > rq_clamp);
	ఒకవేళ (పాత్ర->విలువ >= rq_clamp) {
		bkt_clamp = uclamp_rq_max_విలువ(rq, clamp_id, uc_se->విలువ);
		uclamp_rq_సమితి(rq, clamp_id, bkt_clamp);
	}
}

స్థిర అంతఃవరుస శూన్య uclamp_rq_inc(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	గణన uclamp_id clamp_id;

	/*
	 * Avoid any overhead until uclamp is actually used by the userspace.
	 *
	 * The condition is constructed such that a NOP is generated when
	 * sched_uclamp_used is disabled.
	 */
	ఒకవేళ (!uclamp_is_వాడిన())
		తిరిగి;

	ఒకవేళ (unlikely(!p->sched_class->uclamp_enabled))
		తిరిగి;

	/* Only inc the delayed task which being woken up. */
	ఒకవేళ (p->se.sched_delayed && !(flags & వరుసలో_చేర్చు_DELAYED))
		తిరిగి;

	for_each_clamp_id(clamp_id)
		uclamp_rq_inc_id(rq, p, clamp_id);

	/* Reset clamp idle holding when there is one RUNNABLE task */
	ఒకవేళ (rq->uclamp_flags & UCLAMP_FLAG_IDLE)
		rq->uclamp_flags &= ~UCLAMP_FLAG_IDLE;
}

స్థిర అంతఃవరుస శూన్య uclamp_rq_dec(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	గణన uclamp_id clamp_id;

	/*
	 * Avoid any overhead until uclamp is actually used by the userspace.
	 *
	 * The condition is constructed such that a NOP is generated when
	 * sched_uclamp_used is disabled.
	 */
	ఒకవేళ (!uclamp_is_వాడిన())
		తిరిగి;

	ఒకవేళ (unlikely(!p->sched_class->uclamp_enabled))
		తిరిగి;

	ఒకవేళ (p->se.sched_delayed)
		తిరిగి;

	for_each_clamp_id(clamp_id)
		uclamp_rq_dec_id(rq, p, clamp_id);
}

స్థిర అంతఃవరుస శూన్య uclamp_rq_reinc_id(నిర్మాణం rq *rq, నిర్మాణం task_struct *p,
				      గణన uclamp_id clamp_id)
{
	ఒకవేళ (!p->uclamp[clamp_id].చురుకైన)
		తిరిగి;

	uclamp_rq_dec_id(rq, p, clamp_id);
	uclamp_rq_inc_id(rq, p, clamp_id);

	/*
	 * Make sure to clear the idle flag if we've transiently reached 0
	 * active tasks on rq.
	 */
	ఒకవేళ (clamp_id == UCLAMP_MAX && (rq->uclamp_flags & UCLAMP_FLAG_IDLE))
		rq->uclamp_flags &= ~UCLAMP_FLAG_IDLE;
}

స్థిర అంతఃవరుస శూన్య
uclamp_update_చురుకైన(నిర్మాణం task_struct *p)
{
	గణన uclamp_id clamp_id;
	నిర్మాణం rq_flags rf;
	నిర్మాణం rq *rq;

	/*
	 * Lock the task and the rq where the task is (or was) queued.
	 *
	 * We might lock the (previous) rq of a !RUNNABLE task, but that's the
	 * price to pay to safely serialize util_{min,max} updates with
	 * enqueues, dequeues and migration operations.
	 * This is the same locking schema used by __set_cpus_allowed_ptr().
	 */
	rq = task_rq_తాళం_2(p, &rf);

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

#నిర్వచితమైతే CONFIG_UCLAMP_TASK_సమూహం
స్థిర అంతఃవరుస శూన్య
uclamp_update_చురుకైన_tasks(నిర్మాణం cgroup_subsys_state *css)
{
	నిర్మాణం css_task_iter it;
	నిర్మాణం task_struct *p;

	css_task_iter_ప్రారంభం(css, 0, &it);
	అయితే ((p = css_task_iter_తదుపరి(&it)))
		uclamp_update_చురుకైన(p);
	css_task_iter_చివర(&it);
}

స్థిర శూన్య cpu_util_update_eff(నిర్మాణం cgroup_subsys_state *css);
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_SYSCTL
#నిర్వచితమైతే CONFIG_UCLAMP_TASK_సమూహం
స్థిర శూన్య uclamp_update_మూలం_tg(శూన్య)
{
	నిర్మాణం task_సమూహం *tg = &మూలం_task_సమూహం;

	uclamp_se_సమితి(&tg->uclamp_req[UCLAMP_MIN],
		      sysctl_sched_uclamp_util_min, false);
	uclamp_se_సమితి(&tg->uclamp_req[UCLAMP_MAX],
		      sysctl_sched_uclamp_util_max, false);

	guard(rcu)();
	cpu_util_update_eff(&మూలం_task_సమూహం.css);
}
#లేకపోతే
స్థిర శూన్య uclamp_update_మూలం_tg(శూన్య) { }
#అయితే_ముగింపు

స్థిర శూన్య uclamp_sync_util_min_rt_default(శూన్య)
{
	నిర్మాణం task_struct *g, *p;

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
	read_తాళం(&tasklist_తాళం);
	smp_mb__after_spinlock();
	read_unlock(&tasklist_తాళం);

	guard(rcu)();
	for_each_process_thread(g, p)
		uclamp_update_util_min_rt_default(p);
}

స్థిర పూర్ణాంకం sysctl_sched_uclamp_handler(స్థిరరాశి నిర్మాణం ctl_పట్టిక *పట్టిక, పూర్ణాంకం లిఖ,
				శూన్య *బఫరు, కొలత_t *lenp, loff_t *ppos)
{
	bool update_మూలం_tg = false;
	పూర్ణాంకం old_min, old_max, old_min_rt;
	పూర్ణాంకం ఫలితం;

	guard(mutex)(&uclamp_mutex);

	old_min = sysctl_sched_uclamp_util_min;
	old_max = sysctl_sched_uclamp_util_max;
	old_min_rt = sysctl_sched_uclamp_util_min_rt_default;

	ఫలితం = proc_dointvec(పట్టిక, లిఖ, బఫరు, lenp, ppos);
	ఒకవేళ (ఫలితం)
		వెళ్ళు undo;
	ఒకవేళ (!లిఖ)
		తిరిగి 0;

	ఒకవేళ (sysctl_sched_uclamp_util_min > sysctl_sched_uclamp_util_max ||
	    sysctl_sched_uclamp_util_max > SCHED_సామర్థ్యం_SCALE	||
	    sysctl_sched_uclamp_util_min_rt_default > SCHED_సామర్థ్యం_SCALE) {

		ఫలితం = -EINVAL;
		వెళ్ళు undo;
	}

	ఒకవేళ (old_min != sysctl_sched_uclamp_util_min) {
		uclamp_se_సమితి(&uclamp_default[UCLAMP_MIN],
			      sysctl_sched_uclamp_util_min, false);
		update_మూలం_tg = true;
	}
	ఒకవేళ (old_max != sysctl_sched_uclamp_util_max) {
		uclamp_se_సమితి(&uclamp_default[UCLAMP_MAX],
			      sysctl_sched_uclamp_util_max, false);
		update_మూలం_tg = true;
	}

	ఒకవేళ (update_మూలం_tg) {
		sched_uclamp_enable();
		uclamp_update_మూలం_tg();
	}

	ఒకవేళ (old_min_rt != sysctl_sched_uclamp_util_min_rt_default) {
		sched_uclamp_enable();
		uclamp_sync_util_min_rt_default();
	}

	/*
	 * We update all RUNNABLE tasks only when task groups are in use.
	 * Otherwise, keep it simple and do just a lazy update at each next
	 * task enqueue time.
	 */
	తిరిగి 0;

undo:
	sysctl_sched_uclamp_util_min = old_min;
	sysctl_sched_uclamp_util_max = old_max;
	sysctl_sched_uclamp_util_min_rt_default = old_min_rt;
	తిరిగి ఫలితం;
}
#అయితే_ముగింపు /* CONFIG_SYSCTL */

స్థిర శూన్య uclamp_fork(నిర్మాణం task_struct *p)
{
	గణన uclamp_id clamp_id;

	/*
	 * We don't need to hold task_rq_lock() when updating p->uclamp_* here
	 * as the task is still at its early fork stages.
	 */
	for_each_clamp_id(clamp_id)
		p->uclamp[clamp_id].చురుకైన = false;

	ఒకవేళ (likely(!p->sched_reset_on_fork))
		తిరిగి;

	for_each_clamp_id(clamp_id) {
		uclamp_se_సమితి(&p->uclamp_req[clamp_id],
			      uclamp_none(clamp_id), false);
	}
}

స్థిర శూన్య uclamp_post_fork(నిర్మాణం task_struct *p)
{
	uclamp_update_util_min_rt_default(p);
}

స్థిర శూన్య __init init_uclamp_rq(నిర్మాణం rq *rq)
{
	గణన uclamp_id clamp_id;
	నిర్మాణం uclamp_rq *uc_rq = rq->uclamp;

	for_each_clamp_id(clamp_id) {
		uc_rq[clamp_id] = (నిర్మాణం uclamp_rq) {
			.విలువ = uclamp_none(clamp_id)
		};
	}

	rq->uclamp_flags = UCLAMP_FLAG_IDLE;
}

స్థిర శూన్య __init init_uclamp(శూన్య)
{
	నిర్మాణం uclamp_se uc_max = {};
	గణన uclamp_id clamp_id;
	పూర్ణాంకం cpu;

	for_each_possible_cpu(cpu)
		init_uclamp_rq(cpu_rq(cpu));

	for_each_clamp_id(clamp_id) {
		uclamp_se_సమితి(&init_task.uclamp_req[clamp_id],
			      uclamp_none(clamp_id), false);
	}

	/* System defaults allow max clamp values for both indexes */
	uclamp_se_సమితి(&uc_max, uclamp_none(UCLAMP_MAX), false);
	for_each_clamp_id(clamp_id) {
		uclamp_default[clamp_id] = uc_max;
#నిర్వచితమైతే CONFIG_UCLAMP_TASK_సమూహం
		మూలం_task_సమూహం.uclamp_req[clamp_id] = uc_max;
		మూలం_task_సమూహం.uclamp[clamp_id] = uc_max;
#అయితే_ముగింపు
	}
}

#లేకపోతే /* !CONFIG_UCLAMP_TASK: */
స్థిర అంతఃవరుస శూన్య uclamp_rq_inc(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags) { }
స్థిర అంతఃవరుస శూన్య uclamp_rq_dec(నిర్మాణం rq *rq, నిర్మాణం task_struct *p) { }
స్థిర అంతఃవరుస శూన్య uclamp_fork(నిర్మాణం task_struct *p) { }
స్థిర అంతఃవరుస శూన్య uclamp_post_fork(నిర్మాణం task_struct *p) { }
స్థిర అంతఃవరుస శూన్య init_uclamp(శూన్య) { }
#అయితే_ముగింపు /* !CONFIG_UCLAMP_TASK */

bool sched_task_on_rq(నిర్మాణం task_struct *p)
{
	తిరిగి task_on_rq_queued(p);
}

గుర్తులేని దీర్ఘ get_wchan(నిర్మాణం task_struct *p)
{
	గుర్తులేని దీర్ఘ ip = 0;
	గుర్తులేని పూర్ణాంకం state;

	ఒకవేళ (!p || p == current)
		తిరిగి 0;

	/* Only get wchan if task is blocked and we can keep it that way. */
	raw_spin_తాళం_irq(&p->pi_తాళం);
	state = READ_ONCE(p->__state);
	smp_rmb(); /* see try_to_wake_up() */
	ఒకవేళ (state != TASK_RUNNING && state != TASK_WAKING && !p->on_rq)
		ip = __get_wchan(p);
	raw_spin_unlock_irq(&p->pi_తాళం);

	తిరిగి ip;
}

శూన్య వరుసలో_చేర్చు_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	ఒకవేళ (!(flags & వరుసలో_చేర్చు_NOCLOCK))
		update_rq_clock(rq);

	/*
	 * Can be before ->enqueue_task() because uclamp considers the
	 * ENQUEUE_DELAYED task before its ->sched_delayed gets cleared
	 * in ->enqueue_task().
	 */
	uclamp_rq_inc(rq, p, flags);

	p->sched_class->వరుసలో_చేర్చు_task(rq, p, flags);

	psi_వరుసలో_చేర్చు(p, flags);

	ఒకవేళ (!(flags & వరుసలో_చేర్చు_RESTORE))
		sched_info_వరుసలో_చేర్చు(rq, p);

	ఒకవేళ (sched_core_enabled(rq))
		sched_core_వరుసలో_చేర్చు(rq, p);
}

/*
 * Must only return false when DEQUEUE_SLEEP.
 */
అంతఃవరుస bool వరుస_నుండి_తీయి_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	ఒకవేళ (sched_core_enabled(rq))
		sched_core_వరుస_నుండి_తీయి(rq, p, flags);

	ఒకవేళ (!(flags & వరుస_నుండి_తీయి_NOCLOCK))
		update_rq_clock(rq);

	ఒకవేళ (!(flags & వరుస_నుండి_తీయి_SAVE))
		sched_info_వరుస_నుండి_తీయి(rq, p);

	psi_వరుస_నుండి_తీయి(p, flags);

	/*
	 * Must be before ->dequeue_task() because ->dequeue_task() can 'fail'
	 * and mark the task ->sched_delayed.
	 */
	uclamp_rq_dec(rq, p);
	తిరిగి p->sched_class->వరుస_నుండి_తీయి_task(rq, p, flags);
}

శూన్య activate_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	ఒకవేళ (task_on_rq_migrating(p))
		flags |= వరుసలో_చేర్చు_MIGRATED;

	వరుసలో_చేర్చు_task(rq, p, flags);

	WRITE_ONCE(p->on_rq, TASK_ON_RQ_QUEUED);
	ASSERT_EXCLUSIVE_WRITER(p->on_rq);
}

శూన్య deactivate_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	WARN_ON_ONCE(flags & వరుస_నుండి_తీయి_SLEEP);

	WRITE_ONCE(p->on_rq, TASK_ON_RQ_MIGRATING);
	ASSERT_EXCLUSIVE_WRITER(p->on_rq);

	/*
	 * Code explicitly relies on TASK_ON_RQ_MIGRATING begin set *before*
	 * dequeue_task() and cleared *after* enqueue_task().
	 */

	వరుస_నుండి_తీయి_task(rq, p, flags);
}

స్థిర bool వరుస_నుండి_తీయి_ఖండం_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p,
			       గుర్తులేని దీర్ఘ task_state)
{
	పూర్ణాంకం flags = వరుస_నుండి_తీయి_NOCLOCK;

	p->sched_contributes_గమ్యం_ఎక్కించు =
		(task_state & TASK_UNINTERRUPTIBLE) &&
		!(task_state & TASK_NOLOAD) &&
		!(task_state & TASK_FROZEN);

	ఒకవేళ (unlikely(is_special_task_state(task_state)))
		flags |= వరుస_నుండి_తీయి_SPECIAL;

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
	తిరిగి వరుస_నుండి_తీయి_task(rq, p, వరుస_నుండి_తీయి_SLEEP | flags);
}

స్థిర శూన్య ఖండం_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, గుర్తులేని దీర్ఘ task_state)
{
	ఒకవేళ (వరుస_నుండి_తీయి_ఖండం_task(rq, p, task_state))
		__block_task(rq, p);
}

/**
 * task_curr - is this task currently executing on a CPU?
 * @p: the task in question.
 *
 * Return: 1 if the task is currently executing. 0 otherwise.
 */
అంతఃవరుస పూర్ణాంకం task_curr(స్థిరరాశి నిర్మాణం task_struct *p)
{
	తిరిగి cpu_curr(task_cpu(p)) == p;
}

శూన్య wakeup_preempt(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	నిర్మాణం task_struct *donor = rq->donor;

	ఒకవేళ (p->sched_class == rq->తదుపరి_class) {
		rq->తదుపరి_class->wakeup_preempt(rq, p, flags);

	} లేకపోతే ఒకవేళ (sched_class_above(p->sched_class, rq->తదుపరి_class)) {
		rq->తదుపరి_class->wakeup_preempt(rq, p, flags);
		resched_curr(rq);
		rq->తదుపరి_class = p->sched_class;
	}

	/*
	 * A queue event has occurred, and we're going to schedule.  In
	 * this case, we can save a useless back to back clock update.
	 */
	ఒకవేళ (task_on_rq_queued(donor) && test_tsk_need_resched(rq->curr))
		rq_clock_skip_update(rq);
}

స్థిర __always_inline
పూర్ణాంకం __task_state_match(నిర్మాణం task_struct *p, గుర్తులేని పూర్ణాంకం state)
{
	ఒకవేళ (READ_ONCE(p->__state) & state)
		తిరిగి 1;

	ఒకవేళ (READ_ONCE(p->saved_state) & state)
		తిరిగి -1;

	తిరిగి 0;
}

స్థిర __always_inline
పూర్ణాంకం task_state_సరిపోలిక(నిర్మాణం task_struct *p, గుర్తులేని పూర్ణాంకం state)
{
	/*
	 * Serialize against current_save_and_set_rtlock_wait_state(),
	 * current_restore_rtlock_saved_state(), and __refrigerator().
	 */
	guard(raw_spinlock_irq)(&p->pi_తాళం);
	తిరిగి __task_state_match(p, state);
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
గుర్తులేని దీర్ఘ wait_task_inactive(నిర్మాణం task_struct *p, గుర్తులేని పూర్ణాంకం సరిపోలిక_state)
{
	పూర్ణాంకం running, queued, సరిపోలిక;
	నిర్మాణం rq_flags rf;
	గుర్తులేని దీర్ఘ ncsw;
	నిర్మాణం rq *rq;

	కొరకు (;;) {
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
		అయితే (task_on_cpu(rq, p)) {
			ఒకవేళ (!task_state_సరిపోలిక(p, సరిపోలిక_state))
				తిరిగి 0;
			cpu_relax();
		}

		/*
		 * Ok, time to look more closely! We need the rq
		 * lock now, to be *sure*. If we're wrong, we'll
		 * just go back and repeat.
		 */
		rq = task_rq_తాళం_2(p, &rf);
		/*
		 * If task is sched_delayed, force dequeue it, to avoid always
		 * hitting the tick timeout in the queued case
		 */
		ఒకవేళ (p->se.sched_delayed)
			వరుస_నుండి_తీయి_task(rq, p, వరుస_నుండి_తీయి_SLEEP | వరుస_నుండి_తీయి_DELAYED);
		trace_sched_wait_task(p);
		running = task_on_cpu(rq, p);
		queued = task_on_rq_queued(p);
		ncsw = 0;
		ఒకవేళ ((సరిపోలిక = __task_state_match(p, సరిపోలిక_state))) {
			/*
			 * When matching on p->saved_state, consider this task
			 * still queued so it will wait.
			 */
			ఒకవేళ (సరిపోలిక < 0)
				queued = 1;
			ncsw = p->nvcsw | LONG_MIN; /* sets MSB */
		}
		task_rq_unlock(rq, p, &rf);

		/*
		 * If it changed from the expected state, bail out now.
		 */
		ఒకవేళ (unlikely(!ncsw))
			ఆపు;

		/*
		 * Was it really running after all now that we
		 * checked with the proper locks actually held?
		 *
		 * Oops. Go back and try again..
		 */
		ఒకవేళ (unlikely(running)) {
			cpu_relax();
			కొనసాగు;
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
		ఒకవేళ (unlikely(queued)) {
			ktime_t గమ్యం = NSEC_చొప్పున_SEC / HZ;

			సమితి_current_state(TASK_UNINTERRUPTIBLE);
			కాలపట్టిక_hrtimeout(&గమ్యం, HRTIMER_బహుళకం_REL_HARD);
			కొనసాగు;
		}

		/*
		 * Ahh, all good. It wasn't running, and it wasn't
		 * runnable, which means that it will never become
		 * running in the future either. We're all done!
		 */
		ఆపు;
	}

	తిరిగి ncsw;
}

స్థిర శూన్య
do_సమితి_cpus_allowed(నిర్మాణం task_struct *p, నిర్మాణం affinity_context *ctx);

స్థిర శూన్య migrate_disable_switch(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	నిర్మాణం affinity_context ac = {
		.new_ముసుగు  = cpumask_యొక్క(rq->cpu),
		.flags     = SCA_MIGRATE_DISABLE,
	};

	ఒకవేళ (likely(!p->migration_disabled))
		తిరిగి;

	ఒకవేళ (p->cpus_ptr != &p->cpus_ముసుగు)
		తిరిగి;

	scoped_guard (task_rq_తాళం_2, p)
		do_సమితి_cpus_allowed(p, &ac);
}

శూన్య ___migrate_enable(శూన్య)
{
	నిర్మాణం task_struct *p = current;
	నిర్మాణం affinity_context ac = {
		.new_ముసుగు  = &p->cpus_ముసుగు,
		.flags     = SCA_MIGRATE_ENABLE,
	};

	__set_cpus_allowed_ptr(p, &ac);
}
EXPORT_చిహ్నం_GPL(___migrate_enable);

శూన్య migrate_disable(శూన్య)
{
	__migrate_disable();
}
EXPORT_చిహ్నం_GPL(migrate_disable);

శూన్య migrate_enable(శూన్య)
{
	__migrate_enable();
}
EXPORT_చిహ్నం_GPL(migrate_enable);

స్థిర అంతఃవరుస bool rq_has_pinned_tasks(నిర్మాణం rq *rq)
{
	తిరిగి rq->nr_pinned;
}

స్థిర అంతఃవరుస bool task_can_migrate_గమ్యం_preferred(నిర్మాణం task_struct *p, పూర్ణాంకం cpu)
{
	/* No need to migrate from a preferred CPU */
	ఒకవేళ (cpu_preferred(cpu))
		తిరిగి false;

	/* Only FAIR tasks honor preferred CPU state */
	ఒకవేళ (unlikely(p->sched_class != &fair_sched_class))
		తిరిగి false;

	/* Ignore preferred state if task affinity is changing */
	ఒకవేళ (unlikely(!cpumask_test_cpu(task_cpu(p), p->cpus_ptr)))
		తిరిగి false;

	తిరిగి cpumask_intersects_and(p->cpus_ptr, cpu_preferred_ముసుగు,
				      task_cpu_possible_ముసుగు(p));
}

/*
 * Per-CPU kthreads are allowed to run on !active && online CPUs, see
 * __set_cpus_allowed_ptr() and select_fallback_rq().
 */
స్థిర అంతఃవరుస bool is_cpu_allowed(నిర్మాణం task_struct *p, పూర్ణాంకం cpu)
{
	/* When not in the task's cpumask, no point in looking further. */
	ఒకవేళ (!task_allowed_on_cpu(p, cpu))
		తిరిగి false;

	/* migrate_disabled() must be allowed to finish. */
	ఒకవేళ (is_migration_disabled(p))
		తిరిగి cpu_online(cpu);

	/* Non kernel threads are not allowed during either online or offline. */
	ఒకవేళ (!(p->flags & PF_KTHREAD)) {
		/* Try to use preferred CPU if task's affinity allows */
		ఒకవేళ (task_can_migrate_గమ్యం_preferred(p, cpu))
			తిరిగి false;
		తిరిగి cpu_చురుకైన(cpu);
	}

	/* KTHREAD_IS_PER_CPU is always allowed. */
	ఒకవేళ (kthread_is_చొప్పున_cpu(p))
		తిరిగి cpu_online(cpu);

	/* Regular kernel threads don't get to stay during offline. */
	ఒకవేళ (cpu_dying(cpu))
		తిరిగి false;

	/* Try to keep unbound kthreads on a preferred CPU if possible. */
	ఒకవేళ (task_can_migrate_గమ్యం_preferred(p, cpu))
		తిరిగి false;

	/* Otherwise, they are allowed to run on online CPU. */
	తిరిగి cpu_online(cpu);
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
స్థిర నిర్మాణం rq *move_queued_task(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf,
				   నిర్మాణం task_struct *p, పూర్ణాంకం new_cpu)
	__must_hold(__rq_lockp(rq))
{
	lockdep_assert_rq_held(rq);

	deactivate_task(rq, p, వరుస_నుండి_తీయి_NOCLOCK);
	సమితి_task_cpu(p, new_cpu);
	rq_unlock(rq, rf);

	rq = cpu_rq(new_cpu);

	rq_తాళం(rq, rf);
	WARN_ON_ONCE(task_cpu(p) != new_cpu);
	activate_task(rq, p, 0);
	wakeup_preempt(rq, p, 0);

	తిరిగి rq;
}

నిర్మాణం migration_arg {
	నిర్మాణం task_struct		*task;
	పూర్ణాంకం				dest_cpu;
	నిర్మాణం సమితి_affinity_pending	*pending;
};

/*
 * @refs: number of wait_for_completion()
 * @stop_pending: is @stop_work in use
 */
నిర్మాణం సమితి_affinity_pending {
	refcount_t		refs;
	గుర్తులేని పూర్ణాంకం		stop_pending;
	నిర్మాణం completion	పూర్తి;
	నిర్మాణం cpu_stop_work	stop_work;
	నిర్మాణం migration_arg	arg;
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
స్థిర నిర్మాణం rq *__migrate_task(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf,
				 నిర్మాణం task_struct *p, పూర్ణాంకం dest_cpu)
	__must_hold(__rq_lockp(rq))
{
	/* Affinity changed (again). */
	ఒకవేళ (!is_cpu_allowed(p, dest_cpu))
		తిరిగి rq;

	rq = move_queued_task(rq, rf, p, dest_cpu);

	తిరిగి rq;
}

/*
 * migration_cpu_stop - this will be executed by a high-prio stopper thread
 * and performs thread migration by bumping thread off CPU then
 * 'pushing' onto another runqueue.
 */
స్థిర పూర్ణాంకం migration_cpu_stop(శూన్య *దత్తాంశం)
{
	నిర్మాణం migration_arg *arg = దత్తాంశం;
	నిర్మాణం సమితి_affinity_pending *pending = arg->pending;
	నిర్మాణం task_struct *p = arg->task;
	నిర్మాణం rq *rq = this_rq();
	bool complete = false;
	నిర్మాణం rq_flags rf;

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
	flush_smp_call_function_వరుస();

	/*
	 * We may change the underlying rq, but the locks held will
	 * appropriately be "transferred" when switching.
	 */
	context_unsafe_alias(rq);

	raw_spin_తాళం(&p->pi_తాళం);
	rq_తాళం(rq, &rf);

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
	ఒకవేళ (task_rq(p) == rq) {
		ఒకవేళ (is_migration_disabled(p))
			వెళ్ళు బహిర్గతం;

		ఒకవేళ (pending) {
			p->migration_pending = ఖాళీ;
			complete = true;

			ఒకవేళ (cpumask_test_cpu(task_cpu(p), &p->cpus_ముసుగు))
				వెళ్ళు బహిర్గతం;
		}

		ఒకవేళ (task_on_rq_queued(p)) {
			update_rq_clock(rq);
			rq = __migrate_task(rq, &rf, p, arg->dest_cpu);
		} లేకపోతే {
			p->wake_cpu = arg->dest_cpu;
		}

		/*
		 * XXX __migrate_task() can fail, at which point we might end
		 * up running on a dodgy CPU, AFAICT this can only happen
		 * during CPU hotplug, at which point we'll get pushed out
		 * anyway, so it's probably not a big deal.
		 */

	} లేకపోతే ఒకవేళ (pending) {
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
		ఒకవేళ (cpumask_test_cpu(task_cpu(p), p->cpus_ptr)) {
			p->migration_pending = ఖాళీ;
			complete = true;
			వెళ్ళు బహిర్గతం;
		}

		/*
		 * When migrate_enable() hits a rq mis-match we can't reliably
		 * determine is_migration_disabled() and so have to chase after
		 * it.
		 */
		WARN_ON_ONCE(!pending->stop_pending);
		preempt_disable();
		rq_unlock(rq, &rf);
		raw_spin_unlock_irqrestore(&p->pi_తాళం, rf.flags);
		stop_one_cpu_nowait(task_cpu(p), migration_cpu_stop,
				    &pending->arg, &pending->stop_work);
		preempt_enable();
		తిరిగి 0;
	}
బహిర్గతం:
	ఒకవేళ (pending)
		pending->stop_pending = false;
	rq_unlock(rq, &rf);
	raw_spin_unlock_irqrestore(&p->pi_తాళం, rf.flags);

	ఒకవేళ (complete)
		complete_all(&pending->పూర్తి);

	తిరిగి 0;
}

పూర్ణాంకం నెట్టు_cpu_stop(శూన్య *arg)
{
	నిర్మాణం rq *అత్యల్ప_rq = ఖాళీ, *rq = this_rq();
	నిర్మాణం task_struct *p = arg;

	raw_spin_తాళం_irq(&p->pi_తాళం);
	raw_spin_rq_తాళం(rq);

	ఒకవేళ (task_rq(p) != rq)
		వెళ్ళు బహిర్గతం_unlock;

	ఒకవేళ (is_migration_disabled(p)) {
		p->migration_flags |= MDF_నెట్టు;
		వెళ్ళు బహిర్గతం_unlock;
	}

	p->migration_flags &= ~MDF_నెట్టు;

	ఒకవేళ (p->sched_class->కనుగొను_తాళం_rq)
		అత్యల్ప_rq = p->sched_class->కనుగొను_తాళం_rq(p, rq);

	ఒకవేళ (!అత్యల్ప_rq)
		వెళ్ళు బహిర్గతం_unlock;

	lockdep_assert_rq_held(అత్యల్ప_rq);

	// XXX validate p is still the highest prio task
	ఒకవేళ (task_rq(p) == rq) {
		move_queued_task_locked(rq, అత్యల్ప_rq, p);
		resched_curr(అత్యల్ప_rq);
	}

	double_unlock_balance(rq, అత్యల్ప_rq);

బహిర్గతం_unlock:
	rq->నెట్టు_busy = false;
	raw_spin_rq_unlock(rq);
	raw_spin_unlock_irq(&p->pi_తాళం);

	put_task_struct(p);
	తిరిగి 0;
}

స్థిర అంతఃవరుస శూన్య mm_update_cpus_allowed(నిర్మాణం mm_struct *mm, స్థిరరాశి cpumask_t *affmask);

/*
 * sched_class::set_cpus_allowed must do the below, but is not required to
 * actually call this function.
 */
శూన్య సమితి_cpus_allowed_common(నిర్మాణం task_struct *p, నిర్మాణం affinity_context *ctx)
{
	ఒకవేళ (ctx->flags & (SCA_MIGRATE_ENABLE | SCA_MIGRATE_DISABLE)) {
		p->cpus_ptr = ctx->new_ముసుగు;
		తిరిగి;
	}

	cpumask_copy(&p->cpus_ముసుగు, ctx->new_ముసుగు);
	p->nr_cpus_allowed = cpumask_బరువు(ctx->new_ముసుగు);
	mm_update_cpus_allowed(p->mm, ctx->new_ముసుగు);

	/*
	 * Swap in a new user_cpus_ptr if SCA_USER flag set
	 */
	ఒకవేళ (ctx->flags & SCA_USER)
		మార్పిడి(p->user_cpus_ptr, ctx->user_ముసుగు);
}

స్థిర శూన్య
do_సమితి_cpus_allowed(నిర్మాణం task_struct *p, నిర్మాణం affinity_context *ctx)
{
	scoped_guard (sched_change, p, వరుస_నుండి_తీయి_SAVE)
		p->sched_class->సమితి_cpus_allowed(p, ctx);
}

/*
 * Used for kthread_bind() and select_fallback_rq(), in both cases the user
 * affinity (if any) should be destroyed too.
 */
శూన్య సమితి_cpus_allowed_force(నిర్మాణం task_struct *p, స్థిరరాశి నిర్మాణం cpumask *new_ముసుగు)
{
	నిర్మాణం affinity_context ac = {
		.new_ముసుగు  = new_ముసుగు,
		.user_ముసుగు = ఖాళీ,
		.flags     = SCA_USER,	/* clear the user requested mask */
	};
	సంఘం cpumask_rcuhead {
		cpumask_t cpumask;
		నిర్మాణం rcu_తల rcu;
	};

	scoped_guard (__task_rq_lock, p)
		do_సమితి_cpus_allowed(p, &ac);

	/*
	 * Because this is called with p->pi_lock held, it is not possible
	 * to use kfree() here (when PREEMPT_RT=y), therefore punt to using
	 * kfree_rcu().
	 */
	kfree_rcu((సంఘం cpumask_rcuhead *)ac.user_ముసుగు, rcu);
}

పూర్ణాంకం dup_user_cpus_ptr(నిర్మాణం task_struct *dst, నిర్మాణం task_struct *src,
		      పూర్ణాంకం కణుపు)
{
	cpumask_t *user_ముసుగు;
	గుర్తులేని దీర్ఘ flags;

	/*
	 * Always clear dst->user_cpus_ptr first as their user_cpus_ptr's
	 * may differ by now due to racing.
	 */
	dst->user_cpus_ptr = ఖాళీ;

	/*
	 * This check is racy and losing the race is a valid situation.
	 * It is not worth the extra overhead of taking the pi_lock on
	 * every fork/clone.
	 */
	ఒకవేళ (దత్తాంశం_race(!src->user_cpus_ptr))
		తిరిగి 0;

	user_ముసుగు = alloc_user_cpus_ptr(కణుపు);
	ఒకవేళ (!user_ముసుగు)
		తిరిగి -ENOMEM;

	/*
	 * Use pi_lock to protect content of user_cpus_ptr
	 *
	 * Though unlikely, user_cpus_ptr can be reset to NULL by a concurrent
	 * set_cpus_allowed_force().
	 */
	raw_spin_తాళం_irqsave(&src->pi_తాళం, flags);
	ఒకవేళ (src->user_cpus_ptr) {
		మార్పిడి(dst->user_cpus_ptr, user_ముసుగు);
		cpumask_copy(dst->user_cpus_ptr, src->user_cpus_ptr);
	}
	raw_spin_unlock_irqrestore(&src->pi_తాళం, flags);

	ఒకవేళ (unlikely(user_ముసుగు))
		kfree(user_ముసుగు);

	తిరిగి 0;
}

స్థిర అంతఃవరుస నిర్మాణం cpumask *clear_user_cpus_ptr(నిర్మాణం task_struct *p)
{
	నిర్మాణం cpumask *user_ముసుగు = ఖాళీ;

	మార్పిడి(p->user_cpus_ptr, user_ముసుగు);

	తిరిగి user_ముసుగు;
}

శూన్య release_user_cpus_ptr(నిర్మాణం task_struct *p)
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
స్థిర పూర్ణాంకం affine_move_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, నిర్మాణం rq_flags *rf,
			    పూర్ణాంకం dest_cpu, గుర్తులేని పూర్ణాంకం flags)
	__releases(__rq_lockp(rq), &p->pi_తాళం)
{
	నిర్మాణం సమితి_affinity_pending my_pending = { }, *pending = ఖాళీ;
	bool stop_pending, complete = false;

	/*
	 * Can the task run on the task's current CPU? If so, we're done
	 *
	 * We are also done if the task is the current donor, boosting a lock-
	 * holding proxy, (and potentially has been migrated outside its
	 * current or previous affinity mask)
	 */
	ఒకవేళ (cpumask_test_cpu(task_cpu(p), &p->cpus_ముసుగు) ||
	    (task_current_donor(rq, p) && !task_current(rq, p))) {
		నిర్మాణం task_struct *నెట్టు_task = ఖాళీ;

		ఒకవేళ ((flags & SCA_MIGRATE_ENABLE) &&
		    (p->migration_flags & MDF_నెట్టు) && !rq->నెట్టు_busy) {
			rq->నెట్టు_busy = true;
			నెట్టు_task = get_task_struct(p);
		}

		/*
		 * If there are pending waiters, but no pending stop_work,
		 * then complete now.
		 */
		pending = p->migration_pending;
		ఒకవేళ (pending && !pending->stop_pending) {
			p->migration_pending = ఖాళీ;
			complete = true;
		}

		preempt_disable();
		task_rq_unlock(rq, p, rf);
		ఒకవేళ (నెట్టు_task) {
			stop_one_cpu_nowait(rq->cpu, నెట్టు_cpu_stop,
					    p, &rq->నెట్టు_work);
		}
		preempt_enable();

		ఒకవేళ (complete)
			complete_all(&pending->పూర్తి);

		తిరిగి 0;
	}

	ఒకవేళ (!(flags & SCA_MIGRATE_ENABLE)) {
		/* serialized by p->pi_lock */
		ఒకవేళ (!p->migration_pending) {
			/* Install the request */
			refcount_సమితి(&my_pending.refs, 1);
			init_completion(&my_pending.పూర్తి);
			my_pending.arg = (నిర్మాణం migration_arg) {
				.task = p,
				.dest_cpu = dest_cpu,
				.pending = &my_pending,
			};

			p->migration_pending = &my_pending;
		} లేకపోతే {
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
	ఒకవేళ (WARN_ON_ONCE(!pending)) {
		task_rq_unlock(rq, p, rf);
		తిరిగి -EINVAL;
	}

	ఒకవేళ (task_on_cpu(rq, p) || READ_ONCE(p->__state) == TASK_WAKING) {
		/*
		 * MIGRATE_ENABLE gets here because 'p == current', but for
		 * anything else we cannot do is_migration_disabled(), punt
		 * and have the stopper function handle it all race-free.
		 */
		stop_pending = pending->stop_pending;
		ఒకవేళ (!stop_pending)
			pending->stop_pending = true;

		ఒకవేళ (flags & SCA_MIGRATE_ENABLE)
			p->migration_flags &= ~MDF_నెట్టు;

		preempt_disable();
		task_rq_unlock(rq, p, rf);
		ఒకవేళ (!stop_pending) {
			stop_one_cpu_nowait(cpu_యొక్క(rq), migration_cpu_stop,
					    &pending->arg, &pending->stop_work);
		}
		preempt_enable();

		ఒకవేళ (flags & SCA_MIGRATE_ENABLE)
			తిరిగి 0;
	} లేకపోతే {

		ఒకవేళ (!is_migration_disabled(p)) {
			ఒకవేళ (task_on_rq_queued(p))
				rq = move_queued_task(rq, rf, p, dest_cpu);

			ఒకవేళ (!pending->stop_pending) {
				p->migration_pending = ఖాళీ;
				complete = true;
			}
		}
		task_rq_unlock(rq, p, rf);

		ఒకవేళ (complete)
			complete_all(&pending->పూర్తి);
	}

	wait_for_completion(&pending->పూర్తి);

	ఒకవేళ (refcount_dec_and_test(&pending->refs))
		wake_పైకి_var(&pending->refs); /* No UaF, just an address */

	/*
	 * Block the original owner of &pending until all subsequent callers
	 * have seen the completion and decremented the refcount
	 */
	wait_var_event(&my_pending.refs, !refcount_read(&my_pending.refs));

	/* ARGH */
	WARN_ON_ONCE(my_pending.stop_pending);

	తిరిగి 0;
}

/*
 * Called with both p->pi_lock and rq->lock held; drops both before returning.
 */
స్థిర పూర్ణాంకం __set_cpus_allowed_ptr_locked(నిర్మాణం task_struct *p,
					 నిర్మాణం affinity_context *ctx,
					 నిర్మాణం rq *rq,
					 నిర్మాణం rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_తాళం)
{
	స్థిరరాశి నిర్మాణం cpumask *cpu_allowed_ముసుగు = task_cpu_possible_ముసుగు(p);
	స్థిరరాశి నిర్మాణం cpumask *cpu_valid_ముసుగు = cpu_చురుకైన_ముసుగు;
	bool kthread = p->flags & PF_KTHREAD;
	గుర్తులేని పూర్ణాంకం dest_cpu;
	పూర్ణాంకం ret = 0;

	ఒకవేళ (kthread || is_migration_disabled(p)) {
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
		cpu_valid_ముసుగు = cpu_online_ముసుగు;
	}

	ఒకవేళ (!kthread && !cpumask_subset(ctx->new_ముసుగు, cpu_allowed_ముసుగు)) {
		ret = -EINVAL;
		వెళ్ళు బహిర్గతం;
	}

	/*
	 * Must re-check here, to close a race against __kthread_bind(),
	 * sched_setaffinity() is not guaranteed to observe the flag.
	 */
	ఒకవేళ ((ctx->flags & SCA_CHECK) && (p->flags & PF_NO_SETAFFINITY)) {
		ret = -EINVAL;
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (!(ctx->flags & SCA_MIGRATE_ENABLE)) {
		ఒకవేళ (cpumask_equal(&p->cpus_ముసుగు, ctx->new_ముసుగు)) {
			ఒకవేళ (ctx->flags & SCA_USER)
				మార్పిడి(p->user_cpus_ptr, ctx->user_ముసుగు);
			వెళ్ళు బహిర్గతం;
		}

		ఒకవేళ (WARN_ON_ONCE(p == current &&
				 is_migration_disabled(p) &&
				 !cpumask_test_cpu(task_cpu(p), ctx->new_ముసుగు))) {
			ret = -EBUSY;
			వెళ్ళు బహిర్గతం;
		}
	}

	/*
	 * Picking a ~random cpu helps in cases where we are changing affinity
	 * for groups of tasks (ie. cpuset), so that load balancing is not
	 * immediately required to distribute the tasks within their new mask.
	 */
	dest_cpu = cpumask_any_and_distribute(cpu_valid_ముసుగు, ctx->new_ముసుగు);
	ఒకవేళ (dest_cpu >= nr_cpu_ids) {
		ret = -EINVAL;
		వెళ్ళు బహిర్గతం;
	}

	do_సమితి_cpus_allowed(p, ctx);

	తిరిగి affine_move_task(rq, p, rf, dest_cpu, ctx->flags);

బహిర్గతం:
	task_rq_unlock(rq, p, rf);

	తిరిగి ret;
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
పూర్ణాంకం __set_cpus_allowed_ptr(నిర్మాణం task_struct *p, నిర్మాణం affinity_context *ctx)
{
	నిర్మాణం rq_flags rf;
	నిర్మాణం rq *rq;

	rq = task_rq_తాళం_2(p, &rf);
	/*
	 * Masking should be skipped if SCA_USER or any of the SCA_MIGRATE_*
	 * flags are set.
	 */
	ఒకవేళ (p->user_cpus_ptr &&
	    !(ctx->flags & (SCA_USER | SCA_MIGRATE_ENABLE | SCA_MIGRATE_DISABLE)) &&
	    cpumask_and(rq->scratch_ముసుగు, ctx->new_ముసుగు, p->user_cpus_ptr))
		ctx->new_ముసుగు = rq->scratch_ముసుగు;

	తిరిగి __set_cpus_allowed_ptr_locked(p, ctx, rq, &rf);
}

పూర్ణాంకం సమితి_cpus_allowed_ptr(నిర్మాణం task_struct *p, స్థిరరాశి నిర్మాణం cpumask *new_ముసుగు)
{
	నిర్మాణం affinity_context ac = {
		.new_ముసుగు  = new_ముసుగు,
		.flags     = 0,
	};

	తిరిగి __set_cpus_allowed_ptr(p, &ac);
}
EXPORT_చిహ్నం_GPL(సమితి_cpus_allowed_ptr);

/*
 * Change a given task's CPU affinity to the intersection of its current
 * affinity mask and @subset_mask, writing the resulting mask to @new_mask.
 * If user_cpus_ptr is defined, use it as the basis for restricting CPU
 * affinity or use cpu_online_mask instead.
 *
 * If the resulting mask is empty, leave the affinity unchanged and return
 * -EINVAL.
 */
స్థిర పూర్ణాంకం restrict_cpus_allowed_ptr(నిర్మాణం task_struct *p,
				     నిర్మాణం cpumask *new_ముసుగు,
				     స్థిరరాశి నిర్మాణం cpumask *subset_ముసుగు)
{
	నిర్మాణం affinity_context ac = {
		.new_ముసుగు  = new_ముసుగు,
		.flags     = 0,
	};
	నిర్మాణం rq_flags rf;
	నిర్మాణం rq *rq;
	పూర్ణాంకం err;

	rq = task_rq_తాళం_2(p, &rf);

	/*
	 * Forcefully restricting the affinity of a deadline task is
	 * likely to cause problems, so fail and noisily override the
	 * mask entirely.
	 */
	ఒకవేళ (task_has_dl_policy(p) && dl_bandwidth_enabled()) {
		err = -EPERM;
		వెళ్ళు err_unlock;
	}

	ఒకవేళ (!cpumask_and(new_ముసుగు, task_user_cpus(p), subset_ముసుగు)) {
		err = -EINVAL;
		వెళ్ళు err_unlock;
	}

	తిరిగి __set_cpus_allowed_ptr_locked(p, &ac, rq, &rf);

err_unlock:
	task_rq_unlock(rq, p, &rf);
	తిరిగి err;
}

/*
 * Restrict the CPU affinity of task @p so that it is a subset of
 * task_cpu_possible_mask() and point @p->user_cpus_ptr to a copy of the
 * old affinity mask. If the resulting mask is empty, we warn and walk
 * up the cpuset hierarchy until we find a suitable mask.
 */
శూన్య force_compatible_cpus_allowed_ptr(నిర్మాణం task_struct *p)
{
	cpumask_var_t new_ముసుగు;
	స్థిరరాశి నిర్మాణం cpumask *override_ముసుగు = task_cpu_possible_ముసుగు(p);

	alloc_cpumask_var(&new_ముసుగు, GFP_KERNEL);

	/*
	 * __migrate_task() can fail silently in the face of concurrent
	 * offlining of the chosen destination CPU, so take the hotplug
	 * lock to ensure that the migration succeeds.
	 */
	cpus_read_తాళం();
	ఒకవేళ (!cpumask_అందుబాటు(new_ముసుగు))
		వెళ్ళు బహిర్గతం_సమితి_ముసుగు;

	ఒకవేళ (!restrict_cpus_allowed_ptr(p, new_ముసుగు, override_ముసుగు))
		వెళ్ళు బహిర్గతం_free_ముసుగు;

	/*
	 * We failed to find a valid subset of the affinity mask for the
	 * task, so override it based on its cpuset hierarchy.
	 */
	cpuset_cpus_allowed(p, new_ముసుగు);
	override_ముసుగు = new_ముసుగు;

బహిర్గతం_సమితి_ముసుగు:
	ఒకవేళ (printk_ratelimit()) {
		printk_deferred("Overriding affinity for process %d (%s) to CPUs %*pbl\n",
				task_pid_nr(p), p->comm,
				cpumask_pr_వాదనలు(override_ముసుగు));
	}

	WARN_ON(సమితి_cpus_allowed_ptr(p, override_ముసుగు));
బహిర్గతం_free_ముసుగు:
	cpus_read_unlock();
	free_cpumask_var(new_ముసుగు);
}

/*
 * Restore the affinity of a task @p which was previously restricted by a
 * call to force_compatible_cpus_allowed_ptr().
 *
 * It is the caller's responsibility to serialise this with any calls to
 * force_compatible_cpus_allowed_ptr(@p).
 */
శూన్య relax_compatible_cpus_allowed_ptr(నిర్మాణం task_struct *p)
{
	నిర్మాణం affinity_context ac = {
		.new_ముసుగు  = task_user_cpus(p),
		.flags     = 0,
	};
	పూర్ణాంకం ret;

	/*
	 * Try to restore the old affinity mask with __sched_setaffinity().
	 * Cpuset masking will be done there too.
	 */
	ret = __sched_setaffinity(p, &ac);
	WARN_ON_ONCE(ret);
}

#నిర్వచితమైతే CONFIG_SMP

శూన్య సమితి_task_cpu(నిర్మాణం task_struct *p, గుర్తులేని పూర్ణాంకం new_cpu)
{
	గుర్తులేని పూర్ణాంకం state = READ_ONCE(p->__state);
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

#నిర్వచితమైతే CONFIG_LOCKDEP
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
	WARN_ON_ONCE(debug_locks && !(lockdep_is_held(&p->pi_తాళం) ||
				      lockdep_is_held(__rq_lockp(task_rq(p)))));
#అయితే_ముగింపు
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

	ఒకవేళ (task_cpu(p) != new_cpu) {
		ఒకవేళ (p->sched_class->migrate_task_rq)
			p->sched_class->migrate_task_rq(p, new_cpu);
		p->se.nr_migrations++;
		perf_event_task_migrate(p);
	}

	__set_task_cpu(p, new_cpu);
}
#అయితే_ముగింపు /* CONFIG_SMP */

#నిర్వచితమైతే CONFIG_NUMA_BALANCING
స్థిర శూన్య __migrate_swap_task(నిర్మాణం task_struct *p, పూర్ణాంకం cpu)
{
	ఒకవేళ (task_on_rq_queued(p)) {
		నిర్మాణం rq *src_rq, *dst_rq;
		నిర్మాణం rq_flags srf, drf;

		src_rq = task_rq(p);
		dst_rq = cpu_rq(cpu);

		rq_pin_తాళం(src_rq, &srf);
		rq_pin_తాళం(dst_rq, &drf);

		move_queued_task_locked(src_rq, dst_rq, p);
		wakeup_preempt(dst_rq, p, 0);

		rq_unpin_తాళం(dst_rq, &drf);
		rq_unpin_తాళం(src_rq, &srf);

	} లేకపోతే {
		/*
		 * Task isn't running anymore; make it appear like we migrated
		 * it before it went to sleep. This means on wakeup we make the
		 * previous CPU our target instead of where it really is.
		 */
		p->wake_cpu = cpu;
	}
}

నిర్మాణం migration_మార్పిడి_arg {
	నిర్మాణం task_struct *src_task, *dst_task;
	పూర్ణాంకం src_cpu, dst_cpu;
};

స్థిర పూర్ణాంకం migrate_మార్పిడి_stop(శూన్య *దత్తాంశం)
{
	నిర్మాణం migration_మార్పిడి_arg *arg = దత్తాంశం;
	నిర్మాణం rq *src_rq, *dst_rq;

	ఒకవేళ (!cpu_చురుకైన(arg->src_cpu) || !cpu_చురుకైన(arg->dst_cpu))
		తిరిగి -EAGAIN;

	src_rq = cpu_rq(arg->src_cpu);
	dst_rq = cpu_rq(arg->dst_cpu);

	guard(double_raw_spinlock)(&arg->src_task->pi_తాళం, &arg->dst_task->pi_తాళం);
	guard(double_rq_తాళం)(src_rq, dst_rq);

	ఒకవేళ (task_cpu(arg->dst_task) != arg->dst_cpu)
		తిరిగి -EAGAIN;

	ఒకవేళ (task_cpu(arg->src_task) != arg->src_cpu)
		తిరిగి -EAGAIN;

	ఒకవేళ (!cpumask_test_cpu(arg->dst_cpu, arg->src_task->cpus_ptr))
		తిరిగి -EAGAIN;

	ఒకవేళ (!cpumask_test_cpu(arg->src_cpu, arg->dst_task->cpus_ptr))
		తిరిగి -EAGAIN;

	__migrate_swap_task(arg->src_task, arg->dst_cpu);
	__migrate_swap_task(arg->dst_task, arg->src_cpu);

	తిరిగి 0;
}

/*
 * Cross migrate two tasks
 */
పూర్ణాంకం migrate_మార్పిడి(నిర్మాణం task_struct *cur, నిర్మాణం task_struct *p,
		పూర్ణాంకం లక్ష్యం_cpu, పూర్ణాంకం curr_cpu)
{
	నిర్మాణం migration_మార్పిడి_arg arg;
	పూర్ణాంకం ret = -EINVAL;

	arg = (నిర్మాణం migration_మార్పిడి_arg){
		.src_task = cur,
		.src_cpu = curr_cpu,
		.dst_task = p,
		.dst_cpu = లక్ష్యం_cpu,
	};

	ఒకవేళ (arg.src_cpu == arg.dst_cpu)
		వెళ్ళు బహిర్గతం;

	/*
	 * These three tests are all lockless; this is OK since all of them
	 * will be re-checked with proper locks held further down the line.
	 */
	ఒకవేళ (!cpu_చురుకైన(arg.src_cpu) || !cpu_చురుకైన(arg.dst_cpu))
		వెళ్ళు బహిర్గతం;

	ఒకవేళ (!cpumask_test_cpu(arg.dst_cpu, arg.src_task->cpus_ptr))
		వెళ్ళు బహిర్గతం;

	ఒకవేళ (!cpumask_test_cpu(arg.src_cpu, arg.dst_task->cpus_ptr))
		వెళ్ళు బహిర్గతం;

	trace_sched_మార్పిడి_numa(cur, arg.src_cpu, p, arg.dst_cpu);
	ret = stop_two_cpus(arg.dst_cpu, arg.src_cpu, migrate_మార్పిడి_stop, &arg);

బహిర్గతం:
	తిరిగి ret;
}
#అయితే_ముగింపు /* CONFIG_NUMA_BALANCING */

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
శూన్య kick_process(నిర్మాణం task_struct *p)
{
	guard(preempt)();
	పూర్ణాంకం cpu = task_cpu(p);

	ఒకవేళ ((cpu != smp_processor_id()) && task_curr(p))
		smp_send_reschedule(cpu);
}
EXPORT_చిహ్నం_GPL(kick_process);

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
స్థిర పూర్ణాంకం select_fallback_rq(పూర్ణాంకం cpu, నిర్మాణం task_struct *p)
{
	పూర్ణాంకం nid = cpu_గమ్యం_కణుపు(cpu);
	స్థిరరాశి నిర్మాణం cpumask *nodemask = ఖాళీ;
	గణన { cpuset, possible, fail } state = cpuset;
	పూర్ణాంకం dest_cpu;

	/*
	 * If the node that the CPU is on has been offlined, cpu_to_node()
	 * will return -1. There is no CPU on the node, and we should
	 * select the CPU on the other node.
	 */
	ఒకవేళ (nid != -1) {
		nodemask = cpumask_యొక్క_కణుపు(nid);

		/* Look for allowed, online CPU in same node. */
		for_each_cpu(dest_cpu, nodemask) {
			ఒకవేళ (is_cpu_allowed(p, dest_cpu))
				తిరిగి dest_cpu;
		}
	}

	కొరకు (;;) {
		/* Any allowed, online CPU? */
		for_each_cpu(dest_cpu, p->cpus_ptr) {
			ఒకవేళ (!is_cpu_allowed(p, dest_cpu))
				కొనసాగు;

			వెళ్ళు బహిర్గతం;
		}

		/* No more Mr. Nice Guy. */
		ఎంపిక (state) {
		స్థితి cpuset:
			ఒకవేళ (cpuset_cpus_allowed_fallback(p)) {
				state = possible;
				ఆపు;
			}
			fallthrough;
		స్థితి possible:
			సమితి_cpus_allowed_force(p, task_cpu_fallback_ముసుగు(p));
			state = fail;
			ఆపు;
		స్థితి fail:
			BUG();
			ఆపు;
		}
	}

బహిర్గతం:
	ఒకవేళ (state != cpuset) {
		/*
		 * Don't tell them about moving exiting tasks or
		 * kernel threads (both mm NULL), since they never
		 * leave kernel.
		 */
		ఒకవేళ (p->mm && printk_ratelimit()) {
			printk_deferred("process %d (%s) no longer affine to cpu%d\n",
					task_pid_nr(p), p->comm, cpu);
		}
	}

	తిరిగి dest_cpu;
}

/*
 * The caller (fork, wakeup) owns p->pi_lock, ->cpus_ptr is stable.
 */
స్థిర అంతఃవరుస
పూర్ణాంకం select_task_rq(నిర్మాణం task_struct *p, పూర్ణాంకం cpu, పూర్ణాంకం *wake_flags)
{
	lockdep_assert_held(&p->pi_తాళం);

	ఒకవేళ (p->nr_cpus_allowed > 1 && !is_migration_disabled(p)) {
		cpu = p->sched_class->select_task_rq(p, cpu, *wake_flags);
		*wake_flags |= WF_RQ_SELECTED;
	} లేకపోతే {
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
	ఒకవేళ (unlikely(!is_cpu_allowed(p, cpu)))
		cpu = select_fallback_rq(task_cpu(p), p);

	తిరిగి cpu;
}

శూన్య sched_సమితి_stop_task(పూర్ణాంకం cpu, నిర్మాణం task_struct *stop)
{
	స్థిర నిర్మాణం తాళం_class_కీలకం stop_pi_తాళం;
	నిర్మాణం sched_param param = { .sched_ప్రాధాన్యత = MAX_RT_PRIO - 1 };
	నిర్మాణం task_struct *old_stop = cpu_rq(cpu)->stop;

	ఒకవేళ (stop) {
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
		lockdep_సమితి_class(&stop->pi_తాళం, &stop_pi_తాళం);
	}

	cpu_rq(cpu)->stop = stop;

	ఒకవేళ (old_stop) {
		/*
		 * Reset it back to a normal scheduling class so that
		 * it can die in pieces.
		 */
		old_stop->sched_class = &rt_sched_class;
	}
}

స్థిర శూన్య
ttwu_stat(నిర్మాణం task_struct *p, పూర్ణాంకం cpu, పూర్ణాంకం wake_flags)
{
	నిర్మాణం rq *rq;

	ఒకవేళ (!schedstat_enabled())
		తిరిగి;

	rq = this_rq();

	ఒకవేళ (cpu == rq->cpu) {
		__schedstat_inc(rq->ttwu_local);
		__schedstat_inc(p->stats.nr_wakeups_local);
	} లేకపోతే {
		నిర్మాణం sched_domain *sd;

		__schedstat_inc(p->stats.nr_wakeups_remote);

		guard(rcu)();
		for_each_domain(rq->cpu, sd) {
			ఒకవేళ (cpumask_test_cpu(cpu, sched_domain_పరిధి(sd))) {
				__schedstat_inc(sd->ttwu_wake_remote);
				ఆపు;
			}
		}
	}

	ఒకవేళ (wake_flags & WF_MIGRATED)
		__schedstat_inc(p->stats.nr_wakeups_migrate);

	__schedstat_inc(rq->ttwu_లెక్క);
	__schedstat_inc(p->stats.nr_wakeups);

	ఒకవేళ (wake_flags & WF_SYNC)
		__schedstat_inc(p->stats.nr_wakeups_sync);
}

/*
 * Mark the task runnable.
 */
స్థిర అంతఃవరుస శూన్య ttwu_do_wakeup(నిర్మాణం task_struct *p)
{
	p->is_blocked = 0;
	WRITE_ONCE(p->__state, TASK_RUNNING);
	trace_sched_wakeup(p);
}

శూన్య update_rq_avg_idle(నిర్మాణం rq *rq)
{
	u64 idle_stamp = rq->idle_stamp;
	u64 delta, max;

	ఒకవేళ (!idle_stamp)
		తిరిగి;

	delta = rq_clock(rq) - idle_stamp;

	update_avg(&rq->avg_idle, delta);

	max = 2 * rq->max_idle_balance_ఖర్చు;
	ఒకవేళ (rq->avg_idle > max)
		rq->avg_idle = max;
	rq->idle_stamp = 0;
}

#నిర్వచితమైతే CONFIG_SCHED_PROXY_EXEC
స్థిర శూన్య zap_balance_callbacks(నిర్మాణం rq *rq);

స్థిర అంతఃవరుస శూన్య proxy_reset_donor(నిర్మాణం rq *rq)
{
	WARN_ON_ONCE(rq->donor == rq->curr);

	put_prev_సమితి_తదుపరి_task(rq, rq->donor, rq->curr);
	rq->తదుపరి_class = rq->curr->sched_class;
	rq_సమితి_donor(rq, rq->curr);
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
స్థిర అంతఃవరుస bool proxy_needs_return(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
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
	ఒకవేళ (task_cpu(p) == p->wake_cpu)
		తిరిగి false;

	scoped_guard(raw_spinlock, &p->blocked_తాళం) {
		/* Task is waking up; clear any blocked_on relationship */
		__clear_task_blocked_on(p, ఖాళీ);

		/* If already current, don't need to return migrate */
		ఒకవేళ (task_current(rq, p))
			తిరిగి false;
	}

	dequeued = వరుస_నుండి_తీయి_ఖండం_task(rq, p, TASK_WAKING);

	/*
	 * Dequeue @p from its scheduling class before resetting rq->donor.
	 * In particular, sched_ext needs to end the donor's running session
	 * and clear SCX_TASK_QUEUED before put_prev_task_scx() is called by
	 * proxy_reset_donor(); otherwise it would reenqueue the blocked donor.
	 *
	 * Keep on_rq set until all donor references have been replaced.
	 */
	ఒకవేళ (task_current_donor(rq, p))
		proxy_reset_donor(rq);

	ఒకవేళ (dequeued)
		__block_task(rq, p);
	తిరిగి true;
}
#లేకపోతే /* !CONFIG_SCHED_PROXY_EXEC */
స్థిర అంతఃవరుస bool proxy_needs_return(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	తిరిగి false;
}
#అయితే_ముగింపు /* CONFIG_SCHED_PROXY_EXEC */

స్థిర శూన్య
ttwu_do_activate(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం wake_flags,
		 నిర్మాణం rq_flags *rf)
{
	పూర్ణాంకం en_flags = వరుసలో_చేర్చు_WAKEUP | వరుసలో_చేర్చు_NOCLOCK;

	lockdep_assert_rq_held(rq);

	ఒకవేళ (p->sched_contributes_గమ్యం_ఎక్కించు)
		rq->nr_uninterruptible--;

	ఒకవేళ (wake_flags & WF_RQ_SELECTED)
		en_flags |= వరుసలో_చేర్చు_RQ_SELECTED;
	ఒకవేళ (wake_flags & WF_MIGRATED)
		en_flags |= వరుసలో_చేర్చు_MIGRATED;
	లేకపోతే ఒకవేళ (p->లోపల_iowait) {
		delayacct_blkio_చివర(p);
		atomic_dec(&task_rq(p)->nr_iowait);
	}

	activate_task(rq, p, en_flags);
	wakeup_preempt(rq, p, wake_flags);

	ttwu_do_wakeup(p);

	ఒకవేళ (p->sched_class->task_woken) {
		/*
		 * Our task @p is fully woken up and running; so it's safe to
		 * drop the rq->lock, hereafter rq is only used for statistics.
		 */
		rq_unpin_తాళం(rq, rf);
		p->sched_class->task_woken(rq, p);
		rq_repin_తాళం(rq, rf);
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
స్థిర పూర్ణాంకం ttwu_runnable(నిర్మాణం task_struct *p, పూర్ణాంకం wake_flags)
{
	ACQUIRE(__task_rq_lock, guard)(p);
	నిర్మాణం rq *rq = guard.rq;

	ఒకవేళ (!task_on_rq_queued(p))
		తిరిగి 0;

	update_rq_clock(rq);
	ఒకవేళ (p->is_blocked) {
		ఒకవేళ (p->se.sched_delayed)
			వరుసలో_చేర్చు_task(rq, p, వరుసలో_చేర్చు_NOCLOCK | వరుసలో_చేర్చు_DELAYED);
		ఒకవేళ (proxy_needs_return(rq, p))
			తిరిగి 0;
	}
	ఒకవేళ (!task_on_cpu(rq, p)) {
		/*
		 * When on_rq && !on_cpu the task is preempted, see if
		 * it should preempt the task that is current now.
		 */
		wakeup_preempt(rq, p, wake_flags | WF_TTWU_RQ);
	}
	ttwu_do_wakeup(p);
	తిరిగి 1;
}

శూన్య sched_ttwu_pending(శూన్య *arg)
{
	నిర్మాణం llist_కణుపు *llist = arg;
	నిర్మాణం rq *rq = this_rq();
	నిర్మాణం task_struct *p, *t;
	నిర్మాణం rq_flags rf;

	ఒకవేళ (!llist)
		తిరిగి;

	rq_తాళం_irqsave(rq, &rf);
	update_rq_clock(rq);

	llist_for_each_నమోదు_సురక్షిత(p, t, llist, wake_నమోదు.llist) {
		ఒకవేళ (WARN_ON_ONCE(p->on_cpu))
			smp_cond_ఎక్కించు_acquire(&p->on_cpu, !VAL);

		ఒకవేళ (WARN_ON_ONCE(task_cpu(p) != cpu_యొక్క(rq)))
			సమితి_task_cpu(p, cpu_యొక్క(rq));

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
bool call_function_single_prep_ipi(పూర్ణాంకం cpu)
{
	ఒకవేళ (సమితి_nr_if_polling(cpu_rq(cpu)->idle)) {
		trace_sched_wake_idle_without_ipi(cpu);
		తిరిగి false;
	}

	తిరిగి true;
}

/*
 * Queue a task on the target CPUs wake_list and wake the CPU via IPI if
 * necessary. The wakee CPU on receipt of the IPI will queue the task
 * via sched_ttwu_wakeup() for activation so the wakee incurs the cost
 * of the wakeup instead of the waker.
 */
స్థిర శూన్య __ttwu_queue_wakelist(నిర్మాణం task_struct *p, పూర్ణాంకం cpu, పూర్ణాంకం wake_flags)
{
	నిర్మాణం rq *rq = cpu_rq(cpu);

	p->sched_remote_wakeup = !!(wake_flags & WF_MIGRATED);

	WRITE_ONCE(rq->ttwu_pending, 1);
#నిర్వచితమైతే CONFIG_SMP
	__smp_call_single_queue(cpu, &p->wake_నమోదు.llist);
#అయితే_ముగింపు
}

శూన్య wake_పైకి_if_idle(పూర్ణాంకం cpu)
{
	నిర్మాణం rq *rq = cpu_rq(cpu);

	guard(rcu)();
	ఒకవేళ (is_idle_task(rcu_dereference(rq->curr))) {
		guard(rq_తాళం_irqsave)(rq);
		ఒకవేళ (is_idle_task(rq->curr))
			resched_curr(rq);
	}
}

bool cpus_equal_సామర్థ్యం(పూర్ణాంకం this_cpu, పూర్ణాంకం that_cpu)
{
	ఒకవేళ (!sched_asym_cpucap_చురుకైన())
		తిరిగి true;

	ఒకవేళ (this_cpu == that_cpu)
		తిరిగి true;

	తిరిగి arch_scale_cpu_సామర్థ్యం(this_cpu) == arch_scale_cpu_సామర్థ్యం(that_cpu);
}

bool cpus_share_cache(పూర్ణాంకం this_cpu, పూర్ణాంకం that_cpu)
{
	ఒకవేళ (this_cpu == that_cpu)
		తిరిగి true;

	తిరిగి చొప్పున_cpu(sd_llc_id, this_cpu) == చొప్పున_cpu(sd_llc_id, that_cpu);
}

/*
 * Whether CPUs are share cache resources, which means LLC on non-cluster
 * machines and LLC tag or L2 on machines with clusters.
 */
bool cpus_share_resources(పూర్ణాంకం this_cpu, పూర్ణాంకం that_cpu)
{
	ఒకవేళ (this_cpu == that_cpu)
		తిరిగి true;

	తిరిగి చొప్పున_cpu(sd_share_id, this_cpu) == చొప్పున_cpu(sd_share_id, that_cpu);
}

స్థిర అంతఃవరుస bool ttwu_వరుస_cond(నిర్మాణం task_struct *p, పూర్ణాంకం cpu)
{
	పూర్ణాంకం this_cpu = smp_processor_id();

	/* See SCX_OPS_ALLOW_QUEUED_WAKEUP. */
	ఒకవేళ (!scx_allow_ttwu_వరుస(p))
		తిరిగి false;

#నిర్వచితమైతే CONFIG_SMP
	ఒకవేళ (p->sched_class == &stop_sched_class)
		తిరిగి false;
#అయితే_ముగింపు

	/*
	 * Do not complicate things with the async wake_list while the CPU is
	 * in hotplug state.
	 */
	ఒకవేళ (!cpu_చురుకైన(cpu))
		తిరిగి false;

	/* Ensure the task will still be allowed to run on the CPU. */
	ఒకవేళ (!cpumask_test_cpu(cpu, p->cpus_ptr))
		తిరిగి false;

	/*
	 * If the CPU does not share cache, then queue the task on the
	 * remote rqs wakelist to avoid accessing remote data.
	 */
	ఒకవేళ (!cpus_share_cache(this_cpu, cpu))
		తిరిగి true;

	ఒకవేళ (cpu == this_cpu)
		తిరిగి false;

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
	ఒకవేళ (!cpu_rq(cpu)->nr_running)
		తిరిగి true;

	తిరిగి false;
}

స్థిర bool ttwu_వరుస_wakelist(నిర్మాణం task_struct *p, పూర్ణాంకం cpu, పూర్ణాంకం wake_flags)
{
	ఒకవేళ (sched_feat(TTWU_వరుస) && ttwu_వరుస_cond(p, cpu)) {
		sched_clock_cpu(cpu); /* Sync clocks across CPUs */
		__ttwu_queue_wakelist(p, cpu, wake_flags);
		తిరిగి true;
	}

	తిరిగి false;
}

స్థిర శూన్య ttwu_వరుస(నిర్మాణం task_struct *p, పూర్ణాంకం cpu, పూర్ణాంకం wake_flags)
{
	నిర్మాణం rq *rq = cpu_rq(cpu);
	నిర్మాణం rq_flags rf;

	ఒకవేళ (ttwu_వరుస_wakelist(p, cpu, wake_flags))
		తిరిగి;

	rq_తాళం(rq, &rf);
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
స్థిర __always_inline
bool ttwu_state_సరిపోలిక(నిర్మాణం task_struct *p, గుర్తులేని పూర్ణాంకం state, పూర్ణాంకం *success)
{
	పూర్ణాంకం సరిపోలిక;

	ఒకవేళ (IS_ENABLED(CONFIG_DEBUG_PREEMPT)) {
		WARN_ON_ONCE((state & TASK_RTLOCK_WAIT) &&
			     state != TASK_RTLOCK_WAIT);
	}

	*success = !!(సరిపోలిక = __task_state_match(p, state));

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
	ఒకవేళ (సరిపోలిక < 0)
		p->saved_state = TASK_RUNNING;

	తిరిగి సరిపోలిక > 0;
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
పూర్ణాంకం try_గమ్యం_wake_పైకి(నిర్మాణం task_struct *p, గుర్తులేని పూర్ణాంకం state, పూర్ణాంకం wake_flags)
{
	guard(preempt)();
	పూర్ణాంకం cpu, success = 0;

	wake_flags |= WF_TTWU;

	ఒకవేళ (p == current) {
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
		clear_task_blocked_on(p, ఖాళీ);
		ఒకవేళ (!ttwu_state_సరిపోలిక(p, state, &success))
			వెళ్ళు బహిర్గతం;

		trace_sched_waking(p);
		ttwu_do_wakeup(p);
		వెళ్ళు బహిర్గతం;
	}

	/*
	 * If we are going to wake up a thread waiting for CONDITION we
	 * need to ensure that CONDITION=1 done by the caller can not be
	 * reordered with p->state check below. This pairs with smp_store_mb()
	 * in set_current_state() that the waiting thread does.
	 */
	scoped_guard (raw_spinlock_irqsave, &p->pi_తాళం) {
		smp_mb__after_spinlock();

		ఒకవేళ (!ttwu_state_సరిపోలిక(p, state, &success))
			ఆపు;

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
		ఒకవేళ (READ_ONCE(p->on_rq) && ttwu_runnable(p, wake_flags))
			ఆపు;

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
		ఒకవేళ (smp_ఎక్కించు_acquire(&p->on_cpu) &&
		    ttwu_వరుస_wakelist(p, task_cpu(p), wake_flags))
			ఆపు;

		/*
		 * If the owning (remote) CPU is still in the middle of schedule() with
		 * this task as prev, wait until it's done referencing the task.
		 *
		 * Pairs with the smp_store_release() in finish_task().
		 *
		 * This ensures that tasks getting woken will be fully ordered against
		 * their previous state and preserve Program Order.
		 */
		smp_cond_ఎక్కించు_acquire(&p->on_cpu, !VAL);

		cpu = select_task_rq(p, p->wake_cpu, &wake_flags);
		ఒకవేళ (task_cpu(p) != cpu) {
			ఒకవేళ (p->లోపల_iowait) {
				delayacct_blkio_చివర(p);
				atomic_dec(&task_rq(p)->nr_iowait);
			}

			wake_flags |= WF_MIGRATED;
			psi_ttwu_వరుస_నుండి_తీయి(p);
			సమితి_task_cpu(p, cpu);
		} లేకపోతే ఒకవేళ (cpu != p->wake_cpu) {
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

		ttwu_వరుస(p, cpu, wake_flags);
	}
బహిర్గతం:
	ఒకవేళ (success)
		ttwu_stat(p, task_cpu(p), wake_flags);

	తిరిగి success;
}

స్థిర bool __task_needs_rq_lock(నిర్మాణం task_struct *p)
{
	గుర్తులేని పూర్ణాంకం state = READ_ONCE(p->__state);

	/*
	 * Since pi->lock blocks try_to_wake_up(), we don't need rq->lock when
	 * the task is blocked. Make sure to check @state since ttwu() can drop
	 * locks at the end, see ttwu_queue_wakelist().
	 */
	ఒకవేళ (state == TASK_RUNNING || state == TASK_WAKING)
		తిరిగి true;

	/*
	 * Ensure we load p->on_rq after p->__state, otherwise it would be
	 * possible to, falsely, observe p->on_rq == 0.
	 *
	 * See try_to_wake_up() for a longer comment.
	 */
	smp_rmb();
	ఒకవేళ (p->on_rq)
		తిరిగి true;

	/*
	 * Ensure the task has finished __schedule() and will not be referenced
	 * anymore. Again, see try_to_wake_up() for a longer comment.
	 */
	smp_rmb();
	smp_cond_ఎక్కించు_acquire(&p->on_cpu, !VAL);

	తిరిగి false;
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
పూర్ణాంకం task_call_func(నిర్మాణం task_struct *p, task_call_f func, శూన్య *arg)
{
	నిర్మాణం rq_flags rf;
	పూర్ణాంకం ret;

	raw_spin_తాళం_irqsave(&p->pi_తాళం, rf.flags);

	ఒకవేళ (__task_needs_rq_lock(p)) {
		నిర్మాణం rq *rq = __task_rq_lock(p, &rf);

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
	} లేకపోతే {
		ret = func(p, arg);
	}

	raw_spin_unlock_irqrestore(&p->pi_తాళం, rf.flags);
	తిరిగి ret;
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
నిర్మాణం task_struct *cpu_curr_snapshot(పూర్ణాంకం cpu)
{
	నిర్మాణం rq *rq = cpu_rq(cpu);
	నిర్మాణం task_struct *t;
	నిర్మాణం rq_flags rf;

	rq_తాళం_irqsave(rq, &rf);
	smp_mb__after_spinlock(); /* Pairing determined by caller's synchronization design. */
	t = rcu_dereference(cpu_curr(cpu));
	rq_unlock_irqrestore(rq, &rf);
	smp_mb(); /* Pairing determined by caller's synchronization design. */

	తిరిగి t;
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
పూర్ణాంకం wake_పైకి_process(నిర్మాణం task_struct *p)
{
	తిరిగి try_గమ్యం_wake_పైకి(p, TASK_సాధారణ, 0);
}
EXPORT_చిహ్నం(wake_పైకి_process);

పూర్ణాంకం wake_పైకి_state(నిర్మాణం task_struct *p, గుర్తులేని పూర్ణాంకం state)
{
	తిరిగి try_గమ్యం_wake_పైకి(p, state, 0);
}

/*
 * Perform scheduler related setup for a newly forked process p.
 * p is forked by current.
 *
 * __sched_fork() is basic setup which is also used by sched_init() to
 * initialize the boot CPU's idle task.
 */
స్థిర శూన్య __sched_fork(u64 clone_flags, నిర్మాణం task_struct *p)
{
	p->on_rq			= 0;

	p->se.on_rq			= 0;
	p->se.exec_ప్రారంభం		= 0;
	p->se.కూడిక_exec_runtime		= 0;
	p->se.prev_కూడిక_exec_runtime	= 0;
	p->se.nr_migrations		= 0;
	p->se.vruntime			= 0;
	p->se.vlag			= 0;
	p->se.rel_deadline		= 0;
	INIT_LIST_తల(&p->se.సమూహం_కణుపు);

	/* A delayed task cannot be in clone(). */
	WARN_ON_ONCE(p->se.sched_delayed);
	WARN_ON_ONCE(p->is_blocked);

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
	p->se.cfs_rq			= ఖాళీ;
#నిర్వచితమైతే CONFIG_CFS_BANDWIDTH
	init_cfs_throttle_work(p);
#అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_SCHEDSTATS
	/* Even if schedstat is disabled, there should not be garbage */
	memset(&p->stats, 0, పరిమాణం(p->stats));
#అయితే_ముగింపు

	init_dl_entity(&p->dl);

	INIT_LIST_తల(&p->rt.నిరంతరం_list);
	p->rt.timeout		= 0;
	p->rt.సమయం_ముక్క	= sched_rr_timeslice;
	p->rt.on_rq		= 0;
	p->rt.on_list		= 0;

#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
	init_scx_entity(&p->scx);
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_PREEMPT_NOTIFIERS
	INIT_HLIST_తల(&p->preempt_notifiers);
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_COMPACTION
	p->capture_control = ఖాళీ;
#అయితే_ముగింపు
	init_numa_balancing(clone_flags, p);
	p->wake_నమోదు.u_flags = CSD_TYPE_TTWU;
	p->migration_pending = ఖాళీ;
	init_sched_mm(p);
}

DEFINE_STATIC_కీలకం_FALSE(sched_numa_balancing);

#నిర్వచితమైతే CONFIG_NUMA_BALANCING

పూర్ణాంకం sysctl_numa_balancing_బహుళకం;

స్థిర శూన్య __set_numabalancing_state(bool enabled)
{
	ఒకవేళ (enabled)
		static_branch_enable(&sched_numa_balancing);
	లేకపోతే
		static_branch_disable(&sched_numa_balancing);
}

శూన్య సమితి_numabalancing_state(bool enabled)
{
	ఒకవేళ (enabled)
		sysctl_numa_balancing_బహుళకం = NUMA_BALANCING_సాధారణ;
	లేకపోతే
		sysctl_numa_balancing_బహుళకం = NUMA_BALANCING_DISABLED;
	__set_numabalancing_state(enabled);
}

#నిర్వచితమైతే CONFIG_SYSCTL
స్థిర శూన్య reset_జ్ఞాపకం_tiering(శూన్య)
{
	నిర్మాణం pglist_దత్తాంశం *pgdat;

	for_each_online_pgdat(pgdat) {
		pgdat->nbp_పరిమితి = 0;
		pgdat->nbp_th_nr_cand = కణుపు_పుట_state(pgdat, PGPROMOTE_CANDIDATE);
		pgdat->nbp_th_ప్రారంభం = jiffies_గమ్యం_msecs(jiffies);
	}
}

స్థిర పూర్ణాంకం sysctl_numa_balancing(స్థిరరాశి నిర్మాణం ctl_పట్టిక *పట్టిక, పూర్ణాంకం లిఖ,
			  శూన్య *బఫరు, కొలత_t *lenp, loff_t *ppos)
{
	నిర్మాణం ctl_పట్టిక t;
	పూర్ణాంకం err;
	పూర్ణాంకం state = sysctl_numa_balancing_బహుళకం;

	ఒకవేళ (లిఖ && !capable(CAP_SYS_ADMIN))
		తిరిగి -EPERM;

	t = *పట్టిక;
	t.దత్తాంశం = &state;
	err = proc_dointvec_minmax(&t, లిఖ, బఫరు, lenp, ppos);
	ఒకవేళ (err < 0)
		తిరిగి err;
	ఒకవేళ (లిఖ) {
		ఒకవేళ (!(sysctl_numa_balancing_బహుళకం & NUMA_BALANCING_జ్ఞాపకం_TIERING) &&
		    (state & NUMA_BALANCING_జ్ఞాపకం_TIERING))
			reset_జ్ఞాపకం_tiering();
		sysctl_numa_balancing_బహుళకం = state;
		__set_numabalancing_state(state);
	}
	తిరిగి err;
}
#అయితే_ముగింపు /* CONFIG_SYSCTL */
#అయితే_ముగింపు /* CONFIG_NUMA_BALANCING */

#నిర్వచితమైతే CONFIG_SCHEDSTATS

DEFINE_STATIC_కీలకం_FALSE(sched_schedstats);

స్థిర శూన్య సమితి_schedstats(bool enabled)
{
	ఒకవేళ (enabled)
		static_branch_enable(&sched_schedstats);
	లేకపోతే
		static_branch_disable(&sched_schedstats);
}

శూన్య force_schedstat_enabled(శూన్య)
{
	ఒకవేళ (!schedstat_enabled()) {
		pr_info("kernel profiling enabled schedstats, disable via kernel.sched_schedstats.\n");
		static_branch_enable(&sched_schedstats);
	}
}

స్థిర పూర్ణాంకం __init అమరిక_schedstats(అక్షరం *str)
{
	పూర్ణాంకం ret = 0;
	ఒకవేళ (!str)
		వెళ్ళు బహిర్గతం;

	ఒకవేళ (!strcmp(str, "enable")) {
		సమితి_schedstats(true);
		ret = 1;
	} లేకపోతే ఒకవేళ (!strcmp(str, "disable")) {
		సమితి_schedstats(false);
		ret = 1;
	}
బహిర్గతం:
	ఒకవేళ (!ret)
		pr_warn("Unable to parse schedstats=\n");

	తిరిగి ret;
}
__setup("schedstats=", అమరిక_schedstats);

#నిర్వచితమైతే CONFIG_SYSCTL
స్థిర పూర్ణాంకం sysctl_schedstats(స్థిరరాశి నిర్మాణం ctl_పట్టిక *పట్టిక, పూర్ణాంకం లిఖ, శూన్య *బఫరు,
		కొలత_t *lenp, loff_t *ppos)
{
	నిర్మాణం ctl_పట్టిక t;
	పూర్ణాంకం err;
	పూర్ణాంకం state = static_branch_likely(&sched_schedstats);

	ఒకవేళ (లిఖ && !capable(CAP_SYS_ADMIN))
		తిరిగి -EPERM;

	t = *పట్టిక;
	t.దత్తాంశం = &state;
	err = proc_dointvec_minmax(&t, లిఖ, బఫరు, lenp, ppos);
	ఒకవేళ (err < 0)
		తిరిగి err;
	ఒకవేళ (లిఖ)
		సమితి_schedstats(state);
	తిరిగి err;
}
#అయితే_ముగింపు /* CONFIG_SYSCTL */
#అయితే_ముగింపు /* CONFIG_SCHEDSTATS */

#నిర్వచితమైతే CONFIG_SYSCTL
స్థిర స్థిరరాశి నిర్మాణం ctl_పట్టిక sched_core_sysctls[] = {
#నిర్వచితమైతే CONFIG_SCHEDSTATS
	{
		.procname       = "sched_schedstats",
		.దత్తాంశం           = ఖాళీ,
		.maxlen         = పరిమాణం(గుర్తులేని పూర్ణాంకం),
		.బహుళకం           = 0644,
		.proc_handler   = sysctl_schedstats,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
#అయితే_ముగింపు /* CONFIG_SCHEDSTATS */
#నిర్వచితమైతే CONFIG_UCLAMP_TASK
	{
		.procname       = "sched_util_clamp_min",
		.దత్తాంశం           = &sysctl_sched_uclamp_util_min,
		.maxlen         = పరిమాణం(గుర్తులేని పూర్ణాంకం),
		.బహుళకం           = 0644,
		.proc_handler   = sysctl_sched_uclamp_handler,
	},
	{
		.procname       = "sched_util_clamp_max",
		.దత్తాంశం           = &sysctl_sched_uclamp_util_max,
		.maxlen         = పరిమాణం(గుర్తులేని పూర్ణాంకం),
		.బహుళకం           = 0644,
		.proc_handler   = sysctl_sched_uclamp_handler,
	},
	{
		.procname       = "sched_util_clamp_min_rt_default",
		.దత్తాంశం           = &sysctl_sched_uclamp_util_min_rt_default,
		.maxlen         = పరిమాణం(గుర్తులేని పూర్ణాంకం),
		.బహుళకం           = 0644,
		.proc_handler   = sysctl_sched_uclamp_handler,
	},
#అయితే_ముగింపు /* CONFIG_UCLAMP_TASK */
#నిర్వచితమైతే CONFIG_NUMA_BALANCING
	{
		.procname	= "numa_balancing",
		.దత్తాంశం		= ఖాళీ, /* filled in by handler */
		.maxlen		= పరిమాణం(గుర్తులేని పూర్ణాంకం),
		.బహుళకం		= 0644,
		.proc_handler	= sysctl_numa_balancing,
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_FOUR,
	},
#అయితే_ముగింపు /* CONFIG_NUMA_BALANCING */
};
స్థిర పూర్ణాంకం __init sched_core_sysctl_init(శూన్య)
{
	రిజిస్టరు_sysctl_init("kernel", sched_core_sysctls);
	తిరిగి 0;
}
late_initcall(sched_core_sysctl_init);
#అయితే_ముగింపు /* CONFIG_SYSCTL */

/*
 * fork()/clone()-time setup:
 */
పూర్ణాంకం sched_fork(u64 clone_flags, నిర్మాణం task_struct *p)
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
	p->prio = current->సాధారణ_prio;

	uclamp_fork(p);

	/*
	 * Revert to default priority/policy on fork if requested.
	 */
	ఒకవేళ (unlikely(p->sched_reset_on_fork)) {
		ఒకవేళ (task_has_dl_policy(p) || task_has_rt_policy(p)) {
			p->policy = SCHED_సాధారణ;
			p->static_prio = NICE_గమ్యం_PRIO(0);
			p->rt_ప్రాధాన్యత = 0;
			p->timer_slack_ns = p->default_timer_slack_ns;
		} లేకపోతే ఒకవేళ (PRIO_గమ్యం_NICE(p->static_prio) < 0)
			p->static_prio = NICE_గమ్యం_PRIO(0);

		p->prio = p->సాధారణ_prio = p->static_prio;
		సమితి_ఎక్కించు_బరువు(p, false);
		p->se.custom_ముక్క = 0;
		p->se.ముక్క = sysctl_sched_పునాది_ముక్క;

		/*
		 * We don't need the reset flag anymore after the fork. It has
		 * fulfilled its duty:
		 */
		p->sched_reset_on_fork = 0;
	}

	ఒకవేళ (dl_prio(p->prio))
		తిరిగి -EAGAIN;

	scx_pre_fork(p);

	ఒకవేళ (rt_prio(p->prio)) {
		p->sched_class = &rt_sched_class;
#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
	} లేకపోతే ఒకవేళ (task_should_scx(p->policy)) {
		p->sched_class = &ext_sched_class;
#అయితే_ముగింపు
	} లేకపోతే {
		p->sched_class = &fair_sched_class;
	}

	init_entity_runnable_సరాసరి(&p->se);


#నిర్వచితమైతే CONFIG_SCHED_INFO
	ఒకవేళ (likely(sched_info_on()))
		memset(&p->sched_info, 0, పరిమాణం(p->sched_info));
#అయితే_ముగింపు
	p->on_cpu = 0;
	init_task_preempt_లెక్క(p);
	plist_కణుపు_init(&p->pushable_tasks, MAX_PRIO);
	RB_CLEAR_కణుపు(&p->pushable_dl_tasks);

	తిరిగి 0;
}

పూర్ణాంకం sched_cgroup_fork(నిర్మాణం task_struct *p, నిర్మాణం kernel_clone_వాదనలు *kargs)
{
	గుర్తులేని దీర్ఘ flags;

	/*
	 * Because we're not yet on the pid-hash, p->pi_lock isn't strictly
	 * required yet, but lockdep gets upset if rules are violated.
	 */
	raw_spin_తాళం_irqsave(&p->pi_తాళం, flags);
#నిర్వచితమైతే CONFIG_CGROUP_SCHED
	ఒకవేళ (1) {
		నిర్మాణం task_సమూహం *tg;
		tg = container_యొక్క(kargs->cset->subsys[cpu_cgrp_id],
				  నిర్మాణం task_సమూహం, css);
		tg = autogroup_task_సమూహం(p, tg);
		p->sched_task_సమూహం = tg;
	}
#అయితే_ముగింపు
	/*
	 * We're setting the CPU for the first time, we don't migrate,
	 * so use __set_task_cpu().
	 */
	__set_task_cpu(p, smp_processor_id());
	ఒకవేళ (p->sched_class->task_fork)
		p->sched_class->task_fork(p);
	raw_spin_unlock_irqrestore(&p->pi_తాళం, flags);

	తిరిగి scx_fork(p, kargs);
}

శూన్య sched_cancel_fork(నిర్మాణం task_struct *p)
{
	scx_cancel_fork(p);
}

స్థిర శూన్య sched_mm_cid_fork(నిర్మాణం task_struct *t);

శూన్య sched_post_fork(నిర్మాణం task_struct *p)
{
	sched_mm_cid_fork(p);
	uclamp_post_fork(p);
	scx_post_fork(p);
}

u64 గమ్యం_నిష్పత్తి(u64 period, u64 runtime)
{
	ఒకవేళ (runtime == RUNTIME_INF)
		తిరిగి BW_UNIT;

	/*
	 * Doing this here saves a lot of checks in all
	 * the calling paths, and returning zero seems
	 * safe for them anyway.
	 */
	ఒకవేళ (period == 0)
		తిరిగి 0;

	తిరిగి div64_u64(runtime << BW_జరుపు, period);
}

/*
 * wake_up_new_task - wake up a newly created task for the first time.
 *
 * This function will do some initial scheduler statistics housekeeping
 * that must be done for every newly created context, then puts the task
 * on the runqueue and wakes it.
 */
శూన్య wake_పైకి_new_task(నిర్మాణం task_struct *p)
{
	నిర్మాణం rq_flags rf;
	నిర్మాణం rq *rq;
	పూర్ణాంకం wake_flags = WF_FORK;

	raw_spin_తాళం_irqsave(&p->pi_తాళం, rf.flags);
	WRITE_ONCE(p->__state, TASK_RUNNING);
	/*
	 * Fork balancing, do it here and not earlier because:
	 *  - cpus_ptr can change in the fork path
	 *  - any previously selected CPU might disappear through hotplug
	 *
	 * Use __set_task_cpu() to avoid calling sched_class::migrate_task_rq,
	 * as we're not fully set-up yet.
	 */
	p->recent_వాడిన_cpu = task_cpu(p);
	__set_task_cpu(p, select_task_rq(p, task_cpu(p), &wake_flags));
	rq = __task_rq_lock(p, &rf);
	update_rq_clock(rq);
	post_init_entity_util_avg(p);

	activate_task(rq, p, వరుసలో_చేర్చు_NOCLOCK | వరుసలో_చేర్చు_INITIAL);
	trace_sched_wakeup_new(p);
	wakeup_preempt(rq, p, wake_flags);
	ఒకవేళ (p->sched_class->task_woken) {
		/*
		 * Nothing relies on rq->lock after this, so it's fine to
		 * drop it.
		 */
		rq_unpin_తాళం(rq, &rf);
		p->sched_class->task_woken(rq, p);
		rq_repin_తాళం(rq, &rf);
	}
	task_rq_unlock(rq, p, &rf);
}

#నిర్వచితమైతే CONFIG_PREEMPT_NOTIFIERS

స్థిర DEFINE_STATIC_కీలకం_FALSE(preempt_notifier_కీలకం);

శూన్య preempt_notifier_inc(శూన్య)
{
	static_branch_inc(&preempt_notifier_కీలకం);
}
EXPORT_చిహ్నం_GPL(preempt_notifier_inc);

శూన్య preempt_notifier_dec(శూన్య)
{
	static_branch_dec(&preempt_notifier_కీలకం);
}
EXPORT_చిహ్నం_GPL(preempt_notifier_dec);

/**
 * preempt_notifier_register - tell me when current is being preempted & rescheduled
 * @notifier: notifier struct to register
 */
శూన్య preempt_notifier_రిజిస్టరు(నిర్మాణం preempt_notifier *notifier)
{
	ఒకవేళ (!static_branch_unlikely(&preempt_notifier_కీలకం))
		WARN(1, "registering preempt_notifier while notifiers disabled\n");

	hlist_కూడు_తల(&notifier->link, &current->preempt_notifiers);
}
EXPORT_చిహ్నం_GPL(preempt_notifier_రిజిస్టరు);

/**
 * preempt_notifier_unregister - no longer interested in preemption notifications
 * @notifier: notifier struct to unregister
 *
 * This is *not* safe to call from within a preemption notifier.
 */
శూన్య preempt_notifier_unregister(నిర్మాణం preempt_notifier *notifier)
{
	hlist_del(&notifier->link);
}
EXPORT_చిహ్నం_GPL(preempt_notifier_unregister);

స్థిర శూన్య __fire_sched_in_preempt_notifiers(నిర్మాణం task_struct *curr)
{
	నిర్మాణం preempt_notifier *notifier;

	hlist_for_each_నమోదు(notifier, &curr->preempt_notifiers, link)
		notifier->ops->sched_లోపల(notifier, raw_smp_processor_id());
}

స్థిర __always_inline శూన్య fire_sched_లోపల_preempt_notifiers(నిర్మాణం task_struct *curr)
{
	ఒకవేళ (static_branch_unlikely(&preempt_notifier_కీలకం))
		__fire_sched_in_preempt_notifiers(curr);
}

స్థిర శూన్య
__fire_sched_out_preempt_notifiers(నిర్మాణం task_struct *curr,
				   నిర్మాణం task_struct *తదుపరి)
{
	నిర్మాణం preempt_notifier *notifier;

	hlist_for_each_నమోదు(notifier, &curr->preempt_notifiers, link)
		notifier->ops->sched_బహిర్గతం(notifier, తదుపరి);
}

స్థిర __always_inline శూన్య
fire_sched_బహిర్గతం_preempt_notifiers(నిర్మాణం task_struct *curr,
				 నిర్మాణం task_struct *తదుపరి)
{
	ఒకవేళ (static_branch_unlikely(&preempt_notifier_కీలకం))
		__fire_sched_out_preempt_notifiers(curr, తదుపరి);
}

#లేకపోతే /* !CONFIG_PREEMPT_NOTIFIERS: */

స్థిర అంతఃవరుస శూన్య fire_sched_లోపల_preempt_notifiers(నిర్మాణం task_struct *curr)
{
}

స్థిర అంతఃవరుస శూన్య
fire_sched_బహిర్గతం_preempt_notifiers(నిర్మాణం task_struct *curr,
				 నిర్మాణం task_struct *తదుపరి)
{
}

#అయితే_ముగింపు /* !CONFIG_PREEMPT_NOTIFIERS */

స్థిర అంతఃవరుస శూన్య prepare_task(నిర్మాణం task_struct *తదుపరి)
{
	/*
	 * Claim the task as running, we do this before switching to it
	 * such that any running task will have this set.
	 *
	 * See the smp_load_acquire(&p->on_cpu) case in ttwu() and
	 * its ordering comment.
	 */
	WRITE_ONCE(తదుపరి->on_cpu, 1);
}

స్థిర అంతఃవరుస శూన్య ముగింపు_task(నిర్మాణం task_struct *prev)
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
స్థిర శూన్య zap_balance_callbacks(నిర్మాణం rq *rq)
{
	నిర్మాణం balance_callback *తదుపరి, *తల;
	bool దొరికింది = false;

	lockdep_assert_rq_held(rq);

	తల = rq->balance_callback;
	అయితే (తల) {
		ఒకవేళ (తల == &balance_నెట్టు_callback)
			దొరికింది = true;
		తదుపరి = తల->తదుపరి;
		తల->తదుపరి = ఖాళీ;
		తల = తదుపరి;
	}
	rq->balance_callback = దొరికింది ? &balance_నెట్టు_callback : ఖాళీ;
}

స్థిర శూన్య do_balance_callbacks(నిర్మాణం rq *rq, నిర్మాణం balance_callback *తల)
{
	శూన్య (*func)(నిర్మాణం rq *rq);
	నిర్మాణం balance_callback *తదుపరి;

	lockdep_assert_rq_held(rq);

	అయితే (తల) {
		func = తల->func;
		తదుపరి = తల->తదుపరి;
		తల->తదుపరి = ఖాళీ;
		తల = తదుపరి;

		func(rq);
	}
}

స్థిర శూన్య balance_నెట్టు(నిర్మాణం rq *rq);

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
నిర్మాణం balance_callback balance_నెట్టు_callback = {
	.తదుపరి = ఖాళీ,
	.func = balance_నెట్టు,
};

స్థిర అంతఃవరుస నిర్మాణం balance_callback *
__splice_balance_callbacks(నిర్మాణం rq *rq, bool విభజించు)
{
	నిర్మాణం balance_callback *తల = rq->balance_callback;

	ఒకవేళ (likely(!తల))
		తిరిగి ఖాళీ;

	lockdep_assert_rq_held(rq);
	/*
	 * Must not take balance_push_callback off the list when
	 * splice_balance_callbacks() and balance_callbacks() are not
	 * in the same rq->lock section.
	 *
	 * In that case it would be possible for __schedule() to interleave
	 * and observe the list empty.
	 */
	ఒకవేళ (విభజించు && తల == &balance_నెట్టు_callback)
		తల = ఖాళీ;
	లేకపోతే
		rq->balance_callback = ఖాళీ;

	తిరిగి తల;
}

నిర్మాణం balance_callback *splice_balance_callbacks(నిర్మాణం rq *rq)
{
	తిరిగి __splice_balance_callbacks(rq, true);
}

శూన్య __balance_callbacks(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
{
	ఒకవేళ (rf)
		rq_unpin_తాళం(rq, rf);
	do_balance_callbacks(rq, __splice_balance_callbacks(rq, false));
	ఒకవేళ (rf)
		rq_repin_తాళం(rq, rf);
}

శూన్య balance_callbacks(నిర్మాణం rq *rq, నిర్మాణం balance_callback *తల)
{
	గుర్తులేని దీర్ఘ flags;

	ఒకవేళ (unlikely(తల)) {
		raw_spin_rq_తాళం_irqsave(rq, flags);
		do_balance_callbacks(rq, తల);
		raw_spin_rq_unlock_irqrestore(rq, flags);
	}
}

స్థిర అంతఃవరుస శూన్య
prepare_తాళం_switch(నిర్మాణం rq *rq, నిర్మాణం task_struct *తదుపరి, నిర్మాణం rq_flags *rf)
	__releases(__rq_lockp(rq))
	__acquires(__rq_lockp(this_rq()))
{
	/*
	 * Since the runqueue lock will be released by the next
	 * task (which is an invalid locking op but in the case
	 * of the scheduler it's an obvious special-case), so we
	 * do an early lockdep release here:
	 */
	rq_unpin_తాళం(rq, rf);
	spin_release(&__rq_lockp(rq)->dep_map, _THIS_IP_);
#నిర్వచితమైతే CONFIG_DEBUG_SPINLOCK
	/* this is a valid case when another task releases the spinlock */
	rq_lockp(rq)->owner = తదుపరి;
#అయితే_ముగింపు
	/*
	 * Model the rq reference switcheroo.
	 */
	__release(__rq_lockp(rq));
	__acquire(__rq_lockp(this_rq()));
}

స్థిర అంతఃవరుస శూన్య ముగింపు_తాళం_switch(నిర్మాణం rq *rq)
	__releases(__rq_lockp(rq))
{
	/*
	 * If we are tracking spinlock dependencies then we have to
	 * fix up the runqueue lock - which gets 'carried over' from
	 * prev into current:
	 */
	spin_acquire(&__rq_lockp(rq)->dep_map, 0, 0, _THIS_IP_);
	__balance_callbacks(rq, ఖాళీ);
	hrtick_కాలపట్టిక_exit(rq);
	raw_spin_rq_unlock_irq(rq);
}

/*
 * NOP if the arch has not defined these:
 */

#నిర్వచితం_కాకపోతే prepare_arch_switch
# define prepare_arch_switch(తదుపరి)	చేయి { } అయితే (0)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే ముగింపు_arch_post_తాళం_switch
# define ముగింపు_arch_post_తాళం_switch()	చేయి { } అయితే (0)
#అయితే_ముగింపు

స్థిర అంతఃవరుస శూన్య kmap_local_sched_బహిర్గతం(శూన్య)
{
#నిర్వచితమైతే CONFIG_KMAP_LOCAL
	ఒకవేళ (unlikely(current->kmap_ctrl.idx))
		__kmap_local_sched_out();
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస శూన్య kmap_local_sched_లోపల(శూన్య)
{
#నిర్వచితమైతే CONFIG_KMAP_LOCAL
	ఒకవేళ (unlikely(current->kmap_ctrl.idx))
		__kmap_local_sched_in();
#అయితే_ముగింపు
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
స్థిర అంతఃవరుస శూన్య
prepare_task_switch(నిర్మాణం rq *rq, నిర్మాణం task_struct *prev,
		    నిర్మాణం task_struct *తదుపరి)
	__must_hold(__rq_lockp(rq))
{
	kcov_prepare_switch(prev);
	sched_info_switch(rq, prev, తదుపరి);
	perf_event_task_sched_బహిర్గతం(prev, తదుపరి);
	fire_sched_బహిర్గతం_preempt_notifiers(prev, తదుపరి);
	kmap_local_sched_బహిర్గతం();
	prepare_task(తదుపరి);
	prepare_arch_switch(తదుపరి);
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
స్థిర నిర్మాణం rq *ముగింపు_task_switch(నిర్మాణం task_struct *prev)
	__releases(__rq_lockp(this_rq()))
{
	నిర్మాణం rq *rq = this_rq();
	నిర్మాణం mm_struct *mm = rq->prev_mm;
	గుర్తులేని పూర్ణాంకం prev_state;

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
	ఒకవేళ (WARN_ONCE(preempt_లెక్క() != 2*PREEMPT_DISABLE_స్థానభ్రంశం,
		      "corrupted preempt_count: %s/%d/0x%x\n",
		      current->comm, current->pid, preempt_లెక్క()))
		preempt_లెక్క_సమితి(FORK_PREEMPT_లెక్క);

	rq->prev_mm = ఖాళీ;

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
	perf_event_task_sched_లోపల(prev, current);
	ముగింపు_task(prev);
	tick_nohz_task_switch();
	ముగింపు_తాళం_switch(rq);
	ముగింపు_arch_post_తాళం_switch();
	kcov_ముగింపు_switch(current);
	/*
	 * kmap_local_sched_out() is invoked with rq::lock held and
	 * interrupts disabled. There is no requirement for that, but the
	 * sched out code does not have an interrupt enabled section.
	 * Restoring the maps on sched in does not require interrupts being
	 * disabled either.
	 */
	kmap_local_sched_లోపల();

	/*
	 * Any cached block-layer timestamp (plug->cur_ktime) is stale now,
	 * invalidate it.
	 */
	blk_plug_invalidate_ts();

	fire_sched_లోపల_preempt_notifiers(current);
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
	ఒకవేళ (mm) {
		membarrier_mm_sync_core_before_usermode(mm);
		mmdrop_lazy_tlb_sched(mm);
	}

	ఒకవేళ (unlikely(prev_state == TASK_DEAD)) {
		ఒకవేళ (prev->sched_class->task_dead)
			prev->sched_class->task_dead(prev);

		/*
		 * sched_ext_dead() must come before cgroup_task_dead() to
		 * prevent cgroups from being removed while its member tasks are
		 * visible to SCX schedulers.
		 */
		sched_ext_dead(prev);
		cgroup_task_dead(prev);

		/* Task is done with its stack. */
		put_task_దొంతర(prev);

		put_task_struct_rcu_user(prev);
	}

	తిరిగి rq;
}

/**
 * schedule_tail - first thing a freshly forked thread must call.
 * @prev: the thread we just switched away from.
 */
asmlinkage __visible శూన్య కాలపట్టిక_తోక(నిర్మాణం task_struct *prev)
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

	ముగింపు_task_switch(prev);
	/*
	 * This is a special case: the newly created task has just
	 * switched the context for the first time. It is returning from
	 * schedule for the first time in this path.
	 */
	trace_sched_exit_tp(true);
	preempt_enable();

	ఒకవేళ (current->సమితి_బిడ్డ_tid)
		put_user(task_pid_vnr(current), current->సమితి_బిడ్డ_tid);

	calculate_sigpending();
}

/*
 * context_switch - switch to the new MM and the new thread's register state.
 */
స్థిర __always_inline నిర్మాణం rq *
context_switch(నిర్మాణం rq *rq, నిర్మాణం task_struct *prev,
	       నిర్మాణం task_struct *తదుపరి, నిర్మాణం rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	prepare_task_switch(rq, prev, తదుపరి);

	/*
	 * For paravirt, this is coupled with an exit in switch_to to
	 * combine the page table reload and the switch backend into
	 * one hypercall.
	 */
	arch_ప్రారంభం_context_switch(prev);

	/*
	 * kernel -> kernel   lazy + transfer active
	 *   user -> kernel   lazy + mmgrab_lazy_tlb() active
	 *
	 * kernel ->   user   switch + mmdrop_lazy_tlb() active
	 *   user ->   user   switch
	 */
	ఒకవేళ (!తదుపరి->mm) {				// to kernel
		enter_lazy_tlb(prev->చురుకైన_mm, తదుపరి);

		తదుపరి->చురుకైన_mm = prev->చురుకైన_mm;
		ఒకవేళ (prev->mm)				// from user
			mmgrab_lazy_tlb(prev->చురుకైన_mm);
		లేకపోతే
			prev->చురుకైన_mm = ఖాళీ;
	} లేకపోతే {					// to user
		membarrier_switch_mm(rq, prev->చురుకైన_mm, తదుపరి->mm);
		/*
		 * sys_membarrier() requires an smp_mb() between setting
		 * rq->curr / membarrier_switch_mm() and returning to userspace.
		 *
		 * The below provides this either through switch_mm(), or in
		 * case 'prev->active_mm == next->mm' through
		 * finish_task_switch()'s mmdrop().
		 */
		switch_mm_irqs_off(prev->చురుకైన_mm, తదుపరి->mm, తదుపరి);
		lru_gen_use_mm(తదుపరి->mm);

		ఒకవేళ (!prev->mm) {			// from kernel
			/* will mmdrop_lazy_tlb() in finish_task_switch(). */
			rq->prev_mm = prev->చురుకైన_mm;
			prev->చురుకైన_mm = ఖాళీ;
		}
	}

	mm_cid_switch_గమ్యం(prev, తదుపరి);

	/*
	 * Tell rseq that the task was scheduled in. Must be after
	 * switch_mm_cid() to get the TIF flag set.
	 */
	rseq_sched_switch_event(తదుపరి);

	prepare_తాళం_switch(rq, తదుపరి, rf);

	/* Here we just switch the register state and the stack. */
	switch_గమ్యం(prev, తదుపరి, prev);
	barrier();

	తిరిగి ముగింపు_task_switch(prev);
}

/*
 * nr_running and nr_context_switches:
 *
 * externally visible scheduler statistics: current number of runnable
 * threads, total number of context switches performed since bootup.
 */
గుర్తులేని పూర్ణాంకం nr_running(శూన్య)
{
	గుర్తులేని పూర్ణాంకం i, కూడిక = 0;

	for_each_online_cpu(i)
		కూడిక += cpu_rq(i)->nr_running;

	తిరిగి కూడిక;
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
bool single_task_running(శూన్య)
{
	తిరిగి raw_rq()->nr_running == 1;
}
EXPORT_చిహ్నం(single_task_running);

గుర్తులేని దీర్ఘ దీర్ఘ nr_context_switches_cpu(పూర్ణాంకం cpu)
{
	తిరిగి cpu_rq(cpu)->nr_switches;
}

గుర్తులేని దీర్ఘ దీర్ఘ nr_context_switches(శూన్య)
{
	పూర్ణాంకం i;
	గుర్తులేని దీర్ఘ దీర్ఘ కూడిక = 0;

	for_each_possible_cpu(i)
		కూడిక += cpu_rq(i)->nr_switches;

	తిరిగి కూడిక;
}

/*
 * Return whether any tasks are accounted as I/O waiters on this CPU.
 * They may wake up on a different CPU.
 */
bool cpu_has_iowait(పూర్ణాంకం cpu)
{
	తిరిగి atomic_read(&cpu_rq(cpu)->nr_iowait) != 0;
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

గుర్తులేని పూర్ణాంకం nr_iowait(శూన్య)
{
	గుర్తులేని పూర్ణాంకం i, కూడిక = 0;

	for_each_possible_cpu(i)
		కూడిక += atomic_read(&cpu_rq(i)->nr_iowait);

	తిరిగి కూడిక;
}

/*
 * sched_exec - execve() is a valuable balancing opportunity, because at
 * this point the task has the smallest effective memory and cache footprint.
 */
శూన్య sched_exec(శూన్య)
{
	నిర్మాణం task_struct *p = current;
	నిర్మాణం migration_arg arg;
	పూర్ణాంకం dest_cpu;

	scoped_guard (raw_spinlock_irqsave, &p->pi_తాళం) {
		dest_cpu = p->sched_class->select_task_rq(p, task_cpu(p), WF_EXEC);
		ఒకవేళ (dest_cpu == smp_processor_id())
			తిరిగి;

		ఒకవేళ (unlikely(!cpu_చురుకైన(dest_cpu)))
			తిరిగి;

		arg = (నిర్మాణం migration_arg){ p, dest_cpu };
	}
	stop_one_cpu(task_cpu(p), migration_cpu_stop, &arg);
}

DEFINE_చొప్పున_CPU(నిర్మాణం kernel_stat, kstat);
DEFINE_చొప్పున_CPU(నిర్మాణం kernel_cpustat, kernel_cpustat) = {
#నిర్వచితమైతే CONFIG_NO_HZ_COMMON
	.idle_sleeptime_seq = SEQCNT_ZERO(kernel_cpustat.idle_sleeptime_seq)
#అయితే_ముగింపు
};

EXPORT_చొప్పున_CPU_చిహ్నం(kstat);
EXPORT_చొప్పున_CPU_చిహ్నం(kernel_cpustat);

/*
 * The function fair_sched_class.update_curr accesses the struct curr
 * and its field curr->exec_start; when called from task_sched_runtime(),
 * we observe a high rate of cache misses in practice.
 * Prefetching this data results in improved performance.
 */
స్థిర అంతఃవరుస శూన్య prefetch_curr_exec_ప్రారంభం(నిర్మాణం task_struct *p)
{
	నిర్మాణం sched_entity *curr = task_rq(p)->cfs.curr;

	prefetch(curr);
	prefetch(&curr->exec_ప్రారంభం);
}

/*
 * Return accounted runtime for the task.
 * In case the task is currently running, return the runtime plus current's
 * pending runtime that have not been accounted yet.
 */
గుర్తులేని దీర్ఘ దీర్ఘ task_sched_runtime(నిర్మాణం task_struct *p)
{
	నిర్మాణం rq_flags rf;
	నిర్మాణం rq *rq;
	u64 ns;

#నిర్వచితమైతే CONFIG_64_బిట్టు
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
	ఒకవేళ (!p->on_cpu || !task_on_rq_queued(p))
		తిరిగి p->se.కూడిక_exec_runtime;
#అయితే_ముగింపు

	rq = task_rq_తాళం_2(p, &rf);
	/*
	 * Must be ->curr _and_ ->on_rq.  If dequeued, we would
	 * project cycles that may never be accounted to this
	 * thread, breaking clock_gettime().
	 */
	ఒకవేళ (task_current_donor(rq, p) && task_on_rq_queued(p)) {
		prefetch_curr_exec_ప్రారంభం(p);
		update_rq_clock(rq);
		p->sched_class->update_curr(rq);
	}
	ns = p->se.కూడిక_exec_runtime;
	task_rq_unlock(rq, p, &rf);

	తిరిగి ns;
}

స్థిర u64 cpu_resched_latency(నిర్మాణం rq *rq)
{
	పూర్ణాంకం latency_warn_ms = READ_ONCE(sysctl_resched_latency_warn_ms);
	u64 resched_latency, now = rq_clock(rq);
	స్థిర bool warned_once;

	ఒకవేళ (sysctl_resched_latency_warn_once && warned_once)
		తిరిగి 0;

	ఒకవేళ (!need_resched() || !latency_warn_ms)
		తిరిగి 0;

	ఒకవేళ (system_state == SYSTEM_BOOTING)
		తిరిగి 0;

	ఒకవేళ (!rq->చివరి_చూసిన_need_resched_ns) {
		rq->చివరి_చూసిన_need_resched_ns = now;
		rq->ticks_without_resched = 0;
		తిరిగి 0;
	}

	rq->ticks_without_resched++;
	resched_latency = now - rq->చివరి_చూసిన_need_resched_ns;
	ఒకవేళ (resched_latency <= latency_warn_ms * NSEC_చొప్పున_MSEC)
		తిరిగి 0;

	warned_once = true;

	తిరిగి resched_latency;
}

స్థిర పూర్ణాంకం __init అమరిక_resched_latency_warn_ms(అక్షరం *str)
{
	దీర్ఘ val;

	ఒకవేళ ((kstrtol(str, 0, &val))) {
		pr_warn("Unable to set resched_latency_warn_ms\n");
		తిరిగి 1;
	}

	sysctl_resched_latency_warn_ms = val;
	తిరిగి 1;
}
__setup("resched_latency_warn_ms=", అమరిక_resched_latency_warn_ms);

/*
 * This function gets called by the timer code, with HZ frequency.
 * We call it with interrupts disabled.
 */
శూన్య sched_tick(శూన్య)
{
	పూర్ణాంకం cpu = smp_processor_id();
	నిర్మాణం rq *rq = cpu_rq(cpu);
	/* scheduler accounting goes to the donor task */
	నిర్మాణం task_struct *curr, *donor;
	నిర్మాణం rq_flags rf;
	గుర్తులేని దీర్ఘ hw_pressure;
	u64 resched_latency;

	ఒకవేళ (!cpu_preferred(cpu))
		sched_నెట్టు_current_non_preferred_cpu(rq);

	ఒకవేళ (housekeeping_cpu(cpu, HK_TYPE_KERNEL_NOISE))
		arch_scale_freq_tick();

	sched_clock_tick();

	rq_తాళం(rq, &rf);
	curr = rq->curr;
	donor = rq->donor;

	psi_account_irqtime(rq, curr, ఖాళీ);

	update_rq_clock(rq);
	hw_pressure = arch_scale_hw_pressure(cpu_యొక్క(rq));
	update_hw_ఎక్కించు_avg(rq_clock_task(rq), rq, hw_pressure);

	ఒకవేళ (dynamic_preempt_lazy() && tif_test_బిట్టు(TIF_NEED_RESCHED_LAZY))
		resched_curr(rq);

	donor->sched_class->task_tick(rq, donor, 0);
	ఒకవేళ (sched_feat(LATENCY_WARN))
		resched_latency = cpu_resched_latency(rq);
	calc_global_ఎక్కించు_tick(rq);
	sched_core_tick(rq);
	scx_tick(rq);

	rq_unlock(rq, &rf);

	ఒకవేళ (sched_feat(LATENCY_WARN) && resched_latency)
		resched_latency_warn(cpu, resched_latency);

	perf_event_task_tick();

	ఒకవేళ (curr->flags & PF_WQ_కార్మికుడు)
		wq_కార్మికుడు_tick(curr);

	ఒకవేళ (!scx_switched_all()) {
		rq->idle_balance = idle_cpu(cpu);
		sched_balance_trigger(rq);
	}
}

#నిర్వచితమైతే CONFIG_NO_HZ_నిండిన

నిర్మాణం tick_work {
	పూర్ణాంకం			cpu;
	atomic_t		state;
	నిర్మాణం delayed_work	work;
};
/* Values for ->state, see diagram below. */
#నిర్వచించు TICK_SCHED_REMOTE_OFFLINE	0
#నిర్వచించు TICK_SCHED_REMOTE_OFFLINING	1
#నిర్వచించు TICK_SCHED_REMOTE_RUNNING	2

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

స్థిర నిర్మాణం tick_work __percpu *tick_work_cpu;

స్థిర శూన్య sched_tick_remote(నిర్మాణం work_struct *work)
{
	నిర్మాణం delayed_work *dwork = గమ్యం_delayed_work(work);
	నిర్మాణం tick_work *twork = container_యొక్క(dwork, నిర్మాణం tick_work, work);
	పూర్ణాంకం cpu = twork->cpu;
	నిర్మాణం rq *rq = cpu_rq(cpu);
	పూర్ణాంకం os;

	/*
	 * Handle the tick only if it appears the remote CPU is running in full
	 * dynticks mode. The check is racy by nature, but missing a tick or
	 * having one too much is no big deal because the scheduler tick updates
	 * statistics and checks timeslices in a time-independent way, regardless
	 * of when exactly it is running.
	 */
	ఒకవేళ (tick_nohz_tick_stopped_cpu(cpu)) {
		guard(rq_తాళం_irq)(rq);
		నిర్మాణం task_struct *curr = rq->curr;

		ఒకవేళ (cpu_online(cpu)) {
			/*
			 * Since this is a remote tick for full dynticks mode,
			 * we are always sure that there is no proxy (only a
			 * single task is running).
			 */
			WARN_ON_ONCE(rq->curr != rq->donor);
			update_rq_clock(rq);

			ఒకవేళ (!is_idle_task(curr)) {
				/*
				 * Make sure the next tick runs within a
				 * reasonable amount of time.
				 */
				u64 delta = rq_clock_task(rq) - curr->se.exec_ప్రారంభం;
				WARN_ON_ONCE(delta > (u64)NSEC_చొప్పున_SEC * 30);
			}
			curr->sched_class->task_tick(rq, curr, 0);

			calc_ఎక్కించు_nohz_remote(rq);
		}
	}

	/*
	 * Run the remote tick once per second (1Hz). This arbitrary
	 * frequency is large enough to avoid overload but short enough
	 * to keep scheduler internal stats reasonably up to date.  But
	 * first update state to reflect hotplug activity if required.
	 */
	os = atomic_fetch_కూడు_unless(&twork->state, -1, TICK_SCHED_REMOTE_RUNNING);
	WARN_ON_ONCE(os == TICK_SCHED_REMOTE_OFFLINE);
	ఒకవేళ (os == TICK_SCHED_REMOTE_RUNNING)
		వరుస_delayed_work(system_dfl_wq, dwork, HZ);
}

స్థిర శూన్య sched_tick_ప్రారంభం(పూర్ణాంకం cpu)
{
	పూర్ణాంకం os;
	నిర్మాణం tick_work *twork;

	ఒకవేళ (housekeeping_cpu(cpu, HK_TYPE_KERNEL_NOISE))
		తిరిగి;

	WARN_ON_ONCE(!tick_work_cpu);

	twork = చొప్పున_cpu_ptr(tick_work_cpu, cpu);
	os = atomic_xchg(&twork->state, TICK_SCHED_REMOTE_RUNNING);
	WARN_ON_ONCE(os == TICK_SCHED_REMOTE_RUNNING);
	ఒకవేళ (os == TICK_SCHED_REMOTE_OFFLINE) {
		twork->cpu = cpu;
		INIT_DELAYED_WORK(&twork->work, sched_tick_remote);
		వరుస_delayed_work(system_dfl_wq, &twork->work, HZ);
	}
}

#నిర్వచితమైతే CONFIG_HOTPLUG_CPU
స్థిర శూన్య sched_tick_stop(పూర్ణాంకం cpu)
{
	నిర్మాణం tick_work *twork;
	పూర్ణాంకం os;

	ఒకవేళ (housekeeping_cpu(cpu, HK_TYPE_KERNEL_NOISE))
		తిరిగి;

	WARN_ON_ONCE(!tick_work_cpu);

	twork = చొప్పున_cpu_ptr(tick_work_cpu, cpu);
	/* There cannot be competing actions, but don't rely on stop-machine. */
	os = atomic_xchg(&twork->state, TICK_SCHED_REMOTE_OFFLINING);
	WARN_ON_ONCE(os != TICK_SCHED_REMOTE_RUNNING);
	/* Don't cancel, as this would mess up the state machine. */
}
#అయితే_ముగింపు /* CONFIG_HOTPLUG_CPU */

పూర్ణాంకం __init sched_tick_offload_init(శూన్య)
{
	tick_work_cpu = alloc_percpu(నిర్మాణం tick_work);
	BUG_ON(!tick_work_cpu);
	తిరిగి 0;
}

#లేకపోతే /* !CONFIG_NO_HZ_FULL: */
స్థిర అంతఃవరుస శూన్య sched_tick_ప్రారంభం(పూర్ణాంకం cpu) { }
స్థిర అంతఃవరుస శూన్య sched_tick_stop(పూర్ణాంకం cpu) { }
#అయితే_ముగింపు /* !CONFIG_NO_HZ_FULL */

#ఒకవేళ defined(CONFIG_PREEMPTION) && (defined(CONFIG_DEBUG_PREEMPT) || \
				defined(CONFIG_TRACE_PREEMPT_TOGGLE))
/*
 * If the value passed in is equal to the current preempt count
 * then we just disabled preemption. Start timing the latency.
 */
స్థిర అంతఃవరుస శూన్య preempt_latency_ప్రారంభం(పూర్ణాంకం val)
{
	ఒకవేళ (preempt_లెక్క() == val) {
		గుర్తులేని దీర్ఘ ip = get_తాళం_మాతృ_ip();
#నిర్వచితమైతే CONFIG_DEBUG_PREEMPT
		current->preempt_disable_ip = ip;
#అయితే_ముగింపు
		trace_preempt_off(CALLER_ADDR0, ip);
	}
}

శూన్య preempt_లెక్క_కూడు(పూర్ణాంకం val)
{
#నిర్వచితమైతే CONFIG_DEBUG_PREEMPT
	/*
	 * Underflow?
	 *
	 * Cannot detect underflow based on the current preempt_count() value
	 * if using HAS_SEPARATE_PREEMPT_RESCHED_BITS because preempt count takes all 32
	 * bits.
	 */
	ఒకవేళ (!IS_ENABLED(CONFIG_HAS_SEPARATE_PREEMPT_RESCHED_బిట్లు) &&
	    DEBUG_LOCKS_WARN_ON((preempt_లెక్క() < 0)))
		తిరిగి;
#అయితే_ముగింపు
	__preempt_count_add(val);
#నిర్వచితమైతే CONFIG_DEBUG_PREEMPT
	/*
	 * Spinlock count overflowing soon?
	 */
	DEBUG_LOCKS_WARN_ON((preempt_లెక్క() & PREEMPT_ముసుగు) >=
				PREEMPT_ముసుగు - 10);
#అయితే_ముగింపు
	preempt_latency_ప్రారంభం(val);
}
EXPORT_చిహ్నం(preempt_లెక్క_కూడు);
NOKPROBE_చిహ్నం(preempt_లెక్క_కూడు);

/*
 * If the value passed in equals to the current preempt count
 * then we just enabled preemption. Stop timing the latency.
 */
స్థిర అంతఃవరుస శూన్య preempt_latency_stop(పూర్ణాంకం val)
{
	ఒకవేళ (preempt_లెక్క() == val)
		trace_preempt_on(CALLER_ADDR0, get_తాళం_మాతృ_ip());
}

శూన్య preempt_లెక్క_sub(పూర్ణాంకం val)
{
#నిర్వచితమైతే CONFIG_DEBUG_PREEMPT
	/*
	 * Underflow?
	 */
	గుర్తులేని పూర్ణాంకం uval = val;
	గుర్తులేని పూర్ణాంకం pc = preempt_లెక్క();

	ఒకవేళ (DEBUG_LOCKS_WARN_ON(pc - uval > pc))
		తిరిగి;
	/*
	 * Is the spinlock portion underflowing?
	 */
	ఒకవేళ (DEBUG_LOCKS_WARN_ON((val < PREEMPT_ముసుగు) &&
			!(preempt_లెక్క() & PREEMPT_ముసుగు)))
		తిరిగి;
#అయితే_ముగింపు

	preempt_latency_stop(val);
	__preempt_count_sub(val);
}
EXPORT_చిహ్నం(preempt_లెక్క_sub);
NOKPROBE_చిహ్నం(preempt_లెక్క_sub);

#లేకపోతే
స్థిర అంతఃవరుస శూన్య preempt_latency_ప్రారంభం(పూర్ణాంకం val) { }
స్థిర అంతఃవరుస శూన్య preempt_latency_stop(పూర్ణాంకం val) { }
#అయితే_ముగింపు

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ get_preempt_disable_ip(నిర్మాణం task_struct *p)
{
#నిర్వచితమైతే CONFIG_DEBUG_PREEMPT
	తిరిగి p->preempt_disable_ip;
#లేకపోతే
	తిరిగి 0;
#అయితే_ముగింపు
}

/*
 * Print scheduling while atomic bug:
 */
స్థిర noinline శూన్య __schedule_bug(నిర్మాణం task_struct *prev)
{
	/* Save this before calling printk(), since that will clobber it */
	గుర్తులేని దీర్ఘ preempt_disable_ip = get_preempt_disable_ip(current);

	ఒకవేళ (oops_లోపల_పురోగతి)
		తిరిగి;

	printk(KERN_ERR "BUG: scheduling while atomic: %s/%d/0x%08x\n",
		prev->comm, prev->pid, preempt_లెక్క());

	debug_show_held_locks(prev);
	print_modules();
	ఒకవేళ (irqs_disabled())
		print_irqtrace_events(prev);
	ఒకవేళ (IS_ENABLED(CONFIG_DEBUG_PREEMPT)) {
		pr_err("Preemption disabled at:");
		print_ip_sym(KERN_ERR, preempt_disable_ip);
	}
	check_panic_on_warn("scheduling while atomic");

	dump_దొంతర();
	కూడు_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}

/*
 * Various schedule()-time debugging checks and statistics:
 */
స్థిర అంతఃవరుస శూన్య కాలపట్టిక_debug(నిర్మాణం task_struct *prev, bool preempt)
{
#నిర్వచితమైతే CONFIG_SCHED_దొంతర_చివర_CHECK
	ఒకవేళ (task_దొంతర_చివర_corrupted(prev))
		panic("corrupted stack end detected inside scheduler\n");

	ఒకవేళ (task_scs_చివర_corrupted(prev))
		panic("corrupted shadow stack detected inside scheduler\n");
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_DEBUG_ATOMIC_SLEEP
	ఒకవేళ (!preempt && READ_ONCE(prev->__state) && prev->non_ఖండం_లెక్క) {
		printk(KERN_ERR "BUG: scheduling in a non-blocking section: %s/%d/%i\n",
			prev->comm, prev->pid, prev->non_ఖండం_లెక్క);
		dump_దొంతర();
		కూడు_taint(TAINT_WARN, LOCKDEP_STILL_OK);
	}
#అయితే_ముగింపు

	ఒకవేళ (unlikely(లోపల_atomic_preempt_off())) {
		__schedule_bug(prev);
		preempt_లెక్క_సమితి(PREEMPT_DISABLED);
	}
	rcu_sleep_check();
	WARN_ON_ONCE(ct_state() == CT_STATE_USER);

	profile_తగిలింది(SCHED_PROFILING, __builtin_return_address(0));

	schedstat_inc(this_rq()->sched_లెక్క);
}

స్థిర శూన్య prev_balance(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
{
	స్థిరరాశి నిర్మాణం sched_class *ప్రారంభం_class = rq->donor->sched_class;
	స్థిరరాశి నిర్మాణం sched_class *class;

	/*
	 * We must do the balancing pass before put_prev_task(), such
	 * that when we release the rq->lock the task is in the same
	 * state as before we took rq->lock.
	 *
	 * We can terminate the balance pass as soon as we know there is
	 * a runnable task of @class priority or higher.
	 */
	for_చురుకైన_class_range(class, ప్రారంభం_class, &idle_sched_class) {
		ఒకవేళ (class->balance && class->balance(rq, rf))
			ఆపు;
	}
}

/*
 * Pick up the highest-prio task:
 */
స్థిర అంతఃవరుస నిర్మాణం task_struct *
__pick_next_task(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	స్థిరరాశి నిర్మాణం sched_class *class;
	నిర్మాణం task_struct *p;

	rq->dl_server = ఖాళీ;

	ఒకవేళ (scx_enabled())
		వెళ్ళు restart;

	/*
	 * Optimization: we know that if all tasks are in the fair class we can
	 * call that function directly, but only if the @prev task wasn't of a
	 * higher scheduling class, because otherwise those lose the
	 * opportunity to pull in more work from other CPUs.
	 */
	ఒకవేళ (likely(!sched_class_above(rq->donor->sched_class, &fair_sched_class) &&
		   rq->nr_running == rq->cfs.h_nr_queued)) {

		p = pick_task_fair(rq, rf);
		ఒకవేళ (unlikely(p == RETRY_TASK))
			వెళ్ళు restart;

		/* Assume the next prioritized class is idle_sched_class */
		ఒకవేళ (!p)
			p = pick_task_idle(rq, rf);

		put_prev_సమితి_తదుపరి_task(rq, rq->donor, p);
		తిరిగి p;
	}

restart:
	prev_balance(rq, rf);

	for_each_చురుకైన_class(class) {
		p = class->pick_task(rq, rf);
		ఒకవేళ (unlikely(p == RETRY_TASK))
			వెళ్ళు restart;
		ఒకవేళ (p) {
			put_prev_సమితి_తదుపరి_task(rq, rq->donor, p);
			తిరిగి p;
		}
	}

	BUG(); /* The idle class should always have a runnable task. */
}

#నిర్వచితమైతే CONFIG_SCHED_CORE
స్థిర అంతఃవరుస bool is_task_rq_idle(నిర్మాణం task_struct *t)
{
	తిరిగి (task_rq(t)->idle == t);
}

స్థిర అంతఃవరుస bool cookie_equals(నిర్మాణం task_struct *a, గుర్తులేని దీర్ఘ cookie)
{
	తిరిగి is_task_rq_idle(a) || (a->core_cookie == cookie);
}

స్థిర అంతఃవరుస bool cookie_సరిపోలిక(నిర్మాణం task_struct *a, నిర్మాణం task_struct *b)
{
	ఒకవేళ (is_task_rq_idle(a) || is_task_rq_idle(b))
		తిరిగి true;

	తిరిగి a->core_cookie == b->core_cookie;
}

/*
 * Careful; this can return RETRY_TASK, it does not include the retry-loop
 * itself due to the whole SMT pick retry thing below.
 */
స్థిర అంతఃవరుస నిర్మాణం task_struct *pick_task(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
{
	స్థిరరాశి నిర్మాణం sched_class *class;
	నిర్మాణం task_struct *p;

	rq->dl_server = ఖాళీ;

	for_each_చురుకైన_class(class) {
		p = class->pick_task(rq, rf);
		ఒకవేళ (p)
			తిరిగి p;
	}

	BUG(); /* The idle class should always have a runnable task. */
}

బాహ్య శూన్య task_vruntime_update(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, bool లోపల_fi);

స్థిర శూన్య వరుస_core_balance(నిర్మాణం rq *rq);

స్థిర నిర్మాణం task_struct *
pick_తదుపరి_task(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	నిర్మాణం task_struct *తదుపరి, *p, *max;
	స్థిరరాశి నిర్మాణం cpumask *smt_ముసుగు;
	bool fi_before = false;
	bool core_clock_updated = (rq == rq->core);
	గుర్తులేని దీర్ఘ cookie;
	పూర్ణాంకం i, cpu, occ = 0;
	నిర్మాణం rq *rq_i;
	bool need_sync = false;

	ఒకవేళ (!sched_core_enabled(rq))
		తిరిగి __pick_next_task(rq, rf);

	cpu = cpu_యొక్క(rq);

	/* Stopper task is switching into idle, no need core-wide selection. */
	ఒకవేళ (cpu_is_offline(cpu)) {
		/*
		 * Reset core_pick so that we don't enter the fastpath when
		 * coming online. core_pick would already be migrated to
		 * another cpu during offline.
		 */
		rq->core_pick = ఖాళీ;
		rq->core_dl_server = ఖాళీ;
		తిరిగి __pick_next_task(rq, rf);
	}

	rq->core->core_pick_లోపల_flight++;

	/*
	 * If there were no {en,de}queues since we picked (IOW, the task
	 * pointers are all still valid), and we haven't scheduled the last
	 * pick yet, do so now.
	 *
	 * rq->core_pick can be NULL if no selection was made for a CPU because
	 * it was either offline or went offline during a sibling's core-wide
	 * selection. In this case, do a core-wide selection.
	 */
	ఒకవేళ (rq->core->core_pick_seq == rq->core->core_task_seq &&
	    rq->core_pick) {
		తదుపరి = rq->core_pick;
		rq->dl_server = rq->core_dl_server;
		rq->core_pick = ఖాళీ;
		rq->core_dl_server = ఖాళీ;
		వెళ్ళు బహిర్గతం_సమితి_తదుపరి;
	}

	prev_balance(rq, rf);

	smt_ముసుగు = cpu_smt_ముసుగు(cpu);

restart:
	need_sync |= !!rq->core->core_cookie;

	/* reset state */
	rq->core->core_cookie = 0UL;
	ఒకవేళ (rq->core->core_forceidle_లెక్క) {
		ఒకవేళ (!core_clock_updated) {
			update_rq_clock(rq->core);
			core_clock_updated = true;
		}
		sched_core_account_forceidle(rq);
		/* reset after accounting force idle */
		rq->core->core_forceidle_ప్రారంభం = 0;
		rq->core->core_forceidle_లెక్క = 0;
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
	ఒకవేళ (!need_sync) {
		తదుపరి = pick_task(rq, rf);
		ఒకవేళ (unlikely(తదుపరి == RETRY_TASK)) {
			/* rq lock may have been dropped, clocks invalidated */
			core_clock_updated = false;
			ఒకవేళ (!(rq->clock_update_flags & RQCF_UPDATED))
				update_rq_clock(rq);
			వెళ్ళు restart;
		}

		ఒకవేళ (!తదుపరి->core_cookie) {
			rq->core_pick = ఖాళీ;
			rq->core_dl_server = ఖాళీ;
			/*
			 * For robustness, update the min_vruntime_fi for
			 * unconstrained picks as well.
			 */
			WARN_ON_ONCE(fi_before);
			task_vruntime_update(rq, తదుపరి, false);
			వెళ్ళు బహిర్గతం_సమితి_తదుపరి;
		}
	}

	/*
	 * For each thread: do the regular task pick and find the max prio task
	 * amongst them.
	 *
	 * Tie-break prio towards the current CPU
	 */
	max = ఖాళీ;
	for_each_cpu_wrap(i, smt_ముసుగు, cpu) {
		rq_i = cpu_rq(i);

		/*
		 * Current cpu always has its clock updated on entrance to
		 * pick_next_task(). If the current cpu is not the core,
		 * the core may also have been updated above.
		 */
		ఒకవేళ (i != cpu && (rq_i != rq->core || !core_clock_updated))
			update_rq_clock(rq_i);

		p = pick_task(rq_i, rf);
		ఒకవేళ (unlikely(p == RETRY_TASK)) {
			/* rq lock may have been dropped, clocks invalidated */
			core_clock_updated = false;
			ఒకవేళ (!(rq->clock_update_flags & RQCF_UPDATED))
				update_rq_clock(rq);
			వెళ్ళు restart;
		}

		rq_i->core_pick = p;
		rq_i->core_dl_server = rq_i->dl_server;

		ఒకవేళ (!max || prio_less(max, p, fi_before))
			max = p;
	}

	cookie = rq->core->core_cookie = max->core_cookie;

	/*
	 * For each thread: try and find a runnable task that matches @max or
	 * force idle.
	 */
	for_each_cpu(i, smt_ముసుగు) {
		rq_i = cpu_rq(i);
		p = rq_i->core_pick;

		ఒకవేళ (!cookie_equals(p, cookie)) {
			p = ఖాళీ;
			ఒకవేళ (cookie)
				p = sched_core_కనుగొను(rq_i, cookie);
			ఒకవేళ (!p)
				p = idle_sched_class.pick_task(rq_i, rf);
		}

		rq_i->core_pick = p;
		rq_i->core_dl_server = ఖాళీ;

		ఒకవేళ (p == rq_i->idle) {
			ఒకవేళ (rq_i->nr_running) {
				rq->core->core_forceidle_లెక్క++;
				ఒకవేళ (!fi_before)
					rq->core->core_forceidle_seq++;
			}
		} లేకపోతే {
			occ++;
		}
	}

	ఒకవేళ (schedstat_enabled() && rq->core->core_forceidle_లెక్క) {
		rq->core->core_forceidle_ప్రారంభం = rq_clock(rq->core);
		rq->core->core_forceidle_occupation = occ;
	}

	rq->core->core_pick_seq = rq->core->core_task_seq;
	తదుపరి = rq->core_pick;

	/* Something should have been selected for current CPU */
	WARN_ON_ONCE(!తదుపరి);

	/*
	 * Reschedule siblings
	 *
	 * NOTE: L1TF -- at this point we're no longer running the old task and
	 * sending an IPI (below) ensures the sibling will no longer be running
	 * their task. This ensures there is no inter-sibling overlap between
	 * non-matching user state.
	 */
	for_each_cpu(i, smt_ముసుగు) {
		rq_i = cpu_rq(i);

		/*
		 * An online sibling might have gone offline before a task
		 * could be picked for it, or it might be offline but later
		 * happen to come online, but its too late and nothing was
		 * picked for it.  That's Ok - it will pick tasks for itself,
		 * so ignore it.
		 */
		ఒకవేళ (!rq_i->core_pick)
			కొనసాగు;

		/*
		 * Update for new !FI->FI transitions, or if continuing to be in !FI:
		 * fi_before     fi      update?
		 *  0            0       1
		 *  0            1       1
		 *  1            0       1
		 *  1            1       0
		 */
		ఒకవేళ (!(fi_before && rq->core->core_forceidle_లెక్క))
			task_vruntime_update(rq_i, rq_i->core_pick, !!rq->core->core_forceidle_లెక్క);

		rq_i->core_pick->core_occupation = occ;

		ఒకవేళ (i == cpu) {
			rq_i->core_pick = ఖాళీ;
			rq_i->core_dl_server = ఖాళీ;
			కొనసాగు;
		}

		/* Did we break L1TF mitigation requirements? */
		WARN_ON_ONCE(!cookie_సరిపోలిక(తదుపరి, rq_i->core_pick));

		ఒకవేళ (rq_i->curr == rq_i->core_pick) {
			rq_i->core_pick = ఖాళీ;
			rq_i->core_dl_server = ఖాళీ;
			కొనసాగు;
		}

		resched_curr(rq_i);
	}

బహిర్గతం_సమితి_తదుపరి:
	rq->core->core_pick_లోపల_flight--;
	put_prev_సమితి_తదుపరి_task(rq, rq->donor, తదుపరి);
	ఒకవేళ (rq->core->core_forceidle_లెక్క && తదుపరి == rq->idle)
		వరుస_core_balance(rq);

	తిరిగి తదుపరి;
}

స్థిర bool try_steal_cookie(పూర్ణాంకం this, పూర్ణాంకం that)
{
	నిర్మాణం rq *dst = cpu_rq(this), *src = cpu_rq(that);
	నిర్మాణం task_struct *p;
	గుర్తులేని దీర్ఘ cookie;
	bool success = false;

	guard(irq)();
	guard(double_rq_తాళం)(dst, src);

	cookie = dst->core->core_cookie;
	ఒకవేళ (!cookie)
		తిరిగి false;

	ఒకవేళ (dst->curr != dst->idle)
		తిరిగి false;

	p = sched_core_కనుగొను(src, cookie);
	ఒకవేళ (!p)
		తిరిగి false;

	చేయి {
		ఒకవేళ (p == src->core_pick || p == src->curr || p == src->donor)
			వెళ్ళు తదుపరి;

		ఒకవేళ (task_is_blocked(p))
			వెళ్ళు తదుపరి;

		ఒకవేళ (!is_cpu_allowed(p, this))
			వెళ్ళు తదుపరి;

		ఒకవేళ (p->core_occupation > dst->idle->core_occupation)
			వెళ్ళు తదుపరి;
		/*
		 * sched_core_find() and sched_core_next() will ensure
		 * that task @p is not throttled now, we also need to
		 * check whether the runqueue of the destination CPU is
		 * being throttled.
		 */
		ఒకవేళ (sched_task_is_throttled(p, this))
			వెళ్ళు తదుపరి;

		move_queued_task_locked(src, dst, p);
		resched_curr(dst);

		success = true;
		ఆపు;

తదుపరి:
		p = sched_core_తదుపరి(p, cookie);
	} అయితే (p);

	తిరిగి success;
}

స్థిర bool steal_cookie_task(పూర్ణాంకం cpu, నిర్మాణం sched_domain *sd)
{
	పూర్ణాంకం i;

	for_each_cpu_wrap(i, sched_domain_పరిధి(sd), cpu + 1) {
		ఒకవేళ (i == cpu)
			కొనసాగు;

		ఒకవేళ (need_resched())
			ఆపు;

		ఒకవేళ (try_steal_cookie(cpu, i))
			తిరిగి true;
	}

	తిరిగి false;
}

స్థిర శూన్య sched_core_balance(నిర్మాణం rq *rq)
	__must_hold(__rq_lockp(rq))
{
	నిర్మాణం sched_domain *sd;
	పూర్ణాంకం cpu = cpu_యొక్క(rq);

	guard(preempt)();
	guard(rcu)();

	raw_spin_rq_unlock_irq(rq);
	for_each_domain(cpu, sd) {
		ఒకవేళ (need_resched())
			ఆపు;

		ఒకవేళ (steal_cookie_task(cpu, sd))
			ఆపు;
	}
	raw_spin_rq_తాళం_irq(rq);
}

స్థిర DEFINE_చొప్పున_CPU(నిర్మాణం balance_callback, core_balance_తల);

స్థిర శూన్య వరుస_core_balance(నిర్మాణం rq *rq)
{
	ఒకవేళ (!sched_core_enabled(rq))
		తిరిగి;

	ఒకవేళ (!rq->core->core_cookie)
		తిరిగి;

	ఒకవేళ (!rq->nr_running) /* not forced idle */
		తిరిగి;

	వరుస_balance_callback(rq, &చొప్పున_cpu(core_balance_తల, rq->cpu), sched_core_balance);
}

DEFINE_తాళం_GUARD_1(core_తాళం, పూర్ణాంకం,
		    sched_core_తాళం(*_T->తాళం, &_T->flags),
		    sched_core_unlock(*_T->తాళం, &_T->flags),
		    గుర్తులేని దీర్ఘ flags)

స్థిర శూన్య sched_core_cpu_starting(గుర్తులేని పూర్ణాంకం cpu)
{
	స్థిరరాశి నిర్మాణం cpumask *smt_ముసుగు = cpu_smt_ముసుగు(cpu);
	నిర్మాణం rq *rq = cpu_rq(cpu), *core_rq = ఖాళీ;
	పూర్ణాంకం t;

	guard(core_తాళం)(&cpu);

	WARN_ON_ONCE(rq->core != rq);

	/* if we're the first, we'll be our own leader */
	ఒకవేళ (cpumask_బరువు(smt_ముసుగు) == 1)
		తిరిగి;

	/* find the leader */
	for_each_cpu(t, smt_ముసుగు) {
		ఒకవేళ (t == cpu)
			కొనసాగు;
		rq = cpu_rq(t);
		ఒకవేళ (rq->core == rq) {
			core_rq = rq;
			ఆపు;
		}
	}

	ఒకవేళ (WARN_ON_ONCE(!core_rq)) /* whoopsie */
		తిరిగి;

	/* install and validate core_rq */
	for_each_cpu(t, smt_ముసుగు) {
		rq = cpu_rq(t);

		ఒకవేళ (t == cpu)
			rq->core = core_rq;

		WARN_ON_ONCE(rq->core != core_rq);
	}
}

స్థిర శూన్య sched_core_cpu_deactivate(గుర్తులేని పూర్ణాంకం cpu)
{
	స్థిరరాశి నిర్మాణం cpumask *smt_ముసుగు = cpu_smt_ముసుగు(cpu);
	నిర్మాణం rq *rq = cpu_rq(cpu), *core_rq = ఖాళీ;
	పూర్ణాంకం t;

	guard(core_తాళం)(&cpu);

	/* if we're the last man standing, nothing to do */
	ఒకవేళ (cpumask_బరువు(smt_ముసుగు) == 1) {
		WARN_ON_ONCE(rq->core != rq);
		తిరిగి;
	}

	/* if we're not the leader, nothing to do */
	ఒకవేళ (rq->core != rq)
		తిరిగి;

	/* find a new leader */
	for_each_cpu(t, smt_ముసుగు) {
		ఒకవేళ (t == cpu)
			కొనసాగు;
		core_rq = cpu_rq(t);
		ఆపు;
	}

	ఒకవేళ (WARN_ON_ONCE(!core_rq)) /* impossible */
		తిరిగి;

	/* copy the shared state to the new leader */
	core_rq->core_task_seq             = rq->core_task_seq;
	core_rq->core_pick_seq             = rq->core_pick_seq;
	core_rq->core_cookie               = rq->core_cookie;
	core_rq->core_forceidle_లెక్క      = rq->core_forceidle_లెక్క;
	core_rq->core_forceidle_seq        = rq->core_forceidle_seq;
	core_rq->core_forceidle_occupation = rq->core_forceidle_occupation;

	/*
	 * A stale leftover would bias the count forever if this CPU later
	 * returns as its own leader. Move, don't copy.
	 */
	core_rq->core_pick_లోపల_flight       = rq->core_pick_లోపల_flight;
	rq->core_pick_లోపల_flight            = 0;

	/*
	 * Accounting edge for forced idle is handled in pick_next_task().
	 * Don't need another one here, since the hotplug thread shouldn't
	 * have a cookie.
	 */
	core_rq->core_forceidle_ప్రారంభం = 0;

	/* install new leader */
	for_each_cpu(t, smt_ముసుగు) {
		rq = cpu_rq(t);
		rq->core = core_rq;
	}
}

స్థిర అంతఃవరుస శూన్య sched_core_cpu_dying(గుర్తులేని పూర్ణాంకం cpu)
{
	నిర్మాణం rq *rq = cpu_rq(cpu);

	ఒకవేళ (rq->core != rq)
		rq->core = rq;
}

#లేకపోతే /* !CONFIG_SCHED_CORE: */

స్థిర అంతఃవరుస శూన్య sched_core_cpu_starting(గుర్తులేని పూర్ణాంకం cpu) {}
స్థిర అంతఃవరుస శూన్య sched_core_cpu_deactivate(గుర్తులేని పూర్ణాంకం cpu) {}
స్థిర అంతఃవరుస శూన్య sched_core_cpu_dying(గుర్తులేని పూర్ణాంకం cpu) {}

స్థిర నిర్మాణం task_struct *
pick_తదుపరి_task(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	తిరిగి __pick_next_task(rq, rf);
}

#అయితే_ముగింపు /* !CONFIG_SCHED_CORE */

/*
 * Constants for the sched_mode argument of __schedule().
 *
 * The mode argument allows RT enabled kernels to differentiate a
 * preemption from blocking on an 'sleeping' spin/rwlock.
 */
#నిర్వచించు SM_IDLE			(-1)
#నిర్వచించు SM_NONE			0
#నిర్వచించు SM_PREEMPT		1
#నిర్వచించు SM_RTLOCK_WAIT		2

/*
 * Helper function for __schedule()
 *
 * Tries to deactivate the task, unless the should_block arg
 * is false or if a signal is pending. In the case a signal
 * is pending, marks the task's __state as RUNNING (and clear
 * blocked_on).
 */
స్థిర bool try_గమ్యం_ఖండం_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p,
			      గుర్తులేని దీర్ఘ *task_state_p, bool should_ఖండం)
{
	గుర్తులేని దీర్ఘ task_state = *task_state_p;

	WARN_ON_ONCE(p->is_blocked);

	ఒకవేళ (signal_pending_state(task_state, p)) {
		WRITE_ONCE(p->__state, TASK_RUNNING);
		*task_state_p = TASK_RUNNING;
		clear_task_blocked_on(p, ఖాళీ);

		తిరిగి false;
	}

	p->is_blocked = 1;

	/*
	 * We check should_block after signal_pending because we
	 * will want to wake the task in that case. But if
	 * should_block is false, its likely due to the task being
	 * blocked on a mutex, and we want to keep it on the runqueue
	 * to be selectable for proxy-execution.
	 */
	ఒకవేళ (!should_ఖండం)
		తిరిగి false;

	ఖండం_task(rq, p, task_state);
	తిరిగి true;
}

#నిర్వచితమైతే CONFIG_SCHED_PROXY_EXEC
స్థిర అంతఃవరుస శూన్య proxy_సమితి_task_cpu(నిర్మాణం task_struct *p, పూర్ణాంకం cpu)
{
	గుర్తులేని పూర్ణాంకం wake_cpu;

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

స్థిర అంతఃవరుస నిర్మాణం task_struct *proxy_resched_idle(నిర్మాణం rq *rq)
{
	put_prev_సమితి_తదుపరి_task(rq, rq->donor, rq->idle);
	rq->తదుపరి_class = &idle_sched_class;
	rq_సమితి_donor(rq, rq->idle);
	సమితి_tsk_need_resched(rq->idle);
	తిరిగి rq->idle;
}

స్థిర శూన్య proxy_deactivate(నిర్మాణం rq *rq, నిర్మాణం task_struct *donor)
{
	గుర్తులేని దీర్ఘ state = READ_ONCE(donor->__state);

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
	ఖండం_task(rq, donor, state);
}

/*
 * Remove a retained proxy donor before changing its scheduler ownership.
 * The caller holds p->pi_lock, so p cannot wake and migrate if block_task()
 * drops it from the runqueue. If DELAY_DEQUEUE keeps a blocked fair task
 * queued, switching_from_fair() completes the dequeue in the immediately
 * following sched_change_begin().
 */
శూన్య sched_proxy_ఖండం_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	గుర్తులేని దీర్ఘ state = READ_ONCE(p->__state);

	lockdep_assert_held(&p->pi_తాళం);
	lockdep_assert_rq_held(rq);

	ఒకవేళ (!p->is_blocked || !task_on_rq_queued(p))
		తిరిగి;
	ఒకవేళ (WARN_ON_ONCE(state == TASK_RUNNING))
		తిరిగి;

	ఒకవేళ (task_current_donor(rq, p))
		proxy_reset_donor(rq);

	ఒకవేళ (!p->se.sched_delayed)
		ఖండం_task(rq, p, state);

	WARN_ON_ONCE(task_on_rq_queued(p) && !p->se.sched_delayed);
}

స్థిర అంతఃవరుస శూన్య proxy_release_rq_తాళం(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
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
	rq_unpin_తాళం(rq, rf);
	raw_spin_rq_unlock(rq);
}

స్థిర అంతఃవరుస శూన్య proxy_reacquire_rq_తాళం(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_తాళం(rq);
	rq_repin_తాళం(rq, rf);
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
స్థిర శూన్య proxy_migrate_task(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf,
			       నిర్మాణం task_struct *p, పూర్ణాంకం లక్ష్యం_cpu)
	__must_hold(__rq_lockp(rq))
{
	నిర్మాణం rq *లక్ష్యం_rq = cpu_rq(లక్ష్యం_cpu);
	LIST_తల(migrate_list);

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
	కొరకు (; p; p = p->blocked_donor) {
		WARN_ON(p == rq->curr);
		deactivate_task(rq, p, వరుస_నుండి_తీయి_NOCLOCK);
		proxy_సమితి_task_cpu(p, లక్ష్యం_cpu);
		/*
		 * We can re-use se.group_node to migrate the thing,
		 * because @p is deactivated (won't be balanced) and
		 * we hold the rq_lock.
		 */
		list_కూడు(&p->se.సమూహం_కణుపు, &migrate_list);
	}
	proxy_release_rq_తాళం(rq, rf);

	__attach_tasks(లక్ష్యం_rq, &migrate_list);

	proxy_reacquire_rq_తాళం(rq, rf);
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
స్థిర నిర్మాణం task_struct *
కనుగొను_proxy_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *donor, నిర్మాణం rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	నిర్మాణం task_struct *owner = ఖాళీ;
	bool curr_లోపల_chain = false;
	పూర్ణాంకం this_cpu = cpu_యొక్క(rq);
	నిర్మాణం task_struct *p;
	పూర్ణాంకం owner_cpu;

	/* Follow blocked_on chain. */
	కొరకు (p = donor; p->is_blocked; p = owner) {
		/* if its PROXY_WAKING, do return migration or run if current */
		నిర్మాణం mutex *mutex = p->blocked_on;
		ఒకవేళ (!mutex) {
			clear_task_blocked_on(p, mutex);
			ఒకవేళ (task_current(rq, p)) {
				p->is_blocked = 0;
				తిరిగి p;
			}
			వెళ్ళు deactivate;
		}

		/*
		 * By taking mutex->wait_lock we hold off concurrent mutex_unlock()
		 * and ensure @owner sticks around.
		 */
		guard(raw_spinlock)(&mutex->wait_తాళం);
		guard(raw_spinlock)(&p->blocked_తాళం);

		/* Check again that p is blocked with blocked_lock held */
		ఒకవేళ (mutex != __get_task_blocked_on(p)) {
			/*
			 * Something changed in the blocked_on chain and
			 * we don't know if only at this level. So, let's
			 * just bail out completely and let __schedule()
			 * figure things out (pick_again loop).
			 */
			తిరిగి ఖాళీ;
		}

		ఒకవేళ (task_current(rq, p))
			curr_లోపల_chain = true;

		owner = __mutex_owner(mutex);
		ఒకవేళ (!owner) {
			/*
			 * If there is no owner, either clear blocked_on
			 * and return p (if it is current and safe to
			 * just run on this rq), or return-migrate the task.
			 */
			__clear_task_blocked_on(p, ఖాళీ);
			ఒకవేళ (task_current(rq, p)) {
				p->is_blocked = 0;
				తిరిగి p;
			}
			వెళ్ళు deactivate;
		}

		ఒకవేళ (!READ_ONCE(owner->on_rq) || owner->se.sched_delayed) {
			/* XXX Don't handle blocked owners/delayed dequeue yet */
			ఒకవేళ (curr_లోపల_chain)
				వెళ్ళు resched_idle;
			__clear_task_blocked_on(p, ఖాళీ);
			వెళ్ళు deactivate;
		}

		owner_cpu = task_cpu(owner);
		ఒకవేళ (owner_cpu != this_cpu) {
			/*
			 * @owner can disappear, simply migrate to @owner_cpu
			 * and leave that CPU to sort things out.
			 */
			ఒకవేళ (curr_లోపల_chain)
				వెళ్ళు resched_idle;
			వెళ్ళు migrate_task;
		}

		ఒకవేళ (task_on_rq_migrating(owner)) {
			/*
			 * One of the chain of mutex owners is currently migrating to this
			 * CPU, but has not yet been enqueued because we are holding the
			 * rq lock. As a simple solution, just schedule rq->idle to give
			 * the migration a chance to complete. Much like the migrate_task
			 * case we should end up back in find_proxy_task(), this time
			 * hopefully with all relevant tasks already enqueued.
			 */
			వెళ్ళు resched_idle;
		}

		/*
		 * Its possible to race where after we check owner->on_rq
		 * but before we check (owner_cpu != this_cpu) that the
		 * task on another cpu was migrated back to this cpu. In
		 * that case it could slip by our  checks. So double check
		 * we are still on this cpu and not migrating. If we get
		 * inconsistent results, try again.
		 */
		ఒకవేళ (!task_on_rq_queued(owner) || task_cpu(owner) != this_cpu)
			తిరిగి ఖాళీ;

		ఒకవేళ (owner == p) {
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
			వెళ్ళు resched_idle;
		}
		/*
		 * OK, now we're absolutely sure @owner is on this
		 * rq, therefore holding @rq->lock is sufficient to
		 * guarantee its existence, as per ttwu_remote().
		 */
		owner->blocked_donor = p;
	}
	WARN_ON_ONCE(owner && !owner->on_rq);

	ఒకవేళ (owner && !sched_cpu_cookie_సరిపోలిక(rq, owner)) {
		ఒకవేళ (curr_లోపల_chain)
			తిరిగి proxy_resched_idle(rq);
		p = donor; /* Deactivate the donor, not the runnable owner */
		clear_task_blocked_on(p, ఖాళీ);
		వెళ్ళు deactivate;
	}
	తిరిగి owner;

resched_idle:
	తిరిగి proxy_resched_idle(rq);
deactivate:
	proxy_deactivate(rq, p);
	తిరిగి ఖాళీ;
migrate_task:
	proxy_migrate_task(rq, rf, p, owner_cpu);
	తిరిగి ఖాళీ;
}
#లేకపోతే /* SCHED_PROXY_EXEC */
స్థిర నిర్మాణం task_struct *
కనుగొను_proxy_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *donor, నిర్మాణం rq_flags *rf)
{
	WARN_ONCE(1, "This should never be called in the !SCHED_PROXY_EXEC case\n");
	తిరిగి donor;
}
#అయితే_ముగింపు /* SCHED_PROXY_EXEC */

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
స్థిర శూన్య __sched notrace __schedule(పూర్ణాంకం sched_బహుళకం)
{
	నిర్మాణం task_struct *prev, *తదుపరి;
	/*
	 * On PREEMPT_RT kernel, SM_RTLOCK_WAIT is noted
	 * as a preemption by schedule_debug() and RCU.
	 */
	bool preempt = sched_బహుళకం > SM_NONE;
	bool is_switch = false;
	గుర్తులేని దీర్ఘ *switch_లెక్క;
	గుర్తులేని దీర్ఘ prev_state;
	నిర్మాణం rq_flags rf;
	నిర్మాణం rq *rq;
	పూర్ణాంకం cpu;

	/* Trace preemptions consistently with task switches */
	trace_sched_నమోదు_tp(sched_బహుళకం == SM_PREEMPT);

	cpu = smp_processor_id();
	rq = cpu_rq(cpu);
	prev = rq->curr;

	కాలపట్టిక_debug(prev, preempt);

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
	rq_తాళం(rq, &rf);
	smp_mb__after_spinlock();

	hrtick_కాలపట్టిక_enter(rq);

	/* Promote REQ to ACT */
	rq->clock_update_flags <<= 1;
	update_rq_clock(rq);
	rq->clock_update_flags = RQCF_UPDATED;

	switch_లెక్క = &prev->nivcsw;

	/* Task state changes only considers SM_PREEMPT as preemption */
	preempt = sched_బహుళకం == SM_PREEMPT;

	/*
	 * We must load prev->state once (task_struct::state is volatile), such
	 * that we form a control dependency vs deactivate_task() below.
	 */
	prev_state = READ_ONCE(prev->__state);
	ఒకవేళ (sched_బహుళకం == SM_IDLE) {
		/* SCX must consult the BPF scheduler to tell if rq is empty */
		ఒకవేళ (!rq->nr_running && !scx_enabled()) {
			తదుపరి = prev;
			rq->తదుపరి_class = &idle_sched_class;
			వెళ్ళు picked;
		}
	} లేకపోతే ఒకవేళ (!preempt && prev_state) {
		/*
		 * Keep mutex-blocked tasks on the runqueue for proxy execution
		 * only when their scheduling class allows it. Without proxy
		 * execution, task_is_blocked() always returns false.
		 */
		try_గమ్యం_ఖండం_task(rq, prev, &prev_state,
				  !task_is_blocked(prev) || !scx_allow_proxy_exec(prev));
		switch_లెక్క = &prev->nvcsw;
	}

pick_again:
	assert_balance_callbacks_ఖాళీలు(rq);
	తదుపరి = pick_తదుపరి_task(rq, &rf);
	rq->తదుపరి_class = తదుపరి->sched_class;
	ఒకవేళ (sched_proxy_exec()) {
		నిర్మాణం task_struct *prev_donor = rq->donor;

		rq_సమితి_donor(rq, తదుపరి);
		తదుపరి->blocked_donor = ఖాళీ;
		ఒకవేళ (unlikely(తదుపరి->is_blocked)) {
			తదుపరి = కనుగొను_proxy_task(rq, తదుపరి, &rf);
			ఒకవేళ (!తదుపరి) {
				zap_balance_callbacks(rq);
				వెళ్ళు pick_again;
			}
			ఒకవేళ (తదుపరి == rq->idle) {
				zap_balance_callbacks(rq);
				scx_proxy_reenqueue_retry(rq, తదుపరి);
				వెళ్ళు keep_resched;
			}
		}
		ఒకవేళ (rq->donor == prev_donor && prev != తదుపరి) {
			నిర్మాణం task_struct *donor = rq->donor;
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
			donor->sched_class->సమితి_తదుపరి_task(rq, donor, SNT_PICK);
		}
		scx_proxy_donor_ప్రారంభం(rq);
		scx_proxy_reenqueue_retry(rq, తదుపరి);
	} లేకపోతే {
		rq_సమితి_donor(rq, తదుపరి);
	}

picked:
	clear_tsk_need_resched(prev);
	clear_preempt_need_resched();
keep_resched:
	rq->చివరి_చూసిన_need_resched_ns = 0;

	is_switch = prev != తదుపరి;
	ఒకవేళ (likely(is_switch)) {
		rq->nr_switches++;
		/*
		 * RCU users of rcu_dereference(rq->curr) may not see
		 * changes to task_struct made by pick_next_task().
		 */
		RCU_INIT_సూచిక(rq->curr, తదుపరి);

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
		++*switch_లెక్క;

		psi_account_irqtime(rq, prev, తదుపరి);
		psi_sched_switch(prev, తదుపరి, !task_on_rq_queued(prev) ||
					     prev->se.sched_delayed);

		trace_sched_switch(preempt, prev, తదుపరి, prev_state);

		/* Also unlocks the rq: */
		rq = context_switch(rq, prev, తదుపరి, &rf);
	} లేకపోతే {
		rq_unpin_తాళం(rq, &rf);
		__balance_callbacks(rq, ఖాళీ);
		hrtick_కాలపట్టిక_exit(rq);
		raw_spin_rq_unlock_irq(rq);
	}
	trace_sched_exit_tp(is_switch);
}

శూన్య __noreturn do_task_dead(శూన్య)
{
	/* Causes final put_task_struct in finish_task_switch(): */
	సమితి_special_state(TASK_DEAD);

	/* Tell freezer to ignore us: */
	current->flags |= PF_NOFREEZE;

	__schedule(SM_NONE);
	BUG();

	/* Avoid "noreturn function does return" - but don't continue if BUG() is a NOP: */
	కొరకు (;;)
		cpu_relax();
}

స్థిర అంతఃవరుస శూన్య sched_submit_work(నిర్మాణం task_struct *tsk)
{
	స్థిర DEFINE_WAIT_OVERRIDE_MAP(sched_map, LD_WAIT_CONFIG);
	గుర్తులేని పూర్ణాంకం task_flags;

	/*
	 * Establish LD_WAIT_CONFIG context to ensure none of the code called
	 * will use a blocking primitive -- which would lead to recursion.
	 */
	తాళం_map_acquire_try(&sched_map);

	task_flags = tsk->flags;
	/*
	 * If a worker goes to sleep, notify and ask workqueue whether it
	 * wants to wake up a task to maintain concurrency.
	 */
	ఒకవేళ (task_flags & PF_WQ_కార్మికుడు)
		wq_కార్మికుడు_sleeping(tsk);
	లేకపోతే ఒకవేళ (task_flags & PF_IO_కార్మికుడు)
		io_wq_కార్మికుడు_sleeping(tsk);

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

	తాళం_map_release(&sched_map);
}

స్థిర శూన్య sched_update_కార్మికుడు(నిర్మాణం task_struct *tsk)
{
	ఒకవేళ (tsk->flags & (PF_WQ_కార్మికుడు | PF_IO_కార్మికుడు)) {
		ఒకవేళ (tsk->flags & PF_WQ_కార్మికుడు)
			wq_కార్మికుడు_running(tsk);
		లేకపోతే
			io_wq_కార్మికుడు_running(tsk);
	}
}

స్థిర __always_inline శూన్య __schedule_loop(పూర్ణాంకం sched_బహుళకం)
{
	చేయి {
		preempt_disable();
		__schedule(sched_బహుళకం);
		sched_preempt_enable_no_resched();
	} అయితే (need_resched());
}

asmlinkage __visible శూన్య __sched కాలపట్టిక(శూన్య)
{
	నిర్మాణం task_struct *tsk = current;

#నిర్వచితమైతే CONFIG_RT_MUTEXES
	lockdep_assert(!tsk->sched_rt_mutex);
#అయితే_ముగింపు

	ఒకవేళ (!task_is_running(tsk))
		sched_submit_work(tsk);
	__schedule_loop(SM_NONE);
	sched_update_కార్మికుడు(tsk);
}
EXPORT_చిహ్నం(కాలపట్టిక);

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
శూన్య __sched కాలపట్టిక_idle(శూన్య)
{
	/*
	 * As this skips calling sched_submit_work(), which the idle task does
	 * regardless because that function is a NOP when the task is in a
	 * TASK_RUNNING state, make sure this isn't used someplace that the
	 * current task can be in any other state. Note, idle is always in the
	 * TASK_RUNNING state.
	 */
	WARN_ON_ONCE(current->__state);
	చేయి {
		__schedule(SM_IDLE);
	} అయితే (need_resched());
}

#ఒకవేళ defined(CONFIG_CONTEXT_TRACKING_USER) && !defined(CONFIG_HAVE_CONTEXT_TRACKING_USER_OFFSTACK)
asmlinkage __visible శూన్య __sched కాలపట్టిక_user(శూన్య)
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
	గణన ctx_state prev_state = exception_enter();
	కాలపట్టిక();
	exception_exit(prev_state);
}
#అయితే_ముగింపు

/**
 * schedule_preempt_disabled - called with preemption disabled
 *
 * Returns with preemption disabled. Note: preempt_count must be 1
 */
శూన్య __sched కాలపట్టిక_preempt_disabled(శూన్య)
{
	sched_preempt_enable_no_resched();
	కాలపట్టిక();
	preempt_disable();
}

#నిర్వచితమైతే CONFIG_PREEMPT_RT
శూన్య __sched notrace కాలపట్టిక_rtlock(శూన్య)
{
	__schedule_loop(SM_RTLOCK_WAIT);
}
NOKPROBE_చిహ్నం(కాలపట్టిక_rtlock);
#అయితే_ముగింపు

స్థిర శూన్య __sched notrace preempt_కాలపట్టిక_common(శూన్య)
{
	చేయి {
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
		preempt_latency_ప్రారంభం(1);
		__schedule(SM_PREEMPT);
		preempt_latency_stop(1);
		preempt_enable_no_resched_notrace();

		/*
		 * Check again in case we missed a preemption opportunity
		 * between schedule and now.
		 */
	} అయితే (need_resched());
}

#నిర్వచితమైతే CONFIG_PREEMPTION
/*
 * This is the entry point to schedule() from in-kernel preemption
 * off of preempt_enable.
 */
asmlinkage __visible శూన్య __sched notrace preempt_కాలపట్టిక(శూన్య)
{
	/*
	 * If there is a non-zero preempt_count or interrupts are disabled,
	 * we do not want to preempt the current task. Just return..
	 */
	ఒకవేళ (likely(!preemptible()))
		తిరిగి;
	preempt_కాలపట్టిక_common();
}
NOKPROBE_చిహ్నం(preempt_కాలపట్టిక);
EXPORT_చిహ్నం(preempt_కాలపట్టిక);

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
asmlinkage __visible శూన్య __sched notrace preempt_కాలపట్టిక_notrace(శూన్య)
{
	గణన ctx_state prev_ctx;

	ఒకవేళ (likely(!preemptible()))
		తిరిగి;

	చేయి {
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
		preempt_latency_ప్రారంభం(1);
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
	} అయితే (need_resched());
}
EXPORT_చిహ్నం_GPL(preempt_కాలపట్టిక_notrace);

#అయితే_ముగింపు /* CONFIG_PREEMPTION */

/*
 * This is the entry point to schedule() from kernel preemption
 * off of IRQ context.
 * Note, that this is called and return with IRQs disabled. This will
 * protect us against recursive calling from IRQ contexts.
 */
asmlinkage __visible శూన్య __sched preempt_కాలపట్టిక_irq(శూన్య)
{
	గణన ctx_state prev_state;

	/* Catch callers which need to be fixed */
	BUG_ON(preempt_లెక్క() || !irqs_disabled());

	prev_state = exception_enter();

	చేయి {
		preempt_disable();
		local_irq_enable();
		__schedule(SM_PREEMPT);
		local_irq_disable();
		sched_preempt_enable_no_resched();
	} అయితే (need_resched());

	exception_exit(prev_state);
}

పూర్ణాంకం default_wake_function(wait_వరుస_నమోదు_t *curr, గుర్తులేని బహుళకం, పూర్ణాంకం wake_flags,
			  శూన్య *కీలకం)
{
	WARN_ON_ONCE(wake_flags & ~(WF_SYNC|WF_CURRENT_CPU));
	తిరిగి try_గమ్యం_wake_పైకి(curr->అంతరంగ, బహుళకం, wake_flags);
}
EXPORT_చిహ్నం(default_wake_function);

స్థిరరాశి నిర్మాణం sched_class *__setscheduler_class(పూర్ణాంకం policy, పూర్ణాంకం prio)
{
	ఒకవేళ (dl_prio(prio))
		తిరిగి &dl_sched_class;

	ఒకవేళ (rt_prio(prio))
		తిరిగి &rt_sched_class;

#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
	ఒకవేళ (task_should_scx(policy))
		తిరిగి &ext_sched_class;
#అయితే_ముగింపు

	తిరిగి &fair_sched_class;
}

#నిర్వచితమైతే CONFIG_RT_MUTEXES

/*
 * Would be more useful with typeof()/auto_type but they don't mix with
 * bit-fields. Since it's a local thing, use int. Keep the generic sounding
 * name such that if someone were to implement this function we get to compare
 * notes.
 */
#నిర్వచించు fetch_and_సమితి(x, v) ({ పూర్ణాంకం _x = (x); (x) = (v); _x; })

శూన్య rt_mutex_pre_కాలపట్టిక(శూన్య)
{
	lockdep_assert(!fetch_and_సమితి(current->sched_rt_mutex, 1));
	sched_submit_work(current);
}

/*
 * Used within the futex syscall context, skips sched_submit_work() because none
 * its work will be done. Asserts ensure that it is indeed the case.
 */
శూన్య rt_mutex_futex_pre_కాలపట్టిక(శూన్య)
{
	lockdep_assert(!(current->flags & (PF_WQ_కార్మికుడు | PF_IO_కార్మికుడు)));
	lockdep_assert(!current->plug);
	lockdep_assert(!fetch_and_సమితి(current->sched_rt_mutex, 1));
}

శూన్య rt_mutex_కాలపట్టిక(శూన్య)
{
	lockdep_assert(current->sched_rt_mutex);
	__schedule_loop(SM_NONE);
}

శూన్య rt_mutex_post_కాలపట్టిక(శూన్య)
{
	sched_update_కార్మికుడు(current);
	lockdep_assert(fetch_and_సమితి(current->sched_rt_mutex, 0));
}

శూన్య rt_mutex_futex_post_కాలపట్టిక(శూన్య)
{
	lockdep_assert(fetch_and_సమితి(current->sched_rt_mutex, 0));
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
శూన్య rt_mutex_setprio(నిర్మాణం task_struct *p, నిర్మాణం task_struct *pi_task)
{
	పూర్ణాంకం prio, oldprio, వరుస_flag =
		వరుస_నుండి_తీయి_SAVE | వరుస_నుండి_తీయి_MOVE | వరుస_నుండి_తీయి_NOCLOCK;
	స్థిరరాశి నిర్మాణం sched_class *prev_class, *తదుపరి_class;
	నిర్మాణం rq_flags rf;
	నిర్మాణం rq *rq;

	/* XXX used to be waiter->prio, not waiter->task->prio */
	prio = __rt_effective_prio(pi_task, p->సాధారణ_prio);

	/*
	 * If nothing changed; bail early.
	 */
	ఒకవేళ (p->pi_శిఖరం_task == pi_task && prio == p->prio && !dl_prio(prio))
		తిరిగి;

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
	p->pi_శిఖరం_task = pi_task;

	/*
	 * For FIFO/RR we only need to set prio, if that matches we're done.
	 */
	ఒకవేళ (prio == p->prio && !dl_prio(prio))
		వెళ్ళు బహిర్గతం_unlock;

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
	ఒకవేళ (unlikely(p == rq->idle)) {
		WARN_ON(p != rq->curr);
		WARN_ON(p->pi_blocked_on);
		వెళ్ళు బహిర్గతం_unlock;
	}

	trace_sched_pi_setprio(p, pi_task);
	oldprio = p->prio;

	ఒకవేళ (oldprio == prio && !dl_prio(prio))
		వరుస_flag &= ~వరుస_నుండి_తీయి_MOVE;

	prev_class = p->sched_class;
	తదుపరి_class = __setscheduler_class(p->policy, prio);

	ఒకవేళ (prev_class != తదుపరి_class)
		వరుస_flag |= వరుస_నుండి_తీయి_CLASS;

	scoped_guard (sched_change, p, వరుస_flag) {
		/*
		 * Boosting condition are:
		 * 1. -rt task is running and holds mutex A
		 *      --> -dl task blocks on mutex A
		 *
		 * 2. -dl task is running and holds mutex A
		 *      --> -dl task blocks on mutex A and could preempt the
		 *          running task
		 */
		ఒకవేళ (dl_prio(prio)) {
			ఒకవేళ (!dl_prio(p->సాధారణ_prio) ||
			    (pi_task && dl_prio(pi_task->prio) &&
			     dl_entity_preempt(&pi_task->dl, &p->dl))) {
				p->dl.pi_se = pi_task->dl.pi_se;
				scope->flags |= వరుసలో_చేర్చు_REPLENISH;
			} లేకపోతే {
				p->dl.pi_se = &p->dl;
			}
		} లేకపోతే ఒకవేళ (rt_prio(prio)) {
			ఒకవేళ (dl_prio(oldprio))
				p->dl.pi_se = &p->dl;
			ఒకవేళ (oldprio < prio)
				scope->flags |= వరుసలో_చేర్చు_తల;
		} లేకపోతే {
			ఒకవేళ (dl_prio(oldprio))
				p->dl.pi_se = &p->dl;
			ఒకవేళ (rt_prio(oldprio))
				p->rt.timeout = 0;
		}

		p->sched_class = తదుపరి_class;
		p->prio = prio;
	}
బహిర్గతం_unlock:
	/* Caller holds task_struct::pi_lock, IRQs are still disabled */

	__balance_callbacks(rq, &rf);
	__task_rq_unlock(rq, p, &rf);
}
#అయితే_ముగింపు /* CONFIG_RT_MUTEXES */

#ఒకవేళ !defined(CONFIG_PREEMPTION)
పూర్ణాంకం __sched __cond_resched(శూన్య)
{
	ఒకవేళ (should_resched(0) && !irqs_disabled()) {
		preempt_కాలపట్టిక_common();
		తిరిగి 1;
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
#నిర్వచితం_కాకపోతే CONFIG_PREEMPT_RCU
	rcu_all_qs();
#అయితే_ముగింపు
	తిరిగి 0;
}
EXPORT_చిహ్నం(__cond_resched);
#అయితే_ముగింపు

/*
 * __cond_resched_lock() - if a reschedule is pending, drop the given lock,
 * call schedule, and on return reacquire the lock.
 *
 * This works OK both with and without CONFIG_PREEMPTION. We do strange low-level
 * operations here to prevent schedule() from being called twice (once via
 * spin_unlock(), once by hand).
 */
పూర్ణాంకం __cond_resched_lock(spinlock_t *తాళం)
{
	పూర్ణాంకం resched = should_resched(PREEMPT_తాళం_స్థానభ్రంశం);
	పూర్ణాంకం ret = 0;

	lockdep_assert_held(తాళం);

	ఒకవేళ (spin_needbreak(తాళం) || resched) {
		spin_unlock(తాళం);
		ఒకవేళ (!_cond_resched())
			cpu_relax();
		ret = 1;
		spin_తాళం(తాళం);
	}
	తిరిగి ret;
}
EXPORT_చిహ్నం(__cond_resched_lock);

పూర్ణాంకం __cond_resched_rwlock_read(rwlock_t *తాళం)
{
	పూర్ణాంకం resched = should_resched(PREEMPT_తాళం_స్థానభ్రంశం);
	పూర్ణాంకం ret = 0;

	lockdep_assert_held_read(తాళం);

	ఒకవేళ (rwlock_needbreak(తాళం) || resched) {
		read_unlock(తాళం);
		ఒకవేళ (!_cond_resched())
			cpu_relax();
		ret = 1;
		read_తాళం(తాళం);
	}
	తిరిగి ret;
}
EXPORT_చిహ్నం(__cond_resched_rwlock_read);

పూర్ణాంకం __cond_resched_rwlock_write(rwlock_t *తాళం)
{
	పూర్ణాంకం resched = should_resched(PREEMPT_తాళం_స్థానభ్రంశం);
	పూర్ణాంకం ret = 0;

	lockdep_assert_held_write(తాళం);

	ఒకవేళ (rwlock_needbreak(తాళం) || resched) {
		write_unlock(తాళం);
		ఒకవేళ (!_cond_resched())
			cpu_relax();
		ret = 1;
		write_తాళం(తాళం);
	}
	తిరిగి ret;
}
EXPORT_చిహ్నం(__cond_resched_rwlock_write);

#నిర్వచితమైతే CONFIG_PREEMPT_DYNAMIC

# ifdef CONFIG_GENERIC_IRQ_నమోదు
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

గణన {
	preempt_dynamic_undefined = -1,
	preempt_dynamic_నిండిన,
	preempt_dynamic_lazy,
};

పూర్ణాంకం preempt_dynamic_బహుళకం = preempt_dynamic_undefined;

పూర్ణాంకం sched_dynamic_బహుళకం(స్థిరరాశి అక్షరం *str)
{
	ఒకవేళ (!strcmp(str, "full"))
		తిరిగి preempt_dynamic_నిండిన;

	ఒకవేళ (!strcmp(str, "lazy"))
		తిరిగి preempt_dynamic_lazy;

	తిరిగి -EINVAL;
}

# define preempt_dynamic_కీలకం_enable(f)	static_కీలకం_enable(&sk_dynamic_##f.కీలకం)
# define preempt_dynamic_కీలకం_disable(f)	static_కీలకం_disable(&sk_dynamic_##f.కీలకం)

స్థిర DEFINE_MUTEX(sched_dynamic_mutex);

స్థిర శూన్య __sched_dynamic_update(పూర్ణాంకం బహుళకం)
{
	ఎంపిక (బహుళకం) {
	స్థితి preempt_dynamic_నిండిన:
		preempt_dynamic_కీలకం_disable(preempt_lazy);
		ఒకవేళ (బహుళకం != preempt_dynamic_బహుళకం)
			pr_info("Dynamic Preempt: full\n");
		ఆపు;

	స్థితి preempt_dynamic_lazy:
		preempt_dynamic_కీలకం_enable(preempt_lazy);
		ఒకవేళ (బహుళకం != preempt_dynamic_బహుళకం)
			pr_info("Dynamic Preempt: lazy\n");
		ఆపు;
	}

	WRITE_ONCE(preempt_dynamic_బహుళకం, బహుళకం);
}

శూన్య sched_dynamic_update(పూర్ణాంకం బహుళకం)
{
	mutex_తాళం(&sched_dynamic_mutex);
	__sched_dynamic_update(బహుళకం);
	mutex_unlock(&sched_dynamic_mutex);
}

స్థిర పూర్ణాంకం __init అమరిక_preempt_బహుళకం(అక్షరం *str)
{
	పూర్ణాంకం బహుళకం = sched_dynamic_బహుళకం(str);
	ఒకవేళ (బహుళకం < 0) {
		pr_warn("Dynamic Preempt: unsupported mode: %s\n", str);
		తిరిగి 0;
	}

	sched_dynamic_update(బహుళకం);
	తిరిగి 1;
}
__setup("preempt=", అమరిక_preempt_బహుళకం);

స్థిర శూన్య __init preempt_dynamic_init(శూన్య)
{
	ఒకవేళ (preempt_dynamic_బహుళకం == preempt_dynamic_undefined) {
		ఒకవేళ (IS_ENABLED(CONFIG_PREEMPT_LAZY)) {
			sched_dynamic_update(preempt_dynamic_lazy);
		} లేకపోతే {
			/* Default static call setting, nothing to do */
			WARN_ON_ONCE(!IS_ENABLED(CONFIG_PREEMPT));
			preempt_dynamic_బహుళకం = preempt_dynamic_నిండిన;
			pr_info("Dynamic Preempt: full\n");
		}
	}
}

# define PREEMPT_MODEL_ACCESSOR(బహుళకం)					\
	bool preempt_model_##mode(శూన్య)					\
	{								\
		పూర్ణాంకం బహుళకం = READ_ONCE(preempt_dynamic_బహుళకం);		\
		WARN_ON_ONCE(బహుళకం == preempt_dynamic_undefined);	\
		తిరిగి బహుళకం == preempt_dynamic_##mode;			\
	}								\
	EXPORT_చిహ్నం_GPL(preempt_model_##mode)

PREEMPT_MODEL_ACCESSOR(నిండిన);
PREEMPT_MODEL_ACCESSOR(lazy);

#లేకపోతే /* !CONFIG_PREEMPT_DYNAMIC: */

#నిర్వచించు preempt_dynamic_బహుళకం -1

స్థిర అంతఃవరుస శూన్య preempt_dynamic_init(శూన్య) { }

#అయితే_ముగింపు /* CONFIG_PREEMPT_DYNAMIC */

స్థిరరాశి అక్షరం *preempt_modes[] = {
	"full", "lazy", ఖాళీ,
};

స్థిరరాశి అక్షరం *preempt_model_str(శూన్య)
{
	bool brace = IS_ENABLED(CONFIG_PREEMPT_RT) &&
		(IS_ENABLED(CONFIG_PREEMPT_DYNAMIC) ||
		 IS_ENABLED(CONFIG_PREEMPT_LAZY));
	స్థిర అక్షరం buf[128];

	ఒకవేళ (IS_ENABLED(CONFIG_PREEMPT_BUILD)) {
		నిర్మాణం seq_buf s;

		seq_buf_init(&s, buf, పరిమాణం(buf));
		seq_buf_puts(&s, "PREEMPT");

		ఒకవేళ (IS_ENABLED(CONFIG_PREEMPT_RT))
			seq_buf_printf(&s, "%sRT%s",
				       brace ? "_{" : "_",
				       brace ? "," : "");

		ఒకవేళ (IS_ENABLED(CONFIG_PREEMPT_DYNAMIC)) {
			seq_buf_printf(&s, "(%s)%s",
				       preempt_dynamic_బహుళకం >= 0 ?
				       preempt_modes[preempt_dynamic_బహుళకం] : "undef",
				       brace ? "}" : "");
			తిరిగి seq_buf_str(&s);
		}

		ఒకవేళ (IS_ENABLED(CONFIG_PREEMPT_LAZY)) {
			seq_buf_printf(&s, "LAZY%s",
				       brace ? "}" : "");
			తిరిగి seq_buf_str(&s);
		}

		తిరిగి seq_buf_str(&s);
	}

	ఒకవేళ (IS_ENABLED(CONFIG_PREEMPT_VOLUNTARY_BUILD))
		తిరిగి "VOLUNTARY";

	తిరిగి "NONE";
}

పూర్ణాంకం io_కాలపట్టిక_prepare(శూన్య)
{
	పూర్ణాంకం old_iowait = current->లోపల_iowait;

	current->లోపల_iowait = 1;
	blk_flush_plug(current->plug, true);
	తిరిగి old_iowait;
}

శూన్య io_కాలపట్టిక_ముగింపు(పూర్ణాంకం token)
{
	current->లోపల_iowait = token;
}

/*
 * This task is about to go to sleep on IO. Increment rq->nr_iowait so
 * that process accounting knows that this is a task in IO wait state.
 */
దీర్ఘ __sched io_కాలపట్టిక_timeout(దీర్ఘ timeout)
{
	పూర్ణాంకం token;
	దీర్ఘ ret;

	token = io_కాలపట్టిక_prepare();
	ret = కాలపట్టిక_timeout(timeout);
	io_కాలపట్టిక_ముగింపు(token);

	తిరిగి ret;
}
EXPORT_చిహ్నం(io_కాలపట్టిక_timeout);

శూన్య __sched io_కాలపట్టిక(శూన్య)
{
	పూర్ణాంకం token;

	token = io_కాలపట్టిక_prepare();
	కాలపట్టిక();
	io_కాలపట్టిక_ముగింపు(token);
}
EXPORT_చిహ్నం(io_కాలపట్టిక);

శూన్య sched_show_task(నిర్మాణం task_struct *p)
{
	గుర్తులేని దీర్ఘ విడుదల;
	పూర్ణాంకం ppid;

	ఒకవేళ (!try_get_task_దొంతర(p))
		తిరిగి;

	pr_info("task:%-15.15s state:%c", p->comm, task_state_గమ్యం_char(p));

	ఒకవేళ (task_is_running(p))
		pr_cont("  running task    ");
	విడుదల = దొంతర_not_వాడిన(p);
	ppid = 0;
	rcu_read_తాళం();
	ఒకవేళ (pid_alive(p))
		ppid = task_pid_nr(rcu_dereference(p->real_మాతృ));
	rcu_read_unlock();
	pr_cont(" stack:%-5lu pid:%-5d tgid:%-5d ppid:%-6d task_flags:0x%04x flags:0x%08lx\n",
		విడుదల, task_pid_nr(p), task_tgid_nr(p),
		ppid, p->flags, read_task_thread_flags(p));

	print_కార్మికుడు_info(KERN_INFO, p);
	print_stop_info(KERN_INFO, p);
	print_scx_info(KERN_INFO, p);
	show_దొంతర(p, ఖాళీ, KERN_INFO);
	put_task_దొంతర(p);
}
EXPORT_చిహ్నం_GPL(sched_show_task);

స్థిర అంతఃవరుస bool
state_filter_సరిపోలిక(గుర్తులేని దీర్ఘ state_filter, నిర్మాణం task_struct *p)
{
	గుర్తులేని పూర్ణాంకం state = READ_ONCE(p->__state);

	/* no filter, everything matches */
	ఒకవేళ (!state_filter)
		తిరిగి true;

	/* filter, but doesn't match */
	ఒకవేళ (!(state & state_filter))
		తిరిగి false;

	/*
	 * When looking for TASK_UNINTERRUPTIBLE skip TASK_IDLE (allows
	 * TASK_KILLABLE).
	 */
	ఒకవేళ (state_filter == TASK_UNINTERRUPTIBLE && (state & TASK_NOLOAD))
		తిరిగి false;

	తిరిగి true;
}


శూన్య show_state_filter(గుర్తులేని పూర్ణాంకం state_filter)
{
	నిర్మాణం task_struct *g, *p;

	rcu_read_తాళం();
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
		ఒకవేళ (state_filter_సరిపోలిక(state_filter, p))
			sched_show_task(p);
	}

	ఒకవేళ (!state_filter)
		sysrq_sched_debug_show();

	rcu_read_unlock();
	/*
	 * Only show locks if all tasks are dumped:
	 */
	ఒకవేళ (!state_filter)
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
శూన్య __init init_idle(నిర్మాణం task_struct *idle, పూర్ణాంకం cpu)
{
	నిర్మాణం affinity_context ac = (నిర్మాణం affinity_context) {
		.new_ముసుగు  = cpumask_యొక్క(cpu),
		.flags     = 0,
	};
	నిర్మాణం rq *rq = cpu_rq(cpu);
	గుర్తులేని దీర్ఘ flags;

	raw_spin_తాళం_irqsave(&idle->pi_తాళం, flags);
	raw_spin_rq_తాళం(rq);

	idle->__state = TASK_RUNNING;
	idle->se.exec_ప్రారంభం = sched_clock();
	/*
	 * PF_KTHREAD should already be set at this point; regardless, make it
	 * look like a proper per-CPU kthread.
	 */
	idle->flags |= PF_KTHREAD | PF_NO_SETAFFINITY;
	kthread_సమితి_చొప్పున_cpu(idle, cpu);

	/*
	 * No validation and serialization required at boot time and for
	 * setting up the idle tasks of not yet online CPUs.
	 */
	సమితి_cpus_allowed_common(idle, &ac);
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
	rcu_read_తాళం();
	__set_task_cpu(idle, cpu);
	rcu_read_unlock();

	rq->idle = idle;
	rq_సమితి_donor(rq, idle);
	rcu_కేటాయించు_సూచిక(rq->curr, idle);
	idle->on_rq = TASK_ON_RQ_QUEUED;
	idle->on_cpu = 1;
	raw_spin_rq_unlock(rq);
	raw_spin_unlock_irqrestore(&idle->pi_తాళం, flags);

	/* Set the preempt count _outside_ the spinlocks! */
	init_idle_preempt_లెక్క(idle, cpu);

	/*
	 * The idle tasks have their own, simple scheduling class:
	 */
	idle->sched_class = &idle_sched_class;
	ftrace_లేఖాచిత్రం_init_idle_task(idle, cpu);
	vtime_init_idle(idle, cpu);
	sprintf(idle->comm, "%s/%d", INIT_TASK_COMM, cpu);
}

పూర్ణాంకం cpuset_cpumask_can_shrink(స్థిరరాశి నిర్మాణం cpumask *cur,
			      స్థిరరాశి నిర్మాణం cpumask *trial)
{
	పూర్ణాంకం ret = 1;

	ఒకవేళ (cpumask_ఖాళీలు(cur))
		తిరిగి ret;

	ret = dl_cpuset_cpumask_can_shrink(cur, trial);

	తిరిగి ret;
}

పూర్ణాంకం task_can_attach(నిర్మాణం task_struct *p)
{
	పూర్ణాంకం ret = 0;

	/*
	 * Kthreads which disallow setaffinity shouldn't be moved
	 * to a new cpuset; we don't want to change their CPU
	 * affinity and isolating such threads by their set of
	 * allowed nodes is unnecessary.  Thus, cpusets are not
	 * applicable for such threads.  This prevents checking for
	 * success of set_cpus_allowed_ptr() on all attached tasks
	 * before cpus_mask may be changed.
	 */
	ఒకవేళ (p->flags & PF_NO_SETAFFINITY)
		ret = -EINVAL;

	తిరిగి ret;
}

bool sched_smp_initialized __read_mostly;

#నిర్వచితమైతే CONFIG_NUMA_BALANCING
/* Migrate current task p to target_cpu */
పూర్ణాంకం migrate_task_గమ్యం(నిర్మాణం task_struct *p, పూర్ణాంకం లక్ష్యం_cpu)
{
	నిర్మాణం migration_arg arg = { p, లక్ష్యం_cpu };
	పూర్ణాంకం curr_cpu = task_cpu(p);

	ఒకవేళ (curr_cpu == లక్ష్యం_cpu)
		తిరిగి 0;

	ఒకవేళ (!cpumask_test_cpu(లక్ష్యం_cpu, p->cpus_ptr))
		తిరిగి -EINVAL;

	/* TODO: This is not properly updating schedstats */

	trace_sched_move_numa(p, curr_cpu, లక్ష్యం_cpu);
	తిరిగి stop_one_cpu(curr_cpu, migration_cpu_stop, &arg);
}

/*
 * Requeue a task on a given node and accurately track the number of NUMA
 * tasks on the runqueues
 */
శూన్య sched_setnuma(నిర్మాణం task_struct *p, పూర్ణాంకం nid)
{
	guard(task_rq_తాళం_2)(p);
	scoped_guard (sched_change, p, వరుస_నుండి_తీయి_SAVE)
		p->numa_preferred_nid = nid;
}
#అయితే_ముగింపు /* CONFIG_NUMA_BALANCING */

#నిర్వచితమైతే CONFIG_HOTPLUG_CPU
/*
 * Invoked on the outgoing CPU in context of the CPU hotplug thread
 * after ensuring that there are no user space tasks left on the CPU.
 *
 * If there is a lazy mm in use on the hotplug thread, drop it and
 * switch to init_mm.
 *
 * The reference count on init_mm is dropped in finish_cpu().
 */
స్థిర శూన్య sched_force_init_mm(శూన్య)
{
	నిర్మాణం mm_struct *mm = current->చురుకైన_mm;

	ఒకవేళ (mm != &init_mm) {
		mmgrab_lazy_tlb(&init_mm);
		local_irq_disable();
		current->చురుకైన_mm = &init_mm;
		switch_mm_irqs_off(mm, &init_mm, current);
		local_irq_enable();
		ముగింపు_arch_post_తాళం_switch();
		mmdrop_lazy_tlb(mm);
	}

	/* finish_cpu(), as ran on the BP, will clean up the active_mm state */
}

స్థిర పూర్ణాంకం __balance_push_cpu_stop(శూన్య *arg)
{
	నిర్మాణం task_struct *p = arg;
	నిర్మాణం rq *rq = this_rq();
	నిర్మాణం rq_flags rf;
	పూర్ణాంకం cpu;

	scoped_guard (raw_spinlock_irq, &p->pi_తాళం) {
		/*
		 * We may change the underlying rq, but the locks held will
		 * appropriately be "transferred" when switching.
		 */
		context_unsafe_alias(rq);

		cpu = select_fallback_rq(rq->cpu, p);

		rq_తాళం(rq, &rf);
		update_rq_clock(rq);
		ఒకవేళ (task_rq(p) == rq && task_on_rq_queued(p))
			rq = __migrate_task(rq, &rf, p, cpu);
		rq_unlock(rq, &rf);
	}

	put_task_struct(p);

	తిరిగి 0;
}

స్థిర DEFINE_చొప్పున_CPU(నిర్మాణం cpu_stop_work, నెట్టు_work);

/*
 * Ensure we only run per-cpu kthreads once the CPU goes !active.
 *
 * This is enabled below SCHED_AP_ACTIVE; when !cpu_active(), but only
 * effective when the hotplug motion is down.
 */
స్థిర శూన్య balance_నెట్టు(నిర్మాణం rq *rq)
	__must_hold(__rq_lockp(rq))
{
	నిర్మాణం task_struct *నెట్టు_task = rq->curr;

	lockdep_assert_rq_held(rq);

	/*
	 * Ensure the thing is persistent until balance_push_set(.on = false);
	 */
	rq->balance_callback = &balance_నెట్టు_callback;

	/*
	 * Only active while going offline and when invoked on the outgoing
	 * CPU.
	 */
	ఒకవేళ (!cpu_dying(rq->cpu) || rq != this_rq())
		తిరిగి;

	/*
	 * Both the cpu-hotplug and stop task are in this case and are
	 * required to complete the hotplug process.
	 */
	ఒకవేళ (kthread_is_చొప్పున_cpu(నెట్టు_task) ||
	    is_migration_disabled(నెట్టు_task)) {

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
		ఒకవేళ (!rq->nr_running && !rq_has_pinned_tasks(rq) &&
		    rcuwait_చురుకైన(&rq->hotplug_wait)) {
			raw_spin_rq_unlock(rq);
			rcuwait_wake_పైకి(&rq->hotplug_wait);
			raw_spin_rq_తాళం(rq);
		}
		తిరిగి;
	}

	get_task_struct(నెట్టు_task);
	/*
	 * Temporarily drop rq->lock such that we can wake-up the stop task.
	 * Both preemption and IRQs are still disabled.
	 */
	preempt_disable();
	raw_spin_rq_unlock(rq);
	stop_one_cpu_nowait(rq->cpu, __balance_push_cpu_stop, నెట్టు_task,
			    this_cpu_ptr(&నెట్టు_work));
	preempt_enable();
	/*
	 * At this point need_resched() is true and we'll take the loop in
	 * schedule(). The next pick is obviously going to be the stop task
	 * which kthread_is_per_cpu() and will push this task away.
	 */
	raw_spin_rq_తాళం(rq);
}

స్థిర శూన్య balance_నెట్టు_సమితి(పూర్ణాంకం cpu, bool on)
{
	నిర్మాణం rq *rq = cpu_rq(cpu);
	నిర్మాణం rq_flags rf;

	rq_తాళం_irqsave(rq, &rf);
	ఒకవేళ (on) {
		WARN_ON_ONCE(rq->balance_callback);
		rq->balance_callback = &balance_నెట్టు_callback;
	} లేకపోతే ఒకవేళ (rq->balance_callback == &balance_నెట్టు_callback) {
		rq->balance_callback = ఖాళీ;
	}
	rq_unlock_irqrestore(rq, &rf);
}

/*
 * Invoked from a CPUs hotplug control thread after the CPU has been marked
 * inactive. All tasks which are not per CPU kernel threads are either
 * pushed off this CPU now via balance_push() or placed on a different CPU
 * during wakeup. Wait until the CPU is quiescent.
 */
స్థిర శూన్య balance_hotplug_wait(శూన్య)
{
	నిర్మాణం rq *rq = this_rq();

	rcuwait_wait_event(&rq->hotplug_wait,
			   rq->nr_running == 1 && !rq_has_pinned_tasks(rq),
			   TASK_UNINTERRUPTIBLE);
}

#లేకపోతే /* !CONFIG_HOTPLUG_CPU: */

స్థిర అంతఃవరుస శూన్య balance_నెట్టు(నిర్మాణం rq *rq)
{
}

స్థిర అంతఃవరుస శూన్య balance_నెట్టు_సమితి(పూర్ణాంకం cpu, bool on)
{
}

స్థిర అంతఃవరుస శూన్య balance_hotplug_wait(శూన్య)
{
}

#అయితే_ముగింపు /* !CONFIG_HOTPLUG_CPU */

శూన్య సమితి_rq_online(నిర్మాణం rq *rq)
{
	ఒకవేళ (!rq->online) {
		స్థిరరాశి నిర్మాణం sched_class *class;

		cpumask_సమితి_cpu(rq->cpu, rq->rd->online);
		rq->online = 1;

		for_each_class(class) {
			ఒకవేళ (class->rq_online)
				class->rq_online(rq);
		}
	}
}

శూన్య సమితి_rq_offline(నిర్మాణం rq *rq)
{
	ఒకవేళ (rq->online) {
		స్థిరరాశి నిర్మాణం sched_class *class;

		update_rq_clock(rq);
		for_each_class(class) {
			ఒకవేళ (class->rq_offline)
				class->rq_offline(rq);
		}

		cpumask_clear_cpu(rq->cpu, rq->rd->online);
		rq->online = 0;
	}
}

స్థిర అంతఃవరుస శూన్య sched_సమితి_rq_online(నిర్మాణం rq *rq, పూర్ణాంకం cpu)
{
	నిర్మాణం rq_flags rf;

	rq_తాళం_irqsave(rq, &rf);
	ఒకవేళ (rq->rd) {
		BUG_ON(!cpumask_test_cpu(cpu, rq->rd->పరిధి));
		సమితి_rq_online(rq);
	}
	rq_unlock_irqrestore(rq, &rf);
}

స్థిర అంతఃవరుస శూన్య sched_సమితి_rq_offline(నిర్మాణం rq *rq, పూర్ణాంకం cpu)
{
	నిర్మాణం rq_flags rf;

	rq_తాళం_irqsave(rq, &rf);
	ఒకవేళ (rq->rd) {
		BUG_ON(!cpumask_test_cpu(cpu, rq->rd->పరిధి));
		సమితి_rq_offline(rq);
	}
	rq_unlock_irqrestore(rq, &rf);
}

/*
 * used to mark begin/end of suspend/resume:
 */
స్థిర పూర్ణాంకం అంకె_cpus_frozen;

/*
 * Update cpusets according to cpu_active mask.  If cpusets are
 * disabled, cpuset_update_active_cpus() becomes a simple wrapper
 * around partition_sched_domains().
 *
 * If we come here as part of a suspend/resume, don't touch cpusets because we
 * want to restore it back to its original state upon resume anyway.
 */
స్థిర శూన్య cpuset_cpu_చురుకైన(శూన్య)
{
	ఒకవేళ (cpuhp_tasks_frozen) {
		/*
		 * num_cpus_frozen tracks how many CPUs are involved in suspend
		 * resume sequence. As long as this is not the last online
		 * operation in the resume sequence, just build a single sched
		 * domain, ignoring cpusets.
		 */
		cpuset_reset_sched_domains();
		ఒకవేళ (--అంకె_cpus_frozen)
			తిరిగి;
		/*
		 * This is the last CPU online operation. So fall through and
		 * restore the original sched domains by considering the
		 * cpuset configurations.
		 */
		cpuset_force_rebuild();
	}
	cpuset_update_చురుకైన_cpus();
}

స్థిర శూన్య cpuset_cpu_inactive(గుర్తులేని పూర్ణాంకం cpu)
{
	ఒకవేళ (!cpuhp_tasks_frozen) {
		cpuset_update_చురుకైన_cpus();
	} లేకపోతే {
		అంకె_cpus_frozen++;
		cpuset_reset_sched_domains();
	}
}

స్థిర అంతఃవరుస శూన్య sched_smt_present_inc(పూర్ణాంకం cpu)
{
	ఒకవేళ (cpumask_బరువు(cpu_smt_ముసుగు(cpu)) == 2)
		static_branch_inc_cpuslocked(&sched_smt_present);
}

స్థిర అంతఃవరుస శూన్య sched_smt_present_dec(పూర్ణాంకం cpu)
{
	ఒకవేళ (cpumask_బరువు(cpu_smt_ముసుగు(cpu)) == 2)
		static_branch_dec_cpuslocked(&sched_smt_present);
}

పూర్ణాంకం sched_cpu_activate(గుర్తులేని పూర్ణాంకం cpu)
{
	నిర్మాణం rq *rq = cpu_rq(cpu);

	/*
	 * Clear the balance_push callback and prepare to schedule
	 * regular tasks.
	 */
	balance_నెట్టు_సమితి(cpu, false);

	/*
	 * When going up, increment the number of cores with SMT present.
	 */
	sched_smt_present_inc(cpu);
	సమితి_cpu_చురుకైన(cpu, true);

	ఒకవేళ (sched_smp_initialized) {
		sched_update_numa(cpu, true);
		sched_domains_numa_masks_సమితి(cpu);
		cpuset_cpu_చురుకైన();
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
	sched_సమితి_rq_online(rq, cpu);

	/* preferred is subset of active and follows its state */
	సమితి_cpu_preferred(cpu, true);

	తిరిగి 0;
}

పూర్ణాంకం sched_cpu_deactivate(గుర్తులేని పూర్ణాంకం cpu)
{
	నిర్మాణం rq *rq = cpu_rq(cpu);
	పూర్ణాంకం ret;

	ret = dl_bw_deactivate(cpu);

	ఒకవేళ (ret)
		తిరిగి ret;

	సమితి_cpu_preferred(cpu, false);

	/*
	 * Remove CPU from nohz.idle_cpus_mask to prevent participating in
	 * load balancing when not active
	 */
	scoped_guard (rcu)
		nohz_balance_exit_idle(rq);

	సమితి_cpu_చురుకైన(cpu, false);

	/*
	 * From this point forward, this CPU will refuse to run any task that
	 * is not: migrate_disable() or KTHREAD_IS_PER_CPU, and will actively
	 * push those tasks away until this gets cleared, see
	 * sched_cpu_dying().
	 */
	balance_నెట్టు_సమితి(cpu, true);

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

	sched_సమితి_rq_offline(rq, cpu);

	scx_rq_deactivate(rq);

	/*
	 * When going down, decrement the number of cores with SMT present.
	 */
	sched_smt_present_dec(cpu);

	sched_core_cpu_deactivate(cpu);

	ఒకవేళ (!sched_smp_initialized)
		తిరిగి 0;

	sched_update_numa(cpu, false);
	cpuset_cpu_inactive(cpu);
	sched_domains_numa_masks_clear(cpu);
	తిరిగి 0;
}

స్థిర శూన్య sched_rq_cpu_starting(గుర్తులేని పూర్ణాంకం cpu)
{
	నిర్మాణం rq *rq = cpu_rq(cpu);

	rq->calc_ఎక్కించు_update = calc_ఎక్కించు_update;
	update_max_interval();
}

పూర్ణాంకం sched_cpu_starting(గుర్తులేని పూర్ణాంకం cpu)
{
	sched_core_cpu_starting(cpu);
	sched_rq_cpu_starting(cpu);
	sched_tick_ప్రారంభం(cpu);
	తిరిగి 0;
}

#నిర్వచితమైతే CONFIG_HOTPLUG_CPU

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
పూర్ణాంకం sched_cpu_wait_ఖాళీలు(గుర్తులేని పూర్ణాంకం cpu)
{
	balance_hotplug_wait();
	sched_force_init_mm();
	తిరిగి 0;
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
స్థిర శూన్య calc_ఎక్కించు_migrate(నిర్మాణం rq *rq)
{
	దీర్ఘ delta = calc_ఎక్కించు_fold_చురుకైన(rq, 1);

	ఒకవేళ (delta)
		atomic_long_కూడు(delta, &calc_ఎక్కించు_tasks);
}

స్థిర శూన్య dump_rq_tasks(నిర్మాణం rq *rq, స్థిరరాశి అక్షరం *loglvl)
{
	నిర్మాణం task_struct *g, *p;
	పూర్ణాంకం cpu = cpu_యొక్క(rq);

	lockdep_assert_rq_held(rq);

	printk("%sCPU%d enqueued tasks (%u total):\n", loglvl, cpu, rq->nr_running);
	for_each_process_thread(g, p) {
		ఒకవేళ (task_cpu(p) != cpu)
			కొనసాగు;

		ఒకవేళ (!task_on_rq_queued(p))
			కొనసాగు;

		printk("%s\tpid: %d, name: %s\n", loglvl, p->pid, p->comm);
	}
}

పూర్ణాంకం sched_cpu_dying(గుర్తులేని పూర్ణాంకం cpu)
{
	నిర్మాణం rq *rq = cpu_rq(cpu);
	నిర్మాణం rq_flags rf;

	/* Handle pending wakeups and then migrate everything off */
	sched_tick_stop(cpu);

	rq_తాళం_irqsave(rq, &rf);
	update_rq_clock(rq);
	ఒకవేళ (rq->nr_running != 1 || rq_has_pinned_tasks(rq)) {
		WARN(true, "Dying CPU not properly vacated!");
		dump_rq_tasks(rq, KERN_WARNING);
	}
	dl_server_stop(&rq->fair_server);
#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
	dl_server_stop(&rq->ext_server);
#అయితే_ముగింపు
	rq_unlock_irqrestore(rq, &rf);

	calc_ఎక్కించు_migrate(rq);
	update_max_interval();
	hrtick_clear(rq);
	sched_core_cpu_dying(cpu);
	తిరిగి 0;
}
#అయితే_ముగింపు /* CONFIG_HOTPLUG_CPU */

శూన్య __init sched_init_smp(శూన్య)
{
	sched_init_numa(NUMA_NO_కణుపు);

	prandom_init_once(&sched_rnd_state);

	/*
	 * There's no userspace yet to cause hotplug operations; hence all the
	 * CPU masks are stable and all blatant races in the below code cannot
	 * happen.
	 */
	sched_domains_mutex_తాళం();
	sched_init_domains(cpu_చురుకైన_ముసుగు);
	sched_domains_mutex_unlock();

	/* Move init over to a non-isolated CPU */
	ఒకవేళ (సమితి_cpus_allowed_ptr(current, housekeeping_cpumask(HK_TYPE_DOMAIN)) < 0)
		BUG();
	current->flags &= ~PF_NO_SETAFFINITY;
	sched_init_granularity();

	init_sched_rt_class();
	init_sched_dl_class();

	sched_init_dl_servers();

	sched_smp_initialized = true;
}

స్థిర పూర్ణాంకం __init migration_init(శూన్య)
{
	sched_cpu_starting(smp_processor_id());
	తిరిగి 0;
}
early_initcall(migration_init);

పూర్ణాంకం లోపల_sched_functions(గుర్తులేని దీర్ఘ addr)
{
	తిరిగి లోపల_తాళం_functions(addr) ||
		(addr >= (గుర్తులేని దీర్ఘ)__sched_text_start
		&& addr < (గుర్తులేని దీర్ఘ)__sched_text_end);
}

#నిర్వచితమైతే CONFIG_CGROUP_SCHED
/*
 * Default task group.
 * Every task in system belongs to this group at bootup.
 */
నిర్మాణం task_సమూహం మూలం_task_సమూహం;
LIST_తల(task_groups);

/* Cacheline aligned slab cache for task_group */
స్థిర నిర్మాణం kmem_cache *task_సమూహం_cache __ro_after_init;
#అయితే_ముగింపు

శూన్య __init sched_init(శూన్య)
{
	గుర్తులేని దీర్ఘ __maybe_unused ptr = 0;
	పూర్ణాంకం i;

	/* Make sure the linker didn't screw up */
	BUG_ON(!sched_class_above(&stop_sched_class, &dl_sched_class));
	BUG_ON(!sched_class_above(&dl_sched_class, &rt_sched_class));
	BUG_ON(!sched_class_above(&rt_sched_class, &fair_sched_class));
	BUG_ON(!sched_class_above(&fair_sched_class, &idle_sched_class));
#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
	BUG_ON(!sched_class_above(&fair_sched_class, &ext_sched_class));
	BUG_ON(!sched_class_above(&ext_sched_class, &idle_sched_class));
#అయితే_ముగింపు

	wait_బిట్టు_init();

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
	మూలం_task_సమూహం.cfs_rq = &runqueues.cfs;

	మూలం_task_సమూహం.shares = మూలం_TASK_సమూహం_ఎక్కించు;
	init_cfs_bandwidth(&మూలం_task_సమూహం.cfs_bandwidth, ఖాళీ);
#అయితే_ముగింపు /* CONFIG_FAIR_GROUP_SCHED */
#నిర్వచితమైతే CONFIG_EXT_సమూహం_SCHED
	scx_tg_init(&మూలం_task_సమూహం);
#అయితే_ముగింపు /* CONFIG_EXT_GROUP_SCHED */
#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED
	ptr += 2 * nr_cpu_ids * పరిమాణం(శూన్య **);
	ptr = (గుర్తులేని దీర్ఘ)kzalloc(ptr, GFP_NOWAIT);
	మూలం_task_సమూహం.rt_se = (నిర్మాణం sched_rt_entity **)ptr;
	ptr += nr_cpu_ids * పరిమాణం(శూన్య **);

	మూలం_task_సమూహం.rt_rq = (నిర్మాణం rt_rq **)ptr;
	ptr += nr_cpu_ids * పరిమాణం(శూన్య **);

#అయితే_ముగింపు /* CONFIG_RT_GROUP_SCHED */

	init_defrootdomain();

#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED
	init_rt_bandwidth(&మూలం_task_సమూహం.rt_bandwidth,
			global_rt_period(), global_rt_runtime());
#అయితే_ముగింపు /* CONFIG_RT_GROUP_SCHED */

#నిర్వచితమైతే CONFIG_CGROUP_SCHED
	task_సమూహం_cache = KMEM_CACHE(task_సమూహం, 0);

	list_కూడు(&మూలం_task_సమూహం.list, &task_groups);
	INIT_LIST_తల(&మూలం_task_సమూహం.children);
	INIT_LIST_తల(&మూలం_task_సమూహం.siblings);
	autogroup_init(&init_task);
#అయితే_ముగింపు /* CONFIG_CGROUP_SCHED */

	for_each_possible_cpu(i) {
		నిర్మాణం rq *rq;

		rq = cpu_rq(i);
		raw_spin_తాళం_init(&rq->__lock);
		rq->nr_running = 0;
		rq->calc_ఎక్కించు_చురుకైన = 0;
		rq->calc_ఎక్కించు_update = jiffies + ఎక్కించు_FREQ;
		init_cfs_rq(&rq->cfs);
		init_rt_rq(&rq->rt);
		init_dl_rq(&rq->dl);
#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
		INIT_LIST_తల(&rq->ఆకు_cfs_rq_list);
		rq->tmp_alone_branch = &rq->ఆకు_cfs_rq_list;
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
		init_tg_cfs_నమోదు(&మూలం_task_సమూహం, &rq->cfs, ఖాళీ, i, ఖాళీ);
#అయితే_ముగింపు /* CONFIG_FAIR_GROUP_SCHED */

#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED
		/*
		 * This is required for init cpu because rt.c:__enable_runtime()
		 * starts working after scheduler_running, which is not the case
		 * yet.
		 */
		rq->rt.rt_runtime = global_rt_runtime();
		init_tg_rt_నమోదు(&మూలం_task_సమూహం, &rq->rt, ఖాళీ, i, ఖాళీ);
#అయితే_ముగింపు
		rq->తదుపరి_class = &idle_sched_class;

		rq->sd = ఖాళీ;
		rq->rd = ఖాళీ;
		rq->cpu_సామర్థ్యం = SCHED_సామర్థ్యం_SCALE;
		rq->balance_callback = &balance_నెట్టు_callback;
		rq->చురుకైన_balance = 0;
		rq->తదుపరి_balance = jiffies;
		rq->నెట్టు_cpu = 0;
		rq->cpu = i;
		rq->online = 0;
		rq->idle_stamp = 0;
		rq->avg_idle = 2*sysctl_sched_migration_ఖర్చు;
		rq->max_idle_balance_ఖర్చు = sysctl_sched_migration_ఖర్చు;

		INIT_LIST_తల(&rq->cfs_tasks);

		rq_attach_మూలం(rq, &def_మూలం_domain);
#నిర్వచితమైతే CONFIG_NO_HZ_COMMON
		rq->చివరి_blocked_ఎక్కించు_update_tick = jiffies;
		atomic_సమితి(&rq->nohz_flags, 0);

		INIT_CSD(&rq->nohz_csd, nohz_csd_func, rq);
#అయితే_ముగింపు
#నిర్వచితమైతే CONFIG_HOTPLUG_CPU
		rcuwait_init(&rq->hotplug_wait);
#అయితే_ముగింపు
		hrtick_rq_init(rq);
		atomic_సమితి(&rq->nr_iowait, 0);
		fair_server_init(rq);
#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
		ext_server_init(rq);
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_SCHED_CORE
		rq->core = rq;
		rq->core_pick = ఖాళీ;
		rq->core_dl_server = ఖాళీ;
		rq->core_enabled = 0;
		rq->core_వృక్షం = RB_మూలం;
		rq->core_forceidle_లెక్క = 0;
		rq->core_forceidle_occupation = 0;
		rq->core_forceidle_ప్రారంభం = 0;
		rq->core_pick_లోపల_flight = 0;

		rq->core_cookie = 0UL;
#అయితే_ముగింపు
#నిర్వచితమైతే CONFIG_SCHED_CACHE
		raw_spin_తాళం_init(&rq->cpu_epoch_తాళం);
		rq->cpu_epoch_తదుపరి = jiffies;
#అయితే_ముగింపు

		zalloc_cpumask_var_కణుపు(&rq->scratch_ముసుగు, GFP_KERNEL, cpu_గమ్యం_కణుపు(i));
	}

	సమితి_ఎక్కించు_బరువు(&init_task, false);
	init_task.se.ముక్క = sysctl_sched_పునాది_ముక్క,

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
	WARN_ON(!సమితి_kthread_struct(current));

	/*
	 * Make us the idle thread. Technically, schedule() should not be
	 * called from this thread, however somewhere below it might be,
	 * but because we are the idle thread, we just pick up running again
	 * when this runqueue becomes "idle".
	 */
	__sched_fork(0, current);
	init_idle(current, smp_processor_id());

	calc_ఎక్కించు_update = jiffies + ఎక్కించు_FREQ;

	idle_thread_సమితి_boot_cpu();

	balance_నెట్టు_సమితి(smp_processor_id(), false);
	init_sched_fair_class();
	init_sched_ext_class();

	psi_init();

	init_uclamp();

	preempt_dynamic_init();

	scheduler_running = 1;
}

#నిర్వచితమైతే CONFIG_DEBUG_ATOMIC_SLEEP

శూన్య __might_sleep(స్థిరరాశి అక్షరం *file, పూర్ణాంకం పంక్తి)
{
	గుర్తులేని పూర్ణాంకం state = get_current_state();
	/*
	 * Blocking primitives will set (and therefore destroy) current->state,
	 * since we will exit with TASK_RUNNING make sure we enter with it,
	 * otherwise we will destroy state.
	 */
	WARN_ONCE(state != TASK_RUNNING && current->task_state_change,
			"do not call blocking ops when !TASK_RUNNING; "
			"state=%x set at [<%p>] %pS\n", state,
			(శూన్య *)current->task_state_change,
			(శూన్య *)current->task_state_change);

	__might_resched(file, పంక్తి, 0);
}
EXPORT_చిహ్నం(__might_sleep);

స్థిర శూన్య print_preempt_disable_ip(పూర్ణాంకం preempt_స్థానభ్రంశం, గుర్తులేని దీర్ఘ ip)
{
	ఒకవేళ (!IS_ENABLED(CONFIG_DEBUG_PREEMPT))
		తిరిగి;

	ఒకవేళ (preempt_లెక్క() == preempt_స్థానభ్రంశం)
		తిరిగి;

	pr_err("Preemption disabled at:");
	print_ip_sym(KERN_ERR, ip);
}

స్థిర అంతఃవరుస bool resched_offsets_ok(గుర్తులేని పూర్ణాంకం offsets)
{
	గుర్తులేని పూర్ణాంకం nested = preempt_లెక్క();

	nested += rcu_preempt_లోతు() << MIGHT_RESCHED_RCU_జరుపు;

	తిరిగి nested == offsets;
}

శూన్య __might_resched(స్థిరరాశి అక్షరం *file, పూర్ణాంకం పంక్తి, గుర్తులేని పూర్ణాంకం offsets)
{
	/* Ratelimiting timestamp: */
	స్థిర గుర్తులేని దీర్ఘ prev_jiffy;

	గుర్తులేని దీర్ఘ preempt_disable_ip;

	/* WARN_ON_ONCE() by default, no rate limit required: */
	rcu_sleep_check();

	ఒకవేళ ((resched_offsets_ok(offsets) && !irqs_disabled() &&
	     !is_idle_task(current) && !current->non_ఖండం_లెక్క) ||
	    system_state == SYSTEM_BOOTING || system_state > SYSTEM_RUNNING ||
	    oops_లోపల_పురోగతి)
		తిరిగి;

	ఒకవేళ (సమయం_before(jiffies, prev_jiffy + HZ) && prev_jiffy)
		తిరిగి;
	prev_jiffy = jiffies;

	/* Save this before calling printk(), since that will clobber it: */
	preempt_disable_ip = get_preempt_disable_ip(current);

	pr_err("BUG: sleeping function called from invalid context at %s:%d\n",
	       file, పంక్తి);
	pr_err("in_atomic(): %d, irqs_disabled(): %d, non_block: %d, pid: %d, name: %s\n",
	       లోపల_atomic(), irqs_disabled(), current->non_ఖండం_లెక్క,
	       current->pid, current->comm);
	pr_err("preempt_count: %x, expected: %x\n", preempt_లెక్క(),
	       offsets & MIGHT_RESCHED_PREEMPT_ముసుగు);

	ఒకవేళ (IS_ENABLED(CONFIG_PREEMPT_RCU)) {
		pr_err("RCU nest depth: %d, expected: %u\n",
		       rcu_preempt_లోతు(), offsets >> MIGHT_RESCHED_RCU_జరుపు);
	}

	ఒకవేళ (task_దొంతర_చివర_corrupted(current))
		pr_emerg("Thread overran stack, or stack corrupted\n");

	debug_show_held_locks(current);
	ఒకవేళ (irqs_disabled())
		print_irqtrace_events(current);

	print_preempt_disable_ip(offsets & MIGHT_RESCHED_PREEMPT_ముసుగు,
				 preempt_disable_ip);

	dump_దొంతర();
	కూడు_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}
EXPORT_చిహ్నం(__might_resched);

శూన్య __cant_sleep(స్థిరరాశి అక్షరం *file, పూర్ణాంకం పంక్తి)
{
	స్థిర గుర్తులేని దీర్ఘ prev_jiffy;

	ఒకవేళ (irqs_disabled())
		తిరిగి;

	ఒకవేళ (!IS_ENABLED(CONFIG_PREEMPT_లెక్క))
		తిరిగి;

	ఒకవేళ (preempt_లెక్క())
		తిరిగి;

	ఒకవేళ (సమయం_before(jiffies, prev_jiffy + HZ) && prev_jiffy)
		తిరిగి;
	prev_jiffy = jiffies;

	printk(KERN_ERR "BUG: assuming atomic context at %s:%d\n", file, పంక్తి);
	printk(KERN_ERR "in_atomic(): %d, irqs_disabled(): %d, pid: %d, name: %s\n",
			లోపల_atomic(), irqs_disabled(),
			current->pid, current->comm);

	debug_show_held_locks(current);
	dump_దొంతర();
	కూడు_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}
EXPORT_చిహ్నం_GPL(__cant_sleep);

# ifdef CONFIG_SMP
శూన్య __cant_migrate(స్థిరరాశి అక్షరం *file, పూర్ణాంకం పంక్తి)
{
	స్థిర గుర్తులేని దీర్ఘ prev_jiffy;

	ఒకవేళ (irqs_disabled())
		తిరిగి;

	ఒకవేళ (is_migration_disabled(current))
		తిరిగి;

	ఒకవేళ (!IS_ENABLED(CONFIG_PREEMPT_లెక్క))
		తిరిగి;

	ఒకవేళ (preempt_లెక్క())
		తిరిగి;

	ఒకవేళ (సమయం_before(jiffies, prev_jiffy + HZ) && prev_jiffy)
		తిరిగి;
	prev_jiffy = jiffies;

	pr_err("BUG: assuming non migratable context at %s:%d\n", file, పంక్తి);
	pr_err("in_atomic(): %d, irqs_disabled(): %d, migration_disabled() %u pid: %d, name: %s\n",
	       లోపల_atomic(), irqs_disabled(), is_migration_disabled(current),
	       current->pid, current->comm);

	debug_show_held_locks(current);
	dump_దొంతర();
	కూడు_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}
EXPORT_చిహ్నం_GPL(__cant_migrate);
# endif /* CONFIG_SMP */
#అయితే_ముగింపు /* CONFIG_DEBUG_ATOMIC_SLEEP */

#నిర్వచితమైతే CONFIG_MAGIC_SYSRQ
శూన్య normalize_rt_tasks(శూన్య)
{
	నిర్మాణం task_struct *g, *p;
	నిర్మాణం sched_attr attr = {
		.sched_policy = SCHED_సాధారణ,
	};

	read_తాళం(&tasklist_తాళం);
	for_each_process_thread(g, p) {
		/*
		 * Only normalize user tasks:
		 */
		ఒకవేళ (p->flags & PF_KTHREAD)
			కొనసాగు;

		p->se.exec_ప్రారంభం = 0;
		schedstat_సమితి(p->stats.wait_ప్రారంభం,  0);
		schedstat_సమితి(p->stats.sleep_ప్రారంభం, 0);
		schedstat_సమితి(p->stats.ఖండం_ప్రారంభం, 0);

		ఒకవేళ (!rt_or_dl_task(p)) {
			/*
			 * Renice negative nice level userspace
			 * tasks back to 0:
			 */
			ఒకవేళ (task_nice(p) < 0)
				సమితి_user_nice(p, 0);
			కొనసాగు;
		}

		__sched_setscheduler(p, &attr, false, false);
	}
	read_unlock(&tasklist_తాళం);
}

#అయితే_ముగింపు /* CONFIG_MAGIC_SYSRQ */

#నిర్వచితమైతే CONFIG_KGDB_KDB
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
నిర్మాణం task_struct *curr_task(పూర్ణాంకం cpu)
{
	తిరిగి cpu_curr(cpu);
}

#అయితే_ముగింపు /* CONFIG_KGDB_KDB */

#నిర్వచితమైతే CONFIG_CGROUP_SCHED
/* task_group_lock serializes the addition/removal of task groups */
స్థిర DEFINE_SPINLOCK(task_సమూహం_తాళం);

స్థిర అంతఃవరుస శూన్య alloc_uclamp_sched_సమూహం(నిర్మాణం task_సమూహం *tg,
					    నిర్మాణం task_సమూహం *మాతృ)
{
#నిర్వచితమైతే CONFIG_UCLAMP_TASK_సమూహం
	గణన uclamp_id clamp_id;

	for_each_clamp_id(clamp_id) {
		uclamp_se_సమితి(&tg->uclamp_req[clamp_id],
			      uclamp_none(clamp_id), false);
		tg->uclamp[clamp_id] = మాతృ->uclamp[clamp_id];
	}
#అయితే_ముగింపు
}

స్థిర శూన్య sched_free_సమూహం(నిర్మాణం task_సమూహం *tg)
{
	free_fair_sched_సమూహం(tg);
	free_rt_sched_సమూహం(tg);
	autogroup_free(tg);
	kmem_cache_free(task_సమూహం_cache, tg);
}

స్థిర శూన్య sched_free_సమూహం_rcu(నిర్మాణం rcu_తల *rcu)
{
	sched_free_సమూహం(container_యొక్క(rcu, నిర్మాణం task_సమూహం, rcu));
}

స్థిర శూన్య sched_unregister_సమూహం(నిర్మాణం task_సమూహం *tg)
{
	unregister_fair_sched_సమూహం(tg);
	unregister_rt_sched_సమూహం(tg);
	/*
	 * We have to wait for yet another RCU grace period to expire, as
	 * print_cfs_stats() might run concurrently.
	 */
	call_rcu(&tg->rcu, sched_free_సమూహం_rcu);
}

/* allocate runqueue etc for a new task group */
నిర్మాణం task_సమూహం *sched_create_సమూహం(నిర్మాణం task_సమూహం *మాతృ)
{
	నిర్మాణం task_సమూహం *tg;

	tg = kmem_cache_alloc(task_సమూహం_cache, GFP_KERNEL | __GFP_ZERO);
	ఒకవేళ (!tg)
		తిరిగి ERR_PTR(-ENOMEM);

	ఒకవేళ (!alloc_fair_sched_సమూహం(tg, మాతృ))
		వెళ్ళు err;

	ఒకవేళ (!alloc_rt_sched_సమూహం(tg, మాతృ))
		వెళ్ళు err;

	scx_tg_init(tg);
	alloc_uclamp_sched_సమూహం(tg, మాతృ);

	తిరిగి tg;

err:
	sched_free_సమూహం(tg);
	తిరిగి ERR_PTR(-ENOMEM);
}

శూన్య sched_online_సమూహం(నిర్మాణం task_సమూహం *tg, నిర్మాణం task_సమూహం *మాతృ)
{
	గుర్తులేని దీర్ఘ flags;

	spin_తాళం_irqsave(&task_సమూహం_తాళం, flags);
	list_కూడు_తోక_rcu(&tg->list, &task_groups);

	/* Root should already exist: */
	WARN_ON(!మాతృ);

	tg->మాతృ = మాతృ;
	INIT_LIST_తల(&tg->children);
	list_కూడు_rcu(&tg->siblings, &మాతృ->children);
	spin_unlock_irqrestore(&task_సమూహం_తాళం, flags);

	online_fair_sched_సమూహం(tg);
}

/* RCU callback to free various structures associated with a task group */
స్థిర శూన్య sched_unregister_సమూహం_rcu(నిర్మాణం rcu_తల *rhp)
{
	/* Now it should be safe to free those cfs_rqs: */
	sched_unregister_సమూహం(container_యొక్క(rhp, నిర్మాణం task_సమూహం, rcu));
}

శూన్య sched_destroy_సమూహం(నిర్మాణం task_సమూహం *tg)
{
	/* Wait for possible concurrent references to cfs_rqs complete: */
	call_rcu(&tg->rcu, sched_unregister_సమూహం_rcu);
}

శూన్య sched_release_సమూహం(నిర్మాణం task_సమూహం *tg)
{
	గుర్తులేని దీర్ఘ flags;

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
	spin_తాళం_irqsave(&task_సమూహం_తాళం, flags);
	list_del_rcu(&tg->list);
	list_del_rcu(&tg->siblings);
	spin_unlock_irqrestore(&task_సమూహం_తాళం, flags);
}

స్థిర శూన్య sched_change_సమూహం(నిర్మాణం task_struct *tsk)
{
	నిర్మాణం task_సమూహం *tg;

	/*
	 * All callers are synchronized by task_rq_lock(); we do not use RCU
	 * which is pointless here. Thus, we pass "true" to task_css_check()
	 * to prevent lockdep warnings.
	 */
	tg = container_యొక్క(task_css_check(tsk, cpu_cgrp_id, true),
			  నిర్మాణం task_సమూహం, css);
	tg = autogroup_task_సమూహం(tsk, tg);
	tsk->sched_task_సమూహం = tg;

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
	ఒకవేళ (tsk->sched_class->task_change_సమూహం)
		tsk->sched_class->task_change_సమూహం(tsk);
	లేకపోతే
#అయితే_ముగింపు
		సమితి_task_rq(tsk, task_cpu(tsk));
}

/*
 * Change task's runqueue when it moves between groups.
 *
 * The caller of this function should have put the task in its new group by
 * now. This function just updates tsk->se.cfs_rq and tsk->se.parent to reflect
 * its new group.
 */
శూన్య sched_move_task(నిర్మాణం task_struct *tsk, bool for_autogroup)
{
	గుర్తులేని పూర్ణాంకం వరుస_flags = వరుస_నుండి_తీయి_SAVE | వరుస_నుండి_తీయి_MOVE;
	bool resched = false;
	bool queued = false;
	నిర్మాణం rq *rq;

	CLASS(task_rq_తాళం_2, rq_guard)(tsk);
	rq = rq_guard.rq;

	scoped_guard (sched_change, tsk, వరుస_flags) {
		sched_change_సమూహం(tsk);
		ఒకవేళ (!for_autogroup)
			scx_cgroup_move_task(tsk);
		ఒకవేళ (scope->running)
			resched = true;
		queued = scope->queued;
	}

	ఒకవేళ (resched)
		resched_curr(rq);
	లేకపోతే ఒకవేళ (queued)
		wakeup_preempt(rq, tsk, 0);

	__balance_callbacks(rq, &rq_guard.rf);
}

స్థిర నిర్మాణం cgroup_subsys_state *
cpu_cgroup_css_alloc(నిర్మాణం cgroup_subsys_state *మాతృ_css)
{
	నిర్మాణం task_సమూహం *మాతృ = css_tg(మాతృ_css);
	నిర్మాణం task_సమూహం *tg;

	ఒకవేళ (!మాతృ) {
		/* This is early initialization for the top cgroup */
		తిరిగి &మూలం_task_సమూహం.css;
	}

	tg = sched_create_సమూహం(మాతృ);
	ఒకవేళ (IS_ERR(tg))
		తిరిగి ERR_PTR(-ENOMEM);

	తిరిగి &tg->css;
}

/* Expose task group only after completing cgroup initialization */
స్థిర పూర్ణాంకం cpu_cgroup_css_online(నిర్మాణం cgroup_subsys_state *css)
{
	నిర్మాణం task_సమూహం *tg = css_tg(css);
	నిర్మాణం task_సమూహం *మాతృ = css_tg(css->మాతృ);
	పూర్ణాంకం ret;

	ret = scx_tg_online(tg);
	ఒకవేళ (ret)
		తిరిగి ret;

	ఒకవేళ (మాతృ)
		sched_online_సమూహం(tg, మాతృ);

#నిర్వచితమైతే CONFIG_UCLAMP_TASK_సమూహం
	/* Propagate the effective uclamp value for the new group */
	guard(mutex)(&uclamp_mutex);
	guard(rcu)();
	cpu_util_update_eff(css);
#అయితే_ముగింపు

	తిరిగి 0;
}

స్థిర శూన్య cpu_cgroup_css_offline(నిర్మాణం cgroup_subsys_state *css)
{
	నిర్మాణం task_సమూహం *tg = css_tg(css);

	scx_tg_offline(tg);
}

స్థిర శూన్య cpu_cgroup_css_released(నిర్మాణం cgroup_subsys_state *css)
{
	నిర్మాణం task_సమూహం *tg = css_tg(css);

	sched_release_సమూహం(tg);
}

స్థిర శూన్య cpu_cgroup_css_free(నిర్మాణం cgroup_subsys_state *css)
{
	నిర్మాణం task_సమూహం *tg = css_tg(css);

	/*
	 * Relies on the RCU grace period between css_released() and this.
	 */
	sched_unregister_సమూహం(tg);
}

స్థిర పూర్ణాంకం cpu_cgroup_can_attach(నిర్మాణం cgroup_taskset *tset)
{
#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED
	నిర్మాణం task_struct *task;
	నిర్మాణం cgroup_subsys_state *css;

	ఒకవేళ (!rt_సమూహం_sched_enabled())
		వెళ్ళు scx_check;

	cgroup_taskset_for_each(task, css, tset) {
		ఒకవేళ (!sched_rt_can_attach(css_tg(css), task))
			తిరిగి -EINVAL;
	}
scx_check:
#అయితే_ముగింపు /* CONFIG_RT_GROUP_SCHED */
	తిరిగి scx_cgroup_can_attach(tset);
}

స్థిర శూన్య cpu_cgroup_attach(నిర్మాణం cgroup_taskset *tset)
{
	నిర్మాణం task_struct *task;
	నిర్మాణం cgroup_subsys_state *css;

	cgroup_taskset_for_each(task, css, tset)
		sched_move_task(task, false);
}

స్థిర శూన్య cpu_cgroup_cancel_attach(నిర్మాణం cgroup_taskset *tset)
{
	scx_cgroup_cancel_attach(tset);
}

#నిర్వచితమైతే CONFIG_UCLAMP_TASK_సమూహం
స్థిర శూన్య cpu_util_update_eff(నిర్మాణం cgroup_subsys_state *css)
{
	నిర్మాణం cgroup_subsys_state *శిఖరం_css = css;
	నిర్మాణం uclamp_se *uc_మాతృ = ఖాళీ;
	నిర్మాణం uclamp_se *uc_se = ఖాళీ;
	గుర్తులేని పూర్ణాంకం eff[UCLAMP_CNT];
	గణన uclamp_id clamp_id;
	గుర్తులేని పూర్ణాంకం clamps;

	lockdep_assert_held(&uclamp_mutex);
	WARN_ON_ONCE(!rcu_read_తాళం_held());

	css_for_each_descendant_pre(css, శిఖరం_css) {
		uc_మాతృ = css_tg(css)->మాతృ
			? css_tg(css)->మాతృ->uclamp : ఖాళీ;

		for_each_clamp_id(clamp_id) {
			/* Assume effective clamps matches requested clamps */
			eff[clamp_id] = css_tg(css)->uclamp_req[clamp_id].విలువ;
			/* Cap effective clamps with parent's effective clamps */
			ఒకవేళ (uc_మాతృ &&
			    eff[clamp_id] > uc_మాతృ[clamp_id].విలువ) {
				eff[clamp_id] = uc_మాతృ[clamp_id].విలువ;
			}
		}
		/* Ensure protection is always capped by limit */
		eff[UCLAMP_MIN] = min(eff[UCLAMP_MIN], eff[UCLAMP_MAX]);

		/* Propagate most restrictive effective clamps */
		clamps = 0x0;
		uc_se = css_tg(css)->uclamp;
		for_each_clamp_id(clamp_id) {
			ఒకవేళ (eff[clamp_id] == uc_se[clamp_id].విలువ)
				కొనసాగు;
			uc_se[clamp_id].విలువ = eff[clamp_id];
			uc_se[clamp_id].పాత్ర_id = uclamp_పాత్ర_id(eff[clamp_id]);
			clamps |= (0x1 << clamp_id);
		}
		ఒకవేళ (!clamps) {
			css = css_rightmost_descendant(css);
			కొనసాగు;
		}

		/* Immediately update descendants RUNNABLE tasks */
		uclamp_update_చురుకైన_tasks(css);
	}
}

/*
 * Integer 10^N with a given N exponent by casting to integer the literal "1eN"
 * C expression. Since there is no way to convert a macro argument (N) into a
 * character constant, use two levels of macros.
 */
#నిర్వచించు _POW10(ఘాతాంకం) ((గుర్తులేని పూర్ణాంకం)1e##exp)
#నిర్వచించు POW10(ఘాతాంకం) _POW10(ఘాతాంకం)

నిర్మాణం uclamp_request {
#నిర్వచించు UCLAMP_PERCENT_జరుపు	2
#నిర్వచించు UCLAMP_PERCENT_SCALE	(100 * POW10(UCLAMP_PERCENT_జరుపు))
	s64 percent;
	u64 util;
	పూర్ణాంకం ret;
};

స్థిర అంతఃవరుస నిర్మాణం uclamp_request
సామర్థ్యం_from_percent(అక్షరం *buf)
{
	నిర్మాణం uclamp_request req = {
		.percent = UCLAMP_PERCENT_SCALE,
		.util = SCHED_సామర్థ్యం_SCALE,
		.ret = 0,
	};

	buf = strim(buf);
	ఒకవేళ (strcmp(buf, "max")) {
		req.ret = cgroup_parse_float(buf, UCLAMP_PERCENT_జరుపు,
					     &req.percent);
		ఒకవేళ (req.ret)
			తిరిగి req;
		ఒకవేళ ((u64)req.percent > UCLAMP_PERCENT_SCALE) {
			req.ret = -ERANGE;
			తిరిగి req;
		}

		req.util = req.percent << SCHED_సామర్థ్యం_జరుపు;
		req.util = DIV_విడత_CLOSEST_ULL(req.util, UCLAMP_PERCENT_SCALE);
	}

	తిరిగి req;
}

స్థిర ssize_t cpu_uclamp_write(నిర్మాణం kernfs_open_file *యొక్క, అక్షరం *buf,
				కొలత_t nbytes, loff_t off,
				గణన uclamp_id clamp_id)
{
	నిర్మాణం uclamp_request req;
	నిర్మాణం task_సమూహం *tg;

	req = సామర్థ్యం_from_percent(buf);
	ఒకవేళ (req.ret)
		తిరిగి req.ret;

	sched_uclamp_enable();

	guard(mutex)(&uclamp_mutex);
	guard(rcu)();

	tg = css_tg(యొక్క_css(యొక్క));
	ఒకవేళ (tg->uclamp_req[clamp_id].విలువ != req.util)
		uclamp_se_సమితి(&tg->uclamp_req[clamp_id], req.util, false);

	/*
	 * Because of not recoverable conversion rounding we keep track of the
	 * exact requested value
	 */
	tg->uclamp_pct[clamp_id] = req.percent;

	/* Update effective clamps to track the most restrictive value */
	cpu_util_update_eff(యొక్క_css(యొక్క));

	తిరిగి nbytes;
}

స్థిర ssize_t cpu_uclamp_min_write(నిర్మాణం kernfs_open_file *యొక్క,
				    అక్షరం *buf, కొలత_t nbytes,
				    loff_t off)
{
	తిరిగి cpu_uclamp_write(యొక్క, buf, nbytes, off, UCLAMP_MIN);
}

స్థిర ssize_t cpu_uclamp_max_write(నిర్మాణం kernfs_open_file *యొక్క,
				    అక్షరం *buf, కొలత_t nbytes,
				    loff_t off)
{
	తిరిగి cpu_uclamp_write(యొక్క, buf, nbytes, off, UCLAMP_MAX);
}

స్థిర అంతఃవరుస శూన్య cpu_uclamp_print(నిర్మాణం seq_file *sf,
				    గణన uclamp_id clamp_id)
{
	నిర్మాణం task_సమూహం *tg;
	u64 util_clamp;
	u64 percent;
	u32 rem;

	scoped_guard (rcu) {
		tg = css_tg(seq_css(sf));
		util_clamp = tg->uclamp_req[clamp_id].విలువ;
	}

	ఒకవేళ (util_clamp == SCHED_సామర్థ్యం_SCALE) {
		seq_puts(sf, "max\n");
		తిరిగి;
	}

	percent = tg->uclamp_pct[clamp_id];
	percent = div_u64_rem(percent, POW10(UCLAMP_PERCENT_జరుపు), &rem);
	seq_printf(sf, "%llu.%0*u\n", percent, UCLAMP_PERCENT_జరుపు, rem);
}

స్థిర పూర్ణాంకం cpu_uclamp_min_show(నిర్మాణం seq_file *sf, శూన్య *v)
{
	cpu_uclamp_print(sf, UCLAMP_MIN);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం cpu_uclamp_max_show(నిర్మాణం seq_file *sf, శూన్య *v)
{
	cpu_uclamp_print(sf, UCLAMP_MAX);
	తిరిగి 0;
}
#అయితే_ముగింపు /* CONFIG_UCLAMP_TASK_GROUP */

#నిర్వచితమైతే CONFIG_సమూహం_SCHED_బరువు
స్థిర గుర్తులేని దీర్ఘ tg_బరువు(నిర్మాణం task_సమూహం *tg)
{
#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
	తిరిగి scale_ఎక్కించు_కిందికి(tg->shares);
#లేకపోతే
	తిరిగి sched_బరువు_from_cgroup(tg->scx.బరువు);
#అయితే_ముగింపు
}

స్థిర పూర్ణాంకం cpu_shares_write_u64(నిర్మాణం cgroup_subsys_state *css,
				నిర్మాణం cftype *cftype, u64 shareval)
{
	పూర్ణాంకం ret;

	ఒకవేళ (shareval > scale_ఎక్కించు_కిందికి(ULONG_MAX))
		shareval = MAX_SHARES;
	ret = sched_సమూహం_సమితి_shares(css_tg(css), scale_ఎక్కించు(shareval));
	ఒకవేళ (!ret)
		scx_సమూహం_సమితి_బరువు(css_tg(css),
				     sched_బరువు_గమ్యం_cgroup(shareval));
	తిరిగి ret;
}

స్థిర u64 cpu_shares_read_u64(నిర్మాణం cgroup_subsys_state *css,
			       నిర్మాణం cftype *cft)
{
	తిరిగి tg_బరువు(css_tg(css));
}
#అయితే_ముగింపు /* CONFIG_GROUP_SCHED_WEIGHT */

#నిర్వచితమైతే CONFIG_CFS_BANDWIDTH
స్థిర DEFINE_MUTEX(cfs_constraints_mutex);

స్థిర పూర్ణాంకం __cfs_schedulable(నిర్మాణం task_సమూహం *tg, u64 period, u64 runtime);

స్థిర పూర్ణాంకం tg_సమితి_cfs_bandwidth(నిర్మాణం task_సమూహం *tg,
				u64 period_us, u64 quota_us, u64 వ్యవధి_us)
{
	పూర్ణాంకం i, ret = 0, runtime_enabled, runtime_was_enabled;
	నిర్మాణం cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
	u64 period, quota, వ్యవధి;

	period = (u64)period_us * NSEC_చొప్పున_USEC;

	ఒకవేళ (quota_us == RUNTIME_INF)
		quota = RUNTIME_INF;
	లేకపోతే
		quota = (u64)quota_us * NSEC_చొప్పున_USEC;

	వ్యవధి = (u64)వ్యవధి_us * NSEC_చొప్పున_USEC;

	/*
	 * Prevent race between setting of cfs_rq->runtime_enabled and
	 * unthrottle_offline_cfs_rqs().
	 */
	guard(cpus_read_తాళం)();
	guard(mutex)(&cfs_constraints_mutex);

	ret = __cfs_schedulable(tg, period, quota);
	ఒకవేళ (ret)
		తిరిగి ret;

	runtime_enabled = quota != RUNTIME_INF;
	runtime_was_enabled = cfs_b->quota != RUNTIME_INF;
	/*
	 * If we need to toggle cfs_bandwidth_used, off->on must occur
	 * before making related changes, and on->off must occur afterwards
	 */
	ఒకవేళ (runtime_enabled && !runtime_was_enabled)
		cfs_bandwidth_usage_inc();

	scoped_guard (raw_spinlock_irq, &cfs_b->తాళం) {
		cfs_b->period = ns_గమ్యం_ktime(period);
		cfs_b->quota = quota;
		cfs_b->వ్యవధి = వ్యవధి;

		__refill_cfs_bandwidth_runtime(cfs_b);

		/*
		 * Restart the period timer (if active) to handle new
		 * period expiry:
		 */
		ఒకవేళ (runtime_enabled)
			ప్రారంభం_cfs_bandwidth(cfs_b);
	}

	for_each_online_cpu(i) {
		నిర్మాణం cfs_rq *cfs_rq = tg_cfs_rq(tg, i);
		నిర్మాణం rq *rq = cfs_rq->rq;

		guard(rq_తాళం_irq)(rq);

		cfs_rq->runtime_enabled = runtime_enabled;
		cfs_rq->runtime_మిగిలిన = 1;

		ఒకవేళ (cfs_rq->throttled) {
			update_rq_clock(rq);
			unthrottle_cfs_rq(cfs_rq);
		}
	}

	ఒకవేళ (runtime_was_enabled && !runtime_enabled)
		cfs_bandwidth_usage_dec();

	తిరిగి 0;
}

స్థిర u64 tg_get_cfs_period(నిర్మాణం task_సమూహం *tg)
{
	u64 cfs_period_us;

	cfs_period_us = ktime_గమ్యం_ns(tg->cfs_bandwidth.period);
	do_div(cfs_period_us, NSEC_చొప్పున_USEC);

	తిరిగి cfs_period_us;
}

స్థిర u64 tg_get_cfs_quota(నిర్మాణం task_సమూహం *tg)
{
	u64 quota_us;

	ఒకవేళ (tg->cfs_bandwidth.quota == RUNTIME_INF)
		తిరిగి RUNTIME_INF;

	quota_us = tg->cfs_bandwidth.quota;
	do_div(quota_us, NSEC_చొప్పున_USEC);

	తిరిగి quota_us;
}

స్థిర u64 tg_get_cfs_వ్యవధి(నిర్మాణం task_సమూహం *tg)
{
	u64 వ్యవధి_us;

	వ్యవధి_us = tg->cfs_bandwidth.వ్యవధి;
	do_div(వ్యవధి_us, NSEC_చొప్పున_USEC);

	తిరిగి వ్యవధి_us;
}

నిర్మాణం cfs_schedulable_దత్తాంశం {
	నిర్మాణం task_సమూహం *tg;
	u64 period, quota;
};

/*
 * normalize group quota/period to be quota/max_period
 * note: units are usecs
 */
స్థిర u64 normalize_cfs_quota(నిర్మాణం task_సమూహం *tg,
			       నిర్మాణం cfs_schedulable_దత్తాంశం *d)
{
	u64 quota, period;

	ఒకవేళ (tg == d->tg) {
		period = d->period;
		quota = d->quota;
	} లేకపోతే {
		period = tg_get_cfs_period(tg);
		quota = tg_get_cfs_quota(tg);
	}

	/* note: these should typically be equivalent */
	ఒకవేళ (quota == RUNTIME_INF || quota == -1)
		తిరిగి RUNTIME_INF;

	తిరిగి గమ్యం_నిష్పత్తి(period, quota);
}

స్థిర పూర్ణాంకం tg_cfs_schedulable_కిందికి(నిర్మాణం task_సమూహం *tg, శూన్య *దత్తాంశం)
{
	నిర్మాణం cfs_schedulable_దత్తాంశం *d = దత్తాంశం;
	నిర్మాణం cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
	s64 quota = 0, మాతృ_quota = -1;

	ఒకవేళ (!tg->మాతృ) {
		quota = RUNTIME_INF;
	} లేకపోతే {
		నిర్మాణం cfs_bandwidth *మాతృ_b = &tg->మాతృ->cfs_bandwidth;

		quota = normalize_cfs_quota(tg, d);
		మాతృ_quota = మాతృ_b->క్రమానుగత_quota;

		/*
		 * Ensure max(child_quota) <= parent_quota.  On cgroup2,
		 * always take the non-RUNTIME_INF min.  On cgroup1, only
		 * inherit when no limit is set. In both cases this is used
		 * by the scheduler to determine if a given CFS task has a
		 * bandwidth constraint at some higher level.
		 */
		ఒకవేళ (cgroup_subsys_on_dfl(cpu_cgrp_subsys)) {
			ఒకవేళ (quota == RUNTIME_INF)
				quota = మాతృ_quota;
			లేకపోతే ఒకవేళ (మాతృ_quota != RUNTIME_INF)
				quota = min(quota, మాతృ_quota);
		} లేకపోతే {
			ఒకవేళ (quota == RUNTIME_INF)
				quota = మాతృ_quota;
			లేకపోతే ఒకవేళ (మాతృ_quota != RUNTIME_INF && quota > మాతృ_quota)
				తిరిగి -EINVAL;
		}
	}
	cfs_b->క్రమానుగత_quota = quota;

	తిరిగి 0;
}

స్థిర పూర్ణాంకం __cfs_schedulable(నిర్మాణం task_సమూహం *tg, u64 period, u64 quota)
{
	నిర్మాణం cfs_schedulable_దత్తాంశం దత్తాంశం = {
		.tg = tg,
		.period = period,
		.quota = quota,
	};

	ఒకవేళ (quota != RUNTIME_INF) {
		do_div(దత్తాంశం.period, NSEC_చొప్పున_USEC);
		do_div(దత్తాంశం.quota, NSEC_చొప్పున_USEC);
	}

	guard(rcu)();
	తిరిగి walk_tg_వృక్షం(tg_cfs_schedulable_కిందికి, tg_nop, &దత్తాంశం);
}

స్థిర పూర్ణాంకం cpu_cfs_stat_show(నిర్మాణం seq_file *sf, శూన్య *v)
{
	నిర్మాణం task_సమూహం *tg = css_tg(seq_css(sf));
	నిర్మాణం cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;

	seq_printf(sf, "nr_periods %d\n", cfs_b->nr_periods);
	seq_printf(sf, "nr_throttled %d\n", cfs_b->nr_throttled);
	seq_printf(sf, "throttled_time %llu\n", cfs_b->throttled_సమయం);

	ఒకవేళ (schedstat_enabled() && tg != &మూలం_task_సమూహం) {
		నిర్మాణం sched_statistics *stats;
		u64 ws = 0;
		పూర్ణాంకం i;

		for_each_possible_cpu(i) {
			stats = __schedstats_from_se(tg_se(tg, i));
			ws += schedstat_val(stats->wait_కూడిక);
		}

		seq_printf(sf, "wait_sum %llu\n", ws);
	}

	seq_printf(sf, "nr_bursts %d\n", cfs_b->nr_వ్యవధి);
	seq_printf(sf, "burst_time %llu\n", cfs_b->వ్యవధి_సమయం);

	తిరిగి 0;
}

స్థిర u64 throttled_సమయం_self(నిర్మాణం task_సమూహం *tg)
{
	పూర్ణాంకం i;
	u64 మొత్తం = 0;

	for_each_possible_cpu(i) {
		మొత్తం += READ_ONCE(tg_cfs_rq(tg, i)->throttled_clock_self_సమయం);
	}

	తిరిగి మొత్తం;
}

స్థిర పూర్ణాంకం cpu_cfs_local_stat_show(నిర్మాణం seq_file *sf, శూన్య *v)
{
	నిర్మాణం task_సమూహం *tg = css_tg(seq_css(sf));

	seq_printf(sf, "throttled_time %llu\n", throttled_సమయం_self(tg));

	తిరిగి 0;
}
#అయితే_ముగింపు /* CONFIG_CFS_BANDWIDTH */

#నిర్వచితమైతే CONFIG_సమూహం_SCHED_BANDWIDTH
స్థిరరాశి u64 max_bw_quota_period_us = 1 * USEC_చొప్పున_SEC; /* 1s */
స్థిర స్థిరరాశి u64 min_bw_quota_period_us = 1 * USEC_చొప్పున_MSEC; /* 1ms */
/* More than 203 days if BW_SHIFT equals 20. */
స్థిర స్థిరరాశి u64 max_bw_runtime_us = MAX_BW;

స్థిర శూన్య tg_bandwidth(నిర్మాణం task_సమూహం *tg,
			 u64 *period_us_p, u64 *quota_us_p, u64 *వ్యవధి_us_p)
{
#నిర్వచితమైతే CONFIG_CFS_BANDWIDTH
	ఒకవేళ (period_us_p)
		*period_us_p = tg_get_cfs_period(tg);
	ఒకవేళ (quota_us_p)
		*quota_us_p = tg_get_cfs_quota(tg);
	ఒకవేళ (వ్యవధి_us_p)
		*వ్యవధి_us_p = tg_get_cfs_వ్యవధి(tg);
#లేకపోతే /* !CONFIG_CFS_BANDWIDTH */
	ఒకవేళ (period_us_p)
		*period_us_p = tg->scx.bw_period_us;
	ఒకవేళ (quota_us_p)
		*quota_us_p = tg->scx.bw_quota_us;
	ఒకవేళ (వ్యవధి_us_p)
		*వ్యవధి_us_p = tg->scx.bw_వ్యవధి_us;
#అయితే_ముగింపు /* CONFIG_CFS_BANDWIDTH */
}

స్థిర u64 cpu_period_read_u64(నిర్మాణం cgroup_subsys_state *css,
			       నిర్మాణం cftype *cft)
{
	u64 period_us;

	tg_bandwidth(css_tg(css), &period_us, ఖాళీ, ఖాళీ);
	తిరిగి period_us;
}

స్థిర పూర్ణాంకం tg_సమితి_bandwidth(నిర్మాణం task_సమూహం *tg,
			    u64 period_us, u64 quota_us, u64 వ్యవధి_us)
{
	స్థిరరాశి u64 max_usec = U64_MAX / NSEC_చొప్పున_USEC;
	పూర్ణాంకం ret = 0;

	ఒకవేళ (tg == &మూలం_task_సమూహం)
		తిరిగి -EINVAL;

	/* Values should survive translation to nsec */
	ఒకవేళ (period_us > max_usec ||
	    (quota_us != RUNTIME_INF && quota_us > max_usec) ||
	    వ్యవధి_us > max_usec)
		తిరిగి -EINVAL;

	/*
	 * Ensure we have some amount of bandwidth every period. This is to
	 * prevent reaching a state of large arrears when throttled via
	 * entity_tick() resulting in prolonged exit starvation.
	 */
	ఒకవేళ (quota_us < min_bw_quota_period_us ||
	    period_us < min_bw_quota_period_us)
		తిరిగి -EINVAL;

	/*
	 * Likewise, bound things on the other side by preventing insane quota
	 * periods.  This also allows us to normalize in computing quota
	 * feasibility.
	 */
	ఒకవేళ (period_us > max_bw_quota_period_us)
		తిరిగి -EINVAL;

	/*
	 * Bound quota to defend quota against overflow during bandwidth shift.
	 */
	ఒకవేళ (quota_us != RUNTIME_INF && quota_us > max_bw_runtime_us)
		తిరిగి -EINVAL;

	ఒకవేళ (quota_us != RUNTIME_INF && (వ్యవధి_us > quota_us ||
					వ్యవధి_us + quota_us > max_bw_runtime_us))
		తిరిగి -EINVAL;

#నిర్వచితమైతే CONFIG_CFS_BANDWIDTH
	ret = tg_సమితి_cfs_bandwidth(tg, period_us, quota_us, వ్యవధి_us);
#అయితే_ముగింపు /* CONFIG_CFS_BANDWIDTH */
	ఒకవేళ (!ret)
		scx_సమూహం_సమితి_bandwidth(tg, period_us, quota_us, వ్యవధి_us);
	తిరిగి ret;
}

స్థిర s64 cpu_quota_read_s64(నిర్మాణం cgroup_subsys_state *css,
			      నిర్మాణం cftype *cft)
{
	u64 quota_us;

	tg_bandwidth(css_tg(css), ఖాళీ, &quota_us, ఖాళీ);
	తిరిగి quota_us;	/* (s64)RUNTIME_INF becomes -1 */
}

స్థిర u64 cpu_వ్యవధి_read_u64(నిర్మాణం cgroup_subsys_state *css,
			      నిర్మాణం cftype *cft)
{
	u64 వ్యవధి_us;

	tg_bandwidth(css_tg(css), ఖాళీ, ఖాళీ, &వ్యవధి_us);
	తిరిగి వ్యవధి_us;
}

స్థిర పూర్ణాంకం cpu_period_write_u64(నిర్మాణం cgroup_subsys_state *css,
				నిర్మాణం cftype *cftype, u64 period_us)
{
	నిర్మాణం task_సమూహం *tg = css_tg(css);
	u64 quota_us, వ్యవధి_us;

	tg_bandwidth(tg, ఖాళీ, &quota_us, &వ్యవధి_us);
	తిరిగి tg_సమితి_bandwidth(tg, period_us, quota_us, వ్యవధి_us);
}

స్థిర పూర్ణాంకం cpu_quota_write_s64(నిర్మాణం cgroup_subsys_state *css,
			       నిర్మాణం cftype *cftype, s64 quota_us)
{
	నిర్మాణం task_సమూహం *tg = css_tg(css);
	u64 period_us, వ్యవధి_us;

	ఒకవేళ (quota_us < 0)
		quota_us = RUNTIME_INF;

	tg_bandwidth(tg, &period_us, ఖాళీ, &వ్యవధి_us);
	తిరిగి tg_సమితి_bandwidth(tg, period_us, quota_us, వ్యవధి_us);
}

స్థిర పూర్ణాంకం cpu_వ్యవధి_write_u64(నిర్మాణం cgroup_subsys_state *css,
			       నిర్మాణం cftype *cftype, u64 వ్యవధి_us)
{
	నిర్మాణం task_సమూహం *tg = css_tg(css);
	u64 period_us, quota_us;

	tg_bandwidth(tg, &period_us, &quota_us, ఖాళీ);
	తిరిగి tg_సమితి_bandwidth(tg, period_us, quota_us, వ్యవధి_us);
}
#అయితే_ముగింపు /* CONFIG_GROUP_SCHED_BANDWIDTH */

#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED
స్థిర పూర్ణాంకం cpu_rt_runtime_write(నిర్మాణం cgroup_subsys_state *css,
				నిర్మాణం cftype *cft, s64 val)
{
	తిరిగి sched_సమూహం_సమితి_rt_runtime(css_tg(css), val);
}

స్థిర s64 cpu_rt_runtime_read(నిర్మాణం cgroup_subsys_state *css,
			       నిర్మాణం cftype *cft)
{
	తిరిగి sched_సమూహం_rt_runtime(css_tg(css));
}

స్థిర పూర్ణాంకం cpu_rt_period_write_uint(నిర్మాణం cgroup_subsys_state *css,
				    నిర్మాణం cftype *cftype, u64 rt_period_us)
{
	తిరిగి sched_సమూహం_సమితి_rt_period(css_tg(css), rt_period_us);
}

స్థిర u64 cpu_rt_period_read_uint(నిర్మాణం cgroup_subsys_state *css,
				   నిర్మాణం cftype *cft)
{
	తిరిగి sched_సమూహం_rt_period(css_tg(css));
}
#అయితే_ముగింపు /* CONFIG_RT_GROUP_SCHED */

#నిర్వచితమైతే CONFIG_సమూహం_SCHED_బరువు
స్థిర s64 cpu_idle_read_s64(నిర్మాణం cgroup_subsys_state *css,
			       నిర్మాణం cftype *cft)
{
	తిరిగి css_tg(css)->idle;
}

స్థిర పూర్ణాంకం cpu_idle_write_s64(నిర్మాణం cgroup_subsys_state *css,
				నిర్మాణం cftype *cft, s64 idle)
{
	పూర్ణాంకం ret;

	ret = sched_సమూహం_సమితి_idle(css_tg(css), idle);
	ఒకవేళ (!ret)
		scx_సమూహం_సమితి_idle(css_tg(css), idle);
	తిరిగి ret;
}
#అయితే_ముగింపు /* CONFIG_GROUP_SCHED_WEIGHT */

స్థిర నిర్మాణం cftype cpu_legacy_files[] = {
#నిర్వచితమైతే CONFIG_సమూహం_SCHED_బరువు
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
#అయితే_ముగింపు
#నిర్వచితమైతే CONFIG_సమూహం_SCHED_BANDWIDTH
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
		.read_u64 = cpu_వ్యవధి_read_u64,
		.write_u64 = cpu_వ్యవధి_write_u64,
	},
#అయితే_ముగింపు
#నిర్వచితమైతే CONFIG_CFS_BANDWIDTH
	{
		.name = "stat",
		.seq_show = cpu_cfs_stat_show,
	},
	{
		.name = "stat.local",
		.seq_show = cpu_cfs_local_stat_show,
	},
#అయితే_ముగింపు
#నిర్వచితమైతే CONFIG_UCLAMP_TASK_సమూహం
	{
		.name = "uclamp.min",
		.flags = CFTYPE_NOT_ON_మూలం,
		.seq_show = cpu_uclamp_min_show,
		.లిఖ = cpu_uclamp_min_write,
	},
	{
		.name = "uclamp.max",
		.flags = CFTYPE_NOT_ON_మూలం,
		.seq_show = cpu_uclamp_max_show,
		.లిఖ = cpu_uclamp_max_write,
	},
#అయితే_ముగింపు
	{ }	/* Terminate */
};

#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED
స్థిర నిర్మాణం cftype rt_సమూహం_files[] = {
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

# ifdef CONFIG_RT_సమూహం_SCHED_DEFAULT_DISABLED
DEFINE_STATIC_కీలకం_FALSE(rt_సమూహం_sched);
# లేకపోతే
DEFINE_STATIC_కీలకం_TRUE(rt_సమూహం_sched);
# endif

స్థిర పూర్ణాంకం __init అమరిక_rt_సమూహం_sched(అక్షరం *str)
{
	దీర్ఘ val;

	ఒకవేళ (kstrtol(str, 0, &val) || val < 0 || val > 1) {
		pr_warn("Unable to set rt_group_sched\n");
		తిరిగి 1;
	}
	ఒకవేళ (val)
		static_branch_enable(&rt_సమూహం_sched);
	లేకపోతే
		static_branch_disable(&rt_సమూహం_sched);

	తిరిగి 1;
}
__setup("rt_group_sched=", అమరిక_rt_సమూహం_sched);

స్థిర పూర్ణాంకం __init cpu_rt_సమూహం_init(శూన్య)
{
	ఒకవేళ (!rt_సమూహం_sched_enabled())
		తిరిగి 0;

	WARN_ON(cgroup_కూడు_legacy_cftypes(&cpu_cgrp_subsys, rt_సమూహం_files));
	తిరిగి 0;
}
subsys_initcall(cpu_rt_సమూహం_init);
#అయితే_ముగింపు /* CONFIG_RT_GROUP_SCHED */

స్థిర పూర్ణాంకం cpu_extra_stat_show(నిర్మాణం seq_file *sf,
			       నిర్మాణం cgroup_subsys_state *css)
{
#నిర్వచితమైతే CONFIG_CFS_BANDWIDTH
	{
		నిర్మాణం task_సమూహం *tg = css_tg(css);
		నిర్మాణం cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
		u64 throttled_usec, వ్యవధి_usec;

		throttled_usec = cfs_b->throttled_సమయం;
		do_div(throttled_usec, NSEC_చొప్పున_USEC);
		వ్యవధి_usec = cfs_b->వ్యవధి_సమయం;
		do_div(వ్యవధి_usec, NSEC_చొప్పున_USEC);

		seq_printf(sf, "nr_periods %d\n"
			   "nr_throttled %d\n"
			   "throttled_usec %llu\n"
			   "nr_bursts %d\n"
			   "burst_usec %llu\n",
			   cfs_b->nr_periods, cfs_b->nr_throttled,
			   throttled_usec, cfs_b->nr_వ్యవధి, వ్యవధి_usec);
	}
#అయితే_ముగింపు /* CONFIG_CFS_BANDWIDTH */
	తిరిగి 0;
}

స్థిర పూర్ణాంకం cpu_local_stat_show(నిర్మాణం seq_file *sf,
			       నిర్మాణం cgroup_subsys_state *css)
{
#నిర్వచితమైతే CONFIG_CFS_BANDWIDTH
	{
		నిర్మాణం task_సమూహం *tg = css_tg(css);
		u64 throttled_self_usec;

		throttled_self_usec = throttled_సమయం_self(tg);
		do_div(throttled_self_usec, NSEC_చొప్పున_USEC);

		seq_printf(sf, "throttled_usec %llu\n",
			   throttled_self_usec);
	}
#అయితే_ముగింపు
	తిరిగి 0;
}

#నిర్వచితమైతే CONFIG_సమూహం_SCHED_బరువు

స్థిర u64 cpu_బరువు_read_u64(నిర్మాణం cgroup_subsys_state *css,
			       నిర్మాణం cftype *cft)
{
	తిరిగి sched_బరువు_గమ్యం_cgroup(tg_బరువు(css_tg(css)));
}

స్థిర పూర్ణాంకం cpu_బరువు_write_u64(నిర్మాణం cgroup_subsys_state *css,
				నిర్మాణం cftype *cft, u64 cgrp_బరువు)
{
	గుర్తులేని దీర్ఘ బరువు;
	పూర్ణాంకం ret;

	ఒకవేళ (cgrp_బరువు < CGROUP_బరువు_MIN || cgrp_బరువు > CGROUP_బరువు_MAX)
		తిరిగి -ERANGE;

	బరువు = sched_బరువు_from_cgroup(cgrp_బరువు);

	ret = sched_సమూహం_సమితి_shares(css_tg(css), scale_ఎక్కించు(బరువు));
	ఒకవేళ (!ret)
		scx_సమూహం_సమితి_బరువు(css_tg(css), cgrp_బరువు);
	తిరిగి ret;
}

స్థిర s64 cpu_బరువు_nice_read_s64(నిర్మాణం cgroup_subsys_state *css,
				    నిర్మాణం cftype *cft)
{
	గుర్తులేని దీర్ఘ బరువు = tg_బరువు(css_tg(css));
	పూర్ణాంకం చివరి_delta = గరిష్ఠ_పూర్ణాంకం;
	పూర్ణాంకం prio, delta;

	/* find the closest nice value to the current weight */
	కొరకు (prio = 0; prio < ARRAY_కొలత(sched_prio_గమ్యం_బరువు); prio++) {
		delta = నిరపేక్ష(sched_prio_గమ్యం_బరువు[prio] - బరువు);
		ఒకవేళ (delta >= చివరి_delta)
			ఆపు;
		చివరి_delta = delta;
	}

	తిరిగి PRIO_గమ్యం_NICE(prio - 1 + MAX_RT_PRIO);
}

స్థిర పూర్ణాంకం cpu_బరువు_nice_write_s64(నిర్మాణం cgroup_subsys_state *css,
				     నిర్మాణం cftype *cft, s64 nice)
{
	గుర్తులేని దీర్ఘ బరువు;
	పూర్ణాంకం idx, ret;

	ఒకవేళ (nice < MIN_NICE || nice > MAX_NICE)
		తిరిగి -ERANGE;

	idx = NICE_గమ్యం_PRIO(nice) - MAX_RT_PRIO;
	idx = array_సూచీ_nospec(idx, 40);
	బరువు = sched_prio_గమ్యం_బరువు[idx];

	ret = sched_సమూహం_సమితి_shares(css_tg(css), scale_ఎక్కించు(బరువు));
	ఒకవేళ (!ret)
		scx_సమూహం_సమితి_బరువు(css_tg(css),
				     sched_బరువు_గమ్యం_cgroup(బరువు));
	తిరిగి ret;
}
#అయితే_ముగింపు /* CONFIG_GROUP_SCHED_WEIGHT */

స్థిర శూన్య __maybe_unused cpu_period_quota_print(నిర్మాణం seq_file *sf,
						  దీర్ఘ period, దీర్ఘ quota)
{
	ఒకవేళ (quota < 0)
		seq_puts(sf, "max");
	లేకపోతే
		seq_printf(sf, "%ld", quota);

	seq_printf(sf, " %ld\n", period);
}

/* caller should put the current value in *@periodp before calling */
స్థిర పూర్ణాంకం __maybe_unused cpu_period_quota_parse(అక్షరం *buf, u64 *period_us_p,
						 u64 *quota_us_p)
{
	అక్షరం tok[21];	/* U64_MAX */

	ఒకవేళ (అక్షరశ్రేణి_చదువు(buf, "%20s %llu", tok, period_us_p) < 1)
		తిరిగి -EINVAL;

	ఒకవేళ (అక్షరశ్రేణి_చదువు(tok, "%llu", quota_us_p) < 1) {
		ఒకవేళ (!strcmp(tok, "max"))
			*quota_us_p = RUNTIME_INF;
		లేకపోతే
			తిరిగి -EINVAL;
	}

	తిరిగి 0;
}

#నిర్వచితమైతే CONFIG_సమూహం_SCHED_BANDWIDTH
స్థిర పూర్ణాంకం cpu_max_show(నిర్మాణం seq_file *sf, శూన్య *v)
{
	నిర్మాణం task_సమూహం *tg = css_tg(seq_css(sf));
	u64 period_us, quota_us;

	tg_bandwidth(tg, &period_us, &quota_us, ఖాళీ);
	cpu_period_quota_print(sf, period_us, quota_us);
	తిరిగి 0;
}

స్థిర ssize_t cpu_max_write(నిర్మాణం kernfs_open_file *యొక్క,
			     అక్షరం *buf, కొలత_t nbytes, loff_t off)
{
	నిర్మాణం task_సమూహం *tg = css_tg(యొక్క_css(యొక్క));
	u64 period_us, quota_us, వ్యవధి_us;
	పూర్ణాంకం ret;

	tg_bandwidth(tg, &period_us, ఖాళీ, &వ్యవధి_us);
	ret = cpu_period_quota_parse(buf, &period_us, &quota_us);
	ఒకవేళ (!ret)
		ret = tg_సమితి_bandwidth(tg, period_us, quota_us, వ్యవధి_us);
	తిరిగి ret ?: nbytes;
}
#అయితే_ముగింపు /* CONFIG_CFS_BANDWIDTH */

స్థిర నిర్మాణం cftype cpu_files[] = {
#నిర్వచితమైతే CONFIG_సమూహం_SCHED_బరువు
	{
		.name = "weight",
		.flags = CFTYPE_NOT_ON_మూలం,
		.read_u64 = cpu_బరువు_read_u64,
		.write_u64 = cpu_బరువు_write_u64,
	},
	{
		.name = "weight.nice",
		.flags = CFTYPE_NOT_ON_మూలం,
		.read_s64 = cpu_బరువు_nice_read_s64,
		.write_s64 = cpu_బరువు_nice_write_s64,
	},
	{
		.name = "idle",
		.flags = CFTYPE_NOT_ON_మూలం,
		.read_s64 = cpu_idle_read_s64,
		.write_s64 = cpu_idle_write_s64,
	},
#అయితే_ముగింపు
#నిర్వచితమైతే CONFIG_సమూహం_SCHED_BANDWIDTH
	{
		.name = "max",
		.flags = CFTYPE_NOT_ON_మూలం,
		.seq_show = cpu_max_show,
		.లిఖ = cpu_max_write,
	},
	{
		.name = "max.burst",
		.flags = CFTYPE_NOT_ON_మూలం,
		.read_u64 = cpu_వ్యవధి_read_u64,
		.write_u64 = cpu_వ్యవధి_write_u64,
	},
#అయితే_ముగింపు /* CONFIG_CFS_BANDWIDTH */
#నిర్వచితమైతే CONFIG_UCLAMP_TASK_సమూహం
	{
		.name = "uclamp.min",
		.flags = CFTYPE_NOT_ON_మూలం,
		.seq_show = cpu_uclamp_min_show,
		.లిఖ = cpu_uclamp_min_write,
	},
	{
		.name = "uclamp.max",
		.flags = CFTYPE_NOT_ON_మూలం,
		.seq_show = cpu_uclamp_max_show,
		.లిఖ = cpu_uclamp_max_write,
	},
#అయితే_ముగింపు /* CONFIG_UCLAMP_TASK_GROUP */
	{ }	/* terminate */
};

నిర్మాణం cgroup_subsys cpu_cgrp_subsys = {
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

#అయితే_ముగింపు /* CONFIG_CGROUP_SCHED */

శూన్య dump_cpu_task(పూర్ణాంకం cpu)
{
	ఒకవేళ (లోపల_hardirq() && cpu == smp_processor_id()) {
		నిర్మాణం pt_regs *regs;

		regs = get_irq_regs();
		ఒకవేళ (regs) {
			show_regs(regs);
			తిరిగి;
		}
	}

	ఒకవేళ (trigger_single_cpu_backtrace(cpu))
		తిరిగి;

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
స్థిరరాశి పూర్ణాంకం sched_prio_గమ్యం_బరువు[40] = {
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
స్థిరరాశి u32 sched_prio_గమ్యం_wmult[40] = {
 /* -20 */     48388,     59856,     76040,     92818,    118348,
 /* -15 */    147320,    184698,    229616,    287308,    360437,
 /* -10 */    449829,    563644,    704093,    875809,   1099582,
 /*  -5 */   1376151,   1717300,   2157191,   2708050,   3363326,
 /*   0 */   4194304,   5237765,   6557202,   8165337,  10153587,
 /*   5 */  12820798,  15790321,  19976592,  24970740,  31350126,
 /*  10 */  39045157,  49367440,  61356676,  76695844,  95443717,
 /*  15 */ 119304647, 148102320, 186737708, 238609294, 286331153,
};

శూన్య call_trace_sched_update_nr_running(నిర్మాణం rq *rq, పూర్ణాంకం లెక్క)
{
        trace_sched_update_nr_running_tp(rq, లెక్క);
}

#నిర్వచితమైతే CONFIG_SCHED_MM_CID
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
స్థిర శూన్య __mm_update_max_cids(నిర్మాణం mm_mm_cid *mc)
{
	గుర్తులేని పూర్ణాంకం opt_cids, max_cids;

	/* Calculate the new optimal constraint */
	opt_cids = min(mc->nr_cpus_allowed, mc->users);

	/* Adjust the maximum CIDs to +25% limited by the number of possible CPUs */
	max_cids = min(opt_cids + (opt_cids / 4), అంకె_possible_cpus());
	WRITE_ONCE(mc->max_cids, max_cids);
}

స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం mm_cid_calc_pcpu_thrs(నిర్మాణం mm_mm_cid *mc)
{
	గుర్తులేని పూర్ణాంకం opt_cids;

	opt_cids = min(mc->nr_cpus_allowed, mc->users);
	/* Has to be at least 1 because 0 indicates PCPU mode off */
	తిరిగి max(min(opt_cids - opt_cids / 4, అంకె_possible_cpus() / 2), 1);
}

స్థిర bool mm_update_max_cids(నిర్మాణం mm_struct *mm)
{
	నిర్మాణం mm_mm_cid *mc = &mm->mm_cid;
	bool percpu = cid_on_cpu(mc->బహుళకం);

	lockdep_assert_held(&mm->mm_cid.తాళం);

	/* Clear deferred mode switch flag. A change is handled by the caller */
	mc->update_deferred = false;
	__mm_update_max_cids(mc);

	/* Check whether owner mode must be changed */
	ఒకవేళ (!percpu) {
		/* Enable per CPU mode when the number of users is above max_cids */
		ఒకవేళ (mc->users > mc->max_cids)
			mc->pcpu_thrs = mm_cid_calc_pcpu_thrs(mc);
	} లేకపోతే {
		/* Switch back to per task if user count under threshold */
		ఒకవేళ (mc->users < mc->pcpu_thrs)
			mc->pcpu_thrs = 0;
	}

	/* Mode change required? */
	ఒకవేళ (percpu == !!mc->pcpu_thrs)
		తిరిగి false;

	/* Flip the mode and set the transition flag to bridge the transfer */
	WRITE_ONCE(mc->బహుళకం, mc->బహుళకం ^ (MM_CID_TRANSIT | MM_CID_ONCPU));
	/*
	 * Order the store against the subsequent fixups so that
	 * acquire(rq::lock) cannot be reordered by the CPU before the
	 * store.
	 */
	smp_mb();
	తిరిగి true;
}

స్థిర అంతఃవరుస శూన్య mm_update_cpus_allowed(నిర్మాణం mm_struct *mm, స్థిరరాశి నిర్మాణం cpumask *affmsk)
{
	నిర్మాణం cpumask *mm_allowed;
	నిర్మాణం mm_mm_cid *mc;
	గుర్తులేని పూర్ణాంకం బరువు;

	ఒకవేళ (!mm || !READ_ONCE(mm->mm_cid.users))
		తిరిగి;
	/*
	 * mm::mm_cid::mm_cpus_allowed is the superset of each threads
	 * allowed CPUs mask which means it can only grow.
	 */
	mc = &mm->mm_cid;
	guard(raw_spinlock)(&mc->తాళం);
	mm_allowed = mm_cpus_allowed(mm);
	బరువు = cpumask_weighted_or(mm_allowed, mm_allowed, affmsk);
	ఒకవేళ (బరువు == mc->nr_cpus_allowed)
		తిరిగి;

	WRITE_ONCE(mc->nr_cpus_allowed, బరువు);
	__mm_update_max_cids(mc);
	ఒకవేళ (!cid_on_cpu(mc->బహుళకం))
		తిరిగి;

	/* Adjust the threshold to the wider set */
	mc->pcpu_thrs = mm_cid_calc_pcpu_thrs(mc);
	/* Switch back to per task mode? */
	ఒకవేళ (mc->users >= mc->pcpu_thrs)
		తిరిగి;

	/* Don't queue twice */
	ఒకవేళ (mc->update_deferred)
		తిరిగి;

	/* Queue the irq work, which schedules the real work */
	mc->update_deferred = true;
	irq_work_వరుస(&mc->irq_work);
}

స్థిర అంతఃవరుస శూన్య mm_cid_complete_transit(నిర్మాణం mm_struct *mm, గుర్తులేని పూర్ణాంకం బహుళకం)
{
	/*
	 * Ensure that the store removing the TRANSIT bit cannot be
	 * reordered by the CPU before the fixups have been completed.
	 */
	smp_mb();
	WRITE_ONCE(mm->mm_cid.బహుళకం, బహుళకం);
}

స్థిర అంతఃవరుస శూన్య mm_cid_transit_గమ్యం_task(నిర్మాణం task_struct *t, నిర్మాణం mm_cid_pcpu *pcp)
{
	ఒకవేళ (cid_on_cpu(t->mm_cid.cid)) {
		గుర్తులేని పూర్ణాంకం cid = cpu_cid_గమ్యం_cid(t->mm_cid.cid);

		t->mm_cid.cid = cid_గమ్యం_transit_cid(cid);
		pcp->cid = t->mm_cid.cid;
	}
}

స్థిర శూన్య mm_cid_fixup_cpus_గమ్యం_tasks(నిర్మాణం mm_struct *mm)
{
	గుర్తులేని పూర్ణాంకం cpu;

	/* Walk the CPUs and fixup all stale CIDs */
	for_each_possible_cpu(cpu) {
		నిర్మాణం mm_cid_pcpu *pcp = చొప్పున_cpu_ptr(mm->mm_cid.pcpu, cpu);
		నిర్మాణం rq *rq = cpu_rq(cpu);

		/* Remote access to mm::mm_cid::pcpu requires rq_lock */
		guard(rq_తాళం_irq)(rq);
		/* Is the CID still owned by the CPU? */
		ఒకవేళ (cid_on_cpu(pcp->cid)) {
			/*
			 * If rq->curr has @mm, transfer it with the
			 * transition bit set. Otherwise drop it.
			 */
			ఒకవేళ (rq->curr->mm == mm && rq->curr->mm_cid.చురుకైన)
				mm_cid_transit_గమ్యం_task(rq->curr, pcp);
			లేకపోతే
				mm_drop_cid_on_cpu(mm, pcp);

		} లేకపోతే ఒకవేళ (rq->curr->mm == mm && rq->curr->mm_cid.చురుకైన) {
			గుర్తులేని పూర్ణాంకం cid = rq->curr->mm_cid.cid;

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
			ఒకవేళ (cid != MM_CID_UNSET && !cid_లోపల_transit(cid)) {
				cid = cid_గమ్యం_transit_cid(cid);
				rq->curr->mm_cid.cid = cid;
				pcp->cid = cid;
			}
		}
	}
	mm_cid_complete_transit(mm, 0);
}

స్థిర అంతఃవరుస శూన్య mm_cid_transit_గమ్యం_cpu(నిర్మాణం task_struct *t, నిర్మాణం mm_cid_pcpu *pcp)
{
	ఒకవేళ (cid_on_task(t->mm_cid.cid)) {
		t->mm_cid.cid = cid_గమ్యం_transit_cid(t->mm_cid.cid);
		pcp->cid = t->mm_cid.cid;
	}
}

స్థిర శూన్య mm_cid_fixup_task_గమ్యం_cpu(నిర్మాణం task_struct *t, నిర్మాణం mm_struct *mm)
{
	/* Remote access to mm::mm_cid::pcpu requires rq_lock */
	guard(task_rq_తాళం_2)(t);
	ఒకవేళ (cid_on_task(t->mm_cid.cid)) {
		/* If running on the CPU, put the CID in transit mode, otherwise drop it */
		ఒకవేళ (task_rq(t)->curr == t)
			mm_cid_transit_గమ్యం_cpu(t, చొప్పున_cpu_ptr(mm->mm_cid.pcpu, task_cpu(t)));
		లేకపోతే
			mm_unset_cid_on_task(t);
	}
}

స్థిర శూన్య mm_cid_fixup_tasks_గమ్యం_cpus(శూన్య)
{
	నిర్మాణం mm_struct *mm = current->mm;
	నిర్మాణం task_struct *t;

	lockdep_assert_held(&mm->mm_cid.mutex);

	hlist_for_each_నమోదు(t, &mm->mm_cid.user_list, mm_cid.కణుపు) {
		/* Current has already transferred before invoking the fixup. */
		ఒకవేళ (t != current)
			mm_cid_fixup_task_గమ్యం_cpu(t, mm);
	}

	mm_cid_complete_transit(mm, MM_CID_ONCPU);
}

స్థిర bool sched_mm_cid_కూడు_user(నిర్మాణం task_struct *t, నిర్మాణం mm_struct *mm)
{
	lockdep_assert_held(&mm->mm_cid.తాళం);

	t->mm_cid.చురుకైన = 1;
	hlist_కూడు_తల(&t->mm_cid.కణుపు, &mm->mm_cid.user_list);
	mm->mm_cid.users++;
	తిరిగి mm_update_max_cids(mm);
}

స్థిర శూన్య sched_mm_cid_fork(నిర్మాణం task_struct *t)
{
	నిర్మాణం mm_struct *mm = t->mm;
	bool percpu;

	ఒకవేళ (!mm)
		తిరిగి;

	WARN_ON_ONCE(t->mm_cid.cid != MM_CID_UNSET);

	guard(mutex)(&mm->mm_cid.mutex);
	scoped_guard(raw_spinlock_irq, &mm->mm_cid.తాళం) {
		నిర్మాణం mm_cid_pcpu *pcp = this_cpu_ptr(mm->mm_cid.pcpu);

		/* First user ? */
		ఒకవేళ (!mm->mm_cid.users) {
			sched_mm_cid_కూడు_user(t, mm);
			t->mm_cid.cid = mm_get_cid(mm);
			/* Required for execve() */
			pcp->cid = t->mm_cid.cid;
			తిరిగి;
		}

		ఒకవేళ (!sched_mm_cid_కూడు_user(t, mm)) {
			ఒకవేళ (!cid_on_cpu(mm->mm_cid.బహుళకం))
				t->mm_cid.cid = mm_get_cid(mm);
			తిరిగి;
		}

		/* Handle the mode change and transfer current's CID */
		percpu = cid_on_cpu(mm->mm_cid.బహుళకం);
		ఒకవేళ (!percpu)
			mm_cid_transit_గమ్యం_task(current, pcp);
		లేకపోతే
			mm_cid_transit_గమ్యం_cpu(current, pcp);
	}

	ఒకవేళ (percpu) {
		mm_cid_fixup_tasks_గమ్యం_cpus();
	} లేకపోతే {
		mm_cid_fixup_cpus_గమ్యం_tasks(mm);
		t->mm_cid.cid = mm_get_cid(mm);
	}
}

స్థిర bool sched_mm_cid_remove_user(నిర్మాణం task_struct *t)
{
	lockdep_assert_held(&t->mm->mm_cid.తాళం);

	t->mm_cid.చురుకైన = 0;
	/* Clear the transition bit */
	t->mm_cid.cid = cid_from_transit_cid(t->mm_cid.cid);
	mm_unset_cid_on_task(t);
	hlist_del_init(&t->mm_cid.కణుపు);
	t->mm->mm_cid.users--;
	తిరిగి mm_update_max_cids(t->mm);
}

స్థిర bool __sched_mm_cid_exit(నిర్మాణం task_struct *t)
{
	నిర్మాణం mm_struct *mm = t->mm;

	ఒకవేళ (!sched_mm_cid_remove_user(t))
		తిరిగి false;
	/*
	 * Contrary to fork() this only deals with a switch back to per
	 * task mode either because the above decreased users or an
	 * affinity change increased the number of allowed CPUs and the
	 * deferred fixup did not run yet.
	 */
	ఒకవేళ (WARN_ON_ONCE(cid_on_cpu(mm->mm_cid.బహుళకం)))
		తిరిగి false;
	/*
	 * A failed fork(2) cleanup never gets here, so @current must have
	 * the same MM as @t. That's true for exit() and the failed
	 * pthread_create() cleanup case.
	 */
	ఒకవేళ (WARN_ON_ONCE(current->mm != mm))
		తిరిగి false;
	తిరిగి true;
}

/*
 * When a task exits, the MM CID held by the task is not longer required as
 * the task cannot return to user space.
 */
శూన్య sched_mm_cid_exit(నిర్మాణం task_struct *t)
{
	నిర్మాణం mm_struct *mm = t->mm;

	ఒకవేళ (!mm || !t->mm_cid.చురుకైన)
		తిరిగి;
	/*
	 * Ensure that only one instance is doing MM CID operations within
	 * a MM. The common case is uncontended. The rare fixup case adds
	 * some overhead.
	 */
	scoped_guard(mutex, &mm->mm_cid.mutex) {
		/* mm_cid::mutex is sufficient to protect mm_cid::users */
		ఒకవేళ (likely(mm->mm_cid.users > 1)) {
			scoped_guard(raw_spinlock_irq, &mm->mm_cid.తాళం) {
				ఒకవేళ (!__sched_mm_cid_exit(t))
					తిరిగి;
				/*
				 * Mode change. The task has the CID unset
				 * already and dealt with an eventually set
				 * TRANSIT bit. If the CID is owned by the CPU
				 * then drop it.
				 */
				mm_drop_cid_on_cpu(mm, this_cpu_ptr(mm->mm_cid.pcpu));
			}
			mm_cid_fixup_cpus_గమ్యం_tasks(mm);
			తిరిగి;
		}
		/* Last user */
		scoped_guard(raw_spinlock_irq, &mm->mm_cid.తాళం) {
			/* Required across execve() */
			ఒకవేళ (t == current)
				mm_cid_transit_గమ్యం_task(t, this_cpu_ptr(mm->mm_cid.pcpu));
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
శూన్య sched_mm_cid_before_execve(నిర్మాణం task_struct *t)
{
	sched_mm_cid_exit(t);
}

/* Reactivate MM CID after execve() */
శూన్య sched_mm_cid_after_execve(నిర్మాణం task_struct *t)
{
	ఒకవేళ (t->mm)
		sched_mm_cid_fork(t);
}

స్థిర శూన్య mm_cid_work_fn(నిర్మాణం work_struct *work)
{
	నిర్మాణం mm_struct *mm = container_యొక్క(work, నిర్మాణం mm_struct, mm_cid.work);

	guard(mutex)(&mm->mm_cid.mutex);
	/* Did the last user task exit already? */
	ఒకవేళ (!mm->mm_cid.users)
		తిరిగి;

	scoped_guard(raw_spinlock_irq, &mm->mm_cid.తాళం) {
		/* Have fork() or exit() handled it already? */
		ఒకవేళ (!mm->mm_cid.update_deferred)
			తిరిగి;
		/* This clears mm_cid::update_deferred */
		ఒకవేళ (!mm_update_max_cids(mm))
			తిరిగి;
		/* Affinity changes can only switch back to task mode */
		ఒకవేళ (WARN_ON_ONCE(cid_on_cpu(mm->mm_cid.బహుళకం)))
			తిరిగి;
	}
	mm_cid_fixup_cpus_గమ్యం_tasks(mm);
}

స్థిర శూన్య mm_cid_irq_work(నిర్మాణం irq_work *work)
{
	నిర్మాణం mm_struct *mm = container_యొక్క(work, నిర్మాణం mm_struct, mm_cid.irq_work);

	/*
	 * Needs to be unconditional because mm_cid::lock cannot be held
	 * when scheduling work as mm_update_cpus_allowed() nests inside
	 * rq::lock and schedule_work() might end up in wakeup...
	 */
	కాలపట్టిక_work(&mm->mm_cid.work);
}

శూన్య mm_init_cid(నిర్మాణం mm_struct *mm, నిర్మాణం task_struct *p)
{
	mm->mm_cid.max_cids = 0;
	mm->mm_cid.బహుళకం = 0;
	mm->mm_cid.nr_cpus_allowed = p->nr_cpus_allowed;
	mm->mm_cid.users = 0;
	mm->mm_cid.pcpu_thrs = 0;
	mm->mm_cid.update_deferred = 0;
	raw_spin_తాళం_init(&mm->mm_cid.తాళం);
	mutex_init(&mm->mm_cid.mutex);
	mm->mm_cid.irq_work = IRQ_WORK_INIT_HARD(mm_cid_irq_work);
	INIT_WORK(&mm->mm_cid.work, mm_cid_work_fn);
	INIT_HLIST_తల(&mm->mm_cid.user_list);
	cpumask_copy(mm_cpus_allowed(mm), &p->cpus_ముసుగు);
	bitmap_zero(mm_cidmask(mm), అంకె_possible_cpus());
}
#లేకపోతే /* CONFIG_SCHED_MM_CID */
స్థిర అంతఃవరుస శూన్య mm_update_cpus_allowed(నిర్మాణం mm_struct *mm, స్థిరరాశి నిర్మాణం cpumask *affmsk) { }
స్థిర అంతఃవరుస శూన్య sched_mm_cid_fork(నిర్మాణం task_struct *t) { }
#అయితే_ముగింపు /* !CONFIG_SCHED_MM_CID */

స్థిర DEFINE_చొప్పున_CPU(నిర్మాణం sched_change_ctx, sched_change_ctx);

నిర్మాణం sched_change_ctx *sched_change_begin(నిర్మాణం task_struct *p, గుర్తులేని పూర్ణాంకం flags)
{
	నిర్మాణం sched_change_ctx *ctx = this_cpu_ptr(&sched_change_ctx);
	నిర్మాణం rq *rq = task_rq(p);

	/*
	 * Must exclusively use matched flags since this is both dequeue and
	 * enqueue.
	 */
	WARN_ON_ONCE(flags & 0xFFFF0000);

	lockdep_assert_rq_held(rq);

	ఒకవేళ (!(flags & వరుస_నుండి_తీయి_NOCLOCK)) {
		update_rq_clock(rq);
		flags |= వరుస_నుండి_తీయి_NOCLOCK;
	}

	ఒకవేళ ((flags & వరుస_నుండి_తీయి_CLASS) && p->sched_class->switching_from)
		p->sched_class->switching_from(rq, p);

	*ctx = (నిర్మాణం sched_change_ctx){
		.p = p,
		.class = p->sched_class,
		.flags = flags,
		.queued = task_on_rq_queued(p),
		.running = task_current_donor(rq, p),
	};

	ఒకవేళ (!(flags & వరుస_నుండి_తీయి_CLASS)) {
		ఒకవేళ (p->sched_class->get_prio)
			ctx->prio = p->sched_class->get_prio(rq, p);
		లేకపోతే
			ctx->prio = p->prio;
	}

	ఒకవేళ (ctx->queued)
		వరుస_నుండి_తీయి_task(rq, p, flags);
	ఒకవేళ (ctx->running)
		put_prev_task(rq, p);

	ఒకవేళ ((flags & వరుస_నుండి_తీయి_CLASS) && p->sched_class->switched_from)
		p->sched_class->switched_from(rq, p);

	తిరిగి ctx;
}

శూన్య sched_change_చివర(నిర్మాణం sched_change_ctx *ctx)
{
	నిర్మాణం task_struct *p = ctx->p;
	నిర్మాణం rq *rq = task_rq(p);

	lockdep_assert_rq_held(rq);

	/*
	 * Changing class without *QUEUE_CLASS is bad.
	 */
	WARN_ON_ONCE(p->sched_class != ctx->class && !(ctx->flags & వరుసలో_చేర్చు_CLASS));

	ఒకవేళ ((ctx->flags & వరుసలో_చేర్చు_CLASS) && p->sched_class->switching_గమ్యం)
		p->sched_class->switching_గమ్యం(rq, p);

	ఒకవేళ (ctx->queued)
		వరుసలో_చేర్చు_task(rq, p, ctx->flags);
	ఒకవేళ (ctx->running)
		సమితి_తదుపరి_task(rq, p);

	ఒకవేళ (ctx->flags & వరుసలో_చేర్చు_CLASS) {
		ఒకవేళ (p->sched_class->switched_గమ్యం)
			p->sched_class->switched_గమ్యం(rq, p);

		/*
		 * Check if the task can preempt the current in case the class
		 * was upgraded. Note that none of the switch*_from() methods
		 * know the new class and none of the switch*_to() methods know
		 * the old class so we must resched here.
		 */
		ఒకవేళ (ctx->queued)
			wakeup_preempt(rq, p, 0);

		ఒకవేళ (ctx->running) {
			/*
			 * If this was a degradation in class; make sure to
			 * reschedule.
			 */
			ఒకవేళ (sched_class_above(ctx->class, p->sched_class))
				resched_curr(rq);
		}
	} లేకపోతే {
		p->sched_class->prio_మారింది(rq, p, ctx->prio);
	}
}

#నిర్వచితమైతే CONFIG_PREFERRED_CPU
స్థిర DEFINE_చొప్పున_CPU(నిర్మాణం cpu_stop_work, npc_నెట్టు_task_work);

స్థిర పూర్ణాంకం sched_non_preferred_cpu_నెట్టు_stop(శూన్య *arg)
{
	నిర్మాణం task_struct *p = arg;
	నిర్మాణం rq *rq = this_rq();
	నిర్మాణం rq_flags rf;
	పూర్ణాంకం cpu;

	ఒకవేళ (cpu_preferred(rq->cpu)) {
		scoped_guard(rq_తాళం_irqsave, rq)
			rq->npc_నెట్టు_work_pending = false;
		put_task_struct(p);
		తిరిగి 0;
	}

	scoped_guard (raw_spinlock_irq, &p->pi_తాళం) {
		/*
		 * select_fallback_rq() may acquire the rq lock in case of
		 * fallback. So call it before grabbing rq lock. If the task
		 * migrates to another CPU before the rq lock is acquired,
		 * subsequent validation of task's current rq will help to
		 * safely bail out.
		 */
		cpu = select_fallback_rq(rq->cpu, p);
		context_unsafe_alias(rq);
		rq_తాళం(rq, &rf);
		rq->npc_నెట్టు_work_pending = false;
		update_rq_clock(rq);

		ఒకవేళ (task_rq(p) == rq && task_on_rq_queued(p)) {
			నిర్మాణం rq *dest_rq = __migrate_task(rq, &rf, p, cpu);

			ఒకవేళ (rq != dest_rq)
				schedstat_inc(p->stats.nr_migrations_cpu_non_preferred);
			rq = dest_rq;
		}
		rq_unlock(rq, &rf);
	}

	put_task_struct(p);
	తిరిగి 0;
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
శూన్య sched_నెట్టు_current_non_preferred_cpu(నిర్మాణం rq *rq)
{
	నిర్మాణం task_struct *నెట్టు_task = rq->curr;

	scoped_guard(rq_తాళం, rq) {
		/* Push the task if its explicit affinity allows */
		ఒకవేళ (!task_can_migrate_గమ్యం_preferred(నెట్టు_task, rq->cpu))
			తిరిగి;

		/* There is already a stopper thread. Don't race with it. */
		ఒకవేళ (rq->npc_నెట్టు_work_pending)
			తిరిగి;

		ఒకవేళ (is_migration_disabled(నెట్టు_task))
			తిరిగి;

		rq->npc_నెట్టు_work_pending = true;
	}

	/* sched_tick runs with interrupts disabled. */
	get_task_struct(నెట్టు_task);
	stop_one_cpu_nowait(rq->cpu, sched_non_preferred_cpu_నెట్టు_stop,
			    నెట్టు_task, this_cpu_ptr(&npc_నెట్టు_task_work));
}
#అయితే_ముగింపు
