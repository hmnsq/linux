// SPDX-License-Identifier: GPL-2.0-only
/*
 * kernel/sched/debug.c
 *
 * Print the CFS rbtree and other debugging details
 *
 * Copyright(C) 2007, Red Hat, Inc., Ingo Molnar
 */
#அந்தர்பாவ <linux/debugfs.h>
#அந்தர்பாவ <linux/nmi.h>
#அந்தர்பாவ <linux/log2.h>
#அந்தர்பாவ "sched.h"

/*
 * This allows printing both to /sys/kernel/debug/sched/debug and
 * to the console
 */
#define SEQ_printf(m, x...)			\
 செய் {						\
	எனில் (m)					\
		seq_printf(m, x);		\
	இல்லையெனில்					\
		pr_cont(x);			\
 } போது (0)

/*
 * Ease the printing of nsec fields:
 */
நிலையான long long nsec_உயர்வு(unsigned long long nsec)
{
	எனில் ((long long)nsec < 0) {
		nsec = -nsec;
		do_div(nsec, 1000000);
		திரும்பு -nsec;
	}
	do_div(nsec, 1000000);

	திரும்பு nsec;
}

நிலையான unsigned long nsec_கீழ்(unsigned long long nsec)
{
	எனில் ((long long)nsec < 0)
		nsec = -nsec;

	திரும்பு do_div(nsec, 1000000);
}

#define SPLIT_NS(x) nsec_உயர்வு(x), nsec_கீழ்(x)

#define SCHED_FEAT(name, enabled)	\
	#name ,

நிலையான மாறிலி எழுத்து * மாறிலி sched_feat_names[] = {
#அந்தர்பாவ "features.h"
};

#undef SCHED_FEAT

நிலையான முழு sched_feat_show(கட்டமைப்பு seq_file *m, வெறுமை *v)
{
	முழு i;

	க்கு (i = 0; i < __SCHED_FEAT_NR; i++) {
		எனில் (!(sysctl_sched_features & (1UL << i)))
			seq_puts(m, "NO_");
		seq_printf(m, "%s ", sched_feat_names[i]);
	}
	seq_puts(m, "\n");

	திரும்பு 0;
}

#ifdef CONFIG_JUMP_LABEL

#define jump_label_சாவி_true  { .சாவி_true  = STATIC_சாவி_TRUE_INIT  }
#define jump_label_சாவி_false { .சாவி_false = STATIC_சாவி_FALSE_INIT }

#define SCHED_FEAT(name, enabled)	\
	jump_label_சாவி##enabled ,

ஒன்றிப்பு sched_feat_சாவி sched_feat_சாவிகள்[__SCHED_FEAT_NR] = {
#அந்தர்பாவ "features.h"
};

#undef SCHED_FEAT

நிலையான வெறுமை sched_feat_disable(முழு i)
{
	static_branch_disable_cpuslocked(&sched_feat_சாவிகள்[i].சாவி_true);
}

நிலையான வெறுமை sched_feat_enable(முழு i)
{
	static_branch_enable_cpuslocked(&sched_feat_சாவிகள்[i].சாவி_false);
}
#இல்லையெனில் /* !CONFIG_JUMP_LABEL: */
நிலையான வெறுமை sched_feat_disable(முழு i) { };
நிலையான வெறுமை sched_feat_enable(முழு i) { };
#endif /* !CONFIG_JUMP_LABEL */

நிலையான முழு sched_feat_set(எழுத்து *cmp)
{
	முழு i;
	முழு neg = 0;

	எனில் (strncmp(cmp, "NO_", 3) == 0) {
		neg = 1;
		cmp += 3;
	}

	i = match_string(sched_feat_names, __SCHED_FEAT_NR, cmp);
	எனில் (i < 0)
		திரும்பு i;

	எனில் (neg) {
		sysctl_sched_features &= ~(1UL << i);
		sched_feat_disable(i);
	} இல்லையெனில் {
		sysctl_sched_features |= (1UL << i);
		sched_feat_enable(i);
	}

	திரும்பு 0;
}

நிலையான ssize_t
sched_feat_write(கட்டமைப்பு file *filp, மாறிலி எழுத்து __user *ubuf,
		size_t cnt, loff_t *ppos)
{
	எழுத்து buf[64];
	எழுத்து *cmp;
	முழு ret;
	கட்டமைப்பு inode *inode;

	எனில் (cnt > 63)
		cnt = 63;

	எனில் (copy_from_user(buf, ubuf, cnt))
		திரும்பு -EFAULT;

	buf[cnt] = 0;
	cmp = strstrip(buf);

	/* Ensure the static_key remains in a consistent state */
	inode = file_inode(filp);
	cpus_read_lock();
	inode_lock(inode);
	ret = sched_feat_set(cmp);
	inode_unlock(inode);
	cpus_read_unlock();
	எனில் (ret < 0)
		திரும்பு ret;

	*ppos += cnt;

	திரும்பு cnt;
}

நிலையான முழு sched_feat_open(கட்டமைப்பு inode *inode, கட்டமைப்பு file *filp)
{
	திரும்பு single_open(filp, sched_feat_show, வெற்று);
}

