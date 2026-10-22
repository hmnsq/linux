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

स्थिर ध्रुव वर्ण * ध्रुव cpuacct_stat_desc[] = {
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

स्थिर अन्तःस्थ संरचना cpuacct *css_ca(संरचना cgroup_subsys_state *css)
{
	प्रत्यागम css ? container_of(css, संरचना cpuacct, css) : रिक्त;
}

/* Return CPU accounting group to which this task belongs */
स्थिर अन्तःस्थ संरचना cpuacct *task_ca(संरचना task_struct *tsk)
{
	प्रत्यागम css_ca(task_css(tsk, cpuacct_cgrp_id));
}

स्थिर अन्तःस्थ संरचना cpuacct *parent_ca(संरचना cpuacct *ca)
{
	प्रत्यागम css_ca(ca->css.parent);
}

स्थिर DEFINE_PER_CPU(u64, मूल_cpuacct_cpuusage);
स्थिर संरचना cpuacct मूल_cpuacct = {
	.cpustat	= &kernel_cpustat,
	.cpuusage	= &मूल_cpuacct_cpuusage,
};

/* Create a new CPU accounting group */
स्थिर संरचना cgroup_subsys_state *
cpuacct_css_alloc(संरचना cgroup_subsys_state *parent_css)
{
	संरचना cpuacct *ca;

	यदि (!parent_css)
		प्रत्यागम &मूल_cpuacct.css;

	ca = kzalloc_obj(*ca);
	यदि (!ca)
		गच्छ out;

	ca->cpuusage = alloc_percpu(u64);
	यदि (!ca->cpuusage)
		गच्छ out_free_ca;

	ca->cpustat = alloc_percpu(संरचना kernel_cpustat);
	यदि (!ca->cpustat)
		गच्छ out_free_cpuusage;

	प्रत्यागम &ca->css;

out_free_cpuusage:
	free_percpu(ca->cpuusage);
out_free_ca:
	kfree(ca);
out:
	प्रत्यागम ERR_PTR(-ENOMEM);
}

/* Destroy an existing CPU accounting group */
स्थिर शून्य cpuacct_css_free(संरचना cgroup_subsys_state *css)
{
	संरचना cpuacct *ca = css_ca(css);

	free_percpu(ca->cpustat);
	free_percpu(ca->cpuusage);
	kfree(ca);
}

स्थिर u64 cpuacct_cpuusage_read(संरचना cpuacct *ca, पूर्णाङ्क cpu,
				 गणना cpuacct_stat_index index)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;
	u64 data;

	/*
	 * We allow index == CPUACCT_STAT_NSTATS here to read
	 * the sum of usages.
	 */
	यदि (WARN_ON_ONCE(index > CPUACCT_STAT_NSTATS))
		प्रत्यागम 0;

#ifndef CONFIG_64BIT
	/*
	 * Take rq->lock to make 64-bit read safe on 32-bit platforms.
	 */
	raw_spin_rq_lock_irq(cpu_rq(cpu));
#endif

	विकल्प (index) {
	पक्ष CPUACCT_STAT_USER:
		data = cpustat[CPUTIME_USER] + cpustat[CPUTIME_NICE];
		भङ्ग;
	पक्ष CPUACCT_STAT_SYSTEM:
		data = cpustat[CPUTIME_SYSTEM] + cpustat[CPUTIME_IRQ] +
			cpustat[CPUTIME_SOFTIRQ];
		भङ्ग;
	पक्ष CPUACCT_STAT_NSTATS:
		data = *cpuusage;
		भङ्ग;
	}

#ifndef CONFIG_64BIT
	raw_spin_rq_unlock_irq(cpu_rq(cpu));
#endif

	प्रत्यागम data;
}

स्थिर शून्य cpuacct_cpuusage_write(संरचना cpuacct *ca, पूर्णाङ्क cpu)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;

	/* Don't allow to reset global kernel_cpustat */
	यदि (ca == &मूल_cpuacct)
		प्रत्यागम;

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
	पूर्णाङ्क i;

	for_each_possible_cpu(i)
		totalcpuusage += cpuacct_cpuusage_read(ca, i, index);

	प्रत्यागम totalcpuusage;
}

स्थिर u64 cpuusage_user_read(संरचना cgroup_subsys_state *css,
			      संरचना cftype *cft)
{
	प्रत्यागम __cpuusage_read(css, CPUACCT_STAT_USER);
}

स्थिर u64 cpuusage_sys_read(संरचना cgroup_subsys_state *css,
			     संरचना cftype *cft)
{
	प्रत्यागम __cpuusage_read(css, CPUACCT_STAT_SYSTEM);
}

