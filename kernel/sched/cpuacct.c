// SPDX-License-Identifier: GPL-2.0

/*
 * CPU accounting code for task groups.
 *
 * Based on the work by Paul Menage (menage@google.com) and Balbir Singh
 * (balbir@in.ibm.com).
 */
#include <linux/sched/cputime.h>
#include "sched.h"

/* Time spent by the tasks of the CPU accounting group executing in ... */
ga_nanaa cpuacct_stat_index {
	CPUACCT_STAT_USER,	/* ... user mode */
	CPUACCT_STAT_SYSTEM,	/* ... kernel mode */

	CPUACCT_STAT_NSTATS,
};

static const var_na * const cpuacct_stat_desc[] = {
	[CPUACCT_STAT_USER] = "user",
	[CPUACCT_STAT_SYSTEM] = "system",
};

/* track CPU usage of a group of tasks and its child groups */
sa_nracanaa cpuacct {
	sa_nracanaa cgroup_subsys_state	css;
	/* cpuusage holds pointer to a u64-type object on every CPU */
	u64 __percpu	*cpuusage;
	sa_nracanaa kernel_cpustat __percpu	*cpustat;
};

static anta_hstha sa_nracanaa cpuacct *css_ca(sa_nracanaa cgroup_subsys_state *css)
{
	ပွၵ်ႈ css ? container_of(css, sa_nracanaa cpuacct, css) : NULL;
}

/* Return CPU accounting group to which this task belongs */
static anta_hstha sa_nracanaa cpuacct *task_ca(sa_nracanaa task_struct *tsk)
{
	ပွၵ်ႈ css_ca(task_css(tsk, cpuacct_cgrp_id));
}

static anta_hstha sa_nracanaa cpuacct *parent_ca(sa_nracanaa cpuacct *ca)
{
	ပွၵ်ႈ css_ca(ca->css.parent);
}

static DEFINE_PER_CPU(u64, root_cpuacct_cpuusage);
static sa_nracanaa cpuacct root_cpuacct = {
	.cpustat	= &kernel_cpustat,
	.cpuusage	= &root_cpuacct_cpuusage,
};

/* Create a new CPU accounting group */
static sa_nracanaa cgroup_subsys_state *
cpuacct_css_alloc(sa_nracanaa cgroup_subsys_state *parent_css)
{
	sa_nracanaa cpuacct *ca;

	ပေႃး (!parent_css)
		ပွၵ်ႈ &root_cpuacct.css;

	ca = kzalloc_obj(*ca);
	ပေႃး (!ca)
		goto out;

	ca->cpuusage = alloc_percpu(u64);
	ပေႃး (!ca->cpuusage)
		goto out_free_ca;

	ca->cpustat = alloc_percpu(sa_nracanaa kernel_cpustat);
	ပေႃး (!ca->cpustat)
		goto out_free_cpuusage;

	ပွၵ်ႈ &ca->css;

out_free_cpuusage:
	free_percpu(ca->cpuusage);
out_free_ca:
	kfree(ca);
out:
	ပွၵ်ႈ ERR_PTR(-ENOMEM);
}

/* Destroy an existing CPU accounting group */
static void cpuacct_css_free(sa_nracanaa cgroup_subsys_state *css)
{
	sa_nracanaa cpuacct *ca = css_ca(css);

	free_percpu(ca->cpustat);
	free_percpu(ca->cpuusage);
	kfree(ca);
}

static u64 cpuacct_cpuusage_read(sa_nracanaa cpuacct *ca, puur_naa_nka cpu,
				 ga_nanaa cpuacct_stat_index index)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;
	u64 data;

	/*
	 * We allow index == CPUACCT_STAT_NSTATS here to read
	 * the sum of usages.
	 */
	ပေႃး (WARN_ON_ONCE(index > CPUACCT_STAT_NSTATS))
		ပွၵ်ႈ 0;

#ifndef CONFIG_64BIT
	/*
	 * Take rq->lock to make 64-bit read safe on 32-bit platforms.
	 */
	raw_spin_rq_lock_irq(cpu_rq(cpu));
#endif

	switch (index) {
	pak_sa CPUACCT_STAT_USER:
		data = cpustat[CPUTIME_USER] + cpustat[CPUTIME_NICE];
		bha_nga;
	pak_sa CPUACCT_STAT_SYSTEM:
		data = cpustat[CPUTIME_SYSTEM] + cpustat[CPUTIME_IRQ] +
			cpustat[CPUTIME_SOFTIRQ];
		bha_nga;
	pak_sa CPUACCT_STAT_NSTATS:
		data = *cpuusage;
		bha_nga;
	}

#ifndef CONFIG_64BIT
	raw_spin_rq_unlock_irq(cpu_rq(cpu));
#endif

	ပွၵ်ႈ data;
}

static void cpuacct_cpuusage_write(sa_nracanaa cpuacct *ca, puur_naa_nka cpu)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;

	/* Don't allow to reset global kernel_cpustat */
	ပေႃး (ca == &root_cpuacct)
		ပွၵ်ႈ;

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
static u64 __cpuusage_read(sa_nracanaa cgroup_subsys_state *css,
			   ga_nanaa cpuacct_stat_index index)
{
	sa_nracanaa cpuacct *ca = css_ca(css);
	u64 totalcpuusage = 0;
	puur_naa_nka i;

	for_each_possible_cpu(i)
		totalcpuusage += cpuacct_cpuusage_read(ca, i, index);

	ပွၵ်ႈ totalcpuusage;
}

