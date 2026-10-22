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

DEFINE_STATIC_کلید_FALSE(psi_disabled);
ساكن DEFINE_STATIC_کلید_TRUE(psi_cgroups_enabled);

#اگر_تعریف_شده CONFIG_PSI_DEFAULT_DISABLED
ساكن bool psi_enable;
#وإلا
ساكن bool psi_enable = true;
#پایان_اگر
ساكن صحيح __init راهاندازی_psi(حرف *str)
{
	أرجع kstrtobool(str, &psi_enable) == 0;
}
__setup("psi=", راهاندازی_psi);

/* Running averages - we need to be higher-res than loadavg */
#تعریف_کن PSI_FREQ	(2*HZ+1)	/* 2 sec intervals */
#تعریف_کن EXP_10s		1677		/* 1/exp(2s/10s) as fixed-point */
#تعریف_کن EXP_60s		1981		/* 1/exp(2s/60s) */
#تعریف_کن EXP_300s	2034		/* 1/exp(2s/300s) */

/* PSI trigger definitions */
#تعریف_کن پنجره_MAX_US 10000000	/* Max window size is 10s */
#تعریف_کن UPDATES_در_هر_پنجره 10	/* 10 updates per window */

/* Sampling frequency in nanoseconds */
ساكن u64 psi_period __read_mostly;

/* System-level pressure and stall tracking */
ساكن DEFINE_در_هر_CPU(بنية psi_گروه_cpu, system_گروه_pcpu);
بنية psi_گروه psi_system = {
	.pcpu = &system_گروه_pcpu,
};

ساكن DEFINE_در_هر_CPU(seqcount_t, psi_seq) = SEQCNT_ZERO(psi_seq);

ساكن مضمن فراغ psi_write_begin(صحيح cpu)
{
	write_seqcount_begin(در_هر_cpu_ptr(&psi_seq, cpu));
}

ساكن مضمن فراغ psi_write_انتها(صحيح cpu)
{
	write_seqcount_انتها(در_هر_cpu_ptr(&psi_seq, cpu));
}

ساكن مضمن u32 psi_read_begin(صحيح cpu)
{
	أرجع read_seqcount_begin(در_هر_cpu_ptr(&psi_seq, cpu));
}

ساكن مضمن bool psi_read_retry(صحيح cpu, u32 seq)
{
	أرجع read_seqcount_retry(در_هر_cpu_ptr(&psi_seq, cpu), seq);
}

ساكن فراغ psi_avgs_work(بنية work_struct *work);

ساكن فراغ poll_timer_fn(بنية timer_list *t);

ساكن فراغ گروه_init(بنية psi_گروه *گروه)
{
	گروه->enabled = true;
	گروه->avg_آخرین_update = sched_clock();
	گروه->avg_بعدی_update = گروه->avg_آخرین_update + psi_period;
	mutex_init(&گروه->avgs_قفل);

	/* Init avg trigger-related members */
	INIT_LIST_سر(&گروه->avg_triggers);
	memset(گروه->avg_nr_triggers, 0, حجم(گروه->avg_nr_triggers));
	INIT_DELAYED_WORK(&گروه->avgs_work, psi_avgs_work);

	/* Init rtpoll trigger-related members */
	atomic_مجموعه(&گروه->rtpoll_scheduled, 0);
	mutex_init(&گروه->rtpoll_trigger_قفل);
	INIT_LIST_سر(&گروه->rtpoll_triggers);
	گروه->rtpoll_min_period = U32_MAX;
	گروه->rtpoll_بعدی_update = ULLONG_MAX;
	init_waitqueue_سر(&گروه->rtpoll_wait);
	timer_راهاندازی(&گروه->rtpoll_timer, poll_timer_fn, 0);
	rcu_تخصیص_بده_اشاره_گر(گروه->rtpoll_task, عدم);
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

	psi_period = jiffies_مقصد_nsecs(PSI_FREQ);
	گروه_init(&psi_system);
}

ساكن u32 test_حالتها(بیعلامت صحيح *tasks, u32 state_ماسک)
{
	ثابت bool oncpu = state_ماسک & PSI_ONCPU;

	إذا (tasks[NR_IOWAIT]) {
		state_ماسک |= بیت(PSI_IO_SOME);
		إذا (!tasks[NR_RUNNING])
			state_ماسک |= بیت(PSI_IO_پر);
	}

	إذا (tasks[NR_MEMSTALL]) {
		state_ماسک |= بیت(PSI_MEM_SOME);
		إذا (tasks[NR_RUNNING] == tasks[NR_MEMSTALL_RUNNING])
			state_ماسک |= بیت(PSI_MEM_پر);
	}

	إذا (tasks[NR_RUNNING] > oncpu)
		state_ماسک |= بیت(PSI_CPU_SOME);

	إذا (tasks[NR_RUNNING] && !oncpu)
		state_ماسک |= بیت(PSI_CPU_پر);

	إذا (tasks[NR_IOWAIT] || tasks[NR_MEMSTALL] || tasks[NR_RUNNING])
		state_ماسک |= بیت(PSI_NONIDLE);

	أرجع state_ماسک;
}

