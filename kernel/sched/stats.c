// SPDX-License-Identifier: GPL-2.0
/*
 * /proc/schedstat implementation
 */
#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"

ਖਾਲੀ __update_stats_wait_start(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p,
			       ਬਣਤਰ sched_statistics *stats)
{
	u64 wait_ਸ਼ੁਰੂ, prev_wait_ਸ਼ੁਰੂ;

	wait_ਸ਼ੁਰੂ = rq_clock(rq);
	prev_wait_ਸ਼ੁਰੂ = schedstat_val(stats->wait_ਸ਼ੁਰੂ);

	ਜੇ (p && likely(wait_ਸ਼ੁਰੂ > prev_wait_ਸ਼ੁਰੂ))
		wait_ਸ਼ੁਰੂ -= prev_wait_ਸ਼ੁਰੂ;

	__schedstat_set(stats->wait_ਸ਼ੁਰੂ, wait_ਸ਼ੁਰੂ);
}

ਖਾਲੀ __update_stats_wait_end(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p,
			     ਬਣਤਰ sched_statistics *stats)
{
	u64 delta = rq_clock(rq) - schedstat_val(stats->wait_ਸ਼ੁਰੂ);

	ਜੇ (p) {
		ਜੇ (task_on_rq_migrating(p)) {
			/*
			 * Preserve migrating task's wait time so wait_start
			 * time stamp can be adjusted to accumulate wait time
			 * prior to migration.
			 */
			__schedstat_set(stats->wait_ਸ਼ੁਰੂ, delta);

			ਵਾਪਸ;
		}

		trace_sched_stat_wait(p, delta);
	}

	__schedstat_set(stats->wait_max,
			max(schedstat_val(stats->wait_max), delta));
	__schedstat_inc(stats->wait_ਗਿਣਤੀ);
	__schedstat_add(stats->wait_ਜੋੜ, delta);
	__schedstat_set(stats->wait_ਸ਼ੁਰੂ, 0);
}