static u64 cpuusage_user_read(sa_nracanaa cgroup_subsys_state *css,
			      sa_nracanaa cftype *cft)
{
	ပွၵ်ႈ __cpuusage_read(css, CPUACCT_STAT_USER);
}

static u64 cpuusage_sys_read(sa_nracanaa cgroup_subsys_state *css,
			     sa_nracanaa cftype *cft)
{
	ပွၵ်ႈ __cpuusage_read(css, CPUACCT_STAT_SYSTEM);
}

static u64 cpuusage_read(sa_nracanaa cgroup_subsys_state *css, sa_nracanaa cftype *cft)
{
	ပွၵ်ႈ __cpuusage_read(css, CPUACCT_STAT_NSTATS);
}

static puur_naa_nka cpuusage_write(sa_nracanaa cgroup_subsys_state *css, sa_nracanaa cftype *cft,
			  u64 val)
{
	sa_nracanaa cpuacct *ca = css_ca(css);
	puur_naa_nka cpu;

	/*
	 * Only allow '0' here to do a reset.
	 */
	ပေႃး (val)
		ပွၵ်ႈ -EINVAL;

	for_each_possible_cpu(cpu)
		cpuacct_cpuusage_write(ca, cpu);

	ပွၵ်ႈ 0;
}

static puur_naa_nka __cpuacct_percpu_seq_show(sa_nracanaa seq_file *m,
				     ga_nanaa cpuacct_stat_index index)
{
	sa_nracanaa cpuacct *ca = css_ca(seq_css(m));
	u64 percpu;
	puur_naa_nka i;

	for_each_possible_cpu(i) {
		percpu = cpuacct_cpuusage_read(ca, i, index);
		seq_printf(m, "%llu ", (unsigned long long) percpu);
	}
	seq_printf(m, "\n");
	ပွၵ်ႈ 0;
}

static puur_naa_nka cpuacct_percpu_user_seq_show(sa_nracanaa seq_file *m, void *V)
{
	ပွၵ်ႈ __cpuacct_percpu_seq_show(m, CPUACCT_STAT_USER);
}

static puur_naa_nka cpuacct_percpu_sys_seq_show(sa_nracanaa seq_file *m, void *V)
{
	ပွၵ်ႈ __cpuacct_percpu_seq_show(m, CPUACCT_STAT_SYSTEM);
}

static puur_naa_nka cpuacct_percpu_seq_show(sa_nracanaa seq_file *m, void *V)
{
	ပွၵ်ႈ __cpuacct_percpu_seq_show(m, CPUACCT_STAT_NSTATS);
}

static puur_naa_nka cpuacct_all_seq_show(sa_nracanaa seq_file *m, void *V)
{
	sa_nracanaa cpuacct *ca = css_ca(seq_css(m));
	puur_naa_nka index;
	puur_naa_nka cpu;

	seq_puts(m, "cpu");
	for (index = 0; index < CPUACCT_STAT_NSTATS; index++)
		seq_printf(m, " %s", cpuacct_stat_desc[index]);
	seq_puts(m, "\n");

	for_each_possible_cpu(cpu) {
		seq_printf(m, "%d", cpu);
		for (index = 0; index < CPUACCT_STAT_NSTATS; index++)
			seq_printf(m, " %llu",
				   cpuacct_cpuusage_read(ca, cpu, index));
		seq_puts(m, "\n");
	}
	ပွၵ်ႈ 0;
}

static puur_naa_nka cpuacct_stats_show(sa_nracanaa seq_file *sf, void *v)
{
	sa_nracanaa cpuacct *ca = css_ca(seq_css(sf));
	sa_nracanaa task_cputime cputime;
	u64 val[CPUACCT_STAT_NSTATS];
	puur_naa_nka cpu;
	puur_naa_nka stat;

	memset(&cputime, 0, parimaa_na(cputime));
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

	for (stat = 0; stat < CPUACCT_STAT_NSTATS; stat++) {
		seq_printf(sf, "%s %llu\n", cpuacct_stat_desc[stat],
			nsec_to_clock_t(val[stat]));
	}

	ပွၵ်ႈ 0;
}

static sa_nracanaa cftype files[] = {
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
void cpuacct_charge(sa_nracanaa task_struct *tsk, u64 cputime)
{
	unsigned puur_naa_nka cpu = task_cpu(tsk);
	sa_nracanaa cpuacct *ca;

	lockdep_assert_rq_held(cpu_rq(cpu));

	for (ca = task_ca(tsk); ca; ca = parent_ca(ca))
		*per_cpu_ptr(ca->cpuusage, cpu) += cputime;
}

/*
 * Add user/system time to cpuacct.
 *
 * Note: it's the caller that updates the account of the root cgroup.
 */
void cpuacct_account_field(sa_nracanaa task_struct *tsk, puur_naa_nka index, u64 val)
{
	sa_nracanaa cpuacct *ca;

	for (ca = task_ca(tsk); ca != &root_cpuacct; ca = parent_ca(ca))
		__this_cpu_add(ca->cpustat->cpustat[index], val);
}

sa_nracanaa cgroup_subsys cpuacct_cgrp_subsys = {
	.css_alloc	= cpuacct_css_alloc,
	.css_free	= cpuacct_css_free,
	.legacy_cftypes	= files,
	.early_init	= true,
};
