// SPDX-License-Identifier: GPL-2.0-only
/*
 * kernel/sched/debug.c
 *
 * Print the CFS rbtree and other debugging details
 *
 * Copyright(C) 2007, Red Hat, Inc., Ingo Molnar
 */
#ضمن <linux/debugfs.h>
#ضمن <linux/nmi.h>
#ضمن <linux/log2.h>
#ضمن "sched.h"

/*
 * This allows printing both to /sys/kernel/debug/sched/debug and
 * to the console
 */
#وصف_ڪريو SEQ_printf(m, x...)			\
 افعل {						\
	إذا (m)					\
		seq_printf(m, x);		\
	وإلا					\
		pr_cont(x);			\
 } طالما (0)

/*
 * Ease the printing of nsec fields:
 */
ساكن ڊگهو ڊگهو nsec_مٿاهون(بنانشان ڊگهو ڊگهو nsec)
{
	إذا ((ڊگهو ڊگهو)nsec < 0) {
		nsec = -nsec;
		do_div(nsec, 1000000);
		أرجع -nsec;
	}
	do_div(nsec, 1000000);

	أرجع nsec;
}

ساكن بنانشان ڊگهو nsec_هيٺيون(بنانشان ڊگهو ڊگهو nsec)
{
	إذا ((ڊگهو ڊگهو)nsec < 0)
		nsec = -nsec;

	أرجع do_div(nsec, 1000000);
}

#وصف_ڪريو ورهايو_NS(x) nsec_مٿاهون(x), nsec_هيٺيون(x)

#وصف_ڪريو SCHED_FEAT(name, enabled)	\
	#name ,

ساكن ثابت حرف * ثابت sched_feat_نالا[] = {
#ضمن "features.h"
};

#وصف_رد SCHED_FEAT

ساكن صحيح sched_feat_show(بنية seq_file *m, فراغ *v)
{
	صحيح i;

	لكل (i = 0; i < __SCHED_FEAT_NR; i++) {
		إذا (!(sysctl_sched_features & (1UL << i)))
			seq_puts(m, "NO_");
		seq_printf(m, "%s ", sched_feat_نالا[i]);
	}
	seq_puts(m, "\n");

	أرجع 0;
}

#جيڪڏهن_وصف_ٿيل CONFIG_JUMP_LABEL

#وصف_ڪريو jump_label_ڪنجي_true  { .ڪنجي_true  = STATIC_ڪنجي_TRUE_INIT  }
#وصف_ڪريو jump_label_ڪنجي_false { .ڪنجي_false = STATIC_ڪنجي_FALSE_INIT }

#وصف_ڪريو SCHED_FEAT(name, enabled)	\
	jump_label_ڪنجي##enabled ,

اتحاد sched_feat_ڪنجي sched_feat_ڪنجيون[__SCHED_FEAT_NR] = {
#ضمن "features.h"
};

#وصف_رد SCHED_FEAT

ساكن فراغ sched_feat_disable(صحيح i)
{
	static_branch_disable_cpuslocked(&sched_feat_ڪنجيون[i].ڪنجي_true);
}

ساكن فراغ sched_feat_enable(صحيح i)
{
	static_branch_enable_cpuslocked(&sched_feat_ڪنجيون[i].ڪنجي_false);
}
#وإلا /* !CONFIG_JUMP_LABEL: */
ساكن فراغ sched_feat_disable(صحيح i) { };
ساكن فراغ sched_feat_enable(صحيح i) { };
#جيڪڏهن_پورو /* !CONFIG_JUMP_LABEL */

ساكن صحيح sched_feat_سيٽ(حرف *cmp)
{
	صحيح i;
	صحيح neg = 0;

	إذا (لڙي_ڀيٽ(cmp, "NO_", 3) == 0) {
		neg = 1;
		cmp += 3;
	}

	i = ميلاپ_string(sched_feat_نالا, __SCHED_FEAT_NR, cmp);
	إذا (i < 0)
		أرجع i;

	إذا (neg) {
		sysctl_sched_features &= ~(1UL << i);
		sched_feat_disable(i);
	} وإلا {
		sysctl_sched_features |= (1UL << i);
		sched_feat_enable(i);
	}

	أرجع 0;
}

ساكن ssize_t
sched_feat_write(بنية file *filp, ثابت حرف __user *ubuf,
		سائيز_t cnt, loff_t *ppos)
{
	حرف buf[64];
	حرف *cmp;
	صحيح ret;
	بنية inode *inode;

	إذا (cnt > 63)
		cnt = 63;

	إذا (copy_from_user(buf, ubuf, cnt))
		أرجع -EFAULT;

	buf[cnt] = 0;
	cmp = strstrip(buf);

	/* Ensure the static_key remains in a consistent state */
	inode = file_inode(filp);
	cpus_read_تالو();
	inode_تالو(inode);
	ret = sched_feat_سيٽ(cmp);
	inode_unlock(inode);
	cpus_read_unlock();
	إذا (ret < 0)
		أرجع ret;

	*ppos += cnt;

	أرجع cnt;
}

ساكن صحيح sched_feat_open(بنية inode *inode, بنية file *filp)
{
	أرجع single_open(filp, sched_feat_show, عدم);
}

