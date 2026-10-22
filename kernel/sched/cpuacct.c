// SPDX-License-Identifier: GPL-2.0

/*
 * CPU accounting code for task groups.
 *
 * Based on the work by Paul Menage (menage@google.com) and Balbir Singh
 * (balbir@in.ibm.com).
 */
#અન્તર્ભાવ <linux/sched/cputime.h>
#અન્તર્ભાવ "sched.h"

/* Time spent by the tasks of the CPU accounting group executing in ... */
ગણના cpuacct_stat_index {
	CPUACCT_STAT_USER,	/* ... user mode */
	CPUACCT_STAT_SYSTEM,	/* ... kernel mode */

	CPUACCT_STAT_NSTATS,
};

સ્થિર અચલ અક્ષર * અચલ cpuacct_stat_desc[] = {
	[CPUACCT_STAT_USER] = "user",
	[CPUACCT_STAT_SYSTEM] = "system",
};

/* track CPU usage of a group of tasks and its child groups */
સંરચના cpuacct {
	સંરચના cgroup_subsys_state	css;
	/* cpuusage holds pointer to a u64-type object on every CPU */
	u64 __percpu	*cpuusage;
	સંરચના kernel_cpustat __percpu	*cpustat;
};

સ્થિર અંતઃરેખ સંરચના cpuacct *css_ca(સંરચના cgroup_subsys_state *css)
{
	પાછા css ? container_of(css, સંરચના cpuacct, css) : ખાલી;
}

/* Return CPU accounting group to which this task belongs */
સ્થિર અંતઃરેખ સંરચના cpuacct *task_ca(સંરચના task_struct *tsk)
{
	પાછા css_ca(task_css(tsk, cpuacct_cgrp_id));
}

સ્થિર અંતઃરેખ સંરચના cpuacct *parent_ca(સંરચના cpuacct *ca)
{
	પાછા css_ca(ca->css.parent);
}

સ્થિર DEFINE_PER_CPU(u64, મૂળ_cpuacct_cpuusage);
સ્થિર સંરચના cpuacct મૂળ_cpuacct = {
	.cpustat	= &kernel_cpustat,
	.cpuusage	= &મૂળ_cpuacct_cpuusage,
};

/* Create a new CPU accounting group */
સ્થિર સંરચના cgroup_subsys_state *
cpuacct_css_alloc(સંરચના cgroup_subsys_state *parent_css)
{
	સંરચના cpuacct *ca;

	જો (!parent_css)
		પાછા &મૂળ_cpuacct.css;

	ca = kzalloc_obj(*ca);
	જો (!ca)
		જાઓ out;

	ca->cpuusage = alloc_percpu(u64);
	જો (!ca->cpuusage)
		જાઓ out_free_ca;

	ca->cpustat = alloc_percpu(સંરચના kernel_cpustat);
	જો (!ca->cpustat)
		જાઓ out_free_cpuusage;

	પાછા &ca->css;

out_free_cpuusage:
	free_percpu(ca->cpuusage);
out_free_ca:
	kfree(ca);
out:
	પાછા ERR_PTR(-ENOMEM);
}

/* Destroy an existing CPU accounting group */
સ્થિર શૂન્ય cpuacct_css_free(સંરચના cgroup_subsys_state *css)
{
	સંરચના cpuacct *ca = css_ca(css);

	free_percpu(ca->cpustat);
	free_percpu(ca->cpuusage);
	kfree(ca);
}

સ્થિર u64 cpuacct_cpuusage_read(સંરચના cpuacct *ca, પૂર્ણાંક cpu,
				 ગણના cpuacct_stat_index index)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;
	u64 data;

	/*
	 * We allow index == CPUACCT_STAT_NSTATS here to read
	 * the sum of usages.
	 */
	જો (WARN_ON_ONCE(index > CPUACCT_STAT_NSTATS))
		પાછા 0;

#ifndef CONFIG_64BIT
	/*
	 * Take rq->lock to make 64-bit read safe on 32-bit platforms.
	 */
	raw_spin_rq_lock_irq(cpu_rq(cpu));
#endif

	પસંદગી (index) {
	સ્થિતિ CPUACCT_STAT_USER:
		data = cpustat[CPUTIME_USER] + cpustat[CPUTIME_NICE];
		તોડો;
	સ્થિતિ CPUACCT_STAT_SYSTEM:
		data = cpustat[CPUTIME_SYSTEM] + cpustat[CPUTIME_IRQ] +
			cpustat[CPUTIME_SOFTIRQ];
		તોડો;
	સ્થિતિ CPUACCT_STAT_NSTATS:
		data = *cpuusage;
		તોડો;
	}

#ifndef CONFIG_64BIT
	raw_spin_rq_unlock_irq(cpu_rq(cpu));
#endif

	પાછા data;
}

સ્થિર શૂન્ય cpuacct_cpuusage_write(સંરચના cpuacct *ca, પૂર્ણાંક cpu)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;

	/* Don't allow to reset global kernel_cpustat */
	જો (ca == &મૂળ_cpuacct)
		પાછા;

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
સ્થિર u64 __cpuusage_read(સંરચના cgroup_subsys_state *css,
			   ગણના cpuacct_stat_index index)
{
	સંરચના cpuacct *ca = css_ca(css);
	u64 totalcpuusage = 0;
	પૂર્ણાંક i;

	for_each_possible_cpu(i)
		totalcpuusage += cpuacct_cpuusage_read(ca, i, index);

	પાછા totalcpuusage;
}

