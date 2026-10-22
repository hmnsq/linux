// SPDX-License-Identifier: GPL-2.0
/*
 * Pressure stall information for CPU, memory and IO
 *
 * Copyright (c) 2018 Facebook, Inc.
 * Author: Johannes Weiner <hannes@cmpxchg.org>
 *
 * Polling support by Suren Baghdasaryan <surenb@google.com>
 * Copyright (c) 2018 Google, Inc.
 *
 * When CPU, memory and IO are contended, tasks experience delays that
 * reduce throughput and introduce latencies into the workload. Memory
 * and IO contention, in addition, can cause a full loss of forward
 * progress in which the CPU goes idle.
 *
 * This code aggregates individual task delays into resource pressure
 * metrics that indicate problems with both workload health and
 * resource utilization.
 *
 *			Model
 *
 * The time in which a task can execute on a CPU is our baseline for
 * productivity. Pressure expresses the amount of time in which this
 * potential cannot be realized due to resource contention.
 *
 * This concept of productivity has two components: the workload and
 * the CPU. To measure the impact of pressure on both, we define two
 * contention states for a resource: SOME and FULL.
 *
 * In the SOME state of a given resource, one or more tasks are
 * delayed on that resource. This affects the workload's ability to
 * perform work, but the CPU may still be executing other tasks.
 *
 * In the FULL state of a given resource, all non-idle tasks are
 * delayed on that resource such that nobody is advancing and the CPU
 * goes idle. This leaves both workload and CPU unproductive.
 *
 *	SOME = nr_delayed_tasks != 0
 *	FULL = nr_delayed_tasks != 0 && nr_productive_tasks == 0
 *
 * What it means for a task to be productive is defined differently
 * for each resource. For IO, productive means a running task. For
 * memory, productive means a running task that isn't a reclaimer. For
 * CPU, productive means an on-CPU task.
 *
 * Naturally, the FULL state doesn't exist for the CPU resource at the
 * system level, but exist at the cgroup level. At the cgroup level,
 * FULL means all non-idle tasks in the cgroup are delayed on the CPU
 * resource which is being used by others outside of the cgroup or
 * throttled by the cgroup cpu.max configuration.
 *
 * The percentage of wall clock time spent in those compound stall
 * states gives pressure numbers between 0 and 100 for each resource,
 * where the SOME percentage indicates workload slowdowns and the FULL
 * percentage indicates reduced CPU utilization:
 *
 *	%SOME = time(SOME) / period
 *	%FULL = time(FULL) / period
 *
 *			Multiple CPUs
 *
 * The more tasks and available CPUs there are, the more work can be
 * performed concurrently. This means that the potential that can go
 * unrealized due to resource contention *also* scales with non-idle
 * tasks and CPUs.
 *
 * Consider a scenario where 257 number crunching tasks are trying to
 * run concurrently on 256 CPUs. If we simply aggregated the task
 * states, we would have to conclude a CPU SOME pressure number of
 * 100%, since *somebody* is waiting on a runqueue at all
 * times. However, that is clearly not the amount of contention the
 * workload is experiencing: only one out of 256 possible execution
 * threads will be contended at any given time, or about 0.4%.
 *
 * Conversely, consider a scenario of 4 tasks and 4 CPUs where at any
 * given time *one* of the tasks is delayed due to a lack of memory.
 * Again, looking purely at the task state would yield a memory FULL
 * pressure number of 0%, since *somebody* is always making forward
 * progress. But again this wouldn't capture the amount of execution
 * potential lost, which is 1 out of 4 CPUs, or 25%.
 *
 * To calculate wasted potential (pressure) with multiple processors,
 * we have to base our calculation on the number of non-idle tasks in
 * conjunction with the number of available CPUs, which is the number
 * of potential execution threads. SOME becomes then the proportion of
 * delayed tasks to possible threads, and FULL is the share of possible
 * threads that are unproductive due to delays:
 *
 *	threads = min(nr_nonidle_tasks, nr_cpus)
 *	   SOME = min(nr_delayed_tasks / threads, 1)
 *	   FULL = (threads - min(nr_productive_tasks, threads)) / threads
 *
 * For the 257 number crunchers on 256 CPUs, this yields:
 *
 *	threads = min(257, 256)
 *	   SOME = min(1 / 256, 1)             = 0.4%
 *	   FULL = (256 - min(256, 256)) / 256 = 0%
 *
 * For the 1 out of 4 memory-delayed tasks, this yields:
 *
 *	threads = min(4, 4)
 *	   SOME = min(1 / 4, 1)               = 25%
 *	   FULL = (4 - min(3, 4)) / 4         = 25%
 *
 * [ Substitute nr_cpus with 1, and you can see that it's a natural
 *   extension of the single-CPU model. ]
 *
 *			Implementation
 *
 * To assess the precise time spent in each such state, we would have
 * to freeze the system on task changes and start/stop the state
 * clocks accordingly. Obviously that doesn't scale in practice.
 *
 * Because the scheduler aims to distribute the compute load evenly
 * among the available CPUs, we can track task state locally to each
 * CPU and, at much lower frequency, extrapolate the global state for
 * the cumulative stall times and the running averages.
 *
 * For each runqueue, we track:
 *
 *	   tSOME[cpu] = time(nr_delayed_tasks[cpu] != 0)
 *	   tFULL[cpu] = time(nr_delayed_tasks[cpu] && !nr_productive_tasks[cpu])
 *	tNONIDLE[cpu] = time(nr_nonidle_tasks[cpu] != 0)
 *
 * and then periodically aggregate:
 *
 *	tNONIDLE = sum(tNONIDLE[i])
 *
 *	   tSOME = sum(tSOME[i] * tNONIDLE[i]) / tNONIDLE
 *	   tFULL = sum(tFULL[i] * tNONIDLE[i]) / tNONIDLE
 *
 *	   %SOME = tSOME / period
 *	   %FULL = tFULL / period
 *
 * This gives us an approximation of pressure that is practical
 * cost-wise, yet way more sensitive and accurate than periodic
 * sampling of the aggregate task states would be.
 */
#ଅନ୍ତର୍ଭାଵ <linux/sched/clock.h>
#ଅନ୍ତର୍ଭାଵ <linux/workqueue.h>
#ଅନ୍ତର୍ଭାଵ <linux/psi.h>
#ଅନ୍ତର୍ଭାଵ "sched.h"

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା psi_bug __read_mostly;

DEFINE_STATIC_ଚାବି_FALSE(psi_disabled);
ସ୍ଥିର DEFINE_STATIC_ଚାବି_TRUE(psi_cgroups_enabled);

#ifdef CONFIG_PSI_DEFAULT_DISABLED
ସ୍ଥିର bool psi_enable;
#ଅନ୍ୟଥା
ସ୍ଥିର bool psi_enable = true;
#endif
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init setup_psi(ଅକ୍ଷର *str)
{
	ଫେରନ୍ତୁ kstrtobool(str, &psi_enable) == 0;
}
__setup("psi=", setup_psi);

/* Running averages - we need to be higher-res than loadavg */
#define PSI_FREQ	(2*HZ+1)	/* 2 sec intervals */
#define EXP_10s		1677		/* 1/exp(2s/10s) as fixed-point */
#define EXP_60s		1981		/* 1/exp(2s/60s) */
#define EXP_300s	2034		/* 1/exp(2s/300s) */

/* PSI trigger definitions */
#define WINDOW_MAX_US 10000000	/* Max window size is 10s */
#define UPDATES_PER_WINDOW 10	/* 10 updates per window */

/* Sampling frequency in nanoseconds */
ସ୍ଥିର u64 psi_period __read_mostly;

/* System-level pressure and stall tracking */
ସ୍ଥିର DEFINE_PER_CPU(ସଂରଚନା psi_group_cpu, system_group_pcpu);
ସଂରଚନା psi_group psi_system = {
	.pcpu = &system_group_pcpu,
};

