// SPDX-License-Identifier: GPL-2.0

/*
 * CPU accounting code for task groups.
 *
 * Based on the work by Paul Menage (menage@google.com) and Balbir Singh
 * (balbir@in.ibm.com).
 */
#অন্তর্ভাব <linux/sched/cputime.h>
#অন্তর্ভাব "sched.h"

/* Time spent by the tasks of the CPU accounting group executing in ... */
গণনা cpuacct_stat_সূচক {
	CPUACCT_STAT_USER,	/* ... user mode */
	CPUACCT_STAT_SYSTEM,	/* ... kernel mode */

	CPUACCT_STAT_NSTATS,
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক cpuacct_stat_desc[] = {
	[CPUACCT_STAT_USER] = "user",
	[CPUACCT_STAT_SYSTEM] = "system",
};

/* track CPU usage of a group of tasks and its child groups */
কাঠামো cpuacct {
	কাঠামো cgroup_subsys_state	css;
	/* cpuusage holds pointer to a u64-type object on every CPU */
	u64 __percpu	*cpuusage;
	কাঠামো kernel_cpustat __percpu	*cpustat;
};

স্থির অন্তঃরেখ কাঠামো cpuacct *css_ca(কাঠামো cgroup_subsys_state *css)
{
	ফেরত css ? container_of(css, কাঠামো cpuacct, css) : ফাঁকা;
}

/* Return CPU accounting group to which this task belongs */
স্থির অন্তঃরেখ কাঠামো cpuacct *task_ca(কাঠামো task_struct *tsk)
{
	ফেরত css_ca(task_css(tsk, cpuacct_cgrp_id));
}

স্থির অন্তঃরেখ কাঠামো cpuacct *parent_ca(কাঠামো cpuacct *ca)
{
	ফেরত css_ca(ca->css.parent);
}

স্থির DEFINE_PER_CPU(u64, মূল_cpuacct_cpuusage);
স্থির কাঠামো cpuacct মূল_cpuacct = {
	.cpustat	= &kernel_cpustat,
	.cpuusage	= &মূল_cpuacct_cpuusage,
};

/* Create a new CPU accounting group */
স্থির কাঠামো cgroup_subsys_state *
cpuacct_css_alloc(কাঠামো cgroup_subsys_state *parent_css)
{
	কাঠামো cpuacct *ca;

	যদি (!parent_css)
		ফেরত &মূল_cpuacct.css;

	ca = kzalloc_obj(*ca);
	যদি (!ca)
		যাও out;

	ca->cpuusage = alloc_percpu(u64);
	যদি (!ca->cpuusage)
		যাও out_free_ca;

	ca->cpustat = alloc_percpu(কাঠামো kernel_cpustat);
	যদি (!ca->cpustat)
		যাও out_free_cpuusage;

	ফেরত &ca->css;

out_free_cpuusage:
	free_percpu(ca->cpuusage);
out_free_ca:
	kfree(ca);
out:
	ফেরত ERR_PTR(-ENOMEM);
}

/* Destroy an existing CPU accounting group */
স্থির শূন্য cpuacct_css_free(কাঠামো cgroup_subsys_state *css)
{
	কাঠামো cpuacct *ca = css_ca(css);

	free_percpu(ca->cpustat);
	free_percpu(ca->cpuusage);
	kfree(ca);
}

স্থির u64 cpuacct_cpuusage_read(কাঠামো cpuacct *ca, পূর্ণসংখ্যা cpu,
				 গণনা cpuacct_stat_সূচক সূচক)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;
	u64 তথ্য;

	/*
	 * We allow index == CPUACCT_STAT_NSTATS here to read
	 * the sum of usages.
	 */
	যদি (WARN_ON_ONCE(সূচক > CPUACCT_STAT_NSTATS))
		ফেরত 0;

#ifndef CONFIG_64BIT
	/*
	 * Take rq->lock to make 64-bit read safe on 32-bit platforms.
	 */
	raw_spin_rq_lock_irq(cpu_rq(cpu));
#endif

	নির্বাচন (সূচক) {
	ক্ষেত্র CPUACCT_STAT_USER:
		তথ্য = cpustat[CPUTIME_USER] + cpustat[CPUTIME_NICE];
		বিরতি;
	ক্ষেত্র CPUACCT_STAT_SYSTEM:
		তথ্য = cpustat[CPUTIME_SYSTEM] + cpustat[CPUTIME_IRQ] +
			cpustat[CPUTIME_SOFTIRQ];
		বিরতি;
	ক্ষেত্র CPUACCT_STAT_NSTATS:
		তথ্য = *cpuusage;
		বিরতি;
	}

#ifndef CONFIG_64BIT
	raw_spin_rq_unlock_irq(cpu_rq(cpu));
#endif

	ফেরত তথ্য;
}

স্থির শূন্য cpuacct_cpuusage_write(কাঠামো cpuacct *ca, পূর্ণসংখ্যা cpu)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;

	/* Don't allow to reset global kernel_cpustat */
	যদি (ca == &মূল_cpuacct)
		ফেরত;

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
স্থির u64 __cpuusage_read(কাঠামো cgroup_subsys_state *css,
			   গণনা cpuacct_stat_সূচক সূচক)
{
	কাঠামো cpuacct *ca = css_ca(css);
	u64 totalcpuusage = 0;
	পূর্ণসংখ্যা i;

	for_each_possible_cpu(i)
		totalcpuusage += cpuacct_cpuusage_read(ca, i, সূচক);

	ফেরত totalcpuusage;
}

স্থির u64 cpuusage_user_read(কাঠামো cgroup_subsys_state *css,
			      কাঠামো cftype *cft)
{
	ফেরত __cpuusage_read(css, CPUACCT_STAT_USER);
}

