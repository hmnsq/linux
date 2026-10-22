// SPDX-License-Identifier: GPL-2.0
/*
 * Real-Time Scheduling Class (mapped to the SCHED_FIFO and SCHED_RR
 * policies)
 */

#include "sched.h"
#include "pelt.h"

පූර්ණ_සංඛ්‍යා sched_rr_timeslice = RR_TIMESLICE;
/* More than 4 hours if BW_SHIFT equals 20. */
static const u64 max_rt_runtime = MAX_BW;

/*
 * period over which we measure -rt task CPU usage in us.
 * default: 1s
 */
පූර්ණ_සංඛ්‍යා sysctl_sched_rt_period = 1000000;

/*
 * part of the period that we allow rt tasks to run in us.
 * default: 1s
 */
පූර්ණ_සංඛ්‍යා sysctl_sched_rt_runtime = 1000000;

#ifdef CONFIG_SYSCTL
static පූර්ණ_සංඛ්‍යා sysctl_sched_rr_timeslice = (MSEC_PER_SEC * RR_TIMESLICE) / HZ;
static පූර්ණ_සංඛ්‍යා sched_rt_handler(const sa_nracanaa ctl_table *table, පූර්ණ_සංඛ්‍යා write, void *buffer,
		size_t *lenp, loff_t *ppos);
static පූර්ණ_සංඛ්‍යා sched_rr_handler(const sa_nracanaa ctl_table *table, පූර්ණ_සංඛ්‍යා write, void *buffer,
		size_t *lenp, loff_t *ppos);
static const sa_nracanaa ctl_table sched_rt_sysctls[] = {
	{
		.procname       = "sched_rt_period_us",
		.data           = &sysctl_sched_rt_period,
		.maxlen         = parimaa_na(පූර්ණ_සංඛ්‍යා),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_ONE,
		.extra2         = SYSCTL_INT_MAX,
	},
	{
		.procname       = "sched_rt_runtime_us",
		.data           = &sysctl_sched_rt_runtime,
		.maxlen         = parimaa_na(පූර්ණ_සංඛ්‍යා),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_NEG_ONE,
		.extra2         = (void *)&sysctl_sched_rt_period,
	},
	{
		.procname       = "sched_rr_timeslice_ms",
		.data           = &sysctl_sched_rr_timeslice,
		.maxlen         = parimaa_na(පූර්ණ_සංඛ්‍යා),
		.mode           = 0644,
		.proc_handler   = sched_rr_handler,
	},
};

static පූර්ණ_සංඛ්‍යා __init sched_rt_sysctl_init(void)
{
	register_sysctl_init("kernel", sched_rt_sysctls);
	ආපසු 0;
}
late_initcall(sched_rt_sysctl_init);
#endif /* CONFIG_SYSCTL */

void init_rt_rq(sa_nracanaa rt_rq *rt_rq)
{
	sa_nracanaa rt_prio_array *array;
	පූර්ණ_සංඛ්‍යා i;

	array = &rt_rq->active;
	සඳහා (i = 0; i < MAX_RT_PRIO; i++) {
		INIT_LIST_HEAD(array->queue + i);
		__clear_bit(i, array->bitmap);
	}
	/* delimiter for bitsearch: */
	__set_bit(MAX_RT_PRIO, array->bitmap);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->highest_prio.next = MAX_RT_PRIO-1;
	rt_rq->overloaded = 0;
	plist_head_init(&rt_rq->pushable_tasks);
	/* We start is dequeued state, because no RT tasks are queued */
	rt_rq->rt_queued = 0;

#ifdef CONFIG_RT_GROUP_SCHED
	rt_rq->rt_time = 0;
	rt_rq->rt_throttled = 0;
	rt_rq->rt_runtime = 0;
	raw_spin_lock_init(&rt_rq->rt_runtime_lock);
	rt_rq->tg = &root_task_group;
#endif
}

#ifdef CONFIG_RT_GROUP_SCHED

static පූර්ණ_සංඛ්‍යා do_sched_rt_period_timer(sa_nracanaa rt_bandwidth *rt_b, පූර්ණ_සංඛ්‍යා overrun);

static ga_nanaa hrtimer_restart sched_rt_period_timer(sa_nracanaa hrtimer *timer)
{
	sa_nracanaa rt_bandwidth *rt_b =
		container_of(timer, sa_nracanaa rt_bandwidth, rt_period_timer);
	පූර්ණ_සංඛ්‍යා idle = 0;
	පූර්ණ_සංඛ්‍යා overrun;

	raw_spin_lock(&rt_b->rt_runtime_lock);
	සඳහා (;;) {
		overrun = hrtimer_forward_now(timer, rt_b->rt_period);
		නම් (!overrun)
			bha_nga;

		raw_spin_unlock(&rt_b->rt_runtime_lock);
		idle = do_sched_rt_period_timer(rt_b, overrun);
		raw_spin_lock(&rt_b->rt_runtime_lock);
	}
	නම් (idle)
		rt_b->rt_period_active = 0;
	raw_spin_unlock(&rt_b->rt_runtime_lock);

	ආපසු idle ? HRTIMER_NORESTART : HRTIMER_RESTART;
}

void init_rt_bandwidth(sa_nracanaa rt_bandwidth *rt_b, u64 period, u64 runtime)
{
	rt_b->rt_period = ns_to_ktime(period);
	rt_b->rt_runtime = runtime;

	raw_spin_lock_init(&rt_b->rt_runtime_lock);

	hrtimer_setup(&rt_b->rt_period_timer, sched_rt_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_MODE_REL_HARD);
}

static anta_hstha void do_start_rt_bandwidth(sa_nracanaa rt_bandwidth *rt_b)
{
	raw_spin_lock(&rt_b->rt_runtime_lock);
	නම් (!rt_b->rt_period_active) {
		rt_b->rt_period_active = 1;
		/*
		 * SCHED_DEADLINE updates the bandwidth, as a run away
		 * RT task with a DL task could hog a CPU. But DL does
		 * not reset the period. If a deadline task was running
		 * without an RT task running, it can cause RT tasks to
		 * throttle when they start up. Kick the timer right away
		 * to update the period.
		 */
		hrtimer_forward_now(&rt_b->rt_period_timer, ns_to_ktime(0));
		hrtimer_start_expires(&rt_b->rt_period_timer,
				      HRTIMER_MODE_ABS_PINNED_HARD);
	}
	raw_spin_unlock(&rt_b->rt_runtime_lock);
}

static void start_rt_bandwidth(sa_nracanaa rt_bandwidth *rt_b)
{
	නම් (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF)
		ආපසු;

	do_start_rt_bandwidth(rt_b);
}

static void destroy_rt_bandwidth(sa_nracanaa rt_bandwidth *rt_b)
{
	hrtimer_cancel(&rt_b->rt_period_timer);
}

#define rt_entity_is_task(rt_se) (!(rt_se)->my_q)

static anta_hstha sa_nracanaa task_struct *rt_task_of(sa_nracanaa sched_rt_entity *rt_se)
{
	WARN_ON_ONCE(!rt_entity_is_task(rt_se));

	ආපසු container_of(rt_se, sa_nracanaa task_struct, rt);
}

static anta_hstha sa_nracanaa rq *rq_of_rt_rq(sa_nracanaa rt_rq *rt_rq)
{
	/* Cannot fold with non-CONFIG_RT_GROUP_SCHED version, layout */
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &root_task_group);
	ආපසු rt_rq->rq;
}

static anta_hstha sa_nracanaa rt_rq *rt_rq_of_se(sa_nracanaa sched_rt_entity *rt_se)
{
	WARN_ON(!rt_group_sched_enabled() && rt_se->rt_rq->tg != &root_task_group);
	ආපසු rt_se->rt_rq;
}

static anta_hstha sa_nracanaa rq *rq_of_rt_se(sa_nracanaa sched_rt_entity *rt_se)
{
	sa_nracanaa rt_rq *rt_rq = rt_se->rt_rq;

	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &root_task_group);
	ආපසු rt_rq->rq;
}

void unregister_rt_sched_group(sa_nracanaa task_group *tg)
{
	නම් (!rt_group_sched_enabled())
		ආපසු;

	නම් (tg->rt_se)
		destroy_rt_bandwidth(&tg->rt_bandwidth);
}

void free_rt_sched_group(sa_nracanaa task_group *tg)
{
	පූර්ණ_සංඛ්‍යා i;

	නම් (!rt_group_sched_enabled())
		ආපසු;

	for_each_possible_cpu(i) {
		නම් (tg->rt_rq)
			kfree(tg->rt_rq[i]);
		නම් (tg->rt_se)
			kfree(tg->rt_se[i]);
	}

	kfree(tg->rt_rq);
	kfree(tg->rt_se);
}

void init_tg_rt_entry(sa_nracanaa task_group *tg, sa_nracanaa rt_rq *rt_rq,
		sa_nracanaa sched_rt_entity *rt_se, පූර්ණ_සංඛ්‍යා cpu,
		sa_nracanaa sched_rt_entity *parent)
{
	sa_nracanaa rq *rq = cpu_rq(cpu);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->rt_nr_boosted = 0;
	rt_rq->rq = rq;
	rt_rq->tg = tg;

	tg->rt_rq[cpu] = rt_rq;
	tg->rt_se[cpu] = rt_se;

	නම් (!rt_se)
		ආපසු;

	නම් (!parent)
		rt_se->rt_rq = &rq->rt;
	නැතහොත්
		rt_se->rt_rq = parent->my_q;

	rt_se->my_q = rt_rq;
	rt_se->parent = parent;
	INIT_LIST_HEAD(&rt_se->run_list);
}

පූර්ණ_සංඛ්‍යා alloc_rt_sched_group(sa_nracanaa task_group *tg, sa_nracanaa task_group *parent)
{
	sa_nracanaa rt_rq *rt_rq;
	sa_nracanaa sched_rt_entity *rt_se;
	පූර්ණ_සංඛ්‍යා i;

	නම් (!rt_group_sched_enabled())
		ආපසු 1;

	tg->rt_rq = kzalloc_objs(rt_rq, nr_cpu_ids);
	නම් (!tg->rt_rq)
		goto err;
	tg->rt_se = kzalloc_objs(rt_se, nr_cpu_ids);
	නම් (!tg->rt_se)
		goto err;

	init_rt_bandwidth(&tg->rt_bandwidth, ktime_to_ns(global_rt_period()), 0);

	for_each_possible_cpu(i) {
		rt_rq = kzalloc_node(parimaa_na(sa_nracanaa rt_rq),
				     GFP_KERNEL, cpu_to_node(i));
		නම් (!rt_rq)
			goto err;

		rt_se = kzalloc_node(parimaa_na(sa_nracanaa sched_rt_entity),
				     GFP_KERNEL, cpu_to_node(i));
		නම් (!rt_se)
			goto err_free_rq;

		init_rt_rq(rt_rq);
		rt_rq->rt_runtime = tg->rt_bandwidth.rt_runtime;
		init_tg_rt_entry(tg, rt_rq, rt_se, i, parent->rt_se[i]);
	}

	ආපසු 1;

err_free_rq:
	kfree(rt_rq);
err:
	ආපසු 0;
}

#නැතහොත් /* !CONFIG_RT_GROUP_SCHED: */

#define rt_entity_is_task(rt_se) (1)

static anta_hstha sa_nracanaa task_struct *rt_task_of(sa_nracanaa sched_rt_entity *rt_se)
{
	ආපසු container_of(rt_se, sa_nracanaa task_struct, rt);
}

static anta_hstha sa_nracanaa rq *rq_of_rt_rq(sa_nracanaa rt_rq *rt_rq)
{
	ආපසු container_of(rt_rq, sa_nracanaa rq, rt);
}

static anta_hstha sa_nracanaa rq *rq_of_rt_se(sa_nracanaa sched_rt_entity *rt_se)
{
	sa_nracanaa task_struct *p = rt_task_of(rt_se);

	ආපසු task_rq(p);
}

static anta_hstha sa_nracanaa rt_rq *rt_rq_of_se(sa_nracanaa sched_rt_entity *rt_se)
{
	sa_nracanaa rq *rq = rq_of_rt_se(rt_se);

	ආපසු &rq->rt;
}

void unregister_rt_sched_group(sa_nracanaa task_group *tg) { }

void free_rt_sched_group(sa_nracanaa task_group *tg) { }

