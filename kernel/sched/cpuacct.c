// SPDX-License-Identifier: GPL-2.0

/*
 * CPU accounting code for task groups.
 *
 * Based on the work by Paul Menage (menage@google.com) and Balbir Singh
 * (balbir@in.ibm.com).
 */
#شامل <linux/sched/cputime.h>
#شامل "sched.h"

/* Time spent by the tasks of the CPU accounting group executing in ... */
شمار cpuacct_stat_اشاریہ {
	CPUACCT_STAT_USER,	/* ... user mode */
	CPUACCT_STAT_SYSTEM,	/* ... kernel mode */

	CPUACCT_STAT_NSTATS,
};

ساکن ثابت حرف * ثابت cpuacct_stat_desc[] = {
	[CPUACCT_STAT_USER] = "user",
	[CPUACCT_STAT_SYSTEM] = "system",
};

/* track CPU usage of a group of tasks and its child groups */
ساخت cpuacct {
	ساخت cgroup_subsys_state	css;
	/* cpuusage holds pointer to a u64-type object on every CPU */
	u64 __percpu	*cpuusage;
	ساخت kernel_cpustat __percpu	*cpustat;
};

ساکن اندرونی ساخت cpuacct *css_ca(ساخت cgroup_subsys_state *css)
{
	واپس css ? container_کا(css, ساخت cpuacct, css) : تہی;
}

/* Return CPU accounting group to which this task belongs */
ساکن اندرونی ساخت cpuacct *task_ca(ساخت task_struct *tsk)
{
	واپس css_ca(task_css(tsk, cpuacct_cgrp_id));
}

ساکن اندرونی ساخت cpuacct *والد_ca(ساخت cpuacct *ca)
{
	واپس css_ca(ca->css.والد);
}

ساکن DEFINE_فی_CPU(u64, جڑ_cpuacct_cpuusage);
ساکن ساخت cpuacct جڑ_cpuacct = {
	.cpustat	= &kernel_cpustat,
	.cpuusage	= &جڑ_cpuacct_cpuusage,
};

/* Create a new CPU accounting group */
ساکن ساخت cgroup_subsys_state *
cpuacct_css_alloc(ساخت cgroup_subsys_state *والد_css)
{
	ساخت cpuacct *ca;

	اگر (!والد_css)
		واپس &جڑ_cpuacct.css;

	ca = kzalloc_obj(*ca);
	اگر (!ca)
		جاؤ برآمد;

	ca->cpuusage = alloc_percpu(u64);
	اگر (!ca->cpuusage)
		جاؤ برآمد_free_ca;

	ca->cpustat = alloc_percpu(ساخت kernel_cpustat);
	اگر (!ca->cpustat)
		جاؤ برآمد_free_cpuusage;

	واپس &ca->css;

برآمد_free_cpuusage:
	free_percpu(ca->cpuusage);
برآمد_free_ca:
	kfree(ca);
برآمد:
	واپس ERR_PTR(-ENOMEM);
}

/* Destroy an existing CPU accounting group */
ساکن خالی cpuacct_css_free(ساخت cgroup_subsys_state *css)
{
	ساخت cpuacct *ca = css_ca(css);

	free_percpu(ca->cpustat);
	free_percpu(ca->cpuusage);
	kfree(ca);
}

ساکن u64 cpuacct_cpuusage_read(ساخت cpuacct *ca, صحیح cpu,
				 شمار cpuacct_stat_اشاریہ اشاریہ)
{
	u64 *cpuusage = فی_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = فی_cpu_ptr(ca->cpustat, cpu)->cpustat;
	u64 ڈیٹا;

	/*
	 * We allow index == CPUACCT_STAT_NSTATS here to read
	 * the sum of usages.
	 */
	اگر (WARN_ON_ONCE(اشاریہ > CPUACCT_STAT_NSTATS))
		واپس 0;

#اگر_غیر_تعریف_شدہ CONFIG_64_ثنائی_ہندسہ
	/*
	 * Take rq->lock to make 64-bit read safe on 32-bit platforms.
	 */
	raw_spin_rq_تالا_irq(cpu_rq(cpu));
#اگر_ختم

	انتخاب (اشاریہ) {
	حالت CPUACCT_STAT_USER:
		ڈیٹا = cpustat[CPUTIME_USER] + cpustat[CPUTIME_NICE];
		توڑو;
	حالت CPUACCT_STAT_SYSTEM:
		ڈیٹا = cpustat[CPUTIME_SYSTEM] + cpustat[CPUTIME_IRQ] +
			cpustat[CPUTIME_SOFTIRQ];
		توڑو;
	حالت CPUACCT_STAT_NSTATS:
		ڈیٹا = *cpuusage;
		توڑو;
	}

#اگر_غیر_تعریف_شدہ CONFIG_64_ثنائی_ہندسہ
	raw_spin_rq_unlock_irq(cpu_rq(cpu));
#اگر_ختم

	واپس ڈیٹا;
}

