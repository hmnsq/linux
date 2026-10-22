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
#شامل <linux/sched/clock.h>
#شامل <linux/workqueue.h>
#شامل <linux/psi.h>
#شامل "sched.h"

ساکن صحیح psi_bug __read_mostly;

DEFINE_STATIC_کلید_FALSE(psi_disabled);
ساکن DEFINE_STATIC_کلید_TRUE(psi_cgroups_enabled);

#اگر_تعریف_شدہ CONFIG_PSI_DEFAULT_DISABLED
ساکن bool psi_enable;
#ورنہ
ساکن bool psi_enable = true;
#اگر_ختم
ساکن صحیح __init تیاری_psi(حرف *str)
{
	واپس kstrtobool(str, &psi_enable) == 0;
}
__setup("psi=", تیاری_psi);

/* Running averages - we need to be higher-res than loadavg */
#تعریف_کرو PSI_FREQ	(2*HZ+1)	/* 2 sec intervals */
#تعریف_کرو EXP_10s		1677		/* 1/exp(2s/10s) as fixed-point */
#تعریف_کرو EXP_60s		1981		/* 1/exp(2s/60s) */
#تعریف_کرو EXP_300s	2034		/* 1/exp(2s/300s) */

/* PSI trigger definitions */
#تعریف_کرو کھڑکی_MAX_US 10000000	/* Max window size is 10s */
#تعریف_کرو UPDATES_فی_کھڑکی 10	/* 10 updates per window */

/* Sampling frequency in nanoseconds */
ساکن u64 psi_period __read_mostly;

/* System-level pressure and stall tracking */
ساکن DEFINE_فی_CPU(ساخت psi_گروہ_cpu, system_گروہ_pcpu);
ساخت psi_گروہ psi_system = {
	.pcpu = &system_گروہ_pcpu,
};

ساکن DEFINE_فی_CPU(seqcount_t, psi_seq) = SEQCNT_ZERO(psi_seq);

ساکن اندرونی خالی psi_write_begin(صحیح cpu)
{
	write_seqcount_begin(فی_cpu_ptr(&psi_seq, cpu));
}

ساکن اندرونی خالی psi_write_انتہا(صحیح cpu)
{
	write_seqcount_انتہا(فی_cpu_ptr(&psi_seq, cpu));
}

ساکن اندرونی u32 psi_read_begin(صحیح cpu)
{
	واپس read_seqcount_begin(فی_cpu_ptr(&psi_seq, cpu));
}

ساکن اندرونی bool psi_read_retry(صحیح cpu, u32 seq)
{
	واپس read_seqcount_retry(فی_cpu_ptr(&psi_seq, cpu), seq);
}

ساکن خالی psi_avgs_work(ساخت work_struct *work);

ساکن خالی poll_timer_fn(ساخت timer_list *t);

ساکن خالی گروہ_init(ساخت psi_گروہ *گروہ)
{
	گروہ->enabled = true;
	گروہ->avg_آخری_update = sched_clock();
	گروہ->avg_اگلا_update = گروہ->avg_آخری_update + psi_period;
	mutex_init(&گروہ->avgs_تالا);

	/* Init avg trigger-related members */
	INIT_LIST_سر(&گروہ->avg_triggers);
	memset(گروہ->avg_nr_triggers, 0, حجم(گروہ->avg_nr_triggers));
	INIT_DELAYED_WORK(&گروہ->avgs_work, psi_avgs_work);

	/* Init rtpoll trigger-related members */
	atomic_سیٹ(&گروہ->rtpoll_scheduled, 0);
	mutex_init(&گروہ->rtpoll_trigger_تالا);
	INIT_LIST_سر(&گروہ->rtpoll_triggers);
	گروہ->rtpoll_min_period = U32_MAX;
	گروہ->rtpoll_اگلا_update = ULLONG_MAX;
	init_waitqueue_سر(&گروہ->rtpoll_wait);
	timer_تیاری(&گروہ->rtpoll_timer, poll_timer_fn, 0);
	rcu_تفویض_اشارہ_گر(گروہ->rtpoll_task, تہی);
}

خالی __init psi_init(خالی)
{
	اگر (!psi_enable) {
		static_branch_enable(&psi_disabled);
		static_branch_disable(&psi_cgroups_enabled);
		واپس;
	}

	اگر (!cgroup_psi_enabled())
		static_branch_disable(&psi_cgroups_enabled);

	psi_period = jiffies_مقصد_nsecs(PSI_FREQ);
	گروہ_init(&psi_system);
}

ساکن u32 test_حالتیں(غیرعلامتی صحیح *tasks, u32 state_نقاب)
{
	ثابت bool oncpu = state_نقاب & PSI_ONCPU;

	اگر (tasks[NR_IOWAIT]) {
		state_نقاب |= ثنائی_ہندسہ(PSI_IO_SOME);
		اگر (!tasks[NR_RUNNING])
			state_نقاب |= ثنائی_ہندسہ(PSI_IO_بھرا);
	}

	اگر (tasks[NR_MEMSTALL]) {
		state_نقاب |= ثنائی_ہندسہ(PSI_MEM_SOME);
		اگر (tasks[NR_RUNNING] == tasks[NR_MEMSTALL_RUNNING])
			state_نقاب |= ثنائی_ہندسہ(PSI_MEM_بھرا);
	}

	اگر (tasks[NR_RUNNING] > oncpu)
		state_نقاب |= ثنائی_ہندسہ(PSI_CPU_SOME);

	اگر (tasks[NR_RUNNING] && !oncpu)
		state_نقاب |= ثنائی_ہندسہ(PSI_CPU_بھرا);

	اگر (tasks[NR_IOWAIT] || tasks[NR_MEMSTALL] || tasks[NR_RUNNING])
		state_نقاب |= ثنائی_ہندسہ(PSI_NONIDLE);

	واپس state_نقاب;
}