ساكن فراغ get_recent_times(بنية psi_گروه *گروه, صحيح cpu,
			     تعداد psi_aggregators aggregator, u32 *times,
			     u32 *pchanged_حالتها)
{
	بنية psi_گروه_cpu *groupc = در_هر_cpu_ptr(گروه->pcpu, cpu);
	صحيح current_cpu = raw_smp_processor_id();
	بیعلامت صحيح tasks[NR_PSI_TASK_COUNTS];
	u64 now, state_شروع;
	تعداد psi_حالتها s;
	بیعلامت صحيح seq;
	u32 state_ماسک;

	*pchanged_حالتها = 0;

	/* Snapshot a coherent view of the CPU state */
	افعل {
		seq = psi_read_begin(cpu);
		now = cpu_clock(cpu);
		کپی_حافظه(times, groupc->times, حجم(groupc->times));
		state_ماسک = groupc->state_ماسک;
		state_شروع = groupc->state_شروع;
		إذا (cpu == current_cpu)
			کپی_حافظه(tasks, groupc->tasks, حجم(groupc->tasks));
	} طالما (psi_read_retry(cpu, seq));

	/* Calculate state time deltas against the previous snapshot */
	لكل (s = 0; s < NR_PSI_حالتها; s++) {
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
		إذا (state_ماسک & (1 << s))
			times[s] += now - state_شروع;

		delta = times[s] - groupc->times_prev[aggregator][s];
		groupc->times_prev[aggregator][s] = times[s];

		times[s] = delta;
		إذا (delta)
			*pchanged_حالتها |= (1 << s);
	}

	/*
	 * When collect_percpu_times() from the avgs_work, we don't want to
	 * re-arm avgs_work when all CPUs are IDLE. But the current CPU running
	 * this avgs_work is never IDLE, cause avgs_work can't be shut off.
	 * So for the current CPU, we need to re-arm avgs_work only when
	 * (NR_RUNNING > 1 || NR_IOWAIT > 0 || NR_MEMSTALL > 0), for other CPUs
	 * we can just check PSI_NONIDLE delta.
	 */
	إذا (current_work() == &گروه->avgs_work.work) {
		bool reschedule;

		إذا (cpu == current_cpu)
			reschedule = tasks[NR_RUNNING] +
				     tasks[NR_IOWAIT] +
				     tasks[NR_MEMSTALL] > 1;
		وإلا
			reschedule = *pchanged_حالتها & (1 << PSI_NONIDLE);

		إذا (reschedule)
			*pchanged_حالتها |= PSI_STATE_RESCHEDULE;
	}
}

ساكن فراغ calc_avgs(بیعلامت دراز avg[3], صحيح missed_periods,
		      u64 زمان, u64 period)
{
	بیعلامت دراز pct;

	/* Fill in zeroes for periods of no activity */
	إذا (missed_periods) {
		avg[0] = calc_بارگیری_n(avg[0], EXP_10s, 0, missed_periods);
		avg[1] = calc_بارگیری_n(avg[1], EXP_60s, 0, missed_periods);
		avg[2] = calc_بارگیری_n(avg[2], EXP_300s, 0, missed_periods);
	}

	/* Sample the most recent active period */
	pct = div_u64(زمان * 100, period);
	pct *= FIXED_1;
	avg[0] = calc_بارگیری(avg[0], EXP_10s, pct);
	avg[1] = calc_بارگیری(avg[1], EXP_60s, pct);
	avg[2] = calc_بارگیری(avg[2], EXP_300s, pct);
}

ساكن فراغ collect_percpu_times(بنية psi_گروه *گروه,
				 تعداد psi_aggregators aggregator,
				 u32 *pchanged_حالتها)
{
	u64 deltas[NR_PSI_حالتها - 1] = { 0, };
	بیعلامت دراز nonidle_مجموع = 0;
	u32 تغییریافته_حالتها = 0;
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
		u32 times[NR_PSI_حالتها];
		u32 nonidle;
		u32 cpu_تغییریافته_حالتها;

		get_recent_times(گروه, cpu, aggregator, times,
				&cpu_تغییریافته_حالتها);
		تغییریافته_حالتها |= cpu_تغییریافته_حالتها;

		nonidle = nsecs_مقصد_jiffies(times[PSI_NONIDLE]);
		nonidle_مجموع += nonidle;

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
	لكل (s = 0; s < NR_PSI_حالتها - 1; s++)
		گروه->مجموع[aggregator][s] +=
				div_u64(deltas[s], max(nonidle_مجموع, 1UL));

	إذا (pchanged_حالتها)
		*pchanged_حالتها = تغییریافته_حالتها;
}

/* Trigger tracking window manipulations */
ساكن فراغ پنجره_reset(بنية psi_پنجره *win, u64 now, u64 مقدار,
			 u64 prev_growth)
{
	win->شروع_زمان = now;
	win->شروع_مقدار = مقدار;
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
ساكن u64 پنجره_update(بنية psi_پنجره *win, u64 now, u64 مقدار)
{
	u64 elapsed;
	u64 growth;

	elapsed = now - win->شروع_زمان;
	growth = مقدار - win->شروع_مقدار;
	/*
	 * After each tracking window passes win->start_value and
	 * win->start_time get reset and win->prev_growth stores
	 * the average per-window growth of the previous window.
	 * win->prev_growth is then used to interpolate additional
	 * growth from the previous window assuming it was linear.
	 */
	إذا (elapsed > win->اندازه)
		پنجره_reset(win, now, مقدار, growth);
	وإلا {
		u32 مانده;

		مانده = win->اندازه - elapsed;
		growth += div64_u64(win->prev_growth * مانده, win->اندازه);
	}

	أرجع growth;
}

ساكن فراغ update_triggers(بنية psi_گروه *گروه, u64 now,
						   تعداد psi_aggregators aggregator)
{
	بنية psi_trigger *t;
	u64 *مجموع = گروه->مجموع[aggregator];
	بنية list_سر *triggers;
	u64 *aggregator_مجموع;

	إذا (aggregator == PSI_AVGS) {
		triggers = &گروه->avg_triggers;
		aggregator_مجموع = گروه->avg_مجموع;
	} وإلا {
		triggers = &گروه->rtpoll_triggers;
		aggregator_مجموع = گروه->rtpoll_مجموع;
	}

	/*
	 * On subsequent updates, calculate growth deltas and let
	 * watchers know when their specified thresholds are exceeded.
	 */
	list_for_each_مدخل(t, triggers, گره) {
		u64 growth;
		bool new_stall;

		new_stall = aggregator_مجموع[t->state] != مجموع[t->state];

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
			growth = پنجره_update(&t->win, now, مجموع[t->state]);
			إذا (!t->pending_event) {
				إذا (growth < t->آستانه)
					استمر;

				t->pending_event = true;
			}
		}
		/* Limit event signaling to once per window */
		إذا (now < t->آخرین_event_زمان + t->win.اندازه)
			استمر;

		/* Generate an event */
		إذا (cmpxchg(&t->event, 0, 1) == 0) {
			إذا (t->از)
				kernfs_notify(t->از->kn);
			وإلا
				wake_به_بالا_interruptible(&t->event_wait);
		}
		t->آخرین_event_زمان = now;
		/* Reset threshold breach flag once event got generated */
		t->pending_event = false;
	}
}

