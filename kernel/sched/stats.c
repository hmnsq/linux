// SPDX-License-Identifier: GPL-2.0
/*
 * /proc/schedstat implementation
 */
#അന്തര്ഭാവ "sched.h"

ശൂന്യം __update_stats_wait_start(ഘടന rq *rq, ഘടന task_struct *p,
			       ഘടന sched_statistics *stats)
{
	u64 wait_start, prev_wait_start;

	wait_start = rq_clock(rq);
	prev_wait_start = schedstat_val(stats->wait_start);

	എങ്കിൽ (p && likely(wait_start > prev_wait_start))
		wait_start -= prev_wait_start;

	__schedstat_set(stats->wait_start, wait_start);
}

ശൂന്യം __update_stats_wait_end(ഘടന rq *rq, ഘടന task_struct *p,
			     ഘടന sched_statistics *stats)
{
	u64 delta = rq_clock(rq) - schedstat_val(stats->wait_start);

	എങ്കിൽ (p) {
		എങ്കിൽ (task_on_rq_migrating(p)) {
			/*
			 * Preserve migrating task's wait time so wait_start
			 * time stamp can be adjusted to accumulate wait time
			 * prior to migration.
			 */
			__schedstat_set(stats->wait_start, delta);

			തിരികെ;
		}

		trace_sched_stat_wait(p, delta);
	}

	__schedstat_set(stats->wait_max,
			max(schedstat_val(stats->wait_max), delta));
	__schedstat_inc(stats->wait_എണ്ണം);
	__schedstat_add(stats->wait_sum, delta);
	__schedstat_set(stats->wait_start, 0);
}

ശൂന്യം __update_stats_enqueue_sleeper(ഘടന rq *rq, ഘടന task_struct *p,
				    ഘടന sched_statistics *stats)
{
	u64 sleep_start, block_start;

	sleep_start = schedstat_val(stats->sleep_start);
	block_start = schedstat_val(stats->block_start);

	എങ്കിൽ (sleep_start) {
		u64 delta = rq_clock(rq) - sleep_start;

		എങ്കിൽ ((s64)delta < 0)
			delta = 0;

		എങ്കിൽ (unlikely(delta > schedstat_val(stats->sleep_max)))
			__schedstat_set(stats->sleep_max, delta);

		__schedstat_set(stats->sleep_start, 0);
		__schedstat_add(stats->sum_sleep_runtime, delta);

		എങ്കിൽ (p) {
			account_scheduler_latency(p, delta >> 10, 1);
			trace_sched_stat_sleep(p, delta);
		}
	}

	എങ്കിൽ (block_start) {
		u64 delta = rq_clock(rq) - block_start;

		എങ്കിൽ ((s64)delta < 0)
			delta = 0;

		എങ്കിൽ (unlikely(delta > schedstat_val(stats->block_max)))
			__schedstat_set(stats->block_max, delta);

		__schedstat_set(stats->block_start, 0);
		__schedstat_add(stats->sum_sleep_runtime, delta);
		__schedstat_add(stats->sum_block_runtime, delta);

		എങ്കിൽ (p) {
			എങ്കിൽ (p->in_iowait) {
				__schedstat_add(stats->iowait_sum, delta);
				__schedstat_inc(stats->iowait_എണ്ണം);
				trace_sched_stat_iowait(p, delta);
			}

			trace_sched_stat_blocked(p, delta);

			account_scheduler_latency(p, delta >> 10, 0);
		}
	}
}

/*
 * Current schedstat API version.
 *
 * Bump this up when changing the output format or the meaning of an existing
 * format, so that tools can adapt (or abort)
 */
#define SCHEDSTAT_VERSION 17

