// SPDX-License-Identifier: GPL-2.0

/*
 * CPU accounting code for task groups.
 *
 * Based on the work by Paul Menage (menage@google.com) and Balbir Singh
 * (balbir@in.ibm.com).
 */
#அந்தர்பாவ <linux/sched/cputime.h>
#அந்தர்பாவ "sched.h"

/* Time spent by the tasks of the CPU accounting group executing in ... */
எண்ணிக்கை cpuacct_stat_index {
	CPUACCT_STAT_USER,	/* ... user mode */
	CPUACCT_STAT_SYSTEM,	/* ... kernel mode */

	CPUACCT_STAT_NSTATS,
};

நிலையான மாறிலி எழுத்து * மாறிலி cpuacct_stat_desc[] = {
	[CPUACCT_STAT_USER] = "user",
	[CPUACCT_STAT_SYSTEM] = "system",
};

/* track CPU usage of a group of tasks and its child groups */
கட்டமைப்பு cpuacct {
	கட்டமைப்பு cgroup_subsys_state	css;
	/* cpuusage holds pointer to a u64-type object on every CPU */
	u64 __percpu	*cpuusage;
	கட்டமைப்பு kernel_cpustat __percpu	*cpustat;
};

நிலையான உள்வரி கட்டமைப்பு cpuacct *css_ca(கட்டமைப்பு cgroup_subsys_state *css)
{
	திரும்பு css ? container_of(css, கட்டமைப்பு cpuacct, css) : வெற்று;
}

/* Return CPU accounting group to which this task belongs */
நிலையான உள்வரி கட்டமைப்பு cpuacct *task_ca(கட்டமைப்பு task_struct *tsk)
{
	திரும்பு css_ca(task_css(tsk, cpuacct_cgrp_id));
}

நிலையான உள்வரி கட்டமைப்பு cpuacct *parent_ca(கட்டமைப்பு cpuacct *ca)
{
	திரும்பு css_ca(ca->css.parent);
}

நிலையான DEFINE_PER_CPU(u64, வேர்_cpuacct_cpuusage);
நிலையான கட்டமைப்பு cpuacct வேர்_cpuacct = {
	.cpustat	= &kernel_cpustat,
	.cpuusage	= &வேர்_cpuacct_cpuusage,
};

/* Create a new CPU accounting group */
நிலையான கட்டமைப்பு cgroup_subsys_state *
cpuacct_css_alloc(கட்டமைப்பு cgroup_subsys_state *parent_css)
{
	கட்டமைப்பு cpuacct *ca;

	எனில் (!parent_css)
		திரும்பு &வேர்_cpuacct.css;

	ca = kzalloc_obj(*ca);
	எனில் (!ca)
		செல் out;

	ca->cpuusage = alloc_percpu(u64);
	எனில் (!ca->cpuusage)
		செல் out_free_ca;

	ca->cpustat = alloc_percpu(கட்டமைப்பு kernel_cpustat);
	எனில் (!ca->cpustat)
		செல் out_free_cpuusage;

	திரும்பு &ca->css;

out_free_cpuusage:
	free_percpu(ca->cpuusage);
out_free_ca:
	kfree(ca);
out:
	திரும்பு ERR_PTR(-ENOMEM);
}

/* Destroy an existing CPU accounting group */
நிலையான வெறுமை cpuacct_css_free(கட்டமைப்பு cgroup_subsys_state *css)
{
	கட்டமைப்பு cpuacct *ca = css_ca(css);

	free_percpu(ca->cpustat);
	free_percpu(ca->cpuusage);
	kfree(ca);
}

நிலையான u64 cpuacct_cpuusage_read(கட்டமைப்பு cpuacct *ca, முழு cpu,
				 எண்ணிக்கை cpuacct_stat_index index)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;
	u64 data;

	/*
	 * We allow index == CPUACCT_STAT_NSTATS here to read
	 * the sum of usages.
	 */
	எனில் (WARN_ON_ONCE(index > CPUACCT_STAT_NSTATS))
		திரும்பு 0;

#ifndef CONFIG_64BIT
	/*
	 * Take rq->lock to make 64-bit read safe on 32-bit platforms.
	 */
	raw_spin_rq_lock_irq(cpu_rq(cpu));
#endif

	தேர்வு (index) {
	நிலை CPUACCT_STAT_USER:
		data = cpustat[CPUTIME_USER] + cpustat[CPUTIME_NICE];
		நிறுத்து;
	நிலை CPUACCT_STAT_SYSTEM:
		data = cpustat[CPUTIME_SYSTEM] + cpustat[CPUTIME_IRQ] +
			cpustat[CPUTIME_SOFTIRQ];
		நிறுத்து;
	நிலை CPUACCT_STAT_NSTATS:
		data = *cpuusage;
		நிறுத்து;
	}

#ifndef CONFIG_64BIT
	raw_spin_rq_unlock_irq(cpu_rq(cpu));
#endif

	திரும்பு data;
}

நிலையான வெறுமை cpuacct_cpuusage_write(கட்டமைப்பு cpuacct *ca, முழு cpu)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;

	/* Don't allow to reset global kernel_cpustat */
	எனில் (ca == &வேர்_cpuacct)
		திரும்பு;

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
நிலையான u64 __cpuusage_read(கட்டமைப்பு cgroup_subsys_state *css,
			   எண்ணிக்கை cpuacct_stat_index index)
{
	கட்டமைப்பு cpuacct *ca = css_ca(css);
	u64 totalcpuusage = 0;
	முழு i;

	for_each_possible_cpu(i)
		totalcpuusage += cpuacct_cpuusage_read(ca, i, index);

	திரும்பு totalcpuusage;
}