स्थिर u64 cpuusage_read(संरचना cgroup_subsys_state *css, संरचना cftype *cft)
{
	प्रत्यागम __cpuusage_read(css, CPUACCT_STAT_NSTATS);
}

स्थिर पूर्णाङ्क cpuusage_write(संरचना cgroup_subsys_state *css, संरचना cftype *cft,
			  u64 val)
{
	संरचना cpuacct *ca = css_ca(css);
	पूर्णाङ्क cpu;

	/*
	 * Only allow '0' here to do a reset.
	 */
	यदि (val)
		प्रत्यागम -EINVAL;

	for_each_possible_cpu(cpu)
		cpuacct_cpuusage_write(ca, cpu);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क __cpuacct_percpu_seq_show(संरचना seq_file *m,
				     गणना cpuacct_stat_index index)
{
	संरचना cpuacct *ca = css_ca(seq_css(m));
	u64 percpu;
	पूर्णाङ्क i;

	for_each_possible_cpu(i) {
		percpu = cpuacct_cpuusage_read(ca, i, index);
		seq_printf(m, "%llu ", (अचिन्हित दीर्घ दीर्घ) percpu);
	}
	seq_printf(m, "\n");
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क cpuacct_percpu_user_seq_show(संरचना seq_file *m, शून्य *V)
{
	प्रत्यागम __cpuacct_percpu_seq_show(m, CPUACCT_STAT_USER);
}

स्थिर पूर्णाङ्क cpuacct_percpu_sys_seq_show(संरचना seq_file *m, शून्य *V)
{
	प्रत्यागम __cpuacct_percpu_seq_show(m, CPUACCT_STAT_SYSTEM);
}

स्थिर पूर्णाङ्क cpuacct_percpu_seq_show(संरचना seq_file *m, शून्य *V)
{
	प्रत्यागम __cpuacct_percpu_seq_show(m, CPUACCT_STAT_NSTATS);
}

स्थिर पूर्णाङ्क cpuacct_all_seq_show(संरचना seq_file *m, शून्य *V)
{
	संरचना cpuacct *ca = css_ca(seq_css(m));
	पूर्णाङ्क index;
	पूर्णाङ्क cpu;

	seq_puts(m, "cpu");
	प्रति (index = 0; index < CPUACCT_STAT_NSTATS; index++)
		seq_printf(m, " %s", cpuacct_stat_desc[index]);
	seq_puts(m, "\n");

	for_each_possible_cpu(cpu) {
		seq_printf(m, "%d", cpu);
		प्रति (index = 0; index < CPUACCT_STAT_NSTATS; index++)
			seq_printf(m, " %llu",
				   cpuacct_cpuusage_read(ca, cpu, index));
		seq_puts(m, "\n");
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क cpuacct_stats_show(संरचना seq_file *sf, शून्य *v)
{
	संरचना cpuacct *ca = css_ca(seq_css(sf));
	संरचना task_cputime cputime;
	u64 val[CPUACCT_STAT_NSTATS];
	पूर्णाङ्क cpu;
	पूर्णाङ्क stat;

	स_रखो(&cputime, 0, परिमाण(cputime));
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

	प्रति (stat = 0; stat < CPUACCT_STAT_NSTATS; stat++) {
		seq_printf(sf, "%s %llu\n", cpuacct_stat_desc[stat],
			nsec_to_clock_t(val[stat]));
	}

	प्रत्यागम 0;
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
	अचिन्हित पूर्णाङ्क cpu = task_cpu(tsk);
	संरचना cpuacct *ca;

	lockdep_assert_rq_held(cpu_rq(cpu));

	प्रति (ca = task_ca(tsk); ca; ca = parent_ca(ca))
		*per_cpu_ptr(ca->cpuusage, cpu) += cputime;
}

/*
 * Add user/system time to cpuacct.
 *
 * Note: it's the caller that updates the account of the root cgroup.
 */
शून्य cpuacct_account_field(संरचना task_struct *tsk, पूर्णाङ्क index, u64 val)
{
	संरचना cpuacct *ca;

	प्रति (ca = task_ca(tsk); ca != &मूल_cpuacct; ca = parent_ca(ca))
		__this_cpu_add(ca->cpustat->cpustat[index], val);
}

संरचना cgroup_subsys cpuacct_cgrp_subsys = {
	.css_alloc	= cpuacct_css_alloc,
	.css_free	= cpuacct_css_free,
	.legacy_cftypes	= files,
	.early_init	= true,
};
