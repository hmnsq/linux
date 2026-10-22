// SPDX-License-Identifier: GPL-2.0
/*
 * /proc/schedstat implementation
 */
#అన్తర్భావ "sched.h"

శూన్య __update_stats_wait_start(నిర్మాణం rq *rq, నిర్మాణం task_struct *p,
			       నిర్మాణం sched_statistics *stats)
{
	u64 wait_ప్రారంభం, prev_wait_ప్రారంభం;

	wait_ప్రారంభం = rq_clock(rq);
	prev_wait_ప్రారంభం = schedstat_val(stats->wait_ప్రారంభం);

	ఒకవేళ (p && likely(wait_ప్రారంభం > prev_wait_ప్రారంభం))
		wait_ప్రారంభం -= prev_wait_ప్రారంభం;

	__schedstat_set(stats->wait_ప్రారంభం, wait_ప్రారంభం);
}

శూన్య __update_stats_wait_end(నిర్మాణం rq *rq, నిర్మాణం task_struct *p,
			     నిర్మాణం sched_statistics *stats)
{
	u64 delta = rq_clock(rq) - schedstat_val(stats->wait_ప్రారంభం);

	ఒకవేళ (p) {
		ఒకవేళ (task_on_rq_migrating(p)) {
			/*
			 * Preserve migrating task's wait time so wait_start
			 * time stamp can be adjusted to accumulate wait time
			 * prior to migration.
			 */
			__schedstat_set(stats->wait_ప్రారంభం, delta);

			తిరిగి;
		}

		trace_sched_stat_wait(p, delta);
	}

	__schedstat_set(stats->wait_max,
			max(schedstat_val(stats->wait_max), delta));
	__schedstat_inc(stats->wait_లెక్క);
	__schedstat_add(stats->wait_కూడిక, delta);
	__schedstat_set(stats->wait_ప్రారంభం, 0);
}