ساكن u64 update_averages(بنية psi_گروه *گروه, u64 now)
{
	بیعلامت دراز missed_periods = 0;
	u64 expires, period;
	u64 avg_بعدی_update;
	صحيح s;

	/* avgX= */
	expires = گروه->avg_بعدی_update;
	إذا (now - expires >= psi_period)
		missed_periods = div_u64(now - expires, psi_period);

	/*
	 * The periodic clock tick can get delayed for various
	 * reasons, especially on loaded systems. To avoid clock
	 * drift, we schedule the clock in fixed psi_period intervals.
	 * But the deltas we sample out of the per-cpu buckets above
	 * are based on the actual time elapsing between clock ticks.
	 */
	avg_بعدی_update = expires + ((1 + missed_periods) * psi_period);
	period = now - (گروه->avg_آخرین_update + (missed_periods * psi_period));
	گروه->avg_آخرین_update = now;

	لكل (s = 0; s < NR_PSI_حالتها - 1; s++) {
		u32 sample;

		sample = گروه->مجموع[PSI_AVGS][s] - گروه->avg_مجموع[s];
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
		گروه->avg_مجموع[s] += sample;
		calc_avgs(گروه->avg[s], missed_periods, sample, period);
	}

	أرجع avg_بعدی_update;
}

ساكن فراغ psi_avgs_work(بنية work_struct *work)
{
	بنية delayed_work *dwork;
	بنية psi_گروه *گروه;
	u32 تغییریافته_حالتها;
	u64 now;

	dwork = مقصد_delayed_work(work);
	گروه = container_از(dwork, بنية psi_گروه, avgs_work);

	mutex_قفل(&گروه->avgs_قفل);

	now = sched_clock();

	collect_percpu_times(گروه, PSI_AVGS, &تغییریافته_حالتها);
	/*
	 * If there is task activity, periodically fold the per-cpu
	 * times and feed samples into the running averages. If things
	 * are idle and there is no data to process, stop the clock.
	 * Once restarted, we'll catch up the running averages in one
	 * go - see calc_avgs() and missed_periods.
	 */
	إذا (now >= گروه->avg_بعدی_update) {
		update_triggers(گروه, now, PSI_AVGS);
		گروه->avg_بعدی_update = update_averages(گروه, now);
	}

	إذا (تغییریافته_حالتها & PSI_STATE_RESCHEDULE) {
		زمانبندی_delayed_work(dwork, nsecs_مقصد_jiffies(
				گروه->avg_بعدی_update - now) + 1);
	}

	mutex_unlock(&گروه->avgs_قفل);
}

ساكن فراغ init_rtpoll_triggers(بنية psi_گروه *گروه, u64 now)
{
	بنية psi_trigger *t;

	list_for_each_مدخل(t, &گروه->rtpoll_triggers, گره)
		پنجره_reset(&t->win, now,
				گروه->مجموع[PSI_POLL][t->state], 0);
	کپی_حافظه(گروه->rtpoll_مجموع, گروه->مجموع[PSI_POLL],
		   حجم(گروه->rtpoll_مجموع));
	گروه->rtpoll_بعدی_update = now + گروه->rtpoll_min_period;
}

/* Schedule rtpolling if it's not already scheduled or forced. */
ساكن فراغ psi_زمانبندی_rtpoll_work(بنية psi_گروه *گروه, بیعلامت دراز delay,
				   bool force)
{
	/*
	 * atomic_xchg should be called even when !force to provide a
	 * full memory barrier (see the comment inside psi_rtpoll_work).
	 */
	إذا (atomic_xchg(&گروه->rtpoll_scheduled, 1) && !force)
		أرجع;

	/*
	 * Only test whether a worker is installed; do not dereference the task.
	 * A racing trigger destruction may leave the timer armed.
	 * psi_cgroup_free() shuts it down before freeing the group.
	 * The system PSI group is static.
	 */
	إذا (likely(rcu_access_اشاره_گر(گروه->rtpoll_task)))
		mod_timer(&گروه->rtpoll_timer, jiffies + delay);
	وإلا
		atomic_مجموعه(&گروه->rtpoll_scheduled, 0);
}