ساکن خالی get_recent_times(ساخت psi_گروہ *گروہ, صحیح cpu,
			     شمار psi_aggregators aggregator, u32 *times,
			     u32 *pchanged_حالتیں)
{
	ساخت psi_گروہ_cpu *groupc = فی_cpu_ptr(گروہ->pcpu, cpu);
	صحیح current_cpu = raw_smp_processor_id();
	غیرعلامتی صحیح tasks[NR_PSI_TASK_COUNTS];
	u64 now, state_آغاز;
	شمار psi_حالتیں s;
	غیرعلامتی صحیح seq;
	u32 state_نقاب;

	*pchanged_حالتیں = 0;

	/* Snapshot a coherent view of the CPU state */
	کرو {
		seq = psi_read_begin(cpu);
		now = cpu_clock(cpu);
		حافظہ_نقل(times, groupc->times, حجم(groupc->times));
		state_نقاب = groupc->state_نقاب;
		state_آغاز = groupc->state_آغاز;
		اگر (cpu == current_cpu)
			حافظہ_نقل(tasks, groupc->tasks, حجم(groupc->tasks));
	} جب (psi_read_retry(cpu, seq));

	/* Calculate state time deltas against the previous snapshot */
	برائے (s = 0; s < NR_PSI_حالتیں; s++) {
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
		اگر (state_نقاب & (1 << s))
			times[s] += now - state_آغاز;

		delta = times[s] - groupc->times_پچھلا[aggregator][s];
		groupc->times_پچھلا[aggregator][s] = times[s];

		times[s] = delta;
		اگر (delta)
			*pchanged_حالتیں |= (1 << s);
	}

	/*
	 * When collect_percpu_times() from the avgs_work, we don't want to
	 * re-arm avgs_work when all CPUs are IDLE. But the current CPU running
	 * this avgs_work is never IDLE, cause avgs_work can't be shut off.
	 * So for the current CPU, we need to re-arm avgs_work only when
	 * (NR_RUNNING > 1 || NR_IOWAIT > 0 || NR_MEMSTALL > 0), for other CPUs
	 * we can just check PSI_NONIDLE delta.
	 */
	اگر (current_work() == &گروہ->avgs_work.work) {
		bool reschedule;

		اگر (cpu == current_cpu)
			reschedule = tasks[NR_RUNNING] +
				     tasks[NR_IOWAIT] +
				     tasks[NR_MEMSTALL] > 1;
		ورنہ
			reschedule = *pchanged_حالتیں & (1 << PSI_NONIDLE);

		اگر (reschedule)
			*pchanged_حالتیں |= PSI_STATE_RESCHEDULE;
	}
}

ساکن خالی calc_avgs(غیرعلامتی طویل avg[3], صحیح missed_periods,
		      u64 وقت, u64 period)
{
	غیرعلامتی طویل pct;

	/* Fill in zeroes for periods of no activity */
	اگر (missed_periods) {
		avg[0] = calc_بھرو_n(avg[0], EXP_10s, 0, missed_periods);
		avg[1] = calc_بھرو_n(avg[1], EXP_60s, 0, missed_periods);
		avg[2] = calc_بھرو_n(avg[2], EXP_300s, 0, missed_periods);
	}

	/* Sample the most recent active period */
	pct = div_u64(وقت * 100, period);
	pct *= FIXED_1;
	avg[0] = calc_بھرو(avg[0], EXP_10s, pct);
	avg[1] = calc_بھرو(avg[1], EXP_60s, pct);
	avg[2] = calc_بھرو(avg[2], EXP_300s, pct);
}

