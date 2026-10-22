// SPDX-License-Identifier: GPL-2.0

/*
 * CPU accounting code for task groups.
 *
 * Based on the work by Paul Menage (menage@google.com) and Balbir Singh
 * (balbir@in.ibm.com).
 */
#अन्तर्भाव <linux/sched/cputime.h>
#अन्तर्भाव "sched.h"

/* Time spent by the tasks of the CPU accounting group executing in ... */
गणना cpuacct_stat_index {
	CPUACCT_STAT_USER,	/* ... user mode */
	CPUACCT_STAT_SYSTEM,	/* ... kernel mode */

	CPUACCT_STAT_NSTATS,
};

स्थिर स्थिरांक अक्षर * स्थिरांक cpuacct_stat_desc[] = {
	[CPUACCT_STAT_USER] = "user",
	[CPUACCT_STAT_SYSTEM] = "system",
};

/* track CPU usage of a group of tasks and its child groups */
संरचना cpuacct {
	संरचना cgroup_subsys_state	css;
	/* cpuusage holds pointer to a u64-type object on every CPU */
	u64 __percpu	*cpuusage;
	संरचना kernel_cpustat __percpu	*cpustat;
};

स्थिर ओळीत संरचना cpuacct *css_ca(संरचना cgroup_subsys_state *css)
{
	परत css ? container_of(css, संरचना cpuacct, css) : रिक्त;
}

/* Return CPU accounting group to which this task belongs */
स्थिर ओळीत संरचना cpuacct *task_ca(संरचना task_struct *tsk)
{
	परत css_ca(task_css(tsk, cpuacct_cgrp_id));
}

स्थिर ओळीत संरचना cpuacct *parent_ca(संरचना cpuacct *ca)
{
	परत css_ca(ca->css.parent);
}

स्थिर DEFINE_PER_CPU(u64, मूळ_cpuacct_cpuusage);
स्थिर संरचना cpuacct मूळ_cpuacct = {
	.cpustat	= &kernel_cpustat,
	.cpuusage	= &मूळ_cpuacct_cpuusage,
};

/* Create a new CPU accounting group */
स्थिर संरचना cgroup_subsys_state *
cpuacct_css_alloc(संरचना cgroup_subsys_state *parent_css)
{
	संरचना cpuacct *ca;

	जर (!parent_css)
		परत &मूळ_cpuacct.css;

	ca = kzalloc_obj(*ca);
	जर (!ca)
		जा out;

	ca->cpuusage = alloc_percpu(u64);
	जर (!ca->cpuusage)
		जा out_free_ca;

	ca->cpustat = alloc_percpu(संरचना kernel_cpustat);
	जर (!ca->cpustat)
		जा out_free_cpuusage;

	परत &ca->css;

out_free_cpuusage:
	free_percpu(ca->cpuusage);
out_free_ca:
	kfree(ca);
out:
	परत ERR_PTR(-ENOMEM);
}

/* Destroy an existing CPU accounting group */
स्थिर शून्य cpuacct_css_free(संरचना cgroup_subsys_state *css)
{
	संरचना cpuacct *ca = css_ca(css);

	free_percpu(ca->cpustat);
	free_percpu(ca->cpuusage);
	kfree(ca);
}

स्थिर u64 cpuacct_cpuusage_read(संरचना cpuacct *ca, पूर्णांक cpu,
				 गणना cpuacct_stat_index index)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;
	u64 data;

	/*
	 * We allow index == CPUACCT_STAT_NSTATS here to read
	 * the sum of usages.
	 */
	जर (WARN_ON_ONCE(index > CPUACCT_STAT_NSTATS))
		परत 0;

#ifndef CONFIG_64BIT
	/*
	 * Take rq->lock to make 64-bit read safe on 32-bit platforms.
	 */
	raw_spin_rq_lock_irq(cpu_rq(cpu));
#endif

	निवड (index) {
	स्थिती CPUACCT_STAT_USER:
		data = cpustat[CPUTIME_USER] + cpustat[CPUTIME_NICE];
		थांबा;
	स्थिती CPUACCT_STAT_SYSTEM:
		data = cpustat[CPUTIME_SYSTEM] + cpustat[CPUTIME_IRQ] +
			cpustat[CPUTIME_SOFTIRQ];
		थांबा;
	स्थिती CPUACCT_STAT_NSTATS:
		data = *cpuusage;
		थांबा;
	}

#ifndef CONFIG_64BIT
	raw_spin_rq_unlock_irq(cpu_rq(cpu));
#endif

	परत data;
}

स्थिर शून्य cpuacct_cpuusage_write(संरचना cpuacct *ca, पूर्णांक cpu)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;

	/* Don't allow to reset global kernel_cpustat */
	जर (ca == &मूळ_cpuacct)
		परत;

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
स्थिर u64 __cpuusage_read(संरचना cgroup_subsys_state *css,
			   गणना cpuacct_stat_index index)
{
	संरचना cpuacct *ca = css_ca(css);
	u64 totalcpuusage = 0;
	पूर्णांक i;

	for_each_possible_cpu(i)
		totalcpuusage += cpuacct_cpuusage_read(ca, i, index);

	परत totalcpuusage;
}