ساكن فراغ psi_rtpoll_work(بنية psi_گروه *گروه)
{
	bool force_reschedule = false;
	u32 تغییریافته_حالتها;
	u64 now;

	mutex_قفل(&گروه->rtpoll_trigger_قفل);

	now = sched_clock();

	إذا (now > گروه->rtpoll_until) {
		/*
		 * We are either about to start or might stop rtpolling if no
		 * state change was recorded. Resetting rtpoll_scheduled leaves
		 * a small window for psi_group_change to sneak in and schedule
		 * an immediate rtpoll_work before we get to rescheduling. One
		 * potential extra wakeup at the end of the rtpolling window
		 * should be negligible and rtpoll_next_update still keeps
		 * updates correctly on schedule.
		 */
		atomic_مجموعه(&گروه->rtpoll_scheduled, 0);
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


	collect_percpu_times(گروه, PSI_POLL, &تغییریافته_حالتها);

	إذا (تغییریافته_حالتها & گروه->rtpoll_حالتها) {
		/* Initialize trigger windows when entering rtpolling mode */
		إذا (now > گروه->rtpoll_until)
			init_rtpoll_triggers(گروه, now);

		/*
		 * Keep the monitor active for at least the duration of the
		 * minimum tracking window as long as monitor states are
		 * changing.
		 */
		گروه->rtpoll_until = now +
			گروه->rtpoll_min_period * UPDATES_در_هر_پنجره;
	}

	إذا (now > گروه->rtpoll_until) {
		گروه->rtpoll_بعدی_update = ULLONG_MAX;
		اذهب بیرون;
	}

	إذا (now >= گروه->rtpoll_بعدی_update) {
		إذا (تغییریافته_حالتها & گروه->rtpoll_حالتها) {
			update_triggers(گروه, now, PSI_POLL);
			کپی_حافظه(گروه->rtpoll_مجموع, گروه->مجموع[PSI_POLL],
				   حجم(گروه->rtpoll_مجموع));
		}
		گروه->rtpoll_بعدی_update = now + گروه->rtpoll_min_period;
	}

	psi_زمانبندی_rtpoll_work(گروه,
		nsecs_مقصد_jiffies(گروه->rtpoll_بعدی_update - now) + 1,
		force_reschedule);

بیرون:
	mutex_unlock(&گروه->rtpoll_trigger_قفل);
}

ساكن صحيح psi_rtpoll_کارگر(فراغ *داده)
{
	بنية psi_گروه *گروه = (بنية psi_گروه *)داده;

	sched_مجموعه_fifo_کم(current);

	طالما (true) {
		wait_event_interruptible(گروه->rtpoll_wait,
				atomic_cmpxchg(&گروه->rtpoll_wakeup, 1, 0) ||
				kthread_should_stop());
		إذا (kthread_should_stop())
			اكسر;

		psi_rtpoll_work(گروه);
	}
	أرجع 0;
}

ساكن فراغ poll_timer_fn(بنية timer_list *t)
{
	بنية psi_گروه *گروه = timer_container_از(گروه, t, rtpoll_timer);

	atomic_مجموعه(&گروه->rtpoll_wakeup, 1);
	wake_به_بالا_interruptible(&گروه->rtpoll_wait);
}

ساكن فراغ رکورد_times(بنية psi_گروه_cpu *groupc, u64 now)
{
	u32 delta;

	delta = now - groupc->state_شروع;
	groupc->state_شروع = now;

	إذا (groupc->state_ماسک & (1 << PSI_IO_SOME)) {
		groupc->times[PSI_IO_SOME] += delta;
		إذا (groupc->state_ماسک & (1 << PSI_IO_پر))
			groupc->times[PSI_IO_پر] += delta;
	}

	إذا (groupc->state_ماسک & (1 << PSI_MEM_SOME)) {
		groupc->times[PSI_MEM_SOME] += delta;
		إذا (groupc->state_ماسک & (1 << PSI_MEM_پر))
			groupc->times[PSI_MEM_پر] += delta;
	}

	إذا (groupc->state_ماسک & (1 << PSI_CPU_SOME)) {
		groupc->times[PSI_CPU_SOME] += delta;
		إذا (groupc->state_ماسک & (1 << PSI_CPU_پر))
			groupc->times[PSI_CPU_پر] += delta;
	}

	إذا (groupc->state_ماسک & (1 << PSI_NONIDLE))
		groupc->times[PSI_NONIDLE] += delta;
}

#تعریف_کن for_each_گروه(iter, گروه) \
	لكل (typeof(گروه) iter = گروه; iter; iter = iter->والد)

ساكن فراغ psi_گروه_change(بنية psi_گروه *گروه, صحيح cpu,
			     بیعلامت صحيح clear, بیعلامت صحيح مجموعه,
			     u64 now, bool wake_clock)
{
	بنية psi_گروه_cpu *groupc;
	بیعلامت صحيح t, m;
	u32 state_ماسک;

	lockdep_assert_rq_held(cpu_rq(cpu));
	groupc = در_هر_cpu_ptr(گروه->pcpu, cpu);

	/*
	 * Start with TSK_ONCPU, which doesn't have a corresponding
	 * task count - it's just a boolean flag directly encoded in
	 * the state mask. Clear, set, or carry the current state if
	 * no changes are requested.
	 */
	إذا (unlikely(clear & TSK_ONCPU)) {
		state_ماسک = 0;
		clear &= ~TSK_ONCPU;
	} وإلا إذا (unlikely(مجموعه & TSK_ONCPU)) {
		state_ماسک = PSI_ONCPU;
		مجموعه &= ~TSK_ONCPU;
	} وإلا {
		state_ماسک = groupc->state_ماسک & PSI_ONCPU;
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
					groupc->tasks[3], clear, مجموعه);
			psi_bug = 1;
		}
	}

	لكل (t = 0; مجموعه; مجموعه &= ~(1 << t), t++)
		إذا (مجموعه & (1 << t))
			groupc->tasks[t]++;

	إذا (!گروه->enabled) {
		/*
		 * On the first group change after disabling PSI, conclude
		 * the current state and flush its time. This is unlikely
		 * to matter to the user, but aggregation (get_recent_times)
		 * may have already incorporated the live state into times_prev;
		 * avoid a delta sample underflow when PSI is later re-enabled.
		 */
		إذا (unlikely(groupc->state_ماسک & (1 << PSI_NONIDLE)))
			رکورد_times(groupc, now);

		groupc->state_ماسک = state_ماسک;

		أرجع;
	}

	state_ماسک = test_حالتها(groupc->tasks, state_ماسک);

	/*
	 * Since we care about lost potential, a memstall is FULL
	 * when there are no other working tasks, but also when
	 * the CPU is actively reclaiming and nothing productive
	 * could run even if it were runnable. So when the current
	 * task in a cgroup is in_memstall, the corresponding groupc
	 * on that cpu is in PSI_MEM_FULL state.
	 */
	إذا (unlikely((state_ماسک & PSI_ONCPU) && cpu_curr(cpu)->درون_memstall))
		state_ماسک |= (1 << PSI_MEM_پر);

	رکورد_times(groupc, now);

	groupc->state_ماسک = state_ماسک;

	إذا (state_ماسک & گروه->rtpoll_حالتها)
		psi_زمانبندی_rtpoll_work(گروه, 1, false);

	إذا (wake_clock && !delayed_work_pending(&گروه->avgs_work))
		زمانبندی_delayed_work(&گروه->avgs_work, PSI_FREQ);
}

ساكن مضمن بنية psi_گروه *task_psi_گروه(بنية task_struct *task)
{
#اگر_تعریف_شده CONFIG_CGROUPS
	إذا (static_branch_likely(&psi_cgroups_enabled))
		أرجع cgroup_psi(task_dfl_cgroup(task));
#پایان_اگر
	أرجع &psi_system;
}

