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
#ಅನ್ತರ್ಭಾವ <linux/sched/clock.h>
#ಅನ್ತರ್ಭಾವ <linux/workqueue.h>
#ಅನ್ತರ್ಭಾವ <linux/psi.h>
#ಅನ್ತರ್ಭಾವ "sched.h"

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ psi_bug __read_mostly;

DEFINE_STATIC_ಕೀಲಿ_FALSE(psi_disabled);
ಸ್ಥಿರ DEFINE_STATIC_ಕೀಲಿ_TRUE(psi_cgroups_enabled);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PSI_DEFAULT_DISABLED
ಸ್ಥಿರ bool psi_enable;
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ bool psi_enable = true;
#ಆದರೆ_ಅಂತ್ಯ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init ಸಿದ್ಧತೆ_psi(ಅಕ್ಷರ *str)
{
	ಹಿಂತಿರುಗಿ kstrtobool(str, &psi_enable) == 0;
}
__setup("psi=", ಸಿದ್ಧತೆ_psi);

/* Running averages - we need to be higher-res than loadavg */
#ವ್ಯಾಖ್ಯಾನಿಸು PSI_FREQ	(2*HZ+1)	/* 2 sec intervals */
#ವ್ಯಾಖ್ಯಾನಿಸು EXP_10s		1677		/* 1/exp(2s/10s) as fixed-point */
#ವ್ಯಾಖ್ಯಾನಿಸು EXP_60s		1981		/* 1/exp(2s/60s) */
#ವ್ಯಾಖ್ಯಾನಿಸು EXP_300s	2034		/* 1/exp(2s/300s) */

/* PSI trigger definitions */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಿಟಕಿ_MAX_US 10000000	/* Max window size is 10s */
#ವ್ಯಾಖ್ಯಾನಿಸು UPDATES_ಪ್ರತಿ_ಕಿಟಕಿ 10	/* 10 updates per window */

/* Sampling frequency in nanoseconds */
ಸ್ಥಿರ u64 psi_period __read_mostly;

/* System-level pressure and stall tracking */
ಸ್ಥಿರ DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ psi_ಗುಂಪು_cpu, system_ಗುಂಪು_pcpu);
ರಚನೆ psi_ಗುಂಪು psi_system = {
	.pcpu = &system_ಗುಂಪು_pcpu,
};

