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
#ضمن <linux/sched/clock.h>
#ضمن <linux/workqueue.h>
#ضمن <linux/psi.h>
#ضمن "sched.h"

ساكن صحيح psi_bug __read_mostly;

DEFINE_STATIC_ڪنجي_FALSE(psi_disabled);
ساكن DEFINE_STATIC_ڪنجي_TRUE(psi_cgroups_enabled);

#جيڪڏهن_وصف_ٿيل CONFIG_PSI_DEFAULT_DISABLED
ساكن bool psi_enable;
#وإلا
ساكن bool psi_enable = true;
#جيڪڏهن_پورو
ساكن صحيح __init تياري_psi(حرف *str)
{
	أرجع kstrtobool(str, &psi_enable) == 0;
}
__setup("psi=", تياري_psi);

/* Running averages - we need to be higher-res than loadavg */
#وصف_ڪريو PSI_FREQ	(2*HZ+1)	/* 2 sec intervals */
#وصف_ڪريو EXP_10s		1677		/* 1/exp(2s/10s) as fixed-point */
#وصف_ڪريو EXP_60s		1981		/* 1/exp(2s/60s) */
#وصف_ڪريو EXP_300s	2034		/* 1/exp(2s/300s) */

/* PSI trigger definitions */
#وصف_ڪريو دري_MAX_US 10000000	/* Max window size is 10s */
#وصف_ڪريو UPDATES_في_دري 10	/* 10 updates per window */

/* Sampling frequency in nanoseconds */
ساكن u64 psi_period __read_mostly;

/* System-level pressure and stall tracking */
ساكن DEFINE_في_CPU(بنية psi_گروهه_cpu, system_گروهه_pcpu);
بنية psi_گروهه psi_system = {
	.pcpu = &system_گروهه_pcpu,
};

ساكن DEFINE_في_CPU(seqcount_t, psi_seq) = SEQCNT_ZERO(psi_seq);

ساكن مضمن فراغ psi_write_begin(صحيح cpu)
{
	write_seqcount_begin(في_cpu_ptr(&psi_seq, cpu));
}

ساكن مضمن فراغ psi_write_پڇاڙي(صحيح cpu)
{
	write_seqcount_پڇاڙي(في_cpu_ptr(&psi_seq, cpu));
}

ساكن مضمن u32 psi_read_begin(صحيح cpu)
{
	أرجع read_seqcount_begin(في_cpu_ptr(&psi_seq, cpu));
}

ساكن مضمن bool psi_read_retry(صحيح cpu, u32 seq)
{
	أرجع read_seqcount_retry(في_cpu_ptr(&psi_seq, cpu), seq);
}

ساكن فراغ psi_avgs_work(بنية work_struct *work);

ساكن فراغ poll_timer_fn(بنية timer_list *t);

ساكن فراغ گروهه_init(بنية psi_گروهه *گروهه)
{
	گروهه->enabled = true;
	گروهه->avg_آخري_update = sched_clock();
	گروهه->avg_ايندڙ_update = گروهه->avg_آخري_update + psi_period;
	mutex_init(&گروهه->avgs_تالو);

	/* Init avg trigger-related members */
	INIT_LIST_سر(&گروهه->avg_triggers);
	memset(گروهه->avg_nr_triggers, 0, حجم(گروهه->avg_nr_triggers));
	INIT_DELAYED_WORK(&گروهه->avgs_work, psi_avgs_work);

	/* Init rtpoll trigger-related members */
	atomic_سيٽ(&گروهه->rtpoll_scheduled, 0);
	mutex_init(&گروهه->rtpoll_trigger_تالو);
	INIT_LIST_سر(&گروهه->rtpoll_triggers);
	گروهه->rtpoll_min_period = U32_MAX;
	گروهه->rtpoll_ايندڙ_update = ULLONG_MAX;
	init_waitqueue_سر(&گروهه->rtpoll_wait);
	timer_تياري(&گروهه->rtpoll_timer, poll_timer_fn, 0);
	rcu_سونپيو_اشارو(گروهه->rtpoll_task, عدم);
}

فراغ __init psi_init(فراغ)
{
	إذا (!psi_enable) {
		static_branch_enable(&psi_disabled);
		static_branch_disable(&psi_cgroups_enabled);
		أرجع;
	}

	إذا (!cgroup_psi_enabled())
		static_branch_disable(&psi_cgroups_enabled);

	psi_period = jiffies_منزل_nsecs(PSI_FREQ);
	گروهه_init(&psi_system);
}

ساكن u32 test_حالتون(بنانشان صحيح *tasks, u32 state_نقاب)
{
	ثابت bool oncpu = state_نقاب & PSI_ONCPU;

	إذا (tasks[NR_IOWAIT]) {
		state_نقاب |= ٻٽو_انگ(PSI_IO_SOME);
		إذا (!tasks[NR_RUNNING])
			state_نقاب |= ٻٽو_انگ(PSI_IO_ڀريل);
	}

	إذا (tasks[NR_MEMSTALL]) {
		state_نقاب |= ٻٽو_انگ(PSI_MEM_SOME);
		إذا (tasks[NR_RUNNING] == tasks[NR_MEMSTALL_RUNNING])
			state_نقاب |= ٻٽو_انگ(PSI_MEM_ڀريل);
	}

	إذا (tasks[NR_RUNNING] > oncpu)
		state_نقاب |= ٻٽو_انگ(PSI_CPU_SOME);

	إذا (tasks[NR_RUNNING] && !oncpu)
		state_نقاب |= ٻٽو_انگ(PSI_CPU_ڀريل);

	إذا (tasks[NR_IOWAIT] || tasks[NR_MEMSTALL] || tasks[NR_RUNNING])
		state_نقاب |= ٻٽو_انگ(PSI_NONIDLE);

	أرجع state_نقاب;
}

