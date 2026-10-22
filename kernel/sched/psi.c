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
#అన్తర్భావ <linux/sched/clock.h>
#అన్తర్భావ <linux/workqueue.h>
#అన్తర్భావ <linux/psi.h>
#అన్తర్భావ "sched.h"

స్థిర పూర్ణాంకం psi_bug __read_mostly;

DEFINE_STATIC_కీలకం_FALSE(psi_disabled);
స్థిర DEFINE_STATIC_కీలకం_TRUE(psi_cgroups_enabled);

#నిర్వచితమైతే CONFIG_PSI_DEFAULT_DISABLED
స్థిర bool psi_enable;
#లేకపోతే
స్థిర bool psi_enable = true;
#అయితే_ముగింపు
స్థిర పూర్ణాంకం __init అమరిక_psi(అక్షరం *str)
{
	తిరిగి kstrtobool(str, &psi_enable) == 0;
}
__setup("psi=", అమరిక_psi);

/* Running averages - we need to be higher-res than loadavg */
#నిర్వచించు PSI_FREQ	(2*HZ+1)	/* 2 sec intervals */
#నిర్వచించు EXP_10s		1677		/* 1/exp(2s/10s) as fixed-point */
#నిర్వచించు EXP_60s		1981		/* 1/exp(2s/60s) */
#నిర్వచించు EXP_300s	2034		/* 1/exp(2s/300s) */

/* PSI trigger definitions */
#నిర్వచించు కిటికీ_MAX_US 10000000	/* Max window size is 10s */
#నిర్వచించు UPDATES_చొప్పున_కిటికీ 10	/* 10 updates per window */

/* Sampling frequency in nanoseconds */
స్థిర u64 psi_period __read_mostly;

/* System-level pressure and stall tracking */
స్థిర DEFINE_చొప్పున_CPU(నిర్మాణం psi_సమూహం_cpu, system_సమూహం_pcpu);
నిర్మాణం psi_సమూహం psi_system = {
	.pcpu = &system_సమూహం_pcpu,
};

స్థిర DEFINE_చొప్పున_CPU(seqcount_t, psi_seq) = SEQCNT_ZERO(psi_seq);

స్థిర అంతఃవరుస శూన్య psi_write_begin(పూర్ణాంకం cpu)
{
	write_seqcount_begin(చొప్పున_cpu_ptr(&psi_seq, cpu));
}

స్థిర అంతఃవరుస శూన్య psi_write_చివర(పూర్ణాంకం cpu)
{
	write_seqcount_చివర(చొప్పున_cpu_ptr(&psi_seq, cpu));
}

స్థిర అంతఃవరుస u32 psi_read_begin(పూర్ణాంకం cpu)
{
	తిరిగి read_seqcount_begin(చొప్పున_cpu_ptr(&psi_seq, cpu));
}

స్థిర అంతఃవరుస bool psi_read_retry(పూర్ణాంకం cpu, u32 seq)
{
	తిరిగి read_seqcount_retry(చొప్పున_cpu_ptr(&psi_seq, cpu), seq);
}

స్థిర శూన్య psi_avgs_work(నిర్మాణం work_struct *work);

స్థిర శూన్య poll_timer_fn(నిర్మాణం timer_list *t);

స్థిర శూన్య సమూహం_init(నిర్మాణం psi_సమూహం *సమూహం)
{
	సమూహం->enabled = true;
	సమూహం->avg_చివరి_update = sched_clock();
	సమూహం->avg_తదుపరి_update = సమూహం->avg_చివరి_update + psi_period;
	mutex_init(&సమూహం->avgs_తాళం);

	/* Init avg trigger-related members */
	INIT_LIST_తల(&సమూహం->avg_triggers);
	memset(సమూహం->avg_nr_triggers, 0, పరిమాణం(సమూహం->avg_nr_triggers));
	INIT_DELAYED_WORK(&సమూహం->avgs_work, psi_avgs_work);

	/* Init rtpoll trigger-related members */
	atomic_సమితి(&సమూహం->rtpoll_scheduled, 0);
	mutex_init(&సమూహం->rtpoll_trigger_తాళం);
	INIT_LIST_తల(&సమూహం->rtpoll_triggers);
	సమూహం->rtpoll_min_period = U32_MAX;
	సమూహం->rtpoll_తదుపరి_update = ULLONG_MAX;
	init_waitqueue_తల(&సమూహం->rtpoll_wait);
	timer_అమరిక(&సమూహం->rtpoll_timer, poll_timer_fn, 0);
	rcu_కేటాయించు_సూచిక(సమూహం->rtpoll_task, ఖాళీ);
}

శూన్య __init psi_init(శూన్య)
{
	ఒకవేళ (!psi_enable) {
		static_branch_enable(&psi_disabled);
		static_branch_disable(&psi_cgroups_enabled);
		తిరిగి;
	}

	ఒకవేళ (!cgroup_psi_enabled())
		static_branch_disable(&psi_cgroups_enabled);

	psi_period = jiffies_గమ్యం_nsecs(PSI_FREQ);
	సమూహం_init(&psi_system);
}

స్థిర u32 test_స్థితులు(గుర్తులేని పూర్ణాంకం *tasks, u32 state_ముసుగు)
{
	స్థిరరాశి bool oncpu = state_ముసుగు & PSI_ONCPU;

	ఒకవేళ (tasks[NR_IOWAIT]) {
		state_ముసుగు |= బిట్టు(PSI_IO_SOME);
		ఒకవేళ (!tasks[NR_RUNNING])
			state_ముసుగు |= బిట్టు(PSI_IO_నిండిన);
	}

	ఒకవేళ (tasks[NR_MEMSTALL]) {
		state_ముసుగు |= బిట్టు(PSI_MEM_SOME);
		ఒకవేళ (tasks[NR_RUNNING] == tasks[NR_MEMSTALL_RUNNING])
			state_ముసుగు |= బిట్టు(PSI_MEM_నిండిన);
	}

	ఒకవేళ (tasks[NR_RUNNING] > oncpu)
		state_ముసుగు |= బిట్టు(PSI_CPU_SOME);

	ఒకవేళ (tasks[NR_RUNNING] && !oncpu)
		state_ముసుగు |= బిట్టు(PSI_CPU_నిండిన);

	ఒకవేళ (tasks[NR_IOWAIT] || tasks[NR_MEMSTALL] || tasks[NR_RUNNING])
		state_ముసుగు |= బిట్టు(PSI_NONIDLE);

	తిరిగి state_ముసుగు;
}