శూన్య __update_stats_enqueue_sleeper(నిర్మాణం rq *rq, నిర్మాణం task_struct *p,
				    నిర్మాణం sched_statistics *stats)
{
	u64 sleep_ప్రారంభం, ఖండం_ప్రారంభం;

	sleep_ప్రారంభం = schedstat_val(stats->sleep_ప్రారంభం);
	ఖండం_ప్రారంభం = schedstat_val(stats->ఖండం_ప్రారంభం);

	ఒకవేళ (sleep_ప్రారంభం) {
		u64 delta = rq_clock(rq) - sleep_ప్రారంభం;

		ఒకవేళ ((s64)delta < 0)
			delta = 0;

		ఒకవేళ (unlikely(delta > schedstat_val(stats->sleep_max)))
			__schedstat_set(stats->sleep_max, delta);

		__schedstat_set(stats->sleep_ప్రారంభం, 0);
		__schedstat_add(stats->కూడిక_sleep_runtime, delta);

		ఒకవేళ (p) {
			account_scheduler_latency(p, delta >> 10, 1);
			trace_sched_stat_sleep(p, delta);
		}
	}

	ఒకవేళ (ఖండం_ప్రారంభం) {
		u64 delta = rq_clock(rq) - ఖండం_ప్రారంభం;

		ఒకవేళ ((s64)delta < 0)
			delta = 0;

		ఒకవేళ (unlikely(delta > schedstat_val(stats->ఖండం_max)))
			__schedstat_set(stats->ఖండం_max, delta);

		__schedstat_set(stats->ఖండం_ప్రారంభం, 0);
		__schedstat_add(stats->కూడిక_sleep_runtime, delta);
		__schedstat_add(stats->కూడిక_ఖండం_runtime, delta);

		ఒకవేళ (p) {
			ఒకవేళ (p->లోపల_iowait) {
				__schedstat_add(stats->iowait_కూడిక, delta);
				__schedstat_inc(stats->iowait_లెక్క);
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
#నిర్వచించు SCHEDSTAT_సంస్కరణ 17

స్థిర పూర్ణాంకం show_schedstat(నిర్మాణం seq_file *seq, శూన్య *v)
{
	పూర్ణాంకం cpu;

	ఒకవేళ (v == (శూన్య *)1) {
		seq_printf(seq, "version %d\n", SCHEDSTAT_సంస్కరణ);
		seq_printf(seq, "timestamp %lu\n", jiffies);
	} లేకపోతే {
		నిర్మాణం rq *rq;
		నిర్మాణం sched_domain *sd;
		పూర్ణాంకం dcount = 0;
		cpu = (గుర్తులేని దీర్ఘ)(v - 2);
		rq = cpu_rq(cpu);

		/* runqueue-specific stats */
		seq_printf(seq,
		    "cpu%d %u 0 %u %u %u %u %llu %llu %lu",
		    cpu, rq->yld_లెక్క,
		    rq->sched_లెక్క, rq->sched_goidle,
		    rq->ttwu_లెక్క, rq->ttwu_local,
		    rq->rq_cpu_సమయం,
		    rq->rq_sched_info.నిరంతరం_delay, rq->rq_sched_info.pcount);

		seq_printf(seq, "\n");

		/* domain-specific stats */
		rcu_read_తాళం();
		for_each_domain(cpu, sd) {
			గణన cpu_idle_type itype;

			seq_printf(seq, "domain%d %s %*pb", dcount++, sd->name,
				   cpumask_pr_వాదనలు(sched_domain_పరిధి(sd)));
			కొరకు (itype = 0; itype < CPU_MAX_IDLE_TYPES; itype++) {
				seq_printf(seq, " %u %u %u %u %u %u %u %u %u %u %u",
				    sd->lb_లెక్క[itype],
				    sd->lb_సమతుల్య[itype],
				    sd->lb_failed[itype],
				    sd->lb_imbalance_ఎక్కించు[itype],
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
			    sd->alb_లెక్క, sd->alb_failed, sd->alb_pushed,
			    sd->sbe_లెక్క, sd->sbe_సమతుల్య, sd->sbe_pushed,
			    sd->sbf_లెక్క, sd->sbf_సమతుల్య, sd->sbf_pushed,
			    sd->ttwu_wake_remote, sd->ttwu_move_affine,
			    sd->ttwu_move_balance);
		}
		rcu_read_unlock();
	}
	తిరిగి 0;
}

/*
 * This iterator needs some explanation.
 * It returns 1 for the header position.
 * This means 2 is cpu 0.
 * In a hotplugged system some CPUs, including cpu 0, may be missing so we have
 * to use cpumask_* to iterate over the CPUs.
 */
స్థిర శూన్య *schedstat_ప్రారంభం(నిర్మాణం seq_file *file, loff_t *స్థానభ్రంశం)
{
	గుర్తులేని దీర్ఘ n = *స్థానభ్రంశం;

	ఒకవేళ (n == 0)
		తిరిగి (శూన్య *) 1;

	n--;

	ఒకవేళ (n > 0)
		n = cpumask_తదుపరి(n - 1, cpu_online_ముసుగు);
	లేకపోతే
		n = cpumask_మొదటి(cpu_online_ముసుగు);

	*స్థానభ్రంశం = n + 1;

	ఒకవేళ (n < nr_cpu_ids)
		తిరిగి (శూన్య *)(గుర్తులేని దీర్ఘ)(n + 2);

	తిరిగి ఖాళీ;
}

స్థిర శూన్య *schedstat_తదుపరి(నిర్మాణం seq_file *file, శూన్య *దత్తాంశం, loff_t *స్థానభ్రంశం)
{
	(*స్థానభ్రంశం)++;

	తిరిగి schedstat_ప్రారంభం(file, స్థానభ్రంశం);
}

స్థిర శూన్య schedstat_stop(నిర్మాణం seq_file *file, శూన్య *దత్తాంశం)
{
}

స్థిర స్థిరరాశి నిర్మాణం seq_operations schedstat_sops = {
	.ప్రారంభం = schedstat_ప్రారంభం,
	.తదుపరి  = schedstat_తదుపరి,
	.stop  = schedstat_stop,
	.show  = show_schedstat,
};

స్థిర పూర్ణాంకం __init proc_schedstat_init(శూన్య)
{
	proc_create_seq("schedstat", 0, ఖాళీ, &schedstat_sops);
	తిరిగి 0;
}
subsys_initcall(proc_schedstat_init);
