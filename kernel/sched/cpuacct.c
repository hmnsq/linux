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
	ສົ່ງຄືນ css ? container_of(css, sa_nracanaa cpuacct, css) : NULL;
}

/* Return CPU accounting group to which this task belongs */
static anta_hstha sa_nracanaa cpuacct *task_ca(sa_nracanaa task_struct *tsk)
{
	ສົ່ງຄືນ css_ca(task_css(tsk, cpuacct_cgrp_id));
}

static anta_hstha sa_nracanaa cpuacct *parent_ca(sa_nracanaa cpuacct *ca)
{
	ສົ່ງຄືນ css_ca(ca->css.parent);
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

	ຖ້າ (!parent_css)
		ສົ່ງຄືນ &root_cpuacct.css;

	ca = kzalloc_obj(*ca);
	ຖ້າ (!ca)
		goto out;

	ca->cpuusage = alloc_percpu(u64);
	ຖ້າ (!ca->cpuusage)
		goto out_free_ca;

	ca->cpustat = alloc_percpu(sa_nracanaa kernel_cpustat);
	ຖ້າ (!ca->cpustat)
		goto out_free_cpuusage;

	ສົ່ງຄືນ &ca->css;

out_free_cpuusage:
	free_percpu(ca->cpuusage);
out_free_ca:
	kfree(ca);
out:
	ສົ່ງຄືນ ERR_PTR(-ENOMEM);
}

/* Destroy an existing CPU accounting group */
static void cpuacct_css_free(sa_nracanaa cgroup_subsys_state *css)
{
	sa_nracanaa cpuacct *ca = css_ca(css);

	free_percpu(ca->cpustat);
	free_percpu(ca->cpuusage);
	kfree(ca);
}

static u64 cpuacct_cpuusage_read(sa_nracanaa cpuacct *ca, ຈຳນວນເຕັມ cpu,
				 ga_nanaa cpuacct_stat_index index)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;
	u64 data;

	/*
	 * We allow index == CPUACCT_STAT_NSTATS here to read
	 * the sum of usages.
	 */
	ຖ້າ (WARN_ON_ONCE(index > CPUACCT_STAT_NSTATS))
		ສົ່ງຄືນ 0;

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

	ສົ່ງຄືນ data;
}

static void cpuacct_cpuusage_write(sa_nracanaa cpuacct *ca, ຈຳນວນເຕັມ cpu)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;

	/* Don't allow to reset global kernel_cpustat */
	ຖ້າ (ca == &root_cpuacct)
		ສົ່ງຄືນ;

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
	ຈຳນວນເຕັມ i;

	for_each_possible_cpu(i)
		totalcpuusage += cpuacct_cpuusage_read(ca, i, index);

	ສົ່ງຄືນ totalcpuusage;
}

static u64 cpuusage_user_read(sa_nracanaa cgroup_subsys_state *css,
			      sa_nracanaa cftype *cft)
{
	ສົ່ງຄືນ __cpuusage_read(css, CPUACCT_STAT_USER);
}

static u64 cpuusage_sys_read(sa_nracanaa cgroup_subsys_state *css,
			     sa_nracanaa cftype *cft)
{
	ສົ່ງຄືນ __cpuusage_read(css, CPUACCT_STAT_SYSTEM);
}

static u64 cpuusage_read(sa_nracanaa cgroup_subsys_state *css, sa_nracanaa cftype *cft)
{
	ສົ່ງຄືນ __cpuusage_read(css, CPUACCT_STAT_NSTATS);
}