ಸ್ಥಿರ DEFINE_ಪ್ರತಿ_CPU(seqcount_t, psi_seq) = SEQCNT_ZERO(psi_seq);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ psi_write_begin(ಪೂರ್ಣಾಂಕ cpu)
{
	write_seqcount_begin(ಪ್ರತಿ_cpu_ptr(&psi_seq, cpu));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ psi_write_ಕೊನೆ(ಪೂರ್ಣಾಂಕ cpu)
{
	write_seqcount_ಕೊನೆ(ಪ್ರತಿ_cpu_ptr(&psi_seq, cpu));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ u32 psi_read_begin(ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ read_seqcount_begin(ಪ್ರತಿ_cpu_ptr(&psi_seq, cpu));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool psi_read_retry(ಪೂರ್ಣಾಂಕ cpu, u32 seq)
{
	ಹಿಂತಿರುಗಿ read_seqcount_retry(ಪ್ರತಿ_cpu_ptr(&psi_seq, cpu), seq);
}

ಸ್ಥಿರ ಶೂನ್ಯ psi_avgs_work(ರಚನೆ work_struct *work);

ಸ್ಥಿರ ಶೂನ್ಯ poll_timer_fn(ರಚನೆ timer_list *t);

ಸ್ಥಿರ ಶೂನ್ಯ ಗುಂಪು_init(ರಚನೆ psi_ಗುಂಪು *ಗುಂಪು)
{
	ಗುಂಪು->enabled = true;
	ಗುಂಪು->avg_ಕೊನೆಯ_update = sched_clock();
	ಗುಂಪು->avg_ಮುಂದಿನ_update = ಗುಂಪು->avg_ಕೊನೆಯ_update + psi_period;
	mutex_init(&ಗುಂಪು->avgs_ಬೀಗ);

	/* Init avg trigger-related members */
	INIT_LIST_ತಲೆ(&ಗುಂಪು->avg_triggers);
	memset(ಗುಂಪು->avg_nr_triggers, 0, ಗಾತ್ರ(ಗುಂಪು->avg_nr_triggers));
	INIT_DELAYED_WORK(&ಗುಂಪು->avgs_work, psi_avgs_work);

	/* Init rtpoll trigger-related members */
	atomic_ಗಣ(&ಗುಂಪು->rtpoll_scheduled, 0);
	mutex_init(&ಗುಂಪು->rtpoll_trigger_ಬೀಗ);
	INIT_LIST_ತಲೆ(&ಗುಂಪು->rtpoll_triggers);
	ಗುಂಪು->rtpoll_min_period = U32_MAX;
	ಗುಂಪು->rtpoll_ಮುಂದಿನ_update = ULLONG_MAX;
	init_waitqueue_ತಲೆ(&ಗುಂಪು->rtpoll_wait);
	timer_ಸಿದ್ಧತೆ(&ಗುಂಪು->rtpoll_timer, poll_timer_fn, 0);
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(ಗುಂಪು->rtpoll_task, ಖಾಲಿ);
}

ಶೂನ್ಯ __init psi_init(ಶೂನ್ಯ)
{
	ಆದರೆ (!psi_enable) {
		static_branch_enable(&psi_disabled);
		static_branch_disable(&psi_cgroups_enabled);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (!cgroup_psi_enabled())
		static_branch_disable(&psi_cgroups_enabled);

	psi_period = jiffies_ಗಮ್ಯ_nsecs(PSI_FREQ);
	ಗುಂಪು_init(&psi_system);
}

ಸ್ಥಿರ u32 test_ಸ್ಥಿತಿಗಳು(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *tasks, u32 state_ಮುಖವಾಡ)
{
	ಸ್ಥಿರಾಂಕ bool oncpu = state_ಮುಖವಾಡ & PSI_ONCPU;

	ಆದರೆ (tasks[NR_IOWAIT]) {
		state_ಮುಖವಾಡ |= ಬಿಟ್(PSI_IO_SOME);
		ಆದರೆ (!tasks[NR_RUNNING])
			state_ಮುಖವಾಡ |= ಬಿಟ್(PSI_IO_ತುಂಬಿದ);
	}

	ಆದರೆ (tasks[NR_MEMSTALL]) {
		state_ಮುಖವಾಡ |= ಬಿಟ್(PSI_MEM_SOME);
		ಆದರೆ (tasks[NR_RUNNING] == tasks[NR_MEMSTALL_RUNNING])
			state_ಮುಖವಾಡ |= ಬಿಟ್(PSI_MEM_ತುಂಬಿದ);
	}

	ಆದರೆ (tasks[NR_RUNNING] > oncpu)
		state_ಮುಖವಾಡ |= ಬಿಟ್(PSI_CPU_SOME);

	ಆದರೆ (tasks[NR_RUNNING] && !oncpu)
		state_ಮುಖವಾಡ |= ಬಿಟ್(PSI_CPU_ತುಂಬಿದ);

	ಆದರೆ (tasks[NR_IOWAIT] || tasks[NR_MEMSTALL] || tasks[NR_RUNNING])
		state_ಮುಖವಾಡ |= ಬಿಟ್(PSI_NONIDLE);

	ಹಿಂತಿರುಗಿ state_ಮುಖವಾಡ;
}

ಸ್ಥಿರ ಶೂನ್ಯ get_recent_times(ರಚನೆ psi_ಗುಂಪು *ಗುಂಪು, ಪೂರ್ಣಾಂಕ cpu,
			     ಎಣಿಕೆ psi_aggregators aggregator, u32 *times,
			     u32 *pchanged_ಸ್ಥಿತಿಗಳು)
{
	ರಚನೆ psi_ಗುಂಪು_cpu *groupc = ಪ್ರತಿ_cpu_ptr(ಗುಂಪು->pcpu, cpu);
	ಪೂರ್ಣಾಂಕ current_cpu = raw_smp_processor_id();
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ tasks[NR_PSI_TASK_COUNTS];
	u64 now, state_ಆರಂಭ;
	ಎಣಿಕೆ psi_ಸ್ಥಿತಿಗಳು s;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ seq;
	u32 state_ಮುಖವಾಡ;

	*pchanged_ಸ್ಥಿತಿಗಳು = 0;

	/* Snapshot a coherent view of the CPU state */
	ಮಾಡು {
		seq = psi_read_begin(cpu);
		now = cpu_clock(cpu);
		ಸ್ಮರಣೆ_ನಕಲು(times, groupc->times, ಗಾತ್ರ(groupc->times));
		state_ಮುಖವಾಡ = groupc->state_ಮುಖವಾಡ;
		state_ಆರಂಭ = groupc->state_ಆರಂಭ;
		ಆದರೆ (cpu == current_cpu)
			ಸ್ಮರಣೆ_ನಕಲು(tasks, groupc->tasks, ಗಾತ್ರ(groupc->tasks));
	} ಆಗ (psi_read_retry(cpu, seq));

	/* Calculate state time deltas against the previous snapshot */
	ಗಾಗಿ (s = 0; s < NR_PSI_ಸ್ಥಿತಿಗಳು; s++) {
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
		ಆದರೆ (state_ಮುಖವಾಡ & (1 << s))
			times[s] += now - state_ಆರಂಭ;

		delta = times[s] - groupc->times_prev[aggregator][s];
		groupc->times_prev[aggregator][s] = times[s];

		times[s] = delta;
		ಆದರೆ (delta)
			*pchanged_ಸ್ಥಿತಿಗಳು |= (1 << s);
	}

	/*
	 * When collect_percpu_times() from the avgs_work, we don't want to
	 * re-arm avgs_work when all CPUs are IDLE. But the current CPU running
	 * this avgs_work is never IDLE, cause avgs_work can't be shut off.
	 * So for the current CPU, we need to re-arm avgs_work only when
	 * (NR_RUNNING > 1 || NR_IOWAIT > 0 || NR_MEMSTALL > 0), for other CPUs
	 * we can just check PSI_NONIDLE delta.
	 */
	ಆದರೆ (current_work() == &ಗುಂಪು->avgs_work.work) {
		bool reschedule;

		ಆದರೆ (cpu == current_cpu)
			reschedule = tasks[NR_RUNNING] +
				     tasks[NR_IOWAIT] +
				     tasks[NR_MEMSTALL] > 1;
		ಇಲ್ಲದಿದ್ದರೆ
			reschedule = *pchanged_ಸ್ಥಿತಿಗಳು & (1 << PSI_NONIDLE);

		ಆದರೆ (reschedule)
			*pchanged_ಸ್ಥಿತಿಗಳು |= PSI_STATE_RESCHEDULE;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ calc_avgs(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ avg[3], ಪೂರ್ಣಾಂಕ missed_periods,
		      u64 ಸಮಯ, u64 period)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ pct;

	/* Fill in zeroes for periods of no activity */
	ಆದರೆ (missed_periods) {
		avg[0] = calc_ತುಂಬು_n(avg[0], EXP_10s, 0, missed_periods);
		avg[1] = calc_ತುಂಬು_n(avg[1], EXP_60s, 0, missed_periods);
		avg[2] = calc_ತುಂಬು_n(avg[2], EXP_300s, 0, missed_periods);
	}

	/* Sample the most recent active period */
	pct = div_u64(ಸಮಯ * 100, period);
	pct *= FIXED_1;
	avg[0] = calc_ತುಂಬು(avg[0], EXP_10s, pct);
	avg[1] = calc_ತುಂಬು(avg[1], EXP_60s, pct);
	avg[2] = calc_ತುಂಬು(avg[2], EXP_300s, pct);
}

ಸ್ಥಿರ ಶೂನ್ಯ collect_percpu_times(ರಚನೆ psi_ಗುಂಪು *ಗುಂಪು,
				 ಎಣಿಕೆ psi_aggregators aggregator,
				 u32 *pchanged_ಸ್ಥಿತಿಗಳು)
{
	u64 deltas[NR_PSI_ಸ್ಥಿತಿಗಳು - 1] = { 0, };
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ nonidle_ಒಟ್ಟು = 0;
	u32 ಬದಲಾಯಿತು_ಸ್ಥಿತಿಗಳು = 0;
	ಪೂರ್ಣಾಂಕ cpu;
	ಪೂರ್ಣಾಂಕ s;

	/*
	 * Collect the per-cpu time buckets and average them into a
	 * single time sample that is normalized to wall clock time.
	 *
	 * For averaging, each CPU is weighted by its non-idle time in
	 * the sampling period. This eliminates artifacts from uneven
	 * loading, or even entirely idle CPUs.
	 */
	for_each_possible_cpu(cpu) {
		u32 times[NR_PSI_ಸ್ಥಿತಿಗಳು];
		u32 nonidle;
		u32 cpu_ಬದಲಾಯಿತು_ಸ್ಥಿತಿಗಳು;

		get_recent_times(ಗುಂಪು, cpu, aggregator, times,
				&cpu_ಬದಲಾಯಿತು_ಸ್ಥಿತಿಗಳು);
		ಬದಲಾಯಿತು_ಸ್ಥಿತಿಗಳು |= cpu_ಬದಲಾಯಿತು_ಸ್ಥಿತಿಗಳು;

		nonidle = nsecs_ಗಮ್ಯ_jiffies(times[PSI_NONIDLE]);
		nonidle_ಒಟ್ಟು += nonidle;

		ಗಾಗಿ (s = 0; s < PSI_NONIDLE; s++)
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
	ಗಾಗಿ (s = 0; s < NR_PSI_ಸ್ಥಿತಿಗಳು - 1; s++)
		ಗುಂಪು->ಒಟ್ಟು[aggregator][s] +=
				div_u64(deltas[s], max(nonidle_ಒಟ್ಟು, 1UL));

	ಆದರೆ (pchanged_ಸ್ಥಿತಿಗಳು)
		*pchanged_ಸ್ಥಿತಿಗಳು = ಬದಲಾಯಿತು_ಸ್ಥಿತಿಗಳು;
}

/* Trigger tracking window manipulations */
ಸ್ಥಿರ ಶೂನ್ಯ ಕಿಟಕಿ_reset(ರಚನೆ psi_ಕಿಟಕಿ *win, u64 now, u64 ಮೌಲ್ಯ,
			 u64 prev_growth)
{
	win->ಆರಂಭ_ಸಮಯ = now;
	win->ಆರಂಭ_ಮೌಲ್ಯ = ಮೌಲ್ಯ;
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
ಸ್ಥಿರ u64 ಕಿಟಕಿ_update(ರಚನೆ psi_ಕಿಟಕಿ *win, u64 now, u64 ಮೌಲ್ಯ)
{
	u64 elapsed;
	u64 growth;

	elapsed = now - win->ಆರಂಭ_ಸಮಯ;
	growth = ಮೌಲ್ಯ - win->ಆರಂಭ_ಮೌಲ್ಯ;
	/*
	 * After each tracking window passes win->start_value and
	 * win->start_time get reset and win->prev_growth stores
	 * the average per-window growth of the previous window.
	 * win->prev_growth is then used to interpolate additional
	 * growth from the previous window assuming it was linear.
	 */
	ಆದರೆ (elapsed > win->ಗಾತ್ರ_2)
		ಕಿಟಕಿ_reset(win, now, ಮೌಲ್ಯ, growth);
	ಇಲ್ಲದಿದ್ದರೆ {
		u32 ಉಳಿದ;

		ಉಳಿದ = win->ಗಾತ್ರ_2 - elapsed;
		growth += div64_u64(win->prev_growth * ಉಳಿದ, win->ಗಾತ್ರ_2);
	}

	ಹಿಂತಿರುಗಿ growth;
}

ಸ್ಥಿರ ಶೂನ್ಯ update_triggers(ರಚನೆ psi_ಗುಂಪು *ಗುಂಪು, u64 now,
						   ಎಣಿಕೆ psi_aggregators aggregator)
{
	ರಚನೆ psi_trigger *t;
	u64 *ಒಟ್ಟು = ಗುಂಪು->ಒಟ್ಟು[aggregator];
	ರಚನೆ list_ತಲೆ *triggers;
	u64 *aggregator_ಒಟ್ಟು;

	ಆದರೆ (aggregator == PSI_AVGS) {
		triggers = &ಗುಂಪು->avg_triggers;
		aggregator_ಒಟ್ಟು = ಗುಂಪು->avg_ಒಟ್ಟು;
	} ಇಲ್ಲದಿದ್ದರೆ {
		triggers = &ಗುಂಪು->rtpoll_triggers;
		aggregator_ಒಟ್ಟು = ಗುಂಪು->rtpoll_ಒಟ್ಟು;
	}

	/*
	 * On subsequent updates, calculate growth deltas and let
	 * watchers know when their specified thresholds are exceeded.
	 */
	list_for_each_ನಮೂದು(t, triggers, ಗಂಟು) {
		u64 growth;
		bool new_stall;

		new_stall = aggregator_ಒಟ್ಟು[t->state] != ಒಟ್ಟು[t->state];

		/* Check for stall activity or a previous threshold breach */
		ಆದರೆ (!new_stall && !t->pending_event)
			ಮುಂದುವರಿ;
		/*
		 * Check for new stall activity, as well as deferred
		 * events that occurred in the last window after the
		 * trigger had already fired (we want to ratelimit
		 * events without dropping any).
		 */
		ಆದರೆ (new_stall) {
			/* Calculate growth since last update */
			growth = ಕಿಟಕಿ_update(&t->win, now, ಒಟ್ಟು[t->state]);
			ಆದರೆ (!t->pending_event) {
				ಆದರೆ (growth < t->ಮಿತಿ)
					ಮುಂದುವರಿ;

				t->pending_event = true;
			}
		}
		/* Limit event signaling to once per window */
		ಆದರೆ (now < t->ಕೊನೆಯ_event_ಸಮಯ + t->win.ಗಾತ್ರ_2)
			ಮುಂದುವರಿ;

		/* Generate an event */
		ಆದರೆ (cmpxchg(&t->event, 0, 1) == 0) {
			ಆದರೆ (t->ದ)
				kernfs_notify(t->ದ->kn);
			ಇಲ್ಲದಿದ್ದರೆ
				wake_ಮೇಲೆ_interruptible(&t->event_wait);
		}
		t->ಕೊನೆಯ_event_ಸಮಯ = now;
		/* Reset threshold breach flag once event got generated */
		t->pending_event = false;
	}
}

ಸ್ಥಿರ u64 update_averages(ರಚನೆ psi_ಗುಂಪು *ಗುಂಪು, u64 now)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ missed_periods = 0;
	u64 expires, period;
	u64 avg_ಮುಂದಿನ_update;
	ಪೂರ್ಣಾಂಕ s;

	/* avgX= */
	expires = ಗುಂಪು->avg_ಮುಂದಿನ_update;
	ಆದರೆ (now - expires >= psi_period)
		missed_periods = div_u64(now - expires, psi_period);

	/*
	 * The periodic clock tick can get delayed for various
	 * reasons, especially on loaded systems. To avoid clock
	 * drift, we schedule the clock in fixed psi_period intervals.
	 * But the deltas we sample out of the per-cpu buckets above
	 * are based on the actual time elapsing between clock ticks.
	 */
	avg_ಮುಂದಿನ_update = expires + ((1 + missed_periods) * psi_period);
	period = now - (ಗುಂಪು->avg_ಕೊನೆಯ_update + (missed_periods * psi_period));
	ಗುಂಪು->avg_ಕೊನೆಯ_update = now;

	ಗಾಗಿ (s = 0; s < NR_PSI_ಸ್ಥಿತಿಗಳು - 1; s++) {
		u32 sample;

		sample = ಗುಂಪು->ಒಟ್ಟು[PSI_AVGS][s] - ಗುಂಪು->avg_ಒಟ್ಟು[s];
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
		ಆದರೆ (sample > period)
			sample = period;
		ಗುಂಪು->avg_ಒಟ್ಟು[s] += sample;
		calc_avgs(ಗುಂಪು->avg[s], missed_periods, sample, period);
	}

	ಹಿಂತಿರುಗಿ avg_ಮುಂದಿನ_update;
}

ಸ್ಥಿರ ಶೂನ್ಯ psi_avgs_work(ರಚನೆ work_struct *work)
{
	ರಚನೆ delayed_work *dwork;
	ರಚನೆ psi_ಗುಂಪು *ಗುಂಪು;
	u32 ಬದಲಾಯಿತು_ಸ್ಥಿತಿಗಳು;
	u64 now;

	dwork = ಗಮ್ಯ_delayed_work(work);
	ಗುಂಪು = container_ದ(dwork, ರಚನೆ psi_ಗುಂಪು, avgs_work);

	mutex_ಬೀಗ(&ಗುಂಪು->avgs_ಬೀಗ);

	now = sched_clock();

	collect_percpu_times(ಗುಂಪು, PSI_AVGS, &ಬದಲಾಯಿತು_ಸ್ಥಿತಿಗಳು);
	/*
	 * If there is task activity, periodically fold the per-cpu
	 * times and feed samples into the running averages. If things
	 * are idle and there is no data to process, stop the clock.
	 * Once restarted, we'll catch up the running averages in one
	 * go - see calc_avgs() and missed_periods.
	 */
	ಆದರೆ (now >= ಗುಂಪು->avg_ಮುಂದಿನ_update) {
		update_triggers(ಗುಂಪು, now, PSI_AVGS);
		ಗುಂಪು->avg_ಮುಂದಿನ_update = update_averages(ಗುಂಪು, now);
	}

	ಆದರೆ (ಬದಲಾಯಿತು_ಸ್ಥಿತಿಗಳು & PSI_STATE_RESCHEDULE) {
		ವೇಳಾಪಟ್ಟಿ_delayed_work(dwork, nsecs_ಗಮ್ಯ_jiffies(
				ಗುಂಪು->avg_ಮುಂದಿನ_update - now) + 1);
	}

	mutex_unlock(&ಗುಂಪು->avgs_ಬೀಗ);
}

ಸ್ಥಿರ ಶೂನ್ಯ init_rtpoll_triggers(ರಚನೆ psi_ಗುಂಪು *ಗುಂಪು, u64 now)
{
	ರಚನೆ psi_trigger *t;

	list_for_each_ನಮೂದು(t, &ಗುಂಪು->rtpoll_triggers, ಗಂಟು)
		ಕಿಟಕಿ_reset(&t->win, now,
				ಗುಂಪು->ಒಟ್ಟು[PSI_POLL][t->state], 0);
	ಸ್ಮರಣೆ_ನಕಲು(ಗುಂಪು->rtpoll_ಒಟ್ಟು, ಗುಂಪು->ಒಟ್ಟು[PSI_POLL],
		   ಗಾತ್ರ(ಗುಂಪು->rtpoll_ಒಟ್ಟು));
	ಗುಂಪು->rtpoll_ಮುಂದಿನ_update = now + ಗುಂಪು->rtpoll_min_period;
}

/* Schedule rtpolling if it's not already scheduled or forced. */
ಸ್ಥಿರ ಶೂನ್ಯ psi_ವೇಳಾಪಟ್ಟಿ_rtpoll_work(ರಚನೆ psi_ಗುಂಪು *ಗುಂಪು, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ delay,
				   bool force)
{
	/*
	 * atomic_xchg should be called even when !force to provide a
	 * full memory barrier (see the comment inside psi_rtpoll_work).
	 */
	ಆದರೆ (atomic_xchg(&ಗುಂಪು->rtpoll_scheduled, 1) && !force)
		ಹಿಂತಿರುಗಿ;

	/*
	 * Only test whether a worker is installed; do not dereference the task.
	 * A racing trigger destruction may leave the timer armed.
	 * psi_cgroup_free() shuts it down before freeing the group.
	 * The system PSI group is static.
	 */
	ಆದರೆ (likely(rcu_access_ಸೂಚಕ(ಗುಂಪು->rtpoll_task)))
		mod_timer(&ಗುಂಪು->rtpoll_timer, jiffies + delay);
	ಇಲ್ಲದಿದ್ದರೆ
		atomic_ಗಣ(&ಗುಂಪು->rtpoll_scheduled, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ psi_rtpoll_work(ರಚನೆ psi_ಗುಂಪು *ಗುಂಪು)
{
	bool force_reschedule = false;
	u32 ಬದಲಾಯಿತು_ಸ್ಥಿತಿಗಳು;
	u64 now;

	mutex_ಬೀಗ(&ಗುಂಪು->rtpoll_trigger_ಬೀಗ);

	now = sched_clock();

	ಆದರೆ (now > ಗುಂಪು->rtpoll_until) {
		/*
		 * We are either about to start or might stop rtpolling if no
		 * state change was recorded. Resetting rtpoll_scheduled leaves
		 * a small window for psi_group_change to sneak in and schedule
		 * an immediate rtpoll_work before we get to rescheduling. One
		 * potential extra wakeup at the end of the rtpolling window
		 * should be negligible and rtpoll_next_update still keeps
		 * updates correctly on schedule.
		 */
		atomic_ಗಣ(&ಗುಂಪು->rtpoll_scheduled, 0);
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
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* The rtpolling window is not over, keep rescheduling */
		force_reschedule = true;
	}


	collect_percpu_times(ಗುಂಪು, PSI_POLL, &ಬದಲಾಯಿತು_ಸ್ಥಿತಿಗಳು);

	ಆದರೆ (ಬದಲಾಯಿತು_ಸ್ಥಿತಿಗಳು & ಗುಂಪು->rtpoll_ಸ್ಥಿತಿಗಳು) {
		/* Initialize trigger windows when entering rtpolling mode */
		ಆದರೆ (now > ಗುಂಪು->rtpoll_until)
			init_rtpoll_triggers(ಗುಂಪು, now);

		/*
		 * Keep the monitor active for at least the duration of the
		 * minimum tracking window as long as monitor states are
		 * changing.
		 */
		ಗುಂಪು->rtpoll_until = now +
			ಗುಂಪು->rtpoll_min_period * UPDATES_ಪ್ರತಿ_ಕಿಟಕಿ;
	}

	ಆದರೆ (now > ಗುಂಪು->rtpoll_until) {
		ಗುಂಪು->rtpoll_ಮುಂದಿನ_update = ULLONG_MAX;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (now >= ಗುಂಪು->rtpoll_ಮುಂದಿನ_update) {
		ಆದರೆ (ಬದಲಾಯಿತು_ಸ್ಥಿತಿಗಳು & ಗುಂಪು->rtpoll_ಸ್ಥಿತಿಗಳು) {
			update_triggers(ಗುಂಪು, now, PSI_POLL);
			ಸ್ಮರಣೆ_ನಕಲು(ಗುಂಪು->rtpoll_ಒಟ್ಟು, ಗುಂಪು->ಒಟ್ಟು[PSI_POLL],
				   ಗಾತ್ರ(ಗುಂಪು->rtpoll_ಒಟ್ಟು));
		}
		ಗುಂಪು->rtpoll_ಮುಂದಿನ_update = now + ಗುಂಪು->rtpoll_min_period;
	}

	psi_ವೇಳಾಪಟ್ಟಿ_rtpoll_work(ಗುಂಪು,
		nsecs_ಗಮ್ಯ_jiffies(ಗುಂಪು->rtpoll_ಮುಂದಿನ_update - now) + 1,
		force_reschedule);

ಹೊರಹೋಗು:
	mutex_unlock(&ಗುಂಪು->rtpoll_trigger_ಬೀಗ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ psi_rtpoll_ಕೆಲಸಗಾರ(ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ psi_ಗುಂಪು *ಗುಂಪು = (ರಚನೆ psi_ಗುಂಪು *)ದತ್ತಾಂಶ;

	sched_ಗಣ_fifo_ಕೆಳಗಿನ(current);

	ಆಗ (true) {
		wait_event_interruptible(ಗುಂಪು->rtpoll_wait,
				atomic_cmpxchg(&ಗುಂಪು->rtpoll_wakeup, 1, 0) ||
				kthread_should_stop());
		ಆದರೆ (kthread_should_stop())
			ನಿಲ್ಲಿಸು;

		psi_rtpoll_work(ಗುಂಪು);
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ poll_timer_fn(ರಚನೆ timer_list *t)
{
	ರಚನೆ psi_ಗುಂಪು *ಗುಂಪು = timer_container_ದ(ಗುಂಪು, t, rtpoll_timer);

	atomic_ಗಣ(&ಗುಂಪು->rtpoll_wakeup, 1);
	wake_ಮೇಲೆ_interruptible(&ಗುಂಪು->rtpoll_wait);
}

ಸ್ಥಿರ ಶೂನ್ಯ ದಾಖಲೆ_times(ರಚನೆ psi_ಗುಂಪು_cpu *groupc, u64 now)
{
	u32 delta;

	delta = now - groupc->state_ಆರಂಭ;
	groupc->state_ಆರಂಭ = now;

	ಆದರೆ (groupc->state_ಮುಖವಾಡ & (1 << PSI_IO_SOME)) {
		groupc->times[PSI_IO_SOME] += delta;
		ಆದರೆ (groupc->state_ಮುಖವಾಡ & (1 << PSI_IO_ತುಂಬಿದ))
			groupc->times[PSI_IO_ತುಂಬಿದ] += delta;
	}

	ಆದರೆ (groupc->state_ಮುಖವಾಡ & (1 << PSI_MEM_SOME)) {
		groupc->times[PSI_MEM_SOME] += delta;
		ಆದರೆ (groupc->state_ಮುಖವಾಡ & (1 << PSI_MEM_ತುಂಬಿದ))
			groupc->times[PSI_MEM_ತುಂಬಿದ] += delta;
	}

	ಆದರೆ (groupc->state_ಮುಖವಾಡ & (1 << PSI_CPU_SOME)) {
		groupc->times[PSI_CPU_SOME] += delta;
		ಆದರೆ (groupc->state_ಮುಖವಾಡ & (1 << PSI_CPU_ತುಂಬಿದ))
			groupc->times[PSI_CPU_ತುಂಬಿದ] += delta;
	}

	ಆದರೆ (groupc->state_ಮುಖವಾಡ & (1 << PSI_NONIDLE))
		groupc->times[PSI_NONIDLE] += delta;
}

#ವ್ಯಾಖ್ಯಾನಿಸು for_each_ಗುಂಪು(iter, ಗುಂಪು) \
	ಗಾಗಿ (typeof(ಗುಂಪು) iter = ಗುಂಪು; iter; iter = iter->ಪೋಷಕ)

ಸ್ಥಿರ ಶೂನ್ಯ psi_ಗುಂಪು_change(ರಚನೆ psi_ಗುಂಪು *ಗುಂಪು, ಪೂರ್ಣಾಂಕ cpu,
			     ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ clear, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಗಣ,
			     u64 now, bool wake_clock)
{
	ರಚನೆ psi_ಗುಂಪು_cpu *groupc;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ t, m;
	u32 state_ಮುಖವಾಡ;

	lockdep_assert_rq_held(cpu_rq(cpu));
	groupc = ಪ್ರತಿ_cpu_ptr(ಗುಂಪು->pcpu, cpu);

	/*
	 * Start with TSK_ONCPU, which doesn't have a corresponding
	 * task count - it's just a boolean flag directly encoded in
	 * the state mask. Clear, set, or carry the current state if
	 * no changes are requested.
	 */
	ಆದರೆ (unlikely(clear & TSK_ONCPU)) {
		state_ಮುಖವಾಡ = 0;
		clear &= ~TSK_ONCPU;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (unlikely(ಗಣ & TSK_ONCPU)) {
		state_ಮುಖವಾಡ = PSI_ONCPU;
		ಗಣ &= ~TSK_ONCPU;
	} ಇಲ್ಲದಿದ್ದರೆ {
		state_ಮುಖವಾಡ = groupc->state_ಮುಖವಾಡ & PSI_ONCPU;
	}

	/*
	 * The rest of the state mask is calculated based on the task
	 * counts. Update those first, then construct the mask.
	 */
	ಗಾಗಿ (t = 0, m = clear; m; m &= ~(1 << t), t++) {
		ಆದರೆ (!(m & (1 << t)))
			ಮುಂದುವರಿ;
		ಆದರೆ (groupc->tasks[t]) {
			groupc->tasks[t]--;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!psi_bug) {
			printk_deferred(KERN_ERR "psi: task underflow! cpu=%d t=%d tasks=[%u %u %u %u] clear=%x set=%x\n",
					cpu, t, groupc->tasks[0],
					groupc->tasks[1], groupc->tasks[2],
					groupc->tasks[3], clear, ಗಣ);
			psi_bug = 1;
		}
	}

	ಗಾಗಿ (t = 0; ಗಣ; ಗಣ &= ~(1 << t), t++)
		ಆದರೆ (ಗಣ & (1 << t))
			groupc->tasks[t]++;

	ಆದರೆ (!ಗುಂಪು->enabled) {
		/*
		 * On the first group change after disabling PSI, conclude
		 * the current state and flush its time. This is unlikely
		 * to matter to the user, but aggregation (get_recent_times)
		 * may have already incorporated the live state into times_prev;
		 * avoid a delta sample underflow when PSI is later re-enabled.
		 */
		ಆದರೆ (unlikely(groupc->state_ಮುಖವಾಡ & (1 << PSI_NONIDLE)))
			ದಾಖಲೆ_times(groupc, now);

		groupc->state_ಮುಖವಾಡ = state_ಮುಖವಾಡ;

		ಹಿಂತಿರುಗಿ;
	}

	state_ಮುಖವಾಡ = test_ಸ್ಥಿತಿಗಳು(groupc->tasks, state_ಮುಖವಾಡ);

	/*
	 * Since we care about lost potential, a memstall is FULL
	 * when there are no other working tasks, but also when
	 * the CPU is actively reclaiming and nothing productive
	 * could run even if it were runnable. So when the current
	 * task in a cgroup is in_memstall, the corresponding groupc
	 * on that cpu is in PSI_MEM_FULL state.
	 */
	ಆದರೆ (unlikely((state_ಮುಖವಾಡ & PSI_ONCPU) && cpu_curr(cpu)->ಒಳ_memstall))
		state_ಮುಖವಾಡ |= (1 << PSI_MEM_ತುಂಬಿದ);

	ದಾಖಲೆ_times(groupc, now);

	groupc->state_ಮುಖವಾಡ = state_ಮುಖವಾಡ;

	ಆದರೆ (state_ಮುಖವಾಡ & ಗುಂಪು->rtpoll_ಸ್ಥಿತಿಗಳು)
		psi_ವೇಳಾಪಟ್ಟಿ_rtpoll_work(ಗುಂಪು, 1, false);

	ಆದರೆ (wake_clock && !delayed_work_pending(&ಗುಂಪು->avgs_work))
		ವೇಳಾಪಟ್ಟಿ_delayed_work(&ಗುಂಪು->avgs_work, PSI_FREQ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ psi_ಗುಂಪು *task_psi_ಗುಂಪು(ರಚನೆ task_struct *task)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CGROUPS
	ಆದರೆ (static_branch_likely(&psi_cgroups_enabled))
		ಹಿಂತಿರುಗಿ cgroup_psi(task_dfl_cgroup(task));
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ &psi_system;
}

ಸ್ಥಿರ ಶೂನ್ಯ psi_flags_change(ರಚನೆ task_struct *task, ಪೂರ್ಣಾಂಕ clear, ಪೂರ್ಣಾಂಕ ಗಣ)
{
	ಆದರೆ (((task->psi_flags & ಗಣ) ||
	     (task->psi_flags & clear) != clear) &&
	    !psi_bug) {
		printk_deferred(KERN_ERR "psi: inconsistent task state! task=%d:%s cpu=%d psi_flags=%x clear=%x set=%x\n",
				task->pid, task->comm, task_cpu(task),
				task->psi_flags, clear, ಗಣ);
		psi_bug = 1;
	}

	task->psi_flags &= ~clear;
	task->psi_flags |= ಗಣ;
}

ಶೂನ್ಯ psi_task_change(ರಚನೆ task_struct *task, ಪೂರ್ಣಾಂಕ clear, ಪೂರ್ಣಾಂಕ ಗಣ)
{
	ಪೂರ್ಣಾಂಕ cpu = task_cpu(task);
	u64 now;

	ಆದರೆ (!task->pid)
		ಹಿಂತಿರುಗಿ;

	psi_flags_change(task, clear, ಗಣ);

	psi_write_begin(cpu);
	now = cpu_clock(cpu);
	for_each_ಗುಂಪು(ಗುಂಪು, task_psi_ಗುಂಪು(task))
		psi_ಗುಂಪು_change(ಗುಂಪು, cpu, clear, ಗಣ, now, true);
	psi_write_ಕೊನೆ(cpu);
}

ಶೂನ್ಯ psi_task_switch(ರಚನೆ task_struct *prev, ರಚನೆ task_struct *ಮುಂದಿನ,
		     bool sleep)
{
	ರಚನೆ psi_ಗುಂಪು *common = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ cpu = task_cpu(prev);
	u64 now;

	psi_write_begin(cpu);
	now = cpu_clock(cpu);

	ಆದರೆ (ಮುಂದಿನ->pid) {
		psi_flags_change(ಮುಂದಿನ, 0, TSK_ONCPU);
		/*
		 * Set TSK_ONCPU on @next's cgroups. If @next shares any
		 * ancestors with @prev, those will already have @prev's
		 * TSK_ONCPU bit set, and we can stop the iteration there.
		 */
		for_each_ಗುಂಪು(ಗುಂಪು, task_psi_ಗುಂಪು(ಮುಂದಿನ)) {
			ರಚನೆ psi_ಗುಂಪು_cpu *groupc = ಪ್ರತಿ_cpu_ptr(ಗುಂಪು->pcpu, cpu);

			ಆದರೆ (groupc->state_ಮುಖವಾಡ & PSI_ONCPU) {
				common = ಗುಂಪು;
				ನಿಲ್ಲಿಸು;
			}
			psi_ಗುಂಪು_change(ಗುಂಪು, cpu, 0, TSK_ONCPU, now, true);
		}
	}

	ಆದರೆ (prev->pid) {
		ಪೂರ್ಣಾಂಕ clear = TSK_ONCPU, ಗಣ = 0;
		bool wake_clock = true;

		/*
		 * When we're going to sleep, psi_dequeue() lets us
		 * handle TSK_RUNNING, TSK_MEMSTALL_RUNNING and
		 * TSK_IOWAIT here, where we can combine it with
		 * TSK_ONCPU and save walking common ancestors twice.
		 */
		ಆದರೆ (sleep) {
			clear |= TSK_RUNNING;
			ಆದರೆ (prev->ಒಳ_memstall)
				clear |= TSK_MEMSTALL_RUNNING;
			ಆದರೆ (prev->ಒಳ_iowait)
				ಗಣ |= TSK_IOWAIT;

			/*
			 * Periodic aggregation shuts off if there is a period of no
			 * task changes, so we wake it back up if necessary. However,
			 * don't do this if the task change is the aggregation worker
			 * itself going to sleep, or we'll ping-pong forever.
			 */
			ಆದರೆ (unlikely((prev->flags & PF_WQ_ಕೆಲಸಗಾರ) &&
				     wq_ಕೆಲಸಗಾರ_ಕೊನೆಯ_func(prev) == psi_avgs_work))
				wake_clock = false;
		}

		psi_flags_change(prev, clear, ಗಣ);

		for_each_ಗುಂಪು(ಗುಂಪು, task_psi_ಗುಂಪು(prev)) {
			ಆದರೆ (ಗುಂಪು == common)
				ನಿಲ್ಲಿಸು;
			psi_ಗುಂಪು_change(ಗುಂಪು, cpu, clear, ಗಣ, now, wake_clock);
		}

		/*
		 * TSK_ONCPU is handled up to the common ancestor. If there are
		 * any other differences between the two tasks (e.g. prev goes
		 * to sleep, or only one task is memstall), finish propagating
		 * those differences all the way up to the root.
		 */
		ಆದರೆ ((prev->psi_flags ^ ಮುಂದಿನ->psi_flags) & ~TSK_ONCPU) {
			clear &= ~TSK_ONCPU;
			for_each_ಗುಂಪು(ಗುಂಪು, common)
				psi_ಗುಂಪು_change(ಗುಂಪು, cpu, clear, ಗಣ, now, wake_clock);
		}
	}
	psi_write_ಕೊನೆ(cpu);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_IRQ_ಸಮಯ_ACCOUNTING
ಶೂನ್ಯ psi_account_irqtime(ರಚನೆ rq *rq, ರಚನೆ task_struct *curr, ರಚನೆ task_struct *prev)
{
	ಪೂರ್ಣಾಂಕ cpu = task_cpu(curr);
	ರಚನೆ psi_ಗುಂಪು_cpu *groupc;
	s64 delta;
	u64 irq;
	u64 now;

	ಆದರೆ (static_branch_likely(&psi_disabled) || !irqtime_enabled())
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!curr->pid)
		ಹಿಂತಿರುಗಿ;

	lockdep_assert_rq_held(rq);
	ಆದರೆ (prev && task_psi_ಗುಂಪು(prev) == task_psi_ಗುಂಪು(curr))
		ಹಿಂತಿರುಗಿ;

	irq = irq_ಸಮಯ_read(cpu);
	delta = (s64)(irq - rq->psi_irq_ಸಮಯ);
	ಆದರೆ (delta <= 0)
		ಹಿಂತಿರುಗಿ;
	rq->psi_irq_ಸಮಯ = irq;

	psi_write_begin(cpu);
	now = cpu_clock(cpu);

	for_each_ಗುಂಪು(ಗುಂಪು, task_psi_ಗುಂಪು(curr)) {
		ಆದರೆ (!ಗುಂಪು->enabled)
			ಮುಂದುವರಿ;

		groupc = ಪ್ರತಿ_cpu_ptr(ಗುಂಪು->pcpu, cpu);

		ದಾಖಲೆ_times(groupc, now);
		groupc->times[PSI_IRQ_ತುಂಬಿದ] += delta;

		ಆದರೆ (ಗುಂಪು->rtpoll_ಸ್ಥಿತಿಗಳು & (1 << PSI_IRQ_ತುಂಬಿದ))
			psi_ವೇಳಾಪಟ್ಟಿ_rtpoll_work(ಗುಂಪು, 1, false);
	}
	psi_write_ಕೊನೆ(cpu);
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_IRQ_TIME_ACCOUNTING */

/**
 * psi_memstall_enter - mark the beginning of a memory stall section
 * @flags: flags to handle nested sections
 *
 * Marks the calling task as being stalled due to a lack of memory,
 * such as waiting for a refault or performing reclaim.
 */
ಶೂನ್ಯ psi_memstall_enter(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *flags)
{
	ರಚನೆ rq_flags rf;
	ರಚನೆ rq *rq;

	ಆದರೆ (static_branch_likely(&psi_disabled))
		ಹಿಂತಿರುಗಿ;

	*flags = current->ಒಳ_memstall;
	ಆದರೆ (*flags)
		ಹಿಂತಿರುಗಿ;
	/*
	 * in_memstall setting & accounting needs to be atomic wrt
	 * changes to the task's scheduling state, otherwise we can
	 * race with CPU migration.
	 */
	rq = this_rq_ಬೀಗ_irq(&rf);

	current->ಒಳ_memstall = 1;
	psi_task_change(current, 0, TSK_MEMSTALL | TSK_MEMSTALL_RUNNING);

	rq_unlock_irq(rq, &rf);
}
EXPORT_ಚಿಹ್ನೆ_GPL(psi_memstall_enter);

/**
 * psi_memstall_leave - mark the end of an memory stall section
 * @flags: flags to handle nested memdelay sections
 *
 * Marks the calling task as no longer stalled due to lack of memory.
 */
ಶೂನ್ಯ psi_memstall_leave(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *flags)
{
	ರಚನೆ rq_flags rf;
	ರಚನೆ rq *rq;

	ಆದರೆ (static_branch_likely(&psi_disabled))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (*flags)
		ಹಿಂತಿರುಗಿ;
	/*
	 * in_memstall clearing & accounting needs to be atomic wrt
	 * changes to the task's scheduling state, otherwise we could
	 * race with CPU migration.
	 */
	rq = this_rq_ಬೀಗ_irq(&rf);

	current->ಒಳ_memstall = 0;
	psi_task_change(current, TSK_MEMSTALL | TSK_MEMSTALL_RUNNING, 0);

	rq_unlock_irq(rq, &rf);
}
EXPORT_ಚಿಹ್ನೆ_GPL(psi_memstall_leave);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CGROUPS
ಪೂರ್ಣಾಂಕ psi_cgroup_alloc(ರಚನೆ cgroup *cgroup)
{
	ಆದರೆ (!static_branch_likely(&psi_cgroups_enabled))
		ಹಿಂತಿರುಗಿ 0;

	cgroup->psi = kzalloc_obj(ರಚನೆ psi_ಗುಂಪು);
	ಆದರೆ (!cgroup->psi)
		ಹಿಂತಿರುಗಿ -ENOMEM;

	cgroup->psi->pcpu = alloc_percpu(ರಚನೆ psi_ಗುಂಪು_cpu);
	ಆದರೆ (!cgroup->psi->pcpu) {
		kfree(cgroup->psi);
		ಹಿಂತಿರುಗಿ -ENOMEM;
	}
	ಗುಂಪು_init(cgroup->psi);
	cgroup->psi->ಪೋಷಕ = cgroup_psi(cgroup_ಪೋಷಕ(cgroup));
	ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ psi_cgroup_free(ರಚನೆ cgroup *cgroup)
{
	ಆದರೆ (!static_branch_likely(&psi_cgroups_enabled))
		ಹಿಂತಿರುಗಿ;

	cancel_delayed_work_sync(&cgroup->psi->avgs_work);
	/*
	 * A psi_schedule_rtpoll_work() call racing the last trigger's
	 * destruction may have re-armed the timer after psi_trigger_destroy()
	 * deleted it. Spurious firing while the group is alive is harmless.
	 */
	timer_shutdown_sync(&cgroup->psi->rtpoll_timer);
	free_percpu(cgroup->psi->pcpu);
	/* All triggers must be removed by now */
	WARN_ONCE(cgroup->psi->rtpoll_ಸ್ಥಿತಿಗಳು, "psi: trigger leak\n");
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
ಶೂನ್ಯ cgroup_move_task(ರಚನೆ task_struct *task, ರಚನೆ css_ಗಣ *ಗಮ್ಯ)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ task_flags;
	ರಚನೆ rq_flags rf;
	ರಚನೆ rq *rq;

	ಆದರೆ (!static_branch_likely(&psi_cgroups_enabled)) {
		/*
		 * Lame to do this here, but the scheduler cannot be locked
		 * from the outside, so we move cgroups from inside sched/.
		 */
		rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(task->cgroups, ಗಮ್ಯ);
		ಹಿಂತಿರುಗಿ;
	}

	rq = task_rq_ಬೀಗ_2(task, &rf);

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

	ಆದರೆ (task_flags)
		psi_task_change(task, task_flags, 0);

	/* See comment above */
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(task->cgroups, ಗಮ್ಯ);

	ಆದರೆ (task_flags)
		psi_task_change(task, 0, task_flags);

	task_rq_unlock(rq, task, &rf);
}

ಶೂನ್ಯ psi_cgroup_restart(ರಚನೆ psi_ಗುಂಪು *ಗುಂಪು)
{
	ಪೂರ್ಣಾಂಕ cpu;

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
	ಆದರೆ (!ಗುಂಪು->enabled)
		ಹಿಂತಿರುಗಿ;

	for_each_possible_cpu(cpu) {
		u64 now;

		guard(rq_ಬೀಗ_irq)(cpu_rq(cpu));

		psi_write_begin(cpu);
		now = cpu_clock(cpu);
		psi_ಗುಂಪು_change(ಗುಂಪು, cpu, 0, 0, now, true);
		psi_write_ಕೊನೆ(cpu);
	}
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_CGROUPS */

ಪೂರ್ಣಾಂಕ psi_show(ರಚನೆ seq_file *m, ರಚನೆ psi_ಗುಂಪು *ಗುಂಪು, ಎಣಿಕೆ psi_res res)
{
	bool only_ತುಂಬಿದ = false;
	ಪೂರ್ಣಾಂಕ ತುಂಬಿದ;
	u64 now;

	ಆದರೆ (static_branch_likely(&psi_disabled))
		ಹಿಂತಿರುಗಿ -EOPNOTSUPP;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_IRQ_ಸಮಯ_ACCOUNTING
	ಆದರೆ (!irqtime_enabled() && res == PSI_IRQ)
		ಹಿಂತಿರುಗಿ -EOPNOTSUPP;
#ಆದರೆ_ಅಂತ್ಯ

	/* Update averages before reporting them */
	mutex_ಬೀಗ(&ಗುಂಪು->avgs_ಬೀಗ);
	now = sched_clock();
	collect_percpu_times(ಗುಂಪು, PSI_AVGS, ಖಾಲಿ);
	ಆದರೆ (now >= ಗುಂಪು->avg_ಮುಂದಿನ_update)
		ಗುಂಪು->avg_ಮುಂದಿನ_update = update_averages(ಗುಂಪು, now);
	mutex_unlock(&ಗುಂಪು->avgs_ಬೀಗ);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_IRQ_ಸಮಯ_ACCOUNTING
	only_ತುಂಬಿದ = res == PSI_IRQ;
#ಆದರೆ_ಅಂತ್ಯ

	ಗಾಗಿ (ತುಂಬಿದ = 0; ತುಂಬಿದ < 2 - only_ತುಂಬಿದ; ತುಂಬಿದ++) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ avg[3] = { 0, };
		u64 ಒಟ್ಟು = 0;
		ಪೂರ್ಣಾಂಕ w;

		/* CPU FULL is undefined at the system level */
		ಆದರೆ (!(ಗುಂಪು == &psi_system && res == PSI_CPU && ತುಂಬಿದ)) {
			ಗಾಗಿ (w = 0; w < 3; w++)
				avg[w] = ಗುಂಪು->avg[res * 2 + ತುಂಬಿದ][w];
			ಒಟ್ಟು = div_u64(ಗುಂಪು->ಒಟ್ಟು[PSI_AVGS][res * 2 + ತುಂಬಿದ],
					NSEC_ಪ್ರತಿ_USEC);
		}

		seq_printf(m, "%s avg10=%lu.%02lu avg60=%lu.%02lu avg300=%lu.%02lu total=%llu\n",
			   ತುಂಬಿದ || only_ತುಂಬಿದ ? "full" : "some",
			   ತುಂಬು_INT(avg[0]), ತುಂಬು_FRAC(avg[0]),
			   ತುಂಬು_INT(avg[1]), ತುಂಬು_FRAC(avg[1]),
			   ತುಂಬು_INT(avg[2]), ತುಂಬು_FRAC(avg[2]),
			   ಒಟ್ಟು);
	}

	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Create @group's rtpoll worker after psi_trigger_create() reported the need
 * for one. kthread creation depends on the whole fork path and we don't want
 * all of that nested inside cgroup_mutex, so the caller must drop it and any
 * other lock that forks can wait behind. If two callers race, the loser stops
 * its never-woken kthread.
 */
ಪೂರ್ಣಾಂಕ psi_trigger_create_rtpoll_ಕೆಲಸಗಾರ(ರಚನೆ psi_ಗುಂಪು *ಗುಂಪು)
{
	ರಚನೆ task_struct *task;

	task = kthread_create(psi_rtpoll_ಕೆಲಸಗಾರ, ಗುಂಪು, "psimon");
	ಆದರೆ (IS_ERR(task))
		ಹಿಂತಿರುಗಿ PTR_ERR(task);

	scoped_guard(mutex, &ಗುಂಪು->rtpoll_trigger_ಬೀಗ) {
		ಆದರೆ (!rcu_access_ಸೂಚಕ(ಗುಂಪು->rtpoll_task)) {
			atomic_ಗಣ(&ಗುಂಪು->rtpoll_wakeup, 0);
			wake_ಮೇಲೆ_process(task);
			rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(ಗುಂಪು->rtpoll_task, task);

			/*
			 * Poll once to catch up on scheduling attempts dropped
			 * while there was no rtpoll worker.
			 */
			psi_ವೇಳಾಪಟ್ಟಿ_rtpoll_work(ಗುಂಪು, 1, true);
			ಹಿಂತಿರುಗಿ 0;
		}
	}

	kthread_stop(task);
	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ psi_trigger *psi_trigger_create(ರಚನೆ psi_ಗುಂಪು *ಗುಂಪು, ಅಕ್ಷರ *buf,
				       ಎಣಿಕೆ psi_res res, ರಚನೆ file *file,
				       ರಚನೆ kernfs_open_file *ದ,
				       bool *need_rtpoll_ಕೆಲಸಗಾರ)
{
	ರಚನೆ psi_trigger *t;
	ಎಣಿಕೆ psi_ಸ್ಥಿತಿಗಳು state;
	u32 ಮಿತಿ_us;
	bool privileged;
	u32 ಕಿಟಕಿ_us;

	*need_rtpoll_ಕೆಲಸಗಾರ = false;

	ಆದರೆ (static_branch_likely(&psi_disabled))
		ಹಿಂತಿರುಗಿ ERR_PTR(-EOPNOTSUPP);

	/*
	 * Checking the privilege here on file->f_cred implies that a privileged user
	 * could open the file and delegate the write to an unprivileged one.
	 */
	privileged = cap_raised(file->f_cred->cap_effective, CAP_SYS_RESOURCE);

	ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಓದು(buf, "some %u %u", &ಮಿತಿ_us, &ಕಿಟಕಿ_us) == 2)
		state = PSI_IO_SOME + res * 2;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಓದು(buf, "full %u %u", &ಮಿತಿ_us, &ಕಿಟಕಿ_us) == 2)
		state = PSI_IO_ತುಂಬಿದ + res * 2;
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ ERR_PTR(-EINVAL);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_IRQ_ಸಮಯ_ACCOUNTING
	ಆದರೆ (res == PSI_IRQ && --state != PSI_IRQ_ತುಂಬಿದ)
		ಹಿಂತಿರುಗಿ ERR_PTR(-EINVAL);
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (state >= PSI_NONIDLE)
		ಹಿಂತಿರುಗಿ ERR_PTR(-EINVAL);

	ಆದರೆ (ಕಿಟಕಿ_us == 0 || ಕಿಟಕಿ_us > ಕಿಟಕಿ_MAX_US)
		ಹಿಂತಿರುಗಿ ERR_PTR(-EINVAL);

	/*
	 * Unprivileged users can only use 2s windows so that averages aggregation
	 * work is used, and no RT threads need to be spawned.
	 */
	ಆದರೆ (!privileged && ಕಿಟಕಿ_us % 2000000)
		ಹಿಂತಿರುಗಿ ERR_PTR(-EINVAL);

	/* Check threshold */
	ಆದರೆ (ಮಿತಿ_us == 0 || ಮಿತಿ_us > ಕಿಟಕಿ_us)
		ಹಿಂತಿರುಗಿ ERR_PTR(-EINVAL);

	t = kmalloc_obj(*t);
	ಆದರೆ (!t)
		ಹಿಂತಿರುಗಿ ERR_PTR(-ENOMEM);

	t->ಗುಂಪು = ಗುಂಪು;
	t->state = state;
	t->ಮಿತಿ = ಮಿತಿ_us * NSEC_ಪ್ರತಿ_USEC;
	t->win.ಗಾತ್ರ_2 = ಕಿಟಕಿ_us * NSEC_ಪ್ರತಿ_USEC;
	ಕಿಟಕಿ_reset(&t->win, sched_clock(),
			ಗುಂಪು->ಒಟ್ಟು[PSI_POLL][t->state], 0);

	t->event = 0;
	t->ಕೊನೆಯ_event_ಸಮಯ = 0;
	t->ದ = ದ;
	ಆದರೆ (!ದ)
		init_waitqueue_ತಲೆ(&t->event_wait);
	t->pending_event = false;
	t->aggregator = privileged ? PSI_POLL : PSI_AVGS;

	ಆದರೆ (privileged) {
		mutex_ಬೀಗ(&ಗುಂಪು->rtpoll_trigger_ಬೀಗ);

		list_ಕೂಡಿಸು(&t->ಗಂಟು, &ಗುಂಪು->rtpoll_triggers);
		ಗುಂಪು->rtpoll_min_period = min(ಗುಂಪು->rtpoll_min_period,
			div_u64(t->win.ಗಾತ್ರ_2, UPDATES_ಪ್ರತಿ_ಕಿಟಕಿ));
		ಗುಂಪು->rtpoll_nr_triggers[t->state]++;
		ಗುಂಪು->rtpoll_ಸ್ಥಿತಿಗಳು |= (1 << t->state);

		*need_rtpoll_ಕೆಲಸಗಾರ = !rcu_access_ಸೂಚಕ(ಗುಂಪು->rtpoll_task);

		mutex_unlock(&ಗುಂಪು->rtpoll_trigger_ಬೀಗ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		mutex_ಬೀಗ(&ಗುಂಪು->avgs_ಬೀಗ);

		list_ಕೂಡಿಸು(&t->ಗಂಟು, &ಗುಂಪು->avg_triggers);
		ಗುಂಪು->avg_nr_triggers[t->state]++;

		mutex_unlock(&ಗುಂಪು->avgs_ಬೀಗ);
	}
	ಹಿಂತಿರುಗಿ t;
}

ಶೂನ್ಯ psi_trigger_destroy(ರಚನೆ psi_trigger *t)
{
	ರಚನೆ psi_ಗುಂಪು *ಗುಂಪು;
	ರಚನೆ task_struct *task_ಗಮ್ಯ_destroy = ಖಾಲಿ;

	/*
	 * We do not check psi_disabled since it might have been disabled after
	 * the trigger got created.
	 */
	ಆದರೆ (!t)
		ಹಿಂತಿರುಗಿ;

	ಗುಂಪು = t->ಗುಂಪು;
	/*
	 * Wakeup waiters to stop polling and clear the queue to prevent it from
	 * being accessed later. Can happen if cgroup is deleted from under a
	 * polling process.
	 */
	ಆದರೆ (t->ದ)
		kernfs_notify(t->ದ->kn);
	ಇಲ್ಲದಿದ್ದರೆ
		wake_ಮೇಲೆ_interruptible(&t->event_wait);

	ಆದರೆ (t->aggregator == PSI_AVGS) {
		mutex_ಬೀಗ(&ಗುಂಪು->avgs_ಬೀಗ);
		ಆದರೆ (!list_ಖಾಲಿಗಳು(&t->ಗಂಟು)) {
			list_del(&t->ಗಂಟು);
			ಗುಂಪು->avg_nr_triggers[t->state]--;
		}
		mutex_unlock(&ಗುಂಪು->avgs_ಬೀಗ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		mutex_ಬೀಗ(&ಗುಂಪು->rtpoll_trigger_ಬೀಗ);
		ಆದರೆ (!list_ಖಾಲಿಗಳು(&t->ಗಂಟು)) {
			ರಚನೆ psi_trigger *tmp;
			u64 period = ULLONG_MAX;

			list_del(&t->ಗಂಟು);
			ಗುಂಪು->rtpoll_nr_triggers[t->state]--;
			ಆದರೆ (!ಗುಂಪು->rtpoll_nr_triggers[t->state])
				ಗುಂಪು->rtpoll_ಸ್ಥಿತಿಗಳು &= ~(1 << t->state);
			/*
			 * Reset min update period for the remaining triggers
			 * iff the destroying trigger had the min window size.
			 */
			ಆದರೆ (ಗುಂಪು->rtpoll_min_period == div_u64(t->win.ಗಾತ್ರ_2, UPDATES_ಪ್ರತಿ_ಕಿಟಕಿ)) {
				list_for_each_ನಮೂದು(tmp, &ಗುಂಪು->rtpoll_triggers, ಗಂಟು)
					period = min(period, div_u64(tmp->win.ಗಾತ್ರ_2,
							UPDATES_ಪ್ರತಿ_ಕಿಟಕಿ));
				ಗುಂಪು->rtpoll_min_period = period;
			}
			/* Destroy rtpoll_task when the last trigger is destroyed */
			ಆದರೆ (ಗುಂಪು->rtpoll_ಸ್ಥಿತಿಗಳು == 0) {
				ಗುಂಪು->rtpoll_until = 0;
				task_ಗಮ್ಯ_destroy = rcu_dereference_protected(
						ಗುಂಪು->rtpoll_task,
						lockdep_is_held(&ಗುಂಪು->rtpoll_trigger_ಬೀಗ));
				rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(ಗುಂಪು->rtpoll_task, ಖಾಲಿ);
				timer_delete(&ಗುಂಪು->rtpoll_timer);
			}
		}
		mutex_unlock(&ಗುಂಪು->rtpoll_trigger_ಬೀಗ);
	}

	/*
	 * Stop kthread 'psimon' after releasing rtpoll_trigger_lock to prevent
	 * a deadlock while waiting for psi_rtpoll_work to acquire
	 * rtpoll_trigger_lock
	 */
	ಆದರೆ (task_ಗಮ್ಯ_destroy) {
		kthread_stop(task_ಗಮ್ಯ_destroy);
		atomic_ಗಣ(&ಗುಂಪು->rtpoll_scheduled, 0);
	}
	kfree(t);
}

__poll_t psi_trigger_poll(ಶೂನ್ಯ **trigger_ptr,
				ರಚನೆ file *file, poll_ಕೋಷ್ಟಕ *wait)
{
	__poll_t ret = DEFAULT_POLLMASK;
	ರಚನೆ psi_trigger *t;

	ಆದರೆ (static_branch_likely(&psi_disabled))
		ಹಿಂತಿರುಗಿ DEFAULT_POLLMASK | EPOLLERR | EPOLLPRI;

	t = smp_ತುಂಬು_acquire(trigger_ptr);
	ಆದರೆ (!t)
		ಹಿಂತಿರುಗಿ DEFAULT_POLLMASK | EPOLLERR | EPOLLPRI;

	ಆದರೆ (t->ದ)
		kernfs_generic_poll(t->ದ, wait);
	ಇಲ್ಲದಿದ್ದರೆ
		poll_wait(file, &t->event_wait, wait);

	ಆದರೆ (cmpxchg(&t->event, 1, 0) == 1)
		ret |= EPOLLPRI;

	ಹಿಂತಿರುಗಿ ret;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PROC_FS
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ psi_io_show(ರಚನೆ seq_file *m, ಶೂನ್ಯ *v)
{
	ಹಿಂತಿರುಗಿ psi_show(m, &psi_system, PSI_IO);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ psi_ಸ್ಮರಣೆ_show(ರಚನೆ seq_file *m, ಶೂನ್ಯ *v)
{
	ಹಿಂತಿರುಗಿ psi_show(m, &psi_system, PSI_MEM);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ psi_cpu_show(ರಚನೆ seq_file *m, ಶೂನ್ಯ *v)
{
	ಹಿಂತಿರುಗಿ psi_show(m, &psi_system, PSI_CPU);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ psi_io_open(ರಚನೆ inode *inode, ರಚನೆ file *file)
{
	ಹಿಂತಿರುಗಿ single_open(file, psi_io_show, ಖಾಲಿ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ psi_ಸ್ಮರಣೆ_open(ರಚನೆ inode *inode, ರಚನೆ file *file)
{
	ಹಿಂತಿರುಗಿ single_open(file, psi_ಸ್ಮರಣೆ_show, ಖಾಲಿ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ psi_cpu_open(ರಚನೆ inode *inode, ರಚನೆ file *file)
{
	ಹಿಂತಿರುಗಿ single_open(file, psi_cpu_show, ಖಾಲಿ);
}

ಸ್ಥಿರ ssize_t psi_write(ರಚನೆ file *file, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ __user *user_buf,
			 ಗಾತ್ರ_t nbytes, ಎಣಿಕೆ psi_res res)
{
	ಅಕ್ಷರ buf[32];
	ಗಾತ್ರ_t buf_ಗಾತ್ರ;
	ರಚನೆ seq_file *seq;
	ರಚನೆ psi_trigger *new;
	bool need_rtpoll_ಕೆಲಸಗಾರ;
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (static_branch_likely(&psi_disabled))
		ಹಿಂತಿರುಗಿ -EOPNOTSUPP;

	ಆದರೆ (!nbytes)
		ಹಿಂತಿರುಗಿ -EINVAL;

	buf_ಗಾತ್ರ = min(nbytes, ಗಾತ್ರ(buf));
	ಆದರೆ (copy_from_user(buf, user_buf, buf_ಗಾತ್ರ))
		ಹಿಂತಿರುಗಿ -EFAULT;

	buf[buf_ಗಾತ್ರ - 1] = '\0';

	seq = file->ಖಾಸಗಿ_ದತ್ತಾಂಶ;

	/* Take seq->lock to protect seq->private from concurrent writes */
	mutex_ಬೀಗ(&seq->ಬೀಗ);

	/* Allow only one trigger per file descriptor */
	ಆದರೆ (seq->ಖಾಸಗಿ) {
		mutex_unlock(&seq->ಬೀಗ);
		ಹಿಂತಿರುಗಿ -EBUSY;
	}

	new = psi_trigger_create(&psi_system, buf, res, file, ಖಾಲಿ,
				 &need_rtpoll_ಕೆಲಸಗಾರ);
	ಆದರೆ (IS_ERR(new)) {
		mutex_unlock(&seq->ಬೀಗ);
		ಹಿಂತಿರುಗಿ PTR_ERR(new);
	}

	ಆದರೆ (need_rtpoll_ಕೆಲಸಗಾರ) {
		ret = psi_trigger_create_rtpoll_ಕೆಲಸಗಾರ(&psi_system);
		ಆದರೆ (ret) {
			psi_trigger_destroy(new);
			mutex_unlock(&seq->ಬೀಗ);
			ಹಿಂತಿರುಗಿ ret;
		}
	}

	smp_store_release(&seq->ಖಾಸಗಿ, new);
	mutex_unlock(&seq->ಬೀಗ);

	ಹಿಂತಿರುಗಿ nbytes;
}

ಸ್ಥಿರ ssize_t psi_io_write(ರಚನೆ file *file, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ __user *user_buf,
			    ಗಾತ್ರ_t nbytes, loff_t *ppos)
{
	ಹಿಂತಿರುಗಿ psi_write(file, user_buf, nbytes, PSI_IO);
}

ಸ್ಥಿರ ssize_t psi_ಸ್ಮರಣೆ_write(ರಚನೆ file *file, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ __user *user_buf,
				ಗಾತ್ರ_t nbytes, loff_t *ppos)
{
	ಹಿಂತಿರುಗಿ psi_write(file, user_buf, nbytes, PSI_MEM);
}

ಸ್ಥಿರ ssize_t psi_cpu_write(ರಚನೆ file *file, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ __user *user_buf,
			     ಗಾತ್ರ_t nbytes, loff_t *ppos)
{
	ಹಿಂತಿರುಗಿ psi_write(file, user_buf, nbytes, PSI_CPU);
}

ಸ್ಥಿರ __poll_t psi_fop_poll(ರಚನೆ file *file, poll_ಕೋಷ್ಟಕ *wait)
{
	ರಚನೆ seq_file *seq = file->ಖಾಸಗಿ_ದತ್ತಾಂಶ;

	ಹಿಂತಿರುಗಿ psi_trigger_poll(&seq->ಖಾಸಗಿ, file, wait);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ psi_fop_release(ರಚನೆ inode *inode, ರಚನೆ file *file)
{
	ರಚನೆ seq_file *seq = file->ಖಾಸಗಿ_ದತ್ತಾಂಶ;

	psi_trigger_destroy(seq->ಖಾಸಗಿ);
	ಹಿಂತಿರುಗಿ single_release(inode, file);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ proc_ops psi_io_proc_ops = {
	.proc_open	= psi_io_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_io_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ proc_ops psi_ಸ್ಮರಣೆ_proc_ops = {
	.proc_open	= psi_ಸ್ಮರಣೆ_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_ಸ್ಮರಣೆ_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ proc_ops psi_cpu_proc_ops = {
	.proc_open	= psi_cpu_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_cpu_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_IRQ_ಸಮಯ_ACCOUNTING
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ psi_irq_show(ರಚನೆ seq_file *m, ಶೂನ್ಯ *v)
{
	ಹಿಂತಿರುಗಿ psi_show(m, &psi_system, PSI_IRQ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ psi_irq_open(ರಚನೆ inode *inode, ರಚನೆ file *file)
{
	ಹಿಂತಿರುಗಿ single_open(file, psi_irq_show, ಖಾಲಿ);
}

ಸ್ಥಿರ ssize_t psi_irq_write(ರಚನೆ file *file, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ __user *user_buf,
			     ಗಾತ್ರ_t nbytes, loff_t *ppos)
{
	ಹಿಂತಿರುಗಿ psi_write(file, user_buf, nbytes, PSI_IRQ);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ proc_ops psi_irq_proc_ops = {
	.proc_open	= psi_irq_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_irq_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_IRQ_TIME_ACCOUNTING */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init psi_proc_init(ಶೂನ್ಯ)
{
	ಆದರೆ (psi_enable) {
		proc_mkdir("pressure", ಖಾಲಿ);
		proc_create("pressure/io", 0666, ಖಾಲಿ, &psi_io_proc_ops);
		proc_create("pressure/memory", 0666, ಖಾಲಿ, &psi_ಸ್ಮರಣೆ_proc_ops);
		proc_create("pressure/cpu", 0666, ಖಾಲಿ, &psi_cpu_proc_ops);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_IRQ_ಸಮಯ_ACCOUNTING
		proc_create("pressure/irq", 0666, ಖಾಲಿ, &psi_irq_proc_ops);
#ಆದರೆ_ಅಂತ್ಯ
	}
	ಹಿಂತಿರುಗಿ 0;
}
module_init(psi_proc_init);

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_PROC_FS */
