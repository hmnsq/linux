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
#समावेश <linux/sched/clock.h>
#समावेश <linux/workqueue.h>
#समावेश <linux/psi.h>
#समावेश "sched.h"

अटल पूर्णांक psi_bug __read_mostly;

DEFINE_STATIC_कुंजी_FALSE(psi_disabled);
अटल DEFINE_STATIC_कुंजी_TRUE(psi_cgroups_enabled);

#अगर_घोषित CONFIG_PSI_DEFAULT_DISABLED
अटल bool psi_enable;
#अन्यथा
अटल bool psi_enable = true;
#पूर्ण_अगर
अटल पूर्णांक __init setup_psi(अक्षर *str)
{
	वापस kstrtobool(str, &psi_enable) == 0;
}
__setup("psi=", setup_psi);

/* Running averages - we need to be higher-res than loadavg */
#घोषणा PSI_FREQ	(2*HZ+1)	/* 2 sec intervals */
#घोषणा EXP_10s		1677		/* 1/exp(2s/10s) as fixed-point */
#घोषणा EXP_60s		1981		/* 1/exp(2s/60s) */
#घोषणा EXP_300s	2034		/* 1/exp(2s/300s) */

/* PSI trigger definitions */
#घोषणा WINDOW_MAX_US 10000000	/* Max window size is 10s */
#घोषणा UPDATES_PER_WINDOW 10	/* 10 updates per window */

/* Sampling frequency in nanoseconds */
अटल u64 psi_period __read_mostly;

/* System-level pressure and stall tracking */
अटल DEFINE_PER_CPU(काष्ठा psi_group_cpu, system_group_pcpu);
काष्ठा psi_group psi_system = {
	.pcpu = &system_group_pcpu,
};

अटल DEFINE_PER_CPU(seqcount_t, psi_seq) = SEQCNT_ZERO(psi_seq);

अटल अंतरभूत व्योम psi_write_begin(पूर्णांक cpu)
{
	write_seqcount_begin(per_cpu_ptr(&psi_seq, cpu));
}

अटल अंतरभूत व्योम psi_write_end(पूर्णांक cpu)
{
	write_seqcount_end(per_cpu_ptr(&psi_seq, cpu));
}

अटल अंतरभूत u32 psi_read_begin(पूर्णांक cpu)
{
	वापस read_seqcount_begin(per_cpu_ptr(&psi_seq, cpu));
}

अटल अंतरभूत bool psi_read_retry(पूर्णांक cpu, u32 seq)
{
	वापस read_seqcount_retry(per_cpu_ptr(&psi_seq, cpu), seq);
}

अटल व्योम psi_avgs_work(काष्ठा work_struct *work);

अटल व्योम poll_timer_fn(काष्ठा timer_list *t);

अटल व्योम group_init(काष्ठा psi_group *group)
{
	group->enabled = true;
	group->avg_last_update = sched_clock();
	group->avg_अग्र_update = group->avg_last_update + psi_period;
	mutex_init(&group->avgs_lock);

	/* Init avg trigger-related members */
	INIT_LIST_शीर्ष(&group->avg_triggers);
	स_रखो(group->avg_nr_triggers, 0, माप(group->avg_nr_triggers));
	INIT_DELAYED_WORK(&group->avgs_work, psi_avgs_work);

	/* Init rtpoll trigger-related members */
	atomic_set(&group->rtpoll_scheduled, 0);
	mutex_init(&group->rtpoll_trigger_lock);
	INIT_LIST_शीर्ष(&group->rtpoll_triggers);
	group->rtpoll_min_period = U32_MAX;
	group->rtpoll_अग्र_update = ULLONG_MAX;
	init_waitqueue_शीर्ष(&group->rtpoll_wait);
	timer_setup(&group->rtpoll_timer, poll_timer_fn, 0);
	rcu_assign_pointer(group->rtpoll_task, शून्य);
}

व्योम __init psi_init(व्योम)
{
	अगर (!psi_enable) {
		static_branch_enable(&psi_disabled);
		static_branch_disable(&psi_cgroups_enabled);
		वापस;
	}

	अगर (!cgroup_psi_enabled())
		static_branch_disable(&psi_cgroups_enabled);

	psi_period = jiffies_to_nsecs(PSI_FREQ);
	group_init(&psi_system);
}

अटल u32 test_states(अचिन्हित पूर्णांक *tasks, u32 state_mask)
{
	स्थिर bool oncpu = state_mask & PSI_ONCPU;

	अगर (tasks[NR_IOWAIT]) {
		state_mask |= BIT(PSI_IO_SOME);
		अगर (!tasks[NR_RUNNING])
			state_mask |= BIT(PSI_IO_FULL);
	}

	अगर (tasks[NR_MEMSTALL]) {
		state_mask |= BIT(PSI_MEM_SOME);
		अगर (tasks[NR_RUNNING] == tasks[NR_MEMSTALL_RUNNING])
			state_mask |= BIT(PSI_MEM_FULL);
	}

	अगर (tasks[NR_RUNNING] > oncpu)
		state_mask |= BIT(PSI_CPU_SOME);

	अगर (tasks[NR_RUNNING] && !oncpu)
		state_mask |= BIT(PSI_CPU_FULL);

	अगर (tasks[NR_IOWAIT] || tasks[NR_MEMSTALL] || tasks[NR_RUNNING])
		state_mask |= BIT(PSI_NONIDLE);

	वापस state_mask;
}