ساكن فراغ psi_flags_change(بنية task_struct *task, صحيح clear, صحيح مجموعه)
{
	إذا (((task->psi_flags & مجموعه) ||
	     (task->psi_flags & clear) != clear) &&
	    !psi_bug) {
		printk_deferred(KERN_ERR "psi: inconsistent task state! task=%d:%s cpu=%d psi_flags=%x clear=%x set=%x\n",
				task->pid, task->comm, task_cpu(task),
				task->psi_flags, clear, مجموعه);
		psi_bug = 1;
	}

	task->psi_flags &= ~clear;
	task->psi_flags |= مجموعه;
}

فراغ psi_task_change(بنية task_struct *task, صحيح clear, صحيح مجموعه)
{
	صحيح cpu = task_cpu(task);
	u64 now;

	إذا (!task->pid)
		أرجع;

	psi_flags_change(task, clear, مجموعه);

	psi_write_begin(cpu);
	now = cpu_clock(cpu);
	for_each_گروه(گروه, task_psi_گروه(task))
		psi_گروه_change(گروه, cpu, clear, مجموعه, now, true);
	psi_write_انتها(cpu);
}

فراغ psi_task_switch(بنية task_struct *prev, بنية task_struct *بعدی,
		     bool sleep)
{
	بنية psi_گروه *common = عدم;
	صحيح cpu = task_cpu(prev);
	u64 now;

	psi_write_begin(cpu);
	now = cpu_clock(cpu);

	إذا (بعدی->pid) {
		psi_flags_change(بعدی, 0, TSK_ONCPU);
		/*
		 * Set TSK_ONCPU on @next's cgroups. If @next shares any
		 * ancestors with @prev, those will already have @prev's
		 * TSK_ONCPU bit set, and we can stop the iteration there.
		 */
		for_each_گروه(گروه, task_psi_گروه(بعدی)) {
			بنية psi_گروه_cpu *groupc = در_هر_cpu_ptr(گروه->pcpu, cpu);

			إذا (groupc->state_ماسک & PSI_ONCPU) {
				common = گروه;
				اكسر;
			}
			psi_گروه_change(گروه, cpu, 0, TSK_ONCPU, now, true);
		}
	}

	إذا (prev->pid) {
		صحيح clear = TSK_ONCPU, مجموعه = 0;
		bool wake_clock = true;

		/*
		 * When we're going to sleep, psi_dequeue() lets us
		 * handle TSK_RUNNING, TSK_MEMSTALL_RUNNING and
		 * TSK_IOWAIT here, where we can combine it with
		 * TSK_ONCPU and save walking common ancestors twice.
		 */
		إذا (sleep) {
			clear |= TSK_RUNNING;
			إذا (prev->درون_memstall)
				clear |= TSK_MEMSTALL_RUNNING;
			إذا (prev->درون_iowait)
				مجموعه |= TSK_IOWAIT;

			/*
			 * Periodic aggregation shuts off if there is a period of no
			 * task changes, so we wake it back up if necessary. However,
			 * don't do this if the task change is the aggregation worker
			 * itself going to sleep, or we'll ping-pong forever.
			 */
			إذا (unlikely((prev->flags & PF_WQ_کارگر) &&
				     wq_کارگر_آخرین_func(prev) == psi_avgs_work))
				wake_clock = false;
		}

		psi_flags_change(prev, clear, مجموعه);

		for_each_گروه(گروه, task_psi_گروه(prev)) {
			إذا (گروه == common)
				اكسر;
			psi_گروه_change(گروه, cpu, clear, مجموعه, now, wake_clock);
		}

		/*
		 * TSK_ONCPU is handled up to the common ancestor. If there are
		 * any other differences between the two tasks (e.g. prev goes
		 * to sleep, or only one task is memstall), finish propagating
		 * those differences all the way up to the root.
		 */
		إذا ((prev->psi_flags ^ بعدی->psi_flags) & ~TSK_ONCPU) {
			clear &= ~TSK_ONCPU;
			for_each_گروه(گروه, common)
				psi_گروه_change(گروه, cpu, clear, مجموعه, now, wake_clock);
		}
	}
	psi_write_انتها(cpu);
}

#اگر_تعریف_شده CONFIG_IRQ_زمان_ACCOUNTING
فراغ psi_account_irqtime(بنية rq *rq, بنية task_struct *curr, بنية task_struct *prev)
{
	صحيح cpu = task_cpu(curr);
	بنية psi_گروه_cpu *groupc;
	s64 delta;
	u64 irq;
	u64 now;

	إذا (static_branch_likely(&psi_disabled) || !irqtime_enabled())
		أرجع;

	إذا (!curr->pid)
		أرجع;

	lockdep_assert_rq_held(rq);
	إذا (prev && task_psi_گروه(prev) == task_psi_گروه(curr))
		أرجع;

	irq = irq_زمان_read(cpu);
	delta = (s64)(irq - rq->psi_irq_زمان);
	إذا (delta <= 0)
		أرجع;
	rq->psi_irq_زمان = irq;

	psi_write_begin(cpu);
	now = cpu_clock(cpu);

	for_each_گروه(گروه, task_psi_گروه(curr)) {
		إذا (!گروه->enabled)
			استمر;

		groupc = در_هر_cpu_ptr(گروه->pcpu, cpu);

		رکورد_times(groupc, now);
		groupc->times[PSI_IRQ_پر] += delta;

		إذا (گروه->rtpoll_حالتها & (1 << PSI_IRQ_پر))
			psi_زمانبندی_rtpoll_work(گروه, 1, false);
	}
	psi_write_انتها(cpu);
}
#پایان_اگر /* CONFIG_IRQ_TIME_ACCOUNTING */

/**
 * psi_memstall_enter - mark the beginning of a memory stall section
 * @flags: flags to handle nested sections
 *
 * Marks the calling task as being stalled due to a lack of memory,
 * such as waiting for a refault or performing reclaim.
 */