ساکن خالی collect_percpu_times(ساخت psi_گروہ *گروہ,
				 شمار psi_aggregators aggregator,
				 u32 *pchanged_حالتیں)
{
	u64 deltas[NR_PSI_حالتیں - 1] = { 0, };
	غیرعلامتی طویل nonidle_کل = 0;
	u32 بدلا_حالتیں = 0;
	صحیح cpu;
	صحیح s;

	/*
	 * Collect the per-cpu time buckets and average them into a
	 * single time sample that is normalized to wall clock time.
	 *
	 * For averaging, each CPU is weighted by its non-idle time in
	 * the sampling period. This eliminates artifacts from uneven
	 * loading, or even entirely idle CPUs.
	 */
	for_each_possible_cpu(cpu) {
		u32 times[NR_PSI_حالتیں];
		u32 nonidle;
		u32 cpu_بدلا_حالتیں;

		get_recent_times(گروہ, cpu, aggregator, times,
				&cpu_بدلا_حالتیں);
		بدلا_حالتیں |= cpu_بدلا_حالتیں;

		nonidle = nsecs_مقصد_jiffies(times[PSI_NONIDLE]);
		nonidle_کل += nonidle;

		برائے (s = 0; s < PSI_NONIDLE; s++)
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
	برائے (s = 0; s < NR_PSI_حالتیں - 1; s++)
		گروہ->کل[aggregator][s] +=
				div_u64(deltas[s], max(nonidle_کل, 1UL));

	اگر (pchanged_حالتیں)
		*pchanged_حالتیں = بدلا_حالتیں;
}

/* Trigger tracking window manipulations */
ساکن خالی کھڑکی_reset(ساخت psi_کھڑکی *win, u64 now, u64 قدر,
			 u64 پچھلا_growth)
{
	win->آغاز_وقت = now;
	win->آغاز_قدر = قدر;
	win->پچھلا_growth = پچھلا_growth;
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
ساکن u64 کھڑکی_update(ساخت psi_کھڑکی *win, u64 now, u64 قدر)
{
	u64 elapsed;
	u64 growth;

	elapsed = now - win->آغاز_وقت;
	growth = قدر - win->آغاز_قدر;
	/*
	 * After each tracking window passes win->start_value and
	 * win->start_time get reset and win->prev_growth stores
	 * the average per-window growth of the previous window.
	 * win->prev_growth is then used to interpolate additional
	 * growth from the previous window assuming it was linear.
	 */
	اگر (elapsed > win->سائز)
		کھڑکی_reset(win, now, قدر, growth);
	ورنہ {
		u32 بقایا;

		بقایا = win->سائز - elapsed;
		growth += div64_u64(win->پچھلا_growth * بقایا, win->سائز);
	}

	واپس growth;
}

ساکن خالی update_triggers(ساخت psi_گروہ *گروہ, u64 now,
						   شمار psi_aggregators aggregator)
{
	ساخت psi_trigger *t;
	u64 *کل = گروہ->کل[aggregator];
	ساخت list_سر *triggers;
	u64 *aggregator_کل;

	اگر (aggregator == PSI_AVGS) {
		triggers = &گروہ->avg_triggers;
		aggregator_کل = گروہ->avg_کل;
	} ورنہ {
		triggers = &گروہ->rtpoll_triggers;
		aggregator_کل = گروہ->rtpoll_کل;
	}

	/*
	 * On subsequent updates, calculate growth deltas and let
	 * watchers know when their specified thresholds are exceeded.
	 */
	list_for_each_اندراج(t, triggers, گرہ) {
		u64 growth;
		bool new_stall;

		new_stall = aggregator_کل[t->state] != کل[t->state];

		/* Check for stall activity or a previous threshold breach */
		اگر (!new_stall && !t->pending_event)
			جاری;
		/*
		 * Check for new stall activity, as well as deferred
		 * events that occurred in the last window after the
		 * trigger had already fired (we want to ratelimit
		 * events without dropping any).
		 */
		اگر (new_stall) {
			/* Calculate growth since last update */
			growth = کھڑکی_update(&t->win, now, کل[t->state]);
			اگر (!t->pending_event) {
				اگر (growth < t->حد)
					جاری;

				t->pending_event = true;
			}
		}
		/* Limit event signaling to once per window */
		اگر (now < t->آخری_event_وقت + t->win.سائز)
			جاری;

		/* Generate an event */
		اگر (cmpxchg(&t->event, 0, 1) == 0) {
			اگر (t->کا)
				kernfs_notify(t->کا->kn);
			ورنہ
				wake_اوپر_interruptible(&t->event_wait);
		}
		t->آخری_event_وقت = now;
		/* Reset threshold breach flag once event got generated */
		t->pending_event = false;
	}
}

ساکن u64 update_averages(ساخت psi_گروہ *گروہ, u64 now)
{
	غیرعلامتی طویل missed_periods = 0;
	u64 expires, period;
	u64 avg_اگلا_update;
	صحیح s;

	/* avgX= */
	expires = گروہ->avg_اگلا_update;
	اگر (now - expires >= psi_period)
		missed_periods = div_u64(now - expires, psi_period);

	/*
	 * The periodic clock tick can get delayed for various
	 * reasons, especially on loaded systems. To avoid clock
	 * drift, we schedule the clock in fixed psi_period intervals.
	 * But the deltas we sample out of the per-cpu buckets above
	 * are based on the actual time elapsing between clock ticks.
	 */
	avg_اگلا_update = expires + ((1 + missed_periods) * psi_period);
	period = now - (گروہ->avg_آخری_update + (missed_periods * psi_period));
	گروہ->avg_آخری_update = now;

	برائے (s = 0; s < NR_PSI_حالتیں - 1; s++) {
		u32 sample;

		sample = گروہ->کل[PSI_AVGS][s] - گروہ->avg_کل[s];
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
		اگر (sample > period)
			sample = period;
		گروہ->avg_کل[s] += sample;
		calc_avgs(گروہ->avg[s], missed_periods, sample, period);
	}

	واپس avg_اگلا_update;
}

ساکن خالی psi_avgs_work(ساخت work_struct *work)
{
	ساخت delayed_work *dwork;
	ساخت psi_گروہ *گروہ;
	u32 بدلا_حالتیں;
	u64 now;

	dwork = مقصد_delayed_work(work);
	گروہ = container_کا(dwork, ساخت psi_گروہ, avgs_work);

	mutex_تالا(&گروہ->avgs_تالا);

	now = sched_clock();

	collect_percpu_times(گروہ, PSI_AVGS, &بدلا_حالتیں);
	/*
	 * If there is task activity, periodically fold the per-cpu
	 * times and feed samples into the running averages. If things
	 * are idle and there is no data to process, stop the clock.
	 * Once restarted, we'll catch up the running averages in one
	 * go - see calc_avgs() and missed_periods.
	 */
	اگر (now >= گروہ->avg_اگلا_update) {
		update_triggers(گروہ, now, PSI_AVGS);
		گروہ->avg_اگلا_update = update_averages(گروہ, now);
	}

	اگر (بدلا_حالتیں & PSI_STATE_RESCHEDULE) {
		جدول_وقت_delayed_work(dwork, nsecs_مقصد_jiffies(
				گروہ->avg_اگلا_update - now) + 1);
	}

	mutex_unlock(&گروہ->avgs_تالا);
}

ساکن خالی init_rtpoll_triggers(ساخت psi_گروہ *گروہ, u64 now)
{
	ساخت psi_trigger *t;

	list_for_each_اندراج(t, &گروہ->rtpoll_triggers, گرہ)
		کھڑکی_reset(&t->win, now,
				گروہ->کل[PSI_POLL][t->state], 0);
	حافظہ_نقل(گروہ->rtpoll_کل, گروہ->کل[PSI_POLL],
		   حجم(گروہ->rtpoll_کل));
	گروہ->rtpoll_اگلا_update = now + گروہ->rtpoll_min_period;
}

/* Schedule rtpolling if it's not already scheduled or forced. */
ساکن خالی psi_جدول_وقت_rtpoll_work(ساخت psi_گروہ *گروہ, غیرعلامتی طویل delay,
				   bool force)
{
	/*
	 * atomic_xchg should be called even when !force to provide a
	 * full memory barrier (see the comment inside psi_rtpoll_work).
	 */
	اگر (atomic_xchg(&گروہ->rtpoll_scheduled, 1) && !force)
		واپس;

	/*
	 * Only test whether a worker is installed; do not dereference the task.
	 * A racing trigger destruction may leave the timer armed.
	 * psi_cgroup_free() shuts it down before freeing the group.
	 * The system PSI group is static.
	 */
	اگر (likely(rcu_access_اشارہ_گر(گروہ->rtpoll_task)))
		mod_timer(&گروہ->rtpoll_timer, jiffies + delay);
	ورنہ
		atomic_سیٹ(&گروہ->rtpoll_scheduled, 0);
}

ساکن خالی psi_rtpoll_work(ساخت psi_گروہ *گروہ)
{
	bool force_reschedule = false;
	u32 بدلا_حالتیں;
	u64 now;

	mutex_تالا(&گروہ->rtpoll_trigger_تالا);

	now = sched_clock();

	اگر (now > گروہ->rtpoll_until) {
		/*
		 * We are either about to start or might stop rtpolling if no
		 * state change was recorded. Resetting rtpoll_scheduled leaves
		 * a small window for psi_group_change to sneak in and schedule
		 * an immediate rtpoll_work before we get to rescheduling. One
		 * potential extra wakeup at the end of the rtpolling window
		 * should be negligible and rtpoll_next_update still keeps
		 * updates correctly on schedule.
		 */
		atomic_سیٹ(&گروہ->rtpoll_scheduled, 0);
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
	} ورنہ {
		/* The rtpolling window is not over, keep rescheduling */
		force_reschedule = true;
	}


	collect_percpu_times(گروہ, PSI_POLL, &بدلا_حالتیں);

	اگر (بدلا_حالتیں & گروہ->rtpoll_حالتیں) {
		/* Initialize trigger windows when entering rtpolling mode */
		اگر (now > گروہ->rtpoll_until)
			init_rtpoll_triggers(گروہ, now);

		/*
		 * Keep the monitor active for at least the duration of the
		 * minimum tracking window as long as monitor states are
		 * changing.
		 */
		گروہ->rtpoll_until = now +
			گروہ->rtpoll_min_period * UPDATES_فی_کھڑکی;
	}

	اگر (now > گروہ->rtpoll_until) {
		گروہ->rtpoll_اگلا_update = ULLONG_MAX;
		جاؤ برآمد;
	}

	اگر (now >= گروہ->rtpoll_اگلا_update) {
		اگر (بدلا_حالتیں & گروہ->rtpoll_حالتیں) {
			update_triggers(گروہ, now, PSI_POLL);
			حافظہ_نقل(گروہ->rtpoll_کل, گروہ->کل[PSI_POLL],
				   حجم(گروہ->rtpoll_کل));
		}
		گروہ->rtpoll_اگلا_update = now + گروہ->rtpoll_min_period;
	}

	psi_جدول_وقت_rtpoll_work(گروہ,
		nsecs_مقصد_jiffies(گروہ->rtpoll_اگلا_update - now) + 1,
		force_reschedule);

برآمد:
	mutex_unlock(&گروہ->rtpoll_trigger_تالا);
}

ساکن صحیح psi_rtpoll_کارکن(خالی *ڈیٹا)
{
	ساخت psi_گروہ *گروہ = (ساخت psi_گروہ *)ڈیٹا;

	sched_سیٹ_fifo_کم(current);

	جب (true) {
		wait_event_interruptible(گروہ->rtpoll_wait,
				atomic_cmpxchg(&گروہ->rtpoll_wakeup, 1, 0) ||
				kthread_should_stop());
		اگر (kthread_should_stop())
			توڑو;

		psi_rtpoll_work(گروہ);
	}
	واپس 0;
}

ساکن خالی poll_timer_fn(ساخت timer_list *t)
{
	ساخت psi_گروہ *گروہ = timer_container_کا(گروہ, t, rtpoll_timer);

	atomic_سیٹ(&گروہ->rtpoll_wakeup, 1);
	wake_اوپر_interruptible(&گروہ->rtpoll_wait);
}

ساکن خالی ریکارڈ_times(ساخت psi_گروہ_cpu *groupc, u64 now)
{
	u32 delta;

	delta = now - groupc->state_آغاز;
	groupc->state_آغاز = now;

	اگر (groupc->state_نقاب & (1 << PSI_IO_SOME)) {
		groupc->times[PSI_IO_SOME] += delta;
		اگر (groupc->state_نقاب & (1 << PSI_IO_بھرا))
			groupc->times[PSI_IO_بھرا] += delta;
	}

	اگر (groupc->state_نقاب & (1 << PSI_MEM_SOME)) {
		groupc->times[PSI_MEM_SOME] += delta;
		اگر (groupc->state_نقاب & (1 << PSI_MEM_بھرا))
			groupc->times[PSI_MEM_بھرا] += delta;
	}

	اگر (groupc->state_نقاب & (1 << PSI_CPU_SOME)) {
		groupc->times[PSI_CPU_SOME] += delta;
		اگر (groupc->state_نقاب & (1 << PSI_CPU_بھرا))
			groupc->times[PSI_CPU_بھرا] += delta;
	}

	اگر (groupc->state_نقاب & (1 << PSI_NONIDLE))
		groupc->times[PSI_NONIDLE] += delta;
}

#تعریف_کرو for_each_گروہ(iter, گروہ) \
	برائے (typeof(گروہ) iter = گروہ; iter; iter = iter->والد)

ساکن خالی psi_گروہ_change(ساخت psi_گروہ *گروہ, صحیح cpu,
			     غیرعلامتی صحیح clear, غیرعلامتی صحیح سیٹ,
			     u64 now, bool wake_clock)
{
	ساخت psi_گروہ_cpu *groupc;
	غیرعلامتی صحیح t, m;
	u32 state_نقاب;

	lockdep_assert_rq_held(cpu_rq(cpu));
	groupc = فی_cpu_ptr(گروہ->pcpu, cpu);

	/*
	 * Start with TSK_ONCPU, which doesn't have a corresponding
	 * task count - it's just a boolean flag directly encoded in
	 * the state mask. Clear, set, or carry the current state if
	 * no changes are requested.
	 */
	اگر (unlikely(clear & TSK_ONCPU)) {
		state_نقاب = 0;
		clear &= ~TSK_ONCPU;
	} ورنہ اگر (unlikely(سیٹ & TSK_ONCPU)) {
		state_نقاب = PSI_ONCPU;
		سیٹ &= ~TSK_ONCPU;
	} ورنہ {
		state_نقاب = groupc->state_نقاب & PSI_ONCPU;
	}

	/*
	 * The rest of the state mask is calculated based on the task
	 * counts. Update those first, then construct the mask.
	 */
	برائے (t = 0, m = clear; m; m &= ~(1 << t), t++) {
		اگر (!(m & (1 << t)))
			جاری;
		اگر (groupc->tasks[t]) {
			groupc->tasks[t]--;
		} ورنہ اگر (!psi_bug) {
			printk_deferred(KERN_ERR "psi: task underflow! cpu=%d t=%d tasks=[%u %u %u %u] clear=%x set=%x\n",
					cpu, t, groupc->tasks[0],
					groupc->tasks[1], groupc->tasks[2],
					groupc->tasks[3], clear, سیٹ);
			psi_bug = 1;
		}
	}

	برائے (t = 0; سیٹ; سیٹ &= ~(1 << t), t++)
		اگر (سیٹ & (1 << t))
			groupc->tasks[t]++;

	اگر (!گروہ->enabled) {
		/*
		 * On the first group change after disabling PSI, conclude
		 * the current state and flush its time. This is unlikely
		 * to matter to the user, but aggregation (get_recent_times)
		 * may have already incorporated the live state into times_prev;
		 * avoid a delta sample underflow when PSI is later re-enabled.
		 */
		اگر (unlikely(groupc->state_نقاب & (1 << PSI_NONIDLE)))
			ریکارڈ_times(groupc, now);

		groupc->state_نقاب = state_نقاب;

		واپس;
	}

	state_نقاب = test_حالتیں(groupc->tasks, state_نقاب);

	/*
	 * Since we care about lost potential, a memstall is FULL
	 * when there are no other working tasks, but also when
	 * the CPU is actively reclaiming and nothing productive
	 * could run even if it were runnable. So when the current
	 * task in a cgroup is in_memstall, the corresponding groupc
	 * on that cpu is in PSI_MEM_FULL state.
	 */
	اگر (unlikely((state_نقاب & PSI_ONCPU) && cpu_curr(cpu)->اندر_memstall))
		state_نقاب |= (1 << PSI_MEM_بھرا);

	ریکارڈ_times(groupc, now);

	groupc->state_نقاب = state_نقاب;

	اگر (state_نقاب & گروہ->rtpoll_حالتیں)
		psi_جدول_وقت_rtpoll_work(گروہ, 1, false);

	اگر (wake_clock && !delayed_work_pending(&گروہ->avgs_work))
		جدول_وقت_delayed_work(&گروہ->avgs_work, PSI_FREQ);
}

ساکن اندرونی ساخت psi_گروہ *task_psi_گروہ(ساخت task_struct *task)
{
#اگر_تعریف_شدہ CONFIG_CGROUPS
	اگر (static_branch_likely(&psi_cgroups_enabled))
		واپس cgroup_psi(task_dfl_cgroup(task));
#اگر_ختم
	واپس &psi_system;
}

ساکن خالی psi_flags_change(ساخت task_struct *task, صحیح clear, صحیح سیٹ)
{
	اگر (((task->psi_flags & سیٹ) ||
	     (task->psi_flags & clear) != clear) &&
	    !psi_bug) {
		printk_deferred(KERN_ERR "psi: inconsistent task state! task=%d:%s cpu=%d psi_flags=%x clear=%x set=%x\n",
				task->pid, task->comm, task_cpu(task),
				task->psi_flags, clear, سیٹ);
		psi_bug = 1;
	}

	task->psi_flags &= ~clear;
	task->psi_flags |= سیٹ;
}

خالی psi_task_change(ساخت task_struct *task, صحیح clear, صحیح سیٹ)
{
	صحیح cpu = task_cpu(task);
	u64 now;

	اگر (!task->pid)
		واپس;

	psi_flags_change(task, clear, سیٹ);

	psi_write_begin(cpu);
	now = cpu_clock(cpu);
	for_each_گروہ(گروہ, task_psi_گروہ(task))
		psi_گروہ_change(گروہ, cpu, clear, سیٹ, now, true);
	psi_write_انتہا(cpu);
}

خالی psi_task_switch(ساخت task_struct *پچھلا, ساخت task_struct *اگلا,
		     bool sleep)
{
	ساخت psi_گروہ *common = تہی;
	صحیح cpu = task_cpu(پچھلا);
	u64 now;

	psi_write_begin(cpu);
	now = cpu_clock(cpu);

	اگر (اگلا->pid) {
		psi_flags_change(اگلا, 0, TSK_ONCPU);
		/*
		 * Set TSK_ONCPU on @next's cgroups. If @next shares any
		 * ancestors with @prev, those will already have @prev's
		 * TSK_ONCPU bit set, and we can stop the iteration there.
		 */
		for_each_گروہ(گروہ, task_psi_گروہ(اگلا)) {
			ساخت psi_گروہ_cpu *groupc = فی_cpu_ptr(گروہ->pcpu, cpu);

			اگر (groupc->state_نقاب & PSI_ONCPU) {
				common = گروہ;
				توڑو;
			}
			psi_گروہ_change(گروہ, cpu, 0, TSK_ONCPU, now, true);
		}
	}

	اگر (پچھلا->pid) {
		صحیح clear = TSK_ONCPU, سیٹ = 0;
		bool wake_clock = true;

		/*
		 * When we're going to sleep, psi_dequeue() lets us
		 * handle TSK_RUNNING, TSK_MEMSTALL_RUNNING and
		 * TSK_IOWAIT here, where we can combine it with
		 * TSK_ONCPU and save walking common ancestors twice.
		 */
		اگر (sleep) {
			clear |= TSK_RUNNING;
			اگر (پچھلا->اندر_memstall)
				clear |= TSK_MEMSTALL_RUNNING;
			اگر (پچھلا->اندر_iowait)
				سیٹ |= TSK_IOWAIT;

			/*
			 * Periodic aggregation shuts off if there is a period of no
			 * task changes, so we wake it back up if necessary. However,
			 * don't do this if the task change is the aggregation worker
			 * itself going to sleep, or we'll ping-pong forever.
			 */
			اگر (unlikely((پچھلا->flags & PF_WQ_کارکن) &&
				     wq_کارکن_آخری_func(پچھلا) == psi_avgs_work))
				wake_clock = false;
		}

		psi_flags_change(پچھلا, clear, سیٹ);

		for_each_گروہ(گروہ, task_psi_گروہ(پچھلا)) {
			اگر (گروہ == common)
				توڑو;
			psi_گروہ_change(گروہ, cpu, clear, سیٹ, now, wake_clock);
		}

		/*
		 * TSK_ONCPU is handled up to the common ancestor. If there are
		 * any other differences between the two tasks (e.g. prev goes
		 * to sleep, or only one task is memstall), finish propagating
		 * those differences all the way up to the root.
		 */
		اگر ((پچھلا->psi_flags ^ اگلا->psi_flags) & ~TSK_ONCPU) {
			clear &= ~TSK_ONCPU;
			for_each_گروہ(گروہ, common)
				psi_گروہ_change(گروہ, cpu, clear, سیٹ, now, wake_clock);
		}
	}
	psi_write_انتہا(cpu);
}

#اگر_تعریف_شدہ CONFIG_IRQ_وقت_ACCOUNTING
خالی psi_account_irqtime(ساخت rq *rq, ساخت task_struct *curr, ساخت task_struct *پچھلا)
{
	صحیح cpu = task_cpu(curr);
	ساخت psi_گروہ_cpu *groupc;
	s64 delta;
	u64 irq;
	u64 now;

	اگر (static_branch_likely(&psi_disabled) || !irqtime_enabled())
		واپس;

	اگر (!curr->pid)
		واپس;

	lockdep_assert_rq_held(rq);
	اگر (پچھلا && task_psi_گروہ(پچھلا) == task_psi_گروہ(curr))
		واپس;

	irq = irq_وقت_read(cpu);
	delta = (s64)(irq - rq->psi_irq_وقت);
	اگر (delta <= 0)
		واپس;
	rq->psi_irq_وقت = irq;

	psi_write_begin(cpu);
	now = cpu_clock(cpu);

	for_each_گروہ(گروہ, task_psi_گروہ(curr)) {
		اگر (!گروہ->enabled)
			جاری;

		groupc = فی_cpu_ptr(گروہ->pcpu, cpu);

		ریکارڈ_times(groupc, now);
		groupc->times[PSI_IRQ_بھرا] += delta;

		اگر (گروہ->rtpoll_حالتیں & (1 << PSI_IRQ_بھرا))
			psi_جدول_وقت_rtpoll_work(گروہ, 1, false);
	}
	psi_write_انتہا(cpu);
}
#اگر_ختم /* CONFIG_IRQ_TIME_ACCOUNTING */

/**
 * psi_memstall_enter - mark the beginning of a memory stall section
 * @flags: flags to handle nested sections
 *
 * Marks the calling task as being stalled due to a lack of memory,
 * such as waiting for a refault or performing reclaim.
 */
خالی psi_memstall_enter(غیرعلامتی طویل *flags)
{
	ساخت rq_flags rf;
	ساخت rq *rq;

	اگر (static_branch_likely(&psi_disabled))
		واپس;

	*flags = current->اندر_memstall;
	اگر (*flags)
		واپس;
	/*
	 * in_memstall setting & accounting needs to be atomic wrt
	 * changes to the task's scheduling state, otherwise we can
	 * race with CPU migration.
	 */
	rq = this_rq_تالا_irq(&rf);

	current->اندر_memstall = 1;
	psi_task_change(current, 0, TSK_MEMSTALL | TSK_MEMSTALL_RUNNING);

	rq_unlock_irq(rq, &rf);
}
EXPORT_علامت_GPL(psi_memstall_enter);

/**
 * psi_memstall_leave - mark the end of an memory stall section
 * @flags: flags to handle nested memdelay sections
 *
 * Marks the calling task as no longer stalled due to lack of memory.
 */
خالی psi_memstall_leave(غیرعلامتی طویل *flags)
{
	ساخت rq_flags rf;
	ساخت rq *rq;

	اگر (static_branch_likely(&psi_disabled))
		واپس;

	اگر (*flags)
		واپس;
	/*
	 * in_memstall clearing & accounting needs to be atomic wrt
	 * changes to the task's scheduling state, otherwise we could
	 * race with CPU migration.
	 */
	rq = this_rq_تالا_irq(&rf);

	current->اندر_memstall = 0;
	psi_task_change(current, TSK_MEMSTALL | TSK_MEMSTALL_RUNNING, 0);

	rq_unlock_irq(rq, &rf);
}
EXPORT_علامت_GPL(psi_memstall_leave);

#اگر_تعریف_شدہ CONFIG_CGROUPS
صحیح psi_cgroup_alloc(ساخت cgroup *cgroup)
{
	اگر (!static_branch_likely(&psi_cgroups_enabled))
		واپس 0;

	cgroup->psi = kzalloc_obj(ساخت psi_گروہ);
	اگر (!cgroup->psi)
		واپس -ENOMEM;

	cgroup->psi->pcpu = alloc_percpu(ساخت psi_گروہ_cpu);
	اگر (!cgroup->psi->pcpu) {
		kfree(cgroup->psi);
		واپس -ENOMEM;
	}
	گروہ_init(cgroup->psi);
	cgroup->psi->والد = cgroup_psi(cgroup_والد(cgroup));
	واپس 0;
}

خالی psi_cgroup_free(ساخت cgroup *cgroup)
{
	اگر (!static_branch_likely(&psi_cgroups_enabled))
		واپس;

	cancel_delayed_work_sync(&cgroup->psi->avgs_work);
	/*
	 * A psi_schedule_rtpoll_work() call racing the last trigger's
	 * destruction may have re-armed the timer after psi_trigger_destroy()
	 * deleted it. Spurious firing while the group is alive is harmless.
	 */
	timer_shutdown_sync(&cgroup->psi->rtpoll_timer);
	free_percpu(cgroup->psi->pcpu);
	/* All triggers must be removed by now */
	WARN_ONCE(cgroup->psi->rtpoll_حالتیں, "psi: trigger leak\n");
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
خالی cgroup_move_task(ساخت task_struct *task, ساخت css_سیٹ *مقصد)
{
	غیرعلامتی صحیح task_flags;
	ساخت rq_flags rf;
	ساخت rq *rq;

	اگر (!static_branch_likely(&psi_cgroups_enabled)) {
		/*
		 * Lame to do this here, but the scheduler cannot be locked
		 * from the outside, so we move cgroups from inside sched/.
		 */
		rcu_تفویض_اشارہ_گر(task->cgroups, مقصد);
		واپس;
	}

	rq = task_rq_تالا_2(task, &rf);

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

	اگر (task_flags)
		psi_task_change(task, task_flags, 0);

	/* See comment above */
	rcu_تفویض_اشارہ_گر(task->cgroups, مقصد);

	اگر (task_flags)
		psi_task_change(task, 0, task_flags);

	task_rq_unlock(rq, task, &rf);
}

خالی psi_cgroup_restart(ساخت psi_گروہ *گروہ)
{
	صحیح cpu;

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
	اگر (!گروہ->enabled)
		واپس;

	for_each_possible_cpu(cpu) {
		u64 now;

		guard(rq_تالا_irq)(cpu_rq(cpu));

		psi_write_begin(cpu);
		now = cpu_clock(cpu);
		psi_گروہ_change(گروہ, cpu, 0, 0, now, true);
		psi_write_انتہا(cpu);
	}
}
#اگر_ختم /* CONFIG_CGROUPS */

صحیح psi_show(ساخت seq_file *m, ساخت psi_گروہ *گروہ, شمار psi_res res)
{
	bool only_بھرا = false;
	صحیح بھرا;
	u64 now;

	اگر (static_branch_likely(&psi_disabled))
		واپس -EOPNOTSUPP;

#اگر_تعریف_شدہ CONFIG_IRQ_وقت_ACCOUNTING
	اگر (!irqtime_enabled() && res == PSI_IRQ)
		واپس -EOPNOTSUPP;
#اگر_ختم

	/* Update averages before reporting them */
	mutex_تالا(&گروہ->avgs_تالا);
	now = sched_clock();
	collect_percpu_times(گروہ, PSI_AVGS, تہی);
	اگر (now >= گروہ->avg_اگلا_update)
		گروہ->avg_اگلا_update = update_averages(گروہ, now);
	mutex_unlock(&گروہ->avgs_تالا);

#اگر_تعریف_شدہ CONFIG_IRQ_وقت_ACCOUNTING
	only_بھرا = res == PSI_IRQ;
#اگر_ختم

	برائے (بھرا = 0; بھرا < 2 - only_بھرا; بھرا++) {
		غیرعلامتی طویل avg[3] = { 0, };
		u64 کل = 0;
		صحیح w;

		/* CPU FULL is undefined at the system level */
		اگر (!(گروہ == &psi_system && res == PSI_CPU && بھرا)) {
			برائے (w = 0; w < 3; w++)
				avg[w] = گروہ->avg[res * 2 + بھرا][w];
			کل = div_u64(گروہ->کل[PSI_AVGS][res * 2 + بھرا],
					NSEC_فی_USEC);
		}

		seq_printf(m, "%s avg10=%lu.%02lu avg60=%lu.%02lu avg300=%lu.%02lu total=%llu\n",
			   بھرا || only_بھرا ? "full" : "some",
			   بھرو_INT(avg[0]), بھرو_FRAC(avg[0]),
			   بھرو_INT(avg[1]), بھرو_FRAC(avg[1]),
			   بھرو_INT(avg[2]), بھرو_FRAC(avg[2]),
			   کل);
	}

	واپس 0;
}

/*
 * Create @group's rtpoll worker after psi_trigger_create() reported the need
 * for one. kthread creation depends on the whole fork path and we don't want
 * all of that nested inside cgroup_mutex, so the caller must drop it and any
 * other lock that forks can wait behind. If two callers race, the loser stops
 * its never-woken kthread.
 */
صحیح psi_trigger_create_rtpoll_کارکن(ساخت psi_گروہ *گروہ)
{
	ساخت task_struct *task;

	task = kthread_create(psi_rtpoll_کارکن, گروہ, "psimon");
	اگر (IS_ERR(task))
		واپس PTR_ERR(task);

	scoped_guard(mutex, &گروہ->rtpoll_trigger_تالا) {
		اگر (!rcu_access_اشارہ_گر(گروہ->rtpoll_task)) {
			atomic_سیٹ(&گروہ->rtpoll_wakeup, 0);
			wake_اوپر_process(task);
			rcu_تفویض_اشارہ_گر(گروہ->rtpoll_task, task);

			/*
			 * Poll once to catch up on scheduling attempts dropped
			 * while there was no rtpoll worker.
			 */
			psi_جدول_وقت_rtpoll_work(گروہ, 1, true);
			واپس 0;
		}
	}

	kthread_stop(task);
	واپس 0;
}

ساخت psi_trigger *psi_trigger_create(ساخت psi_گروہ *گروہ, حرف *buf,
				       شمار psi_res res, ساخت file *file,
				       ساخت kernfs_open_file *کا,
				       bool *need_rtpoll_کارکن)
{
	ساخت psi_trigger *t;
	شمار psi_حالتیں state;
	u32 حد_us;
	bool privileged;
	u32 کھڑکی_us;

	*need_rtpoll_کارکن = false;

	اگر (static_branch_likely(&psi_disabled))
		واپس ERR_PTR(-EOPNOTSUPP);

	/*
	 * Checking the privilege here on file->f_cred implies that a privileged user
	 * could open the file and delegate the write to an unprivileged one.
	 */
	privileged = cap_raised(file->f_cred->cap_effective, CAP_SYS_RESOURCE);

	اگر (لڑی_سے_پڑھو(buf, "some %u %u", &حد_us, &کھڑکی_us) == 2)
		state = PSI_IO_SOME + res * 2;
	ورنہ اگر (لڑی_سے_پڑھو(buf, "full %u %u", &حد_us, &کھڑکی_us) == 2)
		state = PSI_IO_بھرا + res * 2;
	ورنہ
		واپس ERR_PTR(-EINVAL);

#اگر_تعریف_شدہ CONFIG_IRQ_وقت_ACCOUNTING
	اگر (res == PSI_IRQ && --state != PSI_IRQ_بھرا)
		واپس ERR_PTR(-EINVAL);
#اگر_ختم

	اگر (state >= PSI_NONIDLE)
		واپس ERR_PTR(-EINVAL);

	اگر (کھڑکی_us == 0 || کھڑکی_us > کھڑکی_MAX_US)
		واپس ERR_PTR(-EINVAL);

	/*
	 * Unprivileged users can only use 2s windows so that averages aggregation
	 * work is used, and no RT threads need to be spawned.
	 */
	اگر (!privileged && کھڑکی_us % 2000000)
		واپس ERR_PTR(-EINVAL);

	/* Check threshold */
	اگر (حد_us == 0 || حد_us > کھڑکی_us)
		واپس ERR_PTR(-EINVAL);

	t = kmalloc_obj(*t);
	اگر (!t)
		واپس ERR_PTR(-ENOMEM);

	t->گروہ = گروہ;
	t->state = state;
	t->حد = حد_us * NSEC_فی_USEC;
	t->win.سائز = کھڑکی_us * NSEC_فی_USEC;
	کھڑکی_reset(&t->win, sched_clock(),
			گروہ->کل[PSI_POLL][t->state], 0);

	t->event = 0;
	t->آخری_event_وقت = 0;
	t->کا = کا;
	اگر (!کا)
		init_waitqueue_سر(&t->event_wait);
	t->pending_event = false;
	t->aggregator = privileged ? PSI_POLL : PSI_AVGS;

	اگر (privileged) {
		mutex_تالا(&گروہ->rtpoll_trigger_تالا);

		list_جوڑو(&t->گرہ, &گروہ->rtpoll_triggers);
		گروہ->rtpoll_min_period = min(گروہ->rtpoll_min_period,
			div_u64(t->win.سائز, UPDATES_فی_کھڑکی));
		گروہ->rtpoll_nr_triggers[t->state]++;
		گروہ->rtpoll_حالتیں |= (1 << t->state);

		*need_rtpoll_کارکن = !rcu_access_اشارہ_گر(گروہ->rtpoll_task);

		mutex_unlock(&گروہ->rtpoll_trigger_تالا);
	} ورنہ {
		mutex_تالا(&گروہ->avgs_تالا);

		list_جوڑو(&t->گرہ, &گروہ->avg_triggers);
		گروہ->avg_nr_triggers[t->state]++;

		mutex_unlock(&گروہ->avgs_تالا);
	}
	واپس t;
}

خالی psi_trigger_destroy(ساخت psi_trigger *t)
{
	ساخت psi_گروہ *گروہ;
	ساخت task_struct *task_مقصد_destroy = تہی;

	/*
	 * We do not check psi_disabled since it might have been disabled after
	 * the trigger got created.
	 */
	اگر (!t)
		واپس;

	گروہ = t->گروہ;
	/*
	 * Wakeup waiters to stop polling and clear the queue to prevent it from
	 * being accessed later. Can happen if cgroup is deleted from under a
	 * polling process.
	 */
	اگر (t->کا)
		kernfs_notify(t->کا->kn);
	ورنہ
		wake_اوپر_interruptible(&t->event_wait);

	اگر (t->aggregator == PSI_AVGS) {
		mutex_تالا(&گروہ->avgs_تالا);
		اگر (!list_خالی_خانے(&t->گرہ)) {
			list_del(&t->گرہ);
			گروہ->avg_nr_triggers[t->state]--;
		}
		mutex_unlock(&گروہ->avgs_تالا);
	} ورنہ {
		mutex_تالا(&گروہ->rtpoll_trigger_تالا);
		اگر (!list_خالی_خانے(&t->گرہ)) {
			ساخت psi_trigger *tmp;
			u64 period = ULLONG_MAX;

			list_del(&t->گرہ);
			گروہ->rtpoll_nr_triggers[t->state]--;
			اگر (!گروہ->rtpoll_nr_triggers[t->state])
				گروہ->rtpoll_حالتیں &= ~(1 << t->state);
			/*
			 * Reset min update period for the remaining triggers
			 * iff the destroying trigger had the min window size.
			 */
			اگر (گروہ->rtpoll_min_period == div_u64(t->win.سائز, UPDATES_فی_کھڑکی)) {
				list_for_each_اندراج(tmp, &گروہ->rtpoll_triggers, گرہ)
					period = min(period, div_u64(tmp->win.سائز,
							UPDATES_فی_کھڑکی));
				گروہ->rtpoll_min_period = period;
			}
			/* Destroy rtpoll_task when the last trigger is destroyed */
			اگر (گروہ->rtpoll_حالتیں == 0) {
				گروہ->rtpoll_until = 0;
				task_مقصد_destroy = rcu_dereference_protected(
						گروہ->rtpoll_task,
						lockdep_is_held(&گروہ->rtpoll_trigger_تالا));
				rcu_تفویض_اشارہ_گر(گروہ->rtpoll_task, تہی);
				timer_delete(&گروہ->rtpoll_timer);
			}
		}
		mutex_unlock(&گروہ->rtpoll_trigger_تالا);
	}

	/*
	 * Stop kthread 'psimon' after releasing rtpoll_trigger_lock to prevent
	 * a deadlock while waiting for psi_rtpoll_work to acquire
	 * rtpoll_trigger_lock
	 */
	اگر (task_مقصد_destroy) {
		kthread_stop(task_مقصد_destroy);
		atomic_سیٹ(&گروہ->rtpoll_scheduled, 0);
	}
	kfree(t);
}

__poll_t psi_trigger_poll(خالی **trigger_ptr,
				ساخت file *file, poll_جدول *wait)
{
	__poll_t ret = DEFAULT_POLLMASK;
	ساخت psi_trigger *t;

	اگر (static_branch_likely(&psi_disabled))
		واپس DEFAULT_POLLMASK | EPOLLERR | EPOLLPRI;

	t = smp_بھرو_acquire(trigger_ptr);
	اگر (!t)
		واپس DEFAULT_POLLMASK | EPOLLERR | EPOLLPRI;

	اگر (t->کا)
		kernfs_generic_poll(t->کا, wait);
	ورنہ
		poll_wait(file, &t->event_wait, wait);

	اگر (cmpxchg(&t->event, 1, 0) == 1)
		ret |= EPOLLPRI;

	واپس ret;
}

#اگر_تعریف_شدہ CONFIG_PROC_FS
ساکن صحیح psi_io_show(ساخت seq_file *m, خالی *v)
{
	واپس psi_show(m, &psi_system, PSI_IO);
}

ساکن صحیح psi_حافظہ_show(ساخت seq_file *m, خالی *v)
{
	واپس psi_show(m, &psi_system, PSI_MEM);
}

ساکن صحیح psi_cpu_show(ساخت seq_file *m, خالی *v)
{
	واپس psi_show(m, &psi_system, PSI_CPU);
}

ساکن صحیح psi_io_open(ساخت inode *inode, ساخت file *file)
{
	واپس single_open(file, psi_io_show, تہی);
}

ساکن صحیح psi_حافظہ_open(ساخت inode *inode, ساخت file *file)
{
	واپس single_open(file, psi_حافظہ_show, تہی);
}

ساکن صحیح psi_cpu_open(ساخت inode *inode, ساخت file *file)
{
	واپس single_open(file, psi_cpu_show, تہی);
}

ساکن ssize_t psi_write(ساخت file *file, ثابت حرف __user *user_buf,
			 سائز_t nbytes, شمار psi_res res)
{
	حرف buf[32];
	سائز_t buf_سائز;
	ساخت seq_file *seq;
	ساخت psi_trigger *new;
	bool need_rtpoll_کارکن;
	صحیح ret;

	اگر (static_branch_likely(&psi_disabled))
		واپس -EOPNOTSUPP;

	اگر (!nbytes)
		واپس -EINVAL;

	buf_سائز = min(nbytes, حجم(buf));
	اگر (copy_from_user(buf, user_buf, buf_سائز))
		واپس -EFAULT;

	buf[buf_سائز - 1] = '\0';

	seq = file->نجی_ڈیٹا;

	/* Take seq->lock to protect seq->private from concurrent writes */
	mutex_تالا(&seq->تالا);

	/* Allow only one trigger per file descriptor */
	اگر (seq->نجی) {
		mutex_unlock(&seq->تالا);
		واپس -EBUSY;
	}

	new = psi_trigger_create(&psi_system, buf, res, file, تہی,
				 &need_rtpoll_کارکن);
	اگر (IS_ERR(new)) {
		mutex_unlock(&seq->تالا);
		واپس PTR_ERR(new);
	}

	اگر (need_rtpoll_کارکن) {
		ret = psi_trigger_create_rtpoll_کارکن(&psi_system);
		اگر (ret) {
			psi_trigger_destroy(new);
			mutex_unlock(&seq->تالا);
			واپس ret;
		}
	}

	smp_store_release(&seq->نجی, new);
	mutex_unlock(&seq->تالا);

	واپس nbytes;
}

ساکن ssize_t psi_io_write(ساخت file *file, ثابت حرف __user *user_buf,
			    سائز_t nbytes, loff_t *ppos)
{
	واپس psi_write(file, user_buf, nbytes, PSI_IO);
}

ساکن ssize_t psi_حافظہ_write(ساخت file *file, ثابت حرف __user *user_buf,
				سائز_t nbytes, loff_t *ppos)
{
	واپس psi_write(file, user_buf, nbytes, PSI_MEM);
}

ساکن ssize_t psi_cpu_write(ساخت file *file, ثابت حرف __user *user_buf,
			     سائز_t nbytes, loff_t *ppos)
{
	واپس psi_write(file, user_buf, nbytes, PSI_CPU);
}

ساکن __poll_t psi_fop_poll(ساخت file *file, poll_جدول *wait)
{
	ساخت seq_file *seq = file->نجی_ڈیٹا;

	واپس psi_trigger_poll(&seq->نجی, file, wait);
}

ساکن صحیح psi_fop_release(ساخت inode *inode, ساخت file *file)
{
	ساخت seq_file *seq = file->نجی_ڈیٹا;

	psi_trigger_destroy(seq->نجی);
	واپس single_release(inode, file);
}

ساکن ثابت ساخت proc_ops psi_io_proc_ops = {
	.proc_open	= psi_io_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_io_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

ساکن ثابت ساخت proc_ops psi_حافظہ_proc_ops = {
	.proc_open	= psi_حافظہ_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_حافظہ_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

ساکن ثابت ساخت proc_ops psi_cpu_proc_ops = {
	.proc_open	= psi_cpu_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_cpu_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

#اگر_تعریف_شدہ CONFIG_IRQ_وقت_ACCOUNTING
ساکن صحیح psi_irq_show(ساخت seq_file *m, خالی *v)
{
	واپس psi_show(m, &psi_system, PSI_IRQ);
}

ساکن صحیح psi_irq_open(ساخت inode *inode, ساخت file *file)
{
	واپس single_open(file, psi_irq_show, تہی);
}

ساکن ssize_t psi_irq_write(ساخت file *file, ثابت حرف __user *user_buf,
			     سائز_t nbytes, loff_t *ppos)
{
	واپس psi_write(file, user_buf, nbytes, PSI_IRQ);
}

ساکن ثابت ساخت proc_ops psi_irq_proc_ops = {
	.proc_open	= psi_irq_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_irq_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};
#اگر_ختم /* CONFIG_IRQ_TIME_ACCOUNTING */

ساکن صحیح __init psi_proc_init(خالی)
{
	اگر (psi_enable) {
		proc_mkdir("pressure", تہی);
		proc_create("pressure/io", 0666, تہی, &psi_io_proc_ops);
		proc_create("pressure/memory", 0666, تہی, &psi_حافظہ_proc_ops);
		proc_create("pressure/cpu", 0666, تہی, &psi_cpu_proc_ops);
#اگر_تعریف_شدہ CONFIG_IRQ_وقت_ACCOUNTING
		proc_create("pressure/irq", 0666, تہی, &psi_irq_proc_ops);
#اگر_ختم
	}
	واپس 0;
}
module_init(psi_proc_init);

#اگر_ختم /* CONFIG_PROC_FS */