स्थिर u64 cpuusage_user_read(संरचना cgroup_subsys_state *css,
			      संरचना cftype *cft)
{
	परत __cpuusage_read(css, CPUACCT_STAT_USER);
}

स्थिर u64 cpuusage_sys_read(संरचना cgroup_subsys_state *css,
			     संरचना cftype *cft)
{
	परत __cpuusage_read(css, CPUACCT_STAT_SYSTEM);
}

स्थिर u64 cpuusage_read(संरचना cgroup_subsys_state *css, संरचना cftype *cft)
{
	परत __cpuusage_read(css, CPUACCT_STAT_NSTATS);
}

स्थिर पूर्णांक cpuusage_write(संरचना cgroup_subsys_state *css, संरचना cftype *cft,
			  u64 val)
{
	संरचना cpuacct *ca = css_ca(css);
	पूर्णांक cpu;

	/*
	 * Only allow '0' here to do a reset.
	 */
	जर (val)
		परत -EINVAL;

	for_each_possible_cpu(cpu)
		cpuacct_cpuusage_write(ca, cpu);

	परत 0;
}

स्थिर पूर्णांक __cpuacct_percpu_seq_show(संरचना seq_file *m,
				     गणना cpuacct_stat_index index)
{
	संरचना cpuacct *ca = css_ca(seq_css(m));
	u64 percpu;
	पूर्णांक i;

	for_each_possible_cpu(i) {
		percpu = cpuacct_cpuusage_read(ca, i, index);
		seq_printf(m, "%llu ", (अचिन्हित दीर्घ दीर्घ) percpu);
	}
	seq_printf(m, "\n");
	परत 0;
}

स्थिर पूर्णांक cpuacct_percpu_user_seq_show(संरचना seq_file *m, शून्य *V)
{
	परत __cpuacct_percpu_seq_show(m, CPUACCT_STAT_USER);
}

स्थिर पूर्णांक cpuacct_percpu_sys_seq_show(संरचना seq_file *m, शून्य *V)
{
	परत __cpuacct_percpu_seq_show(m, CPUACCT_STAT_SYSTEM);
}

स्थिर पूर्णांक cpuacct_percpu_seq_show(संरचना seq_file *m, शून्य *V)
{
	परत __cpuacct_percpu_seq_show(m, CPUACCT_STAT_NSTATS);
}

स्थिर पूर्णांक cpuacct_all_seq_show(संरचना seq_file *m, शून्य *V)
{
	संरचना cpuacct *ca = css_ca(seq_css(m));
	पूर्णांक index;
	पूर्णांक cpu;

	seq_puts(m, "cpu");
	साठी (index = 0; index < CPUACCT_STAT_NSTATS; index++)
		seq_printf(m, " %s", cpuacct_stat_desc[index]);
	seq_puts(m, "\n");

	for_each_possible_cpu(cpu) {
		seq_printf(m, "%d", cpu);
		साठी (index = 0; index < CPUACCT_STAT_NSTATS; index++)
			seq_printf(m, " %llu",
				   cpuacct_cpuusage_read(ca, cpu, index));
		seq_puts(m, "\n");
	}
	परत 0;
}

स्थिर पूर्णांक cpuacct_stats_show(संरचना seq_file *sf, शून्य *v)
{
	संरचना cpuacct *ca = css_ca(seq_css(sf));
	संरचना task_cputime cputime;
	u64 val[CPUACCT_STAT_NSTATS];
	पूर्णांक cpu;
	पूर्णांक stat;

	स_रखो(&cputime, 0, आकार(cputime));
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

	साठी (stat = 0; stat < CPUACCT_STAT_NSTATS; stat++) {
		seq_printf(sf, "%s %llu\n", cpuacct_stat_desc[stat],
			nsec_to_clock_t(val[stat]));
	}

	परत 0;
}

स्थिर संरचना cftype files[] = {
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
शून्य cpuacct_charge(संरचना task_struct *tsk, u64 cputime)
{
	अचिन्हित पूर्णांक cpu = task_cpu(tsk);
	संरचना cpuacct *ca;

	lockdep_assert_rq_held(cpu_rq(cpu));

	साठी (ca = task_ca(tsk); ca; ca = parent_ca(ca))
		*per_cpu_ptr(ca->cpuusage, cpu) += cputime;
}

/*
 * Add user/system time to cpuacct.
 *
 * Note: it's the caller that updates the account of the root cgroup.
 */
शून्य cpuacct_account_field(संरचना task_struct *tsk, पूर्णांक index, u64 val)
{
	संरचना cpuacct *ca;

	साठी (ca = task_ca(tsk); ca != &मूळ_cpuacct; ca = parent_ca(ca))
		__this_cpu_add(ca->cpustat->cpustat[index], val);
}

संरचना cgroup_subsys cpuacct_cgrp_subsys = {
	.css_alloc	= cpuacct_css_alloc,
	.css_free	= cpuacct_css_free,
	.legacy_cftypes	= files,
	.early_init	= true,
};