நிலையான மாறிலி கட்டமைப்பு file_operations sched_feat_fops = {
	.open		= sched_feat_open,
	.லிக		= sched_feat_write,
	.பட		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

நிலையான ssize_t sched_scaling_write(கட்டமைப்பு file *filp, மாறிலி எழுத்து __user *ubuf,
				   size_t cnt, loff_t *ppos)
{
	unsigned முழு scaling;
	முழு ret;

	ret = kstrtouint_from_user(ubuf, cnt, 10, &scaling);
	எனில் (ret)
		திரும்பு ret;

	எனில் (scaling >= SCHED_TUNABLESCALING_END)
		திரும்பு -EINVAL;

	sysctl_sched_tunable_scaling = scaling;
	எனில் (sched_update_scaling())
		திரும்பு -EINVAL;

	*ppos += cnt;
	திரும்பு cnt;
}

நிலையான முழு sched_scaling_show(கட்டமைப்பு seq_file *m, வெறுமை *v)
{
	seq_printf(m, "%d\n", sysctl_sched_tunable_scaling);
	திரும்பு 0;
}

நிலையான முழு sched_scaling_open(கட்டமைப்பு inode *inode, கட்டமைப்பு file *filp)
{
	திரும்பு single_open(filp, sched_scaling_show, வெற்று);
}

நிலையான மாறிலி கட்டமைப்பு file_operations sched_scaling_fops = {
	.open		= sched_scaling_open,
	.லிக		= sched_scaling_write,
	.பட		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#ifdef CONFIG_SCHED_CACHE
நிலையான ssize_t
sched_cache_enable_write(கட்டமைப்பு file *filp, மாறிலி எழுத்து __user *ubuf,
			 size_t cnt, loff_t *ppos)
{
	bool val;
	முழு ret;

	ret = kstrtobool_from_user(ubuf, cnt, &val);
	எனில் (ret)
		திரும்பு ret;

	sysctl_sched_cache_user = val;

	sched_cache_active_set();

	*ppos += cnt;

	திரும்பு cnt;
}

நிலையான முழு sched_cache_enable_show(கட்டமைப்பு seq_file *m, வெறுமை *v)
{
	seq_printf(m, "%d\n", sysctl_sched_cache_user);
	திரும்பு 0;
}

நிலையான முழு sched_cache_enable_open(கட்டமைப்பு inode *inode,
				   கட்டமைப்பு file *filp)
{
	திரும்பு single_open(filp, sched_cache_enable_show, வெற்று);
}

நிலையான மாறிலி கட்டமைப்பு file_operations sched_cache_enable_fops = {
	.open           = sched_cache_enable_open,
	.லிக          = sched_cache_enable_write,
	.பட           = seq_read,
	.llseek         = seq_lseek,
	.release        = single_release,
};
#endif

#ifdef CONFIG_PREEMPT_DYNAMIC

நிலையான ssize_t sched_dynamic_write(கட்டமைப்பு file *filp, மாறிலி எழுத்து __user *ubuf,
				   size_t cnt, loff_t *ppos)
{
	எழுத்து buf[16];
	முழு mode;

	எனில் (cnt > 15)
		cnt = 15;

	எனில் (copy_from_user(buf, ubuf, cnt))
		திரும்பு -EFAULT;

	buf[cnt] = 0;
	mode = sched_dynamic_mode(strstrip(buf));
	எனில் (mode < 0)
		திரும்பு mode;

	sched_dynamic_update(mode);

	*ppos += cnt;

	திரும்பு cnt;
}

நிலையான முழு sched_dynamic_show(கட்டமைப்பு seq_file *m, வெறுமை *v)
{
	முழு mode = READ_ONCE(preempt_dynamic_mode);

	/* Stop at NULL terminator */
	க்கு (முழு i = 0; preempt_modes[i]; i++) {
		எனில் (mode == i)
			seq_puts(m, "(");
		seq_puts(m, preempt_modes[i]);
		எனில் (mode == i)
			seq_puts(m, ")");

		seq_puts(m, " ");
	}

	seq_puts(m, "\n");
	திரும்பு 0;
}

நிலையான முழு sched_dynamic_open(கட்டமைப்பு inode *inode, கட்டமைப்பு file *filp)
{
	திரும்பு single_open(filp, sched_dynamic_show, வெற்று);
}

நிலையான மாறிலி கட்டமைப்பு file_operations sched_dynamic_fops = {
	.open		= sched_dynamic_open,
	.லிக		= sched_dynamic_write,
	.பட		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#endif /* CONFIG_PREEMPT_DYNAMIC */

__read_mostly bool sched_debug_verbose;

நிலையான கட்டமைப்பு dentry           *sd_dentry;


நிலையான ssize_t sched_verbose_write(கட்டமைப்பு file *filp, மாறிலி எழுத்து __user *ubuf,
				  size_t cnt, loff_t *ppos)
{
	ssize_t result;
	bool orig;

	cpus_read_lock();
	sched_domains_mutex_lock();

	orig = sched_debug_verbose;
	result = debugfs_write_file_bool(filp, ubuf, cnt, ppos);

	எனில் (sched_debug_verbose && !orig)
		update_sched_domain_debugfs();
	இல்லையெனில் எனில் (!sched_debug_verbose && orig) {
		debugfs_remove(sd_dentry);
		sd_dentry = வெற்று;
	}

	sched_domains_mutex_unlock();
	cpus_read_unlock();

	திரும்பு result;
}

நிலையான மாறிலி கட்டமைப்பு file_operations sched_verbose_fops = {
	.பட =         debugfs_read_file_bool,
	.லிக =        sched_verbose_write,
	.open =         simple_open,
	.llseek =       default_llseek,
};

நிலையான மாறிலி கட்டமைப்பு seq_operations sched_debug_sops;

நிலையான முழு sched_debug_open(கட்டமைப்பு inode *inode, கட்டமைப்பு file *filp)
{
	திரும்பு seq_open(filp, &sched_debug_sops);
}

நிலையான மாறிலி கட்டமைப்பு file_operations sched_debug_fops = {
	.open		= sched_debug_open,
	.பட		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

எண்ணிக்கை dl_param {
	DL_RUNTIME = 0,
	DL_PERIOD,
};

நிலையான unsigned long dl_server_period_max = (1UL << 22) * NSEC_PER_USEC; /* ~4 seconds */
நிலையான unsigned long dl_server_period_min = (100) * NSEC_PER_USEC;     /* 100 us */

நிலையான ssize_t sched_server_write_common(கட்டமைப்பு file *filp, மாறிலி எழுத்து __user *ubuf,
					 size_t cnt, loff_t *ppos, எண்ணிக்கை dl_param param,
					 வெறுமை *server)
{
	long cpu = (long) ((கட்டமைப்பு seq_file *) filp->private_data)->தனிப்பட்ட;
	கட்டமைப்பு sched_dl_entity *dl_se = (கட்டமைப்பு sched_dl_entity *)server;
	u64 old_runtime, runtime, period;
	கட்டமைப்பு rq *rq = cpu_rq(cpu);
	முழு retval = 0;
	size_t err;
	u64 மதிப்பு;

	err = kstrtoull_from_user(ubuf, cnt, 10, &மதிப்பு);
	எனில் (err)
		திரும்பு err;

	scoped_guard (rq_lock_irqsave, rq) {
		old_runtime = runtime = dl_se->dl_runtime;
		period = dl_se->dl_period;

		தேர்வு (param) {
		நிலை DL_RUNTIME:
			எனில் (runtime == மதிப்பு)
				நிறுத்து;
			runtime = மதிப்பு;
			நிறுத்து;
		நிலை DL_PERIOD:
			எனில் (மதிப்பு == period)
				நிறுத்து;
			period = மதிப்பு;
			நிறுத்து;
		}

		எனில் (runtime > period ||
		    period > dl_server_period_max ||
		    period < dl_server_period_min) {
			திரும்பு  -EINVAL;
		}

		எனில் (!cpu_online(cpu_of(rq)))
			திரும்பு -EBUSY;

		update_rq_clock(rq);
		dl_server_stop(dl_se);
		retval = dl_server_apply_params(dl_se, runtime, period, 0);
		dl_server_start(dl_se);

		எனில் (retval < 0)
			திரும்பு retval;
	}

	எனில் (!!old_runtime ^ !!runtime) {
		pr_info("%s server %sabled on CPU %d%s.\n",
			server == &rq->fair_server ? "Fair" : "Ext",
			runtime ? "en" : "dis",
			cpu_of(rq),
			runtime ? "" : ", system may malfunction due to starvation");
	}

	*ppos += cnt;
	திரும்பு cnt;
}

நிலையான size_t sched_server_show_common(கட்டமைப்பு seq_file *m, வெறுமை *v, எண்ணிக்கை dl_param param,
				       வெறுமை *server)
{
	கட்டமைப்பு sched_dl_entity *dl_se = (கட்டமைப்பு sched_dl_entity *)server;
	u64 மதிப்பு;

	தேர்வு (param) {
	நிலை DL_RUNTIME:
		மதிப்பு = dl_se->dl_runtime;
		நிறுத்து;
	நிலை DL_PERIOD:
		மதிப்பு = dl_se->dl_period;
		நிறுத்து;
	}

	seq_printf(m, "%llu\n", மதிப்பு);
	திரும்பு 0;
}

நிலையான ssize_t
sched_fair_server_runtime_write(கட்டமைப்பு file *filp, மாறிலி எழுத்து __user *ubuf,
				size_t cnt, loff_t *ppos)
{
	long cpu = (long) ((கட்டமைப்பு seq_file *) filp->private_data)->தனிப்பட்ட;
	கட்டமைப்பு rq *rq = cpu_rq(cpu);

	திரும்பு sched_server_write_common(filp, ubuf, cnt, ppos, DL_RUNTIME,
					&rq->fair_server);
}

நிலையான முழு sched_fair_server_runtime_show(கட்டமைப்பு seq_file *m, வெறுமை *v)
{
	unsigned long cpu = (unsigned long) m->தனிப்பட்ட;
	கட்டமைப்பு rq *rq = cpu_rq(cpu);

	திரும்பு sched_server_show_common(m, v, DL_RUNTIME, &rq->fair_server);
}

நிலையான முழு sched_fair_server_runtime_open(கட்டமைப்பு inode *inode, கட்டமைப்பு file *filp)
{
	திரும்பு single_open(filp, sched_fair_server_runtime_show, inode->i_private);
}

நிலையான மாறிலி கட்டமைப்பு file_operations fair_server_runtime_fops = {
	.open		= sched_fair_server_runtime_open,
	.லிக		= sched_fair_server_runtime_write,
	.பட		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

நிலையான கட்டமைப்பு dentry *debugfs_sched;

#ifdef CONFIG_SCHED_CLASS_EXT
நிலையான ssize_t
sched_ext_server_runtime_write(கட்டமைப்பு file *filp, மாறிலி எழுத்து __user *ubuf,
			       size_t cnt, loff_t *ppos)
{
	long cpu = (long) ((கட்டமைப்பு seq_file *) filp->private_data)->தனிப்பட்ட;
	கட்டமைப்பு rq *rq = cpu_rq(cpu);

	திரும்பு sched_server_write_common(filp, ubuf, cnt, ppos, DL_RUNTIME,
					&rq->ext_server);
}

நிலையான முழு sched_ext_server_runtime_show(கட்டமைப்பு seq_file *m, வெறுமை *v)
{
	unsigned long cpu = (unsigned long) m->தனிப்பட்ட;
	கட்டமைப்பு rq *rq = cpu_rq(cpu);

	திரும்பு sched_server_show_common(m, v, DL_RUNTIME, &rq->ext_server);
}

நிலையான முழு sched_ext_server_runtime_open(கட்டமைப்பு inode *inode, கட்டமைப்பு file *filp)
{
	திரும்பு single_open(filp, sched_ext_server_runtime_show, inode->i_private);
}

நிலையான மாறிலி கட்டமைப்பு file_operations ext_server_runtime_fops = {
	.open		= sched_ext_server_runtime_open,
	.லிக		= sched_ext_server_runtime_write,
	.பட		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

நிலையான ssize_t
sched_ext_server_period_write(கட்டமைப்பு file *filp, மாறிலி எழுத்து __user *ubuf,
			      size_t cnt, loff_t *ppos)
{
	long cpu = (long) ((கட்டமைப்பு seq_file *) filp->private_data)->தனிப்பட்ட;
	கட்டமைப்பு rq *rq = cpu_rq(cpu);

	திரும்பு sched_server_write_common(filp, ubuf, cnt, ppos, DL_PERIOD,
					&rq->ext_server);
}

நிலையான முழு sched_ext_server_period_show(கட்டமைப்பு seq_file *m, வெறுமை *v)
{
	unsigned long cpu = (unsigned long) m->தனிப்பட்ட;
	கட்டமைப்பு rq *rq = cpu_rq(cpu);

	திரும்பு sched_server_show_common(m, v, DL_PERIOD, &rq->ext_server);
}

நிலையான முழு sched_ext_server_period_open(கட்டமைப்பு inode *inode, கட்டமைப்பு file *filp)
{
	திரும்பு single_open(filp, sched_ext_server_period_show, inode->i_private);
}

நிலையான மாறிலி கட்டமைப்பு file_operations ext_server_period_fops = {
	.open		= sched_ext_server_period_open,
	.லிக		= sched_ext_server_period_write,
	.பட		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

நிலையான வெறுமை debugfs_ext_server_init(வெறுமை)
{
	கட்டமைப்பு dentry *d_ext;
	unsigned long cpu;

	d_ext = debugfs_create_dir("ext_server", debugfs_sched);
	எனில் (!d_ext)
		திரும்பு;

	for_each_possible_cpu(cpu) {
		கட்டமைப்பு dentry *d_cpu;
		எழுத்து buf[32];

		snprintf(buf, அளவு(buf), "cpu%lu", cpu);
		d_cpu = debugfs_create_dir(buf, d_ext);

		debugfs_create_file("runtime", 0644, d_cpu, (வெறுமை *) cpu, &ext_server_runtime_fops);
		debugfs_create_file("period", 0644, d_cpu, (வெறுமை *) cpu, &ext_server_period_fops);
	}
}
#endif /* CONFIG_SCHED_CLASS_EXT */

நிலையான ssize_t
sched_fair_server_period_write(கட்டமைப்பு file *filp, மாறிலி எழுத்து __user *ubuf,
			       size_t cnt, loff_t *ppos)
{
	long cpu = (long) ((கட்டமைப்பு seq_file *) filp->private_data)->தனிப்பட்ட;
	கட்டமைப்பு rq *rq = cpu_rq(cpu);

	திரும்பு sched_server_write_common(filp, ubuf, cnt, ppos, DL_PERIOD,
					&rq->fair_server);
}

நிலையான முழு sched_fair_server_period_show(கட்டமைப்பு seq_file *m, வெறுமை *v)
{
	unsigned long cpu = (unsigned long) m->தனிப்பட்ட;
	கட்டமைப்பு rq *rq = cpu_rq(cpu);

	திரும்பு sched_server_show_common(m, v, DL_PERIOD, &rq->fair_server);
}

நிலையான முழு sched_fair_server_period_open(கட்டமைப்பு inode *inode, கட்டமைப்பு file *filp)
{
	திரும்பு single_open(filp, sched_fair_server_period_show, inode->i_private);
}

நிலையான மாறிலி கட்டமைப்பு file_operations fair_server_period_fops = {
	.open		= sched_fair_server_period_open,
	.லிக		= sched_fair_server_period_write,
	.பட		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

நிலையான வெறுமை debugfs_fair_server_init(வெறுமை)
{
	கட்டமைப்பு dentry *d_fair;
	unsigned long cpu;

	d_fair = debugfs_create_dir("fair_server", debugfs_sched);
	எனில் (!d_fair)
		திரும்பு;

	for_each_possible_cpu(cpu) {
		கட்டமைப்பு dentry *d_cpu;
		எழுத்து buf[32];

		snprintf(buf, அளவு(buf), "cpu%lu", cpu);
		d_cpu = debugfs_create_dir(buf, d_fair);

		debugfs_create_file("runtime", 0644, d_cpu, (வெறுமை *) cpu, &fair_server_runtime_fops);
		debugfs_create_file("period", 0644, d_cpu, (வெறுமை *) cpu, &fair_server_period_fops);
	}
}

#ifdef CONFIG_FAIR_GROUP_SCHED
நிலையான முழு cgroup_mode = 2;

/* See __sched_cgroup_mode_update(). */
நிலையான மாறிலி எழுத்து *cgroup_mode_str[] = {
	"up",
	"smp",
	"concur",
	"max",
	"tasks",
};

நிலையான முழு sched_cgroup_mode(மாறிலி எழுத்து *str)
{
	க்கு (முழு i = 0; i < ARRAY_SIZE(cgroup_mode_str); i++) {
		எனில் (!strcmp(str, cgroup_mode_str[i]))
			திரும்பு i;
	}
	திரும்பு -EINVAL;
}

நிலையான ssize_t sched_cgroup_write(கட்டமைப்பு file *filp, மாறிலி எழுத்து __user *ubuf,
				   size_t cnt, loff_t *ppos)
{
	எழுத்து buf[16];
	முழு mode;

	எனில் (cnt > 15)
		cnt = 15;

	எனில் (copy_from_user(buf, ubuf, cnt))
		திரும்பு -EFAULT;

	buf[cnt] = 0;
	mode = sched_cgroup_mode(strstrip(buf));
	எனில் (mode < 0)
		திரும்பு mode;

	__sched_cgroup_mode_update(mode);
	WRITE_ONCE(cgroup_mode, mode);

	*ppos += cnt;
	திரும்பு cnt;
}

நிலையான முழு sched_cgroup_show(கட்டமைப்பு seq_file *m, வெறுமை *v)
{
	முழு mode = READ_ONCE(cgroup_mode);

	க்கு (முழு i = 0; i < ARRAY_SIZE(cgroup_mode_str); i++) {
		எனில் (mode == i)
			seq_puts(m, "(");
		seq_puts(m, cgroup_mode_str[i]);
		எனில் (mode == i)
			seq_puts(m, ")");

		seq_puts(m, " ");
	}
	seq_puts(m, "\n");
	திரும்பு 0;
}

நிலையான முழு sched_cgroup_open(கட்டமைப்பு inode *inode, கட்டமைப்பு file *filp)
{
	திரும்பு single_open(filp, sched_cgroup_show, வெற்று);
}

நிலையான மாறிலி கட்டமைப்பு file_operations sched_cgroup_fops = {
	.open		= sched_cgroup_open,
	.லிக		= sched_cgroup_write,
	.பட		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};
#endif

நிலையான __init முழு sched_init_debug(வெறுமை)
{
	கட்டமைப்பு dentry __maybe_unused *numa, *llc;

	debugfs_sched = debugfs_create_dir("sched", வெற்று);

	debugfs_create_file("features", 0644, debugfs_sched, வெற்று, &sched_feat_fops);
	debugfs_create_file_unsafe("verbose", 0644, debugfs_sched, &sched_debug_verbose, &sched_verbose_fops);
#ifdef CONFIG_PREEMPT_DYNAMIC
	debugfs_create_file("preempt", 0644, debugfs_sched, வெற்று, &sched_dynamic_fops);
#endif

	debugfs_create_u32("base_slice_ns", 0644, debugfs_sched, &sysctl_sched_base_slice);

	debugfs_create_u32("latency_warn_ms", 0644, debugfs_sched, &sysctl_resched_latency_warn_ms);
	debugfs_create_u32("latency_warn_once", 0644, debugfs_sched, &sysctl_resched_latency_warn_once);

	debugfs_create_file("tunable_scaling", 0644, debugfs_sched, வெற்று, &sched_scaling_fops);
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
	debugfs_create_file("enabled", 0644, llc, வெற்று,
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

	debugfs_create_file("debug", 0444, debugfs_sched, வெற்று, &sched_debug_fops);

#ifdef CONFIG_FAIR_GROUP_SCHED
	debugfs_create_file("cgroup_mode", 0644, debugfs_sched, வெற்று, &sched_cgroup_fops);
#endif

	debugfs_fair_server_init();
#ifdef CONFIG_SCHED_CLASS_EXT
	debugfs_ext_server_init();
#endif

	திரும்பு 0;
}
late_initcall(sched_init_debug);

நிலையான cpumask_var_t		sd_sysctl_cpus;

நிலையான முழு sd_flags_show(கட்டமைப்பு seq_file *m, வெறுமை *v)
{
	unsigned long flags = *(unsigned முழு *)m->தனிப்பட்ட;
	முழு idx;

	for_each_set_bit(idx, &flags, __SD_FLAG_CNT) {
		seq_puts(m, sd_flag_debug[idx].name);
		seq_puts(m, " ");
	}
	seq_puts(m, "\n");

	திரும்பு 0;
}

நிலையான முழு sd_flags_open(கட்டமைப்பு inode *inode, கட்டமைப்பு file *file)
{
	திரும்பு single_open(file, sd_flags_show, inode->i_private);
}

நிலையான மாறிலி கட்டமைப்பு file_operations sd_flags_fops = {
	.open		= sd_flags_open,
	.பட		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

நிலையான வெறுமை register_sd(கட்டமைப்பு sched_domain *sd, கட்டமைப்பு dentry *parent)
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

	எனில் (sd->flags & SD_ASYM_PACKING)
		debugfs_create_u32("group_asym_prefer_cpu", 0444, parent,
				   (u32 *)&sd->groups->asym_prefer_cpu);
}

வெறுமை update_sched_domain_debugfs(வெறுமை)
{
	முழு cpu, i;

	/*
	 * This can unfortunately be invoked before sched_debug_init() creates
	 * the debug directory. Don't touch sd_sysctl_cpus until then.
	 */
	எனில் (!debugfs_sched)
		திரும்பு;

	எனில் (!sched_debug_verbose)
		திரும்பு;

	எனில் (!cpumask_available(sd_sysctl_cpus)) {
		எனில் (!alloc_cpumask_var(&sd_sysctl_cpus, GFP_KERNEL))
			திரும்பு;
		cpumask_copy(sd_sysctl_cpus, cpu_possible_mask);
	}

	எனில் (!sd_dentry) {
		sd_dentry = debugfs_create_dir("domains", debugfs_sched);

		/* rebuild sd_sysctl_cpus if empty since it gets cleared below */
		எனில் (cpumask_empty(sd_sysctl_cpus))
			cpumask_copy(sd_sysctl_cpus, cpu_online_mask);
	}

	for_each_cpu(cpu, sd_sysctl_cpus) {
		கட்டமைப்பு sched_domain *sd;
		கட்டமைப்பு dentry *d_cpu;
		எழுத்து buf[32];

		snprintf(buf, அளவு(buf), "cpu%d", cpu);
		debugfs_lookup_and_remove(buf, sd_dentry);
		d_cpu = debugfs_create_dir(buf, sd_dentry);

		i = 0;
		for_each_domain(cpu, sd) {
			கட்டமைப்பு dentry *d_sd;

			snprintf(buf, அளவு(buf), "domain%d", i);
			d_sd = debugfs_create_dir(buf, d_cpu);

			register_sd(sd, d_sd);
			i++;
		}

		__cpumask_clear_cpu(cpu, sd_sysctl_cpus);
	}
}

வெறுமை dirty_sched_domain_sysctl(முழு cpu)
{
	எனில் (cpumask_available(sd_sysctl_cpus))
		__cpumask_set_cpu(cpu, sd_sysctl_cpus);
}

#ifdef CONFIG_FAIR_GROUP_SCHED
நிலையான வெறுமை print_cfs_group_stats(கட்டமைப்பு seq_file *m, முழு cpu, கட்டமைப்பு task_group *tg)
{
	கட்டமைப்பு sched_entity *se = tg_se(tg, cpu);

#define P(F)		SEQ_printf(m, "  .%-30s: %lld\n",	#F, (long long)F)
#define P_SCHEDSTAT(F)	SEQ_printf(m, "  .%-30s: %lld\n",	\
		#F, (long long)schedstat_val(stats->F))
#define PN(F)		SEQ_printf(m, "  .%-30s: %lld.%06ld\n", #F, SPLIT_NS((long long)F))
#define PN_SCHEDSTAT(F)	SEQ_printf(m, "  .%-30s: %lld.%06ld\n", \
		#F, SPLIT_NS((long long)schedstat_val(stats->F)))

	எனில் (!se)
		திரும்பு;

	PN(se->exec_start);
	PN(se->vruntime);
	PN(se->sum_exec_runtime);

	எனில் (schedstat_enabled()) {
		கட்டமைப்பு sched_statistics *stats;
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
		P_SCHEDSTAT(wait_மொத்தம்);
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
நிலையான DEFINE_SPINLOCK(sched_debug_lock);
நிலையான எழுத்து group_path[PATH_MAX];

நிலையான வெறுமை task_group_path(கட்டமைப்பு task_group *tg, எழுத்து *path, முழு plen)
{
	எனில் (autogroup_path(tg, path, plen))
		திரும்பு;

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
	எனில் (spin_trylock(&sched_debug_lock)) {				\
		task_group_path(tg, group_path, அளவு(group_path));	\
		SEQ_printf(m, fmt, group_path);				\
		spin_unlock(&sched_debug_lock);				\
	} இல்லையெனில் {							\
		எழுத்து buf[128];						\
		எழுத்து *bufend = buf + அளவு(buf) - 3;			\
		task_group_path(tg, buf, bufend - buf);			\
		strcpy(bufend - 1, "...");				\
		SEQ_printf(m, fmt, buf);				\
	}								\
}
#endif

நிலையான வெறுமை
print_task(கட்டமைப்பு seq_file *m, கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	எனில் (task_current(rq, p))
		SEQ_printf(m, ">R");
	இல்லையெனில்
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
	SEQ_printf(m, "   %d      %d", task_முனை(p), task_numa_group_id(p));
#endif
#ifdef CONFIG_CGROUP_SCHED
	SEQ_printf_task_group_path(m, task_group(p), "        %s")
#endif

	SEQ_printf(m, "\n");
}

நிலையான வெறுமை print_rq(கட்டமைப்பு seq_file *m, கட்டமைப்பு rq *rq, முழு rq_cpu)
{
	கட்டமைப்பு task_struct *g, *p;

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
		எனில் (task_cpu(p) != rq_cpu)
			தொடர்;

		print_task(m, rq, p);
	}
	rcu_read_unlock();
}

வெறுமை print_cfs_rq(கட்டமைப்பு seq_file *m, முழு cpu, கட்டமைப்பு cfs_rq *cfs_rq)
{
	s64 இடது_vruntime = -1, வலது_vruntime = -1, இடது_deadline = -1, spread;
	s64 zero_vruntime = -1, sum_w_vruntime = -1;
	u64 avruntime;
	கட்டமைப்பு sched_entity *last, *first, *வேர்;
	கட்டமைப்பு rq *rq = cpu_rq(cpu);
	unsigned முழு sum_shift;
	unsigned long flags;
	u64 sum_weight;

#ifdef CONFIG_FAIR_GROUP_SCHED
	SEQ_printf(m, "\n");
	SEQ_printf_task_group_path(m, cfs_rq->tg, "cfs_rq[%d]:%s\n", cpu);
#இல்லையெனில்
	SEQ_printf(m, "\n");
	SEQ_printf(m, "cfs_rq[%d]:\n", cpu);
#endif

	raw_spin_rq_lock_irqsave(rq, flags);
	வேர் = __pick_root_entity(cfs_rq);
	எனில் (வேர்)
		இடது_vruntime = வேர்->min_vruntime;
	first = __pick_first_entity(cfs_rq);
	எனில் (first)
		இடது_deadline = first->deadline;
	last = __pick_last_entity(cfs_rq);
	எனில் (last)
		வலது_vruntime = last->vruntime;
	zero_vruntime = cfs_rq->zero_vruntime;
	sum_w_vruntime = cfs_rq->sum_w_vruntime;
	sum_weight = cfs_rq->sum_weight;
	sum_shift = cfs_rq->sum_shift;
	avruntime = avg_vruntime(cfs_rq);
	raw_spin_rq_unlock_irqrestore(rq, flags);

	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "left_deadline",
			SPLIT_NS(இடது_deadline));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "left_vruntime",
			SPLIT_NS(இடது_vruntime));
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
			SPLIT_NS(வலது_vruntime));
	spread = வலது_vruntime - இடது_vruntime;
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
			cfs_rq->throttle_மொத்தம்);
#endif

#ifdef CONFIG_FAIR_GROUP_SCHED
	print_cfs_group_stats(m, cpu, cfs_rq->tg);
#endif
}

வெறுமை print_rt_rq(கட்டமைப்பு seq_file *m, முழு cpu, கட்டமைப்பு rt_rq *rt_rq)
{
#ifdef CONFIG_RT_GROUP_SCHED
	SEQ_printf(m, "\n");
	SEQ_printf_task_group_path(m, rt_rq->tg, "rt_rq[%d]:%s\n", cpu);
#இல்லையெனில்
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

வெறுமை print_dl_rq(கட்டமைப்பு seq_file *m, முழு cpu, கட்டமைப்பு dl_rq *dl_rq)
{
	கட்டமைப்பு dl_bw *dl_bw;

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

நிலையான வெறுமை print_cpu(கட்டமைப்பு seq_file *m, முழு cpu)
{
	கட்டமைப்பு rq *rq = cpu_rq(cpu);

#ifdef CONFIG_X86
	{
		unsigned முழு freq = cpu_khz ? : 1;

		SEQ_printf(m, "cpu#%d, %u.%03u MHz\n",
			   cpu, freq / 1000, (freq % 1000));
	}
#இல்லையெனில் /* !CONFIG_X86: */
	SEQ_printf(m, "cpu#%d\n", cpu);
#endif /* !CONFIG_X86 */

#define P(x)								\
செய் {									\
	எனில் (அளவு(rq->x) == 4)						\
		SEQ_printf(m, "  .%-30s: %d\n", #x, (முழு)(rq->x));	\
	இல்லையெனில்								\
		SEQ_printf(m, "  .%-30s: %Ld\n", #x, (long long)(rq->x));\
} போது (0)

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
	எனில் (schedstat_enabled()) {
		P(yld_மொத்தம்);
		P(sched_மொத்தம்);
		P(sched_goidle);
		P(ttwu_மொத்தம்);
		P(ttwu_local);
	}
#undef P

	print_cfs_stats(m, cpu);
	print_rt_stats(m, cpu);
	print_dl_stats(m, cpu);

	print_rq(m, rq, cpu);
	SEQ_printf(m, "\n");
}

நிலையான மாறிலி எழுத்து *sched_tunable_scaling_names[] = {
	"none",
	"logarithmic",
	"linear"
};

நிலையான வெறுமை sched_debug_header(கட்டமைப்பு seq_file *m)
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
		(முழு)strcspn(init_utsname()->version, " "),
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

நிலையான முழு sched_debug_show(கட்டமைப்பு seq_file *m, வெறுமை *v)
{
	முழு cpu = (unsigned long)(v - 2);

	எனில் (cpu != -1)
		print_cpu(m, cpu);
	இல்லையெனில்
		sched_debug_header(m);

	திரும்பு 0;
}

வெறுமை sysrq_sched_debug_show(வெறுமை)
{
	முழு cpu;

	sched_debug_header(வெற்று);
	for_each_online_cpu(cpu) {
		/*
		 * Need to reset softlockup watchdogs on all CPUs, because
		 * another CPU might be blocked waiting for us to process
		 * an IPI or stop_machine.
		 */
		touch_nmi_watchdog();
		touch_all_softlockup_watchdogs();
		print_cpu(வெற்று, cpu);
	}
}

/*
 * This iterator needs some explanation.
 * It returns 1 for the header position.
 * This means 2 is CPU 0.
 * In a hotplugged system some CPUs, including CPU 0, may be missing so we have
 * to use cpumask_* to iterate over the CPUs.
 */
நிலையான வெறுமை *sched_debug_start(கட்டமைப்பு seq_file *file, loff_t *offset)
{
	unsigned long n = *offset;

	எனில் (n == 0)
		திரும்பு (வெறுமை *) 1;

	n--;

	எனில் (n > 0)
		n = cpumask_next(n - 1, cpu_online_mask);
	இல்லையெனில்
		n = cpumask_first(cpu_online_mask);

	*offset = n + 1;

	எனில் (n < nr_cpu_ids)
		திரும்பு (வெறுமை *)(unsigned long)(n + 2);

	திரும்பு வெற்று;
}

நிலையான வெறுமை *sched_debug_next(கட்டமைப்பு seq_file *file, வெறுமை *data, loff_t *offset)
{
	(*offset)++;
	திரும்பு sched_debug_start(file, offset);
}

நிலையான வெறுமை sched_debug_stop(கட்டமைப்பு seq_file *file, வெறுமை *data)
{
}

நிலையான மாறிலி கட்டமைப்பு seq_operations sched_debug_sops = {
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
வெறுமை print_numa_stats(கட்டமைப்பு seq_file *m, முழு முனை, unsigned long tsf,
		unsigned long tpf, unsigned long gsf, unsigned long gpf)
{
	SEQ_printf(m, "numa_faults node=%d ", முனை);
	SEQ_printf(m, "task_private=%lu task_shared=%lu ", tpf, tsf);
	SEQ_printf(m, "group_private=%lu group_shared=%lu\n", gpf, gsf);
}
#endif


நிலையான வெறுமை sched_show_numa(கட்டமைப்பு task_struct *p, கட்டமைப்பு seq_file *m)
{
#ifdef CONFIG_NUMA_BALANCING
	எனில் (p->mm)
		P(mm->numa_scan_seq);

	P(numa_pages_migrated);
	P(numa_preferred_nid);
	P(total_numa_faults);
	SEQ_printf(m, "current_node=%d, numa_group_id=%d\n",
			task_முனை(p), task_numa_group_id(p));
	show_numa_stats(p, m);
#endif /* CONFIG_NUMA_BALANCING */
}

வெறுமை proc_sched_show_task(கட்டமைப்பு task_struct *p, கட்டமைப்பு pid_namespace *ns,
						  கட்டமைப்பு seq_file *m)
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

	எனில் (schedstat_enabled()) {
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
		P_SCHEDSTAT(wait_மொத்தம்);
		PN_SCHEDSTAT(iowait_sum);
		P_SCHEDSTAT(iowait_மொத்தம்);
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
		எனில் (nr_switches)
			avg_atom = div64_ul(avg_atom, nr_switches);
		இல்லையெனில்
			avg_atom = -1LL;

		avg_per_cpu = p->se.sum_exec_runtime;
		எனில் (p->se.nr_migrations) {
			avg_per_cpu = div64_u64(avg_per_cpu,
						p->se.nr_migrations);
		} இல்லையெனில் {
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
	__PS("uclamp.min", p->uclamp_req[UCLAMP_MIN].மதிப்பு);
	__PS("uclamp.max", p->uclamp_req[UCLAMP_MAX].மதிப்பு);
	__PS("effective uclamp.min", uclamp_eff_மதிப்பு(p, UCLAMP_MIN));
	__PS("effective uclamp.max", uclamp_eff_மதிப்பு(p, UCLAMP_MAX));
#endif /* CONFIG_UCLAMP_TASK */
	P(policy);
	P(prio);
	எனில் (task_has_dl_policy(p)) {
		P(dl.runtime);
		P(dl.deadline);
	} இல்லையெனில் எனில் (fair_policy(p->policy)) {
		P(se.slice);
	}
#ifdef CONFIG_SCHED_CLASS_EXT
	__PS("ext.enabled", task_on_scx(p));
#endif
#undef PN_SCHEDSTAT
#undef P_SCHEDSTAT

	{
		unsigned முழு this_cpu = raw_smp_processor_id();
		u64 t0, t1;

		t0 = cpu_clock(this_cpu);
		t1 = cpu_clock(this_cpu);
		__PS("clock-delta", t1-t0);
	}

	sched_show_numa(p, m);
}

வெறுமை proc_sched_set_task(கட்டமைப்பு task_struct *p)
{
#ifdef CONFIG_SCHEDSTATS
	memset(&p->stats, 0, அளவு(p->stats));
#endif
}

வெறுமை resched_latency_warn(முழு cpu, u64 latency)
{
	நிலையான DEFINE_RATELIMIT_STATE(latency_check_ratelimit, 60 * 60 * HZ, 1);

	எனில் (likely(!__ratelimit(&latency_check_ratelimit)))
		திரும்பு;

	pr_err("sched: CPU %d need_resched set for > %llu ns (%d ticks) without schedule\n",
	       cpu, latency, cpu_rq(cpu)->ticks_without_resched);
	dump_அடுக்கு();
}
