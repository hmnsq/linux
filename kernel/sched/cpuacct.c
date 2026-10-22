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
গণনা cpuacct_stat_index {
	CPUACCT_STAT_USER,	/* ... user mode */
	CPUACCT_STAT_SYSTEM,	/* ... kernel mode */

	CPUACCT_STAT_NSTATS,
};

স্থির ধ্রুব বর্ণ * ধ্রুব cpuacct_stat_desc[] = {
	[CPUACCT_STAT_USER] = "user",
	[CPUACCT_STAT_SYSTEM] = "system",
};

/* track CPU usage of a group of tasks and its child groups */
সংরচনা cpuacct {
	সংরচনা cgroup_subsys_state	css;
	/* cpuusage holds pointer to a u64-type object on every CPU */
	u64 __percpu	*cpuusage;
	সংরচনা kernel_cpustat __percpu	*cpustat;
};

স্থির অন্তঃস্থ সংরচনা cpuacct *css_ca(সংরচনা cgroup_subsys_state *css)
{
	প্রত্যাগম css ? container_of(css, সংরচনা cpuacct, css) : রিক্ত;
}

/* Return CPU accounting group to which this task belongs */
স্থির অন্তঃস্থ সংরচনা cpuacct *task_ca(সংরচনা task_struct *tsk)
{
	প্রত্যাগম css_ca(task_css(tsk, cpuacct_cgrp_id));
}

স্থির অন্তঃস্থ সংরচনা cpuacct *parent_ca(সংরচনা cpuacct *ca)
{
	প্রত্যাগম css_ca(ca->css.parent);
}

স্থির DEFINE_PER_CPU(u64, মূল_cpuacct_cpuusage);
স্থির সংরচনা cpuacct মূল_cpuacct = {
	.cpustat	= &kernel_cpustat,
	.cpuusage	= &মূল_cpuacct_cpuusage,
};

/* Create a new CPU accounting group */
স্থির সংরচনা cgroup_subsys_state *
cpuacct_css_alloc(সংরচনা cgroup_subsys_state *parent_css)
{
	সংরচনা cpuacct *ca;

	যদি (!parent_css)
		প্রত্যাগম &মূল_cpuacct.css;

	ca = kzalloc_obj(*ca);
	যদি (!ca)
		গচ্ছ out;

	ca->cpuusage = alloc_percpu(u64);
	যদি (!ca->cpuusage)
		গচ্ছ out_free_ca;

	ca->cpustat = alloc_percpu(সংরচনা kernel_cpustat);
	যদি (!ca->cpustat)
		গচ্ছ out_free_cpuusage;

	প্রত্যাগম &ca->css;

out_free_cpuusage:
	free_percpu(ca->cpuusage);
out_free_ca:
	kfree(ca);
out:
	প্রত্যাগম ERR_PTR(-ENOMEM);
}

/* Destroy an existing CPU accounting group */
স্থির শূন্য cpuacct_css_free(সংরচনা cgroup_subsys_state *css)
{
	সংরচনা cpuacct *ca = css_ca(css);

	free_percpu(ca->cpustat);
	free_percpu(ca->cpuusage);
	kfree(ca);
}

স্থির u64 cpuacct_cpuusage_read(সংরচনা cpuacct *ca, পূর্ণাঙ্ক cpu,
				 গণনা cpuacct_stat_index index)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;
	u64 data;

	/*
	 * We allow index == CPUACCT_STAT_NSTATS here to read
	 * the sum of usages.
	 */
	যদি (WARN_ON_ONCE(index > CPUACCT_STAT_NSTATS))
		প্রত্যাগম 0;

#ifndef CONFIG_64BIT
	/*
	 * Take rq->lock to make 64-bit read safe on 32-bit platforms.
	 */
	raw_spin_rq_lock_irq(cpu_rq(cpu));
#endif

	বিকল্প (index) {
	পক্ষ CPUACCT_STAT_USER:
		data = cpustat[CPUTIME_USER] + cpustat[CPUTIME_NICE];
		ভঙ্গ;
	পক্ষ CPUACCT_STAT_SYSTEM:
		data = cpustat[CPUTIME_SYSTEM] + cpustat[CPUTIME_IRQ] +
			cpustat[CPUTIME_SOFTIRQ];
		ভঙ্গ;
	পক্ষ CPUACCT_STAT_NSTATS:
		data = *cpuusage;
		ভঙ্গ;
	}

#ifndef CONFIG_64BIT
	raw_spin_rq_unlock_irq(cpu_rq(cpu));
#endif

	প্রত্যাগম data;
}

স্থির শূন্য cpuacct_cpuusage_write(সংরচনা cpuacct *ca, পূর্ণাঙ্ক cpu)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;

	/* Don't allow to reset global kernel_cpustat */
	যদি (ca == &মূল_cpuacct)
		প্রত্যাগম;

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
স্থির u64 __cpuusage_read(সংরচনা cgroup_subsys_state *css,
			   গণনা cpuacct_stat_index index)
{
	সংরচনা cpuacct *ca = css_ca(css);
	u64 totalcpuusage = 0;
	পূর্ণাঙ্ক i;

	for_each_possible_cpu(i)
		totalcpuusage += cpuacct_cpuusage_read(ca, i, index);

	প্রত্যাগম totalcpuusage;
}

