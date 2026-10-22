// SPDX-License-Identifier: GPL-2.0-only
/*
 * kernel/sched/debug.c
 *
 * Print the CFS rbtree and other debugging details
 *
 * Copyright(C) 2007, Red Hat, Inc., Ingo Molnar
 */
#شامل <linux/debugfs.h>
#شامل <linux/nmi.h>
#شامل <linux/log2.h>
#شامل "sched.h"

/*
 * This allows printing both to /sys/kernel/debug/sched/debug and
 * to the console
 */
#define SEQ_printf(m, x...)			\
 انجامبده {						\
	اگر (m)					\
		seq_printf(m, x);		\
	وگرنه					\
		pr_cont(x);			\
 } وقتی (0)

/*
 * Ease the printing of nsec fields:
 */
ایستا long long nsec_بالا(unsigned long long nsec)
{
	اگر ((long long)nsec < 0) {
		nsec = -nsec;
		do_div(nsec, 1000000);
		بازگشت -nsec;
	}
	do_div(nsec, 1000000);

	بازگشت nsec;
}

ایستا unsigned long nsec_کم(unsigned long long nsec)
{
	اگر ((long long)nsec < 0)
		nsec = -nsec;

	بازگشت do_div(nsec, 1000000);
}

#define SPLIT_NS(x) nsec_بالا(x), nsec_کم(x)

#define SCHED_FEAT(name, enabled)	\
	#name ,

ایستا ثابت نویسه * ثابت sched_feat_names[] = {
#شامل "features.h"
};

#undef SCHED_FEAT

ایستا صحیح sched_feat_show(ساختار seq_file *m, تهی *v)
{
	صحیح i;

	برای (i = 0; i < __SCHED_FEAT_NR; i++) {
		اگر (!(sysctl_sched_features & (1UL << i)))
			seq_puts(m, "NO_");
		seq_printf(m, "%s ", sched_feat_names[i]);
	}
	seq_puts(m, "\n");

	بازگشت 0;
}

#ifdef CONFIG_JUMP_LABEL

#define jump_label_کلید_true  { .کلید_true  = STATIC_کلید_TRUE_INIT  }
#define jump_label_کلید_false { .کلید_false = STATIC_کلید_FALSE_INIT }

#define SCHED_FEAT(name, enabled)	\
	jump_label_کلید##enabled ,

اجتماع sched_feat_کلید sched_feat_کلیدها[__SCHED_FEAT_NR] = {
#شامل "features.h"
};

#undef SCHED_FEAT

ایستا تهی sched_feat_disable(صحیح i)
{
	static_branch_disable_cpuslocked(&sched_feat_کلیدها[i].کلید_true);
}

ایستا تهی sched_feat_enable(صحیح i)
{
	static_branch_enable_cpuslocked(&sched_feat_کلیدها[i].کلید_false);
}
#وگرنه /* !CONFIG_JUMP_LABEL: */
ایستا تهی sched_feat_disable(صحیح i) { };
ایستا تهی sched_feat_enable(صحیح i) { };
#endif /* !CONFIG_JUMP_LABEL */

ایستا صحیح sched_feat_set(نویسه *cmp)
{
	صحیح i;
	صحیح neg = 0;

	اگر (strncmp(cmp, "NO_", 3) == 0) {
		neg = 1;
		cmp += 3;
	}

	i = match_string(sched_feat_names, __SCHED_FEAT_NR, cmp);
	اگر (i < 0)
		بازگشت i;

	اگر (neg) {
		sysctl_sched_features &= ~(1UL << i);
		sched_feat_disable(i);
	} وگرنه {
		sysctl_sched_features |= (1UL << i);
		sched_feat_enable(i);
	}

	بازگشت 0;
}

ایستا ssize_t
sched_feat_write(ساختار file *filp, ثابت نویسه __user *ubuf,
		size_t cnt, loff_t *ppos)
{
	نویسه buf[64];
	نویسه *cmp;
	صحیح ret;
	ساختار inode *inode;

	اگر (cnt > 63)
		cnt = 63;

	اگر (copy_from_user(buf, ubuf, cnt))
		بازگشت -EFAULT;

	buf[cnt] = 0;
	cmp = strstrip(buf);

	/* Ensure the static_key remains in a consistent state */
	inode = file_inode(filp);
	cpus_read_lock();
	inode_lock(inode);
	ret = sched_feat_set(cmp);
	inode_unlock(inode);
	cpus_read_unlock();
	اگر (ret < 0)
		بازگشت ret;

	*ppos += cnt;

	بازگشت cnt;
}

ایستا صحیح sched_feat_open(ساختار inode *inode, ساختار file *filp)
{
	بازگشت single_open(filp, sched_feat_show, پوچ);
}

