// SPDX-License-Identifier: GPL-2.0-only
/*
 * kernel/sched/debug.c
 *
 * Print the CFS rbtree and other debugging details
 *
 * Copyright(C) 2007, Red Hat, Inc., Ingo Molnar
 */
#అన్తర్భావ <linux/debugfs.h>
#అన్తర్భావ <linux/nmi.h>
#అన్తర్భావ <linux/log2.h>
#అన్తర్భావ "sched.h"

/*
 * This allows printing both to /sys/kernel/debug/sched/debug and
 * to the console
 */
#నిర్వచించు SEQ_printf(m, x...)			\
 చేయి {						\
	ఒకవేళ (m)					\
		seq_printf(m, x);		\
	లేకపోతే					\
		pr_cont(x);			\
 } అయితే (0)

/*
 * Ease the printing of nsec fields:
 */
స్థిర దీర్ఘ దీర్ఘ nsec_ఎగువ(గుర్తులేని దీర్ఘ దీర్ఘ nsec)
{
	ఒకవేళ ((దీర్ఘ దీర్ఘ)nsec < 0) {
		nsec = -nsec;
		do_div(nsec, 1000000);
		తిరిగి -nsec;
	}
	do_div(nsec, 1000000);

	తిరిగి nsec;
}

స్థిర గుర్తులేని దీర్ఘ nsec_దిగువ(గుర్తులేని దీర్ఘ దీర్ఘ nsec)
{
	ఒకవేళ ((దీర్ఘ దీర్ఘ)nsec < 0)
		nsec = -nsec;

	తిరిగి do_div(nsec, 1000000);
}

#నిర్వచించు విభజించు_NS(x) nsec_ఎగువ(x), nsec_దిగువ(x)

#నిర్వచించు SCHED_FEAT(name, enabled)	\
	#name ,

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి sched_feat_పేర్లు[] = {
#అన్తర్భావ "features.h"
};

#నిర్వచనం_రద్దు SCHED_FEAT

స్థిర పూర్ణాంకం sched_feat_show(నిర్మాణం seq_file *m, శూన్య *v)
{
	పూర్ణాంకం i;

	కొరకు (i = 0; i < __SCHED_FEAT_NR; i++) {
		ఒకవేళ (!(sysctl_sched_features & (1UL << i)))
			seq_puts(m, "NO_");
		seq_printf(m, "%s ", sched_feat_పేర్లు[i]);
	}
	seq_puts(m, "\n");

	తిరిగి 0;
}

#నిర్వచితమైతే CONFIG_JUMP_LABEL

#నిర్వచించు jump_label_కీలకం_true  { .కీలకం_true  = STATIC_కీలకం_TRUE_INIT  }
#నిర్వచించు jump_label_కీలకం_false { .కీలకం_false = STATIC_కీలకం_FALSE_INIT }

#నిర్వచించు SCHED_FEAT(name, enabled)	\
	jump_label_కీలకం##enabled ,

సంఘం sched_feat_కీలకం sched_feat_కీలకాలు[__SCHED_FEAT_NR] = {
#అన్తర్భావ "features.h"
};

#నిర్వచనం_రద్దు SCHED_FEAT

స్థిర శూన్య sched_feat_disable(పూర్ణాంకం i)
{
	static_branch_disable_cpuslocked(&sched_feat_కీలకాలు[i].కీలకం_true);
}

స్థిర శూన్య sched_feat_enable(పూర్ణాంకం i)
{
	static_branch_enable_cpuslocked(&sched_feat_కీలకాలు[i].కీలకం_false);
}
#లేకపోతే /* !CONFIG_JUMP_LABEL: */
స్థిర శూన్య sched_feat_disable(పూర్ణాంకం i) { };
స్థిర శూన్య sched_feat_enable(పూర్ణాంకం i) { };
#అయితే_ముగింపు /* !CONFIG_JUMP_LABEL */

స్థిర పూర్ణాంకం sched_feat_సమితి(అక్షరం *cmp)
{
	పూర్ణాంకం i;
	పూర్ణాంకం neg = 0;

	ఒకవేళ (అక్షరశ్రేణి_పోలిక(cmp, "NO_", 3) == 0) {
		neg = 1;
		cmp += 3;
	}

	i = సరిపోలిక_string(sched_feat_పేర్లు, __SCHED_FEAT_NR, cmp);
	ఒకవేళ (i < 0)
		తిరిగి i;

	ఒకవేళ (neg) {
		sysctl_sched_features &= ~(1UL << i);
		sched_feat_disable(i);
	} లేకపోతే {
		sysctl_sched_features |= (1UL << i);
		sched_feat_enable(i);
	}

	తిరిగి 0;
}

స్థిర ssize_t
sched_feat_write(నిర్మాణం file *filp, స్థిరరాశి అక్షరం __user *ubuf,
		కొలత_t cnt, loff_t *ppos)
{
	అక్షరం buf[64];
	అక్షరం *cmp;
	పూర్ణాంకం ret;
	నిర్మాణం inode *inode;

	ఒకవేళ (cnt > 63)
		cnt = 63;

	ఒకవేళ (copy_from_user(buf, ubuf, cnt))
		తిరిగి -EFAULT;

	buf[cnt] = 0;
	cmp = strstrip(buf);

	/* Ensure the static_key remains in a consistent state */
	inode = file_inode(filp);
	cpus_read_తాళం();
	inode_తాళం(inode);
	ret = sched_feat_సమితి(cmp);
	inode_unlock(inode);
	cpus_read_unlock();
	ఒకవేళ (ret < 0)
		తిరిగి ret;

	*ppos += cnt;

	తిరిగి cnt;
}

స్థిర పూర్ణాంకం sched_feat_open(నిర్మాణం inode *inode, నిర్మాణం file *filp)
{
	తిరిగి single_open(filp, sched_feat_show, ఖాళీ);
}