நிலையான u64 cpuusage_user_read(கட்டமைப்பு cgroup_subsys_state *css,
			      கட்டமைப்பு cftype *cft)
{
	திரும்பு __cpuusage_read(css, CPUACCT_STAT_USER);
}

நிலையான u64 cpuusage_sys_read(கட்டமைப்பு cgroup_subsys_state *css,
			     கட்டமைப்பு cftype *cft)
{
	திரும்பு __cpuusage_read(css, CPUACCT_STAT_SYSTEM);
}

நிலையான u64 cpuusage_read(கட்டமைப்பு cgroup_subsys_state *css, கட்டமைப்பு cftype *cft)
{
	திரும்பு __cpuusage_read(css, CPUACCT_STAT_NSTATS);
}

நிலையான முழு cpuusage_write(கட்டமைப்பு cgroup_subsys_state *css, கட்டமைப்பு cftype *cft,
			  u64 val)
{
	கட்டமைப்பு cpuacct *ca = css_ca(css);
	முழு cpu;

	/*
	 * Only allow '0' here to do a reset.
	 */
	எனில் (val)
		திரும்பு -EINVAL;

	for_each_possible_cpu(cpu)
		cpuacct_cpuusage_write(ca, cpu);

	திரும்பு 0;
}

நிலையான முழு __cpuacct_percpu_seq_show(கட்டமைப்பு seq_file *m,
				     எண்ணிக்கை cpuacct_stat_index index)
{
	கட்டமைப்பு cpuacct *ca = css_ca(seq_css(m));
	u64 percpu;
	முழு i;

	for_each_possible_cpu(i) {
		percpu = cpuacct_cpuusage_read(ca, i, index);
		seq_printf(m, "%llu ", (unsigned long long) percpu);
	}
	seq_printf(m, "\n");
	திரும்பு 0;
}

நிலையான முழு cpuacct_percpu_user_seq_show(கட்டமைப்பு seq_file *m, வெறுமை *V)
{
	திரும்பு __cpuacct_percpu_seq_show(m, CPUACCT_STAT_USER);
}

நிலையான முழு cpuacct_percpu_sys_seq_show(கட்டமைப்பு seq_file *m, வெறுமை *V)
{
	திரும்பு __cpuacct_percpu_seq_show(m, CPUACCT_STAT_SYSTEM);
}

நிலையான முழு cpuacct_percpu_seq_show(கட்டமைப்பு seq_file *m, வெறுமை *V)
{
	திரும்பு __cpuacct_percpu_seq_show(m, CPUACCT_STAT_NSTATS);
}

நிலையான முழு cpuacct_all_seq_show(கட்டமைப்பு seq_file *m, வெறுமை *V)
{
	கட்டமைப்பு cpuacct *ca = css_ca(seq_css(m));
	முழு index;
	முழு cpu;

	seq_puts(m, "cpu");
	க்கு (index = 0; index < CPUACCT_STAT_NSTATS; index++)
		seq_printf(m, " %s", cpuacct_stat_desc[index]);
	seq_puts(m, "\n");

	for_each_possible_cpu(cpu) {
		seq_printf(m, "%d", cpu);
		க்கு (index = 0; index < CPUACCT_STAT_NSTATS; index++)
			seq_printf(m, " %llu",
				   cpuacct_cpuusage_read(ca, cpu, index));
		seq_puts(m, "\n");
	}
	திரும்பு 0;
}

நிலையான முழு cpuacct_stats_show(கட்டமைப்பு seq_file *sf, வெறுமை *v)
{
	கட்டமைப்பு cpuacct *ca = css_ca(seq_css(sf));
	கட்டமைப்பு task_cputime cputime;
	u64 val[CPUACCT_STAT_NSTATS];
	முழு cpu;
	முழு stat;

	memset(&cputime, 0, அளவு(cputime));
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

	க்கு (stat = 0; stat < CPUACCT_STAT_NSTATS; stat++) {
		seq_printf(sf, "%s %llu\n", cpuacct_stat_desc[stat],
			nsec_to_clock_t(val[stat]));
	}

	திரும்பு 0;
}

நிலையான கட்டமைப்பு cftype files[] = {
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
வெறுமை cpuacct_charge(கட்டமைப்பு task_struct *tsk, u64 cputime)
{
	unsigned முழு cpu = task_cpu(tsk);
	கட்டமைப்பு cpuacct *ca;

	lockdep_assert_rq_held(cpu_rq(cpu));

	க்கு (ca = task_ca(tsk); ca; ca = parent_ca(ca))
		*per_cpu_ptr(ca->cpuusage, cpu) += cputime;
}

/*
 * Add user/system time to cpuacct.
 *
 * Note: it's the caller that updates the account of the root cgroup.
 */
வெறுமை cpuacct_account_field(கட்டமைப்பு task_struct *tsk, முழு index, u64 val)
{
	கட்டமைப்பு cpuacct *ca;

	க்கு (ca = task_ca(tsk); ca != &வேர்_cpuacct; ca = parent_ca(ca))
		__this_cpu_add(ca->cpustat->cpustat[index], val);
}

கட்டமைப்பு cgroup_subsys cpuacct_cgrp_subsys = {
	.css_alloc	= cpuacct_css_alloc,
	.css_free	= cpuacct_css_free,
	.legacy_cftypes	= files,
	.early_init	= true,
};