अटल व्योम get_recent_times(काष्ठा psi_group *group, पूर्णांक cpu,
			     क्रमागत psi_aggregators aggregator, u32 *बार,
			     u32 *pchanged_states)
{
	काष्ठा psi_group_cpu *groupc = per_cpu_ptr(group->pcpu, cpu);
	पूर्णांक current_cpu = raw_smp_processor_id();
	अचिन्हित पूर्णांक tasks[NR_PSI_TASK_COUNTS];
	u64 now, state_start;
	क्रमागत psi_states s;
	अचिन्हित पूर्णांक seq;
	u32 state_mask;

	*pchanged_states = 0;

	/* Snapshot a coherent view of the CPU state */
	करो {
		seq = psi_read_begin(cpu);
		now = cpu_clock(cpu);
		स_नकल(बार, groupc->बार, माप(groupc->बार));
		state_mask = groupc->state_mask;
		state_start = groupc->state_start;
		अगर (cpu == current_cpu)
			स_नकल(tasks, groupc->tasks, माप(groupc->tasks));
	} जबतक (psi_read_retry(cpu, seq));

	/* Calculate state time deltas against the previous snapshot */
	क्रम (s = 0; s < NR_PSI_STATES; s++) {
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
		अगर (state_mask & (1 << s))
			बार[s] += now - state_start;

		delta = बार[s] - groupc->times_पूर्व[aggregator][s];
		groupc->times_पूर्व[aggregator][s] = बार[s];

		बार[s] = delta;
		अगर (delta)
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
	अगर (current_work() == &group->avgs_work.work) {
		bool reschedule;

		अगर (cpu == current_cpu)
			reschedule = tasks[NR_RUNNING] +
				     tasks[NR_IOWAIT] +
				     tasks[NR_MEMSTALL] > 1;
		अन्यथा
			reschedule = *pchanged_states & (1 << PSI_NONIDLE);

		अगर (reschedule)
			*pchanged_states |= PSI_STATE_RESCHEDULE;
	}
}

अटल व्योम calc_avgs(अचिन्हित दीर्घ avg[3], पूर्णांक missed_periods,
		      u64 समय, u64 period)
{
	अचिन्हित दीर्घ pct;

	/* Fill in zeroes for periods of no activity */
	अगर (missed_periods) {
		avg[0] = calc_load_n(avg[0], EXP_10s, 0, missed_periods);
		avg[1] = calc_load_n(avg[1], EXP_60s, 0, missed_periods);
		avg[2] = calc_load_n(avg[2], EXP_300s, 0, missed_periods);
	}

	/* Sample the most recent active period */
	pct = div_u64(समय * 100, period);
	pct *= FIXED_1;
	avg[0] = calc_load(avg[0], EXP_10s, pct);
	avg[1] = calc_load(avg[1], EXP_60s, pct);
	avg[2] = calc_load(avg[2], EXP_300s, pct);
}

अटल व्योम collect_percpu_times(काष्ठा psi_group *group,
				 क्रमागत psi_aggregators aggregator,
				 u32 *pchanged_states)
{
	u64 deltas[NR_PSI_STATES - 1] = { 0, };
	अचिन्हित दीर्घ nonidle_योग = 0;
	u32 changed_states = 0;
	पूर्णांक cpu;
	पूर्णांक s;

	/*
	 * Collect the per-cpu time buckets and average them into a
	 * single time sample that is normalized to wall clock time.
	 *
	 * For averaging, each CPU is weighted by its non-idle time in
	 * the sampling period. This eliminates artifacts from uneven
	 * loading, or even entirely idle CPUs.
	 */
	for_each_possible_cpu(cpu) {
		u32 बार[NR_PSI_STATES];
		u32 nonidle;
		u32 cpu_changed_states;

		get_recent_times(group, cpu, aggregator, बार,
				&cpu_changed_states);
		changed_states |= cpu_changed_states;

		nonidle = nsecs_to_jiffies(बार[PSI_NONIDLE]);
		nonidle_योग += nonidle;

		क्रम (s = 0; s < PSI_NONIDLE; s++)
			deltas[s] += (u64)बार[s] * nonidle;
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
	क्रम (s = 0; s < NR_PSI_STATES - 1; s++)
		group->योग_2[aggregator][s] +=
				div_u64(deltas[s], max(nonidle_योग, 1UL));

	अगर (pchanged_states)
		*pchanged_states = changed_states;
}

/* Trigger tracking window manipulations */
अटल व्योम window_reset(काष्ठा psi_window *win, u64 now, u64 मूल्य,
			 u64 पूर्व_growth)
{
	win->start_time = now;
	win->start_मूल्य = मूल्य;
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
अटल u64 window_update(काष्ठा psi_window *win, u64 now, u64 मूल्य)
{
	u64 elapsed;
	u64 growth;

	elapsed = now - win->start_time;
	growth = मूल्य - win->start_मूल्य;
	/*
	 * After each tracking window passes win->start_value and
	 * win->start_time get reset and win->prev_growth stores
	 * the average per-window growth of the previous window.
	 * win->prev_growth is then used to interpolate additional
	 * growth from the previous window assuming it was linear.
	 */
	अगर (elapsed > win->आकार)
		window_reset(win, now, मूल्य, growth);
	अन्यथा {
		u32 remaining;

		remaining = win->आकार - elapsed;
		growth += div64_u64(win->पूर्व_growth * remaining, win->आकार);
	}

	वापस growth;
}

अटल व्योम update_triggers(काष्ठा psi_group *group, u64 now,
						   क्रमागत psi_aggregators aggregator)
{
	काष्ठा psi_trigger *t;
	u64 *योग_2 = group->योग_2[aggregator];
	काष्ठा list_शीर्ष *triggers;
	u64 *aggregator_योग;

	अगर (aggregator == PSI_AVGS) {
		triggers = &group->avg_triggers;
		aggregator_योग = group->avg_योग;
	} अन्यथा {
		triggers = &group->rtpoll_triggers;
		aggregator_योग = group->rtpoll_योग;
	}

	/*
	 * On subsequent updates, calculate growth deltas and let
	 * watchers know when their specified thresholds are exceeded.
	 */
	list_for_each_entry(t, triggers, गाँठ) {
		u64 growth;
		bool new_stall;

		new_stall = aggregator_योग[t->state] != योग_2[t->state];

		/* Check for stall activity or a previous threshold breach */
		अगर (!new_stall && !t->pending_event)
			जारी;
		/*
		 * Check for new stall activity, as well as deferred
		 * events that occurred in the last window after the
		 * trigger had already fired (we want to ratelimit
		 * events without dropping any).
		 */
		अगर (new_stall) {
			/* Calculate growth since last update */
			growth = window_update(&t->win, now, योग_2[t->state]);
			अगर (!t->pending_event) {
				अगर (growth < t->threshold)
					जारी;

				t->pending_event = true;
			}
		}
		/* Limit event signaling to once per window */
		अगर (now < t->last_event_time + t->win.आकार)
			जारी;

		/* Generate an event */
		अगर (cmpxchg(&t->event, 0, 1) == 0) {
			अगर (t->of)
				kernfs_notify(t->of->kn);
			अन्यथा
				wake_up_interruptible(&t->event_wait);
		}
		t->last_event_time = now;
		/* Reset threshold breach flag once event got generated */
		t->pending_event = false;
	}
}

अटल u64 update_averages(काष्ठा psi_group *group, u64 now)
{
	अचिन्हित दीर्घ missed_periods = 0;
	u64 expires, period;
	u64 avg_अग्र_update;
	पूर्णांक s;

	/* avgX= */
	expires = group->avg_अग्र_update;
	अगर (now - expires >= psi_period)
		missed_periods = div_u64(now - expires, psi_period);

	/*
	 * The periodic clock tick can get delayed for various
	 * reasons, especially on loaded systems. To avoid clock
	 * drift, we schedule the clock in fixed psi_period intervals.
	 * But the deltas we sample out of the per-cpu buckets above
	 * are based on the actual time elapsing between clock ticks.
	 */
	avg_अग्र_update = expires + ((1 + missed_periods) * psi_period);
	period = now - (group->avg_last_update + (missed_periods * psi_period));
	group->avg_last_update = now;

	क्रम (s = 0; s < NR_PSI_STATES - 1; s++) {
		u32 sample;

		sample = group->योग_2[PSI_AVGS][s] - group->avg_योग[s];
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
		अगर (sample > period)
			sample = period;
		group->avg_योग[s] += sample;
		calc_avgs(group->avg[s], missed_periods, sample, period);
	}

	वापस avg_अग्र_update;
}

अटल व्योम psi_avgs_work(काष्ठा work_struct *work)
{
	काष्ठा delayed_work *dwork;
	काष्ठा psi_group *group;
	u32 changed_states;
	u64 now;

	dwork = to_delayed_work(work);
	group = container_of(dwork, काष्ठा psi_group, avgs_work);

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
	अगर (now >= group->avg_अग्र_update) {
		update_triggers(group, now, PSI_AVGS);
		group->avg_अग्र_update = update_averages(group, now);
	}

	अगर (changed_states & PSI_STATE_RESCHEDULE) {
		schedule_delayed_work(dwork, nsecs_to_jiffies(
				group->avg_अग्र_update - now) + 1);
	}

	mutex_unlock(&group->avgs_lock);
}

अटल व्योम init_rtpoll_triggers(काष्ठा psi_group *group, u64 now)
{
	काष्ठा psi_trigger *t;

	list_for_each_entry(t, &group->rtpoll_triggers, गाँठ)
		window_reset(&t->win, now,
				group->योग_2[PSI_POLL][t->state], 0);
	स_नकल(group->rtpoll_योग, group->योग_2[PSI_POLL],
		   माप(group->rtpoll_योग));
	group->rtpoll_अग्र_update = now + group->rtpoll_min_period;
}

/* Schedule rtpolling if it's not already scheduled or forced. */
अटल व्योम psi_schedule_rtpoll_work(काष्ठा psi_group *group, अचिन्हित दीर्घ delay,
				   bool force)
{
	/*
	 * atomic_xchg should be called even when !force to provide a
	 * full memory barrier (see the comment inside psi_rtpoll_work).
	 */
	अगर (atomic_xchg(&group->rtpoll_scheduled, 1) && !force)
		वापस;

	/*
	 * Only test whether a worker is installed; do not dereference the task.
	 * A racing trigger destruction may leave the timer armed.
	 * psi_cgroup_free() shuts it down before freeing the group.
	 * The system PSI group is static.
	 */
	अगर (likely(rcu_access_pointer(group->rtpoll_task)))
		mod_timer(&group->rtpoll_timer, jiffies + delay);
	अन्यथा
		atomic_set(&group->rtpoll_scheduled, 0);
}

अटल व्योम psi_rtpoll_work(काष्ठा psi_group *group)
{
	bool force_reschedule = false;
	u32 changed_states;
	u64 now;

	mutex_lock(&group->rtpoll_trigger_lock);

	now = sched_clock();

	अगर (now > group->rtpoll_until) {
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
	} अन्यथा {
		/* The rtpolling window is not over, keep rescheduling */
		force_reschedule = true;
	}


	collect_percpu_times(group, PSI_POLL, &changed_states);

	अगर (changed_states & group->rtpoll_states) {
		/* Initialize trigger windows when entering rtpolling mode */
		अगर (now > group->rtpoll_until)
			init_rtpoll_triggers(group, now);

		/*
		 * Keep the monitor active for at least the duration of the
		 * minimum tracking window as long as monitor states are
		 * changing.
		 */
		group->rtpoll_until = now +
			group->rtpoll_min_period * UPDATES_PER_WINDOW;
	}

	अगर (now > group->rtpoll_until) {
		group->rtpoll_अग्र_update = ULLONG_MAX;
		जाओ out;
	}

	अगर (now >= group->rtpoll_अग्र_update) {
		अगर (changed_states & group->rtpoll_states) {
			update_triggers(group, now, PSI_POLL);
			स_नकल(group->rtpoll_योग, group->योग_2[PSI_POLL],
				   माप(group->rtpoll_योग));
		}
		group->rtpoll_अग्र_update = now + group->rtpoll_min_period;
	}

	psi_schedule_rtpoll_work(group,
		nsecs_to_jiffies(group->rtpoll_अग्र_update - now) + 1,
		force_reschedule);

out:
	mutex_unlock(&group->rtpoll_trigger_lock);
}

अटल पूर्णांक psi_rtpoll_worker(व्योम *तथ्य)
{
	काष्ठा psi_group *group = (काष्ठा psi_group *)तथ्य;

	sched_set_fifo_निचला(current);

	जबतक (true) {
		wait_event_interruptible(group->rtpoll_wait,
				atomic_cmpxchg(&group->rtpoll_wakeup, 1, 0) ||
				kthread_should_stop());
		अगर (kthread_should_stop())
			अवरोध;

		psi_rtpoll_work(group);
	}
	वापस 0;
}

अटल व्योम poll_timer_fn(काष्ठा timer_list *t)
{
	काष्ठा psi_group *group = timer_container_of(group, t, rtpoll_timer);

	atomic_set(&group->rtpoll_wakeup, 1);
	wake_up_interruptible(&group->rtpoll_wait);
}

अटल व्योम record_times(काष्ठा psi_group_cpu *groupc, u64 now)
{
	u32 delta;

	delta = now - groupc->state_start;
	groupc->state_start = now;

	अगर (groupc->state_mask & (1 << PSI_IO_SOME)) {
		groupc->बार[PSI_IO_SOME] += delta;
		अगर (groupc->state_mask & (1 << PSI_IO_FULL))
			groupc->बार[PSI_IO_FULL] += delta;
	}

	अगर (groupc->state_mask & (1 << PSI_MEM_SOME)) {
		groupc->बार[PSI_MEM_SOME] += delta;
		अगर (groupc->state_mask & (1 << PSI_MEM_FULL))
			groupc->बार[PSI_MEM_FULL] += delta;
	}

	अगर (groupc->state_mask & (1 << PSI_CPU_SOME)) {
		groupc->बार[PSI_CPU_SOME] += delta;
		अगर (groupc->state_mask & (1 << PSI_CPU_FULL))
			groupc->बार[PSI_CPU_FULL] += delta;
	}

	अगर (groupc->state_mask & (1 << PSI_NONIDLE))
		groupc->बार[PSI_NONIDLE] += delta;
}

#घोषणा for_each_group(iter, group) \
	क्रम (typeof(group) iter = group; iter; iter = iter->parent)

अटल व्योम psi_group_change(काष्ठा psi_group *group, पूर्णांक cpu,
			     अचिन्हित पूर्णांक clear, अचिन्हित पूर्णांक set,
			     u64 now, bool wake_clock)
{
	काष्ठा psi_group_cpu *groupc;
	अचिन्हित पूर्णांक t, m;
	u32 state_mask;

	lockdep_assert_rq_held(cpu_rq(cpu));
	groupc = per_cpu_ptr(group->pcpu, cpu);

	/*
	 * Start with TSK_ONCPU, which doesn't have a corresponding
	 * task count - it's just a boolean flag directly encoded in
	 * the state mask. Clear, set, or carry the current state if
	 * no changes are requested.
	 */
	अगर (unlikely(clear & TSK_ONCPU)) {
		state_mask = 0;
		clear &= ~TSK_ONCPU;
	} अन्यथा अगर (unlikely(set & TSK_ONCPU)) {
		state_mask = PSI_ONCPU;
		set &= ~TSK_ONCPU;
	} अन्यथा {
		state_mask = groupc->state_mask & PSI_ONCPU;
	}

	/*
	 * The rest of the state mask is calculated based on the task
	 * counts. Update those first, then construct the mask.
	 */
	क्रम (t = 0, m = clear; m; m &= ~(1 << t), t++) {
		अगर (!(m & (1 << t)))
			जारी;
		अगर (groupc->tasks[t]) {
			groupc->tasks[t]--;
		} अन्यथा अगर (!psi_bug) {
			printk_deferred(KERN_ERR "psi: task underflow! cpu=%d t=%d tasks=[%u %u %u %u] clear=%x set=%x\n",
					cpu, t, groupc->tasks[0],
					groupc->tasks[1], groupc->tasks[2],
					groupc->tasks[3], clear, set);
			psi_bug = 1;
		}
	}

	क्रम (t = 0; set; set &= ~(1 << t), t++)
		अगर (set & (1 << t))
			groupc->tasks[t]++;

	अगर (!group->enabled) {
		/*
		 * On the first group change after disabling PSI, conclude
		 * the current state and flush its time. This is unlikely
		 * to matter to the user, but aggregation (get_recent_times)
		 * may have already incorporated the live state into times_prev;
		 * avoid a delta sample underflow when PSI is later re-enabled.
		 */
		अगर (unlikely(groupc->state_mask & (1 << PSI_NONIDLE)))
			record_times(groupc, now);

		groupc->state_mask = state_mask;

		वापस;
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
	अगर (unlikely((state_mask & PSI_ONCPU) && cpu_curr(cpu)->in_memstall))
		state_mask |= (1 << PSI_MEM_FULL);

	record_times(groupc, now);

	groupc->state_mask = state_mask;

	अगर (state_mask & group->rtpoll_states)
		psi_schedule_rtpoll_work(group, 1, false);

	अगर (wake_clock && !delayed_work_pending(&group->avgs_work))
		schedule_delayed_work(&group->avgs_work, PSI_FREQ);
}

अटल अंतरभूत काष्ठा psi_group *task_psi_group(काष्ठा task_struct *task)
{
#अगर_घोषित CONFIG_CGROUPS
	अगर (static_branch_likely(&psi_cgroups_enabled))
		वापस cgroup_psi(task_dfl_cgroup(task));
#पूर्ण_अगर
	वापस &psi_system;
}

अटल व्योम psi_flags_change(काष्ठा task_struct *task, पूर्णांक clear, पूर्णांक set)
{
	अगर (((task->psi_flags & set) ||
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

व्योम psi_task_change(काष्ठा task_struct *task, पूर्णांक clear, पूर्णांक set)
{
	पूर्णांक cpu = task_cpu(task);
	u64 now;

	अगर (!task->pid)
		वापस;

	psi_flags_change(task, clear, set);

	psi_write_begin(cpu);
	now = cpu_clock(cpu);
	for_each_group(group, task_psi_group(task))
		psi_group_change(group, cpu, clear, set, now, true);
	psi_write_end(cpu);
}

व्योम psi_task_switch(काष्ठा task_struct *पूर्व, काष्ठा task_struct *अग्र,
		     bool sleep)
{
	काष्ठा psi_group *common = शून्य;
	पूर्णांक cpu = task_cpu(पूर्व);
	u64 now;

	psi_write_begin(cpu);
	now = cpu_clock(cpu);

	अगर (अग्र->pid) {
		psi_flags_change(अग्र, 0, TSK_ONCPU);
		/*
		 * Set TSK_ONCPU on @next's cgroups. If @next shares any
		 * ancestors with @prev, those will already have @prev's
		 * TSK_ONCPU bit set, and we can stop the iteration there.
		 */
		for_each_group(group, task_psi_group(अग्र)) {
			काष्ठा psi_group_cpu *groupc = per_cpu_ptr(group->pcpu, cpu);

			अगर (groupc->state_mask & PSI_ONCPU) {
				common = group;
				अवरोध;
			}
			psi_group_change(group, cpu, 0, TSK_ONCPU, now, true);
		}
	}

	अगर (पूर्व->pid) {
		पूर्णांक clear = TSK_ONCPU, set = 0;
		bool wake_clock = true;

		/*
		 * When we're going to sleep, psi_dequeue() lets us
		 * handle TSK_RUNNING, TSK_MEMSTALL_RUNNING and
		 * TSK_IOWAIT here, where we can combine it with
		 * TSK_ONCPU and save walking common ancestors twice.
		 */
		अगर (sleep) {
			clear |= TSK_RUNNING;
			अगर (पूर्व->in_memstall)
				clear |= TSK_MEMSTALL_RUNNING;
			अगर (पूर्व->in_iowait)
				set |= TSK_IOWAIT;

			/*
			 * Periodic aggregation shuts off if there is a period of no
			 * task changes, so we wake it back up if necessary. However,
			 * don't do this if the task change is the aggregation worker
			 * itself going to sleep, or we'll ping-pong forever.
			 */
			अगर (unlikely((पूर्व->flags & PF_WQ_WORKER) &&
				     wq_worker_last_func(पूर्व) == psi_avgs_work))
				wake_clock = false;
		}

		psi_flags_change(पूर्व, clear, set);

		for_each_group(group, task_psi_group(पूर्व)) {
			अगर (group == common)
				अवरोध;
			psi_group_change(group, cpu, clear, set, now, wake_clock);
		}

		/*
		 * TSK_ONCPU is handled up to the common ancestor. If there are
		 * any other differences between the two tasks (e.g. prev goes
		 * to sleep, or only one task is memstall), finish propagating
		 * those differences all the way up to the root.
		 */
		अगर ((पूर्व->psi_flags ^ अग्र->psi_flags) & ~TSK_ONCPU) {
			clear &= ~TSK_ONCPU;
			for_each_group(group, common)
				psi_group_change(group, cpu, clear, set, now, wake_clock);
		}
	}
	psi_write_end(cpu);
}

#अगर_घोषित CONFIG_IRQ_TIME_ACCOUNTING
व्योम psi_account_irqtime(काष्ठा rq *rq, काष्ठा task_struct *curr, काष्ठा task_struct *पूर्व)
{
	पूर्णांक cpu = task_cpu(curr);
	काष्ठा psi_group_cpu *groupc;
	s64 delta;
	u64 irq;
	u64 now;

	अगर (static_branch_likely(&psi_disabled) || !irqtime_enabled())
		वापस;

	अगर (!curr->pid)
		वापस;

	lockdep_assert_rq_held(rq);
	अगर (पूर्व && task_psi_group(पूर्व) == task_psi_group(curr))
		वापस;

	irq = irq_time_read(cpu);
	delta = (s64)(irq - rq->psi_irq_time);
	अगर (delta <= 0)
		वापस;
	rq->psi_irq_time = irq;

	psi_write_begin(cpu);
	now = cpu_clock(cpu);

	for_each_group(group, task_psi_group(curr)) {
		अगर (!group->enabled)
			जारी;

		groupc = per_cpu_ptr(group->pcpu, cpu);

		record_times(groupc, now);
		groupc->बार[PSI_IRQ_FULL] += delta;

		अगर (group->rtpoll_states & (1 << PSI_IRQ_FULL))
			psi_schedule_rtpoll_work(group, 1, false);
	}
	psi_write_end(cpu);
}
#पूर्ण_अगर /* CONFIG_IRQ_TIME_ACCOUNTING */

/**
 * psi_memstall_enter - mark the beginning of a memory stall section
 * @flags: flags to handle nested sections
 *
 * Marks the calling task as being stalled due to a lack of memory,
 * such as waiting for a refault or performing reclaim.
 */
व्योम psi_memstall_enter(अचिन्हित दीर्घ *flags)
{
	काष्ठा rq_flags rf;
	काष्ठा rq *rq;

	अगर (static_branch_likely(&psi_disabled))
		वापस;

	*flags = current->in_memstall;
	अगर (*flags)
		वापस;
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
EXPORT_प्रतीक_GPL(psi_memstall_enter);

/**
 * psi_memstall_leave - mark the end of an memory stall section
 * @flags: flags to handle nested memdelay sections
 *
 * Marks the calling task as no longer stalled due to lack of memory.
 */
व्योम psi_memstall_leave(अचिन्हित दीर्घ *flags)
{
	काष्ठा rq_flags rf;
	काष्ठा rq *rq;

	अगर (static_branch_likely(&psi_disabled))
		वापस;

	अगर (*flags)
		वापस;
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
EXPORT_प्रतीक_GPL(psi_memstall_leave);

#अगर_घोषित CONFIG_CGROUPS
पूर्णांक psi_cgroup_alloc(काष्ठा cgroup *cgroup)
{
	अगर (!static_branch_likely(&psi_cgroups_enabled))
		वापस 0;

	cgroup->psi = kzalloc_obj(काष्ठा psi_group);
	अगर (!cgroup->psi)
		वापस -ENOMEM;

	cgroup->psi->pcpu = alloc_percpu(काष्ठा psi_group_cpu);
	अगर (!cgroup->psi->pcpu) {
		kfree(cgroup->psi);
		वापस -ENOMEM;
	}
	group_init(cgroup->psi);
	cgroup->psi->parent = cgroup_psi(cgroup_parent(cgroup));
	वापस 0;
}

व्योम psi_cgroup_free(काष्ठा cgroup *cgroup)
{
	अगर (!static_branch_likely(&psi_cgroups_enabled))
		वापस;

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
व्योम cgroup_move_task(काष्ठा task_struct *task, काष्ठा css_set *to)
{
	अचिन्हित पूर्णांक task_flags;
	काष्ठा rq_flags rf;
	काष्ठा rq *rq;

	अगर (!static_branch_likely(&psi_cgroups_enabled)) {
		/*
		 * Lame to do this here, but the scheduler cannot be locked
		 * from the outside, so we move cgroups from inside sched/.
		 */
		rcu_assign_pointer(task->cgroups, to);
		वापस;
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

	अगर (task_flags)
		psi_task_change(task, task_flags, 0);

	/* See comment above */
	rcu_assign_pointer(task->cgroups, to);

	अगर (task_flags)
		psi_task_change(task, 0, task_flags);

	task_rq_unlock(rq, task, &rf);
}

व्योम psi_cgroup_restart(काष्ठा psi_group *group)
{
	पूर्णांक cpu;

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
	अगर (!group->enabled)
		वापस;

	for_each_possible_cpu(cpu) {
		u64 now;

		guard(rq_lock_irq)(cpu_rq(cpu));

		psi_write_begin(cpu);
		now = cpu_clock(cpu);
		psi_group_change(group, cpu, 0, 0, now, true);
		psi_write_end(cpu);
	}
}
#पूर्ण_अगर /* CONFIG_CGROUPS */

पूर्णांक psi_show(काष्ठा seq_file *m, काष्ठा psi_group *group, क्रमागत psi_res res)
{
	bool only_full = false;
	पूर्णांक full;
	u64 now;

	अगर (static_branch_likely(&psi_disabled))
		वापस -EOPNOTSUPP;

#अगर_घोषित CONFIG_IRQ_TIME_ACCOUNTING
	अगर (!irqtime_enabled() && res == PSI_IRQ)
		वापस -EOPNOTSUPP;
#पूर्ण_अगर

	/* Update averages before reporting them */
	mutex_lock(&group->avgs_lock);
	now = sched_clock();
	collect_percpu_times(group, PSI_AVGS, शून्य);
	अगर (now >= group->avg_अग्र_update)
		group->avg_अग्र_update = update_averages(group, now);
	mutex_unlock(&group->avgs_lock);

#अगर_घोषित CONFIG_IRQ_TIME_ACCOUNTING
	only_full = res == PSI_IRQ;
#पूर्ण_अगर

	क्रम (full = 0; full < 2 - only_full; full++) {
		अचिन्हित दीर्घ avg[3] = { 0, };
		u64 योग_2 = 0;
		पूर्णांक w;

		/* CPU FULL is undefined at the system level */
		अगर (!(group == &psi_system && res == PSI_CPU && full)) {
			क्रम (w = 0; w < 3; w++)
				avg[w] = group->avg[res * 2 + full][w];
			योग_2 = div_u64(group->योग_2[PSI_AVGS][res * 2 + full],
					NSEC_PER_USEC);
		}

		seq_printf(m, "%s avg10=%lu.%02lu avg60=%lu.%02lu avg300=%lu.%02lu total=%llu\n",
			   full || only_full ? "full" : "some",
			   LOAD_INT(avg[0]), LOAD_FRAC(avg[0]),
			   LOAD_INT(avg[1]), LOAD_FRAC(avg[1]),
			   LOAD_INT(avg[2]), LOAD_FRAC(avg[2]),
			   योग_2);
	}

	वापस 0;
}

/*
 * Create @group's rtpoll worker after psi_trigger_create() reported the need
 * for one. kthread creation depends on the whole fork path and we don't want
 * all of that nested inside cgroup_mutex, so the caller must drop it and any
 * other lock that forks can wait behind. If two callers race, the loser stops
 * its never-woken kthread.
 */
पूर्णांक psi_trigger_create_rtpoll_worker(काष्ठा psi_group *group)
{
	काष्ठा task_struct *task;

	task = kthread_create(psi_rtpoll_worker, group, "psimon");
	अगर (IS_ERR(task))
		वापस PTR_ERR(task);

	scoped_guard(mutex, &group->rtpoll_trigger_lock) {
		अगर (!rcu_access_pointer(group->rtpoll_task)) {
			atomic_set(&group->rtpoll_wakeup, 0);
			wake_up_process(task);
			rcu_assign_pointer(group->rtpoll_task, task);

			/*
			 * Poll once to catch up on scheduling attempts dropped
			 * while there was no rtpoll worker.
			 */
			psi_schedule_rtpoll_work(group, 1, true);
			वापस 0;
		}
	}

	kthread_stop(task);
	वापस 0;
}

काष्ठा psi_trigger *psi_trigger_create(काष्ठा psi_group *group, अक्षर *buf,
				       क्रमागत psi_res res, काष्ठा file *file,
				       काष्ठा kernfs_open_file *of,
				       bool *need_rtpoll_worker)
{
	काष्ठा psi_trigger *t;
	क्रमागत psi_states state;
	u32 threshold_us;
	bool privileged;
	u32 window_us;

	*need_rtpoll_worker = false;

	अगर (static_branch_likely(&psi_disabled))
		वापस ERR_PTR(-EOPNOTSUPP);

	/*
	 * Checking the privilege here on file->f_cred implies that a privileged user
	 * could open the file and delegate the write to an unprivileged one.
	 */
	privileged = cap_raised(file->f_cred->cap_effective, CAP_SYS_RESOURCE);

	अगर (माला_पूछो(buf, "some %u %u", &threshold_us, &window_us) == 2)
		state = PSI_IO_SOME + res * 2;
	अन्यथा अगर (माला_पूछो(buf, "full %u %u", &threshold_us, &window_us) == 2)
		state = PSI_IO_FULL + res * 2;
	अन्यथा
		वापस ERR_PTR(-EINVAL);

#अगर_घोषित CONFIG_IRQ_TIME_ACCOUNTING
	अगर (res == PSI_IRQ && --state != PSI_IRQ_FULL)
		वापस ERR_PTR(-EINVAL);
#पूर्ण_अगर

	अगर (state >= PSI_NONIDLE)
		वापस ERR_PTR(-EINVAL);

	अगर (window_us == 0 || window_us > WINDOW_MAX_US)
		वापस ERR_PTR(-EINVAL);

	/*
	 * Unprivileged users can only use 2s windows so that averages aggregation
	 * work is used, and no RT threads need to be spawned.
	 */
	अगर (!privileged && window_us % 2000000)
		वापस ERR_PTR(-EINVAL);

	/* Check threshold */
	अगर (threshold_us == 0 || threshold_us > window_us)
		वापस ERR_PTR(-EINVAL);

	t = kmalloc_obj(*t);
	अगर (!t)
		वापस ERR_PTR(-ENOMEM);

	t->group = group;
	t->state = state;
	t->threshold = threshold_us * NSEC_PER_USEC;
	t->win.आकार = window_us * NSEC_PER_USEC;
	window_reset(&t->win, sched_clock(),
			group->योग_2[PSI_POLL][t->state], 0);

	t->event = 0;
	t->last_event_time = 0;
	t->of = of;
	अगर (!of)
		init_waitqueue_शीर्ष(&t->event_wait);
	t->pending_event = false;
	t->aggregator = privileged ? PSI_POLL : PSI_AVGS;

	अगर (privileged) {
		mutex_lock(&group->rtpoll_trigger_lock);

		list_add(&t->गाँठ, &group->rtpoll_triggers);
		group->rtpoll_min_period = min(group->rtpoll_min_period,
			div_u64(t->win.आकार, UPDATES_PER_WINDOW));
		group->rtpoll_nr_triggers[t->state]++;
		group->rtpoll_states |= (1 << t->state);

		*need_rtpoll_worker = !rcu_access_pointer(group->rtpoll_task);

		mutex_unlock(&group->rtpoll_trigger_lock);
	} अन्यथा {
		mutex_lock(&group->avgs_lock);

		list_add(&t->गाँठ, &group->avg_triggers);
		group->avg_nr_triggers[t->state]++;

		mutex_unlock(&group->avgs_lock);
	}
	वापस t;
}

व्योम psi_trigger_destroy(काष्ठा psi_trigger *t)
{
	काष्ठा psi_group *group;
	काष्ठा task_struct *task_to_destroy = शून्य;

	/*
	 * We do not check psi_disabled since it might have been disabled after
	 * the trigger got created.
	 */
	अगर (!t)
		वापस;

	group = t->group;
	/*
	 * Wakeup waiters to stop polling and clear the queue to prevent it from
	 * being accessed later. Can happen if cgroup is deleted from under a
	 * polling process.
	 */
	अगर (t->of)
		kernfs_notify(t->of->kn);
	अन्यथा
		wake_up_interruptible(&t->event_wait);

	अगर (t->aggregator == PSI_AVGS) {
		mutex_lock(&group->avgs_lock);
		अगर (!list_empty(&t->गाँठ)) {
			list_del(&t->गाँठ);
			group->avg_nr_triggers[t->state]--;
		}
		mutex_unlock(&group->avgs_lock);
	} अन्यथा {
		mutex_lock(&group->rtpoll_trigger_lock);
		अगर (!list_empty(&t->गाँठ)) {
			काष्ठा psi_trigger *tmp;
			u64 period = ULLONG_MAX;

			list_del(&t->गाँठ);
			group->rtpoll_nr_triggers[t->state]--;
			अगर (!group->rtpoll_nr_triggers[t->state])
				group->rtpoll_states &= ~(1 << t->state);
			/*
			 * Reset min update period for the remaining triggers
			 * iff the destroying trigger had the min window size.
			 */
			अगर (group->rtpoll_min_period == div_u64(t->win.आकार, UPDATES_PER_WINDOW)) {
				list_for_each_entry(tmp, &group->rtpoll_triggers, गाँठ)
					period = min(period, div_u64(tmp->win.आकार,
							UPDATES_PER_WINDOW));
				group->rtpoll_min_period = period;
			}
			/* Destroy rtpoll_task when the last trigger is destroyed */
			अगर (group->rtpoll_states == 0) {
				group->rtpoll_until = 0;
				task_to_destroy = rcu_dereference_protected(
						group->rtpoll_task,
						lockdep_is_held(&group->rtpoll_trigger_lock));
				rcu_assign_pointer(group->rtpoll_task, शून्य);
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
	अगर (task_to_destroy) {
		kthread_stop(task_to_destroy);
		atomic_set(&group->rtpoll_scheduled, 0);
	}
	kfree(t);
}

__poll_t psi_trigger_poll(व्योम **trigger_ptr,
				काष्ठा file *file, poll_table *रुको)
{
	__poll_t ret = DEFAULT_POLLMASK;
	काष्ठा psi_trigger *t;

	अगर (static_branch_likely(&psi_disabled))
		वापस DEFAULT_POLLMASK | EPOLLERR | EPOLLPRI;

	t = smp_load_acquire(trigger_ptr);
	अगर (!t)
		वापस DEFAULT_POLLMASK | EPOLLERR | EPOLLPRI;

	अगर (t->of)
		kernfs_generic_poll(t->of, रुको);
	अन्यथा
		poll_wait(file, &t->event_wait, रुको);

	अगर (cmpxchg(&t->event, 1, 0) == 1)
		ret |= EPOLLPRI;

	वापस ret;
}

#अगर_घोषित CONFIG_PROC_FS
अटल पूर्णांक psi_io_show(काष्ठा seq_file *m, व्योम *v)
{
	वापस psi_show(m, &psi_system, PSI_IO);
}

अटल पूर्णांक psi_memory_show(काष्ठा seq_file *m, व्योम *v)
{
	वापस psi_show(m, &psi_system, PSI_MEM);
}

अटल पूर्णांक psi_cpu_show(काष्ठा seq_file *m, व्योम *v)
{
	वापस psi_show(m, &psi_system, PSI_CPU);
}

अटल पूर्णांक psi_io_open(काष्ठा inode *inode, काष्ठा file *file)
{
	वापस single_open(file, psi_io_show, शून्य);
}

अटल पूर्णांक psi_memory_open(काष्ठा inode *inode, काष्ठा file *file)
{
	वापस single_open(file, psi_memory_show, शून्य);
}

अटल पूर्णांक psi_cpu_open(काष्ठा inode *inode, काष्ठा file *file)
{
	वापस single_open(file, psi_cpu_show, शून्य);
}

अटल ssize_t psi_write(काष्ठा file *file, स्थिर अक्षर __user *user_buf,
			 माप_प्रकार nbytes, क्रमागत psi_res res)
{
	अक्षर buf[32];
	माप_प्रकार buf_आकार;
	काष्ठा seq_file *seq;
	काष्ठा psi_trigger *new;
	bool need_rtpoll_worker;
	पूर्णांक ret;

	अगर (static_branch_likely(&psi_disabled))
		वापस -EOPNOTSUPP;

	अगर (!nbytes)
		वापस -EINVAL;

	buf_आकार = min(nbytes, माप(buf));
	अगर (copy_from_user(buf, user_buf, buf_आकार))
		वापस -EFAULT;

	buf[buf_आकार - 1] = '\0';

	seq = file->private_तथ्य;

	/* Take seq->lock to protect seq->private from concurrent writes */
	mutex_lock(&seq->lock);

	/* Allow only one trigger per file descriptor */
	अगर (seq->निजी) {
		mutex_unlock(&seq->lock);
		वापस -EBUSY;
	}

	new = psi_trigger_create(&psi_system, buf, res, file, शून्य,
				 &need_rtpoll_worker);
	अगर (IS_ERR(new)) {
		mutex_unlock(&seq->lock);
		वापस PTR_ERR(new);
	}

	अगर (need_rtpoll_worker) {
		ret = psi_trigger_create_rtpoll_worker(&psi_system);
		अगर (ret) {
			psi_trigger_destroy(new);
			mutex_unlock(&seq->lock);
			वापस ret;
		}
	}

	smp_store_release(&seq->निजी, new);
	mutex_unlock(&seq->lock);

	वापस nbytes;
}

अटल ssize_t psi_io_write(काष्ठा file *file, स्थिर अक्षर __user *user_buf,
			    माप_प्रकार nbytes, loff_t *ppos)
{
	वापस psi_write(file, user_buf, nbytes, PSI_IO);
}

अटल ssize_t psi_memory_write(काष्ठा file *file, स्थिर अक्षर __user *user_buf,
				माप_प्रकार nbytes, loff_t *ppos)
{
	वापस psi_write(file, user_buf, nbytes, PSI_MEM);
}

अटल ssize_t psi_cpu_write(काष्ठा file *file, स्थिर अक्षर __user *user_buf,
			     माप_प्रकार nbytes, loff_t *ppos)
{
	वापस psi_write(file, user_buf, nbytes, PSI_CPU);
}

अटल __poll_t psi_fop_poll(काष्ठा file *file, poll_table *रुको)
{
	काष्ठा seq_file *seq = file->private_तथ्य;

	वापस psi_trigger_poll(&seq->निजी, file, रुको);
}

अटल पूर्णांक psi_fop_release(काष्ठा inode *inode, काष्ठा file *file)
{
	काष्ठा seq_file *seq = file->private_तथ्य;

	psi_trigger_destroy(seq->निजी);
	वापस single_release(inode, file);
}

अटल स्थिर काष्ठा proc_ops psi_io_proc_ops = {
	.proc_open	= psi_io_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_io_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

अटल स्थिर काष्ठा proc_ops psi_memory_proc_ops = {
	.proc_open	= psi_memory_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_memory_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

अटल स्थिर काष्ठा proc_ops psi_cpu_proc_ops = {
	.proc_open	= psi_cpu_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_cpu_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};

#अगर_घोषित CONFIG_IRQ_TIME_ACCOUNTING
अटल पूर्णांक psi_irq_show(काष्ठा seq_file *m, व्योम *v)
{
	वापस psi_show(m, &psi_system, PSI_IRQ);
}

अटल पूर्णांक psi_irq_open(काष्ठा inode *inode, काष्ठा file *file)
{
	वापस single_open(file, psi_irq_show, शून्य);
}

अटल ssize_t psi_irq_write(काष्ठा file *file, स्थिर अक्षर __user *user_buf,
			     माप_प्रकार nbytes, loff_t *ppos)
{
	वापस psi_write(file, user_buf, nbytes, PSI_IRQ);
}

अटल स्थिर काष्ठा proc_ops psi_irq_proc_ops = {
	.proc_open	= psi_irq_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_write	= psi_irq_write,
	.proc_poll	= psi_fop_poll,
	.proc_release	= psi_fop_release,
};
#पूर्ण_अगर /* CONFIG_IRQ_TIME_ACCOUNTING */

अटल पूर्णांक __init psi_proc_init(व्योम)
{
	अगर (psi_enable) {
		proc_mkdir("pressure", शून्य);
		proc_create("pressure/io", 0666, शून्य, &psi_io_proc_ops);
		proc_create("pressure/memory", 0666, शून्य, &psi_memory_proc_ops);
		proc_create("pressure/cpu", 0666, शून्य, &psi_cpu_proc_ops);
#अगर_घोषित CONFIG_IRQ_TIME_ACCOUNTING
		proc_create("pressure/irq", 0666, शून्य, &psi_irq_proc_ops);
#पूर्ण_अगर
	}
	वापस 0;
}
module_init(psi_proc_init);

#पूर्ण_अगर /* CONFIG_PROC_FS */