స్థిర స్థిరరాశి నిర్మాణం file_operations sched_feat_fops = {
	.open		= sched_feat_open,
	.లిఖ		= sched_feat_write,
	.పఠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

స్థిర ssize_t sched_scaling_write(నిర్మాణం file *filp, స్థిరరాశి అక్షరం __user *ubuf,
				   కొలత_t cnt, loff_t *ppos)
{
	గుర్తులేని పూర్ణాంకం scaling;
	పూర్ణాంకం ret;

	ret = kstrtouint_from_user(ubuf, cnt, 10, &scaling);
	ఒకవేళ (ret)
		తిరిగి ret;

	ఒకవేళ (scaling >= SCHED_TUNABLESCALING_చివర)
		తిరిగి -EINVAL;

	sysctl_sched_tunable_scaling = scaling;
	ఒకవేళ (sched_update_scaling())
		తిరిగి -EINVAL;

	*ppos += cnt;
	తిరిగి cnt;
}

స్థిర పూర్ణాంకం sched_scaling_show(నిర్మాణం seq_file *m, శూన్య *v)
{
	seq_printf(m, "%d\n", sysctl_sched_tunable_scaling);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం sched_scaling_open(నిర్మాణం inode *inode, నిర్మాణం file *filp)
{
	తిరిగి single_open(filp, sched_scaling_show, ఖాళీ);
}

స్థిర స్థిరరాశి నిర్మాణం file_operations sched_scaling_fops = {
	.open		= sched_scaling_open,
	.లిఖ		= sched_scaling_write,
	.పఠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#నిర్వచితమైతే CONFIG_SCHED_CACHE
స్థిర ssize_t
sched_cache_enable_write(నిర్మాణం file *filp, స్థిరరాశి అక్షరం __user *ubuf,
			 కొలత_t cnt, loff_t *ppos)
{
	bool val;
	పూర్ణాంకం ret;

	ret = kstrtobool_from_user(ubuf, cnt, &val);
	ఒకవేళ (ret)
		తిరిగి ret;

	sysctl_sched_cache_user = val;

	sched_cache_చురుకైన_సమితి();

	*ppos += cnt;

	తిరిగి cnt;
}

స్థిర పూర్ణాంకం sched_cache_enable_show(నిర్మాణం seq_file *m, శూన్య *v)
{
	seq_printf(m, "%d\n", sysctl_sched_cache_user);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం sched_cache_enable_open(నిర్మాణం inode *inode,
				   నిర్మాణం file *filp)
{
	తిరిగి single_open(filp, sched_cache_enable_show, ఖాళీ);
}

స్థిర స్థిరరాశి నిర్మాణం file_operations sched_cache_enable_fops = {
	.open           = sched_cache_enable_open,
	.లిఖ          = sched_cache_enable_write,
	.పఠ           = seq_read,
	.llseek         = seq_lseek,
	.release        = single_release,
};
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_PREEMPT_DYNAMIC

స్థిర ssize_t sched_dynamic_write(నిర్మాణం file *filp, స్థిరరాశి అక్షరం __user *ubuf,
				   కొలత_t cnt, loff_t *ppos)
{
	అక్షరం buf[16];
	పూర్ణాంకం బహుళకం;

	ఒకవేళ (cnt > 15)
		cnt = 15;

	ఒకవేళ (copy_from_user(buf, ubuf, cnt))
		తిరిగి -EFAULT;

	buf[cnt] = 0;
	బహుళకం = sched_dynamic_బహుళకం(strstrip(buf));
	ఒకవేళ (బహుళకం < 0)
		తిరిగి బహుళకం;

	sched_dynamic_update(బహుళకం);

	*ppos += cnt;

	తిరిగి cnt;
}

స్థిర పూర్ణాంకం sched_dynamic_show(నిర్మాణం seq_file *m, శూన్య *v)
{
	పూర్ణాంకం బహుళకం = READ_ONCE(preempt_dynamic_బహుళకం);

	/* Stop at NULL terminator */
	కొరకు (పూర్ణాంకం i = 0; preempt_modes[i]; i++) {
		ఒకవేళ (బహుళకం == i)
			seq_puts(m, "(");
		seq_puts(m, preempt_modes[i]);
		ఒకవేళ (బహుళకం == i)
			seq_puts(m, ")");

		seq_puts(m, " ");
	}

	seq_puts(m, "\n");
	తిరిగి 0;
}

స్థిర పూర్ణాంకం sched_dynamic_open(నిర్మాణం inode *inode, నిర్మాణం file *filp)
{
	తిరిగి single_open(filp, sched_dynamic_show, ఖాళీ);
}

స్థిర స్థిరరాశి నిర్మాణం file_operations sched_dynamic_fops = {
	.open		= sched_dynamic_open,
	.లిఖ		= sched_dynamic_write,
	.పఠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#అయితే_ముగింపు /* CONFIG_PREEMPT_DYNAMIC */

__read_mostly bool sched_debug_verbose;

స్థిర నిర్మాణం dentry           *sd_dentry;


స్థిర ssize_t sched_verbose_write(నిర్మాణం file *filp, స్థిరరాశి అక్షరం __user *ubuf,
				  కొలత_t cnt, loff_t *ppos)
{
	ssize_t ఫలితం;
	bool orig;

	cpus_read_తాళం();
	sched_domains_mutex_తాళం();

	orig = sched_debug_verbose;
	ఫలితం = debugfs_write_file_bool(filp, ubuf, cnt, ppos);

	ఒకవేళ (sched_debug_verbose && !orig)
		update_sched_domain_debugfs();
	లేకపోతే ఒకవేళ (!sched_debug_verbose && orig) {
		debugfs_remove(sd_dentry);
		sd_dentry = ఖాళీ;
	}

	sched_domains_mutex_unlock();
	cpus_read_unlock();

	తిరిగి ఫలితం;
}

స్థిర స్థిరరాశి నిర్మాణం file_operations sched_verbose_fops = {
	.పఠ =         debugfs_read_file_bool,
	.లిఖ =        sched_verbose_write,
	.open =         simple_open,
	.llseek =       default_llseek,
};

స్థిర స్థిరరాశి నిర్మాణం seq_operations sched_debug_sops;

స్థిర పూర్ణాంకం sched_debug_open(నిర్మాణం inode *inode, నిర్మాణం file *filp)
{
	తిరిగి seq_open(filp, &sched_debug_sops);
}

స్థిర స్థిరరాశి నిర్మాణం file_operations sched_debug_fops = {
	.open		= sched_debug_open,
	.పఠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

గణన dl_param {
	DL_RUNTIME = 0,
	DL_PERIOD,
};

స్థిర గుర్తులేని దీర్ఘ dl_server_period_max = (1UL << 22) * NSEC_చొప్పున_USEC; /* ~4 seconds */
స్థిర గుర్తులేని దీర్ఘ dl_server_period_min = (100) * NSEC_చొప్పున_USEC;     /* 100 us */

స్థిర ssize_t sched_server_write_common(నిర్మాణం file *filp, స్థిరరాశి అక్షరం __user *ubuf,
					 కొలత_t cnt, loff_t *ppos, గణన dl_param param,
					 శూన్య *server)
{
	దీర్ఘ cpu = (దీర్ఘ) ((నిర్మాణం seq_file *) filp->వ్యక్తిగత_దత్తాంశం)->అంతరంగ;
	నిర్మాణం sched_dl_entity *dl_se = (నిర్మాణం sched_dl_entity *)server;
	u64 old_runtime, runtime, period;
	నిర్మాణం rq *rq = cpu_rq(cpu);
	పూర్ణాంకం retval = 0;
	కొలత_t err;
	u64 విలువ;

	err = kstrtoull_from_user(ubuf, cnt, 10, &విలువ);
	ఒకవేళ (err)
		తిరిగి err;

	scoped_guard (rq_తాళం_irqsave, rq) {
		old_runtime = runtime = dl_se->dl_runtime;
		period = dl_se->dl_period;

		ఎంపిక (param) {
		స్థితి DL_RUNTIME:
			ఒకవేళ (runtime == విలువ)
				ఆపు;
			runtime = విలువ;
			ఆపు;
		స్థితి DL_PERIOD:
			ఒకవేళ (విలువ == period)
				ఆపు;
			period = విలువ;
			ఆపు;
		}

		ఒకవేళ (runtime > period ||
		    period > dl_server_period_max ||
		    period < dl_server_period_min) {
			తిరిగి  -EINVAL;
		}

		ఒకవేళ (!cpu_online(cpu_యొక్క(rq)))
			తిరిగి -EBUSY;

		update_rq_clock(rq);
		dl_server_stop(dl_se);
		retval = dl_server_apply_params(dl_se, runtime, period, 0);
		dl_server_ప్రారంభం(dl_se);

		ఒకవేళ (retval < 0)
			తిరిగి retval;
	}

	ఒకవేళ (!!old_runtime ^ !!runtime) {
		pr_info("%s server %sabled on CPU %d%s.\n",
			server == &rq->fair_server ? "Fair" : "Ext",
			runtime ? "en" : "dis",
			cpu_యొక్క(rq),
			runtime ? "" : ", system may malfunction due to starvation");
	}

	*ppos += cnt;
	తిరిగి cnt;
}

స్థిర కొలత_t sched_server_show_common(నిర్మాణం seq_file *m, శూన్య *v, గణన dl_param param,
				       శూన్య *server)
{
	నిర్మాణం sched_dl_entity *dl_se = (నిర్మాణం sched_dl_entity *)server;
	u64 విలువ;

	ఎంపిక (param) {
	స్థితి DL_RUNTIME:
		విలువ = dl_se->dl_runtime;
		ఆపు;
	స్థితి DL_PERIOD:
		విలువ = dl_se->dl_period;
		ఆపు;
	}

	seq_printf(m, "%llu\n", విలువ);
	తిరిగి 0;
}

స్థిర ssize_t
sched_fair_server_runtime_write(నిర్మాణం file *filp, స్థిరరాశి అక్షరం __user *ubuf,
				కొలత_t cnt, loff_t *ppos)
{
	దీర్ఘ cpu = (దీర్ఘ) ((నిర్మాణం seq_file *) filp->వ్యక్తిగత_దత్తాంశం)->అంతరంగ;
	నిర్మాణం rq *rq = cpu_rq(cpu);

	తిరిగి sched_server_write_common(filp, ubuf, cnt, ppos, DL_RUNTIME,
					&rq->fair_server);
}

స్థిర పూర్ణాంకం sched_fair_server_runtime_show(నిర్మాణం seq_file *m, శూన్య *v)
{
	గుర్తులేని దీర్ఘ cpu = (గుర్తులేని దీర్ఘ) m->అంతరంగ;
	నిర్మాణం rq *rq = cpu_rq(cpu);

	తిరిగి sched_server_show_common(m, v, DL_RUNTIME, &rq->fair_server);
}

స్థిర పూర్ణాంకం sched_fair_server_runtime_open(నిర్మాణం inode *inode, నిర్మాణం file *filp)
{
	తిరిగి single_open(filp, sched_fair_server_runtime_show, inode->i_వ్యక్తిగత);
}

స్థిర స్థిరరాశి నిర్మాణం file_operations fair_server_runtime_fops = {
	.open		= sched_fair_server_runtime_open,
	.లిఖ		= sched_fair_server_runtime_write,
	.పఠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

స్థిర నిర్మాణం dentry *debugfs_sched;

#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
స్థిర ssize_t
sched_ext_server_runtime_write(నిర్మాణం file *filp, స్థిరరాశి అక్షరం __user *ubuf,
			       కొలత_t cnt, loff_t *ppos)
{
	దీర్ఘ cpu = (దీర్ఘ) ((నిర్మాణం seq_file *) filp->వ్యక్తిగత_దత్తాంశం)->అంతరంగ;
	నిర్మాణం rq *rq = cpu_rq(cpu);

	తిరిగి sched_server_write_common(filp, ubuf, cnt, ppos, DL_RUNTIME,
					&rq->ext_server);
}

స్థిర పూర్ణాంకం sched_ext_server_runtime_show(నిర్మాణం seq_file *m, శూన్య *v)
{
	గుర్తులేని దీర్ఘ cpu = (గుర్తులేని దీర్ఘ) m->అంతరంగ;
	నిర్మాణం rq *rq = cpu_rq(cpu);

	తిరిగి sched_server_show_common(m, v, DL_RUNTIME, &rq->ext_server);
}

స్థిర పూర్ణాంకం sched_ext_server_runtime_open(నిర్మాణం inode *inode, నిర్మాణం file *filp)
{
	తిరిగి single_open(filp, sched_ext_server_runtime_show, inode->i_వ్యక్తిగత);
}

స్థిర స్థిరరాశి నిర్మాణం file_operations ext_server_runtime_fops = {
	.open		= sched_ext_server_runtime_open,
	.లిఖ		= sched_ext_server_runtime_write,
	.పఠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

స్థిర ssize_t
sched_ext_server_period_write(నిర్మాణం file *filp, స్థిరరాశి అక్షరం __user *ubuf,
			      కొలత_t cnt, loff_t *ppos)
{
	దీర్ఘ cpu = (దీర్ఘ) ((నిర్మాణం seq_file *) filp->వ్యక్తిగత_దత్తాంశం)->అంతరంగ;
	నిర్మాణం rq *rq = cpu_rq(cpu);

	తిరిగి sched_server_write_common(filp, ubuf, cnt, ppos, DL_PERIOD,
					&rq->ext_server);
}

స్థిర పూర్ణాంకం sched_ext_server_period_show(నిర్మాణం seq_file *m, శూన్య *v)
{
	గుర్తులేని దీర్ఘ cpu = (గుర్తులేని దీర్ఘ) m->అంతరంగ;
	నిర్మాణం rq *rq = cpu_rq(cpu);

	తిరిగి sched_server_show_common(m, v, DL_PERIOD, &rq->ext_server);
}

స్థిర పూర్ణాంకం sched_ext_server_period_open(నిర్మాణం inode *inode, నిర్మాణం file *filp)
{
	తిరిగి single_open(filp, sched_ext_server_period_show, inode->i_వ్యక్తిగత);
}

స్థిర స్థిరరాశి నిర్మాణం file_operations ext_server_period_fops = {
	.open		= sched_ext_server_period_open,
	.లిఖ		= sched_ext_server_period_write,
	.పఠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

స్థిర శూన్య debugfs_ext_server_init(శూన్య)
{
	నిర్మాణం dentry *d_ext;
	గుర్తులేని దీర్ఘ cpu;

	d_ext = debugfs_create_dir("ext_server", debugfs_sched);
	ఒకవేళ (!d_ext)
		తిరిగి;

	for_each_possible_cpu(cpu) {
		నిర్మాణం dentry *d_cpu;
		అక్షరం buf[32];

		snprintf(buf, పరిమాణం(buf), "cpu%lu", cpu);
		d_cpu = debugfs_create_dir(buf, d_ext);

		debugfs_create_file("runtime", 0644, d_cpu, (శూన్య *) cpu, &ext_server_runtime_fops);
		debugfs_create_file("period", 0644, d_cpu, (శూన్య *) cpu, &ext_server_period_fops);
	}
}
#అయితే_ముగింపు /* CONFIG_SCHED_CLASS_EXT */

స్థిర ssize_t
sched_fair_server_period_write(నిర్మాణం file *filp, స్థిరరాశి అక్షరం __user *ubuf,
			       కొలత_t cnt, loff_t *ppos)
{
	దీర్ఘ cpu = (దీర్ఘ) ((నిర్మాణం seq_file *) filp->వ్యక్తిగత_దత్తాంశం)->అంతరంగ;
	నిర్మాణం rq *rq = cpu_rq(cpu);

	తిరిగి sched_server_write_common(filp, ubuf, cnt, ppos, DL_PERIOD,
					&rq->fair_server);
}

స్థిర పూర్ణాంకం sched_fair_server_period_show(నిర్మాణం seq_file *m, శూన్య *v)
{
	గుర్తులేని దీర్ఘ cpu = (గుర్తులేని దీర్ఘ) m->అంతరంగ;
	నిర్మాణం rq *rq = cpu_rq(cpu);

	తిరిగి sched_server_show_common(m, v, DL_PERIOD, &rq->fair_server);
}

స్థిర పూర్ణాంకం sched_fair_server_period_open(నిర్మాణం inode *inode, నిర్మాణం file *filp)
{
	తిరిగి single_open(filp, sched_fair_server_period_show, inode->i_వ్యక్తిగత);
}

స్థిర స్థిరరాశి నిర్మాణం file_operations fair_server_period_fops = {
	.open		= sched_fair_server_period_open,
	.లిఖ		= sched_fair_server_period_write,
	.పఠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

స్థిర శూన్య debugfs_fair_server_init(శూన్య)
{
	నిర్మాణం dentry *d_fair;
	గుర్తులేని దీర్ఘ cpu;

	d_fair = debugfs_create_dir("fair_server", debugfs_sched);
	ఒకవేళ (!d_fair)
		తిరిగి;

	for_each_possible_cpu(cpu) {
		నిర్మాణం dentry *d_cpu;
		అక్షరం buf[32];

		snprintf(buf, పరిమాణం(buf), "cpu%lu", cpu);
		d_cpu = debugfs_create_dir(buf, d_fair);

		debugfs_create_file("runtime", 0644, d_cpu, (శూన్య *) cpu, &fair_server_runtime_fops);
		debugfs_create_file("period", 0644, d_cpu, (శూన్య *) cpu, &fair_server_period_fops);
	}
}

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
స్థిర పూర్ణాంకం cgroup_బహుళకం = 2;

/* See __sched_cgroup_mode_update(). */
స్థిర స్థిరరాశి అక్షరం *cgroup_బహుళకం_str[] = {
	"up",
	"smp",
	"concur",
	"max",
	"tasks",
};

స్థిర పూర్ణాంకం sched_cgroup_బహుళకం(స్థిరరాశి అక్షరం *str)
{
	కొరకు (పూర్ణాంకం i = 0; i < ARRAY_కొలత(cgroup_బహుళకం_str); i++) {
		ఒకవేళ (!strcmp(str, cgroup_బహుళకం_str[i]))
			తిరిగి i;
	}
	తిరిగి -EINVAL;
}

స్థిర ssize_t sched_cgroup_write(నిర్మాణం file *filp, స్థిరరాశి అక్షరం __user *ubuf,
				   కొలత_t cnt, loff_t *ppos)
{
	అక్షరం buf[16];
	పూర్ణాంకం బహుళకం;

	ఒకవేళ (cnt > 15)
		cnt = 15;

	ఒకవేళ (copy_from_user(buf, ubuf, cnt))
		తిరిగి -EFAULT;

	buf[cnt] = 0;
	బహుళకం = sched_cgroup_బహుళకం(strstrip(buf));
	ఒకవేళ (బహుళకం < 0)
		తిరిగి బహుళకం;

	__sched_cgroup_mode_update(బహుళకం);
	WRITE_ONCE(cgroup_బహుళకం, బహుళకం);

	*ppos += cnt;
	తిరిగి cnt;
}

స్థిర పూర్ణాంకం sched_cgroup_show(నిర్మాణం seq_file *m, శూన్య *v)
{
	పూర్ణాంకం బహుళకం = READ_ONCE(cgroup_బహుళకం);

	కొరకు (పూర్ణాంకం i = 0; i < ARRAY_కొలత(cgroup_బహుళకం_str); i++) {
		ఒకవేళ (బహుళకం == i)
			seq_puts(m, "(");
		seq_puts(m, cgroup_బహుళకం_str[i]);
		ఒకవేళ (బహుళకం == i)
			seq_puts(m, ")");

		seq_puts(m, " ");
	}
	seq_puts(m, "\n");
	తిరిగి 0;
}

స్థిర పూర్ణాంకం sched_cgroup_open(నిర్మాణం inode *inode, నిర్మాణం file *filp)
{
	తిరిగి single_open(filp, sched_cgroup_show, ఖాళీ);
}

స్థిర స్థిరరాశి నిర్మాణం file_operations sched_cgroup_fops = {
	.open		= sched_cgroup_open,
	.లిఖ		= sched_cgroup_write,
	.పఠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};
#అయితే_ముగింపు

స్థిర __init పూర్ణాంకం sched_init_debug(శూన్య)
{
	నిర్మాణం dentry __maybe_unused *numa, *llc;

	debugfs_sched = debugfs_create_dir("sched", ఖాళీ);

	debugfs_create_file("features", 0644, debugfs_sched, ఖాళీ, &sched_feat_fops);
	debugfs_create_file_unsafe("verbose", 0644, debugfs_sched, &sched_debug_verbose, &sched_verbose_fops);
#నిర్వచితమైతే CONFIG_PREEMPT_DYNAMIC
	debugfs_create_file("preempt", 0644, debugfs_sched, ఖాళీ, &sched_dynamic_fops);
#అయితే_ముగింపు

	debugfs_create_u32("base_slice_ns", 0644, debugfs_sched, &sysctl_sched_పునాది_ముక్క);

	debugfs_create_u32("latency_warn_ms", 0644, debugfs_sched, &sysctl_resched_latency_warn_ms);
	debugfs_create_u32("latency_warn_once", 0644, debugfs_sched, &sysctl_resched_latency_warn_once);

	debugfs_create_file("tunable_scaling", 0644, debugfs_sched, ఖాళీ, &sched_scaling_fops);
	debugfs_create_u32("migration_cost_ns", 0644, debugfs_sched, &sysctl_sched_migration_ఖర్చు);
	debugfs_create_u32("nr_migrate", 0644, debugfs_sched, &sysctl_sched_nr_migrate);

	sched_domains_mutex_తాళం();
	update_sched_domain_debugfs();
	sched_domains_mutex_unlock();

#నిర్వచితమైతే CONFIG_NUMA_BALANCING
	numa = debugfs_create_dir("numa_balancing", debugfs_sched);

	debugfs_create_u32("scan_delay_ms", 0644, numa, &sysctl_numa_balancing_పరిశీలన_delay);
	debugfs_create_u32("scan_period_min_ms", 0644, numa, &sysctl_numa_balancing_పరిశీలన_period_min);
	debugfs_create_u32("scan_period_max_ms", 0644, numa, &sysctl_numa_balancing_పరిశీలన_period_max);
	debugfs_create_u32("scan_size_mb", 0644, numa, &sysctl_numa_balancing_పరిశీలన_కొలత);
	debugfs_create_u32("hot_threshold_ms", 0644, numa, &sysctl_numa_balancing_hot_పరిమితి);
#అయితే_ముగింపు /* CONFIG_NUMA_BALANCING */

#నిర్వచితమైతే CONFIG_SCHED_CACHE
	llc = debugfs_create_dir("llc_balancing", debugfs_sched);
	debugfs_create_file("enabled", 0644, llc, ఖాళీ,
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
#అయితే_ముగింపు

	debugfs_create_file("debug", 0444, debugfs_sched, ఖాళీ, &sched_debug_fops);

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
	debugfs_create_file("cgroup_mode", 0644, debugfs_sched, ఖాళీ, &sched_cgroup_fops);
#అయితే_ముగింపు

	debugfs_fair_server_init();
#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
	debugfs_ext_server_init();
#అయితే_ముగింపు

	తిరిగి 0;
}
late_initcall(sched_init_debug);

స్థిర cpumask_var_t		sd_sysctl_cpus;

స్థిర పూర్ణాంకం sd_flags_show(నిర్మాణం seq_file *m, శూన్య *v)
{
	గుర్తులేని దీర్ఘ flags = *(గుర్తులేని పూర్ణాంకం *)m->అంతరంగ;
	పూర్ణాంకం idx;

	for_each_సమితి_బిట్టు(idx, &flags, __SD_FLAG_CNT) {
		seq_puts(m, sd_flag_debug[idx].name);
		seq_puts(m, " ");
	}
	seq_puts(m, "\n");

	తిరిగి 0;
}

స్థిర పూర్ణాంకం sd_flags_open(నిర్మాణం inode *inode, నిర్మాణం file *file)
{
	తిరిగి single_open(file, sd_flags_show, inode->i_వ్యక్తిగత);
}

స్థిర స్థిరరాశి నిర్మాణం file_operations sd_flags_fops = {
	.open		= sd_flags_open,
	.పఠ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

స్థిర శూన్య రిజిస్టరు_sd(నిర్మాణం sched_domain *sd, నిర్మాణం dentry *మాతృ)
{
#నిర్వచించు SDM(type, బహుళకం, member)	\
	debugfs_create_##type(#member, బహుళకం, మాతృ, &sd->member)

	SDM(ulong, 0644, min_interval);
	SDM(ulong, 0644, max_interval);
	SDM(u64,   0644, max_newidle_lb_ఖర్చు);
	SDM(u32,   0644, busy_గుణకం);
	SDM(u32,   0644, imbalance_pct);
	SDM(u32,   0644, cache_nice_tries);
	SDM(str,   0444, name);

#నిర్వచనం_రద్దు SDM

	debugfs_create_file("flags", 0444, మాతృ, &sd->flags, &sd_flags_fops);
	debugfs_create_file("groups_flags", 0444, మాతృ, &sd->groups->flags, &sd_flags_fops);
	debugfs_create_u32("level", 0444, మాతృ, (u32 *)&sd->level);

	ఒకవేళ (sd->flags & SD_ASYM_PACKING)
		debugfs_create_u32("group_asym_prefer_cpu", 0444, మాతృ,
				   (u32 *)&sd->groups->asym_prefer_cpu);
}

శూన్య update_sched_domain_debugfs(శూన్య)
{
	పూర్ణాంకం cpu, i;

	/*
	 * This can unfortunately be invoked before sched_debug_init() creates
	 * the debug directory. Don't touch sd_sysctl_cpus until then.
	 */
	ఒకవేళ (!debugfs_sched)
		తిరిగి;

	ఒకవేళ (!sched_debug_verbose)
		తిరిగి;

	ఒకవేళ (!cpumask_అందుబాటు(sd_sysctl_cpus)) {
		ఒకవేళ (!alloc_cpumask_var(&sd_sysctl_cpus, GFP_KERNEL))
			తిరిగి;
		cpumask_copy(sd_sysctl_cpus, cpu_possible_ముసుగు);
	}

	ఒకవేళ (!sd_dentry) {
		sd_dentry = debugfs_create_dir("domains", debugfs_sched);

		/* rebuild sd_sysctl_cpus if empty since it gets cleared below */
		ఒకవేళ (cpumask_ఖాళీలు(sd_sysctl_cpus))
			cpumask_copy(sd_sysctl_cpus, cpu_online_ముసుగు);
	}

	for_each_cpu(cpu, sd_sysctl_cpus) {
		నిర్మాణం sched_domain *sd;
		నిర్మాణం dentry *d_cpu;
		అక్షరం buf[32];

		snprintf(buf, పరిమాణం(buf), "cpu%d", cpu);
		debugfs_lookup_and_remove(buf, sd_dentry);
		d_cpu = debugfs_create_dir(buf, sd_dentry);

		i = 0;
		for_each_domain(cpu, sd) {
			నిర్మాణం dentry *d_sd;

			snprintf(buf, పరిమాణం(buf), "domain%d", i);
			d_sd = debugfs_create_dir(buf, d_cpu);

			రిజిస్టరు_sd(sd, d_sd);
			i++;
		}

		__cpumask_clear_cpu(cpu, sd_sysctl_cpus);
	}
}

శూన్య dirty_sched_domain_sysctl(పూర్ణాంకం cpu)
{
	ఒకవేళ (cpumask_అందుబాటు(sd_sysctl_cpus))
		__cpumask_set_cpu(cpu, sd_sysctl_cpus);
}

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
స్థిర శూన్య print_cfs_సమూహం_stats(నిర్మాణం seq_file *m, పూర్ణాంకం cpu, నిర్మాణం task_సమూహం *tg)
{
	నిర్మాణం sched_entity *se = tg_se(tg, cpu);

#నిర్వచించు P(F)		SEQ_printf(m, "  .%-30s: %lld\n",	#F, (దీర్ఘ దీర్ఘ)F)
#నిర్వచించు P_SCHEDSTAT(F)	SEQ_printf(m, "  .%-30s: %lld\n",	\
		#F, (దీర్ఘ దీర్ఘ)schedstat_val(stats->F))
#నిర్వచించు PN(F)		SEQ_printf(m, "  .%-30s: %lld.%06ld\n", #F, విభజించు_NS((దీర్ఘ దీర్ఘ)F))
#నిర్వచించు PN_SCHEDSTAT(F)	SEQ_printf(m, "  .%-30s: %lld.%06ld\n", \
		#F, విభజించు_NS((దీర్ఘ దీర్ఘ)schedstat_val(stats->F)))

	ఒకవేళ (!se)
		తిరిగి;

	PN(se->exec_ప్రారంభం);
	PN(se->vruntime);
	PN(se->కూడిక_exec_runtime);

	ఒకవేళ (schedstat_enabled()) {
		నిర్మాణం sched_statistics *stats;
		stats = __schedstats_from_se(se);

		PN_SCHEDSTAT(wait_ప్రారంభం);
		PN_SCHEDSTAT(sleep_ప్రారంభం);
		PN_SCHEDSTAT(ఖండం_ప్రారంభం);
		PN_SCHEDSTAT(sleep_max);
		PN_SCHEDSTAT(ఖండం_max);
		PN_SCHEDSTAT(exec_max);
		PN_SCHEDSTAT(ముక్క_max);
		PN_SCHEDSTAT(wait_max);
		PN_SCHEDSTAT(wait_కూడిక);
		P_SCHEDSTAT(wait_లెక్క);
	}

	P(se->ఎక్కించు.బరువు);
	P(se->avg.ఎక్కించు_avg);
	P(se->avg.util_avg);
	P(se->avg.runnable_avg);

#నిర్వచనం_రద్దు PN_SCHEDSTAT
#నిర్వచనం_రద్దు PN
#నిర్వచనం_రద్దు P_SCHEDSTAT
#నిర్వచనం_రద్దు P
}
#అయితే_ముగింపు /* CONFIG_FAIR_GROUP_SCHED */

#నిర్వచితమైతే CONFIG_CGROUP_SCHED
స్థిర DEFINE_SPINLOCK(sched_debug_తాళం);
స్థిర అక్షరం సమూహం_మార్గం[మార్గం_MAX];

స్థిర శూన్య task_సమూహం_మార్గం(నిర్మాణం task_సమూహం *tg, అక్షరం *మార్గం, పూర్ణాంకం plen)
{
	ఒకవేళ (autogroup_మార్గం(tg, మార్గం, plen))
		తిరిగి;

	cgroup_మార్గం(tg->css.cgroup, మార్గం, plen);
}

/*
 * Only 1 SEQ_printf_task_group_path() caller can use the full length
 * group_path[] for cgroup path. Other simultaneous callers will have
 * to use a shorter stack buffer. A "..." suffix is appended at the end
 * of the stack buffer so that it will show up in case the output length
 * matches the given buffer size to indicate possible path name truncation.
 */
#నిర్వచించు SEQ_printf_task_సమూహం_మార్గం(m, tg, fmt...)			\
{									\
	ఒకవేళ (spin_trylock(&sched_debug_తాళం)) {				\
		task_సమూహం_మార్గం(tg, సమూహం_మార్గం, పరిమాణం(సమూహం_మార్గం));	\
		SEQ_printf(m, fmt, సమూహం_మార్గం);				\
		spin_unlock(&sched_debug_తాళం);				\
	} లేకపోతే {							\
		అక్షరం buf[128];						\
		అక్షరం *bufend = buf + పరిమాణం(buf) - 3;			\
		task_సమూహం_మార్గం(tg, buf, bufend - buf);			\
		strcpy(bufend - 1, "...");				\
		SEQ_printf(m, fmt, buf);				\
	}								\
}
#అయితే_ముగింపు

స్థిర శూన్య
print_task(నిర్మాణం seq_file *m, నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	ఒకవేళ (task_current(rq, p))
		SEQ_printf(m, ">R");
	లేకపోతే
		SEQ_printf(m, " %c", task_state_గమ్యం_char(p));

	SEQ_printf(m, " %15s %5d %10ld %9Ld.%06ld   %c   %9Ld.%06ld %c %9Ld.%06ld %9Ld.%06ld %9Ld   %5d ",
		p->comm, task_pid_nr(p),
		p->se.h_ఎక్కించు.బరువు,
		విభజించు_NS(p->se.vruntime),
		entity_eligible(&rq->cfs, &p->se) ? 'E' : 'N',
		విభజించు_NS(p->se.deadline),
		p->se.custom_ముక్క ? 'S' : ' ',
		విభజించు_NS(p->se.ముక్క),
		విభజించు_NS(p->se.కూడిక_exec_runtime),
		(దీర్ఘ దీర్ఘ)(p->nvcsw + p->nivcsw),
		p->prio);

	SEQ_printf(m, "%9lld.%06ld %9lld.%06ld %9lld.%06ld",
		విభజించు_NS(schedstat_val_or_zero(p->stats.wait_కూడిక)),
		విభజించు_NS(schedstat_val_or_zero(p->stats.కూడిక_sleep_runtime)),
		విభజించు_NS(schedstat_val_or_zero(p->stats.కూడిక_ఖండం_runtime)));

#నిర్వచితమైతే CONFIG_NUMA_BALANCING
	SEQ_printf(m, "   %d      %d", task_కణుపు(p), task_numa_సమూహం_id(p));
#అయితే_ముగింపు
#నిర్వచితమైతే CONFIG_CGROUP_SCHED
	SEQ_printf_task_సమూహం_మార్గం(m, task_సమూహం(p), "        %s")
#అయితే_ముగింపు

	SEQ_printf(m, "\n");
}

స్థిర శూన్య print_rq(నిర్మాణం seq_file *m, నిర్మాణం rq *rq, పూర్ణాంకం rq_cpu)
{
	నిర్మాణం task_struct *g, *p;

	SEQ_printf(m, "\n");
	SEQ_printf(m, "runnable tasks:\n");
	SEQ_printf(m, " S            task   PID     weight       vruntime   eligible    "
		   "deadline             slice          sum-exec      switches  "
		   "prio         wait-time        sum-sleep       sum-block"
#నిర్వచితమైతే CONFIG_NUMA_BALANCING
		   "  node   group-id"
#అయితే_ముగింపు
#నిర్వచితమైతే CONFIG_CGROUP_SCHED
		   "  group-path"
#అయితే_ముగింపు
		   "\n");
	SEQ_printf(m, "-------------------------------------------------------"
		   "------------------------------------------------------"
		   "------------------------------------------------------"
#నిర్వచితమైతే CONFIG_NUMA_BALANCING
		   "--------------"
#అయితే_ముగింపు
#నిర్వచితమైతే CONFIG_CGROUP_SCHED
		   "--------------"
#అయితే_ముగింపు
		   "\n");

	rcu_read_తాళం();
	for_each_process_thread(g, p) {
		ఒకవేళ (task_cpu(p) != rq_cpu)
			కొనసాగు;

		print_task(m, rq, p);
	}
	rcu_read_unlock();
}

శూన్య print_cfs_rq(నిర్మాణం seq_file *m, పూర్ణాంకం cpu, నిర్మాణం cfs_rq *cfs_rq)
{
	s64 ఎడమ_vruntime = -1, కుడి_vruntime = -1, ఎడమ_deadline = -1, విస్తరణ;
	s64 zero_vruntime = -1, కూడిక_w_vruntime = -1;
	u64 avruntime;
	నిర్మాణం sched_entity *చివరి, *మొదటి, *మూలం;
	నిర్మాణం rq *rq = cpu_rq(cpu);
	గుర్తులేని పూర్ణాంకం కూడిక_జరుపు;
	గుర్తులేని దీర్ఘ flags;
	u64 కూడిక_బరువు;

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
	SEQ_printf(m, "\n");
	SEQ_printf_task_సమూహం_మార్గం(m, cfs_rq->tg, "cfs_rq[%d]:%s\n", cpu);
#లేకపోతే
	SEQ_printf(m, "\n");
	SEQ_printf(m, "cfs_rq[%d]:\n", cpu);
#అయితే_ముగింపు

	raw_spin_rq_తాళం_irqsave(rq, flags);
	మూలం = __pick_root_entity(cfs_rq);
	ఒకవేళ (మూలం)
		ఎడమ_vruntime = మూలం->min_vruntime;
	మొదటి = __pick_first_entity(cfs_rq);
	ఒకవేళ (మొదటి)
		ఎడమ_deadline = మొదటి->deadline;
	చివరి = __pick_last_entity(cfs_rq);
	ఒకవేళ (చివరి)
		కుడి_vruntime = చివరి->vruntime;
	zero_vruntime = cfs_rq->zero_vruntime;
	కూడిక_w_vruntime = cfs_rq->కూడిక_w_vruntime;
	కూడిక_బరువు = cfs_rq->కూడిక_బరువు;
	కూడిక_జరుపు = cfs_rq->కూడిక_జరుపు;
	avruntime = avg_vruntime(cfs_rq);
	raw_spin_rq_unlock_irqrestore(rq, flags);

	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "left_deadline",
			విభజించు_NS(ఎడమ_deadline));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "left_vruntime",
			విభజించు_NS(ఎడమ_vruntime));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "zero_vruntime",
			విభజించు_NS(zero_vruntime));
	SEQ_printf(m, "  .%-30s: %Ld (%d bits)\n", "sum_w_vruntime",
		   కూడిక_w_vruntime, ilog2(నిరపేక్ష(కూడిక_w_vruntime)));
	SEQ_printf(m, "  .%-30s: %Lu\n", "sum_weight",
		   కూడిక_బరువు);
	SEQ_printf(m, "  .%-30s: %u\n", "sum_shift", కూడిక_జరుపు);
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "avg_vruntime",
			విభజించు_NS(avruntime));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "right_vruntime",
			విభజించు_NS(కుడి_vruntime));
	విస్తరణ = కుడి_vruntime - ఎడమ_vruntime;
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "spread", విభజించు_NS(విస్తరణ));
	SEQ_printf(m, "  .%-30s: %d\n", "nr_queued", cfs_rq->nr_queued);
	SEQ_printf(m, "  .%-30s: %d\n", "h_nr_runnable", cfs_rq->h_nr_runnable);
	SEQ_printf(m, "  .%-30s: %d\n", "h_nr_queued", cfs_rq->h_nr_queued);
	SEQ_printf(m, "  .%-30s: %d\n", "h_nr_idle", cfs_rq->h_nr_idle);
	SEQ_printf(m, "  .%-30s: %ld\n", "load", cfs_rq->ఎక్కించు.బరువు);
	SEQ_printf(m, "  .%-30s: %lu\n", "load_avg",
			cfs_rq->avg.ఎక్కించు_avg);
	SEQ_printf(m, "  .%-30s: %lu\n", "runnable_avg",
			cfs_rq->avg.runnable_avg);
	SEQ_printf(m, "  .%-30s: %lu\n", "util_avg",
			cfs_rq->avg.util_avg);
	SEQ_printf(m, "  .%-30s: %u\n", "util_est",
			cfs_rq->avg.util_est);
	SEQ_printf(m, "  .%-30s: %ld\n", "removed.load_avg",
			cfs_rq->removed.ఎక్కించు_avg);
	SEQ_printf(m, "  .%-30s: %ld\n", "removed.util_avg",
			cfs_rq->removed.util_avg);
	SEQ_printf(m, "  .%-30s: %ld\n", "removed.runnable_avg",
			cfs_rq->removed.runnable_avg);
#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
	SEQ_printf(m, "  .%-30s: %lu\n", "tg_load_avg_contrib",
			cfs_rq->tg_ఎక్కించు_avg_contrib);
	SEQ_printf(m, "  .%-30s: %ld\n", "tg_load_avg",
			atomic_long_read(&cfs_rq->tg->ఎక్కించు_avg));
	SEQ_printf(m, "  .%-30s: %lu\n", "h_load",
			cfs_rq->h_ఎక్కించు);
#అయితే_ముగింపు /* CONFIG_FAIR_GROUP_SCHED */
#నిర్వచితమైతే CONFIG_CFS_BANDWIDTH
	SEQ_printf(m, "  .%-30s: %d\n", "throttled",
			cfs_rq->throttled);
	SEQ_printf(m, "  .%-30s: %d\n", "throttle_count",
			cfs_rq->throttle_లెక్క);
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
	print_cfs_సమూహం_stats(m, cpu, cfs_rq->tg);
#అయితే_ముగింపు
}

శూన్య print_rt_rq(నిర్మాణం seq_file *m, పూర్ణాంకం cpu, నిర్మాణం rt_rq *rt_rq)
{
#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED
	SEQ_printf(m, "\n");
	SEQ_printf_task_సమూహం_మార్గం(m, rt_rq->tg, "rt_rq[%d]:%s\n", cpu);
#లేకపోతే
	SEQ_printf(m, "\n");
	SEQ_printf(m, "rt_rq[%d]:\n", cpu);
#అయితే_ముగింపు

#నిర్వచించు P(x) \
	SEQ_printf(m, "  .%-30s: %Ld\n", #x, (దీర్ఘ దీర్ఘ)(rt_rq->x))
#నిర్వచించు PU(x) \
	SEQ_printf(m, "  .%-30s: %lu\n", #x, (గుర్తులేని దీర్ఘ)(rt_rq->x))
#నిర్వచించు PN(x) \
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", #x, విభజించు_NS(rt_rq->x))

	PU(rt_nr_running);

#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED
	P(rt_throttled);
	PN(rt_సమయం);
	PN(rt_runtime);
#అయితే_ముగింపు

#నిర్వచనం_రద్దు PN
#నిర్వచనం_రద్దు PU
#నిర్వచనం_రద్దు P
}

శూన్య print_dl_rq(నిర్మాణం seq_file *m, పూర్ణాంకం cpu, నిర్మాణం dl_rq *dl_rq)
{
	నిర్మాణం dl_bw *dl_bw;

	SEQ_printf(m, "\n");
	SEQ_printf(m, "dl_rq[%d]:\n", cpu);

#నిర్వచించు PU(x) \
	SEQ_printf(m, "  .%-30s: %lu\n", #x, (గుర్తులేని దీర్ఘ)(dl_rq->x))

	PU(dl_nr_running);
	dl_bw = &cpu_rq(cpu)->rd->dl_bw;
	SEQ_printf(m, "  .%-30s: %lld\n", "dl_bw->bw", dl_bw->bw);
	SEQ_printf(m, "  .%-30s: %lld\n", "dl_bw->total_bw", dl_bw->మొత్తం_bw);

#నిర్వచనం_రద్దు PU
}

స్థిర శూన్య print_cpu(నిర్మాణం seq_file *m, పూర్ణాంకం cpu)
{
	నిర్మాణం rq *rq = cpu_rq(cpu);

#నిర్వచితమైతే CONFIG_X86
	{
		గుర్తులేని పూర్ణాంకం freq = cpu_khz ? : 1;

		SEQ_printf(m, "cpu#%d, %u.%03u MHz\n",
			   cpu, freq / 1000, (freq % 1000));
	}
#లేకపోతే /* !CONFIG_X86: */
	SEQ_printf(m, "cpu#%d\n", cpu);
#అయితే_ముగింపు /* !CONFIG_X86 */

#నిర్వచించు P(x)								\
చేయి {									\
	ఒకవేళ (పరిమాణం(rq->x) == 4)						\
		SEQ_printf(m, "  .%-30s: %d\n", #x, (పూర్ణాంకం)(rq->x));	\
	లేకపోతే								\
		SEQ_printf(m, "  .%-30s: %Ld\n", #x, (దీర్ఘ దీర్ఘ)(rq->x));\
} అయితే (0)

#నిర్వచించు PN(x) \
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", #x, విభజించు_NS(rq->x))

	P(nr_running);
	P(nr_switches);
	P(nr_uninterruptible);
	PN(తదుపరి_balance);
	SEQ_printf(m, "  .%-30s: %ld\n", "curr->pid", (దీర్ఘ)(task_pid_nr(rq->curr)));
	PN(clock);
	PN(clock_task);
#నిర్వచనం_రద్దు P
#నిర్వచనం_రద్దు PN

#నిర్వచించు P64(n) SEQ_printf(m, "  .%-30s: %Ld\n", #n, rq->n);
	P64(avg_idle);
	P64(max_idle_balance_ఖర్చు);
#నిర్వచనం_రద్దు P64

#నిర్వచించు P(n) SEQ_printf(m, "  .%-30s: %d\n", #n, schedstat_val(rq->n));
	ఒకవేళ (schedstat_enabled()) {
		P(yld_లెక్క);
		P(sched_లెక్క);
		P(sched_goidle);
		P(ttwu_లెక్క);
		P(ttwu_local);
	}
#నిర్వచనం_రద్దు P

	print_cfs_stats(m, cpu);
	print_rt_stats(m, cpu);
	print_dl_stats(m, cpu);

	print_rq(m, rq, cpu);
	SEQ_printf(m, "\n");
}

స్థిర స్థిరరాశి అక్షరం *sched_tunable_scaling_పేర్లు[] = {
	"none",
	"logarithmic",
	"linear"
};

స్థిర శూన్య sched_debug_శీర్షిక(నిర్మాణం seq_file *m)
{
	u64 ktime, sched_clk, cpu_clk;
	గుర్తులేని దీర్ఘ flags;

	local_irq_save(flags);
	ktime = ktime_గమ్యం_ns(ktime_get());
	sched_clk = sched_clock();
	cpu_clk = local_clock();
	local_irq_restore(flags);

	SEQ_printf(m, "Sched Debug Version: v0.11, %s %.*s\n",
		init_utsname()->release,
		(పూర్ణాంకం)strcspn(init_utsname()->సంస్కరణ, " "),
		init_utsname()->సంస్కరణ);

#నిర్వచించు P(x) \
	SEQ_printf(m, "%-40s: %Ld\n", #x, (దీర్ఘ దీర్ఘ)(x))
#నిర్వచించు PN(x) \
	SEQ_printf(m, "%-40s: %Ld.%06ld\n", #x, విభజించు_NS(x))
	PN(ktime);
	PN(sched_clk);
	PN(cpu_clk);
	P(jiffies);
#నిర్వచితమైతే CONFIG_HAVE_UNSTABLE_SCHED_CLOCK
	P(sched_clock_stable());
#అయితే_ముగింపు
#నిర్వచనం_రద్దు PN
#నిర్వచనం_రద్దు P

	SEQ_printf(m, "\n");
	SEQ_printf(m, "sysctl_sched\n");

#నిర్వచించు P(x) \
	SEQ_printf(m, "  .%-40s: %Ld\n", #x, (దీర్ఘ దీర్ఘ)(x))
#నిర్వచించు PN(x) \
	SEQ_printf(m, "  .%-40s: %Ld.%06ld\n", #x, విభజించు_NS(x))
	PN(sysctl_sched_పునాది_ముక్క);
	P(sysctl_sched_features);
#నిర్వచనం_రద్దు PN
#నిర్వచనం_రద్దు P

	SEQ_printf(m, "  .%-40s: %d (%s)\n",
		"sysctl_sched_tunable_scaling",
		sysctl_sched_tunable_scaling,
		sched_tunable_scaling_పేర్లు[sysctl_sched_tunable_scaling]);
	SEQ_printf(m, "\n");
}

స్థిర పూర్ణాంకం sched_debug_show(నిర్మాణం seq_file *m, శూన్య *v)
{
	పూర్ణాంకం cpu = (గుర్తులేని దీర్ఘ)(v - 2);

	ఒకవేళ (cpu != -1)
		print_cpu(m, cpu);
	లేకపోతే
		sched_debug_శీర్షిక(m);

	తిరిగి 0;
}

శూన్య sysrq_sched_debug_show(శూన్య)
{
	పూర్ణాంకం cpu;

	sched_debug_శీర్షిక(ఖాళీ);
	for_each_online_cpu(cpu) {
		/*
		 * Need to reset softlockup watchdogs on all CPUs, because
		 * another CPU might be blocked waiting for us to process
		 * an IPI or stop_machine.
		 */
		touch_nmi_watchdog();
		touch_all_softlockup_watchdogs();
		print_cpu(ఖాళీ, cpu);
	}
}

/*
 * This iterator needs some explanation.
 * It returns 1 for the header position.
 * This means 2 is CPU 0.
 * In a hotplugged system some CPUs, including CPU 0, may be missing so we have
 * to use cpumask_* to iterate over the CPUs.
 */
స్థిర శూన్య *sched_debug_ప్రారంభం(నిర్మాణం seq_file *file, loff_t *స్థానభ్రంశం)
{
	గుర్తులేని దీర్ఘ n = *స్థానభ్రంశం;

	ఒకవేళ (n == 0)
		తిరిగి (శూన్య *) 1;

	n--;

	ఒకవేళ (n > 0)
		n = cpumask_తదుపరి(n - 1, cpu_online_ముసుగు);
	లేకపోతే
		n = cpumask_మొదటి(cpu_online_ముసుగు);

	*స్థానభ్రంశం = n + 1;

	ఒకవేళ (n < nr_cpu_ids)
		తిరిగి (శూన్య *)(గుర్తులేని దీర్ఘ)(n + 2);

	తిరిగి ఖాళీ;
}

స్థిర శూన్య *sched_debug_తదుపరి(నిర్మాణం seq_file *file, శూన్య *దత్తాంశం, loff_t *స్థానభ్రంశం)
{
	(*స్థానభ్రంశం)++;
	తిరిగి sched_debug_ప్రారంభం(file, స్థానభ్రంశం);
}

స్థిర శూన్య sched_debug_stop(నిర్మాణం seq_file *file, శూన్య *దత్తాంశం)
{
}

స్థిర స్థిరరాశి నిర్మాణం seq_operations sched_debug_sops = {
	.ప్రారంభం		= sched_debug_ప్రారంభం,
	.తదుపరి		= sched_debug_తదుపరి,
	.stop		= sched_debug_stop,
	.show		= sched_debug_show,
};

#నిర్వచించు __PS(S, F) SEQ_printf(m, "%-45s:%21Ld\n", S, (దీర్ఘ దీర్ఘ)(F))
#నిర్వచించు __P(F) __PS(#F, F)
#నిర్వచించు   P(F) __PS(#F, p->F)
#నిర్వచించు   PM(F, M) __PS(#F, p->F & (M))
#నిర్వచించు __PSN(S, F) SEQ_printf(m, "%-45s:%14Ld.%06ld\n", S, విభజించు_NS((దీర్ఘ దీర్ఘ)(F)))
#నిర్వచించు __PN(F) __PSN(#F, F)
#నిర్వచించు   PN(F) __PSN(#F, p->F)


#నిర్వచితమైతే CONFIG_NUMA_BALANCING
శూన్య print_numa_stats(నిర్మాణం seq_file *m, పూర్ణాంకం కణుపు, గుర్తులేని దీర్ఘ tsf,
		గుర్తులేని దీర్ఘ tpf, గుర్తులేని దీర్ఘ gsf, గుర్తులేని దీర్ఘ gpf)
{
	SEQ_printf(m, "numa_faults node=%d ", కణుపు);
	SEQ_printf(m, "task_private=%lu task_shared=%lu ", tpf, tsf);
	SEQ_printf(m, "group_private=%lu group_shared=%lu\n", gpf, gsf);
}
#అయితే_ముగింపు


స్థిర శూన్య sched_show_numa(నిర్మాణం task_struct *p, నిర్మాణం seq_file *m)
{
#నిర్వచితమైతే CONFIG_NUMA_BALANCING
	ఒకవేళ (p->mm)
		P(mm->numa_పరిశీలన_seq);

	P(numa_పుటలు_migrated);
	P(numa_preferred_nid);
	P(మొత్తం_numa_దోషాలు);
	SEQ_printf(m, "current_node=%d, numa_group_id=%d\n",
			task_కణుపు(p), task_numa_సమూహం_id(p));
	show_numa_stats(p, m);
#అయితే_ముగింపు /* CONFIG_NUMA_BALANCING */
}

శూన్య proc_sched_show_task(నిర్మాణం task_struct *p, నిర్మాణం pid_namespace *ns,
						  నిర్మాణం seq_file *m)
{
	గుర్తులేని దీర్ఘ nr_switches;

	SEQ_printf(m, "%s (%d, #threads: %d)\n", p->comm, task_pid_nr_ns(p, ns),
						get_nr_దారాలు(p));
	SEQ_printf(m,
		"---------------------------------------------------------"
		"----------\n");

#నిర్వచించు P_SCHEDSTAT(F)  __PS(#F, schedstat_val(p->stats.F))
#నిర్వచించు PN_SCHEDSTAT(F) __PSN(#F, schedstat_val(p->stats.F))

	PN(se.exec_ప్రారంభం);
	PN(se.vruntime);
	PN(se.కూడిక_exec_runtime);

	nr_switches = p->nvcsw + p->nivcsw;

	P(se.nr_migrations);

	ఒకవేళ (schedstat_enabled()) {
		u64 avg_atom, avg_చొప్పున_cpu;

		PN_SCHEDSTAT(కూడిక_sleep_runtime);
		PN_SCHEDSTAT(కూడిక_ఖండం_runtime);
		PN_SCHEDSTAT(wait_ప్రారంభం);
		PN_SCHEDSTAT(sleep_ప్రారంభం);
		PN_SCHEDSTAT(ఖండం_ప్రారంభం);
		PN_SCHEDSTAT(sleep_max);
		PN_SCHEDSTAT(ఖండం_max);
		PN_SCHEDSTAT(exec_max);
		PN_SCHEDSTAT(ముక్క_max);
		PN_SCHEDSTAT(wait_max);
		PN_SCHEDSTAT(wait_కూడిక);
		P_SCHEDSTAT(wait_లెక్క);
		PN_SCHEDSTAT(iowait_కూడిక);
		P_SCHEDSTAT(iowait_లెక్క);
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

		avg_atom = p->se.కూడిక_exec_runtime;
		ఒకవేళ (nr_switches)
			avg_atom = div64_ul(avg_atom, nr_switches);
		లేకపోతే
			avg_atom = -1LL;

		avg_చొప్పున_cpu = p->se.కూడిక_exec_runtime;
		ఒకవేళ (p->se.nr_migrations) {
			avg_చొప్పున_cpu = div64_u64(avg_చొప్పున_cpu,
						p->se.nr_migrations);
		} లేకపోతే {
			avg_చొప్పున_cpu = -1LL;
		}

		__PN(avg_atom);
		__PN(avg_చొప్పున_cpu);

#నిర్వచితమైతే CONFIG_SCHED_CORE
		PN_SCHEDSTAT(core_forceidle_కూడిక);
#అయితే_ముగింపు
	}

	__P(nr_switches);
	__PS("nr_voluntary_switches", p->nvcsw);
	__PS("nr_involuntary_switches", p->nivcsw);

	P(se.ఎక్కించు.బరువు);
	P(se.avg.ఎక్కించు_కూడిక);
	P(se.avg.runnable_కూడిక);
	P(se.avg.util_కూడిక);
	P(se.avg.ఎక్కించు_avg);
	P(se.avg.runnable_avg);
	P(se.avg.util_avg);
	P(se.avg.చివరి_update_సమయం);
	PM(se.avg.util_est, ~UTIL_AVG_UNCHANGED);
#నిర్వచితమైతే CONFIG_UCLAMP_TASK
	__PS("uclamp.min", p->uclamp_req[UCLAMP_MIN].విలువ);
	__PS("uclamp.max", p->uclamp_req[UCLAMP_MAX].విలువ);
	__PS("effective uclamp.min", uclamp_eff_విలువ(p, UCLAMP_MIN));
	__PS("effective uclamp.max", uclamp_eff_విలువ(p, UCLAMP_MAX));
#అయితే_ముగింపు /* CONFIG_UCLAMP_TASK */
	P(policy);
	P(prio);
	ఒకవేళ (task_has_dl_policy(p)) {
		P(dl.runtime);
		P(dl.deadline);
	} లేకపోతే ఒకవేళ (fair_policy(p->policy)) {
		P(se.ముక్క);
	}
#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
	__PS("ext.enabled", task_on_scx(p));
#అయితే_ముగింపు
#నిర్వచనం_రద్దు PN_SCHEDSTAT
#నిర్వచనం_రద్దు P_SCHEDSTAT

	{
		గుర్తులేని పూర్ణాంకం this_cpu = raw_smp_processor_id();
		u64 t0, t1;

		t0 = cpu_clock(this_cpu);
		t1 = cpu_clock(this_cpu);
		__PS("clock-delta", t1-t0);
	}

	sched_show_numa(p, m);
}

శూన్య proc_sched_సమితి_task(నిర్మాణం task_struct *p)
{
#నిర్వచితమైతే CONFIG_SCHEDSTATS
	memset(&p->stats, 0, పరిమాణం(p->stats));
#అయితే_ముగింపు
}

శూన్య resched_latency_warn(పూర్ణాంకం cpu, u64 latency)
{
	స్థిర DEFINE_RATELIMIT_STATE(latency_check_ratelimit, 60 * 60 * HZ, 1);

	ఒకవేళ (likely(!__ratelimit(&latency_check_ratelimit)))
		తిరిగి;

	pr_err("sched: CPU %d need_resched set for > %llu ns (%d ticks) without schedule\n",
	       cpu, latency, cpu_rq(cpu)->ticks_without_resched);
	dump_దొంతర();
}