ସ୍ଥିର DEFINE_PER_CPU(seqcount_t, psi_seq) = SEQCNT_ZERO(psi_seq);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ psi_write_begin(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	write_seqcount_begin(per_cpu_ptr(&psi_seq, cpu));
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ psi_write_end(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	write_seqcount_end(per_cpu_ptr(&psi_seq, cpu));
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ u32 psi_read_begin(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଫେରନ୍ତୁ read_seqcount_begin(per_cpu_ptr(&psi_seq, cpu));
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool psi_read_retry(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, u32 seq)
{
	ଫେରନ୍ତୁ read_seqcount_retry(per_cpu_ptr(&psi_seq, cpu), seq);
}

ସ୍ଥିର ଶୂନ୍ୟ psi_avgs_work(ସଂରଚନା work_struct *work);

ସ୍ଥିର ଶୂନ୍ୟ poll_timer_fn(ସଂରଚନା timer_list *t);

ସ୍ଥିର ଶୂନ୍ୟ group_init(ସଂରଚନା psi_group *group)
{
	group->enabled = true;
	group->avg_last_update = sched_clock();
	group->avg_next_update = group->avg_last_update + psi_period;
	mutex_init(&group->avgs_lock);

	/* Init avg trigger-related members */
	INIT_LIST_HEAD(&group->avg_triggers);
	memset(group->avg_nr_triggers, 0, ଆକାର(group->avg_nr_triggers));
	INIT_DELAYED_WORK(&group->avgs_work, psi_avgs_work);

	/* Init rtpoll trigger-related members */
	atomic_set(&group->rtpoll_scheduled, 0);
	mutex_init(&group->rtpoll_trigger_lock);
	INIT_LIST_HEAD(&group->rtpoll_triggers);
	group->rtpoll_min_period = U32_MAX;
	group->rtpoll_next_update = ULLONG_MAX;
	init_waitqueue_head(&group->rtpoll_wait);
	timer_setup(&group->rtpoll_timer, poll_timer_fn, 0);
	rcu_assign_pointer(group->rtpoll_task, ଖାଲି);
}

ଶୂନ୍ୟ __init psi_init(ଶୂନ୍ୟ)
{
	ଯଦି (!psi_enable) {
		static_branch_enable(&psi_disabled);
		static_branch_disable(&psi_cgroups_enabled);
		ଫେରନ୍ତୁ;
	}

	ଯଦି (!cgroup_psi_enabled())
		static_branch_disable(&psi_cgroups_enabled);

	psi_period = jiffies_to_nsecs(PSI_FREQ);
	group_init(&psi_system);
}

ସ୍ଥିର u32 test_states(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *tasks, u32 state_mask)
{
	ଅଚଳ bool oncpu = state_mask & PSI_ONCPU;

	ଯଦି (tasks[NR_IOWAIT]) {
		state_mask |= BIT(PSI_IO_SOME);
		ଯଦି (!tasks[NR_RUNNING])
			state_mask |= BIT(PSI_IO_FULL);
	}

	ଯଦି (tasks[NR_MEMSTALL]) {
		state_mask |= BIT(PSI_MEM_SOME);
		ଯଦି (tasks[NR_RUNNING] == tasks[NR_MEMSTALL_RUNNING])
			state_mask |= BIT(PSI_MEM_FULL);
	}

	ଯଦି (tasks[NR_RUNNING] > oncpu)
		state_mask |= BIT(PSI_CPU_SOME);

	ଯଦି (tasks[NR_RUNNING] && !oncpu)
		state_mask |= BIT(PSI_CPU_FULL);

	ଯଦି (tasks[NR_IOWAIT] || tasks[NR_MEMSTALL] || tasks[NR_RUNNING])
		state_mask |= BIT(PSI_NONIDLE);

	ଫେରନ୍ତୁ state_mask;
}

ସ୍ଥିର ଶୂନ୍ୟ get_recent_times(ସଂରଚନା psi_group *group, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu,
			     ଗଣନା psi_aggregators aggregator, u32 *times,
			     u32 *pchanged_states)
{
	ସଂରଚନା psi_group_cpu *groupc = per_cpu_ptr(group->pcpu, cpu);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା current_cpu = raw_smp_processor_id();
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା tasks[NR_PSI_TASK_COUNTS];
	u64 now, state_start;
	ଗଣନା psi_states s;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା seq;
	u32 state_mask;

	*pchanged_states = 0;

	/* Snapshot a coherent view of the CPU state */
	କର {
		seq = psi_read_begin(cpu);
		now = cpu_clock(cpu);
		memcpy(times, groupc->times, ଆକାର(groupc->times));
		state_mask = groupc->state_mask;
		state_start = groupc->state_start;
		ଯଦି (cpu == current_cpu)
			memcpy(tasks, groupc->tasks, ଆକାର(groupc->tasks));
	} ଯେତେବେଳେ (psi_read_retry(cpu, seq));

	/* Calculate state time deltas against the previous snapshot */
	ପାଇଁ (s = 0; s < NR_PSI_STATES; s++) {
		u32 delta;
		/*
		 * In addition to already concluded states, we also
		 * incorporate currently active states on the CPU,
		 * since states may last for many sampling periods.
		 *
		 * This way we keep our delta sampling buckets small
		 * (u32) and our reported pressure close to what's
		 * actually happening.
		 */
		ଯଦି (state_mask & (1 << s))
			times[s] += now - state_start;

		delta = times[s] - groupc->times_prev[aggregator][s];
		groupc->times_prev[aggregator][s] = times[s];

		times[s] = delta;
		ଯଦି (delta)
			*pchanged_states |= (1 << s);
	}

	/*
	 * When collect_percpu_times() from the avgs_work, we don't want to
	 * re-arm avgs_work when all CPUs are IDLE. But the current CPU running
	 * this avgs_work is never IDLE, cause avgs_work can't be shut off.
	 * So for the current CPU, we need to re-arm avgs_work only when
	 * (NR_RUNNING > 1 || NR_IOWAIT > 0 || NR_MEMSTALL > 0), for other CPUs
	 * we can just check PSI_NONIDLE delta.
	 */
	ଯଦି (current_work() == &group->avgs_work.work) {
		bool reschedule;

		ଯଦି (cpu == current_cpu)
			reschedule = tasks[NR_RUNNING] +
				     tasks[NR_IOWAIT] +
				     tasks[NR_MEMSTALL] > 1;
		ଅନ୍ୟଥା
			reschedule = *pchanged_states & (1 << PSI_NONIDLE);

		ଯଦି (reschedule)
			*pchanged_states |= PSI_STATE_RESCHEDULE;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ calc_avgs(unsigned long avg[3], ପୂର୍ଣ୍ଣସଂଖ୍ୟା missed_periods,
		      u64 time, u64 period)
{
	unsigned long pct;

	/* Fill in zeroes for periods of no activity */
	ଯଦି (missed_periods) {
		avg[0] = calc_load_n(avg[0], EXP_10s, 0, missed_periods);
		avg[1] = calc_load_n(avg[1], EXP_60s, 0, missed_periods);
		avg[2] = calc_load_n(avg[2], EXP_300s, 0, missed_periods);
	}

	/* Sample the most recent active period */
	pct = div_u64(time * 100, period);
	pct *= FIXED_1;
	avg[0] = calc_load(avg[0], EXP_10s, pct);
	avg[1] = calc_load(avg[1], EXP_60s, pct);
	avg[2] = calc_load(avg[2], EXP_300s, pct);
}

ସ୍ଥିର ଶୂନ୍ୟ collect_percpu_times(ସଂରଚନା psi_group *group,
				 ଗଣନା psi_aggregators aggregator,
				 u32 *pchanged_states)
{
	u64 deltas[NR_PSI_STATES - 1] = { 0, };
	unsigned long nonidle_total = 0;
	u32 changed_states = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା s;

	/*
	 * Collect the per-cpu time buckets and average them into a
	 * single time sample that is normalized to wall clock time.
	 *
	 * For averaging, each CPU is weighted by its non-idle time in
	 * the sampling period. This eliminates artifacts from uneven
	 * loading, or even entirely idle CPUs.
	 */
	for_each_possible_cpu(cpu) {
		u32 times[NR_PSI_STATES];
		u32 nonidle;
		u32 cpu_changed_states;

		get_recent_times(group, cpu, aggregator, times,
				&cpu_changed_states);
		changed_states |= cpu_changed_states;

		nonidle = nsecs_to_jiffies(times[PSI_NONIDLE]);
		nonidle_total += nonidle;

		ପାଇଁ (s = 0; s < PSI_NONIDLE; s++)
			deltas[s] += (u64)times[s] * nonidle;
	}

	/*
	 * Integrate the sample into the running statistics that are
	 * reported to userspace: the cumulative stall times and the
	 * decaying averages.
	 *
	 * Pressure percentages are sampled at PSI_FREQ. We might be
	 * called more often when the user polls more frequently than
	 * that; we might be called less often when there is no task
	 * activity, thus no data, and clock ticks are sporadic. The
	 * below handles both.
	 */

	/* total= */
	ପାଇଁ (s = 0; s < NR_PSI_STATES - 1; s++)
		group->total[aggregator][s] +=
				div_u64(deltas[s], max(nonidle_total, 1UL));

	ଯଦି (pchanged_states)
		*pchanged_states = changed_states;
}

/* Trigger tracking window manipulations */
ସ୍ଥିର ଶୂନ୍ୟ window_reset(ସଂରଚନା psi_window *win, u64 now, u64 ମୂଲ୍ୟ,
			 u64 prev_growth)
{
	win->start_time = now;
	win->start_ମୂଲ୍ୟ = ମୂଲ୍ୟ;
	win->prev_growth = prev_growth;
}

/*
 * PSI growth tracking window update and growth calculation routine.
 *
 * This approximates a sliding tracking window by interpolating
 * partially elapsed windows using historical growth data from the
 * previous intervals. This minimizes memory requirements (by not storing
 * all the intermediate values in the previous window) and simplifies
 * the calculations. It works well because PSI signal changes only in
 * positive direction and over relatively small window sizes the growth
 * is close to linear.
 */
ସ୍ଥିର u64 window_update(ସଂରଚନା psi_window *win, u64 now, u64 ମୂଲ୍ୟ)
{
	u64 elapsed;
	u64 growth;

	elapsed = now - win->start_time;
	growth = ମୂଲ୍ୟ - win->start_ମୂଲ୍ୟ;
	/*
	 * After each tracking window passes win->start_value and
	 * win->start_time get reset and win->prev_growth stores
	 * the average per-window growth of the previous window.
	 * win->prev_growth is then used to interpolate additional
	 * growth from the previous window assuming it was linear.
	 */
	ଯଦି (elapsed > win->size)
		window_reset(win, now, ମୂଲ୍ୟ, growth);
	ଅନ୍ୟଥା {
		u32 remaining;

		remaining = win->size - elapsed;
		growth += div64_u64(win->prev_growth * remaining, win->size);
	}

	ଫେରନ୍ତୁ growth;
}

ସ୍ଥିର ଶୂନ୍ୟ update_triggers(ସଂରଚନା psi_group *group, u64 now,
						   ଗଣନା psi_aggregators aggregator)
{
	ସଂରଚନା psi_trigger *t;
	u64 *total = group->total[aggregator];
	ସଂରଚନା list_head *triggers;
	u64 *aggregator_total;

	ଯଦି (aggregator == PSI_AVGS) {
		triggers = &group->avg_triggers;
		aggregator_total = group->avg_total;
	} ଅନ୍ୟଥା {
		triggers = &group->rtpoll_triggers;
		aggregator_total = group->rtpoll_total;
	}

	/*
	 * On subsequent updates, calculate growth deltas and let
	 * watchers know when their specified thresholds are exceeded.
	 */
	list_for_each_entry(t, triggers, ଗଣ୍ଠି) {
		u64 growth;
		bool new_stall;

		new_stall = aggregator_total[t->state] != total[t->state];

		/* Check for stall activity or a previous threshold breach */
		ଯଦି (!new_stall && !t->pending_event)
			ଜାରି;
		/*
		 * Check for new stall activity, as well as deferred
		 * events that occurred in the last window after the
		 * trigger had already fired (we want to ratelimit
		 * events without dropping any).
		 */
		ଯଦି (new_stall) {
			/* Calculate growth since last update */
			growth = window_update(&t->win, now, total[t->state]);
			ଯଦି (!t->pending_event) {
				ଯଦି (growth < t->threshold)
					ଜାରି;

				t->pending_event = true;
			}
		}
		/* Limit event signaling to once per window */
		ଯଦି (now < t->last_event_time + t->win.size)
			ଜାରି;

		/* Generate an event */
		ଯଦି (cmpxchg(&t->event, 0, 1) == 0) {
			ଯଦି (t->of)
				kernfs_notify(t->of->kn);
			ଅନ୍ୟଥା
				wake_up_interruptible(&t->event_wait);
		}
		t->last_event_time = now;
		/* Reset threshold breach flag once event got generated */
		t->pending_event = false;
	}
}

ସ୍ଥିର u64 update_averages(ସଂରଚନା psi_group *group, u64 now)
{
	unsigned long missed_periods = 0;
	u64 expires, period;
	u64 avg_next_update;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା s;

	/* avgX= */
	expires = group->avg_next_update;
	ଯଦି (now - expires >= psi_period)
		missed_periods = div_u64(now - expires, psi_period);

	/*
	 * The periodic clock tick can get delayed for various
	 * reasons, especially on loaded systems. To avoid clock
	 * drift, we schedule the clock in fixed psi_period intervals.
	 * But the deltas we sample out of the per-cpu buckets above
	 * are based on the actual time elapsing between clock ticks.
	 */
	avg_next_update = expires + ((1 + missed_periods) * psi_period);
	period = now - (group->avg_last_update + (missed_periods * psi_period));
	group->avg_last_update = now;

	ପାଇଁ (s = 0; s < NR_PSI_STATES - 1; s++) {
		u32 sample;

		sample = group->total[PSI_AVGS][s] - group->avg_total[s];
		/*
		 * Due to the lockless sampling of the time buckets,
		 * recorded time deltas can slip into the next period,
		 * which under full pressure can result in samples in
		 * excess of the period length.
		 *
		 * We don't want to report non-sensical pressures in
		 * excess of 100%, nor do we want to drop such events
		 * on the floor. Instead we punt any overage into the
		 * future until pressure subsides. By doing this we
		 * don't underreport the occurring pressure curve, we
		 * just report it delayed by one period length.
		 *
		 * The error isn't cumulative. As soon as another
		 * delta slips from a period P to P+1, by definition
		 * it frees up its time T in P.
		 */
		ଯଦି (sample > period)
			sample = period;
		group->avg_total[s] += sample;
		calc_avgs(group->avg[s], missed_periods, sample, period);
	}

	ଫେରନ୍ତୁ avg_next_update;
}

ସ୍ଥିର ଶୂନ୍ୟ psi_avgs_work(ସଂରଚନା work_struct *work)
{
	ସଂରଚନା delayed_work *dwork;
	ସଂରଚନା psi_group *group;
	u32 changed_states;
	u64 now;

	dwork = to_delayed_work(work);
	group = container_of(dwork, ସଂରଚନା psi_group, avgs_work);

	mutex_lock(&group->avgs_lock);

	now = sched_clock();

	collect_percpu_times(group, PSI_AVGS, &changed_states);
	/*
	 * If there is task activity, periodically fold the per-cpu
	 * times and feed samples into the running averages. If things
	 * are idle and there is no data to process, stop the clock.
	 * Once restarted, we'll catch up the running averages in one
	 * go - see calc_avgs() and missed_periods.
	 */
	ଯଦି (now >= group->avg_next_update) {
		update_triggers(group, now, PSI_AVGS);
		group->avg_next_update = update_averages(group, now);
	}

	ଯଦି (changed_states & PSI_STATE_RESCHEDULE) {
		schedule_delayed_work(dwork, nsecs_to_jiffies(
				group->avg_next_update - now) + 1);
	}

	mutex_unlock(&group->avgs_lock);
}

ସ୍ଥିର ଶୂନ୍ୟ init_rtpoll_triggers(ସଂରଚନା psi_group *group, u64 now)
{
	ସଂରଚନା psi_trigger *t;

	list_for_each_entry(t, &group->rtpoll_triggers, ଗଣ୍ଠି)
		window_reset(&t->win, now,
				group->total[PSI_POLL][t->state], 0);
	memcpy(group->rtpoll_total, group->total[PSI_POLL],
		   ଆକାର(group->rtpoll_total));
	group->rtpoll_next_update = now + group->rtpoll_min_period;
}

/* Schedule rtpolling if it's not already scheduled or forced. */
ସ୍ଥିର ଶୂନ୍ୟ psi_schedule_rtpoll_work(ସଂରଚନା psi_group *group, unsigned long delay,
				   bool force)
{
	/*
	 * atomic_xchg should be called even when !force to provide a
	 * full memory barrier (see the comment inside psi_rtpoll_work).
	 */
	ଯଦି (atomic_xchg(&group->rtpoll_scheduled, 1) && !force)
		ଫେରନ୍ତୁ;

	/*
	 * Only test whether a worker is installed; do not dereference the task.
	 * A racing trigger destruction may leave the timer armed.
	 * psi_cgroup_free() shuts it down before freeing the group.
	 * The system PSI group is static.
	 */
	ଯଦି (likely(rcu_access_pointer(group->rtpoll_task)))
		mod_timer(&group->rtpoll_timer, jiffies + delay);
	ଅନ୍ୟଥା
		atomic_set(&group->rtpoll_scheduled, 0);
}

ସ୍ଥିର ଶୂନ୍ୟ psi_rtpoll_work(ସଂରଚନା psi_group *group)
{
	bool force_reschedule = false;
	u32 changed_states;
	u64 now;

	mutex_lock(&group->rtpoll_trigger_lock);

	now = sched_clock();

	ଯଦି (now > group->rtpoll_until) {
		/*
		 * We are either about to start or might stop rtpolling if no
		 * state change was recorded. Resetting rtpoll_scheduled leaves
		 * a small window for psi_group_change to sneak in and schedule
		 * an immediate rtpoll_work before we get to rescheduling. One
		 * potential extra wakeup at the end of the rtpolling window
		 * should be negligible and rtpoll_next_update still keeps
		 * updates correctly on schedule.
		 */
		atomic_set(&group->rtpoll_scheduled, 0);
		/*
		 * A task change can race with the rtpoll worker that is supposed to
		 * report on it. To avoid missing events, ensure ordering between
		 * rtpoll_scheduled and the task state accesses, such that if the
		 * rtpoll worker misses the state update, the task change is
		 * guaranteed to reschedule the rtpoll worker:
		 *
		 * rtpoll worker:
		 *   atomic_set(rtpoll_scheduled, 0)
		 *   smp_mb()
		 *   LOAD states
		 *
		 * task change:
		 *   STORE states
		 *   if atomic_xchg(rtpoll_scheduled, 1) == 0:
		 *     schedule rtpoll worker
		 *
		 * The atomic_xchg() implies a full barrier.
		 */
		smp_mb();
	} ଅନ୍ୟଥା {
		/* The rtpolling window is not over, keep rescheduling */
		force_reschedule = true;
	}


	collect_percpu_times(group, PSI_POLL, &changed_states);

	ଯଦି (changed_states & group->rtpoll_states) {
		/* Initialize trigger windows when entering rtpolling mode */
		ଯଦି (now > group->rtpoll_until)
			init_rtpoll_triggers(group, now);

		/*
		 * Keep the monitor active for at least the duration of the
		 * minimum tracking window as long as monitor states are
		 * changing.
		 */
		group->rtpoll_until = now +
			group->rtpoll_min_period * UPDATES_PER_WINDOW;
	}

	ଯଦି (now > group->rtpoll_until) {
		group->rtpoll_next_update = ULLONG_MAX;
		ଯାଅ out;
	}

	ଯଦି (now >= group->rtpoll_next_update) {
		ଯଦି (changed_states & group->rtpoll_states) {
			update_triggers(group, now, PSI_POLL);
			memcpy(group->rtpoll_total, group->total[PSI_POLL],
				   ଆକାର(group->rtpoll_total));
		}
		group->rtpoll_next_update = now + group->rtpoll_min_period;
	}

	psi_schedule_rtpoll_work(group,
		nsecs_to_jiffies(group->rtpoll_next_update - now) + 1,
		force_reschedule);

out:
	mutex_unlock(&group->rtpoll_trigger_lock);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା psi_rtpoll_worker(ଶୂନ୍ୟ *data)
{
	ସଂରଚନା psi_group *group = (ସଂରଚନା psi_group *)data;

	sched_set_fifo_ତଳ(current);

	ଯେତେବେଳେ (true) {
		wait_event_interruptible(group->rtpoll_wait,
				atomic_cmpxchg(&group->rtpoll_wakeup, 1, 0) ||
				kthread_should_stop());
		ଯଦି (kthread_should_stop())
			ଭାଙ୍ଗ;

		psi_rtpoll_work(group);
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ poll_timer_fn(ସଂରଚନା timer_list *t)
{
	ସଂରଚନା psi_group *group = timer_container_of(group, t, rtpoll_timer);

	atomic_set(&group->rtpoll_wakeup, 1);
	wake_up_interruptible(&group->rtpoll_wait);
}

ସ୍ଥିର ଶୂନ୍ୟ record_times(ସଂରଚନା psi_group_cpu *groupc, u64 now)
{
	u32 delta;

	delta = now - groupc->state_start;
	groupc->state_start = now;

	ଯଦି (groupc->state_mask & (1 << PSI_IO_SOME)) {
		groupc->times[PSI_IO_SOME] += delta;
		ଯଦି (groupc->state_mask & (1 << PSI_IO_FULL))
			groupc->times[PSI_IO_FULL] += delta;
	}

	ଯଦି (groupc->state_mask & (1 << PSI_MEM_SOME)) {
		groupc->times[PSI_MEM_SOME] += delta;
		ଯଦି (groupc->state_mask & (1 << PSI_MEM_FULL))
			groupc->times[PSI_MEM_FULL] += delta;
	}

	ଯଦି (groupc->state_mask & (1 << PSI_CPU_SOME)) {
		groupc->times[PSI_CPU_SOME] += delta;
		ଯଦି (groupc->state_mask & (1 << PSI_CPU_FULL))
			groupc->times[PSI_CPU_FULL] += delta;
	}

	ଯଦି (groupc->state_mask & (1 << PSI_NONIDLE))
		groupc->times[PSI_NONIDLE] += delta;
}

#define for_each_group(iter, group) \
	ପାଇଁ (typeof(group) iter = group; iter; iter = iter->parent)

ସ୍ଥିର ଶୂନ୍ୟ psi_group_change(ସଂରଚନା psi_group *group, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା clear, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା set,
			     u64 now, bool wake_clock)
{
	ସଂରଚନା psi_group_cpu *groupc;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା t, m;
	u32 state_mask;

	lockdep_assert_rq_held(cpu_rq(cpu));
	groupc = per_cpu_ptr(group->pcpu, cpu);

	/*
	 * Start with TSK_ONCPU, which doesn't have a corresponding
	 * task count - it's just a boolean flag directly encoded in
	 * the state mask. Clear, set, or carry the current state if
	 * no changes are requested.
	 */
	ଯଦି (unlikely(clear & TSK_ONCPU)) {
		state_mask = 0;
		clear &= ~TSK_ONCPU;
	} ଅନ୍ୟଥା ଯଦି (unlikely(set & TSK_ONCPU)) {
		state_mask = PSI_ONCPU;
		set &= ~TSK_ONCPU;
	} ଅନ୍ୟଥା {
		state_mask = groupc->state_mask & PSI_ONCPU;
	}

	/*
	 * The rest of the state mask is calculated based on the task
	 * counts. Update those first, then construct the mask.
	 */
	ପାଇଁ (t = 0, m = clear; m; m &= ~(1 << t), t++) {
		ଯଦି (!(m & (1 << t)))
			ଜାରି;
		ଯଦି (groupc->tasks[t]) {
			groupc->tasks[t]--;
		} ଅନ୍ୟଥା ଯଦି (!psi_bug) {
			printk_deferred(KERN_ERR "psi: task underflow! cpu=%d t=%d tasks=[%u %u %u %u] clear=%x set=%x\n",
					cpu, t, groupc->tasks[0],
					groupc->tasks[1], groupc->tasks[2],
					groupc->tasks[3], clear, set);
			psi_bug = 1;
		}
	}

	ପାଇଁ (t = 0; set; set &= ~(1 << t), t++)
		ଯଦି (set & (1 << t))
			groupc->tasks[t]++;

	ଯଦି (!group->enabled) {
		/*
		 * On the first group change after disabling PSI, conclude
		 * the current state and flush its time. This is unlikely
		 * to matter to the user, but aggregation (get_recent_times)
		 * may have already incorporated the live state into times_prev;
		 * avoid a delta sample underflow when PSI is later re-enabled.
		 */
		ଯଦି (unlikely(groupc->state_mask & (1 << PSI_NONIDLE)))
			record_times(groupc, now);

		groupc->state_mask = state_mask;

		ଫେରନ୍ତୁ;
	}

	state_mask = test_states(groupc->tasks, state_mask);

	/*
	 * Since we care about lost potential, a memstall is FULL
	 * when there are no other working tasks, but also when
	 * the CPU is actively reclaiming and nothing productive
	 * could run even if it were runnable. So when the current
	 * task in a cgroup is in_memstall, the corresponding groupc
	 * on that cpu is in PSI_MEM_FULL state.
	 */
	ଯଦି (unlikely((state_mask & PSI_ONCPU) && cpu_curr(cpu)->in_memstall))
		state_mask |= (1 << PSI_MEM_FULL);

	record_times(groupc, now);

	groupc->state_mask = state_mask;

	ଯଦି (state_mask & group->rtpoll_states)
		psi_schedule_rtpoll_work(group, 1, false);

	ଯଦି (wake_clock && !delayed_work_pending(&group->avgs_work))
		schedule_delayed_work(&group->avgs_work, PSI_FREQ);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା psi_group *task_psi_group(ସଂରଚନା task_struct *task)
{
#ifdef CONFIG_CGROUPS
	ଯଦି (static_branch_likely(&psi_cgroups_enabled))
		ଫେରନ୍ତୁ cgroup_psi(task_dfl_cgroup(task));
#endif
	ଫେରନ୍ତୁ &psi_system;
}

ସ୍ଥିର ଶୂନ୍ୟ psi_flags_change(ସଂରଚନା task_struct *task, ପୂର୍ଣ୍ଣସଂଖ୍ୟା clear, ପୂର୍ଣ୍ଣସଂଖ୍ୟା set)
{
	ଯଦି (((task->psi_flags & set) ||
	     (task->psi_flags & clear) != clear) &&
	    !psi_bug) {
		printk_deferred(KERN_ERR "psi: inconsistent task state! task=%d:%s cpu=%d psi_flags=%x clear=%x set=%x\n",
				task->pid, task->comm, task_cpu(task),
				task->psi_flags, clear, set);
		psi_bug = 1;
	}

	task->psi_flags &= ~clear;
	task->psi_flags |= set;
}

ଶୂନ୍ୟ psi_task_change(ସଂରଚନା task_struct *task, ପୂର୍ଣ୍ଣସଂଖ୍ୟା clear, ପୂର୍ଣ୍ଣସଂଖ୍ୟା set)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = task_cpu(task);
	u64 now;

	ଯଦି (!task->pid)
		ଫେରନ୍ତୁ;

	psi_flags_change(task, clear, set);

	psi_write_begin(cpu);
	now = cpu_clock(cpu);
	for_each_group(group, task_psi_group(task))
		psi_group_change(group, cpu, clear, set, now, true);
	psi_write_end(cpu);
}

ଶୂନ୍ୟ psi_task_switch(ସଂରଚନା task_struct *prev, ସଂରଚନା task_struct *next,
		     bool sleep)
{
	ସଂରଚନା psi_group *common = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = task_cpu(prev);
	u64 now;

	psi_write_begin(cpu);
	now = cpu_clock(cpu);

	ଯଦି (next->pid) {
		psi_flags_change(next, 0, TSK_ONCPU);
		/*
		 * Set TSK_ONCPU on @next's cgroups. If @next shares any
		 * ancestors with @prev, those will already have @prev's
		 * TSK_ONCPU bit set, and we can stop the iteration there.
		 */
		for_each_group(group, task_psi_group(next)) {
			ସଂରଚନା psi_group_cpu *groupc = per_cpu_ptr(group->pcpu, cpu);

			ଯଦି (groupc->state_mask & PSI_ONCPU) {
				common = group;
				ଭାଙ୍ଗ;
			}
			psi_group_change(group, cpu, 0, TSK_ONCPU, now, true);
		}
	}

	ଯଦି (prev->pid) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା clear = TSK_ONCPU, set = 0;
		bool wake_clock = true;

		/*
		 * When we're going to sleep, psi_dequeue() lets us
		 * handle TSK_RUNNING, TSK_MEMSTALL_RUNNING and
		 * TSK_IOWAIT here, where we can combine it with
		 * TSK_ONCPU and save walking common ancestors twice.
		 */
		ଯଦି (sleep) {
			clear |= TSK_RUNNING;
			ଯଦି (prev->in_memstall)
				clear |= TSK_MEMSTALL_RUNNING;
			ଯଦି (prev->in_iowait)
				set |= TSK_IOWAIT;

			/*
			 * Periodic aggregation shuts off if there is a period of no
			 * task changes, so we wake it back up if necessary. However,
			 * don't do this if the task change is the aggregation worker
			 * itself going to sleep, or we'll ping-pong forever.
			 */
			ଯଦି (unlikely((prev->flags & PF_WQ_WORKER) &&
				     wq_worker_last_func(prev) == psi_avgs_work))
				wake_clock = false;
		}

		psi_flags_change(prev, clear, set);

		for_each_group(group, task_psi_group(prev)) {
			ଯଦି (group == common)
				ଭାଙ୍ଗ;
			psi_group_change(group, cpu, clear, set, now, wake_clock);
		}

		/*
		 * TSK_ONCPU is handled up to the common ancestor. If there are
		 * any other differences between the two tasks (e.g. prev goes
		 * to sleep, or only one task is memstall), finish propagating
		 * those differences all the way up to the root.
		 */
		ଯଦି ((prev->psi_flags ^ next->psi_flags) & ~TSK_ONCPU) {
			clear &= ~TSK_ONCPU;
			for_each_group(group, common)
				psi_group_change(group, cpu, clear, set, now, wake_clock);
		}
	}
	psi_write_end(cpu);
}

#ifdef CONFIG_IRQ_TIME_ACCOUNTING
ଶୂନ୍ୟ psi_account_irqtime(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *curr, ସଂରଚନା task_struct *prev)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = task_cpu(curr);
	ସଂରଚନା psi_group_cpu *groupc;
	s64 delta;
	u64 irq;
	u64 now;

	ଯଦି (static_branch_likely(&psi_disabled) || !irqtime_enabled())
		ଫେରନ୍ତୁ;

	ଯଦି (!curr->pid)
		ଫେରନ୍ତୁ;

	lockdep_assert_rq_held(rq);
	ଯଦି (prev && task_psi_group(prev) == task_psi_group(curr))
		ଫେରନ୍ତୁ;

	irq = irq_time_read(cpu);
	delta = (s64)(irq - rq->psi_irq_time);
	ଯଦି (delta <= 0)
		ଫେରନ୍ତୁ;
	rq->psi_irq_time = irq;

	psi_write_begin(cpu);
	now = cpu_clock(cpu);

	for_each_group(group, task_psi_group(curr)) {
		ଯଦି (!group->enabled)
			ଜାରି;

		groupc = per_cpu_ptr(group->pcpu, cpu);

		record_times(groupc, now);
		groupc->times[PSI_IRQ_FULL] += delta;

		ଯଦି (group->rtpoll_states & (1 << PSI_IRQ_FULL))
			psi_schedule_rtpoll_work(group, 1, false);
	}
	psi_write_end(cpu);
}
#endif /* CONFIG_IRQ_TIME_ACCOUNTING */

/**
 * psi_memstall_enter - mark the beginning of a memory stall section
 * @flags: flags to handle nested sections
 *
 * Marks the calling task as being stalled due to a lack of memory,
 * such as waiting for a refault or performing reclaim.
 */
ଶୂନ୍ୟ psi_memstall_enter(unsigned long *flags)
{
	ସଂରଚନା rq_flags rf;
	ସଂରଚନା rq *rq;

	ଯଦି (static_branch_likely(&psi_disabled))
		ଫେରନ୍ତୁ;

	*flags = current->in_memstall;
	ଯଦି (*flags)
		ଫେରନ୍ତୁ;
	/*
	 * in_memstall setting & accounting needs to be atomic wrt
	 * changes to the task's scheduling state, otherwise we can
	 * race with CPU migration.
	 */
	rq = this_rq_lock_irq(&rf);

	current->in_memstall = 1;
	psi_task_change(current, 0, TSK_MEMSTALL | TSK_MEMSTALL_RUNNING);

	rq_unlock_irq(rq, &rf);
}
EXPORT_ପ୍ରତୀକ_GPL(psi_memstall_enter);

/**
 * psi_memstall_leave - mark the end of an memory stall section
 * @flags: flags to handle nested memdelay sections
 *
 * Marks the calling task as no longer stalled due to lack of memory.
 */
ଶୂନ୍ୟ psi_memstall_leave(unsigned long *flags)
{
	ସଂରଚନା rq_flags rf;
	ସଂରଚନା rq *rq;

	ଯଦି (static_branch_likely(&psi_disabled))
		ଫେରନ୍ତୁ;

	ଯଦି (*flags)
		ଫେରନ୍ତୁ;
	/*
	 * in_memstall clearing & accounting needs to be atomic wrt
	 * changes to the task's scheduling state, otherwise we could
	 * race with CPU migration.
	 */
	rq = this_rq_lock_irq(&rf);

	current->in_memstall = 0;
	psi_task_change(current, TSK_MEMSTALL | TSK_MEMSTALL_RUNNING, 0);

	rq_unlock_irq(rq, &rf);
}
EXPORT_ପ୍ରତୀକ_GPL(psi_memstall_leave);

#ifdef CONFIG_CGROUPS
ପୂର୍ଣ୍ଣସଂଖ୍ୟା psi_cgroup_alloc(ସଂରଚନା cgroup *cgroup)
{
	ଯଦି (!static_branch_likely(&psi_cgroups_enabled))
		ଫେରନ୍ତୁ 0;

	cgroup->psi = kzalloc_obj(ସଂରଚନା psi_group);
	ଯଦି (!cgroup->psi)
		ଫେରନ୍ତୁ -ENOMEM;

	cgroup->psi->pcpu = alloc_percpu(ସଂରଚନା psi_group_cpu);
	ଯଦି (!cgroup->psi->pcpu) {
		kfree(cgroup->psi);
		ଫେରନ୍ତୁ -ENOMEM;
	}
	group_init(cgroup->psi);
	cgroup->psi->parent = cgroup_psi(cgroup_parent(cgroup));
	ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ psi_cgroup_free(ସଂରଚନା cgroup *cgroup)
{
	ଯଦି (!static_branch_likely(&psi_cgroups_enabled))
		ଫେରନ୍ତୁ;

	cancel_delayed_work_sync(&cgroup->psi->avgs_work);
	/*
	 * A psi_schedule_rtpoll_work() call racing the last trigger's
	 * destruction may have re-armed the timer after psi_trigger_destroy()
	 * deleted it. Spurious firing while the group is alive is harmless.
	 */
	timer_shutdown_sync(&cgroup->psi->rtpoll_timer);
	free_percpu(cgroup->psi->pcpu);
	/* All triggers must be removed by now */
	WARN_ONCE(cgroup->psi->rtpoll_states, "psi: trigger leak\n");
	kfree(cgroup->psi);
}

/**
 * cgroup_move_task - move task to a different cgroup
 * @task: the task
 * @to: the target css_set
 *
 * Move task to a new cgroup and safely migrate its associated stall
 * state between the different groups.
 *
 * This function acquires the task's rq lock to lock out concurrent
 * changes to the task's scheduling state and - in case the task is
 * running - concurrent changes to its stall state.
 */
ଶୂନ୍ୟ cgroup_move_task(ସଂରଚନା task_struct *task, ସଂରଚନା css_set *to)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା task_flags;
	ସଂରଚନା rq_flags rf;
	ସଂରଚନା rq *rq;

	ଯଦି (!static_branch_likely(&psi_cgroups_enabled)) {
		/*
		 * Lame to do this here, but the scheduler cannot be locked
		 * from the outside, so we move cgroups from inside sched/.
		 */
		rcu_assign_pointer(task->cgroups, to);
		ଫେରନ୍ତୁ;
	}

	rq = task_rq_lock(task, &rf);

	/*
	 * We may race with schedule() dropping the rq lock between
	 * deactivating prev and switching to next. Because the psi
	 * updates from the deactivation are deferred to the switch
	 * callback to save cgroup tree updates, the task's scheduling
	 * state here is not coherent with its psi state:
	 *
	 * schedule()                   cgroup_move_task()
	 *   rq_lock()
	 *   deactivate_task()
	 *     p->on_rq = 0
	 *     psi_dequeue() // defers TSK_RUNNING & TSK_IOWAIT updates
	 *   pick_next_task()
	 *     rq_unlock()
	 *                                rq_lock()
	 *                                psi_task_change() // old cgroup
	 *                                task->cgroups = to
	 *                                psi_task_change() // new cgroup
	 *                                rq_unlock()
	 *     rq_lock()
	 *   psi_sched_switch() // does deferred updates in new cgroup
	 *
	 * Don't rely on the scheduling state. Use psi_flags instead.
	 */
	task_flags = task->psi_flags;

	ଯଦି (task_flags)
		psi_task_change(task, task_flags, 0);

	/* See comment above */
	rcu_assign_pointer(task->cgroups, to);

	ଯଦି (task_flags)
		psi_task_change(task, 0, task_flags);

	task_rq_unlock(rq, task, &rf);
}

ଶୂନ୍ୟ psi_cgroup_restart(ସଂରଚନା psi_group *group)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

	/*
	 * After we disable psi_group->enabled, we don't actually
	 * stop percpu tasks accounting in each psi_group_cpu,
	 * instead only stop test_states() loop, record_times()
	 * and averaging worker, see psi_group_change() for details.
	 *
	 * When disable cgroup PSI, this function has nothing to sync
	 * since cgroup pressure files are hidden and percpu psi_group_cpu
	 * would see !psi_group->enabled and only do task accounting.
	 *
	 * When re-enable cgroup PSI, this function use psi_group_change()
	 * to get correct state mask from test_states() loop on tasks[],
	 * and restart groupc->state_start from now, use .clear = .set = 0
	 * here since no task status really changed.
	 */
	ଯଦି (!group->enabled)
		ଫେରନ୍ତୁ;

	for_each_possible_cpu(cpu) {
		u64 now;

		guard(rq_lock_irq)(cpu_rq(cpu));

		psi_write_begin(cpu);
		now = cpu_clock(cpu);
		psi_group_change(group, cpu, 0, 0, now, true);
		psi_write_end(cpu);
	}
}
#endif /* CONFIG_CGROUPS */

ପୂର୍ଣ୍ଣସଂଖ୍ୟା psi_show(ସଂରଚନା seq_file *m, ସଂରଚନା psi_group *group, ଗଣନା psi_res res)
{
	bool only_full = false;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା full;
	u64 now;

	ଯଦି (static_branch_likely(&psi_disabled))
		ଫେରନ୍ତୁ -EOPNOTSUPP;

#ifdef CONFIG_IRQ_TIME_ACCOUNTING
	ଯଦି (!irqtime_enabled() && res == PSI_IRQ)
		ଫେରନ୍ତୁ -EOPNOTSUPP;
#endif

	/* Update averages before reporting them */
	mutex_lock(&group->avgs_lock);
	now = sched_clock();
	collect_percpu_times(group, PSI_AVGS, ଖାଲି);
	ଯଦି (now >= group->avg_next_update)
		group->avg_next_update = update_averages(group, now);
	mutex_unlock(&group->avgs_lock);

#ifdef CONFIG_IRQ_TIME_ACCOUNTING
	only_full = res == PSI_IRQ;
#endif

	ପାଇଁ (full = 0; full < 2 - only_full; full++) {
		unsigned long avg[3] = { 0, };
		u64 total = 0;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା w;

		/* CPU FULL is undefined at the system level */
		ଯଦି (!(group == &psi_system && res == PSI_CPU && full)) {
			ପାଇଁ (w = 0; w < 3; w++)
				avg[w] = group->avg[res * 2 + full][w];
			total = div_u64(group->total[PSI_AVGS][res * 2 + full],
					NSEC_PER_USEC);
		}

		seq_printf(m, "%s avg10=%lu.%02lu avg60=%lu.%02lu avg300=%lu.%02lu total=%llu\n",
			   full || only_full ? "full" : "some",
			   LOAD_INT(avg[0]), LOAD_FRAC(avg[0]),
			   LOAD_INT(avg[1]), LOAD_FRAC(avg[1]),
			   LOAD_INT(avg[2]), LOAD_FRAC(avg[2]),
			   total);
	}

	ଫେରନ୍ତୁ 0;
}

/*
 * Create @group's rtpoll worker after psi_trigger_create() reported the need
 * for one. kthread creation depends on the whole fork path and we don't want
 * all of that nested inside cgroup_mutex, so the caller must drop it and any
 * other lock that forks can wait behind. If two callers race, the loser stops
 * its never-woken kthread.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା psi_trigger_create_rtpoll_worker(ସଂରଚନା psi_group *group)
{
	ସଂରଚନା task_struct *task;

	task = kthread_create(psi_rtpoll_worker, group, "psimon");
	ଯଦି (IS_ERR(task))
		ଫେରନ୍ତୁ PTR_ERR(task);

	scoped_guard(mutex, &group->rtpoll_trigger_lock) {
		ଯଦି (!rcu_access_pointer(group->rtpoll_task)) {
			atomic_set(&group->rtpoll_wakeup, 0);
			wake_up_process(task);
			rcu_assign_pointer(group->rtpoll_task, task);

			/*
			 * Poll once to catch up on scheduling attempts dropped
			 * while there was no rtpoll worker.
			 */
			psi_schedule_rtpoll_work(group, 1, true);
			ଫେରନ୍ତୁ 0;
		}
	}

	kthread_stop(task);
	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା psi_trigger *psi_trigger_create(ସଂରଚନା psi_group *group, ଅକ୍ଷର *buf,
				       ଗଣନା psi_res res, ସଂରଚନା file *file,
				       ସଂରଚନା kernfs_open_file *of,
				       bool *need_rtpoll_worker)
{
	ସଂରଚନା psi_trigger *t;
	ଗଣନା psi_states state;
	u32 threshold_us;
	bool privileged;
	u32 window_us;

	*need_rtpoll_worker = false;

	ଯଦି (static_branch_likely(&psi_disabled))
		ଫେରନ୍ତୁ ERR_PTR(-EOPNOTSUPP);

	/*
	 * Checking the privilege here on file->f_cred implies that a privileged user
	 * could open the file and delegate the write to an unprivileged one.
	 */
	privileged = cap_raised(file->f_cred->cap_effective, CAP_SYS_RESOURCE);

	ଯଦି (sscanf(buf, "some %u %u", &threshold_us, &window_us) == 2)
		state = PSI_IO_SOME + res * 2;
	ଅନ୍ୟଥା ଯଦି (sscanf(buf, "full %u %u", &threshold_us, &window_us) == 2)
		state = PSI_IO_FULL + res * 2;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ ERR_PTR(-EINVAL);

#ifdef CONFIG_IRQ_TIME_ACCOUNTING
	ଯଦି (res == PSI_IRQ && --state != PSI_IRQ_FULL)
		ଫେରନ୍ତୁ ERR_PTR(-EINVAL);
#endif

	ଯଦି (state >= PSI_NONIDLE)
		ଫେରନ୍ତୁ ERR_PTR(-EINVAL);

	ଯଦି (window_us == 0 || window_us > WINDOW_MAX_US)
		ଫେରନ୍ତୁ ERR_PTR(-EINVAL);

	/*
	 * Unprivileged users can only use 2s windows so that averages aggregation
	 * work is used, and no RT threads need to be spawned.
	 */
	ଯଦି (!privileged && window_us % 2000000)
		ଫେରନ୍ତୁ ERR_PTR(-EINVAL);

	/* Check threshold */
	ଯଦି (threshold_us == 0 || threshold_us > window_us)
		ଫେରନ୍ତୁ ERR_PTR(-EINVAL);

	t = kmalloc_obj(*t);
	ଯଦି (!t)
		ଫେରନ୍ତୁ ERR_PTR(-ENOMEM);

	t->group = group;
	t->state = state;
	t->threshold = threshold_us * NSEC_PER_USEC;
	t->win.size = window_us * NSEC_PER_USEC;
	window_reset(&t->win, sched_clock(),
			group->total[PSI_POLL][t->state], 0);

	t->event = 0;
	t->last_event_time = 0;
	t->of = of;
	ଯଦି (!of)
		init_waitqueue_head(&t->event_wait);
	t->pending_event = false;
	t->aggregator = privileged ? PSI_POLL : PSI_AVGS;

	ଯଦି (privileged) {
		mutex_lock(&group->rtpoll_trigger_lock);

		list_add(&t->ଗଣ୍ଠି, &group->rtpoll_triggers);
		group->rtpoll_min_period = min(group->rtpoll_min_period,
			div_u64(t->win.size, UPDATES_PER_WINDOW));
		group->rtpoll_nr_triggers[t->state]++;
		group->rtpoll_states |= (1 << t->state);

		*need_rtpoll_worker = !rcu_access_pointer(group->rtpoll_task);

		mutex_unlock(&group->rtpoll_trigger_lock);
	} ଅନ୍ୟଥା {
		mutex_lock(&group->avgs_lock);

		list_add(&t->ଗଣ୍ଠି, &group->avg_triggers);
		group->avg_nr_triggers[t->state]++;

		mutex_unlock(&group->avgs_lock);
	}
	ଫେରନ୍ତୁ t;
}

ଶୂନ୍ୟ psi_trigger_destroy(ସଂରଚନା psi_trigger *t)
{
	ସଂରଚନା psi_group *group;
	ସଂରଚନା task_struct *task_to_destroy = ଖାଲି;

	/*
	 * We do not check psi_disabled since it might have been disabled after
	 * the trigger got created.
	 */
	ଯଦି (!t)
		ଫେରନ୍ତୁ;

	group = t->group;
	/*
	 * Wakeup waiters to stop polling and clear the queue to prevent it from
	 * being accessed later. Can happen if cgroup is deleted from under a
	 * polling process.
	 */
	ଯଦି (t->of)
		kernfs_notify(t->of->kn);
	ଅନ୍ୟଥା
		wake_up_interruptible(&t->event_wait);

	ଯଦି (t->aggregator == PSI_AVGS) {
		mutex_lock(&group->avgs_lock);
		ଯଦି (!list_empty(&t->ଗଣ୍ଠି)) {
			list_del(&t->ଗଣ୍ଠି);
			group->avg_nr_triggers[t->state]--;
		}
		mutex_unlock(&group->avgs_lock);
	} ଅନ୍ୟଥା {
		mutex_lock(&group->rtpoll_trigger_lock);
		ଯଦି (!list_empty(&t->ଗଣ୍ଠି)) {
			ସଂରଚନା psi_trigger *tmp;
			u64 period = ULLONG_MAX;

			list_del(&t->ଗଣ୍ଠି);
			group->rtpoll_nr_triggers[t->state]--;
			ଯଦି (!group->rtpoll_nr_triggers[t->state])
				group->rtpoll_states &= ~(1 << t->state);
			/*
			 * Reset min update period for the remaining triggers
			 * iff the destroying trigger had the min window size.
			 */
			ଯଦି (group->rtpoll_min_period == div_u64(t->win.size, UPDATES_PER_WINDOW)) {
				list_for_each_entry(tmp, &group->rtpoll_triggers, ଗଣ୍ଠି)
					period = min(period, div_u64(tmp->win.size,
							UPDATES_PER_WINDOW));
				group->rtpoll_min_period = period;
			}
			/* Destroy rtpoll_task when the last trigger is destroyed */
			ଯଦି (group->rtpoll_states == 0) {
				group->rtpoll_until = 0;
				task_to_destroy = rcu_dereference_protected(
						group->rtpoll_task,
						lockdep_is_held(&group->rtpoll_trigger_lock));
				rcu_assign_pointer(group->rtpoll_task, ଖାଲି);
				timer_delete(&group->rtpoll_timer);
			}
		}
		mutex_unlock(&group->rtpoll_trigger_lock);
	}

	/*
	 * Stop kthread 'psimon' after releasing rtpoll_trigger_lock to prevent
	 * a deadlock while waiting for psi_rtpoll_work to acquire
	 * rtpoll_trigger_lock
	 */
	ଯଦି (task_to_destroy) {
		kthread_stop(task_to_destroy);
		atomic_set(&group->rtpoll_scheduled, 0);
	}
	kfree(t);
}

__poll_t psi_trigger_poll(ଶୂନ୍ୟ **trigger_ptr,
				ସଂରଚନା file *file, poll_table *wait)
{
	__poll_t ret = DEFAULT_POLLMASK;
	ସଂରଚନା psi_trigger *t;

	ଯଦି (static_branch_likely(&psi_disabled))
		ଫେରନ୍ତୁ DEFAULT_POLLMASK | EPOLLERR | EPOLLPRI;

	t = smp_load_acquire(trigger_ptr);
	ଯଦି (!t)
		ଫେରନ୍ତୁ DEFAULT_POLLMASK | EPOLLERR | EPOLLPRI;

	ଯଦି (t->of)
		kernfs_generic_poll(t->of, wait);
	ଅନ୍ୟଥା
		poll_wait(file, &t->event_wait, wait);

	ଯଦି (cmpxchg(&t->event, 1, 0) == 1)
		ret |= EPOLLPRI;

	ଫେରନ୍ତୁ ret;
}

#ifdef CONFIG_PROC_FS
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା psi_io_show(ସଂରଚନା seq_file *m, ଶୂନ୍ୟ *v)
{
	ଫେରନ୍ତୁ psi_show(m, &psi_system, PSI_IO);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା psi_memory_show(ସଂରଚନା seq_file *m, ଶୂନ୍ୟ *v)
{
	ଫେରନ୍ତୁ psi_show(m, &psi_system, PSI_MEM);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା psi_cpu_show(ସଂରଚନା seq_file *m, ଶୂନ୍ୟ *v)
{
	ଫେରନ୍ତୁ psi_show(m, &psi_system, PSI_CPU);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା psi_io_open(ସଂରଚନା inode *inode, ସଂରଚନା file *file)
{
	ଫେରନ୍ତୁ single_open(file, psi_io_show, ଖାଲି);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା psi_memory_open(ସଂରଚନା inode *inode, ସଂରଚନା file *file)
{
	ଫେରନ୍ତୁ single_open(file, psi_memory_show, ଖାଲି);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା psi_cpu_open(ସଂରଚନା inode *inode, ସଂରଚନା file *file)
{
	ଫେରନ୍ତୁ single_open(file, psi_cpu_show, ଖାଲି);
}

ସ୍ଥିର ssize_t psi_write(ସଂରଚନା file *file, ଅଚଳ ଅକ୍ଷର __user *user_buf,
			 size_t nbytes, ଗଣନା psi_res res)
{
	ଅକ୍ଷର buf[32];
	size_t buf_size;
	ସଂରଚନା seq_file *seq;
	ସଂରଚନା psi_trigger *new;
	bool need_rtpoll_worker;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ଯଦି (static_branch_likely(&psi_disabled))
		ଫେରନ୍ତୁ -EOPNOTSUPP;

	ଯଦି (!nbytes)
		ଫେରନ୍ତୁ -EINVAL;

	buf_size = min(nbytes, ଆକାର(buf));
	ଯଦି (copy_from_user(buf, user_buf, buf_size))
		ଫେରନ୍ତୁ -EFAULT;

	buf[buf_size - 1] = '\0';

	seq = file->private_data;

	/* Take seq->lock to protect seq->private from concurrent writes */
	mutex_lock(&seq->lock);

	/* Allow only one trigger per file descriptor */
	ଯଦି (seq->ବ୍ୟକ୍ତିଗତ) {
		mutex_unlock(&seq->lock);
		ଫେରନ୍ତୁ -EBUSY;
	}

	new = psi_trigger_create(&psi_system, buf, res, file, ଖାଲି,
				 &need_rtpoll_worker);
	ଯଦି (IS_ERR(new)) {
		mutex_unlock(&seq->lock);
		ଫେରନ୍ତୁ PTR_ERR(new);
	}

	ଯଦି (need_rtpoll_worker) {
		ret = psi_trigger_create_rtpoll_worker(&psi_system);
		ଯଦି (ret) {
			psi_trigger_destroy(new);
			mutex_unlock(&seq->lock);
			ଫେରନ୍ତୁ ret;
		}
	}

	smp_store_release(&seq->ବ୍ୟକ୍ତିଗତ, new);
	mutex_unlock(&seq->lock);

	ଫେରନ୍ତୁ nbytes;
}

ସ୍ଥିର ssize_t psi_io_write(ସଂରଚନା file *file, ଅଚଳ ଅକ୍ଷର __user *user_buf,
			    size_t nbytes, loff_t *ppos)
{
	ଫେରନ୍ତୁ psi_write(file, user_buf, nbytes, PSI_IO);
}

ସ୍ଥିର ssize_t psi_memory_write(ସଂରଚନା file *file, ଅଚଳ ଅକ୍ଷର __user *user_buf,
				size_t nbytes, loff_t *ppos)
{
	ଫେରନ୍ତୁ psi_write(file, user_buf, nbytes, PSI_MEM);
}

ସ୍ଥିର ssize_t psi_cpu_write(ସଂରଚନା file *file, ଅଚଳ ଅକ୍ଷର __user *user_buf,
			     size_t nbytes, loff_t *ppos)
{
	ଫେରନ୍ତୁ psi_write(file, user_buf, nbytes, PSI_CPU);
}

ସ୍ଥିର __poll_t psi_fop_poll(ସଂରଚନା file *file, poll_table *wait)
{
	ସଂରଚନା seq_file *seq = file->private_data;

	ଫେରନ୍ତୁ psi_trigger_poll(&seq->ବ୍ୟକ୍ତିଗତ, file, wait);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା psi_fop_release(ସଂରଚନା inode *inode, ସଂରଚନା file *file)
{
	ସଂରଚନା seq_file *seq = file->private_data;

	psi_trigger_destroy(seq->ବ୍ୟକ୍ତିଗତ);
	ଫେରନ୍ତୁ single_release(inode, file);
}

ସ୍ଥିର ଅଚଳ ସଂରଚନା proc_ops psi_io_proc_ops = {
	.proc_open	= psi_io_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_io_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

ସ୍ଥିର ଅଚଳ ସଂରଚନା proc_ops psi_memory_proc_ops = {
	.proc_open	= psi_memory_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_memory_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

ସ୍ଥିର ଅଚଳ ସଂରଚନା proc_ops psi_cpu_proc_ops = {
	.proc_open	= psi_cpu_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_cpu_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

#ifdef CONFIG_IRQ_TIME_ACCOUNTING
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା psi_irq_show(ସଂରଚନା seq_file *m, ଶୂନ୍ୟ *v)
{
	ଫେରନ୍ତୁ psi_show(m, &psi_system, PSI_IRQ);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା psi_irq_open(ସଂରଚନା inode *inode, ସଂରଚନା file *file)
{
	ଫେରନ୍ତୁ single_open(file, psi_irq_show, ଖାଲି);
}

ସ୍ଥିର ssize_t psi_irq_write(ସଂରଚନା file *file, ଅଚଳ ଅକ୍ଷର __user *user_buf,
			     size_t nbytes, loff_t *ppos)
{
	ଫେରନ୍ତୁ psi_write(file, user_buf, nbytes, PSI_IRQ);
}

ସ୍ଥିର ଅଚଳ ସଂରଚନା proc_ops psi_irq_proc_ops = {
	.proc_open	= psi_irq_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_irq_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};
#endif /* CONFIG_IRQ_TIME_ACCOUNTING */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init psi_proc_init(ଶୂନ୍ୟ)
{
	ଯଦି (psi_enable) {
		proc_mkdir("pressure", ଖାଲି);
		proc_create("pressure/io", 0666, ଖାଲି, &psi_io_proc_ops);
		proc_create("pressure/memory", 0666, ଖାଲି, &psi_memory_proc_ops);
		proc_create("pressure/cpu", 0666, ଖାଲି, &psi_cpu_proc_ops);
#ifdef CONFIG_IRQ_TIME_ACCOUNTING
		proc_create("pressure/irq", 0666, ଖାଲି, &psi_irq_proc_ops);
#endif
	}
	ଫେରନ୍ତୁ 0;
}
module_init(psi_proc_init);

#endif /* CONFIG_PROC_FS */