ایستا ثابت ساختار file_operations sched_feat_fops = {
	.open		= sched_feat_open,
	.اكتب		= sched_feat_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ایستا ssize_t sched_scaling_write(ساختار file *filp, ثابت نویسه __user *ubuf,
				   size_t cnt, loff_t *ppos)
{
	unsigned صحیح scaling;
	صحیح ret;

	ret = kstrtouint_from_user(ubuf, cnt, 10, &scaling);
	اگر (ret)
		بازگشت ret;

	اگر (scaling >= SCHED_TUNABLESCALING_END)
		بازگشت -EINVAL;

	sysctl_sched_tunable_scaling = scaling;
	اگر (sched_update_scaling())
		بازگشت -EINVAL;

	*ppos += cnt;
	بازگشت cnt;
}

ایستا صحیح sched_scaling_show(ساختار seq_file *m, تهی *v)
{
	seq_printf(m, "%d\n", sysctl_sched_tunable_scaling);
	بازگشت 0;
}

ایستا صحیح sched_scaling_open(ساختار inode *inode, ساختار file *filp)
{
	بازگشت single_open(filp, sched_scaling_show, پوچ);
}

ایستا ثابت ساختار file_operations sched_scaling_fops = {
	.open		= sched_scaling_open,
	.اكتب		= sched_scaling_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#ifdef CONFIG_SCHED_CACHE
ایستا ssize_t
sched_cache_enable_write(ساختار file *filp, ثابت نویسه __user *ubuf,
			 size_t cnt, loff_t *ppos)
{
	bool val;
	صحیح ret;

	ret = kstrtobool_from_user(ubuf, cnt, &val);
	اگر (ret)
		بازگشت ret;

	sysctl_sched_cache_user = val;

	sched_cache_active_set();

	*ppos += cnt;

	بازگشت cnt;
}

ایستا صحیح sched_cache_enable_show(ساختار seq_file *m, تهی *v)
{
	seq_printf(m, "%d\n", sysctl_sched_cache_user);
	بازگشت 0;
}

ایستا صحیح sched_cache_enable_open(ساختار inode *inode,
				   ساختار file *filp)
{
	بازگشت single_open(filp, sched_cache_enable_show, پوچ);
}

ایستا ثابت ساختار file_operations sched_cache_enable_fops = {
	.open           = sched_cache_enable_open,
	.اكتب          = sched_cache_enable_write,
	.اقرأ           = seq_read,
	.llseek         = seq_lseek,
	.release        = single_release,
};
#endif

#ifdef CONFIG_PREEMPT_DYNAMIC

ایستا ssize_t sched_dynamic_write(ساختار file *filp, ثابت نویسه __user *ubuf,
				   size_t cnt, loff_t *ppos)
{
	نویسه buf[16];
	صحیح mode;

	اگر (cnt > 15)
		cnt = 15;

	اگر (copy_from_user(buf, ubuf, cnt))
		بازگشت -EFAULT;

	buf[cnt] = 0;
	mode = sched_dynamic_mode(strstrip(buf));
	اگر (mode < 0)
		بازگشت mode;

	sched_dynamic_update(mode);

	*ppos += cnt;

	بازگشت cnt;
}

ایستا صحیح sched_dynamic_show(ساختار seq_file *m, تهی *v)
{
	صحیح mode = READ_ONCE(preempt_dynamic_mode);

	/* Stop at NULL terminator */
	برای (صحیح i = 0; preempt_modes[i]; i++) {
		اگر (mode == i)
			seq_puts(m, "(");
		seq_puts(m, preempt_modes[i]);
		اگر (mode == i)
			seq_puts(m, ")");

		seq_puts(m, " ");
	}

	seq_puts(m, "\n");
	بازگشت 0;
}

ایستا صحیح sched_dynamic_open(ساختار inode *inode, ساختار file *filp)
{
	بازگشت single_open(filp, sched_dynamic_show, پوچ);
}

ایستا ثابت ساختار file_operations sched_dynamic_fops = {
	.open		= sched_dynamic_open,
	.اكتب		= sched_dynamic_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#endif /* CONFIG_PREEMPT_DYNAMIC */

__read_mostly bool sched_debug_verbose;

ایستا ساختار dentry           *sd_dentry;


ایستا ssize_t sched_verbose_write(ساختار file *filp, ثابت نویسه __user *ubuf,
				  size_t cnt, loff_t *ppos)
{
	ssize_t result;
	bool orig;

	cpus_read_lock();
	sched_domains_mutex_lock();

	orig = sched_debug_verbose;
	result = debugfs_write_file_bool(filp, ubuf, cnt, ppos);

	اگر (sched_debug_verbose && !orig)
		update_sched_domain_debugfs();
	وگرنه اگر (!sched_debug_verbose && orig) {
		debugfs_remove(sd_dentry);
		sd_dentry = پوچ;
	}

	sched_domains_mutex_unlock();
	cpus_read_unlock();

	بازگشت result;
}

ایستا ثابت ساختار file_operations sched_verbose_fops = {
	.اقرأ =         debugfs_read_file_bool,
	.اكتب =        sched_verbose_write,
	.open =         simple_open,
	.llseek =       default_llseek,
};

ایستا ثابت ساختار seq_operations sched_debug_sops;

ایستا صحیح sched_debug_open(ساختار inode *inode, ساختار file *filp)
{
	بازگشت seq_open(filp, &sched_debug_sops);
}

ایستا ثابت ساختار file_operations sched_debug_fops = {
	.open		= sched_debug_open,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

شمارش dl_param {
	DL_RUNTIME = 0,
	DL_PERIOD,
};

ایستا unsigned long dl_server_period_max = (1UL << 22) * NSEC_PER_USEC; /* ~4 seconds */
ایستا unsigned long dl_server_period_min = (100) * NSEC_PER_USEC;     /* 100 us */

ایستا ssize_t sched_server_write_common(ساختار file *filp, ثابت نویسه __user *ubuf,
					 size_t cnt, loff_t *ppos, شمارش dl_param param,
					 تهی *server)
{
	long cpu = (long) ((ساختار seq_file *) filp->private_data)->خصوصی;
	ساختار sched_dl_entity *dl_se = (ساختار sched_dl_entity *)server;
	u64 old_runtime, runtime, period;
	ساختار rq *rq = cpu_rq(cpu);
	صحیح retval = 0;
	size_t err;
	u64 مقدار;

	err = kstrtoull_from_user(ubuf, cnt, 10, &مقدار);
	اگر (err)
		بازگشت err;

	scoped_guard (rq_lock_irqsave, rq) {
		old_runtime = runtime = dl_se->dl_runtime;
		period = dl_se->dl_period;

		انتخاب (param) {
		حالت DL_RUNTIME:
			اگر (runtime == مقدار)
				بشکن;
			runtime = مقدار;
			بشکن;
		حالت DL_PERIOD:
			اگر (مقدار == period)
				بشکن;
			period = مقدار;
			بشکن;
		}

		اگر (runtime > period ||
		    period > dl_server_period_max ||
		    period < dl_server_period_min) {
			بازگشت  -EINVAL;
		}

		اگر (!cpu_online(cpu_of(rq)))
			بازگشت -EBUSY;

		update_rq_clock(rq);
		dl_server_stop(dl_se);
		retval = dl_server_apply_params(dl_se, runtime, period, 0);
		dl_server_start(dl_se);

		اگر (retval < 0)
			بازگشت retval;
	}

	اگر (!!old_runtime ^ !!runtime) {
		pr_info("%s server %sabled on CPU %d%s.\n",
			server == &rq->fair_server ? "Fair" : "Ext",
			runtime ? "en" : "dis",
			cpu_of(rq),
			runtime ? "" : ", system may malfunction due to starvation");
	}

	*ppos += cnt;
	بازگشت cnt;
}

ایستا size_t sched_server_show_common(ساختار seq_file *m, تهی *v, شمارش dl_param param,
				       تهی *server)
{
	ساختار sched_dl_entity *dl_se = (ساختار sched_dl_entity *)server;
	u64 مقدار;

	انتخاب (param) {
	حالت DL_RUNTIME:
		مقدار = dl_se->dl_runtime;
		بشکن;
	حالت DL_PERIOD:
		مقدار = dl_se->dl_period;
		بشکن;
	}

	seq_printf(m, "%llu\n", مقدار);
	بازگشت 0;
}

ایستا ssize_t
sched_fair_server_runtime_write(ساختار file *filp, ثابت نویسه __user *ubuf,
				size_t cnt, loff_t *ppos)
{
	long cpu = (long) ((ساختار seq_file *) filp->private_data)->خصوصی;
	ساختار rq *rq = cpu_rq(cpu);

	بازگشت sched_server_write_common(filp, ubuf, cnt, ppos, DL_RUNTIME,
					&rq->fair_server);
}

ایستا صحیح sched_fair_server_runtime_show(ساختار seq_file *m, تهی *v)
{
	unsigned long cpu = (unsigned long) m->خصوصی;
	ساختار rq *rq = cpu_rq(cpu);

	بازگشت sched_server_show_common(m, v, DL_RUNTIME, &rq->fair_server);
}

ایستا صحیح sched_fair_server_runtime_open(ساختار inode *inode, ساختار file *filp)
{
	بازگشت single_open(filp, sched_fair_server_runtime_show, inode->i_private);
}

ایستا ثابت ساختار file_operations fair_server_runtime_fops = {
	.open		= sched_fair_server_runtime_open,
	.اكتب		= sched_fair_server_runtime_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ایستا ساختار dentry *debugfs_sched;

#ifdef CONFIG_SCHED_CLASS_EXT
ایستا ssize_t
sched_ext_server_runtime_write(ساختار file *filp, ثابت نویسه __user *ubuf,
			       size_t cnt, loff_t *ppos)
{
	long cpu = (long) ((ساختار seq_file *) filp->private_data)->خصوصی;
	ساختار rq *rq = cpu_rq(cpu);

	بازگشت sched_server_write_common(filp, ubuf, cnt, ppos, DL_RUNTIME,
					&rq->ext_server);
}

ایستا صحیح sched_ext_server_runtime_show(ساختار seq_file *m, تهی *v)
{
	unsigned long cpu = (unsigned long) m->خصوصی;
	ساختار rq *rq = cpu_rq(cpu);

	بازگشت sched_server_show_common(m, v, DL_RUNTIME, &rq->ext_server);
}

ایستا صحیح sched_ext_server_runtime_open(ساختار inode *inode, ساختار file *filp)
{
	بازگشت single_open(filp, sched_ext_server_runtime_show, inode->i_private);
}

ایستا ثابت ساختار file_operations ext_server_runtime_fops = {
	.open		= sched_ext_server_runtime_open,
	.اكتب		= sched_ext_server_runtime_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ایستا ssize_t
sched_ext_server_period_write(ساختار file *filp, ثابت نویسه __user *ubuf,
			      size_t cnt, loff_t *ppos)
{
	long cpu = (long) ((ساختار seq_file *) filp->private_data)->خصوصی;
	ساختار rq *rq = cpu_rq(cpu);

	بازگشت sched_server_write_common(filp, ubuf, cnt, ppos, DL_PERIOD,
					&rq->ext_server);
}

ایستا صحیح sched_ext_server_period_show(ساختار seq_file *m, تهی *v)
{
	unsigned long cpu = (unsigned long) m->خصوصی;
	ساختار rq *rq = cpu_rq(cpu);

	بازگشت sched_server_show_common(m, v, DL_PERIOD, &rq->ext_server);
}

ایستا صحیح sched_ext_server_period_open(ساختار inode *inode, ساختار file *filp)
{
	بازگشت single_open(filp, sched_ext_server_period_show, inode->i_private);
}

ایستا ثابت ساختار file_operations ext_server_period_fops = {
	.open		= sched_ext_server_period_open,
	.اكتب		= sched_ext_server_period_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ایستا تهی debugfs_ext_server_init(تهی)
{
	ساختار dentry *d_ext;
	unsigned long cpu;

	d_ext = debugfs_create_dir("ext_server", debugfs_sched);
	اگر (!d_ext)
		بازگشت;

	for_each_possible_cpu(cpu) {
		ساختار dentry *d_cpu;
		نویسه buf[32];

		snprintf(buf, اندازه(buf), "cpu%lu", cpu);
		d_cpu = debugfs_create_dir(buf, d_ext);

		debugfs_create_file("runtime", 0644, d_cpu, (تهی *) cpu, &ext_server_runtime_fops);
		debugfs_create_file("period", 0644, d_cpu, (تهی *) cpu, &ext_server_period_fops);
	}
}
#endif /* CONFIG_SCHED_CLASS_EXT */

ایستا ssize_t
sched_fair_server_period_write(ساختار file *filp, ثابت نویسه __user *ubuf,
			       size_t cnt, loff_t *ppos)
{
	long cpu = (long) ((ساختار seq_file *) filp->private_data)->خصوصی;
	ساختار rq *rq = cpu_rq(cpu);

	بازگشت sched_server_write_common(filp, ubuf, cnt, ppos, DL_PERIOD,
					&rq->fair_server);
}

ایستا صحیح sched_fair_server_period_show(ساختار seq_file *m, تهی *v)
{
	unsigned long cpu = (unsigned long) m->خصوصی;
	ساختار rq *rq = cpu_rq(cpu);

	بازگشت sched_server_show_common(m, v, DL_PERIOD, &rq->fair_server);
}

ایستا صحیح sched_fair_server_period_open(ساختار inode *inode, ساختار file *filp)
{
	بازگشت single_open(filp, sched_fair_server_period_show, inode->i_private);
}

ایستا ثابت ساختار file_operations fair_server_period_fops = {
	.open		= sched_fair_server_period_open,
	.اكتب		= sched_fair_server_period_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ایستا تهی debugfs_fair_server_init(تهی)
{
	ساختار dentry *d_fair;
	unsigned long cpu;

	d_fair = debugfs_create_dir("fair_server", debugfs_sched);
	اگر (!d_fair)
		بازگشت;

	for_each_possible_cpu(cpu) {
		ساختار dentry *d_cpu;
		نویسه buf[32];

		snprintf(buf, اندازه(buf), "cpu%lu", cpu);
		d_cpu = debugfs_create_dir(buf, d_fair);

		debugfs_create_file("runtime", 0644, d_cpu, (تهی *) cpu, &fair_server_runtime_fops);
		debugfs_create_file("period", 0644, d_cpu, (تهی *) cpu, &fair_server_period_fops);
	}
}

#ifdef CONFIG_FAIR_GROUP_SCHED
ایستا صحیح cgroup_mode = 2;

/* See __sched_cgroup_mode_update(). */
ایستا ثابت نویسه *cgroup_mode_str[] = {
	"up",
	"smp",
	"concur",
	"max",
	"tasks",
};

ایستا صحیح sched_cgroup_mode(ثابت نویسه *str)
{
	برای (صحیح i = 0; i < ARRAY_SIZE(cgroup_mode_str); i++) {
		اگر (!strcmp(str, cgroup_mode_str[i]))
			بازگشت i;
	}
	بازگشت -EINVAL;
}

ایستا ssize_t sched_cgroup_write(ساختار file *filp, ثابت نویسه __user *ubuf,
				   size_t cnt, loff_t *ppos)
{
	نویسه buf[16];
	صحیح mode;

	اگر (cnt > 15)
		cnt = 15;

	اگر (copy_from_user(buf, ubuf, cnt))
		بازگشت -EFAULT;

	buf[cnt] = 0;
	mode = sched_cgroup_mode(strstrip(buf));
	اگر (mode < 0)
		بازگشت mode;

	__sched_cgroup_mode_update(mode);
	WRITE_ONCE(cgroup_mode, mode);

	*ppos += cnt;
	بازگشت cnt;
}

ایستا صحیح sched_cgroup_show(ساختار seq_file *m, تهی *v)
{
	صحیح mode = READ_ONCE(cgroup_mode);

	برای (صحیح i = 0; i < ARRAY_SIZE(cgroup_mode_str); i++) {
		اگر (mode == i)
			seq_puts(m, "(");
		seq_puts(m, cgroup_mode_str[i]);
		اگر (mode == i)
			seq_puts(m, ")");

		seq_puts(m, " ");
	}
	seq_puts(m, "\n");
	بازگشت 0;
}

ایستا صحیح sched_cgroup_open(ساختار inode *inode, ساختار file *filp)
{
	بازگشت single_open(filp, sched_cgroup_show, پوچ);
}

ایستا ثابت ساختار file_operations sched_cgroup_fops = {
	.open		= sched_cgroup_open,
	.اكتب		= sched_cgroup_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};
#endif

ایستا __init صحیح sched_init_debug(تهی)
{
	ساختار dentry __maybe_unused *numa, *llc;

	debugfs_sched = debugfs_create_dir("sched", پوچ);

	debugfs_create_file("features", 0644, debugfs_sched, پوچ, &sched_feat_fops);
	debugfs_create_file_unsafe("verbose", 0644, debugfs_sched, &sched_debug_verbose, &sched_verbose_fops);
#ifdef CONFIG_PREEMPT_DYNAMIC
	debugfs_create_file("preempt", 0644, debugfs_sched, پوچ, &sched_dynamic_fops);
#endif

	debugfs_create_u32("base_slice_ns", 0644, debugfs_sched, &sysctl_sched_base_slice);

	debugfs_create_u32("latency_warn_ms", 0644, debugfs_sched, &sysctl_resched_latency_warn_ms);
	debugfs_create_u32("latency_warn_once", 0644, debugfs_sched, &sysctl_resched_latency_warn_once);

	debugfs_create_file("tunable_scaling", 0644, debugfs_sched, پوچ, &sched_scaling_fops);
	debugfs_create_u32("migration_cost_ns", 0644, debugfs_sched, &sysctl_sched_migration_cost);
	debugfs_create_u32("nr_migrate", 0644, debugfs_sched, &sysctl_sched_nr_migrate);

	sched_domains_mutex_lock();
	update_sched_domain_debugfs();
	sched_domains_mutex_unlock();

#ifdef CONFIG_NUMA_BALANCING
	numa = debugfs_create_dir("numa_balancing", debugfs_sched);

	debugfs_create_u32("scan_delay_ms", 0644, numa, &sysctl_numa_balancing_scan_delay);
	debugfs_create_u32("scan_period_min_ms", 0644, numa, &sysctl_numa_balancing_scan_period_min);
	debugfs_create_u32("scan_period_max_ms", 0644, numa, &sysctl_numa_balancing_scan_period_max);
	debugfs_create_u32("scan_size_mb", 0644, numa, &sysctl_numa_balancing_scan_size);
	debugfs_create_u32("hot_threshold_ms", 0644, numa, &sysctl_numa_balancing_hot_threshold);
#endif /* CONFIG_NUMA_BALANCING */

#ifdef CONFIG_SCHED_CACHE
	llc = debugfs_create_dir("llc_balancing", debugfs_sched);
	debugfs_create_file("enabled", 0644, llc, پوچ,
			    &sched_cache_enable_fops);
	debugfs_create_u32("aggr_tolerance", 0644, llc,
			   &llc_aggr_tolerance);
	debugfs_create_u32("epoch_period", 0644, llc,
			   &llc_epoch_period);
	debugfs_create_u32("epoch_affinity_timeout", 0644, llc,
			   &llc_epoch_affinity_timeout);
	debugfs_create_u32("overaggr_pct", 0644, llc,
			   &llc_overaggr_pct);
	debugfs_create_u32("imb_pct", 0644, llc,
			   &llc_imb_pct);
#endif

	debugfs_create_file("debug", 0444, debugfs_sched, پوچ, &sched_debug_fops);

#ifdef CONFIG_FAIR_GROUP_SCHED
	debugfs_create_file("cgroup_mode", 0644, debugfs_sched, پوچ, &sched_cgroup_fops);
#endif

	debugfs_fair_server_init();
#ifdef CONFIG_SCHED_CLASS_EXT
	debugfs_ext_server_init();
#endif

	بازگشت 0;
}
late_initcall(sched_init_debug);

ایستا cpumask_var_t		sd_sysctl_cpus;

ایستا صحیح sd_flags_show(ساختار seq_file *m, تهی *v)
{
	unsigned long flags = *(unsigned صحیح *)m->خصوصی;
	صحیح idx;

	for_each_set_bit(idx, &flags, __SD_FLAG_CNT) {
		seq_puts(m, sd_flag_debug[idx].name);
		seq_puts(m, " ");
	}
	seq_puts(m, "\n");

	بازگشت 0;
}

ایستا صحیح sd_flags_open(ساختار inode *inode, ساختار file *file)
{
	بازگشت single_open(file, sd_flags_show, inode->i_private);
}

ایستا ثابت ساختار file_operations sd_flags_fops = {
	.open		= sd_flags_open,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ایستا تهی register_sd(ساختار sched_domain *sd, ساختار dentry *parent)
{
#define SDM(type, mode, member)	\
	debugfs_create_##type(#member, mode, parent, &sd->member)

	SDM(ulong, 0644, min_interval);
	SDM(ulong, 0644, max_interval);
	SDM(u64,   0644, max_newidle_lb_cost);
	SDM(u32,   0644, busy_factor);
	SDM(u32,   0644, imbalance_pct);
	SDM(u32,   0644, cache_nice_tries);
	SDM(str,   0444, name);

#undef SDM

	debugfs_create_file("flags", 0444, parent, &sd->flags, &sd_flags_fops);
	debugfs_create_file("groups_flags", 0444, parent, &sd->groups->flags, &sd_flags_fops);
	debugfs_create_u32("level", 0444, parent, (u32 *)&sd->level);

	اگر (sd->flags & SD_ASYM_PACKING)
		debugfs_create_u32("group_asym_prefer_cpu", 0444, parent,
				   (u32 *)&sd->groups->asym_prefer_cpu);
}

تهی update_sched_domain_debugfs(تهی)
{
	صحیح cpu, i;

	/*
	 * This can unfortunately be invoked before sched_debug_init() creates
	 * the debug directory. Don't touch sd_sysctl_cpus until then.
	 */
	اگر (!debugfs_sched)
		بازگشت;

	اگر (!sched_debug_verbose)
		بازگشت;

	اگر (!cpumask_available(sd_sysctl_cpus)) {
		اگر (!alloc_cpumask_var(&sd_sysctl_cpus, GFP_KERNEL))
			بازگشت;
		cpumask_copy(sd_sysctl_cpus, cpu_possible_mask);
	}

	اگر (!sd_dentry) {
		sd_dentry = debugfs_create_dir("domains", debugfs_sched);

		/* rebuild sd_sysctl_cpus if empty since it gets cleared below */
		اگر (cpumask_empty(sd_sysctl_cpus))
			cpumask_copy(sd_sysctl_cpus, cpu_online_mask);
	}

	for_each_cpu(cpu, sd_sysctl_cpus) {
		ساختار sched_domain *sd;
		ساختار dentry *d_cpu;
		نویسه buf[32];

		snprintf(buf, اندازه(buf), "cpu%d", cpu);
		debugfs_lookup_and_remove(buf, sd_dentry);
		d_cpu = debugfs_create_dir(buf, sd_dentry);

		i = 0;
		for_each_domain(cpu, sd) {
			ساختار dentry *d_sd;

			snprintf(buf, اندازه(buf), "domain%d", i);
			d_sd = debugfs_create_dir(buf, d_cpu);

			register_sd(sd, d_sd);
			i++;
		}

		__cpumask_clear_cpu(cpu, sd_sysctl_cpus);
	}
}

تهی dirty_sched_domain_sysctl(صحیح cpu)
{
	اگر (cpumask_available(sd_sysctl_cpus))
		__cpumask_set_cpu(cpu, sd_sysctl_cpus);
}

#ifdef CONFIG_FAIR_GROUP_SCHED
ایستا تهی print_cfs_group_stats(ساختار seq_file *m, صحیح cpu, ساختار task_group *tg)
{
	ساختار sched_entity *se = tg_se(tg, cpu);

#define P(F)		SEQ_printf(m, "  .%-30s: %lld\n",	#F, (long long)F)
#define P_SCHEDSTAT(F)	SEQ_printf(m, "  .%-30s: %lld\n",	\
		#F, (long long)schedstat_val(stats->F))
#define PN(F)		SEQ_printf(m, "  .%-30s: %lld.%06ld\n", #F, SPLIT_NS((long long)F))
#define PN_SCHEDSTAT(F)	SEQ_printf(m, "  .%-30s: %lld.%06ld\n", \
		#F, SPLIT_NS((long long)schedstat_val(stats->F)))

	اگر (!se)
		بازگشت;

	PN(se->exec_start);
	PN(se->vruntime);
	PN(se->sum_exec_runtime);

	اگر (schedstat_enabled()) {
		ساختار sched_statistics *stats;
		stats = __schedstats_from_se(se);

		PN_SCHEDSTAT(wait_start);
		PN_SCHEDSTAT(sleep_start);
		PN_SCHEDSTAT(block_start);
		PN_SCHEDSTAT(sleep_max);
		PN_SCHEDSTAT(block_max);
		PN_SCHEDSTAT(exec_max);
		PN_SCHEDSTAT(slice_max);
		PN_SCHEDSTAT(wait_max);
		PN_SCHEDSTAT(wait_sum);
		P_SCHEDSTAT(wait_شمار);
	}

	P(se->load.weight);
	P(se->avg.load_avg);
	P(se->avg.util_avg);
	P(se->avg.runnable_avg);

#undef PN_SCHEDSTAT
#undef PN
#undef P_SCHEDSTAT
#undef P
}
#endif /* CONFIG_FAIR_GROUP_SCHED */

#ifdef CONFIG_CGROUP_SCHED
ایستا DEFINE_SPINLOCK(sched_debug_lock);
ایستا نویسه group_path[PATH_MAX];

ایستا تهی task_group_path(ساختار task_group *tg, نویسه *path, صحیح plen)
{
	اگر (autogroup_path(tg, path, plen))
		بازگشت;

	cgroup_path(tg->css.cgroup, path, plen);
}

/*
 * Only 1 SEQ_printf_task_group_path() caller can use the full length
 * group_path[] for cgroup path. Other simultaneous callers will have
 * to use a shorter stack buffer. A "..." suffix is appended at the end
 * of the stack buffer so that it will show up in case the output length
 * matches the given buffer size to indicate possible path name truncation.
 */
#define SEQ_printf_task_group_path(m, tg, fmt...)			\
{									\
	اگر (spin_trylock(&sched_debug_lock)) {				\
		task_group_path(tg, group_path, اندازه(group_path));	\
		SEQ_printf(m, fmt, group_path);				\
		spin_unlock(&sched_debug_lock);				\
	} وگرنه {							\
		نویسه buf[128];						\
		نویسه *bufend = buf + اندازه(buf) - 3;			\
		task_group_path(tg, buf, bufend - buf);			\
		strcpy(bufend - 1, "...");				\
		SEQ_printf(m, fmt, buf);				\
	}								\
}
#endif

ایستا تهی
print_task(ساختار seq_file *m, ساختار rq *rq, ساختار task_struct *p)
{
	اگر (task_current(rq, p))
		SEQ_printf(m, ">R");
	وگرنه
		SEQ_printf(m, " %c", task_state_to_char(p));

	SEQ_printf(m, " %15s %5d %10ld %9Ld.%06ld   %c   %9Ld.%06ld %c %9Ld.%06ld %9Ld.%06ld %9Ld   %5d ",
		p->comm, task_pid_nr(p),
		p->se.h_load.weight,
		SPLIT_NS(p->se.vruntime),
		entity_eligible(&rq->cfs, &p->se) ? 'E' : 'N',
		SPLIT_NS(p->se.deadline),
		p->se.custom_slice ? 'S' : ' ',
		SPLIT_NS(p->se.slice),
		SPLIT_NS(p->se.sum_exec_runtime),
		(long long)(p->nvcsw + p->nivcsw),
		p->prio);

	SEQ_printf(m, "%9lld.%06ld %9lld.%06ld %9lld.%06ld",
		SPLIT_NS(schedstat_val_or_zero(p->stats.wait_sum)),
		SPLIT_NS(schedstat_val_or_zero(p->stats.sum_sleep_runtime)),
		SPLIT_NS(schedstat_val_or_zero(p->stats.sum_block_runtime)));

#ifdef CONFIG_NUMA_BALANCING
	SEQ_printf(m, "   %d      %d", task_گره(p), task_numa_group_id(p));
#endif
#ifdef CONFIG_CGROUP_SCHED
	SEQ_printf_task_group_path(m, task_group(p), "        %s")
#endif

	SEQ_printf(m, "\n");
}

ایستا تهی print_rq(ساختار seq_file *m, ساختار rq *rq, صحیح rq_cpu)
{
	ساختار task_struct *g, *p;

	SEQ_printf(m, "\n");
	SEQ_printf(m, "runnable tasks:\n");
	SEQ_printf(m, " S            task   PID     weight       vruntime   eligible    "
		   "deadline             slice          sum-exec      switches  "
		   "prio         wait-time        sum-sleep       sum-block"
#ifdef CONFIG_NUMA_BALANCING
		   "  node   group-id"
#endif
#ifdef CONFIG_CGROUP_SCHED
		   "  group-path"
#endif
		   "\n");
	SEQ_printf(m, "-------------------------------------------------------"
		   "------------------------------------------------------"
		   "------------------------------------------------------"
#ifdef CONFIG_NUMA_BALANCING
		   "--------------"
#endif
#ifdef CONFIG_CGROUP_SCHED
		   "--------------"
#endif
		   "\n");

	rcu_read_lock();
	for_each_process_thread(g, p) {
		اگر (task_cpu(p) != rq_cpu)
			ادامه;

		print_task(m, rq, p);
	}
	rcu_read_unlock();
}

تهی print_cfs_rq(ساختار seq_file *m, صحیح cpu, ساختار cfs_rq *cfs_rq)
{
	s64 چپ_vruntime = -1, راست_vruntime = -1, چپ_deadline = -1, spread;
	s64 zero_vruntime = -1, sum_w_vruntime = -1;
	u64 avruntime;
	ساختار sched_entity *last, *first, *ریشه;
	ساختار rq *rq = cpu_rq(cpu);
	unsigned صحیح sum_shift;
	unsigned long flags;
	u64 sum_weight;

#ifdef CONFIG_FAIR_GROUP_SCHED
	SEQ_printf(m, "\n");
	SEQ_printf_task_group_path(m, cfs_rq->tg, "cfs_rq[%d]:%s\n", cpu);
#وگرنه
	SEQ_printf(m, "\n");
	SEQ_printf(m, "cfs_rq[%d]:\n", cpu);
#endif

	raw_spin_rq_lock_irqsave(rq, flags);
	ریشه = __pick_root_entity(cfs_rq);
	اگر (ریشه)
		چپ_vruntime = ریشه->min_vruntime;
	first = __pick_first_entity(cfs_rq);
	اگر (first)
		چپ_deadline = first->deadline;
	last = __pick_last_entity(cfs_rq);
	اگر (last)
		راست_vruntime = last->vruntime;
	zero_vruntime = cfs_rq->zero_vruntime;
	sum_w_vruntime = cfs_rq->sum_w_vruntime;
	sum_weight = cfs_rq->sum_weight;
	sum_shift = cfs_rq->sum_shift;
	avruntime = avg_vruntime(cfs_rq);
	raw_spin_rq_unlock_irqrestore(rq, flags);

	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "left_deadline",
			SPLIT_NS(چپ_deadline));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "left_vruntime",
			SPLIT_NS(چپ_vruntime));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "zero_vruntime",
			SPLIT_NS(zero_vruntime));
	SEQ_printf(m, "  .%-30s: %Ld (%d bits)\n", "sum_w_vruntime",
		   sum_w_vruntime, ilog2(abs(sum_w_vruntime)));
	SEQ_printf(m, "  .%-30s: %Lu\n", "sum_weight",
		   sum_weight);
	SEQ_printf(m, "  .%-30s: %u\n", "sum_shift", sum_shift);
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "avg_vruntime",
			SPLIT_NS(avruntime));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "right_vruntime",
			SPLIT_NS(راست_vruntime));
	spread = راست_vruntime - چپ_vruntime;
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "spread", SPLIT_NS(spread));
	SEQ_printf(m, "  .%-30s: %d\n", "nr_queued", cfs_rq->nr_queued);
	SEQ_printf(m, "  .%-30s: %d\n", "h_nr_runnable", cfs_rq->h_nr_runnable);
	SEQ_printf(m, "  .%-30s: %d\n", "h_nr_queued", cfs_rq->h_nr_queued);
	SEQ_printf(m, "  .%-30s: %d\n", "h_nr_idle", cfs_rq->h_nr_idle);
	SEQ_printf(m, "  .%-30s: %ld\n", "load", cfs_rq->load.weight);
	SEQ_printf(m, "  .%-30s: %lu\n", "load_avg",
			cfs_rq->avg.load_avg);
	SEQ_printf(m, "  .%-30s: %lu\n", "runnable_avg",
			cfs_rq->avg.runnable_avg);
	SEQ_printf(m, "  .%-30s: %lu\n", "util_avg",
			cfs_rq->avg.util_avg);
	SEQ_printf(m, "  .%-30s: %u\n", "util_est",
			cfs_rq->avg.util_est);
	SEQ_printf(m, "  .%-30s: %ld\n", "removed.load_avg",
			cfs_rq->removed.load_avg);
	SEQ_printf(m, "  .%-30s: %ld\n", "removed.util_avg",
			cfs_rq->removed.util_avg);
	SEQ_printf(m, "  .%-30s: %ld\n", "removed.runnable_avg",
			cfs_rq->removed.runnable_avg);
#ifdef CONFIG_FAIR_GROUP_SCHED
	SEQ_printf(m, "  .%-30s: %lu\n", "tg_load_avg_contrib",
			cfs_rq->tg_load_avg_contrib);
	SEQ_printf(m, "  .%-30s: %ld\n", "tg_load_avg",
			atomic_long_read(&cfs_rq->tg->load_avg));
	SEQ_printf(m, "  .%-30s: %lu\n", "h_load",
			cfs_rq->h_load);
#endif /* CONFIG_FAIR_GROUP_SCHED */
#ifdef CONFIG_CFS_BANDWIDTH
	SEQ_printf(m, "  .%-30s: %d\n", "throttled",
			cfs_rq->throttled);
	SEQ_printf(m, "  .%-30s: %d\n", "throttle_count",
			cfs_rq->throttle_شمار);
#endif

#ifdef CONFIG_FAIR_GROUP_SCHED
	print_cfs_group_stats(m, cpu, cfs_rq->tg);
#endif
}

تهی print_rt_rq(ساختار seq_file *m, صحیح cpu, ساختار rt_rq *rt_rq)
{
#ifdef CONFIG_RT_GROUP_SCHED
	SEQ_printf(m, "\n");
	SEQ_printf_task_group_path(m, rt_rq->tg, "rt_rq[%d]:%s\n", cpu);
#وگرنه
	SEQ_printf(m, "\n");
	SEQ_printf(m, "rt_rq[%d]:\n", cpu);
#endif

#define P(x) \
	SEQ_printf(m, "  .%-30s: %Ld\n", #x, (long long)(rt_rq->x))
#define PU(x) \
	SEQ_printf(m, "  .%-30s: %lu\n", #x, (unsigned long)(rt_rq->x))
#define PN(x) \
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", #x, SPLIT_NS(rt_rq->x))

	PU(rt_nr_running);

#ifdef CONFIG_RT_GROUP_SCHED
	P(rt_throttled);
	PN(rt_time);
	PN(rt_runtime);
#endif

#undef PN
#undef PU
#undef P
}

تهی print_dl_rq(ساختار seq_file *m, صحیح cpu, ساختار dl_rq *dl_rq)
{
	ساختار dl_bw *dl_bw;

	SEQ_printf(m, "\n");
	SEQ_printf(m, "dl_rq[%d]:\n", cpu);

#define PU(x) \
	SEQ_printf(m, "  .%-30s: %lu\n", #x, (unsigned long)(dl_rq->x))

	PU(dl_nr_running);
	dl_bw = &cpu_rq(cpu)->rd->dl_bw;
	SEQ_printf(m, "  .%-30s: %lld\n", "dl_bw->bw", dl_bw->bw);
	SEQ_printf(m, "  .%-30s: %lld\n", "dl_bw->total_bw", dl_bw->total_bw);

#undef PU
}

ایستا تهی print_cpu(ساختار seq_file *m, صحیح cpu)
{
	ساختار rq *rq = cpu_rq(cpu);

#ifdef CONFIG_X86
	{
		unsigned صحیح freq = cpu_khz ? : 1;

		SEQ_printf(m, "cpu#%d, %u.%03u MHz\n",
			   cpu, freq / 1000, (freq % 1000));
	}
#وگرنه /* !CONFIG_X86: */
	SEQ_printf(m, "cpu#%d\n", cpu);
#endif /* !CONFIG_X86 */

#define P(x)								\
انجامبده {									\
	اگر (اندازه(rq->x) == 4)						\
		SEQ_printf(m, "  .%-30s: %d\n", #x, (صحیح)(rq->x));	\
	وگرنه								\
		SEQ_printf(m, "  .%-30s: %Ld\n", #x, (long long)(rq->x));\
} وقتی (0)

#define PN(x) \
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", #x, SPLIT_NS(rq->x))

	P(nr_running);
	P(nr_switches);
	P(nr_uninterruptible);
	PN(next_balance);
	SEQ_printf(m, "  .%-30s: %ld\n", "curr->pid", (long)(task_pid_nr(rq->curr)));
	PN(clock);
	PN(clock_task);
#undef P
#undef PN

#define P64(n) SEQ_printf(m, "  .%-30s: %Ld\n", #n, rq->n);
	P64(avg_idle);
	P64(max_idle_balance_cost);
#undef P64

#define P(n) SEQ_printf(m, "  .%-30s: %d\n", #n, schedstat_val(rq->n));
	اگر (schedstat_enabled()) {
		P(yld_شمار);
		P(sched_شمار);
		P(sched_goidle);
		P(ttwu_شمار);
		P(ttwu_local);
	}
#undef P

	print_cfs_stats(m, cpu);
	print_rt_stats(m, cpu);
	print_dl_stats(m, cpu);

	print_rq(m, rq, cpu);
	SEQ_printf(m, "\n");
}

ایستا ثابت نویسه *sched_tunable_scaling_names[] = {
	"none",
	"logarithmic",
	"linear"
};

ایستا تهی sched_debug_header(ساختار seq_file *m)
{
	u64 ktime, sched_clk, cpu_clk;
	unsigned long flags;

	local_irq_save(flags);
	ktime = ktime_to_ns(ktime_get());
	sched_clk = sched_clock();
	cpu_clk = local_clock();
	local_irq_restore(flags);

	SEQ_printf(m, "Sched Debug Version: v0.11, %s %.*s\n",
		init_utsname()->release,
		(صحیح)strcspn(init_utsname()->version, " "),
		init_utsname()->version);

#define P(x) \
	SEQ_printf(m, "%-40s: %Ld\n", #x, (long long)(x))
#define PN(x) \
	SEQ_printf(m, "%-40s: %Ld.%06ld\n", #x, SPLIT_NS(x))
	PN(ktime);
	PN(sched_clk);
	PN(cpu_clk);
	P(jiffies);
#ifdef CONFIG_HAVE_UNSTABLE_SCHED_CLOCK
	P(sched_clock_stable());
#endif
#undef PN
#undef P

	SEQ_printf(m, "\n");
	SEQ_printf(m, "sysctl_sched\n");

#define P(x) \
	SEQ_printf(m, "  .%-40s: %Ld\n", #x, (long long)(x))
#define PN(x) \
	SEQ_printf(m, "  .%-40s: %Ld.%06ld\n", #x, SPLIT_NS(x))
	PN(sysctl_sched_base_slice);
	P(sysctl_sched_features);
#undef PN
#undef P

	SEQ_printf(m, "  .%-40s: %d (%s)\n",
		"sysctl_sched_tunable_scaling",
		sysctl_sched_tunable_scaling,
		sched_tunable_scaling_names[sysctl_sched_tunable_scaling]);
	SEQ_printf(m, "\n");
}

ایستا صحیح sched_debug_show(ساختار seq_file *m, تهی *v)
{
	صحیح cpu = (unsigned long)(v - 2);

	اگر (cpu != -1)
		print_cpu(m, cpu);
	وگرنه
		sched_debug_header(m);

	بازگشت 0;
}

تهی sysrq_sched_debug_show(تهی)
{
	صحیح cpu;

	sched_debug_header(پوچ);
	for_each_online_cpu(cpu) {
		/*
		 * Need to reset softlockup watchdogs on all CPUs, because
		 * another CPU might be blocked waiting for us to process
		 * an IPI or stop_machine.
		 */
		touch_nmi_watchdog();
		touch_all_softlockup_watchdogs();
		print_cpu(پوچ, cpu);
	}
}

/*
 * This iterator needs some explanation.
 * It returns 1 for the header position.
 * This means 2 is CPU 0.
 * In a hotplugged system some CPUs, including CPU 0, may be missing so we have
 * to use cpumask_* to iterate over the CPUs.
 */
ایستا تهی *sched_debug_start(ساختار seq_file *file, loff_t *offset)
{
	unsigned long n = *offset;

	اگر (n == 0)
		بازگشت (تهی *) 1;

	n--;

	اگر (n > 0)
		n = cpumask_next(n - 1, cpu_online_mask);
	وگرنه
		n = cpumask_first(cpu_online_mask);

	*offset = n + 1;

	اگر (n < nr_cpu_ids)
		بازگشت (تهی *)(unsigned long)(n + 2);

	بازگشت پوچ;
}

ایستا تهی *sched_debug_next(ساختار seq_file *file, تهی *data, loff_t *offset)
{
	(*offset)++;
	بازگشت sched_debug_start(file, offset);
}

ایستا تهی sched_debug_stop(ساختار seq_file *file, تهی *data)
{
}

ایستا ثابت ساختار seq_operations sched_debug_sops = {
	.start		= sched_debug_start,
	.next		= sched_debug_next,
	.stop		= sched_debug_stop,
	.show		= sched_debug_show,
};

#define __PS(S, F) SEQ_printf(m, "%-45s:%21Ld\n", S, (long long)(F))
#define __P(F) __PS(#F, F)
#define   P(F) __PS(#F, p->F)
#define   PM(F, M) __PS(#F, p->F & (M))
#define __PSN(S, F) SEQ_printf(m, "%-45s:%14Ld.%06ld\n", S, SPLIT_NS((long long)(F)))
#define __PN(F) __PSN(#F, F)
#define   PN(F) __PSN(#F, p->F)


#ifdef CONFIG_NUMA_BALANCING
تهی print_numa_stats(ساختار seq_file *m, صحیح گره, unsigned long tsf,
		unsigned long tpf, unsigned long gsf, unsigned long gpf)
{
	SEQ_printf(m, "numa_faults node=%d ", گره);
	SEQ_printf(m, "task_private=%lu task_shared=%lu ", tpf, tsf);
	SEQ_printf(m, "group_private=%lu group_shared=%lu\n", gpf, gsf);
}
#endif


ایستا تهی sched_show_numa(ساختار task_struct *p, ساختار seq_file *m)
{
#ifdef CONFIG_NUMA_BALANCING
	اگر (p->mm)
		P(mm->numa_scan_seq);

	P(numa_pages_migrated);
	P(numa_preferred_nid);
	P(total_numa_faults);
	SEQ_printf(m, "current_node=%d, numa_group_id=%d\n",
			task_گره(p), task_numa_group_id(p));
	show_numa_stats(p, m);
#endif /* CONFIG_NUMA_BALANCING */
}

تهی proc_sched_show_task(ساختار task_struct *p, ساختار pid_namespace *ns,
						  ساختار seq_file *m)
{
	unsigned long nr_switches;

	SEQ_printf(m, "%s (%d, #threads: %d)\n", p->comm, task_pid_nr_ns(p, ns),
						get_nr_threads(p));
	SEQ_printf(m,
		"---------------------------------------------------------"
		"----------\n");

#define P_SCHEDSTAT(F)  __PS(#F, schedstat_val(p->stats.F))
#define PN_SCHEDSTAT(F) __PSN(#F, schedstat_val(p->stats.F))

	PN(se.exec_start);
	PN(se.vruntime);
	PN(se.sum_exec_runtime);

	nr_switches = p->nvcsw + p->nivcsw;

	P(se.nr_migrations);

	اگر (schedstat_enabled()) {
		u64 avg_atom, avg_per_cpu;

		PN_SCHEDSTAT(sum_sleep_runtime);
		PN_SCHEDSTAT(sum_block_runtime);
		PN_SCHEDSTAT(wait_start);
		PN_SCHEDSTAT(sleep_start);
		PN_SCHEDSTAT(block_start);
		PN_SCHEDSTAT(sleep_max);
		PN_SCHEDSTAT(block_max);
		PN_SCHEDSTAT(exec_max);
		PN_SCHEDSTAT(slice_max);
		PN_SCHEDSTAT(wait_max);
		PN_SCHEDSTAT(wait_sum);
		P_SCHEDSTAT(wait_شمار);
		PN_SCHEDSTAT(iowait_sum);
		P_SCHEDSTAT(iowait_شمار);
		P_SCHEDSTAT(nr_failed_migrations_affine);
		P_SCHEDSTAT(nr_failed_migrations_running);
		P_SCHEDSTAT(nr_failed_migrations_hot);
		P_SCHEDSTAT(nr_forced_migrations);
		P_SCHEDSTAT(nr_migrations_cpu_non_preferred);
		P_SCHEDSTAT(nr_wakeups);
		P_SCHEDSTAT(nr_wakeups_sync);
		P_SCHEDSTAT(nr_wakeups_migrate);
		P_SCHEDSTAT(nr_wakeups_local);
		P_SCHEDSTAT(nr_wakeups_remote);
		P_SCHEDSTAT(nr_wakeups_affine);
		P_SCHEDSTAT(nr_wakeups_affine_attempts);

		avg_atom = p->se.sum_exec_runtime;
		اگر (nr_switches)
			avg_atom = div64_ul(avg_atom, nr_switches);
		وگرنه
			avg_atom = -1LL;

		avg_per_cpu = p->se.sum_exec_runtime;
		اگر (p->se.nr_migrations) {
			avg_per_cpu = div64_u64(avg_per_cpu,
						p->se.nr_migrations);
		} وگرنه {
			avg_per_cpu = -1LL;
		}

		__PN(avg_atom);
		__PN(avg_per_cpu);

#ifdef CONFIG_SCHED_CORE
		PN_SCHEDSTAT(core_forceidle_sum);
#endif
	}

	__P(nr_switches);
	__PS("nr_voluntary_switches", p->nvcsw);
	__PS("nr_involuntary_switches", p->nivcsw);

	P(se.load.weight);
	P(se.avg.load_sum);
	P(se.avg.runnable_sum);
	P(se.avg.util_sum);
	P(se.avg.load_avg);
	P(se.avg.runnable_avg);
	P(se.avg.util_avg);
	P(se.avg.last_update_time);
	PM(se.avg.util_est, ~UTIL_AVG_UNCHANGED);
#ifdef CONFIG_UCLAMP_TASK
	__PS("uclamp.min", p->uclamp_req[UCLAMP_MIN].مقدار);
	__PS("uclamp.max", p->uclamp_req[UCLAMP_MAX].مقدار);
	__PS("effective uclamp.min", uclamp_eff_مقدار(p, UCLAMP_MIN));
	__PS("effective uclamp.max", uclamp_eff_مقدار(p, UCLAMP_MAX));
#endif /* CONFIG_UCLAMP_TASK */
	P(policy);
	P(prio);
	اگر (task_has_dl_policy(p)) {
		P(dl.runtime);
		P(dl.deadline);
	} وگرنه اگر (fair_policy(p->policy)) {
		P(se.slice);
	}
#ifdef CONFIG_SCHED_CLASS_EXT
	__PS("ext.enabled", task_on_scx(p));
#endif
#undef PN_SCHEDSTAT
#undef P_SCHEDSTAT

	{
		unsigned صحیح this_cpu = raw_smp_processor_id();
		u64 t0, t1;

		t0 = cpu_clock(this_cpu);
		t1 = cpu_clock(this_cpu);
		__PS("clock-delta", t1-t0);
	}

	sched_show_numa(p, m);
}

تهی proc_sched_set_task(ساختار task_struct *p)
{
#ifdef CONFIG_SCHEDSTATS
	memset(&p->stats, 0, اندازه(p->stats));
#endif
}

تهی resched_latency_warn(صحیح cpu, u64 latency)
{
	ایستا DEFINE_RATELIMIT_STATE(latency_check_ratelimit, 60 * 60 * HZ, 1);

	اگر (likely(!__ratelimit(&latency_check_ratelimit)))
		بازگشت;

	pr_err("sched: CPU %d need_resched set for > %llu ns (%d ticks) without schedule\n",
	       cpu, latency, cpu_rq(cpu)->ticks_without_resched);
	dump_پشته();
}
