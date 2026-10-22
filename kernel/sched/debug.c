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
#تعریف_کرو SEQ_printf(m, x...)			\
 کرو {						\
	اگر (m)					\
		seq_printf(m, x);		\
	ورنہ					\
		pr_cont(x);			\
 } جب (0)

/*
 * Ease the printing of nsec fields:
 */
ساکن طویل طویل nsec_بلند(غیرعلامتی طویل طویل nsec)
{
	اگر ((طویل طویل)nsec < 0) {
		nsec = -nsec;
		do_div(nsec, 1000000);
		واپس -nsec;
	}
	do_div(nsec, 1000000);

	واپس nsec;
}

ساکن غیرعلامتی طویل nsec_کم(غیرعلامتی طویل طویل nsec)
{
	اگر ((طویل طویل)nsec < 0)
		nsec = -nsec;

	واپس do_div(nsec, 1000000);
}

#تعریف_کرو بانٹو_NS(x) nsec_بلند(x), nsec_کم(x)

#تعریف_کرو SCHED_FEAT(name, enabled)	\
	#name ,

ساکن ثابت حرف * ثابت sched_feat_نام[] = {
#شامل "features.h"
};

#تعریف_منسوخ SCHED_FEAT

ساکن صحیح sched_feat_show(ساخت seq_file *m, خالی *v)
{
	صحیح i;

	برائے (i = 0; i < __SCHED_FEAT_NR; i++) {
		اگر (!(sysctl_sched_features & (1UL << i)))
			seq_puts(m, "NO_");
		seq_printf(m, "%s ", sched_feat_نام[i]);
	}
	seq_puts(m, "\n");

	واپس 0;
}

#اگر_تعریف_شدہ CONFIG_JUMP_LABEL

#تعریف_کرو jump_label_کلید_true  { .کلید_true  = STATIC_کلید_TRUE_INIT  }
#تعریف_کرو jump_label_کلید_false { .کلید_false = STATIC_کلید_FALSE_INIT }

#تعریف_کرو SCHED_FEAT(name, enabled)	\
	jump_label_کلید##enabled ,

اتحاد sched_feat_کلید sched_feat_کلیدیں[__SCHED_FEAT_NR] = {
#شامل "features.h"
};

#تعریف_منسوخ SCHED_FEAT

ساکن خالی sched_feat_disable(صحیح i)
{
	static_branch_disable_cpuslocked(&sched_feat_کلیدیں[i].کلید_true);
}

ساکن خالی sched_feat_enable(صحیح i)
{
	static_branch_enable_cpuslocked(&sched_feat_کلیدیں[i].کلید_false);
}
#ورنہ /* !CONFIG_JUMP_LABEL: */
ساکن خالی sched_feat_disable(صحیح i) { };
ساکن خالی sched_feat_enable(صحیح i) { };
#اگر_ختم /* !CONFIG_JUMP_LABEL */

ساکن صحیح sched_feat_سیٹ(حرف *cmp)
{
	صحیح i;
	صحیح neg = 0;

	اگر (لڑی_موازنہ(cmp, "NO_", 3) == 0) {
		neg = 1;
		cmp += 3;
	}

	i = مطابقت_string(sched_feat_نام, __SCHED_FEAT_NR, cmp);
	اگر (i < 0)
		واپس i;

	اگر (neg) {
		sysctl_sched_features &= ~(1UL << i);
		sched_feat_disable(i);
	} ورنہ {
		sysctl_sched_features |= (1UL << i);
		sched_feat_enable(i);
	}

	واپس 0;
}

ساکن ssize_t
sched_feat_write(ساخت file *filp, ثابت حرف __user *ubuf,
		سائز_t cnt, loff_t *ppos)
{
	حرف buf[64];
	حرف *cmp;
	صحیح ret;
	ساخت inode *inode;

	اگر (cnt > 63)
		cnt = 63;

	اگر (copy_from_user(buf, ubuf, cnt))
		واپس -EFAULT;

	buf[cnt] = 0;
	cmp = strstrip(buf);

	/* Ensure the static_key remains in a consistent state */
	inode = file_inode(filp);
	cpus_read_تالا();
	inode_تالا(inode);
	ret = sched_feat_سیٹ(cmp);
	inode_unlock(inode);
	cpus_read_unlock();
	اگر (ret < 0)
		واپس ret;

	*ppos += cnt;

	واپس cnt;
}

ساکن صحیح sched_feat_open(ساخت inode *inode, ساخت file *filp)
{
	واپس single_open(filp, sched_feat_show, تہی);
}

