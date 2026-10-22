// SPDX-License-Identifier: GPL-2.0

/*
 * CPU accounting code for task groups.
 *
 * Based on the work by Paul Menage (menage@google.com) and Balbir Singh
 * (balbir@in.ibm.com).
 */
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/cputime.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"

/* Time spent by the tasks of the CPU accounting group executing in ... */
ਗਿਣਤੀ cpuacct_stat_ਸੂਚਕਾਂਕ {
	CPUACCT_STAT_USER,	/* ... user mode */
	CPUACCT_STAT_SYSTEM,	/* ... kernel mode */

	CPUACCT_STAT_NSTATS,
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ cpuacct_stat_desc[] = {
	[CPUACCT_STAT_USER] = "user",
	[CPUACCT_STAT_SYSTEM] = "system",
};

/* track CPU usage of a group of tasks and its child groups */
ਬਣਤਰ cpuacct {
	ਬਣਤਰ cgroup_subsys_state	css;
	/* cpuusage holds pointer to a u64-type object on every CPU */
	u64 __percpu	*cpuusage;
	ਬਣਤਰ kernel_cpustat __percpu	*cpustat;
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ cpuacct *css_ca(ਬਣਤਰ cgroup_subsys_state *css)
{
	ਵਾਪਸ css ? container_ਦਾ(css, ਬਣਤਰ cpuacct, css) : ਨਿਰਾ;
}

/* Return CPU accounting group to which this task belongs */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ cpuacct *task_ca(ਬਣਤਰ task_struct *tsk)
{
	ਵਾਪਸ css_ca(task_css(tsk, cpuacct_cgrp_id));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ cpuacct *ਮਾਪਾ_ca(ਬਣਤਰ cpuacct *ca)
{
	ਵਾਪਸ css_ca(ca->css.ਮਾਪਾ);
}

ਸਥਿਰ DEFINE_ਪ੍ਰਤੀ_CPU(u64, ਜੜ੍ਹ_cpuacct_cpuusage);
ਸਥਿਰ ਬਣਤਰ cpuacct ਜੜ੍ਹ_cpuacct = {
	.cpustat	= &kernel_cpustat,
	.cpuusage	= &ਜੜ੍ਹ_cpuacct_cpuusage,
};

/* Create a new CPU accounting group */
ਸਥਿਰ ਬਣਤਰ cgroup_subsys_state *
cpuacct_css_alloc(ਬਣਤਰ cgroup_subsys_state *ਮਾਪਾ_css)
{
	ਬਣਤਰ cpuacct *ca;

	ਜੇ (!ਮਾਪਾ_css)
		ਵਾਪਸ &ਜੜ੍ਹ_cpuacct.css;

	ca = kzalloc_obj(*ca);
	ਜੇ (!ca)
		ਜਾਓ ਬਾਹਰਲਾ;

	ca->cpuusage = alloc_percpu(u64);
	ਜੇ (!ca->cpuusage)
		ਜਾਓ ਬਾਹਰਲਾ_free_ca;

	ca->cpustat = alloc_percpu(ਬਣਤਰ kernel_cpustat);
	ਜੇ (!ca->cpustat)
		ਜਾਓ ਬਾਹਰਲਾ_free_cpuusage;

	ਵਾਪਸ &ca->css;

ਬਾਹਰਲਾ_free_cpuusage:
	free_percpu(ca->cpuusage);
ਬਾਹਰਲਾ_free_ca:
	kfree(ca);
ਬਾਹਰਲਾ:
	ਵਾਪਸ ERR_PTR(-ENOMEM);
}

/* Destroy an existing CPU accounting group */
ਸਥਿਰ ਖਾਲੀ cpuacct_css_free(ਬਣਤਰ cgroup_subsys_state *css)
{
	ਬਣਤਰ cpuacct *ca = css_ca(css);

	free_percpu(ca->cpustat);
	free_percpu(ca->cpuusage);
	kfree(ca);
}

ਸਥਿਰ u64 cpuacct_cpuusage_read(ਬਣਤਰ cpuacct *ca, ਪੂਰਨ cpu,
				 ਗਿਣਤੀ cpuacct_stat_ਸੂਚਕਾਂਕ ਸੂਚਕਾਂਕ)
{
	u64 *cpuusage = ਪ੍ਰਤੀ_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = ਪ੍ਰਤੀ_cpu_ptr(ca->cpustat, cpu)->cpustat;
	u64 ਅੰਕੜੇ;

	/*
	 * We allow index == CPUACCT_STAT_NSTATS here to read
	 * the sum of usages.
	 */
	ਜੇ (WARN_ON_ONCE(ਸੂਚਕਾਂਕ > CPUACCT_STAT_NSTATS))
		ਵਾਪਸ 0;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CONFIG_64_ਦੋਆਧਾਰੀ_ਅੰਕ
	/*
	 * Take rq->lock to make 64-bit read safe on 32-bit platforms.
	 */
	raw_spin_rq_ਤਾਲਾ_irq(cpu_rq(cpu));
#ਜੇ_ਅੰਤ

	ਚੋਣ (ਸੂਚਕਾਂਕ) {
	ਹਾਲਤ CPUACCT_STAT_USER:
		ਅੰਕੜੇ = cpustat[CPUTIME_USER] + cpustat[CPUTIME_NICE];
		ਰੋਕੋ;
	ਹਾਲਤ CPUACCT_STAT_SYSTEM:
		ਅੰਕੜੇ = cpustat[CPUTIME_SYSTEM] + cpustat[CPUTIME_IRQ] +
			cpustat[CPUTIME_SOFTIRQ];
		ਰੋਕੋ;
	ਹਾਲਤ CPUACCT_STAT_NSTATS:
		ਅੰਕੜੇ = *cpuusage;
		ਰੋਕੋ;
	}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CONFIG_64_ਦੋਆਧਾਰੀ_ਅੰਕ
	raw_spin_rq_unlock_irq(cpu_rq(cpu));
#ਜੇ_ਅੰਤ

	ਵਾਪਸ ਅੰਕੜੇ;
}

ਸਥਿਰ ਖਾਲੀ cpuacct_cpuusage_write(ਬਣਤਰ cpuacct *ca, ਪੂਰਨ cpu)
{
	u64 *cpuusage = ਪ੍ਰਤੀ_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = ਪ੍ਰਤੀ_cpu_ptr(ca->cpustat, cpu)->cpustat;

	/* Don't allow to reset global kernel_cpustat */
	ਜੇ (ca == &ਜੜ੍ਹ_cpuacct)
		ਵਾਪਸ;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CONFIG_64_ਦੋਆਧਾਰੀ_ਅੰਕ
	/*
	 * Take rq->lock to make 64-bit write safe on 32-bit platforms.
	 */
	raw_spin_rq_ਤਾਲਾ_irq(cpu_rq(cpu));
#ਜੇ_ਅੰਤ
	*cpuusage = 0;
	cpustat[CPUTIME_USER] = cpustat[CPUTIME_NICE] = 0;
	cpustat[CPUTIME_SYSTEM] = cpustat[CPUTIME_IRQ] = 0;
	cpustat[CPUTIME_SOFTIRQ] = 0;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CONFIG_64_ਦੋਆਧਾਰੀ_ਅੰਕ
	raw_spin_rq_unlock_irq(cpu_rq(cpu));
#ਜੇ_ਅੰਤ
}

/* Return total CPU usage (in nanoseconds) of a group */
ਸਥਿਰ u64 __cpuusage_read(ਬਣਤਰ cgroup_subsys_state *css,
			   ਗਿਣਤੀ cpuacct_stat_ਸੂਚਕਾਂਕ ਸੂਚਕਾਂਕ)
{
	ਬਣਤਰ cpuacct *ca = css_ca(css);
	u64 totalcpuusage = 0;
	ਪੂਰਨ i;

	for_each_possible_cpu(i)
		totalcpuusage += cpuacct_cpuusage_read(ca, i, ਸੂਚਕਾਂਕ);

	ਵਾਪਸ totalcpuusage;
}

ਸਥਿਰ u64 cpuusage_user_read(ਬਣਤਰ cgroup_subsys_state *css,
			      ਬਣਤਰ cftype *cft)
{
	ਵਾਪਸ __cpuusage_read(css, CPUACCT_STAT_USER);
}

ਸਥਿਰ u64 cpuusage_sys_read(ਬਣਤਰ cgroup_subsys_state *css,
			     ਬਣਤਰ cftype *cft)
{
	ਵਾਪਸ __cpuusage_read(css, CPUACCT_STAT_SYSTEM);
}

ਸਥਿਰ u64 cpuusage_read(ਬਣਤਰ cgroup_subsys_state *css, ਬਣਤਰ cftype *cft)
{
	ਵਾਪਸ __cpuusage_read(css, CPUACCT_STAT_NSTATS);
}

ਸਥਿਰ ਪੂਰਨ cpuusage_write(ਬਣਤਰ cgroup_subsys_state *css, ਬਣਤਰ cftype *cft,
			  u64 val)
{
	ਬਣਤਰ cpuacct *ca = css_ca(css);
	ਪੂਰਨ cpu;

	/*
	 * Only allow '0' here to do a reset.
	 */
	ਜੇ (val)
		ਵਾਪਸ -EINVAL;

	for_each_possible_cpu(cpu)
		cpuacct_cpuusage_write(ca, cpu);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ __cpuacct_percpu_seq_show(ਬਣਤਰ seq_file *m,
				     ਗਿਣਤੀ cpuacct_stat_ਸੂਚਕਾਂਕ ਸੂਚਕਾਂਕ)
{
	ਬਣਤਰ cpuacct *ca = css_ca(seq_css(m));
	u64 percpu;
	ਪੂਰਨ i;

	for_each_possible_cpu(i) {
		percpu = cpuacct_cpuusage_read(ca, i, ਸੂਚਕਾਂਕ);
		seq_printf(m, "%llu ", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) percpu);
	}
	seq_printf(m, "\n");
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ cpuacct_percpu_user_seq_show(ਬਣਤਰ seq_file *m, ਖਾਲੀ *V)
{
	ਵਾਪਸ __cpuacct_percpu_seq_show(m, CPUACCT_STAT_USER);
}

ਸਥਿਰ ਪੂਰਨ cpuacct_percpu_sys_seq_show(ਬਣਤਰ seq_file *m, ਖਾਲੀ *V)
{
	ਵਾਪਸ __cpuacct_percpu_seq_show(m, CPUACCT_STAT_SYSTEM);
}

ਸਥਿਰ ਪੂਰਨ cpuacct_percpu_seq_show(ਬਣਤਰ seq_file *m, ਖਾਲੀ *V)
{
	ਵਾਪਸ __cpuacct_percpu_seq_show(m, CPUACCT_STAT_NSTATS);
}

ਸਥਿਰ ਪੂਰਨ cpuacct_all_seq_show(ਬਣਤਰ seq_file *m, ਖਾਲੀ *V)
{
	ਬਣਤਰ cpuacct *ca = css_ca(seq_css(m));
	ਪੂਰਨ ਸੂਚਕਾਂਕ;
	ਪੂਰਨ cpu;

	seq_puts(m, "cpu");
	ਲਈ (ਸੂਚਕਾਂਕ = 0; ਸੂਚਕਾਂਕ < CPUACCT_STAT_NSTATS; ਸੂਚਕਾਂਕ++)
		seq_printf(m, " %s", cpuacct_stat_desc[ਸੂਚਕਾਂਕ]);
	seq_puts(m, "\n");

	for_each_possible_cpu(cpu) {
		seq_printf(m, "%d", cpu);
		ਲਈ (ਸੂਚਕਾਂਕ = 0; ਸੂਚਕਾਂਕ < CPUACCT_STAT_NSTATS; ਸੂਚਕਾਂਕ++)
			seq_printf(m, " %llu",
				   cpuacct_cpuusage_read(ca, cpu, ਸੂਚਕਾਂਕ));
		seq_puts(m, "\n");
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ cpuacct_stats_show(ਬਣਤਰ seq_file *sf, ਖਾਲੀ *v)
{
	ਬਣਤਰ cpuacct *ca = css_ca(seq_css(sf));
	ਬਣਤਰ task_cputime cputime;
	u64 val[CPUACCT_STAT_NSTATS];
	ਪੂਰਨ cpu;
	ਪੂਰਨ stat;

	memset(&cputime, 0, ਆਕਾਰ(cputime));
	for_each_possible_cpu(cpu) {
		u64 *cpustat = ਪ੍ਰਤੀ_cpu_ptr(ca->cpustat, cpu)->cpustat;

		cputime.utime += cpustat[CPUTIME_USER];
		cputime.utime += cpustat[CPUTIME_NICE];
		cputime.stime += cpustat[CPUTIME_SYSTEM];
		cputime.stime += cpustat[CPUTIME_IRQ];
		cputime.stime += cpustat[CPUTIME_SOFTIRQ];

		cputime.ਜੋੜ_exec_runtime += *ਪ੍ਰਤੀ_cpu_ptr(ca->cpuusage, cpu);
	}

	cputime_adjust(&cputime, &seq_css(sf)->cgroup->prev_cputime,
		&val[CPUACCT_STAT_USER], &val[CPUACCT_STAT_SYSTEM]);

	ਲਈ (stat = 0; stat < CPUACCT_STAT_NSTATS; stat++) {
		seq_printf(sf, "%s %llu\n", cpuacct_stat_desc[stat],
			nsec_ਮੰਜ਼ਿਲ_clock_t(val[stat]));
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਣਤਰ cftype files[] = {
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
ਖਾਲੀ cpuacct_charge(ਬਣਤਰ task_struct *tsk, u64 cputime)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu = task_cpu(tsk);
	ਬਣਤਰ cpuacct *ca;

	lockdep_assert_rq_held(cpu_rq(cpu));

	ਲਈ (ca = task_ca(tsk); ca; ca = ਮਾਪਾ_ca(ca))
		*ਪ੍ਰਤੀ_cpu_ptr(ca->cpuusage, cpu) += cputime;
}

/*
 * Add user/system time to cpuacct.
 *
 * Note: it's the caller that updates the account of the root cgroup.
 */
ਖਾਲੀ cpuacct_account_ਖੇਤਰ(ਬਣਤਰ task_struct *tsk, ਪੂਰਨ ਸੂਚਕਾਂਕ, u64 val)
{
	ਬਣਤਰ cpuacct *ca;

	ਲਈ (ca = task_ca(tsk); ca != &ਜੜ੍ਹ_cpuacct; ca = ਮਾਪਾ_ca(ca))
		__this_cpu_add(ca->cpustat->cpustat[ਸੂਚਕਾਂਕ], val);
}

ਬਣਤਰ cgroup_subsys cpuacct_cgrp_subsys = {
	.css_alloc	= cpuacct_css_alloc,
	.css_free	= cpuacct_css_free,
	.legacy_cftypes	= files,
	.early_init	= true,
};
