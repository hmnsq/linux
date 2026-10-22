// SPDX-License-Identifier: GPL-2.0-only
/*
 * kernel/sched/debug.c
 *
 * Print the CFS rbtree and other debugging details
 *
 * Copyright(C) 2007, Red Hat, Inc., Ingo Molnar
 */
#ಅನ್ತರ್ಭಾವ <linux/debugfs.h>
#ಅನ್ತರ್ಭಾವ <linux/nmi.h>
#ಅನ್ತರ್ಭಾವ <linux/log2.h>
#ಅನ್ತರ್ಭಾವ "sched.h"

/*
 * This allows printing both to /sys/kernel/debug/sched/debug and
 * to the console
 */
#define SEQ_printf(m, x...)			\
 ಕುರು {						\
	ಯದಿ (m)					\
		seq_printf(m, x);		\
	ಅನ್ಯಥಾ					\
		pr_cont(x);			\
 } ಯಾವತ್ (0)

/*
 * Ease the printing of nsec fields:
 */
ಸ್ಥಿರ long long nsec_ಮೇಲಿನ(unsigned long long nsec)
{
	ಯದಿ ((long long)nsec < 0) {
		nsec = -nsec;
		do_div(nsec, 1000000);
		ಪ್ರತ್ಯಾಗಮ -nsec;
	}
	do_div(nsec, 1000000);

	ಪ್ರತ್ಯಾಗಮ nsec;
}

ಸ್ಥಿರ unsigned long nsec_ಕೆಳಗಿನ(unsigned long long nsec)
{
	ಯದಿ ((long long)nsec < 0)
		nsec = -nsec;

	ಪ್ರತ್ಯಾಗಮ do_div(nsec, 1000000);
}

#define SPLIT_NS(x) nsec_ಮೇಲಿನ(x), nsec_ಕೆಳಗಿನ(x)

#define SCHED_FEAT(name, enabled)	\
	#name ,

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ sched_feat_names[] = {
#ಅನ್ತರ್ಭಾವ "features.h"
};

#undef SCHED_FEAT

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_feat_show(ಸಂರಚನಾ seq_file *m, ಶೂನ್ಯ *v)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = 0; i < __SCHED_FEAT_NR; i++) {
		ಯದಿ (!(sysctl_sched_features & (1UL << i)))
			seq_puts(m, "NO_");
		seq_printf(m, "%s ", sched_feat_names[i]);
	}
	seq_puts(m, "\n");

	ಪ್ರತ್ಯಾಗಮ 0;
}

#ifdef CONFIG_JUMP_LABEL

#define jump_label_ಕೀಲಿ_true  { .ಕೀಲಿ_true  = STATIC_ಕೀಲಿ_TRUE_INIT  }
#define jump_label_ಕೀಲಿ_false { .ಕೀಲಿ_false = STATIC_ಕೀಲಿ_FALSE_INIT }

#define SCHED_FEAT(name, enabled)	\
	jump_label_ಕೀಲಿ##enabled ,

ಸಙ್ಘ sched_feat_ಕೀಲಿ sched_feat_ಕೀಲಿಗಳು[__SCHED_FEAT_NR] = {
#ಅನ್ತರ್ಭಾವ "features.h"
};

#undef SCHED_FEAT