ساکن ثابت ساخت file_operations sched_feat_fops = {
	.open		= sched_feat_open,
	.اكتب		= sched_feat_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ساکن ssize_t sched_scaling_write(ساخت file *filp, ثابت حرف __user *ubuf,
				   سائز_t cnt, loff_t *ppos)
{
	غیرعلامتی صحیح scaling;
	صحیح ret;

	ret = kstrtouint_from_user(ubuf, cnt, 10, &scaling);
	اگر (ret)
		واپس ret;

	اگر (scaling >= SCHED_TUNABLESCALING_انتہا)
		واپس -EINVAL;

	sysctl_sched_tunable_scaling = scaling;
	اگر (sched_update_scaling())
		واپس -EINVAL;

	*ppos += cnt;
	واپس cnt;
}

ساکن صحیح sched_scaling_show(ساخت seq_file *m, خالی *v)
{
	seq_printf(m, "%d\n", sysctl_sched_tunable_scaling);
	واپس 0;
}

ساکن صحیح sched_scaling_open(ساخت inode *inode, ساخت file *filp)
{
	واپس single_open(filp, sched_scaling_show, تہی);
}

ساکن ثابت ساخت file_operations sched_scaling_fops = {
	.open		= sched_scaling_open,
	.اكتب		= sched_scaling_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#اگر_تعریف_شدہ CONFIG_SCHED_CACHE
ساکن ssize_t
sched_cache_enable_write(ساخت file *filp, ثابت حرف __user *ubuf,
			 سائز_t cnt, loff_t *ppos)
{
	bool val;
	صحیح ret;

	ret = kstrtobool_from_user(ubuf, cnt, &val);
	اگر (ret)
		واپس ret;

	sysctl_sched_cache_user = val;

	sched_cache_فعال_سیٹ();

	*ppos += cnt;

	واپس cnt;
}

ساکن صحیح sched_cache_enable_show(ساخت seq_file *m, خالی *v)
{
	seq_printf(m, "%d\n", sysctl_sched_cache_user);
	واپس 0;
}

ساکن صحیح sched_cache_enable_open(ساخت inode *inode,
				   ساخت file *filp)
{
	واپس single_open(filp, sched_cache_enable_show, تہی);
}

ساکن ثابت ساخت file_operations sched_cache_enable_fops = {
	.open           = sched_cache_enable_open,
	.اكتب          = sched_cache_enable_write,
	.اقرأ           = seq_read,
	.llseek         = seq_lseek,
	.release        = single_release,
};
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_PREEMPT_DYNAMIC

ساکن ssize_t sched_dynamic_write(ساخت file *filp, ثابت حرف __user *ubuf,
				   سائز_t cnt, loff_t *ppos)
{
	حرف buf[16];
	صحیح منوال;

	اگر (cnt > 15)
		cnt = 15;

	اگر (copy_from_user(buf, ubuf, cnt))
		واپس -EFAULT;

	buf[cnt] = 0;
	منوال = sched_dynamic_منوال(strstrip(buf));
	اگر (منوال < 0)
		واپس منوال;

	sched_dynamic_update(منوال);

	*ppos += cnt;

	واپس cnt;
}

ساکن صحیح sched_dynamic_show(ساخت seq_file *m, خالی *v)
{
	صحیح منوال = READ_ONCE(preempt_dynamic_منوال);

	/* Stop at NULL terminator */
	برائے (صحیح i = 0; preempt_modes[i]; i++) {
		اگر (منوال == i)
			seq_puts(m, "(");
		seq_puts(m, preempt_modes[i]);
		اگر (منوال == i)
			seq_puts(m, ")");

		seq_puts(m, " ");
	}

	seq_puts(m, "\n");
	واپس 0;
}

ساکن صحیح sched_dynamic_open(ساخت inode *inode, ساخت file *filp)
{
	واپس single_open(filp, sched_dynamic_show, تہی);
}

ساکن ثابت ساخت file_operations sched_dynamic_fops = {
	.open		= sched_dynamic_open,
	.اكتب		= sched_dynamic_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#اگر_ختم /* CONFIG_PREEMPT_DYNAMIC */

__read_mostly bool sched_debug_verbose;

ساکن ساخت dentry           *sd_dentry;


ساکن ssize_t sched_verbose_write(ساخت file *filp, ثابت حرف __user *ubuf,
				  سائز_t cnt, loff_t *ppos)
{
	ssize_t نتیجہ;
	bool orig;

	cpus_read_تالا();
	sched_domains_mutex_تالا();

	orig = sched_debug_verbose;
	نتیجہ = debugfs_write_file_bool(filp, ubuf, cnt, ppos);

	اگر (sched_debug_verbose && !orig)
		update_sched_domain_debugfs();
	ورنہ اگر (!sched_debug_verbose && orig) {
		debugfs_remove(sd_dentry);
		sd_dentry = تہی;
	}

	sched_domains_mutex_unlock();
	cpus_read_unlock();

	واپس نتیجہ;
}

ساکن ثابت ساخت file_operations sched_verbose_fops = {
	.اقرأ =         debugfs_read_file_bool,
	.اكتب =        sched_verbose_write,
	.open =         simple_open,
	.llseek =       default_llseek,
};

ساکن ثابت ساخت seq_operations sched_debug_sops;

ساکن صحیح sched_debug_open(ساخت inode *inode, ساخت file *filp)
{
	واپس seq_open(filp, &sched_debug_sops);
}

ساکن ثابت ساخت file_operations sched_debug_fops = {
	.open		= sched_debug_open,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

شمار dl_param {
	DL_RUNTIME = 0,
	DL_PERIOD,
};

ساکن غیرعلامتی طویل dl_server_period_max = (1UL << 22) * NSEC_فی_USEC; /* ~4 seconds */
ساکن غیرعلامتی طویل dl_server_period_min = (100) * NSEC_فی_USEC;     /* 100 us */

ساکن ssize_t sched_server_write_common(ساخت file *filp, ثابت حرف __user *ubuf,
					 سائز_t cnt, loff_t *ppos, شمار dl_param param,
					 خالی *server)
{
	طویل cpu = (طویل) ((ساخت seq_file *) filp->نجی_ڈیٹا)->نجی;
	ساخت sched_dl_entity *dl_se = (ساخت sched_dl_entity *)server;
	u64 old_runtime, runtime, period;
	ساخت rq *rq = cpu_rq(cpu);
	صحیح retval = 0;
	سائز_t err;
	u64 قدر;

	err = kstrtoull_from_user(ubuf, cnt, 10, &قدر);
	اگر (err)
		واپس err;

	scoped_guard (rq_تالا_irqsave, rq) {
		old_runtime = runtime = dl_se->dl_runtime;
		period = dl_se->dl_period;

		انتخاب (param) {
		حالت DL_RUNTIME:
			اگر (runtime == قدر)
				توڑو;
			runtime = قدر;
			توڑو;
		حالت DL_PERIOD:
			اگر (قدر == period)
				توڑو;
			period = قدر;
			توڑو;
		}

		اگر (runtime > period ||
		    period > dl_server_period_max ||
		    period < dl_server_period_min) {
			واپس  -EINVAL;
		}

		اگر (!cpu_online(cpu_کا(rq)))
			واپس -EBUSY;

		update_rq_clock(rq);
		dl_server_stop(dl_se);
		retval = dl_server_apply_params(dl_se, runtime, period, 0);
		dl_server_آغاز(dl_se);

		اگر (retval < 0)
			واپس retval;
	}

	اگر (!!old_runtime ^ !!runtime) {
		pr_info("%s server %sabled on CPU %d%s.\n",
			server == &rq->fair_server ? "Fair" : "Ext",
			runtime ? "en" : "dis",
			cpu_کا(rq),
			runtime ? "" : ", system may malfunction due to starvation");
	}

	*ppos += cnt;
	واپس cnt;
}

ساکن سائز_t sched_server_show_common(ساخت seq_file *m, خالی *v, شمار dl_param param,
				       خالی *server)
{
	ساخت sched_dl_entity *dl_se = (ساخت sched_dl_entity *)server;
	u64 قدر;

	انتخاب (param) {
	حالت DL_RUNTIME:
		قدر = dl_se->dl_runtime;
		توڑو;
	حالت DL_PERIOD:
		قدر = dl_se->dl_period;
		توڑو;
	}

	seq_printf(m, "%llu\n", قدر);
	واپس 0;
}

ساکن ssize_t
sched_fair_server_runtime_write(ساخت file *filp, ثابت حرف __user *ubuf,
				سائز_t cnt, loff_t *ppos)
{
	طویل cpu = (طویل) ((ساخت seq_file *) filp->نجی_ڈیٹا)->نجی;
	ساخت rq *rq = cpu_rq(cpu);

	واپس sched_server_write_common(filp, ubuf, cnt, ppos, DL_RUNTIME,
					&rq->fair_server);
}

ساکن صحیح sched_fair_server_runtime_show(ساخت seq_file *m, خالی *v)
{
	غیرعلامتی طویل cpu = (غیرعلامتی طویل) m->نجی;
	ساخت rq *rq = cpu_rq(cpu);

	واپس sched_server_show_common(m, v, DL_RUNTIME, &rq->fair_server);
}

ساکن صحیح sched_fair_server_runtime_open(ساخت inode *inode, ساخت file *filp)
{
	واپس single_open(filp, sched_fair_server_runtime_show, inode->i_نجی);
}

ساکن ثابت ساخت file_operations fair_server_runtime_fops = {
	.open		= sched_fair_server_runtime_open,
	.اكتب		= sched_fair_server_runtime_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ساکن ساخت dentry *debugfs_sched;

#اگر_تعریف_شدہ CONFIG_SCHED_CLASS_EXT
ساکن ssize_t
sched_ext_server_runtime_write(ساخت file *filp, ثابت حرف __user *ubuf,
			       سائز_t cnt, loff_t *ppos)
{
	طویل cpu = (طویل) ((ساخت seq_file *) filp->نجی_ڈیٹا)->نجی;
	ساخت rq *rq = cpu_rq(cpu);

	واپس sched_server_write_common(filp, ubuf, cnt, ppos, DL_RUNTIME,
					&rq->ext_server);
}

ساکن صحیح sched_ext_server_runtime_show(ساخت seq_file *m, خالی *v)
{
	غیرعلامتی طویل cpu = (غیرعلامتی طویل) m->نجی;
	ساخت rq *rq = cpu_rq(cpu);

	واپس sched_server_show_common(m, v, DL_RUNTIME, &rq->ext_server);
}

ساکن صحیح sched_ext_server_runtime_open(ساخت inode *inode, ساخت file *filp)
{
	واپس single_open(filp, sched_ext_server_runtime_show, inode->i_نجی);
}

ساکن ثابت ساخت file_operations ext_server_runtime_fops = {
	.open		= sched_ext_server_runtime_open,
	.اكتب		= sched_ext_server_runtime_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ساکن ssize_t
sched_ext_server_period_write(ساخت file *filp, ثابت حرف __user *ubuf,
			      سائز_t cnt, loff_t *ppos)
{
	طویل cpu = (طویل) ((ساخت seq_file *) filp->نجی_ڈیٹا)->نجی;
	ساخت rq *rq = cpu_rq(cpu);

	واپس sched_server_write_common(filp, ubuf, cnt, ppos, DL_PERIOD,
					&rq->ext_server);
}

ساکن صحیح sched_ext_server_period_show(ساخت seq_file *m, خالی *v)
{
	غیرعلامتی طویل cpu = (غیرعلامتی طویل) m->نجی;
	ساخت rq *rq = cpu_rq(cpu);

	واپس sched_server_show_common(m, v, DL_PERIOD, &rq->ext_server);
}

ساکن صحیح sched_ext_server_period_open(ساخت inode *inode, ساخت file *filp)
{
	واپس single_open(filp, sched_ext_server_period_show, inode->i_نجی);
}

ساکن ثابت ساخت file_operations ext_server_period_fops = {
	.open		= sched_ext_server_period_open,
	.اكتب		= sched_ext_server_period_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ساکن خالی debugfs_ext_server_init(خالی)
{
	ساخت dentry *d_ext;
	غیرعلامتی طویل cpu;

	d_ext = debugfs_create_dir("ext_server", debugfs_sched);
	اگر (!d_ext)
		واپس;

	for_each_possible_cpu(cpu) {
		ساخت dentry *d_cpu;
		حرف buf[32];

		snprintf(buf, حجم(buf), "cpu%lu", cpu);
		d_cpu = debugfs_create_dir(buf, d_ext);

		debugfs_create_file("runtime", 0644, d_cpu, (خالی *) cpu, &ext_server_runtime_fops);
		debugfs_create_file("period", 0644, d_cpu, (خالی *) cpu, &ext_server_period_fops);
	}
}
#اگر_ختم /* CONFIG_SCHED_CLASS_EXT */

ساکن ssize_t
sched_fair_server_period_write(ساخت file *filp, ثابت حرف __user *ubuf,
			       سائز_t cnt, loff_t *ppos)
{
	طویل cpu = (طویل) ((ساخت seq_file *) filp->نجی_ڈیٹا)->نجی;
	ساخت rq *rq = cpu_rq(cpu);

	واپس sched_server_write_common(filp, ubuf, cnt, ppos, DL_PERIOD,
					&rq->fair_server);
}

ساکن صحیح sched_fair_server_period_show(ساخت seq_file *m, خالی *v)
{
	غیرعلامتی طویل cpu = (غیرعلامتی طویل) m->نجی;
	ساخت rq *rq = cpu_rq(cpu);

	واپس sched_server_show_common(m, v, DL_PERIOD, &rq->fair_server);
}

ساکن صحیح sched_fair_server_period_open(ساخت inode *inode, ساخت file *filp)
{
	واپس single_open(filp, sched_fair_server_period_show, inode->i_نجی);
}

ساکن ثابت ساخت file_operations fair_server_period_fops = {
	.open		= sched_fair_server_period_open,
	.اكتب		= sched_fair_server_period_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ساکن خالی debugfs_fair_server_init(خالی)
{
	ساخت dentry *d_fair;
	غیرعلامتی طویل cpu;

	d_fair = debugfs_create_dir("fair_server", debugfs_sched);
	اگر (!d_fair)
		واپس;

	for_each_possible_cpu(cpu) {
		ساخت dentry *d_cpu;
		حرف buf[32];

		snprintf(buf, حجم(buf), "cpu%lu", cpu);
		d_cpu = debugfs_create_dir(buf, d_fair);

		debugfs_create_file("runtime", 0644, d_cpu, (خالی *) cpu, &fair_server_runtime_fops);
		debugfs_create_file("period", 0644, d_cpu, (خالی *) cpu, &fair_server_period_fops);
	}
}

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
ساکن صحیح cgroup_منوال = 2;

/* See __sched_cgroup_mode_update(). */
ساکن ثابت حرف *cgroup_منوال_str[] = {
	"up",
	"smp",
	"concur",
	"max",
	"tasks",
};

ساکن صحیح sched_cgroup_منوال(ثابت حرف *str)
{
	برائے (صحیح i = 0; i < ARRAY_سائز(cgroup_منوال_str); i++) {
		اگر (!strcmp(str, cgroup_منوال_str[i]))
			واپس i;
	}
	واپس -EINVAL;
}

ساکن ssize_t sched_cgroup_write(ساخت file *filp, ثابت حرف __user *ubuf,
				   سائز_t cnt, loff_t *ppos)
{
	حرف buf[16];
	صحیح منوال;

	اگر (cnt > 15)
		cnt = 15;

	اگر (copy_from_user(buf, ubuf, cnt))
		واپس -EFAULT;

	buf[cnt] = 0;
	منوال = sched_cgroup_منوال(strstrip(buf));
	اگر (منوال < 0)
		واپس منوال;

	__sched_cgroup_mode_update(منوال);
	WRITE_ONCE(cgroup_منوال, منوال);

	*ppos += cnt;
	واپس cnt;
}

ساکن صحیح sched_cgroup_show(ساخت seq_file *m, خالی *v)
{
	صحیح منوال = READ_ONCE(cgroup_منوال);

	برائے (صحیح i = 0; i < ARRAY_سائز(cgroup_منوال_str); i++) {
		اگر (منوال == i)
			seq_puts(m, "(");
		seq_puts(m, cgroup_منوال_str[i]);
		اگر (منوال == i)
			seq_puts(m, ")");

		seq_puts(m, " ");
	}
	seq_puts(m, "\n");
	واپس 0;
}

ساکن صحیح sched_cgroup_open(ساخت inode *inode, ساخت file *filp)
{
	واپس single_open(filp, sched_cgroup_show, تہی);
}

ساکن ثابت ساخت file_operations sched_cgroup_fops = {
	.open		= sched_cgroup_open,
	.اكتب		= sched_cgroup_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};
#اگر_ختم

ساکن __init صحیح sched_init_debug(خالی)
{
	ساخت dentry __maybe_unused *numa, *llc;

	debugfs_sched = debugfs_create_dir("sched", تہی);

	debugfs_create_file("features", 0644, debugfs_sched, تہی, &sched_feat_fops);
	debugfs_create_file_unsafe("verbose", 0644, debugfs_sched, &sched_debug_verbose, &sched_verbose_fops);
#اگر_تعریف_شدہ CONFIG_PREEMPT_DYNAMIC
	debugfs_create_file("preempt", 0644, debugfs_sched, تہی, &sched_dynamic_fops);
#اگر_ختم

	debugfs_create_u32("base_slice_ns", 0644, debugfs_sched, &sysctl_sched_بنیاد_قاش);

	debugfs_create_u32("latency_warn_ms", 0644, debugfs_sched, &sysctl_resched_latency_warn_ms);
	debugfs_create_u32("latency_warn_once", 0644, debugfs_sched, &sysctl_resched_latency_warn_once);

	debugfs_create_file("tunable_scaling", 0644, debugfs_sched, تہی, &sched_scaling_fops);
	debugfs_create_u32("migration_cost_ns", 0644, debugfs_sched, &sysctl_sched_migration_لاگت);
	debugfs_create_u32("nr_migrate", 0644, debugfs_sched, &sysctl_sched_nr_migrate);

	sched_domains_mutex_تالا();
	update_sched_domain_debugfs();
	sched_domains_mutex_unlock();

#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING
	numa = debugfs_create_dir("numa_balancing", debugfs_sched);

	debugfs_create_u32("scan_delay_ms", 0644, numa, &sysctl_numa_balancing_جائزہ_delay);
	debugfs_create_u32("scan_period_min_ms", 0644, numa, &sysctl_numa_balancing_جائزہ_period_min);
	debugfs_create_u32("scan_period_max_ms", 0644, numa, &sysctl_numa_balancing_جائزہ_period_max);
	debugfs_create_u32("scan_size_mb", 0644, numa, &sysctl_numa_balancing_جائزہ_سائز);
	debugfs_create_u32("hot_threshold_ms", 0644, numa, &sysctl_numa_balancing_hot_حد);
#اگر_ختم /* CONFIG_NUMA_BALANCING */

#اگر_تعریف_شدہ CONFIG_SCHED_CACHE
	llc = debugfs_create_dir("llc_balancing", debugfs_sched);
	debugfs_create_file("enabled", 0644, llc, تہی,
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
#اگر_ختم

	debugfs_create_file("debug", 0444, debugfs_sched, تہی, &sched_debug_fops);

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
	debugfs_create_file("cgroup_mode", 0644, debugfs_sched, تہی, &sched_cgroup_fops);
#اگر_ختم

	debugfs_fair_server_init();
#اگر_تعریف_شدہ CONFIG_SCHED_CLASS_EXT
	debugfs_ext_server_init();
#اگر_ختم

	واپس 0;
}
late_initcall(sched_init_debug);

ساکن cpumask_var_t		sd_sysctl_cpus;

ساکن صحیح sd_flags_show(ساخت seq_file *m, خالی *v)
{
	غیرعلامتی طویل flags = *(غیرعلامتی صحیح *)m->نجی;
	صحیح idx;

	for_each_سیٹ_ثنائی_ہندسہ(idx, &flags, __SD_FLAG_CNT) {
		seq_puts(m, sd_flag_debug[idx].name);
		seq_puts(m, " ");
	}
	seq_puts(m, "\n");

	واپس 0;
}

ساکن صحیح sd_flags_open(ساخت inode *inode, ساخت file *file)
{
	واپس single_open(file, sd_flags_show, inode->i_نجی);
}

ساکن ثابت ساخت file_operations sd_flags_fops = {
	.open		= sd_flags_open,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ساکن خالی رجسٹر_sd(ساخت sched_domain *sd, ساخت dentry *والد)
{
#تعریف_کرو SDM(type, منوال, member)	\
	debugfs_create_##type(#member, منوال, والد, &sd->member)

	SDM(ulong, 0644, min_interval);
	SDM(ulong, 0644, max_interval);
	SDM(u64,   0644, max_newidle_lb_لاگت);
	SDM(u32,   0644, busy_ضربی_عدد);
	SDM(u32,   0644, imbalance_pct);
	SDM(u32,   0644, cache_nice_tries);
	SDM(str,   0444, name);

#تعریف_منسوخ SDM

	debugfs_create_file("flags", 0444, والد, &sd->flags, &sd_flags_fops);
	debugfs_create_file("groups_flags", 0444, والد, &sd->groups->flags, &sd_flags_fops);
	debugfs_create_u32("level", 0444, والد, (u32 *)&sd->level);

	اگر (sd->flags & SD_ASYM_PACKING)
		debugfs_create_u32("group_asym_prefer_cpu", 0444, والد,
				   (u32 *)&sd->groups->asym_prefer_cpu);
}

خالی update_sched_domain_debugfs(خالی)
{
	صحیح cpu, i;

	/*
	 * This can unfortunately be invoked before sched_debug_init() creates
	 * the debug directory. Don't touch sd_sysctl_cpus until then.
	 */
	اگر (!debugfs_sched)
		واپس;

	اگر (!sched_debug_verbose)
		واپس;

	اگر (!cpumask_دستیاب(sd_sysctl_cpus)) {
		اگر (!alloc_cpumask_var(&sd_sysctl_cpus, GFP_KERNEL))
			واپس;
		cpumask_copy(sd_sysctl_cpus, cpu_possible_نقاب);
	}

	اگر (!sd_dentry) {
		sd_dentry = debugfs_create_dir("domains", debugfs_sched);

		/* rebuild sd_sysctl_cpus if empty since it gets cleared below */
		اگر (cpumask_خالی_خانے(sd_sysctl_cpus))
			cpumask_copy(sd_sysctl_cpus, cpu_online_نقاب);
	}

	for_each_cpu(cpu, sd_sysctl_cpus) {
		ساخت sched_domain *sd;
		ساخت dentry *d_cpu;
		حرف buf[32];

		snprintf(buf, حجم(buf), "cpu%d", cpu);
		debugfs_lookup_and_remove(buf, sd_dentry);
		d_cpu = debugfs_create_dir(buf, sd_dentry);

		i = 0;
		for_each_domain(cpu, sd) {
			ساخت dentry *d_sd;

			snprintf(buf, حجم(buf), "domain%d", i);
			d_sd = debugfs_create_dir(buf, d_cpu);

			رجسٹر_sd(sd, d_sd);
			i++;
		}

		__cpumask_clear_cpu(cpu, sd_sysctl_cpus);
	}
}

خالی dirty_sched_domain_sysctl(صحیح cpu)
{
	اگر (cpumask_دستیاب(sd_sysctl_cpus))
		__cpumask_set_cpu(cpu, sd_sysctl_cpus);
}

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
ساکن خالی print_cfs_گروہ_stats(ساخت seq_file *m, صحیح cpu, ساخت task_گروہ *tg)
{
	ساخت sched_entity *se = tg_se(tg, cpu);

#تعریف_کرو P(F)		SEQ_printf(m, "  .%-30s: %lld\n",	#F, (طویل طویل)F)
#تعریف_کرو P_SCHEDSTAT(F)	SEQ_printf(m, "  .%-30s: %lld\n",	\
		#F, (طویل طویل)schedstat_val(stats->F))
#تعریف_کرو PN(F)		SEQ_printf(m, "  .%-30s: %lld.%06ld\n", #F, بانٹو_NS((طویل طویل)F))
#تعریف_کرو PN_SCHEDSTAT(F)	SEQ_printf(m, "  .%-30s: %lld.%06ld\n", \
		#F, بانٹو_NS((طویل طویل)schedstat_val(stats->F)))

	اگر (!se)
		واپس;

	PN(se->exec_آغاز);
	PN(se->vruntime);
	PN(se->مجموع_exec_runtime);

	اگر (schedstat_enabled()) {
		ساخت sched_statistics *stats;
		stats = __schedstats_from_se(se);

		PN_SCHEDSTAT(wait_آغاز);
		PN_SCHEDSTAT(sleep_آغاز);
		PN_SCHEDSTAT(بلاک_آغاز);
		PN_SCHEDSTAT(sleep_max);
		PN_SCHEDSTAT(بلاک_max);
		PN_SCHEDSTAT(exec_max);
		PN_SCHEDSTAT(قاش_max);
		PN_SCHEDSTAT(wait_max);
		PN_SCHEDSTAT(wait_مجموع);
		P_SCHEDSTAT(wait_گنتی);
	}

	P(se->بھرو.وزن);
	P(se->avg.بھرو_avg);
	P(se->avg.util_avg);
	P(se->avg.runnable_avg);

#تعریف_منسوخ PN_SCHEDSTAT
#تعریف_منسوخ PN
#تعریف_منسوخ P_SCHEDSTAT
#تعریف_منسوخ P
}
#اگر_ختم /* CONFIG_FAIR_GROUP_SCHED */

#اگر_تعریف_شدہ CONFIG_CGROUP_SCHED
ساکن DEFINE_SPINLOCK(sched_debug_تالا);
ساکن حرف گروہ_راستہ[راستہ_MAX];

ساکن خالی task_گروہ_راستہ(ساخت task_گروہ *tg, حرف *راستہ, صحیح plen)
{
	اگر (autogroup_راستہ(tg, راستہ, plen))
		واپس;

	cgroup_راستہ(tg->css.cgroup, راستہ, plen);
}

/*
 * Only 1 SEQ_printf_task_group_path() caller can use the full length
 * group_path[] for cgroup path. Other simultaneous callers will have
 * to use a shorter stack buffer. A "..." suffix is appended at the end
 * of the stack buffer so that it will show up in case the output length
 * matches the given buffer size to indicate possible path name truncation.
 */
#تعریف_کرو SEQ_printf_task_گروہ_راستہ(m, tg, fmt...)			\
{									\
	اگر (spin_trylock(&sched_debug_تالا)) {				\
		task_گروہ_راستہ(tg, گروہ_راستہ, حجم(گروہ_راستہ));	\
		SEQ_printf(m, fmt, گروہ_راستہ);				\
		spin_unlock(&sched_debug_تالا);				\
	} ورنہ {							\
		حرف buf[128];						\
		حرف *bufend = buf + حجم(buf) - 3;			\
		task_گروہ_راستہ(tg, buf, bufend - buf);			\
		strcpy(bufend - 1, "...");				\
		SEQ_printf(m, fmt, buf);				\
	}								\
}
#اگر_ختم

ساکن خالی
print_task(ساخت seq_file *m, ساخت rq *rq, ساخت task_struct *p)
{
	اگر (task_current(rq, p))
		SEQ_printf(m, ">R");
	ورنہ
		SEQ_printf(m, " %c", task_state_مقصد_char(p));

	SEQ_printf(m, " %15s %5d %10ld %9Ld.%06ld   %c   %9Ld.%06ld %c %9Ld.%06ld %9Ld.%06ld %9Ld   %5d ",
		p->comm, task_pid_nr(p),
		p->se.h_بھرو.وزن,
		بانٹو_NS(p->se.vruntime),
		entity_eligible(&rq->cfs, &p->se) ? 'E' : 'N',
		بانٹو_NS(p->se.deadline),
		p->se.custom_قاش ? 'S' : ' ',
		بانٹو_NS(p->se.قاش),
		بانٹو_NS(p->se.مجموع_exec_runtime),
		(طویل طویل)(p->nvcsw + p->nivcsw),
		p->prio);

	SEQ_printf(m, "%9lld.%06ld %9lld.%06ld %9lld.%06ld",
		بانٹو_NS(schedstat_val_or_zero(p->stats.wait_مجموع)),
		بانٹو_NS(schedstat_val_or_zero(p->stats.مجموع_sleep_runtime)),
		بانٹو_NS(schedstat_val_or_zero(p->stats.مجموع_بلاک_runtime)));

#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING
	SEQ_printf(m, "   %d      %d", task_گرہ(p), task_numa_گروہ_id(p));
#اگر_ختم
#اگر_تعریف_شدہ CONFIG_CGROUP_SCHED
	SEQ_printf_task_گروہ_راستہ(m, task_گروہ(p), "        %s")
#اگر_ختم

	SEQ_printf(m, "\n");
}

ساکن خالی print_rq(ساخت seq_file *m, ساخت rq *rq, صحیح rq_cpu)
{
	ساخت task_struct *g, *p;

	SEQ_printf(m, "\n");
	SEQ_printf(m, "runnable tasks:\n");
	SEQ_printf(m, " S            task   PID     weight       vruntime   eligible    "
		   "deadline             slice          sum-exec      switches  "
		   "prio         wait-time        sum-sleep       sum-block"
#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING
		   "  node   group-id"
#اگر_ختم
#اگر_تعریف_شدہ CONFIG_CGROUP_SCHED
		   "  group-path"
#اگر_ختم
		   "\n");
	SEQ_printf(m, "-------------------------------------------------------"
		   "------------------------------------------------------"
		   "------------------------------------------------------"
#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING
		   "--------------"
#اگر_ختم
#اگر_تعریف_شدہ CONFIG_CGROUP_SCHED
		   "--------------"
#اگر_ختم
		   "\n");

	rcu_read_تالا();
	for_each_process_thread(g, p) {
		اگر (task_cpu(p) != rq_cpu)
			جاری;

		print_task(m, rq, p);
	}
	rcu_read_unlock();
}

خالی print_cfs_rq(ساخت seq_file *m, صحیح cpu, ساخت cfs_rq *cfs_rq)
{
	s64 بایاں_vruntime = -1, دایاں_vruntime = -1, بایاں_deadline = -1, پھیلاؤ;
	s64 zero_vruntime = -1, مجموع_w_vruntime = -1;
	u64 avruntime;
	ساخت sched_entity *آخری, *پہلا, *جڑ;
	ساخت rq *rq = cpu_rq(cpu);
	غیرعلامتی صحیح مجموع_سرکاؤ;
	غیرعلامتی طویل flags;
	u64 مجموع_وزن;

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
	SEQ_printf(m, "\n");
	SEQ_printf_task_گروہ_راستہ(m, cfs_rq->tg, "cfs_rq[%d]:%s\n", cpu);
#ورنہ
	SEQ_printf(m, "\n");
	SEQ_printf(m, "cfs_rq[%d]:\n", cpu);
#اگر_ختم

	raw_spin_rq_تالا_irqsave(rq, flags);
	جڑ = __pick_root_entity(cfs_rq);
	اگر (جڑ)
		بایاں_vruntime = جڑ->min_vruntime;
	پہلا = __pick_first_entity(cfs_rq);
	اگر (پہلا)
		بایاں_deadline = پہلا->deadline;
	آخری = __pick_last_entity(cfs_rq);
	اگر (آخری)
		دایاں_vruntime = آخری->vruntime;
	zero_vruntime = cfs_rq->zero_vruntime;
	مجموع_w_vruntime = cfs_rq->مجموع_w_vruntime;
	مجموع_وزن = cfs_rq->مجموع_وزن;
	مجموع_سرکاؤ = cfs_rq->مجموع_سرکاؤ;
	avruntime = avg_vruntime(cfs_rq);
	raw_spin_rq_unlock_irqrestore(rq, flags);

	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "left_deadline",
			بانٹو_NS(بایاں_deadline));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "left_vruntime",
			بانٹو_NS(بایاں_vruntime));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "zero_vruntime",
			بانٹو_NS(zero_vruntime));
	SEQ_printf(m, "  .%-30s: %Ld (%d bits)\n", "sum_w_vruntime",
		   مجموع_w_vruntime, ilog2(مطلق_قدر(مجموع_w_vruntime)));
	SEQ_printf(m, "  .%-30s: %Lu\n", "sum_weight",
		   مجموع_وزن);
	SEQ_printf(m, "  .%-30s: %u\n", "sum_shift", مجموع_سرکاؤ);
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "avg_vruntime",
			بانٹو_NS(avruntime));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "right_vruntime",
			بانٹو_NS(دایاں_vruntime));
	پھیلاؤ = دایاں_vruntime - بایاں_vruntime;
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "spread", بانٹو_NS(پھیلاؤ));
	SEQ_printf(m, "  .%-30s: %d\n", "nr_queued", cfs_rq->nr_queued);
	SEQ_printf(m, "  .%-30s: %d\n", "h_nr_runnable", cfs_rq->h_nr_runnable);
	SEQ_printf(m, "  .%-30s: %d\n", "h_nr_queued", cfs_rq->h_nr_queued);
	SEQ_printf(m, "  .%-30s: %d\n", "h_nr_idle", cfs_rq->h_nr_idle);
	SEQ_printf(m, "  .%-30s: %ld\n", "load", cfs_rq->بھرو.وزن);
	SEQ_printf(m, "  .%-30s: %lu\n", "load_avg",
			cfs_rq->avg.بھرو_avg);
	SEQ_printf(m, "  .%-30s: %lu\n", "runnable_avg",
			cfs_rq->avg.runnable_avg);
	SEQ_printf(m, "  .%-30s: %lu\n", "util_avg",
			cfs_rq->avg.util_avg);
	SEQ_printf(m, "  .%-30s: %u\n", "util_est",
			cfs_rq->avg.util_est);
	SEQ_printf(m, "  .%-30s: %ld\n", "removed.load_avg",
			cfs_rq->removed.بھرو_avg);
	SEQ_printf(m, "  .%-30s: %ld\n", "removed.util_avg",
			cfs_rq->removed.util_avg);
	SEQ_printf(m, "  .%-30s: %ld\n", "removed.runnable_avg",
			cfs_rq->removed.runnable_avg);
#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
	SEQ_printf(m, "  .%-30s: %lu\n", "tg_load_avg_contrib",
			cfs_rq->tg_بھرو_avg_contrib);
	SEQ_printf(m, "  .%-30s: %ld\n", "tg_load_avg",
			atomic_long_read(&cfs_rq->tg->بھرو_avg));
	SEQ_printf(m, "  .%-30s: %lu\n", "h_load",
			cfs_rq->h_بھرو);
#اگر_ختم /* CONFIG_FAIR_GROUP_SCHED */
#اگر_تعریف_شدہ CONFIG_CFS_BANDWIDTH
	SEQ_printf(m, "  .%-30s: %d\n", "throttled",
			cfs_rq->throttled);
	SEQ_printf(m, "  .%-30s: %d\n", "throttle_count",
			cfs_rq->throttle_گنتی);
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
	print_cfs_گروہ_stats(m, cpu, cfs_rq->tg);
#اگر_ختم
}

خالی print_rt_rq(ساخت seq_file *m, صحیح cpu, ساخت rt_rq *rt_rq)
{
#اگر_تعریف_شدہ CONFIG_RT_گروہ_SCHED
	SEQ_printf(m, "\n");
	SEQ_printf_task_گروہ_راستہ(m, rt_rq->tg, "rt_rq[%d]:%s\n", cpu);
#ورنہ
	SEQ_printf(m, "\n");
	SEQ_printf(m, "rt_rq[%d]:\n", cpu);
#اگر_ختم

#تعریف_کرو P(x) \
	SEQ_printf(m, "  .%-30s: %Ld\n", #x, (طویل طویل)(rt_rq->x))
#تعریف_کرو PU(x) \
	SEQ_printf(m, "  .%-30s: %lu\n", #x, (غیرعلامتی طویل)(rt_rq->x))
#تعریف_کرو PN(x) \
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", #x, بانٹو_NS(rt_rq->x))

	PU(rt_nr_running);

#اگر_تعریف_شدہ CONFIG_RT_گروہ_SCHED
	P(rt_throttled);
	PN(rt_وقت);
	PN(rt_runtime);
#اگر_ختم

#تعریف_منسوخ PN
#تعریف_منسوخ PU
#تعریف_منسوخ P
}

خالی print_dl_rq(ساخت seq_file *m, صحیح cpu, ساخت dl_rq *dl_rq)
{
	ساخت dl_bw *dl_bw;

	SEQ_printf(m, "\n");
	SEQ_printf(m, "dl_rq[%d]:\n", cpu);

#تعریف_کرو PU(x) \
	SEQ_printf(m, "  .%-30s: %lu\n", #x, (غیرعلامتی طویل)(dl_rq->x))

	PU(dl_nr_running);
	dl_bw = &cpu_rq(cpu)->rd->dl_bw;
	SEQ_printf(m, "  .%-30s: %lld\n", "dl_bw->bw", dl_bw->bw);
	SEQ_printf(m, "  .%-30s: %lld\n", "dl_bw->total_bw", dl_bw->کل_bw);

#تعریف_منسوخ PU
}

ساکن خالی print_cpu(ساخت seq_file *m, صحیح cpu)
{
	ساخت rq *rq = cpu_rq(cpu);

#اگر_تعریف_شدہ CONFIG_X86
	{
		غیرعلامتی صحیح freq = cpu_khz ? : 1;

		SEQ_printf(m, "cpu#%d, %u.%03u MHz\n",
			   cpu, freq / 1000, (freq % 1000));
	}
#ورنہ /* !CONFIG_X86: */
	SEQ_printf(m, "cpu#%d\n", cpu);
#اگر_ختم /* !CONFIG_X86 */

#تعریف_کرو P(x)								\
کرو {									\
	اگر (حجم(rq->x) == 4)						\
		SEQ_printf(m, "  .%-30s: %d\n", #x, (صحیح)(rq->x));	\
	ورنہ								\
		SEQ_printf(m, "  .%-30s: %Ld\n", #x, (طویل طویل)(rq->x));\
} جب (0)

#تعریف_کرو PN(x) \
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", #x, بانٹو_NS(rq->x))

	P(nr_running);
	P(nr_switches);
	P(nr_uninterruptible);
	PN(اگلا_balance);
	SEQ_printf(m, "  .%-30s: %ld\n", "curr->pid", (طویل)(task_pid_nr(rq->curr)));
	PN(clock);
	PN(clock_task);
#تعریف_منسوخ P
#تعریف_منسوخ PN

#تعریف_کرو P64(n) SEQ_printf(m, "  .%-30s: %Ld\n", #n, rq->n);
	P64(avg_idle);
	P64(max_idle_balance_لاگت);
#تعریف_منسوخ P64

#تعریف_کرو P(n) SEQ_printf(m, "  .%-30s: %d\n", #n, schedstat_val(rq->n));
	اگر (schedstat_enabled()) {
		P(yld_گنتی);
		P(sched_گنتی);
		P(sched_goidle);
		P(ttwu_گنتی);
		P(ttwu_local);
	}
#تعریف_منسوخ P

	print_cfs_stats(m, cpu);
	print_rt_stats(m, cpu);
	print_dl_stats(m, cpu);

	print_rq(m, rq, cpu);
	SEQ_printf(m, "\n");
}

ساکن ثابت حرف *sched_tunable_scaling_نام[] = {
	"none",
	"logarithmic",
	"linear"
};

ساکن خالی sched_debug_سرخی(ساخت seq_file *m)
{
	u64 ktime, sched_clk, cpu_clk;
	غیرعلامتی طویل flags;

	local_irq_save(flags);
	ktime = ktime_مقصد_ns(ktime_get());
	sched_clk = sched_clock();
	cpu_clk = local_clock();
	local_irq_restore(flags);

	SEQ_printf(m, "Sched Debug Version: v0.11, %s %.*s\n",
		init_utsname()->release,
		(صحیح)strcspn(init_utsname()->نسخہ, " "),
		init_utsname()->نسخہ);

#تعریف_کرو P(x) \
	SEQ_printf(m, "%-40s: %Ld\n", #x, (طویل طویل)(x))
#تعریف_کرو PN(x) \
	SEQ_printf(m, "%-40s: %Ld.%06ld\n", #x, بانٹو_NS(x))
	PN(ktime);
	PN(sched_clk);
	PN(cpu_clk);
	P(jiffies);
#اگر_تعریف_شدہ CONFIG_HAVE_UNSTABLE_SCHED_CLOCK
	P(sched_clock_stable());
#اگر_ختم
#تعریف_منسوخ PN
#تعریف_منسوخ P

	SEQ_printf(m, "\n");
	SEQ_printf(m, "sysctl_sched\n");

#تعریف_کرو P(x) \
	SEQ_printf(m, "  .%-40s: %Ld\n", #x, (طویل طویل)(x))
#تعریف_کرو PN(x) \
	SEQ_printf(m, "  .%-40s: %Ld.%06ld\n", #x, بانٹو_NS(x))
	PN(sysctl_sched_بنیاد_قاش);
	P(sysctl_sched_features);
#تعریف_منسوخ PN
#تعریف_منسوخ P

	SEQ_printf(m, "  .%-40s: %d (%s)\n",
		"sysctl_sched_tunable_scaling",
		sysctl_sched_tunable_scaling,
		sched_tunable_scaling_نام[sysctl_sched_tunable_scaling]);
	SEQ_printf(m, "\n");
}

ساکن صحیح sched_debug_show(ساخت seq_file *m, خالی *v)
{
	صحیح cpu = (غیرعلامتی طویل)(v - 2);

	اگر (cpu != -1)
		print_cpu(m, cpu);
	ورنہ
		sched_debug_سرخی(m);

	واپس 0;
}

خالی sysrq_sched_debug_show(خالی)
{
	صحیح cpu;

	sched_debug_سرخی(تہی);
	for_each_online_cpu(cpu) {
		/*
		 * Need to reset softlockup watchdogs on all CPUs, because
		 * another CPU might be blocked waiting for us to process
		 * an IPI or stop_machine.
		 */
		touch_nmi_watchdog();
		touch_all_softlockup_watchdogs();
		print_cpu(تہی, cpu);
	}
}

/*
 * This iterator needs some explanation.
 * It returns 1 for the header position.
 * This means 2 is CPU 0.
 * In a hotplugged system some CPUs, including CPU 0, may be missing so we have
 * to use cpumask_* to iterate over the CPUs.
 */
ساکن خالی *sched_debug_آغاز(ساخت seq_file *file, loff_t *انحراف)
{
	غیرعلامتی طویل n = *انحراف;

	اگر (n == 0)
		واپس (خالی *) 1;

	n--;

	اگر (n > 0)
		n = cpumask_اگلا(n - 1, cpu_online_نقاب);
	ورنہ
		n = cpumask_پہلا(cpu_online_نقاب);

	*انحراف = n + 1;

	اگر (n < nr_cpu_ids)
		واپس (خالی *)(غیرعلامتی طویل)(n + 2);

	واپس تہی;
}

ساکن خالی *sched_debug_اگلا(ساخت seq_file *file, خالی *ڈیٹا, loff_t *انحراف)
{
	(*انحراف)++;
	واپس sched_debug_آغاز(file, انحراف);
}

ساکن خالی sched_debug_stop(ساخت seq_file *file, خالی *ڈیٹا)
{
}

ساکن ثابت ساخت seq_operations sched_debug_sops = {
	.آغاز		= sched_debug_آغاز,
	.اگلا		= sched_debug_اگلا,
	.stop		= sched_debug_stop,
	.show		= sched_debug_show,
};

#تعریف_کرو __PS(S, F) SEQ_printf(m, "%-45s:%21Ld\n", S, (طویل طویل)(F))
#تعریف_کرو __P(F) __PS(#F, F)
#تعریف_کرو   P(F) __PS(#F, p->F)
#تعریف_کرو   PM(F, M) __PS(#F, p->F & (M))
#تعریف_کرو __PSN(S, F) SEQ_printf(m, "%-45s:%14Ld.%06ld\n", S, بانٹو_NS((طویل طویل)(F)))
#تعریف_کرو __PN(F) __PSN(#F, F)
#تعریف_کرو   PN(F) __PSN(#F, p->F)


#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING
خالی print_numa_stats(ساخت seq_file *m, صحیح گرہ, غیرعلامتی طویل tsf,
		غیرعلامتی طویل tpf, غیرعلامتی طویل gsf, غیرعلامتی طویل gpf)
{
	SEQ_printf(m, "numa_faults node=%d ", گرہ);
	SEQ_printf(m, "task_private=%lu task_shared=%lu ", tpf, tsf);
	SEQ_printf(m, "group_private=%lu group_shared=%lu\n", gpf, gsf);
}
#اگر_ختم


ساکن خالی sched_show_numa(ساخت task_struct *p, ساخت seq_file *m)
{
#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING
	اگر (p->mm)
		P(mm->numa_جائزہ_seq);

	P(numa_صفحات_migrated);
	P(numa_preferred_nid);
	P(کل_numa_خرابیاں);
	SEQ_printf(m, "current_node=%d, numa_group_id=%d\n",
			task_گرہ(p), task_numa_گروہ_id(p));
	show_numa_stats(p, m);
#اگر_ختم /* CONFIG_NUMA_BALANCING */
}

خالی proc_sched_show_task(ساخت task_struct *p, ساخت pid_namespace *ns,
						  ساخت seq_file *m)
{
	غیرعلامتی طویل nr_switches;

	SEQ_printf(m, "%s (%d, #threads: %d)\n", p->comm, task_pid_nr_ns(p, ns),
						get_nr_دھاگے(p));
	SEQ_printf(m,
		"---------------------------------------------------------"
		"----------\n");

#تعریف_کرو P_SCHEDSTAT(F)  __PS(#F, schedstat_val(p->stats.F))
#تعریف_کرو PN_SCHEDSTAT(F) __PSN(#F, schedstat_val(p->stats.F))

	PN(se.exec_آغاز);
	PN(se.vruntime);
	PN(se.مجموع_exec_runtime);

	nr_switches = p->nvcsw + p->nivcsw;

	P(se.nr_migrations);

	اگر (schedstat_enabled()) {
		u64 avg_atom, avg_فی_cpu;

		PN_SCHEDSTAT(مجموع_sleep_runtime);
		PN_SCHEDSTAT(مجموع_بلاک_runtime);
		PN_SCHEDSTAT(wait_آغاز);
		PN_SCHEDSTAT(sleep_آغاز);
		PN_SCHEDSTAT(بلاک_آغاز);
		PN_SCHEDSTAT(sleep_max);
		PN_SCHEDSTAT(بلاک_max);
		PN_SCHEDSTAT(exec_max);
		PN_SCHEDSTAT(قاش_max);
		PN_SCHEDSTAT(wait_max);
		PN_SCHEDSTAT(wait_مجموع);
		P_SCHEDSTAT(wait_گنتی);
		PN_SCHEDSTAT(iowait_مجموع);
		P_SCHEDSTAT(iowait_گنتی);
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

		avg_atom = p->se.مجموع_exec_runtime;
		اگر (nr_switches)
			avg_atom = div64_ul(avg_atom, nr_switches);
		ورنہ
			avg_atom = -1LL;

		avg_فی_cpu = p->se.مجموع_exec_runtime;
		اگر (p->se.nr_migrations) {
			avg_فی_cpu = div64_u64(avg_فی_cpu,
						p->se.nr_migrations);
		} ورنہ {
			avg_فی_cpu = -1LL;
		}

		__PN(avg_atom);
		__PN(avg_فی_cpu);

#اگر_تعریف_شدہ CONFIG_SCHED_CORE
		PN_SCHEDSTAT(core_forceidle_مجموع);
#اگر_ختم
	}

	__P(nr_switches);
	__PS("nr_voluntary_switches", p->nvcsw);
	__PS("nr_involuntary_switches", p->nivcsw);

	P(se.بھرو.وزن);
	P(se.avg.بھرو_مجموع);
	P(se.avg.runnable_مجموع);
	P(se.avg.util_مجموع);
	P(se.avg.بھرو_avg);
	P(se.avg.runnable_avg);
	P(se.avg.util_avg);
	P(se.avg.آخری_update_وقت);
	PM(se.avg.util_est, ~UTIL_AVG_UNCHANGED);
#اگر_تعریف_شدہ CONFIG_UCLAMP_TASK
	__PS("uclamp.min", p->uclamp_req[UCLAMP_MIN].قدر);
	__PS("uclamp.max", p->uclamp_req[UCLAMP_MAX].قدر);
	__PS("effective uclamp.min", uclamp_eff_قدر(p, UCLAMP_MIN));
	__PS("effective uclamp.max", uclamp_eff_قدر(p, UCLAMP_MAX));
#اگر_ختم /* CONFIG_UCLAMP_TASK */
	P(policy);
	P(prio);
	اگر (task_has_dl_policy(p)) {
		P(dl.runtime);
		P(dl.deadline);
	} ورنہ اگر (fair_policy(p->policy)) {
		P(se.قاش);
	}
#اگر_تعریف_شدہ CONFIG_SCHED_CLASS_EXT
	__PS("ext.enabled", task_on_scx(p));
#اگر_ختم
#تعریف_منسوخ PN_SCHEDSTAT
#تعریف_منسوخ P_SCHEDSTAT

	{
		غیرعلامتی صحیح this_cpu = raw_smp_processor_id();
		u64 t0, t1;

		t0 = cpu_clock(this_cpu);
		t1 = cpu_clock(this_cpu);
		__PS("clock-delta", t1-t0);
	}

	sched_show_numa(p, m);
}

خالی proc_sched_سیٹ_task(ساخت task_struct *p)
{
#اگر_تعریف_شدہ CONFIG_SCHEDSTATS
	memset(&p->stats, 0, حجم(p->stats));
#اگر_ختم
}

خالی resched_latency_warn(صحیح cpu, u64 latency)
{
	ساکن DEFINE_RATELIMIT_STATE(latency_check_ratelimit, 60 * 60 * HZ, 1);

	اگر (likely(!__ratelimit(&latency_check_ratelimit)))
		واپس;

	pr_err("sched: CPU %d need_resched set for > %llu ns (%d ticks) without schedule\n",
	       cpu, latency, cpu_rq(cpu)->ticks_without_resched);
	dump_ڈھیر();
}
