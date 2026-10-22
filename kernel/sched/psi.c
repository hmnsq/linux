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
#अन्तर्भाव <linux/sched/clock.h>
#अन्तर्भाव <linux/workqueue.h>
#अन्तर्भाव <linux/psi.h>
#अन्तर्भाव "sched.h"

स्थिर पूर्णाङ्क psi_bug __read_mostly;

DEFINE_STATIC_कुञ्चिका_FALSE(psi_disabled);
स्थिर DEFINE_STATIC_कुञ्चिका_TRUE(psi_cgroups_enabled);

#यदि_परिभाषितम् CONFIG_PSI_DEFAULT_DISABLED
स्थिर bool psi_enable;
#अन्यथा
स्थिर bool psi_enable = true;
#यदि_अन्तः
स्थिर पूर्णाङ्क __init सज्जता_psi(वर्ण *str)
{
	प्रत्यागम kstrtobool(str, &psi_enable) == 0;
}
__setup("psi=", सज्जता_psi);

/* Running averages - we need to be higher-res than loadavg */
#परिभाषय PSI_FREQ	(2*HZ+1)	/* 2 sec intervals */
#परिभाषय EXP_10s		1677		/* 1/exp(2s/10s) as fixed-point */
#परिभाषय EXP_60s		1981		/* 1/exp(2s/60s) */
#परिभाषय EXP_300s	2034		/* 1/exp(2s/300s) */

/* PSI trigger definitions */
#परिभाषय गवाक्षः_MAX_US 10000000	/* Max window size is 10s */
#परिभाषय UPDATES_प्रत्येकम्_गवाक्षः 10	/* 10 updates per window */

/* Sampling frequency in nanoseconds */
स्थिर u64 psi_period __read_mostly;

/* System-level pressure and stall tracking */
स्थिर DEFINE_प्रत्येकम्_CPU(संरचना psi_गणः_cpu, system_गणः_pcpu);
संरचना psi_गणः psi_system = {
	.pcpu = &system_गणः_pcpu,
};

स्थिर DEFINE_प्रत्येकम्_CPU(seqcount_t, psi_seq) = SEQCNT_ZERO(psi_seq);

स्थिर अन्तःस्थ शून्य psi_write_begin(पूर्णाङ्क cpu)
{
	write_seqcount_begin(प्रत्येकम्_cpu_ptr(&psi_seq, cpu));
}

स्थिर अन्तःस्थ शून्य psi_write_अन्तः(पूर्णाङ्क cpu)
{
	write_seqcount_अन्तः(प्रत्येकम्_cpu_ptr(&psi_seq, cpu));
}

स्थिर अन्तःस्थ u32 psi_read_begin(पूर्णाङ्क cpu)
{
	प्रत्यागम read_seqcount_begin(प्रत्येकम्_cpu_ptr(&psi_seq, cpu));
}

स्थिर अन्तःस्थ bool psi_read_retry(पूर्णाङ्क cpu, u32 seq)
{
	प्रत्यागम read_seqcount_retry(प्रत्येकम्_cpu_ptr(&psi_seq, cpu), seq);
}

स्थिर शून्य psi_avgs_work(संरचना work_struct *work);

स्थिर शून्य poll_timer_fn(संरचना timer_list *t);

स्थिर शून्य गणः_init(संरचना psi_गणः *गणः)
{
	गणः->enabled = true;
	गणः->avg_चरमः_update = sched_clock();
	गणः->avg_अग्र_update = गणः->avg_चरमः_update + psi_period;
	mutex_init(&गणः->avgs_तालकम्);

	/* Init avg trigger-related members */
	INIT_LIST_शीर्ष(&गणः->avg_triggers);
	स्मृतिं_पूरय(गणः->avg_nr_triggers, 0, परिमाण(गणः->avg_nr_triggers));
	INIT_DELAYED_WORK(&गणः->avgs_work, psi_avgs_work);

	/* Init rtpoll trigger-related members */
	atomic_समुच्चयः(&गणः->rtpoll_scheduled, 0);
	mutex_init(&गणः->rtpoll_trigger_तालकम्);
	INIT_LIST_शीर्ष(&गणः->rtpoll_triggers);
	गणः->rtpoll_min_period = U32_MAX;
	गणः->rtpoll_अग्र_update = ULLONG_MAX;
	init_waitqueue_शीर्ष(&गणः->rtpoll_wait);
	timer_सज्जता(&गणः->rtpoll_timer, poll_timer_fn, 0);
	rcu_नियोजय_pointer(गणः->rtpoll_task, रिक्त);
}

शून्य __init psi_init(शून्य)
{
	यदि (!psi_enable) {
		static_branch_enable(&psi_disabled);
		static_branch_disable(&psi_cgroups_enabled);
		प्रत्यागम;
	}

	यदि (!cgroup_psi_enabled())
		static_branch_disable(&psi_cgroups_enabled);

	psi_period = jiffies_गन्तव्यम्_nsecs(PSI_FREQ);
	गणः_init(&psi_system);
}

स्थिर u32 test_अवस्थाः(अचिह्नित पूर्णाङ्क *tasks, u32 state_आवरणम्)
{
	ध्रुव bool oncpu = state_आवरणम् & PSI_ONCPU;

	यदि (tasks[NR_IOWAIT]) {
		state_आवरणम् |= BIT(PSI_IO_SOME);
		यदि (!tasks[NR_RUNNING])
			state_आवरणम् |= BIT(PSI_IO_पूर्णम्);
	}

	यदि (tasks[NR_MEMSTALL]) {
		state_आवरणम् |= BIT(PSI_MEM_SOME);
		यदि (tasks[NR_RUNNING] == tasks[NR_MEMSTALL_RUNNING])
			state_आवरणम् |= BIT(PSI_MEM_पूर्णम्);
	}

	यदि (tasks[NR_RUNNING] > oncpu)
		state_आवरणम् |= BIT(PSI_CPU_SOME);

	यदि (tasks[NR_RUNNING] && !oncpu)
		state_आवरणम् |= BIT(PSI_CPU_पूर्णम्);

	यदि (tasks[NR_IOWAIT] || tasks[NR_MEMSTALL] || tasks[NR_RUNNING])
		state_आवरणम् |= BIT(PSI_NONIDLE);

	प्रत्यागम state_आवरणम्;
}

