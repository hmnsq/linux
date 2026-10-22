// SPDX-License-Identifier: GPL-2.0

/*
 * CPU accounting code for task groups.
 *
 * Based on the work by Paul Menage (menage@google.com) and Balbir Singh
 * (balbir@in.ibm.com).
 */
#అన్తర్భావ <linux/sched/cputime.h>
#అన్తర్భావ "sched.h"

/* Time spent by the tasks of the CPU accounting group executing in ... */
గణన cpuacct_stat_సూచీ {
	CPUACCT_STAT_USER,	/* ... user mode */
	CPUACCT_STAT_SYSTEM,	/* ... kernel mode */

	CPUACCT_STAT_NSTATS,
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి cpuacct_stat_desc[] = {
	[CPUACCT_STAT_USER] = "user",
	[CPUACCT_STAT_SYSTEM] = "system",
};

/* track CPU usage of a group of tasks and its child groups */
నిర్మాణం cpuacct {
	నిర్మాణం cgroup_subsys_state	css;
	/* cpuusage holds pointer to a u64-type object on every CPU */
	u64 __percpu	*cpuusage;
	నిర్మాణం kernel_cpustat __percpu	*cpustat;
};

స్థిర అంతఃవరుస నిర్మాణం cpuacct *css_ca(నిర్మాణం cgroup_subsys_state *css)
{
	తిరిగి css ? container_యొక్క(css, నిర్మాణం cpuacct, css) : ఖాళీ;
}

/* Return CPU accounting group to which this task belongs */
స్థిర అంతఃవరుస నిర్మాణం cpuacct *task_ca(నిర్మాణం task_struct *tsk)
{
	తిరిగి css_ca(task_css(tsk, cpuacct_cgrp_id));
}

స్థిర అంతఃవరుస నిర్మాణం cpuacct *మాతృ_ca(నిర్మాణం cpuacct *ca)
{
	తిరిగి css_ca(ca->css.మాతృ);
}

స్థిర DEFINE_చొప్పున_CPU(u64, మూలం_cpuacct_cpuusage);
స్థిర నిర్మాణం cpuacct మూలం_cpuacct = {
	.cpustat	= &kernel_cpustat,
	.cpuusage	= &మూలం_cpuacct_cpuusage,
};

/* Create a new CPU accounting group */
స్థిర నిర్మాణం cgroup_subsys_state *
cpuacct_css_alloc(నిర్మాణం cgroup_subsys_state *మాతృ_css)
{
	నిర్మాణం cpuacct *ca;

	ఒకవేళ (!మాతృ_css)
		తిరిగి &మూలం_cpuacct.css;

	ca = kzalloc_obj(*ca);
	ఒకవేళ (!ca)
		వెళ్ళు బహిర్గతం;

	ca->cpuusage = alloc_percpu(u64);
	ఒకవేళ (!ca->cpuusage)
		వెళ్ళు బహిర్గతం_free_ca;

	ca->cpustat = alloc_percpu(నిర్మాణం kernel_cpustat);
	ఒకవేళ (!ca->cpustat)
		వెళ్ళు బహిర్గతం_free_cpuusage;

	తిరిగి &ca->css;

బహిర్గతం_free_cpuusage:
	free_percpu(ca->cpuusage);
బహిర్గతం_free_ca:
	kfree(ca);
బహిర్గతం:
	తిరిగి ERR_PTR(-ENOMEM);
}

/* Destroy an existing CPU accounting group */
స్థిర శూన్య cpuacct_css_free(నిర్మాణం cgroup_subsys_state *css)
{
	నిర్మాణం cpuacct *ca = css_ca(css);

	free_percpu(ca->cpustat);
	free_percpu(ca->cpuusage);
	kfree(ca);
}

స్థిర u64 cpuacct_cpuusage_read(నిర్మాణం cpuacct *ca, పూర్ణాంకం cpu,
				 గణన cpuacct_stat_సూచీ సూచీ)
{
	u64 *cpuusage = చొప్పున_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = చొప్పున_cpu_ptr(ca->cpustat, cpu)->cpustat;
	u64 దత్తాంశం;

	/*
	 * We allow index == CPUACCT_STAT_NSTATS here to read
	 * the sum of usages.
	 */
	ఒకవేళ (WARN_ON_ONCE(సూచీ > CPUACCT_STAT_NSTATS))
		తిరిగి 0;

#నిర్వచితం_కాకపోతే CONFIG_64_బిట్టు
	/*
	 * Take rq->lock to make 64-bit read safe on 32-bit platforms.
	 */
	raw_spin_rq_తాళం_irq(cpu_rq(cpu));
#అయితే_ముగింపు

	ఎంపిక (సూచీ) {
	స్థితి CPUACCT_STAT_USER:
		దత్తాంశం = cpustat[CPUTIME_USER] + cpustat[CPUTIME_NICE];
		ఆపు;
	స్థితి CPUACCT_STAT_SYSTEM:
		దత్తాంశం = cpustat[CPUTIME_SYSTEM] + cpustat[CPUTIME_IRQ] +
			cpustat[CPUTIME_SOFTIRQ];
		ఆపు;
	స్థితి CPUACCT_STAT_NSTATS:
		దత్తాంశం = *cpuusage;
		ఆపు;
	}

#నిర్వచితం_కాకపోతే CONFIG_64_బిట్టు
	raw_spin_rq_unlock_irq(cpu_rq(cpu));
#అయితే_ముగింపు

	తిరిగి దత్తాంశం;
}

స్థిర శూన్య cpuacct_cpuusage_write(నిర్మాణం cpuacct *ca, పూర్ణాంకం cpu)
{
	u64 *cpuusage = చొప్పున_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = చొప్పున_cpu_ptr(ca->cpustat, cpu)->cpustat;

	/* Don't allow to reset global kernel_cpustat */
	ఒకవేళ (ca == &మూలం_cpuacct)
		తిరిగి;

#నిర్వచితం_కాకపోతే CONFIG_64_బిట్టు
	/*
	 * Take rq->lock to make 64-bit write safe on 32-bit platforms.
	 */
	raw_spin_rq_తాళం_irq(cpu_rq(cpu));
#అయితే_ముగింపు
	*cpuusage = 0;
	cpustat[CPUTIME_USER] = cpustat[CPUTIME_NICE] = 0;
	cpustat[CPUTIME_SYSTEM] = cpustat[CPUTIME_IRQ] = 0;
	cpustat[CPUTIME_SOFTIRQ] = 0;

#నిర్వచితం_కాకపోతే CONFIG_64_బిట్టు
	raw_spin_rq_unlock_irq(cpu_rq(cpu));
#అయితే_ముగింపు
}

/* Return total CPU usage (in nanoseconds) of a group */
స్థిర u64 __cpuusage_read(నిర్మాణం cgroup_subsys_state *css,
			   గణన cpuacct_stat_సూచీ సూచీ)
{
	నిర్మాణం cpuacct *ca = css_ca(css);
	u64 totalcpuusage = 0;
	పూర్ణాంకం i;

	for_each_possible_cpu(i)
		totalcpuusage += cpuacct_cpuusage_read(ca, i, సూచీ);

	తిరిగి totalcpuusage;
}

స్థిర u64 cpuusage_user_read(నిర్మాణం cgroup_subsys_state *css,
			      నిర్మాణం cftype *cft)
{
	తిరిగి __cpuusage_read(css, CPUACCT_STAT_USER);
}

స్థిర u64 cpuusage_sys_read(నిర్మాణం cgroup_subsys_state *css,
			     నిర్మాణం cftype *cft)
{
	తిరిగి __cpuusage_read(css, CPUACCT_STAT_SYSTEM);
}

స్థిర u64 cpuusage_read(నిర్మాణం cgroup_subsys_state *css, నిర్మాణం cftype *cft)
{
	తిరిగి __cpuusage_read(css, CPUACCT_STAT_NSTATS);
}

స్థిర పూర్ణాంకం cpuusage_write(నిర్మాణం cgroup_subsys_state *css, నిర్మాణం cftype *cft,
			  u64 val)
{
	నిర్మాణం cpuacct *ca = css_ca(css);
	పూర్ణాంకం cpu;

	/*
	 * Only allow '0' here to do a reset.
	 */
	ఒకవేళ (val)
		తిరిగి -EINVAL;

	for_each_possible_cpu(cpu)
		cpuacct_cpuusage_write(ca, cpu);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం __cpuacct_percpu_seq_show(నిర్మాణం seq_file *m,
				     గణన cpuacct_stat_సూచీ సూచీ)
{
	నిర్మాణం cpuacct *ca = css_ca(seq_css(m));
	u64 percpu;
	పూర్ణాంకం i;

	for_each_possible_cpu(i) {
		percpu = cpuacct_cpuusage_read(ca, i, సూచీ);
		seq_printf(m, "%llu ", (గుర్తులేని దీర్ఘ దీర్ఘ) percpu);
	}
	seq_printf(m, "\n");
	తిరిగి 0;
}

స్థిర పూర్ణాంకం cpuacct_percpu_user_seq_show(నిర్మాణం seq_file *m, శూన్య *V)
{
	తిరిగి __cpuacct_percpu_seq_show(m, CPUACCT_STAT_USER);
}

స్థిర పూర్ణాంకం cpuacct_percpu_sys_seq_show(నిర్మాణం seq_file *m, శూన్య *V)
{
	తిరిగి __cpuacct_percpu_seq_show(m, CPUACCT_STAT_SYSTEM);
}

స్థిర పూర్ణాంకం cpuacct_percpu_seq_show(నిర్మాణం seq_file *m, శూన్య *V)
{
	తిరిగి __cpuacct_percpu_seq_show(m, CPUACCT_STAT_NSTATS);
}

స్థిర పూర్ణాంకం cpuacct_all_seq_show(నిర్మాణం seq_file *m, శూన్య *V)
{
	నిర్మాణం cpuacct *ca = css_ca(seq_css(m));
	పూర్ణాంకం సూచీ;
	పూర్ణాంకం cpu;

	seq_puts(m, "cpu");
	కొరకు (సూచీ = 0; సూచీ < CPUACCT_STAT_NSTATS; సూచీ++)
		seq_printf(m, " %s", cpuacct_stat_desc[సూచీ]);
	seq_puts(m, "\n");

	for_each_possible_cpu(cpu) {
		seq_printf(m, "%d", cpu);
		కొరకు (సూచీ = 0; సూచీ < CPUACCT_STAT_NSTATS; సూచీ++)
			seq_printf(m, " %llu",
				   cpuacct_cpuusage_read(ca, cpu, సూచీ));
		seq_puts(m, "\n");
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం cpuacct_stats_show(నిర్మాణం seq_file *sf, శూన్య *v)
{
	నిర్మాణం cpuacct *ca = css_ca(seq_css(sf));
	నిర్మాణం task_cputime cputime;
	u64 val[CPUACCT_STAT_NSTATS];
	పూర్ణాంకం cpu;
	పూర్ణాంకం stat;

	memset(&cputime, 0, పరిమాణం(cputime));
	for_each_possible_cpu(cpu) {
		u64 *cpustat = చొప్పున_cpu_ptr(ca->cpustat, cpu)->cpustat;

		cputime.utime += cpustat[CPUTIME_USER];
		cputime.utime += cpustat[CPUTIME_NICE];
		cputime.stime += cpustat[CPUTIME_SYSTEM];
		cputime.stime += cpustat[CPUTIME_IRQ];
		cputime.stime += cpustat[CPUTIME_SOFTIRQ];

		cputime.కూడిక_exec_runtime += *చొప్పున_cpu_ptr(ca->cpuusage, cpu);
	}

	cputime_adjust(&cputime, &seq_css(sf)->cgroup->prev_cputime,
		&val[CPUACCT_STAT_USER], &val[CPUACCT_STAT_SYSTEM]);

	కొరకు (stat = 0; stat < CPUACCT_STAT_NSTATS; stat++) {
		seq_printf(sf, "%s %llu\n", cpuacct_stat_desc[stat],
			nsec_గమ్యం_clock_t(val[stat]));
	}

	తిరిగి 0;
}

స్థిర నిర్మాణం cftype files[] = {
	{
		.name = "usage",
		.read_u64 = cpuusage_read,
		.write_u64 = cpuusage_write,
	},
	{
		.name = "usage_user",
		.read_u64 = cpuusage_user_read,
	},
	{
		.name = "usage_sys",
		.read_u64 = cpuusage_sys_read,
	},
	{
		.name = "usage_percpu",
		.seq_show = cpuacct_percpu_seq_show,
	},
	{
		.name = "usage_percpu_user",
		.seq_show = cpuacct_percpu_user_seq_show,
	},
	{
		.name = "usage_percpu_sys",
		.seq_show = cpuacct_percpu_sys_seq_show,
	},
	{
		.name = "usage_all",
		.seq_show = cpuacct_all_seq_show,
	},
	{
		.name = "stat",
		.seq_show = cpuacct_stats_show,
	},
	{ }	/* terminate */
};

/*
 * charge this task's execution time to its accounting group.
 *
 * called with rq->lock held.
 */
శూన్య cpuacct_charge(నిర్మాణం task_struct *tsk, u64 cputime)
{
	గుర్తులేని పూర్ణాంకం cpu = task_cpu(tsk);
	నిర్మాణం cpuacct *ca;

	lockdep_assert_rq_held(cpu_rq(cpu));

	కొరకు (ca = task_ca(tsk); ca; ca = మాతృ_ca(ca))
		*చొప్పున_cpu_ptr(ca->cpuusage, cpu) += cputime;
}

/*
 * Add user/system time to cpuacct.
 *
 * Note: it's the caller that updates the account of the root cgroup.
 */
శూన్య cpuacct_account_క్షేత్రం(నిర్మాణం task_struct *tsk, పూర్ణాంకం సూచీ, u64 val)
{
	నిర్మాణం cpuacct *ca;

	కొరకు (ca = task_ca(tsk); ca != &మూలం_cpuacct; ca = మాతృ_ca(ca))
		__this_cpu_add(ca->cpustat->cpustat[సూచీ], val);
}

నిర్మాణం cgroup_subsys cpuacct_cgrp_subsys = {
	.css_alloc	= cpuacct_css_alloc,
	.css_free	= cpuacct_css_free,
	.legacy_cftypes	= files,
	.early_init	= true,
};
