// SPDX-License-Identifier: GPL-2.0
/*
 * /proc/schedstat implementation
 */
#شامل "sched.h"

تهی __update_stats_wait_start(ساختار rq *rq, ساختار task_struct *p,
			       ساختار sched_statistics *stats)
{
	u64 wait_start, prev_wait_start;

	wait_start = rq_clock(rq);
	prev_wait_start = schedstat_val(stats->wait_start);

	اگر (p && likely(wait_start > prev_wait_start))
		wait_start -= prev_wait_start;

	__schedstat_set(stats->wait_start, wait_start);
}

تهی __update_stats_wait_end(ساختار rq *rq, ساختار task_struct *p,
			     ساختار sched_statistics *stats)
{
	u64 delta = rq_clock(rq) - schedstat_val(stats->wait_start);

	اگر (p) {
		اگر (task_on_rq_migrating(p)) {
			/*
			 * Preserve migrating task's wait time so wait_start
			 * time stamp can be adjusted to accumulate wait time
			 * prior to migration.
			 */
			__schedstat_set(stats->wait_start, delta);

			بازگشت;
		}

		trace_sched_stat_wait(p, delta);
	}

	__schedstat_set(stats->wait_max,
			max(schedstat_val(stats->wait_max), delta));
	__schedstat_inc(stats->wait_شمار);
	__schedstat_add(stats->wait_sum, delta);
	__schedstat_set(stats->wait_start, 0);
}

تهی __update_stats_enqueue_sleeper(ساختار rq *rq, ساختار task_struct *p,
				    ساختار sched_statistics *stats)
{
	u64 sleep_start, block_start;

	sleep_start = schedstat_val(stats->sleep_start);
	block_start = schedstat_val(stats->block_start);

	اگر (sleep_start) {
		u64 delta = rq_clock(rq) - sleep_start;

		اگر ((s64)delta < 0)
			delta = 0;

		اگر (unlikely(delta > schedstat_val(stats->sleep_max)))
			__schedstat_set(stats->sleep_max, delta);

		__schedstat_set(stats->sleep_start, 0);
		__schedstat_add(stats->sum_sleep_runtime, delta);

		اگر (p) {
			account_scheduler_latency(p, delta >> 10, 1);
			trace_sched_stat_sleep(p, delta);
		}
	}

	اگر (block_start) {
		u64 delta = rq_clock(rq) - block_start;

		اگر ((s64)delta < 0)
			delta = 0;

		اگر (unlikely(delta > schedstat_val(stats->block_max)))
			__schedstat_set(stats->block_max, delta);

		__schedstat_set(stats->block_start, 0);
		__schedstat_add(stats->sum_sleep_runtime, delta);
		__schedstat_add(stats->sum_block_runtime, delta);

		اگر (p) {
			اگر (p->in_iowait) {
				__schedstat_add(stats->iowait_sum, delta);
				__schedstat_inc(stats->iowait_شمار);
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

ایستا صحیح show_schedstat(ساختار seq_file *seq, تهی *v)
{
	صحیح cpu;

	اگر (v == (تهی *)1) {
		seq_printf(seq, "version %d\n", SCHEDSTAT_VERSION);
		seq_printf(seq, "timestamp %lu\n", jiffies);
	} وگرنه {
		ساختار rq *rq;
		ساختار sched_domain *sd;
		صحیح dcount = 0;
		cpu = (unsigned long)(v - 2);
		rq = cpu_rq(cpu);

		/* runqueue-specific stats */
		seq_printf(seq,
		    "cpu%d %u 0 %u %u %u %u %llu %llu %lu",
		    cpu, rq->yld_شمار,
		    rq->sched_شمار, rq->sched_goidle,
		    rq->ttwu_شمار, rq->ttwu_local,
		    rq->rq_cpu_time,
		    rq->rq_sched_info.run_delay, rq->rq_sched_info.pcount);

		seq_printf(seq, "\n");

		/* domain-specific stats */
		rcu_read_lock();
		for_each_domain(cpu, sd) {
			شمارش cpu_idle_type itype;

			seq_printf(seq, "domain%d %s %*pb", dcount++, sd->name,
				   cpumask_pr_args(sched_domain_span(sd)));
			برای (itype = 0; itype < CPU_MAX_IDLE_TYPES; itype++) {
				seq_printf(seq, " %u %u %u %u %u %u %u %u %u %u %u",
				    sd->lb_شمار[itype],
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
			    sd->alb_شمار, sd->alb_failed, sd->alb_pushed,
			    sd->sbe_شمار, sd->sbe_balanced, sd->sbe_pushed,
			    sd->sbf_شمار, sd->sbf_balanced, sd->sbf_pushed,
			    sd->ttwu_wake_remote, sd->ttwu_move_affine,
			    sd->ttwu_move_balance);
		}
		rcu_read_unlock();
	}
	بازگشت 0;
}

/*
 * This iterator needs some explanation.
 * It returns 1 for the header position.
 * This means 2 is cpu 0.
 * In a hotplugged system some CPUs, including cpu 0, may be missing so we have
 * to use cpumask_* to iterate over the CPUs.
 */
ایستا تهی *schedstat_start(ساختار seq_file *file, loff_t *offset)
{
	unsigned long n = *offset;

	اگر (n == 0)
		بازگشت (تهی *) 1;

	n--;

	اگر (n > 0)
		n = cpumask_next(n - 1, cpu_online_mask);
	وگرنه
		n = cpumask_first(cpu_online_mask);

	*offset = n + 1;

	اگر (n < nr_cpu_ids)
		بازگشت (تهی *)(unsigned long)(n + 2);

	بازگشت پوچ;
}

ایستا تهی *schedstat_next(ساختار seq_file *file, تهی *data, loff_t *offset)
{
	(*offset)++;

	بازگشت schedstat_start(file, offset);
}

ایستا تهی schedstat_stop(ساختار seq_file *file, تهی *data)
{
}

ایستا ثابت ساختار seq_operations schedstat_sops = {
	.start = schedstat_start,
	.next  = schedstat_next,
	.stop  = schedstat_stop,
	.show  = show_schedstat,
};

ایستا صحیح __init proc_schedstat_init(تهی)
{
	proc_create_seq("schedstat", 0, پوچ, &schedstat_sops);
	بازگشت 0;
}
subsys_initcall(proc_schedstat_init);
