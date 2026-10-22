// SPDX-License-Identifier: GPL-2.0
/*
 * /proc/schedstat implementation
 */
#شامل "sched.h"

خالی __update_stats_wait_start(ساخت rq *rq, ساخت task_struct *p,
			       ساخت sched_statistics *stats)
{
	u64 wait_آغاز, پچھلا_wait_آغاز;

	wait_آغاز = rq_clock(rq);
	پچھلا_wait_آغاز = schedstat_val(stats->wait_آغاز);

	اگر (p && likely(wait_آغاز > پچھلا_wait_آغاز))
		wait_آغاز -= پچھلا_wait_آغاز;

	__schedstat_set(stats->wait_آغاز, wait_آغاز);
}

خالی __update_stats_wait_end(ساخت rq *rq, ساخت task_struct *p,
			     ساخت sched_statistics *stats)
{
	u64 delta = rq_clock(rq) - schedstat_val(stats->wait_آغاز);

	اگر (p) {
		اگر (task_on_rq_migrating(p)) {
			/*
			 * Preserve migrating task's wait time so wait_start
			 * time stamp can be adjusted to accumulate wait time
			 * prior to migration.
			 */
			__schedstat_set(stats->wait_آغاز, delta);

			واپس;
		}

		trace_sched_stat_wait(p, delta);
	}

	__schedstat_set(stats->wait_max,
			max(schedstat_val(stats->wait_max), delta));
	__schedstat_inc(stats->wait_گنتی);
	__schedstat_add(stats->wait_مجموع, delta);
	__schedstat_set(stats->wait_آغاز, 0);
}

خالی __update_stats_enqueue_sleeper(ساخت rq *rq, ساخت task_struct *p,
				    ساخت sched_statistics *stats)
{
	u64 sleep_آغاز, بلاک_آغاز;

	sleep_آغاز = schedstat_val(stats->sleep_آغاز);
	بلاک_آغاز = schedstat_val(stats->بلاک_آغاز);

	اگر (sleep_آغاز) {
		u64 delta = rq_clock(rq) - sleep_آغاز;

		اگر ((s64)delta < 0)
			delta = 0;

		اگر (unlikely(delta > schedstat_val(stats->sleep_max)))
			__schedstat_set(stats->sleep_max, delta);

		__schedstat_set(stats->sleep_آغاز, 0);
		__schedstat_add(stats->مجموع_sleep_runtime, delta);

		اگر (p) {
			account_scheduler_latency(p, delta >> 10, 1);
			trace_sched_stat_sleep(p, delta);
		}
	}

	اگر (بلاک_آغاز) {
		u64 delta = rq_clock(rq) - بلاک_آغاز;

		اگر ((s64)delta < 0)
			delta = 0;

		اگر (unlikely(delta > schedstat_val(stats->بلاک_max)))
			__schedstat_set(stats->بلاک_max, delta);

		__schedstat_set(stats->بلاک_آغاز, 0);
		__schedstat_add(stats->مجموع_sleep_runtime, delta);
		__schedstat_add(stats->مجموع_بلاک_runtime, delta);

		اگر (p) {
			اگر (p->اندر_iowait) {
				__schedstat_add(stats->iowait_مجموع, delta);
				__schedstat_inc(stats->iowait_گنتی);
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
#تعریف_کرو SCHEDSTAT_نسخہ 17

ساکن صحیح show_schedstat(ساخت seq_file *seq, خالی *v)
{
	صحیح cpu;

	اگر (v == (خالی *)1) {
		seq_printf(seq, "version %d\n", SCHEDSTAT_نسخہ);
		seq_printf(seq, "timestamp %lu\n", jiffies);
	} ورنہ {
		ساخت rq *rq;
		ساخت sched_domain *sd;
		صحیح dcount = 0;
		cpu = (غیرعلامتی طویل)(v - 2);
		rq = cpu_rq(cpu);

		/* runqueue-specific stats */
		seq_printf(seq,
		    "cpu%d %u 0 %u %u %u %u %llu %llu %lu",
		    cpu, rq->yld_گنتی,
		    rq->sched_گنتی, rq->sched_goidle,
		    rq->ttwu_گنتی, rq->ttwu_local,
		    rq->rq_cpu_وقت,
		    rq->rq_sched_info.تسلسل_delay, rq->rq_sched_info.pcount);

		seq_printf(seq, "\n");

		/* domain-specific stats */
		rcu_read_تالا();
		for_each_domain(cpu, sd) {
			شمار cpu_idle_type itype;

			seq_printf(seq, "domain%d %s %*pb", dcount++, sd->name,
				   cpumask_pr_دلائل(sched_domain_وسعت(sd)));
			برائے (itype = 0; itype < CPU_MAX_IDLE_TYPES; itype++) {
				seq_printf(seq, " %u %u %u %u %u %u %u %u %u %u %u",
				    sd->lb_گنتی[itype],
				    sd->lb_متوازن[itype],
				    sd->lb_failed[itype],
				    sd->lb_imbalance_بھرو[itype],
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
			    sd->alb_گنتی, sd->alb_failed, sd->alb_pushed,
			    sd->sbe_گنتی, sd->sbe_متوازن, sd->sbe_pushed,
			    sd->sbf_گنتی, sd->sbf_متوازن, sd->sbf_pushed,
			    sd->ttwu_wake_remote, sd->ttwu_move_affine,
			    sd->ttwu_move_balance);
		}
		rcu_read_unlock();
	}
	واپس 0;
}

/*
 * This iterator needs some explanation.
 * It returns 1 for the header position.
 * This means 2 is cpu 0.
 * In a hotplugged system some CPUs, including cpu 0, may be missing so we have
 * to use cpumask_* to iterate over the CPUs.
 */
ساکن خالی *schedstat_آغاز(ساخت seq_file *file, loff_t *انحراف)
{
	غیرعلامتی طویل n = *انحراف;

	اگر (n == 0)
		واپس (خالی *) 1;

	n--;

	اگر (n > 0)
		n = cpumask_اگلا(n - 1, cpu_online_نقاب);
	ورنہ
		n = cpumask_پہلا(cpu_online_نقاب);

	*انحراف = n + 1;

	اگر (n < nr_cpu_ids)
		واپس (خالی *)(غیرعلامتی طویل)(n + 2);

	واپس تہی;
}

ساکن خالی *schedstat_اگلا(ساخت seq_file *file, خالی *ڈیٹا, loff_t *انحراف)
{
	(*انحراف)++;

	واپس schedstat_آغاز(file, انحراف);
}

ساکن خالی schedstat_stop(ساخت seq_file *file, خالی *ڈیٹا)
{
}

ساکن ثابت ساخت seq_operations schedstat_sops = {
	.آغاز = schedstat_آغاز,
	.اگلا  = schedstat_اگلا,
	.stop  = schedstat_stop,
	.show  = show_schedstat,
};

ساکن صحیح __init proc_schedstat_init(خالی)
{
	proc_create_seq("schedstat", 0, تہی, &schedstat_sops);
	واپس 0;
}
subsys_initcall(proc_schedstat_init);
