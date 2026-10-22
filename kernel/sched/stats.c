// SPDX-License-Identifier: GPL-2.0
/*
 * /proc/schedstat implementation
 */
#ضمن "sched.h"

فراغ __update_stats_wait_start(بنية rq *rq, بنية task_struct *p,
			       بنية sched_statistics *stats)
{
	u64 wait_شروع, prev_wait_شروع;

	wait_شروع = rq_clock(rq);
	prev_wait_شروع = schedstat_val(stats->wait_شروع);

	إذا (p && likely(wait_شروع > prev_wait_شروع))
		wait_شروع -= prev_wait_شروع;

	__schedstat_set(stats->wait_شروع, wait_شروع);
}

فراغ __update_stats_wait_end(بنية rq *rq, بنية task_struct *p,
			     بنية sched_statistics *stats)
{
	u64 delta = rq_clock(rq) - schedstat_val(stats->wait_شروع);

	إذا (p) {
		إذا (task_on_rq_migrating(p)) {
			/*
			 * Preserve migrating task's wait time so wait_start
			 * time stamp can be adjusted to accumulate wait time
			 * prior to migration.
			 */
			__schedstat_set(stats->wait_شروع, delta);

			أرجع;
		}

		trace_sched_stat_wait(p, delta);
	}

	__schedstat_set(stats->wait_max,
			max(schedstat_val(stats->wait_max), delta));
	__schedstat_inc(stats->wait_شمار);
	__schedstat_add(stats->wait_جمع, delta);
	__schedstat_set(stats->wait_شروع, 0);
}

فراغ __update_stats_enqueue_sleeper(بنية rq *rq, بنية task_struct *p,
				    بنية sched_statistics *stats)
{
	u64 sleep_شروع, بلوک_شروع;

	sleep_شروع = schedstat_val(stats->sleep_شروع);
	بلوک_شروع = schedstat_val(stats->بلوک_شروع);

	إذا (sleep_شروع) {
		u64 delta = rq_clock(rq) - sleep_شروع;

		إذا ((s64)delta < 0)
			delta = 0;

		إذا (unlikely(delta > schedstat_val(stats->sleep_max)))
			__schedstat_set(stats->sleep_max, delta);

		__schedstat_set(stats->sleep_شروع, 0);
		__schedstat_add(stats->جمع_sleep_runtime, delta);

		إذا (p) {
			account_scheduler_latency(p, delta >> 10, 1);
			trace_sched_stat_sleep(p, delta);
		}
	}

	إذا (بلوک_شروع) {
		u64 delta = rq_clock(rq) - بلوک_شروع;

		إذا ((s64)delta < 0)
			delta = 0;

		إذا (unlikely(delta > schedstat_val(stats->بلوک_max)))
			__schedstat_set(stats->بلوک_max, delta);

		__schedstat_set(stats->بلوک_شروع, 0);
		__schedstat_add(stats->جمع_sleep_runtime, delta);
		__schedstat_add(stats->جمع_بلوک_runtime, delta);

		إذا (p) {
			إذا (p->درون_iowait) {
				__schedstat_add(stats->iowait_جمع, delta);
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
#تعریف_کن SCHEDSTAT_نسخه 17

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
		cpu = (بیعلامت دراز)(v - 2);
		rq = cpu_rq(cpu);

		/* runqueue-specific stats */
		seq_printf(seq,
		    "cpu%d %u 0 %u %u %u %u %llu %llu %lu",
		    cpu, rq->yld_شمار,
		    rq->sched_شمار, rq->sched_goidle,
		    rq->ttwu_شمار, rq->ttwu_local,
		    rq->rq_cpu_زمان,
		    rq->rq_sched_info.پیاپی_delay, rq->rq_sched_info.pcount);

		seq_printf(seq, "\n");

		/* domain-specific stats */
		rcu_read_قفل();
		for_each_domain(cpu, sd) {
			تعداد cpu_idle_type itype;

			seq_printf(seq, "domain%d %s %*pb", dcount++, sd->name,
				   cpumask_pr_آرگومانها(sched_domain_بازه(sd)));
			لكل (itype = 0; itype < CPU_MAX_IDLE_TYPES; itype++) {
				seq_printf(seq, " %u %u %u %u %u %u %u %u %u %u %u",
				    sd->lb_شمار[itype],
				    sd->lb_متوازن[itype],
				    sd->lb_failed[itype],
				    sd->lb_imbalance_بارگیری[itype],
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
			    sd->sbe_شمار, sd->sbe_متوازن, sd->sbe_pushed,
			    sd->sbf_شمار, sd->sbf_متوازن, sd->sbf_pushed,
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
ساكن فراغ *schedstat_شروع(بنية seq_file *file, loff_t *جابه_جایی)
{
	بیعلامت دراز n = *جابه_جایی;

	إذا (n == 0)
		أرجع (فراغ *) 1;

	n--;

	إذا (n > 0)
		n = cpumask_بعدی(n - 1, cpu_online_ماسک);
	وإلا
		n = cpumask_اولی(cpu_online_ماسک);

	*جابه_جایی = n + 1;

	إذا (n < nr_cpu_ids)
		أرجع (فراغ *)(بیعلامت دراز)(n + 2);

	أرجع عدم;
}

ساكن فراغ *schedstat_بعدی(بنية seq_file *file, فراغ *داده, loff_t *جابه_جایی)
{
	(*جابه_جایی)++;

	أرجع schedstat_شروع(file, جابه_جایی);
}

ساكن فراغ schedstat_stop(بنية seq_file *file, فراغ *داده)
{
}

ساكن ثابت بنية seq_operations schedstat_sops = {
	.شروع = schedstat_شروع,
	.بعدی  = schedstat_بعدی,
	.stop  = schedstat_stop,
	.show  = show_schedstat,
};

ساكن صحيح __init proc_schedstat_init(فراغ)
{
	proc_create_seq("schedstat", 0, عدم, &schedstat_sops);
	أرجع 0;
}
subsys_initcall(proc_schedstat_init);
