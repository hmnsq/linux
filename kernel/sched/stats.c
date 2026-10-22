// SPDX-License-Identifier: GPL-2.0
/*
 * /proc/schedstat implementation
 */
#ಅನ್ತರ್ಭಾವ "sched.h"

ಶೂನ್ಯ __update_stats_wait_start(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p,
			       ಸಂರಚನಾ sched_statistics *stats)
{
	u64 wait_start, prev_wait_start;

	wait_start = rq_clock(rq);
	prev_wait_start = schedstat_val(stats->wait_start);

	ಯದಿ (p && likely(wait_start > prev_wait_start))
		wait_start -= prev_wait_start;

	__schedstat_set(stats->wait_start, wait_start);
}

ಶೂನ್ಯ __update_stats_wait_end(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p,
			     ಸಂರಚನಾ sched_statistics *stats)
{
	u64 delta = rq_clock(rq) - schedstat_val(stats->wait_start);

	ಯದಿ (p) {
		ಯದಿ (task_on_rq_migrating(p)) {
			/*
			 * Preserve migrating task's wait time so wait_start
			 * time stamp can be adjusted to accumulate wait time
			 * prior to migration.
			 */
			__schedstat_set(stats->wait_start, delta);

			ಪ್ರತ್ಯಾಗಮ;
		}

		trace_sched_stat_wait(p, delta);
	}

	__schedstat_set(stats->wait_max,
			max(schedstat_val(stats->wait_max), delta));
	__schedstat_inc(stats->wait_ಎಣಿಕೆ);
	__schedstat_add(stats->wait_sum, delta);
	__schedstat_set(stats->wait_start, 0);
}

ಶೂನ್ಯ __update_stats_enqueue_sleeper(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p,
				    ಸಂರಚನಾ sched_statistics *stats)
{
	u64 sleep_start, block_start;

	sleep_start = schedstat_val(stats->sleep_start);
	block_start = schedstat_val(stats->block_start);

	ಯದಿ (sleep_start) {
		u64 delta = rq_clock(rq) - sleep_start;

		ಯದಿ ((s64)delta < 0)
			delta = 0;

		ಯದಿ (unlikely(delta > schedstat_val(stats->sleep_max)))
			__schedstat_set(stats->sleep_max, delta);

		__schedstat_set(stats->sleep_start, 0);
		__schedstat_add(stats->sum_sleep_runtime, delta);

		ಯದಿ (p) {
			account_scheduler_latency(p, delta >> 10, 1);
			trace_sched_stat_sleep(p, delta);
		}
	}

	ಯದಿ (block_start) {
		u64 delta = rq_clock(rq) - block_start;

		ಯದಿ ((s64)delta < 0)
			delta = 0;

		ಯದಿ (unlikely(delta > schedstat_val(stats->block_max)))
			__schedstat_set(stats->block_max, delta);

		__schedstat_set(stats->block_start, 0);
		__schedstat_add(stats->sum_sleep_runtime, delta);
		__schedstat_add(stats->sum_block_runtime, delta);

		ಯದಿ (p) {
			ಯದಿ (p->in_iowait) {
				__schedstat_add(stats->iowait_sum, delta);
				__schedstat_inc(stats->iowait_ಎಣಿಕೆ);
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

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_schedstat(ಸಂರಚನಾ seq_file *seq, ಶೂನ್ಯ *v)
{
	ಪೂರ್ಣಾಙ್ಕ cpu;

	ಯದಿ (v == (ಶೂನ್ಯ *)1) {
		seq_printf(seq, "version %d\n", SCHEDSTAT_VERSION);
		seq_printf(seq, "timestamp %lu\n", jiffies);
	} ಅನ್ಯಥಾ {
		ಸಂರಚನಾ rq *rq;
		ಸಂರಚನಾ sched_domain *sd;
		ಪೂರ್ಣಾಙ್ಕ dcount = 0;
		cpu = (unsigned long)(v - 2);
		rq = cpu_rq(cpu);

		/* runqueue-specific stats */
		seq_printf(seq,
		    "cpu%d %u 0 %u %u %u %u %llu %llu %lu",
		    cpu, rq->yld_ಎಣಿಕೆ,
		    rq->sched_ಎಣಿಕೆ, rq->sched_goidle,
		    rq->ttwu_ಎಣಿಕೆ, rq->ttwu_local,
		    rq->rq_cpu_time,
		    rq->rq_sched_info.run_delay, rq->rq_sched_info.pcount);

		seq_printf(seq, "\n");

		/* domain-specific stats */
		rcu_read_lock();
		for_each_domain(cpu, sd) {
			ಗಣನಾ cpu_idle_type itype;

			seq_printf(seq, "domain%d %s %*pb", dcount++, sd->name,
				   cpumask_pr_args(sched_domain_span(sd)));
			ಪ್ರತಿ (itype = 0; itype < CPU_MAX_IDLE_TYPES; itype++) {
				seq_printf(seq, " %u %u %u %u %u %u %u %u %u %u %u",
				    sd->lb_ಎಣಿಕೆ[itype],
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
			    sd->alb_ಎಣಿಕೆ, sd->alb_failed, sd->alb_pushed,
			    sd->sbe_ಎಣಿಕೆ, sd->sbe_balanced, sd->sbe_pushed,
			    sd->sbf_ಎಣಿಕೆ, sd->sbf_balanced, sd->sbf_pushed,
			    sd->ttwu_wake_remote, sd->ttwu_move_affine,
			    sd->ttwu_move_balance);
		}
		rcu_read_unlock();
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * This iterator needs some explanation.
 * It returns 1 for the header position.
 * This means 2 is cpu 0.
 * In a hotplugged system some CPUs, including cpu 0, may be missing so we have
 * to use cpumask_* to iterate over the CPUs.
 */
ಸ್ಥಿರ ಶೂನ್ಯ *schedstat_start(ಸಂರಚನಾ seq_file *file, loff_t *offset)
{
	unsigned long n = *offset;

	ಯದಿ (n == 0)
		ಪ್ರತ್ಯಾಗಮ (ಶೂನ್ಯ *) 1;

	n--;

	ಯದಿ (n > 0)
		n = cpumask_next(n - 1, cpu_online_mask);
	ಅನ್ಯಥಾ
		n = cpumask_first(cpu_online_mask);

	*offset = n + 1;

	ಯದಿ (n < nr_cpu_ids)
		ಪ್ರತ್ಯಾಗಮ (ಶೂನ್ಯ *)(unsigned long)(n + 2);

	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಶೂನ್ಯ *schedstat_next(ಸಂರಚನಾ seq_file *file, ಶೂನ್ಯ *data, loff_t *offset)
{
	(*offset)++;

	ಪ್ರತ್ಯಾಗಮ schedstat_start(file, offset);
}

ಸ್ಥಿರ ಶೂನ್ಯ schedstat_stop(ಸಂರಚನಾ seq_file *file, ಶೂನ್ಯ *data)
{
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ seq_operations schedstat_sops = {
	.start = schedstat_start,
	.next  = schedstat_next,
	.stop  = schedstat_stop,
	.show  = show_schedstat,
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ __init proc_schedstat_init(ಶೂನ್ಯ)
{
	proc_create_seq("schedstat", 0, ರಿಕ್ತ, &schedstat_sops);
	ಪ್ರತ್ಯಾಗಮ 0;
}
subsys_initcall(proc_schedstat_init);
