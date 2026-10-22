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
#ವ್ಯಾಖ್ಯಾನಿಸು SEQ_printf(m, x...)			\
 ಮಾಡು {						\
	ಆದರೆ (m)					\
		seq_printf(m, x);		\
	ಇಲ್ಲದಿದ್ದರೆ					\
		pr_cont(x);			\
 } ಆಗ (0)

/*
 * Ease the printing of nsec fields:
 */
ಸ್ಥಿರ ದೀರ್ಘ ದೀರ್ಘ nsec_ಮೇಲಿನ(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ nsec)
{
	ಆದರೆ ((ದೀರ್ಘ ದೀರ್ಘ)nsec < 0) {
		nsec = -nsec;
		do_div(nsec, 1000000);
		ಹಿಂತಿರುಗಿ -nsec;
	}
	do_div(nsec, 1000000);

	ಹಿಂತಿರುಗಿ nsec;
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ nsec_ಕೆಳಗಿನ(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ nsec)
{
	ಆದರೆ ((ದೀರ್ಘ ದೀರ್ಘ)nsec < 0)
		nsec = -nsec;

	ಹಿಂತಿರುಗಿ do_div(nsec, 1000000);
}

#ವ್ಯಾಖ್ಯಾನಿಸು ವಿಭಜಿಸು_NS(x) nsec_ಮೇಲಿನ(x), nsec_ಕೆಳಗಿನ(x)

#ವ್ಯಾಖ್ಯಾನಿಸು SCHED_FEAT(name, enabled)	\
	#name ,

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ sched_feat_ಹೆಸರುಗಳು[] = {
#ಅನ್ತರ್ಭಾವ "features.h"
};

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು SCHED_FEAT

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_feat_show(ರಚನೆ seq_file *m, ಶೂನ್ಯ *v)
{
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; i < __SCHED_FEAT_NR; i++) {
		ಆದರೆ (!(sysctl_sched_features & (1UL << i)))
			seq_puts(m, "NO_");
		seq_printf(m, "%s ", sched_feat_ಹೆಸರುಗಳು[i]);
	}
	seq_puts(m, "\n");

	ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_JUMP_LABEL

#ವ್ಯಾಖ್ಯಾನಿಸು jump_label_ಕೀಲಿ_true  { .ಕೀಲಿ_true  = STATIC_ಕೀಲಿ_TRUE_INIT  }
#ವ್ಯಾಖ್ಯಾನಿಸು jump_label_ಕೀಲಿ_false { .ಕೀಲಿ_false = STATIC_ಕೀಲಿ_FALSE_INIT }

#ವ್ಯಾಖ್ಯಾನಿಸು SCHED_FEAT(name, enabled)	\
	jump_label_ಕೀಲಿ##enabled ,

ಒಕ್ಕೂಟ sched_feat_ಕೀಲಿ sched_feat_ಕೀಲಿಗಳು[__SCHED_FEAT_NR] = {
#ಅನ್ತರ್ಭಾವ "features.h"
};

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು SCHED_FEAT

ಸ್ಥಿರ ಶೂನ್ಯ sched_feat_disable(ಪೂರ್ಣಾಂಕ i)
{
	static_branch_disable_cpuslocked(&sched_feat_ಕೀಲಿಗಳು[i].ಕೀಲಿ_true);
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_feat_enable(ಪೂರ್ಣಾಂಕ i)
{
	static_branch_enable_cpuslocked(&sched_feat_ಕೀಲಿಗಳು[i].ಕೀಲಿ_false);
}
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_JUMP_LABEL: */
ಸ್ಥಿರ ಶೂನ್ಯ sched_feat_disable(ಪೂರ್ಣಾಂಕ i) { };
ಸ್ಥಿರ ಶೂನ್ಯ sched_feat_enable(ಪೂರ್ಣಾಂಕ i) { };
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_JUMP_LABEL */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_feat_ಗಣ(ಅಕ್ಷರ *cmp)
{
	ಪೂರ್ಣಾಂಕ i;
	ಪೂರ್ಣಾಂಕ neg = 0;

	ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(cmp, "NO_", 3) == 0) {
		neg = 1;
		cmp += 3;
	}

	i = ಹೊಂದಿಕೆ_string(sched_feat_ಹೆಸರುಗಳು, __SCHED_FEAT_NR, cmp);
	ಆದರೆ (i < 0)
		ಹಿಂತಿರುಗಿ i;

	ಆದರೆ (neg) {
		sysctl_sched_features &= ~(1UL << i);
		sched_feat_disable(i);
	} ಇಲ್ಲದಿದ್ದರೆ {
		sysctl_sched_features |= (1UL << i);
		sched_feat_enable(i);
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ssize_t
sched_feat_write(ರಚನೆ file *filp, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ __user *ubuf,
		ಗಾತ್ರ_t cnt, loff_t *ppos)
{
	ಅಕ್ಷರ buf[64];
	ಅಕ್ಷರ *cmp;
	ಪೂರ್ಣಾಂಕ ret;
	ರಚನೆ inode *inode;

	ಆದರೆ (cnt > 63)
		cnt = 63;

	ಆದರೆ (copy_from_user(buf, ubuf, cnt))
		ಹಿಂತಿರುಗಿ -EFAULT;

	buf[cnt] = 0;
	cmp = strstrip(buf);

	/* Ensure the static_key remains in a consistent state */
	inode = file_inode(filp);
	cpus_read_ಬೀಗ();
	inode_ಬೀಗ(inode);
	ret = sched_feat_ಗಣ(cmp);
	inode_unlock(inode);
	cpus_read_unlock();
	ಆದರೆ (ret < 0)
		ಹಿಂತಿರುಗಿ ret;

	*ppos += cnt;

	ಹಿಂತಿರುಗಿ cnt;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_feat_open(ರಚನೆ inode *inode, ರಚನೆ file *filp)
{
	ಹಿಂತಿರುಗಿ single_open(filp, sched_feat_show, ಖಾಲಿ);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ file_operations sched_feat_fops = {
	.open		= sched_feat_open,
	.ಲಿಖ		= sched_feat_write,
	.ಪಠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ಸ್ಥಿರ ssize_t sched_scaling_write(ರಚನೆ file *filp, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ __user *ubuf,
				   ಗಾತ್ರ_t cnt, loff_t *ppos)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ scaling;
	ಪೂರ್ಣಾಂಕ ret;

	ret = kstrtouint_from_user(ubuf, cnt, 10, &scaling);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;

	ಆದರೆ (scaling >= SCHED_TUNABLESCALING_ಕೊನೆ)
		ಹಿಂತಿರುಗಿ -EINVAL;

	sysctl_sched_tunable_scaling = scaling;
	ಆದರೆ (sched_update_scaling())
		ಹಿಂತಿರುಗಿ -EINVAL;

	*ppos += cnt;
	ಹಿಂತಿರುಗಿ cnt;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_scaling_show(ರಚನೆ seq_file *m, ಶೂನ್ಯ *v)
{
	seq_printf(m, "%d\n", sysctl_sched_tunable_scaling);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_scaling_open(ರಚನೆ inode *inode, ರಚನೆ file *filp)
{
	ಹಿಂತಿರುಗಿ single_open(filp, sched_scaling_show, ಖಾಲಿ);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ file_operations sched_scaling_fops = {
	.open		= sched_scaling_open,
	.ಲಿಖ		= sched_scaling_write,
	.ಪಠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CACHE
ಸ್ಥಿರ ssize_t
sched_cache_enable_write(ರಚನೆ file *filp, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ __user *ubuf,
			 ಗಾತ್ರ_t cnt, loff_t *ppos)
{
	bool val;
	ಪೂರ್ಣಾಂಕ ret;

	ret = kstrtobool_from_user(ubuf, cnt, &val);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;

	sysctl_sched_cache_user = val;

	sched_cache_ಸಕ್ರಿಯ_ಗಣ();

	*ppos += cnt;

	ಹಿಂತಿರುಗಿ cnt;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_cache_enable_show(ರಚನೆ seq_file *m, ಶೂನ್ಯ *v)
{
	seq_printf(m, "%d\n", sysctl_sched_cache_user);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_cache_enable_open(ರಚನೆ inode *inode,
				   ರಚನೆ file *filp)
{
	ಹಿಂತಿರುಗಿ single_open(filp, sched_cache_enable_show, ಖಾಲಿ);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ file_operations sched_cache_enable_fops = {
	.open           = sched_cache_enable_open,
	.ಲಿಖ          = sched_cache_enable_write,
	.ಪಠ           = seq_read,
	.llseek         = seq_lseek,
	.release        = single_release,
};
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PREEMPT_DYNAMIC

ಸ್ಥಿರ ssize_t sched_dynamic_write(ರಚನೆ file *filp, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ __user *ubuf,
				   ಗಾತ್ರ_t cnt, loff_t *ppos)
{
	ಅಕ್ಷರ buf[16];
	ಪೂರ್ಣಾಂಕ ಬಹುಲಕ;

	ಆದರೆ (cnt > 15)
		cnt = 15;

	ಆದರೆ (copy_from_user(buf, ubuf, cnt))
		ಹಿಂತಿರುಗಿ -EFAULT;

	buf[cnt] = 0;
	ಬಹುಲಕ = sched_dynamic_ಬಹುಲಕ(strstrip(buf));
	ಆದರೆ (ಬಹುಲಕ < 0)
		ಹಿಂತಿರುಗಿ ಬಹುಲಕ;

	sched_dynamic_update(ಬಹುಲಕ);

	*ppos += cnt;

	ಹಿಂತಿರುಗಿ cnt;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_dynamic_show(ರಚನೆ seq_file *m, ಶೂನ್ಯ *v)
{
	ಪೂರ್ಣಾಂಕ ಬಹುಲಕ = READ_ONCE(preempt_dynamic_ಬಹುಲಕ);

	/* Stop at NULL terminator */
	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; preempt_modes[i]; i++) {
		ಆದರೆ (ಬಹುಲಕ == i)
			seq_puts(m, "(");
		seq_puts(m, preempt_modes[i]);
		ಆದರೆ (ಬಹುಲಕ == i)
			seq_puts(m, ")");

		seq_puts(m, " ");
	}

	seq_puts(m, "\n");
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_dynamic_open(ರಚನೆ inode *inode, ರಚನೆ file *filp)
{
	ಹಿಂತಿರುಗಿ single_open(filp, sched_dynamic_show, ಖಾಲಿ);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ file_operations sched_dynamic_fops = {
	.open		= sched_dynamic_open,
	.ಲಿಖ		= sched_dynamic_write,
	.ಪಠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_PREEMPT_DYNAMIC */

__read_mostly bool sched_debug_verbose;

ಸ್ಥಿರ ರಚನೆ dentry           *sd_dentry;


ಸ್ಥಿರ ssize_t sched_verbose_write(ರಚನೆ file *filp, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ __user *ubuf,
				  ಗಾತ್ರ_t cnt, loff_t *ppos)
{
	ssize_t ಫಲಿತಾಂಶ;
	bool orig;

	cpus_read_ಬೀಗ();
	sched_domains_mutex_ಬೀಗ();

	orig = sched_debug_verbose;
	ಫಲಿತಾಂಶ = debugfs_write_file_bool(filp, ubuf, cnt, ppos);

	ಆದರೆ (sched_debug_verbose && !orig)
		update_sched_domain_debugfs();
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!sched_debug_verbose && orig) {
		debugfs_remove(sd_dentry);
		sd_dentry = ಖಾಲಿ;
	}

	sched_domains_mutex_unlock();
	cpus_read_unlock();

	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ file_operations sched_verbose_fops = {
	.ಪಠ =         debugfs_read_file_bool,
	.ಲಿಖ =        sched_verbose_write,
	.open =         simple_open,
	.llseek =       default_llseek,
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ seq_operations sched_debug_sops;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_debug_open(ರಚನೆ inode *inode, ರಚನೆ file *filp)
{
	ಹಿಂತಿರುಗಿ seq_open(filp, &sched_debug_sops);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ file_operations sched_debug_fops = {
	.open		= sched_debug_open,
	.ಪಠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

ಎಣಿಕೆ dl_param {
	DL_RUNTIME = 0,
	DL_PERIOD,
};

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ dl_server_period_max = (1UL << 22) * NSEC_ಪ್ರತಿ_USEC; /* ~4 seconds */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ dl_server_period_min = (100) * NSEC_ಪ್ರತಿ_USEC;     /* 100 us */

ಸ್ಥಿರ ssize_t sched_server_write_common(ರಚನೆ file *filp, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ __user *ubuf,
					 ಗಾತ್ರ_t cnt, loff_t *ppos, ಎಣಿಕೆ dl_param param,
					 ಶೂನ್ಯ *server)
{
	ದೀರ್ಘ cpu = (ದೀರ್ಘ) ((ರಚನೆ seq_file *) filp->ಖಾಸಗಿ_ದತ್ತಾಂಶ)->ಖಾಸಗಿ;
	ರಚನೆ sched_dl_entity *dl_se = (ರಚನೆ sched_dl_entity *)server;
	u64 old_runtime, runtime, period;
	ರಚನೆ rq *rq = cpu_rq(cpu);
	ಪೂರ್ಣಾಂಕ retval = 0;
	ಗಾತ್ರ_t err;
	u64 ಮೌಲ್ಯ;

	err = kstrtoull_from_user(ubuf, cnt, 10, &ಮೌಲ್ಯ);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ err;

	scoped_guard (rq_ಬೀಗ_irqsave, rq) {
		old_runtime = runtime = dl_se->dl_runtime;
		period = dl_se->dl_period;

		ಆಯ್ಕೆ (param) {
		ಸ್ಥಿತಿ DL_RUNTIME:
			ಆದರೆ (runtime == ಮೌಲ್ಯ)
				ನಿಲ್ಲಿಸು;
			runtime = ಮೌಲ್ಯ;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ DL_PERIOD:
			ಆದರೆ (ಮೌಲ್ಯ == period)
				ನಿಲ್ಲಿಸು;
			period = ಮೌಲ್ಯ;
			ನಿಲ್ಲಿಸು;
		}

		ಆದರೆ (runtime > period ||
		    period > dl_server_period_max ||
		    period < dl_server_period_min) {
			ಹಿಂತಿರುಗಿ  -EINVAL;
		}

		ಆದರೆ (!cpu_online(cpu_ದ(rq)))
			ಹಿಂತಿರುಗಿ -EBUSY;

		update_rq_clock(rq);
		dl_server_stop(dl_se);
		retval = dl_server_apply_params(dl_se, runtime, period, 0);
		dl_server_ಆರಂಭ(dl_se);

		ಆದರೆ (retval < 0)
			ಹಿಂತಿರುಗಿ retval;
	}

	ಆದರೆ (!!old_runtime ^ !!runtime) {
		pr_info("%s server %sabled on CPU %d%s.\n",
			server == &rq->fair_server ? "Fair" : "Ext",
			runtime ? "en" : "dis",
			cpu_ದ(rq),
			runtime ? "" : ", system may malfunction due to starvation");
	}

	*ppos += cnt;
	ಹಿಂತಿರುಗಿ cnt;
}

ಸ್ಥಿರ ಗಾತ್ರ_t sched_server_show_common(ರಚನೆ seq_file *m, ಶೂನ್ಯ *v, ಎಣಿಕೆ dl_param param,
				       ಶೂನ್ಯ *server)
{
	ರಚನೆ sched_dl_entity *dl_se = (ರಚನೆ sched_dl_entity *)server;
	u64 ಮೌಲ್ಯ;

	ಆಯ್ಕೆ (param) {
	ಸ್ಥಿತಿ DL_RUNTIME:
		ಮೌಲ್ಯ = dl_se->dl_runtime;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ DL_PERIOD:
		ಮೌಲ್ಯ = dl_se->dl_period;
		ನಿಲ್ಲಿಸು;
	}

	seq_printf(m, "%llu\n", ಮೌಲ್ಯ);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ssize_t
sched_fair_server_runtime_write(ರಚನೆ file *filp, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ __user *ubuf,
				ಗಾತ್ರ_t cnt, loff_t *ppos)
{
	ದೀರ್ಘ cpu = (ದೀರ್ಘ) ((ರಚನೆ seq_file *) filp->ಖಾಸಗಿ_ದತ್ತಾಂಶ)->ಖಾಸಗಿ;
	ರಚನೆ rq *rq = cpu_rq(cpu);

	ಹಿಂತಿರುಗಿ sched_server_write_common(filp, ubuf, cnt, ppos, DL_RUNTIME,
					&rq->fair_server);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_fair_server_runtime_show(ರಚನೆ seq_file *m, ಶೂನ್ಯ *v)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu = (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) m->ಖಾಸಗಿ;
	ರಚನೆ rq *rq = cpu_rq(cpu);

	ಹಿಂತಿರುಗಿ sched_server_show_common(m, v, DL_RUNTIME, &rq->fair_server);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_fair_server_runtime_open(ರಚನೆ inode *inode, ರಚನೆ file *filp)
{
	ಹಿಂತಿರುಗಿ single_open(filp, sched_fair_server_runtime_show, inode->i_ಖಾಸಗಿ);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ file_operations fair_server_runtime_fops = {
	.open		= sched_fair_server_runtime_open,
	.ಲಿಖ		= sched_fair_server_runtime_write,
	.ಪಠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ಸ್ಥಿರ ರಚನೆ dentry *debugfs_sched;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CLASS_EXT
ಸ್ಥಿರ ssize_t
sched_ext_server_runtime_write(ರಚನೆ file *filp, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ __user *ubuf,
			       ಗಾತ್ರ_t cnt, loff_t *ppos)
{
	ದೀರ್ಘ cpu = (ದೀರ್ಘ) ((ರಚನೆ seq_file *) filp->ಖಾಸಗಿ_ದತ್ತಾಂಶ)->ಖಾಸಗಿ;
	ರಚನೆ rq *rq = cpu_rq(cpu);

	ಹಿಂತಿರುಗಿ sched_server_write_common(filp, ubuf, cnt, ppos, DL_RUNTIME,
					&rq->ext_server);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_ext_server_runtime_show(ರಚನೆ seq_file *m, ಶೂನ್ಯ *v)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu = (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) m->ಖಾಸಗಿ;
	ರಚನೆ rq *rq = cpu_rq(cpu);

	ಹಿಂತಿರುಗಿ sched_server_show_common(m, v, DL_RUNTIME, &rq->ext_server);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_ext_server_runtime_open(ರಚನೆ inode *inode, ರಚನೆ file *filp)
{
	ಹಿಂತಿರುಗಿ single_open(filp, sched_ext_server_runtime_show, inode->i_ಖಾಸಗಿ);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ file_operations ext_server_runtime_fops = {
	.open		= sched_ext_server_runtime_open,
	.ಲಿಖ		= sched_ext_server_runtime_write,
	.ಪಠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ಸ್ಥಿರ ssize_t
sched_ext_server_period_write(ರಚನೆ file *filp, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ __user *ubuf,
			      ಗಾತ್ರ_t cnt, loff_t *ppos)
{
	ದೀರ್ಘ cpu = (ದೀರ್ಘ) ((ರಚನೆ seq_file *) filp->ಖಾಸಗಿ_ದತ್ತಾಂಶ)->ಖಾಸಗಿ;
	ರಚನೆ rq *rq = cpu_rq(cpu);

	ಹಿಂತಿರುಗಿ sched_server_write_common(filp, ubuf, cnt, ppos, DL_PERIOD,
					&rq->ext_server);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_ext_server_period_show(ರಚನೆ seq_file *m, ಶೂನ್ಯ *v)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu = (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) m->ಖಾಸಗಿ;
	ರಚನೆ rq *rq = cpu_rq(cpu);

	ಹಿಂತಿರುಗಿ sched_server_show_common(m, v, DL_PERIOD, &rq->ext_server);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_ext_server_period_open(ರಚನೆ inode *inode, ರಚನೆ file *filp)
{
	ಹಿಂತಿರುಗಿ single_open(filp, sched_ext_server_period_show, inode->i_ಖಾಸಗಿ);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ file_operations ext_server_period_fops = {
	.open		= sched_ext_server_period_open,
	.ಲಿಖ		= sched_ext_server_period_write,
	.ಪಠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ಸ್ಥಿರ ಶೂನ್ಯ debugfs_ext_server_init(ಶೂನ್ಯ)
{
	ರಚನೆ dentry *d_ext;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu;

	d_ext = debugfs_create_dir("ext_server", debugfs_sched);
	ಆದರೆ (!d_ext)
		ಹಿಂತಿರುಗಿ;

	for_each_possible_cpu(cpu) {
		ರಚನೆ dentry *d_cpu;
		ಅಕ್ಷರ buf[32];

		snprintf(buf, ಗಾತ್ರ(buf), "cpu%lu", cpu);
		d_cpu = debugfs_create_dir(buf, d_ext);

		debugfs_create_file("runtime", 0644, d_cpu, (ಶೂನ್ಯ *) cpu, &ext_server_runtime_fops);
		debugfs_create_file("period", 0644, d_cpu, (ಶೂನ್ಯ *) cpu, &ext_server_period_fops);
	}
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SCHED_CLASS_EXT */

ಸ್ಥಿರ ssize_t
sched_fair_server_period_write(ರಚನೆ file *filp, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ __user *ubuf,
			       ಗಾತ್ರ_t cnt, loff_t *ppos)
{
	ದೀರ್ಘ cpu = (ದೀರ್ಘ) ((ರಚನೆ seq_file *) filp->ಖಾಸಗಿ_ದತ್ತಾಂಶ)->ಖಾಸಗಿ;
	ರಚನೆ rq *rq = cpu_rq(cpu);

	ಹಿಂತಿರುಗಿ sched_server_write_common(filp, ubuf, cnt, ppos, DL_PERIOD,
					&rq->fair_server);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_fair_server_period_show(ರಚನೆ seq_file *m, ಶೂನ್ಯ *v)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu = (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) m->ಖಾಸಗಿ;
	ರಚನೆ rq *rq = cpu_rq(cpu);

	ಹಿಂತಿರುಗಿ sched_server_show_common(m, v, DL_PERIOD, &rq->fair_server);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_fair_server_period_open(ರಚನೆ inode *inode, ರಚನೆ file *filp)
{
	ಹಿಂತಿರುಗಿ single_open(filp, sched_fair_server_period_show, inode->i_ಖಾಸಗಿ);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ file_operations fair_server_period_fops = {
	.open		= sched_fair_server_period_open,
	.ಲಿಖ		= sched_fair_server_period_write,
	.ಪಠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ಸ್ಥಿರ ಶೂನ್ಯ debugfs_fair_server_init(ಶೂನ್ಯ)
{
	ರಚನೆ dentry *d_fair;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu;

	d_fair = debugfs_create_dir("fair_server", debugfs_sched);
	ಆದರೆ (!d_fair)
		ಹಿಂತಿರುಗಿ;

	for_each_possible_cpu(cpu) {
		ರಚನೆ dentry *d_cpu;
		ಅಕ್ಷರ buf[32];

		snprintf(buf, ಗಾತ್ರ(buf), "cpu%lu", cpu);
		d_cpu = debugfs_create_dir(buf, d_fair);

		debugfs_create_file("runtime", 0644, d_cpu, (ಶೂನ್ಯ *) cpu, &fair_server_runtime_fops);
		debugfs_create_file("period", 0644, d_cpu, (ಶೂನ್ಯ *) cpu, &fair_server_period_fops);
	}
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ = 2;

/* See __sched_cgroup_mode_update(). */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cgroup_ಬಹುಲಕ_str[] = {
	"up",
	"smp",
	"concur",
	"max",
	"tasks",
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_cgroup_ಬಹುಲಕ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str)
{
	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ARRAY_ಗಾತ್ರ(cgroup_ಬಹುಲಕ_str); i++) {
		ಆದರೆ (!strcmp(str, cgroup_ಬಹುಲಕ_str[i]))
			ಹಿಂತಿರುಗಿ i;
	}
	ಹಿಂತಿರುಗಿ -EINVAL;
}

ಸ್ಥಿರ ssize_t sched_cgroup_write(ರಚನೆ file *filp, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ __user *ubuf,
				   ಗಾತ್ರ_t cnt, loff_t *ppos)
{
	ಅಕ್ಷರ buf[16];
	ಪೂರ್ಣಾಂಕ ಬಹುಲಕ;

	ಆದರೆ (cnt > 15)
		cnt = 15;

	ಆದರೆ (copy_from_user(buf, ubuf, cnt))
		ಹಿಂತಿರುಗಿ -EFAULT;

	buf[cnt] = 0;
	ಬಹುಲಕ = sched_cgroup_ಬಹುಲಕ(strstrip(buf));
	ಆದರೆ (ಬಹುಲಕ < 0)
		ಹಿಂತಿರುಗಿ ಬಹುಲಕ;

	__sched_cgroup_mode_update(ಬಹುಲಕ);
	WRITE_ONCE(cgroup_ಬಹುಲಕ, ಬಹುಲಕ);

	*ppos += cnt;
	ಹಿಂತಿರುಗಿ cnt;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_cgroup_show(ರಚನೆ seq_file *m, ಶೂನ್ಯ *v)
{
	ಪೂರ್ಣಾಂಕ ಬಹುಲಕ = READ_ONCE(cgroup_ಬಹುಲಕ);

	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ARRAY_ಗಾತ್ರ(cgroup_ಬಹುಲಕ_str); i++) {
		ಆದರೆ (ಬಹುಲಕ == i)
			seq_puts(m, "(");
		seq_puts(m, cgroup_ಬಹುಲಕ_str[i]);
		ಆದರೆ (ಬಹುಲಕ == i)
			seq_puts(m, ")");

		seq_puts(m, " ");
	}
	seq_puts(m, "\n");
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_cgroup_open(ರಚನೆ inode *inode, ರಚನೆ file *filp)
{
	ಹಿಂತಿರುಗಿ single_open(filp, sched_cgroup_show, ಖಾಲಿ);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ file_operations sched_cgroup_fops = {
	.open		= sched_cgroup_open,
	.ಲಿಖ		= sched_cgroup_write,
	.ಪಠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ __init ಪೂರ್ಣಾಂಕ sched_init_debug(ಶೂನ್ಯ)
{
	ರಚನೆ dentry __maybe_unused *numa, *llc;

	debugfs_sched = debugfs_create_dir("sched", ಖಾಲಿ);

	debugfs_create_file("features", 0644, debugfs_sched, ಖಾಲಿ, &sched_feat_fops);
	debugfs_create_file_unsafe("verbose", 0644, debugfs_sched, &sched_debug_verbose, &sched_verbose_fops);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PREEMPT_DYNAMIC
	debugfs_create_file("preempt", 0644, debugfs_sched, ಖಾಲಿ, &sched_dynamic_fops);
#ಆದರೆ_ಅಂತ್ಯ

	debugfs_create_u32("base_slice_ns", 0644, debugfs_sched, &sysctl_sched_ತಳ_ತುಂಡು);

	debugfs_create_u32("latency_warn_ms", 0644, debugfs_sched, &sysctl_resched_latency_warn_ms);
	debugfs_create_u32("latency_warn_once", 0644, debugfs_sched, &sysctl_resched_latency_warn_once);

	debugfs_create_file("tunable_scaling", 0644, debugfs_sched, ಖಾಲಿ, &sched_scaling_fops);
	debugfs_create_u32("migration_cost_ns", 0644, debugfs_sched, &sysctl_sched_migration_ವೆಚ್ಚ);
	debugfs_create_u32("nr_migrate", 0644, debugfs_sched, &sysctl_sched_nr_migrate);

	sched_domains_mutex_ಬೀಗ();
	update_sched_domain_debugfs();
	sched_domains_mutex_unlock();

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
	numa = debugfs_create_dir("numa_balancing", debugfs_sched);

	debugfs_create_u32("scan_delay_ms", 0644, numa, &sysctl_numa_balancing_ಪರಿಶೀಲನೆ_delay);
	debugfs_create_u32("scan_period_min_ms", 0644, numa, &sysctl_numa_balancing_ಪರಿಶೀಲನೆ_period_min);
	debugfs_create_u32("scan_period_max_ms", 0644, numa, &sysctl_numa_balancing_ಪರಿಶೀಲನೆ_period_max);
	debugfs_create_u32("scan_size_mb", 0644, numa, &sysctl_numa_balancing_ಪರಿಶೀಲನೆ_ಗಾತ್ರ);
	debugfs_create_u32("hot_threshold_ms", 0644, numa, &sysctl_numa_balancing_hot_ಮಿತಿ);
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_NUMA_BALANCING */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CACHE
	llc = debugfs_create_dir("llc_balancing", debugfs_sched);
	debugfs_create_file("enabled", 0644, llc, ಖಾಲಿ,
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
#ಆದರೆ_ಅಂತ್ಯ

	debugfs_create_file("debug", 0444, debugfs_sched, ಖಾಲಿ, &sched_debug_fops);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
	debugfs_create_file("cgroup_mode", 0644, debugfs_sched, ಖಾಲಿ, &sched_cgroup_fops);
#ಆದರೆ_ಅಂತ್ಯ

	debugfs_fair_server_init();
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CLASS_EXT
	debugfs_ext_server_init();
#ಆದರೆ_ಅಂತ್ಯ

	ಹಿಂತಿರುಗಿ 0;
}
late_initcall(sched_init_debug);

ಸ್ಥಿರ cpumask_var_t		sd_sysctl_cpus;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sd_flags_show(ರಚನೆ seq_file *m, ಶೂನ್ಯ *v)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags = *(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *)m->ಖಾಸಗಿ;
	ಪೂರ್ಣಾಂಕ idx;

	for_each_ಗಣ_ಬಿಟ್(idx, &flags, __SD_FLAG_CNT) {
		seq_puts(m, sd_flag_debug[idx].name);
		seq_puts(m, " ");
	}
	seq_puts(m, "\n");

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sd_flags_open(ರಚನೆ inode *inode, ರಚನೆ file *file)
{
	ಹಿಂತಿರುಗಿ single_open(file, sd_flags_show, inode->i_ಖಾಸಗಿ);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ file_operations sd_flags_fops = {
	.open		= sd_flags_open,
	.ಪಠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ಸ್ಥಿರ ಶೂನ್ಯ ರಿಜಿಸ್ಟರ್_sd(ರಚನೆ sched_domain *sd, ರಚನೆ dentry *ಪೋಷಕ)
{
#ವ್ಯಾಖ್ಯಾನಿಸು SDM(type, ಬಹುಲಕ, member)	\
	debugfs_create_##type(#member, ಬಹುಲಕ, ಪೋಷಕ, &sd->member)

	SDM(ulong, 0644, min_interval);
	SDM(ulong, 0644, max_interval);
	SDM(u64,   0644, max_newidle_lb_ವೆಚ್ಚ);
	SDM(u32,   0644, busy_ಗುಣಕ);
	SDM(u32,   0644, imbalance_pct);
	SDM(u32,   0644, cache_nice_tries);
	SDM(str,   0444, name);

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು SDM

	debugfs_create_file("flags", 0444, ಪೋಷಕ, &sd->flags, &sd_flags_fops);
	debugfs_create_file("groups_flags", 0444, ಪೋಷಕ, &sd->groups->flags, &sd_flags_fops);
	debugfs_create_u32("level", 0444, ಪೋಷಕ, (u32 *)&sd->level);

	ಆದರೆ (sd->flags & SD_ASYM_PACKING)
		debugfs_create_u32("group_asym_prefer_cpu", 0444, ಪೋಷಕ,
				   (u32 *)&sd->groups->asym_prefer_cpu);
}

ಶೂನ್ಯ update_sched_domain_debugfs(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ cpu, i;

	/*
	 * This can unfortunately be invoked before sched_debug_init() creates
	 * the debug directory. Don't touch sd_sysctl_cpus until then.
	 */
	ಆದರೆ (!debugfs_sched)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!sched_debug_verbose)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!cpumask_ಲಭ್ಯ(sd_sysctl_cpus)) {
		ಆದರೆ (!alloc_cpumask_var(&sd_sysctl_cpus, GFP_KERNEL))
			ಹಿಂತಿರುಗಿ;
		cpumask_copy(sd_sysctl_cpus, cpu_possible_ಮುಖವಾಡ);
	}

	ಆದರೆ (!sd_dentry) {
		sd_dentry = debugfs_create_dir("domains", debugfs_sched);

		/* rebuild sd_sysctl_cpus if empty since it gets cleared below */
		ಆದರೆ (cpumask_ಖಾಲಿಗಳು(sd_sysctl_cpus))
			cpumask_copy(sd_sysctl_cpus, cpu_online_ಮುಖವಾಡ);
	}

	for_each_cpu(cpu, sd_sysctl_cpus) {
		ರಚನೆ sched_domain *sd;
		ರಚನೆ dentry *d_cpu;
		ಅಕ್ಷರ buf[32];

		snprintf(buf, ಗಾತ್ರ(buf), "cpu%d", cpu);
		debugfs_lookup_and_remove(buf, sd_dentry);
		d_cpu = debugfs_create_dir(buf, sd_dentry);

		i = 0;
		for_each_domain(cpu, sd) {
			ರಚನೆ dentry *d_sd;

			snprintf(buf, ಗಾತ್ರ(buf), "domain%d", i);
			d_sd = debugfs_create_dir(buf, d_cpu);

			ರಿಜಿಸ್ಟರ್_sd(sd, d_sd);
			i++;
		}

		__cpumask_clear_cpu(cpu, sd_sysctl_cpus);
	}
}

ಶೂನ್ಯ dirty_sched_domain_sysctl(ಪೂರ್ಣಾಂಕ cpu)
{
	ಆದರೆ (cpumask_ಲಭ್ಯ(sd_sysctl_cpus))
		__cpumask_set_cpu(cpu, sd_sysctl_cpus);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
ಸ್ಥಿರ ಶೂನ್ಯ print_cfs_ಗುಂಪು_stats(ರಚನೆ seq_file *m, ಪೂರ್ಣಾಂಕ cpu, ರಚನೆ task_ಗುಂಪು *tg)
{
	ರಚನೆ sched_entity *se = tg_se(tg, cpu);

#ವ್ಯಾಖ್ಯಾನಿಸು P(F)		SEQ_printf(m, "  .%-30s: %lld\n",	#F, (ದೀರ್ಘ ದೀರ್ಘ)F)
#ವ್ಯಾಖ್ಯಾನಿಸು P_SCHEDSTAT(F)	SEQ_printf(m, "  .%-30s: %lld\n",	\
		#F, (ದೀರ್ಘ ದೀರ್ಘ)schedstat_val(stats->F))
#ವ್ಯಾಖ್ಯಾನಿಸು PN(F)		SEQ_printf(m, "  .%-30s: %lld.%06ld\n", #F, ವಿಭಜಿಸು_NS((ದೀರ್ಘ ದೀರ್ಘ)F))
#ವ್ಯಾಖ್ಯಾನಿಸು PN_SCHEDSTAT(F)	SEQ_printf(m, "  .%-30s: %lld.%06ld\n", \
		#F, ವಿಭಜಿಸು_NS((ದೀರ್ಘ ದೀರ್ಘ)schedstat_val(stats->F)))

	ಆದರೆ (!se)
		ಹಿಂತಿರುಗಿ;

	PN(se->exec_ಆರಂಭ);
	PN(se->vruntime);
	PN(se->ಮೊತ್ತ_exec_runtime);

	ಆದರೆ (schedstat_enabled()) {
		ರಚನೆ sched_statistics *stats;
		stats = __schedstats_from_se(se);

		PN_SCHEDSTAT(wait_ಆರಂಭ);
		PN_SCHEDSTAT(sleep_ಆರಂಭ);
		PN_SCHEDSTAT(ಖಂಡ_ಆರಂಭ);
		PN_SCHEDSTAT(sleep_max);
		PN_SCHEDSTAT(ಖಂಡ_max);
		PN_SCHEDSTAT(exec_max);
		PN_SCHEDSTAT(ತುಂಡು_max);
		PN_SCHEDSTAT(wait_max);
		PN_SCHEDSTAT(wait_ಮೊತ್ತ);
		P_SCHEDSTAT(wait_ಎಣಿಕೆ);
	}

	P(se->ತುಂಬು.ತೂಕ);
	P(se->avg.ತುಂಬು_avg);
	P(se->avg.util_avg);
	P(se->avg.runnable_avg);

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು PN_SCHEDSTAT
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು PN
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು P_SCHEDSTAT
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು P
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FAIR_GROUP_SCHED */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CGROUP_SCHED
ಸ್ಥಿರ DEFINE_SPINLOCK(sched_debug_ಬೀಗ);
ಸ್ಥಿರ ಅಕ್ಷರ ಗುಂಪು_ಮಾರ್ಗ[ಮಾರ್ಗ_MAX];

ಸ್ಥಿರ ಶೂನ್ಯ task_ಗುಂಪು_ಮಾರ್ಗ(ರಚನೆ task_ಗುಂಪು *tg, ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ plen)
{
	ಆದರೆ (autogroup_ಮಾರ್ಗ(tg, ಮಾರ್ಗ, plen))
		ಹಿಂತಿರುಗಿ;

	cgroup_ಮಾರ್ಗ(tg->css.cgroup, ಮಾರ್ಗ, plen);
}

/*
 * Only 1 SEQ_printf_task_group_path() caller can use the full length
 * group_path[] for cgroup path. Other simultaneous callers will have
 * to use a shorter stack buffer. A "..." suffix is appended at the end
 * of the stack buffer so that it will show up in case the output length
 * matches the given buffer size to indicate possible path name truncation.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು SEQ_printf_task_ಗುಂಪು_ಮಾರ್ಗ(m, tg, fmt...)			\
{									\
	ಆದರೆ (spin_trylock(&sched_debug_ಬೀಗ)) {				\
		task_ಗುಂಪು_ಮಾರ್ಗ(tg, ಗುಂಪು_ಮಾರ್ಗ, ಗಾತ್ರ(ಗುಂಪು_ಮಾರ್ಗ));	\
		SEQ_printf(m, fmt, ಗುಂಪು_ಮಾರ್ಗ);				\
		spin_unlock(&sched_debug_ಬೀಗ);				\
	} ಇಲ್ಲದಿದ್ದರೆ {							\
		ಅಕ್ಷರ buf[128];						\
		ಅಕ್ಷರ *bufend = buf + ಗಾತ್ರ(buf) - 3;			\
		task_ಗುಂಪು_ಮಾರ್ಗ(tg, buf, bufend - buf);			\
		strcpy(bufend - 1, "...");				\
		SEQ_printf(m, fmt, buf);				\
	}								\
}
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಶೂನ್ಯ
print_task(ರಚನೆ seq_file *m, ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಆದರೆ (task_current(rq, p))
		SEQ_printf(m, ">R");
	ಇಲ್ಲದಿದ್ದರೆ
		SEQ_printf(m, " %c", task_state_ಗಮ್ಯ_char(p));

	SEQ_printf(m, " %15s %5d %10ld %9Ld.%06ld   %c   %9Ld.%06ld %c %9Ld.%06ld %9Ld.%06ld %9Ld   %5d ",
		p->comm, task_pid_nr(p),
		p->se.h_ತುಂಬು.ತೂಕ,
		ವಿಭಜಿಸು_NS(p->se.vruntime),
		entity_eligible(&rq->cfs, &p->se) ? 'E' : 'N',
		ವಿಭಜಿಸು_NS(p->se.deadline),
		p->se.custom_ತುಂಡು ? 'S' : ' ',
		ವಿಭಜಿಸು_NS(p->se.ತುಂಡು),
		ವಿಭಜಿಸು_NS(p->se.ಮೊತ್ತ_exec_runtime),
		(ದೀರ್ಘ ದೀರ್ಘ)(p->nvcsw + p->nivcsw),
		p->prio);

	SEQ_printf(m, "%9lld.%06ld %9lld.%06ld %9lld.%06ld",
		ವಿಭಜಿಸು_NS(schedstat_val_or_zero(p->stats.wait_ಮೊತ್ತ)),
		ವಿಭಜಿಸು_NS(schedstat_val_or_zero(p->stats.ಮೊತ್ತ_sleep_runtime)),
		ವಿಭಜಿಸು_NS(schedstat_val_or_zero(p->stats.ಮೊತ್ತ_ಖಂಡ_runtime)));

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
	SEQ_printf(m, "   %d      %d", task_ಗಂಟು(p), task_numa_ಗುಂಪು_id(p));
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CGROUP_SCHED
	SEQ_printf_task_ಗುಂಪು_ಮಾರ್ಗ(m, task_ಗುಂಪು(p), "        %s")
#ಆದರೆ_ಅಂತ್ಯ

	SEQ_printf(m, "\n");
}

ಸ್ಥಿರ ಶೂನ್ಯ print_rq(ರಚನೆ seq_file *m, ರಚನೆ rq *rq, ಪೂರ್ಣಾಂಕ rq_cpu)
{
	ರಚನೆ task_struct *g, *p;

	SEQ_printf(m, "\n");
	SEQ_printf(m, "runnable tasks:\n");
	SEQ_printf(m, " S            task   PID     weight       vruntime   eligible    "
		   "deadline             slice          sum-exec      switches  "
		   "prio         wait-time        sum-sleep       sum-block"
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
		   "  node   group-id"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CGROUP_SCHED
		   "  group-path"
#ಆದರೆ_ಅಂತ್ಯ
		   "\n");
	SEQ_printf(m, "-------------------------------------------------------"
		   "------------------------------------------------------"
		   "------------------------------------------------------"
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
		   "--------------"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CGROUP_SCHED
		   "--------------"
#ಆದರೆ_ಅಂತ್ಯ
		   "\n");

	rcu_read_ಬೀಗ();
	for_each_process_thread(g, p) {
		ಆದರೆ (task_cpu(p) != rq_cpu)
			ಮುಂದುವರಿ;

		print_task(m, rq, p);
	}
	rcu_read_unlock();
}

ಶೂನ್ಯ print_cfs_rq(ರಚನೆ seq_file *m, ಪೂರ್ಣಾಂಕ cpu, ರಚನೆ cfs_rq *cfs_rq)
{
	s64 ಎಡ_vruntime = -1, ಬಲ_vruntime = -1, ಎಡ_deadline = -1, ಹರಡುವಿಕೆ;
	s64 zero_vruntime = -1, ಮೊತ್ತ_w_vruntime = -1;
	u64 avruntime;
	ರಚನೆ sched_entity *ಕೊನೆಯ, *ಮೊದಲ, *ಬೇರು;
	ರಚನೆ rq *rq = cpu_rq(cpu);
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮೊತ್ತ_ಸರಿಸು;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;
	u64 ಮೊತ್ತ_ತೂಕ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
	SEQ_printf(m, "\n");
	SEQ_printf_task_ಗುಂಪು_ಮಾರ್ಗ(m, cfs_rq->tg, "cfs_rq[%d]:%s\n", cpu);
#ಇಲ್ಲದಿದ್ದರೆ
	SEQ_printf(m, "\n");
	SEQ_printf(m, "cfs_rq[%d]:\n", cpu);
#ಆದರೆ_ಅಂತ್ಯ

	raw_spin_rq_ಬೀಗ_irqsave(rq, flags);
	ಬೇರು = __pick_root_entity(cfs_rq);
	ಆದರೆ (ಬೇರು)
		ಎಡ_vruntime = ಬೇರು->min_vruntime;
	ಮೊದಲ = __pick_first_entity(cfs_rq);
	ಆದರೆ (ಮೊದಲ)
		ಎಡ_deadline = ಮೊದಲ->deadline;
	ಕೊನೆಯ = __pick_last_entity(cfs_rq);
	ಆದರೆ (ಕೊನೆಯ)
		ಬಲ_vruntime = ಕೊನೆಯ->vruntime;
	zero_vruntime = cfs_rq->zero_vruntime;
	ಮೊತ್ತ_w_vruntime = cfs_rq->ಮೊತ್ತ_w_vruntime;
	ಮೊತ್ತ_ತೂಕ = cfs_rq->ಮೊತ್ತ_ತೂಕ;
	ಮೊತ್ತ_ಸರಿಸು = cfs_rq->ಮೊತ್ತ_ಸರಿಸು;
	avruntime = avg_vruntime(cfs_rq);
	raw_spin_rq_unlock_irqrestore(rq, flags);

	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "left_deadline",
			ವಿಭಜಿಸು_NS(ಎಡ_deadline));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "left_vruntime",
			ವಿಭಜಿಸು_NS(ಎಡ_vruntime));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "zero_vruntime",
			ವಿಭಜಿಸು_NS(zero_vruntime));
	SEQ_printf(m, "  .%-30s: %Ld (%d bits)\n", "sum_w_vruntime",
		   ಮೊತ್ತ_w_vruntime, ilog2(ನಿರಪೇಕ್ಷ(ಮೊತ್ತ_w_vruntime)));
	SEQ_printf(m, "  .%-30s: %Lu\n", "sum_weight",
		   ಮೊತ್ತ_ತೂಕ);
	SEQ_printf(m, "  .%-30s: %u\n", "sum_shift", ಮೊತ್ತ_ಸರಿಸು);
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "avg_vruntime",
			ವಿಭಜಿಸು_NS(avruntime));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "right_vruntime",
			ವಿಭಜಿಸು_NS(ಬಲ_vruntime));
	ಹರಡುವಿಕೆ = ಬಲ_vruntime - ಎಡ_vruntime;
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "spread", ವಿಭಜಿಸು_NS(ಹರಡುವಿಕೆ));
	SEQ_printf(m, "  .%-30s: %d\n", "nr_queued", cfs_rq->nr_queued);
	SEQ_printf(m, "  .%-30s: %d\n", "h_nr_runnable", cfs_rq->h_nr_runnable);
	SEQ_printf(m, "  .%-30s: %d\n", "h_nr_queued", cfs_rq->h_nr_queued);
	SEQ_printf(m, "  .%-30s: %d\n", "h_nr_idle", cfs_rq->h_nr_idle);
	SEQ_printf(m, "  .%-30s: %ld\n", "load", cfs_rq->ತುಂಬು.ತೂಕ);
	SEQ_printf(m, "  .%-30s: %lu\n", "load_avg",
			cfs_rq->avg.ತುಂಬು_avg);
	SEQ_printf(m, "  .%-30s: %lu\n", "runnable_avg",
			cfs_rq->avg.runnable_avg);
	SEQ_printf(m, "  .%-30s: %lu\n", "util_avg",
			cfs_rq->avg.util_avg);
	SEQ_printf(m, "  .%-30s: %u\n", "util_est",
			cfs_rq->avg.util_est);
	SEQ_printf(m, "  .%-30s: %ld\n", "removed.load_avg",
			cfs_rq->removed.ತುಂಬು_avg);
	SEQ_printf(m, "  .%-30s: %ld\n", "removed.util_avg",
			cfs_rq->removed.util_avg);
	SEQ_printf(m, "  .%-30s: %ld\n", "removed.runnable_avg",
			cfs_rq->removed.runnable_avg);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
	SEQ_printf(m, "  .%-30s: %lu\n", "tg_load_avg_contrib",
			cfs_rq->tg_ತುಂಬು_avg_contrib);
	SEQ_printf(m, "  .%-30s: %ld\n", "tg_load_avg",
			atomic_long_read(&cfs_rq->tg->ತುಂಬು_avg));
	SEQ_printf(m, "  .%-30s: %lu\n", "h_load",
			cfs_rq->h_ತುಂಬು);
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FAIR_GROUP_SCHED */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CFS_BANDWIDTH
	SEQ_printf(m, "  .%-30s: %d\n", "throttled",
			cfs_rq->throttled);
	SEQ_printf(m, "  .%-30s: %d\n", "throttle_count",
			cfs_rq->throttle_ಎಣಿಕೆ);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
	print_cfs_ಗುಂಪು_stats(m, cpu, cfs_rq->tg);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ print_rt_rq(ರಚನೆ seq_file *m, ಪೂರ್ಣಾಂಕ cpu, ರಚನೆ rt_rq *rt_rq)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED
	SEQ_printf(m, "\n");
	SEQ_printf_task_ಗುಂಪು_ಮಾರ್ಗ(m, rt_rq->tg, "rt_rq[%d]:%s\n", cpu);
#ಇಲ್ಲದಿದ್ದರೆ
	SEQ_printf(m, "\n");
	SEQ_printf(m, "rt_rq[%d]:\n", cpu);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು P(x) \
	SEQ_printf(m, "  .%-30s: %Ld\n", #x, (ದೀರ್ಘ ದೀರ್ಘ)(rt_rq->x))
#ವ್ಯಾಖ್ಯಾನಿಸು PU(x) \
	SEQ_printf(m, "  .%-30s: %lu\n", #x, (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)(rt_rq->x))
#ವ್ಯಾಖ್ಯಾನಿಸು PN(x) \
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", #x, ವಿಭಜಿಸು_NS(rt_rq->x))

	PU(rt_nr_running);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED
	P(rt_throttled);
	PN(rt_ಸಮಯ);
	PN(rt_runtime);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು PN
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು PU
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು P
}

ಶೂನ್ಯ print_dl_rq(ರಚನೆ seq_file *m, ಪೂರ್ಣಾಂಕ cpu, ರಚನೆ dl_rq *dl_rq)
{
	ರಚನೆ dl_bw *dl_bw;

	SEQ_printf(m, "\n");
	SEQ_printf(m, "dl_rq[%d]:\n", cpu);

#ವ್ಯಾಖ್ಯಾನಿಸು PU(x) \
	SEQ_printf(m, "  .%-30s: %lu\n", #x, (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)(dl_rq->x))

	PU(dl_nr_running);
	dl_bw = &cpu_rq(cpu)->rd->dl_bw;
	SEQ_printf(m, "  .%-30s: %lld\n", "dl_bw->bw", dl_bw->bw);
	SEQ_printf(m, "  .%-30s: %lld\n", "dl_bw->total_bw", dl_bw->ಒಟ್ಟು_bw);

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು PU
}

ಸ್ಥಿರ ಶೂನ್ಯ print_cpu(ರಚನೆ seq_file *m, ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_X86
	{
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ freq = cpu_khz ? : 1;

		SEQ_printf(m, "cpu#%d, %u.%03u MHz\n",
			   cpu, freq / 1000, (freq % 1000));
	}
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_X86: */
	SEQ_printf(m, "cpu#%d\n", cpu);
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_X86 */

#ವ್ಯಾಖ್ಯಾನಿಸು P(x)								\
ಮಾಡು {									\
	ಆದರೆ (ಗಾತ್ರ(rq->x) == 4)						\
		SEQ_printf(m, "  .%-30s: %d\n", #x, (ಪೂರ್ಣಾಂಕ)(rq->x));	\
	ಇಲ್ಲದಿದ್ದರೆ								\
		SEQ_printf(m, "  .%-30s: %Ld\n", #x, (ದೀರ್ಘ ದೀರ್ಘ)(rq->x));\
} ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು PN(x) \
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", #x, ವಿಭಜಿಸು_NS(rq->x))

	P(nr_running);
	P(nr_switches);
	P(nr_uninterruptible);
	PN(ಮುಂದಿನ_balance);
	SEQ_printf(m, "  .%-30s: %ld\n", "curr->pid", (ದೀರ್ಘ)(task_pid_nr(rq->curr)));
	PN(clock);
	PN(clock_task);
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು P
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು PN

#ವ್ಯಾಖ್ಯಾನಿಸು P64(n) SEQ_printf(m, "  .%-30s: %Ld\n", #n, rq->n);
	P64(avg_idle);
	P64(max_idle_balance_ವೆಚ್ಚ);
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು P64

#ವ್ಯಾಖ್ಯಾನಿಸು P(n) SEQ_printf(m, "  .%-30s: %d\n", #n, schedstat_val(rq->n));
	ಆದರೆ (schedstat_enabled()) {
		P(yld_ಎಣಿಕೆ);
		P(sched_ಎಣಿಕೆ);
		P(sched_goidle);
		P(ttwu_ಎಣಿಕೆ);
		P(ttwu_local);
	}
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು P

	print_cfs_stats(m, cpu);
	print_rt_stats(m, cpu);
	print_dl_stats(m, cpu);

	print_rq(m, rq, cpu);
	SEQ_printf(m, "\n");
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sched_tunable_scaling_ಹೆಸರುಗಳು[] = {
	"none",
	"logarithmic",
	"linear"
};

ಸ್ಥಿರ ಶೂನ್ಯ sched_debug_ಶೀರ್ಷಿಕೆ(ರಚನೆ seq_file *m)
{
	u64 ktime, sched_clk, cpu_clk;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	local_irq_save(flags);
	ktime = ktime_ಗಮ್ಯ_ns(ktime_get());
	sched_clk = sched_clock();
	cpu_clk = local_clock();
	local_irq_restore(flags);

	SEQ_printf(m, "Sched Debug Version: v0.11, %s %.*s\n",
		init_utsname()->release,
		(ಪೂರ್ಣಾಂಕ)strcspn(init_utsname()->ಆವೃತ್ತಿ, " "),
		init_utsname()->ಆವೃತ್ತಿ);

#ವ್ಯಾಖ್ಯಾನಿಸು P(x) \
	SEQ_printf(m, "%-40s: %Ld\n", #x, (ದೀರ್ಘ ದೀರ್ಘ)(x))
#ವ್ಯಾಖ್ಯಾನಿಸು PN(x) \
	SEQ_printf(m, "%-40s: %Ld.%06ld\n", #x, ವಿಭಜಿಸು_NS(x))
	PN(ktime);
	PN(sched_clk);
	PN(cpu_clk);
	P(jiffies);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_HAVE_UNSTABLE_SCHED_CLOCK
	P(sched_clock_stable());
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು PN
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು P

	SEQ_printf(m, "\n");
	SEQ_printf(m, "sysctl_sched\n");

#ವ್ಯಾಖ್ಯಾನಿಸು P(x) \
	SEQ_printf(m, "  .%-40s: %Ld\n", #x, (ದೀರ್ಘ ದೀರ್ಘ)(x))
#ವ್ಯಾಖ್ಯಾನಿಸು PN(x) \
	SEQ_printf(m, "  .%-40s: %Ld.%06ld\n", #x, ವಿಭಜಿಸು_NS(x))
	PN(sysctl_sched_ತಳ_ತುಂಡು);
	P(sysctl_sched_features);
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು PN
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು P

	SEQ_printf(m, "  .%-40s: %d (%s)\n",
		"sysctl_sched_tunable_scaling",
		sysctl_sched_tunable_scaling,
		sched_tunable_scaling_ಹೆಸರುಗಳು[sysctl_sched_tunable_scaling]);
	SEQ_printf(m, "\n");
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_debug_show(ರಚನೆ seq_file *m, ಶೂನ್ಯ *v)
{
	ಪೂರ್ಣಾಂಕ cpu = (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)(v - 2);

	ಆದರೆ (cpu != -1)
		print_cpu(m, cpu);
	ಇಲ್ಲದಿದ್ದರೆ
		sched_debug_ಶೀರ್ಷಿಕೆ(m);

	ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ sysrq_sched_debug_show(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ cpu;

	sched_debug_ಶೀರ್ಷಿಕೆ(ಖಾಲಿ);
	for_each_online_cpu(cpu) {
		/*
		 * Need to reset softlockup watchdogs on all CPUs, because
		 * another CPU might be blocked waiting for us to process
		 * an IPI or stop_machine.
		 */
		touch_nmi_watchdog();
		touch_all_softlockup_watchdogs();
		print_cpu(ಖಾಲಿ, cpu);
	}
}

/*
 * This iterator needs some explanation.
 * It returns 1 for the header position.
 * This means 2 is CPU 0.
 * In a hotplugged system some CPUs, including CPU 0, may be missing so we have
 * to use cpumask_* to iterate over the CPUs.
 */
ಸ್ಥಿರ ಶೂನ್ಯ *sched_debug_ಆರಂಭ(ರಚನೆ seq_file *file, loff_t *ಸ್ಥಾನಾಂತರ)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ n = *ಸ್ಥಾನಾಂತರ;

	ಆದರೆ (n == 0)
		ಹಿಂತಿರುಗಿ (ಶೂನ್ಯ *) 1;

	n--;

	ಆದರೆ (n > 0)
		n = cpumask_ಮುಂದಿನ(n - 1, cpu_online_ಮುಖವಾಡ);
	ಇಲ್ಲದಿದ್ದರೆ
		n = cpumask_ಮೊದಲ(cpu_online_ಮುಖವಾಡ);

	*ಸ್ಥಾನಾಂತರ = n + 1;

	ಆದರೆ (n < nr_cpu_ids)
		ಹಿಂತಿರುಗಿ (ಶೂನ್ಯ *)(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)(n + 2);

	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಶೂನ್ಯ *sched_debug_ಮುಂದಿನ(ರಚನೆ seq_file *file, ಶೂನ್ಯ *ದತ್ತಾಂಶ, loff_t *ಸ್ಥಾನಾಂತರ)
{
	(*ಸ್ಥಾನಾಂತರ)++;
	ಹಿಂತಿರುಗಿ sched_debug_ಆರಂಭ(file, ಸ್ಥಾನಾಂತರ);
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_debug_stop(ರಚನೆ seq_file *file, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ seq_operations sched_debug_sops = {
	.ಆರಂಭ		= sched_debug_ಆರಂಭ,
	.ಮುಂದಿನ		= sched_debug_ಮುಂದಿನ,
	.stop		= sched_debug_stop,
	.show		= sched_debug_show,
};

#ವ್ಯಾಖ್ಯಾನಿಸು __PS(S, F) SEQ_printf(m, "%-45s:%21Ld\n", S, (ದೀರ್ಘ ದೀರ್ಘ)(F))
#ವ್ಯಾಖ್ಯಾನಿಸು __P(F) __PS(#F, F)
#ವ್ಯಾಖ್ಯಾನಿಸು   P(F) __PS(#F, p->F)
#ವ್ಯಾಖ್ಯಾನಿಸು   PM(F, M) __PS(#F, p->F & (M))
#ವ್ಯಾಖ್ಯಾನಿಸು __PSN(S, F) SEQ_printf(m, "%-45s:%14Ld.%06ld\n", S, ವಿಭಜಿಸು_NS((ದೀರ್ಘ ದೀರ್ಘ)(F)))
#ವ್ಯಾಖ್ಯಾನಿಸು __PN(F) __PSN(#F, F)
#ವ್ಯಾಖ್ಯಾನಿಸು   PN(F) __PSN(#F, p->F)


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
ಶೂನ್ಯ print_numa_stats(ರಚನೆ seq_file *m, ಪೂರ್ಣಾಂಕ ಗಂಟು, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ tsf,
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ tpf, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ gsf, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ gpf)
{
	SEQ_printf(m, "numa_faults node=%d ", ಗಂಟು);
	SEQ_printf(m, "task_private=%lu task_shared=%lu ", tpf, tsf);
	SEQ_printf(m, "group_private=%lu group_shared=%lu\n", gpf, gsf);
}
#ಆದರೆ_ಅಂತ್ಯ


ಸ್ಥಿರ ಶೂನ್ಯ sched_show_numa(ರಚನೆ task_struct *p, ರಚನೆ seq_file *m)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
	ಆದರೆ (p->mm)
		P(mm->numa_ಪರಿಶೀಲನೆ_seq);

	P(numa_ಪುಟಗಳು_migrated);
	P(numa_preferred_nid);
	P(ಒಟ್ಟು_numa_ದೋಷಗಳು);
	SEQ_printf(m, "current_node=%d, numa_group_id=%d\n",
			task_ಗಂಟು(p), task_numa_ಗುಂಪು_id(p));
	show_numa_stats(p, m);
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_NUMA_BALANCING */
}

ಶೂನ್ಯ proc_sched_show_task(ರಚನೆ task_struct *p, ರಚನೆ pid_namespace *ns,
						  ರಚನೆ seq_file *m)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ nr_switches;

	SEQ_printf(m, "%s (%d, #threads: %d)\n", p->comm, task_pid_nr_ns(p, ns),
						get_nr_ಎಳೆಗಳು(p));
	SEQ_printf(m,
		"---------------------------------------------------------"
		"----------\n");

#ವ್ಯಾಖ್ಯಾನಿಸು P_SCHEDSTAT(F)  __PS(#F, schedstat_val(p->stats.F))
#ವ್ಯಾಖ್ಯಾನಿಸು PN_SCHEDSTAT(F) __PSN(#F, schedstat_val(p->stats.F))

	PN(se.exec_ಆರಂಭ);
	PN(se.vruntime);
	PN(se.ಮೊತ್ತ_exec_runtime);

	nr_switches = p->nvcsw + p->nivcsw;

	P(se.nr_migrations);

	ಆದರೆ (schedstat_enabled()) {
		u64 avg_atom, avg_ಪ್ರತಿ_cpu;

		PN_SCHEDSTAT(ಮೊತ್ತ_sleep_runtime);
		PN_SCHEDSTAT(ಮೊತ್ತ_ಖಂಡ_runtime);
		PN_SCHEDSTAT(wait_ಆರಂಭ);
		PN_SCHEDSTAT(sleep_ಆರಂಭ);
		PN_SCHEDSTAT(ಖಂಡ_ಆರಂಭ);
		PN_SCHEDSTAT(sleep_max);
		PN_SCHEDSTAT(ಖಂಡ_max);
		PN_SCHEDSTAT(exec_max);
		PN_SCHEDSTAT(ತುಂಡು_max);
		PN_SCHEDSTAT(wait_max);
		PN_SCHEDSTAT(wait_ಮೊತ್ತ);
		P_SCHEDSTAT(wait_ಎಣಿಕೆ);
		PN_SCHEDSTAT(iowait_ಮೊತ್ತ);
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

		avg_atom = p->se.ಮೊತ್ತ_exec_runtime;
		ಆದರೆ (nr_switches)
			avg_atom = div64_ul(avg_atom, nr_switches);
		ಇಲ್ಲದಿದ್ದರೆ
			avg_atom = -1LL;

		avg_ಪ್ರತಿ_cpu = p->se.ಮೊತ್ತ_exec_runtime;
		ಆದರೆ (p->se.nr_migrations) {
			avg_ಪ್ರತಿ_cpu = div64_u64(avg_ಪ್ರತಿ_cpu,
						p->se.nr_migrations);
		} ಇಲ್ಲದಿದ್ದರೆ {
			avg_ಪ್ರತಿ_cpu = -1LL;
		}

		__PN(avg_atom);
		__PN(avg_ಪ್ರತಿ_cpu);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CORE
		PN_SCHEDSTAT(core_forceidle_ಮೊತ್ತ);
#ಆದರೆ_ಅಂತ್ಯ
	}

	__P(nr_switches);
	__PS("nr_voluntary_switches", p->nvcsw);
	__PS("nr_involuntary_switches", p->nivcsw);

	P(se.ತುಂಬು.ತೂಕ);
	P(se.avg.ತುಂಬು_ಮೊತ್ತ);
	P(se.avg.runnable_ಮೊತ್ತ);
	P(se.avg.util_ಮೊತ್ತ);
	P(se.avg.ತುಂಬು_avg);
	P(se.avg.runnable_avg);
	P(se.avg.util_avg);
	P(se.avg.ಕೊನೆಯ_update_ಸಮಯ);
	PM(se.avg.util_est, ~UTIL_AVG_UNCHANGED);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK
	__PS("uclamp.min", p->uclamp_req[UCLAMP_MIN].ಮೌಲ್ಯ);
	__PS("uclamp.max", p->uclamp_req[UCLAMP_MAX].ಮೌಲ್ಯ);
	__PS("effective uclamp.min", uclamp_eff_ಮೌಲ್ಯ(p, UCLAMP_MIN));
	__PS("effective uclamp.max", uclamp_eff_ಮೌಲ್ಯ(p, UCLAMP_MAX));
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_UCLAMP_TASK */
	P(policy);
	P(prio);
	ಆದರೆ (task_has_dl_policy(p)) {
		P(dl.runtime);
		P(dl.deadline);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fair_policy(p->policy)) {
		P(se.ತುಂಡು);
	}
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CLASS_EXT
	__PS("ext.enabled", task_on_scx(p));
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು PN_SCHEDSTAT
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು P_SCHEDSTAT

	{
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ this_cpu = raw_smp_processor_id();
		u64 t0, t1;

		t0 = cpu_clock(this_cpu);
		t1 = cpu_clock(this_cpu);
		__PS("clock-delta", t1-t0);
	}

	sched_show_numa(p, m);
}

ಶೂನ್ಯ proc_sched_ಗಣ_task(ರಚನೆ task_struct *p)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHEDSTATS
	memset(&p->stats, 0, ಗಾತ್ರ(p->stats));
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ resched_latency_warn(ಪೂರ್ಣಾಂಕ cpu, u64 latency)
{
	ಸ್ಥಿರ DEFINE_RATELIMIT_STATE(latency_check_ratelimit, 60 * 60 * HZ, 1);

	ಆದರೆ (likely(!__ratelimit(&latency_check_ratelimit)))
		ಹಿಂತಿರುಗಿ;

	pr_err("sched: CPU %d need_resched set for > %llu ns (%d ticks) without schedule\n",
	       cpu, latency, cpu_rq(cpu)->ticks_without_resched);
	dump_ರಾಶಿ();
}