ساكن فراغ get_recent_times(بنية psi_گروهه *گروهه, صحيح cpu,
			     تعداد psi_aggregators aggregator, u32 *times,
			     u32 *pchanged_حالتون)
{
	بنية psi_گروهه_cpu *groupc = في_cpu_ptr(گروهه->pcpu, cpu);
	صحيح current_cpu = raw_smp_processor_id();
	بنانشان صحيح tasks[NR_PSI_TASK_COUNTS];
	u64 now, state_شروعات;
	تعداد psi_حالتون s;
	بنانشان صحيح seq;
	u32 state_نقاب;

	*pchanged_حالتون = 0;

	/* Snapshot a coherent view of the CPU state */
	افعل {
		seq = psi_read_begin(cpu);
		now = cpu_clock(cpu);
		يادگيري_نقل(times, groupc->times, حجم(groupc->times));
		state_نقاب = groupc->state_نقاب;
		state_شروعات = groupc->state_شروعات;
		إذا (cpu == current_cpu)
			يادگيري_نقل(tasks, groupc->tasks, حجم(groupc->tasks));
	} طالما (psi_read_retry(cpu, seq));

	/* Calculate state time deltas against the previous snapshot */
	لكل (s = 0; s < NR_PSI_حالتون; s++) {
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
		إذا (state_نقاب & (1 << s))
			times[s] += now - state_شروعات;

		delta = times[s] - groupc->times_prev[aggregator][s];
		groupc->times_prev[aggregator][s] = times[s];

		times[s] = delta;
		إذا (delta)
			*pchanged_حالتون |= (1 << s);
	}

	/*
	 * When collect_percpu_times() from the avgs_work, we don't want to
	 * re-arm avgs_work when all CPUs are IDLE. But the current CPU running
	 * this avgs_work is never IDLE, cause avgs_work can't be shut off.
	 * So for the current CPU, we need to re-arm avgs_work only when
	 * (NR_RUNNING > 1 || NR_IOWAIT > 0 || NR_MEMSTALL > 0), for other CPUs
	 * we can just check PSI_NONIDLE delta.
	 */
	إذا (current_work() == &گروهه->avgs_work.work) {
		bool reschedule;

		إذا (cpu == current_cpu)
			reschedule = tasks[NR_RUNNING] +
				     tasks[NR_IOWAIT] +
				     tasks[NR_MEMSTALL] > 1;
		وإلا
			reschedule = *pchanged_حالتون & (1 << PSI_NONIDLE);

		إذا (reschedule)
			*pchanged_حالتون |= PSI_STATE_RESCHEDULE;
	}
}

ساكن فراغ calc_avgs(بنانشان ڊگهو avg[3], صحيح missed_periods,
		      u64 وقت, u64 period)
{
	بنانشان ڊگهو pct;

	/* Fill in zeroes for periods of no activity */
	إذا (missed_periods) {
		avg[0] = calc_ڀريو_n(avg[0], EXP_10s, 0, missed_periods);
		avg[1] = calc_ڀريو_n(avg[1], EXP_60s, 0, missed_periods);
		avg[2] = calc_ڀريو_n(avg[2], EXP_300s, 0, missed_periods);
	}

	/* Sample the most recent active period */
	pct = div_u64(وقت * 100, period);
	pct *= FIXED_1;
	avg[0] = calc_ڀريو(avg[0], EXP_10s, pct);
	avg[1] = calc_ڀريو(avg[1], EXP_60s, pct);
	avg[2] = calc_ڀريو(avg[2], EXP_300s, pct);
}