පූර්ණ_සංඛ්‍යා alloc_rt_sched_group(sa_nracanaa task_group *tg, sa_nracanaa task_group *parent)
{
	ආපසු 1;
}
#endif /* !CONFIG_RT_GROUP_SCHED */

static anta_hstha bool need_pull_rt_task(sa_nracanaa rq *rq, sa_nracanaa task_struct *prev)
{
	/* Try to pull RT tasks here if we lower this rq's prio */
	ආපසු rq->online && rq->rt.highest_prio.curr > prev->prio;
}

static anta_hstha පූර්ණ_සංඛ්‍යා rt_overloaded(sa_nracanaa rq *rq)
{
	ආපසු atomic_read(&rq->rd->rto_count);
}

static anta_hstha void rt_set_overload(sa_nracanaa rq *rq)
{
	නම් (!rq->online)
		ආපසු;

	cpumask_set_cpu(rq->cpu, rq->rd->rto_mask);
	/*
	 * Make sure the mask is visible before we set
	 * the overload count. That is checked to determine
	 * if we should look at the mask. It would be a shame
	 * if we looked at the mask, but the mask was not
	 * updated yet.
	 *
	 * Matched by the barrier in pull_rt_task().
	 */
	smp_wmb();
	atomic_inc(&rq->rd->rto_count);
}

static anta_hstha void rt_clear_overload(sa_nracanaa rq *rq)
{
	නම් (!rq->online)
		ආපසු;

	/* the order here really doesn't matter */
	atomic_dec(&rq->rd->rto_count);
	cpumask_clear_cpu(rq->cpu, rq->rd->rto_mask);
}

static anta_hstha පූර්ණ_සංඛ්‍යා has_pushable_tasks(sa_nracanaa rq *rq)
{
	ආපසු !plist_head_empty(&rq->rt.pushable_tasks);
}

static DEFINE_PER_CPU(sa_nracanaa balance_callback, rt_push_head);
static DEFINE_PER_CPU(sa_nracanaa balance_callback, rt_pull_head);

static void push_rt_tasks(sa_nracanaa rq *);
static void pull_rt_task(sa_nracanaa rq *);

static anta_hstha void rt_queue_push_tasks(sa_nracanaa rq *rq)
{
	නම් (!has_pushable_tasks(rq))
		ආපසු;

	queue_balance_callback(rq, &per_cpu(rt_push_head, rq->cpu), push_rt_tasks);
}

static anta_hstha void rt_queue_pull_task(sa_nracanaa rq *rq)
{
	queue_balance_callback(rq, &per_cpu(rt_pull_head, rq->cpu), pull_rt_task);
}

static void enqueue_pushable_task(sa_nracanaa rq *rq, sa_nracanaa task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);
	plist_node_init(&p->pushable_tasks, p->prio);
	plist_add(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the highest prio pushable task */
	නම් (p->prio < rq->rt.highest_prio.next)
		rq->rt.highest_prio.next = p->prio;

	නම් (!rq->rt.overloaded) {
		rt_set_overload(rq);
		rq->rt.overloaded = 1;
	}
}

static void dequeue_pushable_task(sa_nracanaa rq *rq, sa_nracanaa task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the new highest prio pushable task */
	නම් (has_pushable_tasks(rq)) {
		p = plist_first_entry(&rq->rt.pushable_tasks,
				      sa_nracanaa task_struct, pushable_tasks);
		rq->rt.highest_prio.next = p->prio;
	} නැතහොත් {
		rq->rt.highest_prio.next = MAX_RT_PRIO-1;

		නම් (rq->rt.overloaded) {
			rt_clear_overload(rq);
			rq->rt.overloaded = 0;
		}
	}
}

static void enqueue_top_rt_rq(sa_nracanaa rt_rq *rt_rq);
static void dequeue_top_rt_rq(sa_nracanaa rt_rq *rt_rq, unsigned පූර්ණ_සංඛ්‍යා count);

static anta_hstha පූර්ණ_සංඛ්‍යා on_rt_rq(sa_nracanaa sched_rt_entity *rt_se)
{
	ආපසු rt_se->on_rq;
}

#ifdef CONFIG_UCLAMP_TASK
/*
 * Verify the fitness of task @p to run on @cpu taking into account the uclamp
 * settings.
 *
 * This check is only important for heterogeneous systems where uclamp_min value
 * is higher than the capacity of a @cpu. For non-heterogeneous system this
 * function will always return true.
 *
 * The function will return true if the capacity of the @cpu is >= the
 * uclamp_min and false otherwise.
 *
 * Note that uclamp_min will be clamped to uclamp_max if uclamp_min
 * > uclamp_max.
 */
static anta_hstha bool rt_task_fits_capacity(sa_nracanaa task_struct *p, පූර්ණ_සංඛ්‍යා cpu)
{
	unsigned පූර්ණ_සංඛ්‍යා min_cap;
	unsigned පූර්ණ_සංඛ්‍යා max_cap;
	unsigned පූර්ණ_සංඛ්‍යා cpu_cap;

	/* Only heterogeneous systems can benefit from this check */
	නම් (!sched_asym_cpucap_active())
		ආපසු true;

	min_cap = uclamp_eff_value(p, UCLAMP_MIN);
	max_cap = uclamp_eff_value(p, UCLAMP_MAX);

	cpu_cap = arch_scale_cpu_capacity(cpu);

	ආපසු cpu_cap >= min(min_cap, max_cap);
}
#නැතහොත් /* !CONFIG_UCLAMP_TASK: */
static anta_hstha bool rt_task_fits_capacity(sa_nracanaa task_struct *p, පූර්ණ_සංඛ්‍යා cpu)
{
	ආපසු true;
}
#endif /* !CONFIG_UCLAMP_TASK */

#ifdef CONFIG_RT_GROUP_SCHED

static anta_hstha u64 sched_rt_runtime(sa_nracanaa rt_rq *rt_rq)
{
	ආපසු rt_rq->rt_runtime;
}

static anta_hstha u64 sched_rt_period(sa_nracanaa rt_rq *rt_rq)
{
	ආපසු ktime_to_ns(rt_rq->tg->rt_bandwidth.rt_period);
}

typedef sa_nracanaa task_group *rt_rq_iter_t;

static anta_hstha sa_nracanaa task_group *next_task_group(sa_nracanaa task_group *tg)
{
	නම් (!rt_group_sched_enabled()) {
		WARN_ON(tg != &root_task_group);
		ආපසු NULL;
	}

	do {
		tg = list_entry_rcu(tg->list.next,
			typeof(sa_nracanaa task_group), list);
	} අතරතුර (&tg->list != &task_groups && task_group_is_autogroup(tg));

	නම් (&tg->list == &task_groups)
		tg = NULL;

	ආපසු tg;
}

#define for_each_rt_rq(rt_rq, iter, rq)					\
	සඳහා (iter = &root_task_group;					\
		iter && (rt_rq = iter->rt_rq[cpu_of(rq)]);		\
		iter = next_task_group(iter))

#define for_each_sched_rt_entity(rt_se) \
	සඳහා (; rt_se; rt_se = rt_se->parent)

static anta_hstha sa_nracanaa rt_rq *group_rt_rq(sa_nracanaa sched_rt_entity *rt_se)
{
	ආපසු rt_se->my_q;
}

static void enqueue_rt_entity(sa_nracanaa sched_rt_entity *rt_se, unsigned පූර්ණ_සංඛ්‍යා flags);
static void dequeue_rt_entity(sa_nracanaa sched_rt_entity *rt_se, unsigned පූර්ණ_සංඛ්‍යා flags);

static void sched_rt_rq_enqueue(sa_nracanaa rt_rq *rt_rq)
{
	sa_nracanaa task_struct *donor = rq_of_rt_rq(rt_rq)->donor;
	sa_nracanaa rq *rq = rq_of_rt_rq(rt_rq);
	sa_nracanaa sched_rt_entity *rt_se;

	පූර්ණ_සංඛ්‍යා cpu = cpu_of(rq);

	rt_se = rt_rq->tg->rt_se[cpu];

	නම් (rt_rq->rt_nr_running) {
		නම් (!rt_se)
			enqueue_top_rt_rq(rt_rq);
		නැතහොත් නම් (!on_rt_rq(rt_se))
			enqueue_rt_entity(rt_se, 0);

		නම් (rt_rq->highest_prio.curr < donor->prio)
			resched_curr(rq);
	}
}

static void sched_rt_rq_dequeue(sa_nracanaa rt_rq *rt_rq)
{
	sa_nracanaa sched_rt_entity *rt_se;
	පූර්ණ_සංඛ්‍යා cpu = cpu_of(rq_of_rt_rq(rt_rq));

	rt_se = rt_rq->tg->rt_se[cpu];

	නම් (!rt_se) {
		dequeue_top_rt_rq(rt_rq, rt_rq->rt_nr_running);
		/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
		cpufreq_update_util(rq_of_rt_rq(rt_rq), 0);
	}
	නැතහොත් නම් (on_rt_rq(rt_se))
		dequeue_rt_entity(rt_se, 0);
}

static anta_hstha පූර්ණ_සංඛ්‍යා rt_rq_throttled(sa_nracanaa rt_rq *rt_rq)
{
	ආපසු rt_rq->rt_throttled && !rt_rq->rt_nr_boosted;
}

static පූර්ණ_සංඛ්‍යා rt_se_boosted(sa_nracanaa sched_rt_entity *rt_se)
{
	sa_nracanaa rt_rq *rt_rq = group_rt_rq(rt_se);
	sa_nracanaa task_struct *p;

	නම් (rt_rq)
		ආපසු !!rt_rq->rt_nr_boosted;

	p = rt_task_of(rt_se);
	ආපසු p->prio != p->normal_prio;
}

static anta_hstha const sa_nracanaa cpumask *sched_rt_period_mask(void)
{
	ආපසු this_rq()->rd->span;
}

static anta_hstha
sa_nracanaa rt_rq *sched_rt_period_rt_rq(sa_nracanaa rt_bandwidth *rt_b, පූර්ණ_සංඛ්‍යා cpu)
{
	ආපසු container_of(rt_b, sa_nracanaa task_group, rt_bandwidth)->rt_rq[cpu];
}

static anta_hstha sa_nracanaa rt_bandwidth *sched_rt_bandwidth(sa_nracanaa rt_rq *rt_rq)
{
	ආපසු &rt_rq->tg->rt_bandwidth;
}

bool sched_rt_bandwidth_account(sa_nracanaa rt_rq *rt_rq)
{
	sa_nracanaa rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

	ආපසු (hrtimer_active(&rt_b->rt_period_timer) ||
		rt_rq->rt_time < rt_b->rt_runtime);
}

/*
 * We ran out of runtime, see if we can borrow some from our neighbours.
 */
static void do_balance_runtime(sa_nracanaa rt_rq *rt_rq)
{
	sa_nracanaa rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
	sa_nracanaa root_domain *rd = rq_of_rt_rq(rt_rq)->rd;
	පූර්ණ_සංඛ්‍යා i, weight;
	u64 rt_period;

	weight = cpumask_weight(rd->span);

	raw_spin_lock(&rt_b->rt_runtime_lock);
	rt_period = ktime_to_ns(rt_b->rt_period);
	for_each_cpu(i, rd->span) {
		sa_nracanaa rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
		s64 diff;

		නම් (iter == rt_rq)
			continue;

		raw_spin_lock(&iter->rt_runtime_lock);
		/*
		 * Either all rqs have inf runtime and there's nothing to steal
		 * or __disable_runtime() below sets a specific rq to inf to
		 * indicate its been disabled and disallow stealing.
		 */
		නම් (iter->rt_runtime == RUNTIME_INF)
			goto next;

		/*
		 * From runqueues with spare time, take 1/n part of their
		 * spare time, but no more than our period.
		 */
		diff = iter->rt_runtime - iter->rt_time;
		නම් (diff > 0) {
			diff = div_u64((u64)diff, weight);
			නම් (rt_rq->rt_runtime + diff > rt_period)
				diff = rt_period - rt_rq->rt_runtime;
			iter->rt_runtime -= diff;
			rt_rq->rt_runtime += diff;
			නම් (rt_rq->rt_runtime == rt_period) {
				raw_spin_unlock(&iter->rt_runtime_lock);
				bha_nga;
			}
		}
next:
		raw_spin_unlock(&iter->rt_runtime_lock);
	}
	raw_spin_unlock(&rt_b->rt_runtime_lock);
}

