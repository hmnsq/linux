// SPDX-License-Identifier: GPL-2.0
/*
 * /proc/schedstat implementation
 */
#ضمن "sched.h"

فراغ __update_stats_wait_start(بنية rq *rq, بنية task_struct *p,
			       بنية sched_statistics *stats)
{
	u64 wait_پیل, prev_wait_پیل;

	wait_پیل = rq_clock(rq);
	prev_wait_پیل = schedstat_val(stats->wait_پیل);

	إذا (p && likely(wait_پیل > prev_wait_پیل))
		wait_پیل -= prev_wait_پیل;

	__schedstat_set(stats->wait_پیل, wait_پیل);
}

فراغ __update_stats_wait_end(بنية rq *rq, بنية task_struct *p,
			     بنية sched_statistics *stats)
{
	u64 delta = rq_clock(rq) - schedstat_val(stats->wait_پیل);

	إذا (p) {
		إذا (task_on_rq_migrating(p)) {
			/*
			 * Preserve migrating task's wait time so wait_start
			 * time stamp can be adjusted to accumulate wait time
			 * prior to migration.
			 */
			__schedstat_set(stats->wait_پیل, delta);

			أرجع;
		}

		trace_sched_stat_wait(p, delta);
	}

	__schedstat_set(stats->wait_max,
			max(schedstat_val(stats->wait_max), delta));
	__schedstat_inc(stats->wait_شمېر);
	__schedstat_add(stats->wait_مجموعه, delta);
	__schedstat_set(stats->wait_پیل, 0);
}

فراغ __update_stats_enqueue_sleeper(بنية rq *rq, بنية task_struct *p,
				    بنية sched_statistics *stats)
{
	u64 sleep_پیل, بلاک_پیل;

	sleep_پیل = schedstat_val(stats->sleep_پیل);
	بلاک_پیل = schedstat_val(stats->بلاک_پیل);

	إذا (sleep_پیل) {
		u64 delta = rq_clock(rq) - sleep_پیل;

		إذا ((s64)delta < 0)
			delta = 0;

		إذا (unlikely(delta > schedstat_val(stats->sleep_max)))
			__schedstat_set(stats->sleep_max, delta);

		__schedstat_set(stats->sleep_پیل, 0);
		__schedstat_add(stats->مجموعه_sleep_runtime, delta);

		إذا (p) {
			account_scheduler_latency(p, delta >> 10, 1);
			trace_sched_stat_sleep(p, delta);
		}
	}

	إذا (بلاک_پیل) {
		u64 delta = rq_clock(rq) - بلاک_پیل;

		إذا ((s64)delta < 0)
			delta = 0;

		إذا (unlikely(delta > schedstat_val(stats->بلاک_max)))
			__schedstat_set(stats->بلاک_max, delta);

		__schedstat_set(stats->بلاک_پیل, 0);
		__schedstat_add(stats->مجموعه_sleep_runtime, delta);
		__schedstat_add(stats->مجموعه_بلاک_runtime, delta);

		إذا (p) {
			إذا (p->دننه_iowait) {
				__schedstat_add(stats->iowait_مجموعه, delta);
				__schedstat_inc(stats->iowait_شمېر);
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
#تعریف_کړه SCHEDSTAT_نسخه 17

ساكن صحيح show_schedstat(بنية seq_file *seq, فراغ *v)
{
	صحيح cpu;

	إذا (v == (فراغ *)1) {
		seq_printf(seq, "version %d\n", SCHEDSTAT_نسخه);
		seq_printf(seq, "timestamp %lu\n", jiffies);
	} وإلا {
		بنية rq *rq;
		بنية sched_domain *sd;
		صحيح dcount = 0;
		cpu = (بینښه اوږد)(v - 2);
		rq = cpu_rq(cpu);

		/* runqueue-specific stats */
		seq_printf(seq,
		    "cpu%d %u 0 %u %u %u %u %llu %llu %lu",
		    cpu, rq->yld_شمېر,
		    rq->sched_شمېر, rq->sched_goidle,
		    rq->ttwu_شمېر, rq->ttwu_local,
		    rq->rq_cpu_وخت,
		    rq->rq_sched_info.پرلپسې_delay, rq->rq_sched_info.pcount);

		seq_printf(seq, "\n");

		/* domain-specific stats */
		rcu_read_قلف();
		for_each_domain(cpu, sd) {
			تعداد cpu_idle_type itype;

			seq_printf(seq, "domain%d %s %*pb", dcount++, sd->name,
				   cpumask_pr_دلیلونه(sched_domain_پراختیا(sd)));
			لكل (itype = 0; itype < CPU_MAX_IDLE_TYPES; itype++) {
				seq_printf(seq, " %u %u %u %u %u %u %u %u %u %u %u",
				    sd->lb_شمېر[itype],
				    sd->lb_متوازن[itype],
				    sd->lb_failed[itype],
				    sd->lb_imbalance_بارول[itype],
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
			    sd->alb_شمېر, sd->alb_failed, sd->alb_pushed,
			    sd->sbe_شمېر, sd->sbe_متوازن, sd->sbe_pushed,
			    sd->sbf_شمېر, sd->sbf_متوازن, sd->sbf_pushed,
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
ساكن فراغ *schedstat_پیل(بنية seq_file *file, loff_t *دننه_واټن)
{
	بینښه اوږد n = *دننه_واټن;

	إذا (n == 0)
		أرجع (فراغ *) 1;

	n--;

	إذا (n > 0)
		n = cpumask_بل(n - 1, cpu_online_ماسک);
	وإلا
		n = cpumask_لومړی(cpu_online_ماسک);

	*دننه_واټن = n + 1;

	إذا (n < nr_cpu_ids)
		أرجع (فراغ *)(بینښه اوږد)(n + 2);

	أرجع عدم;
}

ساكن فراغ *schedstat_بل(بنية seq_file *file, فراغ *معلومات, loff_t *دننه_واټن)
{
	(*دننه_واټن)++;

	أرجع schedstat_پیل(file, دننه_واټن);
}

ساكن فراغ schedstat_stop(بنية seq_file *file, فراغ *معلومات)
{
}

ساكن ثابت بنية seq_operations schedstat_sops = {
	.پیل = schedstat_پیل,
	.بل  = schedstat_بل,
	.stop  = schedstat_stop,
	.show  = show_schedstat,
};

ساكن صحيح __init proc_schedstat_init(فراغ)
{
	proc_create_seq("schedstat", 0, عدم, &schedstat_sops);
	أرجع 0;
}
subsys_initcall(proc_schedstat_init);
