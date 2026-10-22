// SPDX-License-Identifier: GPL-2.0

/*
 * CPU accounting code for task groups.
 *
 * Based on the work by Paul Menage (menage@google.com) and Balbir Singh
 * (balbir@in.ibm.com).
 */
#അന്തര്ഭാവ <linux/sched/cputime.h>
#അന്തര്ഭാവ "sched.h"

/* Time spent by the tasks of the CPU accounting group executing in ... */
എണ്ണൽ cpuacct_stat_index {
	CPUACCT_STAT_USER,	/* ... user mode */
	CPUACCT_STAT_SYSTEM,	/* ... kernel mode */

	CPUACCT_STAT_NSTATS,
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം cpuacct_stat_desc[] = {
	[CPUACCT_STAT_USER] = "user",
	[CPUACCT_STAT_SYSTEM] = "system",
};

/* track CPU usage of a group of tasks and its child groups */
ഘടന cpuacct {
	ഘടന cgroup_subsys_state	css;
	/* cpuusage holds pointer to a u64-type object on every CPU */
	u64 __percpu	*cpuusage;
	ഘടന kernel_cpustat __percpu	*cpustat;
};

സ്ഥിതം വരിയിൽ ഘടന cpuacct *css_ca(ഘടന cgroup_subsys_state *css)
{
	തിരികെ css ? container_of(css, ഘടന cpuacct, css) : ശൂന്യ;
}

/* Return CPU accounting group to which this task belongs */
സ്ഥിതം വരിയിൽ ഘടന cpuacct *task_ca(ഘടന task_struct *tsk)
{
	തിരികെ css_ca(task_css(tsk, cpuacct_cgrp_id));
}

സ്ഥിതം വരിയിൽ ഘടന cpuacct *parent_ca(ഘടന cpuacct *ca)
{
	തിരികെ css_ca(ca->css.parent);
}

സ്ഥിതം DEFINE_PER_CPU(u64, വേര്_cpuacct_cpuusage);
സ്ഥിതം ഘടന cpuacct വേര്_cpuacct = {
	.cpustat	= &kernel_cpustat,
	.cpuusage	= &വേര്_cpuacct_cpuusage,
};

/* Create a new CPU accounting group */
സ്ഥിതം ഘടന cgroup_subsys_state *
cpuacct_css_alloc(ഘടന cgroup_subsys_state *parent_css)
{
	ഘടന cpuacct *ca;

	എങ്കിൽ (!parent_css)
		തിരികെ &വേര്_cpuacct.css;

	ca = kzalloc_obj(*ca);
	എങ്കിൽ (!ca)
		പോകുക out;

	ca->cpuusage = alloc_percpu(u64);
	എങ്കിൽ (!ca->cpuusage)
		പോകുക out_free_ca;

	ca->cpustat = alloc_percpu(ഘടന kernel_cpustat);
	എങ്കിൽ (!ca->cpustat)
		പോകുക out_free_cpuusage;

	തിരികെ &ca->css;

out_free_cpuusage:
	free_percpu(ca->cpuusage);
out_free_ca:
	kfree(ca);
out:
	തിരികെ ERR_PTR(-ENOMEM);
}

/* Destroy an existing CPU accounting group */
സ്ഥിതം ശൂന്യം cpuacct_css_free(ഘടന cgroup_subsys_state *css)
{
	ഘടന cpuacct *ca = css_ca(css);

	free_percpu(ca->cpustat);
	free_percpu(ca->cpuusage);
	kfree(ca);
}

സ്ഥിതം u64 cpuacct_cpuusage_read(ഘടന cpuacct *ca, പൂർണ്ണസംഖ്യ cpu,
				 എണ്ണൽ cpuacct_stat_index index)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;
	u64 data;

	/*
	 * We allow index == CPUACCT_STAT_NSTATS here to read
	 * the sum of usages.
	 */
	എങ്കിൽ (WARN_ON_ONCE(index > CPUACCT_STAT_NSTATS))
		തിരികെ 0;

#ifndef CONFIG_64BIT
	/*
	 * Take rq->lock to make 64-bit read safe on 32-bit platforms.
	 */
	raw_spin_rq_lock_irq(cpu_rq(cpu));
#endif

	തിരഞ്ഞെടുപ്പ് (index) {
	അവസ്ഥ CPUACCT_STAT_USER:
		data = cpustat[CPUTIME_USER] + cpustat[CPUTIME_NICE];
		നിർത്തുക;
	അവസ്ഥ CPUACCT_STAT_SYSTEM:
		data = cpustat[CPUTIME_SYSTEM] + cpustat[CPUTIME_IRQ] +
			cpustat[CPUTIME_SOFTIRQ];
		നിർത്തുക;
	അവസ്ഥ CPUACCT_STAT_NSTATS:
		data = *cpuusage;
		നിർത്തുക;
	}

#ifndef CONFIG_64BIT
	raw_spin_rq_unlock_irq(cpu_rq(cpu));
#endif

	തിരികെ data;
}

സ്ഥിതം ശൂന്യം cpuacct_cpuusage_write(ഘടന cpuacct *ca, പൂർണ്ണസംഖ്യ cpu)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;

	/* Don't allow to reset global kernel_cpustat */
	എങ്കിൽ (ca == &വേര്_cpuacct)
		തിരികെ;

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
സ്ഥിതം u64 __cpuusage_read(ഘടന cgroup_subsys_state *css,
			   എണ്ണൽ cpuacct_stat_index index)
{
	ഘടന cpuacct *ca = css_ca(css);
	u64 totalcpuusage = 0;
	പൂർണ്ണസംഖ്യ i;

	for_each_possible_cpu(i)
		totalcpuusage += cpuacct_cpuusage_read(ca, i, index);

	തിരികെ totalcpuusage;
}

