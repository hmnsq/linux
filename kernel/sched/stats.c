// SPDX-License-Identifier: GPL-2.0
/*
 * /proc/schedstat implementation
 */
#समावेश "sched.h"

व्योम __update_stats_wait_start(काष्ठा rq *rq, काष्ठा task_struct *p,
			       काष्ठा sched_statistics *stats)
{
	u64 wait_start, पूर्व_wait_start;

	wait_start = rq_clock(rq);
	पूर्व_wait_start = schedstat_val(stats->wait_start);

	अगर (p && likely(wait_start > पूर्व_wait_start))
		wait_start -= पूर्व_wait_start;

	__schedstat_set(stats->wait_start, wait_start);
}

व्योम __update_stats_wait_end(काष्ठा rq *rq, काष्ठा task_struct *p,
			     काष्ठा sched_statistics *stats)
{
	u64 delta = rq_clock(rq) - schedstat_val(stats->wait_start);

	अगर (p) {
		अगर (task_on_rq_migrating(p)) {
			/*
			 * Preserve migrating task's wait time so wait_start
			 * time stamp can be adjusted to accumulate wait time
			 * prior to migration.
			 */
			__schedstat_set(stats->wait_start, delta);

			वापस;
		}

		trace_sched_stat_wait(p, delta);
	}

	__schedstat_set(stats->wait_max,
			max(schedstat_val(stats->wait_max), delta));
	__schedstat_inc(stats->wait_गिनती);
	__schedstat_add(stats->wait_योग, delta);
	__schedstat_set(stats->wait_start, 0);
}

व्योम __update_stats_enqueue_sleeper(काष्ठा rq *rq, काष्ठा task_struct *p,
				    काष्ठा sched_statistics *stats)
{
	u64 sleep_start, block_start;

	sleep_start = schedstat_val(stats->sleep_start);
	block_start = schedstat_val(stats->block_start);

	अगर (sleep_start) {
		u64 delta = rq_clock(rq) - sleep_start;

		अगर ((s64)delta < 0)
			delta = 0;

		अगर (unlikely(delta > schedstat_val(stats->sleep_max)))
			__schedstat_set(stats->sleep_max, delta);

		__schedstat_set(stats->sleep_start, 0);
		__schedstat_add(stats->योग_sleep_runtime, delta);

		अगर (p) {
			account_scheduler_latency(p, delta >> 10, 1);
			trace_sched_stat_sleep(p, delta);
		}
	}

	अगर (block_start) {
		u64 delta = rq_clock(rq) - block_start;

		अगर ((s64)delta < 0)
			delta = 0;

		अगर (unlikely(delta > schedstat_val(stats->block_max)))
			__schedstat_set(stats->block_max, delta);

		__schedstat_set(stats->block_start, 0);
		__schedstat_add(stats->योग_sleep_runtime, delta);
		__schedstat_add(stats->योग_block_runtime, delta);

		अगर (p) {
			अगर (p->in_iowait) {
				__schedstat_add(stats->iowait_योग, delta);
				__schedstat_inc(stats->iowait_गिनती);
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
#घोषणा SCHEDSTAT_VERSION 17

अटल पूर्णांक show_schedstat(काष्ठा seq_file *seq, व्योम *v)
{
	पूर्णांक cpu;

	अगर (v == (व्योम *)1) {
		seq_printf(seq, "version %d\n", SCHEDSTAT_VERSION);
		seq_printf(seq, "timestamp %lu\n", jiffies);
	} अन्यथा {
		काष्ठा rq *rq;
		काष्ठा sched_domain *sd;
		पूर्णांक dcount = 0;
		cpu = (अचिन्हित दीर्घ)(v - 2);
		rq = cpu_rq(cpu);

		/* runqueue-specific stats */
		seq_printf(seq,
		    "cpu%d %u 0 %u %u %u %u %llu %llu %lu",
		    cpu, rq->yld_गिनती,
		    rq->sched_गिनती, rq->sched_goidle,
		    rq->ttwu_गिनती, rq->ttwu_local,
		    rq->rq_cpu_time,
		    rq->rq_sched_info.run_delay, rq->rq_sched_info.pcount);

		seq_printf(seq, "\n");

		/* domain-specific stats */
		rcu_read_lock();
		for_each_domain(cpu, sd) {
			क्रमागत cpu_idle_type itype;

			seq_printf(seq, "domain%d %s %*pb", dcount++, sd->name,
				   cpumask_pr_args(sched_domain_span(sd)));
			क्रम (itype = 0; itype < CPU_MAX_IDLE_TYPES; itype++) {
				seq_printf(seq, " %u %u %u %u %u %u %u %u %u %u %u",
				    sd->lb_गिनती[itype],
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
			    sd->alb_गिनती, sd->alb_failed, sd->alb_pushed,
			    sd->sbe_गिनती, sd->sbe_balanced, sd->sbe_pushed,
			    sd->sbf_गिनती, sd->sbf_balanced, sd->sbf_pushed,
			    sd->ttwu_wake_remote, sd->ttwu_move_affine,
			    sd->ttwu_move_balance);
		}
		rcu_read_unlock();
	}
	वापस 0;
}

/*
 * This iterator needs some explanation.
 * It returns 1 for the header position.
 * This means 2 is cpu 0.
 * In a hotplugged system some CPUs, including cpu 0, may be missing so we have
 * to use cpumask_* to iterate over the CPUs.
 */
अटल व्योम *schedstat_start(काष्ठा seq_file *file, loff_t *offset)
{
	अचिन्हित दीर्घ n = *offset;

	अगर (n == 0)
		वापस (व्योम *) 1;

	n--;

	अगर (n > 0)
		n = cpumask_अग्र(n - 1, cpu_online_mask);
	अन्यथा
		n = cpumask_first(cpu_online_mask);

	*offset = n + 1;

	अगर (n < nr_cpu_ids)
		वापस (व्योम *)(अचिन्हित दीर्घ)(n + 2);

	वापस शून्य;
}

अटल व्योम *schedstat_अग्र(काष्ठा seq_file *file, व्योम *तथ्य, loff_t *offset)
{
	(*offset)++;

	वापस schedstat_start(file, offset);
}

अटल व्योम schedstat_stop(काष्ठा seq_file *file, व्योम *तथ्य)
{
}

अटल स्थिर काष्ठा seq_operations schedstat_sops = {
	.start = schedstat_start,
	.अग्र  = schedstat_अग्र,
	.stop  = schedstat_stop,
	.show  = show_schedstat,
};

अटल पूर्णांक __init proc_schedstat_init(व्योम)
{
	proc_create_seq("schedstat", 0, शून्य, &schedstat_sops);
	वापस 0;
}
subsys_initcall(proc_schedstat_init);
