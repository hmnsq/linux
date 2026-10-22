// SPDX-License-Identifier: GPL-2.0
/*
 * Real-Time Scheduling Class (mapped to the SCHED_FIFO and SCHED_RR
 * policies)
 */

#include "sched.h"
#include "pelt.h"

ཧྲིལ sched_rr_timeslice = RR_TIMESLICE;
/* More than 4 hours if BW_SHIFT equals 20. */
static const u64 max_rt_runtime = MAX_BW;

/*
 * period over which we measure -rt task CPU usage in us.
 * default: 1s
 */
ཧྲིལ sysctl_sched_rt_period = 1000000;

/*
 * part of the period that we allow rt tasks to run in us.
 * default: 1s
 */
ཧྲིལ sysctl_sched_rt_runtime = 1000000;

#ifdef CONFIG_SYSCTL
static ཧྲིལ sysctl_sched_rr_timeslice = (MSEC_PER_SEC * RR_TIMESLICE) / HZ;
static ཧྲིལ sched_rt_handler(const sa_nracanaa ctl_table *table, ཧྲིལ write, void *buffer,
		size_t *lenp, loff_t *ppos);
static ཧྲིལ sched_rr_handler(const sa_nracanaa ctl_table *table, ཧྲིལ write, void *buffer,
		size_t *lenp, loff_t *ppos);
static const sa_nracanaa ctl_table sched_rt_sysctls[] = {
	{
		.procname       = "sched_rt_period_us",
		.data           = &sysctl_sched_rt_period,
		.maxlen         = parimaa_na(ཧྲིལ),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_ONE,
		.extra2         = SYSCTL_INT_MAX,
	},
	{
		.procname       = "sched_rt_runtime_us",
		.data           = &sysctl_sched_rt_runtime,
		.maxlen         = parimaa_na(ཧྲིལ),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_NEG_ONE,
		.extra2         = (void *)&sysctl_sched_rt_period,
	},
	{
		.procname       = "sched_rr_timeslice_ms",
		.data           = &sysctl_sched_rr_timeslice,
		.maxlen         = parimaa_na(ཧྲིལ),
		.mode           = 0644,
		.proc_handler   = sched_rr_handler,
	},
};

static ཧྲིལ __init sched_rt_sysctl_init(void)
{
	register_sysctl_init("kernel", sched_rt_sysctls);
	ལོག 0;
}
late_initcall(sched_rt_sysctl_init);
#endif /* CONFIG_SYSCTL */

