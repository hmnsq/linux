// SPDX-License-Identifier: GPL-2.0

/*
 * CPU accounting code for task groups.
 *
 * Based on the work by Paul Menage (menage@google.com) and Balbir Singh
 * (balbir@in.ibm.com).
 */
#समावेश <linux/sched/cputime.h>
#समावेश "sched.h"

/* Time spent by the tasks of the CPU accounting group executing in ... */
क्रमागत cpuacct_stat_सूचकांक {
	CPUACCT_STAT_USER,	/* ... user mode */
	CPUACCT_STAT_SYSTEM,	/* ... kernel mode */

	CPUACCT_STAT_NSTATS,
};

अटल स्थिर अक्षर * स्थिर cpuacct_stat_desc[] = {
	[CPUACCT_STAT_USER] = "user",
	[CPUACCT_STAT_SYSTEM] = "system",
};

/* track CPU usage of a group of tasks and its child groups */
काष्ठा cpuacct {
	काष्ठा cgroup_subsys_state	css;
	/* cpuusage holds pointer to a u64-type object on every CPU */
	u64 __percpu	*cpuusage;
	काष्ठा kernel_cpustat __percpu	*cpustat;
};

अटल अंतरभूत काष्ठा cpuacct *css_ca(काष्ठा cgroup_subsys_state *css)
{
	वापस css ? container_of(css, काष्ठा cpuacct, css) : शून्य;
}

/* Return CPU accounting group to which this task belongs */
अटल अंतरभूत काष्ठा cpuacct *task_ca(काष्ठा task_struct *tsk)
{
	वापस css_ca(task_css(tsk, cpuacct_cgrp_id));
}

अटल अंतरभूत काष्ठा cpuacct *parent_ca(काष्ठा cpuacct *ca)
{
	वापस css_ca(ca->css.parent);
}

अटल DEFINE_PER_CPU(u64, मूल_cpuacct_cpuusage);
अटल काष्ठा cpuacct मूल_cpuacct = {
	.cpustat	= &kernel_cpustat,
	.cpuusage	= &मूल_cpuacct_cpuusage,
};

/* Create a new CPU accounting group */
अटल काष्ठा cgroup_subsys_state *
cpuacct_css_alloc(काष्ठा cgroup_subsys_state *parent_css)
{
	काष्ठा cpuacct *ca;

	अगर (!parent_css)
		वापस &मूल_cpuacct.css;

	ca = kzalloc_obj(*ca);
	अगर (!ca)
		जाओ out;

	ca->cpuusage = alloc_percpu(u64);
	अगर (!ca->cpuusage)
		जाओ out_free_ca;

	ca->cpustat = alloc_percpu(काष्ठा kernel_cpustat);
	अगर (!ca->cpustat)
		जाओ out_free_cpuusage;

	वापस &ca->css;

out_free_cpuusage:
	free_percpu(ca->cpuusage);
out_free_ca:
	kfree(ca);
out:
	वापस ERR_PTR(-ENOMEM);
}

/* Destroy an existing CPU accounting group */
अटल व्योम cpuacct_css_free(काष्ठा cgroup_subsys_state *css)
{
	काष्ठा cpuacct *ca = css_ca(css);

	free_percpu(ca->cpustat);
	free_percpu(ca->cpuusage);
	kfree(ca);
}

अटल u64 cpuacct_cpuusage_read(काष्ठा cpuacct *ca, पूर्णांक cpu,
				 क्रमागत cpuacct_stat_सूचकांक सूचकांक)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;
	u64 तथ्य;

	/*
	 * We allow index == CPUACCT_STAT_NSTATS here to read
	 * the sum of usages.
	 */
	अगर (WARN_ON_ONCE(सूचकांक > CPUACCT_STAT_NSTATS))
		वापस 0;

#अगर_अघोषित CONFIG_64BIT
	/*
	 * Take rq->lock to make 64-bit read safe on 32-bit platforms.
	 */
	raw_spin_rq_lock_irq(cpu_rq(cpu));
