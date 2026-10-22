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
#অন্তর্ভাৱ <linux/sched/clock.h>
#অন্তর্ভাৱ <linux/workqueue.h>
#অন্তর্ভাৱ <linux/psi.h>
#অন্তর্ভাৱ "sched.h"

স্থির পূর্ণাঙ্ক psi_bug __read_mostly;

DEFINE_STATIC_চাবি_FALSE(psi_disabled);
স্থির DEFINE_STATIC_চাবি_TRUE(psi_cgroups_enabled);

#ifdef CONFIG_PSI_DEFAULT_DISABLED
স্থির bool psi_enable;
#অন্যথা
স্থির bool psi_enable = true;
#endif
স্থির পূর্ণাঙ্ক __init setup_psi(ৱর্ণ *str)
{
	ঘূৰাই kstrtobool(str, &psi_enable) == 0;
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
স্থির u64 psi_period __read_mostly;

/* System-level pressure and stall tracking */
স্থির DEFINE_PER_CPU(সংরচনা psi_group_cpu, system_group_pcpu);
সংরচনা psi_group psi_system = {
	.pcpu = &system_group_pcpu,
};

স্থির DEFINE_PER_CPU(seqcount_t, psi_seq) = SEQCNT_ZERO(psi_seq);

স্থির অন্তঃস্থ শূন্য psi_write_begin(পূর্ণাঙ্ক cpu)
{
	write_seqcount_begin(per_cpu_ptr(&psi_seq, cpu));
}

স্থির অন্তঃস্থ শূন্য psi_write_end(পূর্ণাঙ্ক cpu)
{
	write_seqcount_end(per_cpu_ptr(&psi_seq, cpu));
}

স্থির অন্তঃস্থ u32 psi_read_begin(পূর্ণাঙ্ক cpu)
{
	ঘূৰাই read_seqcount_begin(per_cpu_ptr(&psi_seq, cpu));
}

স্থির অন্তঃস্থ bool psi_read_retry(পূর্ণাঙ্ক cpu, u32 seq)
{
	ঘূৰাই read_seqcount_retry(per_cpu_ptr(&psi_seq, cpu), seq);
}

স্থির শূন্য psi_avgs_work(সংরচনা work_struct *work);

স্থির শূন্য poll_timer_fn(সংরচনা timer_list *t);

স্থির শূন্য group_init(সংরচনা psi_group *group)
{
	group->enabled = true;
	group->avg_last_update = sched_clock();
	group->avg_next_update = group->avg_last_update + psi_period;
	mutex_init(&group->avgs_lock);

	/* Init avg trigger-related members */
	INIT_LIST_HEAD(&group->avg_triggers);
	memset(group->avg_nr_triggers, 0, পরিমাণ(group->avg_nr_triggers));
	INIT_DELAYED_WORK(&group->avgs_work, psi_avgs_work);

	/* Init rtpoll trigger-related members */
	atomic_set(&group->rtpoll_scheduled, 0);
	mutex_init(&group->rtpoll_trigger_lock);
	INIT_LIST_HEAD(&group->rtpoll_triggers);
	group->rtpoll_min_period = U32_MAX;
	group->rtpoll_next_update = ULLONG_MAX;
	init_waitqueue_head(&group->rtpoll_wait);
	timer_setup(&group->rtpoll_timer, poll_timer_fn, 0);
	rcu_assign_pointer(group->rtpoll_task, রিক্ত);
}

শূন্য __init psi_init(শূন্য)
{
	যদি (!psi_enable) {
		static_branch_enable(&psi_disabled);
		static_branch_disable(&psi_cgroups_enabled);
		ঘূৰাই;
	}

	যদি (!cgroup_psi_enabled())
		static_branch_disable(&psi_cgroups_enabled);

	psi_period = jiffies_to_nsecs(PSI_FREQ);
	group_init(&psi_system);
}

স্থির u32 test_states(unsigned পূর্ণাঙ্ক *tasks, u32 state_mask)
{
	ধ্রুৱ bool oncpu = state_mask & PSI_ONCPU;

	যদি (tasks[NR_IOWAIT]) {
		state_mask |= BIT(PSI_IO_SOME);
		যদি (!tasks[NR_RUNNING])
			state_mask |= BIT(PSI_IO_FULL);
	}

	যদি (tasks[NR_MEMSTALL]) {
		state_mask |= BIT(PSI_MEM_SOME);
		যদি (tasks[NR_RUNNING] == tasks[NR_MEMSTALL_RUNNING])
			state_mask |= BIT(PSI_MEM_FULL);
	}

	যদি (tasks[NR_RUNNING] > oncpu)
		state_mask |= BIT(PSI_CPU_SOME);

	যদি (tasks[NR_RUNNING] && !oncpu)
		state_mask |= BIT(PSI_CPU_FULL);

	যদি (tasks[NR_IOWAIT] || tasks[NR_MEMSTALL] || tasks[NR_RUNNING])
		state_mask |= BIT(PSI_NONIDLE);

	ঘূৰাই state_mask;
}

স্থির শূন্য get_recent_times(সংরচনা psi_group *group, পূর্ণাঙ্ক cpu,
			     গণনা psi_aggregators aggregator, u32 *times,
			     u32 *pchanged_states)
{
	সংরচনা psi_group_cpu *groupc = per_cpu_ptr(group->pcpu, cpu);
	পূর্ণাঙ্ক current_cpu = raw_smp_processor_id();
	unsigned পূর্ণাঙ্ক tasks[NR_PSI_TASK_COUNTS];
	u64 now, state_start;
	গণনা psi_states s;
	unsigned পূর্ণাঙ্ক seq;
	u32 state_mask;

	*pchanged_states = 0;

	/* Snapshot a coherent view of the CPU state */
	কৰা {
		seq = psi_read_begin(cpu);
		now = cpu_clock(cpu);
		memcpy(times, groupc->times, পরিমাণ(groupc->times));
		state_mask = groupc->state_mask;
		state_start = groupc->state_start;
		যদি (cpu == current_cpu)
			memcpy(tasks, groupc->tasks, পরিমাণ(groupc->tasks));
	} যেতিয়ালৈ (psi_read_retry(cpu, seq));

	/* Calculate state time deltas against the previous snapshot */
	বাবে (s = 0; s < NR_PSI_STATES; s++) {
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
		যদি (state_mask & (1 << s))
			times[s] += now - state_start;

		delta = times[s] - groupc->times_prev[aggregator][s];
		groupc->times_prev[aggregator][s] = times[s];

		times[s] = delta;
		যদি (delta)
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
	যদি (current_work() == &group->avgs_work.work) {
		bool reschedule;

		যদি (cpu == current_cpu)
			reschedule = tasks[NR_RUNNING] +
				     tasks[NR_IOWAIT] +
				     tasks[NR_MEMSTALL] > 1;
		অন্যথা
			reschedule = *pchanged_states & (1 << PSI_NONIDLE);

		যদি (reschedule)
			*pchanged_states |= PSI_STATE_RESCHEDULE;
	}
}

স্থির শূন্য calc_avgs(unsigned long avg[3], পূর্ণাঙ্ক missed_periods,
		      u64 time, u64 period)
{
	unsigned long pct;

	/* Fill in zeroes for periods of no activity */
	যদি (missed_periods) {
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

স্থির শূন্য collect_percpu_times(সংরচনা psi_group *group,
				 গণনা psi_aggregators aggregator,
				 u32 *pchanged_states)
{
	u64 deltas[NR_PSI_STATES - 1] = { 0, };
	unsigned long nonidle_total = 0;
	u32 changed_states = 0;
	পূর্ণাঙ্ক cpu;
	পূর্ণাঙ্ক s;

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

		বাবে (s = 0; s < PSI_NONIDLE; s++)
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
	বাবে (s = 0; s < NR_PSI_STATES - 1; s++)
		group->total[aggregator][s] +=
				div_u64(deltas[s], max(nonidle_total, 1UL));

	যদি (pchanged_states)
		*pchanged_states = changed_states;
}

/* Trigger tracking window manipulations */
স্থির শূন্য window_reset(সংরচনা psi_window *win, u64 now, u64 মান,
			 u64 prev_growth)
{
	win->start_time = now;
	win->start_মান = মান;
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
স্থির u64 window_update(সংরচনা psi_window *win, u64 now, u64 মান)
{
	u64 elapsed;
	u64 growth;

	elapsed = now - win->start_time;
	growth = মান - win->start_মান;
	/*
	 * After each tracking window passes win->start_value and
	 * win->start_time get reset and win->prev_growth stores
	 * the average per-window growth of the previous window.
	 * win->prev_growth is then used to interpolate additional
	 * growth from the previous window assuming it was linear.
	 */
	যদি (elapsed > win->size)
		window_reset(win, now, মান, growth);
	অন্যথা {
		u32 remaining;

		remaining = win->size - elapsed;
		growth += div64_u64(win->prev_growth * remaining, win->size);
	}

	ঘূৰাই growth;
}

স্থির শূন্য update_triggers(সংরচনা psi_group *group, u64 now,
						   গণনা psi_aggregators aggregator)
{
	সংরচনা psi_trigger *t;
	u64 *total = group->total[aggregator];
	সংরচনা list_head *triggers;
	u64 *aggregator_total;

	যদি (aggregator == PSI_AVGS) {
		triggers = &group->avg_triggers;
		aggregator_total = group->avg_total;
	} অন্যথা {
		triggers = &group->rtpoll_triggers;
		aggregator_total = group->rtpoll_total;
	}

	/*
	 * On subsequent updates, calculate growth deltas and let
	 * watchers know when their specified thresholds are exceeded.
	 */
	list_for_each_entry(t, triggers, গ্ৰন্থি) {
		u64 growth;
		bool new_stall;

		new_stall = aggregator_total[t->state] != total[t->state];

		/* Check for stall activity or a previous threshold breach */
		যদি (!new_stall && !t->pending_event)
			অনুৱর্ত;
		/*
		 * Check for new stall activity, as well as deferred
		 * events that occurred in the last window after the
		 * trigger had already fired (we want to ratelimit
		 * events without dropping any).
		 */
		যদি (new_stall) {
			/* Calculate growth since last update */
			growth = window_update(&t->win, now, total[t->state]);
			যদি (!t->pending_event) {
				যদি (growth < t->threshold)
					অনুৱর্ত;

				t->pending_event = true;
			}
		}
		/* Limit event signaling to once per window */
		যদি (now < t->last_event_time + t->win.size)
			অনুৱর্ত;

		/* Generate an event */
		যদি (cmpxchg(&t->event, 0, 1) == 0) {
			যদি (t->of)
				kernfs_notify(t->of->kn);
			অন্যথা
				wake_up_interruptible(&t->event_wait);
		}
		t->last_event_time = now;
		/* Reset threshold breach flag once event got generated */
		t->pending_event = false;
	}
}

স্থির u64 update_averages(সংরচনা psi_group *group, u64 now)
{
	unsigned long missed_periods = 0;
	u64 expires, period;
	u64 avg_next_update;
	পূর্ণাঙ্ক s;

	/* avgX= */
	expires = group->avg_next_update;
	যদি (now - expires >= psi_period)
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

	বাবে (s = 0; s < NR_PSI_STATES - 1; s++) {
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
		যদি (sample > period)
			sample = period;
		group->avg_total[s] += sample;
		calc_avgs(group->avg[s], missed_periods, sample, period);
	}

	ঘূৰাই avg_next_update;
}

স্থির শূন্য psi_avgs_work(সংরচনা work_struct *work)
{
	সংরচনা delayed_work *dwork;
	সংরচনা psi_group *group;
	u32 changed_states;
	u64 now;

	dwork = to_delayed_work(work);
	group = container_of(dwork, সংরচনা psi_group, avgs_work);

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
	যদি (now >= group->avg_next_update) {
		update_triggers(group, now, PSI_AVGS);
		group->avg_next_update = update_averages(group, now);
	}

	যদি (changed_states & PSI_STATE_RESCHEDULE) {
		schedule_delayed_work(dwork, nsecs_to_jiffies(
				group->avg_next_update - now) + 1);
	}

	mutex_unlock(&group->avgs_lock);
}

স্থির শূন্য init_rtpoll_triggers(সংরচনা psi_group *group, u64 now)
{
	সংরচনা psi_trigger *t;

	list_for_each_entry(t, &group->rtpoll_triggers, গ্ৰন্থি)
		window_reset(&t->win, now,
				group->total[PSI_POLL][t->state], 0);
	memcpy(group->rtpoll_total, group->total[PSI_POLL],
		   পরিমাণ(group->rtpoll_total));
	group->rtpoll_next_update = now + group->rtpoll_min_period;
}

/* Schedule rtpolling if it's not already scheduled or forced. */
স্থির শূন্য psi_schedule_rtpoll_work(সংরচনা psi_group *group, unsigned long delay,
				   bool force)
{
	/*
	 * atomic_xchg should be called even when !force to provide a
	 * full memory barrier (see the comment inside psi_rtpoll_work).
	 */
	যদি (atomic_xchg(&group->rtpoll_scheduled, 1) && !force)
		ঘূৰাই;

	/*
	 * Only test whether a worker is installed; do not dereference the task.
	 * A racing trigger destruction may leave the timer armed.
	 * psi_cgroup_free() shuts it down before freeing the group.
	 * The system PSI group is static.
	 */
	যদি (likely(rcu_access_pointer(group->rtpoll_task)))
		mod_timer(&group->rtpoll_timer, jiffies + delay);
	অন্যথা
		atomic_set(&group->rtpoll_scheduled, 0);
}

স্থির শূন্য psi_rtpoll_work(সংরচনা psi_group *group)
{
	bool force_reschedule = false;
	u32 changed_states;
	u64 now;

	mutex_lock(&group->rtpoll_trigger_lock);

	now = sched_clock();

	যদি (now > group->rtpoll_until) {
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
	} অন্যথা {
		/* The rtpolling window is not over, keep rescheduling */
		force_reschedule = true;
	}


	collect_percpu_times(group, PSI_POLL, &changed_states);

	যদি (changed_states & group->rtpoll_states) {
		/* Initialize trigger windows when entering rtpolling mode */
		যদি (now > group->rtpoll_until)
			init_rtpoll_triggers(group, now);

		/*
		 * Keep the monitor active for at least the duration of the
		 * minimum tracking window as long as monitor states are
		 * changing.
		 */
		group->rtpoll_until = now +
			group->rtpoll_min_period * UPDATES_PER_WINDOW;
	}

	যদি (now > group->rtpoll_until) {
		group->rtpoll_next_update = ULLONG_MAX;
		গচ্ছ out;
	}

	যদি (now >= group->rtpoll_next_update) {
		যদি (changed_states & group->rtpoll_states) {
			update_triggers(group, now, PSI_POLL);
			memcpy(group->rtpoll_total, group->total[PSI_POLL],
				   পরিমাণ(group->rtpoll_total));
		}
		group->rtpoll_next_update = now + group->rtpoll_min_period;
	}

	psi_schedule_rtpoll_work(group,
		nsecs_to_jiffies(group->rtpoll_next_update - now) + 1,
		force_reschedule);

out:
	mutex_unlock(&group->rtpoll_trigger_lock);
}

স্থির পূর্ণাঙ্ক psi_rtpoll_worker(শূন্য *data)
{
	সংরচনা psi_group *group = (সংরচনা psi_group *)data;

	sched_set_fifo_নিচেৰ(current);

	যেতিয়ালৈ (true) {
		wait_event_interruptible(group->rtpoll_wait,
				atomic_cmpxchg(&group->rtpoll_wakeup, 1, 0) ||
				kthread_should_stop());
		যদি (kthread_should_stop())
			ভঙ্গ;

		psi_rtpoll_work(group);
	}
	ঘূৰাই 0;
}

স্থির শূন্য poll_timer_fn(সংরচনা timer_list *t)
{
	সংরচনা psi_group *group = timer_container_of(group, t, rtpoll_timer);

	atomic_set(&group->rtpoll_wakeup, 1);
	wake_up_interruptible(&group->rtpoll_wait);
}

স্থির শূন্য record_times(সংরচনা psi_group_cpu *groupc, u64 now)
{
	u32 delta;

	delta = now - groupc->state_start;
	groupc->state_start = now;

	যদি (groupc->state_mask & (1 << PSI_IO_SOME)) {
		groupc->times[PSI_IO_SOME] += delta;
		যদি (groupc->state_mask & (1 << PSI_IO_FULL))
			groupc->times[PSI_IO_FULL] += delta;
	}

	যদি (groupc->state_mask & (1 << PSI_MEM_SOME)) {
		groupc->times[PSI_MEM_SOME] += delta;
		যদি (groupc->state_mask & (1 << PSI_MEM_FULL))
			groupc->times[PSI_MEM_FULL] += delta;
	}

	যদি (groupc->state_mask & (1 << PSI_CPU_SOME)) {
		groupc->times[PSI_CPU_SOME] += delta;
		যদি (groupc->state_mask & (1 << PSI_CPU_FULL))
			groupc->times[PSI_CPU_FULL] += delta;
	}

	যদি (groupc->state_mask & (1 << PSI_NONIDLE))
		groupc->times[PSI_NONIDLE] += delta;
}

#define for_each_group(iter, group) \
	বাবে (typeof(group) iter = group; iter; iter = iter->parent)

স্থির শূন্য psi_group_change(সংরচনা psi_group *group, পূর্ণাঙ্ক cpu,
			     unsigned পূর্ণাঙ্ক clear, unsigned পূর্ণাঙ্ক set,
			     u64 now, bool wake_clock)
{
	সংরচনা psi_group_cpu *groupc;
	unsigned পূর্ণাঙ্ক t, m;
	u32 state_mask;

	lockdep_assert_rq_held(cpu_rq(cpu));
	groupc = per_cpu_ptr(group->pcpu, cpu);

	/*
	 * Start with TSK_ONCPU, which doesn't have a corresponding
	 * task count - it's just a boolean flag directly encoded in
	 * the state mask. Clear, set, or carry the current state if
	 * no changes are requested.
	 */
	যদি (unlikely(clear & TSK_ONCPU)) {
		state_mask = 0;
		clear &= ~TSK_ONCPU;
	} অন্যথা যদি (unlikely(set & TSK_ONCPU)) {
		state_mask = PSI_ONCPU;
		set &= ~TSK_ONCPU;
	} অন্যথা {
		state_mask = groupc->state_mask & PSI_ONCPU;
	}

	/*
	 * The rest of the state mask is calculated based on the task
	 * counts. Update those first, then construct the mask.
	 */
	বাবে (t = 0, m = clear; m; m &= ~(1 << t), t++) {
		যদি (!(m & (1 << t)))
			অনুৱর্ত;
		যদি (groupc->tasks[t]) {
			groupc->tasks[t]--;
		} অন্যথা যদি (!psi_bug) {
			printk_deferred(KERN_ERR "psi: task underflow! cpu=%d t=%d tasks=[%u %u %u %u] clear=%x set=%x\n",
					cpu, t, groupc->tasks[0],
					groupc->tasks[1], groupc->tasks[2],
					groupc->tasks[3], clear, set);
			psi_bug = 1;
		}
	}

	বাবে (t = 0; set; set &= ~(1 << t), t++)
		যদি (set & (1 << t))
			groupc->tasks[t]++;

	যদি (!group->enabled) {
		/*
		 * On the first group change after disabling PSI, conclude
		 * the current state and flush its time. This is unlikely
		 * to matter to the user, but aggregation (get_recent_times)
		 * may have already incorporated the live state into times_prev;
		 * avoid a delta sample underflow when PSI is later re-enabled.
		 */
		যদি (unlikely(groupc->state_mask & (1 << PSI_NONIDLE)))
			record_times(groupc, now);

		groupc->state_mask = state_mask;

		ঘূৰাই;
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
	যদি (unlikely((state_mask & PSI_ONCPU) && cpu_curr(cpu)->in_memstall))
		state_mask |= (1 << PSI_MEM_FULL);

	record_times(groupc, now);

	groupc->state_mask = state_mask;

	যদি (state_mask & group->rtpoll_states)
		psi_schedule_rtpoll_work(group, 1, false);

	যদি (wake_clock && !delayed_work_pending(&group->avgs_work))
		schedule_delayed_work(&group->avgs_work, PSI_FREQ);
}

স্থির অন্তঃস্থ সংরচনা psi_group *task_psi_group(সংরচনা task_struct *task)
{
#ifdef CONFIG_CGROUPS
	যদি (static_branch_likely(&psi_cgroups_enabled))
		ঘূৰাই cgroup_psi(task_dfl_cgroup(task));
#endif
	ঘূৰাই &psi_system;
}

স্থির শূন্য psi_flags_change(সংরচনা task_struct *task, পূর্ণাঙ্ক clear, পূর্ণাঙ্ক set)
{
	যদি (((task->psi_flags & set) ||
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

শূন্য psi_task_change(সংরচনা task_struct *task, পূর্ণাঙ্ক clear, পূর্ণাঙ্ক set)
{
	পূর্ণাঙ্ক cpu = task_cpu(task);
	u64 now;

	যদি (!task->pid)
		ঘূৰাই;

	psi_flags_change(task, clear, set);

	psi_write_begin(cpu);
	now = cpu_clock(cpu);
	for_each_group(group, task_psi_group(task))
		psi_group_change(group, cpu, clear, set, now, true);
	psi_write_end(cpu);
}

শূন্য psi_task_switch(সংরচনা task_struct *prev, সংরচনা task_struct *next,
		     bool sleep)
{
	সংরচনা psi_group *common = রিক্ত;
	পূর্ণাঙ্ক cpu = task_cpu(prev);
	u64 now;

	psi_write_begin(cpu);
	now = cpu_clock(cpu);

	যদি (next->pid) {
		psi_flags_change(next, 0, TSK_ONCPU);
		/*
		 * Set TSK_ONCPU on @next's cgroups. If @next shares any
		 * ancestors with @prev, those will already have @prev's
		 * TSK_ONCPU bit set, and we can stop the iteration there.
		 */
		for_each_group(group, task_psi_group(next)) {
			সংরচনা psi_group_cpu *groupc = per_cpu_ptr(group->pcpu, cpu);

			যদি (groupc->state_mask & PSI_ONCPU) {
				common = group;
				ভঙ্গ;
			}
			psi_group_change(group, cpu, 0, TSK_ONCPU, now, true);
		}
	}

	যদি (prev->pid) {
		পূর্ণাঙ্ক clear = TSK_ONCPU, set = 0;
		bool wake_clock = true;

		/*
		 * When we're going to sleep, psi_dequeue() lets us
		 * handle TSK_RUNNING, TSK_MEMSTALL_RUNNING and
		 * TSK_IOWAIT here, where we can combine it with
		 * TSK_ONCPU and save walking common ancestors twice.
		 */
		যদি (sleep) {
			clear |= TSK_RUNNING;
			যদি (prev->in_memstall)
				clear |= TSK_MEMSTALL_RUNNING;
			যদি (prev->in_iowait)
				set |= TSK_IOWAIT;

			/*
			 * Periodic aggregation shuts off if there is a period of no
			 * task changes, so we wake it back up if necessary. However,
			 * don't do this if the task change is the aggregation worker
			 * itself going to sleep, or we'll ping-pong forever.
			 */
			যদি (unlikely((prev->flags & PF_WQ_WORKER) &&
				     wq_worker_last_func(prev) == psi_avgs_work))
				wake_clock = false;
		}

		psi_flags_change(prev, clear, set);

		for_each_group(group, task_psi_group(prev)) {
			যদি (group == common)
				ভঙ্গ;
			psi_group_change(group, cpu, clear, set, now, wake_clock);
		}

		/*
		 * TSK_ONCPU is handled up to the common ancestor. If there are
		 * any other differences between the two tasks (e.g. prev goes
		 * to sleep, or only one task is memstall), finish propagating
		 * those differences all the way up to the root.
		 */
		যদি ((prev->psi_flags ^ next->psi_flags) & ~TSK_ONCPU) {
			clear &= ~TSK_ONCPU;
			for_each_group(group, common)
				psi_group_change(group, cpu, clear, set, now, wake_clock);
		}
	}
	psi_write_end(cpu);
}

#ifdef CONFIG_IRQ_TIME_ACCOUNTING
শূন্য psi_account_irqtime(সংরচনা rq *rq, সংরচনা task_struct *curr, সংরচনা task_struct *prev)
{
	পূর্ণাঙ্ক cpu = task_cpu(curr);
	সংরচনা psi_group_cpu *groupc;
	s64 delta;
	u64 irq;
	u64 now;

	যদি (static_branch_likely(&psi_disabled) || !irqtime_enabled())
		ঘূৰাই;

	যদি (!curr->pid)
		ঘূৰাই;

	lockdep_assert_rq_held(rq);
	যদি (prev && task_psi_group(prev) == task_psi_group(curr))
		ঘূৰাই;

	irq = irq_time_read(cpu);
	delta = (s64)(irq - rq->psi_irq_time);
	যদি (delta <= 0)
		ঘূৰাই;
	rq->psi_irq_time = irq;

	psi_write_begin(cpu);
	now = cpu_clock(cpu);

	for_each_group(group, task_psi_group(curr)) {
		যদি (!group->enabled)
			অনুৱর্ত;

		groupc = per_cpu_ptr(group->pcpu, cpu);

		record_times(groupc, now);
		groupc->times[PSI_IRQ_FULL] += delta;

		যদি (group->rtpoll_states & (1 << PSI_IRQ_FULL))
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
শূন্য psi_memstall_enter(unsigned long *flags)
{
	সংরচনা rq_flags rf;
	সংরচনা rq *rq;

	যদি (static_branch_likely(&psi_disabled))
		ঘূৰাই;

	*flags = current->in_memstall;
	যদি (*flags)
		ঘূৰাই;
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
EXPORT_প্ৰতীক_GPL(psi_memstall_enter);

/**
 * psi_memstall_leave - mark the end of an memory stall section
 * @flags: flags to handle nested memdelay sections
 *
 * Marks the calling task as no longer stalled due to lack of memory.
 */
শূন্য psi_memstall_leave(unsigned long *flags)
{
	সংরচনা rq_flags rf;
	সংরচনা rq *rq;

	যদি (static_branch_likely(&psi_disabled))
		ঘূৰাই;

	যদি (*flags)
		ঘূৰাই;
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
EXPORT_প্ৰতীক_GPL(psi_memstall_leave);

#ifdef CONFIG_CGROUPS
পূর্ণাঙ্ক psi_cgroup_alloc(সংরচনা cgroup *cgroup)
{
	যদি (!static_branch_likely(&psi_cgroups_enabled))
		ঘূৰাই 0;

	cgroup->psi = kzalloc_obj(সংরচনা psi_group);
	যদি (!cgroup->psi)
		ঘূৰাই -ENOMEM;

	cgroup->psi->pcpu = alloc_percpu(সংরচনা psi_group_cpu);
	যদি (!cgroup->psi->pcpu) {
		kfree(cgroup->psi);
		ঘূৰাই -ENOMEM;
	}
	group_init(cgroup->psi);
	cgroup->psi->parent = cgroup_psi(cgroup_parent(cgroup));
	ঘূৰাই 0;
}

শূন্য psi_cgroup_free(সংরচনা cgroup *cgroup)
{
	যদি (!static_branch_likely(&psi_cgroups_enabled))
		ঘূৰাই;

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
শূন্য cgroup_move_task(সংরচনা task_struct *task, সংরচনা css_set *to)
{
	unsigned পূর্ণাঙ্ক task_flags;
	সংরচনা rq_flags rf;
	সংরচনা rq *rq;

	যদি (!static_branch_likely(&psi_cgroups_enabled)) {
		/*
		 * Lame to do this here, but the scheduler cannot be locked
		 * from the outside, so we move cgroups from inside sched/.
		 */
		rcu_assign_pointer(task->cgroups, to);
		ঘূৰাই;
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

	যদি (task_flags)
		psi_task_change(task, task_flags, 0);

	/* See comment above */
	rcu_assign_pointer(task->cgroups, to);

	যদি (task_flags)
		psi_task_change(task, 0, task_flags);

	task_rq_unlock(rq, task, &rf);
}

শূন্য psi_cgroup_restart(সংরচনা psi_group *group)
{
	পূর্ণাঙ্ক cpu;

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
	যদি (!group->enabled)
		ঘূৰাই;

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

পূর্ণাঙ্ক psi_show(সংরচনা seq_file *m, সংরচনা psi_group *group, গণনা psi_res res)
{
	bool only_full = false;
	পূর্ণাঙ্ক full;
	u64 now;

	যদি (static_branch_likely(&psi_disabled))
		ঘূৰাই -EOPNOTSUPP;

#ifdef CONFIG_IRQ_TIME_ACCOUNTING
	যদি (!irqtime_enabled() && res == PSI_IRQ)
		ঘূৰাই -EOPNOTSUPP;
#endif

	/* Update averages before reporting them */
	mutex_lock(&group->avgs_lock);
	now = sched_clock();
	collect_percpu_times(group, PSI_AVGS, রিক্ত);
	যদি (now >= group->avg_next_update)
		group->avg_next_update = update_averages(group, now);
	mutex_unlock(&group->avgs_lock);

#ifdef CONFIG_IRQ_TIME_ACCOUNTING
	only_full = res == PSI_IRQ;
#endif

	বাবে (full = 0; full < 2 - only_full; full++) {
		unsigned long avg[3] = { 0, };
		u64 total = 0;
		পূর্ণাঙ্ক w;

		/* CPU FULL is undefined at the system level */
		যদি (!(group == &psi_system && res == PSI_CPU && full)) {
			বাবে (w = 0; w < 3; w++)
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

	ঘূৰাই 0;
}

/*
 * Create @group's rtpoll worker after psi_trigger_create() reported the need
 * for one. kthread creation depends on the whole fork path and we don't want
 * all of that nested inside cgroup_mutex, so the caller must drop it and any
 * other lock that forks can wait behind. If two callers race, the loser stops
 * its never-woken kthread.
 */
পূর্ণাঙ্ক psi_trigger_create_rtpoll_worker(সংরচনা psi_group *group)
{
	সংরচনা task_struct *task;

	task = kthread_create(psi_rtpoll_worker, group, "psimon");
	যদি (IS_ERR(task))
		ঘূৰাই PTR_ERR(task);

	scoped_guard(mutex, &group->rtpoll_trigger_lock) {
		যদি (!rcu_access_pointer(group->rtpoll_task)) {
			atomic_set(&group->rtpoll_wakeup, 0);
			wake_up_process(task);
			rcu_assign_pointer(group->rtpoll_task, task);

			/*
			 * Poll once to catch up on scheduling attempts dropped
			 * while there was no rtpoll worker.
			 */
			psi_schedule_rtpoll_work(group, 1, true);
			ঘূৰাই 0;
		}
	}

	kthread_stop(task);
	ঘূৰাই 0;
}

সংরচনা psi_trigger *psi_trigger_create(সংরচনা psi_group *group, ৱর্ণ *buf,
				       গণনা psi_res res, সংরচনা file *file,
				       সংরচনা kernfs_open_file *of,
				       bool *need_rtpoll_worker)
{
	সংরচনা psi_trigger *t;
	গণনা psi_states state;
	u32 threshold_us;
	bool privileged;
	u32 window_us;

	*need_rtpoll_worker = false;

	যদি (static_branch_likely(&psi_disabled))
		ঘূৰাই ERR_PTR(-EOPNOTSUPP);

	/*
	 * Checking the privilege here on file->f_cred implies that a privileged user
	 * could open the file and delegate the write to an unprivileged one.
	 */
	privileged = cap_raised(file->f_cred->cap_effective, CAP_SYS_RESOURCE);

	যদি (sscanf(buf, "some %u %u", &threshold_us, &window_us) == 2)
		state = PSI_IO_SOME + res * 2;
	অন্যথা যদি (sscanf(buf, "full %u %u", &threshold_us, &window_us) == 2)
		state = PSI_IO_FULL + res * 2;
	অন্যথা
		ঘূৰাই ERR_PTR(-EINVAL);

#ifdef CONFIG_IRQ_TIME_ACCOUNTING
	যদি (res == PSI_IRQ && --state != PSI_IRQ_FULL)
		ঘূৰাই ERR_PTR(-EINVAL);
#endif

	যদি (state >= PSI_NONIDLE)
		ঘূৰাই ERR_PTR(-EINVAL);

	যদি (window_us == 0 || window_us > WINDOW_MAX_US)
		ঘূৰাই ERR_PTR(-EINVAL);

	/*
	 * Unprivileged users can only use 2s windows so that averages aggregation
	 * work is used, and no RT threads need to be spawned.
	 */
	যদি (!privileged && window_us % 2000000)
		ঘূৰাই ERR_PTR(-EINVAL);

	/* Check threshold */
	যদি (threshold_us == 0 || threshold_us > window_us)
		ঘূৰাই ERR_PTR(-EINVAL);

	t = kmalloc_obj(*t);
	যদি (!t)
		ঘূৰাই ERR_PTR(-ENOMEM);

	t->group = group;
	t->state = state;
	t->threshold = threshold_us * NSEC_PER_USEC;
	t->win.size = window_us * NSEC_PER_USEC;
	window_reset(&t->win, sched_clock(),
			group->total[PSI_POLL][t->state], 0);

	t->event = 0;
	t->last_event_time = 0;
	t->of = of;
	যদি (!of)
		init_waitqueue_head(&t->event_wait);
	t->pending_event = false;
	t->aggregator = privileged ? PSI_POLL : PSI_AVGS;

	যদি (privileged) {
		mutex_lock(&group->rtpoll_trigger_lock);

		list_add(&t->গ্ৰন্থি, &group->rtpoll_triggers);
		group->rtpoll_min_period = min(group->rtpoll_min_period,
			div_u64(t->win.size, UPDATES_PER_WINDOW));
		group->rtpoll_nr_triggers[t->state]++;
		group->rtpoll_states |= (1 << t->state);

		*need_rtpoll_worker = !rcu_access_pointer(group->rtpoll_task);

		mutex_unlock(&group->rtpoll_trigger_lock);
	} অন্যথা {
		mutex_lock(&group->avgs_lock);

		list_add(&t->গ্ৰন্থি, &group->avg_triggers);
		group->avg_nr_triggers[t->state]++;

		mutex_unlock(&group->avgs_lock);
	}
	ঘূৰাই t;
}

শূন্য psi_trigger_destroy(সংরচনা psi_trigger *t)
{
	সংরচনা psi_group *group;
	সংরচনা task_struct *task_to_destroy = রিক্ত;

	/*
	 * We do not check psi_disabled since it might have been disabled after
	 * the trigger got created.
	 */
	যদি (!t)
		ঘূৰাই;

	group = t->group;
	/*
	 * Wakeup waiters to stop polling and clear the queue to prevent it from
	 * being accessed later. Can happen if cgroup is deleted from under a
	 * polling process.
	 */
	যদি (t->of)
		kernfs_notify(t->of->kn);
	অন্যথা
		wake_up_interruptible(&t->event_wait);

	যদি (t->aggregator == PSI_AVGS) {
		mutex_lock(&group->avgs_lock);
		যদি (!list_empty(&t->গ্ৰন্থি)) {
			list_del(&t->গ্ৰন্থি);
			group->avg_nr_triggers[t->state]--;
		}
		mutex_unlock(&group->avgs_lock);
	} অন্যথা {
		mutex_lock(&group->rtpoll_trigger_lock);
		যদি (!list_empty(&t->গ্ৰন্থি)) {
			সংরচনা psi_trigger *tmp;
			u64 period = ULLONG_MAX;

			list_del(&t->গ্ৰন্থি);
			group->rtpoll_nr_triggers[t->state]--;
			যদি (!group->rtpoll_nr_triggers[t->state])
				group->rtpoll_states &= ~(1 << t->state);
			/*
			 * Reset min update period for the remaining triggers
			 * iff the destroying trigger had the min window size.
			 */
			যদি (group->rtpoll_min_period == div_u64(t->win.size, UPDATES_PER_WINDOW)) {
				list_for_each_entry(tmp, &group->rtpoll_triggers, গ্ৰন্থি)
					period = min(period, div_u64(tmp->win.size,
							UPDATES_PER_WINDOW));
				group->rtpoll_min_period = period;
			}
			/* Destroy rtpoll_task when the last trigger is destroyed */
			যদি (group->rtpoll_states == 0) {
				group->rtpoll_until = 0;
				task_to_destroy = rcu_dereference_protected(
						group->rtpoll_task,
						lockdep_is_held(&group->rtpoll_trigger_lock));
				rcu_assign_pointer(group->rtpoll_task, রিক্ত);
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
	যদি (task_to_destroy) {
		kthread_stop(task_to_destroy);
		atomic_set(&group->rtpoll_scheduled, 0);
	}
	kfree(t);
}

__poll_t psi_trigger_poll(শূন্য **trigger_ptr,
				সংরচনা file *file, poll_table *wait)
{
	__poll_t ret = DEFAULT_POLLMASK;
	সংরচনা psi_trigger *t;

	যদি (static_branch_likely(&psi_disabled))
		ঘূৰাই DEFAULT_POLLMASK | EPOLLERR | EPOLLPRI;

	t = smp_load_acquire(trigger_ptr);
	যদি (!t)
		ঘূৰাই DEFAULT_POLLMASK | EPOLLERR | EPOLLPRI;

	যদি (t->of)
		kernfs_generic_poll(t->of, wait);
	অন্যথা
		poll_wait(file, &t->event_wait, wait);

	যদি (cmpxchg(&t->event, 1, 0) == 1)
		ret |= EPOLLPRI;

	ঘূৰাই ret;
}

#ifdef CONFIG_PROC_FS
স্থির পূর্ণাঙ্ক psi_io_show(সংরচনা seq_file *m, শূন্য *v)
{
	ঘূৰাই psi_show(m, &psi_system, PSI_IO);
}

স্থির পূর্ণাঙ্ক psi_memory_show(সংরচনা seq_file *m, শূন্য *v)
{
	ঘূৰাই psi_show(m, &psi_system, PSI_MEM);
}

স্থির পূর্ণাঙ্ক psi_cpu_show(সংরচনা seq_file *m, শূন্য *v)
{
	ঘূৰাই psi_show(m, &psi_system, PSI_CPU);
}

স্থির পূর্ণাঙ্ক psi_io_open(সংরচনা inode *inode, সংরচনা file *file)
{
	ঘূৰাই single_open(file, psi_io_show, রিক্ত);
}

স্থির পূর্ণাঙ্ক psi_memory_open(সংরচনা inode *inode, সংরচনা file *file)
{
	ঘূৰাই single_open(file, psi_memory_show, রিক্ত);
}

স্থির পূর্ণাঙ্ক psi_cpu_open(সংরচনা inode *inode, সংরচনা file *file)
{
	ঘূৰাই single_open(file, psi_cpu_show, রিক্ত);
}

স্থির ssize_t psi_write(সংরচনা file *file, ধ্রুৱ ৱর্ণ __user *user_buf,
			 size_t nbytes, গণনা psi_res res)
{
	ৱর্ণ buf[32];
	size_t buf_size;
	সংরচনা seq_file *seq;
	সংরচনা psi_trigger *new;
	bool need_rtpoll_worker;
	পূর্ণাঙ্ক ret;

	যদি (static_branch_likely(&psi_disabled))
		ঘূৰাই -EOPNOTSUPP;

	যদি (!nbytes)
		ঘূৰাই -EINVAL;

	buf_size = min(nbytes, পরিমাণ(buf));
	যদি (copy_from_user(buf, user_buf, buf_size))
		ঘূৰাই -EFAULT;

	buf[buf_size - 1] = '\0';

	seq = file->private_data;

	/* Take seq->lock to protect seq->private from concurrent writes */
	mutex_lock(&seq->lock);

	/* Allow only one trigger per file descriptor */
	যদি (seq->গুপ্ত) {
		mutex_unlock(&seq->lock);
		ঘূৰাই -EBUSY;
	}

	new = psi_trigger_create(&psi_system, buf, res, file, রিক্ত,
				 &need_rtpoll_worker);
	যদি (IS_ERR(new)) {
		mutex_unlock(&seq->lock);
		ঘূৰাই PTR_ERR(new);
	}

	যদি (need_rtpoll_worker) {
		ret = psi_trigger_create_rtpoll_worker(&psi_system);
		যদি (ret) {
			psi_trigger_destroy(new);
			mutex_unlock(&seq->lock);
			ঘূৰাই ret;
		}
	}

	smp_store_release(&seq->গুপ্ত, new);
	mutex_unlock(&seq->lock);

	ঘূৰাই nbytes;
}

স্থির ssize_t psi_io_write(সংরচনা file *file, ধ্রুৱ ৱর্ণ __user *user_buf,
			    size_t nbytes, loff_t *ppos)
{
	ঘূৰাই psi_write(file, user_buf, nbytes, PSI_IO);
}

স্থির ssize_t psi_memory_write(সংরচনা file *file, ধ্রুৱ ৱর্ণ __user *user_buf,
				size_t nbytes, loff_t *ppos)
{
	ঘূৰাই psi_write(file, user_buf, nbytes, PSI_MEM);
}

স্থির ssize_t psi_cpu_write(সংরচনা file *file, ধ্রুৱ ৱর্ণ __user *user_buf,
			     size_t nbytes, loff_t *ppos)
{
	ঘূৰাই psi_write(file, user_buf, nbytes, PSI_CPU);
}

স্থির __poll_t psi_fop_poll(সংরচনা file *file, poll_table *wait)
{
	সংরচনা seq_file *seq = file->private_data;

	ঘূৰাই psi_trigger_poll(&seq->গুপ্ত, file, wait);
}

স্থির পূর্ণাঙ্ক psi_fop_release(সংরচনা inode *inode, সংরচনা file *file)
{
	সংরচনা seq_file *seq = file->private_data;

	psi_trigger_destroy(seq->গুপ্ত);
	ঘূৰাই single_release(inode, file);
}

স্থির ধ্রুৱ সংরচনা proc_ops psi_io_proc_ops = {
	.proc_open	= psi_io_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_io_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

স্থির ধ্রুৱ সংরচনা proc_ops psi_memory_proc_ops = {
	.proc_open	= psi_memory_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_memory_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

স্থির ধ্রুৱ সংরচনা proc_ops psi_cpu_proc_ops = {
	.proc_open	= psi_cpu_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_cpu_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

#ifdef CONFIG_IRQ_TIME_ACCOUNTING
স্থির পূর্ণাঙ্ক psi_irq_show(সংরচনা seq_file *m, শূন্য *v)
{
	ঘূৰাই psi_show(m, &psi_system, PSI_IRQ);
}

স্থির পূর্ণাঙ্ক psi_irq_open(সংরচনা inode *inode, সংরচনা file *file)
{
	ঘূৰাই single_open(file, psi_irq_show, রিক্ত);
}

স্থির ssize_t psi_irq_write(সংরচনা file *file, ধ্রুৱ ৱর্ণ __user *user_buf,
			     size_t nbytes, loff_t *ppos)
{
	ঘূৰাই psi_write(file, user_buf, nbytes, PSI_IRQ);
}

স্থির ধ্রুৱ সংরচনা proc_ops psi_irq_proc_ops = {
	.proc_open	= psi_irq_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_irq_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};
#endif /* CONFIG_IRQ_TIME_ACCOUNTING */

স্থির পূর্ণাঙ্ক __init psi_proc_init(শূন্য)
{
	যদি (psi_enable) {
		proc_mkdir("pressure", রিক্ত);
		proc_create("pressure/io", 0666, রিক্ত, &psi_io_proc_ops);
		proc_create("pressure/memory", 0666, রিক্ত, &psi_memory_proc_ops);
		proc_create("pressure/cpu", 0666, রিক্ত, &psi_cpu_proc_ops);
#ifdef CONFIG_IRQ_TIME_ACCOUNTING
		proc_create("pressure/irq", 0666, রিক্ত, &psi_irq_proc_ops);
#endif
	}
	ঘূৰাই 0;
}
module_init(psi_proc_init);

#endif /* CONFIG_PROC_FS */