/*
 * Ensure this RQ takes back all the runtime it lend to its neighbours.
 */
static void __disable_runtime(sa_nracanaa rq *rq)
{
	sa_nracanaa root_domain *rd = rq->rd;
	rt_rq_iter_t iter;
	sa_nracanaa rt_rq *rt_rq;

	නම් (unlikely(!scheduler_running))
		ආපසු;

	for_each_rt_rq(rt_rq, iter, rq) {
		sa_nracanaa rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
		s64 want;
		පූර්ණ_සංඛ්‍යා i;

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		/*
		 * Either we're all inf and nobody needs to borrow, or we're
		 * already disabled and thus have nothing to do, or we have
		 * exactly the right amount of runtime to take out.
		 */
		නම් (rt_rq->rt_runtime == RUNTIME_INF ||
				rt_rq->rt_runtime == rt_b->rt_runtime)
			goto balanced;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);

		/*
		 * Calculate the difference between what we started out with
		 * and what we current have, that's the amount of runtime
		 * we lend and now have to reclaim.
		 */
		want = rt_b->rt_runtime - rt_rq->rt_runtime;

		/*
		 * Greedy reclaim, take back as much as we can.
		 */
		for_each_cpu(i, rd->span) {
			sa_nracanaa rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
			s64 diff;

			/*
			 * Can't reclaim from ourselves or disabled runqueues.
			 */
			නම් (iter == rt_rq || iter->rt_runtime == RUNTIME_INF)
				continue;

			raw_spin_lock(&iter->rt_runtime_lock);
			නම් (want > 0) {
				diff = min_t(s64, iter->rt_runtime, want);
				iter->rt_runtime -= diff;
				want -= diff;
			} නැතහොත් {
				iter->rt_runtime -= want;
				want -= want;
			}
			raw_spin_unlock(&iter->rt_runtime_lock);

			නම් (!want)
				bha_nga;
		}

		raw_spin_lock(&rt_rq->rt_runtime_lock);
		/*
		 * We cannot be left wanting - that would mean some runtime
		 * leaked out of the system.
		 */
		WARN_ON_ONCE(want);
balanced:
		/*
		 * Disable all the borrow logic by pretending we have inf
		 * runtime - in which case borrowing doesn't make sense.
		 */
		rt_rq->rt_runtime = RUNTIME_INF;
		rt_rq->rt_throttled = 0;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		raw_spin_unlock(&rt_b->rt_runtime_lock);

		/* Make rt_rq available for pick_next_task() */
		sched_rt_rq_enqueue(rt_rq);
	}
}

static void __enable_runtime(sa_nracanaa rq *rq)
{
	rt_rq_iter_t iter;
	sa_nracanaa rt_rq *rt_rq;

	නම් (unlikely(!scheduler_running))
		ආපසු;

	/*
	 * Reset each runqueue's bandwidth settings
	 */
	for_each_rt_rq(rt_rq, iter, rq) {
		sa_nracanaa rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_b->rt_runtime;
		rt_rq->rt_time = 0;
		rt_rq->rt_throttled = 0;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		raw_spin_unlock(&rt_b->rt_runtime_lock);
	}
}

static void balance_runtime(sa_nracanaa rt_rq *rt_rq)
{
	නම් (!sched_feat(RT_RUNTIME_SHARE))
		ආපසු;

	නම් (rt_rq->rt_time > rt_rq->rt_runtime) {
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		do_balance_runtime(rt_rq);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
	}
}

static පූර්ණ_සංඛ්‍යා do_sched_rt_period_timer(sa_nracanaa rt_bandwidth *rt_b, පූර්ණ_සංඛ්‍යා overrun)
{
	පූර්ණ_සංඛ්‍යා i, idle = 1, throttled = 0;
	const sa_nracanaa cpumask *span;

	span = sched_rt_period_mask();

	/*
	 * FIXME: isolated CPUs should really leave the root task group,
	 * whether they are isolcpus or were isolated via cpusets, lest
	 * the timer run on a CPU which does not service all runqueues,
	 * potentially leaving other CPUs indefinitely throttled.  If
	 * isolation is really required, the user will turn the throttle
	 * off to kill the perturbations it causes anyway.  Meanwhile,
	 * this maintains functionality for boot and/or troubleshooting.
	 */
	නම් (rt_b == &root_task_group.rt_bandwidth)
		span = cpu_online_mask;

	for_each_cpu(i, span) {
		පූර්ණ_සංඛ්‍යා enqueue = 0;
		sa_nracanaa rt_rq *rt_rq = sched_rt_period_rt_rq(rt_b, i);
		sa_nracanaa rq *rq = rq_of_rt_rq(rt_rq);
		sa_nracanaa rq_flags rf;
		පූර්ණ_සංඛ්‍යා skip;

		/*
		 * When span == cpu_online_mask, taking each rq->lock
		 * can be time-consuming. Try to avoid it when possible.
		 */
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		නම් (!sched_feat(RT_RUNTIME_SHARE) && rt_rq->rt_runtime != RUNTIME_INF)
			rt_rq->rt_runtime = rt_b->rt_runtime;
		skip = !rt_rq->rt_time && !rt_rq->rt_nr_running;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		නම් (skip)
			continue;

		rq_lock(rq, &rf);
		update_rq_clock(rq);

		නම් (rt_rq->rt_time) {
			u64 runtime;

			raw_spin_lock(&rt_rq->rt_runtime_lock);
			නම් (rt_rq->rt_throttled)
				balance_runtime(rt_rq);
			runtime = rt_rq->rt_runtime;
			rt_rq->rt_time -= min(rt_rq->rt_time, overrun*runtime);
			නම් (rt_rq->rt_throttled && rt_rq->rt_time < runtime) {
				rt_rq->rt_throttled = 0;
				enqueue = 1;

				/*
				 * When we're idle and a woken (rt) task is
				 * throttled wakeup_preempt() will set
				 * skip_update and the time between the wakeup
				 * and this unthrottle will get accounted as
				 * 'runtime'.
				 */
				නම් (rt_rq->rt_nr_running && rq->curr == rq->idle)
					rq_clock_cancel_skipupdate(rq);
			}
			නම් (rt_rq->rt_time || rt_rq->rt_nr_running)
				idle = 0;
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
		} නැතහොත් නම් (rt_rq->rt_nr_running) {
			idle = 0;
			නම් (!rt_rq_throttled(rt_rq))
				enqueue = 1;
		}
		නම් (rt_rq->rt_throttled)
			throttled = 1;

		නම් (enqueue)
			sched_rt_rq_enqueue(rt_rq);
		rq_unlock(rq, &rf);
	}

	නම් (!throttled && (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF))
		ආපසු 1;

	ආපසු idle;
}

static පූර්ණ_සංඛ්‍යා sched_rt_runtime_exceeded(sa_nracanaa rt_rq *rt_rq)
{
	u64 runtime = sched_rt_runtime(rt_rq);

	නම් (rt_rq->rt_throttled)
		ආපසු rt_rq_throttled(rt_rq);

	නම් (runtime >= sched_rt_period(rt_rq))
		ආපසු 0;

	balance_runtime(rt_rq);
	runtime = sched_rt_runtime(rt_rq);
	නම් (runtime == RUNTIME_INF)
		ආපසු 0;

	නම් (rt_rq->rt_time > runtime) {
		sa_nracanaa rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		/*
		 * Don't actually throttle groups that have no runtime assigned
		 * but accrue some time due to boosting.
		 */
		නම් (likely(rt_b->rt_runtime)) {
			rt_rq->rt_throttled = 1;
			printk_deferred_once("sched: RT throttling activated\n");
		} නැතහොත් {
			/*
			 * In case we did anyway, make it go away,
			 * replenishment is a joke, since it will replenish us
			 * with exactly 0 ns.
			 */
			rt_rq->rt_time = 0;
		}

		නම් (rt_rq_throttled(rt_rq)) {
			sched_rt_rq_dequeue(rt_rq);
			ආපසු 1;
		}
	}

	ආපසු 0;
}

#නැතහොත් /* !CONFIG_RT_GROUP_SCHED: */

typedef sa_nracanaa rt_rq *rt_rq_iter_t;

#define for_each_rt_rq(rt_rq, iter, rq) \
	සඳහා ((void) iter, rt_rq = &rq->rt; rt_rq; rt_rq = NULL)

#define for_each_sched_rt_entity(rt_se) \
	සඳහා (; rt_se; rt_se = NULL)

static anta_hstha sa_nracanaa rt_rq *group_rt_rq(sa_nracanaa sched_rt_entity *rt_se)
{
	ආපසු NULL;
}

static anta_hstha void sched_rt_rq_enqueue(sa_nracanaa rt_rq *rt_rq)
{
	sa_nracanaa rq *rq = rq_of_rt_rq(rt_rq);

	නම් (!rt_rq->rt_nr_running)
		ආපසු;

	enqueue_top_rt_rq(rt_rq);
	resched_curr(rq);
}

static anta_hstha void sched_rt_rq_dequeue(sa_nracanaa rt_rq *rt_rq)
{
	dequeue_top_rt_rq(rt_rq, rt_rq->rt_nr_running);
}

static anta_hstha පූර්ණ_සංඛ්‍යා rt_rq_throttled(sa_nracanaa rt_rq *rt_rq)
{
	ආපසු false;
}

static anta_hstha const sa_nracanaa cpumask *sched_rt_period_mask(void)
{
	ආපසු cpu_online_mask;
}

static anta_hstha
sa_nracanaa rt_rq *sched_rt_period_rt_rq(sa_nracanaa rt_bandwidth *rt_b, පූර්ණ_සංඛ්‍යා cpu)
{
	ආපසු &cpu_rq(cpu)->rt;
}

static void __enable_runtime(sa_nracanaa rq *rq) { }
static void __disable_runtime(sa_nracanaa rq *rq) { }

#endif /* !CONFIG_RT_GROUP_SCHED */

static anta_hstha පූර්ණ_සංඛ්‍යා rt_se_prio(sa_nracanaa sched_rt_entity *rt_se)
{
#ifdef CONFIG_RT_GROUP_SCHED
	sa_nracanaa rt_rq *rt_rq = group_rt_rq(rt_se);

	නම් (rt_rq)
		ආපසු rt_rq->highest_prio.curr;
#endif

	ආපසු rt_task_of(rt_se)->prio;
}

/*
 * Update the current task's runtime statistics. Skip current tasks that
 * are not in our scheduling class.
 */
static void update_curr_rt(sa_nracanaa rq *rq)
{
	sa_nracanaa task_struct *donor = rq->donor;
	s64 delta_exec;

	නම් (donor->sched_class != &rt_sched_class)
		ආපසු;

	delta_exec = update_curr_common(rq);
	නම් (unlikely(delta_exec <= 0))
		ආපසු;

#ifdef CONFIG_RT_GROUP_SCHED
	sa_nracanaa sched_rt_entity *rt_se = &donor->rt;

	නම් (!rt_bandwidth_enabled())
		ආපසු;

	for_each_sched_rt_entity(rt_se) {
		sa_nracanaa rt_rq *rt_rq = rt_rq_of_se(rt_se);
		පූර්ණ_සංඛ්‍යා exceeded;

		නම් (sched_rt_runtime(rt_rq) != RUNTIME_INF) {
			raw_spin_lock(&rt_rq->rt_runtime_lock);
			rt_rq->rt_time += delta_exec;
			exceeded = sched_rt_runtime_exceeded(rt_rq);
			නම් (exceeded)
				resched_curr(rq);
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
			නම් (exceeded)
				do_start_rt_bandwidth(sched_rt_bandwidth(rt_rq));
		}
	}
#endif /* CONFIG_RT_GROUP_SCHED */
}

static void
dequeue_top_rt_rq(sa_nracanaa rt_rq *rt_rq, unsigned පූර්ණ_සංඛ්‍යා count)
{
	sa_nracanaa rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	නම් (!rt_rq->rt_queued)
		ආපසු;

	BUG_ON(!rq->nr_running);

	sub_nr_running(rq, count);
	rt_rq->rt_queued = 0;

}