static ຈຳນວນເຕັມ cpuusage_write(sa_nracanaa cgroup_subsys_state *css, sa_nracanaa cftype *cft,
			  u64 val)
{
	sa_nracanaa cpuacct *ca = css_ca(css);
	ຈຳນວນເຕັມ cpu;

	/*
	 * Only allow '0' here to do a reset.
	 */
	ຖ້າ (val)
		ສົ່ງຄືນ -EINVAL;

	for_each_possible_cpu(cpu)
		cpuacct_cpuusage_write(ca, cpu);

	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ __cpuacct_percpu_seq_show(sa_nracanaa seq_file *m,
				     ga_nanaa cpuacct_stat_index index)
{
	sa_nracanaa cpuacct *ca = css_ca(seq_css(m));
	u64 percpu;
	ຈຳນວນເຕັມ i;

	for_each_possible_cpu(i) {
		percpu = cpuacct_cpuusage_read(ca, i, index);
		seq_printf(m, "%llu ", (unsigned long long) percpu);
	}
	seq_printf(m, "\n");
	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ cpuacct_percpu_user_seq_show(sa_nracanaa seq_file *m, void *V)
{
	ສົ່ງຄືນ __cpuacct_percpu_seq_show(m, CPUACCT_STAT_USER);
}

static ຈຳນວນເຕັມ cpuacct_percpu_sys_seq_show(sa_nracanaa seq_file *m, void *V)
{
	ສົ່ງຄືນ __cpuacct_percpu_seq_show(m, CPUACCT_STAT_SYSTEM);
}

static ຈຳນວນເຕັມ cpuacct_percpu_seq_show(sa_nracanaa seq_file *m, void *V)
{
	ສົ່ງຄືນ __cpuacct_percpu_seq_show(m, CPUACCT_STAT_NSTATS);
}

static ຈຳນວນເຕັມ cpuacct_all_seq_show(sa_nracanaa seq_file *m, void *V)
{
	sa_nracanaa cpuacct *ca = css_ca(seq_css(m));
	ຈຳນວນເຕັມ index;
	ຈຳນວນເຕັມ cpu;

	seq_puts(m, "cpu");
	ສຳລັບ (index = 0; index < CPUACCT_STAT_NSTATS; index++)
		seq_printf(m, " %s", cpuacct_stat_desc[index]);
	seq_puts(m, "\n");

	for_each_possible_cpu(cpu) {
		seq_printf(m, "%d", cpu);
		ສຳລັບ (index = 0; index < CPUACCT_STAT_NSTATS; index++)
			seq_printf(m, " %llu",
				   cpuacct_cpuusage_read(ca, cpu, index));
		seq_puts(m, "\n");
	}
	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ cpuacct_stats_show(sa_nracanaa seq_file *sf, void *v)
{
	sa_nracanaa cpuacct *ca = css_ca(seq_css(sf));
	sa_nracanaa task_cputime cputime;
	u64 val[CPUACCT_STAT_NSTATS];
	ຈຳນວນເຕັມ cpu;
	ຈຳນວນເຕັມ stat;

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

	ສຳລັບ (stat = 0; stat < CPUACCT_STAT_NSTATS; stat++) {
		seq_printf(sf, "%s %llu\n", cpuacct_stat_desc[stat],
			nsec_to_clock_t(val[stat]));
	}

	ສົ່ງຄືນ 0;
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
	unsigned ຈຳນວນເຕັມ cpu = task_cpu(tsk);
	sa_nracanaa cpuacct *ca;

	lockdep_assert_rq_held(cpu_rq(cpu));

	ສຳລັບ (ca = task_ca(tsk); ca; ca = parent_ca(ca))
		*per_cpu_ptr(ca->cpuusage, cpu) += cputime;
}

/*
 * Add user/system time to cpuacct.
 *
 * Note: it's the caller that updates the account of the root cgroup.
 */
void cpuacct_account_field(sa_nracanaa task_struct *tsk, ຈຳນວນເຕັມ index, u64 val)
{
	sa_nracanaa cpuacct *ca;

	ສຳລັບ (ca = task_ca(tsk); ca != &root_cpuacct; ca = parent_ca(ca))
		__this_cpu_add(ca->cpustat->cpustat[index], val);
}

sa_nracanaa cgroup_subsys cpuacct_cgrp_subsys = {
	.css_alloc	= cpuacct_css_alloc,
	.css_free	= cpuacct_css_free,
	.legacy_cftypes	= files,
	.early_init	= true,
};