స్థిర శూన్య get_recent_times(నిర్మాణం psi_సమూహం *సమూహం, పూర్ణాంకం cpu,
			     గణన psi_aggregators aggregator, u32 *times,
			     u32 *pchanged_స్థితులు)
{
	నిర్మాణం psi_సమూహం_cpu *groupc = చొప్పున_cpu_ptr(సమూహం->pcpu, cpu);
	పూర్ణాంకం current_cpu = raw_smp_processor_id();
	గుర్తులేని పూర్ణాంకం tasks[NR_PSI_TASK_COUNTS];
	u64 now, state_ప్రారంభం;
	గణన psi_స్థితులు s;
	గుర్తులేని పూర్ణాంకం seq;
	u32 state_ముసుగు;

	*pchanged_స్థితులు = 0;

	/* Snapshot a coherent view of the CPU state */
	చేయి {
		seq = psi_read_begin(cpu);
		now = cpu_clock(cpu);
		జ్ఞాపకం_నకలు(times, groupc->times, పరిమాణం(groupc->times));
		state_ముసుగు = groupc->state_ముసుగు;
		state_ప్రారంభం = groupc->state_ప్రారంభం;
		ఒకవేళ (cpu == current_cpu)
			జ్ఞాపకం_నకలు(tasks, groupc->tasks, పరిమాణం(groupc->tasks));
	} అయితే (psi_read_retry(cpu, seq));

	/* Calculate state time deltas against the previous snapshot */
	కొరకు (s = 0; s < NR_PSI_స్థితులు; s++) {
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
		ఒకవేళ (state_ముసుగు & (1 << s))
			times[s] += now - state_ప్రారంభం;

		delta = times[s] - groupc->times_prev[aggregator][s];
		groupc->times_prev[aggregator][s] = times[s];

		times[s] = delta;
		ఒకవేళ (delta)
			*pchanged_స్థితులు |= (1 << s);
	}

	/*
	 * When collect_percpu_times() from the avgs_work, we don't want to
	 * re-arm avgs_work when all CPUs are IDLE. But the current CPU running
	 * this avgs_work is never IDLE, cause avgs_work can't be shut off.
	 * So for the current CPU, we need to re-arm avgs_work only when
	 * (NR_RUNNING > 1 || NR_IOWAIT > 0 || NR_MEMSTALL > 0), for other CPUs
	 * we can just check PSI_NONIDLE delta.
	 */
	ఒకవేళ (current_work() == &సమూహం->avgs_work.work) {
		bool reschedule;

		ఒకవేళ (cpu == current_cpu)
			reschedule = tasks[NR_RUNNING] +
				     tasks[NR_IOWAIT] +
				     tasks[NR_MEMSTALL] > 1;
		లేకపోతే
			reschedule = *pchanged_స్థితులు & (1 << PSI_NONIDLE);

		ఒకవేళ (reschedule)
			*pchanged_స్థితులు |= PSI_STATE_RESCHEDULE;
	}
}

స్థిర శూన్య calc_avgs(గుర్తులేని దీర్ఘ avg[3], పూర్ణాంకం missed_periods,
		      u64 సమయం, u64 period)
{
	గుర్తులేని దీర్ఘ pct;

	/* Fill in zeroes for periods of no activity */
	ఒకవేళ (missed_periods) {
		avg[0] = calc_ఎక్కించు_n(avg[0], EXP_10s, 0, missed_periods);
		avg[1] = calc_ఎక్కించు_n(avg[1], EXP_60s, 0, missed_periods);
		avg[2] = calc_ఎక్కించు_n(avg[2], EXP_300s, 0, missed_periods);
	}

	/* Sample the most recent active period */
	pct = div_u64(సమయం * 100, period);
	pct *= FIXED_1;
	avg[0] = calc_ఎక్కించు(avg[0], EXP_10s, pct);
	avg[1] = calc_ఎక్కించు(avg[1], EXP_60s, pct);
	avg[2] = calc_ఎక్కించు(avg[2], EXP_300s, pct);
}