स्थिर शून्य get_recent_times(संरचना psi_गणः *गणः, पूर्णाङ्क cpu,
			     गणना psi_aggregators aggregator, u32 *times,
			     u32 *pchanged_अवस्थाः)
{
	संरचना psi_गणः_cpu *groupc = प्रत्येकम्_cpu_ptr(गणः->pcpu, cpu);
	पूर्णाङ्क current_cpu = raw_smp_processor_id();
	अचिह्नित पूर्णाङ्क tasks[NR_PSI_TASK_COUNTS];
	u64 now, state_start;
	गणना psi_अवस्थाः s;
	अचिह्नित पूर्णाङ्क seq;
	u32 state_आवरणम्;

	*pchanged_अवस्थाः = 0;

	/* Snapshot a coherent view of the CPU state */
	कुरु {
		seq = psi_read_begin(cpu);
		now = cpu_clock(cpu);
		स्मृतिं_प्रतिलिख(times, groupc->times, परिमाण(groupc->times));
		state_आवरणम् = groupc->state_आवरणम्;
		state_start = groupc->state_start;
		यदि (cpu == current_cpu)
			स्मृतिं_प्रतिलिख(tasks, groupc->tasks, परिमाण(groupc->tasks));
	} यावत् (psi_read_retry(cpu, seq));

	/* Calculate state time deltas against the previous snapshot */
	प्रति (s = 0; s < NR_PSI_अवस्थाः; s++) {
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
		यदि (state_आवरणम् & (1 << s))
			times[s] += now - state_start;

		delta = times[s] - groupc->times_पूर्व[aggregator][s];
		groupc->times_पूर्व[aggregator][s] = times[s];

		times[s] = delta;
		यदि (delta)
			*pchanged_अवस्थाः |= (1 << s);
	}

	/*
	 * When collect_percpu_times() from the avgs_work, we don't want to
	 * re-arm avgs_work when all CPUs are IDLE. But the current CPU running
	 * this avgs_work is never IDLE, cause avgs_work can't be shut off.
	 * So for the current CPU, we need to re-arm avgs_work only when
	 * (NR_RUNNING > 1 || NR_IOWAIT > 0 || NR_MEMSTALL > 0), for other CPUs
	 * we can just check PSI_NONIDLE delta.
	 */
	यदि (current_work() == &गणः->avgs_work.work) {
		bool reschedule;

		यदि (cpu == current_cpu)
			reschedule = tasks[NR_RUNNING] +
				     tasks[NR_IOWAIT] +
				     tasks[NR_MEMSTALL] > 1;
		अन्यथा
			reschedule = *pchanged_अवस्थाः & (1 << PSI_NONIDLE);

		यदि (reschedule)
			*pchanged_अवस्थाः |= PSI_STATE_RESCHEDULE;
	}
}

स्थिर शून्य calc_avgs(अचिह्नित दीर्घ avg[3], पूर्णाङ्क missed_periods,
		      u64 time, u64 period)
{
	अचिह्नित दीर्घ pct;

	/* Fill in zeroes for periods of no activity */
	यदि (missed_periods) {
		avg[0] = calc_आरोपय_n(avg[0], EXP_10s, 0, missed_periods);
		avg[1] = calc_आरोपय_n(avg[1], EXP_60s, 0, missed_periods);
		avg[2] = calc_आरोपय_n(avg[2], EXP_300s, 0, missed_periods);
	}

	/* Sample the most recent active period */
	pct = div_u64(time * 100, period);
	pct *= FIXED_1;
	avg[0] = calc_आरोपय(avg[0], EXP_10s, pct);
	avg[1] = calc_आरोपय(avg[1], EXP_60s, pct);
	avg[2] = calc_आरोपय(avg[2], EXP_300s, pct);
}

