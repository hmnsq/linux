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
ಗಣನಾ cpuacct_stat_index {
	CPUACCT_STAT_USER,	/* ... user mode */
	CPUACCT_STAT_SYSTEM,	/* ... kernel mode */

	CPUACCT_STAT_NSTATS,
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ cpuacct_stat_desc[] = {
	[CPUACCT_STAT_USER] = "user",
	[CPUACCT_STAT_SYSTEM] = "system",
};

/* track CPU usage of a group of tasks and its child groups */
ಸಂರಚನಾ cpuacct {
	ಸಂರಚನಾ cgroup_subsys_state	css;
	/* cpuusage holds pointer to a u64-type object on every CPU */
	u64 __percpu	*cpuusage;
	ಸಂರಚನಾ kernel_cpustat __percpu	*cpustat;
};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ cpuacct *css_ca(ಸಂರಚನಾ cgroup_subsys_state *css)
{
	ಪ್ರತ್ಯಾಗಮ css ? container_of(css, ಸಂರಚನಾ cpuacct, css) : ರಿಕ್ತ;
}

/* Return CPU accounting group to which this task belongs */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ cpuacct *task_ca(ಸಂರಚನಾ task_struct *tsk)
{
	ಪ್ರತ್ಯಾಗಮ css_ca(task_css(tsk, cpuacct_cgrp_id));
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ cpuacct *parent_ca(ಸಂರಚನಾ cpuacct *ca)
{
	ಪ್ರತ್ಯಾಗಮ css_ca(ca->css.parent);
}

ಸ್ಥಿರ DEFINE_PER_CPU(u64, ಬೇರು_cpuacct_cpuusage);
ಸ್ಥಿರ ಸಂರಚನಾ cpuacct ಬೇರು_cpuacct = {
	.cpustat	= &kernel_cpustat,
	.cpuusage	= &ಬೇರು_cpuacct_cpuusage,
};

/* Create a new CPU accounting group */
ಸ್ಥಿರ ಸಂರಚನಾ cgroup_subsys_state *
cpuacct_css_alloc(ಸಂರಚನಾ cgroup_subsys_state *parent_css)
{
	ಸಂರಚನಾ cpuacct *ca;

	ಯದಿ (!parent_css)
		ಪ್ರತ್ಯಾಗಮ &ಬೇರು_cpuacct.css;

	ca = kzalloc_obj(*ca);
	ಯದಿ (!ca)
		ಗಚ್ಛ out;

	ca->cpuusage = alloc_percpu(u64);
	ಯದಿ (!ca->cpuusage)
		ಗಚ್ಛ out_free_ca;

	ca->cpustat = alloc_percpu(ಸಂರಚನಾ kernel_cpustat);
	ಯದಿ (!ca->cpustat)
		ಗಚ್ಛ out_free_cpuusage;

	ಪ್ರತ್ಯಾಗಮ &ca->css;

out_free_cpuusage:
	free_percpu(ca->cpuusage);
out_free_ca:
	kfree(ca);
out:
	ಪ್ರತ್ಯಾಗಮ ERR_PTR(-ENOMEM);
}

/* Destroy an existing CPU accounting group */
ಸ್ಥಿರ ಶೂನ್ಯ cpuacct_css_free(ಸಂರಚನಾ cgroup_subsys_state *css)
{
	ಸಂರಚನಾ cpuacct *ca = css_ca(css);

	free_percpu(ca->cpustat);
	free_percpu(ca->cpuusage);
	kfree(ca);
}

ಸ್ಥಿರ u64 cpuacct_cpuusage_read(ಸಂರಚನಾ cpuacct *ca, ಪೂರ್ಣಾಙ್ಕ cpu,
				 ಗಣನಾ cpuacct_stat_index index)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;
	u64 data;

	/*
	 * We allow index == CPUACCT_STAT_NSTATS here to read
	 * the sum of usages.
	 */
	ಯದಿ (WARN_ON_ONCE(index > CPUACCT_STAT_NSTATS))
		ಪ್ರತ್ಯಾಗಮ 0;

#ifndef CONFIG_64BIT
	/*
	 * Take rq->lock to make 64-bit read safe on 32-bit platforms.
	 */
	raw_spin_rq_lock_irq(cpu_rq(cpu));
#endif

	ವಿಕಲ್ಪ (index) {
	ಪಕ್ಷ CPUACCT_STAT_USER:
		data = cpustat[CPUTIME_USER] + cpustat[CPUTIME_NICE];
		ಭಙ್ಗ;
	ಪಕ್ಷ CPUACCT_STAT_SYSTEM:
		data = cpustat[CPUTIME_SYSTEM] + cpustat[CPUTIME_IRQ] +
			cpustat[CPUTIME_SOFTIRQ];
		ಭಙ್ಗ;
	ಪಕ್ಷ CPUACCT_STAT_NSTATS:
		data = *cpuusage;
		ಭಙ್ಗ;
	}

#ifndef CONFIG_64BIT
	raw_spin_rq_unlock_irq(cpu_rq(cpu));
#endif

	ಪ್ರತ್ಯಾಗಮ data;
}

ಸ್ಥಿರ ಶೂನ್ಯ cpuacct_cpuusage_write(ಸಂರಚನಾ cpuacct *ca, ಪೂರ್ಣಾಙ್ಕ cpu)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;

	/* Don't allow to reset global kernel_cpustat */
	ಯದಿ (ca == &ಬೇರು_cpuacct)
		ಪ್ರತ್ಯಾಗಮ;

#ifndef CONFIG_64BIT
	/*
	 * Take rq->lock to make 64-bit write safe on 32-bit platforms.
	 */
	raw_spin_rq_lock_irq(cpu_rq(cpu));
#endif
	*cpuusage = 0;
	cpustat[CPUTIME_USER] = cpustat[CPUTIME_NICE] = 0;
	cpustat[CPUTIME_SYSTEM] = cpustat[CPUTIME_IRQ] = 0;
	cpustat[CPUTIME_SOFTIRQ] = 0;

#ifndef CONFIG_64BIT
	raw_spin_rq_unlock_irq(cpu_rq(cpu));
#endif
}