ಸ್ಥಿರ ಶೂನ್ಯ sched_feat_disable(ಪೂರ್ಣಾಙ್ಕ i)
{
	static_branch_disable_cpuslocked(&sched_feat_ಕೀಲಿಗಳು[i].ಕೀಲಿ_true);
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_feat_enable(ಪೂರ್ಣಾಙ್ಕ i)
{
	static_branch_enable_cpuslocked(&sched_feat_ಕೀಲಿಗಳು[i].ಕೀಲಿ_false);
}
#ಅನ್ಯಥಾ /* !CONFIG_JUMP_LABEL: */
ಸ್ಥಿರ ಶೂನ್ಯ sched_feat_disable(ಪೂರ್ಣಾಙ್ಕ i) { };
ಸ್ಥಿರ ಶೂನ್ಯ sched_feat_enable(ಪೂರ್ಣಾಙ್ಕ i) { };
#endif /* !CONFIG_JUMP_LABEL */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_feat_set(ವರ್ಣ *cmp)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಪೂರ್ಣಾಙ್ಕ neg = 0;

	ಯದಿ (strncmp(cmp, "NO_", 3) == 0) {
		neg = 1;
		cmp += 3;
	}

	i = match_string(sched_feat_names, __SCHED_FEAT_NR, cmp);
	ಯದಿ (i < 0)
		ಪ್ರತ್ಯಾಗಮ i;

	ಯದಿ (neg) {
		sysctl_sched_features &= ~(1UL << i);
		sched_feat_disable(i);
	} ಅನ್ಯಥಾ {
		sysctl_sched_features |= (1UL << i);
		sched_feat_enable(i);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ssize_t
sched_feat_write(ಸಂರಚನಾ file *filp, ಧ್ರುವ ವರ್ಣ __user *ubuf,
		size_t cnt, loff_t *ppos)
{
	ವರ್ಣ buf[64];
	ವರ್ಣ *cmp;
	ಪೂರ್ಣಾಙ್ಕ ret;
	ಸಂರಚನಾ inode *inode;

	ಯದಿ (cnt > 63)
		cnt = 63;

	ಯದಿ (copy_from_user(buf, ubuf, cnt))
		ಪ್ರತ್ಯಾಗಮ -EFAULT;

	buf[cnt] = 0;
	cmp = strstrip(buf);

	/* Ensure the static_key remains in a consistent state */
	inode = file_inode(filp);
	cpus_read_lock();
	inode_lock(inode);
	ret = sched_feat_set(cmp);
	inode_unlock(inode);
	cpus_read_unlock();
	ಯದಿ (ret < 0)
		ಪ್ರತ್ಯಾಗಮ ret;

	*ppos += cnt;

	ಪ್ರತ್ಯಾಗಮ cnt;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_feat_open(ಸಂರಚನಾ inode *inode, ಸಂರಚನಾ file *filp)
{
	ಪ್ರತ್ಯಾಗಮ single_open(filp, sched_feat_show, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ file_operations sched_feat_fops = {
	.open		= sched_feat_open,
	.ಲಿಖ		= sched_feat_write,
	.ಪಠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ಸ್ಥಿರ ssize_t sched_scaling_write(ಸಂರಚನಾ file *filp, ಧ್ರುವ ವರ್ಣ __user *ubuf,
				   size_t cnt, loff_t *ppos)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ scaling;
	ಪೂರ್ಣಾಙ್ಕ ret;

	ret = kstrtouint_from_user(ubuf, cnt, 10, &scaling);
	ಯದಿ (ret)
		ಪ್ರತ್ಯಾಗಮ ret;

	ಯದಿ (scaling >= SCHED_TUNABLESCALING_END)
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	sysctl_sched_tunable_scaling = scaling;
	ಯದಿ (sched_update_scaling())
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	*ppos += cnt;
	ಪ್ರತ್ಯಾಗಮ cnt;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_scaling_show(ಸಂರಚನಾ seq_file *m, ಶೂನ್ಯ *v)
{
	seq_printf(m, "%d\n", sysctl_sched_tunable_scaling);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_scaling_open(ಸಂರಚನಾ inode *inode, ಸಂರಚನಾ file *filp)
{
	ಪ್ರತ್ಯಾಗಮ single_open(filp, sched_scaling_show, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ file_operations sched_scaling_fops = {
	.open		= sched_scaling_open,
	.ಲಿಖ		= sched_scaling_write,
	.ಪಠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#ifdef CONFIG_SCHED_CACHE
ಸ್ಥಿರ ssize_t
sched_cache_enable_write(ಸಂರಚನಾ file *filp, ಧ್ರುವ ವರ್ಣ __user *ubuf,
			 size_t cnt, loff_t *ppos)
{
	bool val;
	ಪೂರ್ಣಾಙ್ಕ ret;

	ret = kstrtobool_from_user(ubuf, cnt, &val);
	ಯದಿ (ret)
		ಪ್ರತ್ಯಾಗಮ ret;

	sysctl_sched_cache_user = val;

	sched_cache_active_set();

	*ppos += cnt;

	ಪ್ರತ್ಯಾಗಮ cnt;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_cache_enable_show(ಸಂರಚನಾ seq_file *m, ಶೂನ್ಯ *v)
{
	seq_printf(m, "%d\n", sysctl_sched_cache_user);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_cache_enable_open(ಸಂರಚನಾ inode *inode,
				   ಸಂರಚನಾ file *filp)
{
	ಪ್ರತ್ಯಾಗಮ single_open(filp, sched_cache_enable_show, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ file_operations sched_cache_enable_fops = {
	.open           = sched_cache_enable_open,
	.ಲಿಖ          = sched_cache_enable_write,
	.ಪಠ           = seq_read,
	.llseek         = seq_lseek,
	.release        = single_release,
};
#endif

#ifdef CONFIG_PREEMPT_DYNAMIC

ಸ್ಥಿರ ssize_t sched_dynamic_write(ಸಂರಚನಾ file *filp, ಧ್ರುವ ವರ್ಣ __user *ubuf,
				   size_t cnt, loff_t *ppos)
{
	ವರ್ಣ buf[16];
	ಪೂರ್ಣಾಙ್ಕ mode;

	ಯದಿ (cnt > 15)
		cnt = 15;

	ಯದಿ (copy_from_user(buf, ubuf, cnt))
		ಪ್ರತ್ಯಾಗಮ -EFAULT;

	buf[cnt] = 0;
	mode = sched_dynamic_mode(strstrip(buf));
	ಯದಿ (mode < 0)
		ಪ್ರತ್ಯಾಗಮ mode;

	sched_dynamic_update(mode);

	*ppos += cnt;

	ಪ್ರತ್ಯಾಗಮ cnt;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_dynamic_show(ಸಂರಚನಾ seq_file *m, ಶೂನ್ಯ *v)
{
	ಪೂರ್ಣಾಙ್ಕ mode = READ_ONCE(preempt_dynamic_mode);

	/* Stop at NULL terminator */
	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; preempt_modes[i]; i++) {
		ಯದಿ (mode == i)
			seq_puts(m, "(");
		seq_puts(m, preempt_modes[i]);
		ಯದಿ (mode == i)
			seq_puts(m, ")");

		seq_puts(m, " ");
	}

	seq_puts(m, "\n");
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_dynamic_open(ಸಂರಚನಾ inode *inode, ಸಂರಚನಾ file *filp)
{
	ಪ್ರತ್ಯಾಗಮ single_open(filp, sched_dynamic_show, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ file_operations sched_dynamic_fops = {
	.open		= sched_dynamic_open,
	.ಲಿಖ		= sched_dynamic_write,
	.ಪಠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#endif /* CONFIG_PREEMPT_DYNAMIC */

__read_mostly bool sched_debug_verbose;

ಸ್ಥಿರ ಸಂರಚನಾ dentry           *sd_dentry;


ಸ್ಥಿರ ssize_t sched_verbose_write(ಸಂರಚನಾ file *filp, ಧ್ರುವ ವರ್ಣ __user *ubuf,
				  size_t cnt, loff_t *ppos)
{
	ssize_t result;
	bool orig;

	cpus_read_lock();
	sched_domains_mutex_lock();

	orig = sched_debug_verbose;
	result = debugfs_write_file_bool(filp, ubuf, cnt, ppos);

	ಯದಿ (sched_debug_verbose && !orig)
		update_sched_domain_debugfs();
	ಅನ್ಯಥಾ ಯದಿ (!sched_debug_verbose && orig) {
		debugfs_remove(sd_dentry);
		sd_dentry = ರಿಕ್ತ;
	}

	sched_domains_mutex_unlock();
	cpus_read_unlock();

	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ file_operations sched_verbose_fops = {
	.ಪಠ =         debugfs_read_file_bool,
	.ಲಿಖ =        sched_verbose_write,
	.open =         simple_open,
	.llseek =       default_llseek,
};

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ seq_operations sched_debug_sops;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_debug_open(ಸಂರಚನಾ inode *inode, ಸಂರಚನಾ file *filp)
{
	ಪ್ರತ್ಯಾಗಮ seq_open(filp, &sched_debug_sops);
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ file_operations sched_debug_fops = {
	.open		= sched_debug_open,
	.ಪಠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

ಗಣನಾ dl_param {
	DL_RUNTIME = 0,
	DL_PERIOD,
};

ಸ್ಥಿರ unsigned long dl_server_period_max = (1UL << 22) * NSEC_PER_USEC; /* ~4 seconds */
ಸ್ಥಿರ unsigned long dl_server_period_min = (100) * NSEC_PER_USEC;     /* 100 us */

ಸ್ಥಿರ ssize_t sched_server_write_common(ಸಂರಚನಾ file *filp, ಧ್ರುವ ವರ್ಣ __user *ubuf,
					 size_t cnt, loff_t *ppos, ಗಣನಾ dl_param param,
					 ಶೂನ್ಯ *server)
{
	long cpu = (long) ((ಸಂರಚನಾ seq_file *) filp->private_data)->ಗುಪ್ತ;
	ಸಂರಚನಾ sched_dl_entity *dl_se = (ಸಂರಚನಾ sched_dl_entity *)server;
	u64 old_runtime, runtime, period;
	ಸಂರಚನಾ rq *rq = cpu_rq(cpu);
	ಪೂರ್ಣಾಙ್ಕ retval = 0;
	size_t err;
	u64 ಮೌಲ್ಯ;

	err = kstrtoull_from_user(ubuf, cnt, 10, &ಮೌಲ್ಯ);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ err;

	scoped_guard (rq_lock_irqsave, rq) {
		old_runtime = runtime = dl_se->dl_runtime;
		period = dl_se->dl_period;

		ವಿಕಲ್ಪ (param) {
		ಪಕ್ಷ DL_RUNTIME:
			ಯದಿ (runtime == ಮೌಲ್ಯ)
				ಭಙ್ಗ;
			runtime = ಮೌಲ್ಯ;
			ಭಙ್ಗ;
		ಪಕ್ಷ DL_PERIOD:
			ಯದಿ (ಮೌಲ್ಯ == period)
				ಭಙ್ಗ;
			period = ಮೌಲ್ಯ;
			ಭಙ್ಗ;
		}

		ಯದಿ (runtime > period ||
		    period > dl_server_period_max ||
		    period < dl_server_period_min) {
			ಪ್ರತ್ಯಾಗಮ  -EINVAL;
		}

		ಯದಿ (!cpu_online(cpu_of(rq)))
			ಪ್ರತ್ಯಾಗಮ -EBUSY;

		update_rq_clock(rq);
		dl_server_stop(dl_se);
		retval = dl_server_apply_params(dl_se, runtime, period, 0);
		dl_server_start(dl_se);

		ಯದಿ (retval < 0)
			ಪ್ರತ್ಯಾಗಮ retval;
	}

	ಯದಿ (!!old_runtime ^ !!runtime) {
		pr_info("%s server %sabled on CPU %d%s.\n",
			server == &rq->fair_server ? "Fair" : "Ext",
			runtime ? "en" : "dis",
			cpu_of(rq),
			runtime ? "" : ", system may malfunction due to starvation");
	}

	*ppos += cnt;
	ಪ್ರತ್ಯಾಗಮ cnt;
}

ಸ್ಥಿರ size_t sched_server_show_common(ಸಂರಚನಾ seq_file *m, ಶೂನ್ಯ *v, ಗಣನಾ dl_param param,
				       ಶೂನ್ಯ *server)
{
	ಸಂರಚನಾ sched_dl_entity *dl_se = (ಸಂರಚನಾ sched_dl_entity *)server;
	u64 ಮೌಲ್ಯ;

	ವಿಕಲ್ಪ (param) {
	ಪಕ್ಷ DL_RUNTIME:
		ಮೌಲ್ಯ = dl_se->dl_runtime;
		ಭಙ್ಗ;
	ಪಕ್ಷ DL_PERIOD:
		ಮೌಲ್ಯ = dl_se->dl_period;
		ಭಙ್ಗ;
	}

	seq_printf(m, "%llu\n", ಮೌಲ್ಯ);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ssize_t
sched_fair_server_runtime_write(ಸಂರಚನಾ file *filp, ಧ್ರುವ ವರ್ಣ __user *ubuf,
				size_t cnt, loff_t *ppos)
{
	long cpu = (long) ((ಸಂರಚನಾ seq_file *) filp->private_data)->ಗುಪ್ತ;
	ಸಂರಚನಾ rq *rq = cpu_rq(cpu);

	ಪ್ರತ್ಯಾಗಮ sched_server_write_common(filp, ubuf, cnt, ppos, DL_RUNTIME,
					&rq->fair_server);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_fair_server_runtime_show(ಸಂರಚನಾ seq_file *m, ಶೂನ್ಯ *v)
{
	unsigned long cpu = (unsigned long) m->ಗುಪ್ತ;
	ಸಂರಚನಾ rq *rq = cpu_rq(cpu);

	ಪ್ರತ್ಯಾಗಮ sched_server_show_common(m, v, DL_RUNTIME, &rq->fair_server);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_fair_server_runtime_open(ಸಂರಚನಾ inode *inode, ಸಂರಚನಾ file *filp)
{
	ಪ್ರತ್ಯಾಗಮ single_open(filp, sched_fair_server_runtime_show, inode->i_private);
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ file_operations fair_server_runtime_fops = {
	.open		= sched_fair_server_runtime_open,
	.ಲಿಖ		= sched_fair_server_runtime_write,
	.ಪಠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ಸ್ಥಿರ ಸಂರಚನಾ dentry *debugfs_sched;

#ifdef CONFIG_SCHED_CLASS_EXT
ಸ್ಥಿರ ssize_t
sched_ext_server_runtime_write(ಸಂರಚನಾ file *filp, ಧ್ರುವ ವರ್ಣ __user *ubuf,
			       size_t cnt, loff_t *ppos)
{
	long cpu = (long) ((ಸಂರಚನಾ seq_file *) filp->private_data)->ಗುಪ್ತ;
	ಸಂರಚನಾ rq *rq = cpu_rq(cpu);

	ಪ್ರತ್ಯಾಗಮ sched_server_write_common(filp, ubuf, cnt, ppos, DL_RUNTIME,
					&rq->ext_server);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_ext_server_runtime_show(ಸಂರಚನಾ seq_file *m, ಶೂನ್ಯ *v)
{
	unsigned long cpu = (unsigned long) m->ಗುಪ್ತ;
	ಸಂರಚನಾ rq *rq = cpu_rq(cpu);

	ಪ್ರತ್ಯಾಗಮ sched_server_show_common(m, v, DL_RUNTIME, &rq->ext_server);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_ext_server_runtime_open(ಸಂರಚನಾ inode *inode, ಸಂರಚನಾ file *filp)
{
	ಪ್ರತ್ಯಾಗಮ single_open(filp, sched_ext_server_runtime_show, inode->i_private);
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ file_operations ext_server_runtime_fops = {
	.open		= sched_ext_server_runtime_open,
	.ಲಿಖ		= sched_ext_server_runtime_write,
	.ಪಠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ಸ್ಥಿರ ssize_t
sched_ext_server_period_write(ಸಂರಚನಾ file *filp, ಧ್ರುವ ವರ್ಣ __user *ubuf,
			      size_t cnt, loff_t *ppos)
{
	long cpu = (long) ((ಸಂರಚನಾ seq_file *) filp->private_data)->ಗುಪ್ತ;
	ಸಂರಚನಾ rq *rq = cpu_rq(cpu);

	ಪ್ರತ್ಯಾಗಮ sched_server_write_common(filp, ubuf, cnt, ppos, DL_PERIOD,
					&rq->ext_server);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_ext_server_period_show(ಸಂರಚನಾ seq_file *m, ಶೂನ್ಯ *v)
{
	unsigned long cpu = (unsigned long) m->ಗುಪ್ತ;
	ಸಂರಚನಾ rq *rq = cpu_rq(cpu);

	ಪ್ರತ್ಯಾಗಮ sched_server_show_common(m, v, DL_PERIOD, &rq->ext_server);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_ext_server_period_open(ಸಂರಚನಾ inode *inode, ಸಂರಚನಾ file *filp)
{
	ಪ್ರತ್ಯಾಗಮ single_open(filp, sched_ext_server_period_show, inode->i_private);
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ file_operations ext_server_period_fops = {
	.open		= sched_ext_server_period_open,
	.ಲಿಖ		= sched_ext_server_period_write,
	.ಪಠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ಸ್ಥಿರ ಶೂನ್ಯ debugfs_ext_server_init(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ dentry *d_ext;
	unsigned long cpu;

	d_ext = debugfs_create_dir("ext_server", debugfs_sched);
	ಯದಿ (!d_ext)
		ಪ್ರತ್ಯಾಗಮ;

	for_each_possible_cpu(cpu) {
		ಸಂರಚನಾ dentry *d_cpu;
		ವರ್ಣ buf[32];

		snprintf(buf, ಪರಿಮಾಣ(buf), "cpu%lu", cpu);
		d_cpu = debugfs_create_dir(buf, d_ext);

		debugfs_create_file("runtime", 0644, d_cpu, (ಶೂನ್ಯ *) cpu, &ext_server_runtime_fops);
		debugfs_create_file("period", 0644, d_cpu, (ಶೂನ್ಯ *) cpu, &ext_server_period_fops);
	}
}
#endif /* CONFIG_SCHED_CLASS_EXT */

ಸ್ಥಿರ ssize_t
sched_fair_server_period_write(ಸಂರಚನಾ file *filp, ಧ್ರುವ ವರ್ಣ __user *ubuf,
			       size_t cnt, loff_t *ppos)
{
	long cpu = (long) ((ಸಂರಚನಾ seq_file *) filp->private_data)->ಗುಪ್ತ;
	ಸಂರಚನಾ rq *rq = cpu_rq(cpu);

	ಪ್ರತ್ಯಾಗಮ sched_server_write_common(filp, ubuf, cnt, ppos, DL_PERIOD,
					&rq->fair_server);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_fair_server_period_show(ಸಂರಚನಾ seq_file *m, ಶೂನ್ಯ *v)
{
	unsigned long cpu = (unsigned long) m->ಗುಪ್ತ;
	ಸಂರಚನಾ rq *rq = cpu_rq(cpu);

	ಪ್ರತ್ಯಾಗಮ sched_server_show_common(m, v, DL_PERIOD, &rq->fair_server);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_fair_server_period_open(ಸಂರಚನಾ inode *inode, ಸಂರಚನಾ file *filp)
{
	ಪ್ರತ್ಯಾಗಮ single_open(filp, sched_fair_server_period_show, inode->i_private);
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ file_operations fair_server_period_fops = {
	.open		= sched_fair_server_period_open,
	.ಲಿಖ		= sched_fair_server_period_write,
	.ಪಠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ಸ್ಥಿರ ಶೂನ್ಯ debugfs_fair_server_init(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ dentry *d_fair;
	unsigned long cpu;

	d_fair = debugfs_create_dir("fair_server", debugfs_sched);
	ಯದಿ (!d_fair)
		ಪ್ರತ್ಯಾಗಮ;

	for_each_possible_cpu(cpu) {
		ಸಂರಚನಾ dentry *d_cpu;
		ವರ್ಣ buf[32];

		snprintf(buf, ಪರಿಮಾಣ(buf), "cpu%lu", cpu);
		d_cpu = debugfs_create_dir(buf, d_fair);

		debugfs_create_file("runtime", 0644, d_cpu, (ಶೂನ್ಯ *) cpu, &fair_server_runtime_fops);
		debugfs_create_file("period", 0644, d_cpu, (ಶೂನ್ಯ *) cpu, &fair_server_period_fops);
	}
}

#ifdef CONFIG_FAIR_GROUP_SCHED
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cgroup_mode = 2;

/* See __sched_cgroup_mode_update(). */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *cgroup_mode_str[] = {
	"up",
	"smp",
	"concur",
	"max",
	"tasks",
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_cgroup_mode(ಧ್ರುವ ವರ್ಣ *str)
{
	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ARRAY_SIZE(cgroup_mode_str); i++) {
		ಯದಿ (!strcmp(str, cgroup_mode_str[i]))
			ಪ್ರತ್ಯಾಗಮ i;
	}
	ಪ್ರತ್ಯಾಗಮ -EINVAL;
}

ಸ್ಥಿರ ssize_t sched_cgroup_write(ಸಂರಚನಾ file *filp, ಧ್ರುವ ವರ್ಣ __user *ubuf,
				   size_t cnt, loff_t *ppos)
{
	ವರ್ಣ buf[16];
	ಪೂರ್ಣಾಙ್ಕ mode;

	ಯದಿ (cnt > 15)
		cnt = 15;

	ಯದಿ (copy_from_user(buf, ubuf, cnt))
		ಪ್ರತ್ಯಾಗಮ -EFAULT;

	buf[cnt] = 0;
	mode = sched_cgroup_mode(strstrip(buf));
	ಯದಿ (mode < 0)
		ಪ್ರತ್ಯಾಗಮ mode;

	__sched_cgroup_mode_update(mode);
	WRITE_ONCE(cgroup_mode, mode);

	*ppos += cnt;
	ಪ್ರತ್ಯಾಗಮ cnt;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_cgroup_show(ಸಂರಚನಾ seq_file *m, ಶೂನ್ಯ *v)
{
	ಪೂರ್ಣಾಙ್ಕ mode = READ_ONCE(cgroup_mode);

	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ARRAY_SIZE(cgroup_mode_str); i++) {
		ಯದಿ (mode == i)
			seq_puts(m, "(");
		seq_puts(m, cgroup_mode_str[i]);
		ಯದಿ (mode == i)
			seq_puts(m, ")");

		seq_puts(m, " ");
	}
	seq_puts(m, "\n");
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_cgroup_open(ಸಂರಚನಾ inode *inode, ಸಂರಚನಾ file *filp)
{
	ಪ್ರತ್ಯಾಗಮ single_open(filp, sched_cgroup_show, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ file_operations sched_cgroup_fops = {
	.open		= sched_cgroup_open,
	.ಲಿಖ		= sched_cgroup_write,
	.ಪಠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};
#endif

ಸ್ಥಿರ __init ಪೂರ್ಣಾಙ್ಕ sched_init_debug(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ dentry __maybe_unused *numa, *llc;

	debugfs_sched = debugfs_create_dir("sched", ರಿಕ್ತ);

	debugfs_create_file("features", 0644, debugfs_sched, ರಿಕ್ತ, &sched_feat_fops);
	debugfs_create_file_unsafe("verbose", 0644, debugfs_sched, &sched_debug_verbose, &sched_verbose_fops);
#ifdef CONFIG_PREEMPT_DYNAMIC
	debugfs_create_file("preempt", 0644, debugfs_sched, ರಿಕ್ತ, &sched_dynamic_fops);
#endif

	debugfs_create_u32("base_slice_ns", 0644, debugfs_sched, &sysctl_sched_base_slice);

	debugfs_create_u32("latency_warn_ms", 0644, debugfs_sched, &sysctl_resched_latency_warn_ms);
	debugfs_create_u32("latency_warn_once", 0644, debugfs_sched, &sysctl_resched_latency_warn_once);

	debugfs_create_file("tunable_scaling", 0644, debugfs_sched, ರಿಕ್ತ, &sched_scaling_fops);
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
	debugfs_create_file("enabled", 0644, llc, ರಿಕ್ತ,
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

	debugfs_create_file("debug", 0444, debugfs_sched, ರಿಕ್ತ, &sched_debug_fops);

#ifdef CONFIG_FAIR_GROUP_SCHED
	debugfs_create_file("cgroup_mode", 0644, debugfs_sched, ರಿಕ್ತ, &sched_cgroup_fops);
#endif

	debugfs_fair_server_init();
#ifdef CONFIG_SCHED_CLASS_EXT
	debugfs_ext_server_init();
#endif

	ಪ್ರತ್ಯಾಗಮ 0;
}
late_initcall(sched_init_debug);

ಸ್ಥಿರ cpumask_var_t		sd_sysctl_cpus;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sd_flags_show(ಸಂರಚನಾ seq_file *m, ಶೂನ್ಯ *v)
{
	unsigned long flags = *(unsigned ಪೂರ್ಣಾಙ್ಕ *)m->ಗುಪ್ತ;
	ಪೂರ್ಣಾಙ್ಕ idx;

	for_each_set_bit(idx, &flags, __SD_FLAG_CNT) {
		seq_puts(m, sd_flag_debug[idx].name);
		seq_puts(m, " ");
	}
	seq_puts(m, "\n");

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sd_flags_open(ಸಂರಚನಾ inode *inode, ಸಂರಚನಾ file *file)
{
	ಪ್ರತ್ಯಾಗಮ single_open(file, sd_flags_show, inode->i_private);
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ file_operations sd_flags_fops = {
	.open		= sd_flags_open,
	.ಪಠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ಸ್ಥಿರ ಶೂನ್ಯ register_sd(ಸಂರಚನಾ sched_domain *sd, ಸಂರಚನಾ dentry *parent)
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

	ಯದಿ (sd->flags & SD_ASYM_PACKING)
		debugfs_create_u32("group_asym_prefer_cpu", 0444, parent,
				   (u32 *)&sd->groups->asym_prefer_cpu);
}

ಶೂನ್ಯ update_sched_domain_debugfs(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ cpu, i;

	/*
	 * This can unfortunately be invoked before sched_debug_init() creates
	 * the debug directory. Don't touch sd_sysctl_cpus until then.
	 */
	ಯದಿ (!debugfs_sched)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (!sched_debug_verbose)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (!cpumask_available(sd_sysctl_cpus)) {
		ಯದಿ (!alloc_cpumask_var(&sd_sysctl_cpus, GFP_KERNEL))
			ಪ್ರತ್ಯಾಗಮ;
		cpumask_copy(sd_sysctl_cpus, cpu_possible_mask);
	}

	ಯದಿ (!sd_dentry) {
		sd_dentry = debugfs_create_dir("domains", debugfs_sched);

		/* rebuild sd_sysctl_cpus if empty since it gets cleared below */
		ಯದಿ (cpumask_empty(sd_sysctl_cpus))
			cpumask_copy(sd_sysctl_cpus, cpu_online_mask);
	}

	for_each_cpu(cpu, sd_sysctl_cpus) {
		ಸಂರಚನಾ sched_domain *sd;
		ಸಂರಚನಾ dentry *d_cpu;
		ವರ್ಣ buf[32];

		snprintf(buf, ಪರಿಮಾಣ(buf), "cpu%d", cpu);
		debugfs_lookup_and_remove(buf, sd_dentry);
		d_cpu = debugfs_create_dir(buf, sd_dentry);

		i = 0;
		for_each_domain(cpu, sd) {
			ಸಂರಚನಾ dentry *d_sd;

			snprintf(buf, ಪರಿಮಾಣ(buf), "domain%d", i);
			d_sd = debugfs_create_dir(buf, d_cpu);

			register_sd(sd, d_sd);
			i++;
		}

		__cpumask_clear_cpu(cpu, sd_sysctl_cpus);
	}
}

ಶೂನ್ಯ dirty_sched_domain_sysctl(ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಯದಿ (cpumask_available(sd_sysctl_cpus))
		__cpumask_set_cpu(cpu, sd_sysctl_cpus);
}

#ifdef CONFIG_FAIR_GROUP_SCHED
ಸ್ಥಿರ ಶೂನ್ಯ print_cfs_group_stats(ಸಂರಚನಾ seq_file *m, ಪೂರ್ಣಾಙ್ಕ cpu, ಸಂರಚನಾ task_group *tg)
{
	ಸಂರಚನಾ sched_entity *se = tg_se(tg, cpu);

#define P(F)		SEQ_printf(m, "  .%-30s: %lld\n",	#F, (long long)F)
#define P_SCHEDSTAT(F)	SEQ_printf(m, "  .%-30s: %lld\n",	\
		#F, (long long)schedstat_val(stats->F))
#define PN(F)		SEQ_printf(m, "  .%-30s: %lld.%06ld\n", #F, SPLIT_NS((long long)F))
#define PN_SCHEDSTAT(F)	SEQ_printf(m, "  .%-30s: %lld.%06ld\n", \
		#F, SPLIT_NS((long long)schedstat_val(stats->F)))

	ಯದಿ (!se)
		ಪ್ರತ್ಯಾಗಮ;

	PN(se->exec_start);
	PN(se->vruntime);
	PN(se->sum_exec_runtime);

	ಯದಿ (schedstat_enabled()) {
		ಸಂರಚನಾ sched_statistics *stats;
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
		P_SCHEDSTAT(wait_ಎಣಿಕೆ);
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
ಸ್ಥಿರ DEFINE_SPINLOCK(sched_debug_lock);
ಸ್ಥಿರ ವರ್ಣ group_path[PATH_MAX];

ಸ್ಥಿರ ಶೂನ್ಯ task_group_path(ಸಂರಚನಾ task_group *tg, ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ plen)
{
	ಯದಿ (autogroup_path(tg, path, plen))
		ಪ್ರತ್ಯಾಗಮ;

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
	ಯದಿ (spin_trylock(&sched_debug_lock)) {				\
		task_group_path(tg, group_path, ಪರಿಮಾಣ(group_path));	\
		SEQ_printf(m, fmt, group_path);				\
		spin_unlock(&sched_debug_lock);				\
	} ಅನ್ಯಥಾ {							\
		ವರ್ಣ buf[128];						\
		ವರ್ಣ *bufend = buf + ಪರಿಮಾಣ(buf) - 3;			\
		task_group_path(tg, buf, bufend - buf);			\
		strcpy(bufend - 1, "...");				\
		SEQ_printf(m, fmt, buf);				\
	}								\
}
#endif

ಸ್ಥಿರ ಶೂನ್ಯ
print_task(ಸಂರಚನಾ seq_file *m, ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p)
{
	ಯದಿ (task_current(rq, p))
		SEQ_printf(m, ">R");
	ಅನ್ಯಥಾ
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
	SEQ_printf(m, "   %d      %d", task_ಗಂಟು(p), task_numa_group_id(p));
#endif
#ifdef CONFIG_CGROUP_SCHED
	SEQ_printf_task_group_path(m, task_group(p), "        %s")
#endif

	SEQ_printf(m, "\n");
}

ಸ್ಥಿರ ಶೂನ್ಯ print_rq(ಸಂರಚನಾ seq_file *m, ಸಂರಚನಾ rq *rq, ಪೂರ್ಣಾಙ್ಕ rq_cpu)
{
	ಸಂರಚನಾ task_struct *g, *p;

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
		ಯದಿ (task_cpu(p) != rq_cpu)
			ಅನುವರ್ತ;

		print_task(m, rq, p);
	}
	rcu_read_unlock();
}

ಶೂನ್ಯ print_cfs_rq(ಸಂರಚನಾ seq_file *m, ಪೂರ್ಣಾಙ್ಕ cpu, ಸಂರಚನಾ cfs_rq *cfs_rq)
{
	s64 ಎಡ_vruntime = -1, ಬಲ_vruntime = -1, ಎಡ_deadline = -1, spread;
	s64 zero_vruntime = -1, sum_w_vruntime = -1;
	u64 avruntime;
	ಸಂರಚನಾ sched_entity *last, *first, *ಬೇರು;
	ಸಂರಚನಾ rq *rq = cpu_rq(cpu);
	unsigned ಪೂರ್ಣಾಙ್ಕ sum_shift;
	unsigned long flags;
	u64 sum_weight;

#ifdef CONFIG_FAIR_GROUP_SCHED
	SEQ_printf(m, "\n");
	SEQ_printf_task_group_path(m, cfs_rq->tg, "cfs_rq[%d]:%s\n", cpu);
#ಅನ್ಯಥಾ
	SEQ_printf(m, "\n");
	SEQ_printf(m, "cfs_rq[%d]:\n", cpu);
#endif

	raw_spin_rq_lock_irqsave(rq, flags);
	ಬೇರು = __pick_root_entity(cfs_rq);
	ಯದಿ (ಬೇರು)
		ಎಡ_vruntime = ಬೇರು->min_vruntime;
	first = __pick_first_entity(cfs_rq);
	ಯದಿ (first)
		ಎಡ_deadline = first->deadline;
	last = __pick_last_entity(cfs_rq);
	ಯದಿ (last)
		ಬಲ_vruntime = last->vruntime;
	zero_vruntime = cfs_rq->zero_vruntime;
	sum_w_vruntime = cfs_rq->sum_w_vruntime;
	sum_weight = cfs_rq->sum_weight;
	sum_shift = cfs_rq->sum_shift;
	avruntime = avg_vruntime(cfs_rq);
	raw_spin_rq_unlock_irqrestore(rq, flags);

	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "left_deadline",
			SPLIT_NS(ಎಡ_deadline));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "left_vruntime",
			SPLIT_NS(ಎಡ_vruntime));
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
			SPLIT_NS(ಬಲ_vruntime));
	spread = ಬಲ_vruntime - ಎಡ_vruntime;
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
			cfs_rq->throttle_ಎಣಿಕೆ);
#endif

#ifdef CONFIG_FAIR_GROUP_SCHED
	print_cfs_group_stats(m, cpu, cfs_rq->tg);
#endif
}

ಶೂನ್ಯ print_rt_rq(ಸಂರಚನಾ seq_file *m, ಪೂರ್ಣಾಙ್ಕ cpu, ಸಂರಚನಾ rt_rq *rt_rq)
{
#ifdef CONFIG_RT_GROUP_SCHED
	SEQ_printf(m, "\n");
	SEQ_printf_task_group_path(m, rt_rq->tg, "rt_rq[%d]:%s\n", cpu);
#ಅನ್ಯಥಾ
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

ಶೂನ್ಯ print_dl_rq(ಸಂರಚನಾ seq_file *m, ಪೂರ್ಣಾಙ್ಕ cpu, ಸಂರಚನಾ dl_rq *dl_rq)
{
	ಸಂರಚನಾ dl_bw *dl_bw;

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

ಸ್ಥಿರ ಶೂನ್ಯ print_cpu(ಸಂರಚನಾ seq_file *m, ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಸಂರಚನಾ rq *rq = cpu_rq(cpu);

#ifdef CONFIG_X86
	{
		unsigned ಪೂರ್ಣಾಙ್ಕ freq = cpu_khz ? : 1;

		SEQ_printf(m, "cpu#%d, %u.%03u MHz\n",
			   cpu, freq / 1000, (freq % 1000));
	}
#ಅನ್ಯಥಾ /* !CONFIG_X86: */
	SEQ_printf(m, "cpu#%d\n", cpu);
#endif /* !CONFIG_X86 */

#define P(x)								\
ಕುರು {									\
	ಯದಿ (ಪರಿಮಾಣ(rq->x) == 4)						\
		SEQ_printf(m, "  .%-30s: %d\n", #x, (ಪೂರ್ಣಾಙ್ಕ)(rq->x));	\
	ಅನ್ಯಥಾ								\
		SEQ_printf(m, "  .%-30s: %Ld\n", #x, (long long)(rq->x));\
} ಯಾವತ್ (0)

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
	ಯದಿ (schedstat_enabled()) {
		P(yld_ಎಣಿಕೆ);
		P(sched_ಎಣಿಕೆ);
		P(sched_goidle);
		P(ttwu_ಎಣಿಕೆ);
		P(ttwu_local);
	}
#undef P

	print_cfs_stats(m, cpu);
	print_rt_stats(m, cpu);
	print_dl_stats(m, cpu);

	print_rq(m, rq, cpu);
	SEQ_printf(m, "\n");
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *sched_tunable_scaling_names[] = {
	"none",
	"logarithmic",
	"linear"
};

ಸ್ಥಿರ ಶೂನ್ಯ sched_debug_header(ಸಂರಚನಾ seq_file *m)
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
		(ಪೂರ್ಣಾಙ್ಕ)strcspn(init_utsname()->version, " "),
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

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_debug_show(ಸಂರಚನಾ seq_file *m, ಶೂನ್ಯ *v)
{
	ಪೂರ್ಣಾಙ್ಕ cpu = (unsigned long)(v - 2);

	ಯದಿ (cpu != -1)
		print_cpu(m, cpu);
	ಅನ್ಯಥಾ
		sched_debug_header(m);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ sysrq_sched_debug_show(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ cpu;

	sched_debug_header(ರಿಕ್ತ);
	for_each_online_cpu(cpu) {
		/*
		 * Need to reset softlockup watchdogs on all CPUs, because
		 * another CPU might be blocked waiting for us to process
		 * an IPI or stop_machine.
		 */
		touch_nmi_watchdog();
		touch_all_softlockup_watchdogs();
		print_cpu(ರಿಕ್ತ, cpu);
	}
}

/*
 * This iterator needs some explanation.
 * It returns 1 for the header position.
 * This means 2 is CPU 0.
 * In a hotplugged system some CPUs, including CPU 0, may be missing so we have
 * to use cpumask_* to iterate over the CPUs.
 */
ಸ್ಥಿರ ಶೂನ್ಯ *sched_debug_start(ಸಂರಚನಾ seq_file *file, loff_t *offset)
{
	unsigned long n = *offset;

	ಯದಿ (n == 0)
		ಪ್ರತ್ಯಾಗಮ (ಶೂನ್ಯ *) 1;

	n--;

	ಯದಿ (n > 0)
		n = cpumask_next(n - 1, cpu_online_mask);
	ಅನ್ಯಥಾ
		n = cpumask_first(cpu_online_mask);

	*offset = n + 1;

	ಯದಿ (n < nr_cpu_ids)
		ಪ್ರತ್ಯಾಗಮ (ಶೂನ್ಯ *)(unsigned long)(n + 2);

	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಶೂನ್ಯ *sched_debug_next(ಸಂರಚನಾ seq_file *file, ಶೂನ್ಯ *data, loff_t *offset)
{
	(*offset)++;
	ಪ್ರತ್ಯಾಗಮ sched_debug_start(file, offset);
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_debug_stop(ಸಂರಚನಾ seq_file *file, ಶೂನ್ಯ *data)
{
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ seq_operations sched_debug_sops = {
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
ಶೂನ್ಯ print_numa_stats(ಸಂರಚನಾ seq_file *m, ಪೂರ್ಣಾಙ್ಕ ಗಂಟು, unsigned long tsf,
		unsigned long tpf, unsigned long gsf, unsigned long gpf)
{
	SEQ_printf(m, "numa_faults node=%d ", ಗಂಟು);
	SEQ_printf(m, "task_private=%lu task_shared=%lu ", tpf, tsf);
	SEQ_printf(m, "group_private=%lu group_shared=%lu\n", gpf, gsf);
}
#endif


ಸ್ಥಿರ ಶೂನ್ಯ sched_show_numa(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ seq_file *m)
{
#ifdef CONFIG_NUMA_BALANCING
	ಯದಿ (p->mm)
		P(mm->numa_scan_seq);

	P(numa_pages_migrated);
	P(numa_preferred_nid);
	P(total_numa_faults);
	SEQ_printf(m, "current_node=%d, numa_group_id=%d\n",
			task_ಗಂಟು(p), task_numa_group_id(p));
	show_numa_stats(p, m);
#endif /* CONFIG_NUMA_BALANCING */
}

ಶೂನ್ಯ proc_sched_show_task(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ pid_namespace *ns,
						  ಸಂರಚನಾ seq_file *m)
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

	ಯದಿ (schedstat_enabled()) {
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
		P_SCHEDSTAT(wait_ಎಣಿಕೆ);
		PN_SCHEDSTAT(iowait_sum);
		P_SCHEDSTAT(iowait_ಎಣಿಕೆ);
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
		ಯದಿ (nr_switches)
			avg_atom = div64_ul(avg_atom, nr_switches);
		ಅನ್ಯಥಾ
			avg_atom = -1LL;

		avg_per_cpu = p->se.sum_exec_runtime;
		ಯದಿ (p->se.nr_migrations) {
			avg_per_cpu = div64_u64(avg_per_cpu,
						p->se.nr_migrations);
		} ಅನ್ಯಥಾ {
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
	__PS("uclamp.min", p->uclamp_req[UCLAMP_MIN].ಮೌಲ್ಯ);
	__PS("uclamp.max", p->uclamp_req[UCLAMP_MAX].ಮೌಲ್ಯ);
	__PS("effective uclamp.min", uclamp_eff_ಮೌಲ್ಯ(p, UCLAMP_MIN));
	__PS("effective uclamp.max", uclamp_eff_ಮೌಲ್ಯ(p, UCLAMP_MAX));
#endif /* CONFIG_UCLAMP_TASK */
	P(policy);
	P(prio);
	ಯದಿ (task_has_dl_policy(p)) {
		P(dl.runtime);
		P(dl.deadline);
	} ಅನ್ಯಥಾ ಯದಿ (fair_policy(p->policy)) {
		P(se.slice);
	}
#ifdef CONFIG_SCHED_CLASS_EXT
	__PS("ext.enabled", task_on_scx(p));
#endif
#undef PN_SCHEDSTAT
#undef P_SCHEDSTAT

	{
		unsigned ಪೂರ್ಣಾಙ್ಕ this_cpu = raw_smp_processor_id();
		u64 t0, t1;

		t0 = cpu_clock(this_cpu);
		t1 = cpu_clock(this_cpu);
		__PS("clock-delta", t1-t0);
	}

	sched_show_numa(p, m);
}

ಶೂನ್ಯ proc_sched_set_task(ಸಂರಚನಾ task_struct *p)
{
#ifdef CONFIG_SCHEDSTATS
	memset(&p->stats, 0, ಪರಿಮಾಣ(p->stats));
#endif
}

ಶೂನ್ಯ resched_latency_warn(ಪೂರ್ಣಾಙ್ಕ cpu, u64 latency)
{
	ಸ್ಥಿರ DEFINE_RATELIMIT_STATE(latency_check_ratelimit, 60 * 60 * HZ, 1);

	ಯದಿ (likely(!__ratelimit(&latency_check_ratelimit)))
		ಪ್ರತ್ಯಾಗಮ;

	pr_err("sched: CPU %d need_resched set for > %llu ns (%d ticks) without schedule\n",
	       cpu, latency, cpu_rq(cpu)->ticks_without_resched);
	dump_ರಾಶಿ();
}