स्थिर शून्य collect_percpu_times(संरचना psi_गणः *गणः,
				 गणना psi_aggregators aggregator,
				 u32 *pchanged_अवस्थाः)
{
	u64 deltas[NR_PSI_अवस्थाः - 1] = { 0, };
	अचिह्नित दीर्घ nonidle_योग = 0;
	u32 परिवर्तितम्_अवस्थाः = 0;
	पूर्णाङ्क cpu;
	पूर्णाङ्क s;

	/*
	 * Collect the per-cpu time buckets and average them into a
	 * single time sample that is normalized to wall clock time.
	 *
	 * For averaging, each CPU is weighted by its non-idle time in
	 * the sampling period. This eliminates artifacts from uneven
	 * loading, or even entirely idle CPUs.
	 */
	for_each_possible_cpu(cpu) {
		u32 times[NR_PSI_अवस्थाः];
		u32 nonidle;
		u32 cpu_परिवर्तितम्_अवस्थाः;

		get_recent_times(गणः, cpu, aggregator, times,
				&cpu_परिवर्तितम्_अवस्थाः);
		परिवर्तितम्_अवस्थाः |= cpu_परिवर्तितम्_अवस्थाः;

		nonidle = nsecs_गन्तव्यम्_jiffies(times[PSI_NONIDLE]);
		nonidle_योग += nonidle;

		प्रति (s = 0; s < PSI_NONIDLE; s++)
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
	प्रति (s = 0; s < NR_PSI_अवस्थाः - 1; s++)
		गणः->योग_2[aggregator][s] +=
				div_u64(deltas[s], max(nonidle_योग, 1UL));

	यदि (pchanged_अवस्थाः)
		*pchanged_अवस्थाः = परिवर्तितम्_अवस्थाः;
}

/* Trigger tracking window manipulations */
स्थिर शून्य गवाक्षः_reset(संरचना psi_गवाक्षः *win, u64 now, u64 मानम्,
			 u64 पूर्व_growth)
{
	win->start_time = now;
	win->start_मानम् = मानम्;
	win->पूर्व_growth = पूर्व_growth;
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
स्थिर u64 गवाक्षः_update(संरचना psi_गवाक्षः *win, u64 now, u64 मानम्)
{
	u64 elapsed;
	u64 growth;

	elapsed = now - win->start_time;
	growth = मानम् - win->start_मानम्;
	/*
	 * After each tracking window passes win->start_value and
	 * win->start_time get reset and win->prev_growth stores
	 * the average per-window growth of the previous window.
	 * win->prev_growth is then used to interpolate additional
	 * growth from the previous window assuming it was linear.
	 */
	यदि (elapsed > win->आकार)
		गवाक्षः_reset(win, now, मानम्, growth);
	अन्यथा {
		u32 remaining;

		remaining = win->आकार - elapsed;
		growth += div64_u64(win->पूर्व_growth * remaining, win->आकार);
	}

	प्रत्यागम growth;
}

स्थिर शून्य update_triggers(संरचना psi_गणः *गणः, u64 now,
						   गणना psi_aggregators aggregator)
{
	संरचना psi_trigger *t;
	u64 *योग_2 = गणः->योग_2[aggregator];
	संरचना list_शीर्ष *triggers;
	u64 *aggregator_योग;

	यदि (aggregator == PSI_AVGS) {
		triggers = &गणः->avg_triggers;
		aggregator_योग = गणः->avg_योग;
	} अन्यथा {
		triggers = &गणः->rtpoll_triggers;
		aggregator_योग = गणः->rtpoll_योग;
	}

	/*
	 * On subsequent updates, calculate growth deltas and let
	 * watchers know when their specified thresholds are exceeded.
	 */
	list_for_each_प्रविष्टिः(t, triggers, ग्रन्थिः) {
		u64 growth;
		bool new_stall;

		new_stall = aggregator_योग[t->state] != योग_2[t->state];

		/* Check for stall activity or a previous threshold breach */
		यदि (!new_stall && !t->pending_event)
			अनुवर्त;
		/*
		 * Check for new stall activity, as well as deferred
		 * events that occurred in the last window after the
		 * trigger had already fired (we want to ratelimit
		 * events without dropping any).
		 */
		यदि (new_stall) {
			/* Calculate growth since last update */
			growth = गवाक्षः_update(&t->win, now, योग_2[t->state]);
			यदि (!t->pending_event) {
				यदि (growth < t->देहली)
					अनुवर्त;

				t->pending_event = true;
			}
		}
		/* Limit event signaling to once per window */
		यदि (now < t->चरमः_event_time + t->win.आकार)
			अनुवर्त;

		/* Generate an event */
		यदि (cmpxchg(&t->event, 0, 1) == 0) {
			यदि (t->सम्बन्धि)
				kernfs_notify(t->सम्बन्धि->kn);
			अन्यथा
				wake_ऊर्ध्वम्_interruptible(&t->event_wait);
		}
		t->चरमः_event_time = now;
		/* Reset threshold breach flag once event got generated */
		t->pending_event = false;
	}
}

स्थिर u64 update_averages(संरचना psi_गणः *गणः, u64 now)
{
	अचिह्नित दीर्घ missed_periods = 0;
	u64 expires, period;
	u64 avg_अग्र_update;
	पूर्णाङ्क s;

	/* avgX= */
	expires = गणः->avg_अग्र_update;
	यदि (now - expires >= psi_period)
		missed_periods = div_u64(now - expires, psi_period);

	/*
	 * The periodic clock tick can get delayed for various
	 * reasons, especially on loaded systems. To avoid clock
	 * drift, we schedule the clock in fixed psi_period intervals.
	 * But the deltas we sample out of the per-cpu buckets above
	 * are based on the actual time elapsing between clock ticks.
	 */
	avg_अग्र_update = expires + ((1 + missed_periods) * psi_period);
	period = now - (गणः->avg_चरमः_update + (missed_periods * psi_period));
	गणः->avg_चरमः_update = now;

	प्रति (s = 0; s < NR_PSI_अवस्थाः - 1; s++) {
		u32 sample;

		sample = गणः->योग_2[PSI_AVGS][s] - गणः->avg_योग[s];
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
		यदि (sample > period)
			sample = period;
		गणः->avg_योग[s] += sample;
		calc_avgs(गणः->avg[s], missed_periods, sample, period);
	}

	प्रत्यागम avg_अग्र_update;
}

स्थिर शून्य psi_avgs_work(संरचना work_struct *work)
{
	संरचना delayed_work *dwork;
	संरचना psi_गणः *गणः;
	u32 परिवर्तितम्_अवस्थाः;
	u64 now;

	dwork = गन्तव्यम्_delayed_work(work);
	गणः = container_सम्बन्धि(dwork, संरचना psi_गणः, avgs_work);

	mutex_तालकम्(&गणः->avgs_तालकम्);

	now = sched_clock();

	collect_percpu_times(गणः, PSI_AVGS, &परिवर्तितम्_अवस्थाः);
	/*
	 * If there is task activity, periodically fold the per-cpu
	 * times and feed samples into the running averages. If things
	 * are idle and there is no data to process, stop the clock.
	 * Once restarted, we'll catch up the running averages in one
	 * go - see calc_avgs() and missed_periods.
	 */
	यदि (now >= गणः->avg_अग्र_update) {
		update_triggers(गणः, now, PSI_AVGS);
		गणः->avg_अग्र_update = update_averages(गणः, now);
	}

	यदि (परिवर्तितम्_अवस्थाः & PSI_STATE_RESCHEDULE) {
		कालसारणी_delayed_work(dwork, nsecs_गन्तव्यम्_jiffies(
				गणः->avg_अग्र_update - now) + 1);
	}

	mutex_unlock(&गणः->avgs_तालकम्);
}

स्थिर शून्य init_rtpoll_triggers(संरचना psi_गणः *गणः, u64 now)
{
	संरचना psi_trigger *t;

	list_for_each_प्रविष्टिः(t, &गणः->rtpoll_triggers, ग्रन्थिः)
		गवाक्षः_reset(&t->win, now,
				गणः->योग_2[PSI_POLL][t->state], 0);
	स्मृतिं_प्रतिलिख(गणः->rtpoll_योग, गणः->योग_2[PSI_POLL],
		   परिमाण(गणः->rtpoll_योग));
	गणः->rtpoll_अग्र_update = now + गणः->rtpoll_min_period;
}

/* Schedule rtpolling if it's not already scheduled or forced. */
स्थिर शून्य psi_कालसारणी_rtpoll_work(संरचना psi_गणः *गणः, अचिह्नित दीर्घ delay,
				   bool force)
{
	/*
	 * atomic_xchg should be called even when !force to provide a
	 * full memory barrier (see the comment inside psi_rtpoll_work).
	 */
	यदि (atomic_xchg(&गणः->rtpoll_scheduled, 1) && !force)
		प्रत्यागम;

	/*
	 * Only test whether a worker is installed; do not dereference the task.
	 * A racing trigger destruction may leave the timer armed.
	 * psi_cgroup_free() shuts it down before freeing the group.
	 * The system PSI group is static.
	 */
	यदि (likely(rcu_access_pointer(गणः->rtpoll_task)))
		mod_timer(&गणः->rtpoll_timer, jiffies + delay);
	अन्यथा
		atomic_समुच्चयः(&गणः->rtpoll_scheduled, 0);
}

स्थिर शून्य psi_rtpoll_work(संरचना psi_गणः *गणः)
{
	bool force_reschedule = false;
	u32 परिवर्तितम्_अवस्थाः;
	u64 now;

	mutex_तालकम्(&गणः->rtpoll_trigger_तालकम्);

	now = sched_clock();

	यदि (now > गणः->rtpoll_until) {
		/*
		 * We are either about to start or might stop rtpolling if no
		 * state change was recorded. Resetting rtpoll_scheduled leaves
		 * a small window for psi_group_change to sneak in and schedule
		 * an immediate rtpoll_work before we get to rescheduling. One
		 * potential extra wakeup at the end of the rtpolling window
		 * should be negligible and rtpoll_next_update still keeps
		 * updates correctly on schedule.
		 */
		atomic_समुच्चयः(&गणः->rtpoll_scheduled, 0);
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
	} अन्यथा {
		/* The rtpolling window is not over, keep rescheduling */
		force_reschedule = true;
	}


	collect_percpu_times(गणः, PSI_POLL, &परिवर्तितम्_अवस्थाः);

	यदि (परिवर्तितम्_अवस्थाः & गणः->rtpoll_अवस्थाः) {
		/* Initialize trigger windows when entering rtpolling mode */
		यदि (now > गणः->rtpoll_until)
			init_rtpoll_triggers(गणः, now);

		/*
		 * Keep the monitor active for at least the duration of the
		 * minimum tracking window as long as monitor states are
		 * changing.
		 */
		गणः->rtpoll_until = now +
			गणः->rtpoll_min_period * UPDATES_प्रत्येकम्_गवाक्षः;
	}

	यदि (now > गणः->rtpoll_until) {
		गणः->rtpoll_अग्र_update = ULLONG_MAX;
		गच्छ बहिर्गमनम्;
	}

	यदि (now >= गणः->rtpoll_अग्र_update) {
		यदि (परिवर्तितम्_अवस्थाः & गणः->rtpoll_अवस्थाः) {
			update_triggers(गणः, now, PSI_POLL);
			स्मृतिं_प्रतिलिख(गणः->rtpoll_योग, गणः->योग_2[PSI_POLL],
				   परिमाण(गणः->rtpoll_योग));
		}
		गणः->rtpoll_अग्र_update = now + गणः->rtpoll_min_period;
	}

	psi_कालसारणी_rtpoll_work(गणः,
		nsecs_गन्तव्यम्_jiffies(गणः->rtpoll_अग्र_update - now) + 1,
		force_reschedule);

बहिर्गमनम्:
	mutex_unlock(&गणः->rtpoll_trigger_तालकम्);
}

स्थिर पूर्णाङ्क psi_rtpoll_कार्यकर्ता(शून्य *दत्त)
{
	संरचना psi_गणः *गणः = (संरचना psi_गणः *)दत्त;

	sched_समुच्चयः_fifo_नीचः(current);

	यावत् (true) {
		wait_event_interruptible(गणः->rtpoll_wait,
				atomic_cmpxchg(&गणः->rtpoll_wakeup, 1, 0) ||
				kthread_should_stop());
		यदि (kthread_should_stop())
			भङ्ग;

		psi_rtpoll_work(गणः);
	}
	प्रत्यागम 0;
}

स्थिर शून्य poll_timer_fn(संरचना timer_list *t)
{
	संरचना psi_गणः *गणः = timer_container_सम्बन्धि(गणः, t, rtpoll_timer);

	atomic_समुच्चयः(&गणः->rtpoll_wakeup, 1);
	wake_ऊर्ध्वम्_interruptible(&गणः->rtpoll_wait);
}

स्थिर शून्य लेखः_times(संरचना psi_गणः_cpu *groupc, u64 now)
{
	u32 delta;

	delta = now - groupc->state_start;
	groupc->state_start = now;

	यदि (groupc->state_आवरणम् & (1 << PSI_IO_SOME)) {
		groupc->times[PSI_IO_SOME] += delta;
		यदि (groupc->state_आवरणम् & (1 << PSI_IO_पूर्णम्))
			groupc->times[PSI_IO_पूर्णम्] += delta;
	}

	यदि (groupc->state_आवरणम् & (1 << PSI_MEM_SOME)) {
		groupc->times[PSI_MEM_SOME] += delta;
		यदि (groupc->state_आवरणम् & (1 << PSI_MEM_पूर्णम्))
			groupc->times[PSI_MEM_पूर्णम्] += delta;
	}

	यदि (groupc->state_आवरणम् & (1 << PSI_CPU_SOME)) {
		groupc->times[PSI_CPU_SOME] += delta;
		यदि (groupc->state_आवरणम् & (1 << PSI_CPU_पूर्णम्))
			groupc->times[PSI_CPU_पूर्णम्] += delta;
	}

	यदि (groupc->state_आवरणम् & (1 << PSI_NONIDLE))
		groupc->times[PSI_NONIDLE] += delta;
}

#परिभाषय for_each_गणः(iter, गणः) \
	प्रति (typeof(गणः) iter = गणः; iter; iter = iter->parent)

स्थिर शून्य psi_गणः_change(संरचना psi_गणः *गणः, पूर्णाङ्क cpu,
			     अचिह्नित पूर्णाङ्क clear, अचिह्नित पूर्णाङ्क समुच्चयः,
			     u64 now, bool wake_clock)
{
	संरचना psi_गणः_cpu *groupc;
	अचिह्नित पूर्णाङ्क t, m;
	u32 state_आवरणम्;

	lockdep_assert_rq_held(cpu_rq(cpu));
	groupc = प्रत्येकम्_cpu_ptr(गणः->pcpu, cpu);

	/*
	 * Start with TSK_ONCPU, which doesn't have a corresponding
	 * task count - it's just a boolean flag directly encoded in
	 * the state mask. Clear, set, or carry the current state if
	 * no changes are requested.
	 */
	यदि (unlikely(clear & TSK_ONCPU)) {
		state_आवरणम् = 0;
		clear &= ~TSK_ONCPU;
	} अन्यथा यदि (unlikely(समुच्चयः & TSK_ONCPU)) {
		state_आवरणम् = PSI_ONCPU;
		समुच्चयः &= ~TSK_ONCPU;
	} अन्यथा {
		state_आवरणम् = groupc->state_आवरणम् & PSI_ONCPU;
	}

	/*
	 * The rest of the state mask is calculated based on the task
	 * counts. Update those first, then construct the mask.
	 */
	प्रति (t = 0, m = clear; m; m &= ~(1 << t), t++) {
		यदि (!(m & (1 << t)))
			अनुवर्त;
		यदि (groupc->tasks[t]) {
			groupc->tasks[t]--;
		} अन्यथा यदि (!psi_bug) {
			printk_deferred(KERN_ERR "psi: task underflow! cpu=%d t=%d tasks=[%u %u %u %u] clear=%x set=%x\n",
					cpu, t, groupc->tasks[0],
					groupc->tasks[1], groupc->tasks[2],
					groupc->tasks[3], clear, समुच्चयः);
			psi_bug = 1;
		}
	}

	प्रति (t = 0; समुच्चयः; समुच्चयः &= ~(1 << t), t++)
		यदि (समुच्चयः & (1 << t))
			groupc->tasks[t]++;

	यदि (!गणः->enabled) {
		/*
		 * On the first group change after disabling PSI, conclude
		 * the current state and flush its time. This is unlikely
		 * to matter to the user, but aggregation (get_recent_times)
		 * may have already incorporated the live state into times_prev;
		 * avoid a delta sample underflow when PSI is later re-enabled.
		 */
		यदि (unlikely(groupc->state_आवरणम् & (1 << PSI_NONIDLE)))
			लेखः_times(groupc, now);

		groupc->state_आवरणम् = state_आवरणम्;

		प्रत्यागम;
	}

	state_आवरणम् = test_अवस्थाः(groupc->tasks, state_आवरणम्);

	/*
	 * Since we care about lost potential, a memstall is FULL
	 * when there are no other working tasks, but also when
	 * the CPU is actively reclaiming and nothing productive
	 * could run even if it were runnable. So when the current
	 * task in a cgroup is in_memstall, the corresponding groupc
	 * on that cpu is in PSI_MEM_FULL state.
	 */
	यदि (unlikely((state_आवरणम् & PSI_ONCPU) && cpu_curr(cpu)->अन्तर्_memstall))
		state_आवरणम् |= (1 << PSI_MEM_पूर्णम्);

	लेखः_times(groupc, now);

	groupc->state_आवरणम् = state_आवरणम्;

	यदि (state_आवरणम् & गणः->rtpoll_अवस्थाः)
		psi_कालसारणी_rtpoll_work(गणः, 1, false);

	यदि (wake_clock && !delayed_work_pending(&गणः->avgs_work))
		कालसारणी_delayed_work(&गणः->avgs_work, PSI_FREQ);
}

स्थिर अन्तःस्थ संरचना psi_गणः *task_psi_गणः(संरचना task_struct *task)
{
#यदि_परिभाषितम् CONFIG_CGROUPS
	यदि (static_branch_likely(&psi_cgroups_enabled))
		प्रत्यागम cgroup_psi(task_dfl_cgroup(task));
#यदि_अन्तः
	प्रत्यागम &psi_system;
}

स्थिर शून्य psi_flags_change(संरचना task_struct *task, पूर्णाङ्क clear, पूर्णाङ्क समुच्चयः)
{
	यदि (((task->psi_flags & समुच्चयः) ||
	     (task->psi_flags & clear) != clear) &&
	    !psi_bug) {
		printk_deferred(KERN_ERR "psi: inconsistent task state! task=%d:%s cpu=%d psi_flags=%x clear=%x set=%x\n",
				task->pid, task->comm, task_cpu(task),
				task->psi_flags, clear, समुच्चयः);
		psi_bug = 1;
	}

	task->psi_flags &= ~clear;
	task->psi_flags |= समुच्चयः;
}

शून्य psi_task_change(संरचना task_struct *task, पूर्णाङ्क clear, पूर्णाङ्क समुच्चयः)
{
	पूर्णाङ्क cpu = task_cpu(task);
	u64 now;

	यदि (!task->pid)
		प्रत्यागम;

	psi_flags_change(task, clear, समुच्चयः);

	psi_write_begin(cpu);
	now = cpu_clock(cpu);
	for_each_गणः(गणः, task_psi_गणः(task))
		psi_गणः_change(गणः, cpu, clear, समुच्चयः, now, true);
	psi_write_अन्तः(cpu);
}

शून्य psi_task_switch(संरचना task_struct *पूर्व, संरचना task_struct *अग्र,
		     bool sleep)
{
	संरचना psi_गणः *common = रिक्त;
	पूर्णाङ्क cpu = task_cpu(पूर्व);
	u64 now;

	psi_write_begin(cpu);
	now = cpu_clock(cpu);

	यदि (अग्र->pid) {
		psi_flags_change(अग्र, 0, TSK_ONCPU);
		/*
		 * Set TSK_ONCPU on @next's cgroups. If @next shares any
		 * ancestors with @prev, those will already have @prev's
		 * TSK_ONCPU bit set, and we can stop the iteration there.
		 */
		for_each_गणः(गणः, task_psi_गणः(अग्र)) {
			संरचना psi_गणः_cpu *groupc = प्रत्येकम्_cpu_ptr(गणः->pcpu, cpu);

			यदि (groupc->state_आवरणम् & PSI_ONCPU) {
				common = गणः;
				भङ्ग;
			}
			psi_गणः_change(गणः, cpu, 0, TSK_ONCPU, now, true);
		}
	}

	यदि (पूर्व->pid) {
		पूर्णाङ्क clear = TSK_ONCPU, समुच्चयः = 0;
		bool wake_clock = true;

		/*
		 * When we're going to sleep, psi_dequeue() lets us
		 * handle TSK_RUNNING, TSK_MEMSTALL_RUNNING and
		 * TSK_IOWAIT here, where we can combine it with
		 * TSK_ONCPU and save walking common ancestors twice.
		 */
		यदि (sleep) {
			clear |= TSK_RUNNING;
			यदि (पूर्व->अन्तर्_memstall)
				clear |= TSK_MEMSTALL_RUNNING;
			यदि (पूर्व->अन्तर्_iowait)
				समुच्चयः |= TSK_IOWAIT;

			/*
			 * Periodic aggregation shuts off if there is a period of no
			 * task changes, so we wake it back up if necessary. However,
			 * don't do this if the task change is the aggregation worker
			 * itself going to sleep, or we'll ping-pong forever.
			 */
			यदि (unlikely((पूर्व->flags & PF_WQ_कार्यकर्ता) &&
				     wq_कार्यकर्ता_चरमः_func(पूर्व) == psi_avgs_work))
				wake_clock = false;
		}

		psi_flags_change(पूर्व, clear, समुच्चयः);

		for_each_गणः(गणः, task_psi_गणः(पूर्व)) {
			यदि (गणः == common)
				भङ्ग;
			psi_गणः_change(गणः, cpu, clear, समुच्चयः, now, wake_clock);
		}

		/*
		 * TSK_ONCPU is handled up to the common ancestor. If there are
		 * any other differences between the two tasks (e.g. prev goes
		 * to sleep, or only one task is memstall), finish propagating
		 * those differences all the way up to the root.
		 */
		यदि ((पूर्व->psi_flags ^ अग्र->psi_flags) & ~TSK_ONCPU) {
			clear &= ~TSK_ONCPU;
			for_each_गणः(गणः, common)
				psi_गणः_change(गणः, cpu, clear, समुच्चयः, now, wake_clock);
		}
	}
	psi_write_अन्तः(cpu);
}

#यदि_परिभाषितम् CONFIG_IRQ_TIME_ACCOUNTING
शून्य psi_account_irqtime(संरचना rq *rq, संरचना task_struct *curr, संरचना task_struct *पूर्व)
{
	पूर्णाङ्क cpu = task_cpu(curr);
	संरचना psi_गणः_cpu *groupc;
	s64 delta;
	u64 irq;
	u64 now;

	यदि (static_branch_likely(&psi_disabled) || !irqtime_enabled())
		प्रत्यागम;

	यदि (!curr->pid)
		प्रत्यागम;

	lockdep_assert_rq_held(rq);
	यदि (पूर्व && task_psi_गणः(पूर्व) == task_psi_गणः(curr))
		प्रत्यागम;

	irq = irq_time_read(cpu);
	delta = (s64)(irq - rq->psi_irq_time);
	यदि (delta <= 0)
		प्रत्यागम;
	rq->psi_irq_time = irq;

	psi_write_begin(cpu);
	now = cpu_clock(cpu);

	for_each_गणः(गणः, task_psi_गणः(curr)) {
		यदि (!गणः->enabled)
			अनुवर्त;

		groupc = प्रत्येकम्_cpu_ptr(गणः->pcpu, cpu);

		लेखः_times(groupc, now);
		groupc->times[PSI_IRQ_पूर्णम्] += delta;

		यदि (गणः->rtpoll_अवस्थाः & (1 << PSI_IRQ_पूर्णम्))
			psi_कालसारणी_rtpoll_work(गणः, 1, false);
	}
	psi_write_अन्तः(cpu);
}
#यदि_अन्तः /* CONFIG_IRQ_TIME_ACCOUNTING */

/**
 * psi_memstall_enter - mark the beginning of a memory stall section
 * @flags: flags to handle nested sections
 *
 * Marks the calling task as being stalled due to a lack of memory,
 * such as waiting for a refault or performing reclaim.
 */
शून्य psi_memstall_enter(अचिह्नित दीर्घ *flags)
{
	संरचना rq_flags rf;
	संरचना rq *rq;

	यदि (static_branch_likely(&psi_disabled))
		प्रत्यागम;

	*flags = current->अन्तर्_memstall;
	यदि (*flags)
		प्रत्यागम;
	/*
	 * in_memstall setting & accounting needs to be atomic wrt
	 * changes to the task's scheduling state, otherwise we can
	 * race with CPU migration.
	 */
	rq = this_rq_तालकम्_irq(&rf);

	current->अन्तर्_memstall = 1;
	psi_task_change(current, 0, TSK_MEMSTALL | TSK_MEMSTALL_RUNNING);

	rq_unlock_irq(rq, &rf);
}
EXPORT_चिह्नम्_GPL(psi_memstall_enter);

/**
 * psi_memstall_leave - mark the end of an memory stall section
 * @flags: flags to handle nested memdelay sections
 *
 * Marks the calling task as no longer stalled due to lack of memory.
 */
शून्य psi_memstall_leave(अचिह्नित दीर्घ *flags)
{
	संरचना rq_flags rf;
	संरचना rq *rq;

	यदि (static_branch_likely(&psi_disabled))
		प्रत्यागम;

	यदि (*flags)
		प्रत्यागम;
	/*
	 * in_memstall clearing & accounting needs to be atomic wrt
	 * changes to the task's scheduling state, otherwise we could
	 * race with CPU migration.
	 */
	rq = this_rq_तालकम्_irq(&rf);

	current->अन्तर्_memstall = 0;
	psi_task_change(current, TSK_MEMSTALL | TSK_MEMSTALL_RUNNING, 0);

	rq_unlock_irq(rq, &rf);
}
EXPORT_चिह्नम्_GPL(psi_memstall_leave);

#यदि_परिभाषितम् CONFIG_CGROUPS
पूर्णाङ्क psi_cgroup_alloc(संरचना cgroup *cgroup)
{
	यदि (!static_branch_likely(&psi_cgroups_enabled))
		प्रत्यागम 0;

	cgroup->psi = kzalloc_obj(संरचना psi_गणः);
	यदि (!cgroup->psi)
		प्रत्यागम -ENOMEM;

	cgroup->psi->pcpu = alloc_percpu(संरचना psi_गणः_cpu);
	यदि (!cgroup->psi->pcpu) {
		kfree(cgroup->psi);
		प्रत्यागम -ENOMEM;
	}
	गणः_init(cgroup->psi);
	cgroup->psi->parent = cgroup_psi(cgroup_parent(cgroup));
	प्रत्यागम 0;
}

शून्य psi_cgroup_free(संरचना cgroup *cgroup)
{
	यदि (!static_branch_likely(&psi_cgroups_enabled))
		प्रत्यागम;

	cancel_delayed_work_sync(&cgroup->psi->avgs_work);
	/*
	 * A psi_schedule_rtpoll_work() call racing the last trigger's
	 * destruction may have re-armed the timer after psi_trigger_destroy()
	 * deleted it. Spurious firing while the group is alive is harmless.
	 */
	timer_shutdown_sync(&cgroup->psi->rtpoll_timer);
	free_percpu(cgroup->psi->pcpu);
	/* All triggers must be removed by now */
	WARN_ONCE(cgroup->psi->rtpoll_अवस्थाः, "psi: trigger leak\n");
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
शून्य cgroup_move_task(संरचना task_struct *task, संरचना css_समुच्चयः *गन्तव्यम्)
{
	अचिह्नित पूर्णाङ्क task_flags;
	संरचना rq_flags rf;
	संरचना rq *rq;

	यदि (!static_branch_likely(&psi_cgroups_enabled)) {
		/*
		 * Lame to do this here, but the scheduler cannot be locked
		 * from the outside, so we move cgroups from inside sched/.
		 */
		rcu_नियोजय_pointer(task->cgroups, गन्तव्यम्);
		प्रत्यागम;
	}

	rq = task_rq_तालकम्_2(task, &rf);

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

	यदि (task_flags)
		psi_task_change(task, task_flags, 0);

	/* See comment above */
	rcu_नियोजय_pointer(task->cgroups, गन्तव्यम्);

	यदि (task_flags)
		psi_task_change(task, 0, task_flags);

	task_rq_unlock(rq, task, &rf);
}

शून्य psi_cgroup_restart(संरचना psi_गणः *गणः)
{
	पूर्णाङ्क cpu;

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
	यदि (!गणः->enabled)
		प्रत्यागम;

	for_each_possible_cpu(cpu) {
		u64 now;

		guard(rq_तालकम्_irq)(cpu_rq(cpu));

		psi_write_begin(cpu);
		now = cpu_clock(cpu);
		psi_गणः_change(गणः, cpu, 0, 0, now, true);
		psi_write_अन्तः(cpu);
	}
}
#यदि_अन्तः /* CONFIG_CGROUPS */

पूर्णाङ्क psi_show(संरचना seq_file *m, संरचना psi_गणः *गणः, गणना psi_res res)
{
	bool only_पूर्णम् = false;
	पूर्णाङ्क पूर्णम्;
	u64 now;

	यदि (static_branch_likely(&psi_disabled))
		प्रत्यागम -EOPNOTSUPP;

#यदि_परिभाषितम् CONFIG_IRQ_TIME_ACCOUNTING
	यदि (!irqtime_enabled() && res == PSI_IRQ)
		प्रत्यागम -EOPNOTSUPP;
#यदि_अन्तः

	/* Update averages before reporting them */
	mutex_तालकम्(&गणः->avgs_तालकम्);
	now = sched_clock();
	collect_percpu_times(गणः, PSI_AVGS, रिक्त);
	यदि (now >= गणः->avg_अग्र_update)
		गणः->avg_अग्र_update = update_averages(गणः, now);
	mutex_unlock(&गणः->avgs_तालकम्);

#यदि_परिभाषितम् CONFIG_IRQ_TIME_ACCOUNTING
	only_पूर्णम् = res == PSI_IRQ;
#यदि_अन्तः

	प्रति (पूर्णम् = 0; पूर्णम् < 2 - only_पूर्णम्; पूर्णम्++) {
		अचिह्नित दीर्घ avg[3] = { 0, };
		u64 योग_2 = 0;
		पूर्णाङ्क w;

		/* CPU FULL is undefined at the system level */
		यदि (!(गणः == &psi_system && res == PSI_CPU && पूर्णम्)) {
			प्रति (w = 0; w < 3; w++)
				avg[w] = गणः->avg[res * 2 + पूर्णम्][w];
			योग_2 = div_u64(गणः->योग_2[PSI_AVGS][res * 2 + पूर्णम्],
					NSEC_प्रत्येकम्_USEC);
		}

		seq_printf(m, "%s avg10=%lu.%02lu avg60=%lu.%02lu avg300=%lu.%02lu total=%llu\n",
			   पूर्णम् || only_पूर्णम् ? "full" : "some",
			   आरोपय_INT(avg[0]), आरोपय_FRAC(avg[0]),
			   आरोपय_INT(avg[1]), आरोपय_FRAC(avg[1]),
			   आरोपय_INT(avg[2]), आरोपय_FRAC(avg[2]),
			   योग_2);
	}

	प्रत्यागम 0;
}

/*
 * Create @group's rtpoll worker after psi_trigger_create() reported the need
 * for one. kthread creation depends on the whole fork path and we don't want
 * all of that nested inside cgroup_mutex, so the caller must drop it and any
 * other lock that forks can wait behind. If two callers race, the loser stops
 * its never-woken kthread.
 */
पूर्णाङ्क psi_trigger_create_rtpoll_कार्यकर्ता(संरचना psi_गणः *गणः)
{
	संरचना task_struct *task;

	task = kthread_create(psi_rtpoll_कार्यकर्ता, गणः, "psimon");
	यदि (IS_ERR(task))
		प्रत्यागम PTR_ERR(task);

	scoped_guard(mutex, &गणः->rtpoll_trigger_तालकम्) {
		यदि (!rcu_access_pointer(गणः->rtpoll_task)) {
			atomic_समुच्चयः(&गणः->rtpoll_wakeup, 0);
			wake_ऊर्ध्वम्_process(task);
			rcu_नियोजय_pointer(गणः->rtpoll_task, task);

			/*
			 * Poll once to catch up on scheduling attempts dropped
			 * while there was no rtpoll worker.
			 */
			psi_कालसारणी_rtpoll_work(गणः, 1, true);
			प्रत्यागम 0;
		}
	}

	kthread_stop(task);
	प्रत्यागम 0;
}

संरचना psi_trigger *psi_trigger_create(संरचना psi_गणः *गणः, वर्ण *buf,
				       गणना psi_res res, संरचना file *file,
				       संरचना kernfs_open_file *सम्बन्धि,
				       bool *need_rtpoll_कार्यकर्ता)
{
	संरचना psi_trigger *t;
	गणना psi_अवस्थाः state;
	u32 देहली_us;
	bool privileged;
	u32 गवाक्षः_us;

	*need_rtpoll_कार्यकर्ता = false;

	यदि (static_branch_likely(&psi_disabled))
		प्रत्यागम ERR_PTR(-EOPNOTSUPP);

	/*
	 * Checking the privilege here on file->f_cred implies that a privileged user
	 * could open the file and delegate the write to an unprivileged one.
	 */
	privileged = cap_raised(file->f_cred->cap_effective, CAP_SYS_RESOURCE);

	यदि (वाक्यात्_पठ(buf, "some %u %u", &देहली_us, &गवाक्षः_us) == 2)
		state = PSI_IO_SOME + res * 2;
	अन्यथा यदि (वाक्यात्_पठ(buf, "full %u %u", &देहली_us, &गवाक्षः_us) == 2)
		state = PSI_IO_पूर्णम् + res * 2;
	अन्यथा
		प्रत्यागम ERR_PTR(-EINVAL);

#यदि_परिभाषितम् CONFIG_IRQ_TIME_ACCOUNTING
	यदि (res == PSI_IRQ && --state != PSI_IRQ_पूर्णम्)
		प्रत्यागम ERR_PTR(-EINVAL);
#यदि_अन्तः

	यदि (state >= PSI_NONIDLE)
		प्रत्यागम ERR_PTR(-EINVAL);

	यदि (गवाक्षः_us == 0 || गवाक्षः_us > गवाक्षः_MAX_US)
		प्रत्यागम ERR_PTR(-EINVAL);

	/*
	 * Unprivileged users can only use 2s windows so that averages aggregation
	 * work is used, and no RT threads need to be spawned.
	 */
	यदि (!privileged && गवाक्षः_us % 2000000)
		प्रत्यागम ERR_PTR(-EINVAL);

	/* Check threshold */
	यदि (देहली_us == 0 || देहली_us > गवाक्षः_us)
		प्रत्यागम ERR_PTR(-EINVAL);

	t = kmalloc_obj(*t);
	यदि (!t)
		प्रत्यागम ERR_PTR(-ENOMEM);

	t->गणः = गणः;
	t->state = state;
	t->देहली = देहली_us * NSEC_प्रत्येकम्_USEC;
	t->win.आकार = गवाक्षः_us * NSEC_प्रत्येकम्_USEC;
	गवाक्षः_reset(&t->win, sched_clock(),
			गणः->योग_2[PSI_POLL][t->state], 0);

	t->event = 0;
	t->चरमः_event_time = 0;
	t->सम्बन्धि = सम्बन्धि;
	यदि (!सम्बन्धि)
		init_waitqueue_शीर्ष(&t->event_wait);
	t->pending_event = false;
	t->aggregator = privileged ? PSI_POLL : PSI_AVGS;

	यदि (privileged) {
		mutex_तालकम्(&गणः->rtpoll_trigger_तालकम्);

		list_सङ्कलय(&t->ग्रन्थिः, &गणः->rtpoll_triggers);
		गणः->rtpoll_min_period = min(गणः->rtpoll_min_period,
			div_u64(t->win.आकार, UPDATES_प्रत्येकम्_गवाक्षः));
		गणः->rtpoll_nr_triggers[t->state]++;
		गणः->rtpoll_अवस्थाः |= (1 << t->state);

		*need_rtpoll_कार्यकर्ता = !rcu_access_pointer(गणः->rtpoll_task);

		mutex_unlock(&गणः->rtpoll_trigger_तालकम्);
	} अन्यथा {
		mutex_तालकम्(&गणः->avgs_तालकम्);

		list_सङ्कलय(&t->ग्रन्थिः, &गणः->avg_triggers);
		गणः->avg_nr_triggers[t->state]++;

		mutex_unlock(&गणः->avgs_तालकम्);
	}
	प्रत्यागम t;
}

शून्य psi_trigger_destroy(संरचना psi_trigger *t)
{
	संरचना psi_गणः *गणः;
	संरचना task_struct *task_गन्तव्यम्_destroy = रिक्त;

	/*
	 * We do not check psi_disabled since it might have been disabled after
	 * the trigger got created.
	 */
	यदि (!t)
		प्रत्यागम;

	गणः = t->गणः;
	/*
	 * Wakeup waiters to stop polling and clear the queue to prevent it from
	 * being accessed later. Can happen if cgroup is deleted from under a
	 * polling process.
	 */
	यदि (t->सम्बन्धि)
		kernfs_notify(t->सम्बन्धि->kn);
	अन्यथा
		wake_ऊर्ध्वम्_interruptible(&t->event_wait);

	यदि (t->aggregator == PSI_AVGS) {
		mutex_तालकम्(&गणः->avgs_तालकम्);
		यदि (!list_रिक्तानि(&t->ग्रन्थिः)) {
			list_del(&t->ग्रन्थिः);
			गणः->avg_nr_triggers[t->state]--;
		}
		mutex_unlock(&गणः->avgs_तालकम्);
	} अन्यथा {
		mutex_तालकम्(&गणः->rtpoll_trigger_तालकम्);
		यदि (!list_रिक्तानि(&t->ग्रन्थिः)) {
			संरचना psi_trigger *tmp;
			u64 period = ULLONG_MAX;

			list_del(&t->ग्रन्थिः);
			गणः->rtpoll_nr_triggers[t->state]--;
			यदि (!गणः->rtpoll_nr_triggers[t->state])
				गणः->rtpoll_अवस्थाः &= ~(1 << t->state);
			/*
			 * Reset min update period for the remaining triggers
			 * iff the destroying trigger had the min window size.
			 */
			यदि (गणः->rtpoll_min_period == div_u64(t->win.आकार, UPDATES_प्रत्येकम्_गवाक्षः)) {
				list_for_each_प्रविष्टिः(tmp, &गणः->rtpoll_triggers, ग्रन्थिः)
					period = min(period, div_u64(tmp->win.आकार,
							UPDATES_प्रत्येकम्_गवाक्षः));
				गणः->rtpoll_min_period = period;
			}
			/* Destroy rtpoll_task when the last trigger is destroyed */
			यदि (गणः->rtpoll_अवस्थाः == 0) {
				गणः->rtpoll_until = 0;
				task_गन्तव्यम्_destroy = rcu_dereference_protected(
						गणः->rtpoll_task,
						lockdep_is_held(&गणः->rtpoll_trigger_तालकम्));
				rcu_नियोजय_pointer(गणः->rtpoll_task, रिक्त);
				timer_delete(&गणः->rtpoll_timer);
			}
		}
		mutex_unlock(&गणः->rtpoll_trigger_तालकम्);
	}

	/*
	 * Stop kthread 'psimon' after releasing rtpoll_trigger_lock to prevent
	 * a deadlock while waiting for psi_rtpoll_work to acquire
	 * rtpoll_trigger_lock
	 */
	यदि (task_गन्तव्यम्_destroy) {
		kthread_stop(task_गन्तव्यम्_destroy);
		atomic_समुच्चयः(&गणः->rtpoll_scheduled, 0);
	}
	kfree(t);
}

__poll_t psi_trigger_poll(शून्य **trigger_ptr,
				संरचना file *file, poll_सारणी *प्रतीक्षस्व)
{
	__poll_t ret = DEFAULT_POLLMASK;
	संरचना psi_trigger *t;

	यदि (static_branch_likely(&psi_disabled))
		प्रत्यागम DEFAULT_POLLMASK | EPOLLERR | EPOLLPRI;

	t = smp_आरोपय_acquire(trigger_ptr);
	यदि (!t)
		प्रत्यागम DEFAULT_POLLMASK | EPOLLERR | EPOLLPRI;

	यदि (t->सम्बन्धि)
		kernfs_generic_poll(t->सम्बन्धि, प्रतीक्षस्व);
	अन्यथा
		poll_wait(file, &t->event_wait, प्रतीक्षस्व);

	यदि (cmpxchg(&t->event, 1, 0) == 1)
		ret |= EPOLLPRI;

	प्रत्यागम ret;
}

#यदि_परिभाषितम् CONFIG_PROC_FS
स्थिर पूर्णाङ्क psi_io_show(संरचना seq_file *m, शून्य *v)
{
	प्रत्यागम psi_show(m, &psi_system, PSI_IO);
}

स्थिर पूर्णाङ्क psi_स्मृतिः_show(संरचना seq_file *m, शून्य *v)
{
	प्रत्यागम psi_show(m, &psi_system, PSI_MEM);
}

स्थिर पूर्णाङ्क psi_cpu_show(संरचना seq_file *m, शून्य *v)
{
	प्रत्यागम psi_show(m, &psi_system, PSI_CPU);
}

स्थिर पूर्णाङ्क psi_io_open(संरचना inode *inode, संरचना file *file)
{
	प्रत्यागम single_open(file, psi_io_show, रिक्त);
}

स्थिर पूर्णाङ्क psi_स्मृतिः_open(संरचना inode *inode, संरचना file *file)
{
	प्रत्यागम single_open(file, psi_स्मृतिः_show, रिक्त);
}

स्थिर पूर्णाङ्क psi_cpu_open(संरचना inode *inode, संरचना file *file)
{
	प्रत्यागम single_open(file, psi_cpu_show, रिक्त);
}

स्थिर ssize_t psi_write(संरचना file *file, ध्रुव वर्ण __user *user_buf,
			 आकार_t nbytes, गणना psi_res res)
{
	वर्ण buf[32];
	आकार_t buf_आकार;
	संरचना seq_file *seq;
	संरचना psi_trigger *new;
	bool need_rtpoll_कार्यकर्ता;
	पूर्णाङ्क ret;

	यदि (static_branch_likely(&psi_disabled))
		प्रत्यागम -EOPNOTSUPP;

	यदि (!nbytes)
		प्रत्यागम -EINVAL;

	buf_आकार = min(nbytes, परिमाण(buf));
	यदि (copy_from_user(buf, user_buf, buf_आकार))
		प्रत्यागम -EFAULT;

	buf[buf_आकार - 1] = '\0';

	seq = file->वैयक्तिकम्_दत्त;

	/* Take seq->lock to protect seq->private from concurrent writes */
	mutex_तालकम्(&seq->तालकम्);

	/* Allow only one trigger per file descriptor */
	यदि (seq->गुप्त) {
		mutex_unlock(&seq->तालकम्);
		प्रत्यागम -EBUSY;
	}

	new = psi_trigger_create(&psi_system, buf, res, file, रिक्त,
				 &need_rtpoll_कार्यकर्ता);
	यदि (IS_ERR(new)) {
		mutex_unlock(&seq->तालकम्);
		प्रत्यागम PTR_ERR(new);
	}

	यदि (need_rtpoll_कार्यकर्ता) {
		ret = psi_trigger_create_rtpoll_कार्यकर्ता(&psi_system);
		यदि (ret) {
			psi_trigger_destroy(new);
			mutex_unlock(&seq->तालकम्);
			प्रत्यागम ret;
		}
	}

	smp_store_release(&seq->गुप्त, new);
	mutex_unlock(&seq->तालकम्);

	प्रत्यागम nbytes;
}

स्थिर ssize_t psi_io_write(संरचना file *file, ध्रुव वर्ण __user *user_buf,
			    आकार_t nbytes, loff_t *ppos)
{
	प्रत्यागम psi_write(file, user_buf, nbytes, PSI_IO);
}

स्थिर ssize_t psi_स्मृतिः_write(संरचना file *file, ध्रुव वर्ण __user *user_buf,
				आकार_t nbytes, loff_t *ppos)
{
	प्रत्यागम psi_write(file, user_buf, nbytes, PSI_MEM);
}

स्थिर ssize_t psi_cpu_write(संरचना file *file, ध्रुव वर्ण __user *user_buf,
			     आकार_t nbytes, loff_t *ppos)
{
	प्रत्यागम psi_write(file, user_buf, nbytes, PSI_CPU);
}

स्थिर __poll_t psi_fop_poll(संरचना file *file, poll_सारणी *प्रतीक्षस्व)
{
	संरचना seq_file *seq = file->वैयक्तिकम्_दत्त;

	प्रत्यागम psi_trigger_poll(&seq->गुप्त, file, प्रतीक्षस्व);
}

स्थिर पूर्णाङ्क psi_fop_release(संरचना inode *inode, संरचना file *file)
{
	संरचना seq_file *seq = file->वैयक्तिकम्_दत्त;

	psi_trigger_destroy(seq->गुप्त);
	प्रत्यागम single_release(inode, file);
}

स्थिर ध्रुव संरचना proc_ops psi_io_proc_ops = {
	.proc_open	= psi_io_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_io_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

स्थिर ध्रुव संरचना proc_ops psi_स्मृतिः_proc_ops = {
	.proc_open	= psi_स्मृतिः_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_स्मृतिः_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

स्थिर ध्रुव संरचना proc_ops psi_cpu_proc_ops = {
	.proc_open	= psi_cpu_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_cpu_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

#यदि_परिभाषितम् CONFIG_IRQ_TIME_ACCOUNTING
स्थिर पूर्णाङ्क psi_irq_show(संरचना seq_file *m, शून्य *v)
{
	प्रत्यागम psi_show(m, &psi_system, PSI_IRQ);
}

स्थिर पूर्णाङ्क psi_irq_open(संरचना inode *inode, संरचना file *file)
{
	प्रत्यागम single_open(file, psi_irq_show, रिक्त);
}

स्थिर ssize_t psi_irq_write(संरचना file *file, ध्रुव वर्ण __user *user_buf,
			     आकार_t nbytes, loff_t *ppos)
{
	प्रत्यागम psi_write(file, user_buf, nbytes, PSI_IRQ);
}

स्थिर ध्रुव संरचना proc_ops psi_irq_proc_ops = {
	.proc_open	= psi_irq_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_irq_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};
#यदि_अन्तः /* CONFIG_IRQ_TIME_ACCOUNTING */

स्थिर पूर्णाङ्क __init psi_proc_init(शून्य)
{
	यदि (psi_enable) {
		proc_mkdir("pressure", रिक्त);
		proc_create("pressure/io", 0666, रिक्त, &psi_io_proc_ops);
		proc_create("pressure/memory", 0666, रिक्त, &psi_स्मृतिः_proc_ops);
		proc_create("pressure/cpu", 0666, रिक्त, &psi_cpu_proc_ops);
#यदि_परिभाषितम् CONFIG_IRQ_TIME_ACCOUNTING
		proc_create("pressure/irq", 0666, रिक्त, &psi_irq_proc_ops);
#यदि_अन्तः
	}
	प्रत्यागम 0;
}
module_init(psi_proc_init);

#यदि_अन्तः /* CONFIG_PROC_FS */