స్థిర శూన్య collect_percpu_times(నిర్మాణం psi_సమూహం *సమూహం,
				 గణన psi_aggregators aggregator,
				 u32 *pchanged_స్థితులు)
{
	u64 deltas[NR_PSI_స్థితులు - 1] = { 0, };
	గుర్తులేని దీర్ఘ nonidle_మొత్తం = 0;
	u32 మారింది_స్థితులు = 0;
	పూర్ణాంకం cpu;
	పూర్ణాంకం s;

	/*
	 * Collect the per-cpu time buckets and average them into a
	 * single time sample that is normalized to wall clock time.
	 *
	 * For averaging, each CPU is weighted by its non-idle time in
	 * the sampling period. This eliminates artifacts from uneven
	 * loading, or even entirely idle CPUs.
	 */
	for_each_possible_cpu(cpu) {
		u32 times[NR_PSI_స్థితులు];
		u32 nonidle;
		u32 cpu_మారింది_స్థితులు;

		get_recent_times(సమూహం, cpu, aggregator, times,
				&cpu_మారింది_స్థితులు);
		మారింది_స్థితులు |= cpu_మారింది_స్థితులు;

		nonidle = nsecs_గమ్యం_jiffies(times[PSI_NONIDLE]);
		nonidle_మొత్తం += nonidle;

		కొరకు (s = 0; s < PSI_NONIDLE; s++)
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
	కొరకు (s = 0; s < NR_PSI_స్థితులు - 1; s++)
		సమూహం->మొత్తం[aggregator][s] +=
				div_u64(deltas[s], max(nonidle_మొత్తం, 1UL));

	ఒకవేళ (pchanged_స్థితులు)
		*pchanged_స్థితులు = మారింది_స్థితులు;
}

/* Trigger tracking window manipulations */
స్థిర శూన్య కిటికీ_reset(నిర్మాణం psi_కిటికీ *win, u64 now, u64 విలువ,
			 u64 prev_growth)
{
	win->ప్రారంభం_సమయం = now;
	win->ప్రారంభం_విలువ = విలువ;
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
స్థిర u64 కిటికీ_update(నిర్మాణం psi_కిటికీ *win, u64 now, u64 విలువ)
{
	u64 elapsed;
	u64 growth;

	elapsed = now - win->ప్రారంభం_సమయం;
	growth = విలువ - win->ప్రారంభం_విలువ;
	/*
	 * After each tracking window passes win->start_value and
	 * win->start_time get reset and win->prev_growth stores
	 * the average per-window growth of the previous window.
	 * win->prev_growth is then used to interpolate additional
	 * growth from the previous window assuming it was linear.
	 */
	ఒకవేళ (elapsed > win->కొలత)
		కిటికీ_reset(win, now, విలువ, growth);
	లేకపోతే {
		u32 మిగిలిన;

		మిగిలిన = win->కొలత - elapsed;
		growth += div64_u64(win->prev_growth * మిగిలిన, win->కొలత);
	}

	తిరిగి growth;
}

స్థిర శూన్య update_triggers(నిర్మాణం psi_సమూహం *సమూహం, u64 now,
						   గణన psi_aggregators aggregator)
{
	నిర్మాణం psi_trigger *t;
	u64 *మొత్తం = సమూహం->మొత్తం[aggregator];
	నిర్మాణం list_తల *triggers;
	u64 *aggregator_మొత్తం;

	ఒకవేళ (aggregator == PSI_AVGS) {
		triggers = &సమూహం->avg_triggers;
		aggregator_మొత్తం = సమూహం->avg_మొత్తం;
	} లేకపోతే {
		triggers = &సమూహం->rtpoll_triggers;
		aggregator_మొత్తం = సమూహం->rtpoll_మొత్తం;
	}

	/*
	 * On subsequent updates, calculate growth deltas and let
	 * watchers know when their specified thresholds are exceeded.
	 */
	list_for_each_నమోదు(t, triggers, కణుపు) {
		u64 growth;
		bool new_stall;

		new_stall = aggregator_మొత్తం[t->state] != మొత్తం[t->state];

		/* Check for stall activity or a previous threshold breach */
		ఒకవేళ (!new_stall && !t->pending_event)
			కొనసాగు;
		/*
		 * Check for new stall activity, as well as deferred
		 * events that occurred in the last window after the
		 * trigger had already fired (we want to ratelimit
		 * events without dropping any).
		 */
		ఒకవేళ (new_stall) {
			/* Calculate growth since last update */
			growth = కిటికీ_update(&t->win, now, మొత్తం[t->state]);
			ఒకవేళ (!t->pending_event) {
				ఒకవేళ (growth < t->పరిమితి)
					కొనసాగు;

				t->pending_event = true;
			}
		}
		/* Limit event signaling to once per window */
		ఒకవేళ (now < t->చివరి_event_సమయం + t->win.కొలత)
			కొనసాగు;

		/* Generate an event */
		ఒకవేళ (cmpxchg(&t->event, 0, 1) == 0) {
			ఒకవేళ (t->యొక్క)
				kernfs_notify(t->యొక్క->kn);
			లేకపోతే
				wake_పైకి_interruptible(&t->event_wait);
		}
		t->చివరి_event_సమయం = now;
		/* Reset threshold breach flag once event got generated */
		t->pending_event = false;
	}
}

స్థిర u64 update_averages(నిర్మాణం psi_సమూహం *సమూహం, u64 now)
{
	గుర్తులేని దీర్ఘ missed_periods = 0;
	u64 expires, period;
	u64 avg_తదుపరి_update;
	పూర్ణాంకం s;

	/* avgX= */
	expires = సమూహం->avg_తదుపరి_update;
	ఒకవేళ (now - expires >= psi_period)
		missed_periods = div_u64(now - expires, psi_period);

	/*
	 * The periodic clock tick can get delayed for various
	 * reasons, especially on loaded systems. To avoid clock
	 * drift, we schedule the clock in fixed psi_period intervals.
	 * But the deltas we sample out of the per-cpu buckets above
	 * are based on the actual time elapsing between clock ticks.
	 */
	avg_తదుపరి_update = expires + ((1 + missed_periods) * psi_period);
	period = now - (సమూహం->avg_చివరి_update + (missed_periods * psi_period));
	సమూహం->avg_చివరి_update = now;

	కొరకు (s = 0; s < NR_PSI_స్థితులు - 1; s++) {
		u32 sample;

		sample = సమూహం->మొత్తం[PSI_AVGS][s] - సమూహం->avg_మొత్తం[s];
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
		ఒకవేళ (sample > period)
			sample = period;
		సమూహం->avg_మొత్తం[s] += sample;
		calc_avgs(సమూహం->avg[s], missed_periods, sample, period);
	}

	తిరిగి avg_తదుపరి_update;
}

స్థిర శూన్య psi_avgs_work(నిర్మాణం work_struct *work)
{
	నిర్మాణం delayed_work *dwork;
	నిర్మాణం psi_సమూహం *సమూహం;
	u32 మారింది_స్థితులు;
	u64 now;

	dwork = గమ్యం_delayed_work(work);
	సమూహం = container_యొక్క(dwork, నిర్మాణం psi_సమూహం, avgs_work);

	mutex_తాళం(&సమూహం->avgs_తాళం);

	now = sched_clock();

	collect_percpu_times(సమూహం, PSI_AVGS, &మారింది_స్థితులు);
	/*
	 * If there is task activity, periodically fold the per-cpu
	 * times and feed samples into the running averages. If things
	 * are idle and there is no data to process, stop the clock.
	 * Once restarted, we'll catch up the running averages in one
	 * go - see calc_avgs() and missed_periods.
	 */
	ఒకవేళ (now >= సమూహం->avg_తదుపరి_update) {
		update_triggers(సమూహం, now, PSI_AVGS);
		సమూహం->avg_తదుపరి_update = update_averages(సమూహం, now);
	}

	ఒకవేళ (మారింది_స్థితులు & PSI_STATE_RESCHEDULE) {
		కాలపట్టిక_delayed_work(dwork, nsecs_గమ్యం_jiffies(
				సమూహం->avg_తదుపరి_update - now) + 1);
	}

	mutex_unlock(&సమూహం->avgs_తాళం);
}

స్థిర శూన్య init_rtpoll_triggers(నిర్మాణం psi_సమూహం *సమూహం, u64 now)
{
	నిర్మాణం psi_trigger *t;

	list_for_each_నమోదు(t, &సమూహం->rtpoll_triggers, కణుపు)
		కిటికీ_reset(&t->win, now,
				సమూహం->మొత్తం[PSI_POLL][t->state], 0);
	జ్ఞాపకం_నకలు(సమూహం->rtpoll_మొత్తం, సమూహం->మొత్తం[PSI_POLL],
		   పరిమాణం(సమూహం->rtpoll_మొత్తం));
	సమూహం->rtpoll_తదుపరి_update = now + సమూహం->rtpoll_min_period;
}

/* Schedule rtpolling if it's not already scheduled or forced. */
స్థిర శూన్య psi_కాలపట్టిక_rtpoll_work(నిర్మాణం psi_సమూహం *సమూహం, గుర్తులేని దీర్ఘ delay,
				   bool force)
{
	/*
	 * atomic_xchg should be called even when !force to provide a
	 * full memory barrier (see the comment inside psi_rtpoll_work).
	 */
	ఒకవేళ (atomic_xchg(&సమూహం->rtpoll_scheduled, 1) && !force)
		తిరిగి;

	/*
	 * Only test whether a worker is installed; do not dereference the task.
	 * A racing trigger destruction may leave the timer armed.
	 * psi_cgroup_free() shuts it down before freeing the group.
	 * The system PSI group is static.
	 */
	ఒకవేళ (likely(rcu_access_సూచిక(సమూహం->rtpoll_task)))
		mod_timer(&సమూహం->rtpoll_timer, jiffies + delay);
	లేకపోతే
		atomic_సమితి(&సమూహం->rtpoll_scheduled, 0);
}

స్థిర శూన్య psi_rtpoll_work(నిర్మాణం psi_సమూహం *సమూహం)
{
	bool force_reschedule = false;
	u32 మారింది_స్థితులు;
	u64 now;

	mutex_తాళం(&సమూహం->rtpoll_trigger_తాళం);

	now = sched_clock();

	ఒకవేళ (now > సమూహం->rtpoll_until) {
		/*
		 * We are either about to start or might stop rtpolling if no
		 * state change was recorded. Resetting rtpoll_scheduled leaves
		 * a small window for psi_group_change to sneak in and schedule
		 * an immediate rtpoll_work before we get to rescheduling. One
		 * potential extra wakeup at the end of the rtpolling window
		 * should be negligible and rtpoll_next_update still keeps
		 * updates correctly on schedule.
		 */
		atomic_సమితి(&సమూహం->rtpoll_scheduled, 0);
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
	} లేకపోతే {
		/* The rtpolling window is not over, keep rescheduling */
		force_reschedule = true;
	}


	collect_percpu_times(సమూహం, PSI_POLL, &మారింది_స్థితులు);

	ఒకవేళ (మారింది_స్థితులు & సమూహం->rtpoll_స్థితులు) {
		/* Initialize trigger windows when entering rtpolling mode */
		ఒకవేళ (now > సమూహం->rtpoll_until)
			init_rtpoll_triggers(సమూహం, now);

		/*
		 * Keep the monitor active for at least the duration of the
		 * minimum tracking window as long as monitor states are
		 * changing.
		 */
		సమూహం->rtpoll_until = now +
			సమూహం->rtpoll_min_period * UPDATES_చొప్పున_కిటికీ;
	}

	ఒకవేళ (now > సమూహం->rtpoll_until) {
		సమూహం->rtpoll_తదుపరి_update = ULLONG_MAX;
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (now >= సమూహం->rtpoll_తదుపరి_update) {
		ఒకవేళ (మారింది_స్థితులు & సమూహం->rtpoll_స్థితులు) {
			update_triggers(సమూహం, now, PSI_POLL);
			జ్ఞాపకం_నకలు(సమూహం->rtpoll_మొత్తం, సమూహం->మొత్తం[PSI_POLL],
				   పరిమాణం(సమూహం->rtpoll_మొత్తం));
		}
		సమూహం->rtpoll_తదుపరి_update = now + సమూహం->rtpoll_min_period;
	}

	psi_కాలపట్టిక_rtpoll_work(సమూహం,
		nsecs_గమ్యం_jiffies(సమూహం->rtpoll_తదుపరి_update - now) + 1,
		force_reschedule);

బహిర్గతం:
	mutex_unlock(&సమూహం->rtpoll_trigger_తాళం);
}

స్థిర పూర్ణాంకం psi_rtpoll_కార్మికుడు(శూన్య *దత్తాంశం)
{
	నిర్మాణం psi_సమూహం *సమూహం = (నిర్మాణం psi_సమూహం *)దత్తాంశం;

	sched_సమితి_fifo_దిగువ(current);

	అయితే (true) {
		wait_event_interruptible(సమూహం->rtpoll_wait,
				atomic_cmpxchg(&సమూహం->rtpoll_wakeup, 1, 0) ||
				kthread_should_stop());
		ఒకవేళ (kthread_should_stop())
			ఆపు;

		psi_rtpoll_work(సమూహం);
	}
	తిరిగి 0;
}

స్థిర శూన్య poll_timer_fn(నిర్మాణం timer_list *t)
{
	నిర్మాణం psi_సమూహం *సమూహం = timer_container_యొక్క(సమూహం, t, rtpoll_timer);

	atomic_సమితి(&సమూహం->rtpoll_wakeup, 1);
	wake_పైకి_interruptible(&సమూహం->rtpoll_wait);
}

స్థిర శూన్య రికార్డు_times(నిర్మాణం psi_సమూహం_cpu *groupc, u64 now)
{
	u32 delta;

	delta = now - groupc->state_ప్రారంభం;
	groupc->state_ప్రారంభం = now;

	ఒకవేళ (groupc->state_ముసుగు & (1 << PSI_IO_SOME)) {
		groupc->times[PSI_IO_SOME] += delta;
		ఒకవేళ (groupc->state_ముసుగు & (1 << PSI_IO_నిండిన))
			groupc->times[PSI_IO_నిండిన] += delta;
	}

	ఒకవేళ (groupc->state_ముసుగు & (1 << PSI_MEM_SOME)) {
		groupc->times[PSI_MEM_SOME] += delta;
		ఒకవేళ (groupc->state_ముసుగు & (1 << PSI_MEM_నిండిన))
			groupc->times[PSI_MEM_నిండిన] += delta;
	}

	ఒకవేళ (groupc->state_ముసుగు & (1 << PSI_CPU_SOME)) {
		groupc->times[PSI_CPU_SOME] += delta;
		ఒకవేళ (groupc->state_ముసుగు & (1 << PSI_CPU_నిండిన))
			groupc->times[PSI_CPU_నిండిన] += delta;
	}

	ఒకవేళ (groupc->state_ముసుగు & (1 << PSI_NONIDLE))
		groupc->times[PSI_NONIDLE] += delta;
}

#నిర్వచించు for_each_సమూహం(iter, సమూహం) \
	కొరకు (typeof(సమూహం) iter = సమూహం; iter; iter = iter->మాతృ)

స్థిర శూన్య psi_సమూహం_change(నిర్మాణం psi_సమూహం *సమూహం, పూర్ణాంకం cpu,
			     గుర్తులేని పూర్ణాంకం clear, గుర్తులేని పూర్ణాంకం సమితి,
			     u64 now, bool wake_clock)
{
	నిర్మాణం psi_సమూహం_cpu *groupc;
	గుర్తులేని పూర్ణాంకం t, m;
	u32 state_ముసుగు;

	lockdep_assert_rq_held(cpu_rq(cpu));
	groupc = చొప్పున_cpu_ptr(సమూహం->pcpu, cpu);

	/*
	 * Start with TSK_ONCPU, which doesn't have a corresponding
	 * task count - it's just a boolean flag directly encoded in
	 * the state mask. Clear, set, or carry the current state if
	 * no changes are requested.
	 */
	ఒకవేళ (unlikely(clear & TSK_ONCPU)) {
		state_ముసుగు = 0;
		clear &= ~TSK_ONCPU;
	} లేకపోతే ఒకవేళ (unlikely(సమితి & TSK_ONCPU)) {
		state_ముసుగు = PSI_ONCPU;
		సమితి &= ~TSK_ONCPU;
	} లేకపోతే {
		state_ముసుగు = groupc->state_ముసుగు & PSI_ONCPU;
	}

	/*
	 * The rest of the state mask is calculated based on the task
	 * counts. Update those first, then construct the mask.
	 */
	కొరకు (t = 0, m = clear; m; m &= ~(1 << t), t++) {
		ఒకవేళ (!(m & (1 << t)))
			కొనసాగు;
		ఒకవేళ (groupc->tasks[t]) {
			groupc->tasks[t]--;
		} లేకపోతే ఒకవేళ (!psi_bug) {
			printk_deferred(KERN_ERR "psi: task underflow! cpu=%d t=%d tasks=[%u %u %u %u] clear=%x set=%x\n",
					cpu, t, groupc->tasks[0],
					groupc->tasks[1], groupc->tasks[2],
					groupc->tasks[3], clear, సమితి);
			psi_bug = 1;
		}
	}

	కొరకు (t = 0; సమితి; సమితి &= ~(1 << t), t++)
		ఒకవేళ (సమితి & (1 << t))
			groupc->tasks[t]++;

	ఒకవేళ (!సమూహం->enabled) {
		/*
		 * On the first group change after disabling PSI, conclude
		 * the current state and flush its time. This is unlikely
		 * to matter to the user, but aggregation (get_recent_times)
		 * may have already incorporated the live state into times_prev;
		 * avoid a delta sample underflow when PSI is later re-enabled.
		 */
		ఒకవేళ (unlikely(groupc->state_ముసుగు & (1 << PSI_NONIDLE)))
			రికార్డు_times(groupc, now);

		groupc->state_ముసుగు = state_ముసుగు;

		తిరిగి;
	}

	state_ముసుగు = test_స్థితులు(groupc->tasks, state_ముసుగు);

	/*
	 * Since we care about lost potential, a memstall is FULL
	 * when there are no other working tasks, but also when
	 * the CPU is actively reclaiming and nothing productive
	 * could run even if it were runnable. So when the current
	 * task in a cgroup is in_memstall, the corresponding groupc
	 * on that cpu is in PSI_MEM_FULL state.
	 */
	ఒకవేళ (unlikely((state_ముసుగు & PSI_ONCPU) && cpu_curr(cpu)->లోపల_memstall))
		state_ముసుగు |= (1 << PSI_MEM_నిండిన);

	రికార్డు_times(groupc, now);

	groupc->state_ముసుగు = state_ముసుగు;

	ఒకవేళ (state_ముసుగు & సమూహం->rtpoll_స్థితులు)
		psi_కాలపట్టిక_rtpoll_work(సమూహం, 1, false);

	ఒకవేళ (wake_clock && !delayed_work_pending(&సమూహం->avgs_work))
		కాలపట్టిక_delayed_work(&సమూహం->avgs_work, PSI_FREQ);
}

స్థిర అంతఃవరుస నిర్మాణం psi_సమూహం *task_psi_సమూహం(నిర్మాణం task_struct *task)
{
#నిర్వచితమైతే CONFIG_CGROUPS
	ఒకవేళ (static_branch_likely(&psi_cgroups_enabled))
		తిరిగి cgroup_psi(task_dfl_cgroup(task));
#అయితే_ముగింపు
	తిరిగి &psi_system;
}

స్థిర శూన్య psi_flags_change(నిర్మాణం task_struct *task, పూర్ణాంకం clear, పూర్ణాంకం సమితి)
{
	ఒకవేళ (((task->psi_flags & సమితి) ||
	     (task->psi_flags & clear) != clear) &&
	    !psi_bug) {
		printk_deferred(KERN_ERR "psi: inconsistent task state! task=%d:%s cpu=%d psi_flags=%x clear=%x set=%x\n",
				task->pid, task->comm, task_cpu(task),
				task->psi_flags, clear, సమితి);
		psi_bug = 1;
	}

	task->psi_flags &= ~clear;
	task->psi_flags |= సమితి;
}

శూన్య psi_task_change(నిర్మాణం task_struct *task, పూర్ణాంకం clear, పూర్ణాంకం సమితి)
{
	పూర్ణాంకం cpu = task_cpu(task);
	u64 now;

	ఒకవేళ (!task->pid)
		తిరిగి;

	psi_flags_change(task, clear, సమితి);

	psi_write_begin(cpu);
	now = cpu_clock(cpu);
	for_each_సమూహం(సమూహం, task_psi_సమూహం(task))
		psi_సమూహం_change(సమూహం, cpu, clear, సమితి, now, true);
	psi_write_చివర(cpu);
}

శూన్య psi_task_switch(నిర్మాణం task_struct *prev, నిర్మాణం task_struct *తదుపరి,
		     bool sleep)
{
	నిర్మాణం psi_సమూహం *common = ఖాళీ;
	పూర్ణాంకం cpu = task_cpu(prev);
	u64 now;

	psi_write_begin(cpu);
	now = cpu_clock(cpu);

	ఒకవేళ (తదుపరి->pid) {
		psi_flags_change(తదుపరి, 0, TSK_ONCPU);
		/*
		 * Set TSK_ONCPU on @next's cgroups. If @next shares any
		 * ancestors with @prev, those will already have @prev's
		 * TSK_ONCPU bit set, and we can stop the iteration there.
		 */
		for_each_సమూహం(సమూహం, task_psi_సమూహం(తదుపరి)) {
			నిర్మాణం psi_సమూహం_cpu *groupc = చొప్పున_cpu_ptr(సమూహం->pcpu, cpu);

			ఒకవేళ (groupc->state_ముసుగు & PSI_ONCPU) {
				common = సమూహం;
				ఆపు;
			}
			psi_సమూహం_change(సమూహం, cpu, 0, TSK_ONCPU, now, true);
		}
	}

	ఒకవేళ (prev->pid) {
		పూర్ణాంకం clear = TSK_ONCPU, సమితి = 0;
		bool wake_clock = true;

		/*
		 * When we're going to sleep, psi_dequeue() lets us
		 * handle TSK_RUNNING, TSK_MEMSTALL_RUNNING and
		 * TSK_IOWAIT here, where we can combine it with
		 * TSK_ONCPU and save walking common ancestors twice.
		 */
		ఒకవేళ (sleep) {
			clear |= TSK_RUNNING;
			ఒకవేళ (prev->లోపల_memstall)
				clear |= TSK_MEMSTALL_RUNNING;
			ఒకవేళ (prev->లోపల_iowait)
				సమితి |= TSK_IOWAIT;

			/*
			 * Periodic aggregation shuts off if there is a period of no
			 * task changes, so we wake it back up if necessary. However,
			 * don't do this if the task change is the aggregation worker
			 * itself going to sleep, or we'll ping-pong forever.
			 */
			ఒకవేళ (unlikely((prev->flags & PF_WQ_కార్మికుడు) &&
				     wq_కార్మికుడు_చివరి_func(prev) == psi_avgs_work))
				wake_clock = false;
		}

		psi_flags_change(prev, clear, సమితి);

		for_each_సమూహం(సమూహం, task_psi_సమూహం(prev)) {
			ఒకవేళ (సమూహం == common)
				ఆపు;
			psi_సమూహం_change(సమూహం, cpu, clear, సమితి, now, wake_clock);
		}

		/*
		 * TSK_ONCPU is handled up to the common ancestor. If there are
		 * any other differences between the two tasks (e.g. prev goes
		 * to sleep, or only one task is memstall), finish propagating
		 * those differences all the way up to the root.
		 */
		ఒకవేళ ((prev->psi_flags ^ తదుపరి->psi_flags) & ~TSK_ONCPU) {
			clear &= ~TSK_ONCPU;
			for_each_సమూహం(సమూహం, common)
				psi_సమూహం_change(సమూహం, cpu, clear, సమితి, now, wake_clock);
		}
	}
	psi_write_చివర(cpu);
}

#నిర్వచితమైతే CONFIG_IRQ_సమయం_ACCOUNTING
శూన్య psi_account_irqtime(నిర్మాణం rq *rq, నిర్మాణం task_struct *curr, నిర్మాణం task_struct *prev)
{
	పూర్ణాంకం cpu = task_cpu(curr);
	నిర్మాణం psi_సమూహం_cpu *groupc;
	s64 delta;
	u64 irq;
	u64 now;

	ఒకవేళ (static_branch_likely(&psi_disabled) || !irqtime_enabled())
		తిరిగి;

	ఒకవేళ (!curr->pid)
		తిరిగి;

	lockdep_assert_rq_held(rq);
	ఒకవేళ (prev && task_psi_సమూహం(prev) == task_psi_సమూహం(curr))
		తిరిగి;

	irq = irq_సమయం_read(cpu);
	delta = (s64)(irq - rq->psi_irq_సమయం);
	ఒకవేళ (delta <= 0)
		తిరిగి;
	rq->psi_irq_సమయం = irq;

	psi_write_begin(cpu);
	now = cpu_clock(cpu);

	for_each_సమూహం(సమూహం, task_psi_సమూహం(curr)) {
		ఒకవేళ (!సమూహం->enabled)
			కొనసాగు;

		groupc = చొప్పున_cpu_ptr(సమూహం->pcpu, cpu);

		రికార్డు_times(groupc, now);
		groupc->times[PSI_IRQ_నిండిన] += delta;

		ఒకవేళ (సమూహం->rtpoll_స్థితులు & (1 << PSI_IRQ_నిండిన))
			psi_కాలపట్టిక_rtpoll_work(సమూహం, 1, false);
	}
	psi_write_చివర(cpu);
}
#అయితే_ముగింపు /* CONFIG_IRQ_TIME_ACCOUNTING */

/**
 * psi_memstall_enter - mark the beginning of a memory stall section
 * @flags: flags to handle nested sections
 *
 * Marks the calling task as being stalled due to a lack of memory,
 * such as waiting for a refault or performing reclaim.
 */
శూన్య psi_memstall_enter(గుర్తులేని దీర్ఘ *flags)
{
	నిర్మాణం rq_flags rf;
	నిర్మాణం rq *rq;

	ఒకవేళ (static_branch_likely(&psi_disabled))
		తిరిగి;

	*flags = current->లోపల_memstall;
	ఒకవేళ (*flags)
		తిరిగి;
	/*
	 * in_memstall setting & accounting needs to be atomic wrt
	 * changes to the task's scheduling state, otherwise we can
	 * race with CPU migration.
	 */
	rq = this_rq_తాళం_irq(&rf);

	current->లోపల_memstall = 1;
	psi_task_change(current, 0, TSK_MEMSTALL | TSK_MEMSTALL_RUNNING);

	rq_unlock_irq(rq, &rf);
}
EXPORT_చిహ్నం_GPL(psi_memstall_enter);

/**
 * psi_memstall_leave - mark the end of an memory stall section
 * @flags: flags to handle nested memdelay sections
 *
 * Marks the calling task as no longer stalled due to lack of memory.
 */
శూన్య psi_memstall_leave(గుర్తులేని దీర్ఘ *flags)
{
	నిర్మాణం rq_flags rf;
	నిర్మాణం rq *rq;

	ఒకవేళ (static_branch_likely(&psi_disabled))
		తిరిగి;

	ఒకవేళ (*flags)
		తిరిగి;
	/*
	 * in_memstall clearing & accounting needs to be atomic wrt
	 * changes to the task's scheduling state, otherwise we could
	 * race with CPU migration.
	 */
	rq = this_rq_తాళం_irq(&rf);

	current->లోపల_memstall = 0;
	psi_task_change(current, TSK_MEMSTALL | TSK_MEMSTALL_RUNNING, 0);

	rq_unlock_irq(rq, &rf);
}
EXPORT_చిహ్నం_GPL(psi_memstall_leave);

#నిర్వచితమైతే CONFIG_CGROUPS
పూర్ణాంకం psi_cgroup_alloc(నిర్మాణం cgroup *cgroup)
{
	ఒకవేళ (!static_branch_likely(&psi_cgroups_enabled))
		తిరిగి 0;

	cgroup->psi = kzalloc_obj(నిర్మాణం psi_సమూహం);
	ఒకవేళ (!cgroup->psi)
		తిరిగి -ENOMEM;

	cgroup->psi->pcpu = alloc_percpu(నిర్మాణం psi_సమూహం_cpu);
	ఒకవేళ (!cgroup->psi->pcpu) {
		kfree(cgroup->psi);
		తిరిగి -ENOMEM;
	}
	సమూహం_init(cgroup->psi);
	cgroup->psi->మాతృ = cgroup_psi(cgroup_మాతృ(cgroup));
	తిరిగి 0;
}

శూన్య psi_cgroup_free(నిర్మాణం cgroup *cgroup)
{
	ఒకవేళ (!static_branch_likely(&psi_cgroups_enabled))
		తిరిగి;

	cancel_delayed_work_sync(&cgroup->psi->avgs_work);
	/*
	 * A psi_schedule_rtpoll_work() call racing the last trigger's
	 * destruction may have re-armed the timer after psi_trigger_destroy()
	 * deleted it. Spurious firing while the group is alive is harmless.
	 */
	timer_shutdown_sync(&cgroup->psi->rtpoll_timer);
	free_percpu(cgroup->psi->pcpu);
	/* All triggers must be removed by now */
	WARN_ONCE(cgroup->psi->rtpoll_స్థితులు, "psi: trigger leak\n");
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
శూన్య cgroup_move_task(నిర్మాణం task_struct *task, నిర్మాణం css_సమితి *గమ్యం)
{
	గుర్తులేని పూర్ణాంకం task_flags;
	నిర్మాణం rq_flags rf;
	నిర్మాణం rq *rq;

	ఒకవేళ (!static_branch_likely(&psi_cgroups_enabled)) {
		/*
		 * Lame to do this here, but the scheduler cannot be locked
		 * from the outside, so we move cgroups from inside sched/.
		 */
		rcu_కేటాయించు_సూచిక(task->cgroups, గమ్యం);
		తిరిగి;
	}

	rq = task_rq_తాళం_2(task, &rf);

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

	ఒకవేళ (task_flags)
		psi_task_change(task, task_flags, 0);

	/* See comment above */
	rcu_కేటాయించు_సూచిక(task->cgroups, గమ్యం);

	ఒకవేళ (task_flags)
		psi_task_change(task, 0, task_flags);

	task_rq_unlock(rq, task, &rf);
}

శూన్య psi_cgroup_restart(నిర్మాణం psi_సమూహం *సమూహం)
{
	పూర్ణాంకం cpu;

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
	ఒకవేళ (!సమూహం->enabled)
		తిరిగి;

	for_each_possible_cpu(cpu) {
		u64 now;

		guard(rq_తాళం_irq)(cpu_rq(cpu));

		psi_write_begin(cpu);
		now = cpu_clock(cpu);
		psi_సమూహం_change(సమూహం, cpu, 0, 0, now, true);
		psi_write_చివర(cpu);
	}
}
#అయితే_ముగింపు /* CONFIG_CGROUPS */

పూర్ణాంకం psi_show(నిర్మాణం seq_file *m, నిర్మాణం psi_సమూహం *సమూహం, గణన psi_res res)
{
	bool only_నిండిన = false;
	పూర్ణాంకం నిండిన;
	u64 now;

	ఒకవేళ (static_branch_likely(&psi_disabled))
		తిరిగి -EOPNOTSUPP;

#నిర్వచితమైతే CONFIG_IRQ_సమయం_ACCOUNTING
	ఒకవేళ (!irqtime_enabled() && res == PSI_IRQ)
		తిరిగి -EOPNOTSUPP;
#అయితే_ముగింపు

	/* Update averages before reporting them */
	mutex_తాళం(&సమూహం->avgs_తాళం);
	now = sched_clock();
	collect_percpu_times(సమూహం, PSI_AVGS, ఖాళీ);
	ఒకవేళ (now >= సమూహం->avg_తదుపరి_update)
		సమూహం->avg_తదుపరి_update = update_averages(సమూహం, now);
	mutex_unlock(&సమూహం->avgs_తాళం);

#నిర్వచితమైతే CONFIG_IRQ_సమయం_ACCOUNTING
	only_నిండిన = res == PSI_IRQ;
#అయితే_ముగింపు

	కొరకు (నిండిన = 0; నిండిన < 2 - only_నిండిన; నిండిన++) {
		గుర్తులేని దీర్ఘ avg[3] = { 0, };
		u64 మొత్తం = 0;
		పూర్ణాంకం w;

		/* CPU FULL is undefined at the system level */
		ఒకవేళ (!(సమూహం == &psi_system && res == PSI_CPU && నిండిన)) {
			కొరకు (w = 0; w < 3; w++)
				avg[w] = సమూహం->avg[res * 2 + నిండిన][w];
			మొత్తం = div_u64(సమూహం->మొత్తం[PSI_AVGS][res * 2 + నిండిన],
					NSEC_చొప్పున_USEC);
		}

		seq_printf(m, "%s avg10=%lu.%02lu avg60=%lu.%02lu avg300=%lu.%02lu total=%llu\n",
			   నిండిన || only_నిండిన ? "full" : "some",
			   ఎక్కించు_INT(avg[0]), ఎక్కించు_FRAC(avg[0]),
			   ఎక్కించు_INT(avg[1]), ఎక్కించు_FRAC(avg[1]),
			   ఎక్కించు_INT(avg[2]), ఎక్కించు_FRAC(avg[2]),
			   మొత్తం);
	}

	తిరిగి 0;
}

/*
 * Create @group's rtpoll worker after psi_trigger_create() reported the need
 * for one. kthread creation depends on the whole fork path and we don't want
 * all of that nested inside cgroup_mutex, so the caller must drop it and any
 * other lock that forks can wait behind. If two callers race, the loser stops
 * its never-woken kthread.
 */
పూర్ణాంకం psi_trigger_create_rtpoll_కార్మికుడు(నిర్మాణం psi_సమూహం *సమూహం)
{
	నిర్మాణం task_struct *task;

	task = kthread_create(psi_rtpoll_కార్మికుడు, సమూహం, "psimon");
	ఒకవేళ (IS_ERR(task))
		తిరిగి PTR_ERR(task);

	scoped_guard(mutex, &సమూహం->rtpoll_trigger_తాళం) {
		ఒకవేళ (!rcu_access_సూచిక(సమూహం->rtpoll_task)) {
			atomic_సమితి(&సమూహం->rtpoll_wakeup, 0);
			wake_పైకి_process(task);
			rcu_కేటాయించు_సూచిక(సమూహం->rtpoll_task, task);

			/*
			 * Poll once to catch up on scheduling attempts dropped
			 * while there was no rtpoll worker.
			 */
			psi_కాలపట్టిక_rtpoll_work(సమూహం, 1, true);
			తిరిగి 0;
		}
	}

	kthread_stop(task);
	తిరిగి 0;
}

నిర్మాణం psi_trigger *psi_trigger_create(నిర్మాణం psi_సమూహం *సమూహం, అక్షరం *buf,
				       గణన psi_res res, నిర్మాణం file *file,
				       నిర్మాణం kernfs_open_file *యొక్క,
				       bool *need_rtpoll_కార్మికుడు)
{
	నిర్మాణం psi_trigger *t;
	గణన psi_స్థితులు state;
	u32 పరిమితి_us;
	bool privileged;
	u32 కిటికీ_us;

	*need_rtpoll_కార్మికుడు = false;

	ఒకవేళ (static_branch_likely(&psi_disabled))
		తిరిగి ERR_PTR(-EOPNOTSUPP);

	/*
	 * Checking the privilege here on file->f_cred implies that a privileged user
	 * could open the file and delegate the write to an unprivileged one.
	 */
	privileged = cap_raised(file->f_cred->cap_effective, CAP_SYS_RESOURCE);

	ఒకవేళ (అక్షరశ్రేణి_చదువు(buf, "some %u %u", &పరిమితి_us, &కిటికీ_us) == 2)
		state = PSI_IO_SOME + res * 2;
	లేకపోతే ఒకవేళ (అక్షరశ్రేణి_చదువు(buf, "full %u %u", &పరిమితి_us, &కిటికీ_us) == 2)
		state = PSI_IO_నిండిన + res * 2;
	లేకపోతే
		తిరిగి ERR_PTR(-EINVAL);

#నిర్వచితమైతే CONFIG_IRQ_సమయం_ACCOUNTING
	ఒకవేళ (res == PSI_IRQ && --state != PSI_IRQ_నిండిన)
		తిరిగి ERR_PTR(-EINVAL);
#అయితే_ముగింపు

	ఒకవేళ (state >= PSI_NONIDLE)
		తిరిగి ERR_PTR(-EINVAL);

	ఒకవేళ (కిటికీ_us == 0 || కిటికీ_us > కిటికీ_MAX_US)
		తిరిగి ERR_PTR(-EINVAL);

	/*
	 * Unprivileged users can only use 2s windows so that averages aggregation
	 * work is used, and no RT threads need to be spawned.
	 */
	ఒకవేళ (!privileged && కిటికీ_us % 2000000)
		తిరిగి ERR_PTR(-EINVAL);

	/* Check threshold */
	ఒకవేళ (పరిమితి_us == 0 || పరిమితి_us > కిటికీ_us)
		తిరిగి ERR_PTR(-EINVAL);

	t = kmalloc_obj(*t);
	ఒకవేళ (!t)
		తిరిగి ERR_PTR(-ENOMEM);

	t->సమూహం = సమూహం;
	t->state = state;
	t->పరిమితి = పరిమితి_us * NSEC_చొప్పున_USEC;
	t->win.కొలత = కిటికీ_us * NSEC_చొప్పున_USEC;
	కిటికీ_reset(&t->win, sched_clock(),
			సమూహం->మొత్తం[PSI_POLL][t->state], 0);

	t->event = 0;
	t->చివరి_event_సమయం = 0;
	t->యొక్క = యొక్క;
	ఒకవేళ (!యొక్క)
		init_waitqueue_తల(&t->event_wait);
	t->pending_event = false;
	t->aggregator = privileged ? PSI_POLL : PSI_AVGS;

	ఒకవేళ (privileged) {
		mutex_తాళం(&సమూహం->rtpoll_trigger_తాళం);

		list_కూడు(&t->కణుపు, &సమూహం->rtpoll_triggers);
		సమూహం->rtpoll_min_period = min(సమూహం->rtpoll_min_period,
			div_u64(t->win.కొలత, UPDATES_చొప్పున_కిటికీ));
		సమూహం->rtpoll_nr_triggers[t->state]++;
		సమూహం->rtpoll_స్థితులు |= (1 << t->state);

		*need_rtpoll_కార్మికుడు = !rcu_access_సూచిక(సమూహం->rtpoll_task);

		mutex_unlock(&సమూహం->rtpoll_trigger_తాళం);
	} లేకపోతే {
		mutex_తాళం(&సమూహం->avgs_తాళం);

		list_కూడు(&t->కణుపు, &సమూహం->avg_triggers);
		సమూహం->avg_nr_triggers[t->state]++;

		mutex_unlock(&సమూహం->avgs_తాళం);
	}
	తిరిగి t;
}

శూన్య psi_trigger_destroy(నిర్మాణం psi_trigger *t)
{
	నిర్మాణం psi_సమూహం *సమూహం;
	నిర్మాణం task_struct *task_గమ్యం_destroy = ఖాళీ;

	/*
	 * We do not check psi_disabled since it might have been disabled after
	 * the trigger got created.
	 */
	ఒకవేళ (!t)
		తిరిగి;

	సమూహం = t->సమూహం;
	/*
	 * Wakeup waiters to stop polling and clear the queue to prevent it from
	 * being accessed later. Can happen if cgroup is deleted from under a
	 * polling process.
	 */
	ఒకవేళ (t->యొక్క)
		kernfs_notify(t->యొక్క->kn);
	లేకపోతే
		wake_పైకి_interruptible(&t->event_wait);

	ఒకవేళ (t->aggregator == PSI_AVGS) {
		mutex_తాళం(&సమూహం->avgs_తాళం);
		ఒకవేళ (!list_ఖాళీలు(&t->కణుపు)) {
			list_del(&t->కణుపు);
			సమూహం->avg_nr_triggers[t->state]--;
		}
		mutex_unlock(&సమూహం->avgs_తాళం);
	} లేకపోతే {
		mutex_తాళం(&సమూహం->rtpoll_trigger_తాళం);
		ఒకవేళ (!list_ఖాళీలు(&t->కణుపు)) {
			నిర్మాణం psi_trigger *tmp;
			u64 period = ULLONG_MAX;

			list_del(&t->కణుపు);
			సమూహం->rtpoll_nr_triggers[t->state]--;
			ఒకవేళ (!సమూహం->rtpoll_nr_triggers[t->state])
				సమూహం->rtpoll_స్థితులు &= ~(1 << t->state);
			/*
			 * Reset min update period for the remaining triggers
			 * iff the destroying trigger had the min window size.
			 */
			ఒకవేళ (సమూహం->rtpoll_min_period == div_u64(t->win.కొలత, UPDATES_చొప్పున_కిటికీ)) {
				list_for_each_నమోదు(tmp, &సమూహం->rtpoll_triggers, కణుపు)
					period = min(period, div_u64(tmp->win.కొలత,
							UPDATES_చొప్పున_కిటికీ));
				సమూహం->rtpoll_min_period = period;
			}
			/* Destroy rtpoll_task when the last trigger is destroyed */
			ఒకవేళ (సమూహం->rtpoll_స్థితులు == 0) {
				సమూహం->rtpoll_until = 0;
				task_గమ్యం_destroy = rcu_dereference_protected(
						సమూహం->rtpoll_task,
						lockdep_is_held(&సమూహం->rtpoll_trigger_తాళం));
				rcu_కేటాయించు_సూచిక(సమూహం->rtpoll_task, ఖాళీ);
				timer_delete(&సమూహం->rtpoll_timer);
			}
		}
		mutex_unlock(&సమూహం->rtpoll_trigger_తాళం);
	}

	/*
	 * Stop kthread 'psimon' after releasing rtpoll_trigger_lock to prevent
	 * a deadlock while waiting for psi_rtpoll_work to acquire
	 * rtpoll_trigger_lock
	 */
	ఒకవేళ (task_గమ్యం_destroy) {
		kthread_stop(task_గమ్యం_destroy);
		atomic_సమితి(&సమూహం->rtpoll_scheduled, 0);
	}
	kfree(t);
}

__poll_t psi_trigger_poll(శూన్య **trigger_ptr,
				నిర్మాణం file *file, poll_పట్టిక *wait)
{
	__poll_t ret = DEFAULT_POLLMASK;
	నిర్మాణం psi_trigger *t;

	ఒకవేళ (static_branch_likely(&psi_disabled))
		తిరిగి DEFAULT_POLLMASK | EPOLLERR | EPOLLPRI;

	t = smp_ఎక్కించు_acquire(trigger_ptr);
	ఒకవేళ (!t)
		తిరిగి DEFAULT_POLLMASK | EPOLLERR | EPOLLPRI;

	ఒకవేళ (t->యొక్క)
		kernfs_generic_poll(t->యొక్క, wait);
	లేకపోతే
		poll_wait(file, &t->event_wait, wait);

	ఒకవేళ (cmpxchg(&t->event, 1, 0) == 1)
		ret |= EPOLLPRI;

	తిరిగి ret;
}

#నిర్వచితమైతే CONFIG_PROC_FS
స్థిర పూర్ణాంకం psi_io_show(నిర్మాణం seq_file *m, శూన్య *v)
{
	తిరిగి psi_show(m, &psi_system, PSI_IO);
}

స్థిర పూర్ణాంకం psi_జ్ఞాపకం_show(నిర్మాణం seq_file *m, శూన్య *v)
{
	తిరిగి psi_show(m, &psi_system, PSI_MEM);
}

స్థిర పూర్ణాంకం psi_cpu_show(నిర్మాణం seq_file *m, శూన్య *v)
{
	తిరిగి psi_show(m, &psi_system, PSI_CPU);
}

స్థిర పూర్ణాంకం psi_io_open(నిర్మాణం inode *inode, నిర్మాణం file *file)
{
	తిరిగి single_open(file, psi_io_show, ఖాళీ);
}

స్థిర పూర్ణాంకం psi_జ్ఞాపకం_open(నిర్మాణం inode *inode, నిర్మాణం file *file)
{
	తిరిగి single_open(file, psi_జ్ఞాపకం_show, ఖాళీ);
}

స్థిర పూర్ణాంకం psi_cpu_open(నిర్మాణం inode *inode, నిర్మాణం file *file)
{
	తిరిగి single_open(file, psi_cpu_show, ఖాళీ);
}

స్థిర ssize_t psi_write(నిర్మాణం file *file, స్థిరరాశి అక్షరం __user *user_buf,
			 కొలత_t nbytes, గణన psi_res res)
{
	అక్షరం buf[32];
	కొలత_t buf_కొలత;
	నిర్మాణం seq_file *seq;
	నిర్మాణం psi_trigger *new;
	bool need_rtpoll_కార్మికుడు;
	పూర్ణాంకం ret;

	ఒకవేళ (static_branch_likely(&psi_disabled))
		తిరిగి -EOPNOTSUPP;

	ఒకవేళ (!nbytes)
		తిరిగి -EINVAL;

	buf_కొలత = min(nbytes, పరిమాణం(buf));
	ఒకవేళ (copy_from_user(buf, user_buf, buf_కొలత))
		తిరిగి -EFAULT;

	buf[buf_కొలత - 1] = '\0';

	seq = file->వ్యక్తిగత_దత్తాంశం;

	/* Take seq->lock to protect seq->private from concurrent writes */
	mutex_తాళం(&seq->తాళం);

	/* Allow only one trigger per file descriptor */
	ఒకవేళ (seq->అంతరంగ) {
		mutex_unlock(&seq->తాళం);
		తిరిగి -EBUSY;
	}

	new = psi_trigger_create(&psi_system, buf, res, file, ఖాళీ,
				 &need_rtpoll_కార్మికుడు);
	ఒకవేళ (IS_ERR(new)) {
		mutex_unlock(&seq->తాళం);
		తిరిగి PTR_ERR(new);
	}

	ఒకవేళ (need_rtpoll_కార్మికుడు) {
		ret = psi_trigger_create_rtpoll_కార్మికుడు(&psi_system);
		ఒకవేళ (ret) {
			psi_trigger_destroy(new);
			mutex_unlock(&seq->తాళం);
			తిరిగి ret;
		}
	}

	smp_store_release(&seq->అంతరంగ, new);
	mutex_unlock(&seq->తాళం);

	తిరిగి nbytes;
}

స్థిర ssize_t psi_io_write(నిర్మాణం file *file, స్థిరరాశి అక్షరం __user *user_buf,
			    కొలత_t nbytes, loff_t *ppos)
{
	తిరిగి psi_write(file, user_buf, nbytes, PSI_IO);
}

స్థిర ssize_t psi_జ్ఞాపకం_write(నిర్మాణం file *file, స్థిరరాశి అక్షరం __user *user_buf,
				కొలత_t nbytes, loff_t *ppos)
{
	తిరిగి psi_write(file, user_buf, nbytes, PSI_MEM);
}

స్థిర ssize_t psi_cpu_write(నిర్మాణం file *file, స్థిరరాశి అక్షరం __user *user_buf,
			     కొలత_t nbytes, loff_t *ppos)
{
	తిరిగి psi_write(file, user_buf, nbytes, PSI_CPU);
}

స్థిర __poll_t psi_fop_poll(నిర్మాణం file *file, poll_పట్టిక *wait)
{
	నిర్మాణం seq_file *seq = file->వ్యక్తిగత_దత్తాంశం;

	తిరిగి psi_trigger_poll(&seq->అంతరంగ, file, wait);
}

స్థిర పూర్ణాంకం psi_fop_release(నిర్మాణం inode *inode, నిర్మాణం file *file)
{
	నిర్మాణం seq_file *seq = file->వ్యక్తిగత_దత్తాంశం;

	psi_trigger_destroy(seq->అంతరంగ);
	తిరిగి single_release(inode, file);
}

స్థిర స్థిరరాశి నిర్మాణం proc_ops psi_io_proc_ops = {
	.proc_open	= psi_io_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_io_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

స్థిర స్థిరరాశి నిర్మాణం proc_ops psi_జ్ఞాపకం_proc_ops = {
	.proc_open	= psi_జ్ఞాపకం_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_జ్ఞాపకం_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

స్థిర స్థిరరాశి నిర్మాణం proc_ops psi_cpu_proc_ops = {
	.proc_open	= psi_cpu_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_cpu_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

#నిర్వచితమైతే CONFIG_IRQ_సమయం_ACCOUNTING
స్థిర పూర్ణాంకం psi_irq_show(నిర్మాణం seq_file *m, శూన్య *v)
{
	తిరిగి psi_show(m, &psi_system, PSI_IRQ);
}

స్థిర పూర్ణాంకం psi_irq_open(నిర్మాణం inode *inode, నిర్మాణం file *file)
{
	తిరిగి single_open(file, psi_irq_show, ఖాళీ);
}

స్థిర ssize_t psi_irq_write(నిర్మాణం file *file, స్థిరరాశి అక్షరం __user *user_buf,
			     కొలత_t nbytes, loff_t *ppos)
{
	తిరిగి psi_write(file, user_buf, nbytes, PSI_IRQ);
}

స్థిర స్థిరరాశి నిర్మాణం proc_ops psi_irq_proc_ops = {
	.proc_open	= psi_irq_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_irq_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};
#అయితే_ముగింపు /* CONFIG_IRQ_TIME_ACCOUNTING */

స్థిర పూర్ణాంకం __init psi_proc_init(శూన్య)
{
	ఒకవేళ (psi_enable) {
		proc_mkdir("pressure", ఖాళీ);
		proc_create("pressure/io", 0666, ఖాళీ, &psi_io_proc_ops);
		proc_create("pressure/memory", 0666, ఖాళీ, &psi_జ్ఞాపకం_proc_ops);
		proc_create("pressure/cpu", 0666, ఖాళీ, &psi_cpu_proc_ops);
#నిర్వచితమైతే CONFIG_IRQ_సమయం_ACCOUNTING
		proc_create("pressure/irq", 0666, ఖాళీ, &psi_irq_proc_ops);
#అయితే_ముగింపు
	}
	తిరిగి 0;
}
module_init(psi_proc_init);

#అయితే_ముగింపు /* CONFIG_PROC_FS */
