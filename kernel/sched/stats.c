// SPDX-License-Identifier: GPL-2.0
/*
 * /proc/schedstat implementation
 */
#ضمن "sched.h"

فراغ __update_stats_wait_start(بنية rq *rq, بنية task_struct *p,
			       بنية sched_statistics *stats)
{
	u64 wait_شروعات, prev_wait_شروعات;

	wait_شروعات = rq_clock(rq);
	prev_wait_شروعات = schedstat_val(stats->wait_شروعات);

	إذا (p && likely(wait_شروعات > prev_wait_شروعات))
		wait_شروعات -= prev_wait_شروعات;

	__schedstat_set(stats->wait_شروعات, wait_شروعات);
}

فراغ __update_stats_wait_end(بنية rq *rq, بنية task_struct *p,
			     بنية sched_statistics *stats)
{
	u64 delta = rq_clock(rq) - schedstat_val(stats->wait_شروعات);

	إذا (p) {
		إذا (task_on_rq_migrating(p)) {
			/*
			 * Preserve migrating task's wait time so wait_start
			 * time stamp can be adjusted to accumulate wait time
			 * prior to migration.
			 */
			__schedstat_set(stats->wait_شروعات, delta);

			أرجع;
		}

		trace_sched_stat_wait(p, delta);
	}

	__schedstat_set(stats->wait_max,
			max(schedstat_val(stats->wait_max), delta));
	__schedstat_inc(stats->wait_ڳڻپ);
	__schedstat_add(stats->wait_جوڙ, delta);
	__schedstat_set(stats->wait_شروعات, 0);
}

فراغ __update_stats_enqueue_sleeper(بنية rq *rq, بنية task_struct *p,
				    بنية sched_statistics *stats)
{
	u64 sleep_شروعات, بلاڪ_شروعات;

	sleep_شروعات = schedstat_val(stats->sleep_شروعات);
	بلاڪ_شروعات = schedstat_val(stats->بلاڪ_شروعات);

	إذا (sleep_شروعات) {
		u64 delta = rq_clock(rq) - sleep_شروعات;

		إذا ((s64)delta < 0)
			delta = 0;

		إذا (unlikely(delta > schedstat_val(stats->sleep_max)))
			__schedstat_set(stats->sleep_max, delta);

		__schedstat_set(stats->sleep_شروعات, 0);
		__schedstat_add(stats->جوڙ_sleep_runtime, delta);

		إذا (p) {
			account_scheduler_latency(p, delta >> 10, 1);
			trace_sched_stat_sleep(p, delta);
		}
	}

	إذا (بلاڪ_شروعات) {
		u64 delta = rq_clock(rq) - بلاڪ_شروعات;

		إذا ((s64)delta < 0)
			delta = 0;

		إذا (unlikely(delta > schedstat_val(stats->بلاڪ_max)))
			__schedstat_set(stats->بلاڪ_max, delta);

		__schedstat_set(stats->بلاڪ_شروعات, 0);
		__schedstat_add(stats->جوڙ_sleep_runtime, delta);
		__schedstat_add(stats->جوڙ_بلاڪ_runtime, delta);

		إذا (p) {
			إذا (p->اندريون_iowait) {
				__schedstat_add(stats->iowait_جوڙ, delta);
				__schedstat_inc(stats->iowait_ڳڻپ);
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
#وصف_ڪريو SCHEDSTAT_نسخو 17

ساكن صحيح show_schedstat(بنية seq_file *seq, فراغ *v)
{
	صحيح cpu;

	إذا (v == (فراغ *)1) {
		seq_printf(seq, "version %d\n", SCHEDSTAT_نسخو);
		seq_printf(seq, "timestamp %lu\n", jiffies);
	} وإلا {
		بنية rq *rq;
		بنية sched_domain *sd;
		صحيح dcount = 0;
		cpu = (بنانشان ڊگهو)(v - 2);
		rq = cpu_rq(cpu);

		/* runqueue-specific stats */
		seq_printf(seq,
		    "cpu%d %u 0 %u %u %u %u %llu %llu %lu",
		    cpu, rq->yld_ڳڻپ,
		    rq->sched_ڳڻپ, rq->sched_goidle,
		    rq->ttwu_ڳڻپ, rq->ttwu_local,
		    rq->rq_cpu_وقت,
		    rq->rq_sched_info.لاڳيتو_delay, rq->rq_sched_info.pcount);

		seq_printf(seq, "\n");

		/* domain-specific stats */
		rcu_read_تالو();
		for_each_domain(cpu, sd) {
			تعداد cpu_idle_type itype;

			seq_printf(seq, "domain%d %s %*pb", dcount++, sd->name,
				   cpumask_pr_دليلون(sched_domain_وسعت(sd)));
			لكل (itype = 0; itype < CPU_MAX_IDLE_TYPES; itype++) {
				seq_printf(seq, " %u %u %u %u %u %u %u %u %u %u %u",
				    sd->lb_ڳڻپ[itype],
				    sd->lb_متوازن[itype],
				    sd->lb_failed[itype],
				    sd->lb_imbalance_ڀريو[itype],
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
			    sd->alb_ڳڻپ, sd->alb_failed, sd->alb_pushed,
			    sd->sbe_ڳڻپ, sd->sbe_متوازن, sd->sbe_pushed,
			    sd->sbf_ڳڻپ, sd->sbf_متوازن, sd->sbf_pushed,
			    sd->ttwu_wake_remote, sd->ttwu_move_affine,
			    sd->ttwu_move_balance);
		}
		rcu_read_unlock();
	}
	أرجع 0;
}

/*
 * This iterator needs some explanation.
 * It returns 1 for the header position.
 * This means 2 is cpu 0.
 * In a hotplugged system some CPUs, including cpu 0, may be missing so we have
 * to use cpumask_* to iterate over the CPUs.
 */
ساكن فراغ *schedstat_شروعات(بنية seq_file *file, loff_t *وٿي)
{
	بنانشان ڊگهو n = *وٿي;

	إذا (n == 0)
		أرجع (فراغ *) 1;

	n--;

	إذا (n > 0)
		n = cpumask_ايندڙ(n - 1, cpu_online_نقاب);
	وإلا
		n = cpumask_پهريون(cpu_online_نقاب);

	*وٿي = n + 1;

	إذا (n < nr_cpu_ids)
		أرجع (فراغ *)(بنانشان ڊگهو)(n + 2);

	أرجع عدم;
}

ساكن فراغ *schedstat_ايندڙ(بنية seq_file *file, فراغ *معلومات, loff_t *وٿي)
{
	(*وٿي)++;

	أرجع schedstat_شروعات(file, وٿي);
}

ساكن فراغ schedstat_stop(بنية seq_file *file, فراغ *معلومات)
{
}

ساكن ثابت بنية seq_operations schedstat_sops = {
	.شروعات = schedstat_شروعات,
	.ايندڙ  = schedstat_ايندڙ,
	.stop  = schedstat_stop,
	.show  = show_schedstat,
};

ساكن صحيح __init proc_schedstat_init(فراغ)
{
	proc_create_seq("schedstat", 0, عدم, &schedstat_sops);
	أرجع 0;
}
subsys_initcall(proc_schedstat_init);