static void
enqueue_top_rt_rq(sa_nracanaa rt_rq *rt_rq)
{
	sa_nracanaa rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	නම් (rt_rq->rt_queued)
		ආපසු;

	නම් (rt_rq_throttled(rt_rq))
		ආපසු;

	නම් (rt_rq->rt_nr_running) {
		add_nr_running(rq, rt_rq->rt_nr_running);
		rt_rq->rt_queued = 1;
	}

	/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
	cpufreq_update_util(rq, 0);
}

static void
inc_rt_prio_smp(sa_nracanaa rt_rq *rt_rq, පූර්ණ_සංඛ්‍යා prio, පූර්ණ_සංඛ්‍යා prev_prio)
{
	sa_nracanaa rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	නම් (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		ආපසු;

	නම් (rq->online && prio < prev_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, prio);
}

static void
dec_rt_prio_smp(sa_nracanaa rt_rq *rt_rq, පූර්ණ_සංඛ්‍යා prio, පූර්ණ_සංඛ්‍යා prev_prio)
{
	sa_nracanaa rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	නම් (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		ආපසු;

	නම් (rq->online && rt_rq->highest_prio.curr != prev_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, rt_rq->highest_prio.curr);
}

static void
inc_rt_prio(sa_nracanaa rt_rq *rt_rq, පූර්ණ_සංඛ්‍යා prio)
{
	පූර්ණ_සංඛ්‍යා prev_prio = rt_rq->highest_prio.curr;

	නම් (prio < prev_prio)
		rt_rq->highest_prio.curr = prio;

	inc_rt_prio_smp(rt_rq, prio, prev_prio);
}

static void
dec_rt_prio(sa_nracanaa rt_rq *rt_rq, පූර්ණ_සංඛ්‍යා prio)
{
	පූර්ණ_සංඛ්‍යා prev_prio = rt_rq->highest_prio.curr;

	නම් (rt_rq->rt_nr_running) {

		WARN_ON(prio < prev_prio);

		/*
		 * This may have been our highest task, and therefore
		 * we may have some re-computation to do
		 */
		නම් (prio == prev_prio) {
			sa_nracanaa rt_prio_array *array = &rt_rq->active;

			rt_rq->highest_prio.curr =
				sched_find_first_bit(array->bitmap);
		}

	} නැතහොත් {
		rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	}

	dec_rt_prio_smp(rt_rq, prio, prev_prio);
}

#ifdef CONFIG_RT_GROUP_SCHED

static void
inc_rt_group(sa_nracanaa sched_rt_entity *rt_se, sa_nracanaa rt_rq *rt_rq)
{
	නම් (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted++;

	start_rt_bandwidth(&rt_rq->tg->rt_bandwidth);
}

static void
dec_rt_group(sa_nracanaa sched_rt_entity *rt_se, sa_nracanaa rt_rq *rt_rq)
{
	නම් (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted--;

	WARN_ON(!rt_rq->rt_nr_running && rt_rq->rt_nr_boosted);
}

#නැතහොත් /* !CONFIG_RT_GROUP_SCHED: */

static void
inc_rt_group(sa_nracanaa sched_rt_entity *rt_se, sa_nracanaa rt_rq *rt_rq)
{
}

static anta_hstha
void dec_rt_group(sa_nracanaa sched_rt_entity *rt_se, sa_nracanaa rt_rq *rt_rq) {}

#endif /* !CONFIG_RT_GROUP_SCHED */

static anta_hstha
unsigned පූර්ණ_සංඛ්‍යා rt_se_nr_running(sa_nracanaa sched_rt_entity *rt_se)
{
	sa_nracanaa rt_rq *group_rq = group_rt_rq(rt_se);

	නම් (group_rq)
		ආපසු group_rq->rt_nr_running;
	නැතහොත්
		ආපසු 1;
}

static anta_hstha
unsigned පූර්ණ_සංඛ්‍යා rt_se_rr_nr_running(sa_nracanaa sched_rt_entity *rt_se)
{
	sa_nracanaa rt_rq *group_rq = group_rt_rq(rt_se);
	sa_nracanaa task_struct *tsk;

	නම් (group_rq)
		ආපසු group_rq->rr_nr_running;

	tsk = rt_task_of(rt_se);

	ආපසු (tsk->policy == SCHED_RR) ? 1 : 0;
}

static anta_hstha
void inc_rt_tasks(sa_nracanaa sched_rt_entity *rt_se, sa_nracanaa rt_rq *rt_rq)
{
	පූර්ණ_සංඛ්‍යා prio = rt_se_prio(rt_se);

	WARN_ON(!rt_prio(prio));
	rt_rq->rt_nr_running += rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running += rt_se_rr_nr_running(rt_se);

	inc_rt_prio(rt_rq, prio);
	inc_rt_group(rt_se, rt_rq);
}

static anta_hstha
void dec_rt_tasks(sa_nracanaa sched_rt_entity *rt_se, sa_nracanaa rt_rq *rt_rq)
{
	WARN_ON(!rt_prio(rt_se_prio(rt_se)));
	WARN_ON(!rt_rq->rt_nr_running);
	rt_rq->rt_nr_running -= rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running -= rt_se_rr_nr_running(rt_se);

	dec_rt_prio(rt_rq, rt_se_prio(rt_se));
	dec_rt_group(rt_se, rt_rq);
}

/*
 * Change rt_se->run_list location unless SAVE && !MOVE
 *
 * assumes ENQUEUE/DEQUEUE flags match
 */
static anta_hstha bool move_entity(unsigned පූර්ණ_සංඛ්‍යා flags)
{
	නම් ((flags & (DEQUEUE_SAVE | DEQUEUE_MOVE)) == DEQUEUE_SAVE)
		ආපසු false;

	ආපසු true;
}

static void __delist_rt_entity(sa_nracanaa sched_rt_entity *rt_se, sa_nracanaa rt_prio_array *array)
{
	list_del_init(&rt_se->run_list);

	නම් (list_empty(array->queue + rt_se_prio(rt_se)))
		__clear_bit(rt_se_prio(rt_se), array->bitmap);

	rt_se->on_list = 0;
}

static anta_hstha sa_nracanaa sched_statistics *
__schedstats_from_rt_se(sa_nracanaa sched_rt_entity *rt_se)
{
	/* schedstats is not supported for rt group. */
	නම් (!rt_entity_is_task(rt_se))
		ආපසු NULL;

	ආපසු &rt_task_of(rt_se)->stats;
}

static anta_hstha void
update_stats_wait_start_rt(sa_nracanaa rt_rq *rt_rq, sa_nracanaa sched_rt_entity *rt_se)
{
	sa_nracanaa sched_statistics *stats;
	sa_nracanaa task_struct *p = NULL;

	නම් (!schedstat_enabled())
		ආපසු;

	නම් (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	නම් (!stats)
		ආපසු;

	__update_stats_wait_start(rq_of_rt_rq(rt_rq), p, stats);
}

static anta_hstha void
update_stats_enqueue_sleeper_rt(sa_nracanaa rt_rq *rt_rq, sa_nracanaa sched_rt_entity *rt_se)
{
	sa_nracanaa sched_statistics *stats;
	sa_nracanaa task_struct *p = NULL;

	නම් (!schedstat_enabled())
		ආපසු;

	නම් (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	නම් (!stats)
		ආපසු;

	__update_stats_enqueue_sleeper(rq_of_rt_rq(rt_rq), p, stats);
}

static anta_hstha void
update_stats_enqueue_rt(sa_nracanaa rt_rq *rt_rq, sa_nracanaa sched_rt_entity *rt_se,
			පූර්ණ_සංඛ්‍යා flags)
{
	නම් (!schedstat_enabled())
		ආපසු;

	නම් (flags & ENQUEUE_WAKEUP)
		update_stats_enqueue_sleeper_rt(rt_rq, rt_se);
}

static anta_hstha void
update_stats_wait_end_rt(sa_nracanaa rt_rq *rt_rq, sa_nracanaa sched_rt_entity *rt_se)
{
	sa_nracanaa sched_statistics *stats;
	sa_nracanaa task_struct *p = NULL;

	නම් (!schedstat_enabled())
		ආපසු;

	නම් (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	නම් (!stats)
		ආපසු;

	__update_stats_wait_end(rq_of_rt_rq(rt_rq), p, stats);
}

static anta_hstha void
update_stats_dequeue_rt(sa_nracanaa rt_rq *rt_rq, sa_nracanaa sched_rt_entity *rt_se,
			පූර්ණ_සංඛ්‍යා flags)
{
	sa_nracanaa task_struct *p = NULL;
	sa_nracanaa rq *rq = rq_of_rt_rq(rt_rq);

	නම් (!schedstat_enabled())
		ආපසු;

	නම් (rt_entity_is_task(rt_se)) {
		p = rt_task_of(rt_se);

		නම් (p != rq->curr)
			update_stats_wait_end_rt(rt_rq, rt_se);
	}

	නම් ((flags & DEQUEUE_SLEEP) && p) {
		unsigned පූර්ණ_සංඛ්‍යා state;

		state = READ_ONCE(p->__state);
		නම් (state & TASK_INTERRUPTIBLE)
			__schedstat_set(p->stats.sleep_start,
					rq_clock(rq_of_rt_rq(rt_rq)));

		නම් (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(p->stats.block_start,
					rq_clock(rq_of_rt_rq(rt_rq)));
	}
}

static void __enqueue_rt_entity(sa_nracanaa sched_rt_entity *rt_se, unsigned පූර්ණ_සංඛ්‍යා flags)
{
	sa_nracanaa rt_rq *rt_rq = rt_rq_of_se(rt_se);
	sa_nracanaa rt_prio_array *array = &rt_rq->active;
	sa_nracanaa rt_rq *group_rq = group_rt_rq(rt_se);
	sa_nracanaa list_head *queue = array->queue + rt_se_prio(rt_se);

	/*
	 * Don't enqueue the group if its throttled, or when empty.
	 * The latter is a consequence of the former when a child group
	 * get throttled and the current group doesn't have any other
	 * active members.
	 */
	නම් (group_rq && (rt_rq_throttled(group_rq) || !group_rq->rt_nr_running)) {
		නම් (rt_se->on_list)
			__delist_rt_entity(rt_se, array);
		ආපසු;
	}

	නම් (move_entity(flags)) {
		WARN_ON_ONCE(rt_se->on_list);
		නම් (flags & ENQUEUE_HEAD)
			list_add(&rt_se->run_list, queue);
		නැතහොත්
			list_add_tail(&rt_se->run_list, queue);

		__set_bit(rt_se_prio(rt_se), array->bitmap);
		rt_se->on_list = 1;
	}
	rt_se->on_rq = 1;

	inc_rt_tasks(rt_se, rt_rq);
}

static void __dequeue_rt_entity(sa_nracanaa sched_rt_entity *rt_se, unsigned පූර්ණ_සංඛ්‍යා flags)
{
	sa_nracanaa rt_rq *rt_rq = rt_rq_of_se(rt_se);
	sa_nracanaa rt_prio_array *array = &rt_rq->active;

	නම් (move_entity(flags)) {
		WARN_ON_ONCE(!rt_se->on_list);
		__delist_rt_entity(rt_se, array);
	}
	rt_se->on_rq = 0;

	dec_rt_tasks(rt_se, rt_rq);
}

/*
 * Because the prio of an upper entry depends on the lower
 * entries, we must remove entries top - down.
 */
static void dequeue_rt_stack(sa_nracanaa sched_rt_entity *rt_se, unsigned පූර්ණ_සංඛ්‍යා flags)
{
	sa_nracanaa sched_rt_entity *back = NULL;
	unsigned පූර්ණ_සංඛ්‍යා rt_nr_running;

	for_each_sched_rt_entity(rt_se) {
		rt_se->back = back;
		back = rt_se;
	}

	rt_nr_running = rt_rq_of_se(back)->rt_nr_running;

	සඳහා (rt_se = back; rt_se; rt_se = rt_se->back) {
		නම් (on_rt_rq(rt_se))
			__dequeue_rt_entity(rt_se, flags);
	}

	dequeue_top_rt_rq(rt_rq_of_se(back), rt_nr_running);
}

static void enqueue_rt_entity(sa_nracanaa sched_rt_entity *rt_se, unsigned පූර්ණ_සංඛ්‍යා flags)
{
	sa_nracanaa rq *rq = rq_of_rt_se(rt_se);

	update_stats_enqueue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_stack(rt_se, flags);
	for_each_sched_rt_entity(rt_se)
		__enqueue_rt_entity(rt_se, flags);
	enqueue_top_rt_rq(&rq->rt);
}

static void dequeue_rt_entity(sa_nracanaa sched_rt_entity *rt_se, unsigned පූර්ණ_සංඛ්‍යා flags)
{
	sa_nracanaa rq *rq = rq_of_rt_se(rt_se);

	update_stats_dequeue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_stack(rt_se, flags);

	for_each_sched_rt_entity(rt_se) {
		sa_nracanaa rt_rq *rt_rq = group_rt_rq(rt_se);

		නම් (rt_rq && rt_rq->rt_nr_running)
			__enqueue_rt_entity(rt_se, flags);
	}
	enqueue_top_rt_rq(&rq->rt);
}

/*
 * Adding/removing a task to/from a priority array:
 */
static void
enqueue_task_rt(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, පූර්ණ_සංඛ්‍යා flags)
{
	sa_nracanaa sched_rt_entity *rt_se = &p->rt;

	නම් (flags & ENQUEUE_WAKEUP)
		rt_se->timeout = 0;

	check_schedstat_required();
	update_stats_wait_start_rt(rt_rq_of_se(rt_se), rt_se);

	enqueue_rt_entity(rt_se, flags);

	නම් (task_is_blocked(p))
		ආපසු;

	නම් (!task_current(rq, p) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

static bool dequeue_task_rt(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, පූර්ණ_සංඛ්‍යා flags)
{
	sa_nracanaa sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	dequeue_rt_entity(rt_se, flags);

	dequeue_pushable_task(rq, p);

	ආපසු true;
}

/*
 * Put task to the head or the end of the run list without the overhead of
 * dequeue followed by enqueue.
 */
static void
requeue_rt_entity(sa_nracanaa rt_rq *rt_rq, sa_nracanaa sched_rt_entity *rt_se, පූර්ණ_සංඛ්‍යා head)
{
	නම් (on_rt_rq(rt_se)) {
		sa_nracanaa rt_prio_array *array = &rt_rq->active;
		sa_nracanaa list_head *queue = array->queue + rt_se_prio(rt_se);

		නම් (head)
			list_move(&rt_se->run_list, queue);
		නැතහොත්
			list_move_tail(&rt_se->run_list, queue);
	}
}

static void requeue_task_rt(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, පූර්ණ_සංඛ්‍යා head)
{
	sa_nracanaa sched_rt_entity *rt_se = &p->rt;
	sa_nracanaa rt_rq *rt_rq;

	for_each_sched_rt_entity(rt_se) {
		rt_rq = rt_rq_of_se(rt_se);
		requeue_rt_entity(rt_rq, rt_se, head);
	}
}

static void yield_task_rt(sa_nracanaa rq *rq)
{
	requeue_task_rt(rq, rq->donor, 0);
}

static පූර්ණ_සංඛ්‍යා find_lowest_rq(sa_nracanaa task_struct *task);

static පූර්ණ_සංඛ්‍යා
select_task_rq_rt(sa_nracanaa task_struct *p, පූර්ණ_සංඛ්‍යා cpu, පූර්ණ_සංඛ්‍යා flags)
{
	sa_nracanaa task_struct *curr, *donor;
	sa_nracanaa rq *rq;
	bool test;

	/* For anything but wake ups, just return the task_cpu */
	නම් (!(flags & (WF_TTWU | WF_FORK)))
		goto out;

	rq = cpu_rq(cpu);

	rcu_read_lock();
	curr = READ_ONCE(rq->curr); /* unlocked access */
	donor = READ_ONCE(rq->donor);

	/*
	 * If the current task on @p's runqueue is an RT task, then
	 * try to see if we can wake this RT task up on another
	 * runqueue. Otherwise simply start this RT task
	 * on its current runqueue.
	 *
	 * We want to avoid overloading runqueues. If the woken
	 * task is a higher priority, then it will stay on this CPU
	 * and the lower prio task should be moved to another CPU.
	 * Even though this will probably make the lower prio task
	 * lose its cache, we do not want to bounce a higher task
	 * around just because it gave up its CPU, perhaps for a
	 * lock?
	 *
	 * For equal prio tasks, we just let the scheduler sort it out.
	 *
	 * Otherwise, just let it ride on the affine RQ and the
	 * post-schedule router will push the preempted task away
	 *
	 * This test is optimistic, if we get it wrong the load-balancer
	 * will have to sort it out.
	 *
	 * We take into account the capacity of the CPU to ensure it fits the
	 * requirement of the task - which is only important on heterogeneous
	 * systems like big.LITTLE.
	 */
	test = curr &&
	       unlikely(rt_task(donor)) &&
	       (curr->nr_cpus_allowed < 2 || donor->prio <= p->prio);

	නම් (test || !rt_task_fits_capacity(p, cpu)) {
		පූර්ණ_සංඛ්‍යා target = find_lowest_rq(p);

		/*
		 * Bail out if we were forcing a migration to find a better
		 * fitting CPU but our search failed.
		 */
		නම් (!test && target != -1 && !rt_task_fits_capacity(p, target))
			goto out_unlock;

		/*
		 * Don't bother moving it if the destination CPU is
		 * not running a lower priority task.
		 */
		නම් (target != -1 &&
		    p->prio < cpu_rq(target)->rt.highest_prio.curr)
			cpu = target;
	}

out_unlock:
	rcu_read_unlock();

out:
	ආපසු cpu;
}

static void check_preempt_equal_prio(sa_nracanaa rq *rq, sa_nracanaa task_struct *p)
{
	නම් (rq->curr->nr_cpus_allowed == 1 ||
	    !cpupri_find(&rq->rd->cpupri, rq->donor, NULL))
		ආපසු;

	/*
	 * p is migratable, so let's not schedule it and
	 * see if it is pushed or pulled somewhere else.
	 */
	නම් (p->nr_cpus_allowed != 1 &&
	    cpupri_find(&rq->rd->cpupri, p, NULL))
		ආපසු;

	/*
	 * There appear to be other CPUs that can accept
	 * the current task but none can run 'p', so lets reschedule
	 * to try and push the current task away:
	 */
	requeue_task_rt(rq, p, 1);
	resched_curr(rq);
}

static පූර්ණ_සංඛ්‍යා balance_rt(sa_nracanaa rq *rq, sa_nracanaa rq_flags *rf)
{
	/*
	 * Note, rq->donor may change during rq lock drops,
	 * so don't re-use p across lock drops
	 */
	sa_nracanaa task_struct *p = rq->donor;

	නම් (!on_rt_rq(&p->rt) && need_pull_rt_task(rq, p)) {
		/*
		 * This is OK, because current is on_cpu, which avoids it being
		 * picked for load-balance and preemption/IRQs are still
		 * disabled avoiding further scheduler activity on it and we've
		 * not yet started the picking loop.
		 */
		rq_unpin_lock(rq, rf);
		pull_rt_task(rq);
		rq_repin_lock(rq, rf);
	}

	ආපසු sched_stop_runnable(rq) || sched_dl_runnable(rq) || sched_rt_runnable(rq);
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
static void wakeup_preempt_rt(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, පූර්ණ_සංඛ්‍යා flags)
{
	sa_nracanaa task_struct *donor = rq->donor;

	/*
	 * XXX If we're preempted by DL, queue a push?
	 */
	නම් (p->sched_class != &rt_sched_class ||
	    donor->sched_class != &rt_sched_class)
		ආපසු;

	නම් (p->prio < donor->prio) {
		resched_curr(rq);
		ආපසු;
	}

	/*
	 * If:
	 *
	 * - the newly woken task is of equal priority to the current task
	 * - the newly woken task is non-migratable while current is migratable
	 * - current will be preempted on the next reschedule
	 *
	 * we should check to see if current can readily move to a different
	 * cpu.  If so, we will reschedule to allow the push logic to try
	 * to move current somewhere else, making room for our non-migratable
	 * task.
	 */
	නම් (p->prio == donor->prio && !test_tsk_need_resched(rq->curr))
		check_preempt_equal_prio(rq, p);
}

static anta_hstha void set_next_task_rt(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, ga_nanaa snt_e type)
{
	sa_nracanaa sched_rt_entity *rt_se = &p->rt;
	sa_nracanaa rt_rq *rt_rq = &rq->rt;

	නම් (type == SNT_REPICK)
		ආපසු;

	p->se.exec_start = rq_clock_task(rq);
	නම් (on_rt_rq(&p->rt))
		update_stats_wait_end_rt(rt_rq, rt_se);

	/* The running task is never eligible for pushing */
	dequeue_pushable_task(rq, p);

	නම් (type != SNT_PICK)
		ආපසු;

	/*
	 * If prev task was rt, put_prev_task() has already updated the
	 * utilization. We only care of the case where we start to schedule a
	 * rt task
	 */
	නම් (rq->donor->sched_class != &rt_sched_class)
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);

	rt_queue_push_tasks(rq);
}

static sa_nracanaa sched_rt_entity *pick_next_rt_entity(sa_nracanaa rt_rq *rt_rq)
{
	sa_nracanaa rt_prio_array *array = &rt_rq->active;
	sa_nracanaa sched_rt_entity *next = NULL;
	sa_nracanaa list_head *queue;
	පූර්ණ_සංඛ්‍යා idx;

	idx = sched_find_first_bit(array->bitmap);
	BUG_ON(idx >= MAX_RT_PRIO);

	queue = array->queue + idx;
	නම් (WARN_ON_ONCE(list_empty(queue)))
		ආපසු NULL;
	next = list_entry(queue->next, sa_nracanaa sched_rt_entity, run_list);

	ආපසු next;
}

static sa_nracanaa task_struct *_pick_next_task_rt(sa_nracanaa rq *rq)
{
	sa_nracanaa sched_rt_entity *rt_se;
	sa_nracanaa rt_rq *rt_rq  = &rq->rt;

	do {
		rt_se = pick_next_rt_entity(rt_rq);
		නම් (unlikely(!rt_se))
			ආපසු NULL;
		rt_rq = group_rt_rq(rt_se);
	} අතරතුර (rt_rq);

	ආපසු rt_task_of(rt_se);
}

static sa_nracanaa task_struct *pick_task_rt(sa_nracanaa rq *rq, sa_nracanaa rq_flags *rf)
{
	sa_nracanaa task_struct *p;

	නම් (!sched_rt_runnable(rq))
		ආපසු NULL;

	p = _pick_next_task_rt(rq);

	ආපසු p;
}

static void put_prev_task_rt(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, sa_nracanaa task_struct *next)
{
	sa_nracanaa sched_rt_entity *rt_se = &p->rt;
	sa_nracanaa rt_rq *rt_rq = &rq->rt;

	නම් (on_rt_rq(&p->rt))
		update_stats_wait_start_rt(rt_rq, rt_se);

	update_curr_rt(rq);

	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	නම් (task_is_blocked(p))
		ආපසු;
	/*
	 * The previous task needs to be made eligible for pushing
	 * if it is still active
	 */
	නම් (on_rt_rq(&p->rt) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

/* Only try algorithms three times */
#define RT_MAX_TRIES 3

/*
 * Return the highest pushable rq's task, which is suitable to be executed
 * on the CPU, NULL otherwise
 */
static sa_nracanaa task_struct *pick_highest_pushable_task(sa_nracanaa rq *rq, පූර්ණ_සංඛ්‍යා cpu)
{
	sa_nracanaa plist_head *head = &rq->rt.pushable_tasks;
	sa_nracanaa task_struct *p;

	නම් (!has_pushable_tasks(rq))
		ආපසු NULL;

	plist_for_each_entry(p, head, pushable_tasks) {
		නම් (task_is_pushable(rq, p, cpu))
			ආපසු p;
	}

	ආපසු NULL;
}

static DEFINE_PER_CPU(cpumask_var_t, local_cpu_mask);

static පූර්ණ_සංඛ්‍යා find_lowest_rq(sa_nracanaa task_struct *task)
{
	sa_nracanaa sched_domain *sd;
	sa_nracanaa cpumask *lowest_mask = this_cpu_cpumask_var_ptr(local_cpu_mask);
	පූර්ණ_සංඛ්‍යා this_cpu = smp_processor_id();
	පූර්ණ_සංඛ්‍යා cpu      = task_cpu(task);
	පූර්ණ_සංඛ්‍යා ret;

	/* Make sure the mask is initialized first */
	නම් (unlikely(!lowest_mask))
		ආපසු -1;

	නම් (task->nr_cpus_allowed == 1)
		ආපසු -1; /* No other targets possible */

	/*
	 * If we're on asym system ensure we consider the different capacities
	 * of the CPUs when searching for the lowest_mask.
	 */
	නම් (sched_asym_cpucap_active()) {

		ret = cpupri_find_fitness(&task_rq(task)->rd->cpupri,
					  task, lowest_mask,
					  rt_task_fits_capacity);
	} නැතහොත් {

		ret = cpupri_find(&task_rq(task)->rd->cpupri,
				  task, lowest_mask);
	}

	නම් (!ret)
		ආපසු -1; /* No targets found */

	/*
	 * At this point we have built a mask of CPUs representing the
	 * lowest priority tasks in the system.  Now we want to elect
	 * the best one based on our affinity and topology.
	 *
	 * We prioritize the last CPU that the task executed on since
	 * it is most likely cache-hot in that location.
	 */
	නම් (cpumask_test_cpu(cpu, lowest_mask))
		ආපසු cpu;

	/*
	 * Otherwise, we consult the sched_domains span maps to figure
	 * out which CPU is logically closest to our hot cache data.
	 */
	නම් (!cpumask_test_cpu(this_cpu, lowest_mask))
		this_cpu = -1; /* Skip this_cpu opt if not among lowest */

	rcu_read_lock();
	for_each_domain(cpu, sd) {
		නම් (sd->flags & SD_WAKE_AFFINE) {
			පූර්ණ_සංඛ්‍යා best_cpu;

			/*
			 * "this_cpu" is cheaper to preempt than a
			 * remote processor.
			 */
			නම් (this_cpu != -1 &&
			    cpumask_test_cpu(this_cpu, sched_domain_span(sd))) {
				rcu_read_unlock();
				ආපසු this_cpu;
			}

			best_cpu = cpumask_any_and_distribute(lowest_mask,
							      sched_domain_span(sd));
			නම් (best_cpu < nr_cpu_ids) {
				rcu_read_unlock();
				ආපසු best_cpu;
			}
		}
	}
	rcu_read_unlock();

	/*
	 * And finally, if there were no matches within the domains
	 * just give the caller *something* to work with from the compatible
	 * locations.
	 */
	නම් (this_cpu != -1)
		ආපසු this_cpu;

	cpu = cpumask_any_distribute(lowest_mask);
	නම් (cpu < nr_cpu_ids)
		ආපසු cpu;

	ආපසු -1;
}

static sa_nracanaa task_struct *pick_next_pushable_task(sa_nracanaa rq *rq)
{
	sa_nracanaa plist_head *head = &rq->rt.pushable_tasks;
	sa_nracanaa task_struct *i, *p = NULL;

	නම් (!has_pushable_tasks(rq))
		ආපසු NULL;

	plist_for_each_entry(i, head, pushable_tasks) {
		/* skip tasks that cannot be migrated */
		නම් (!task_on_cpu(rq, i) && !is_migration_disabled(i)) {
			p = i;
			bha_nga;
		}
	}

	නම් (!p)
		ආපසු NULL;

	BUG_ON(rq->cpu != task_cpu(p));
	BUG_ON(task_current(rq, p));
	BUG_ON(task_current_donor(rq, p));
	BUG_ON(p->nr_cpus_allowed <= 1);

	BUG_ON(!task_on_rq_queued(p));
	BUG_ON(!rt_task(p));

	ආපසු p;
}

/* Will lock the rq it finds */
static sa_nracanaa rq *find_lock_lowest_rq(sa_nracanaa task_struct *task, sa_nracanaa rq *rq)
{
	sa_nracanaa rq *lowest_rq = NULL;
	පූර්ණ_සංඛ්‍යා tries;
	පූර්ණ_සංඛ්‍යා cpu;

	සඳහා (tries = 0; tries < RT_MAX_TRIES; tries++) {
		cpu = find_lowest_rq(task);

		නම් ((cpu == -1) || (cpu == rq->cpu))
			bha_nga;

		lowest_rq = cpu_rq(cpu);

		නම් (lowest_rq->rt.highest_prio.curr <= task->prio) {
			/*
			 * Target rq has tasks of equal or higher priority,
			 * retrying does not release any lock and is unlikely
			 * to yield a different result.
			 */
			lowest_rq = NULL;
			bha_nga;
		}

		/* if the prio of this runqueue changed, try again */
		නම් (double_lock_balance(rq, lowest_rq)) {
			/*
			 * We had to unlock the run queue. In
			 * the mean time, task could have
			 * migrated already or had its affinity changed,
			 * therefore check if the task is still at the
			 * head of the pushable tasks list.
			 * It is possible the task was scheduled, set
			 * "migrate_disabled" and then got preempted, so we must
			 * check the task migration disable flag here too.
			 */
			නම් (unlikely(is_migration_disabled(task) ||
				     !cpumask_test_cpu(lowest_rq->cpu, &task->cpus_mask) ||
				     task != pick_next_pushable_task(rq))) {

				double_unlock_balance(rq, lowest_rq);
				lowest_rq = NULL;
				bha_nga;
			}
		}

		/* If this rq is still suitable use it. */
		නම් (lowest_rq->rt.highest_prio.curr > task->prio)
			bha_nga;

		/* try again */
		double_unlock_balance(rq, lowest_rq);
		lowest_rq = NULL;
	}

	ආපසු lowest_rq;
}

/*
 * If the current CPU has more than one RT task, see if the non
 * running task can migrate over to a CPU that is running a task
 * of lesser priority.
 */
static පූර්ණ_සංඛ්‍යා push_rt_task(sa_nracanaa rq *rq, bool pull)
{
	sa_nracanaa task_struct *next_task;
	sa_nracanaa rq *lowest_rq;
	පූර්ණ_සංඛ්‍යා ret = 0;

	නම් (!rq->rt.overloaded)
		ආපසු 0;

	next_task = pick_next_pushable_task(rq);
	නම් (!next_task)
		ආපසු 0;

retry:
	/*
	 * It's possible that the next_task slipped in of
	 * higher priority than current. If that's the case
	 * just reschedule current.
	 */
	නම් (unlikely(next_task->prio < rq->donor->prio)) {
		resched_curr(rq);
		ආපසු 0;
	}

	නම් (is_migration_disabled(next_task)) {
		sa_nracanaa task_struct *push_task = NULL;
		පූර්ණ_සංඛ්‍යා cpu;

		නම් (!pull || rq->push_busy)
			ආපසු 0;

		/*
		 * Invoking find_lowest_rq() on anything but an RT task doesn't
		 * make sense. Per the above priority check, curr has to
		 * be of higher priority than next_task, so no need to
		 * reschedule when bailing out.
		 *
		 * Note that the stoppers are masqueraded as SCHED_FIFO
		 * (cf. sched_set_stop_task()), so we can't rely on rt_task().
		 */
		නම් (rq->donor->sched_class != &rt_sched_class)
			ආපසු 0;

		cpu = find_lowest_rq(rq->curr);
		නම් (cpu == -1 || cpu == rq->cpu)
			ආපසු 0;

		/*
		 * Given we found a CPU with lower priority than @next_task,
		 * therefore it should be running. However we cannot migrate it
		 * to this other CPU, instead attempt to push the current
		 * running task on this CPU away.
		 */
		push_task = get_push_task(rq);
		නම් (push_task) {
			preempt_disable();
			raw_spin_rq_unlock(rq);
			stop_one_cpu_nowait(rq->cpu, push_cpu_stop,
					    push_task, &rq->push_work);
			preempt_enable();
			raw_spin_rq_lock(rq);
		}

		ආපසු 0;
	}

	නම් (WARN_ON(next_task == rq->curr))
		ආපසු 0;

	/* We might release rq lock */
	get_task_struct(next_task);

	/* find_lock_lowest_rq locks the rq if found */
	lowest_rq = find_lock_lowest_rq(next_task, rq);
	නම් (!lowest_rq) {
		sa_nracanaa task_struct *task;
		/*
		 * find_lock_lowest_rq releases rq->lock
		 * so it is possible that next_task has migrated.
		 *
		 * We need to make sure that the task is still on the same
		 * run-queue and is also still the next task eligible for
		 * pushing.
		 */
		task = pick_next_pushable_task(rq);
		නම් (task == next_task) {
			/*
			 * The task hasn't migrated, and is still the next
			 * eligible task, but we failed to find a run-queue
			 * to push it to.  Do not retry in this case, since
			 * other CPUs will pull from us when ready.
			 */
			goto out;
		}

		නම් (!task)
			/* No more tasks, just exit */
			goto out;

		/*
		 * Something has shifted, try again.
		 */
		put_task_struct(next_task);
		next_task = task;
		goto retry;
	}

	move_queued_task_locked(rq, lowest_rq, next_task);
	resched_curr(lowest_rq);
	ret = 1;

	double_unlock_balance(rq, lowest_rq);
out:
	put_task_struct(next_task);

	ආපසු ret;
}

static void push_rt_tasks(sa_nracanaa rq *rq)
{
	/* push_rt_task will return true if it moved an RT */
	අතරතුර (push_rt_task(rq, false))
		;
}

#ifdef HAVE_RT_PUSH_IPI

/*
 * When a high priority task schedules out from a CPU and a lower priority
 * task is scheduled in, a check is made to see if there's any RT tasks
 * on other CPUs that are waiting to run because a higher priority RT task
 * is currently running on its CPU. In this case, the CPU with multiple RT
 * tasks queued on it (overloaded) needs to be notified that a CPU has opened
 * up that may be able to run one of its non-running queued RT tasks.
 *
 * All CPUs with overloaded RT tasks need to be notified as there is currently
 * no way to know which of these CPUs have the highest priority task waiting
 * to run. Instead of trying to take a spinlock on each of these CPUs,
 * which has shown to cause large latency when done on machines with many
 * CPUs, sending an IPI to the CPUs to have them push off the overloaded
 * RT tasks waiting to run.
 *
 * Just sending an IPI to each of the CPUs is also an issue, as on large
 * count CPU machines, this can cause an IPI storm on a CPU, especially
 * if its the only CPU with multiple RT tasks queued, and a large number
 * of CPUs scheduling a lower priority task at the same time.
 *
 * Each root domain has its own IRQ work function that can iterate over
 * all CPUs with RT overloaded tasks. Since all CPUs with overloaded RT
 * task must be checked if there's one or many CPUs that are lowering
 * their priority, there's a single IRQ work iterator that will try to
 * push off RT tasks that are waiting to run.
 *
 * When a CPU schedules a lower priority task, it will kick off the
 * IRQ work iterator that will jump to each CPU with overloaded RT tasks.
 * As it only takes the first CPU that schedules a lower priority task
 * to start the process, the rto_start variable is incremented and if
 * the atomic result is one, then that CPU will try to take the rto_lock.
 * This prevents high contention on the lock as the process handles all
 * CPUs scheduling lower priority tasks.
 *
 * All CPUs that are scheduling a lower priority task will increment the
 * rt_loop_next variable. This will make sure that the IRQ work iterator
 * checks all RT overloaded CPUs whenever a CPU schedules a new lower
 * priority task, even if the iterator is in the middle of a scan. Incrementing
 * the rt_loop_next will cause the iterator to perform another scan.
 *
 */
static පූර්ණ_සංඛ්‍යා rto_next_cpu(sa_nracanaa root_domain *rd)
{
	පූර්ණ_සංඛ්‍යා this_cpu = smp_processor_id();
	පූර්ණ_සංඛ්‍යා next;
	පූර්ණ_සංඛ්‍යා cpu;

	/*
	 * When starting the IPI RT pushing, the rto_cpu is set to -1,
	 * rt_next_cpu() will simply return the first CPU found in
	 * the rto_mask.
	 *
	 * If rto_next_cpu() is called with rto_cpu is a valid CPU, it
	 * will return the next CPU found in the rto_mask.
	 *
	 * If there are no more CPUs left in the rto_mask, then a check is made
	 * against rto_loop and rto_loop_next. rto_loop is only updated with
	 * the rto_lock held, but any CPU may increment the rto_loop_next
	 * without any locking.
	 */
	සඳහා (;;) {

		/* When rto_cpu is -1 this acts like cpumask_first() */
		cpu = cpumask_next(rd->rto_cpu, rd->rto_mask);

		rd->rto_cpu = cpu;

		/* Do not send IPI to self */
		නම් (cpu == this_cpu)
			continue;

		නම් (cpu < nr_cpu_ids)
			ආපසු cpu;

		rd->rto_cpu = -1;

		/*
		 * ACQUIRE ensures we see the @rto_mask changes
		 * made prior to the @next value observed.
		 *
		 * Matches WMB in rt_set_overload().
		 */
		next = atomic_read_acquire(&rd->rto_loop_next);

		නම් (rd->rto_loop == next)
			bha_nga;

		rd->rto_loop = next;
	}

	ආපසු -1;
}

static anta_hstha bool rto_start_trylock(atomic_t *v)
{
	ආපසු !atomic_cmpxchg_acquire(v, 0, 1);
}

static anta_hstha void rto_start_unlock(atomic_t *v)
{
	atomic_set_release(v, 0);
}

static void tell_cpu_to_push(sa_nracanaa rq *rq)
{
	පූර්ණ_සංඛ්‍යා cpu = -1;

	/* Keep the loop going if the IPI is currently active */
	atomic_inc(&rq->rd->rto_loop_next);

	/* Only one CPU can initiate a loop at a time */
	නම් (!rto_start_trylock(&rq->rd->rto_loop_start))
		ආපසු;

	raw_spin_lock(&rq->rd->rto_lock);

	/*
	 * The rto_cpu is updated under the lock, if it has a valid CPU
	 * then the IPI is still running and will continue due to the
	 * update to loop_next, and nothing needs to be done here.
	 * Otherwise it is finishing up and an IPI needs to be sent.
	 */
	නම් (rq->rd->rto_cpu < 0)
		cpu = rto_next_cpu(rq->rd);

	raw_spin_unlock(&rq->rd->rto_lock);

	rto_start_unlock(&rq->rd->rto_loop_start);

	නම් (cpu >= 0) {
		/* Make sure the rd does not get freed while pushing */
		sched_get_rd(rq->rd);
		irq_work_queue_on(&rq->rd->rto_push_work, cpu);
	}
}

/* Called from hardirq context */
void rto_push_irq_work_func(sa_nracanaa irq_work *work)
{
	sa_nracanaa root_domain *rd =
		container_of(work, sa_nracanaa root_domain, rto_push_work);
	sa_nracanaa rq *rq;
	පූර්ණ_සංඛ්‍යා cpu;

	rq = this_rq();

	/*
	 * We do not need to grab the lock to check for has_pushable_tasks.
	 * When it gets updated, a check is made if a push is possible.
	 */
	නම් (has_pushable_tasks(rq)) {
		raw_spin_rq_lock(rq);
		අතරතුර (push_rt_task(rq, true))
			;
		raw_spin_rq_unlock(rq);
	}

	raw_spin_lock(&rd->rto_lock);

	/* Pass the IPI to the next rt overloaded queue */
	cpu = rto_next_cpu(rd);

	raw_spin_unlock(&rd->rto_lock);

	නම් (cpu < 0) {
		sched_put_rd(rd);
		ආපසු;
	}

	/* Try the next RT overloaded CPU */
	irq_work_queue_on(&rd->rto_push_work, cpu);
}
#endif /* HAVE_RT_PUSH_IPI */

static void pull_rt_task(sa_nracanaa rq *this_rq)
{
	පූර්ණ_සංඛ්‍යා this_cpu = this_rq->cpu, cpu;
	bool resched = false;
	sa_nracanaa task_struct *p, *push_task;
	sa_nracanaa rq *src_rq;
	පූර්ණ_සංඛ්‍යා rt_overload_count = rt_overloaded(this_rq);

	නම් (likely(!rt_overload_count))
		ආපසු;

	/*
	 * Match the barrier from rt_set_overloaded; this guarantees that if we
	 * see overloaded we must also see the rto_mask bit.
	 */
	smp_rmb();

	/* If we are the only overloaded CPU do nothing */
	නම් (rt_overload_count == 1 &&
	    cpumask_test_cpu(this_rq->cpu, this_rq->rd->rto_mask))
		ආපසු;

#ifdef HAVE_RT_PUSH_IPI
	නම් (sched_feat(RT_PUSH_IPI)) {
		tell_cpu_to_push(this_rq);
		ආපසු;
	}
#endif

	for_each_cpu(cpu, this_rq->rd->rto_mask) {
		නම් (this_cpu == cpu)
			continue;

		src_rq = cpu_rq(cpu);

		/*
		 * Don't bother taking the src_rq->lock if the next highest
		 * task is known to be lower-priority than our current task.
		 * This may look racy, but if this value is about to go
		 * logically higher, the src_rq will push this task away.
		 * And if its going logically lower, we do not care
		 */
		නම් (src_rq->rt.highest_prio.next >=
		    this_rq->rt.highest_prio.curr)
			continue;

		/*
		 * We can potentially drop this_rq's lock in
		 * double_lock_balance, and another CPU could
		 * alter this_rq
		 */
		push_task = NULL;
		double_lock_balance(this_rq, src_rq);

		/*
		 * We can pull only a task, which is pushable
		 * on its rq, and no others.
		 */
		p = pick_highest_pushable_task(src_rq, this_cpu);

		/*
		 * Do we have an RT task that preempts
		 * the to-be-scheduled task?
		 */
		නම් (p && (p->prio < this_rq->rt.highest_prio.curr)) {
			WARN_ON(p == src_rq->curr);
			WARN_ON(!task_on_rq_queued(p));

			/*
			 * There's a chance that p is higher in priority
			 * than what's currently running on its CPU.
			 * This is just that p is waking up and hasn't
			 * had a chance to schedule. We only pull
			 * p if it is lower in priority than the
			 * current task on the run queue
			 */
			නම් (p->prio < src_rq->donor->prio)
				goto skip;

			නම් (is_migration_disabled(p)) {
				push_task = get_push_task(src_rq);
			} නැතහොත් {
				move_queued_task_locked(src_rq, this_rq, p);
				resched = true;
			}
			/*
			 * We continue with the search, just in
			 * case there's an even higher prio task
			 * in another runqueue. (low likelihood
			 * but possible)
			 */
		}
skip:
		double_unlock_balance(this_rq, src_rq);

		නම් (push_task) {
			preempt_disable();
			raw_spin_rq_unlock(this_rq);
			stop_one_cpu_nowait(src_rq->cpu, push_cpu_stop,
					    push_task, &src_rq->push_work);
			preempt_enable();
			raw_spin_rq_lock(this_rq);
		}
	}

	නම් (resched)
		resched_curr(this_rq);
}

/*
 * If we are not running and we are not going to reschedule soon, we should
 * try to push tasks away now
 */
static void task_woken_rt(sa_nracanaa rq *rq, sa_nracanaa task_struct *p)
{
	bool need_to_push = !task_on_cpu(rq, p) &&
			    !test_tsk_need_resched(rq->curr) &&
			    p->nr_cpus_allowed > 1 &&
			    (dl_task(rq->donor) || rt_task(rq->donor)) &&
			    (rq->curr->nr_cpus_allowed < 2 ||
			     rq->donor->prio <= p->prio);

	නම් (need_to_push)
		push_rt_tasks(rq);
}

/* Assumes rq->lock is held */
static void rq_online_rt(sa_nracanaa rq *rq)
{
	නම් (rq->rt.overloaded)
		rt_set_overload(rq);

	__enable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, rq->rt.highest_prio.curr);
}

/* Assumes rq->lock is held */
static void rq_offline_rt(sa_nracanaa rq *rq)
{
	නම් (rq->rt.overloaded)
		rt_clear_overload(rq);

	__disable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, CPUPRI_INVALID);
}

/*
 * When switch from the rt queue, we bring ourselves to a position
 * that we might want to pull RT tasks from other runqueues.
 */
static void switched_from_rt(sa_nracanaa rq *rq, sa_nracanaa task_struct *p)
{
	/*
	 * If there are other RT tasks then we will reschedule
	 * and the scheduling of the other RT tasks will handle
	 * the balancing. But if we are the last RT task
	 * we may need to handle the pulling of RT tasks
	 * now.
	 */
	නම් (!task_on_rq_queued(p) || rq->rt.rt_nr_running)
		ආපසු;

	rt_queue_pull_task(rq);
}

void __init init_sched_rt_class(void)
{
	unsigned පූර්ණ_සංඛ්‍යා i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_node(&per_cpu(local_cpu_mask, i),
					GFP_KERNEL, cpu_to_node(i));
	}
}

/*
 * When switching a task to RT, we may overload the runqueue
 * with RT tasks. In this case we try to push them off to
 * other runqueues.
 */
static void switched_to_rt(sa_nracanaa rq *rq, sa_nracanaa task_struct *p)
{
	/*
	 * If we are running, update the avg_rt tracking, as the running time
	 * will now on be accounted into the latter.
	 */
	නම් (task_current(rq, p)) {
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);
		ආපසු;
	}

	/*
	 * If we are not running we may need to preempt the current
	 * running task. If that current running task is also an RT task
	 * then see if we can move to another run queue.
	 */
	නම් (task_on_rq_queued(p)) {
		නම් (p->nr_cpus_allowed > 1 && rq->rt.overloaded)
			rt_queue_push_tasks(rq);
		නම් (p->prio < rq->donor->prio && cpu_online(cpu_of(rq)))
			resched_curr(rq);
	}
}

/*
 * Priority of the task has changed. This may cause
 * us to initiate a push or pull.
 */
static void
prio_changed_rt(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, u64 oldprio)
{
	නම් (!task_on_rq_queued(p))
		ආපසු;

	නම් (p->prio == oldprio)
		ආපසු;

	නම් (task_current_donor(rq, p)) {
		/*
		 * If our priority decreases while running, we
		 * may need to pull tasks to this runqueue.
		 */
		නම් (oldprio < p->prio)
			rt_queue_pull_task(rq);

		/*
		 * If there's a higher priority task waiting to run
		 * then reschedule.
		 */
		නම් (p->prio > rq->rt.highest_prio.curr)
			resched_curr(rq);
	} නැතහොත් {
		/*
		 * This task is not running, but if it is
		 * greater than the current running task
		 * then reschedule.
		 */
		නම් (p->prio < rq->donor->prio)
			resched_curr(rq);
	}
}

#ifdef CONFIG_POSIX_TIMERS
static void watchdog(sa_nracanaa rq *rq, sa_nracanaa task_struct *p)
{
	unsigned long soft, hard;

	/* max may change after cur was read, this will be fixed next tick */
	soft = task_rlimit(p, RLIMIT_RTTIME);
	hard = task_rlimit_max(p, RLIMIT_RTTIME);

	නම් (soft != RLIM_INFINITY) {
		unsigned long next;

		නම් (p->rt.watchdog_stamp != jiffies) {
			p->rt.timeout++;
			p->rt.watchdog_stamp = jiffies;
		}

		next = DIV_ROUND_UP(min(soft, hard), USEC_PER_SEC/HZ);
		නම් (p->rt.timeout > next) {
			posix_cputimers_rt_watchdog(&p->posix_cputimers,
						    p->se.sum_exec_runtime);
		}
	}
}
#නැතහොත් /* !CONFIG_POSIX_TIMERS: */
static anta_hstha void watchdog(sa_nracanaa rq *rq, sa_nracanaa task_struct *p) { }
#endif /* !CONFIG_POSIX_TIMERS */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
static void task_tick_rt(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, පූර්ණ_සංඛ්‍යා queued)
{
	sa_nracanaa sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	watchdog(rq, p);

	/*
	 * RR tasks need a special form of time-slice management.
	 * FIFO tasks have no timeslices.
	 */
	නම් (p->policy != SCHED_RR)
		ආපසු;

	නම් (--p->rt.time_slice)
		ආපසු;

	p->rt.time_slice = sched_rr_timeslice;

	/*
	 * Requeue to the end of queue if we (and all of our ancestors) are not
	 * the only element on the queue
	 */
	for_each_sched_rt_entity(rt_se) {
		නම් (rt_se->run_list.prev != rt_se->run_list.next) {
			requeue_task_rt(rq, p, 0);
			resched_curr(rq);
			ආපසු;
		}
	}
}

static unsigned පූර්ණ_සංඛ්‍යා get_rr_interval_rt(sa_nracanaa rq *rq, sa_nracanaa task_struct *task)
{
	/*
	 * Time slice is 0 for SCHED_FIFO tasks
	 */
	නම් (task->policy == SCHED_RR)
		ආපසු sched_rr_timeslice;
	නැතහොත්
		ආපසු 0;
}

#ifdef CONFIG_SCHED_CORE
static පූර්ණ_සංඛ්‍යා task_is_throttled_rt(sa_nracanaa task_struct *p, පූර්ණ_සංඛ්‍යා cpu)
{
	sa_nracanaa rt_rq *rt_rq;

#ifdef CONFIG_RT_GROUP_SCHED // XXX maybe add task_rt_rq(), see also sched_rt_period_rt_rq
	rt_rq = task_group(p)->rt_rq[cpu];
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &root_task_group);
#නැතහොත්
	rt_rq = &cpu_rq(cpu)->rt;
#endif

	ආපසු rt_rq_throttled(rt_rq);
}
#endif /* CONFIG_SCHED_CORE */

DEFINE_SCHED_CLASS(rt) = {
	.enqueue_task		= enqueue_task_rt,
	.dequeue_task		= dequeue_task_rt,
	.yield_task		= yield_task_rt,

	.wakeup_preempt		= wakeup_preempt_rt,

	.pick_task		= pick_task_rt,
	.put_prev_task		= put_prev_task_rt,
	.set_next_task          = set_next_task_rt,

	.balance		= balance_rt,
	.select_task_rq		= select_task_rq_rt,
	.set_cpus_allowed       = set_cpus_allowed_common,
	.rq_online              = rq_online_rt,
	.rq_offline             = rq_offline_rt,
	.task_woken		= task_woken_rt,
	.switched_from		= switched_from_rt,
	.find_lock_rq		= find_lock_lowest_rq,

	.task_tick		= task_tick_rt,

	.get_rr_interval	= get_rr_interval_rt,

	.switched_to		= switched_to_rt,
	.prio_changed		= prio_changed_rt,

	.update_curr		= update_curr_rt,

#ifdef CONFIG_SCHED_CORE
	.task_is_throttled	= task_is_throttled_rt,
#endif

#ifdef CONFIG_UCLAMP_TASK
	.uclamp_enabled		= 1,
#endif
};

#ifdef CONFIG_RT_GROUP_SCHED
/*
 * Ensure that the real time constraints are schedulable.
 */
static DEFINE_MUTEX(rt_constraints_mutex);

static anta_hstha පූර්ණ_සංඛ්‍යා tg_has_rt_tasks(sa_nracanaa task_group *tg)
{
	sa_nracanaa task_struct *task;
	sa_nracanaa css_task_iter it;
	පූර්ණ_සංඛ්‍යා ret = 0;

	/*
	 * Autogroups do not have RT tasks; see autogroup_create().
	 */
	නම් (task_group_is_autogroup(tg))
		ආපසු 0;

	css_task_iter_start(&tg->css, 0, &it);
	අතරතුර (!ret && (task = css_task_iter_next(&it)))
		ret |= rt_task(task);
	css_task_iter_end(&it);

	ආපසු ret;
}

sa_nracanaa rt_schedulable_data {
	sa_nracanaa task_group *tg;
	u64 rt_period;
	u64 rt_runtime;
};

static පූර්ණ_සංඛ්‍යා tg_rt_schedulable(sa_nracanaa task_group *tg, void *data)
{
	sa_nracanaa rt_schedulable_data *d = data;
	sa_nracanaa task_group *child;
	u64 total, sum = 0;
	u64 period, runtime;

	period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	runtime = tg->rt_bandwidth.rt_runtime;

	නම් (tg == d->tg) {
		period = d->rt_period;
		runtime = d->rt_runtime;
	}

	/*
	 * Cannot have more runtime than the period.
	 */
	නම් (runtime > period && runtime != RUNTIME_INF)
		ආපසු -EINVAL;

	/*
	 * Ensure we don't starve existing RT tasks if runtime turns zero.
	 */
	නම් (rt_bandwidth_enabled() && !runtime &&
	    tg->rt_bandwidth.rt_runtime && tg_has_rt_tasks(tg))
		ආපසු -EBUSY;

	total = to_ratio(period, runtime);

	/*
	 * Nobody can have more than the global setting allows.
	 */
	නම් (total > to_ratio(global_rt_period(), global_rt_runtime()))
		ආපසු -EINVAL;

	/*
	 * The sum of our children's runtime should not exceed our own.
	 */
	list_for_each_entry_rcu(child, &tg->children, siblings) {
		period = ktime_to_ns(child->rt_bandwidth.rt_period);
		runtime = child->rt_bandwidth.rt_runtime;

		නම් (child == d->tg) {
			period = d->rt_period;
			runtime = d->rt_runtime;
		}

		sum += to_ratio(period, runtime);
	}

	නම් (sum > total)
		ආපසු -EINVAL;

	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා __rt_schedulable(sa_nracanaa task_group *tg, u64 period, u64 runtime)
{
	පූර්ණ_සංඛ්‍යා ret;

	sa_nracanaa rt_schedulable_data data = {
		.tg = tg,
		.rt_period = period,
		.rt_runtime = runtime,
	};

	rcu_read_lock();
	ret = walk_tg_tree(tg_rt_schedulable, tg_nop, &data);
	rcu_read_unlock();

	ආපසු ret;
}

static පූර්ණ_සංඛ්‍යා tg_set_rt_bandwidth(sa_nracanaa task_group *tg,
		u64 rt_period, u64 rt_runtime)
{
	පූර්ණ_සංඛ්‍යා i, err = 0;

	/*
	 * Disallowing the root group RT runtime is BAD, it would disallow the
	 * kernel creating (and or operating) RT threads.
	 */
	නම් (tg == &root_task_group && rt_runtime == 0)
		ආපසු -EINVAL;

	/* No period doesn't make any sense. */
	නම් (rt_period == 0)
		ආපසු -EINVAL;

	/*
	 * Bound quota to defend quota against overflow during bandwidth shift.
	 */
	නම් (rt_runtime != RUNTIME_INF && rt_runtime > max_rt_runtime)
		ආපසු -EINVAL;

	mutex_lock(&rt_constraints_mutex);
	err = __rt_schedulable(tg, rt_period, rt_runtime);
	නම් (err)
		goto unlock;

	raw_spin_lock_irq(&tg->rt_bandwidth.rt_runtime_lock);
	tg->rt_bandwidth.rt_period = ns_to_ktime(rt_period);
	tg->rt_bandwidth.rt_runtime = rt_runtime;

	for_each_possible_cpu(i) {
		sa_nracanaa rt_rq *rt_rq = tg->rt_rq[i];

		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_runtime;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
	}
	raw_spin_unlock_irq(&tg->rt_bandwidth.rt_runtime_lock);
unlock:
	mutex_unlock(&rt_constraints_mutex);

	ආපසු err;
}

පූර්ණ_සංඛ්‍යා sched_group_set_rt_runtime(sa_nracanaa task_group *tg, long rt_runtime_us)
{
	u64 rt_runtime, rt_period;

	rt_period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	rt_runtime = (u64)rt_runtime_us * NSEC_PER_USEC;
	නම් (rt_runtime_us < 0)
		rt_runtime = RUNTIME_INF;
	නැතහොත් නම් ((u64)rt_runtime_us > U64_MAX / NSEC_PER_USEC)
		ආපසු -EINVAL;

	ආපසු tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

long sched_group_rt_runtime(sa_nracanaa task_group *tg)
{
	u64 rt_runtime_us;

	නම් (tg->rt_bandwidth.rt_runtime == RUNTIME_INF)
		ආපසු -1;

	rt_runtime_us = tg->rt_bandwidth.rt_runtime;
	do_div(rt_runtime_us, NSEC_PER_USEC);
	ආපසු rt_runtime_us;
}

පූර්ණ_සංඛ්‍යා sched_group_set_rt_period(sa_nracanaa task_group *tg, u64 rt_period_us)
{
	u64 rt_runtime, rt_period;

	නම් (rt_period_us > U64_MAX / NSEC_PER_USEC)
		ආපසු -EINVAL;

	rt_period = rt_period_us * NSEC_PER_USEC;
	rt_runtime = tg->rt_bandwidth.rt_runtime;

	ආපසු tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

long sched_group_rt_period(sa_nracanaa task_group *tg)
{
	u64 rt_period_us;

	rt_period_us = ktime_to_ns(tg->rt_bandwidth.rt_period);
	do_div(rt_period_us, NSEC_PER_USEC);
	ආපසු rt_period_us;
}

පූර්ණ_සංඛ්‍යා sched_rt_can_attach(sa_nracanaa task_group *tg, sa_nracanaa task_struct *tsk)
{
	/* Don't accept real-time tasks when there is no way for them to run */
	නම් (rt_group_sched_enabled() && rt_task(tsk) && tg->rt_bandwidth.rt_runtime == 0)
		ආපසු 0;

	ආපසු 1;
}

#endif /* !CONFIG_RT_GROUP_SCHED */

#ifdef CONFIG_SYSCTL
static පූර්ණ_සංඛ්‍යා sched_rt_global_validate(void)
{
	නම් ((sysctl_sched_rt_runtime != RUNTIME_INF) &&
		((sysctl_sched_rt_runtime > sysctl_sched_rt_period) ||
		 ((u64)sysctl_sched_rt_runtime *
			NSEC_PER_USEC > max_rt_runtime)))
		ආපසු -EINVAL;

#ifdef CONFIG_RT_GROUP_SCHED
	නම් (!rt_group_sched_enabled())
		ආපසු 0;

	scoped_guard(mutex, &rt_constraints_mutex)
		ආපසු __rt_schedulable(NULL, 0, 0);
#endif
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා sched_rt_handler(const sa_nracanaa ctl_table *table, පූර්ණ_සංඛ්‍යා write, void *buffer,
		size_t *lenp, loff_t *ppos)
{
	පූර්ණ_සංඛ්‍යා old_period, old_runtime;
	static DEFINE_MUTEX(mutex);
	පූර්ණ_සංඛ්‍යා ret;

	mutex_lock(&mutex);
	sched_domains_mutex_lock();
	old_period = sysctl_sched_rt_period;
	old_runtime = sysctl_sched_rt_runtime;

	ret = proc_dointvec_minmax(table, write, buffer, lenp, ppos);

	නම් (!ret && write) {
		ret = sched_rt_global_validate();
		නම් (ret)
			goto undo;

		ret = sched_dl_global_validate();
		නම් (ret)
			goto undo;

		sched_dl_do_global();
	}
	නම් (0) {
undo:
		sysctl_sched_rt_period = old_period;
		sysctl_sched_rt_runtime = old_runtime;
	}
	sched_domains_mutex_unlock();
	mutex_unlock(&mutex);

	/*
	 * After changing maximum available bandwidth for DEADLINE, we need to
	 * recompute per root domain and per cpus variables accordingly.
	 */
	rebuild_sched_domains();

	ආපසු ret;
}

static පූර්ණ_සංඛ්‍යා sched_rr_handler(const sa_nracanaa ctl_table *table, පූර්ණ_සංඛ්‍යා write, void *buffer,
		size_t *lenp, loff_t *ppos)
{
	පූර්ණ_සංඛ්‍යා ret;
	static DEFINE_MUTEX(mutex);

	mutex_lock(&mutex);
	ret = proc_dointvec(table, write, buffer, lenp, ppos);
	/*
	 * Make sure that internally we keep jiffies.
	 * Also, writing zero resets the time-slice to default:
	 */
	නම් (!ret && write) {
		sched_rr_timeslice =
			sysctl_sched_rr_timeslice <= 0 ? RR_TIMESLICE :
			msecs_to_jiffies(sysctl_sched_rr_timeslice);

		නම් (sysctl_sched_rr_timeslice <= 0)
			sysctl_sched_rr_timeslice = jiffies_to_msecs(RR_TIMESLICE);
	}
	mutex_unlock(&mutex);

	ආපසු ret;
}
#endif /* CONFIG_SYSCTL */

void print_rt_stats(sa_nracanaa seq_file *m, පූර්ණ_සංඛ්‍යා cpu)
{
	rt_rq_iter_t iter;
	sa_nracanaa rt_rq *rt_rq;

	rcu_read_lock();
	for_each_rt_rq(rt_rq, iter, cpu_rq(cpu))
		print_rt_rq(m, cpu, rt_rq);
	rcu_read_unlock();
}
