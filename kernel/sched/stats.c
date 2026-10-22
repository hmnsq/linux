// SPDX-License-Identifier: GPL-2.0
/*
 * /proc/schedstat implementation
 */
#ಅನ್ತರ್ಭಾವ "sched.h"

ಶೂನ್ಯ __update_stats_wait_start(ರಚನೆ rq *rq, ರಚನೆ task_struct *p,
			       ರಚನೆ sched_statistics *stats)
{
	u64 wait_ಆರಂಭ, prev_wait_ಆರಂಭ;

	wait_ಆರಂಭ = rq_clock(rq);
	prev_wait_ಆರಂಭ = schedstat_val(stats->wait_ಆರಂಭ);

	ಆದರೆ (p && likely(wait_ಆರಂಭ > prev_wait_ಆರಂಭ))
		wait_ಆರಂಭ -= prev_wait_ಆರಂಭ;

	__schedstat_set(stats->wait_ಆರಂಭ, wait_ಆರಂಭ);
}

ಶೂನ್ಯ __update_stats_wait_end(ರಚನೆ rq *rq, ರಚನೆ task_struct *p,
			     ರಚನೆ sched_statistics *stats)
{
	u64 delta = rq_clock(rq) - schedstat_val(stats->wait_ಆರಂಭ);

	ಆದರೆ (p) {
		ಆದರೆ (task_on_rq_migrating(p)) {
			/*
			 * Preserve migrating task's wait time so wait_start
			 * time stamp can be adjusted to accumulate wait time
			 * prior to migration.
			 */
			__schedstat_set(stats->wait_ಆರಂಭ, delta);

			ಹಿಂತಿರುಗಿ;
		}

		trace_sched_stat_wait(p, delta);
	}

	__schedstat_set(stats->wait_max,
			max(schedstat_val(stats->wait_max), delta));
	__schedstat_inc(stats->wait_ಎಣಿಕೆ);
	__schedstat_add(stats->wait_ಮೊತ್ತ, delta);
	__schedstat_set(stats->wait_ಆರಂಭ, 0);
}