/* Return total CPU usage (in nanoseconds) of a group */
ಸ್ಥಿರ u64 __cpuusage_read(ಸಂರಚನಾ cgroup_subsys_state *css,
			   ಗಣನಾ cpuacct_stat_index index)
{
	ಸಂರಚನಾ cpuacct *ca = css_ca(css);
	u64 totalcpuusage = 0;
	ಪೂರ್ಣಾಙ್ಕ i;

	for_each_possible_cpu(i)
		totalcpuusage += cpuacct_cpuusage_read(ca, i, index);

	ಪ್ರತ್ಯಾಗಮ totalcpuusage;
}

ಸ್ಥಿರ u64 cpuusage_user_read(ಸಂರಚನಾ cgroup_subsys_state *css,
			      ಸಂರಚನಾ cftype *cft)
{
	ಪ್ರತ್ಯಾಗಮ __cpuusage_read(css, CPUACCT_STAT_USER);
}

ಸ್ಥಿರ u64 cpuusage_sys_read(ಸಂರಚನಾ cgroup_subsys_state *css,
			     ಸಂರಚನಾ cftype *cft)
{
	ಪ್ರತ್ಯಾಗಮ __cpuusage_read(css, CPUACCT_STAT_SYSTEM);
}

ಸ್ಥಿರ u64 cpuusage_read(ಸಂರಚನಾ cgroup_subsys_state *css, ಸಂರಚನಾ cftype *cft)
{
	ಪ್ರತ್ಯಾಗಮ __cpuusage_read(css, CPUACCT_STAT_NSTATS);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cpuusage_write(ಸಂರಚನಾ cgroup_subsys_state *css, ಸಂರಚನಾ cftype *cft,
			  u64 val)
{
	ಸಂರಚನಾ cpuacct *ca = css_ca(css);
	ಪೂರ್ಣಾಙ್ಕ cpu;

	/*
	 * Only allow '0' here to do a reset.
	 */
	ಯದಿ (val)
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	for_each_possible_cpu(cpu)
		cpuacct_cpuusage_write(ca, cpu);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ __cpuacct_percpu_seq_show(ಸಂರಚನಾ seq_file *m,
				     ಗಣನಾ cpuacct_stat_index index)
{
	ಸಂರಚನಾ cpuacct *ca = css_ca(seq_css(m));
	u64 percpu;
	ಪೂರ್ಣಾಙ್ಕ i;

	for_each_possible_cpu(i) {
		percpu = cpuacct_cpuusage_read(ca, i, index);
		seq_printf(m, "%llu ", (unsigned long long) percpu);
	}
	seq_printf(m, "\n");
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cpuacct_percpu_user_seq_show(ಸಂರಚನಾ seq_file *m, ಶೂನ್ಯ *V)
{
	ಪ್ರತ್ಯಾಗಮ __cpuacct_percpu_seq_show(m, CPUACCT_STAT_USER);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cpuacct_percpu_sys_seq_show(ಸಂರಚನಾ seq_file *m, ಶೂನ್ಯ *V)
{
	ಪ್ರತ್ಯಾಗಮ __cpuacct_percpu_seq_show(m, CPUACCT_STAT_SYSTEM);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cpuacct_percpu_seq_show(ಸಂರಚನಾ seq_file *m, ಶೂನ್ಯ *V)
{
	ಪ್ರತ್ಯಾಗಮ __cpuacct_percpu_seq_show(m, CPUACCT_STAT_NSTATS);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cpuacct_all_seq_show(ಸಂರಚನಾ seq_file *m, ಶೂನ್ಯ *V)
{
	ಸಂರಚನಾ cpuacct *ca = css_ca(seq_css(m));
	ಪೂರ್ಣಾಙ್ಕ index;
	ಪೂರ್ಣಾಙ್ಕ cpu;

	seq_puts(m, "cpu");
	ಪ್ರತಿ (index = 0; index < CPUACCT_STAT_NSTATS; index++)
		seq_printf(m, " %s", cpuacct_stat_desc[index]);
	seq_puts(m, "\n");

	for_each_possible_cpu(cpu) {
		seq_printf(m, "%d", cpu);
		ಪ್ರತಿ (index = 0; index < CPUACCT_STAT_NSTATS; index++)
			seq_printf(m, " %llu",
				   cpuacct_cpuusage_read(ca, cpu, index));
		seq_puts(m, "\n");
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cpuacct_stats_show(ಸಂರಚನಾ seq_file *sf, ಶೂನ್ಯ *v)
{
	ಸಂರಚನಾ cpuacct *ca = css_ca(seq_css(sf));
	ಸಂರಚನಾ task_cputime cputime;
	u64 val[CPUACCT_STAT_NSTATS];
	ಪೂರ್ಣಾಙ್ಕ cpu;
	ಪೂರ್ಣಾಙ್ಕ stat;

	memset(&cputime, 0, ಪರಿಮಾಣ(cputime));
	for_each_possible_cpu(cpu) {
		u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;

		cputime.utime += cpustat[CPUTIME_USER];
		cputime.utime += cpustat[CPUTIME_NICE];
		cputime.stime += cpustat[CPUTIME_SYSTEM];
		cputime.stime += cpustat[CPUTIME_IRQ];
		cputime.stime += cpustat[CPUTIME_SOFTIRQ];

		cputime.sum_exec_runtime += *per_cpu_ptr(ca->cpuusage, cpu);
	}

	cputime_adjust(&cputime, &seq_css(sf)->cgroup->prev_cputime,
		&val[CPUACCT_STAT_USER], &val[CPUACCT_STAT_SYSTEM]);

	ಪ್ರತಿ (stat = 0; stat < CPUACCT_STAT_NSTATS; stat++) {
		seq_printf(sf, "%s %llu\n", cpuacct_stat_desc[stat],
			nsec_to_clock_t(val[stat]));
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಸಂರಚನಾ cftype files[] = {
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
ಶೂನ್ಯ cpuacct_charge(ಸಂರಚನಾ task_struct *tsk, u64 cputime)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ cpu = task_cpu(tsk);
	ಸಂರಚನಾ cpuacct *ca;

	lockdep_assert_rq_held(cpu_rq(cpu));

	ಪ್ರತಿ (ca = task_ca(tsk); ca; ca = parent_ca(ca))
		*per_cpu_ptr(ca->cpuusage, cpu) += cputime;
}

/*
 * Add user/system time to cpuacct.
 *
 * Note: it's the caller that updates the account of the root cgroup.
 */
ಶೂನ್ಯ cpuacct_account_field(ಸಂರಚನಾ task_struct *tsk, ಪೂರ್ಣಾಙ್ಕ index, u64 val)
{
	ಸಂರಚನಾ cpuacct *ca;

	ಪ್ರತಿ (ca = task_ca(tsk); ca != &ಬೇರು_cpuacct; ca = parent_ca(ca))
		__this_cpu_add(ca->cpustat->cpustat[index], val);
}

ಸಂರಚನಾ cgroup_subsys cpuacct_cgrp_subsys = {
	.css_alloc	= cpuacct_css_alloc,
	.css_free	= cpuacct_css_free,
	.legacy_cftypes	= files,
	.early_init	= true,
};