স্থির u64 cpuusage_user_read(সংরচনা cgroup_subsys_state *css,
			      সংরচনা cftype *cft)
{
	প্রত্যাগম __cpuusage_read(css, CPUACCT_STAT_USER);
}

স্থির u64 cpuusage_sys_read(সংরচনা cgroup_subsys_state *css,
			     সংরচনা cftype *cft)
{
	প্রত্যাগম __cpuusage_read(css, CPUACCT_STAT_SYSTEM);
}

স্থির u64 cpuusage_read(সংরচনা cgroup_subsys_state *css, সংরচনা cftype *cft)
{
	প্রত্যাগম __cpuusage_read(css, CPUACCT_STAT_NSTATS);
}

স্থির পূর্ণাঙ্ক cpuusage_write(সংরচনা cgroup_subsys_state *css, সংরচনা cftype *cft,
			  u64 val)
{
	সংরচনা cpuacct *ca = css_ca(css);
	পূর্ণাঙ্ক cpu;

	/*
	 * Only allow '0' here to do a reset.
	 */
	যদি (val)
		প্রত্যাগম -EINVAL;

	for_each_possible_cpu(cpu)
		cpuacct_cpuusage_write(ca, cpu);

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক __cpuacct_percpu_seq_show(সংরচনা seq_file *m,
				     গণনা cpuacct_stat_index index)
{
	সংরচনা cpuacct *ca = css_ca(seq_css(m));
	u64 percpu;
	পূর্ণাঙ্ক i;

	for_each_possible_cpu(i) {
		percpu = cpuacct_cpuusage_read(ca, i, index);
		seq_printf(m, "%llu ", (unsigned long long) percpu);
	}
	seq_printf(m, "\n");
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক cpuacct_percpu_user_seq_show(সংরচনা seq_file *m, শূন্য *V)
{
	প্রত্যাগম __cpuacct_percpu_seq_show(m, CPUACCT_STAT_USER);
}

স্থির পূর্ণাঙ্ক cpuacct_percpu_sys_seq_show(সংরচনা seq_file *m, শূন্য *V)
{
	প্রত্যাগম __cpuacct_percpu_seq_show(m, CPUACCT_STAT_SYSTEM);
}

স্থির পূর্ণাঙ্ক cpuacct_percpu_seq_show(সংরচনা seq_file *m, শূন্য *V)
{
	প্রত্যাগম __cpuacct_percpu_seq_show(m, CPUACCT_STAT_NSTATS);
}

স্থির পূর্ণাঙ্ক cpuacct_all_seq_show(সংরচনা seq_file *m, শূন্য *V)
{
	সংরচনা cpuacct *ca = css_ca(seq_css(m));
	পূর্ণাঙ্ক index;
	পূর্ণাঙ্ক cpu;

	seq_puts(m, "cpu");
	প্রতি (index = 0; index < CPUACCT_STAT_NSTATS; index++)
		seq_printf(m, " %s", cpuacct_stat_desc[index]);
	seq_puts(m, "\n");

	for_each_possible_cpu(cpu) {
		seq_printf(m, "%d", cpu);
		প্রতি (index = 0; index < CPUACCT_STAT_NSTATS; index++)
			seq_printf(m, " %llu",
				   cpuacct_cpuusage_read(ca, cpu, index));
		seq_puts(m, "\n");
	}
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক cpuacct_stats_show(সংরচনা seq_file *sf, শূন্য *v)
{
	সংরচনা cpuacct *ca = css_ca(seq_css(sf));
	সংরচনা task_cputime cputime;
	u64 val[CPUACCT_STAT_NSTATS];
	পূর্ণাঙ্ক cpu;
	পূর্ণাঙ্ক stat;

	memset(&cputime, 0, পরিমাণ(cputime));
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

	প্রতি (stat = 0; stat < CPUACCT_STAT_NSTATS; stat++) {
		seq_printf(sf, "%s %llu\n", cpuacct_stat_desc[stat],
			nsec_to_clock_t(val[stat]));
	}

	প্রত্যাগম 0;
}

স্থির সংরচনা cftype files[] = {
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
শূন্য cpuacct_charge(সংরচনা task_struct *tsk, u64 cputime)
{
	unsigned পূর্ণাঙ্ক cpu = task_cpu(tsk);
	সংরচনা cpuacct *ca;

	lockdep_assert_rq_held(cpu_rq(cpu));

	প্রতি (ca = task_ca(tsk); ca; ca = parent_ca(ca))
		*per_cpu_ptr(ca->cpuusage, cpu) += cputime;
}

/*
 * Add user/system time to cpuacct.
 *
 * Note: it's the caller that updates the account of the root cgroup.
 */
শূন্য cpuacct_account_field(সংরচনা task_struct *tsk, পূর্ণাঙ্ক index, u64 val)
{
	সংরচনা cpuacct *ca;

	প্রতি (ca = task_ca(tsk); ca != &মূল_cpuacct; ca = parent_ca(ca))
		__this_cpu_add(ca->cpustat->cpustat[index], val);
}

সংরচনা cgroup_subsys cpuacct_cgrp_subsys = {
	.css_alloc	= cpuacct_css_alloc,
	.css_free	= cpuacct_css_free,
	.legacy_cftypes	= files,
	.early_init	= true,
};