ساكن ثابت بنية file_operations sched_feat_fops = {
	.open		= sched_feat_open,
	.اكتب		= sched_feat_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ساكن ssize_t sched_scaling_write(بنية file *filp, ثابت حرف __user *ubuf,
				   سائيز_t cnt, loff_t *ppos)
{
	بنانشان صحيح scaling;
	صحيح ret;

	ret = kstrtouint_from_user(ubuf, cnt, 10, &scaling);
	إذا (ret)
		أرجع ret;

	إذا (scaling >= SCHED_TUNABLESCALING_پڇاڙي)
		أرجع -EINVAL;

	sysctl_sched_tunable_scaling = scaling;
	إذا (sched_update_scaling())
		أرجع -EINVAL;

	*ppos += cnt;
	أرجع cnt;
}

ساكن صحيح sched_scaling_show(بنية seq_file *m, فراغ *v)
{
	seq_printf(m, "%d\n", sysctl_sched_tunable_scaling);
	أرجع 0;
}

ساكن صحيح sched_scaling_open(بنية inode *inode, بنية file *filp)
{
	أرجع single_open(filp, sched_scaling_show, عدم);
}

ساكن ثابت بنية file_operations sched_scaling_fops = {
	.open		= sched_scaling_open,
	.اكتب		= sched_scaling_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CACHE
ساكن ssize_t
sched_cache_enable_write(بنية file *filp, ثابت حرف __user *ubuf,
			 سائيز_t cnt, loff_t *ppos)
{
	bool val;
	صحيح ret;

	ret = kstrtobool_from_user(ubuf, cnt, &val);
	إذا (ret)
		أرجع ret;

	sysctl_sched_cache_user = val;

	sched_cache_سرگرم_سيٽ();

	*ppos += cnt;

	أرجع cnt;
}

ساكن صحيح sched_cache_enable_show(بنية seq_file *m, فراغ *v)
{
	seq_printf(m, "%d\n", sysctl_sched_cache_user);
	أرجع 0;
}

ساكن صحيح sched_cache_enable_open(بنية inode *inode,
				   بنية file *filp)
{
	أرجع single_open(filp, sched_cache_enable_show, عدم);
}

ساكن ثابت بنية file_operations sched_cache_enable_fops = {
	.open           = sched_cache_enable_open,
	.اكتب          = sched_cache_enable_write,
	.اقرأ           = seq_read,
	.llseek         = seq_lseek,
	.release        = single_release,
};
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل CONFIG_PREEMPT_DYNAMIC

ساكن ssize_t sched_dynamic_write(بنية file *filp, ثابت حرف __user *ubuf,
				   سائيز_t cnt, loff_t *ppos)
{
	حرف buf[16];
	صحيح گهڻائي;

	إذا (cnt > 15)
		cnt = 15;

	إذا (copy_from_user(buf, ubuf, cnt))
		أرجع -EFAULT;

	buf[cnt] = 0;
	گهڻائي = sched_dynamic_گهڻائي(strstrip(buf));
	إذا (گهڻائي < 0)
		أرجع گهڻائي;

	sched_dynamic_update(گهڻائي);

	*ppos += cnt;

	أرجع cnt;
}

ساكن صحيح sched_dynamic_show(بنية seq_file *m, فراغ *v)
{
	صحيح گهڻائي = READ_ONCE(preempt_dynamic_گهڻائي);

	/* Stop at NULL terminator */
	لكل (صحيح i = 0; preempt_modes[i]; i++) {
		إذا (گهڻائي == i)
			seq_puts(m, "(");
		seq_puts(m, preempt_modes[i]);
		إذا (گهڻائي == i)
			seq_puts(m, ")");

		seq_puts(m, " ");
	}

	seq_puts(m, "\n");
	أرجع 0;
}

ساكن صحيح sched_dynamic_open(بنية inode *inode, بنية file *filp)
{
	أرجع single_open(filp, sched_dynamic_show, عدم);
}

ساكن ثابت بنية file_operations sched_dynamic_fops = {
	.open		= sched_dynamic_open,
	.اكتب		= sched_dynamic_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#جيڪڏهن_پورو /* CONFIG_PREEMPT_DYNAMIC */

__read_mostly bool sched_debug_verbose;

ساكن بنية dentry           *sd_dentry;


ساكن ssize_t sched_verbose_write(بنية file *filp, ثابت حرف __user *ubuf,
				  سائيز_t cnt, loff_t *ppos)
{
	ssize_t نتيجو;
	bool orig;

	cpus_read_تالو();
	sched_domains_mutex_تالو();

	orig = sched_debug_verbose;
	نتيجو = debugfs_write_file_bool(filp, ubuf, cnt, ppos);

	إذا (sched_debug_verbose && !orig)
		update_sched_domain_debugfs();
	وإلا إذا (!sched_debug_verbose && orig) {
		debugfs_remove(sd_dentry);
		sd_dentry = عدم;
	}

	sched_domains_mutex_unlock();
	cpus_read_unlock();

	أرجع نتيجو;
}

ساكن ثابت بنية file_operations sched_verbose_fops = {
	.اقرأ =         debugfs_read_file_bool,
	.اكتب =        sched_verbose_write,
	.open =         simple_open,
	.llseek =       default_llseek,
};

ساكن ثابت بنية seq_operations sched_debug_sops;

ساكن صحيح sched_debug_open(بنية inode *inode, بنية file *filp)
{
	أرجع seq_open(filp, &sched_debug_sops);
}

ساكن ثابت بنية file_operations sched_debug_fops = {
	.open		= sched_debug_open,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

تعداد dl_param {
	DL_RUNTIME = 0,
	DL_PERIOD,
};

ساكن بنانشان ڊگهو dl_server_period_max = (1UL << 22) * NSEC_في_USEC; /* ~4 seconds */
ساكن بنانشان ڊگهو dl_server_period_min = (100) * NSEC_في_USEC;     /* 100 us */

ساكن ssize_t sched_server_write_common(بنية file *filp, ثابت حرف __user *ubuf,
					 سائيز_t cnt, loff_t *ppos, تعداد dl_param param,
					 فراغ *server)
{
	ڊگهو cpu = (ڊگهو) ((بنية seq_file *) filp->ذاتي_معلومات)->خاص;
	بنية sched_dl_entity *dl_se = (بنية sched_dl_entity *)server;
	u64 old_runtime, runtime, period;
	بنية rq *rq = cpu_rq(cpu);
	صحيح retval = 0;
	سائيز_t err;
	u64 قدر;

	err = kstrtoull_from_user(ubuf, cnt, 10, &قدر);
	إذا (err)
		أرجع err;

	scoped_guard (rq_تالو_irqsave, rq) {
		old_runtime = runtime = dl_se->dl_runtime;
		period = dl_se->dl_period;

		بدل (param) {
		حالة DL_RUNTIME:
			إذا (runtime == قدر)
				اكسر;
			runtime = قدر;
			اكسر;
		حالة DL_PERIOD:
			إذا (قدر == period)
				اكسر;
			period = قدر;
			اكسر;
		}

		إذا (runtime > period ||
		    period > dl_server_period_max ||
		    period < dl_server_period_min) {
			أرجع  -EINVAL;
		}

		إذا (!cpu_online(cpu_جو(rq)))
			أرجع -EBUSY;

		update_rq_clock(rq);
		dl_server_stop(dl_se);
		retval = dl_server_apply_params(dl_se, runtime, period, 0);
		dl_server_شروعات(dl_se);

		إذا (retval < 0)
			أرجع retval;
	}

	إذا (!!old_runtime ^ !!runtime) {
		pr_info("%s server %sabled on CPU %d%s.\n",
			server == &rq->fair_server ? "Fair" : "Ext",
			runtime ? "en" : "dis",
			cpu_جو(rq),
			runtime ? "" : ", system may malfunction due to starvation");
	}

	*ppos += cnt;
	أرجع cnt;
}

ساكن سائيز_t sched_server_show_common(بنية seq_file *m, فراغ *v, تعداد dl_param param,
				       فراغ *server)
{
	بنية sched_dl_entity *dl_se = (بنية sched_dl_entity *)server;
	u64 قدر;

	بدل (param) {
	حالة DL_RUNTIME:
		قدر = dl_se->dl_runtime;
		اكسر;
	حالة DL_PERIOD:
		قدر = dl_se->dl_period;
		اكسر;
	}

	seq_printf(m, "%llu\n", قدر);
	أرجع 0;
}

ساكن ssize_t
sched_fair_server_runtime_write(بنية file *filp, ثابت حرف __user *ubuf,
				سائيز_t cnt, loff_t *ppos)
{
	ڊگهو cpu = (ڊگهو) ((بنية seq_file *) filp->ذاتي_معلومات)->خاص;
	بنية rq *rq = cpu_rq(cpu);

	أرجع sched_server_write_common(filp, ubuf, cnt, ppos, DL_RUNTIME,
					&rq->fair_server);
}

ساكن صحيح sched_fair_server_runtime_show(بنية seq_file *m, فراغ *v)
{
	بنانشان ڊگهو cpu = (بنانشان ڊگهو) m->خاص;
	بنية rq *rq = cpu_rq(cpu);

	أرجع sched_server_show_common(m, v, DL_RUNTIME, &rq->fair_server);
}

ساكن صحيح sched_fair_server_runtime_open(بنية inode *inode, بنية file *filp)
{
	أرجع single_open(filp, sched_fair_server_runtime_show, inode->i_ذاتي);
}

ساكن ثابت بنية file_operations fair_server_runtime_fops = {
	.open		= sched_fair_server_runtime_open,
	.اكتب		= sched_fair_server_runtime_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ساكن بنية dentry *debugfs_sched;

#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CLASS_EXT
ساكن ssize_t
sched_ext_server_runtime_write(بنية file *filp, ثابت حرف __user *ubuf,
			       سائيز_t cnt, loff_t *ppos)
{
	ڊگهو cpu = (ڊگهو) ((بنية seq_file *) filp->ذاتي_معلومات)->خاص;
	بنية rq *rq = cpu_rq(cpu);

	أرجع sched_server_write_common(filp, ubuf, cnt, ppos, DL_RUNTIME,
					&rq->ext_server);
}

ساكن صحيح sched_ext_server_runtime_show(بنية seq_file *m, فراغ *v)
{
	بنانشان ڊگهو cpu = (بنانشان ڊگهو) m->خاص;
	بنية rq *rq = cpu_rq(cpu);

	أرجع sched_server_show_common(m, v, DL_RUNTIME, &rq->ext_server);
}

ساكن صحيح sched_ext_server_runtime_open(بنية inode *inode, بنية file *filp)
{
	أرجع single_open(filp, sched_ext_server_runtime_show, inode->i_ذاتي);
}

ساكن ثابت بنية file_operations ext_server_runtime_fops = {
	.open		= sched_ext_server_runtime_open,
	.اكتب		= sched_ext_server_runtime_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ساكن ssize_t
sched_ext_server_period_write(بنية file *filp, ثابت حرف __user *ubuf,
			      سائيز_t cnt, loff_t *ppos)
{
	ڊگهو cpu = (ڊگهو) ((بنية seq_file *) filp->ذاتي_معلومات)->خاص;
	بنية rq *rq = cpu_rq(cpu);

	أرجع sched_server_write_common(filp, ubuf, cnt, ppos, DL_PERIOD,
					&rq->ext_server);
}

ساكن صحيح sched_ext_server_period_show(بنية seq_file *m, فراغ *v)
{
	بنانشان ڊگهو cpu = (بنانشان ڊگهو) m->خاص;
	بنية rq *rq = cpu_rq(cpu);

	أرجع sched_server_show_common(m, v, DL_PERIOD, &rq->ext_server);
}

ساكن صحيح sched_ext_server_period_open(بنية inode *inode, بنية file *filp)
{
	أرجع single_open(filp, sched_ext_server_period_show, inode->i_ذاتي);
}

ساكن ثابت بنية file_operations ext_server_period_fops = {
	.open		= sched_ext_server_period_open,
	.اكتب		= sched_ext_server_period_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ساكن فراغ debugfs_ext_server_init(فراغ)
{
	بنية dentry *d_ext;
	بنانشان ڊگهو cpu;

	d_ext = debugfs_create_dir("ext_server", debugfs_sched);
	إذا (!d_ext)
		أرجع;

	for_each_possible_cpu(cpu) {
		بنية dentry *d_cpu;
		حرف buf[32];

		snprintf(buf, حجم(buf), "cpu%lu", cpu);
		d_cpu = debugfs_create_dir(buf, d_ext);

		debugfs_create_file("runtime", 0644, d_cpu, (فراغ *) cpu, &ext_server_runtime_fops);
		debugfs_create_file("period", 0644, d_cpu, (فراغ *) cpu, &ext_server_period_fops);
	}
}
#جيڪڏهن_پورو /* CONFIG_SCHED_CLASS_EXT */

ساكن ssize_t
sched_fair_server_period_write(بنية file *filp, ثابت حرف __user *ubuf,
			       سائيز_t cnt, loff_t *ppos)
{
	ڊگهو cpu = (ڊگهو) ((بنية seq_file *) filp->ذاتي_معلومات)->خاص;
	بنية rq *rq = cpu_rq(cpu);

	أرجع sched_server_write_common(filp, ubuf, cnt, ppos, DL_PERIOD,
					&rq->fair_server);
}

ساكن صحيح sched_fair_server_period_show(بنية seq_file *m, فراغ *v)
{
	بنانشان ڊگهو cpu = (بنانشان ڊگهو) m->خاص;
	بنية rq *rq = cpu_rq(cpu);

	أرجع sched_server_show_common(m, v, DL_PERIOD, &rq->fair_server);
}

ساكن صحيح sched_fair_server_period_open(بنية inode *inode, بنية file *filp)
{
	أرجع single_open(filp, sched_fair_server_period_show, inode->i_ذاتي);
}

ساكن ثابت بنية file_operations fair_server_period_fops = {
	.open		= sched_fair_server_period_open,
	.اكتب		= sched_fair_server_period_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ساكن فراغ debugfs_fair_server_init(فراغ)
{
	بنية dentry *d_fair;
	بنانشان ڊگهو cpu;

	d_fair = debugfs_create_dir("fair_server", debugfs_sched);
	إذا (!d_fair)
		أرجع;

	for_each_possible_cpu(cpu) {
		بنية dentry *d_cpu;
		حرف buf[32];

		snprintf(buf, حجم(buf), "cpu%lu", cpu);
		d_cpu = debugfs_create_dir(buf, d_fair);

		debugfs_create_file("runtime", 0644, d_cpu, (فراغ *) cpu, &fair_server_runtime_fops);
		debugfs_create_file("period", 0644, d_cpu, (فراغ *) cpu, &fair_server_period_fops);
	}
}

#جيڪڏهن_وصف_ٿيل CONFIG_FAIR_گروهه_SCHED
ساكن صحيح cgroup_گهڻائي = 2;

/* See __sched_cgroup_mode_update(). */
ساكن ثابت حرف *cgroup_گهڻائي_str[] = {
	"up",
	"smp",
	"concur",
	"max",
	"tasks",
};

ساكن صحيح sched_cgroup_گهڻائي(ثابت حرف *str)
{
	لكل (صحيح i = 0; i < ARRAY_سائيز(cgroup_گهڻائي_str); i++) {
		إذا (!strcmp(str, cgroup_گهڻائي_str[i]))
			أرجع i;
	}
	أرجع -EINVAL;
}

ساكن ssize_t sched_cgroup_write(بنية file *filp, ثابت حرف __user *ubuf,
				   سائيز_t cnt, loff_t *ppos)
{
	حرف buf[16];
	صحيح گهڻائي;

	إذا (cnt > 15)
		cnt = 15;

	إذا (copy_from_user(buf, ubuf, cnt))
		أرجع -EFAULT;

	buf[cnt] = 0;
	گهڻائي = sched_cgroup_گهڻائي(strstrip(buf));
	إذا (گهڻائي < 0)
		أرجع گهڻائي;

	__sched_cgroup_mode_update(گهڻائي);
	WRITE_ONCE(cgroup_گهڻائي, گهڻائي);

	*ppos += cnt;
	أرجع cnt;
}

ساكن صحيح sched_cgroup_show(بنية seq_file *m, فراغ *v)
{
	صحيح گهڻائي = READ_ONCE(cgroup_گهڻائي);

	لكل (صحيح i = 0; i < ARRAY_سائيز(cgroup_گهڻائي_str); i++) {
		إذا (گهڻائي == i)
			seq_puts(m, "(");
		seq_puts(m, cgroup_گهڻائي_str[i]);
		إذا (گهڻائي == i)
			seq_puts(m, ")");

		seq_puts(m, " ");
	}
	seq_puts(m, "\n");
	أرجع 0;
}

ساكن صحيح sched_cgroup_open(بنية inode *inode, بنية file *filp)
{
	أرجع single_open(filp, sched_cgroup_show, عدم);
}

ساكن ثابت بنية file_operations sched_cgroup_fops = {
	.open		= sched_cgroup_open,
	.اكتب		= sched_cgroup_write,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};
#جيڪڏهن_پورو

ساكن __init صحيح sched_init_debug(فراغ)
{
	بنية dentry __maybe_unused *numa, *llc;

	debugfs_sched = debugfs_create_dir("sched", عدم);

	debugfs_create_file("features", 0644, debugfs_sched, عدم, &sched_feat_fops);
	debugfs_create_file_unsafe("verbose", 0644, debugfs_sched, &sched_debug_verbose, &sched_verbose_fops);
#جيڪڏهن_وصف_ٿيل CONFIG_PREEMPT_DYNAMIC
	debugfs_create_file("preempt", 0644, debugfs_sched, عدم, &sched_dynamic_fops);
#جيڪڏهن_پورو

	debugfs_create_u32("base_slice_ns", 0644, debugfs_sched, &sysctl_sched_بنياد_ٽڪرو);

	debugfs_create_u32("latency_warn_ms", 0644, debugfs_sched, &sysctl_resched_latency_warn_ms);
	debugfs_create_u32("latency_warn_once", 0644, debugfs_sched, &sysctl_resched_latency_warn_once);

	debugfs_create_file("tunable_scaling", 0644, debugfs_sched, عدم, &sched_scaling_fops);
	debugfs_create_u32("migration_cost_ns", 0644, debugfs_sched, &sysctl_sched_migration_لاڳت);
	debugfs_create_u32("nr_migrate", 0644, debugfs_sched, &sysctl_sched_nr_migrate);

	sched_domains_mutex_تالو();
	update_sched_domain_debugfs();
	sched_domains_mutex_unlock();

#جيڪڏهن_وصف_ٿيل CONFIG_NUMA_BALANCING
	numa = debugfs_create_dir("numa_balancing", debugfs_sched);

	debugfs_create_u32("scan_delay_ms", 0644, numa, &sysctl_numa_balancing_جاچ_delay);
	debugfs_create_u32("scan_period_min_ms", 0644, numa, &sysctl_numa_balancing_جاچ_period_min);
	debugfs_create_u32("scan_period_max_ms", 0644, numa, &sysctl_numa_balancing_جاچ_period_max);
	debugfs_create_u32("scan_size_mb", 0644, numa, &sysctl_numa_balancing_جاچ_سائيز);
	debugfs_create_u32("hot_threshold_ms", 0644, numa, &sysctl_numa_balancing_hot_حد);
#جيڪڏهن_پورو /* CONFIG_NUMA_BALANCING */

#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CACHE
	llc = debugfs_create_dir("llc_balancing", debugfs_sched);
	debugfs_create_file("enabled", 0644, llc, عدم,
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
#جيڪڏهن_پورو

	debugfs_create_file("debug", 0444, debugfs_sched, عدم, &sched_debug_fops);

#جيڪڏهن_وصف_ٿيل CONFIG_FAIR_گروهه_SCHED
	debugfs_create_file("cgroup_mode", 0644, debugfs_sched, عدم, &sched_cgroup_fops);
#جيڪڏهن_پورو

	debugfs_fair_server_init();
#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CLASS_EXT
	debugfs_ext_server_init();
#جيڪڏهن_پورو

	أرجع 0;
}
late_initcall(sched_init_debug);

ساكن cpumask_var_t		sd_sysctl_cpus;

ساكن صحيح sd_flags_show(بنية seq_file *m, فراغ *v)
{
	بنانشان ڊگهو flags = *(بنانشان صحيح *)m->خاص;
	صحيح idx;

	for_each_سيٽ_ٻٽو_انگ(idx, &flags, __SD_FLAG_CNT) {
		seq_puts(m, sd_flag_debug[idx].name);
		seq_puts(m, " ");
	}
	seq_puts(m, "\n");

	أرجع 0;
}

ساكن صحيح sd_flags_open(بنية inode *inode, بنية file *file)
{
	أرجع single_open(file, sd_flags_show, inode->i_ذاتي);
}

ساكن ثابت بنية file_operations sd_flags_fops = {
	.open		= sd_flags_open,
	.اقرأ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

ساكن فراغ رجسٽر_sd(بنية sched_domain *sd, بنية dentry *والد)
{
#وصف_ڪريو SDM(type, گهڻائي, member)	\
	debugfs_create_##type(#member, گهڻائي, والد, &sd->member)

	SDM(ulong, 0644, min_interval);
	SDM(ulong, 0644, max_interval);
	SDM(u64,   0644, max_newidle_lb_لاڳت);
	SDM(u32,   0644, busy_ضرب_ڪندڙ);
	SDM(u32,   0644, imbalance_pct);
	SDM(u32,   0644, cache_nice_tries);
	SDM(str,   0444, name);

#وصف_رد SDM

	debugfs_create_file("flags", 0444, والد, &sd->flags, &sd_flags_fops);
	debugfs_create_file("groups_flags", 0444, والد, &sd->groups->flags, &sd_flags_fops);
	debugfs_create_u32("level", 0444, والد, (u32 *)&sd->level);

	إذا (sd->flags & SD_ASYM_PACKING)
		debugfs_create_u32("group_asym_prefer_cpu", 0444, والد,
				   (u32 *)&sd->groups->asym_prefer_cpu);
}

فراغ update_sched_domain_debugfs(فراغ)
{
	صحيح cpu, i;

	/*
	 * This can unfortunately be invoked before sched_debug_init() creates
	 * the debug directory. Don't touch sd_sysctl_cpus until then.
	 */
	إذا (!debugfs_sched)
		أرجع;

	إذا (!sched_debug_verbose)
		أرجع;

	إذا (!cpumask_موجود(sd_sysctl_cpus)) {
		إذا (!alloc_cpumask_var(&sd_sysctl_cpus, GFP_KERNEL))
			أرجع;
		cpumask_copy(sd_sysctl_cpus, cpu_possible_نقاب);
	}

	إذا (!sd_dentry) {
		sd_dentry = debugfs_create_dir("domains", debugfs_sched);

		/* rebuild sd_sysctl_cpus if empty since it gets cleared below */
		إذا (cpumask_خالي_جايون(sd_sysctl_cpus))
			cpumask_copy(sd_sysctl_cpus, cpu_online_نقاب);
	}

	for_each_cpu(cpu, sd_sysctl_cpus) {
		بنية sched_domain *sd;
		بنية dentry *d_cpu;
		حرف buf[32];

		snprintf(buf, حجم(buf), "cpu%d", cpu);
		debugfs_lookup_and_remove(buf, sd_dentry);
		d_cpu = debugfs_create_dir(buf, sd_dentry);

		i = 0;
		for_each_domain(cpu, sd) {
			بنية dentry *d_sd;

			snprintf(buf, حجم(buf), "domain%d", i);
			d_sd = debugfs_create_dir(buf, d_cpu);

			رجسٽر_sd(sd, d_sd);
			i++;
		}

		__cpumask_clear_cpu(cpu, sd_sysctl_cpus);
	}
}

فراغ dirty_sched_domain_sysctl(صحيح cpu)
{
	إذا (cpumask_موجود(sd_sysctl_cpus))
		__cpumask_set_cpu(cpu, sd_sysctl_cpus);
}

#جيڪڏهن_وصف_ٿيل CONFIG_FAIR_گروهه_SCHED
ساكن فراغ print_cfs_گروهه_stats(بنية seq_file *m, صحيح cpu, بنية task_گروهه *tg)
{
	بنية sched_entity *se = tg_se(tg, cpu);

#وصف_ڪريو P(F)		SEQ_printf(m, "  .%-30s: %lld\n",	#F, (ڊگهو ڊگهو)F)
#وصف_ڪريو P_SCHEDSTAT(F)	SEQ_printf(m, "  .%-30s: %lld\n",	\
		#F, (ڊگهو ڊگهو)schedstat_val(stats->F))
#وصف_ڪريو PN(F)		SEQ_printf(m, "  .%-30s: %lld.%06ld\n", #F, ورهايو_NS((ڊگهو ڊگهو)F))
#وصف_ڪريو PN_SCHEDSTAT(F)	SEQ_printf(m, "  .%-30s: %lld.%06ld\n", \
		#F, ورهايو_NS((ڊگهو ڊگهو)schedstat_val(stats->F)))

	إذا (!se)
		أرجع;

	PN(se->exec_شروعات);
	PN(se->vruntime);
	PN(se->جوڙ_exec_runtime);

	إذا (schedstat_enabled()) {
		بنية sched_statistics *stats;
		stats = __schedstats_from_se(se);

		PN_SCHEDSTAT(wait_شروعات);
		PN_SCHEDSTAT(sleep_شروعات);
		PN_SCHEDSTAT(بلاڪ_شروعات);
		PN_SCHEDSTAT(sleep_max);
		PN_SCHEDSTAT(بلاڪ_max);
		PN_SCHEDSTAT(exec_max);
		PN_SCHEDSTAT(ٽڪرو_max);
		PN_SCHEDSTAT(wait_max);
		PN_SCHEDSTAT(wait_جوڙ);
		P_SCHEDSTAT(wait_ڳڻپ);
	}

	P(se->ڀريو.وزن);
	P(se->avg.ڀريو_avg);
	P(se->avg.util_avg);
	P(se->avg.runnable_avg);

#وصف_رد PN_SCHEDSTAT
#وصف_رد PN
#وصف_رد P_SCHEDSTAT
#وصف_رد P
}
#جيڪڏهن_پورو /* CONFIG_FAIR_GROUP_SCHED */

#جيڪڏهن_وصف_ٿيل CONFIG_CGROUP_SCHED
ساكن DEFINE_SPINLOCK(sched_debug_تالو);
ساكن حرف گروهه_رستو[رستو_MAX];

ساكن فراغ task_گروهه_رستو(بنية task_گروهه *tg, حرف *رستو, صحيح plen)
{
	إذا (autogroup_رستو(tg, رستو, plen))
		أرجع;

	cgroup_رستو(tg->css.cgroup, رستو, plen);
}

/*
 * Only 1 SEQ_printf_task_group_path() caller can use the full length
 * group_path[] for cgroup path. Other simultaneous callers will have
 * to use a shorter stack buffer. A "..." suffix is appended at the end
 * of the stack buffer so that it will show up in case the output length
 * matches the given buffer size to indicate possible path name truncation.
 */
#وصف_ڪريو SEQ_printf_task_گروهه_رستو(m, tg, fmt...)			\
{									\
	إذا (spin_trylock(&sched_debug_تالو)) {				\
		task_گروهه_رستو(tg, گروهه_رستو, حجم(گروهه_رستو));	\
		SEQ_printf(m, fmt, گروهه_رستو);				\
		spin_unlock(&sched_debug_تالو);				\
	} وإلا {							\
		حرف buf[128];						\
		حرف *bufend = buf + حجم(buf) - 3;			\
		task_گروهه_رستو(tg, buf, bufend - buf);			\
		strcpy(bufend - 1, "...");				\
		SEQ_printf(m, fmt, buf);				\
	}								\
}
#جيڪڏهن_پورو

ساكن فراغ
print_task(بنية seq_file *m, بنية rq *rq, بنية task_struct *p)
{
	إذا (task_current(rq, p))
		SEQ_printf(m, ">R");
	وإلا
		SEQ_printf(m, " %c", task_state_منزل_char(p));

	SEQ_printf(m, " %15s %5d %10ld %9Ld.%06ld   %c   %9Ld.%06ld %c %9Ld.%06ld %9Ld.%06ld %9Ld   %5d ",
		p->comm, task_pid_nr(p),
		p->se.h_ڀريو.وزن,
		ورهايو_NS(p->se.vruntime),
		entity_eligible(&rq->cfs, &p->se) ? 'E' : 'N',
		ورهايو_NS(p->se.deadline),
		p->se.custom_ٽڪرو ? 'S' : ' ',
		ورهايو_NS(p->se.ٽڪرو),
		ورهايو_NS(p->se.جوڙ_exec_runtime),
		(ڊگهو ڊگهو)(p->nvcsw + p->nivcsw),
		p->prio);

	SEQ_printf(m, "%9lld.%06ld %9lld.%06ld %9lld.%06ld",
		ورهايو_NS(schedstat_val_or_zero(p->stats.wait_جوڙ)),
		ورهايو_NS(schedstat_val_or_zero(p->stats.جوڙ_sleep_runtime)),
		ورهايو_NS(schedstat_val_or_zero(p->stats.جوڙ_بلاڪ_runtime)));

#جيڪڏهن_وصف_ٿيل CONFIG_NUMA_BALANCING
	SEQ_printf(m, "   %d      %d", task_ڳنڍ(p), task_numa_گروهه_id(p));
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل CONFIG_CGROUP_SCHED
	SEQ_printf_task_گروهه_رستو(m, task_گروهه(p), "        %s")
#جيڪڏهن_پورو

	SEQ_printf(m, "\n");
}

ساكن فراغ print_rq(بنية seq_file *m, بنية rq *rq, صحيح rq_cpu)
{
	بنية task_struct *g, *p;

	SEQ_printf(m, "\n");
	SEQ_printf(m, "runnable tasks:\n");
	SEQ_printf(m, " S            task   PID     weight       vruntime   eligible    "
		   "deadline             slice          sum-exec      switches  "
		   "prio         wait-time        sum-sleep       sum-block"
#جيڪڏهن_وصف_ٿيل CONFIG_NUMA_BALANCING
		   "  node   group-id"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل CONFIG_CGROUP_SCHED
		   "  group-path"
#جيڪڏهن_پورو
		   "\n");
	SEQ_printf(m, "-------------------------------------------------------"
		   "------------------------------------------------------"
		   "------------------------------------------------------"
#جيڪڏهن_وصف_ٿيل CONFIG_NUMA_BALANCING
		   "--------------"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل CONFIG_CGROUP_SCHED
		   "--------------"
#جيڪڏهن_پورو
		   "\n");

	rcu_read_تالو();
	for_each_process_thread(g, p) {
		إذا (task_cpu(p) != rq_cpu)
			استمر;

		print_task(m, rq, p);
	}
	rcu_read_unlock();
}

فراغ print_cfs_rq(بنية seq_file *m, صحيح cpu, بنية cfs_rq *cfs_rq)
{
	s64 کاٻو_vruntime = -1, ساڄو_vruntime = -1, کاٻو_deadline = -1, پکيڙ;
	s64 zero_vruntime = -1, جوڙ_w_vruntime = -1;
	u64 avruntime;
	بنية sched_entity *آخري, *پهريون, *پاڙ;
	بنية rq *rq = cpu_rq(cpu);
	بنانشان صحيح جوڙ_سرڪايو;
	بنانشان ڊگهو flags;
	u64 جوڙ_وزن;

#جيڪڏهن_وصف_ٿيل CONFIG_FAIR_گروهه_SCHED
	SEQ_printf(m, "\n");
	SEQ_printf_task_گروهه_رستو(m, cfs_rq->tg, "cfs_rq[%d]:%s\n", cpu);
#وإلا
	SEQ_printf(m, "\n");
	SEQ_printf(m, "cfs_rq[%d]:\n", cpu);
#جيڪڏهن_پورو

	raw_spin_rq_تالو_irqsave(rq, flags);
	پاڙ = __pick_root_entity(cfs_rq);
	إذا (پاڙ)
		کاٻو_vruntime = پاڙ->min_vruntime;
	پهريون = __pick_first_entity(cfs_rq);
	إذا (پهريون)
		کاٻو_deadline = پهريون->deadline;
	آخري = __pick_last_entity(cfs_rq);
	إذا (آخري)
		ساڄو_vruntime = آخري->vruntime;
	zero_vruntime = cfs_rq->zero_vruntime;
	جوڙ_w_vruntime = cfs_rq->جوڙ_w_vruntime;
	جوڙ_وزن = cfs_rq->جوڙ_وزن;
	جوڙ_سرڪايو = cfs_rq->جوڙ_سرڪايو;
	avruntime = avg_vruntime(cfs_rq);
	raw_spin_rq_unlock_irqrestore(rq, flags);

	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "left_deadline",
			ورهايو_NS(کاٻو_deadline));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "left_vruntime",
			ورهايو_NS(کاٻو_vruntime));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "zero_vruntime",
			ورهايو_NS(zero_vruntime));
	SEQ_printf(m, "  .%-30s: %Ld (%d bits)\n", "sum_w_vruntime",
		   جوڙ_w_vruntime, ilog2(مطلق_قدر(جوڙ_w_vruntime)));
	SEQ_printf(m, "  .%-30s: %Lu\n", "sum_weight",
		   جوڙ_وزن);
	SEQ_printf(m, "  .%-30s: %u\n", "sum_shift", جوڙ_سرڪايو);
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "avg_vruntime",
			ورهايو_NS(avruntime));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "right_vruntime",
			ورهايو_NS(ساڄو_vruntime));
	پکيڙ = ساڄو_vruntime - کاٻو_vruntime;
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "spread", ورهايو_NS(پکيڙ));
	SEQ_printf(m, "  .%-30s: %d\n", "nr_queued", cfs_rq->nr_queued);
	SEQ_printf(m, "  .%-30s: %d\n", "h_nr_runnable", cfs_rq->h_nr_runnable);
	SEQ_printf(m, "  .%-30s: %d\n", "h_nr_queued", cfs_rq->h_nr_queued);
	SEQ_printf(m, "  .%-30s: %d\n", "h_nr_idle", cfs_rq->h_nr_idle);
	SEQ_printf(m, "  .%-30s: %ld\n", "load", cfs_rq->ڀريو.وزن);
	SEQ_printf(m, "  .%-30s: %lu\n", "load_avg",
			cfs_rq->avg.ڀريو_avg);
	SEQ_printf(m, "  .%-30s: %lu\n", "runnable_avg",
			cfs_rq->avg.runnable_avg);
	SEQ_printf(m, "  .%-30s: %lu\n", "util_avg",
			cfs_rq->avg.util_avg);
	SEQ_printf(m, "  .%-30s: %u\n", "util_est",
			cfs_rq->avg.util_est);
	SEQ_printf(m, "  .%-30s: %ld\n", "removed.load_avg",
			cfs_rq->removed.ڀريو_avg);
	SEQ_printf(m, "  .%-30s: %ld\n", "removed.util_avg",
			cfs_rq->removed.util_avg);
	SEQ_printf(m, "  .%-30s: %ld\n", "removed.runnable_avg",
			cfs_rq->removed.runnable_avg);
#جيڪڏهن_وصف_ٿيل CONFIG_FAIR_گروهه_SCHED
	SEQ_printf(m, "  .%-30s: %lu\n", "tg_load_avg_contrib",
			cfs_rq->tg_ڀريو_avg_contrib);
	SEQ_printf(m, "  .%-30s: %ld\n", "tg_load_avg",
			atomic_long_read(&cfs_rq->tg->ڀريو_avg));
	SEQ_printf(m, "  .%-30s: %lu\n", "h_load",
			cfs_rq->h_ڀريو);
#جيڪڏهن_پورو /* CONFIG_FAIR_GROUP_SCHED */
#جيڪڏهن_وصف_ٿيل CONFIG_CFS_BANDWIDTH
	SEQ_printf(m, "  .%-30s: %d\n", "throttled",
			cfs_rq->throttled);
	SEQ_printf(m, "  .%-30s: %d\n", "throttle_count",
			cfs_rq->throttle_ڳڻپ);
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل CONFIG_FAIR_گروهه_SCHED
	print_cfs_گروهه_stats(m, cpu, cfs_rq->tg);
#جيڪڏهن_پورو
}

فراغ print_rt_rq(بنية seq_file *m, صحيح cpu, بنية rt_rq *rt_rq)
{
#جيڪڏهن_وصف_ٿيل CONFIG_RT_گروهه_SCHED
	SEQ_printf(m, "\n");
	SEQ_printf_task_گروهه_رستو(m, rt_rq->tg, "rt_rq[%d]:%s\n", cpu);
#وإلا
	SEQ_printf(m, "\n");
	SEQ_printf(m, "rt_rq[%d]:\n", cpu);
#جيڪڏهن_پورو

#وصف_ڪريو P(x) \
	SEQ_printf(m, "  .%-30s: %Ld\n", #x, (ڊگهو ڊگهو)(rt_rq->x))
#وصف_ڪريو PU(x) \
	SEQ_printf(m, "  .%-30s: %lu\n", #x, (بنانشان ڊگهو)(rt_rq->x))
#وصف_ڪريو PN(x) \
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", #x, ورهايو_NS(rt_rq->x))

	PU(rt_nr_running);

#جيڪڏهن_وصف_ٿيل CONFIG_RT_گروهه_SCHED
	P(rt_throttled);
	PN(rt_وقت);
	PN(rt_runtime);
#جيڪڏهن_پورو

#وصف_رد PN
#وصف_رد PU
#وصف_رد P
}

فراغ print_dl_rq(بنية seq_file *m, صحيح cpu, بنية dl_rq *dl_rq)
{
	بنية dl_bw *dl_bw;

	SEQ_printf(m, "\n");
	SEQ_printf(m, "dl_rq[%d]:\n", cpu);

#وصف_ڪريو PU(x) \
	SEQ_printf(m, "  .%-30s: %lu\n", #x, (بنانشان ڊگهو)(dl_rq->x))

	PU(dl_nr_running);
	dl_bw = &cpu_rq(cpu)->rd->dl_bw;
	SEQ_printf(m, "  .%-30s: %lld\n", "dl_bw->bw", dl_bw->bw);
	SEQ_printf(m, "  .%-30s: %lld\n", "dl_bw->total_bw", dl_bw->ڪل_bw);

#وصف_رد PU
}

ساكن فراغ print_cpu(بنية seq_file *m, صحيح cpu)
{
	بنية rq *rq = cpu_rq(cpu);

#جيڪڏهن_وصف_ٿيل CONFIG_X86
	{
		بنانشان صحيح freq = cpu_khz ? : 1;

		SEQ_printf(m, "cpu#%d, %u.%03u MHz\n",
			   cpu, freq / 1000, (freq % 1000));
	}
#وإلا /* !CONFIG_X86: */
	SEQ_printf(m, "cpu#%d\n", cpu);
#جيڪڏهن_پورو /* !CONFIG_X86 */

#وصف_ڪريو P(x)								\
افعل {									\
	إذا (حجم(rq->x) == 4)						\
		SEQ_printf(m, "  .%-30s: %d\n", #x, (صحيح)(rq->x));	\
	وإلا								\
		SEQ_printf(m, "  .%-30s: %Ld\n", #x, (ڊگهو ڊگهو)(rq->x));\
} طالما (0)

#وصف_ڪريو PN(x) \
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", #x, ورهايو_NS(rq->x))

	P(nr_running);
	P(nr_switches);
	P(nr_uninterruptible);
	PN(ايندڙ_balance);
	SEQ_printf(m, "  .%-30s: %ld\n", "curr->pid", (ڊگهو)(task_pid_nr(rq->curr)));
	PN(clock);
	PN(clock_task);
#وصف_رد P
#وصف_رد PN

#وصف_ڪريو P64(n) SEQ_printf(m, "  .%-30s: %Ld\n", #n, rq->n);
	P64(avg_idle);
	P64(max_idle_balance_لاڳت);
#وصف_رد P64

#وصف_ڪريو P(n) SEQ_printf(m, "  .%-30s: %d\n", #n, schedstat_val(rq->n));
	إذا (schedstat_enabled()) {
		P(yld_ڳڻپ);
		P(sched_ڳڻپ);
		P(sched_goidle);
		P(ttwu_ڳڻپ);
		P(ttwu_local);
	}
#وصف_رد P

	print_cfs_stats(m, cpu);
	print_rt_stats(m, cpu);
	print_dl_stats(m, cpu);

	print_rq(m, rq, cpu);
	SEQ_printf(m, "\n");
}

ساكن ثابت حرف *sched_tunable_scaling_نالا[] = {
	"none",
	"logarithmic",
	"linear"
};

ساكن فراغ sched_debug_سرو(بنية seq_file *m)
{
	u64 ktime, sched_clk, cpu_clk;
	بنانشان ڊگهو flags;

	local_irq_save(flags);
	ktime = ktime_منزل_ns(ktime_get());
	sched_clk = sched_clock();
	cpu_clk = local_clock();
	local_irq_restore(flags);

	SEQ_printf(m, "Sched Debug Version: v0.11, %s %.*s\n",
		init_utsname()->release,
		(صحيح)strcspn(init_utsname()->نسخو, " "),
		init_utsname()->نسخو);

#وصف_ڪريو P(x) \
	SEQ_printf(m, "%-40s: %Ld\n", #x, (ڊگهو ڊگهو)(x))
#وصف_ڪريو PN(x) \
	SEQ_printf(m, "%-40s: %Ld.%06ld\n", #x, ورهايو_NS(x))
	PN(ktime);
	PN(sched_clk);
	PN(cpu_clk);
	P(jiffies);
#جيڪڏهن_وصف_ٿيل CONFIG_HAVE_UNSTABLE_SCHED_CLOCK
	P(sched_clock_stable());
#جيڪڏهن_پورو
#وصف_رد PN
#وصف_رد P

	SEQ_printf(m, "\n");
	SEQ_printf(m, "sysctl_sched\n");

#وصف_ڪريو P(x) \
	SEQ_printf(m, "  .%-40s: %Ld\n", #x, (ڊگهو ڊگهو)(x))
#وصف_ڪريو PN(x) \
	SEQ_printf(m, "  .%-40s: %Ld.%06ld\n", #x, ورهايو_NS(x))
	PN(sysctl_sched_بنياد_ٽڪرو);
	P(sysctl_sched_features);
#وصف_رد PN
#وصف_رد P

	SEQ_printf(m, "  .%-40s: %d (%s)\n",
		"sysctl_sched_tunable_scaling",
		sysctl_sched_tunable_scaling,
		sched_tunable_scaling_نالا[sysctl_sched_tunable_scaling]);
	SEQ_printf(m, "\n");
}

ساكن صحيح sched_debug_show(بنية seq_file *m, فراغ *v)
{
	صحيح cpu = (بنانشان ڊگهو)(v - 2);

	إذا (cpu != -1)
		print_cpu(m, cpu);
	وإلا
		sched_debug_سرو(m);

	أرجع 0;
}

فراغ sysrq_sched_debug_show(فراغ)
{
	صحيح cpu;

	sched_debug_سرو(عدم);
	for_each_online_cpu(cpu) {
		/*
		 * Need to reset softlockup watchdogs on all CPUs, because
		 * another CPU might be blocked waiting for us to process
		 * an IPI or stop_machine.
		 */
		touch_nmi_watchdog();
		touch_all_softlockup_watchdogs();
		print_cpu(عدم, cpu);
	}
}

/*
 * This iterator needs some explanation.
 * It returns 1 for the header position.
 * This means 2 is CPU 0.
 * In a hotplugged system some CPUs, including CPU 0, may be missing so we have
 * to use cpumask_* to iterate over the CPUs.
 */
ساكن فراغ *sched_debug_شروعات(بنية seq_file *file, loff_t *وٿي)
{
	بنانشان ڊگهو n = *وٿي;

	إذا (n == 0)
		أرجع (فراغ *) 1;

	n--;

	إذا (n > 0)
		n = cpumask_ايندڙ(n - 1, cpu_online_نقاب);
	وإلا
		n = cpumask_پهريون(cpu_online_نقاب);

	*وٿي = n + 1;

	إذا (n < nr_cpu_ids)
		أرجع (فراغ *)(بنانشان ڊگهو)(n + 2);

	أرجع عدم;
}

ساكن فراغ *sched_debug_ايندڙ(بنية seq_file *file, فراغ *معلومات, loff_t *وٿي)
{
	(*وٿي)++;
	أرجع sched_debug_شروعات(file, وٿي);
}

ساكن فراغ sched_debug_stop(بنية seq_file *file, فراغ *معلومات)
{
}

ساكن ثابت بنية seq_operations sched_debug_sops = {
	.شروعات		= sched_debug_شروعات,
	.ايندڙ		= sched_debug_ايندڙ,
	.stop		= sched_debug_stop,
	.show		= sched_debug_show,
};

#وصف_ڪريو __PS(S, F) SEQ_printf(m, "%-45s:%21Ld\n", S, (ڊگهو ڊگهو)(F))
#وصف_ڪريو __P(F) __PS(#F, F)
#وصف_ڪريو   P(F) __PS(#F, p->F)
#وصف_ڪريو   PM(F, M) __PS(#F, p->F & (M))
#وصف_ڪريو __PSN(S, F) SEQ_printf(m, "%-45s:%14Ld.%06ld\n", S, ورهايو_NS((ڊگهو ڊگهو)(F)))
#وصف_ڪريو __PN(F) __PSN(#F, F)
#وصف_ڪريو   PN(F) __PSN(#F, p->F)


#جيڪڏهن_وصف_ٿيل CONFIG_NUMA_BALANCING
فراغ print_numa_stats(بنية seq_file *m, صحيح ڳنڍ, بنانشان ڊگهو tsf,
		بنانشان ڊگهو tpf, بنانشان ڊگهو gsf, بنانشان ڊگهو gpf)
{
	SEQ_printf(m, "numa_faults node=%d ", ڳنڍ);
	SEQ_printf(m, "task_private=%lu task_shared=%lu ", tpf, tsf);
	SEQ_printf(m, "group_private=%lu group_shared=%lu\n", gpf, gsf);
}
#جيڪڏهن_پورو


ساكن فراغ sched_show_numa(بنية task_struct *p, بنية seq_file *m)
{
#جيڪڏهن_وصف_ٿيل CONFIG_NUMA_BALANCING
	إذا (p->mm)
		P(mm->numa_جاچ_seq);

	P(numa_صفحا_migrated);
	P(numa_preferred_nid);
	P(ڪل_numa_نقص);
	SEQ_printf(m, "current_node=%d, numa_group_id=%d\n",
			task_ڳنڍ(p), task_numa_گروهه_id(p));
	show_numa_stats(p, m);
#جيڪڏهن_پورو /* CONFIG_NUMA_BALANCING */
}

فراغ proc_sched_show_task(بنية task_struct *p, بنية pid_namespace *ns,
						  بنية seq_file *m)
{
	بنانشان ڊگهو nr_switches;

	SEQ_printf(m, "%s (%d, #threads: %d)\n", p->comm, task_pid_nr_ns(p, ns),
						get_nr_ڌاڳا(p));
	SEQ_printf(m,
		"---------------------------------------------------------"
		"----------\n");

#وصف_ڪريو P_SCHEDSTAT(F)  __PS(#F, schedstat_val(p->stats.F))
#وصف_ڪريو PN_SCHEDSTAT(F) __PSN(#F, schedstat_val(p->stats.F))

	PN(se.exec_شروعات);
	PN(se.vruntime);
	PN(se.جوڙ_exec_runtime);

	nr_switches = p->nvcsw + p->nivcsw;

	P(se.nr_migrations);

	إذا (schedstat_enabled()) {
		u64 avg_atom, avg_في_cpu;

		PN_SCHEDSTAT(جوڙ_sleep_runtime);
		PN_SCHEDSTAT(جوڙ_بلاڪ_runtime);
		PN_SCHEDSTAT(wait_شروعات);
		PN_SCHEDSTAT(sleep_شروعات);
		PN_SCHEDSTAT(بلاڪ_شروعات);
		PN_SCHEDSTAT(sleep_max);
		PN_SCHEDSTAT(بلاڪ_max);
		PN_SCHEDSTAT(exec_max);
		PN_SCHEDSTAT(ٽڪرو_max);
		PN_SCHEDSTAT(wait_max);
		PN_SCHEDSTAT(wait_جوڙ);
		P_SCHEDSTAT(wait_ڳڻپ);
		PN_SCHEDSTAT(iowait_جوڙ);
		P_SCHEDSTAT(iowait_ڳڻپ);
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

		avg_atom = p->se.جوڙ_exec_runtime;
		إذا (nr_switches)
			avg_atom = div64_ul(avg_atom, nr_switches);
		وإلا
			avg_atom = -1LL;

		avg_في_cpu = p->se.جوڙ_exec_runtime;
		إذا (p->se.nr_migrations) {
			avg_في_cpu = div64_u64(avg_في_cpu,
						p->se.nr_migrations);
		} وإلا {
			avg_في_cpu = -1LL;
		}

		__PN(avg_atom);
		__PN(avg_في_cpu);

#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CORE
		PN_SCHEDSTAT(core_forceidle_جوڙ);
#جيڪڏهن_پورو
	}

	__P(nr_switches);
	__PS("nr_voluntary_switches", p->nvcsw);
	__PS("nr_involuntary_switches", p->nivcsw);

	P(se.ڀريو.وزن);
	P(se.avg.ڀريو_جوڙ);
	P(se.avg.runnable_جوڙ);
	P(se.avg.util_جوڙ);
	P(se.avg.ڀريو_avg);
	P(se.avg.runnable_avg);
	P(se.avg.util_avg);
	P(se.avg.آخري_update_وقت);
	PM(se.avg.util_est, ~UTIL_AVG_UNCHANGED);
#جيڪڏهن_وصف_ٿيل CONFIG_UCLAMP_TASK
	__PS("uclamp.min", p->uclamp_req[UCLAMP_MIN].قدر);
	__PS("uclamp.max", p->uclamp_req[UCLAMP_MAX].قدر);
	__PS("effective uclamp.min", uclamp_eff_قدر(p, UCLAMP_MIN));
	__PS("effective uclamp.max", uclamp_eff_قدر(p, UCLAMP_MAX));
#جيڪڏهن_پورو /* CONFIG_UCLAMP_TASK */
	P(policy);
	P(prio);
	إذا (task_has_dl_policy(p)) {
		P(dl.runtime);
		P(dl.deadline);
	} وإلا إذا (fair_policy(p->policy)) {
		P(se.ٽڪرو);
	}
#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CLASS_EXT
	__PS("ext.enabled", task_on_scx(p));
#جيڪڏهن_پورو
#وصف_رد PN_SCHEDSTAT
#وصف_رد P_SCHEDSTAT

	{
		بنانشان صحيح this_cpu = raw_smp_processor_id();
		u64 t0, t1;

		t0 = cpu_clock(this_cpu);
		t1 = cpu_clock(this_cpu);
		__PS("clock-delta", t1-t0);
	}

	sched_show_numa(p, m);
}

فراغ proc_sched_سيٽ_task(بنية task_struct *p)
{
#جيڪڏهن_وصف_ٿيل CONFIG_SCHEDSTATS
	memset(&p->stats, 0, حجم(p->stats));
#جيڪڏهن_پورو
}

فراغ resched_latency_warn(صحيح cpu, u64 latency)
{
	ساكن DEFINE_RATELIMIT_STATE(latency_check_ratelimit, 60 * 60 * HZ, 1);

	إذا (likely(!__ratelimit(&latency_check_ratelimit)))
		أرجع;

	pr_err("sched: CPU %d need_resched set for > %llu ns (%d ticks) without schedule\n",
	       cpu, latency, cpu_rq(cpu)->ticks_without_resched);
	dump_ڍير();
}
