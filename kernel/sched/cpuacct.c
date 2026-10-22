// SPDX-License-Identifier: GPL-2.0

/*
 * CPU accounting code for task groups.
 *
 * Based on the work by Paul Menage (menage@google.com) and Balbir Singh
 * (balbir@in.ibm.com).
 */
#ଅନ୍ତର୍ଭାଵ <linux/sched/cputime.h>
#ଅନ୍ତର୍ଭାଵ "sched.h"

/* Time spent by the tasks of the CPU accounting group executing in ... */
ଗଣନା cpuacct_stat_index {
	CPUACCT_STAT_USER,	/* ... user mode */
	CPUACCT_STAT_SYSTEM,	/* ... kernel mode */

	CPUACCT_STAT_NSTATS,
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ cpuacct_stat_desc[] = {
	[CPUACCT_STAT_USER] = "user",
	[CPUACCT_STAT_SYSTEM] = "system",
};

/* track CPU usage of a group of tasks and its child groups */
ସଂରଚନା cpuacct {
	ସଂରଚନା cgroup_subsys_state	css;
	/* cpuusage holds pointer to a u64-type object on every CPU */
	u64 __percpu	*cpuusage;
	ସଂରଚନା kernel_cpustat __percpu	*cpustat;
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା cpuacct *css_ca(ସଂରଚନା cgroup_subsys_state *css)
{
	ଫେରନ୍ତୁ css ? container_of(css, ସଂରଚନା cpuacct, css) : ଖାଲି;
}

/* Return CPU accounting group to which this task belongs */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା cpuacct *task_ca(ସଂରଚନା task_struct *tsk)
{
	ଫେରନ୍ତୁ css_ca(task_css(tsk, cpuacct_cgrp_id));
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା cpuacct *parent_ca(ସଂରଚନା cpuacct *ca)
{
	ଫେରନ୍ତୁ css_ca(ca->css.parent);
}

ସ୍ଥିର DEFINE_PER_CPU(u64, ମୂଳ_cpuacct_cpuusage);
ସ୍ଥିର ସଂରଚନା cpuacct ମୂଳ_cpuacct = {
	.cpustat	= &kernel_cpustat,
	.cpuusage	= &ମୂଳ_cpuacct_cpuusage,
};

/* Create a new CPU accounting group */
ସ୍ଥିର ସଂରଚନା cgroup_subsys_state *
cpuacct_css_alloc(ସଂରଚନା cgroup_subsys_state *parent_css)
{
	ସଂରଚନା cpuacct *ca;

	ଯଦି (!parent_css)
		ଫେରନ୍ତୁ &ମୂଳ_cpuacct.css;

	ca = kzalloc_obj(*ca);
	ଯଦି (!ca)
		ଯାଅ out;

	ca->cpuusage = alloc_percpu(u64);
	ଯଦି (!ca->cpuusage)
		ଯାଅ out_free_ca;

	ca->cpustat = alloc_percpu(ସଂରଚନା kernel_cpustat);
	ଯଦି (!ca->cpustat)
		ଯାଅ out_free_cpuusage;

	ଫେରନ୍ତୁ &ca->css;

out_free_cpuusage:
	free_percpu(ca->cpuusage);
out_free_ca:
	kfree(ca);
out:
	ଫେରନ୍ତୁ ERR_PTR(-ENOMEM);
}

/* Destroy an existing CPU accounting group */
ସ୍ଥିର ଶୂନ୍ୟ cpuacct_css_free(ସଂରଚନା cgroup_subsys_state *css)
{
	ସଂରଚନା cpuacct *ca = css_ca(css);

	free_percpu(ca->cpustat);
	free_percpu(ca->cpuusage);
	kfree(ca);
}

ସ୍ଥିର u64 cpuacct_cpuusage_read(ସଂରଚନା cpuacct *ca, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu,
				 ଗଣନା cpuacct_stat_index index)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;
	u64 data;

	/*
	 * We allow index == CPUACCT_STAT_NSTATS here to read
	 * the sum of usages.
	 */
	ଯଦି (WARN_ON_ONCE(index > CPUACCT_STAT_NSTATS))
		ଫେରନ୍ତୁ 0;

#ifndef CONFIG_64BIT
	/*
	 * Take rq->lock to make 64-bit read safe on 32-bit platforms.
	 */
	raw_spin_rq_lock_irq(cpu_rq(cpu));
#endif

	ବାଛ (index) {
	ସ୍ଥିତି CPUACCT_STAT_USER:
		data = cpustat[CPUTIME_USER] + cpustat[CPUTIME_NICE];
		ଭାଙ୍ଗ;
	ସ୍ଥିତି CPUACCT_STAT_SYSTEM:
		data = cpustat[CPUTIME_SYSTEM] + cpustat[CPUTIME_IRQ] +
			cpustat[CPUTIME_SOFTIRQ];
		ଭାଙ୍ଗ;
	ସ୍ଥିତି CPUACCT_STAT_NSTATS:
		data = *cpuusage;
		ଭାଙ୍ଗ;
	}

#ifndef CONFIG_64BIT
	raw_spin_rq_unlock_irq(cpu_rq(cpu));
#endif

	ଫେରନ୍ତୁ data;
}

ସ୍ଥିର ଶୂନ୍ୟ cpuacct_cpuusage_write(ସଂରଚନା cpuacct *ca, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;

	/* Don't allow to reset global kernel_cpustat */
	ଯଦି (ca == &ମୂଳ_cpuacct)
		ଫେରନ୍ତୁ;

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
ସ୍ଥିର u64 __cpuusage_read(ସଂରଚନା cgroup_subsys_state *css,
			   ଗଣନା cpuacct_stat_index index)
{
	ସଂରଚନା cpuacct *ca = css_ca(css);
	u64 totalcpuusage = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	for_each_possible_cpu(i)
		totalcpuusage += cpuacct_cpuusage_read(ca, i, index);

	ଫେରନ୍ତୁ totalcpuusage;
}

ସ୍ଥିର u64 cpuusage_user_read(ସଂରଚନା cgroup_subsys_state *css,
			      ସଂରଚନା cftype *cft)
{
	ଫେରନ୍ତୁ __cpuusage_read(css, CPUACCT_STAT_USER);
}

ସ୍ଥିର u64 cpuusage_sys_read(ସଂରଚନା cgroup_subsys_state *css,
			     ସଂରଚନା cftype *cft)
{
	ଫେରନ୍ତୁ __cpuusage_read(css, CPUACCT_STAT_SYSTEM);
}

ସ୍ଥିର u64 cpuusage_read(ସଂରଚନା cgroup_subsys_state *css, ସଂରଚନା cftype *cft)
{
	ଫେରନ୍ତୁ __cpuusage_read(css, CPUACCT_STAT_NSTATS);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpuusage_write(ସଂରଚନା cgroup_subsys_state *css, ସଂରଚନା cftype *cft,
			  u64 val)
{
	ସଂରଚନା cpuacct *ca = css_ca(css);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

	/*
	 * Only allow '0' here to do a reset.
	 */
	ଯଦି (val)
		ଫେରନ୍ତୁ -EINVAL;

	for_each_possible_cpu(cpu)
		cpuacct_cpuusage_write(ca, cpu);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __cpuacct_percpu_seq_show(ସଂରଚନା seq_file *m,
				     ଗଣନା cpuacct_stat_index index)
{
	ସଂରଚନା cpuacct *ca = css_ca(seq_css(m));
	u64 percpu;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	for_each_possible_cpu(i) {
		percpu = cpuacct_cpuusage_read(ca, i, index);
		seq_printf(m, "%llu ", (unsigned long long) percpu);
	}
	seq_printf(m, "\n");
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpuacct_percpu_user_seq_show(ସଂରଚନା seq_file *m, ଶୂନ୍ୟ *V)
{
	ଫେରନ୍ତୁ __cpuacct_percpu_seq_show(m, CPUACCT_STAT_USER);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpuacct_percpu_sys_seq_show(ସଂରଚନା seq_file *m, ଶୂନ୍ୟ *V)
{
	ଫେରନ୍ତୁ __cpuacct_percpu_seq_show(m, CPUACCT_STAT_SYSTEM);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpuacct_percpu_seq_show(ସଂରଚନା seq_file *m, ଶୂନ୍ୟ *V)
{
	ଫେରନ୍ତୁ __cpuacct_percpu_seq_show(m, CPUACCT_STAT_NSTATS);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpuacct_all_seq_show(ସଂରଚନା seq_file *m, ଶୂନ୍ୟ *V)
{
	ସଂରଚନା cpuacct *ca = css_ca(seq_css(m));
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା index;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

	seq_puts(m, "cpu");
	ପାଇଁ (index = 0; index < CPUACCT_STAT_NSTATS; index++)
		seq_printf(m, " %s", cpuacct_stat_desc[index]);
	seq_puts(m, "\n");

	for_each_possible_cpu(cpu) {
		seq_printf(m, "%d", cpu);
		ପାଇଁ (index = 0; index < CPUACCT_STAT_NSTATS; index++)
			seq_printf(m, " %llu",
				   cpuacct_cpuusage_read(ca, cpu, index));
		seq_puts(m, "\n");
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpuacct_stats_show(ସଂରଚନା seq_file *sf, ଶୂନ୍ୟ *v)
{
	ସଂରଚନା cpuacct *ca = css_ca(seq_css(sf));
	ସଂରଚନା task_cputime cputime;
	u64 val[CPUACCT_STAT_NSTATS];
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା stat;

	memset(&cputime, 0, ଆକାର(cputime));
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

	ପାଇଁ (stat = 0; stat < CPUACCT_STAT_NSTATS; stat++) {
		seq_printf(sf, "%s %llu\n", cpuacct_stat_desc[stat],
			nsec_to_clock_t(val[stat]));
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ସଂରଚନା cftype files[] = {
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
ଶୂନ୍ୟ cpuacct_charge(ସଂରଚନା task_struct *tsk, u64 cputime)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = task_cpu(tsk);
	ସଂରଚନା cpuacct *ca;

	lockdep_assert_rq_held(cpu_rq(cpu));

	ପାଇଁ (ca = task_ca(tsk); ca; ca = parent_ca(ca))
		*per_cpu_ptr(ca->cpuusage, cpu) += cputime;
}

/*
 * Add user/system time to cpuacct.
 *
 * Note: it's the caller that updates the account of the root cgroup.
 */
ଶୂନ୍ୟ cpuacct_account_field(ସଂରଚନା task_struct *tsk, ପୂର୍ଣ୍ଣସଂଖ୍ୟା index, u64 val)
{
	ସଂରଚନା cpuacct *ca;

	ପାଇଁ (ca = task_ca(tsk); ca != &ମୂଳ_cpuacct; ca = parent_ca(ca))
		__this_cpu_add(ca->cpustat->cpustat[index], val);
}

ସଂରଚନା cgroup_subsys cpuacct_cgrp_subsys = {
	.css_alloc	= cpuacct_css_alloc,
	.css_free	= cpuacct_css_free,
	.legacy_cftypes	= files,
	.early_init	= true,
};