فراغ psi_memstall_enter(بیعلامت دراز *flags)
{
	بنية rq_flags rf;
	بنية rq *rq;

	إذا (static_branch_likely(&psi_disabled))
		أرجع;

	*flags = current->درون_memstall;
	إذا (*flags)
		أرجع;
	/*
	 * in_memstall setting & accounting needs to be atomic wrt
	 * changes to the task's scheduling state, otherwise we can
	 * race with CPU migration.
	 */
	rq = this_rq_قفل_irq(&rf);

	current->درون_memstall = 1;
	psi_task_change(current, 0, TSK_MEMSTALL | TSK_MEMSTALL_RUNNING);

	rq_unlock_irq(rq, &rf);
}
EXPORT_نماد_GPL(psi_memstall_enter);

/**
 * psi_memstall_leave - mark the end of an memory stall section
 * @flags: flags to handle nested memdelay sections
 *
 * Marks the calling task as no longer stalled due to lack of memory.
 */
فراغ psi_memstall_leave(بیعلامت دراز *flags)
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
	rq = this_rq_قفل_irq(&rf);

	current->درون_memstall = 0;
	psi_task_change(current, TSK_MEMSTALL | TSK_MEMSTALL_RUNNING, 0);

	rq_unlock_irq(rq, &rf);
}
EXPORT_نماد_GPL(psi_memstall_leave);

#اگر_تعریف_شده CONFIG_CGROUPS
صحيح psi_cgroup_alloc(بنية cgroup *cgroup)
{
	إذا (!static_branch_likely(&psi_cgroups_enabled))
		أرجع 0;

	cgroup->psi = kzalloc_obj(بنية psi_گروه);
	إذا (!cgroup->psi)
		أرجع -ENOMEM;

	cgroup->psi->pcpu = alloc_percpu(بنية psi_گروه_cpu);
	إذا (!cgroup->psi->pcpu) {
		kfree(cgroup->psi);
		أرجع -ENOMEM;
	}
	گروه_init(cgroup->psi);
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
	WARN_ONCE(cgroup->psi->rtpoll_حالتها, "psi: trigger leak\n");
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
فراغ cgroup_move_task(بنية task_struct *task, بنية css_مجموعه *مقصد)
{
	بیعلامت صحيح task_flags;
	بنية rq_flags rf;
	بنية rq *rq;

	إذا (!static_branch_likely(&psi_cgroups_enabled)) {
		/*
		 * Lame to do this here, but the scheduler cannot be locked
		 * from the outside, so we move cgroups from inside sched/.
		 */
		rcu_تخصیص_بده_اشاره_گر(task->cgroups, مقصد);
		أرجع;
	}

	rq = task_rq_قفل_2(task, &rf);

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
	rcu_تخصیص_بده_اشاره_گر(task->cgroups, مقصد);

	إذا (task_flags)
		psi_task_change(task, 0, task_flags);

	task_rq_unlock(rq, task, &rf);
}

فراغ psi_cgroup_restart(بنية psi_گروه *گروه)
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
	إذا (!گروه->enabled)
		أرجع;

	for_each_possible_cpu(cpu) {
		u64 now;

		guard(rq_قفل_irq)(cpu_rq(cpu));

		psi_write_begin(cpu);
		now = cpu_clock(cpu);
		psi_گروه_change(گروه, cpu, 0, 0, now, true);
		psi_write_انتها(cpu);
	}
}
#پایان_اگر /* CONFIG_CGROUPS */

