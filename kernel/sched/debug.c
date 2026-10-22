// SPDX-License-Identifier: GPL-2.0-only
/*
 * kernel/sched/debug.c
 *
 * Print the CFS rbtree and other debugging details
 *
 * Copyright(C) 2007, Red Hat, Inc., Ingo Molnar
 */
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/debugfs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/nmi.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/log2.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"

/*
 * This allows printing both to /sys/kernel/debug/sched/debug and
 * to the console
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SEQ_printf(m, x...)			\
 ਕਰੋ {						\
	ਜੇ (m)					\
		seq_printf(m, x);		\
	ਨਹੀਂਤਾਂ					\
		pr_cont(x);			\
 } ਜਦੋਂ (0)

/*
 * Ease the printing of nsec fields:
 */
ਸਥਿਰ ਲੰਮਾ ਲੰਮਾ nsec_ਉੱਚਾ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ nsec)
{
	ਜੇ ((ਲੰਮਾ ਲੰਮਾ)nsec < 0) {
		nsec = -nsec;
		do_div(nsec, 1000000);
		ਵਾਪਸ -nsec;
	}
	do_div(nsec, 1000000);

	ਵਾਪਸ nsec;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ nsec_ਨੀਵਾਂ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ nsec)
{
	ਜੇ ((ਲੰਮਾ ਲੰਮਾ)nsec < 0)
		nsec = -nsec;

	ਵਾਪਸ do_div(nsec, 1000000);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਵੰਡੋ_NS(x) nsec_ਉੱਚਾ(x), nsec_ਨੀਵਾਂ(x)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCHED_FEAT(name, enabled)	\
	#name ,

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ sched_feat_ਨਾਂ[] = {
#ਅਨ੍ਤਰ੍ਭਾਵ "features.h"
};

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ SCHED_FEAT

ਸਥਿਰ ਪੂਰਨ sched_feat_show(ਬਣਤਰ seq_file *m, ਖਾਲੀ *v)
{
	ਪੂਰਨ i;

	ਲਈ (i = 0; i < __SCHED_FEAT_NR; i++) {
		ਜੇ (!(sysctl_sched_features & (1UL << i)))
			seq_puts(m, "NO_");
		seq_printf(m, "%s ", sched_feat_ਨਾਂ[i]);
	}
	seq_puts(m, "\n");

	ਵਾਪਸ 0;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_JUMP_LABEL

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ jump_label_ਕੁੰਜੀ_true  { .ਕੁੰਜੀ_true  = STATIC_ਕੁੰਜੀ_TRUE_INIT  }
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ jump_label_ਕੁੰਜੀ_false { .ਕੁੰਜੀ_false = STATIC_ਕੁੰਜੀ_FALSE_INIT }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCHED_FEAT(name, enabled)	\
	jump_label_ਕੁੰਜੀ##enabled ,

ਸੰਘ sched_feat_ਕੁੰਜੀ sched_feat_ਕੁੰਜੀਆਂ[__SCHED_FEAT_NR] = {
#ਅਨ੍ਤਰ੍ਭਾਵ "features.h"
};

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ SCHED_FEAT

ਸਥਿਰ ਖਾਲੀ sched_feat_disable(ਪੂਰਨ i)
{
	static_branch_disable_cpuslocked(&sched_feat_ਕੁੰਜੀਆਂ[i].ਕੁੰਜੀ_true);
}

ਸਥਿਰ ਖਾਲੀ sched_feat_enable(ਪੂਰਨ i)
{
	static_branch_enable_cpuslocked(&sched_feat_ਕੁੰਜੀਆਂ[i].ਕੁੰਜੀ_false);
}
#ਨਹੀਂਤਾਂ /* !CONFIG_JUMP_LABEL: */
ਸਥਿਰ ਖਾਲੀ sched_feat_disable(ਪੂਰਨ i) { };
ਸਥਿਰ ਖਾਲੀ sched_feat_enable(ਪੂਰਨ i) { };
#ਜੇ_ਅੰਤ /* !CONFIG_JUMP_LABEL */

ਸਥਿਰ ਪੂਰਨ sched_feat_ਸੈੱਟ(ਅੱਖਰ *cmp)
{
	ਪੂਰਨ i;
	ਪੂਰਨ neg = 0;

	ਜੇ (ਲੜੀ_ਤੁਲਨਾ(cmp, "NO_", 3) == 0) {
		neg = 1;
		cmp += 3;
	}

	i = ਮੇਲ_string(sched_feat_ਨਾਂ, __SCHED_FEAT_NR, cmp);
	ਜੇ (i < 0)
		ਵਾਪਸ i;

	ਜੇ (neg) {
		sysctl_sched_features &= ~(1UL << i);
		sched_feat_disable(i);
	} ਨਹੀਂਤਾਂ {
		sysctl_sched_features |= (1UL << i);
		sched_feat_enable(i);
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ssize_t
sched_feat_write(ਬਣਤਰ file *filp, ਅਚਲ ਅੱਖਰ __user *ubuf,
		ਮਾਪ_t cnt, loff_t *ppos)
{
	ਅੱਖਰ buf[64];
	ਅੱਖਰ *cmp;
	ਪੂਰਨ ret;
	ਬਣਤਰ inode *inode;

	ਜੇ (cnt > 63)
		cnt = 63;

	ਜੇ (copy_from_user(buf, ubuf, cnt))
		ਵਾਪਸ -EFAULT;

	buf[cnt] = 0;
	cmp = strstrip(buf);

	/* Ensure the static_key remains in a consistent state */
	inode = file_inode(filp);
	cpus_read_ਤਾਲਾ();
	inode_ਤਾਲਾ(inode);
	ret = sched_feat_ਸੈੱਟ(cmp);
	inode_unlock(inode);
	cpus_read_unlock();
	ਜੇ (ret < 0)
		ਵਾਪਸ ret;

	*ppos += cnt;

	ਵਾਪਸ cnt;
}

ਸਥਿਰ ਪੂਰਨ sched_feat_open(ਬਣਤਰ inode *inode, ਬਣਤਰ file *filp)
{
	ਵਾਪਸ single_open(filp, sched_feat_show, ਨਿਰਾ);
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ file_operations sched_feat_fops = {
	.open		= sched_feat_open,
	.ਲਿਖ		= sched_feat_write,
	.ਪਠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ਸਥਿਰ ssize_t sched_scaling_write(ਬਣਤਰ file *filp, ਅਚਲ ਅੱਖਰ __user *ubuf,
				   ਮਾਪ_t cnt, loff_t *ppos)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ scaling;
	ਪੂਰਨ ret;

	ret = kstrtouint_from_user(ubuf, cnt, 10, &scaling);
	ਜੇ (ret)
		ਵਾਪਸ ret;

	ਜੇ (scaling >= SCHED_TUNABLESCALING_ਸਿਰਾ)
		ਵਾਪਸ -EINVAL;

	sysctl_sched_tunable_scaling = scaling;
	ਜੇ (sched_update_scaling())
		ਵਾਪਸ -EINVAL;

	*ppos += cnt;
	ਵਾਪਸ cnt;
}

ਸਥਿਰ ਪੂਰਨ sched_scaling_show(ਬਣਤਰ seq_file *m, ਖਾਲੀ *v)
{
	seq_printf(m, "%d\n", sysctl_sched_tunable_scaling);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ sched_scaling_open(ਬਣਤਰ inode *inode, ਬਣਤਰ file *filp)
{
	ਵਾਪਸ single_open(filp, sched_scaling_show, ਨਿਰਾ);
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ file_operations sched_scaling_fops = {
	.open		= sched_scaling_open,
	.ਲਿਖ		= sched_scaling_write,
	.ਪਠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CACHE
ਸਥਿਰ ssize_t
sched_cache_enable_write(ਬਣਤਰ file *filp, ਅਚਲ ਅੱਖਰ __user *ubuf,
			 ਮਾਪ_t cnt, loff_t *ppos)
{
	bool val;
	ਪੂਰਨ ret;

	ret = kstrtobool_from_user(ubuf, cnt, &val);
	ਜੇ (ret)
		ਵਾਪਸ ret;

	sysctl_sched_cache_user = val;

	sched_cache_ਸਰਗਰਮ_ਸੈੱਟ();

	*ppos += cnt;

	ਵਾਪਸ cnt;
}

ਸਥਿਰ ਪੂਰਨ sched_cache_enable_show(ਬਣਤਰ seq_file *m, ਖਾਲੀ *v)
{
	seq_printf(m, "%d\n", sysctl_sched_cache_user);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ sched_cache_enable_open(ਬਣਤਰ inode *inode,
				   ਬਣਤਰ file *filp)
{
	ਵਾਪਸ single_open(filp, sched_cache_enable_show, ਨਿਰਾ);
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ file_operations sched_cache_enable_fops = {
	.open           = sched_cache_enable_open,
	.ਲਿਖ          = sched_cache_enable_write,
	.ਪਠ           = seq_read,
	.llseek         = seq_lseek,
	.release        = single_release,
};
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PREEMPT_DYNAMIC

ਸਥਿਰ ssize_t sched_dynamic_write(ਬਣਤਰ file *filp, ਅਚਲ ਅੱਖਰ __user *ubuf,
				   ਮਾਪ_t cnt, loff_t *ppos)
{
	ਅੱਖਰ buf[16];
	ਪੂਰਨ ਬਹੁਲਕ;

	ਜੇ (cnt > 15)
		cnt = 15;

	ਜੇ (copy_from_user(buf, ubuf, cnt))
		ਵਾਪਸ -EFAULT;

	buf[cnt] = 0;
	ਬਹੁਲਕ = sched_dynamic_ਬਹੁਲਕ(strstrip(buf));
	ਜੇ (ਬਹੁਲਕ < 0)
		ਵਾਪਸ ਬਹੁਲਕ;

	sched_dynamic_update(ਬਹੁਲਕ);

	*ppos += cnt;

	ਵਾਪਸ cnt;
}

ਸਥਿਰ ਪੂਰਨ sched_dynamic_show(ਬਣਤਰ seq_file *m, ਖਾਲੀ *v)
{
	ਪੂਰਨ ਬਹੁਲਕ = READ_ONCE(preempt_dynamic_ਬਹੁਲਕ);

	/* Stop at NULL terminator */
	ਲਈ (ਪੂਰਨ i = 0; preempt_modes[i]; i++) {
		ਜੇ (ਬਹੁਲਕ == i)
			seq_puts(m, "(");
		seq_puts(m, preempt_modes[i]);
		ਜੇ (ਬਹੁਲਕ == i)
			seq_puts(m, ")");

		seq_puts(m, " ");
	}

	seq_puts(m, "\n");
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ sched_dynamic_open(ਬਣਤਰ inode *inode, ਬਣਤਰ file *filp)
{
	ਵਾਪਸ single_open(filp, sched_dynamic_show, ਨਿਰਾ);
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ file_operations sched_dynamic_fops = {
	.open		= sched_dynamic_open,
	.ਲਿਖ		= sched_dynamic_write,
	.ਪਠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#ਜੇ_ਅੰਤ /* CONFIG_PREEMPT_DYNAMIC */

__read_mostly bool sched_debug_verbose;

ਸਥਿਰ ਬਣਤਰ dentry           *sd_dentry;


ਸਥਿਰ ssize_t sched_verbose_write(ਬਣਤਰ file *filp, ਅਚਲ ਅੱਖਰ __user *ubuf,
				  ਮਾਪ_t cnt, loff_t *ppos)
{
	ssize_t ਨਤੀਜਾ;
	bool orig;

	cpus_read_ਤਾਲਾ();
	sched_domains_mutex_ਤਾਲਾ();

	orig = sched_debug_verbose;
	ਨਤੀਜਾ = debugfs_write_file_bool(filp, ubuf, cnt, ppos);

	ਜੇ (sched_debug_verbose && !orig)
		update_sched_domain_debugfs();
	ਨਹੀਂਤਾਂ ਜੇ (!sched_debug_verbose && orig) {
		debugfs_remove(sd_dentry);
		sd_dentry = ਨਿਰਾ;
	}

	sched_domains_mutex_unlock();
	cpus_read_unlock();

	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ file_operations sched_verbose_fops = {
	.ਪਠ =         debugfs_read_file_bool,
	.ਲਿਖ =        sched_verbose_write,
	.open =         simple_open,
	.llseek =       default_llseek,
};

ਸਥਿਰ ਅਚਲ ਬਣਤਰ seq_operations sched_debug_sops;

ਸਥਿਰ ਪੂਰਨ sched_debug_open(ਬਣਤਰ inode *inode, ਬਣਤਰ file *filp)
{
	ਵਾਪਸ seq_open(filp, &sched_debug_sops);
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ file_operations sched_debug_fops = {
	.open		= sched_debug_open,
	.ਪਠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

ਗਿਣਤੀ dl_param {
	DL_RUNTIME = 0,
	DL_PERIOD,
};

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ dl_server_period_max = (1UL << 22) * NSEC_ਪ੍ਰਤੀ_USEC; /* ~4 seconds */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ dl_server_period_min = (100) * NSEC_ਪ੍ਰਤੀ_USEC;     /* 100 us */

ਸਥਿਰ ssize_t sched_server_write_common(ਬਣਤਰ file *filp, ਅਚਲ ਅੱਖਰ __user *ubuf,
					 ਮਾਪ_t cnt, loff_t *ppos, ਗਿਣਤੀ dl_param param,
					 ਖਾਲੀ *server)
{
	ਲੰਮਾ cpu = (ਲੰਮਾ) ((ਬਣਤਰ seq_file *) filp->ਨਿੱਜੀ_ਅੰਕੜੇ)->ਨਿਜੀ;
	ਬਣਤਰ sched_dl_entity *dl_se = (ਬਣਤਰ sched_dl_entity *)server;
	u64 old_runtime, runtime, period;
	ਬਣਤਰ rq *rq = cpu_rq(cpu);
	ਪੂਰਨ retval = 0;
	ਮਾਪ_t err;
	u64 ਮੁੱਲ;

	err = kstrtoull_from_user(ubuf, cnt, 10, &ਮੁੱਲ);
	ਜੇ (err)
		ਵਾਪਸ err;

	scoped_guard (rq_ਤਾਲਾ_irqsave, rq) {
		old_runtime = runtime = dl_se->dl_runtime;
		period = dl_se->dl_period;

		ਚੋਣ (param) {
		ਹਾਲਤ DL_RUNTIME:
			ਜੇ (runtime == ਮੁੱਲ)
				ਰੋਕੋ;
			runtime = ਮੁੱਲ;
			ਰੋਕੋ;
		ਹਾਲਤ DL_PERIOD:
			ਜੇ (ਮੁੱਲ == period)
				ਰੋਕੋ;
			period = ਮੁੱਲ;
			ਰੋਕੋ;
		}

		ਜੇ (runtime > period ||
		    period > dl_server_period_max ||
		    period < dl_server_period_min) {
			ਵਾਪਸ  -EINVAL;
		}

		ਜੇ (!cpu_online(cpu_ਦਾ(rq)))
			ਵਾਪਸ -EBUSY;

		update_rq_clock(rq);
		dl_server_stop(dl_se);
		retval = dl_server_apply_params(dl_se, runtime, period, 0);
		dl_server_ਸ਼ੁਰੂ(dl_se);

		ਜੇ (retval < 0)
			ਵਾਪਸ retval;
	}

	ਜੇ (!!old_runtime ^ !!runtime) {
		pr_info("%s server %sabled on CPU %d%s.\n",
			server == &rq->fair_server ? "Fair" : "Ext",
			runtime ? "en" : "dis",
			cpu_ਦਾ(rq),
			runtime ? "" : ", system may malfunction due to starvation");
	}

	*ppos += cnt;
	ਵਾਪਸ cnt;
}

ਸਥਿਰ ਮਾਪ_t sched_server_show_common(ਬਣਤਰ seq_file *m, ਖਾਲੀ *v, ਗਿਣਤੀ dl_param param,
				       ਖਾਲੀ *server)
{
	ਬਣਤਰ sched_dl_entity *dl_se = (ਬਣਤਰ sched_dl_entity *)server;
	u64 ਮੁੱਲ;

	ਚੋਣ (param) {
	ਹਾਲਤ DL_RUNTIME:
		ਮੁੱਲ = dl_se->dl_runtime;
		ਰੋਕੋ;
	ਹਾਲਤ DL_PERIOD:
		ਮੁੱਲ = dl_se->dl_period;
		ਰੋਕੋ;
	}

	seq_printf(m, "%llu\n", ਮੁੱਲ);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ssize_t
sched_fair_server_runtime_write(ਬਣਤਰ file *filp, ਅਚਲ ਅੱਖਰ __user *ubuf,
				ਮਾਪ_t cnt, loff_t *ppos)
{
	ਲੰਮਾ cpu = (ਲੰਮਾ) ((ਬਣਤਰ seq_file *) filp->ਨਿੱਜੀ_ਅੰਕੜੇ)->ਨਿਜੀ;
	ਬਣਤਰ rq *rq = cpu_rq(cpu);

	ਵਾਪਸ sched_server_write_common(filp, ubuf, cnt, ppos, DL_RUNTIME,
					&rq->fair_server);
}

ਸਥਿਰ ਪੂਰਨ sched_fair_server_runtime_show(ਬਣਤਰ seq_file *m, ਖਾਲੀ *v)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) m->ਨਿਜੀ;
	ਬਣਤਰ rq *rq = cpu_rq(cpu);

	ਵਾਪਸ sched_server_show_common(m, v, DL_RUNTIME, &rq->fair_server);
}

ਸਥਿਰ ਪੂਰਨ sched_fair_server_runtime_open(ਬਣਤਰ inode *inode, ਬਣਤਰ file *filp)
{
	ਵਾਪਸ single_open(filp, sched_fair_server_runtime_show, inode->i_ਨਿੱਜੀ);
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ file_operations fair_server_runtime_fops = {
	.open		= sched_fair_server_runtime_open,
	.ਲਿਖ		= sched_fair_server_runtime_write,
	.ਪਠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ਸਥਿਰ ਬਣਤਰ dentry *debugfs_sched;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CLASS_EXT
ਸਥਿਰ ssize_t
sched_ext_server_runtime_write(ਬਣਤਰ file *filp, ਅਚਲ ਅੱਖਰ __user *ubuf,
			       ਮਾਪ_t cnt, loff_t *ppos)
{
	ਲੰਮਾ cpu = (ਲੰਮਾ) ((ਬਣਤਰ seq_file *) filp->ਨਿੱਜੀ_ਅੰਕੜੇ)->ਨਿਜੀ;
	ਬਣਤਰ rq *rq = cpu_rq(cpu);

	ਵਾਪਸ sched_server_write_common(filp, ubuf, cnt, ppos, DL_RUNTIME,
					&rq->ext_server);
}

ਸਥਿਰ ਪੂਰਨ sched_ext_server_runtime_show(ਬਣਤਰ seq_file *m, ਖਾਲੀ *v)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) m->ਨਿਜੀ;
	ਬਣਤਰ rq *rq = cpu_rq(cpu);

	ਵਾਪਸ sched_server_show_common(m, v, DL_RUNTIME, &rq->ext_server);
}

ਸਥਿਰ ਪੂਰਨ sched_ext_server_runtime_open(ਬਣਤਰ inode *inode, ਬਣਤਰ file *filp)
{
	ਵਾਪਸ single_open(filp, sched_ext_server_runtime_show, inode->i_ਨਿੱਜੀ);
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ file_operations ext_server_runtime_fops = {
	.open		= sched_ext_server_runtime_open,
	.ਲਿਖ		= sched_ext_server_runtime_write,
	.ਪਠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ਸਥਿਰ ssize_t
sched_ext_server_period_write(ਬਣਤਰ file *filp, ਅਚਲ ਅੱਖਰ __user *ubuf,
			      ਮਾਪ_t cnt, loff_t *ppos)
{
	ਲੰਮਾ cpu = (ਲੰਮਾ) ((ਬਣਤਰ seq_file *) filp->ਨਿੱਜੀ_ਅੰਕੜੇ)->ਨਿਜੀ;
	ਬਣਤਰ rq *rq = cpu_rq(cpu);

	ਵਾਪਸ sched_server_write_common(filp, ubuf, cnt, ppos, DL_PERIOD,
					&rq->ext_server);
}

ਸਥਿਰ ਪੂਰਨ sched_ext_server_period_show(ਬਣਤਰ seq_file *m, ਖਾਲੀ *v)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) m->ਨਿਜੀ;
	ਬਣਤਰ rq *rq = cpu_rq(cpu);

	ਵਾਪਸ sched_server_show_common(m, v, DL_PERIOD, &rq->ext_server);
}

ਸਥਿਰ ਪੂਰਨ sched_ext_server_period_open(ਬਣਤਰ inode *inode, ਬਣਤਰ file *filp)
{
	ਵਾਪਸ single_open(filp, sched_ext_server_period_show, inode->i_ਨਿੱਜੀ);
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ file_operations ext_server_period_fops = {
	.open		= sched_ext_server_period_open,
	.ਲਿਖ		= sched_ext_server_period_write,
	.ਪਠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ਸਥਿਰ ਖਾਲੀ debugfs_ext_server_init(ਖਾਲੀ)
{
	ਬਣਤਰ dentry *d_ext;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu;

	d_ext = debugfs_create_dir("ext_server", debugfs_sched);
	ਜੇ (!d_ext)
		ਵਾਪਸ;

	for_each_possible_cpu(cpu) {
		ਬਣਤਰ dentry *d_cpu;
		ਅੱਖਰ buf[32];

		snprintf(buf, ਆਕਾਰ(buf), "cpu%lu", cpu);
		d_cpu = debugfs_create_dir(buf, d_ext);

		debugfs_create_file("runtime", 0644, d_cpu, (ਖਾਲੀ *) cpu, &ext_server_runtime_fops);
		debugfs_create_file("period", 0644, d_cpu, (ਖਾਲੀ *) cpu, &ext_server_period_fops);
	}
}
#ਜੇ_ਅੰਤ /* CONFIG_SCHED_CLASS_EXT */

ਸਥਿਰ ssize_t
sched_fair_server_period_write(ਬਣਤਰ file *filp, ਅਚਲ ਅੱਖਰ __user *ubuf,
			       ਮਾਪ_t cnt, loff_t *ppos)
{
	ਲੰਮਾ cpu = (ਲੰਮਾ) ((ਬਣਤਰ seq_file *) filp->ਨਿੱਜੀ_ਅੰਕੜੇ)->ਨਿਜੀ;
	ਬਣਤਰ rq *rq = cpu_rq(cpu);

	ਵਾਪਸ sched_server_write_common(filp, ubuf, cnt, ppos, DL_PERIOD,
					&rq->fair_server);
}

ਸਥਿਰ ਪੂਰਨ sched_fair_server_period_show(ਬਣਤਰ seq_file *m, ਖਾਲੀ *v)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) m->ਨਿਜੀ;
	ਬਣਤਰ rq *rq = cpu_rq(cpu);

	ਵਾਪਸ sched_server_show_common(m, v, DL_PERIOD, &rq->fair_server);
}

ਸਥਿਰ ਪੂਰਨ sched_fair_server_period_open(ਬਣਤਰ inode *inode, ਬਣਤਰ file *filp)
{
	ਵਾਪਸ single_open(filp, sched_fair_server_period_show, inode->i_ਨਿੱਜੀ);
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ file_operations fair_server_period_fops = {
	.open		= sched_fair_server_period_open,
	.ਲਿਖ		= sched_fair_server_period_write,
	.ਪਠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ਸਥਿਰ ਖਾਲੀ debugfs_fair_server_init(ਖਾਲੀ)
{
	ਬਣਤਰ dentry *d_fair;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu;

	d_fair = debugfs_create_dir("fair_server", debugfs_sched);
	ਜੇ (!d_fair)
		ਵਾਪਸ;

	for_each_possible_cpu(cpu) {
		ਬਣਤਰ dentry *d_cpu;
		ਅੱਖਰ buf[32];

		snprintf(buf, ਆਕਾਰ(buf), "cpu%lu", cpu);
		d_cpu = debugfs_create_dir(buf, d_fair);

		debugfs_create_file("runtime", 0644, d_cpu, (ਖਾਲੀ *) cpu, &fair_server_runtime_fops);
		debugfs_create_file("period", 0644, d_cpu, (ਖਾਲੀ *) cpu, &fair_server_period_fops);
	}
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
ਸਥਿਰ ਪੂਰਨ cgroup_ਬਹੁਲਕ = 2;

/* See __sched_cgroup_mode_update(). */
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *cgroup_ਬਹੁਲਕ_str[] = {
	"up",
	"smp",
	"concur",
	"max",
	"tasks",
};

ਸਥਿਰ ਪੂਰਨ sched_cgroup_ਬਹੁਲਕ(ਅਚਲ ਅੱਖਰ *str)
{
	ਲਈ (ਪੂਰਨ i = 0; i < ARRAY_ਮਾਪ(cgroup_ਬਹੁਲਕ_str); i++) {
		ਜੇ (!strcmp(str, cgroup_ਬਹੁਲਕ_str[i]))
			ਵਾਪਸ i;
	}
	ਵਾਪਸ -EINVAL;
}

ਸਥਿਰ ssize_t sched_cgroup_write(ਬਣਤਰ file *filp, ਅਚਲ ਅੱਖਰ __user *ubuf,
				   ਮਾਪ_t cnt, loff_t *ppos)
{
	ਅੱਖਰ buf[16];
	ਪੂਰਨ ਬਹੁਲਕ;

	ਜੇ (cnt > 15)
		cnt = 15;

	ਜੇ (copy_from_user(buf, ubuf, cnt))
		ਵਾਪਸ -EFAULT;

	buf[cnt] = 0;
	ਬਹੁਲਕ = sched_cgroup_ਬਹੁਲਕ(strstrip(buf));
	ਜੇ (ਬਹੁਲਕ < 0)
		ਵਾਪਸ ਬਹੁਲਕ;

	__sched_cgroup_mode_update(ਬਹੁਲਕ);
	WRITE_ONCE(cgroup_ਬਹੁਲਕ, ਬਹੁਲਕ);

	*ppos += cnt;
	ਵਾਪਸ cnt;
}

ਸਥਿਰ ਪੂਰਨ sched_cgroup_show(ਬਣਤਰ seq_file *m, ਖਾਲੀ *v)
{
	ਪੂਰਨ ਬਹੁਲਕ = READ_ONCE(cgroup_ਬਹੁਲਕ);

	ਲਈ (ਪੂਰਨ i = 0; i < ARRAY_ਮਾਪ(cgroup_ਬਹੁਲਕ_str); i++) {
		ਜੇ (ਬਹੁਲਕ == i)
			seq_puts(m, "(");
		seq_puts(m, cgroup_ਬਹੁਲਕ_str[i]);
		ਜੇ (ਬਹੁਲਕ == i)
			seq_puts(m, ")");

		seq_puts(m, " ");
	}
	seq_puts(m, "\n");
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ sched_cgroup_open(ਬਣਤਰ inode *inode, ਬਣਤਰ file *filp)
{
	ਵਾਪਸ single_open(filp, sched_cgroup_show, ਨਿਰਾ);
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ file_operations sched_cgroup_fops = {
	.open		= sched_cgroup_open,
	.ਲਿਖ		= sched_cgroup_write,
	.ਪਠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};
#ਜੇ_ਅੰਤ

ਸਥਿਰ __init ਪੂਰਨ sched_init_debug(ਖਾਲੀ)
{
	ਬਣਤਰ dentry __maybe_unused *numa, *llc;

	debugfs_sched = debugfs_create_dir("sched", ਨਿਰਾ);

	debugfs_create_file("features", 0644, debugfs_sched, ਨਿਰਾ, &sched_feat_fops);
	debugfs_create_file_unsafe("verbose", 0644, debugfs_sched, &sched_debug_verbose, &sched_verbose_fops);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PREEMPT_DYNAMIC
	debugfs_create_file("preempt", 0644, debugfs_sched, ਨਿਰਾ, &sched_dynamic_fops);
#ਜੇ_ਅੰਤ

	debugfs_create_u32("base_slice_ns", 0644, debugfs_sched, &sysctl_sched_ਅਧਾਰ_ਟੁਕੜਾ);

	debugfs_create_u32("latency_warn_ms", 0644, debugfs_sched, &sysctl_resched_latency_warn_ms);
	debugfs_create_u32("latency_warn_once", 0644, debugfs_sched, &sysctl_resched_latency_warn_once);

	debugfs_create_file("tunable_scaling", 0644, debugfs_sched, ਨਿਰਾ, &sched_scaling_fops);
	debugfs_create_u32("migration_cost_ns", 0644, debugfs_sched, &sysctl_sched_migration_ਲਾਗਤ);
	debugfs_create_u32("nr_migrate", 0644, debugfs_sched, &sysctl_sched_nr_migrate);

	sched_domains_mutex_ਤਾਲਾ();
	update_sched_domain_debugfs();
	sched_domains_mutex_unlock();

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
	numa = debugfs_create_dir("numa_balancing", debugfs_sched);

	debugfs_create_u32("scan_delay_ms", 0644, numa, &sysctl_numa_balancing_ਜਾਂਚ_delay);
	debugfs_create_u32("scan_period_min_ms", 0644, numa, &sysctl_numa_balancing_ਜਾਂਚ_period_min);
	debugfs_create_u32("scan_period_max_ms", 0644, numa, &sysctl_numa_balancing_ਜਾਂਚ_period_max);
	debugfs_create_u32("scan_size_mb", 0644, numa, &sysctl_numa_balancing_ਜਾਂਚ_ਮਾਪ);
	debugfs_create_u32("hot_threshold_ms", 0644, numa, &sysctl_numa_balancing_hot_ਹੱਦ);
#ਜੇ_ਅੰਤ /* CONFIG_NUMA_BALANCING */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CACHE
	llc = debugfs_create_dir("llc_balancing", debugfs_sched);
	debugfs_create_file("enabled", 0644, llc, ਨਿਰਾ,
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
#ਜੇ_ਅੰਤ

	debugfs_create_file("debug", 0444, debugfs_sched, ਨਿਰਾ, &sched_debug_fops);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
	debugfs_create_file("cgroup_mode", 0644, debugfs_sched, ਨਿਰਾ, &sched_cgroup_fops);
#ਜੇ_ਅੰਤ

	debugfs_fair_server_init();
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CLASS_EXT
	debugfs_ext_server_init();
#ਜੇ_ਅੰਤ

	ਵਾਪਸ 0;
}
late_initcall(sched_init_debug);

ਸਥਿਰ cpumask_var_t		sd_sysctl_cpus;

ਸਥਿਰ ਪੂਰਨ sd_flags_show(ਬਣਤਰ seq_file *m, ਖਾਲੀ *v)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags = *(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *)m->ਨਿਜੀ;
	ਪੂਰਨ idx;

	for_each_ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(idx, &flags, __SD_FLAG_CNT) {
		seq_puts(m, sd_flag_debug[idx].name);
		seq_puts(m, " ");
	}
	seq_puts(m, "\n");

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ sd_flags_open(ਬਣਤਰ inode *inode, ਬਣਤਰ file *file)
{
	ਵਾਪਸ single_open(file, sd_flags_show, inode->i_ਨਿੱਜੀ);
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ file_operations sd_flags_fops = {
	.open		= sd_flags_open,
	.ਪਠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ਸਥਿਰ ਖਾਲੀ ਰਜਿਸਟਰ_sd(ਬਣਤਰ sched_domain *sd, ਬਣਤਰ dentry *ਮਾਪਾ)
{
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SDM(type, ਬਹੁਲਕ, member)	\
	debugfs_create_##type(#member, ਬਹੁਲਕ, ਮਾਪਾ, &sd->member)

	SDM(ulong, 0644, min_interval);
	SDM(ulong, 0644, max_interval);
	SDM(u64,   0644, max_newidle_lb_ਲਾਗਤ);
	SDM(u32,   0644, busy_ਗੁਣਕ);
	SDM(u32,   0644, imbalance_pct);
	SDM(u32,   0644, cache_nice_tries);
	SDM(str,   0444, name);

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ SDM

	debugfs_create_file("flags", 0444, ਮਾਪਾ, &sd->flags, &sd_flags_fops);
	debugfs_create_file("groups_flags", 0444, ਮਾਪਾ, &sd->groups->flags, &sd_flags_fops);
	debugfs_create_u32("level", 0444, ਮਾਪਾ, (u32 *)&sd->level);

	ਜੇ (sd->flags & SD_ASYM_PACKING)
		debugfs_create_u32("group_asym_prefer_cpu", 0444, ਮਾਪਾ,
				   (u32 *)&sd->groups->asym_prefer_cpu);
}

ਖਾਲੀ update_sched_domain_debugfs(ਖਾਲੀ)
{
	ਪੂਰਨ cpu, i;

	/*
	 * This can unfortunately be invoked before sched_debug_init() creates
	 * the debug directory. Don't touch sd_sysctl_cpus until then.
	 */
	ਜੇ (!debugfs_sched)
		ਵਾਪਸ;

	ਜੇ (!sched_debug_verbose)
		ਵਾਪਸ;

	ਜੇ (!cpumask_ਉਪਲਬਧ(sd_sysctl_cpus)) {
		ਜੇ (!alloc_cpumask_var(&sd_sysctl_cpus, GFP_KERNEL))
			ਵਾਪਸ;
		cpumask_copy(sd_sysctl_cpus, cpu_possible_ਨਕਾਬ);
	}

	ਜੇ (!sd_dentry) {
		sd_dentry = debugfs_create_dir("domains", debugfs_sched);

		/* rebuild sd_sysctl_cpus if empty since it gets cleared below */
		ਜੇ (cpumask_ਖਾਲੀ_ਥਾਵਾਂ(sd_sysctl_cpus))
			cpumask_copy(sd_sysctl_cpus, cpu_online_ਨਕਾਬ);
	}

	for_each_cpu(cpu, sd_sysctl_cpus) {
		ਬਣਤਰ sched_domain *sd;
		ਬਣਤਰ dentry *d_cpu;
		ਅੱਖਰ buf[32];

		snprintf(buf, ਆਕਾਰ(buf), "cpu%d", cpu);
		debugfs_lookup_and_remove(buf, sd_dentry);
		d_cpu = debugfs_create_dir(buf, sd_dentry);

		i = 0;
		for_each_domain(cpu, sd) {
			ਬਣਤਰ dentry *d_sd;

			snprintf(buf, ਆਕਾਰ(buf), "domain%d", i);
			d_sd = debugfs_create_dir(buf, d_cpu);

			ਰਜਿਸਟਰ_sd(sd, d_sd);
			i++;
		}

		__cpumask_clear_cpu(cpu, sd_sysctl_cpus);
	}
}

ਖਾਲੀ dirty_sched_domain_sysctl(ਪੂਰਨ cpu)
{
	ਜੇ (cpumask_ਉਪਲਬਧ(sd_sysctl_cpus))
		__cpumask_set_cpu(cpu, sd_sysctl_cpus);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
ਸਥਿਰ ਖਾਲੀ print_cfs_ਸਮੂਹ_stats(ਬਣਤਰ seq_file *m, ਪੂਰਨ cpu, ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	ਬਣਤਰ sched_entity *se = tg_se(tg, cpu);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ P(F)		SEQ_printf(m, "  .%-30s: %lld\n",	#F, (ਲੰਮਾ ਲੰਮਾ)F)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ P_SCHEDSTAT(F)	SEQ_printf(m, "  .%-30s: %lld\n",	\
		#F, (ਲੰਮਾ ਲੰਮਾ)schedstat_val(stats->F))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PN(F)		SEQ_printf(m, "  .%-30s: %lld.%06ld\n", #F, ਵੰਡੋ_NS((ਲੰਮਾ ਲੰਮਾ)F))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PN_SCHEDSTAT(F)	SEQ_printf(m, "  .%-30s: %lld.%06ld\n", \
		#F, ਵੰਡੋ_NS((ਲੰਮਾ ਲੰਮਾ)schedstat_val(stats->F)))

	ਜੇ (!se)
		ਵਾਪਸ;

	PN(se->exec_ਸ਼ੁਰੂ);
	PN(se->vruntime);
	PN(se->ਜੋੜ_exec_runtime);

	ਜੇ (schedstat_enabled()) {
		ਬਣਤਰ sched_statistics *stats;
		stats = __schedstats_from_se(se);

		PN_SCHEDSTAT(wait_ਸ਼ੁਰੂ);
		PN_SCHEDSTAT(sleep_ਸ਼ੁਰੂ);
		PN_SCHEDSTAT(ਖੰਡ_ਸ਼ੁਰੂ);
		PN_SCHEDSTAT(sleep_max);
		PN_SCHEDSTAT(ਖੰਡ_max);
		PN_SCHEDSTAT(exec_max);
		PN_SCHEDSTAT(ਟੁਕੜਾ_max);
		PN_SCHEDSTAT(wait_max);
		PN_SCHEDSTAT(wait_ਜੋੜ);
		P_SCHEDSTAT(wait_ਗਿਣਤੀ);
	}

	P(se->ਲੱਦੋ.ਭਾਰ);
	P(se->avg.ਲੱਦੋ_avg);
	P(se->avg.util_avg);
	P(se->avg.runnable_avg);

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PN_SCHEDSTAT
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PN
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ P_SCHEDSTAT
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ P
}
#ਜੇ_ਅੰਤ /* CONFIG_FAIR_GROUP_SCHED */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CGROUP_SCHED
ਸਥਿਰ DEFINE_SPINLOCK(sched_debug_ਤਾਲਾ);
ਸਥਿਰ ਅੱਖਰ ਸਮੂਹ_ਰਸਤਾ[ਰਸਤਾ_MAX];

ਸਥਿਰ ਖਾਲੀ task_ਸਮੂਹ_ਰਸਤਾ(ਬਣਤਰ task_ਸਮੂਹ *tg, ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ plen)
{
	ਜੇ (autogroup_ਰਸਤਾ(tg, ਰਸਤਾ, plen))
		ਵਾਪਸ;

	cgroup_ਰਸਤਾ(tg->css.cgroup, ਰਸਤਾ, plen);
}

/*
 * Only 1 SEQ_printf_task_group_path() caller can use the full length
 * group_path[] for cgroup path. Other simultaneous callers will have
 * to use a shorter stack buffer. A "..." suffix is appended at the end
 * of the stack buffer so that it will show up in case the output length
 * matches the given buffer size to indicate possible path name truncation.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SEQ_printf_task_ਸਮੂਹ_ਰਸਤਾ(m, tg, fmt...)			\
{									\
	ਜੇ (spin_trylock(&sched_debug_ਤਾਲਾ)) {				\
		task_ਸਮੂਹ_ਰਸਤਾ(tg, ਸਮੂਹ_ਰਸਤਾ, ਆਕਾਰ(ਸਮੂਹ_ਰਸਤਾ));	\
		SEQ_printf(m, fmt, ਸਮੂਹ_ਰਸਤਾ);				\
		spin_unlock(&sched_debug_ਤਾਲਾ);				\
	} ਨਹੀਂਤਾਂ {							\
		ਅੱਖਰ buf[128];						\
		ਅੱਖਰ *bufend = buf + ਆਕਾਰ(buf) - 3;			\
		task_ਸਮੂਹ_ਰਸਤਾ(tg, buf, bufend - buf);			\
		strcpy(bufend - 1, "...");				\
		SEQ_printf(m, fmt, buf);				\
	}								\
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਖਾਲੀ
print_task(ਬਣਤਰ seq_file *m, ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਜੇ (task_current(rq, p))
		SEQ_printf(m, ">R");
	ਨਹੀਂਤਾਂ
		SEQ_printf(m, " %c", task_state_ਮੰਜ਼ਿਲ_char(p));

	SEQ_printf(m, " %15s %5d %10ld %9Ld.%06ld   %c   %9Ld.%06ld %c %9Ld.%06ld %9Ld.%06ld %9Ld   %5d ",
		p->comm, task_pid_nr(p),
		p->se.h_ਲੱਦੋ.ਭਾਰ,
		ਵੰਡੋ_NS(p->se.vruntime),
		entity_eligible(&rq->cfs, &p->se) ? 'E' : 'N',
		ਵੰਡੋ_NS(p->se.deadline),
		p->se.custom_ਟੁਕੜਾ ? 'S' : ' ',
		ਵੰਡੋ_NS(p->se.ਟੁਕੜਾ),
		ਵੰਡੋ_NS(p->se.ਜੋੜ_exec_runtime),
		(ਲੰਮਾ ਲੰਮਾ)(p->nvcsw + p->nivcsw),
		p->prio);

	SEQ_printf(m, "%9lld.%06ld %9lld.%06ld %9lld.%06ld",
		ਵੰਡੋ_NS(schedstat_val_or_zero(p->stats.wait_ਜੋੜ)),
		ਵੰਡੋ_NS(schedstat_val_or_zero(p->stats.ਜੋੜ_sleep_runtime)),
		ਵੰਡੋ_NS(schedstat_val_or_zero(p->stats.ਜੋੜ_ਖੰਡ_runtime)));

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
	SEQ_printf(m, "   %d      %d", task_ਗੰਢ(p), task_numa_ਸਮੂਹ_id(p));
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CGROUP_SCHED
	SEQ_printf_task_ਸਮੂਹ_ਰਸਤਾ(m, task_ਸਮੂਹ(p), "        %s")
#ਜੇ_ਅੰਤ

	SEQ_printf(m, "\n");
}

ਸਥਿਰ ਖਾਲੀ print_rq(ਬਣਤਰ seq_file *m, ਬਣਤਰ rq *rq, ਪੂਰਨ rq_cpu)
{
	ਬਣਤਰ task_struct *g, *p;

	SEQ_printf(m, "\n");
	SEQ_printf(m, "runnable tasks:\n");
	SEQ_printf(m, " S            task   PID     weight       vruntime   eligible    "
		   "deadline             slice          sum-exec      switches  "
		   "prio         wait-time        sum-sleep       sum-block"
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
		   "  node   group-id"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CGROUP_SCHED
		   "  group-path"
#ਜੇ_ਅੰਤ
		   "\n");
	SEQ_printf(m, "-------------------------------------------------------"
		   "------------------------------------------------------"
		   "------------------------------------------------------"
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
		   "--------------"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CGROUP_SCHED
		   "--------------"
#ਜੇ_ਅੰਤ
		   "\n");

	rcu_read_ਤਾਲਾ();
	for_each_process_thread(g, p) {
		ਜੇ (task_cpu(p) != rq_cpu)
			ਜਾਰੀ;

		print_task(m, rq, p);
	}
	rcu_read_unlock();
}

ਖਾਲੀ print_cfs_rq(ਬਣਤਰ seq_file *m, ਪੂਰਨ cpu, ਬਣਤਰ cfs_rq *cfs_rq)
{
	s64 ਖੱਬਾ_vruntime = -1, ਸੱਜਾ_vruntime = -1, ਖੱਬਾ_deadline = -1, ਫੈਲਾਅ;
	s64 zero_vruntime = -1, ਜੋੜ_w_vruntime = -1;
	u64 avruntime;
	ਬਣਤਰ sched_entity *ਆਖਰੀ, *ਪਹਿਲਾ, *ਜੜ੍ਹ;
	ਬਣਤਰ rq *rq = cpu_rq(cpu);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਜੋੜ_ਖਿਸਕਾਓ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;
	u64 ਜੋੜ_ਭਾਰ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
	SEQ_printf(m, "\n");
	SEQ_printf_task_ਸਮੂਹ_ਰਸਤਾ(m, cfs_rq->tg, "cfs_rq[%d]:%s\n", cpu);
#ਨਹੀਂਤਾਂ
	SEQ_printf(m, "\n");
	SEQ_printf(m, "cfs_rq[%d]:\n", cpu);
#ਜੇ_ਅੰਤ

	raw_spin_rq_ਤਾਲਾ_irqsave(rq, flags);
	ਜੜ੍ਹ = __pick_root_entity(cfs_rq);
	ਜੇ (ਜੜ੍ਹ)
		ਖੱਬਾ_vruntime = ਜੜ੍ਹ->min_vruntime;
	ਪਹਿਲਾ = __pick_first_entity(cfs_rq);
	ਜੇ (ਪਹਿਲਾ)
		ਖੱਬਾ_deadline = ਪਹਿਲਾ->deadline;
	ਆਖਰੀ = __pick_last_entity(cfs_rq);
	ਜੇ (ਆਖਰੀ)
		ਸੱਜਾ_vruntime = ਆਖਰੀ->vruntime;
	zero_vruntime = cfs_rq->zero_vruntime;
	ਜੋੜ_w_vruntime = cfs_rq->ਜੋੜ_w_vruntime;
	ਜੋੜ_ਭਾਰ = cfs_rq->ਜੋੜ_ਭਾਰ;
	ਜੋੜ_ਖਿਸਕਾਓ = cfs_rq->ਜੋੜ_ਖਿਸਕਾਓ;
	avruntime = avg_vruntime(cfs_rq);
	raw_spin_rq_unlock_irqrestore(rq, flags);

	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "left_deadline",
			ਵੰਡੋ_NS(ਖੱਬਾ_deadline));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "left_vruntime",
			ਵੰਡੋ_NS(ਖੱਬਾ_vruntime));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "zero_vruntime",
			ਵੰਡੋ_NS(zero_vruntime));
	SEQ_printf(m, "  .%-30s: %Ld (%d bits)\n", "sum_w_vruntime",
		   ਜੋੜ_w_vruntime, ilog2(ਨਿਰਪੇਖ(ਜੋੜ_w_vruntime)));
	SEQ_printf(m, "  .%-30s: %Lu\n", "sum_weight",
		   ਜੋੜ_ਭਾਰ);
	SEQ_printf(m, "  .%-30s: %u\n", "sum_shift", ਜੋੜ_ਖਿਸਕਾਓ);
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "avg_vruntime",
			ਵੰਡੋ_NS(avruntime));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "right_vruntime",
			ਵੰਡੋ_NS(ਸੱਜਾ_vruntime));
	ਫੈਲਾਅ = ਸੱਜਾ_vruntime - ਖੱਬਾ_vruntime;
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "spread", ਵੰਡੋ_NS(ਫੈਲਾਅ));
	SEQ_printf(m, "  .%-30s: %d\n", "nr_queued", cfs_rq->nr_queued);
	SEQ_printf(m, "  .%-30s: %d\n", "h_nr_runnable", cfs_rq->h_nr_runnable);
	SEQ_printf(m, "  .%-30s: %d\n", "h_nr_queued", cfs_rq->h_nr_queued);
	SEQ_printf(m, "  .%-30s: %d\n", "h_nr_idle", cfs_rq->h_nr_idle);
	SEQ_printf(m, "  .%-30s: %ld\n", "load", cfs_rq->ਲੱਦੋ.ਭਾਰ);
	SEQ_printf(m, "  .%-30s: %lu\n", "load_avg",
			cfs_rq->avg.ਲੱਦੋ_avg);
	SEQ_printf(m, "  .%-30s: %lu\n", "runnable_avg",
			cfs_rq->avg.runnable_avg);
	SEQ_printf(m, "  .%-30s: %lu\n", "util_avg",
			cfs_rq->avg.util_avg);
	SEQ_printf(m, "  .%-30s: %u\n", "util_est",
			cfs_rq->avg.util_est);
	SEQ_printf(m, "  .%-30s: %ld\n", "removed.load_avg",
			cfs_rq->removed.ਲੱਦੋ_avg);
	SEQ_printf(m, "  .%-30s: %ld\n", "removed.util_avg",
			cfs_rq->removed.util_avg);
	SEQ_printf(m, "  .%-30s: %ld\n", "removed.runnable_avg",
			cfs_rq->removed.runnable_avg);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
	SEQ_printf(m, "  .%-30s: %lu\n", "tg_load_avg_contrib",
			cfs_rq->tg_ਲੱਦੋ_avg_contrib);
	SEQ_printf(m, "  .%-30s: %ld\n", "tg_load_avg",
			atomic_long_read(&cfs_rq->tg->ਲੱਦੋ_avg));
	SEQ_printf(m, "  .%-30s: %lu\n", "h_load",
			cfs_rq->h_ਲੱਦੋ);
#ਜੇ_ਅੰਤ /* CONFIG_FAIR_GROUP_SCHED */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CFS_BANDWIDTH
	SEQ_printf(m, "  .%-30s: %d\n", "throttled",
			cfs_rq->throttled);
	SEQ_printf(m, "  .%-30s: %d\n", "throttle_count",
			cfs_rq->throttle_ਗਿਣਤੀ);
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
	print_cfs_ਸਮੂਹ_stats(m, cpu, cfs_rq->tg);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ print_rt_rq(ਬਣਤਰ seq_file *m, ਪੂਰਨ cpu, ਬਣਤਰ rt_rq *rt_rq)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED
	SEQ_printf(m, "\n");
	SEQ_printf_task_ਸਮੂਹ_ਰਸਤਾ(m, rt_rq->tg, "rt_rq[%d]:%s\n", cpu);
#ਨਹੀਂਤਾਂ
	SEQ_printf(m, "\n");
	SEQ_printf(m, "rt_rq[%d]:\n", cpu);
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ P(x) \
	SEQ_printf(m, "  .%-30s: %Ld\n", #x, (ਲੰਮਾ ਲੰਮਾ)(rt_rq->x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PU(x) \
	SEQ_printf(m, "  .%-30s: %lu\n", #x, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)(rt_rq->x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PN(x) \
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", #x, ਵੰਡੋ_NS(rt_rq->x))

	PU(rt_nr_running);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED
	P(rt_throttled);
	PN(rt_ਸਮਾਂ);
	PN(rt_runtime);
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PN
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PU
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ P
}

ਖਾਲੀ print_dl_rq(ਬਣਤਰ seq_file *m, ਪੂਰਨ cpu, ਬਣਤਰ dl_rq *dl_rq)
{
	ਬਣਤਰ dl_bw *dl_bw;

	SEQ_printf(m, "\n");
	SEQ_printf(m, "dl_rq[%d]:\n", cpu);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PU(x) \
	SEQ_printf(m, "  .%-30s: %lu\n", #x, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)(dl_rq->x))

	PU(dl_nr_running);
	dl_bw = &cpu_rq(cpu)->rd->dl_bw;
	SEQ_printf(m, "  .%-30s: %lld\n", "dl_bw->bw", dl_bw->bw);
	SEQ_printf(m, "  .%-30s: %lld\n", "dl_bw->total_bw", dl_bw->ਕੁੱਲ_bw);

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PU
}

ਸਥਿਰ ਖਾਲੀ print_cpu(ਬਣਤਰ seq_file *m, ਪੂਰਨ cpu)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_X86
	{
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ freq = cpu_khz ? : 1;

		SEQ_printf(m, "cpu#%d, %u.%03u MHz\n",
			   cpu, freq / 1000, (freq % 1000));
	}
#ਨਹੀਂਤਾਂ /* !CONFIG_X86: */
	SEQ_printf(m, "cpu#%d\n", cpu);
#ਜੇ_ਅੰਤ /* !CONFIG_X86 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ P(x)								\
ਕਰੋ {									\
	ਜੇ (ਆਕਾਰ(rq->x) == 4)						\
		SEQ_printf(m, "  .%-30s: %d\n", #x, (ਪੂਰਨ)(rq->x));	\
	ਨਹੀਂਤਾਂ								\
		SEQ_printf(m, "  .%-30s: %Ld\n", #x, (ਲੰਮਾ ਲੰਮਾ)(rq->x));\
} ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PN(x) \
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", #x, ਵੰਡੋ_NS(rq->x))

	P(nr_running);
	P(nr_switches);
	P(nr_uninterruptible);
	PN(ਅਗਲਾ_balance);
	SEQ_printf(m, "  .%-30s: %ld\n", "curr->pid", (ਲੰਮਾ)(task_pid_nr(rq->curr)));
	PN(clock);
	PN(clock_task);
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ P
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PN

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ P64(n) SEQ_printf(m, "  .%-30s: %Ld\n", #n, rq->n);
	P64(avg_idle);
	P64(max_idle_balance_ਲਾਗਤ);
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ P64

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ P(n) SEQ_printf(m, "  .%-30s: %d\n", #n, schedstat_val(rq->n));
	ਜੇ (schedstat_enabled()) {
		P(yld_ਗਿਣਤੀ);
		P(sched_ਗਿਣਤੀ);
		P(sched_goidle);
		P(ttwu_ਗਿਣਤੀ);
		P(ttwu_local);
	}
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ P

	print_cfs_stats(m, cpu);
	print_rt_stats(m, cpu);
	print_dl_stats(m, cpu);

	print_rq(m, rq, cpu);
	SEQ_printf(m, "\n");
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *sched_tunable_scaling_ਨਾਂ[] = {
	"none",
	"logarithmic",
	"linear"
};

ਸਥਿਰ ਖਾਲੀ sched_debug_ਸਿਰਲੇਖ(ਬਣਤਰ seq_file *m)
{
	u64 ktime, sched_clk, cpu_clk;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	local_irq_save(flags);
	ktime = ktime_ਮੰਜ਼ਿਲ_ns(ktime_get());
	sched_clk = sched_clock();
	cpu_clk = local_clock();
	local_irq_restore(flags);

	SEQ_printf(m, "Sched Debug Version: v0.11, %s %.*s\n",
		init_utsname()->release,
		(ਪੂਰਨ)strcspn(init_utsname()->ਸੰਸਕਰਣ, " "),
		init_utsname()->ਸੰਸਕਰਣ);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ P(x) \
	SEQ_printf(m, "%-40s: %Ld\n", #x, (ਲੰਮਾ ਲੰਮਾ)(x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PN(x) \
	SEQ_printf(m, "%-40s: %Ld.%06ld\n", #x, ਵੰਡੋ_NS(x))
	PN(ktime);
	PN(sched_clk);
	PN(cpu_clk);
	P(jiffies);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_HAVE_UNSTABLE_SCHED_CLOCK
	P(sched_clock_stable());
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PN
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ P

	SEQ_printf(m, "\n");
	SEQ_printf(m, "sysctl_sched\n");

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ P(x) \
	SEQ_printf(m, "  .%-40s: %Ld\n", #x, (ਲੰਮਾ ਲੰਮਾ)(x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PN(x) \
	SEQ_printf(m, "  .%-40s: %Ld.%06ld\n", #x, ਵੰਡੋ_NS(x))
	PN(sysctl_sched_ਅਧਾਰ_ਟੁਕੜਾ);
	P(sysctl_sched_features);
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PN
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ P

	SEQ_printf(m, "  .%-40s: %d (%s)\n",
		"sysctl_sched_tunable_scaling",
		sysctl_sched_tunable_scaling,
		sched_tunable_scaling_ਨਾਂ[sysctl_sched_tunable_scaling]);
	SEQ_printf(m, "\n");
}

ਸਥਿਰ ਪੂਰਨ sched_debug_show(ਬਣਤਰ seq_file *m, ਖਾਲੀ *v)
{
	ਪੂਰਨ cpu = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)(v - 2);

	ਜੇ (cpu != -1)
		print_cpu(m, cpu);
	ਨਹੀਂਤਾਂ
		sched_debug_ਸਿਰਲੇਖ(m);

	ਵਾਪਸ 0;
}

ਖਾਲੀ sysrq_sched_debug_show(ਖਾਲੀ)
{
	ਪੂਰਨ cpu;

	sched_debug_ਸਿਰਲੇਖ(ਨਿਰਾ);
	for_each_online_cpu(cpu) {
		/*
		 * Need to reset softlockup watchdogs on all CPUs, because
		 * another CPU might be blocked waiting for us to process
		 * an IPI or stop_machine.
		 */
		touch_nmi_watchdog();
		touch_all_softlockup_watchdogs();
		print_cpu(ਨਿਰਾ, cpu);
	}
}

/*
 * This iterator needs some explanation.
 * It returns 1 for the header position.
 * This means 2 is CPU 0.
 * In a hotplugged system some CPUs, including CPU 0, may be missing so we have
 * to use cpumask_* to iterate over the CPUs.
 */
ਸਥਿਰ ਖਾਲੀ *sched_debug_ਸ਼ੁਰੂ(ਬਣਤਰ seq_file *file, loff_t *ਵਿੱਥ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ n = *ਵਿੱਥ;

	ਜੇ (n == 0)
		ਵਾਪਸ (ਖਾਲੀ *) 1;

	n--;

	ਜੇ (n > 0)
		n = cpumask_ਅਗਲਾ(n - 1, cpu_online_ਨਕਾਬ);
	ਨਹੀਂਤਾਂ
		n = cpumask_ਪਹਿਲਾ(cpu_online_ਨਕਾਬ);

	*ਵਿੱਥ = n + 1;

	ਜੇ (n < nr_cpu_ids)
		ਵਾਪਸ (ਖਾਲੀ *)(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)(n + 2);

	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ *sched_debug_ਅਗਲਾ(ਬਣਤਰ seq_file *file, ਖਾਲੀ *ਅੰਕੜੇ, loff_t *ਵਿੱਥ)
{
	(*ਵਿੱਥ)++;
	ਵਾਪਸ sched_debug_ਸ਼ੁਰੂ(file, ਵਿੱਥ);
}

ਸਥਿਰ ਖਾਲੀ sched_debug_stop(ਬਣਤਰ seq_file *file, ਖਾਲੀ *ਅੰਕੜੇ)
{
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ seq_operations sched_debug_sops = {
	.ਸ਼ੁਰੂ		= sched_debug_ਸ਼ੁਰੂ,
	.ਅਗਲਾ		= sched_debug_ਅਗਲਾ,
	.stop		= sched_debug_stop,
	.show		= sched_debug_show,
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __PS(S, F) SEQ_printf(m, "%-45s:%21Ld\n", S, (ਲੰਮਾ ਲੰਮਾ)(F))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __P(F) __PS(#F, F)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ   P(F) __PS(#F, p->F)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ   PM(F, M) __PS(#F, p->F & (M))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __PSN(S, F) SEQ_printf(m, "%-45s:%14Ld.%06ld\n", S, ਵੰਡੋ_NS((ਲੰਮਾ ਲੰਮਾ)(F)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __PN(F) __PSN(#F, F)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ   PN(F) __PSN(#F, p->F)


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
ਖਾਲੀ print_numa_stats(ਬਣਤਰ seq_file *m, ਪੂਰਨ ਗੰਢ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ tsf,
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ tpf, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ gsf, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ gpf)
{
	SEQ_printf(m, "numa_faults node=%d ", ਗੰਢ);
	SEQ_printf(m, "task_private=%lu task_shared=%lu ", tpf, tsf);
	SEQ_printf(m, "group_private=%lu group_shared=%lu\n", gpf, gsf);
}
#ਜੇ_ਅੰਤ


ਸਥਿਰ ਖਾਲੀ sched_show_numa(ਬਣਤਰ task_struct *p, ਬਣਤਰ seq_file *m)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
	ਜੇ (p->mm)
		P(mm->numa_ਜਾਂਚ_seq);

	P(numa_ਪੰਨੇ_migrated);
	P(numa_preferred_nid);
	P(ਕੁੱਲ_numa_ਨੁਕਸ);
	SEQ_printf(m, "current_node=%d, numa_group_id=%d\n",
			task_ਗੰਢ(p), task_numa_ਸਮੂਹ_id(p));
	show_numa_stats(p, m);
#ਜੇ_ਅੰਤ /* CONFIG_NUMA_BALANCING */
}

ਖਾਲੀ proc_sched_show_task(ਬਣਤਰ task_struct *p, ਬਣਤਰ pid_namespace *ns,
						  ਬਣਤਰ seq_file *m)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ nr_switches;

	SEQ_printf(m, "%s (%d, #threads: %d)\n", p->comm, task_pid_nr_ns(p, ns),
						get_nr_ਧਾਗੇ(p));
	SEQ_printf(m,
		"---------------------------------------------------------"
		"----------\n");

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ P_SCHEDSTAT(F)  __PS(#F, schedstat_val(p->stats.F))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PN_SCHEDSTAT(F) __PSN(#F, schedstat_val(p->stats.F))

	PN(se.exec_ਸ਼ੁਰੂ);
	PN(se.vruntime);
	PN(se.ਜੋੜ_exec_runtime);

	nr_switches = p->nvcsw + p->nivcsw;

	P(se.nr_migrations);

	ਜੇ (schedstat_enabled()) {
		u64 avg_atom, avg_ਪ੍ਰਤੀ_cpu;

		PN_SCHEDSTAT(ਜੋੜ_sleep_runtime);
		PN_SCHEDSTAT(ਜੋੜ_ਖੰਡ_runtime);
		PN_SCHEDSTAT(wait_ਸ਼ੁਰੂ);
		PN_SCHEDSTAT(sleep_ਸ਼ੁਰੂ);
		PN_SCHEDSTAT(ਖੰਡ_ਸ਼ੁਰੂ);
		PN_SCHEDSTAT(sleep_max);
		PN_SCHEDSTAT(ਖੰਡ_max);
		PN_SCHEDSTAT(exec_max);
		PN_SCHEDSTAT(ਟੁਕੜਾ_max);
		PN_SCHEDSTAT(wait_max);
		PN_SCHEDSTAT(wait_ਜੋੜ);
		P_SCHEDSTAT(wait_ਗਿਣਤੀ);
		PN_SCHEDSTAT(iowait_ਜੋੜ);
		P_SCHEDSTAT(iowait_ਗਿਣਤੀ);
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

		avg_atom = p->se.ਜੋੜ_exec_runtime;
		ਜੇ (nr_switches)
			avg_atom = div64_ul(avg_atom, nr_switches);
		ਨਹੀਂਤਾਂ
			avg_atom = -1LL;

		avg_ਪ੍ਰਤੀ_cpu = p->se.ਜੋੜ_exec_runtime;
		ਜੇ (p->se.nr_migrations) {
			avg_ਪ੍ਰਤੀ_cpu = div64_u64(avg_ਪ੍ਰਤੀ_cpu,
						p->se.nr_migrations);
		} ਨਹੀਂਤਾਂ {
			avg_ਪ੍ਰਤੀ_cpu = -1LL;
		}

		__PN(avg_atom);
		__PN(avg_ਪ੍ਰਤੀ_cpu);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CORE
		PN_SCHEDSTAT(core_forceidle_ਜੋੜ);
#ਜੇ_ਅੰਤ
	}

	__P(nr_switches);
	__PS("nr_voluntary_switches", p->nvcsw);
	__PS("nr_involuntary_switches", p->nivcsw);

	P(se.ਲੱਦੋ.ਭਾਰ);
	P(se.avg.ਲੱਦੋ_ਜੋੜ);
	P(se.avg.runnable_ਜੋੜ);
	P(se.avg.util_ਜੋੜ);
	P(se.avg.ਲੱਦੋ_avg);
	P(se.avg.runnable_avg);
	P(se.avg.util_avg);
	P(se.avg.ਆਖਰੀ_update_ਸਮਾਂ);
	PM(se.avg.util_est, ~UTIL_AVG_UNCHANGED);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK
	__PS("uclamp.min", p->uclamp_req[UCLAMP_MIN].ਮੁੱਲ);
	__PS("uclamp.max", p->uclamp_req[UCLAMP_MAX].ਮੁੱਲ);
	__PS("effective uclamp.min", uclamp_eff_ਮੁੱਲ(p, UCLAMP_MIN));
	__PS("effective uclamp.max", uclamp_eff_ਮੁੱਲ(p, UCLAMP_MAX));
#ਜੇ_ਅੰਤ /* CONFIG_UCLAMP_TASK */
	P(policy);
	P(prio);
	ਜੇ (task_has_dl_policy(p)) {
		P(dl.runtime);
		P(dl.deadline);
	} ਨਹੀਂਤਾਂ ਜੇ (fair_policy(p->policy)) {
		P(se.ਟੁਕੜਾ);
	}
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CLASS_EXT
	__PS("ext.enabled", task_on_scx(p));
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PN_SCHEDSTAT
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ P_SCHEDSTAT

	{
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ this_cpu = raw_smp_processor_id();
		u64 t0, t1;

		t0 = cpu_clock(this_cpu);
		t1 = cpu_clock(this_cpu);
		__PS("clock-delta", t1-t0);
	}

	sched_show_numa(p, m);
}

ਖਾਲੀ proc_sched_ਸੈੱਟ_task(ਬਣਤਰ task_struct *p)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHEDSTATS
	memset(&p->stats, 0, ਆਕਾਰ(p->stats));
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ resched_latency_warn(ਪੂਰਨ cpu, u64 latency)
{
	ਸਥਿਰ DEFINE_RATELIMIT_STATE(latency_check_ratelimit, 60 * 60 * HZ, 1);

	ਜੇ (likely(!__ratelimit(&latency_check_ratelimit)))
		ਵਾਪਸ;

	pr_err("sched: CPU %d need_resched set for > %llu ns (%d ticks) without schedule\n",
	       cpu, latency, cpu_rq(cpu)->ticks_without_resched);
	dump_ਢੇਰ();
}
