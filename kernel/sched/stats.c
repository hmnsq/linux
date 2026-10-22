// SPDX-License-Identifier: GPL-2.0
/*
 * /proc/schedstat implementation
 */
#अन्तर्भाव "sched.h"

शून्य __update_stats_wait_start(संरचना rq *rq, संरचना task_struct *p,
			       संरचना sched_statistics *stats)
{
	u64 wait_start, prev_wait_start;

	wait_start = rq_clock(rq);
	prev_wait_start = schedstat_val(stats->wait_start);

	जर (p && likely(wait_start > prev_wait_start))
		wait_start -= prev_wait_start;

	__schedstat_set(stats->wait_start, wait_start);
}

शून्य __update_stats_wait_end(संरचना rq *rq, संरचना task_struct *p,
			     संरचना sched_statistics *stats)
{
	u64 delta = rq_clock(rq) - schedstat_val(stats->wait_start);

	जर (p) {
		जर (task_on_rq_migrating(p)) {
			/*
			 * Preserve migrating task's wait time so wait_start
			 * time stamp can be adjusted to accumulate wait time
			 * prior to migration.
			 */
			__schedstat_set(stats->wait_start, delta);

			परत;
		}

		trace_sched_stat_wait(p, delta);
	}

	__schedstat_set(stats->wait_max,
			max(schedstat_val(stats->wait_max), delta));
	__schedstat_inc(stats->wait_मोजणी);
	__schedstat_add(stats->wait_sum, delta);
	__schedstat_set(stats->wait_start, 0);
}

शून्य __update_stats_enqueue_sleeper(संरचना rq *rq, संरचना task_struct *p,
				    संरचना sched_statistics *stats)
{
	u64 sleep_start, block_start;

	sleep_start = schedstat_val(stats->sleep_start);
	block_start = schedstat_val(stats->block_start);

	जर (sleep_start) {
		u64 delta = rq_clock(rq) - sleep_start;

		जर ((s64)delta < 0)
			delta = 0;

		जर (unlikely(delta > schedstat_val(stats->sleep_max)))
			__schedstat_set(stats->sleep_max, delta);

		__schedstat_set(stats->sleep_start, 0);
		__schedstat_add(stats->sum_sleep_runtime, delta);

		जर (p) {
			account_scheduler_latency(p, delta >> 10, 1);
			trace_sched_stat_sleep(p, delta);
		}
	}

	जर (block_start) {
		u64 delta = rq_clock(rq) - block_start;

		जर ((s64)delta < 0)
			delta = 0;

		जर (unlikely(delta > schedstat_val(stats->block_max)))
			__schedstat_set(stats->block_max, delta);

		__schedstat_set(stats->block_start, 0);
		__schedstat_add(stats->sum_sleep_runtime, delta);
		__schedstat_add(stats->sum_block_runtime, delta);

		जर (p) {
			जर (p->in_iowait) {
				__schedstat_add(stats->iowait_sum, delta);
				__schedstat_inc(stats->iowait_मोजणी);
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

स्थिर पूर्णांक show_schedstat(संरचना seq_file *seq, शून्य *v)
{
	पूर्णांक cpu;

	जर (v == (शून्य *)1) {
		seq_printf(seq, "version %d\n", SCHEDSTAT_VERSION);
		seq_printf(seq, "timestamp %lu\n", jiffies);
	} अन्यथा {
		संरचना rq *rq;
		संरचना sched_domain *sd;
		पूर्णांक dcount = 0;
		cpu = (अचिन्हित दीर्घ)(v - 2);
		rq = cpu_rq(cpu);

		/* runqueue-specific stats */
		seq_printf(seq,
		    "cpu%d %u 0 %u %u %u %u %llu %llu %lu",
		    cpu, rq->yld_मोजणी,
		    rq->sched_मोजणी, rq->sched_goidle,
		    rq->ttwu_मोजणी, rq->ttwu_local,
		    rq->rq_cpu_time,
		    rq->rq_sched_info.run_delay, rq->rq_sched_info.pcount);

		seq_printf(seq, "\n");

		/* domain-specific stats */
		rcu_read_lock();
		for_each_domain(cpu, sd) {
			गणना cpu_idle_type itype;

			seq_printf(seq, "domain%d %s %*pb", dcount++, sd->name,
				   cpumask_pr_args(sched_domain_span(sd)));
			साठी (itype = 0; itype < CPU_MAX_IDLE_TYPES; itype++) {
				seq_printf(seq, " %u %u %u %u %u %u %u %u %u %u %u",
				    sd->lb_मोजणी[itype],
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
			    sd->alb_मोजणी, sd->alb_failed, sd->alb_pushed,
			    sd->sbe_मोजणी, sd->sbe_balanced, sd->sbe_pushed,
			    sd->sbf_मोजणी, sd->sbf_balanced, sd->sbf_pushed,
			    sd->ttwu_wake_remote, sd->ttwu_move_affine,
			    sd->ttwu_move_balance);
		}
		rcu_read_unlock();
	}
	परत 0;
}

/*
 * This iterator needs some explanation.
 * It returns 1 for the header position.
 * This means 2 is cpu 0.
 * In a hotplugged system some CPUs, including cpu 0, may be missing so we have
 * to use cpumask_* to iterate over the CPUs.
 */
स्थिर शून्य *schedstat_start(संरचना seq_file *file, loff_t *offset)
{
	अचिन्हित दीर्घ n = *offset;

	जर (n == 0)
		परत (शून्य *) 1;

	n--;

	जर (n > 0)
		n = cpumask_next(n - 1, cpu_online_mask);
	अन्यथा
		n = cpumask_first(cpu_online_mask);

	*offset = n + 1;

	जर (n < nr_cpu_ids)
		परत (शून्य *)(अचिन्हित दीर्घ)(n + 2);

	परत रिक्त;
}

स्थिर शून्य *schedstat_next(संरचना seq_file *file, शून्य *data, loff_t *offset)
{
	(*offset)++;

	परत schedstat_start(file, offset);
}

स्थिर शून्य schedstat_stop(संरचना seq_file *file, शून्य *data)
{
}

स्थिर स्थिरांक संरचना seq_operations schedstat_sops = {
	.start = schedstat_start,
	.next  = schedstat_next,
	.stop  = schedstat_stop,
	.show  = show_schedstat,
};

स्थिर पूर्णांक __init proc_schedstat_init(शून्य)
{
	proc_create_seq("schedstat", 0, रिक्त, &schedstat_sops);
	परत 0;
}
subsys_initcall(proc_schedstat_init);