صحيح psi_show(بنية seq_file *m, بنية psi_گروه *گروه, تعداد psi_res res)
{
	bool only_پر = false;
	صحيح پر;
	u64 now;

	إذا (static_branch_likely(&psi_disabled))
		أرجع -EOPNOTSUPP;

#اگر_تعریف_شده CONFIG_IRQ_زمان_ACCOUNTING
	إذا (!irqtime_enabled() && res == PSI_IRQ)
		أرجع -EOPNOTSUPP;
#پایان_اگر

	/* Update averages before reporting them */
	mutex_قفل(&گروه->avgs_قفل);
	now = sched_clock();
	collect_percpu_times(گروه, PSI_AVGS, عدم);
	إذا (now >= گروه->avg_بعدی_update)
		گروه->avg_بعدی_update = update_averages(گروه, now);
	mutex_unlock(&گروه->avgs_قفل);

#اگر_تعریف_شده CONFIG_IRQ_زمان_ACCOUNTING
	only_پر = res == PSI_IRQ;
#پایان_اگر

	لكل (پر = 0; پر < 2 - only_پر; پر++) {
		بیعلامت دراز avg[3] = { 0, };
		u64 مجموع = 0;
		صحيح w;

		/* CPU FULL is undefined at the system level */
		إذا (!(گروه == &psi_system && res == PSI_CPU && پر)) {
			لكل (w = 0; w < 3; w++)
				avg[w] = گروه->avg[res * 2 + پر][w];
			مجموع = div_u64(گروه->مجموع[PSI_AVGS][res * 2 + پر],
					NSEC_در_هر_USEC);
		}

		seq_printf(m, "%s avg10=%lu.%02lu avg60=%lu.%02lu avg300=%lu.%02lu total=%llu\n",
			   پر || only_پر ? "full" : "some",
			   بارگیری_INT(avg[0]), بارگیری_FRAC(avg[0]),
			   بارگیری_INT(avg[1]), بارگیری_FRAC(avg[1]),
			   بارگیری_INT(avg[2]), بارگیری_FRAC(avg[2]),
			   مجموع);
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
صحيح psi_trigger_create_rtpoll_کارگر(بنية psi_گروه *گروه)
{
	بنية task_struct *task;

	task = kthread_create(psi_rtpoll_کارگر, گروه, "psimon");
	إذا (IS_ERR(task))
		أرجع PTR_ERR(task);

	scoped_guard(mutex, &گروه->rtpoll_trigger_قفل) {
		إذا (!rcu_access_اشاره_گر(گروه->rtpoll_task)) {
			atomic_مجموعه(&گروه->rtpoll_wakeup, 0);
			wake_به_بالا_process(task);
			rcu_تخصیص_بده_اشاره_گر(گروه->rtpoll_task, task);

			/*
			 * Poll once to catch up on scheduling attempts dropped
			 * while there was no rtpoll worker.
			 */
			psi_زمانبندی_rtpoll_work(گروه, 1, true);
			أرجع 0;
		}
	}

	kthread_stop(task);
	أرجع 0;
}

بنية psi_trigger *psi_trigger_create(بنية psi_گروه *گروه, حرف *buf,
				       تعداد psi_res res, بنية file *file,
				       بنية kernfs_open_file *از,
				       bool *need_rtpoll_کارگر)
{
	بنية psi_trigger *t;
	تعداد psi_حالتها state;
	u32 آستانه_us;
	bool privileged;
	u32 پنجره_us;

	*need_rtpoll_کارگر = false;

	إذا (static_branch_likely(&psi_disabled))
		أرجع ERR_PTR(-EOPNOTSUPP);

	/*
	 * Checking the privilege here on file->f_cred implies that a privileged user
	 * could open the file and delegate the write to an unprivileged one.
	 */
	privileged = cap_raised(file->f_cred->cap_effective, CAP_SYS_RESOURCE);

	إذا (از_رشته_بخوان(buf, "some %u %u", &آستانه_us, &پنجره_us) == 2)
		state = PSI_IO_SOME + res * 2;
	وإلا إذا (از_رشته_بخوان(buf, "full %u %u", &آستانه_us, &پنجره_us) == 2)
		state = PSI_IO_پر + res * 2;
	وإلا
		أرجع ERR_PTR(-EINVAL);

#اگر_تعریف_شده CONFIG_IRQ_زمان_ACCOUNTING
	إذا (res == PSI_IRQ && --state != PSI_IRQ_پر)
		أرجع ERR_PTR(-EINVAL);
#پایان_اگر

	إذا (state >= PSI_NONIDLE)
		أرجع ERR_PTR(-EINVAL);

	إذا (پنجره_us == 0 || پنجره_us > پنجره_MAX_US)
		أرجع ERR_PTR(-EINVAL);

	/*
	 * Unprivileged users can only use 2s windows so that averages aggregation
	 * work is used, and no RT threads need to be spawned.
	 */
	إذا (!privileged && پنجره_us % 2000000)
		أرجع ERR_PTR(-EINVAL);

	/* Check threshold */
	إذا (آستانه_us == 0 || آستانه_us > پنجره_us)
		أرجع ERR_PTR(-EINVAL);

	t = kmalloc_obj(*t);
	إذا (!t)
		أرجع ERR_PTR(-ENOMEM);

	t->گروه = گروه;
	t->state = state;
	t->آستانه = آستانه_us * NSEC_در_هر_USEC;
	t->win.اندازه = پنجره_us * NSEC_در_هر_USEC;
	پنجره_reset(&t->win, sched_clock(),
			گروه->مجموع[PSI_POLL][t->state], 0);

	t->event = 0;
	t->آخرین_event_زمان = 0;
	t->از = از;
	إذا (!از)
		init_waitqueue_سر(&t->event_wait);
	t->pending_event = false;
	t->aggregator = privileged ? PSI_POLL : PSI_AVGS;

	إذا (privileged) {
		mutex_قفل(&گروه->rtpoll_trigger_قفل);

		list_جمع_کن(&t->گره, &گروه->rtpoll_triggers);
		گروه->rtpoll_min_period = min(گروه->rtpoll_min_period,
			div_u64(t->win.اندازه, UPDATES_در_هر_پنجره));
		گروه->rtpoll_nr_triggers[t->state]++;
		گروه->rtpoll_حالتها |= (1 << t->state);

		*need_rtpoll_کارگر = !rcu_access_اشاره_گر(گروه->rtpoll_task);

		mutex_unlock(&گروه->rtpoll_trigger_قفل);
	} وإلا {
		mutex_قفل(&گروه->avgs_قفل);

		list_جمع_کن(&t->گره, &گروه->avg_triggers);
		گروه->avg_nr_triggers[t->state]++;

		mutex_unlock(&گروه->avgs_قفل);
	}
	أرجع t;
}

فراغ psi_trigger_destroy(بنية psi_trigger *t)
{
	بنية psi_گروه *گروه;
	بنية task_struct *task_مقصد_destroy = عدم;

	/*
	 * We do not check psi_disabled since it might have been disabled after
	 * the trigger got created.
	 */
	إذا (!t)
		أرجع;

	گروه = t->گروه;
	/*
	 * Wakeup waiters to stop polling and clear the queue to prevent it from
	 * being accessed later. Can happen if cgroup is deleted from under a
	 * polling process.
	 */
	إذا (t->از)
		kernfs_notify(t->از->kn);
	وإلا
		wake_به_بالا_interruptible(&t->event_wait);

	إذا (t->aggregator == PSI_AVGS) {
		mutex_قفل(&گروه->avgs_قفل);
		إذا (!list_خالی(&t->گره)) {
			list_del(&t->گره);
			گروه->avg_nr_triggers[t->state]--;
		}
		mutex_unlock(&گروه->avgs_قفل);
	} وإلا {
		mutex_قفل(&گروه->rtpoll_trigger_قفل);
		إذا (!list_خالی(&t->گره)) {
			بنية psi_trigger *tmp;
			u64 period = ULLONG_MAX;

			list_del(&t->گره);
			گروه->rtpoll_nr_triggers[t->state]--;
			إذا (!گروه->rtpoll_nr_triggers[t->state])
				گروه->rtpoll_حالتها &= ~(1 << t->state);
			/*
			 * Reset min update period for the remaining triggers
			 * iff the destroying trigger had the min window size.
			 */
			إذا (گروه->rtpoll_min_period == div_u64(t->win.اندازه, UPDATES_در_هر_پنجره)) {
				list_for_each_مدخل(tmp, &گروه->rtpoll_triggers, گره)
					period = min(period, div_u64(tmp->win.اندازه,
							UPDATES_در_هر_پنجره));
				گروه->rtpoll_min_period = period;
			}
			/* Destroy rtpoll_task when the last trigger is destroyed */
			إذا (گروه->rtpoll_حالتها == 0) {
				گروه->rtpoll_until = 0;
				task_مقصد_destroy = rcu_dereference_protected(
						گروه->rtpoll_task,
						lockdep_is_held(&گروه->rtpoll_trigger_قفل));
				rcu_تخصیص_بده_اشاره_گر(گروه->rtpoll_task, عدم);
				timer_delete(&گروه->rtpoll_timer);
			}
		}
		mutex_unlock(&گروه->rtpoll_trigger_قفل);
	}

	/*
	 * Stop kthread 'psimon' after releasing rtpoll_trigger_lock to prevent
	 * a deadlock while waiting for psi_rtpoll_work to acquire
	 * rtpoll_trigger_lock
	 */
	إذا (task_مقصد_destroy) {
		kthread_stop(task_مقصد_destroy);
		atomic_مجموعه(&گروه->rtpoll_scheduled, 0);
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

	t = smp_بارگیری_acquire(trigger_ptr);
	إذا (!t)
		أرجع DEFAULT_POLLMASK | EPOLLERR | EPOLLPRI;

	إذا (t->از)
		kernfs_generic_poll(t->از, wait);
	وإلا
		poll_wait(file, &t->event_wait, wait);

	إذا (cmpxchg(&t->event, 1, 0) == 1)
		ret |= EPOLLPRI;

	أرجع ret;
}

#اگر_تعریف_شده CONFIG_PROC_FS
ساكن صحيح psi_io_show(بنية seq_file *m, فراغ *v)
{
	أرجع psi_show(m, &psi_system, PSI_IO);
}

ساكن صحيح psi_حافظه_show(بنية seq_file *m, فراغ *v)
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

ساكن صحيح psi_حافظه_open(بنية inode *inode, بنية file *file)
{
	أرجع single_open(file, psi_حافظه_show, عدم);
}

ساكن صحيح psi_cpu_open(بنية inode *inode, بنية file *file)
{
	أرجع single_open(file, psi_cpu_show, عدم);
}

ساكن ssize_t psi_write(بنية file *file, ثابت حرف __user *user_buf,
			 اندازه_t nbytes, تعداد psi_res res)
{
	حرف buf[32];
	اندازه_t buf_اندازه;
	بنية seq_file *seq;
	بنية psi_trigger *new;
	bool need_rtpoll_کارگر;
	صحيح ret;

	إذا (static_branch_likely(&psi_disabled))
		أرجع -EOPNOTSUPP;

	إذا (!nbytes)
		أرجع -EINVAL;

	buf_اندازه = min(nbytes, حجم(buf));
	إذا (copy_from_user(buf, user_buf, buf_اندازه))
		أرجع -EFAULT;

	buf[buf_اندازه - 1] = '\0';

	seq = file->خصوصی_داده;

	/* Take seq->lock to protect seq->private from concurrent writes */
	mutex_قفل(&seq->قفل);

	/* Allow only one trigger per file descriptor */
	إذا (seq->خاص) {
		mutex_unlock(&seq->قفل);
		أرجع -EBUSY;
	}

	new = psi_trigger_create(&psi_system, buf, res, file, عدم,
				 &need_rtpoll_کارگر);
	إذا (IS_ERR(new)) {
		mutex_unlock(&seq->قفل);
		أرجع PTR_ERR(new);
	}

	إذا (need_rtpoll_کارگر) {
		ret = psi_trigger_create_rtpoll_کارگر(&psi_system);
		إذا (ret) {
			psi_trigger_destroy(new);
			mutex_unlock(&seq->قفل);
			أرجع ret;
		}
	}

	smp_store_release(&seq->خاص, new);
	mutex_unlock(&seq->قفل);

	أرجع nbytes;
}

ساكن ssize_t psi_io_write(بنية file *file, ثابت حرف __user *user_buf,
			    اندازه_t nbytes, loff_t *ppos)
{
	أرجع psi_write(file, user_buf, nbytes, PSI_IO);
}

ساكن ssize_t psi_حافظه_write(بنية file *file, ثابت حرف __user *user_buf,
				اندازه_t nbytes, loff_t *ppos)
{
	أرجع psi_write(file, user_buf, nbytes, PSI_MEM);
}

ساكن ssize_t psi_cpu_write(بنية file *file, ثابت حرف __user *user_buf,
			     اندازه_t nbytes, loff_t *ppos)
{
	أرجع psi_write(file, user_buf, nbytes, PSI_CPU);
}

ساكن __poll_t psi_fop_poll(بنية file *file, poll_جدول *wait)
{
	بنية seq_file *seq = file->خصوصی_داده;

	أرجع psi_trigger_poll(&seq->خاص, file, wait);
}

ساكن صحيح psi_fop_release(بنية inode *inode, بنية file *file)
{
	بنية seq_file *seq = file->خصوصی_داده;

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

ساكن ثابت بنية proc_ops psi_حافظه_proc_ops = {
	.proc_open	= psi_حافظه_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_حافظه_write,
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

#اگر_تعریف_شده CONFIG_IRQ_زمان_ACCOUNTING
ساكن صحيح psi_irq_show(بنية seq_file *m, فراغ *v)
{
	أرجع psi_show(m, &psi_system, PSI_IRQ);
}

ساكن صحيح psi_irq_open(بنية inode *inode, بنية file *file)
{
	أرجع single_open(file, psi_irq_show, عدم);
}

ساكن ssize_t psi_irq_write(بنية file *file, ثابت حرف __user *user_buf,
			     اندازه_t nbytes, loff_t *ppos)
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
#پایان_اگر /* CONFIG_IRQ_TIME_ACCOUNTING */

ساكن صحيح __init psi_proc_init(فراغ)
{
	إذا (psi_enable) {
		proc_mkdir("pressure", عدم);
		proc_create("pressure/io", 0666, عدم, &psi_io_proc_ops);
		proc_create("pressure/memory", 0666, عدم, &psi_حافظه_proc_ops);
		proc_create("pressure/cpu", 0666, عدم, &psi_cpu_proc_ops);
#اگر_تعریف_شده CONFIG_IRQ_زمان_ACCOUNTING
		proc_create("pressure/irq", 0666, عدم, &psi_irq_proc_ops);
#پایان_اگر
	}
	أرجع 0;
}
module_init(psi_proc_init);

#پایان_اگر /* CONFIG_PROC_FS */