#पूर्ण_अगर

	चयन (सूचकांक) {
	हाल CPUACCT_STAT_USER:
		तथ्य = cpustat[CPUTIME_USER] + cpustat[CPUTIME_NICE];
		अवरोध;
	हाल CPUACCT_STAT_SYSTEM:
		तथ्य = cpustat[CPUTIME_SYSTEM] + cpustat[CPUTIME_IRQ] +
			cpustat[CPUTIME_SOFTIRQ];
		अवरोध;
	हाल CPUACCT_STAT_NSTATS:
		तथ्य = *cpuusage;
		अवरोध;
	}

#अगर_अघोषित CONFIG_64BIT
	raw_spin_rq_unlock_irq(cpu_rq(cpu));
#पूर्ण_अगर

	वापस तथ्य;
}

अटल व्योम cpuacct_cpuusage_write(काष्ठा cpuacct *ca, पूर्णांक cpu)
{
	u64 *cpuusage = per_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;

	/* Don't allow to reset global kernel_cpustat */
	अगर (ca == &मूल_cpuacct)
		वापस;

#अगर_अघोषित CONFIG_64BIT
	/*
	 * Take rq->lock to make 64-bit write safe on 32-bit platforms.
	 */
	raw_spin_rq_lock_irq(cpu_rq(cpu));
#पूर्ण_अगर
	*cpuusage = 0;
	cpustat[CPUTIME_USER] = cpustat[CPUTIME_NICE] = 0;
	cpustat[CPUTIME_SYSTEM] = cpustat[CPUTIME_IRQ] = 0;
	cpustat[CPUTIME_SOFTIRQ] = 0;

#अगर_अघोषित CONFIG_64BIT
	raw_spin_rq_unlock_irq(cpu_rq(cpu));
#पूर्ण_अगर
}

/* Return total CPU usage (in nanoseconds) of a group */
अटल u64 __cpuusage_read(काष्ठा cgroup_subsys_state *css,
			   क्रमागत cpuacct_stat_सूचकांक सूचकांक)
{
	काष्ठा cpuacct *ca = css_ca(css);
	u64 totalcpuusage = 0;
	पूर्णांक i;

	for_each_possible_cpu(i)
		totalcpuusage += cpuacct_cpuusage_read(ca, i, सूचकांक);

	वापस totalcpuusage;
}

अटल u64 cpuusage_user_read(काष्ठा cgroup_subsys_state *css,
			      काष्ठा cftype *cft)
{
	वापस __cpuusage_read(css, CPUACCT_STAT_USER);
}

अटल u64 cpuusage_sys_read(काष्ठा cgroup_subsys_state *css,
			     काष्ठा cftype *cft)
{
	वापस __cpuusage_read(css, CPUACCT_STAT_SYSTEM);
}

अटल u64 cpuusage_read(काष्ठा cgroup_subsys_state *css, काष्ठा cftype *cft)
{
	वापस __cpuusage_read(css, CPUACCT_STAT_NSTATS);
}

अटल पूर्णांक cpuusage_write(काष्ठा cgroup_subsys_state *css, काष्ठा cftype *cft,
			  u64 val)
{
	काष्ठा cpuacct *ca = css_ca(css);
	पूर्णांक cpu;

	/*
	 * Only allow '0' here to do a reset.
	 */
	अगर (val)
		वापस -EINVAL;

	for_each_possible_cpu(cpu)
		cpuacct_cpuusage_write(ca, cpu);

	वापस 0;
}

अटल पूर्णांक __cpuacct_percpu_seq_show(काष्ठा seq_file *m,
				     क्रमागत cpuacct_stat_सूचकांक सूचकांक)
{
	काष्ठा cpuacct *ca = css_ca(seq_css(m));
	u64 percpu;
	पूर्णांक i;

	for_each_possible_cpu(i) {
		percpu = cpuacct_cpuusage_read(ca, i, सूचकांक);
		seq_printf(m, "%llu ", (अचिन्हित दीर्घ दीर्घ) percpu);
	}
	seq_printf(m, "\n");
	वापस 0;
}

अटल पूर्णांक cpuacct_percpu_user_seq_show(काष्ठा seq_file *m, व्योम *V)
{
	वापस __cpuacct_percpu_seq_show(m, CPUACCT_STAT_USER);
}