void init_rt_rq(sa_nracanaa rt_rq *rt_rq)
{
	sa_nracanaa rt_prio_array *array;
	ཧྲིལ i;

	array = &rt_rq->active;
	དོན་ལུ (i = 0; i < MAX_RT_PRIO; i++) {
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

static ཧྲིལ do_sched_rt_period_timer(sa_nracanaa rt_bandwidth *rt_b, ཧྲིལ overrun);

static ga_nanaa hrtimer_restart sched_rt_period_timer(sa_nracanaa hrtimer *timer)
{
	sa_nracanaa rt_bandwidth *rt_b =
		container_of(timer, sa_nracanaa rt_bandwidth, rt_period_timer);
	ཧྲིལ idle = 0;
	ཧྲིལ overrun;

	raw_spin_lock(&rt_b->rt_runtime_lock);
	དོན་ལུ (;;) {
		overrun = hrtimer_forward_now(timer, rt_b->rt_period);
		གལ་སྲིད (!overrun)
			bha_nga;

		raw_spin_unlock(&rt_b->rt_runtime_lock);
		idle = do_sched_rt_period_timer(rt_b, overrun);
		raw_spin_lock(&rt_b->rt_runtime_lock);
	}
	གལ་སྲིད (idle)
		rt_b->rt_period_active = 0;
	raw_spin_unlock(&rt_b->rt_runtime_lock);

	ལོག idle ? HRTIMER_NORESTART : HRTIMER_RESTART;
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
	གལ་སྲིད (!rt_b->rt_period_active) {
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
	གལ་སྲིད (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF)
		ལོག;

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

	ལོག container_of(rt_se, sa_nracanaa task_struct, rt);
}

static anta_hstha sa_nracanaa rq *rq_of_rt_rq(sa_nracanaa rt_rq *rt_rq)
{
	/* Cannot fold with non-CONFIG_RT_GROUP_SCHED version, layout */
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &root_task_group);
	ལོག rt_rq->rq;
}

static anta_hstha sa_nracanaa rt_rq *rt_rq_of_se(sa_nracanaa sched_rt_entity *rt_se)
{
	WARN_ON(!rt_group_sched_enabled() && rt_se->rt_rq->tg != &root_task_group);
	ལོག rt_se->rt_rq;
}

static anta_hstha sa_nracanaa rq *rq_of_rt_se(sa_nracanaa sched_rt_entity *rt_se)
{
	sa_nracanaa rt_rq *rt_rq = rt_se->rt_rq;

	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &root_task_group);
	ལོག rt_rq->rq;
}

void unregister_rt_sched_group(sa_nracanaa task_group *tg)
{
	གལ་སྲིད (!rt_group_sched_enabled())
		ལོག;

	གལ་སྲིད (tg->rt_se)
		destroy_rt_bandwidth(&tg->rt_bandwidth);
}

void free_rt_sched_group(sa_nracanaa task_group *tg)
{
	ཧྲིལ i;

	གལ་སྲིད (!rt_group_sched_enabled())
		ལོག;

	for_each_possible_cpu(i) {
		གལ་སྲིད (tg->rt_rq)
			kfree(tg->rt_rq[i]);
		གལ་སྲིད (tg->rt_se)
			kfree(tg->rt_se[i]);
	}

	kfree(tg->rt_rq);
	kfree(tg->rt_se);
}

void init_tg_rt_entry(sa_nracanaa task_group *tg, sa_nracanaa rt_rq *rt_rq,
		sa_nracanaa sched_rt_entity *rt_se, ཧྲིལ cpu,
		sa_nracanaa sched_rt_entity *parent)
{
	sa_nracanaa rq *rq = cpu_rq(cpu);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->rt_nr_boosted = 0;
	rt_rq->rq = rq;
	rt_rq->tg = tg;

	tg->rt_rq[cpu] = rt_rq;
	tg->rt_se[cpu] = rt_se;

	གལ་སྲིད (!rt_se)
		ལོག;

	གལ་སྲིད (!parent)
		rt_se->rt_rq = &rq->rt;
	དེ་མིན
		rt_se->rt_rq = parent->my_q;

	rt_se->my_q = rt_rq;
	rt_se->parent = parent;
	INIT_LIST_HEAD(&rt_se->run_list);
}

ཧྲིལ alloc_rt_sched_group(sa_nracanaa task_group *tg, sa_nracanaa task_group *parent)
{
	sa_nracanaa rt_rq *rt_rq;
	sa_nracanaa sched_rt_entity *rt_se;
	ཧྲིལ i;

	གལ་སྲིད (!rt_group_sched_enabled())
		ལོག 1;

	tg->rt_rq = kzalloc_objs(rt_rq, nr_cpu_ids);
	གལ་སྲིད (!tg->rt_rq)
		goto err;
	tg->rt_se = kzalloc_objs(rt_se, nr_cpu_ids);
	གལ་སྲིད (!tg->rt_se)
		goto err;

	init_rt_bandwidth(&tg->rt_bandwidth, ktime_to_ns(global_rt_period()), 0);

	for_each_possible_cpu(i) {
		rt_rq = kzalloc_node(parimaa_na(sa_nracanaa rt_rq),
				     GFP_KERNEL, cpu_to_node(i));
		གལ་སྲིད (!rt_rq)
			goto err;

		rt_se = kzalloc_node(parimaa_na(sa_nracanaa sched_rt_entity),
				     GFP_KERNEL, cpu_to_node(i));
		གལ་སྲིད (!rt_se)
			goto err_free_rq;

		init_rt_rq(rt_rq);
		rt_rq->rt_runtime = tg->rt_bandwidth.rt_runtime;
		init_tg_rt_entry(tg, rt_rq, rt_se, i, parent->rt_se[i]);
	}

	ལོག 1;

err_free_rq:
	kfree(rt_rq);
err:
	ལོག 0;
}

#དེ་མིན /* !CONFIG_RT_GROUP_SCHED: */

#define rt_entity_is_task(rt_se) (1)

static anta_hstha sa_nracanaa task_struct *rt_task_of(sa_nracanaa sched_rt_entity *rt_se)
{
	ལོག container_of(rt_se, sa_nracanaa task_struct, rt);
}

static anta_hstha sa_nracanaa rq *rq_of_rt_rq(sa_nracanaa rt_rq *rt_rq)
{
	ལོག container_of(rt_rq, sa_nracanaa rq, rt);
}

static anta_hstha sa_nracanaa rq *rq_of_rt_se(sa_nracanaa sched_rt_entity *rt_se)
{
	sa_nracanaa task_struct *p = rt_task_of(rt_se);

	ལོག task_rq(p);
}

static anta_hstha sa_nracanaa rt_rq *rt_rq_of_se(sa_nracanaa sched_rt_entity *rt_se)
{
	sa_nracanaa rq *rq = rq_of_rt_se(rt_se);

	ལོག &rq->rt;
}

void unregister_rt_sched_group(sa_nracanaa task_group *tg) { }

void free_rt_sched_group(sa_nracanaa task_group *tg) { }

ཧྲིལ alloc_rt_sched_group(sa_nracanaa task_group *tg, sa_nracanaa task_group *parent)
{
	ལོག 1;
}
#endif /* !CONFIG_RT_GROUP_SCHED */

static anta_hstha bool need_pull_rt_task(sa_nracanaa rq *rq, sa_nracanaa task_struct *prev)
{
	/* Try to pull RT tasks here if we lower this rq's prio */
	ལོག rq->online && rq->rt.highest_prio.curr > prev->prio;
}

static anta_hstha ཧྲིལ rt_overloaded(sa_nracanaa rq *rq)
{
	ལོག atomic_read(&rq->rd->rto_count);
}

static anta_hstha void rt_set_overload(sa_nracanaa rq *rq)
{
	གལ་སྲིད (!rq->online)
		ལོག;

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
	གལ་སྲིད (!rq->online)
		ལོག;

	/* the order here really doesn't matter */
	atomic_dec(&rq->rd->rto_count);
	cpumask_clear_cpu(rq->cpu, rq->rd->rto_mask);
}

static anta_hstha ཧྲིལ has_pushable_tasks(sa_nracanaa rq *rq)
{
	ལོག !plist_head_empty(&rq->rt.pushable_tasks);
}

static DEFINE_PER_CPU(sa_nracanaa balance_callback, rt_push_head);
static DEFINE_PER_CPU(sa_nracanaa balance_callback, rt_pull_head);

static void push_rt_tasks(sa_nracanaa rq *);
static void pull_rt_task(sa_nracanaa rq *);

static anta_hstha void rt_queue_push_tasks(sa_nracanaa rq *rq)
{
	གལ་སྲིད (!has_pushable_tasks(rq))
		ལོག;

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
	གལ་སྲིད (p->prio < rq->rt.highest_prio.next)
		rq->rt.highest_prio.next = p->prio;

	གལ་སྲིད (!rq->rt.overloaded) {
		rt_set_overload(rq);
		rq->rt.overloaded = 1;
	}
}

static void dequeue_pushable_task(sa_nracanaa rq *rq, sa_nracanaa task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the new highest prio pushable task */
	གལ་སྲིད (has_pushable_tasks(rq)) {
		p = plist_first_entry(&rq->rt.pushable_tasks,
				      sa_nracanaa task_struct, pushable_tasks);
		rq->rt.highest_prio.next = p->prio;
	} དེ་མིན {
		rq->rt.highest_prio.next = MAX_RT_PRIO-1;

		གལ་སྲིད (rq->rt.overloaded) {
			rt_clear_overload(rq);
			rq->rt.overloaded = 0;
		}
	}
}

static void enqueue_top_rt_rq(sa_nracanaa rt_rq *rt_rq);
static void dequeue_top_rt_rq(sa_nracanaa rt_rq *rt_rq, unsigned ཧྲིལ count);

static anta_hstha ཧྲིལ on_rt_rq(sa_nracanaa sched_rt_entity *rt_se)
{
	ལོག rt_se->on_rq;
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
static anta_hstha bool rt_task_fits_capacity(sa_nracanaa task_struct *p, ཧྲིལ cpu)
{
	unsigned ཧྲིལ min_cap;
	unsigned ཧྲིལ max_cap;
	unsigned ཧྲིལ cpu_cap;

	/* Only heterogeneous systems can benefit from this check */
	གལ་སྲིད (!sched_asym_cpucap_active())
		ལོག true;

	min_cap = uclamp_eff_value(p, UCLAMP_MIN);
	max_cap = uclamp_eff_value(p, UCLAMP_MAX);

	cpu_cap = arch_scale_cpu_capacity(cpu);

	ལོག cpu_cap >= min(min_cap, max_cap);
}
#དེ་མིན /* !CONFIG_UCLAMP_TASK: */
static anta_hstha bool rt_task_fits_capacity(sa_nracanaa task_struct *p, ཧྲིལ cpu)
{
	ལོག true;
}
#endif /* !CONFIG_UCLAMP_TASK */

#ifdef CONFIG_RT_GROUP_SCHED

static anta_hstha u64 sched_rt_runtime(sa_nracanaa rt_rq *rt_rq)
{
	ལོག rt_rq->rt_runtime;
}

static anta_hstha u64 sched_rt_period(sa_nracanaa rt_rq *rt_rq)
{
	ལོག ktime_to_ns(rt_rq->tg->rt_bandwidth.rt_period);
}

typedef sa_nracanaa task_group *rt_rq_iter_t;

static anta_hstha sa_nracanaa task_group *next_task_group(sa_nracanaa task_group *tg)
{
	གལ་སྲིད (!rt_group_sched_enabled()) {
		WARN_ON(tg != &root_task_group);
		ལོག NULL;
	}

	do {
		tg = list_entry_rcu(tg->list.next,
			typeof(sa_nracanaa task_group), list);
	} སྐབས (&tg->list != &task_groups && task_group_is_autogroup(tg));

	གལ་སྲིད (&tg->list == &task_groups)
		tg = NULL;

	ལོག tg;
}

#define for_each_rt_rq(rt_rq, iter, rq)					\
	དོན་ལུ (iter = &root_task_group;					\
		iter && (rt_rq = iter->rt_rq[cpu_of(rq)]);		\
		iter = next_task_group(iter))

#define for_each_sched_rt_entity(rt_se) \
	དོན་ལུ (; rt_se; rt_se = rt_se->parent)

static anta_hstha sa_nracanaa rt_rq *group_rt_rq(sa_nracanaa sched_rt_entity *rt_se)
{
	ལོག rt_se->my_q;
}

static void enqueue_rt_entity(sa_nracanaa sched_rt_entity *rt_se, unsigned ཧྲིལ flags);
static void dequeue_rt_entity(sa_nracanaa sched_rt_entity *rt_se, unsigned ཧྲིལ flags);

static void sched_rt_rq_enqueue(sa_nracanaa rt_rq *rt_rq)
{
	sa_nracanaa task_struct *donor = rq_of_rt_rq(rt_rq)->donor;
	sa_nracanaa rq *rq = rq_of_rt_rq(rt_rq);
	sa_nracanaa sched_rt_entity *rt_se;

	ཧྲིལ cpu = cpu_of(rq);

	rt_se = rt_rq->tg->rt_se[cpu];

	གལ་སྲིད (rt_rq->rt_nr_running) {
		གལ་སྲིད (!rt_se)
			enqueue_top_rt_rq(rt_rq);
		དེ་མིན གལ་སྲིད (!on_rt_rq(rt_se))
			enqueue_rt_entity(rt_se, 0);

		གལ་སྲིད (rt_rq->highest_prio.curr < donor->prio)
			resched_curr(rq);
	}
}

static void sched_rt_rq_dequeue(sa_nracanaa rt_rq *rt_rq)
{
	sa_nracanaa sched_rt_entity *rt_se;
	ཧྲིལ cpu = cpu_of(rq_of_rt_rq(rt_rq));

	rt_se = rt_rq->tg->rt_se[cpu];

	གལ་སྲིད (!rt_se) {
		dequeue_top_rt_rq(rt_rq, rt_rq->rt_nr_running);
		/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
		cpufreq_update_util(rq_of_rt_rq(rt_rq), 0);
	}
	དེ་མིན གལ་སྲིད (on_rt_rq(rt_se))
		dequeue_rt_entity(rt_se, 0);
}

static anta_hstha ཧྲིལ rt_rq_throttled(sa_nracanaa rt_rq *rt_rq)
{
	ལོག rt_rq->rt_throttled && !rt_rq->rt_nr_boosted;
}

static ཧྲིལ rt_se_boosted(sa_nracanaa sched_rt_entity *rt_se)
{
	sa_nracanaa rt_rq *rt_rq = group_rt_rq(rt_se);
	sa_nracanaa task_struct *p;

	གལ་སྲིད (rt_rq)
		ལོག !!rt_rq->rt_nr_boosted;

	p = rt_task_of(rt_se);
	ལོག p->prio != p->normal_prio;
}

static anta_hstha const sa_nracanaa cpumask *sched_rt_period_mask(void)
{
	ལོག this_rq()->rd->span;
}

static anta_hstha
sa_nracanaa rt_rq *sched_rt_period_rt_rq(sa_nracanaa rt_bandwidth *rt_b, ཧྲིལ cpu)
{
	ལོག container_of(rt_b, sa_nracanaa task_group, rt_bandwidth)->rt_rq[cpu];
}

static anta_hstha sa_nracanaa rt_bandwidth *sched_rt_bandwidth(sa_nracanaa rt_rq *rt_rq)
{
	ལོག &rt_rq->tg->rt_bandwidth;
}

bool sched_rt_bandwidth_account(sa_nracanaa rt_rq *rt_rq)
{
	sa_nracanaa rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

	ལོག (hrtimer_active(&rt_b->rt_period_timer) ||
		rt_rq->rt_time < rt_b->rt_runtime);
}

/*
 * We ran out of runtime, see if we can borrow some from our neighbours.
 */
static void do_balance_runtime(sa_nracanaa rt_rq *rt_rq)
{
	sa_nracanaa rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
	sa_nracanaa root_domain *rd = rq_of_rt_rq(rt_rq)->rd;
	ཧྲིལ i, weight;
	u64 rt_period;

	weight = cpumask_weight(rd->span);

	raw_spin_lock(&rt_b->rt_runtime_lock);
	rt_period = ktime_to_ns(rt_b->rt_period);
	for_each_cpu(i, rd->span) {
		sa_nracanaa rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
		s64 diff;

		གལ་སྲིད (iter == rt_rq)
			continue;

		raw_spin_lock(&iter->rt_runtime_lock);
		/*
		 * Either all rqs have inf runtime and there's nothing to steal
		 * or __disable_runtime() below sets a specific rq to inf to
		 * indicate its been disabled and disallow stealing.
		 */
		གལ་སྲིད (iter->rt_runtime == RUNTIME_INF)
			goto next;

		/*
		 * From runqueues with spare time, take 1/n part of their
		 * spare time, but no more than our period.
		 */
		diff = iter->rt_runtime - iter->rt_time;
		གལ་སྲིད (diff > 0) {
			diff = div_u64((u64)diff, weight);
			གལ་སྲིད (rt_rq->rt_runtime + diff > rt_period)
				diff = rt_period - rt_rq->rt_runtime;
			iter->rt_runtime -= diff;
			rt_rq->rt_runtime += diff;
			གལ་སྲིད (rt_rq->rt_runtime == rt_period) {
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

	གལ་སྲིད (unlikely(!scheduler_running))
		ལོག;

	for_each_rt_rq(rt_rq, iter, rq) {
		sa_nracanaa rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
		s64 want;
		ཧྲིལ i;

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		/*
		 * Either we're all inf and nobody needs to borrow, or we're
		 * already disabled and thus have nothing to do, or we have
		 * exactly the right amount of runtime to take out.
		 */
		གལ་སྲིད (rt_rq->rt_runtime == RUNTIME_INF ||
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
			གལ་སྲིད (iter == rt_rq || iter->rt_runtime == RUNTIME_INF)
				continue;

			raw_spin_lock(&iter->rt_runtime_lock);
			གལ་སྲིད (want > 0) {
				diff = min_t(s64, iter->rt_runtime, want);
				iter->rt_runtime -= diff;
				want -= diff;
			} དེ་མིན {
				iter->rt_runtime -= want;
				want -= want;
			}
			raw_spin_unlock(&iter->rt_runtime_lock);

			གལ་སྲིད (!want)
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

	གལ་སྲིད (unlikely(!scheduler_running))
		ལོག;

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
	གལ་སྲིད (!sched_feat(RT_RUNTIME_SHARE))
		ལོག;

	གལ་སྲིད (rt_rq->rt_time > rt_rq->rt_runtime) {
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		do_balance_runtime(rt_rq);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
	}
}

static ཧྲིལ do_sched_rt_period_timer(sa_nracanaa rt_bandwidth *rt_b, ཧྲིལ overrun)
{
	ཧྲིལ i, idle = 1, throttled = 0;
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
	གལ་སྲིད (rt_b == &root_task_group.rt_bandwidth)
		span = cpu_online_mask;

	for_each_cpu(i, span) {
		ཧྲིལ enqueue = 0;
		sa_nracanaa rt_rq *rt_rq = sched_rt_period_rt_rq(rt_b, i);
		sa_nracanaa rq *rq = rq_of_rt_rq(rt_rq);
		sa_nracanaa rq_flags rf;
		ཧྲིལ skip;

		/*
		 * When span == cpu_online_mask, taking each rq->lock
		 * can be time-consuming. Try to avoid it when possible.
		 */
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		གལ་སྲིད (!sched_feat(RT_RUNTIME_SHARE) && rt_rq->rt_runtime != RUNTIME_INF)
			rt_rq->rt_runtime = rt_b->rt_runtime;
		skip = !rt_rq->rt_time && !rt_rq->rt_nr_running;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		གལ་སྲིད (skip)
			continue;

		rq_lock(rq, &rf);
		update_rq_clock(rq);

		གལ་སྲིད (rt_rq->rt_time) {
			u64 runtime;

			raw_spin_lock(&rt_rq->rt_runtime_lock);
			གལ་སྲིད (rt_rq->rt_throttled)
				balance_runtime(rt_rq);
			runtime = rt_rq->rt_runtime;
			rt_rq->rt_time -= min(rt_rq->rt_time, overrun*runtime);
			གལ་སྲིད (rt_rq->rt_throttled && rt_rq->rt_time < runtime) {
				rt_rq->rt_throttled = 0;
				enqueue = 1;

				/*
				 * When we're idle and a woken (rt) task is
				 * throttled wakeup_preempt() will set
				 * skip_update and the time between the wakeup
				 * and this unthrottle will get accounted as
				 * 'runtime'.
				 */
				གལ་སྲིད (rt_rq->rt_nr_running && rq->curr == rq->idle)
					rq_clock_cancel_skipupdate(rq);
			}
			གལ་སྲིད (rt_rq->rt_time || rt_rq->rt_nr_running)
				idle = 0;
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
		} དེ་མིན གལ་སྲིད (rt_rq->rt_nr_running) {
			idle = 0;
			གལ་སྲིད (!rt_rq_throttled(rt_rq))
				enqueue = 1;
		}
		གལ་སྲིད (rt_rq->rt_throttled)
			throttled = 1;

		གལ་སྲིད (enqueue)
			sched_rt_rq_enqueue(rt_rq);
		rq_unlock(rq, &rf);
	}

	གལ་སྲིད (!throttled && (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF))
		ལོག 1;

	ལོག idle;
}

static ཧྲིལ sched_rt_runtime_exceeded(sa_nracanaa rt_rq *rt_rq)
{
	u64 runtime = sched_rt_runtime(rt_rq);

	གལ་སྲིད (rt_rq->rt_throttled)
		ལོག rt_rq_throttled(rt_rq);

	གལ་སྲིད (runtime >= sched_rt_period(rt_rq))
		ལོག 0;

	balance_runtime(rt_rq);
	runtime = sched_rt_runtime(rt_rq);
	གལ་སྲིད (runtime == RUNTIME_INF)
		ལོག 0;

	གལ་སྲིད (rt_rq->rt_time > runtime) {
		sa_nracanaa rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		/*
		 * Don't actually throttle groups that have no runtime assigned
		 * but accrue some time due to boosting.
		 */
		གལ་སྲིད (likely(rt_b->rt_runtime)) {
			rt_rq->rt_throttled = 1;
			printk_deferred_once("sched: RT throttling activated\n");
		} དེ་མིན {
			/*
			 * In case we did anyway, make it go away,
			 * replenishment is a joke, since it will replenish us
			 * with exactly 0 ns.
			 */
			rt_rq->rt_time = 0;
		}

		གལ་སྲིད (rt_rq_throttled(rt_rq)) {
			sched_rt_rq_dequeue(rt_rq);
			ལོག 1;
		}
	}

	ལོག 0;
}

#དེ་མིན /* !CONFIG_RT_GROUP_SCHED: */

typedef sa_nracanaa rt_rq *rt_rq_iter_t;

#define for_each_rt_rq(rt_rq, iter, rq) \
	དོན་ལུ ((void) iter, rt_rq = &rq->rt; rt_rq; rt_rq = NULL)

#define for_each_sched_rt_entity(rt_se) \
	དོན་ལུ (; rt_se; rt_se = NULL)

static anta_hstha sa_nracanaa rt_rq *group_rt_rq(sa_nracanaa sched_rt_entity *rt_se)
{
	ལོག NULL;
}

static anta_hstha void sched_rt_rq_enqueue(sa_nracanaa rt_rq *rt_rq)
{
	sa_nracanaa rq *rq = rq_of_rt_rq(rt_rq);

	གལ་སྲིད (!rt_rq->rt_nr_running)
		ལོག;

	enqueue_top_rt_rq(rt_rq);
	resched_curr(rq);
}

static anta_hstha void sched_rt_rq_dequeue(sa_nracanaa rt_rq *rt_rq)
{
	dequeue_top_rt_rq(rt_rq, rt_rq->rt_nr_running);
}

static anta_hstha ཧྲིལ rt_rq_throttled(sa_nracanaa rt_rq *rt_rq)
{
	ལོག false;
}

static anta_hstha const sa_nracanaa cpumask *sched_rt_period_mask(void)
{
	ལོག cpu_online_mask;
}

static anta_hstha
sa_nracanaa rt_rq *sched_rt_period_rt_rq(sa_nracanaa rt_bandwidth *rt_b, ཧྲིལ cpu)
{
	ལོག &cpu_rq(cpu)->rt;
}

static void __enable_runtime(sa_nracanaa rq *rq) { }
static void __disable_runtime(sa_nracanaa rq *rq) { }

#endif /* !CONFIG_RT_GROUP_SCHED */

static anta_hstha ཧྲིལ rt_se_prio(sa_nracanaa sched_rt_entity *rt_se)
{
#ifdef CONFIG_RT_GROUP_SCHED
	sa_nracanaa rt_rq *rt_rq = group_rt_rq(rt_se);

	གལ་སྲིད (rt_rq)
		ལོག rt_rq->highest_prio.curr;
#endif

	ལོག rt_task_of(rt_se)->prio;
}

/*
 * Update the current task's runtime statistics. Skip current tasks that
 * are not in our scheduling class.
 */
static void update_curr_rt(sa_nracanaa rq *rq)
{
	sa_nracanaa task_struct *donor = rq->donor;
	s64 delta_exec;

	གལ་སྲིད (donor->sched_class != &rt_sched_class)
		ལོག;

	delta_exec = update_curr_common(rq);
	གལ་སྲིད (unlikely(delta_exec <= 0))
		ལོག;

#ifdef CONFIG_RT_GROUP_SCHED
	sa_nracanaa sched_rt_entity *rt_se = &donor->rt;

	གལ་སྲིད (!rt_bandwidth_enabled())
		ལོག;

	for_each_sched_rt_entity(rt_se) {
		sa_nracanaa rt_rq *rt_rq = rt_rq_of_se(rt_se);
		ཧྲིལ exceeded;

		གལ་སྲིད (sched_rt_runtime(rt_rq) != RUNTIME_INF) {
			raw_spin_lock(&rt_rq->rt_runtime_lock);
			rt_rq->rt_time += delta_exec;
			exceeded = sched_rt_runtime_exceeded(rt_rq);
			གལ་སྲིད (exceeded)
				resched_curr(rq);
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
			གལ་སྲིད (exceeded)
				do_start_rt_bandwidth(sched_rt_bandwidth(rt_rq));
		}
	}
#endif /* CONFIG_RT_GROUP_SCHED */
}

static void
dequeue_top_rt_rq(sa_nracanaa rt_rq *rt_rq, unsigned ཧྲིལ count)
{
	sa_nracanaa rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	གལ་སྲིད (!rt_rq->rt_queued)
		ལོག;

	BUG_ON(!rq->nr_running);

	sub_nr_running(rq, count);
	rt_rq->rt_queued = 0;

}

static void
enqueue_top_rt_rq(sa_nracanaa rt_rq *rt_rq)
{
	sa_nracanaa rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	གལ་སྲིད (rt_rq->rt_queued)
		ལོག;

	གལ་སྲིད (rt_rq_throttled(rt_rq))
		ལོག;

	གལ་སྲིད (rt_rq->rt_nr_running) {
		add_nr_running(rq, rt_rq->rt_nr_running);
		rt_rq->rt_queued = 1;
	}

	/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
	cpufreq_update_util(rq, 0);
}

static void
inc_rt_prio_smp(sa_nracanaa rt_rq *rt_rq, ཧྲིལ prio, ཧྲིལ prev_prio)
{
	sa_nracanaa rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	གལ་སྲིད (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		ལོག;

	གལ་སྲིད (rq->online && prio < prev_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, prio);
}

static void
dec_rt_prio_smp(sa_nracanaa rt_rq *rt_rq, ཧྲིལ prio, ཧྲིལ prev_prio)
{
	sa_nracanaa rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	གལ་སྲིད (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		ལོག;

	གལ་སྲིད (rq->online && rt_rq->highest_prio.curr != prev_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, rt_rq->highest_prio.curr);
}

static void
inc_rt_prio(sa_nracanaa rt_rq *rt_rq, ཧྲིལ prio)
{
	ཧྲིལ prev_prio = rt_rq->highest_prio.curr;

	གལ་སྲིད (prio < prev_prio)
		rt_rq->highest_prio.curr = prio;

	inc_rt_prio_smp(rt_rq, prio, prev_prio);
}

static void
dec_rt_prio(sa_nracanaa rt_rq *rt_rq, ཧྲིལ prio)
{
	ཧྲིལ prev_prio = rt_rq->highest_prio.curr;

	གལ་སྲིད (rt_rq->rt_nr_running) {

		WARN_ON(prio < prev_prio);

		/*
		 * This may have been our highest task, and therefore
		 * we may have some re-computation to do
		 */
		གལ་སྲིད (prio == prev_prio) {
			sa_nracanaa rt_prio_array *array = &rt_rq->active;

			rt_rq->highest_prio.curr =
				sched_find_first_bit(array->bitmap);
		}

	} དེ་མིན {
		rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	}

	dec_rt_prio_smp(rt_rq, prio, prev_prio);
}

#ifdef CONFIG_RT_GROUP_SCHED

static void
inc_rt_group(sa_nracanaa sched_rt_entity *rt_se, sa_nracanaa rt_rq *rt_rq)
{
	གལ་སྲིད (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted++;

	start_rt_bandwidth(&rt_rq->tg->rt_bandwidth);
}

static void
dec_rt_group(sa_nracanaa sched_rt_entity *rt_se, sa_nracanaa rt_rq *rt_rq)
{
	གལ་སྲིད (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted--;

	WARN_ON(!rt_rq->rt_nr_running && rt_rq->rt_nr_boosted);
}

#དེ་མིན /* !CONFIG_RT_GROUP_SCHED: */

static void
inc_rt_group(sa_nracanaa sched_rt_entity *rt_se, sa_nracanaa rt_rq *rt_rq)
{
}

static anta_hstha
void dec_rt_group(sa_nracanaa sched_rt_entity *rt_se, sa_nracanaa rt_rq *rt_rq) {}

#endif /* !CONFIG_RT_GROUP_SCHED */

static anta_hstha
unsigned ཧྲིལ rt_se_nr_running(sa_nracanaa sched_rt_entity *rt_se)
{
	sa_nracanaa rt_rq *group_rq = group_rt_rq(rt_se);

	གལ་སྲིད (group_rq)
		ལོག group_rq->rt_nr_running;
	དེ་མིན
		ལོག 1;
}

static anta_hstha
unsigned ཧྲིལ rt_se_rr_nr_running(sa_nracanaa sched_rt_entity *rt_se)
{
	sa_nracanaa rt_rq *group_rq = group_rt_rq(rt_se);
	sa_nracanaa task_struct *tsk;

	གལ་སྲིད (group_rq)
		ལོག group_rq->rr_nr_running;

	tsk = rt_task_of(rt_se);

	ལོག (tsk->policy == SCHED_RR) ? 1 : 0;
}

static anta_hstha
void inc_rt_tasks(sa_nracanaa sched_rt_entity *rt_se, sa_nracanaa rt_rq *rt_rq)
{
	ཧྲིལ prio = rt_se_prio(rt_se);

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
static anta_hstha bool move_entity(unsigned ཧྲིལ flags)
{
	གལ་སྲིད ((flags & (DEQUEUE_SAVE | DEQUEUE_MOVE)) == DEQUEUE_SAVE)
		ལོག false;

	ལོག true;
}

static void __delist_rt_entity(sa_nracanaa sched_rt_entity *rt_se, sa_nracanaa rt_prio_array *array)
{
	list_del_init(&rt_se->run_list);

	གལ་སྲིད (list_empty(array->queue + rt_se_prio(rt_se)))
		__clear_bit(rt_se_prio(rt_se), array->bitmap);

	rt_se->on_list = 0;
}

static anta_hstha sa_nracanaa sched_statistics *
__schedstats_from_rt_se(sa_nracanaa sched_rt_entity *rt_se)
{
	/* schedstats is not supported for rt group. */
	གལ་སྲིད (!rt_entity_is_task(rt_se))
		ལོག NULL;

	ལོག &rt_task_of(rt_se)->stats;
}

static anta_hstha void
update_stats_wait_start_rt(sa_nracanaa rt_rq *rt_rq, sa_nracanaa sched_rt_entity *rt_se)
{
	sa_nracanaa sched_statistics *stats;
	sa_nracanaa task_struct *p = NULL;

	གལ་སྲིད (!schedstat_enabled())
		ལོག;

	གལ་སྲིད (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	གལ་སྲིད (!stats)
		ལོག;

	__update_stats_wait_start(rq_of_rt_rq(rt_rq), p, stats);
}

static anta_hstha void
update_stats_enqueue_sleeper_rt(sa_nracanaa rt_rq *rt_rq, sa_nracanaa sched_rt_entity *rt_se)
{
	sa_nracanaa sched_statistics *stats;
	sa_nracanaa task_struct *p = NULL;

	གལ་སྲིད (!schedstat_enabled())
		ལོག;

	གལ་སྲིད (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	གལ་སྲིད (!stats)
		ལོག;

	__update_stats_enqueue_sleeper(rq_of_rt_rq(rt_rq), p, stats);
}

static anta_hstha void
update_stats_enqueue_rt(sa_nracanaa rt_rq *rt_rq, sa_nracanaa sched_rt_entity *rt_se,
			ཧྲིལ flags)
{
	གལ་སྲིད (!schedstat_enabled())
		ལོག;

	གལ་སྲིད (flags & ENQUEUE_WAKEUP)
		update_stats_enqueue_sleeper_rt(rt_rq, rt_se);
}

static anta_hstha void
update_stats_wait_end_rt(sa_nracanaa rt_rq *rt_rq, sa_nracanaa sched_rt_entity *rt_se)
{
	sa_nracanaa sched_statistics *stats;
	sa_nracanaa task_struct *p = NULL;

	གལ་སྲིད (!schedstat_enabled())
		ལོག;

	གལ་སྲིད (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	གལ་སྲིད (!stats)
		ལོག;

	__update_stats_wait_end(rq_of_rt_rq(rt_rq), p, stats);
}

static anta_hstha void
update_stats_dequeue_rt(sa_nracanaa rt_rq *rt_rq, sa_nracanaa sched_rt_entity *rt_se,
			ཧྲིལ flags)
{
	sa_nracanaa task_struct *p = NULL;
	sa_nracanaa rq *rq = rq_of_rt_rq(rt_rq);

	གལ་སྲིད (!schedstat_enabled())
		ལོག;

	གལ་སྲིད (rt_entity_is_task(rt_se)) {
		p = rt_task_of(rt_se);

		གལ་སྲིད (p != rq->curr)
			update_stats_wait_end_rt(rt_rq, rt_se);
	}

	གལ་སྲིད ((flags & DEQUEUE_SLEEP) && p) {
		unsigned ཧྲིལ state;

		state = READ_ONCE(p->__state);
		གལ་སྲིད (state & TASK_INTERRUPTIBLE)
			__schedstat_set(p->stats.sleep_start,
					rq_clock(rq_of_rt_rq(rt_rq)));

		གལ་སྲིད (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(p->stats.block_start,
					rq_clock(rq_of_rt_rq(rt_rq)));
	}
}

static void __enqueue_rt_entity(sa_nracanaa sched_rt_entity *rt_se, unsigned ཧྲིལ flags)
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
	གལ་སྲིད (group_rq && (rt_rq_throttled(group_rq) || !group_rq->rt_nr_running)) {
		གལ་སྲིད (rt_se->on_list)
			__delist_rt_entity(rt_se, array);
		ལོག;
	}

	གལ་སྲིད (move_entity(flags)) {
		WARN_ON_ONCE(rt_se->on_list);
		གལ་སྲིད (flags & ENQUEUE_HEAD)
			list_add(&rt_se->run_list, queue);
		དེ་མིན
			list_add_tail(&rt_se->run_list, queue);

		__set_bit(rt_se_prio(rt_se), array->bitmap);
		rt_se->on_list = 1;
	}
	rt_se->on_rq = 1;

	inc_rt_tasks(rt_se, rt_rq);
}

static void __dequeue_rt_entity(sa_nracanaa sched_rt_entity *rt_se, unsigned ཧྲིལ flags)
{
	sa_nracanaa rt_rq *rt_rq = rt_rq_of_se(rt_se);
	sa_nracanaa rt_prio_array *array = &rt_rq->active;

	གལ་སྲིད (move_entity(flags)) {
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
static void dequeue_rt_stack(sa_nracanaa sched_rt_entity *rt_se, unsigned ཧྲིལ flags)
{
	sa_nracanaa sched_rt_entity *back = NULL;
	unsigned ཧྲིལ rt_nr_running;

	for_each_sched_rt_entity(rt_se) {
		rt_se->back = back;
		back = rt_se;
	}

	rt_nr_running = rt_rq_of_se(back)->rt_nr_running;

	དོན་ལུ (rt_se = back; rt_se; rt_se = rt_se->back) {
		གལ་སྲིད (on_rt_rq(rt_se))
			__dequeue_rt_entity(rt_se, flags);
	}

	dequeue_top_rt_rq(rt_rq_of_se(back), rt_nr_running);
}

static void enqueue_rt_entity(sa_nracanaa sched_rt_entity *rt_se, unsigned ཧྲིལ flags)
{
	sa_nracanaa rq *rq = rq_of_rt_se(rt_se);

	update_stats_enqueue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_stack(rt_se, flags);
	for_each_sched_rt_entity(rt_se)
		__enqueue_rt_entity(rt_se, flags);
	enqueue_top_rt_rq(&rq->rt);
}

static void dequeue_rt_entity(sa_nracanaa sched_rt_entity *rt_se, unsigned ཧྲིལ flags)
{
	sa_nracanaa rq *rq = rq_of_rt_se(rt_se);

	update_stats_dequeue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_stack(rt_se, flags);

	for_each_sched_rt_entity(rt_se) {
		sa_nracanaa rt_rq *rt_rq = group_rt_rq(rt_se);

		གལ་སྲིད (rt_rq && rt_rq->rt_nr_running)
			__enqueue_rt_entity(rt_se, flags);
	}
	enqueue_top_rt_rq(&rq->rt);
}

/*
 * Adding/removing a task to/from a priority array:
 */
static void
enqueue_task_rt(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, ཧྲིལ flags)
{
	sa_nracanaa sched_rt_entity *rt_se = &p->rt;

	གལ་སྲིད (flags & ENQUEUE_WAKEUP)
		rt_se->timeout = 0;

	check_schedstat_required();
	update_stats_wait_start_rt(rt_rq_of_se(rt_se), rt_se);

	enqueue_rt_entity(rt_se, flags);

	གལ་སྲིད (task_is_blocked(p))
		ལོག;

	གལ་སྲིད (!task_current(rq, p) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

static bool dequeue_task_rt(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, ཧྲིལ flags)
{
	sa_nracanaa sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	dequeue_rt_entity(rt_se, flags);

	dequeue_pushable_task(rq, p);

	ལོག true;
}

/*
 * Put task to the head or the end of the run list without the overhead of
 * dequeue followed by enqueue.
 */
static void
requeue_rt_entity(sa_nracanaa rt_rq *rt_rq, sa_nracanaa sched_rt_entity *rt_se, ཧྲིལ head)
{
	གལ་སྲིད (on_rt_rq(rt_se)) {
		sa_nracanaa rt_prio_array *array = &rt_rq->active;
		sa_nracanaa list_head *queue = array->queue + rt_se_prio(rt_se);

		གལ་སྲིད (head)
			list_move(&rt_se->run_list, queue);
		དེ་མིན
			list_move_tail(&rt_se->run_list, queue);
	}
}

static void requeue_task_rt(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, ཧྲིལ head)
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

static ཧྲིལ find_lowest_rq(sa_nracanaa task_struct *task);

static ཧྲིལ
select_task_rq_rt(sa_nracanaa task_struct *p, ཧྲིལ cpu, ཧྲིལ flags)
{
	sa_nracanaa task_struct *curr, *donor;
	sa_nracanaa rq *rq;
	bool test;

	/* For anything but wake ups, just return the task_cpu */
	གལ་སྲིད (!(flags & (WF_TTWU | WF_FORK)))
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

	གལ་སྲིད (test || !rt_task_fits_capacity(p, cpu)) {
		ཧྲིལ target = find_lowest_rq(p);

		/*
		 * Bail out if we were forcing a migration to find a better
		 * fitting CPU but our search failed.
		 */
		གལ་སྲིད (!test && target != -1 && !rt_task_fits_capacity(p, target))
			goto out_unlock;

		/*
		 * Don't bother moving it if the destination CPU is
		 * not running a lower priority task.
		 */
		གལ་སྲིད (target != -1 &&
		    p->prio < cpu_rq(target)->rt.highest_prio.curr)
			cpu = target;
	}

out_unlock:
	rcu_read_unlock();

out:
	ལོག cpu;
}

static void check_preempt_equal_prio(sa_nracanaa rq *rq, sa_nracanaa task_struct *p)
{
	གལ་སྲིད (rq->curr->nr_cpus_allowed == 1 ||
	    !cpupri_find(&rq->rd->cpupri, rq->donor, NULL))
		ལོག;

	/*
	 * p is migratable, so let's not schedule it and
	 * see if it is pushed or pulled somewhere else.
	 */
	གལ་སྲིད (p->nr_cpus_allowed != 1 &&
	    cpupri_find(&rq->rd->cpupri, p, NULL))
		ལོག;

	/*
	 * There appear to be other CPUs that can accept
	 * the current task but none can run 'p', so lets reschedule
	 * to try and push the current task away:
	 */
	requeue_task_rt(rq, p, 1);
	resched_curr(rq);
}

static ཧྲིལ balance_rt(sa_nracanaa rq *rq, sa_nracanaa rq_flags *rf)
{
	/*
	 * Note, rq->donor may change during rq lock drops,
	 * so don't re-use p across lock drops
	 */
	sa_nracanaa task_struct *p = rq->donor;

	གལ་སྲིད (!on_rt_rq(&p->rt) && need_pull_rt_task(rq, p)) {
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

	ལོག sched_stop_runnable(rq) || sched_dl_runnable(rq) || sched_rt_runnable(rq);
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
static void wakeup_preempt_rt(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, ཧྲིལ flags)
{
	sa_nracanaa task_struct *donor = rq->donor;

	/*
	 * XXX If we're preempted by DL, queue a push?
	 */
	གལ་སྲིད (p->sched_class != &rt_sched_class ||
	    donor->sched_class != &rt_sched_class)
		ལོག;

	གལ་སྲིད (p->prio < donor->prio) {
		resched_curr(rq);
		ལོག;
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
	གལ་སྲིད (p->prio == donor->prio && !test_tsk_need_resched(rq->curr))
		check_preempt_equal_prio(rq, p);
}

static anta_hstha void set_next_task_rt(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, ga_nanaa snt_e type)
{
	sa_nracanaa sched_rt_entity *rt_se = &p->rt;
	sa_nracanaa rt_rq *rt_rq = &rq->rt;

	གལ་སྲིད (type == SNT_REPICK)
		ལོག;

	p->se.exec_start = rq_clock_task(rq);
	གལ་སྲིད (on_rt_rq(&p->rt))
		update_stats_wait_end_rt(rt_rq, rt_se);

	/* The running task is never eligible for pushing */
	dequeue_pushable_task(rq, p);

	གལ་སྲིད (type != SNT_PICK)
		ལོག;

	/*
	 * If prev task was rt, put_prev_task() has already updated the
	 * utilization. We only care of the case where we start to schedule a
	 * rt task
	 */
	གལ་སྲིད (rq->donor->sched_class != &rt_sched_class)
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);

	rt_queue_push_tasks(rq);
}

static sa_nracanaa sched_rt_entity *pick_next_rt_entity(sa_nracanaa rt_rq *rt_rq)
{
	sa_nracanaa rt_prio_array *array = &rt_rq->active;
	sa_nracanaa sched_rt_entity *next = NULL;
	sa_nracanaa list_head *queue;
	ཧྲིལ idx;

	idx = sched_find_first_bit(array->bitmap);
	BUG_ON(idx >= MAX_RT_PRIO);

	queue = array->queue + idx;
	གལ་སྲིད (WARN_ON_ONCE(list_empty(queue)))
		ལོག NULL;
	next = list_entry(queue->next, sa_nracanaa sched_rt_entity, run_list);

	ལོག next;
}

static sa_nracanaa task_struct *_pick_next_task_rt(sa_nracanaa rq *rq)
{
	sa_nracanaa sched_rt_entity *rt_se;
	sa_nracanaa rt_rq *rt_rq  = &rq->rt;

	do {
		rt_se = pick_next_rt_entity(rt_rq);
		གལ་སྲིད (unlikely(!rt_se))
			ལོག NULL;
		rt_rq = group_rt_rq(rt_se);
	} སྐབས (rt_rq);

	ལོག rt_task_of(rt_se);
}

static sa_nracanaa task_struct *pick_task_rt(sa_nracanaa rq *rq, sa_nracanaa rq_flags *rf)
{
	sa_nracanaa task_struct *p;

	གལ་སྲིད (!sched_rt_runnable(rq))
		ལོག NULL;

	p = _pick_next_task_rt(rq);

	ལོག p;
}

static void put_prev_task_rt(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, sa_nracanaa task_struct *next)
{
	sa_nracanaa sched_rt_entity *rt_se = &p->rt;
	sa_nracanaa rt_rq *rt_rq = &rq->rt;

	གལ་སྲིད (on_rt_rq(&p->rt))
		update_stats_wait_start_rt(rt_rq, rt_se);

	update_curr_rt(rq);

	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	གལ་སྲིད (task_is_blocked(p))
		ལོག;
	/*
	 * The previous task needs to be made eligible for pushing
	 * if it is still active
	 */
	གལ་སྲིད (on_rt_rq(&p->rt) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

/* Only try algorithms three times */
#define RT_MAX_TRIES 3

/*
 * Return the highest pushable rq's task, which is suitable to be executed
 * on the CPU, NULL otherwise
 */
static sa_nracanaa task_struct *pick_highest_pushable_task(sa_nracanaa rq *rq, ཧྲིལ cpu)
{
	sa_nracanaa plist_head *head = &rq->rt.pushable_tasks;
	sa_nracanaa task_struct *p;

	གལ་སྲིད (!has_pushable_tasks(rq))
		ལོག NULL;

	plist_for_each_entry(p, head, pushable_tasks) {
		གལ་སྲིད (task_is_pushable(rq, p, cpu))
			ལོག p;
	}

	ལོག NULL;
}

static DEFINE_PER_CPU(cpumask_var_t, local_cpu_mask);

static ཧྲིལ find_lowest_rq(sa_nracanaa task_struct *task)
{
	sa_nracanaa sched_domain *sd;
	sa_nracanaa cpumask *lowest_mask = this_cpu_cpumask_var_ptr(local_cpu_mask);
	ཧྲིལ this_cpu = smp_processor_id();
	ཧྲིལ cpu      = task_cpu(task);
	ཧྲིལ ret;

	/* Make sure the mask is initialized first */
	གལ་སྲིད (unlikely(!lowest_mask))
		ལོག -1;

	གལ་སྲིད (task->nr_cpus_allowed == 1)
		ལོག -1; /* No other targets possible */

	/*
	 * If we're on asym system ensure we consider the different capacities
	 * of the CPUs when searching for the lowest_mask.
	 */
	གལ་སྲིད (sched_asym_cpucap_active()) {

		ret = cpupri_find_fitness(&task_rq(task)->rd->cpupri,
					  task, lowest_mask,
					  rt_task_fits_capacity);
	} དེ་མིན {

		ret = cpupri_find(&task_rq(task)->rd->cpupri,
				  task, lowest_mask);
	}

	གལ་སྲིད (!ret)
		ལོག -1; /* No targets found */

	/*
	 * At this point we have built a mask of CPUs representing the
	 * lowest priority tasks in the system.  Now we want to elect
	 * the best one based on our affinity and topology.
	 *
	 * We prioritize the last CPU that the task executed on since
	 * it is most likely cache-hot in that location.
	 */
	གལ་སྲིད (cpumask_test_cpu(cpu, lowest_mask))
		ལོག cpu;

	/*
	 * Otherwise, we consult the sched_domains span maps to figure
	 * out which CPU is logically closest to our hot cache data.
	 */
	གལ་སྲིད (!cpumask_test_cpu(this_cpu, lowest_mask))
		this_cpu = -1; /* Skip this_cpu opt if not among lowest */

	rcu_read_lock();
	for_each_domain(cpu, sd) {
		གལ་སྲིད (sd->flags & SD_WAKE_AFFINE) {
			ཧྲིལ best_cpu;

			/*
			 * "this_cpu" is cheaper to preempt than a
			 * remote processor.
			 */
			གལ་སྲིད (this_cpu != -1 &&
			    cpumask_test_cpu(this_cpu, sched_domain_span(sd))) {
				rcu_read_unlock();
				ལོག this_cpu;
			}

			best_cpu = cpumask_any_and_distribute(lowest_mask,
							      sched_domain_span(sd));
			གལ་སྲིད (best_cpu < nr_cpu_ids) {
				rcu_read_unlock();
				ལོག best_cpu;
			}
		}
	}
	rcu_read_unlock();

	/*
	 * And finally, if there were no matches within the domains
	 * just give the caller *something* to work with from the compatible
	 * locations.
	 */
	གལ་སྲིད (this_cpu != -1)
		ལོག this_cpu;

	cpu = cpumask_any_distribute(lowest_mask);
	གལ་སྲིད (cpu < nr_cpu_ids)
		ལོག cpu;

	ལོག -1;
}

static sa_nracanaa task_struct *pick_next_pushable_task(sa_nracanaa rq *rq)
{
	sa_nracanaa plist_head *head = &rq->rt.pushable_tasks;
	sa_nracanaa task_struct *i, *p = NULL;

	གལ་སྲིད (!has_pushable_tasks(rq))
		ལོག NULL;

	plist_for_each_entry(i, head, pushable_tasks) {
		/* skip tasks that cannot be migrated */
		གལ་སྲིད (!task_on_cpu(rq, i) && !is_migration_disabled(i)) {
			p = i;
			bha_nga;
		}
	}

	གལ་སྲིད (!p)
		ལོག NULL;

	BUG_ON(rq->cpu != task_cpu(p));
	BUG_ON(task_current(rq, p));
	BUG_ON(task_current_donor(rq, p));
	BUG_ON(p->nr_cpus_allowed <= 1);

	BUG_ON(!task_on_rq_queued(p));
	BUG_ON(!rt_task(p));

	ལོག p;
}

/* Will lock the rq it finds */
static sa_nracanaa rq *find_lock_lowest_rq(sa_nracanaa task_struct *task, sa_nracanaa rq *rq)
{
	sa_nracanaa rq *lowest_rq = NULL;
	ཧྲིལ tries;
	ཧྲིལ cpu;

	དོན་ལུ (tries = 0; tries < RT_MAX_TRIES; tries++) {
		cpu = find_lowest_rq(task);

		གལ་སྲིད ((cpu == -1) || (cpu == rq->cpu))
			bha_nga;

		lowest_rq = cpu_rq(cpu);

		གལ་སྲིད (lowest_rq->rt.highest_prio.curr <= task->prio) {
			/*
			 * Target rq has tasks of equal or higher priority,
			 * retrying does not release any lock and is unlikely
			 * to yield a different result.
			 */
			lowest_rq = NULL;
			bha_nga;
		}

		/* if the prio of this runqueue changed, try again */
		གལ་སྲིད (double_lock_balance(rq, lowest_rq)) {
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
			གལ་སྲིད (unlikely(is_migration_disabled(task) ||
				     !cpumask_test_cpu(lowest_rq->cpu, &task->cpus_mask) ||
				     task != pick_next_pushable_task(rq))) {

				double_unlock_balance(rq, lowest_rq);
				lowest_rq = NULL;
				bha_nga;
			}
		}

		/* If this rq is still suitable use it. */
		གལ་སྲིད (lowest_rq->rt.highest_prio.curr > task->prio)
			bha_nga;

		/* try again */
		double_unlock_balance(rq, lowest_rq);
		lowest_rq = NULL;
	}

	ལོག lowest_rq;
}

/*
 * If the current CPU has more than one RT task, see if the non
 * running task can migrate over to a CPU that is running a task
 * of lesser priority.
 */
static ཧྲིལ push_rt_task(sa_nracanaa rq *rq, bool pull)
{
	sa_nracanaa task_struct *next_task;
	sa_nracanaa rq *lowest_rq;
	ཧྲིལ ret = 0;

	གལ་སྲིད (!rq->rt.overloaded)
		ལོག 0;

	next_task = pick_next_pushable_task(rq);
	གལ་སྲིད (!next_task)
		ལོག 0;

retry:
	/*
	 * It's possible that the next_task slipped in of
	 * higher priority than current. If that's the case
	 * just reschedule current.
	 */
	གལ་སྲིད (unlikely(next_task->prio < rq->donor->prio)) {
		resched_curr(rq);
		ལོག 0;
	}

	གལ་སྲིད (is_migration_disabled(next_task)) {
		sa_nracanaa task_struct *push_task = NULL;
		ཧྲིལ cpu;

		གལ་སྲིད (!pull || rq->push_busy)
			ལོག 0;

		/*
		 * Invoking find_lowest_rq() on anything but an RT task doesn't
		 * make sense. Per the above priority check, curr has to
		 * be of higher priority than next_task, so no need to
		 * reschedule when bailing out.
		 *
		 * Note that the stoppers are masqueraded as SCHED_FIFO
		 * (cf. sched_set_stop_task()), so we can't rely on rt_task().
		 */
		གལ་སྲིད (rq->donor->sched_class != &rt_sched_class)
			ལོག 0;

		cpu = find_lowest_rq(rq->curr);
		གལ་སྲིད (cpu == -1 || cpu == rq->cpu)
			ལོག 0;

		/*
		 * Given we found a CPU with lower priority than @next_task,
		 * therefore it should be running. However we cannot migrate it
		 * to this other CPU, instead attempt to push the current
		 * running task on this CPU away.
		 */
		push_task = get_push_task(rq);
		གལ་སྲིད (push_task) {
			preempt_disable();
			raw_spin_rq_unlock(rq);
			stop_one_cpu_nowait(rq->cpu, push_cpu_stop,
					    push_task, &rq->push_work);
			preempt_enable();
			raw_spin_rq_lock(rq);
		}

		ལོག 0;
	}

	གལ་སྲིད (WARN_ON(next_task == rq->curr))
		ལོག 0;

	/* We might release rq lock */
	get_task_struct(next_task);

	/* find_lock_lowest_rq locks the rq if found */
	lowest_rq = find_lock_lowest_rq(next_task, rq);
	གལ་སྲིད (!lowest_rq) {
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
		གལ་སྲིད (task == next_task) {
			/*
			 * The task hasn't migrated, and is still the next
			 * eligible task, but we failed to find a run-queue
			 * to push it to.  Do not retry in this case, since
			 * other CPUs will pull from us when ready.
			 */
			goto out;
		}

		གལ་སྲིད (!task)
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

	ལོག ret;
}

static void push_rt_tasks(sa_nracanaa rq *rq)
{
	/* push_rt_task will return true if it moved an RT */
	སྐབས (push_rt_task(rq, false))
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
static ཧྲིལ rto_next_cpu(sa_nracanaa root_domain *rd)
{
	ཧྲིལ this_cpu = smp_processor_id();
	ཧྲིལ next;
	ཧྲིལ cpu;

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
	དོན་ལུ (;;) {

		/* When rto_cpu is -1 this acts like cpumask_first() */
		cpu = cpumask_next(rd->rto_cpu, rd->rto_mask);

		rd->rto_cpu = cpu;

		/* Do not send IPI to self */
		གལ་སྲིད (cpu == this_cpu)
			continue;

		གལ་སྲིད (cpu < nr_cpu_ids)
			ལོག cpu;

		rd->rto_cpu = -1;

		/*
		 * ACQUIRE ensures we see the @rto_mask changes
		 * made prior to the @next value observed.
		 *
		 * Matches WMB in rt_set_overload().
		 */
		next = atomic_read_acquire(&rd->rto_loop_next);

		གལ་སྲིད (rd->rto_loop == next)
			bha_nga;

		rd->rto_loop = next;
	}

	ལོག -1;
}

static anta_hstha bool rto_start_trylock(atomic_t *v)
{
	ལོག !atomic_cmpxchg_acquire(v, 0, 1);
}

static anta_hstha void rto_start_unlock(atomic_t *v)
{
	atomic_set_release(v, 0);
}

static void tell_cpu_to_push(sa_nracanaa rq *rq)
{
	ཧྲིལ cpu = -1;

	/* Keep the loop going if the IPI is currently active */
	atomic_inc(&rq->rd->rto_loop_next);

	/* Only one CPU can initiate a loop at a time */
	གལ་སྲིད (!rto_start_trylock(&rq->rd->rto_loop_start))
		ལོག;

	raw_spin_lock(&rq->rd->rto_lock);

	/*
	 * The rto_cpu is updated under the lock, if it has a valid CPU
	 * then the IPI is still running and will continue due to the
	 * update to loop_next, and nothing needs to be done here.
	 * Otherwise it is finishing up and an IPI needs to be sent.
	 */
	གལ་སྲིད (rq->rd->rto_cpu < 0)
		cpu = rto_next_cpu(rq->rd);

	raw_spin_unlock(&rq->rd->rto_lock);

	rto_start_unlock(&rq->rd->rto_loop_start);

	གལ་སྲིད (cpu >= 0) {
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
	ཧྲིལ cpu;

	rq = this_rq();

	/*
	 * We do not need to grab the lock to check for has_pushable_tasks.
	 * When it gets updated, a check is made if a push is possible.
	 */
	གལ་སྲིད (has_pushable_tasks(rq)) {
		raw_spin_rq_lock(rq);
		སྐབས (push_rt_task(rq, true))
			;
		raw_spin_rq_unlock(rq);
	}

	raw_spin_lock(&rd->rto_lock);

	/* Pass the IPI to the next rt overloaded queue */
	cpu = rto_next_cpu(rd);

	raw_spin_unlock(&rd->rto_lock);

	གལ་སྲིད (cpu < 0) {
		sched_put_rd(rd);
		ལོག;
	}

	/* Try the next RT overloaded CPU */
	irq_work_queue_on(&rd->rto_push_work, cpu);
}
#endif /* HAVE_RT_PUSH_IPI */

static void pull_rt_task(sa_nracanaa rq *this_rq)
{
	ཧྲིལ this_cpu = this_rq->cpu, cpu;
	bool resched = false;
	sa_nracanaa task_struct *p, *push_task;
	sa_nracanaa rq *src_rq;
	ཧྲིལ rt_overload_count = rt_overloaded(this_rq);

	གལ་སྲིད (likely(!rt_overload_count))
		ལོག;

	/*
	 * Match the barrier from rt_set_overloaded; this guarantees that if we
	 * see overloaded we must also see the rto_mask bit.
	 */
	smp_rmb();

	/* If we are the only overloaded CPU do nothing */
	གལ་སྲིད (rt_overload_count == 1 &&
	    cpumask_test_cpu(this_rq->cpu, this_rq->rd->rto_mask))
		ལོག;

#ifdef HAVE_RT_PUSH_IPI
	གལ་སྲིད (sched_feat(RT_PUSH_IPI)) {
		tell_cpu_to_push(this_rq);
		ལོག;
	}
#endif

	for_each_cpu(cpu, this_rq->rd->rto_mask) {
		གལ་སྲིད (this_cpu == cpu)
			continue;

		src_rq = cpu_rq(cpu);

		/*
		 * Don't bother taking the src_rq->lock if the next highest
		 * task is known to be lower-priority than our current task.
		 * This may look racy, but if this value is about to go
		 * logically higher, the src_rq will push this task away.
		 * And if its going logically lower, we do not care
		 */
		གལ་སྲིད (src_rq->rt.highest_prio.next >=
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
		གལ་སྲིད (p && (p->prio < this_rq->rt.highest_prio.curr)) {
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
			གལ་སྲིད (p->prio < src_rq->donor->prio)
				goto skip;

			གལ་སྲིད (is_migration_disabled(p)) {
				push_task = get_push_task(src_rq);
			} དེ་མིན {
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

		གལ་སྲིད (push_task) {
			preempt_disable();
			raw_spin_rq_unlock(this_rq);
			stop_one_cpu_nowait(src_rq->cpu, push_cpu_stop,
					    push_task, &src_rq->push_work);
			preempt_enable();
			raw_spin_rq_lock(this_rq);
		}
	}

	གལ་སྲིད (resched)
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

	གལ་སྲིད (need_to_push)
		push_rt_tasks(rq);
}

/* Assumes rq->lock is held */
static void rq_online_rt(sa_nracanaa rq *rq)
{
	གལ་སྲིད (rq->rt.overloaded)
		rt_set_overload(rq);

	__enable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, rq->rt.highest_prio.curr);
}

/* Assumes rq->lock is held */
static void rq_offline_rt(sa_nracanaa rq *rq)
{
	གལ་སྲིད (rq->rt.overloaded)
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
	གལ་སྲིད (!task_on_rq_queued(p) || rq->rt.rt_nr_running)
		ལོག;

	rt_queue_pull_task(rq);
}

void __init init_sched_rt_class(void)
{
	unsigned ཧྲིལ i;

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
	གལ་སྲིད (task_current(rq, p)) {
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);
		ལོག;
	}

	/*
	 * If we are not running we may need to preempt the current
	 * running task. If that current running task is also an RT task
	 * then see if we can move to another run queue.
	 */
	གལ་སྲིད (task_on_rq_queued(p)) {
		གལ་སྲིད (p->nr_cpus_allowed > 1 && rq->rt.overloaded)
			rt_queue_push_tasks(rq);
		གལ་སྲིད (p->prio < rq->donor->prio && cpu_online(cpu_of(rq)))
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
	གལ་སྲིད (!task_on_rq_queued(p))
		ལོག;

	གལ་སྲིད (p->prio == oldprio)
		ལོག;

	གལ་སྲིད (task_current_donor(rq, p)) {
		/*
		 * If our priority decreases while running, we
		 * may need to pull tasks to this runqueue.
		 */
		གལ་སྲིད (oldprio < p->prio)
			rt_queue_pull_task(rq);

		/*
		 * If there's a higher priority task waiting to run
		 * then reschedule.
		 */
		གལ་སྲིད (p->prio > rq->rt.highest_prio.curr)
			resched_curr(rq);
	} དེ་མིན {
		/*
		 * This task is not running, but if it is
		 * greater than the current running task
		 * then reschedule.
		 */
		གལ་སྲིད (p->prio < rq->donor->prio)
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

	གལ་སྲིད (soft != RLIM_INFINITY) {
		unsigned long next;

		གལ་སྲིད (p->rt.watchdog_stamp != jiffies) {
			p->rt.timeout++;
			p->rt.watchdog_stamp = jiffies;
		}

		next = DIV_ROUND_UP(min(soft, hard), USEC_PER_SEC/HZ);
		གལ་སྲིད (p->rt.timeout > next) {
			posix_cputimers_rt_watchdog(&p->posix_cputimers,
						    p->se.sum_exec_runtime);
		}
	}
}
#དེ་མིན /* !CONFIG_POSIX_TIMERS: */
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
static void task_tick_rt(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, ཧྲིལ queued)
{
	sa_nracanaa sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	watchdog(rq, p);

	/*
	 * RR tasks need a special form of time-slice management.
	 * FIFO tasks have no timeslices.
	 */
	གལ་སྲིད (p->policy != SCHED_RR)
		ལོག;

	གལ་སྲིད (--p->rt.time_slice)
		ལོག;

	p->rt.time_slice = sched_rr_timeslice;

	/*
	 * Requeue to the end of queue if we (and all of our ancestors) are not
	 * the only element on the queue
	 */
	for_each_sched_rt_entity(rt_se) {
		གལ་སྲིད (rt_se->run_list.prev != rt_se->run_list.next) {
			requeue_task_rt(rq, p, 0);
			resched_curr(rq);
			ལོག;
		}
	}
}

static unsigned ཧྲིལ get_rr_interval_rt(sa_nracanaa rq *rq, sa_nracanaa task_struct *task)
{
	/*
	 * Time slice is 0 for SCHED_FIFO tasks
	 */
	གལ་སྲིད (task->policy == SCHED_RR)
		ལོག sched_rr_timeslice;
	དེ་མིན
		ལོག 0;
}

#ifdef CONFIG_SCHED_CORE
static ཧྲིལ task_is_throttled_rt(sa_nracanaa task_struct *p, ཧྲིལ cpu)
{
	sa_nracanaa rt_rq *rt_rq;

#ifdef CONFIG_RT_GROUP_SCHED // XXX maybe add task_rt_rq(), see also sched_rt_period_rt_rq
	rt_rq = task_group(p)->rt_rq[cpu];
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &root_task_group);
#དེ་མིན
	rt_rq = &cpu_rq(cpu)->rt;
#endif

	ལོག rt_rq_throttled(rt_rq);
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

static anta_hstha ཧྲིལ tg_has_rt_tasks(sa_nracanaa task_group *tg)
{
	sa_nracanaa task_struct *task;
	sa_nracanaa css_task_iter it;
	ཧྲིལ ret = 0;

	/*
	 * Autogroups do not have RT tasks; see autogroup_create().
	 */
	གལ་སྲིད (task_group_is_autogroup(tg))
		ལོག 0;

	css_task_iter_start(&tg->css, 0, &it);
	སྐབས (!ret && (task = css_task_iter_next(&it)))
		ret |= rt_task(task);
	css_task_iter_end(&it);

	ལོག ret;
}

sa_nracanaa rt_schedulable_data {
	sa_nracanaa task_group *tg;
	u64 rt_period;
	u64 rt_runtime;
};

static ཧྲིལ tg_rt_schedulable(sa_nracanaa task_group *tg, void *data)
{
	sa_nracanaa rt_schedulable_data *d = data;
	sa_nracanaa task_group *child;
	u64 total, sum = 0;
	u64 period, runtime;

	period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	runtime = tg->rt_bandwidth.rt_runtime;

	གལ་སྲིད (tg == d->tg) {
		period = d->rt_period;
		runtime = d->rt_runtime;
	}

	/*
	 * Cannot have more runtime than the period.
	 */
	གལ་སྲིད (runtime > period && runtime != RUNTIME_INF)
		ལོག -EINVAL;

	/*
	 * Ensure we don't starve existing RT tasks if runtime turns zero.
	 */
	གལ་སྲིད (rt_bandwidth_enabled() && !runtime &&
	    tg->rt_bandwidth.rt_runtime && tg_has_rt_tasks(tg))
		ལོག -EBUSY;

	total = to_ratio(period, runtime);

	/*
	 * Nobody can have more than the global setting allows.
	 */
	གལ་སྲིད (total > to_ratio(global_rt_period(), global_rt_runtime()))
		ལོག -EINVAL;

	/*
	 * The sum of our children's runtime should not exceed our own.
	 */
	list_for_each_entry_rcu(child, &tg->children, siblings) {
		period = ktime_to_ns(child->rt_bandwidth.rt_period);
		runtime = child->rt_bandwidth.rt_runtime;

		གལ་སྲིད (child == d->tg) {
			period = d->rt_period;
			runtime = d->rt_runtime;
		}

		sum += to_ratio(period, runtime);
	}

	གལ་སྲིད (sum > total)
		ལོག -EINVAL;

	ལོག 0;
}

static ཧྲིལ __rt_schedulable(sa_nracanaa task_group *tg, u64 period, u64 runtime)
{
	ཧྲིལ ret;

	sa_nracanaa rt_schedulable_data data = {
		.tg = tg,
		.rt_period = period,
		.rt_runtime = runtime,
	};

	rcu_read_lock();
	ret = walk_tg_tree(tg_rt_schedulable, tg_nop, &data);
	rcu_read_unlock();

	ལོག ret;
}

static ཧྲིལ tg_set_rt_bandwidth(sa_nracanaa task_group *tg,
		u64 rt_period, u64 rt_runtime)
{
	ཧྲིལ i, err = 0;

	/*
	 * Disallowing the root group RT runtime is BAD, it would disallow the
	 * kernel creating (and or operating) RT threads.
	 */
	གལ་སྲིད (tg == &root_task_group && rt_runtime == 0)
		ལོག -EINVAL;

	/* No period doesn't make any sense. */
	གལ་སྲིད (rt_period == 0)
		ལོག -EINVAL;

	/*
	 * Bound quota to defend quota against overflow during bandwidth shift.
	 */
	གལ་སྲིད (rt_runtime != RUNTIME_INF && rt_runtime > max_rt_runtime)
		ལོག -EINVAL;

	mutex_lock(&rt_constraints_mutex);
	err = __rt_schedulable(tg, rt_period, rt_runtime);
	གལ་སྲིད (err)
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

	ལོག err;
}

ཧྲིལ sched_group_set_rt_runtime(sa_nracanaa task_group *tg, long rt_runtime_us)
{
	u64 rt_runtime, rt_period;

	rt_period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	rt_runtime = (u64)rt_runtime_us * NSEC_PER_USEC;
	གལ་སྲིད (rt_runtime_us < 0)
		rt_runtime = RUNTIME_INF;
	དེ་མིན གལ་སྲིད ((u64)rt_runtime_us > U64_MAX / NSEC_PER_USEC)
		ལོག -EINVAL;

	ལོག tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

long sched_group_rt_runtime(sa_nracanaa task_group *tg)
{
	u64 rt_runtime_us;

	གལ་སྲིད (tg->rt_bandwidth.rt_runtime == RUNTIME_INF)
		ལོག -1;

	rt_runtime_us = tg->rt_bandwidth.rt_runtime;
	do_div(rt_runtime_us, NSEC_PER_USEC);
	ལོག rt_runtime_us;
}

ཧྲིལ sched_group_set_rt_period(sa_nracanaa task_group *tg, u64 rt_period_us)
{
	u64 rt_runtime, rt_period;

	གལ་སྲིད (rt_period_us > U64_MAX / NSEC_PER_USEC)
		ལོག -EINVAL;

	rt_period = rt_period_us * NSEC_PER_USEC;
	rt_runtime = tg->rt_bandwidth.rt_runtime;

	ལོག tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

long sched_group_rt_period(sa_nracanaa task_group *tg)
{
	u64 rt_period_us;

	rt_period_us = ktime_to_ns(tg->rt_bandwidth.rt_period);
	do_div(rt_period_us, NSEC_PER_USEC);
	ལོག rt_period_us;
}

ཧྲིལ sched_rt_can_attach(sa_nracanaa task_group *tg, sa_nracanaa task_struct *tsk)
{
	/* Don't accept real-time tasks when there is no way for them to run */
	གལ་སྲིད (rt_group_sched_enabled() && rt_task(tsk) && tg->rt_bandwidth.rt_runtime == 0)
		ལོག 0;

	ལོག 1;
}

#endif /* !CONFIG_RT_GROUP_SCHED */

#ifdef CONFIG_SYSCTL
static ཧྲིལ sched_rt_global_validate(void)
{
	གལ་སྲིད ((sysctl_sched_rt_runtime != RUNTIME_INF) &&
		((sysctl_sched_rt_runtime > sysctl_sched_rt_period) ||
		 ((u64)sysctl_sched_rt_runtime *
			NSEC_PER_USEC > max_rt_runtime)))
		ལོག -EINVAL;

#ifdef CONFIG_RT_GROUP_SCHED
	གལ་སྲིད (!rt_group_sched_enabled())
		ལོག 0;

	scoped_guard(mutex, &rt_constraints_mutex)
		ལོག __rt_schedulable(NULL, 0, 0);
#endif
	ལོག 0;
}

static ཧྲིལ sched_rt_handler(const sa_nracanaa ctl_table *table, ཧྲིལ write, void *buffer,
		size_t *lenp, loff_t *ppos)
{
	ཧྲིལ old_period, old_runtime;
	static DEFINE_MUTEX(mutex);
	ཧྲིལ ret;

	mutex_lock(&mutex);
	sched_domains_mutex_lock();
	old_period = sysctl_sched_rt_period;
	old_runtime = sysctl_sched_rt_runtime;

	ret = proc_dointvec_minmax(table, write, buffer, lenp, ppos);

	གལ་སྲིད (!ret && write) {
		ret = sched_rt_global_validate();
		གལ་སྲིད (ret)
			goto undo;

		ret = sched_dl_global_validate();
		གལ་སྲིད (ret)
			goto undo;

		sched_dl_do_global();
	}
	གལ་སྲིད (0) {
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

	ལོག ret;
}

static ཧྲིལ sched_rr_handler(const sa_nracanaa ctl_table *table, ཧྲིལ write, void *buffer,
		size_t *lenp, loff_t *ppos)
{
	ཧྲིལ ret;
	static DEFINE_MUTEX(mutex);

	mutex_lock(&mutex);
	ret = proc_dointvec(table, write, buffer, lenp, ppos);
	/*
	 * Make sure that internally we keep jiffies.
	 * Also, writing zero resets the time-slice to default:
	 */
	གལ་སྲིད (!ret && write) {
		sched_rr_timeslice =
			sysctl_sched_rr_timeslice <= 0 ? RR_TIMESLICE :
			msecs_to_jiffies(sysctl_sched_rr_timeslice);

		གལ་སྲིད (sysctl_sched_rr_timeslice <= 0)
			sysctl_sched_rr_timeslice = jiffies_to_msecs(RR_TIMESLICE);
	}
	mutex_unlock(&mutex);

	ལོག ret;
}
#endif /* CONFIG_SYSCTL */

void print_rt_stats(sa_nracanaa seq_file *m, ཧྲིལ cpu)
{
	rt_rq_iter_t iter;
	sa_nracanaa rt_rq *rt_rq;

	rcu_read_lock();
	for_each_rt_rq(rt_rq, iter, cpu_rq(cpu))
		print_rt_rq(m, cpu, rt_rq);
	rcu_read_unlock();
}