സ്ഥിതം u64 cpuusage_user_read(ഘടന cgroup_subsys_state *css,
			      ഘടന cftype *cft)
{
	തിരികെ __cpuusage_read(css, CPUACCT_STAT_USER);
}

സ്ഥിതം u64 cpuusage_sys_read(ഘടന cgroup_subsys_state *css,
			     ഘടന cftype *cft)
{
	തിരികെ __cpuusage_read(css, CPUACCT_STAT_SYSTEM);
}

സ്ഥിതം u64 cpuusage_read(ഘടന cgroup_subsys_state *css, ഘടന cftype *cft)
{
	തിരികെ __cpuusage_read(css, CPUACCT_STAT_NSTATS);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cpuusage_write(ഘടന cgroup_subsys_state *css, ഘടന cftype *cft,
			  u64 val)
{
	ഘടന cpuacct *ca = css_ca(css);
	പൂർണ്ണസംഖ്യ cpu;

	/*
	 * Only allow '0' here to do a reset.
	 */
	എങ്കിൽ (val)
		തിരികെ -EINVAL;

	for_each_possible_cpu(cpu)
		cpuacct_cpuusage_write(ca, cpu);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ __cpuacct_percpu_seq_show(ഘടന seq_file *m,
				     എണ്ണൽ cpuacct_stat_index index)
{
	ഘടന cpuacct *ca = css_ca(seq_css(m));
	u64 percpu;
	പൂർണ്ണസംഖ്യ i;

	for_each_possible_cpu(i) {
		percpu = cpuacct_cpuusage_read(ca, i, index);
		seq_printf(m, "%llu ", (unsigned long long) percpu);
	}
	seq_printf(m, "\n");
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cpuacct_percpu_user_seq_show(ഘടന seq_file *m, ശൂന്യം *V)
{
	തിരികെ __cpuacct_percpu_seq_show(m, CPUACCT_STAT_USER);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cpuacct_percpu_sys_seq_show(ഘടന seq_file *m, ശൂന്യം *V)
{
	തിരികെ __cpuacct_percpu_seq_show(m, CPUACCT_STAT_SYSTEM);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cpuacct_percpu_seq_show(ഘടന seq_file *m, ശൂന്യം *V)
{
	തിരികെ __cpuacct_percpu_seq_show(m, CPUACCT_STAT_NSTATS);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cpuacct_all_seq_show(ഘടന seq_file *m, ശൂന്യം *V)
{
	ഘടന cpuacct *ca = css_ca(seq_css(m));
	പൂർണ്ണസംഖ്യ index;
	പൂർണ്ണസംഖ്യ cpu;

	seq_puts(m, "cpu");
	വേണ്ടി (index = 0; index < CPUACCT_STAT_NSTATS; index++)
		seq_printf(m, " %s", cpuacct_stat_desc[index]);
	seq_puts(m, "\n");

	for_each_possible_cpu(cpu) {
		seq_printf(m, "%d", cpu);
		വേണ്ടി (index = 0; index < CPUACCT_STAT_NSTATS; index++)
			seq_printf(m, " %llu",
				   cpuacct_cpuusage_read(ca, cpu, index));
		seq_puts(m, "\n");
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cpuacct_stats_show(ഘടന seq_file *sf, ശൂന്യം *v)
{
	ഘടന cpuacct *ca = css_ca(seq_css(sf));
	ഘടന task_cputime cputime;
	u64 val[CPUACCT_STAT_NSTATS];
	പൂർണ്ണസംഖ്യ cpu;
	പൂർണ്ണസംഖ്യ stat;

	memset(&cputime, 0, വലുപ്പം(cputime));
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

	വേണ്ടി (stat = 0; stat < CPUACCT_STAT_NSTATS; stat++) {
		seq_printf(sf, "%s %llu\n", cpuacct_stat_desc[stat],
			nsec_to_clock_t(val[stat]));
	}

	തിരികെ 0;
}

സ്ഥിതം ഘടന cftype files[] = {
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
ശൂന്യം cpuacct_charge(ഘടന task_struct *tsk, u64 cputime)
{
	unsigned പൂർണ്ണസംഖ്യ cpu = task_cpu(tsk);
	ഘടന cpuacct *ca;

	lockdep_assert_rq_held(cpu_rq(cpu));

	വേണ്ടി (ca = task_ca(tsk); ca; ca = parent_ca(ca))
		*per_cpu_ptr(ca->cpuusage, cpu) += cputime;
}

/*
 * Add user/system time to cpuacct.
 *
 * Note: it's the caller that updates the account of the root cgroup.
 */
ശൂന്യം cpuacct_account_field(ഘടന task_struct *tsk, പൂർണ്ണസംഖ്യ index, u64 val)
{
	ഘടന cpuacct *ca;

	വേണ്ടി (ca = task_ca(tsk); ca != &വേര്_cpuacct; ca = parent_ca(ca))
		__this_cpu_add(ca->cpustat->cpustat[index], val);
}

ഘടന cgroup_subsys cpuacct_cgrp_subsys = {
	.css_alloc	= cpuacct_css_alloc,
	.css_free	= cpuacct_css_free,
	.legacy_cftypes	= files,
	.early_init	= true,
};