ساكن فراغ collect_percpu_times(بنية psi_گروهه *گروهه,
				 تعداد psi_aggregators aggregator,
				 u32 *pchanged_حالتون)
{
	u64 deltas[NR_PSI_حالتون - 1] = { 0, };
	بنانشان ڊگهو nonidle_ڪل = 0;
	u32 بدليو_حالتون = 0;
	صحيح cpu;
	صحيح s;

	/*
	 * Collect the per-cpu time buckets and average them into a
	 * single time sample that is normalized to wall clock time.
	 *
	 * For averaging, each CPU is weighted by its non-idle time in
	 * the sampling period. This eliminates artifacts from uneven
	 * loading, or even entirely idle CPUs.
	 */
	for_each_possible_cpu(cpu) {
		u32 times[NR_PSI_حالتون];
		u32 nonidle;
		u32 cpu_بدليو_حالتون;

		get_recent_times(گروهه, cpu, aggregator, times,
				&cpu_بدليو_حالتون);
		بدليو_حالتون |= cpu_بدليو_حالتون;

		nonidle = nsecs_منزل_jiffies(times[PSI_NONIDLE]);
		nonidle_ڪل += nonidle;

		لكل (s = 0; s < PSI_NONIDLE; s++)
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
	لكل (s = 0; s < NR_PSI_حالتون - 1; s++)
		گروهه->ڪل[aggregator][s] +=
				div_u64(deltas[s], max(nonidle_ڪل, 1UL));

	إذا (pchanged_حالتون)
		*pchanged_حالتون = بدليو_حالتون;
}

/* Trigger tracking window manipulations */
ساكن فراغ دري_reset(بنية psi_دري *win, u64 now, u64 قدر,
			 u64 prev_growth)
{
	win->شروعات_وقت = now;
	win->شروعات_قدر = قدر;
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
ساكن u64 دري_update(بنية psi_دري *win, u64 now, u64 قدر)
{
	u64 elapsed;
	u64 growth;

	elapsed = now - win->شروعات_وقت;
	growth = قدر - win->شروعات_قدر;
	/*
	 * After each tracking window passes win->start_value and
	 * win->start_time get reset and win->prev_growth stores
	 * the average per-window growth of the previous window.
	 * win->prev_growth is then used to interpolate additional
	 * growth from the previous window assuming it was linear.
	 */
	إذا (elapsed > win->سائيز)
		دري_reset(win, now, قدر, growth);
	وإلا {
		u32 بچيل;

		بچيل = win->سائيز - elapsed;
		growth += div64_u64(win->prev_growth * بچيل, win->سائيز);
	}

	أرجع growth;
}

ساكن فراغ update_triggers(بنية psi_گروهه *گروهه, u64 now,
						   تعداد psi_aggregators aggregator)
{
	بنية psi_trigger *t;
	u64 *ڪل = گروهه->ڪل[aggregator];
	بنية list_سر *triggers;
	u64 *aggregator_ڪل;

	إذا (aggregator == PSI_AVGS) {
		triggers = &گروهه->avg_triggers;
		aggregator_ڪل = گروهه->avg_ڪل;
	} وإلا {
		triggers = &گروهه->rtpoll_triggers;
		aggregator_ڪل = گروهه->rtpoll_ڪل;
	}

	/*
	 * On subsequent updates, calculate growth deltas and let
	 * watchers know when their specified thresholds are exceeded.
	 */
	list_for_each_داخلا(t, triggers, ڳنڍ) {
		u64 growth;
		bool new_stall;

		new_stall = aggregator_ڪل[t->state] != ڪل[t->state];

		/* Check for stall activity or a previous threshold breach */
		إذا (!new_stall && !t->pending_event)
			استمر;
		/*
		 * Check for new stall activity, as well as deferred
		 * events that occurred in the last window after the
		 * trigger had already fired (we want to ratelimit
		 * events without dropping any).
		 */
		إذا (new_stall) {
			/* Calculate growth since last update */
			growth = دري_update(&t->win, now, ڪل[t->state]);
			إذا (!t->pending_event) {
				إذا (growth < t->حد)
					استمر;

				t->pending_event = true;
			}
		}
		/* Limit event signaling to once per window */
		إذا (now < t->آخري_event_وقت + t->win.سائيز)
			استمر;

		/* Generate an event */
		إذا (cmpxchg(&t->event, 0, 1) == 0) {
			إذا (t->جو)
				kernfs_notify(t->جو->kn);
			وإلا
				wake_مٿي_interruptible(&t->event_wait);
		}
		t->آخري_event_وقت = now;
		/* Reset threshold breach flag once event got generated */
		t->pending_event = false;
	}
}

ساكن u64 update_averages(بنية psi_گروهه *گروهه, u64 now)
{
	بنانشان ڊگهو missed_periods = 0;
	u64 expires, period;
	u64 avg_ايندڙ_update;
	صحيح s;

	/* avgX= */
	expires = گروهه->avg_ايندڙ_update;
	إذا (now - expires >= psi_period)
		missed_periods = div_u64(now - expires, psi_period);

	/*
	 * The periodic clock tick can get delayed for various
	 * reasons, especially on loaded systems. To avoid clock
	 * drift, we schedule the clock in fixed psi_period intervals.
	 * But the deltas we sample out of the per-cpu buckets above
	 * are based on the actual time elapsing between clock ticks.
	 */
	avg_ايندڙ_update = expires + ((1 + missed_periods) * psi_period);
	period = now - (گروهه->avg_آخري_update + (missed_periods * psi_period));
	گروهه->avg_آخري_update = now;

	لكل (s = 0; s < NR_PSI_حالتون - 1; s++) {
		u32 sample;

		sample = گروهه->ڪل[PSI_AVGS][s] - گروهه->avg_ڪل[s];
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
		إذا (sample > period)
			sample = period;
		گروهه->avg_ڪل[s] += sample;
		calc_avgs(گروهه->avg[s], missed_periods, sample, period);
	}

	أرجع avg_ايندڙ_update;
}

ساكن فراغ psi_avgs_work(بنية work_struct *work)
{
	بنية delayed_work *dwork;
	بنية psi_گروهه *گروهه;
	u32 بدليو_حالتون;
	u64 now;

	dwork = منزل_delayed_work(work);
	گروهه = container_جو(dwork, بنية psi_گروهه, avgs_work);

	mutex_تالو(&گروهه->avgs_تالو);

	now = sched_clock();

	collect_percpu_times(گروهه, PSI_AVGS, &بدليو_حالتون);
	/*
	 * If there is task activity, periodically fold the per-cpu
	 * times and feed samples into the running averages. If things
	 * are idle and there is no data to process, stop the clock.
	 * Once restarted, we'll catch up the running averages in one
	 * go - see calc_avgs() and missed_periods.
	 */
	إذا (now >= گروهه->avg_ايندڙ_update) {
		update_triggers(گروهه, now, PSI_AVGS);
		گروهه->avg_ايندڙ_update = update_averages(گروهه, now);
	}

	إذا (بدليو_حالتون & PSI_STATE_RESCHEDULE) {
		وقت_جدول_delayed_work(dwork, nsecs_منزل_jiffies(
				گروهه->avg_ايندڙ_update - now) + 1);
	}

	mutex_unlock(&گروهه->avgs_تالو);
}

ساكن فراغ init_rtpoll_triggers(بنية psi_گروهه *گروهه, u64 now)
{
	بنية psi_trigger *t;

	list_for_each_داخلا(t, &گروهه->rtpoll_triggers, ڳنڍ)
		دري_reset(&t->win, now,
				گروهه->ڪل[PSI_POLL][t->state], 0);
	يادگيري_نقل(گروهه->rtpoll_ڪل, گروهه->ڪل[PSI_POLL],
		   حجم(گروهه->rtpoll_ڪل));
	گروهه->rtpoll_ايندڙ_update = now + گروهه->rtpoll_min_period;
}

/* Schedule rtpolling if it's not already scheduled or forced. */
ساكن فراغ psi_وقت_جدول_rtpoll_work(بنية psi_گروهه *گروهه, بنانشان ڊگهو delay,
				   bool force)
{
	/*
	 * atomic_xchg should be called even when !force to provide a
	 * full memory barrier (see the comment inside psi_rtpoll_work).
	 */
	إذا (atomic_xchg(&گروهه->rtpoll_scheduled, 1) && !force)
		أرجع;

	/*
	 * Only test whether a worker is installed; do not dereference the task.
	 * A racing trigger destruction may leave the timer armed.
	 * psi_cgroup_free() shuts it down before freeing the group.
	 * The system PSI group is static.
	 */
	إذا (likely(rcu_access_اشارو(گروهه->rtpoll_task)))
		mod_timer(&گروهه->rtpoll_timer, jiffies + delay);
	وإلا
		atomic_سيٽ(&گروهه->rtpoll_scheduled, 0);
}

ساكن فراغ psi_rtpoll_work(بنية psi_گروهه *گروهه)
{
	bool force_reschedule = false;
	u32 بدليو_حالتون;
	u64 now;

	mutex_تالو(&گروهه->rtpoll_trigger_تالو);

	now = sched_clock();

	إذا (now > گروهه->rtpoll_until) {
		/*
		 * We are either about to start or might stop rtpolling if no
		 * state change was recorded. Resetting rtpoll_scheduled leaves
		 * a small window for psi_group_change to sneak in and schedule
		 * an immediate rtpoll_work before we get to rescheduling. One
		 * potential extra wakeup at the end of the rtpolling window
		 * should be negligible and rtpoll_next_update still keeps
		 * updates correctly on schedule.
		 */
		atomic_سيٽ(&گروهه->rtpoll_scheduled, 0);
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
	} وإلا {
		/* The rtpolling window is not over, keep rescheduling */
		force_reschedule = true;
	}


	collect_percpu_times(گروهه, PSI_POLL, &بدليو_حالتون);

	إذا (بدليو_حالتون & گروهه->rtpoll_حالتون) {
		/* Initialize trigger windows when entering rtpolling mode */
		إذا (now > گروهه->rtpoll_until)
			init_rtpoll_triggers(گروهه, now);

		/*
		 * Keep the monitor active for at least the duration of the
		 * minimum tracking window as long as monitor states are
		 * changing.
		 */
		گروهه->rtpoll_until = now +
			گروهه->rtpoll_min_period * UPDATES_في_دري;
	}

	إذا (now > گروهه->rtpoll_until) {
		گروهه->rtpoll_ايندڙ_update = ULLONG_MAX;
		اذهب ٻاهريون;
	}

	إذا (now >= گروهه->rtpoll_ايندڙ_update) {
		إذا (بدليو_حالتون & گروهه->rtpoll_حالتون) {
			update_triggers(گروهه, now, PSI_POLL);
			يادگيري_نقل(گروهه->rtpoll_ڪل, گروهه->ڪل[PSI_POLL],
				   حجم(گروهه->rtpoll_ڪل));
		}
		گروهه->rtpoll_ايندڙ_update = now + گروهه->rtpoll_min_period;
	}

	psi_وقت_جدول_rtpoll_work(گروهه,
		nsecs_منزل_jiffies(گروهه->rtpoll_ايندڙ_update - now) + 1,
		force_reschedule);

ٻاهريون:
	mutex_unlock(&گروهه->rtpoll_trigger_تالو);
}

ساكن صحيح psi_rtpoll_ڪم_ڪندڙ(فراغ *معلومات)
{
	بنية psi_گروهه *گروهه = (بنية psi_گروهه *)معلومات;

	sched_سيٽ_fifo_هيٺيون(current);

	طالما (true) {
		wait_event_interruptible(گروهه->rtpoll_wait,
				atomic_cmpxchg(&گروهه->rtpoll_wakeup, 1, 0) ||
				kthread_should_stop());
		إذا (kthread_should_stop())
			اكسر;

		psi_rtpoll_work(گروهه);
	}
	أرجع 0;
}

ساكن فراغ poll_timer_fn(بنية timer_list *t)
{
	بنية psi_گروهه *گروهه = timer_container_جو(گروهه, t, rtpoll_timer);

	atomic_سيٽ(&گروهه->rtpoll_wakeup, 1);
	wake_مٿي_interruptible(&گروهه->rtpoll_wait);
}

ساكن فراغ رڪارڊ_times(بنية psi_گروهه_cpu *groupc, u64 now)
{
	u32 delta;

	delta = now - groupc->state_شروعات;
	groupc->state_شروعات = now;

	إذا (groupc->state_نقاب & (1 << PSI_IO_SOME)) {
		groupc->times[PSI_IO_SOME] += delta;
		إذا (groupc->state_نقاب & (1 << PSI_IO_ڀريل))
			groupc->times[PSI_IO_ڀريل] += delta;
	}

	إذا (groupc->state_نقاب & (1 << PSI_MEM_SOME)) {
		groupc->times[PSI_MEM_SOME] += delta;
		إذا (groupc->state_نقاب & (1 << PSI_MEM_ڀريل))
			groupc->times[PSI_MEM_ڀريل] += delta;
	}

	إذا (groupc->state_نقاب & (1 << PSI_CPU_SOME)) {
		groupc->times[PSI_CPU_SOME] += delta;
		إذا (groupc->state_نقاب & (1 << PSI_CPU_ڀريل))
			groupc->times[PSI_CPU_ڀريل] += delta;
	}

	إذا (groupc->state_نقاب & (1 << PSI_NONIDLE))
		groupc->times[PSI_NONIDLE] += delta;
}

#وصف_ڪريو for_each_گروهه(iter, گروهه) \
	لكل (typeof(گروهه) iter = گروهه; iter; iter = iter->والد)

ساكن فراغ psi_گروهه_change(بنية psi_گروهه *گروهه, صحيح cpu,
			     بنانشان صحيح clear, بنانشان صحيح سيٽ,
			     u64 now, bool wake_clock)
{
	بنية psi_گروهه_cpu *groupc;
	بنانشان صحيح t, m;
	u32 state_نقاب;

	lockdep_assert_rq_held(cpu_rq(cpu));
	groupc = في_cpu_ptr(گروهه->pcpu, cpu);

	/*
	 * Start with TSK_ONCPU, which doesn't have a corresponding
	 * task count - it's just a boolean flag directly encoded in
	 * the state mask. Clear, set, or carry the current state if
	 * no changes are requested.
	 */
	إذا (unlikely(clear & TSK_ONCPU)) {
		state_نقاب = 0;
		clear &= ~TSK_ONCPU;
	} وإلا إذا (unlikely(سيٽ & TSK_ONCPU)) {
		state_نقاب = PSI_ONCPU;
		سيٽ &= ~TSK_ONCPU;
	} وإلا {
		state_نقاب = groupc->state_نقاب & PSI_ONCPU;
	}

	/*
	 * The rest of the state mask is calculated based on the task
	 * counts. Update those first, then construct the mask.
	 */
	لكل (t = 0, m = clear; m; m &= ~(1 << t), t++) {
		إذا (!(m & (1 << t)))
			استمر;
		إذا (groupc->tasks[t]) {
			groupc->tasks[t]--;
		} وإلا إذا (!psi_bug) {
			printk_deferred(KERN_ERR "psi: task underflow! cpu=%d t=%d tasks=[%u %u %u %u] clear=%x set=%x\n",
					cpu, t, groupc->tasks[0],
					groupc->tasks[1], groupc->tasks[2],
					groupc->tasks[3], clear, سيٽ);
			psi_bug = 1;
		}
	}

	لكل (t = 0; سيٽ; سيٽ &= ~(1 << t), t++)
		إذا (سيٽ & (1 << t))
			groupc->tasks[t]++;

	إذا (!گروهه->enabled) {
		/*
		 * On the first group change after disabling PSI, conclude
		 * the current state and flush its time. This is unlikely
		 * to matter to the user, but aggregation (get_recent_times)
		 * may have already incorporated the live state into times_prev;
		 * avoid a delta sample underflow when PSI is later re-enabled.
		 */
		إذا (unlikely(groupc->state_نقاب & (1 << PSI_NONIDLE)))
			رڪارڊ_times(groupc, now);

		groupc->state_نقاب = state_نقاب;

		أرجع;
	}

	state_نقاب = test_حالتون(groupc->tasks, state_نقاب);

	/*
	 * Since we care about lost potential, a memstall is FULL
	 * when there are no other working tasks, but also when
	 * the CPU is actively reclaiming and nothing productive
	 * could run even if it were runnable. So when the current
	 * task in a cgroup is in_memstall, the corresponding groupc
	 * on that cpu is in PSI_MEM_FULL state.
	 */
	إذا (unlikely((state_نقاب & PSI_ONCPU) && cpu_curr(cpu)->اندريون_memstall))
		state_نقاب |= (1 << PSI_MEM_ڀريل);

	رڪارڊ_times(groupc, now);

	groupc->state_نقاب = state_نقاب;

	إذا (state_نقاب & گروهه->rtpoll_حالتون)
		psi_وقت_جدول_rtpoll_work(گروهه, 1, false);

	إذا (wake_clock && !delayed_work_pending(&گروهه->avgs_work))
		وقت_جدول_delayed_work(&گروهه->avgs_work, PSI_FREQ);
}

ساكن مضمن بنية psi_گروهه *task_psi_گروهه(بنية task_struct *task)
{
#جيڪڏهن_وصف_ٿيل CONFIG_CGROUPS
	إذا (static_branch_likely(&psi_cgroups_enabled))
		أرجع cgroup_psi(task_dfl_cgroup(task));
#جيڪڏهن_پورو
	أرجع &psi_system;
}

ساكن فراغ psi_flags_change(بنية task_struct *task, صحيح clear, صحيح سيٽ)
{
	إذا (((task->psi_flags & سيٽ) ||
	     (task->psi_flags & clear) != clear) &&
	    !psi_bug) {
		printk_deferred(KERN_ERR "psi: inconsistent task state! task=%d:%s cpu=%d psi_flags=%x clear=%x set=%x\n",
				task->pid, task->comm, task_cpu(task),
				task->psi_flags, clear, سيٽ);
		psi_bug = 1;
	}

	task->psi_flags &= ~clear;
	task->psi_flags |= سيٽ;
}

فراغ psi_task_change(بنية task_struct *task, صحيح clear, صحيح سيٽ)
{
	صحيح cpu = task_cpu(task);
	u64 now;

	إذا (!task->pid)
		أرجع;

	psi_flags_change(task, clear, سيٽ);

	psi_write_begin(cpu);
	now = cpu_clock(cpu);
	for_each_گروهه(گروهه, task_psi_گروهه(task))
		psi_گروهه_change(گروهه, cpu, clear, سيٽ, now, true);
	psi_write_پڇاڙي(cpu);
}

فراغ psi_task_switch(بنية task_struct *prev, بنية task_struct *ايندڙ,
		     bool sleep)
{
	بنية psi_گروهه *common = عدم;
	صحيح cpu = task_cpu(prev);
	u64 now;

	psi_write_begin(cpu);
	now = cpu_clock(cpu);

	إذا (ايندڙ->pid) {
		psi_flags_change(ايندڙ, 0, TSK_ONCPU);
		/*
		 * Set TSK_ONCPU on @next's cgroups. If @next shares any
		 * ancestors with @prev, those will already have @prev's
		 * TSK_ONCPU bit set, and we can stop the iteration there.
		 */
		for_each_گروهه(گروهه, task_psi_گروهه(ايندڙ)) {
			بنية psi_گروهه_cpu *groupc = في_cpu_ptr(گروهه->pcpu, cpu);

			إذا (groupc->state_نقاب & PSI_ONCPU) {
				common = گروهه;
				اكسر;
			}
			psi_گروهه_change(گروهه, cpu, 0, TSK_ONCPU, now, true);
		}
	}

	إذا (prev->pid) {
		صحيح clear = TSK_ONCPU, سيٽ = 0;
		bool wake_clock = true;

		/*
		 * When we're going to sleep, psi_dequeue() lets us
		 * handle TSK_RUNNING, TSK_MEMSTALL_RUNNING and
		 * TSK_IOWAIT here, where we can combine it with
		 * TSK_ONCPU and save walking common ancestors twice.
		 */
		إذا (sleep) {
			clear |= TSK_RUNNING;
			إذا (prev->اندريون_memstall)
				clear |= TSK_MEMSTALL_RUNNING;
			إذا (prev->اندريون_iowait)
				سيٽ |= TSK_IOWAIT;

			/*
			 * Periodic aggregation shuts off if there is a period of no
			 * task changes, so we wake it back up if necessary. However,
			 * don't do this if the task change is the aggregation worker
			 * itself going to sleep, or we'll ping-pong forever.
			 */
			إذا (unlikely((prev->flags & PF_WQ_ڪم_ڪندڙ) &&
				     wq_ڪم_ڪندڙ_آخري_func(prev) == psi_avgs_work))
				wake_clock = false;
		}

		psi_flags_change(prev, clear, سيٽ);

		for_each_گروهه(گروهه, task_psi_گروهه(prev)) {
			إذا (گروهه == common)
				اكسر;
			psi_گروهه_change(گروهه, cpu, clear, سيٽ, now, wake_clock);
		}

		/*
		 * TSK_ONCPU is handled up to the common ancestor. If there are
		 * any other differences between the two tasks (e.g. prev goes
		 * to sleep, or only one task is memstall), finish propagating
		 * those differences all the way up to the root.
		 */
		إذا ((prev->psi_flags ^ ايندڙ->psi_flags) & ~TSK_ONCPU) {
			clear &= ~TSK_ONCPU;
			for_each_گروهه(گروهه, common)
				psi_گروهه_change(گروهه, cpu, clear, سيٽ, now, wake_clock);
		}
	}
	psi_write_پڇاڙي(cpu);
}

#جيڪڏهن_وصف_ٿيل CONFIG_IRQ_وقت_ACCOUNTING
فراغ psi_account_irqtime(بنية rq *rq, بنية task_struct *curr, بنية task_struct *prev)
{
	صحيح cpu = task_cpu(curr);
	بنية psi_گروهه_cpu *groupc;
	s64 delta;
	u64 irq;
	u64 now;

	إذا (static_branch_likely(&psi_disabled) || !irqtime_enabled())
		أرجع;

	إذا (!curr->pid)
		أرجع;

	lockdep_assert_rq_held(rq);
	إذا (prev && task_psi_گروهه(prev) == task_psi_گروهه(curr))
		أرجع;

	irq = irq_وقت_read(cpu);
	delta = (s64)(irq - rq->psi_irq_وقت);
	إذا (delta <= 0)
		أرجع;
	rq->psi_irq_وقت = irq;

	psi_write_begin(cpu);
	now = cpu_clock(cpu);

	for_each_گروهه(گروهه, task_psi_گروهه(curr)) {
		إذا (!گروهه->enabled)
			استمر;

		groupc = في_cpu_ptr(گروهه->pcpu, cpu);

		رڪارڊ_times(groupc, now);
		groupc->times[PSI_IRQ_ڀريل] += delta;

		إذا (گروهه->rtpoll_حالتون & (1 << PSI_IRQ_ڀريل))
			psi_وقت_جدول_rtpoll_work(گروهه, 1, false);
	}
	psi_write_پڇاڙي(cpu);
}
#جيڪڏهن_پورو /* CONFIG_IRQ_TIME_ACCOUNTING */

/**
 * psi_memstall_enter - mark the beginning of a memory stall section
 * @flags: flags to handle nested sections
 *
 * Marks the calling task as being stalled due to a lack of memory,
 * such as waiting for a refault or performing reclaim.
 */
فراغ psi_memstall_enter(بنانشان ڊگهو *flags)
{
	بنية rq_flags rf;
	بنية rq *rq;

	إذا (static_branch_likely(&psi_disabled))
		أرجع;

	*flags = current->اندريون_memstall;
	إذا (*flags)
		أرجع;
	/*
	 * in_memstall setting & accounting needs to be atomic wrt
	 * changes to the task's scheduling state, otherwise we can
	 * race with CPU migration.
	 */
	rq = this_rq_تالو_irq(&rf);

	current->اندريون_memstall = 1;
	psi_task_change(current, 0, TSK_MEMSTALL | TSK_MEMSTALL_RUNNING);

	rq_unlock_irq(rq, &rf);
}
EXPORT_نشان_GPL(psi_memstall_enter);

/**
 * psi_memstall_leave - mark the end of an memory stall section
 * @flags: flags to handle nested memdelay sections
 *
 * Marks the calling task as no longer stalled due to lack of memory.
 */
فراغ psi_memstall_leave(بنانشان ڊگهو *flags)
{
	بنية rq_flags rf;
	بنية rq *rq;

	إذا (static_branch_likely(&psi_disabled))
		أرجع;

	إذا (*flags)
		أرجع;
	/*
	 * in_memstall clearing & accounting needs to be atomic wrt
	 * changes to the task's scheduling state, otherwise we could
	 * race with CPU migration.
	 */
	rq = this_rq_تالو_irq(&rf);

	current->اندريون_memstall = 0;
	psi_task_change(current, TSK_MEMSTALL | TSK_MEMSTALL_RUNNING, 0);

	rq_unlock_irq(rq, &rf);
}
EXPORT_نشان_GPL(psi_memstall_leave);

#جيڪڏهن_وصف_ٿيل CONFIG_CGROUPS
صحيح psi_cgroup_alloc(بنية cgroup *cgroup)
{
	إذا (!static_branch_likely(&psi_cgroups_enabled))
		أرجع 0;

	cgroup->psi = kzalloc_obj(بنية psi_گروهه);
	إذا (!cgroup->psi)
		أرجع -ENOMEM;

	cgroup->psi->pcpu = alloc_percpu(بنية psi_گروهه_cpu);
	إذا (!cgroup->psi->pcpu) {
		kfree(cgroup->psi);
		أرجع -ENOMEM;
	}
	گروهه_init(cgroup->psi);
	cgroup->psi->والد = cgroup_psi(cgroup_والد(cgroup));
	أرجع 0;
}

فراغ psi_cgroup_free(بنية cgroup *cgroup)
{
	إذا (!static_branch_likely(&psi_cgroups_enabled))
		أرجع;

	cancel_delayed_work_sync(&cgroup->psi->avgs_work);
	/*
	 * A psi_schedule_rtpoll_work() call racing the last trigger's
	 * destruction may have re-armed the timer after psi_trigger_destroy()
	 * deleted it. Spurious firing while the group is alive is harmless.
	 */
	timer_shutdown_sync(&cgroup->psi->rtpoll_timer);
	free_percpu(cgroup->psi->pcpu);
	/* All triggers must be removed by now */
	WARN_ONCE(cgroup->psi->rtpoll_حالتون, "psi: trigger leak\n");
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
فراغ cgroup_move_task(بنية task_struct *task, بنية css_سيٽ *منزل)
{
	بنانشان صحيح task_flags;
	بنية rq_flags rf;
	بنية rq *rq;

	إذا (!static_branch_likely(&psi_cgroups_enabled)) {
		/*
		 * Lame to do this here, but the scheduler cannot be locked
		 * from the outside, so we move cgroups from inside sched/.
		 */
		rcu_سونپيو_اشارو(task->cgroups, منزل);
		أرجع;
	}

	rq = task_rq_تالو_2(task, &rf);

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

	إذا (task_flags)
		psi_task_change(task, task_flags, 0);

	/* See comment above */
	rcu_سونپيو_اشارو(task->cgroups, منزل);

	إذا (task_flags)
		psi_task_change(task, 0, task_flags);

	task_rq_unlock(rq, task, &rf);
}

فراغ psi_cgroup_restart(بنية psi_گروهه *گروهه)
{
	صحيح cpu;

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
	إذا (!گروهه->enabled)
		أرجع;

	for_each_possible_cpu(cpu) {
		u64 now;

		guard(rq_تالو_irq)(cpu_rq(cpu));

		psi_write_begin(cpu);
		now = cpu_clock(cpu);
		psi_گروهه_change(گروهه, cpu, 0, 0, now, true);
		psi_write_پڇاڙي(cpu);
	}
}
#جيڪڏهن_پورو /* CONFIG_CGROUPS */

صحيح psi_show(بنية seq_file *m, بنية psi_گروهه *گروهه, تعداد psi_res res)
{
	bool only_ڀريل = false;
	صحيح ڀريل;
	u64 now;

	إذا (static_branch_likely(&psi_disabled))
		أرجع -EOPNOTSUPP;

#جيڪڏهن_وصف_ٿيل CONFIG_IRQ_وقت_ACCOUNTING
	إذا (!irqtime_enabled() && res == PSI_IRQ)
		أرجع -EOPNOTSUPP;
#جيڪڏهن_پورو

	/* Update averages before reporting them */
	mutex_تالو(&گروهه->avgs_تالو);
	now = sched_clock();
	collect_percpu_times(گروهه, PSI_AVGS, عدم);
	إذا (now >= گروهه->avg_ايندڙ_update)
		گروهه->avg_ايندڙ_update = update_averages(گروهه, now);
	mutex_unlock(&گروهه->avgs_تالو);

#جيڪڏهن_وصف_ٿيل CONFIG_IRQ_وقت_ACCOUNTING
	only_ڀريل = res == PSI_IRQ;
#جيڪڏهن_پورو

	لكل (ڀريل = 0; ڀريل < 2 - only_ڀريل; ڀريل++) {
		بنانشان ڊگهو avg[3] = { 0, };
		u64 ڪل = 0;
		صحيح w;

		/* CPU FULL is undefined at the system level */
		إذا (!(گروهه == &psi_system && res == PSI_CPU && ڀريل)) {
			لكل (w = 0; w < 3; w++)
				avg[w] = گروهه->avg[res * 2 + ڀريل][w];
			ڪل = div_u64(گروهه->ڪل[PSI_AVGS][res * 2 + ڀريل],
					NSEC_في_USEC);
		}

		seq_printf(m, "%s avg10=%lu.%02lu avg60=%lu.%02lu avg300=%lu.%02lu total=%llu\n",
			   ڀريل || only_ڀريل ? "full" : "some",
			   ڀريو_INT(avg[0]), ڀريو_FRAC(avg[0]),
			   ڀريو_INT(avg[1]), ڀريو_FRAC(avg[1]),
			   ڀريو_INT(avg[2]), ڀريو_FRAC(avg[2]),
			   ڪل);
	}

	أرجع 0;
}

/*
 * Create @group's rtpoll worker after psi_trigger_create() reported the need
 * for one. kthread creation depends on the whole fork path and we don't want
 * all of that nested inside cgroup_mutex, so the caller must drop it and any
 * other lock that forks can wait behind. If two callers race, the loser stops
 * its never-woken kthread.
 */
صحيح psi_trigger_create_rtpoll_ڪم_ڪندڙ(بنية psi_گروهه *گروهه)
{
	بنية task_struct *task;

	task = kthread_create(psi_rtpoll_ڪم_ڪندڙ, گروهه, "psimon");
	إذا (IS_ERR(task))
		أرجع PTR_ERR(task);

	scoped_guard(mutex, &گروهه->rtpoll_trigger_تالو) {
		إذا (!rcu_access_اشارو(گروهه->rtpoll_task)) {
			atomic_سيٽ(&گروهه->rtpoll_wakeup, 0);
			wake_مٿي_process(task);
			rcu_سونپيو_اشارو(گروهه->rtpoll_task, task);

			/*
			 * Poll once to catch up on scheduling attempts dropped
			 * while there was no rtpoll worker.
			 */
			psi_وقت_جدول_rtpoll_work(گروهه, 1, true);
			أرجع 0;
		}
	}

	kthread_stop(task);
	أرجع 0;
}

بنية psi_trigger *psi_trigger_create(بنية psi_گروهه *گروهه, حرف *buf,
				       تعداد psi_res res, بنية file *file,
				       بنية kernfs_open_file *جو,
				       bool *need_rtpoll_ڪم_ڪندڙ)
{
	بنية psi_trigger *t;
	تعداد psi_حالتون state;
	u32 حد_us;
	bool privileged;
	u32 دري_us;

	*need_rtpoll_ڪم_ڪندڙ = false;

	إذا (static_branch_likely(&psi_disabled))
		أرجع ERR_PTR(-EOPNOTSUPP);

	/*
	 * Checking the privilege here on file->f_cred implies that a privileged user
	 * could open the file and delegate the write to an unprivileged one.
	 */
	privileged = cap_raised(file->f_cred->cap_effective, CAP_SYS_RESOURCE);

	إذا (لڙي_مان_پڙهو(buf, "some %u %u", &حد_us, &دري_us) == 2)
		state = PSI_IO_SOME + res * 2;
	وإلا إذا (لڙي_مان_پڙهو(buf, "full %u %u", &حد_us, &دري_us) == 2)
		state = PSI_IO_ڀريل + res * 2;
	وإلا
		أرجع ERR_PTR(-EINVAL);

#جيڪڏهن_وصف_ٿيل CONFIG_IRQ_وقت_ACCOUNTING
	إذا (res == PSI_IRQ && --state != PSI_IRQ_ڀريل)
		أرجع ERR_PTR(-EINVAL);
#جيڪڏهن_پورو

	إذا (state >= PSI_NONIDLE)
		أرجع ERR_PTR(-EINVAL);

	إذا (دري_us == 0 || دري_us > دري_MAX_US)
		أرجع ERR_PTR(-EINVAL);

	/*
	 * Unprivileged users can only use 2s windows so that averages aggregation
	 * work is used, and no RT threads need to be spawned.
	 */
	إذا (!privileged && دري_us % 2000000)
		أرجع ERR_PTR(-EINVAL);

	/* Check threshold */
	إذا (حد_us == 0 || حد_us > دري_us)
		أرجع ERR_PTR(-EINVAL);

	t = kmalloc_obj(*t);
	إذا (!t)
		أرجع ERR_PTR(-ENOMEM);

	t->گروهه = گروهه;
	t->state = state;
	t->حد = حد_us * NSEC_في_USEC;
	t->win.سائيز = دري_us * NSEC_في_USEC;
	دري_reset(&t->win, sched_clock(),
			گروهه->ڪل[PSI_POLL][t->state], 0);

	t->event = 0;
	t->آخري_event_وقت = 0;
	t->جو = جو;
	إذا (!جو)
		init_waitqueue_سر(&t->event_wait);
	t->pending_event = false;
	t->aggregator = privileged ? PSI_POLL : PSI_AVGS;

	إذا (privileged) {
		mutex_تالو(&گروهه->rtpoll_trigger_تالو);

		list_جوڙيو(&t->ڳنڍ, &گروهه->rtpoll_triggers);
		گروهه->rtpoll_min_period = min(گروهه->rtpoll_min_period,
			div_u64(t->win.سائيز, UPDATES_في_دري));
		گروهه->rtpoll_nr_triggers[t->state]++;
		گروهه->rtpoll_حالتون |= (1 << t->state);

		*need_rtpoll_ڪم_ڪندڙ = !rcu_access_اشارو(گروهه->rtpoll_task);

		mutex_unlock(&گروهه->rtpoll_trigger_تالو);
	} وإلا {
		mutex_تالو(&گروهه->avgs_تالو);

		list_جوڙيو(&t->ڳنڍ, &گروهه->avg_triggers);
		گروهه->avg_nr_triggers[t->state]++;

		mutex_unlock(&گروهه->avgs_تالو);
	}
	أرجع t;
}

فراغ psi_trigger_destroy(بنية psi_trigger *t)
{
	بنية psi_گروهه *گروهه;
	بنية task_struct *task_منزل_destroy = عدم;

	/*
	 * We do not check psi_disabled since it might have been disabled after
	 * the trigger got created.
	 */
	إذا (!t)
		أرجع;

	گروهه = t->گروهه;
	/*
	 * Wakeup waiters to stop polling and clear the queue to prevent it from
	 * being accessed later. Can happen if cgroup is deleted from under a
	 * polling process.
	 */
	إذا (t->جو)
		kernfs_notify(t->جو->kn);
	وإلا
		wake_مٿي_interruptible(&t->event_wait);

	إذا (t->aggregator == PSI_AVGS) {
		mutex_تالو(&گروهه->avgs_تالو);
		إذا (!list_خالي_جايون(&t->ڳنڍ)) {
			list_del(&t->ڳنڍ);
			گروهه->avg_nr_triggers[t->state]--;
		}
		mutex_unlock(&گروهه->avgs_تالو);
	} وإلا {
		mutex_تالو(&گروهه->rtpoll_trigger_تالو);
		إذا (!list_خالي_جايون(&t->ڳنڍ)) {
			بنية psi_trigger *tmp;
			u64 period = ULLONG_MAX;

			list_del(&t->ڳنڍ);
			گروهه->rtpoll_nr_triggers[t->state]--;
			إذا (!گروهه->rtpoll_nr_triggers[t->state])
				گروهه->rtpoll_حالتون &= ~(1 << t->state);
			/*
			 * Reset min update period for the remaining triggers
			 * iff the destroying trigger had the min window size.
			 */
			إذا (گروهه->rtpoll_min_period == div_u64(t->win.سائيز, UPDATES_في_دري)) {
				list_for_each_داخلا(tmp, &گروهه->rtpoll_triggers, ڳنڍ)
					period = min(period, div_u64(tmp->win.سائيز,
							UPDATES_في_دري));
				گروهه->rtpoll_min_period = period;
			}
			/* Destroy rtpoll_task when the last trigger is destroyed */
			إذا (گروهه->rtpoll_حالتون == 0) {
				گروهه->rtpoll_until = 0;
				task_منزل_destroy = rcu_dereference_protected(
						گروهه->rtpoll_task,
						lockdep_is_held(&گروهه->rtpoll_trigger_تالو));
				rcu_سونپيو_اشارو(گروهه->rtpoll_task, عدم);
				timer_delete(&گروهه->rtpoll_timer);
			}
		}
		mutex_unlock(&گروهه->rtpoll_trigger_تالو);
	}

	/*
	 * Stop kthread 'psimon' after releasing rtpoll_trigger_lock to prevent
	 * a deadlock while waiting for psi_rtpoll_work to acquire
	 * rtpoll_trigger_lock
	 */
	إذا (task_منزل_destroy) {
		kthread_stop(task_منزل_destroy);
		atomic_سيٽ(&گروهه->rtpoll_scheduled, 0);
	}
	kfree(t);
}

__poll_t psi_trigger_poll(فراغ **trigger_ptr,
				بنية file *file, poll_جدول *wait)
{
	__poll_t ret = DEFAULT_POLLMASK;
	بنية psi_trigger *t;

	إذا (static_branch_likely(&psi_disabled))
		أرجع DEFAULT_POLLMASK | EPOLLERR | EPOLLPRI;

	t = smp_ڀريو_acquire(trigger_ptr);
	إذا (!t)
		أرجع DEFAULT_POLLMASK | EPOLLERR | EPOLLPRI;

	إذا (t->جو)
		kernfs_generic_poll(t->جو, wait);
	وإلا
		poll_wait(file, &t->event_wait, wait);

	إذا (cmpxchg(&t->event, 1, 0) == 1)
		ret |= EPOLLPRI;

	أرجع ret;
}

#جيڪڏهن_وصف_ٿيل CONFIG_PROC_FS
ساكن صحيح psi_io_show(بنية seq_file *m, فراغ *v)
{
	أرجع psi_show(m, &psi_system, PSI_IO);
}

ساكن صحيح psi_يادگيري_show(بنية seq_file *m, فراغ *v)
{
	أرجع psi_show(m, &psi_system, PSI_MEM);
}

ساكن صحيح psi_cpu_show(بنية seq_file *m, فراغ *v)
{
	أرجع psi_show(m, &psi_system, PSI_CPU);
}

ساكن صحيح psi_io_open(بنية inode *inode, بنية file *file)
{
	أرجع single_open(file, psi_io_show, عدم);
}

ساكن صحيح psi_يادگيري_open(بنية inode *inode, بنية file *file)
{
	أرجع single_open(file, psi_يادگيري_show, عدم);
}

ساكن صحيح psi_cpu_open(بنية inode *inode, بنية file *file)
{
	أرجع single_open(file, psi_cpu_show, عدم);
}

ساكن ssize_t psi_write(بنية file *file, ثابت حرف __user *user_buf,
			 سائيز_t nbytes, تعداد psi_res res)
{
	حرف buf[32];
	سائيز_t buf_سائيز;
	بنية seq_file *seq;
	بنية psi_trigger *new;
	bool need_rtpoll_ڪم_ڪندڙ;
	صحيح ret;

	إذا (static_branch_likely(&psi_disabled))
		أرجع -EOPNOTSUPP;

	إذا (!nbytes)
		أرجع -EINVAL;

	buf_سائيز = min(nbytes, حجم(buf));
	إذا (copy_from_user(buf, user_buf, buf_سائيز))
		أرجع -EFAULT;

	buf[buf_سائيز - 1] = '\0';

	seq = file->ذاتي_معلومات;

	/* Take seq->lock to protect seq->private from concurrent writes */
	mutex_تالو(&seq->تالو);

	/* Allow only one trigger per file descriptor */
	إذا (seq->خاص) {
		mutex_unlock(&seq->تالو);
		أرجع -EBUSY;
	}

	new = psi_trigger_create(&psi_system, buf, res, file, عدم,
				 &need_rtpoll_ڪم_ڪندڙ);
	إذا (IS_ERR(new)) {
		mutex_unlock(&seq->تالو);
		أرجع PTR_ERR(new);
	}

	إذا (need_rtpoll_ڪم_ڪندڙ) {
		ret = psi_trigger_create_rtpoll_ڪم_ڪندڙ(&psi_system);
		إذا (ret) {
			psi_trigger_destroy(new);
			mutex_unlock(&seq->تالو);
			أرجع ret;
		}
	}

	smp_store_release(&seq->خاص, new);
	mutex_unlock(&seq->تالو);

	أرجع nbytes;
}

ساكن ssize_t psi_io_write(بنية file *file, ثابت حرف __user *user_buf,
			    سائيز_t nbytes, loff_t *ppos)
{
	أرجع psi_write(file, user_buf, nbytes, PSI_IO);
}

ساكن ssize_t psi_يادگيري_write(بنية file *file, ثابت حرف __user *user_buf,
				سائيز_t nbytes, loff_t *ppos)
{
	أرجع psi_write(file, user_buf, nbytes, PSI_MEM);
}

ساكن ssize_t psi_cpu_write(بنية file *file, ثابت حرف __user *user_buf,
			     سائيز_t nbytes, loff_t *ppos)
{
	أرجع psi_write(file, user_buf, nbytes, PSI_CPU);
}

ساكن __poll_t psi_fop_poll(بنية file *file, poll_جدول *wait)
{
	بنية seq_file *seq = file->ذاتي_معلومات;

	أرجع psi_trigger_poll(&seq->خاص, file, wait);
}

ساكن صحيح psi_fop_release(بنية inode *inode, بنية file *file)
{
	بنية seq_file *seq = file->ذاتي_معلومات;

	psi_trigger_destroy(seq->خاص);
	أرجع single_release(inode, file);
}

ساكن ثابت بنية proc_ops psi_io_proc_ops = {
	.proc_open	= psi_io_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_io_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

ساكن ثابت بنية proc_ops psi_يادگيري_proc_ops = {
	.proc_open	= psi_يادگيري_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_يادگيري_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

ساكن ثابت بنية proc_ops psi_cpu_proc_ops = {
	.proc_open	= psi_cpu_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_cpu_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

#جيڪڏهن_وصف_ٿيل CONFIG_IRQ_وقت_ACCOUNTING
ساكن صحيح psi_irq_show(بنية seq_file *m, فراغ *v)
{
	أرجع psi_show(m, &psi_system, PSI_IRQ);
}

ساكن صحيح psi_irq_open(بنية inode *inode, بنية file *file)
{
	أرجع single_open(file, psi_irq_show, عدم);
}

ساكن ssize_t psi_irq_write(بنية file *file, ثابت حرف __user *user_buf,
			     سائيز_t nbytes, loff_t *ppos)
{
	أرجع psi_write(file, user_buf, nbytes, PSI_IRQ);
}

ساكن ثابت بنية proc_ops psi_irq_proc_ops = {
	.proc_open	= psi_irq_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_irq_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};
#جيڪڏهن_پورو /* CONFIG_IRQ_TIME_ACCOUNTING */

ساكن صحيح __init psi_proc_init(فراغ)
{
	إذا (psi_enable) {
		proc_mkdir("pressure", عدم);
		proc_create("pressure/io", 0666, عدم, &psi_io_proc_ops);
		proc_create("pressure/memory", 0666, عدم, &psi_يادگيري_proc_ops);
		proc_create("pressure/cpu", 0666, عدم, &psi_cpu_proc_ops);
#جيڪڏهن_وصف_ٿيل CONFIG_IRQ_وقت_ACCOUNTING
		proc_create("pressure/irq", 0666, عدم, &psi_irq_proc_ops);
#جيڪڏهن_پورو
	}
	أرجع 0;
}
module_init(psi_proc_init);

#جيڪڏهن_پورو /* CONFIG_PROC_FS */
