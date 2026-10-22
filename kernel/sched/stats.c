// SPDX-License-Identifier: GPL-2.0
/*
 * /proc/schedstat implementation
 */
#અન્તર્ભાવ "sched.h"

શૂન્ય __update_stats_wait_start(સંરચના rq *rq, સંરચના task_struct *p,
			       સંરચના sched_statistics *stats)
{
	u64 wait_start, prev_wait_start;

	wait_start = rq_clock(rq);
	prev_wait_start = schedstat_val(stats->wait_start);

	જો (p && likely(wait_start > prev_wait_start))
		wait_start -= prev_wait_start;

	__schedstat_set(stats->wait_start, wait_start);
}

શૂન્ય __update_stats_wait_end(સંરચના rq *rq, સંરચના task_struct *p,
			     સંરચના sched_statistics *stats)
{
	u64 delta = rq_clock(rq) - schedstat_val(stats->wait_start);

	જો (p) {
		જો (task_on_rq_migrating(p)) {
			/*
			 * Preserve migrating task's wait time so wait_start
			 * time stamp can be adjusted to accumulate wait time
			 * prior to migration.
			 */
			__schedstat_set(stats->wait_start, delta);

			પાછા;
		}

		trace_sched_stat_wait(p, delta);
	}

	__schedstat_set(stats->wait_max,
			max(schedstat_val(stats->wait_max), delta));
	__schedstat_inc(stats->wait_ગણતરી);
	__schedstat_add(stats->wait_sum, delta);
	__schedstat_set(stats->wait_start, 0);
}

શૂન્ય __update_stats_enqueue_sleeper(સંરચના rq *rq, સંરચના task_struct *p,
				    સંરચના sched_statistics *stats)
{
	u64 sleep_start, block_start;

	sleep_start = schedstat_val(stats->sleep_start);
	block_start = schedstat_val(stats->block_start);

	જો (sleep_start) {
		u64 delta = rq_clock(rq) - sleep_start;

		જો ((s64)delta < 0)
			delta = 0;

		જો (unlikely(delta > schedstat_val(stats->sleep_max)))
			__schedstat_set(stats->sleep_max, delta);

		__schedstat_set(stats->sleep_start, 0);
		__schedstat_add(stats->sum_sleep_runtime, delta);

		જો (p) {
			account_scheduler_latency(p, delta >> 10, 1);
			trace_sched_stat_sleep(p, delta);
		}
	}

	જો (block_start) {
		u64 delta = rq_clock(rq) - block_start;

		જો ((s64)delta < 0)
			delta = 0;

		જો (unlikely(delta > schedstat_val(stats->block_max)))
			__schedstat_set(stats->block_max, delta);

		__schedstat_set(stats->block_start, 0);
		__schedstat_add(stats->sum_sleep_runtime, delta);
		__schedstat_add(stats->sum_block_runtime, delta);

		જો (p) {
			જો (p->in_iowait) {
				__schedstat_add(stats->iowait_sum, delta);
				__schedstat_inc(stats->iowait_ગણતરી);
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

સ્થિર પૂર્ણાંક show_schedstat(સંરચના seq_file *seq, શૂન્ય *v)
{
	પૂર્ણાંક cpu;

	જો (v == (શૂન્ય *)1) {
		seq_printf(seq, "version %d\n", SCHEDSTAT_VERSION);
		seq_printf(seq, "timestamp %lu\n", jiffies);
	} નહીંતર {
		સંરચના rq *rq;
		સંરચના sched_domain *sd;
		પૂર્ણાંક dcount = 0;
		cpu = (unsigned long)(v - 2);
		rq = cpu_rq(cpu);

		/* runqueue-specific stats */
		seq_printf(seq,
		    "cpu%d %u 0 %u %u %u %u %llu %llu %lu",
		    cpu, rq->yld_ગણતરી,
		    rq->sched_ગણતરી, rq->sched_goidle,
		    rq->ttwu_ગણતરી, rq->ttwu_local,
		    rq->rq_cpu_time,
		    rq->rq_sched_info.run_delay, rq->rq_sched_info.pcount);

		seq_printf(seq, "\n");

		/* domain-specific stats */
		rcu_read_lock();
		for_each_domain(cpu, sd) {
			ગણના cpu_idle_type itype;

			seq_printf(seq, "domain%d %s %*pb", dcount++, sd->name,
				   cpumask_pr_args(sched_domain_span(sd)));
			માટે (itype = 0; itype < CPU_MAX_IDLE_TYPES; itype++) {
				seq_printf(seq, " %u %u %u %u %u %u %u %u %u %u %u",
				    sd->lb_ગણતરી[itype],
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
			    sd->alb_ગણતરી, sd->alb_failed, sd->alb_pushed,
			    sd->sbe_ગણતરી, sd->sbe_balanced, sd->sbe_pushed,
			    sd->sbf_ગણતરી, sd->sbf_balanced, sd->sbf_pushed,
			    sd->ttwu_wake_remote, sd->ttwu_move_affine,
			    sd->ttwu_move_balance);
		}
		rcu_read_unlock();
	}
	પાછા 0;
}

/*
 * This iterator needs some explanation.
 * It returns 1 for the header position.
 * This means 2 is cpu 0.
 * In a hotplugged system some CPUs, including cpu 0, may be missing so we have
 * to use cpumask_* to iterate over the CPUs.
 */
સ્થિર શૂન્ય *schedstat_start(સંરચના seq_file *file, loff_t *offset)
{
	unsigned long n = *offset;

	જો (n == 0)
		પાછા (શૂન્ય *) 1;

	n--;

	જો (n > 0)
		n = cpumask_next(n - 1, cpu_online_mask);
	નહીંતર
		n = cpumask_first(cpu_online_mask);

	*offset = n + 1;

	જો (n < nr_cpu_ids)
		પાછા (શૂન્ય *)(unsigned long)(n + 2);

	પાછા ખાલી;
}

સ્થિર શૂન્ય *schedstat_next(સંરચના seq_file *file, શૂન્ય *data, loff_t *offset)
{
	(*offset)++;

	પાછા schedstat_start(file, offset);
}

સ્થિર શૂન્ય schedstat_stop(સંરચના seq_file *file, શૂન્ય *data)
{
}

સ્થિર અચલ સંરચના seq_operations schedstat_sops = {
	.start = schedstat_start,
	.next  = schedstat_next,
	.stop  = schedstat_stop,
	.show  = show_schedstat,
};

સ્થિર પૂર્ણાંક __init proc_schedstat_init(શૂન્ય)
{
	proc_create_seq("schedstat", 0, ખાલી, &schedstat_sops);
	પાછા 0;
}
subsys_initcall(proc_schedstat_init);
