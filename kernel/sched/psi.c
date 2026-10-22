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
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/clock.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/workqueue.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/psi.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"

ਸਥਿਰ ਪੂਰਨ psi_bug __read_mostly;

DEFINE_STATIC_ਕੁੰਜੀ_FALSE(psi_disabled);
ਸਥਿਰ DEFINE_STATIC_ਕੁੰਜੀ_TRUE(psi_cgroups_enabled);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PSI_DEFAULT_DISABLED
ਸਥਿਰ bool psi_enable;
#ਨਹੀਂਤਾਂ
ਸਥਿਰ bool psi_enable = true;
#ਜੇ_ਅੰਤ
ਸਥਿਰ ਪੂਰਨ __init ਤਿਆਰੀ_psi(ਅੱਖਰ *str)
{
	ਵਾਪਸ kstrtobool(str, &psi_enable) == 0;
}
__setup("psi=", ਤਿਆਰੀ_psi);

/* Running averages - we need to be higher-res than loadavg */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PSI_FREQ	(2*HZ+1)	/* 2 sec intervals */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXP_10s		1677		/* 1/exp(2s/10s) as fixed-point */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXP_60s		1981		/* 1/exp(2s/60s) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXP_300s	2034		/* 1/exp(2s/300s) */

/* PSI trigger definitions */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਖਿੜਕੀ_MAX_US 10000000	/* Max window size is 10s */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UPDATES_ਪ੍ਰਤੀ_ਖਿੜਕੀ 10	/* 10 updates per window */

/* Sampling frequency in nanoseconds */
ਸਥਿਰ u64 psi_period __read_mostly;

/* System-level pressure and stall tracking */
ਸਥਿਰ DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ psi_ਸਮੂਹ_cpu, system_ਸਮੂਹ_pcpu);
ਬਣਤਰ psi_ਸਮੂਹ psi_system = {
	.pcpu = &system_ਸਮੂਹ_pcpu,
};