ਖਾਲੀ __update_stats_enqueue_sleeper(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p,
				    ਬਣਤਰ sched_statistics *stats)
{
	u64 sleep_ਸ਼ੁਰੂ, ਖੰਡ_ਸ਼ੁਰੂ;

	sleep_ਸ਼ੁਰੂ = schedstat_val(stats->sleep_ਸ਼ੁਰੂ);
	ਖੰਡ_ਸ਼ੁਰੂ = schedstat_val(stats->ਖੰਡ_ਸ਼ੁਰੂ);

	ਜੇ (sleep_ਸ਼ੁਰੂ) {
		u64 delta = rq_clock(rq) - sleep_ਸ਼ੁਰੂ;

		ਜੇ ((s64)delta < 0)
			delta = 0;

		ਜੇ (unlikely(delta > schedstat_val(stats->sleep_max)))
			__schedstat_set(stats->sleep_max, delta);

		__schedstat_set(stats->sleep_ਸ਼ੁਰੂ, 0);
		__schedstat_add(stats->ਜੋੜ_sleep_runtime, delta);

		ਜੇ (p) {
			account_scheduler_latency(p, delta >> 10, 1);
			trace_sched_stat_sleep(p, delta);
		}
	}

	ਜੇ (ਖੰਡ_ਸ਼ੁਰੂ) {
		u64 delta = rq_clock(rq) - ਖੰਡ_ਸ਼ੁਰੂ;

		ਜੇ ((s64)delta < 0)
			delta = 0;

		ਜੇ (unlikely(delta > schedstat_val(stats->ਖੰਡ_max)))
			__schedstat_set(stats->ਖੰਡ_max, delta);

		__schedstat_set(stats->ਖੰਡ_ਸ਼ੁਰੂ, 0);
		__schedstat_add(stats->ਜੋੜ_sleep_runtime, delta);
		__schedstat_add(stats->ਜੋੜ_ਖੰਡ_runtime, delta);

		ਜੇ (p) {
			ਜੇ (p->ਵਿੱਚ_iowait) {
				__schedstat_add(stats->iowait_ਜੋੜ, delta);
				__schedstat_inc(stats->iowait_ਗਿਣਤੀ);
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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCHEDSTAT_ਸੰਸਕਰਣ 17

ਸਥਿਰ ਪੂਰਨ show_schedstat(ਬਣਤਰ seq_file *seq, ਖਾਲੀ *v)
{
	ਪੂਰਨ cpu;

	ਜੇ (v == (ਖਾਲੀ *)1) {
		seq_printf(seq, "version %d\n", SCHEDSTAT_ਸੰਸਕਰਣ);
		seq_printf(seq, "timestamp %lu\n", jiffies);
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ rq *rq;
		ਬਣਤਰ sched_domain *sd;
		ਪੂਰਨ dcount = 0;
		cpu = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)(v - 2);
		rq = cpu_rq(cpu);

		/* runqueue-specific stats */
		seq_printf(seq,
		    "cpu%d %u 0 %u %u %u %u %llu %llu %lu",
		    cpu, rq->yld_ਗਿਣਤੀ,
		    rq->sched_ਗਿਣਤੀ, rq->sched_goidle,
		    rq->ttwu_ਗਿਣਤੀ, rq->ttwu_local,
		    rq->rq_cpu_ਸਮਾਂ,
		    rq->rq_sched_info.ਲੜੀਵਾਰ_delay, rq->rq_sched_info.pcount);

		seq_printf(seq, "\n");

		/* domain-specific stats */
		rcu_read_ਤਾਲਾ();
		for_each_domain(cpu, sd) {
			ਗਿਣਤੀ cpu_idle_type itype;

			seq_printf(seq, "domain%d %s %*pb", dcount++, sd->name,
				   cpumask_pr_ਦਲੀਲਾਂ(sched_domain_ਵਿਸਤਾਰ(sd)));
			ਲਈ (itype = 0; itype < CPU_MAX_IDLE_TYPES; itype++) {
				seq_printf(seq, " %u %u %u %u %u %u %u %u %u %u %u",
				    sd->lb_ਗਿਣਤੀ[itype],
				    sd->lb_ਸੰਤੁਲਿਤ[itype],
				    sd->lb_failed[itype],
				    sd->lb_imbalance_ਲੱਦੋ[itype],
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
			    sd->alb_ਗਿਣਤੀ, sd->alb_failed, sd->alb_pushed,
			    sd->sbe_ਗਿਣਤੀ, sd->sbe_ਸੰਤੁਲਿਤ, sd->sbe_pushed,
			    sd->sbf_ਗਿਣਤੀ, sd->sbf_ਸੰਤੁਲਿਤ, sd->sbf_pushed,
			    sd->ttwu_wake_remote, sd->ttwu_move_affine,
			    sd->ttwu_move_balance);
		}
		rcu_read_unlock();
	}
	ਵਾਪਸ 0;
}

/*
 * This iterator needs some explanation.
 * It returns 1 for the header position.
 * This means 2 is cpu 0.
 * In a hotplugged system some CPUs, including cpu 0, may be missing so we have
 * to use cpumask_* to iterate over the CPUs.
 */
ਸਥਿਰ ਖਾਲੀ *schedstat_ਸ਼ੁਰੂ(ਬਣਤਰ seq_file *file, loff_t *ਵਿੱਥ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ n = *ਵਿੱਥ;

	ਜੇ (n == 0)
		ਵਾਪਸ (ਖਾਲੀ *) 1;

	n--;

	ਜੇ (n > 0)
		n = cpumask_ਅਗਲਾ(n - 1, cpu_online_ਨਕਾਬ);
	ਨਹੀਂਤਾਂ
		n = cpumask_ਪਹਿਲਾ(cpu_online_ਨਕਾਬ);

	*ਵਿੱਥ = n + 1;

	ਜੇ (n < nr_cpu_ids)
		ਵਾਪਸ (ਖਾਲੀ *)(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)(n + 2);

	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ *schedstat_ਅਗਲਾ(ਬਣਤਰ seq_file *file, ਖਾਲੀ *ਅੰਕੜੇ, loff_t *ਵਿੱਥ)
{
	(*ਵਿੱਥ)++;

	ਵਾਪਸ schedstat_ਸ਼ੁਰੂ(file, ਵਿੱਥ);
}

ਸਥਿਰ ਖਾਲੀ schedstat_stop(ਬਣਤਰ seq_file *file, ਖਾਲੀ *ਅੰਕੜੇ)
{
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ seq_operations schedstat_sops = {
	.ਸ਼ੁਰੂ = schedstat_ਸ਼ੁਰੂ,
	.ਅਗਲਾ  = schedstat_ਅਗਲਾ,
	.stop  = schedstat_stop,
	.show  = show_schedstat,
};

ਸਥਿਰ ਪੂਰਨ __init proc_schedstat_init(ਖਾਲੀ)
{
	proc_create_seq("schedstat", 0, ਨਿਰਾ, &schedstat_sops);
	ਵਾਪਸ 0;
}
subsys_initcall(proc_schedstat_init);