अटल पूर्णांक cpuacct_percpu_sys_seq_show(काष्ठा seq_file *m, व्योम *V)
{
	वापस __cpuacct_percpu_seq_show(m, CPUACCT_STAT_SYSTEM);
}

अटल पूर्णांक cpuacct_percpu_seq_show(काष्ठा seq_file *m, व्योम *V)
{
	वापस __cpuacct_percpu_seq_show(m, CPUACCT_STAT_NSTATS);
}

अटल पूर्णांक cpuacct_all_seq_show(काष्ठा seq_file *m, व्योम *V)
{
	काष्ठा cpuacct *ca = css_ca(seq_css(m));
	पूर्णांक सूचकांक;
	पूर्णांक cpu;

	seq_puts(m, "cpu");
	क्रम (सूचकांक = 0; सूचकांक < CPUACCT_STAT_NSTATS; सूचकांक++)
		seq_printf(m, " %s", cpuacct_stat_desc[सूचकांक]);
	seq_puts(m, "\n");

	for_each_possible_cpu(cpu) {
		seq_printf(m, "%d", cpu);
		क्रम (सूचकांक = 0; सूचकांक < CPUACCT_STAT_NSTATS; सूचकांक++)
			seq_printf(m, " %llu",
				   cpuacct_cpuusage_read(ca, cpu, सूचकांक));
		seq_puts(m, "\n");
	}
	वापस 0;
}

अटल पूर्णांक cpuacct_stats_show(काष्ठा seq_file *sf, व्योम *v)
{
	काष्ठा cpuacct *ca = css_ca(seq_css(sf));
	काष्ठा task_cputime cputime;
	u64 val[CPUACCT_STAT_NSTATS];
	पूर्णांक cpu;
	पूर्णांक stat;

	स_रखो(&cputime, 0, माप(cputime));
	for_each_possible_cpu(cpu) {
		u64 *cpustat = per_cpu_ptr(ca->cpustat, cpu)->cpustat;

		cputime.utime += cpustat[CPUTIME_USER];
		cputime.utime += cpustat[CPUTIME_NICE];
		cputime.stime += cpustat[CPUTIME_SYSTEM];
		cputime.stime += cpustat[CPUTIME_IRQ];
		cputime.stime += cpustat[CPUTIME_SOFTIRQ];

		cputime.योग_exec_runtime += *per_cpu_ptr(ca->cpuusage, cpu);
	}

	cputime_adjust(&cputime, &seq_css(sf)->cgroup->पूर्व_cputime,
		&val[CPUACCT_STAT_USER], &val[CPUACCT_STAT_SYSTEM]);

	क्रम (stat = 0; stat < CPUACCT_STAT_NSTATS; stat++) {
		seq_printf(sf, "%s %llu\n", cpuacct_stat_desc[stat],
			nsec_to_clock_t(val[stat]));
	}

	वापस 0;
}

अटल काष्ठा cftype files[] = {
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
व्योम cpuacct_charge(काष्ठा task_struct *tsk, u64 cputime)
{
	अचिन्हित पूर्णांक cpu = task_cpu(tsk);
	काष्ठा cpuacct *ca;

	lockdep_assert_rq_held(cpu_rq(cpu));

	क्रम (ca = task_ca(tsk); ca; ca = parent_ca(ca))
		*per_cpu_ptr(ca->cpuusage, cpu) += cputime;
}

/*
 * Add user/system time to cpuacct.
 *
 * Note: it's the caller that updates the account of the root cgroup.
 */
व्योम cpuacct_account_field(काष्ठा task_struct *tsk, पूर्णांक सूचकांक, u64 val)
{
	काष्ठा cpuacct *ca;

	क्रम (ca = task_ca(tsk); ca != &मूल_cpuacct; ca = parent_ca(ca))
		__this_cpu_add(ca->cpustat->cpustat[सूचकांक], val);
}

काष्ठा cgroup_subsys cpuacct_cgrp_subsys = {
	.css_alloc	= cpuacct_css_alloc,
	.css_free	= cpuacct_css_free,
	.legacy_cftypes	= files,
	.early_init	= true,
};
