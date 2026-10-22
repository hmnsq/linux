// SPDX-License-Identifier: GPL-2.0

/*
 * CPU accounting code for task groups.
 *
 * Based on the work by Paul Menage (menage@google.com) and Balbir Singh
 * (balbir@in.ibm.com).
 */
#ضمن <linux/sched/cputime.h>
#ضمن "sched.h"

/* Time spent by the tasks of the CPU accounting group executing in ... */
تعداد cpuacct_stat_index {
	CPUACCT_STAT_USER,	/* ... user mode */
	CPUACCT_STAT_SYSTEM,	/* ... kernel mode */

	CPUACCT_STAT_NSTATS,
};

ساكن ثابت حرف * ثابت cpuacct_stat_desc[] = {
	[CPUACCT_STAT_USER] = "user",
	[CPUACCT_STAT_SYSTEM] = "system",
};

/* track CPU usage of a group of tasks and its child groups */
بنية cpuacct {
	بنية cgroup_subsys_state	css;
	/* cpuusage holds pointer to a u64-type object on every CPU */
	u64 __percpu	*cpuusage;
	بنية kernel_cpustat __percpu	*cpustat;
};

ساكن مضمن بنية cpuacct *css_ca(بنية cgroup_subsys_state *css)
{
	أرجع css ? container_of(css, بنية cpuacct, css) : عدم;
}

/* Return CPU accounting group to which this task belongs */
ساكن مضمن بنية cpuacct *task_ca(بنية task_struct *tsk)
{
	أرجع css_ca(task_css(tsk, cpuacct_cgrp_id));
}

ساكن مضمن بنية cpuacct *parent_ca(بنية cpuacct *ca)
{
	أرجع css_ca(ca->css.parent);
}

ساكن DEFINE_PER_CPU(u64, سيو_cpuacct_cpuusage);
ساكن بنية cpuacct سيو_cpuacct = {
	.cpustat	= &kernel_cpustat,
	.cpuusage	= &سيو_cpuacct_cpuusage,
};

/* Create a new CPU accounting group */
ساكن بنية cgroup_subsys_state *
cpuacct_css_alloc(بنية cgroup_subsys_state *parent_css)
{
	بنية cpuacct *ca;

	إذا (!parent_css)
		أرجع &سيو_cpuacct.css;

	ca = kzalloc_obj(*ca);
	إذا (!ca)
		اذهب out;

	ca->cpuusage = alloc_percpu(u64);
	إذا (!ca->cpuusage)
		اذهب out_free_ca;

	ca->cpustat = alloc_percpu(بنية kernel_cpustat);
	إذا (!ca->cpustat)
		اذهب out_free_cpuusage;

	أرجع &ca->css;

out_free_cpuusage:
	free_percpu(ca->cpuusage);
out_free_ca:
	kfree(ca);
out:
	أرجع ERR_PTR(-ENOMEM);
}

/* Destroy an existing CPU accounting group */
ساكن فراغ cpuacct_css_free(بنية cgroup_subsys_state *css)
{
	بنية cpuacct *ca = css_ca(css);

	free_percpu(ca->cpustat);
	free_percpu(ca->cpuusage);
	kfree(ca);
}

ساكن u64 cpuacct_cpuusage_read(بنية cpuacct *ca, صحيح cpu,
				 تعداد cpuacct_stat_index index)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;
	u64 data;

	/*
	 * We allow index == CPUACCT_STAT_NSTATS here to read
	 * the sum of usages.
	 */
	إذا (WARN_ON_ONCE(index > CPUACCT_STAT_NSTATS))
		أرجع 0;

#ifndef CONFIG_64BIT
	/*
	 * Take rq->lock to make 64-bit read safe on 32-bit platforms.
	 */
	raw_spin_rq_lock_irq(cpu_rq(cpu));
#endif

	بدل (index) {
	حالة CPUACCT_STAT_USER:
		data = cpustat[CPUTIME_USER] + cpustat[CPUTIME_NICE];
		اكسر;
	حالة CPUACCT_STAT_SYSTEM:
		data = cpustat[CPUTIME_SYSTEM] + cpustat[CPUTIME_IRQ] +
			cpustat[CPUTIME_SOFTIRQ];
		اكسر;
	حالة CPUACCT_STAT_NSTATS:
		data = *cpuusage;
		اكسر;
	}

#ifndef CONFIG_64BIT
	raw_spin_rq_unlock_irq(cpu_rq(cpu));
#endif

	أرجع data;
}

ساكن فراغ cpuacct_cpuusage_write(بنية cpuacct *ca, صحيح cpu)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;

	/* Don't allow to reset global kernel_cpustat */
	إذا (ca == &سيو_cpuacct)
		أرجع;

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
ساكن u64 __cpuusage_read(بنية cgroup_subsys_state *css,
			   تعداد cpuacct_stat_index index)
{
	بنية cpuacct *ca = css_ca(css);
	u64 totalcpuusage = 0;
	صحيح i;

	for_each_possible_cpu(i)
		totalcpuusage += cpuacct_cpuusage_read(ca, i, index);

	أرجع totalcpuusage;
}