സ്ഥിതം പൂർണ്ണസംഖ്യ show_schedstat(ഘടന seq_file *seq, ശൂന്യം *v)
{
	പൂർണ്ണസംഖ്യ cpu;

	എങ്കിൽ (v == (ശൂന്യം *)1) {
		seq_printf(seq, "version %d\n", SCHEDSTAT_VERSION);
		seq_printf(seq, "timestamp %lu\n", jiffies);
	} അല്ലെങ്കിൽ {
		ഘടന rq *rq;
		ഘടന sched_domain *sd;
		പൂർണ്ണസംഖ്യ dcount = 0;
		cpu = (unsigned long)(v - 2);
		rq = cpu_rq(cpu);

		/* runqueue-specific stats */
		seq_printf(seq,
		    "cpu%d %u 0 %u %u %u %u %llu %llu %lu",
		    cpu, rq->yld_എണ്ണം,
		    rq->sched_എണ്ണം, rq->sched_goidle,
		    rq->ttwu_എണ്ണം, rq->ttwu_local,
		    rq->rq_cpu_time,
		    rq->rq_sched_info.run_delay, rq->rq_sched_info.pcount);

		seq_printf(seq, "\n");

		/* domain-specific stats */
		rcu_read_lock();
		for_each_domain(cpu, sd) {
			എണ്ണൽ cpu_idle_type itype;

			seq_printf(seq, "domain%d %s %*pb", dcount++, sd->name,
				   cpumask_pr_args(sched_domain_span(sd)));
			വേണ്ടി (itype = 0; itype < CPU_MAX_IDLE_TYPES; itype++) {
				seq_printf(seq, " %u %u %u %u %u %u %u %u %u %u %u",
				    sd->lb_എണ്ണം[itype],
				    sd->lb_balanced[itype],
				    sd->lb_failed[itype],
				    sd->lb_imbalance_load[itype],
				    sd->lb_imbalance_util[itype],
				    sd->lb_imbalance_task[itype],
				    sd->lb_imbalance_misfit[itype],
				    sd->lb_gained[itype],
				    sd->lb_hot_gained[itype],
				    sd->lb_nobusyq[itype],
				    sd->lb_nobusyg[itype]);
			}
			seq_printf(seq,
				   " %u %u %u %u %u %u %u %u %u %u %u %u\n",
			    sd->alb_എണ്ണം, sd->alb_failed, sd->alb_pushed,
			    sd->sbe_എണ്ണം, sd->sbe_balanced, sd->sbe_pushed,
			    sd->sbf_എണ്ണം, sd->sbf_balanced, sd->sbf_pushed,
			    sd->ttwu_wake_remote, sd->ttwu_move_affine,
			    sd->ttwu_move_balance);
		}
		rcu_read_unlock();
	}
	തിരികെ 0;
}

/*
 * This iterator needs some explanation.
 * It returns 1 for the header position.
 * This means 2 is cpu 0.
 * In a hotplugged system some CPUs, including cpu 0, may be missing so we have
 * to use cpumask_* to iterate over the CPUs.
 */
സ്ഥിതം ശൂന്യം *schedstat_start(ഘടന seq_file *file, loff_t *offset)
{
	unsigned long n = *offset;

	എങ്കിൽ (n == 0)
		തിരികെ (ശൂന്യം *) 1;

	n--;

	എങ്കിൽ (n > 0)
		n = cpumask_next(n - 1, cpu_online_mask);
	അല്ലെങ്കിൽ
		n = cpumask_first(cpu_online_mask);

	*offset = n + 1;

	എങ്കിൽ (n < nr_cpu_ids)
		തിരികെ (ശൂന്യം *)(unsigned long)(n + 2);

	തിരികെ ശൂന്യ;
}

സ്ഥിതം ശൂന്യം *schedstat_next(ഘടന seq_file *file, ശൂന്യം *data, loff_t *offset)
{
	(*offset)++;

	തിരികെ schedstat_start(file, offset);
}

സ്ഥിതം ശൂന്യം schedstat_stop(ഘടന seq_file *file, ശൂന്യം *data)
{
}

സ്ഥിതം സ്ഥിരം ഘടന seq_operations schedstat_sops = {
	.start = schedstat_start,
	.next  = schedstat_next,
	.stop  = schedstat_stop,
	.show  = show_schedstat,
};

സ്ഥിതം പൂർണ്ണസംഖ്യ __init proc_schedstat_init(ശൂന്യം)
{
	proc_create_seq("schedstat", 0, ശൂന്യ, &schedstat_sops);
	തിരികെ 0;
}
subsys_initcall(proc_schedstat_init);