ساکن خالی cpuacct_cpuusage_write(ساخت cpuacct *ca, صحیح cpu)
{
	u64 *cpuusage = فی_cpu_ptr(ca->cpuusage, cpu);
	u64 *cpustat = فی_cpu_ptr(ca->cpustat, cpu)->cpustat;

	/* Don't allow to reset global kernel_cpustat */
	اگر (ca == &جڑ_cpuacct)
		واپس;

#اگر_غیر_تعریف_شدہ CONFIG_64_ثنائی_ہندسہ
	/*
	 * Take rq->lock to make 64-bit write safe on 32-bit platforms.
	 */
	raw_spin_rq_تالا_irq(cpu_rq(cpu));
#اگر_ختم
	*cpuusage = 0;
	cpustat[CPUTIME_USER] = cpustat[CPUTIME_NICE] = 0;
	cpustat[CPUTIME_SYSTEM] = cpustat[CPUTIME_IRQ] = 0;
	cpustat[CPUTIME_SOFTIRQ] = 0;

#اگر_غیر_تعریف_شدہ CONFIG_64_ثنائی_ہندسہ
	raw_spin_rq_unlock_irq(cpu_rq(cpu));
#اگر_ختم
}

/* Return total CPU usage (in nanoseconds) of a group */
ساکن u64 __cpuusage_read(ساخت cgroup_subsys_state *css,
			   شمار cpuacct_stat_اشاریہ اشاریہ)
{
	ساخت cpuacct *ca = css_ca(css);
	u64 totalcpuusage = 0;
	صحیح i;

	for_each_possible_cpu(i)
		totalcpuusage += cpuacct_cpuusage_read(ca, i, اشاریہ);

	واپس totalcpuusage;
}

ساکن u64 cpuusage_user_read(ساخت cgroup_subsys_state *css,
			      ساخت cftype *cft)
{
	واپس __cpuusage_read(css, CPUACCT_STAT_USER);
}

ساکن u64 cpuusage_sys_read(ساخت cgroup_subsys_state *css,
			     ساخت cftype *cft)
{
	واپس __cpuusage_read(css, CPUACCT_STAT_SYSTEM);
}

ساکن u64 cpuusage_read(ساخت cgroup_subsys_state *css, ساخت cftype *cft)
{
	واپس __cpuusage_read(css, CPUACCT_STAT_NSTATS);
}

ساکن صحیح cpuusage_write(ساخت cgroup_subsys_state *css, ساخت cftype *cft,
			  u64 val)
{
	ساخت cpuacct *ca = css_ca(css);
	صحیح cpu;

	/*
	 * Only allow '0' here to do a reset.
	 */
	اگر (val)
		واپس -EINVAL;

	for_each_possible_cpu(cpu)
		cpuacct_cpuusage_write(ca, cpu);

	واپس 0;
}

ساکن صحیح __cpuacct_percpu_seq_show(ساخت seq_file *m,
				     شمار cpuacct_stat_اشاریہ اشاریہ)
{
	ساخت cpuacct *ca = css_ca(seq_css(m));
	u64 percpu;
	صحیح i;

	for_each_possible_cpu(i) {
		percpu = cpuacct_cpuusage_read(ca, i, اشاریہ);
		seq_printf(m, "%llu ", (غیرعلامتی طویل طویل) percpu);
	}
	seq_printf(m, "\n");
	واپس 0;
}