ಶೂನ್ಯ __update_stats_enqueue_sleeper(ರಚನೆ rq *rq, ರಚನೆ task_struct *p,
				    ರಚನೆ sched_statistics *stats)
{
	u64 sleep_ಆರಂಭ, ಖಂಡ_ಆರಂಭ;

	sleep_ಆರಂಭ = schedstat_val(stats->sleep_ಆರಂಭ);
	ಖಂಡ_ಆರಂಭ = schedstat_val(stats->ಖಂಡ_ಆರಂಭ);

	ಆದರೆ (sleep_ಆರಂಭ) {
		u64 delta = rq_clock(rq) - sleep_ಆರಂಭ;

		ಆದರೆ ((s64)delta < 0)
			delta = 0;

		ಆದರೆ (unlikely(delta > schedstat_val(stats->sleep_max)))
			__schedstat_set(stats->sleep_max, delta);

		__schedstat_set(stats->sleep_ಆರಂಭ, 0);
		__schedstat_add(stats->ಮೊತ್ತ_sleep_runtime, delta);

		ಆದರೆ (p) {
			account_scheduler_latency(p, delta >> 10, 1);
			trace_sched_stat_sleep(p, delta);
		}
	}

	ಆದರೆ (ಖಂಡ_ಆರಂಭ) {
		u64 delta = rq_clock(rq) - ಖಂಡ_ಆರಂಭ;

		ಆದರೆ ((s64)delta < 0)
			delta = 0;

		ಆದರೆ (unlikely(delta > schedstat_val(stats->ಖಂಡ_max)))
			__schedstat_set(stats->ಖಂಡ_max, delta);

		__schedstat_set(stats->ಖಂಡ_ಆರಂಭ, 0);
		__schedstat_add(stats->ಮೊತ್ತ_sleep_runtime, delta);
		__schedstat_add(stats->ಮೊತ್ತ_ಖಂಡ_runtime, delta);

		ಆದರೆ (p) {
			ಆದರೆ (p->ಒಳ_iowait) {
				__schedstat_add(stats->iowait_ಮೊತ್ತ, delta);
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
#ವ್ಯಾಖ್ಯಾನಿಸು SCHEDSTAT_ಆವೃತ್ತಿ 17

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_schedstat(ರಚನೆ seq_file *seq, ಶೂನ್ಯ *v)
{
	ಪೂರ್ಣಾಂಕ cpu;

	ಆದರೆ (v == (ಶೂನ್ಯ *)1) {
		seq_printf(seq, "version %d\n", SCHEDSTAT_ಆವೃತ್ತಿ);
		seq_printf(seq, "timestamp %lu\n", jiffies);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ rq *rq;
		ರಚನೆ sched_domain *sd;
		ಪೂರ್ಣಾಂಕ dcount = 0;
		cpu = (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)(v - 2);
		rq = cpu_rq(cpu);

		/* runqueue-specific stats */
		seq_printf(seq,
		    "cpu%d %u 0 %u %u %u %u %llu %llu %lu",
		    cpu, rq->yld_ಎಣಿಕೆ,
		    rq->sched_ಎಣಿಕೆ, rq->sched_goidle,
		    rq->ttwu_ಎಣಿಕೆ, rq->ttwu_local,
		    rq->rq_cpu_ಸಮಯ,
		    rq->rq_sched_info.ಸತತ_delay, rq->rq_sched_info.pcount);

		seq_printf(seq, "\n");

		/* domain-specific stats */
		rcu_read_ಬೀಗ();
		for_each_domain(cpu, sd) {
			ಎಣಿಕೆ cpu_idle_type itype;

			seq_printf(seq, "domain%d %s %*pb", dcount++, sd->name,
				   cpumask_pr_ವಾದಗಳು(sched_domain_ಹರವು(sd)));
			ಗಾಗಿ (itype = 0; itype < CPU_MAX_IDLE_TYPES; itype++) {
				seq_printf(seq, " %u %u %u %u %u %u %u %u %u %u %u",
				    sd->lb_ಎಣಿಕೆ[itype],
				    sd->lb_ಸಮತೋಲಿತ[itype],
				    sd->lb_failed[itype],
				    sd->lb_imbalance_ತುಂಬು[itype],
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
			    sd->sbe_ಎಣಿಕೆ, sd->sbe_ಸಮತೋಲಿತ, sd->sbe_pushed,
			    sd->sbf_ಎಣಿಕೆ, sd->sbf_ಸಮತೋಲಿತ, sd->sbf_pushed,
			    sd->ttwu_wake_remote, sd->ttwu_move_affine,
			    sd->ttwu_move_balance);
		}
		rcu_read_unlock();
	}
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * This iterator needs some explanation.
 * It returns 1 for the header position.
 * This means 2 is cpu 0.
 * In a hotplugged system some CPUs, including cpu 0, may be missing so we have
 * to use cpumask_* to iterate over the CPUs.
 */
ಸ್ಥಿರ ಶೂನ್ಯ *schedstat_ಆರಂಭ(ರಚನೆ seq_file *file, loff_t *ಸ್ಥಾನಾಂತರ)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ n = *ಸ್ಥಾನಾಂತರ;

	ಆದರೆ (n == 0)
		ಹಿಂತಿರುಗಿ (ಶೂನ್ಯ *) 1;

	n--;

	ಆದರೆ (n > 0)
		n = cpumask_ಮುಂದಿನ(n - 1, cpu_online_ಮುಖವಾಡ);
	ಇಲ್ಲದಿದ್ದರೆ
		n = cpumask_ಮೊದಲ(cpu_online_ಮುಖವಾಡ);

	*ಸ್ಥಾನಾಂತರ = n + 1;

	ಆದರೆ (n < nr_cpu_ids)
		ಹಿಂತಿರುಗಿ (ಶೂನ್ಯ *)(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)(n + 2);

	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಶೂನ್ಯ *schedstat_ಮುಂದಿನ(ರಚನೆ seq_file *file, ಶೂನ್ಯ *ದತ್ತಾಂಶ, loff_t *ಸ್ಥಾನಾಂತರ)
{
	(*ಸ್ಥಾನಾಂತರ)++;

	ಹಿಂತಿರುಗಿ schedstat_ಆರಂಭ(file, ಸ್ಥಾನಾಂತರ);
}

ಸ್ಥಿರ ಶೂನ್ಯ schedstat_stop(ರಚನೆ seq_file *file, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ seq_operations schedstat_sops = {
	.ಆರಂಭ = schedstat_ಆರಂಭ,
	.ಮುಂದಿನ  = schedstat_ಮುಂದಿನ,
	.stop  = schedstat_stop,
	.show  = show_schedstat,
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init proc_schedstat_init(ಶೂನ್ಯ)
{
	proc_create_seq("schedstat", 0, ಖಾಲಿ, &schedstat_sops);
	ಹಿಂತಿರುಗಿ 0;
}
subsys_initcall(proc_schedstat_init);