স্থির u64 cpuusage_sys_read(কাঠামো cgroup_subsys_state *css,
			     কাঠামো cftype *cft)
{
	ফেরত __cpuusage_read(css, CPUACCT_STAT_SYSTEM);
}

স্থির u64 cpuusage_read(কাঠামো cgroup_subsys_state *css, কাঠামো cftype *cft)
{
	ফেরত __cpuusage_read(css, CPUACCT_STAT_NSTATS);
}

স্থির পূর্ণসংখ্যা cpuusage_write(কাঠামো cgroup_subsys_state *css, কাঠামো cftype *cft,
			  u64 val)
{
	কাঠামো cpuacct *ca = css_ca(css);
	পূর্ণসংখ্যা cpu;

	/*
	 * Only allow '0' here to do a reset.
	 */
	যদি (val)
		ফেরত -EINVAL;

	for_each_possible_cpu(cpu)
		cpuacct_cpuusage_write(ca, cpu);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা __cpuacct_percpu_seq_show(কাঠামো seq_file *m,
				     গণনা cpuacct_stat_সূচক সূচক)
{
	কাঠামো cpuacct *ca = css_ca(seq_css(m));
	u64 percpu;
	পূর্ণসংখ্যা i;

	for_each_possible_cpu(i) {
		percpu = cpuacct_cpuusage_read(ca, i, সূচক);
		seq_printf(m, "%llu ", (unsigned long long) percpu);
	}
	seq_printf(m, "\n");
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা cpuacct_percpu_user_seq_show(কাঠামো seq_file *m, শূন্য *V)
{
	ফেরত __cpuacct_percpu_seq_show(m, CPUACCT_STAT_USER);
}

স্থির পূর্ণসংখ্যা cpuacct_percpu_sys_seq_show(কাঠামো seq_file *m, শূন্য *V)
{
	ফেরত __cpuacct_percpu_seq_show(m, CPUACCT_STAT_SYSTEM);
}

স্থির পূর্ণসংখ্যা cpuacct_percpu_seq_show(কাঠামো seq_file *m, শূন্য *V)
{
	ফেরত __cpuacct_percpu_seq_show(m, CPUACCT_STAT_NSTATS);
}

স্থির পূর্ণসংখ্যা cpuacct_all_seq_show(কাঠামো seq_file *m, শূন্য *V)
{
	কাঠামো cpuacct *ca = css_ca(seq_css(m));
	পূর্ণসংখ্যা সূচক;
	পূর্ণসংখ্যা cpu;

	seq_puts(m, "cpu");
	জন্য (সূচক = 0; সূচক < CPUACCT_STAT_NSTATS; সূচক++)
		seq_printf(m, " %s", cpuacct_stat_desc[সূচক]);
	seq_puts(m, "\n");

	for_each_possible_cpu(cpu) {
		seq_printf(m, "%d", cpu);
		জন্য (সূচক = 0; সূচক < CPUACCT_STAT_NSTATS; সূচক++)
			seq_printf(m, " %llu",
				   cpuacct_cpuusage_read(ca, cpu, সূচক));
		seq_puts(m, "\n");
	}
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা cpuacct_stats_show(কাঠামো seq_file *sf, শূন্য *v)
{
	কাঠামো cpuacct *ca = css_ca(seq_css(sf));
	কাঠামো task_cputime cputime;
	u64 val[CPUACCT_STAT_NSTATS];
	পূর্ণসংখ্যা cpu;
	পূর্ণসংখ্যা stat;

	memset(&cputime, 0, আকার(cputime));
	for_each_possible_cpu(cpu) {
		u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;

		cputime.utime += cpustat[CPUTIME_USER];
		cputime.utime += cpustat[CPUTIME_NICE];
		cputime.stime += cpustat[CPUTIME_SYSTEM];
		cputime.stime += cpustat[CPUTIME_IRQ];
		cputime.stime += cpustat[CPUTIME_SOFTIRQ];

		cputime.যোগ_exec_runtime += *per_cpu_ptr(ca->cpuusage, cpu);
	}

	cputime_adjust(&cputime, &seq_css(sf)->cgroup->পূর্ব_cputime,
		&val[CPUACCT_STAT_USER], &val[CPUACCT_STAT_SYSTEM]);

	জন্য (stat = 0; stat < CPUACCT_STAT_NSTATS; stat++) {
		seq_printf(sf, "%s %llu\n", cpuacct_stat_desc[stat],
			nsec_to_clock_t(val[stat]));
	}

	ফেরত 0;
}

স্থির কাঠামো cftype files[] = {
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
শূন্য cpuacct_charge(কাঠামো task_struct *tsk, u64 cputime)
{
	unsigned পূর্ণসংখ্যা cpu = task_cpu(tsk);
	কাঠামো cpuacct *ca;

	lockdep_assert_rq_held(cpu_rq(cpu));

	জন্য (ca = task_ca(tsk); ca; ca = parent_ca(ca))
		*per_cpu_ptr(ca->cpuusage, cpu) += cputime;
}

/*
 * Add user/system time to cpuacct.
 *
 * Note: it's the caller that updates the account of the root cgroup.
 */
শূন্য cpuacct_account_field(কাঠামো task_struct *tsk, পূর্ণসংখ্যা সূচক, u64 val)
{
	কাঠামো cpuacct *ca;

	জন্য (ca = task_ca(tsk); ca != &মূল_cpuacct; ca = parent_ca(ca))
		__this_cpu_add(ca->cpustat->cpustat[সূচক], val);
}

কাঠামো cgroup_subsys cpuacct_cgrp_subsys = {
	.css_alloc	= cpuacct_css_alloc,
	.css_free	= cpuacct_css_free,
	.legacy_cftypes	= files,
	.early_init	= true,
};