ਸਥਿਰ DEFINE_ਪ੍ਰਤੀ_CPU(seqcount_t, psi_seq) = SEQCNT_ZERO(psi_seq);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ psi_write_begin(ਪੂਰਨ cpu)
{
	write_seqcount_begin(ਪ੍ਰਤੀ_cpu_ptr(&psi_seq, cpu));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ psi_write_ਸਿਰਾ(ਪੂਰਨ cpu)
{
	write_seqcount_ਸਿਰਾ(ਪ੍ਰਤੀ_cpu_ptr(&psi_seq, cpu));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ u32 psi_read_begin(ਪੂਰਨ cpu)
{
	ਵਾਪਸ read_seqcount_begin(ਪ੍ਰਤੀ_cpu_ptr(&psi_seq, cpu));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool psi_read_retry(ਪੂਰਨ cpu, u32 seq)
{
	ਵਾਪਸ read_seqcount_retry(ਪ੍ਰਤੀ_cpu_ptr(&psi_seq, cpu), seq);
}

ਸਥਿਰ ਖਾਲੀ psi_avgs_work(ਬਣਤਰ work_struct *work);

ਸਥਿਰ ਖਾਲੀ poll_timer_fn(ਬਣਤਰ timer_list *t);

ਸਥਿਰ ਖਾਲੀ ਸਮੂਹ_init(ਬਣਤਰ psi_ਸਮੂਹ *ਸਮੂਹ)
{
	ਸਮੂਹ->enabled = true;
	ਸਮੂਹ->avg_ਆਖਰੀ_update = sched_clock();
	ਸਮੂਹ->avg_ਅਗਲਾ_update = ਸਮੂਹ->avg_ਆਖਰੀ_update + psi_period;
	mutex_init(&ਸਮੂਹ->avgs_ਤਾਲਾ);

	/* Init avg trigger-related members */
	INIT_LIST_ਸਿਰ(&ਸਮੂਹ->avg_triggers);
	memset(ਸਮੂਹ->avg_nr_triggers, 0, ਆਕਾਰ(ਸਮੂਹ->avg_nr_triggers));
	INIT_DELAYED_WORK(&ਸਮੂਹ->avgs_work, psi_avgs_work);

	/* Init rtpoll trigger-related members */
	atomic_ਸੈੱਟ(&ਸਮੂਹ->rtpoll_scheduled, 0);
	mutex_init(&ਸਮੂਹ->rtpoll_trigger_ਤਾਲਾ);
	INIT_LIST_ਸਿਰ(&ਸਮੂਹ->rtpoll_triggers);
	ਸਮੂਹ->rtpoll_min_period = U32_MAX;
	ਸਮੂਹ->rtpoll_ਅਗਲਾ_update = ULLONG_MAX;
	init_waitqueue_ਸਿਰ(&ਸਮੂਹ->rtpoll_wait);
	timer_ਤਿਆਰੀ(&ਸਮੂਹ->rtpoll_timer, poll_timer_fn, 0);
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(ਸਮੂਹ->rtpoll_task, ਨਿਰਾ);
}

ਖਾਲੀ __init psi_init(ਖਾਲੀ)
{
	ਜੇ (!psi_enable) {
		static_branch_enable(&psi_disabled);
		static_branch_disable(&psi_cgroups_enabled);
		ਵਾਪਸ;
	}

	ਜੇ (!cgroup_psi_enabled())
		static_branch_disable(&psi_cgroups_enabled);

	psi_period = jiffies_ਮੰਜ਼ਿਲ_nsecs(PSI_FREQ);
	ਸਮੂਹ_init(&psi_system);
}

ਸਥਿਰ u32 test_ਹਾਲਤਾਂ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *tasks, u32 state_ਨਕਾਬ)
{
	ਅਚਲ bool oncpu = state_ਨਕਾਬ & PSI_ONCPU;

	ਜੇ (tasks[NR_IOWAIT]) {
		state_ਨਕਾਬ |= ਦੋਆਧਾਰੀ_ਅੰਕ(PSI_IO_SOME);
		ਜੇ (!tasks[NR_RUNNING])
			state_ਨਕਾਬ |= ਦੋਆਧਾਰੀ_ਅੰਕ(PSI_IO_ਭਰਿਆ);
	}

	ਜੇ (tasks[NR_MEMSTALL]) {
		state_ਨਕਾਬ |= ਦੋਆਧਾਰੀ_ਅੰਕ(PSI_MEM_SOME);
		ਜੇ (tasks[NR_RUNNING] == tasks[NR_MEMSTALL_RUNNING])
			state_ਨਕਾਬ |= ਦੋਆਧਾਰੀ_ਅੰਕ(PSI_MEM_ਭਰਿਆ);
	}

	ਜੇ (tasks[NR_RUNNING] > oncpu)
		state_ਨਕਾਬ |= ਦੋਆਧਾਰੀ_ਅੰਕ(PSI_CPU_SOME);

	ਜੇ (tasks[NR_RUNNING] && !oncpu)
		state_ਨਕਾਬ |= ਦੋਆਧਾਰੀ_ਅੰਕ(PSI_CPU_ਭਰਿਆ);

	ਜੇ (tasks[NR_IOWAIT] || tasks[NR_MEMSTALL] || tasks[NR_RUNNING])
		state_ਨਕਾਬ |= ਦੋਆਧਾਰੀ_ਅੰਕ(PSI_NONIDLE);

	ਵਾਪਸ state_ਨਕਾਬ;
}

ਸਥਿਰ ਖਾਲੀ get_recent_times(ਬਣਤਰ psi_ਸਮੂਹ *ਸਮੂਹ, ਪੂਰਨ cpu,
			     ਗਿਣਤੀ psi_aggregators aggregator, u32 *times,
			     u32 *pchanged_ਹਾਲਤਾਂ)
{
	ਬਣਤਰ psi_ਸਮੂਹ_cpu *groupc = ਪ੍ਰਤੀ_cpu_ptr(ਸਮੂਹ->pcpu, cpu);
	ਪੂਰਨ current_cpu = raw_smp_processor_id();
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ tasks[NR_PSI_TASK_COUNTS];
	u64 now, state_ਸ਼ੁਰੂ;
	ਗਿਣਤੀ psi_ਹਾਲਤਾਂ s;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ seq;
	u32 state_ਨਕਾਬ;

	*pchanged_ਹਾਲਤਾਂ = 0;

	/* Snapshot a coherent view of the CPU state */
	ਕਰੋ {
		seq = psi_read_begin(cpu);
		now = cpu_clock(cpu);
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(times, groupc->times, ਆਕਾਰ(groupc->times));
		state_ਨਕਾਬ = groupc->state_ਨਕਾਬ;
		state_ਸ਼ੁਰੂ = groupc->state_ਸ਼ੁਰੂ;
		ਜੇ (cpu == current_cpu)
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(tasks, groupc->tasks, ਆਕਾਰ(groupc->tasks));
	} ਜਦੋਂ (psi_read_retry(cpu, seq));

	/* Calculate state time deltas against the previous snapshot */
	ਲਈ (s = 0; s < NR_PSI_ਹਾਲਤਾਂ; s++) {
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
		ਜੇ (state_ਨਕਾਬ & (1 << s))
			times[s] += now - state_ਸ਼ੁਰੂ;

		delta = times[s] - groupc->times_prev[aggregator][s];
		groupc->times_prev[aggregator][s] = times[s];

		times[s] = delta;
		ਜੇ (delta)
			*pchanged_ਹਾਲਤਾਂ |= (1 << s);
	}

	/*
	 * When collect_percpu_times() from the avgs_work, we don't want to
	 * re-arm avgs_work when all CPUs are IDLE. But the current CPU running
	 * this avgs_work is never IDLE, cause avgs_work can't be shut off.
	 * So for the current CPU, we need to re-arm avgs_work only when
	 * (NR_RUNNING > 1 || NR_IOWAIT > 0 || NR_MEMSTALL > 0), for other CPUs
	 * we can just check PSI_NONIDLE delta.
	 */
	ਜੇ (current_work() == &ਸਮੂਹ->avgs_work.work) {
		bool reschedule;

		ਜੇ (cpu == current_cpu)
			reschedule = tasks[NR_RUNNING] +
				     tasks[NR_IOWAIT] +
				     tasks[NR_MEMSTALL] > 1;
		ਨਹੀਂਤਾਂ
			reschedule = *pchanged_ਹਾਲਤਾਂ & (1 << PSI_NONIDLE);

		ਜੇ (reschedule)
			*pchanged_ਹਾਲਤਾਂ |= PSI_STATE_RESCHEDULE;
	}
}

ਸਥਿਰ ਖਾਲੀ calc_avgs(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ avg[3], ਪੂਰਨ missed_periods,
		      u64 ਸਮਾਂ, u64 period)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ pct;

	/* Fill in zeroes for periods of no activity */
	ਜੇ (missed_periods) {
		avg[0] = calc_ਲੱਦੋ_n(avg[0], EXP_10s, 0, missed_periods);
		avg[1] = calc_ਲੱਦੋ_n(avg[1], EXP_60s, 0, missed_periods);
		avg[2] = calc_ਲੱਦੋ_n(avg[2], EXP_300s, 0, missed_periods);
	}

	/* Sample the most recent active period */
	pct = div_u64(ਸਮਾਂ * 100, period);
	pct *= FIXED_1;
	avg[0] = calc_ਲੱਦੋ(avg[0], EXP_10s, pct);
	avg[1] = calc_ਲੱਦੋ(avg[1], EXP_60s, pct);
	avg[2] = calc_ਲੱਦੋ(avg[2], EXP_300s, pct);
}

ਸਥਿਰ ਖਾਲੀ collect_percpu_times(ਬਣਤਰ psi_ਸਮੂਹ *ਸਮੂਹ,
				 ਗਿਣਤੀ psi_aggregators aggregator,
				 u32 *pchanged_ਹਾਲਤਾਂ)
{
	u64 deltas[NR_PSI_ਹਾਲਤਾਂ - 1] = { 0, };
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ nonidle_ਕੁੱਲ = 0;
	u32 ਬਦਲਿਆ_ਹਾਲਤਾਂ = 0;
	ਪੂਰਨ cpu;
	ਪੂਰਨ s;

	/*
	 * Collect the per-cpu time buckets and average them into a
	 * single time sample that is normalized to wall clock time.
	 *
	 * For averaging, each CPU is weighted by its non-idle time in
	 * the sampling period. This eliminates artifacts from uneven
	 * loading, or even entirely idle CPUs.
	 */
	for_each_possible_cpu(cpu) {
		u32 times[NR_PSI_ਹਾਲਤਾਂ];
		u32 nonidle;
		u32 cpu_ਬਦਲਿਆ_ਹਾਲਤਾਂ;

		get_recent_times(ਸਮੂਹ, cpu, aggregator, times,
				&cpu_ਬਦਲਿਆ_ਹਾਲਤਾਂ);
		ਬਦਲਿਆ_ਹਾਲਤਾਂ |= cpu_ਬਦਲਿਆ_ਹਾਲਤਾਂ;

		nonidle = nsecs_ਮੰਜ਼ਿਲ_jiffies(times[PSI_NONIDLE]);
		nonidle_ਕੁੱਲ += nonidle;

		ਲਈ (s = 0; s < PSI_NONIDLE; s++)
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
	ਲਈ (s = 0; s < NR_PSI_ਹਾਲਤਾਂ - 1; s++)
		ਸਮੂਹ->ਕੁੱਲ[aggregator][s] +=
				div_u64(deltas[s], max(nonidle_ਕੁੱਲ, 1UL));

	ਜੇ (pchanged_ਹਾਲਤਾਂ)
		*pchanged_ਹਾਲਤਾਂ = ਬਦਲਿਆ_ਹਾਲਤਾਂ;
}

/* Trigger tracking window manipulations */
ਸਥਿਰ ਖਾਲੀ ਖਿੜਕੀ_reset(ਬਣਤਰ psi_ਖਿੜਕੀ *win, u64 now, u64 ਮੁੱਲ,
			 u64 prev_growth)
{
	win->ਸ਼ੁਰੂ_ਸਮਾਂ = now;
	win->ਸ਼ੁਰੂ_ਮੁੱਲ = ਮੁੱਲ;
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
ਸਥਿਰ u64 ਖਿੜਕੀ_update(ਬਣਤਰ psi_ਖਿੜਕੀ *win, u64 now, u64 ਮੁੱਲ)
{
	u64 elapsed;
	u64 growth;

	elapsed = now - win->ਸ਼ੁਰੂ_ਸਮਾਂ;
	growth = ਮੁੱਲ - win->ਸ਼ੁਰੂ_ਮੁੱਲ;
	/*
	 * After each tracking window passes win->start_value and
	 * win->start_time get reset and win->prev_growth stores
	 * the average per-window growth of the previous window.
	 * win->prev_growth is then used to interpolate additional
	 * growth from the previous window assuming it was linear.
	 */
	ਜੇ (elapsed > win->ਮਾਪ)
		ਖਿੜਕੀ_reset(win, now, ਮੁੱਲ, growth);
	ਨਹੀਂਤਾਂ {
		u32 ਬਚਿਆ;

		ਬਚਿਆ = win->ਮਾਪ - elapsed;
		growth += div64_u64(win->prev_growth * ਬਚਿਆ, win->ਮਾਪ);
	}

	ਵਾਪਸ growth;
}

ਸਥਿਰ ਖਾਲੀ update_triggers(ਬਣਤਰ psi_ਸਮੂਹ *ਸਮੂਹ, u64 now,
						   ਗਿਣਤੀ psi_aggregators aggregator)
{
	ਬਣਤਰ psi_trigger *t;
	u64 *ਕੁੱਲ = ਸਮੂਹ->ਕੁੱਲ[aggregator];
	ਬਣਤਰ list_ਸਿਰ *triggers;
	u64 *aggregator_ਕੁੱਲ;

	ਜੇ (aggregator == PSI_AVGS) {
		triggers = &ਸਮੂਹ->avg_triggers;
		aggregator_ਕੁੱਲ = ਸਮੂਹ->avg_ਕੁੱਲ;
	} ਨਹੀਂਤਾਂ {
		triggers = &ਸਮੂਹ->rtpoll_triggers;
		aggregator_ਕੁੱਲ = ਸਮੂਹ->rtpoll_ਕੁੱਲ;
	}

	/*
	 * On subsequent updates, calculate growth deltas and let
	 * watchers know when their specified thresholds are exceeded.
	 */
	list_for_each_ਇੰਦਰਾਜ(t, triggers, ਗੰਢ) {
		u64 growth;
		bool new_stall;

		new_stall = aggregator_ਕੁੱਲ[t->state] != ਕੁੱਲ[t->state];

		/* Check for stall activity or a previous threshold breach */
		ਜੇ (!new_stall && !t->pending_event)
			ਜਾਰੀ;
		/*
		 * Check for new stall activity, as well as deferred
		 * events that occurred in the last window after the
		 * trigger had already fired (we want to ratelimit
		 * events without dropping any).
		 */
		ਜੇ (new_stall) {
			/* Calculate growth since last update */
			growth = ਖਿੜਕੀ_update(&t->win, now, ਕੁੱਲ[t->state]);
			ਜੇ (!t->pending_event) {
				ਜੇ (growth < t->ਹੱਦ)
					ਜਾਰੀ;

				t->pending_event = true;
			}
		}
		/* Limit event signaling to once per window */
		ਜੇ (now < t->ਆਖਰੀ_event_ਸਮਾਂ + t->win.ਮਾਪ)
			ਜਾਰੀ;

		/* Generate an event */
		ਜੇ (cmpxchg(&t->event, 0, 1) == 0) {
			ਜੇ (t->ਦਾ)
				kernfs_notify(t->ਦਾ->kn);
			ਨਹੀਂਤਾਂ
				wake_ਉੱਪਰ_interruptible(&t->event_wait);
		}
		t->ਆਖਰੀ_event_ਸਮਾਂ = now;
		/* Reset threshold breach flag once event got generated */
		t->pending_event = false;
	}
}

ਸਥਿਰ u64 update_averages(ਬਣਤਰ psi_ਸਮੂਹ *ਸਮੂਹ, u64 now)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ missed_periods = 0;
	u64 expires, period;
	u64 avg_ਅਗਲਾ_update;
	ਪੂਰਨ s;

	/* avgX= */
	expires = ਸਮੂਹ->avg_ਅਗਲਾ_update;
	ਜੇ (now - expires >= psi_period)
		missed_periods = div_u64(now - expires, psi_period);

	/*
	 * The periodic clock tick can get delayed for various
	 * reasons, especially on loaded systems. To avoid clock
	 * drift, we schedule the clock in fixed psi_period intervals.
	 * But the deltas we sample out of the per-cpu buckets above
	 * are based on the actual time elapsing between clock ticks.
	 */
	avg_ਅਗਲਾ_update = expires + ((1 + missed_periods) * psi_period);
	period = now - (ਸਮੂਹ->avg_ਆਖਰੀ_update + (missed_periods * psi_period));
	ਸਮੂਹ->avg_ਆਖਰੀ_update = now;

	ਲਈ (s = 0; s < NR_PSI_ਹਾਲਤਾਂ - 1; s++) {
		u32 sample;

		sample = ਸਮੂਹ->ਕੁੱਲ[PSI_AVGS][s] - ਸਮੂਹ->avg_ਕੁੱਲ[s];
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
		ਜੇ (sample > period)
			sample = period;
		ਸਮੂਹ->avg_ਕੁੱਲ[s] += sample;
		calc_avgs(ਸਮੂਹ->avg[s], missed_periods, sample, period);
	}

	ਵਾਪਸ avg_ਅਗਲਾ_update;
}

ਸਥਿਰ ਖਾਲੀ psi_avgs_work(ਬਣਤਰ work_struct *work)
{
	ਬਣਤਰ delayed_work *dwork;
	ਬਣਤਰ psi_ਸਮੂਹ *ਸਮੂਹ;
	u32 ਬਦਲਿਆ_ਹਾਲਤਾਂ;
	u64 now;

	dwork = ਮੰਜ਼ਿਲ_delayed_work(work);
	ਸਮੂਹ = container_ਦਾ(dwork, ਬਣਤਰ psi_ਸਮੂਹ, avgs_work);

	mutex_ਤਾਲਾ(&ਸਮੂਹ->avgs_ਤਾਲਾ);

	now = sched_clock();

	collect_percpu_times(ਸਮੂਹ, PSI_AVGS, &ਬਦਲਿਆ_ਹਾਲਤਾਂ);
	/*
	 * If there is task activity, periodically fold the per-cpu
	 * times and feed samples into the running averages. If things
	 * are idle and there is no data to process, stop the clock.
	 * Once restarted, we'll catch up the running averages in one
	 * go - see calc_avgs() and missed_periods.
	 */
	ਜੇ (now >= ਸਮੂਹ->avg_ਅਗਲਾ_update) {
		update_triggers(ਸਮੂਹ, now, PSI_AVGS);
		ਸਮੂਹ->avg_ਅਗਲਾ_update = update_averages(ਸਮੂਹ, now);
	}

	ਜੇ (ਬਦਲਿਆ_ਹਾਲਤਾਂ & PSI_STATE_RESCHEDULE) {
		ਸਮਾਂ_ਸਾਰਣੀ_delayed_work(dwork, nsecs_ਮੰਜ਼ਿਲ_jiffies(
				ਸਮੂਹ->avg_ਅਗਲਾ_update - now) + 1);
	}

	mutex_unlock(&ਸਮੂਹ->avgs_ਤਾਲਾ);
}

ਸਥਿਰ ਖਾਲੀ init_rtpoll_triggers(ਬਣਤਰ psi_ਸਮੂਹ *ਸਮੂਹ, u64 now)
{
	ਬਣਤਰ psi_trigger *t;

	list_for_each_ਇੰਦਰਾਜ(t, &ਸਮੂਹ->rtpoll_triggers, ਗੰਢ)
		ਖਿੜਕੀ_reset(&t->win, now,
				ਸਮੂਹ->ਕੁੱਲ[PSI_POLL][t->state], 0);
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਸਮੂਹ->rtpoll_ਕੁੱਲ, ਸਮੂਹ->ਕੁੱਲ[PSI_POLL],
		   ਆਕਾਰ(ਸਮੂਹ->rtpoll_ਕੁੱਲ));
	ਸਮੂਹ->rtpoll_ਅਗਲਾ_update = now + ਸਮੂਹ->rtpoll_min_period;
}

/* Schedule rtpolling if it's not already scheduled or forced. */
ਸਥਿਰ ਖਾਲੀ psi_ਸਮਾਂ_ਸਾਰਣੀ_rtpoll_work(ਬਣਤਰ psi_ਸਮੂਹ *ਸਮੂਹ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ delay,
				   bool force)
{
	/*
	 * atomic_xchg should be called even when !force to provide a
	 * full memory barrier (see the comment inside psi_rtpoll_work).
	 */
	ਜੇ (atomic_xchg(&ਸਮੂਹ->rtpoll_scheduled, 1) && !force)
		ਵਾਪਸ;

	/*
	 * Only test whether a worker is installed; do not dereference the task.
	 * A racing trigger destruction may leave the timer armed.
	 * psi_cgroup_free() shuts it down before freeing the group.
	 * The system PSI group is static.
	 */
	ਜੇ (likely(rcu_access_ਸੂਚਕ(ਸਮੂਹ->rtpoll_task)))
		mod_timer(&ਸਮੂਹ->rtpoll_timer, jiffies + delay);
	ਨਹੀਂਤਾਂ
		atomic_ਸੈੱਟ(&ਸਮੂਹ->rtpoll_scheduled, 0);
}

ਸਥਿਰ ਖਾਲੀ psi_rtpoll_work(ਬਣਤਰ psi_ਸਮੂਹ *ਸਮੂਹ)
{
	bool force_reschedule = false;
	u32 ਬਦਲਿਆ_ਹਾਲਤਾਂ;
	u64 now;

	mutex_ਤਾਲਾ(&ਸਮੂਹ->rtpoll_trigger_ਤਾਲਾ);

	now = sched_clock();

	ਜੇ (now > ਸਮੂਹ->rtpoll_until) {
		/*
		 * We are either about to start or might stop rtpolling if no
		 * state change was recorded. Resetting rtpoll_scheduled leaves
		 * a small window for psi_group_change to sneak in and schedule
		 * an immediate rtpoll_work before we get to rescheduling. One
		 * potential extra wakeup at the end of the rtpolling window
		 * should be negligible and rtpoll_next_update still keeps
		 * updates correctly on schedule.
		 */
		atomic_ਸੈੱਟ(&ਸਮੂਹ->rtpoll_scheduled, 0);
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
	} ਨਹੀਂਤਾਂ {
		/* The rtpolling window is not over, keep rescheduling */
		force_reschedule = true;
	}


	collect_percpu_times(ਸਮੂਹ, PSI_POLL, &ਬਦਲਿਆ_ਹਾਲਤਾਂ);

	ਜੇ (ਬਦਲਿਆ_ਹਾਲਤਾਂ & ਸਮੂਹ->rtpoll_ਹਾਲਤਾਂ) {
		/* Initialize trigger windows when entering rtpolling mode */
		ਜੇ (now > ਸਮੂਹ->rtpoll_until)
			init_rtpoll_triggers(ਸਮੂਹ, now);

		/*
		 * Keep the monitor active for at least the duration of the
		 * minimum tracking window as long as monitor states are
		 * changing.
		 */
		ਸਮੂਹ->rtpoll_until = now +
			ਸਮੂਹ->rtpoll_min_period * UPDATES_ਪ੍ਰਤੀ_ਖਿੜਕੀ;
	}

	ਜੇ (now > ਸਮੂਹ->rtpoll_until) {
		ਸਮੂਹ->rtpoll_ਅਗਲਾ_update = ULLONG_MAX;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (now >= ਸਮੂਹ->rtpoll_ਅਗਲਾ_update) {
		ਜੇ (ਬਦਲਿਆ_ਹਾਲਤਾਂ & ਸਮੂਹ->rtpoll_ਹਾਲਤਾਂ) {
			update_triggers(ਸਮੂਹ, now, PSI_POLL);
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਸਮੂਹ->rtpoll_ਕੁੱਲ, ਸਮੂਹ->ਕੁੱਲ[PSI_POLL],
				   ਆਕਾਰ(ਸਮੂਹ->rtpoll_ਕੁੱਲ));
		}
		ਸਮੂਹ->rtpoll_ਅਗਲਾ_update = now + ਸਮੂਹ->rtpoll_min_period;
	}

	psi_ਸਮਾਂ_ਸਾਰਣੀ_rtpoll_work(ਸਮੂਹ,
		nsecs_ਮੰਜ਼ਿਲ_jiffies(ਸਮੂਹ->rtpoll_ਅਗਲਾ_update - now) + 1,
		force_reschedule);

ਬਾਹਰਲਾ:
	mutex_unlock(&ਸਮੂਹ->rtpoll_trigger_ਤਾਲਾ);
}

ਸਥਿਰ ਪੂਰਨ psi_rtpoll_ਕਾਮਾ(ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ psi_ਸਮੂਹ *ਸਮੂਹ = (ਬਣਤਰ psi_ਸਮੂਹ *)ਅੰਕੜੇ;

	sched_ਸੈੱਟ_fifo_ਨੀਵਾਂ(current);

	ਜਦੋਂ (true) {
		wait_event_interruptible(ਸਮੂਹ->rtpoll_wait,
				atomic_cmpxchg(&ਸਮੂਹ->rtpoll_wakeup, 1, 0) ||
				kthread_should_stop());
		ਜੇ (kthread_should_stop())
			ਰੋਕੋ;

		psi_rtpoll_work(ਸਮੂਹ);
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ poll_timer_fn(ਬਣਤਰ timer_list *t)
{
	ਬਣਤਰ psi_ਸਮੂਹ *ਸਮੂਹ = timer_container_ਦਾ(ਸਮੂਹ, t, rtpoll_timer);

	atomic_ਸੈੱਟ(&ਸਮੂਹ->rtpoll_wakeup, 1);
	wake_ਉੱਪਰ_interruptible(&ਸਮੂਹ->rtpoll_wait);
}

ਸਥਿਰ ਖਾਲੀ ਰਿਕਾਰਡ_times(ਬਣਤਰ psi_ਸਮੂਹ_cpu *groupc, u64 now)
{
	u32 delta;

	delta = now - groupc->state_ਸ਼ੁਰੂ;
	groupc->state_ਸ਼ੁਰੂ = now;

	ਜੇ (groupc->state_ਨਕਾਬ & (1 << PSI_IO_SOME)) {
		groupc->times[PSI_IO_SOME] += delta;
		ਜੇ (groupc->state_ਨਕਾਬ & (1 << PSI_IO_ਭਰਿਆ))
			groupc->times[PSI_IO_ਭਰਿਆ] += delta;
	}

	ਜੇ (groupc->state_ਨਕਾਬ & (1 << PSI_MEM_SOME)) {
		groupc->times[PSI_MEM_SOME] += delta;
		ਜੇ (groupc->state_ਨਕਾਬ & (1 << PSI_MEM_ਭਰਿਆ))
			groupc->times[PSI_MEM_ਭਰਿਆ] += delta;
	}

	ਜੇ (groupc->state_ਨਕਾਬ & (1 << PSI_CPU_SOME)) {
		groupc->times[PSI_CPU_SOME] += delta;
		ਜੇ (groupc->state_ਨਕਾਬ & (1 << PSI_CPU_ਭਰਿਆ))
			groupc->times[PSI_CPU_ਭਰਿਆ] += delta;
	}

	ਜੇ (groupc->state_ਨਕਾਬ & (1 << PSI_NONIDLE))
		groupc->times[PSI_NONIDLE] += delta;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ for_each_ਸਮੂਹ(iter, ਸਮੂਹ) \
	ਲਈ (typeof(ਸਮੂਹ) iter = ਸਮੂਹ; iter; iter = iter->ਮਾਪਾ)

ਸਥਿਰ ਖਾਲੀ psi_ਸਮੂਹ_change(ਬਣਤਰ psi_ਸਮੂਹ *ਸਮੂਹ, ਪੂਰਨ cpu,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ clear, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੈੱਟ,
			     u64 now, bool wake_clock)
{
	ਬਣਤਰ psi_ਸਮੂਹ_cpu *groupc;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ t, m;
	u32 state_ਨਕਾਬ;

	lockdep_assert_rq_held(cpu_rq(cpu));
	groupc = ਪ੍ਰਤੀ_cpu_ptr(ਸਮੂਹ->pcpu, cpu);

	/*
	 * Start with TSK_ONCPU, which doesn't have a corresponding
	 * task count - it's just a boolean flag directly encoded in
	 * the state mask. Clear, set, or carry the current state if
	 * no changes are requested.
	 */
	ਜੇ (unlikely(clear & TSK_ONCPU)) {
		state_ਨਕਾਬ = 0;
		clear &= ~TSK_ONCPU;
	} ਨਹੀਂਤਾਂ ਜੇ (unlikely(ਸੈੱਟ & TSK_ONCPU)) {
		state_ਨਕਾਬ = PSI_ONCPU;
		ਸੈੱਟ &= ~TSK_ONCPU;
	} ਨਹੀਂਤਾਂ {
		state_ਨਕਾਬ = groupc->state_ਨਕਾਬ & PSI_ONCPU;
	}

	/*
	 * The rest of the state mask is calculated based on the task
	 * counts. Update those first, then construct the mask.
	 */
	ਲਈ (t = 0, m = clear; m; m &= ~(1 << t), t++) {
		ਜੇ (!(m & (1 << t)))
			ਜਾਰੀ;
		ਜੇ (groupc->tasks[t]) {
			groupc->tasks[t]--;
		} ਨਹੀਂਤਾਂ ਜੇ (!psi_bug) {
			printk_deferred(KERN_ERR "psi: task underflow! cpu=%d t=%d tasks=[%u %u %u %u] clear=%x set=%x\n",
					cpu, t, groupc->tasks[0],
					groupc->tasks[1], groupc->tasks[2],
					groupc->tasks[3], clear, ਸੈੱਟ);
			psi_bug = 1;
		}
	}

	ਲਈ (t = 0; ਸੈੱਟ; ਸੈੱਟ &= ~(1 << t), t++)
		ਜੇ (ਸੈੱਟ & (1 << t))
			groupc->tasks[t]++;

	ਜੇ (!ਸਮੂਹ->enabled) {
		/*
		 * On the first group change after disabling PSI, conclude
		 * the current state and flush its time. This is unlikely
		 * to matter to the user, but aggregation (get_recent_times)
		 * may have already incorporated the live state into times_prev;
		 * avoid a delta sample underflow when PSI is later re-enabled.
		 */
		ਜੇ (unlikely(groupc->state_ਨਕਾਬ & (1 << PSI_NONIDLE)))
			ਰਿਕਾਰਡ_times(groupc, now);

		groupc->state_ਨਕਾਬ = state_ਨਕਾਬ;

		ਵਾਪਸ;
	}

	state_ਨਕਾਬ = test_ਹਾਲਤਾਂ(groupc->tasks, state_ਨਕਾਬ);

	/*
	 * Since we care about lost potential, a memstall is FULL
	 * when there are no other working tasks, but also when
	 * the CPU is actively reclaiming and nothing productive
	 * could run even if it were runnable. So when the current
	 * task in a cgroup is in_memstall, the corresponding groupc
	 * on that cpu is in PSI_MEM_FULL state.
	 */
	ਜੇ (unlikely((state_ਨਕਾਬ & PSI_ONCPU) && cpu_curr(cpu)->ਵਿੱਚ_memstall))
		state_ਨਕਾਬ |= (1 << PSI_MEM_ਭਰਿਆ);

	ਰਿਕਾਰਡ_times(groupc, now);

	groupc->state_ਨਕਾਬ = state_ਨਕਾਬ;

	ਜੇ (state_ਨਕਾਬ & ਸਮੂਹ->rtpoll_ਹਾਲਤਾਂ)
		psi_ਸਮਾਂ_ਸਾਰਣੀ_rtpoll_work(ਸਮੂਹ, 1, false);

	ਜੇ (wake_clock && !delayed_work_pending(&ਸਮੂਹ->avgs_work))
		ਸਮਾਂ_ਸਾਰਣੀ_delayed_work(&ਸਮੂਹ->avgs_work, PSI_FREQ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ psi_ਸਮੂਹ *task_psi_ਸਮੂਹ(ਬਣਤਰ task_struct *task)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CGROUPS
	ਜੇ (static_branch_likely(&psi_cgroups_enabled))
		ਵਾਪਸ cgroup_psi(task_dfl_cgroup(task));
#ਜੇ_ਅੰਤ
	ਵਾਪਸ &psi_system;
}

ਸਥਿਰ ਖਾਲੀ psi_flags_change(ਬਣਤਰ task_struct *task, ਪੂਰਨ clear, ਪੂਰਨ ਸੈੱਟ)
{
	ਜੇ (((task->psi_flags & ਸੈੱਟ) ||
	     (task->psi_flags & clear) != clear) &&
	    !psi_bug) {
		printk_deferred(KERN_ERR "psi: inconsistent task state! task=%d:%s cpu=%d psi_flags=%x clear=%x set=%x\n",
				task->pid, task->comm, task_cpu(task),
				task->psi_flags, clear, ਸੈੱਟ);
		psi_bug = 1;
	}

	task->psi_flags &= ~clear;
	task->psi_flags |= ਸੈੱਟ;
}

ਖਾਲੀ psi_task_change(ਬਣਤਰ task_struct *task, ਪੂਰਨ clear, ਪੂਰਨ ਸੈੱਟ)
{
	ਪੂਰਨ cpu = task_cpu(task);
	u64 now;

	ਜੇ (!task->pid)
		ਵਾਪਸ;

	psi_flags_change(task, clear, ਸੈੱਟ);

	psi_write_begin(cpu);
	now = cpu_clock(cpu);
	for_each_ਸਮੂਹ(ਸਮੂਹ, task_psi_ਸਮੂਹ(task))
		psi_ਸਮੂਹ_change(ਸਮੂਹ, cpu, clear, ਸੈੱਟ, now, true);
	psi_write_ਸਿਰਾ(cpu);
}

ਖਾਲੀ psi_task_switch(ਬਣਤਰ task_struct *prev, ਬਣਤਰ task_struct *ਅਗਲਾ,
		     bool sleep)
{
	ਬਣਤਰ psi_ਸਮੂਹ *common = ਨਿਰਾ;
	ਪੂਰਨ cpu = task_cpu(prev);
	u64 now;

	psi_write_begin(cpu);
	now = cpu_clock(cpu);

	ਜੇ (ਅਗਲਾ->pid) {
		psi_flags_change(ਅਗਲਾ, 0, TSK_ONCPU);
		/*
		 * Set TSK_ONCPU on @next's cgroups. If @next shares any
		 * ancestors with @prev, those will already have @prev's
		 * TSK_ONCPU bit set, and we can stop the iteration there.
		 */
		for_each_ਸਮੂਹ(ਸਮੂਹ, task_psi_ਸਮੂਹ(ਅਗਲਾ)) {
			ਬਣਤਰ psi_ਸਮੂਹ_cpu *groupc = ਪ੍ਰਤੀ_cpu_ptr(ਸਮੂਹ->pcpu, cpu);

			ਜੇ (groupc->state_ਨਕਾਬ & PSI_ONCPU) {
				common = ਸਮੂਹ;
				ਰੋਕੋ;
			}
			psi_ਸਮੂਹ_change(ਸਮੂਹ, cpu, 0, TSK_ONCPU, now, true);
		}
	}

	ਜੇ (prev->pid) {
		ਪੂਰਨ clear = TSK_ONCPU, ਸੈੱਟ = 0;
		bool wake_clock = true;

		/*
		 * When we're going to sleep, psi_dequeue() lets us
		 * handle TSK_RUNNING, TSK_MEMSTALL_RUNNING and
		 * TSK_IOWAIT here, where we can combine it with
		 * TSK_ONCPU and save walking common ancestors twice.
		 */
		ਜੇ (sleep) {
			clear |= TSK_RUNNING;
			ਜੇ (prev->ਵਿੱਚ_memstall)
				clear |= TSK_MEMSTALL_RUNNING;
			ਜੇ (prev->ਵਿੱਚ_iowait)
				ਸੈੱਟ |= TSK_IOWAIT;

			/*
			 * Periodic aggregation shuts off if there is a period of no
			 * task changes, so we wake it back up if necessary. However,
			 * don't do this if the task change is the aggregation worker
			 * itself going to sleep, or we'll ping-pong forever.
			 */
			ਜੇ (unlikely((prev->flags & PF_WQ_ਕਾਮਾ) &&
				     wq_ਕਾਮਾ_ਆਖਰੀ_func(prev) == psi_avgs_work))
				wake_clock = false;
		}

		psi_flags_change(prev, clear, ਸੈੱਟ);

		for_each_ਸਮੂਹ(ਸਮੂਹ, task_psi_ਸਮੂਹ(prev)) {
			ਜੇ (ਸਮੂਹ == common)
				ਰੋਕੋ;
			psi_ਸਮੂਹ_change(ਸਮੂਹ, cpu, clear, ਸੈੱਟ, now, wake_clock);
		}

		/*
		 * TSK_ONCPU is handled up to the common ancestor. If there are
		 * any other differences between the two tasks (e.g. prev goes
		 * to sleep, or only one task is memstall), finish propagating
		 * those differences all the way up to the root.
		 */
		ਜੇ ((prev->psi_flags ^ ਅਗਲਾ->psi_flags) & ~TSK_ONCPU) {
			clear &= ~TSK_ONCPU;
			for_each_ਸਮੂਹ(ਸਮੂਹ, common)
				psi_ਸਮੂਹ_change(ਸਮੂਹ, cpu, clear, ਸੈੱਟ, now, wake_clock);
		}
	}
	psi_write_ਸਿਰਾ(cpu);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_IRQ_ਸਮਾਂ_ACCOUNTING
ਖਾਲੀ psi_account_irqtime(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *curr, ਬਣਤਰ task_struct *prev)
{
	ਪੂਰਨ cpu = task_cpu(curr);
	ਬਣਤਰ psi_ਸਮੂਹ_cpu *groupc;
	s64 delta;
	u64 irq;
	u64 now;

	ਜੇ (static_branch_likely(&psi_disabled) || !irqtime_enabled())
		ਵਾਪਸ;

	ਜੇ (!curr->pid)
		ਵਾਪਸ;

	lockdep_assert_rq_held(rq);
	ਜੇ (prev && task_psi_ਸਮੂਹ(prev) == task_psi_ਸਮੂਹ(curr))
		ਵਾਪਸ;

	irq = irq_ਸਮਾਂ_read(cpu);
	delta = (s64)(irq - rq->psi_irq_ਸਮਾਂ);
	ਜੇ (delta <= 0)
		ਵਾਪਸ;
	rq->psi_irq_ਸਮਾਂ = irq;

	psi_write_begin(cpu);
	now = cpu_clock(cpu);

	for_each_ਸਮੂਹ(ਸਮੂਹ, task_psi_ਸਮੂਹ(curr)) {
		ਜੇ (!ਸਮੂਹ->enabled)
			ਜਾਰੀ;

		groupc = ਪ੍ਰਤੀ_cpu_ptr(ਸਮੂਹ->pcpu, cpu);

		ਰਿਕਾਰਡ_times(groupc, now);
		groupc->times[PSI_IRQ_ਭਰਿਆ] += delta;

		ਜੇ (ਸਮੂਹ->rtpoll_ਹਾਲਤਾਂ & (1 << PSI_IRQ_ਭਰਿਆ))
			psi_ਸਮਾਂ_ਸਾਰਣੀ_rtpoll_work(ਸਮੂਹ, 1, false);
	}
	psi_write_ਸਿਰਾ(cpu);
}
#ਜੇ_ਅੰਤ /* CONFIG_IRQ_TIME_ACCOUNTING */

/**
 * psi_memstall_enter - mark the beginning of a memory stall section
 * @flags: flags to handle nested sections
 *
 * Marks the calling task as being stalled due to a lack of memory,
 * such as waiting for a refault or performing reclaim.
 */
ਖਾਲੀ psi_memstall_enter(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *flags)
{
	ਬਣਤਰ rq_flags rf;
	ਬਣਤਰ rq *rq;

	ਜੇ (static_branch_likely(&psi_disabled))
		ਵਾਪਸ;

	*flags = current->ਵਿੱਚ_memstall;
	ਜੇ (*flags)
		ਵਾਪਸ;
	/*
	 * in_memstall setting & accounting needs to be atomic wrt
	 * changes to the task's scheduling state, otherwise we can
	 * race with CPU migration.
	 */
	rq = this_rq_ਤਾਲਾ_irq(&rf);

	current->ਵਿੱਚ_memstall = 1;
	psi_task_change(current, 0, TSK_MEMSTALL | TSK_MEMSTALL_RUNNING);

	rq_unlock_irq(rq, &rf);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(psi_memstall_enter);

/**
 * psi_memstall_leave - mark the end of an memory stall section
 * @flags: flags to handle nested memdelay sections
 *
 * Marks the calling task as no longer stalled due to lack of memory.
 */
ਖਾਲੀ psi_memstall_leave(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *flags)
{
	ਬਣਤਰ rq_flags rf;
	ਬਣਤਰ rq *rq;

	ਜੇ (static_branch_likely(&psi_disabled))
		ਵਾਪਸ;

	ਜੇ (*flags)
		ਵਾਪਸ;
	/*
	 * in_memstall clearing & accounting needs to be atomic wrt
	 * changes to the task's scheduling state, otherwise we could
	 * race with CPU migration.
	 */
	rq = this_rq_ਤਾਲਾ_irq(&rf);

	current->ਵਿੱਚ_memstall = 0;
	psi_task_change(current, TSK_MEMSTALL | TSK_MEMSTALL_RUNNING, 0);

	rq_unlock_irq(rq, &rf);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(psi_memstall_leave);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CGROUPS
ਪੂਰਨ psi_cgroup_alloc(ਬਣਤਰ cgroup *cgroup)
{
	ਜੇ (!static_branch_likely(&psi_cgroups_enabled))
		ਵਾਪਸ 0;

	cgroup->psi = kzalloc_obj(ਬਣਤਰ psi_ਸਮੂਹ);
	ਜੇ (!cgroup->psi)
		ਵਾਪਸ -ENOMEM;

	cgroup->psi->pcpu = alloc_percpu(ਬਣਤਰ psi_ਸਮੂਹ_cpu);
	ਜੇ (!cgroup->psi->pcpu) {
		kfree(cgroup->psi);
		ਵਾਪਸ -ENOMEM;
	}
	ਸਮੂਹ_init(cgroup->psi);
	cgroup->psi->ਮਾਪਾ = cgroup_psi(cgroup_ਮਾਪਾ(cgroup));
	ਵਾਪਸ 0;
}

ਖਾਲੀ psi_cgroup_free(ਬਣਤਰ cgroup *cgroup)
{
	ਜੇ (!static_branch_likely(&psi_cgroups_enabled))
		ਵਾਪਸ;

	cancel_delayed_work_sync(&cgroup->psi->avgs_work);
	/*
	 * A psi_schedule_rtpoll_work() call racing the last trigger's
	 * destruction may have re-armed the timer after psi_trigger_destroy()
	 * deleted it. Spurious firing while the group is alive is harmless.
	 */
	timer_shutdown_sync(&cgroup->psi->rtpoll_timer);
	free_percpu(cgroup->psi->pcpu);
	/* All triggers must be removed by now */
	WARN_ONCE(cgroup->psi->rtpoll_ਹਾਲਤਾਂ, "psi: trigger leak\n");
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
ਖਾਲੀ cgroup_move_task(ਬਣਤਰ task_struct *task, ਬਣਤਰ css_ਸੈੱਟ *ਮੰਜ਼ਿਲ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ task_flags;
	ਬਣਤਰ rq_flags rf;
	ਬਣਤਰ rq *rq;

	ਜੇ (!static_branch_likely(&psi_cgroups_enabled)) {
		/*
		 * Lame to do this here, but the scheduler cannot be locked
		 * from the outside, so we move cgroups from inside sched/.
		 */
		rcu_ਸੌਂਪੋ_ਸੂਚਕ(task->cgroups, ਮੰਜ਼ਿਲ);
		ਵਾਪਸ;
	}

	rq = task_rq_ਤਾਲਾ_2(task, &rf);

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

	ਜੇ (task_flags)
		psi_task_change(task, task_flags, 0);

	/* See comment above */
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(task->cgroups, ਮੰਜ਼ਿਲ);

	ਜੇ (task_flags)
		psi_task_change(task, 0, task_flags);

	task_rq_unlock(rq, task, &rf);
}

ਖਾਲੀ psi_cgroup_restart(ਬਣਤਰ psi_ਸਮੂਹ *ਸਮੂਹ)
{
	ਪੂਰਨ cpu;

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
	ਜੇ (!ਸਮੂਹ->enabled)
		ਵਾਪਸ;

	for_each_possible_cpu(cpu) {
		u64 now;

		guard(rq_ਤਾਲਾ_irq)(cpu_rq(cpu));

		psi_write_begin(cpu);
		now = cpu_clock(cpu);
		psi_ਸਮੂਹ_change(ਸਮੂਹ, cpu, 0, 0, now, true);
		psi_write_ਸਿਰਾ(cpu);
	}
}
#ਜੇ_ਅੰਤ /* CONFIG_CGROUPS */

ਪੂਰਨ psi_show(ਬਣਤਰ seq_file *m, ਬਣਤਰ psi_ਸਮੂਹ *ਸਮੂਹ, ਗਿਣਤੀ psi_res res)
{
	bool only_ਭਰਿਆ = false;
	ਪੂਰਨ ਭਰਿਆ;
	u64 now;

	ਜੇ (static_branch_likely(&psi_disabled))
		ਵਾਪਸ -EOPNOTSUPP;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_IRQ_ਸਮਾਂ_ACCOUNTING
	ਜੇ (!irqtime_enabled() && res == PSI_IRQ)
		ਵਾਪਸ -EOPNOTSUPP;
#ਜੇ_ਅੰਤ

	/* Update averages before reporting them */
	mutex_ਤਾਲਾ(&ਸਮੂਹ->avgs_ਤਾਲਾ);
	now = sched_clock();
	collect_percpu_times(ਸਮੂਹ, PSI_AVGS, ਨਿਰਾ);
	ਜੇ (now >= ਸਮੂਹ->avg_ਅਗਲਾ_update)
		ਸਮੂਹ->avg_ਅਗਲਾ_update = update_averages(ਸਮੂਹ, now);
	mutex_unlock(&ਸਮੂਹ->avgs_ਤਾਲਾ);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_IRQ_ਸਮਾਂ_ACCOUNTING
	only_ਭਰਿਆ = res == PSI_IRQ;
#ਜੇ_ਅੰਤ

	ਲਈ (ਭਰਿਆ = 0; ਭਰਿਆ < 2 - only_ਭਰਿਆ; ਭਰਿਆ++) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ avg[3] = { 0, };
		u64 ਕੁੱਲ = 0;
		ਪੂਰਨ w;

		/* CPU FULL is undefined at the system level */
		ਜੇ (!(ਸਮੂਹ == &psi_system && res == PSI_CPU && ਭਰਿਆ)) {
			ਲਈ (w = 0; w < 3; w++)
				avg[w] = ਸਮੂਹ->avg[res * 2 + ਭਰਿਆ][w];
			ਕੁੱਲ = div_u64(ਸਮੂਹ->ਕੁੱਲ[PSI_AVGS][res * 2 + ਭਰਿਆ],
					NSEC_ਪ੍ਰਤੀ_USEC);
		}

		seq_printf(m, "%s avg10=%lu.%02lu avg60=%lu.%02lu avg300=%lu.%02lu total=%llu\n",
			   ਭਰਿਆ || only_ਭਰਿਆ ? "full" : "some",
			   ਲੱਦੋ_INT(avg[0]), ਲੱਦੋ_FRAC(avg[0]),
			   ਲੱਦੋ_INT(avg[1]), ਲੱਦੋ_FRAC(avg[1]),
			   ਲੱਦੋ_INT(avg[2]), ਲੱਦੋ_FRAC(avg[2]),
			   ਕੁੱਲ);
	}

	ਵਾਪਸ 0;
}

/*
 * Create @group's rtpoll worker after psi_trigger_create() reported the need
 * for one. kthread creation depends on the whole fork path and we don't want
 * all of that nested inside cgroup_mutex, so the caller must drop it and any
 * other lock that forks can wait behind. If two callers race, the loser stops
 * its never-woken kthread.
 */
ਪੂਰਨ psi_trigger_create_rtpoll_ਕਾਮਾ(ਬਣਤਰ psi_ਸਮੂਹ *ਸਮੂਹ)
{
	ਬਣਤਰ task_struct *task;

	task = kthread_create(psi_rtpoll_ਕਾਮਾ, ਸਮੂਹ, "psimon");
	ਜੇ (IS_ERR(task))
		ਵਾਪਸ PTR_ERR(task);

	scoped_guard(mutex, &ਸਮੂਹ->rtpoll_trigger_ਤਾਲਾ) {
		ਜੇ (!rcu_access_ਸੂਚਕ(ਸਮੂਹ->rtpoll_task)) {
			atomic_ਸੈੱਟ(&ਸਮੂਹ->rtpoll_wakeup, 0);
			wake_ਉੱਪਰ_process(task);
			rcu_ਸੌਂਪੋ_ਸੂਚਕ(ਸਮੂਹ->rtpoll_task, task);

			/*
			 * Poll once to catch up on scheduling attempts dropped
			 * while there was no rtpoll worker.
			 */
			psi_ਸਮਾਂ_ਸਾਰਣੀ_rtpoll_work(ਸਮੂਹ, 1, true);
			ਵਾਪਸ 0;
		}
	}

	kthread_stop(task);
	ਵਾਪਸ 0;
}

ਬਣਤਰ psi_trigger *psi_trigger_create(ਬਣਤਰ psi_ਸਮੂਹ *ਸਮੂਹ, ਅੱਖਰ *buf,
				       ਗਿਣਤੀ psi_res res, ਬਣਤਰ file *file,
				       ਬਣਤਰ kernfs_open_file *ਦਾ,
				       bool *need_rtpoll_ਕਾਮਾ)
{
	ਬਣਤਰ psi_trigger *t;
	ਗਿਣਤੀ psi_ਹਾਲਤਾਂ state;
	u32 ਹੱਦ_us;
	bool privileged;
	u32 ਖਿੜਕੀ_us;

	*need_rtpoll_ਕਾਮਾ = false;

	ਜੇ (static_branch_likely(&psi_disabled))
		ਵਾਪਸ ERR_PTR(-EOPNOTSUPP);

	/*
	 * Checking the privilege here on file->f_cred implies that a privileged user
	 * could open the file and delegate the write to an unprivileged one.
	 */
	privileged = cap_raised(file->f_cred->cap_effective, CAP_SYS_RESOURCE);

	ਜੇ (ਲੜੀ_ਤੋਂ_ਪੜ੍ਹੋ(buf, "some %u %u", &ਹੱਦ_us, &ਖਿੜਕੀ_us) == 2)
		state = PSI_IO_SOME + res * 2;
	ਨਹੀਂਤਾਂ ਜੇ (ਲੜੀ_ਤੋਂ_ਪੜ੍ਹੋ(buf, "full %u %u", &ਹੱਦ_us, &ਖਿੜਕੀ_us) == 2)
		state = PSI_IO_ਭਰਿਆ + res * 2;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ ERR_PTR(-EINVAL);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_IRQ_ਸਮਾਂ_ACCOUNTING
	ਜੇ (res == PSI_IRQ && --state != PSI_IRQ_ਭਰਿਆ)
		ਵਾਪਸ ERR_PTR(-EINVAL);
#ਜੇ_ਅੰਤ

	ਜੇ (state >= PSI_NONIDLE)
		ਵਾਪਸ ERR_PTR(-EINVAL);

	ਜੇ (ਖਿੜਕੀ_us == 0 || ਖਿੜਕੀ_us > ਖਿੜਕੀ_MAX_US)
		ਵਾਪਸ ERR_PTR(-EINVAL);

	/*
	 * Unprivileged users can only use 2s windows so that averages aggregation
	 * work is used, and no RT threads need to be spawned.
	 */
	ਜੇ (!privileged && ਖਿੜਕੀ_us % 2000000)
		ਵਾਪਸ ERR_PTR(-EINVAL);

	/* Check threshold */
	ਜੇ (ਹੱਦ_us == 0 || ਹੱਦ_us > ਖਿੜਕੀ_us)
		ਵਾਪਸ ERR_PTR(-EINVAL);

	t = kmalloc_obj(*t);
	ਜੇ (!t)
		ਵਾਪਸ ERR_PTR(-ENOMEM);

	t->ਸਮੂਹ = ਸਮੂਹ;
	t->state = state;
	t->ਹੱਦ = ਹੱਦ_us * NSEC_ਪ੍ਰਤੀ_USEC;
	t->win.ਮਾਪ = ਖਿੜਕੀ_us * NSEC_ਪ੍ਰਤੀ_USEC;
	ਖਿੜਕੀ_reset(&t->win, sched_clock(),
			ਸਮੂਹ->ਕੁੱਲ[PSI_POLL][t->state], 0);

	t->event = 0;
	t->ਆਖਰੀ_event_ਸਮਾਂ = 0;
	t->ਦਾ = ਦਾ;
	ਜੇ (!ਦਾ)
		init_waitqueue_ਸਿਰ(&t->event_wait);
	t->pending_event = false;
	t->aggregator = privileged ? PSI_POLL : PSI_AVGS;

	ਜੇ (privileged) {
		mutex_ਤਾਲਾ(&ਸਮੂਹ->rtpoll_trigger_ਤਾਲਾ);

		list_ਜੋੜੋ(&t->ਗੰਢ, &ਸਮੂਹ->rtpoll_triggers);
		ਸਮੂਹ->rtpoll_min_period = min(ਸਮੂਹ->rtpoll_min_period,
			div_u64(t->win.ਮਾਪ, UPDATES_ਪ੍ਰਤੀ_ਖਿੜਕੀ));
		ਸਮੂਹ->rtpoll_nr_triggers[t->state]++;
		ਸਮੂਹ->rtpoll_ਹਾਲਤਾਂ |= (1 << t->state);

		*need_rtpoll_ਕਾਮਾ = !rcu_access_ਸੂਚਕ(ਸਮੂਹ->rtpoll_task);

		mutex_unlock(&ਸਮੂਹ->rtpoll_trigger_ਤਾਲਾ);
	} ਨਹੀਂਤਾਂ {
		mutex_ਤਾਲਾ(&ਸਮੂਹ->avgs_ਤਾਲਾ);

		list_ਜੋੜੋ(&t->ਗੰਢ, &ਸਮੂਹ->avg_triggers);
		ਸਮੂਹ->avg_nr_triggers[t->state]++;

		mutex_unlock(&ਸਮੂਹ->avgs_ਤਾਲਾ);
	}
	ਵਾਪਸ t;
}

ਖਾਲੀ psi_trigger_destroy(ਬਣਤਰ psi_trigger *t)
{
	ਬਣਤਰ psi_ਸਮੂਹ *ਸਮੂਹ;
	ਬਣਤਰ task_struct *task_ਮੰਜ਼ਿਲ_destroy = ਨਿਰਾ;

	/*
	 * We do not check psi_disabled since it might have been disabled after
	 * the trigger got created.
	 */
	ਜੇ (!t)
		ਵਾਪਸ;

	ਸਮੂਹ = t->ਸਮੂਹ;
	/*
	 * Wakeup waiters to stop polling and clear the queue to prevent it from
	 * being accessed later. Can happen if cgroup is deleted from under a
	 * polling process.
	 */
	ਜੇ (t->ਦਾ)
		kernfs_notify(t->ਦਾ->kn);
	ਨਹੀਂਤਾਂ
		wake_ਉੱਪਰ_interruptible(&t->event_wait);

	ਜੇ (t->aggregator == PSI_AVGS) {
		mutex_ਤਾਲਾ(&ਸਮੂਹ->avgs_ਤਾਲਾ);
		ਜੇ (!list_ਖਾਲੀ_ਥਾਵਾਂ(&t->ਗੰਢ)) {
			list_del(&t->ਗੰਢ);
			ਸਮੂਹ->avg_nr_triggers[t->state]--;
		}
		mutex_unlock(&ਸਮੂਹ->avgs_ਤਾਲਾ);
	} ਨਹੀਂਤਾਂ {
		mutex_ਤਾਲਾ(&ਸਮੂਹ->rtpoll_trigger_ਤਾਲਾ);
		ਜੇ (!list_ਖਾਲੀ_ਥਾਵਾਂ(&t->ਗੰਢ)) {
			ਬਣਤਰ psi_trigger *tmp;
			u64 period = ULLONG_MAX;

			list_del(&t->ਗੰਢ);
			ਸਮੂਹ->rtpoll_nr_triggers[t->state]--;
			ਜੇ (!ਸਮੂਹ->rtpoll_nr_triggers[t->state])
				ਸਮੂਹ->rtpoll_ਹਾਲਤਾਂ &= ~(1 << t->state);
			/*
			 * Reset min update period for the remaining triggers
			 * iff the destroying trigger had the min window size.
			 */
			ਜੇ (ਸਮੂਹ->rtpoll_min_period == div_u64(t->win.ਮਾਪ, UPDATES_ਪ੍ਰਤੀ_ਖਿੜਕੀ)) {
				list_for_each_ਇੰਦਰਾਜ(tmp, &ਸਮੂਹ->rtpoll_triggers, ਗੰਢ)
					period = min(period, div_u64(tmp->win.ਮਾਪ,
							UPDATES_ਪ੍ਰਤੀ_ਖਿੜਕੀ));
				ਸਮੂਹ->rtpoll_min_period = period;
			}
			/* Destroy rtpoll_task when the last trigger is destroyed */
			ਜੇ (ਸਮੂਹ->rtpoll_ਹਾਲਤਾਂ == 0) {
				ਸਮੂਹ->rtpoll_until = 0;
				task_ਮੰਜ਼ਿਲ_destroy = rcu_dereference_protected(
						ਸਮੂਹ->rtpoll_task,
						lockdep_is_held(&ਸਮੂਹ->rtpoll_trigger_ਤਾਲਾ));
				rcu_ਸੌਂਪੋ_ਸੂਚਕ(ਸਮੂਹ->rtpoll_task, ਨਿਰਾ);
				timer_delete(&ਸਮੂਹ->rtpoll_timer);
			}
		}
		mutex_unlock(&ਸਮੂਹ->rtpoll_trigger_ਤਾਲਾ);
	}

	/*
	 * Stop kthread 'psimon' after releasing rtpoll_trigger_lock to prevent
	 * a deadlock while waiting for psi_rtpoll_work to acquire
	 * rtpoll_trigger_lock
	 */
	ਜੇ (task_ਮੰਜ਼ਿਲ_destroy) {
		kthread_stop(task_ਮੰਜ਼ਿਲ_destroy);
		atomic_ਸੈੱਟ(&ਸਮੂਹ->rtpoll_scheduled, 0);
	}
	kfree(t);
}

__poll_t psi_trigger_poll(ਖਾਲੀ **trigger_ptr,
				ਬਣਤਰ file *file, poll_ਸਾਰਣੀ *wait)
{
	__poll_t ret = DEFAULT_POLLMASK;
	ਬਣਤਰ psi_trigger *t;

	ਜੇ (static_branch_likely(&psi_disabled))
		ਵਾਪਸ DEFAULT_POLLMASK | EPOLLERR | EPOLLPRI;

	t = smp_ਲੱਦੋ_acquire(trigger_ptr);
	ਜੇ (!t)
		ਵਾਪਸ DEFAULT_POLLMASK | EPOLLERR | EPOLLPRI;

	ਜੇ (t->ਦਾ)
		kernfs_generic_poll(t->ਦਾ, wait);
	ਨਹੀਂਤਾਂ
		poll_wait(file, &t->event_wait, wait);

	ਜੇ (cmpxchg(&t->event, 1, 0) == 1)
		ret |= EPOLLPRI;

	ਵਾਪਸ ret;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PROC_FS
ਸਥਿਰ ਪੂਰਨ psi_io_show(ਬਣਤਰ seq_file *m, ਖਾਲੀ *v)
{
	ਵਾਪਸ psi_show(m, &psi_system, PSI_IO);
}

ਸਥਿਰ ਪੂਰਨ psi_ਸਮ੍ਰਿਤੀ_show(ਬਣਤਰ seq_file *m, ਖਾਲੀ *v)
{
	ਵਾਪਸ psi_show(m, &psi_system, PSI_MEM);
}

ਸਥਿਰ ਪੂਰਨ psi_cpu_show(ਬਣਤਰ seq_file *m, ਖਾਲੀ *v)
{
	ਵਾਪਸ psi_show(m, &psi_system, PSI_CPU);
}

ਸਥਿਰ ਪੂਰਨ psi_io_open(ਬਣਤਰ inode *inode, ਬਣਤਰ file *file)
{
	ਵਾਪਸ single_open(file, psi_io_show, ਨਿਰਾ);
}

ਸਥਿਰ ਪੂਰਨ psi_ਸਮ੍ਰਿਤੀ_open(ਬਣਤਰ inode *inode, ਬਣਤਰ file *file)
{
	ਵਾਪਸ single_open(file, psi_ਸਮ੍ਰਿਤੀ_show, ਨਿਰਾ);
}

ਸਥਿਰ ਪੂਰਨ psi_cpu_open(ਬਣਤਰ inode *inode, ਬਣਤਰ file *file)
{
	ਵਾਪਸ single_open(file, psi_cpu_show, ਨਿਰਾ);
}

ਸਥਿਰ ssize_t psi_write(ਬਣਤਰ file *file, ਅਚਲ ਅੱਖਰ __user *user_buf,
			 ਮਾਪ_t nbytes, ਗਿਣਤੀ psi_res res)
{
	ਅੱਖਰ buf[32];
	ਮਾਪ_t buf_ਮਾਪ;
	ਬਣਤਰ seq_file *seq;
	ਬਣਤਰ psi_trigger *new;
	bool need_rtpoll_ਕਾਮਾ;
	ਪੂਰਨ ret;

	ਜੇ (static_branch_likely(&psi_disabled))
		ਵਾਪਸ -EOPNOTSUPP;

	ਜੇ (!nbytes)
		ਵਾਪਸ -EINVAL;

	buf_ਮਾਪ = min(nbytes, ਆਕਾਰ(buf));
	ਜੇ (copy_from_user(buf, user_buf, buf_ਮਾਪ))
		ਵਾਪਸ -EFAULT;

	buf[buf_ਮਾਪ - 1] = '\0';

	seq = file->ਨਿੱਜੀ_ਅੰਕੜੇ;

	/* Take seq->lock to protect seq->private from concurrent writes */
	mutex_ਤਾਲਾ(&seq->ਤਾਲਾ);

	/* Allow only one trigger per file descriptor */
	ਜੇ (seq->ਨਿਜੀ) {
		mutex_unlock(&seq->ਤਾਲਾ);
		ਵਾਪਸ -EBUSY;
	}

	new = psi_trigger_create(&psi_system, buf, res, file, ਨਿਰਾ,
				 &need_rtpoll_ਕਾਮਾ);
	ਜੇ (IS_ERR(new)) {
		mutex_unlock(&seq->ਤਾਲਾ);
		ਵਾਪਸ PTR_ERR(new);
	}

	ਜੇ (need_rtpoll_ਕਾਮਾ) {
		ret = psi_trigger_create_rtpoll_ਕਾਮਾ(&psi_system);
		ਜੇ (ret) {
			psi_trigger_destroy(new);
			mutex_unlock(&seq->ਤਾਲਾ);
			ਵਾਪਸ ret;
		}
	}

	smp_store_release(&seq->ਨਿਜੀ, new);
	mutex_unlock(&seq->ਤਾਲਾ);

	ਵਾਪਸ nbytes;
}

ਸਥਿਰ ssize_t psi_io_write(ਬਣਤਰ file *file, ਅਚਲ ਅੱਖਰ __user *user_buf,
			    ਮਾਪ_t nbytes, loff_t *ppos)
{
	ਵਾਪਸ psi_write(file, user_buf, nbytes, PSI_IO);
}

ਸਥਿਰ ssize_t psi_ਸਮ੍ਰਿਤੀ_write(ਬਣਤਰ file *file, ਅਚਲ ਅੱਖਰ __user *user_buf,
				ਮਾਪ_t nbytes, loff_t *ppos)
{
	ਵਾਪਸ psi_write(file, user_buf, nbytes, PSI_MEM);
}

ਸਥਿਰ ssize_t psi_cpu_write(ਬਣਤਰ file *file, ਅਚਲ ਅੱਖਰ __user *user_buf,
			     ਮਾਪ_t nbytes, loff_t *ppos)
{
	ਵਾਪਸ psi_write(file, user_buf, nbytes, PSI_CPU);
}

ਸਥਿਰ __poll_t psi_fop_poll(ਬਣਤਰ file *file, poll_ਸਾਰਣੀ *wait)
{
	ਬਣਤਰ seq_file *seq = file->ਨਿੱਜੀ_ਅੰਕੜੇ;

	ਵਾਪਸ psi_trigger_poll(&seq->ਨਿਜੀ, file, wait);
}

ਸਥਿਰ ਪੂਰਨ psi_fop_release(ਬਣਤਰ inode *inode, ਬਣਤਰ file *file)
{
	ਬਣਤਰ seq_file *seq = file->ਨਿੱਜੀ_ਅੰਕੜੇ;

	psi_trigger_destroy(seq->ਨਿਜੀ);
	ਵਾਪਸ single_release(inode, file);
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ proc_ops psi_io_proc_ops = {
	.proc_open	= psi_io_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_io_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

ਸਥਿਰ ਅਚਲ ਬਣਤਰ proc_ops psi_ਸਮ੍ਰਿਤੀ_proc_ops = {
	.proc_open	= psi_ਸਮ੍ਰਿਤੀ_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_ਸਮ੍ਰਿਤੀ_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

ਸਥਿਰ ਅਚਲ ਬਣਤਰ proc_ops psi_cpu_proc_ops = {
	.proc_open	= psi_cpu_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_cpu_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_IRQ_ਸਮਾਂ_ACCOUNTING
ਸਥਿਰ ਪੂਰਨ psi_irq_show(ਬਣਤਰ seq_file *m, ਖਾਲੀ *v)
{
	ਵਾਪਸ psi_show(m, &psi_system, PSI_IRQ);
}

ਸਥਿਰ ਪੂਰਨ psi_irq_open(ਬਣਤਰ inode *inode, ਬਣਤਰ file *file)
{
	ਵਾਪਸ single_open(file, psi_irq_show, ਨਿਰਾ);
}

ਸਥਿਰ ssize_t psi_irq_write(ਬਣਤਰ file *file, ਅਚਲ ਅੱਖਰ __user *user_buf,
			     ਮਾਪ_t nbytes, loff_t *ppos)
{
	ਵਾਪਸ psi_write(file, user_buf, nbytes, PSI_IRQ);
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ proc_ops psi_irq_proc_ops = {
	.proc_open	= psi_irq_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_irq_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};
#ਜੇ_ਅੰਤ /* CONFIG_IRQ_TIME_ACCOUNTING */

ਸਥਿਰ ਪੂਰਨ __init psi_proc_init(ਖਾਲੀ)
{
	ਜੇ (psi_enable) {
		proc_mkdir("pressure", ਨਿਰਾ);
		proc_create("pressure/io", 0666, ਨਿਰਾ, &psi_io_proc_ops);
		proc_create("pressure/memory", 0666, ਨਿਰਾ, &psi_ਸਮ੍ਰਿਤੀ_proc_ops);
		proc_create("pressure/cpu", 0666, ਨਿਰਾ, &psi_cpu_proc_ops);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_IRQ_ਸਮਾਂ_ACCOUNTING
		proc_create("pressure/irq", 0666, ਨਿਰਾ, &psi_irq_proc_ops);
#ਜੇ_ਅੰਤ
	}
	ਵਾਪਸ 0;
}
module_init(psi_proc_init);

#ਜੇ_ਅੰਤ /* CONFIG_PROC_FS */
