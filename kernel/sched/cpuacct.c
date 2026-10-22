// SPDX-License-Identifier: GPL-2.0

/*
 * CPU accounting code for task groups.
 *
 * Based on the work by Paul Menage (menage@google.com) and Balbir Singh
 * (balbir@in.ibm.com).
 */
#ಅನ್ತರ್ಭಾವ <linux/sched/cputime.h>
#ಅನ್ತರ್ಭಾವ "sched.h"

/* Time spent by the tasks of the CPU accounting group executing in ... */
ಎಣಿಕೆ cpuacct_stat_ಸೂಚ್ಯಂಕ {
	CPUACCT_STAT_USER,	/* ... user mode */
	CPUACCT_STAT_SYSTEM,	/* ... kernel mode */

	CPUACCT_STAT_NSTATS,
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ cpuacct_stat_desc[] = {
	[CPUACCT_STAT_USER] = "user",
	[CPUACCT_STAT_SYSTEM] = "system",
};

/* track CPU usage of a group of tasks and its child groups */
ರಚನೆ cpuacct {
	ರಚನೆ cgroup_subsys_state	css;
	/* cpuusage holds pointer to a u64-type object on every CPU */
	u64 __percpu	*cpuusage;
	ರಚನೆ kernel_cpustat __percpu	*cpustat;
};

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ cpuacct *css_ca(ರಚನೆ cgroup_subsys_state *css)
{
	ಹಿಂತಿರುಗಿ css ? container_ದ(css, ರಚನೆ cpuacct, css) : ಖಾಲಿ;
}

/* Return CPU accounting group to which this task belongs */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ cpuacct *task_ca(ರಚನೆ task_struct *tsk)
{
	ಹಿಂತಿರುಗಿ css_ca(task_css(tsk, cpuacct_cgrp_id));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ cpuacct *ಪೋಷಕ_ca(ರಚನೆ cpuacct *ca)
{
	ಹಿಂತಿರುಗಿ css_ca(ca->css.ಪೋಷಕ);
}

ಸ್ಥಿರ DEFINE_ಪ್ರತಿ_CPU(u64, ಬೇರು_cpuacct_cpuusage);
ಸ್ಥಿರ ರಚನೆ cpuacct ಬೇರು_cpuacct = {
	.cpustat	= &kernel_cpustat,
	.cpuusage	= &ಬೇರು_cpuacct_cpuusage,
};

/* Create a new CPU accounting group */
ಸ್ಥಿರ ರಚನೆ cgroup_subsys_state *
cpuacct_css_alloc(ರಚನೆ cgroup_subsys_state *ಪೋಷಕ_css)
{
	ರಚನೆ cpuacct *ca;

	ಆದರೆ (!ಪೋಷಕ_css)
		ಹಿಂತಿರುಗಿ &ಬೇರು_cpuacct.css;

	ca = kzalloc_obj(*ca);
	ಆದರೆ (!ca)
		ಹೋಗು ಹೊರಹೋಗು;

	ca->cpuusage = alloc_percpu(u64);
	ಆದರೆ (!ca->cpuusage)
		ಹೋಗು ಹೊರಹೋಗು_free_ca;

	ca->cpustat = alloc_percpu(ರಚನೆ kernel_cpustat);
	ಆದರೆ (!ca->cpustat)
		ಹೋಗು ಹೊರಹೋಗು_free_cpuusage;

	ಹಿಂತಿರುಗಿ &ca->css;

ಹೊರಹೋಗು_free_cpuusage:
	free_percpu(ca->cpuusage);
ಹೊರಹೋಗು_free_ca:
	kfree(ca);
ಹೊರಹೋಗು:
	ಹಿಂತಿರುಗಿ ERR_PTR(-ENOMEM);
}

/* Destroy an existing CPU accounting group */
ಸ್ಥಿರ ಶೂನ್ಯ cpuacct_css_free(ರಚನೆ cgroup_subsys_state *css)
{
	ರಚನೆ cpuacct *ca = css_ca(css);

	free_percpu(ca->cpustat);
	free_percpu(ca->cpuusage);
	kfree(ca);
}

ಸ್ಥಿರ u64 cpuacct_cpuusage_read(ರಚನೆ cpuacct *ca, ಪೂರ್ಣಾಂಕ cpu,
				 ಎಣಿಕೆ cpuacct_stat_ಸೂಚ್ಯಂಕ ಸೂಚ್ಯಂಕ)
{
	u64 *cpuusage = ಪ್ರತಿ_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = ಪ್ರತಿ_cpu_ptr(ca->cpustat, cpu)->cpustat;
	u64 ದತ್ತಾಂಶ;

	/*
	 * We allow index == CPUACCT_STAT_NSTATS here to read
	 * the sum of usages.
	 */
	ಆದರೆ (WARN_ON_ONCE(ಸೂಚ್ಯಂಕ > CPUACCT_STAT_NSTATS))
		ಹಿಂತಿರುಗಿ 0;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CONFIG_64_ಬಿಟ್
	/*
	 * Take rq->lock to make 64-bit read safe on 32-bit platforms.
	 */
	raw_spin_rq_ಬೀಗ_irq(cpu_rq(cpu));
#ಆದರೆ_ಅಂತ್ಯ

	ಆಯ್ಕೆ (ಸೂಚ್ಯಂಕ) {
	ಸ್ಥಿತಿ CPUACCT_STAT_USER:
		ದತ್ತಾಂಶ = cpustat[CPUTIME_USER] + cpustat[CPUTIME_NICE];
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ CPUACCT_STAT_SYSTEM:
		ದತ್ತಾಂಶ = cpustat[CPUTIME_SYSTEM] + cpustat[CPUTIME_IRQ] +
			cpustat[CPUTIME_SOFTIRQ];
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ CPUACCT_STAT_NSTATS:
		ದತ್ತಾಂಶ = *cpuusage;
		ನಿಲ್ಲಿಸು;
	}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CONFIG_64_ಬಿಟ್
	raw_spin_rq_unlock_irq(cpu_rq(cpu));
#ಆದರೆ_ಅಂತ್ಯ

	ಹಿಂತಿರುಗಿ ದತ್ತಾಂಶ;
}

ಸ್ಥಿರ ಶೂನ್ಯ cpuacct_cpuusage_write(ರಚನೆ cpuacct *ca, ಪೂರ್ಣಾಂಕ cpu)
{
	u64 *cpuusage = ಪ್ರತಿ_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = ಪ್ರತಿ_cpu_ptr(ca->cpustat, cpu)->cpustat;

	/* Don't allow to reset global kernel_cpustat */
	ಆದರೆ (ca == &ಬೇರು_cpuacct)
		ಹಿಂತಿರುಗಿ;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CONFIG_64_ಬಿಟ್
	/*
	 * Take rq->lock to make 64-bit write safe on 32-bit platforms.
	 */
	raw_spin_rq_ಬೀಗ_irq(cpu_rq(cpu));
#ಆದರೆ_ಅಂತ್ಯ
	*cpuusage = 0;
	cpustat[CPUTIME_USER] = cpustat[CPUTIME_NICE] = 0;
	cpustat[CPUTIME_SYSTEM] = cpustat[CPUTIME_IRQ] = 0;
	cpustat[CPUTIME_SOFTIRQ] = 0;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CONFIG_64_ಬಿಟ್
	raw_spin_rq_unlock_irq(cpu_rq(cpu));
#ಆದರೆ_ಅಂತ್ಯ
}

/* Return total CPU usage (in nanoseconds) of a group */
ಸ್ಥಿರ u64 __cpuusage_read(ರಚನೆ cgroup_subsys_state *css,
			   ಎಣಿಕೆ cpuacct_stat_ಸೂಚ್ಯಂಕ ಸೂಚ್ಯಂಕ)
{
	ರಚನೆ cpuacct *ca = css_ca(css);
	u64 totalcpuusage = 0;
	ಪೂರ್ಣಾಂಕ i;

	for_each_possible_cpu(i)
		totalcpuusage += cpuacct_cpuusage_read(ca, i, ಸೂಚ್ಯಂಕ);

	ಹಿಂತಿರುಗಿ totalcpuusage;
}

ಸ್ಥಿರ u64 cpuusage_user_read(ರಚನೆ cgroup_subsys_state *css,
			      ರಚನೆ cftype *cft)
{
	ಹಿಂತಿರುಗಿ __cpuusage_read(css, CPUACCT_STAT_USER);
}

ಸ್ಥಿರ u64 cpuusage_sys_read(ರಚನೆ cgroup_subsys_state *css,
			     ರಚನೆ cftype *cft)
{
	ಹಿಂತಿರುಗಿ __cpuusage_read(css, CPUACCT_STAT_SYSTEM);
}

ಸ್ಥಿರ u64 cpuusage_read(ರಚನೆ cgroup_subsys_state *css, ರಚನೆ cftype *cft)
{
	ಹಿಂತಿರುಗಿ __cpuusage_read(css, CPUACCT_STAT_NSTATS);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpuusage_write(ರಚನೆ cgroup_subsys_state *css, ರಚನೆ cftype *cft,
			  u64 val)
{
	ರಚನೆ cpuacct *ca = css_ca(css);
	ಪೂರ್ಣಾಂಕ cpu;

	/*
	 * Only allow '0' here to do a reset.
	 */
	ಆದರೆ (val)
		ಹಿಂತಿರುಗಿ -EINVAL;

	for_each_possible_cpu(cpu)
		cpuacct_cpuusage_write(ca, cpu);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __cpuacct_percpu_seq_show(ರಚನೆ seq_file *m,
				     ಎಣಿಕೆ cpuacct_stat_ಸೂಚ್ಯಂಕ ಸೂಚ್ಯಂಕ)
{
	ರಚನೆ cpuacct *ca = css_ca(seq_css(m));
	u64 percpu;
	ಪೂರ್ಣಾಂಕ i;

	for_each_possible_cpu(i) {
		percpu = cpuacct_cpuusage_read(ca, i, ಸೂಚ್ಯಂಕ);
		seq_printf(m, "%llu ", (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) percpu);
	}
	seq_printf(m, "\n");
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpuacct_percpu_user_seq_show(ರಚನೆ seq_file *m, ಶೂನ್ಯ *V)
{
	ಹಿಂತಿರುಗಿ __cpuacct_percpu_seq_show(m, CPUACCT_STAT_USER);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpuacct_percpu_sys_seq_show(ರಚನೆ seq_file *m, ಶೂನ್ಯ *V)
{
	ಹಿಂತಿರುಗಿ __cpuacct_percpu_seq_show(m, CPUACCT_STAT_SYSTEM);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpuacct_percpu_seq_show(ರಚನೆ seq_file *m, ಶೂನ್ಯ *V)
{
	ಹಿಂತಿರುಗಿ __cpuacct_percpu_seq_show(m, CPUACCT_STAT_NSTATS);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpuacct_all_seq_show(ರಚನೆ seq_file *m, ಶೂನ್ಯ *V)
{
	ರಚನೆ cpuacct *ca = css_ca(seq_css(m));
	ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ;
	ಪೂರ್ಣಾಂಕ cpu;

	seq_puts(m, "cpu");
	ಗಾಗಿ (ಸೂಚ್ಯಂಕ = 0; ಸೂಚ್ಯಂಕ < CPUACCT_STAT_NSTATS; ಸೂಚ್ಯಂಕ++)
		seq_printf(m, " %s", cpuacct_stat_desc[ಸೂಚ್ಯಂಕ]);
	seq_puts(m, "\n");

	for_each_possible_cpu(cpu) {
		seq_printf(m, "%d", cpu);
		ಗಾಗಿ (ಸೂಚ್ಯಂಕ = 0; ಸೂಚ್ಯಂಕ < CPUACCT_STAT_NSTATS; ಸೂಚ್ಯಂಕ++)
			seq_printf(m, " %llu",
				   cpuacct_cpuusage_read(ca, cpu, ಸೂಚ್ಯಂಕ));
		seq_puts(m, "\n");
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpuacct_stats_show(ರಚನೆ seq_file *sf, ಶೂನ್ಯ *v)
{
	ರಚನೆ cpuacct *ca = css_ca(seq_css(sf));
	ರಚನೆ task_cputime cputime;
	u64 val[CPUACCT_STAT_NSTATS];
	ಪೂರ್ಣಾಂಕ cpu;
	ಪೂರ್ಣಾಂಕ stat;

	memset(&cputime, 0, ಗಾತ್ರ(cputime));
	for_each_possible_cpu(cpu) {
		u64 *cpustat = ಪ್ರತಿ_cpu_ptr(ca->cpustat, cpu)->cpustat;

		cputime.utime += cpustat[CPUTIME_USER];
		cputime.utime += cpustat[CPUTIME_NICE];
		cputime.stime += cpustat[CPUTIME_SYSTEM];
		cputime.stime += cpustat[CPUTIME_IRQ];
		cputime.stime += cpustat[CPUTIME_SOFTIRQ];

		cputime.ಮೊತ್ತ_exec_runtime += *ಪ್ರತಿ_cpu_ptr(ca->cpuusage, cpu);
	}

	cputime_adjust(&cputime, &seq_css(sf)->cgroup->prev_cputime,
		&val[CPUACCT_STAT_USER], &val[CPUACCT_STAT_SYSTEM]);

	ಗಾಗಿ (stat = 0; stat < CPUACCT_STAT_NSTATS; stat++) {
		seq_printf(sf, "%s %llu\n", cpuacct_stat_desc[stat],
			nsec_ಗಮ್ಯ_clock_t(val[stat]));
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ರಚನೆ cftype files[] = {
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
ಶೂನ್ಯ cpuacct_charge(ರಚನೆ task_struct *tsk, u64 cputime)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu = task_cpu(tsk);
	ರಚನೆ cpuacct *ca;

	lockdep_assert_rq_held(cpu_rq(cpu));

	ಗಾಗಿ (ca = task_ca(tsk); ca; ca = ಪೋಷಕ_ca(ca))
		*ಪ್ರತಿ_cpu_ptr(ca->cpuusage, cpu) += cputime;
}

/*
 * Add user/system time to cpuacct.
 *
 * Note: it's the caller that updates the account of the root cgroup.
 */
ಶೂನ್ಯ cpuacct_account_ಕ್ಷೇತ್ರ(ರಚನೆ task_struct *tsk, ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ, u64 val)
{
	ರಚನೆ cpuacct *ca;

	ಗಾಗಿ (ca = task_ca(tsk); ca != &ಬೇರು_cpuacct; ca = ಪೋಷಕ_ca(ca))
		__this_cpu_add(ca->cpustat->cpustat[ಸೂಚ್ಯಂಕ], val);
}

ರಚನೆ cgroup_subsys cpuacct_cgrp_subsys = {
	.css_alloc	= cpuacct_css_alloc,
	.css_free	= cpuacct_css_free,
	.legacy_cftypes	= files,
	.early_init	= true,
};