ساكن u64 cpuusage_user_read(بنية cgroup_subsys_state *css,
			      بنية cftype *cft)
{
	أرجع __cpuusage_read(css, CPUACCT_STAT_USER);
}

ساكن u64 cpuusage_sys_read(بنية cgroup_subsys_state *css,
			     بنية cftype *cft)
{
	أرجع __cpuusage_read(css, CPUACCT_STAT_SYSTEM);
}

ساكن u64 cpuusage_read(بنية cgroup_subsys_state *css, بنية cftype *cft)
{
	أرجع __cpuusage_read(css, CPUACCT_STAT_NSTATS);
}

ساكن صحيح cpuusage_write(بنية cgroup_subsys_state *css, بنية cftype *cft,
			  u64 val)
{
	بنية cpuacct *ca = css_ca(css);
	صحيح cpu;

	/*
	 * Only allow '0' here to do a reset.
	 */
	إذا (val)
		أرجع -EINVAL;

	for_each_possible_cpu(cpu)
		cpuacct_cpuusage_write(ca, cpu);

	أرجع 0;
}

ساكن صحيح __cpuacct_percpu_seq_show(بنية seq_file *m,
				     تعداد cpuacct_stat_index index)
{
	بنية cpuacct *ca = css_ca(seq_css(m));
	u64 percpu;
	صحيح i;

	for_each_possible_cpu(i) {
		percpu = cpuacct_cpuusage_read(ca, i, index);
		seq_printf(m, "%llu ", (unsigned long long) percpu);
	}
	seq_printf(m, "\n");
	أرجع 0;
}

ساكن صحيح cpuacct_percpu_user_seq_show(بنية seq_file *m, فراغ *V)
{
	أرجع __cpuacct_percpu_seq_show(m, CPUACCT_STAT_USER);
}

ساكن صحيح cpuacct_percpu_sys_seq_show(بنية seq_file *m, فراغ *V)
{
	أرجع __cpuacct_percpu_seq_show(m, CPUACCT_STAT_SYSTEM);
}

ساكن صحيح cpuacct_percpu_seq_show(بنية seq_file *m, فراغ *V)
{
	أرجع __cpuacct_percpu_seq_show(m, CPUACCT_STAT_NSTATS);
}

ساكن صحيح cpuacct_all_seq_show(بنية seq_file *m, فراغ *V)
{
	بنية cpuacct *ca = css_ca(seq_css(m));
	صحيح index;
	صحيح cpu;

	seq_puts(m, "cpu");
	لكل (index = 0; index < CPUACCT_STAT_NSTATS; index++)
		seq_printf(m, " %s", cpuacct_stat_desc[index]);
	seq_puts(m, "\n");

	for_each_possible_cpu(cpu) {
		seq_printf(m, "%d", cpu);
		لكل (index = 0; index < CPUACCT_STAT_NSTATS; index++)
			seq_printf(m, " %llu",
				   cpuacct_cpuusage_read(ca, cpu, index));
		seq_puts(m, "\n");
	}
	أرجع 0;
}

ساكن صحيح cpuacct_stats_show(بنية seq_file *sf, فراغ *v)
{
	بنية cpuacct *ca = css_ca(seq_css(sf));
	بنية task_cputime cputime;
	u64 val[CPUACCT_STAT_NSTATS];
	صحيح cpu;
	صحيح stat;

	memset(&cputime, 0, حجم(cputime));
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

	لكل (stat = 0; stat < CPUACCT_STAT_NSTATS; stat++) {
		seq_printf(sf, "%s %llu\n", cpuacct_stat_desc[stat],
			nsec_to_clock_t(val[stat]));
	}

	أرجع 0;
}

ساكن بنية cftype files[] = {
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
فراغ cpuacct_charge(بنية task_struct *tsk, u64 cputime)
{
	unsigned صحيح cpu = task_cpu(tsk);
	بنية cpuacct *ca;

	lockdep_assert_rq_held(cpu_rq(cpu));

	لكل (ca = task_ca(tsk); ca; ca = parent_ca(ca))
		*per_cpu_ptr(ca->cpuusage, cpu) += cputime;
}

/*
 * Add user/system time to cpuacct.
 *
 * Note: it's the caller that updates the account of the root cgroup.
 */
فراغ cpuacct_account_field(بنية task_struct *tsk, صحيح index, u64 val)
{
	بنية cpuacct *ca;

	لكل (ca = task_ca(tsk); ca != &سيو_cpuacct; ca = parent_ca(ca))
		__this_cpu_add(ca->cpustat->cpustat[index], val);
}

بنية cgroup_subsys cpuacct_cgrp_subsys = {
	.css_alloc	= cpuacct_css_alloc,
	.css_free	= cpuacct_css_free,
	.legacy_cftypes	= files,
	.early_init	= true,
};