ساکن صحیح cpuacct_percpu_user_seq_show(ساخت seq_file *m, خالی *V)
{
	واپس __cpuacct_percpu_seq_show(m, CPUACCT_STAT_USER);
}

ساکن صحیح cpuacct_percpu_sys_seq_show(ساخت seq_file *m, خالی *V)
{
	واپس __cpuacct_percpu_seq_show(m, CPUACCT_STAT_SYSTEM);
}

ساکن صحیح cpuacct_percpu_seq_show(ساخت seq_file *m, خالی *V)
{
	واپس __cpuacct_percpu_seq_show(m, CPUACCT_STAT_NSTATS);
}

ساکن صحیح cpuacct_all_seq_show(ساخت seq_file *m, خالی *V)
{
	ساخت cpuacct *ca = css_ca(seq_css(m));
	صحیح اشاریہ;
	صحیح cpu;

	seq_puts(m, "cpu");
	برائے (اشاریہ = 0; اشاریہ < CPUACCT_STAT_NSTATS; اشاریہ++)
		seq_printf(m, " %s", cpuacct_stat_desc[اشاریہ]);
	seq_puts(m, "\n");

	for_each_possible_cpu(cpu) {
		seq_printf(m, "%d", cpu);
		برائے (اشاریہ = 0; اشاریہ < CPUACCT_STAT_NSTATS; اشاریہ++)
			seq_printf(m, " %llu",
				   cpuacct_cpuusage_read(ca, cpu, اشاریہ));
		seq_puts(m, "\n");
	}
	واپس 0;
}

ساکن صحیح cpuacct_stats_show(ساخت seq_file *sf, خالی *v)
{
	ساخت cpuacct *ca = css_ca(seq_css(sf));
	ساخت task_cputime cputime;
	u64 val[CPUACCT_STAT_NSTATS];
	صحیح cpu;
	صحیح stat;

	memset(&cputime, 0, حجم(cputime));
	for_each_possible_cpu(cpu) {
		u64 *cpustat = فی_cpu_ptr(ca->cpustat, cpu)->cpustat;

		cputime.utime += cpustat[CPUTIME_USER];
		cputime.utime += cpustat[CPUTIME_NICE];
		cputime.stime += cpustat[CPUTIME_SYSTEM];
		cputime.stime += cpustat[CPUTIME_IRQ];
		cputime.stime += cpustat[CPUTIME_SOFTIRQ];

		cputime.مجموع_exec_runtime += *فی_cpu_ptr(ca->cpuusage, cpu);
	}

	cputime_adjust(&cputime, &seq_css(sf)->cgroup->پچھلا_cputime,
		&val[CPUACCT_STAT_USER], &val[CPUACCT_STAT_SYSTEM]);

	برائے (stat = 0; stat < CPUACCT_STAT_NSTATS; stat++) {
		seq_printf(sf, "%s %llu\n", cpuacct_stat_desc[stat],
			nsec_مقصد_clock_t(val[stat]));
	}

	واپس 0;
}

ساکن ساخت cftype files[] = {
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
خالی cpuacct_charge(ساخت task_struct *tsk, u64 cputime)
{
	غیرعلامتی صحیح cpu = task_cpu(tsk);
	ساخت cpuacct *ca;

	lockdep_assert_rq_held(cpu_rq(cpu));

	برائے (ca = task_ca(tsk); ca; ca = والد_ca(ca))
		*فی_cpu_ptr(ca->cpuusage, cpu) += cputime;
}

/*
 * Add user/system time to cpuacct.
 *
 * Note: it's the caller that updates the account of the root cgroup.
 */
خالی cpuacct_account_میدان(ساخت task_struct *tsk, صحیح اشاریہ, u64 val)
{
	ساخت cpuacct *ca;

	برائے (ca = task_ca(tsk); ca != &جڑ_cpuacct; ca = والد_ca(ca))
		__this_cpu_add(ca->cpustat->cpustat[اشاریہ], val);
}

ساخت cgroup_subsys cpuacct_cgrp_subsys = {
	.css_alloc	= cpuacct_css_alloc,
	.css_free	= cpuacct_css_free,
	.legacy_cftypes	= files,
	.early_init	= true,
};