સ્થિર u64 cpuusage_user_read(સંરચના cgroup_subsys_state *css,
			      સંરચના cftype *cft)
{
	પાછા __cpuusage_read(css, CPUACCT_STAT_USER);
}

સ્થિર u64 cpuusage_sys_read(સંરચના cgroup_subsys_state *css,
			     સંરચના cftype *cft)
{
	પાછા __cpuusage_read(css, CPUACCT_STAT_SYSTEM);
}

સ્થિર u64 cpuusage_read(સંરચના cgroup_subsys_state *css, સંરચના cftype *cft)
{
	પાછા __cpuusage_read(css, CPUACCT_STAT_NSTATS);
}

સ્થિર પૂર્ણાંક cpuusage_write(સંરચના cgroup_subsys_state *css, સંરચના cftype *cft,
			  u64 val)
{
	સંરચના cpuacct *ca = css_ca(css);
	પૂર્ણાંક cpu;

	/*
	 * Only allow '0' here to do a reset.
	 */
	જો (val)
		પાછા -EINVAL;

	for_each_possible_cpu(cpu)
		cpuacct_cpuusage_write(ca, cpu);

	પાછા 0;
}

સ્થિર પૂર્ણાંક __cpuacct_percpu_seq_show(સંરચના seq_file *m,
				     ગણના cpuacct_stat_index index)
{
	સંરચના cpuacct *ca = css_ca(seq_css(m));
	u64 percpu;
	પૂર્ણાંક i;

	for_each_possible_cpu(i) {
		percpu = cpuacct_cpuusage_read(ca, i, index);
		seq_printf(m, "%llu ", (unsigned long long) percpu);
	}
	seq_printf(m, "\n");
	પાછા 0;
}

સ્થિર પૂર્ણાંક cpuacct_percpu_user_seq_show(સંરચના seq_file *m, શૂન્ય *V)
{
	પાછા __cpuacct_percpu_seq_show(m, CPUACCT_STAT_USER);
}

સ્થિર પૂર્ણાંક cpuacct_percpu_sys_seq_show(સંરચના seq_file *m, શૂન્ય *V)
{
	પાછા __cpuacct_percpu_seq_show(m, CPUACCT_STAT_SYSTEM);
}

સ્થિર પૂર્ણાંક cpuacct_percpu_seq_show(સંરચના seq_file *m, શૂન્ય *V)
{
	પાછા __cpuacct_percpu_seq_show(m, CPUACCT_STAT_NSTATS);
}

સ્થિર પૂર્ણાંક cpuacct_all_seq_show(સંરચના seq_file *m, શૂન્ય *V)
{
	સંરચના cpuacct *ca = css_ca(seq_css(m));
	પૂર્ણાંક index;
	પૂર્ણાંક cpu;

	seq_puts(m, "cpu");
	માટે (index = 0; index < CPUACCT_STAT_NSTATS; index++)
		seq_printf(m, " %s", cpuacct_stat_desc[index]);
	seq_puts(m, "\n");

	for_each_possible_cpu(cpu) {
		seq_printf(m, "%d", cpu);
		માટે (index = 0; index < CPUACCT_STAT_NSTATS; index++)
			seq_printf(m, " %llu",
				   cpuacct_cpuusage_read(ca, cpu, index));
		seq_puts(m, "\n");
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક cpuacct_stats_show(સંરચના seq_file *sf, શૂન્ય *v)
{
	સંરચના cpuacct *ca = css_ca(seq_css(sf));
	સંરચના task_cputime cputime;
	u64 val[CPUACCT_STAT_NSTATS];
	પૂર્ણાંક cpu;
	પૂર્ણાંક stat;

	memset(&cputime, 0, કદ(cputime));
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

	માટે (stat = 0; stat < CPUACCT_STAT_NSTATS; stat++) {
		seq_printf(sf, "%s %llu\n", cpuacct_stat_desc[stat],
			nsec_to_clock_t(val[stat]));
	}

	પાછા 0;
}

સ્થિર સંરચના cftype files[] = {
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
શૂન્ય cpuacct_charge(સંરચના task_struct *tsk, u64 cputime)
{
	unsigned પૂર્ણાંક cpu = task_cpu(tsk);
	સંરચના cpuacct *ca;

	lockdep_assert_rq_held(cpu_rq(cpu));

	માટે (ca = task_ca(tsk); ca; ca = parent_ca(ca))
		*per_cpu_ptr(ca->cpuusage, cpu) += cputime;
}

/*
 * Add user/system time to cpuacct.
 *
 * Note: it's the caller that updates the account of the root cgroup.
 */
શૂન્ય cpuacct_account_field(સંરચના task_struct *tsk, પૂર્ણાંક index, u64 val)
{
	સંરચના cpuacct *ca;

	માટે (ca = task_ca(tsk); ca != &મૂળ_cpuacct; ca = parent_ca(ca))
		__this_cpu_add(ca->cpustat->cpustat[index], val);
}

સંરચના cgroup_subsys cpuacct_cgrp_subsys = {
	.css_alloc	= cpuacct_css_alloc,
	.css_free	= cpuacct_css_free,
	.legacy_cftypes	= files,
	.early_init	= true,
};
