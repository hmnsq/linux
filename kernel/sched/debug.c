// SPDX-License-Identifier: GPL-2.0-only
/*
 * kernel/sched/debug.c
 *
 * Print the CFS rbtree and other debugging details
 *
 * Copyright(C) 2007, Red Hat, Inc., Ingo Molnar
 */
#अन्तर्भाव <linux/debugfs.h>
#अन्तर्भाव <linux/nmi.h>
#अन्तर्भाव <linux/log2.h>
#अन्तर्भाव "sched.h"

/*
 * This allows printing both to /sys/kernel/debug/sched/debug and
 * to the console
 */
#परिभाषय SEQ_printf(m, x...)			\
 कुरु {						\
	यदि (m)					\
		seq_printf(m, x);		\
	अन्यथा					\
		pr_cont(x);			\
 } यावत् (0)

/*
 * Ease the printing of nsec fields:
 */
स्थिर दीर्घ दीर्घ nsec_उच्चः(अचिह्नित दीर्घ दीर्घ nsec)
{
	यदि ((दीर्घ दीर्घ)nsec < 0) {
		nsec = -nsec;
		do_div(nsec, 1000000);
		प्रत्यागम -nsec;
	}
	do_div(nsec, 1000000);

	प्रत्यागम nsec;
}

स्थिर अचिह्नित दीर्घ nsec_नीचः(अचिह्नित दीर्घ दीर्घ nsec)
{
	यदि ((दीर्घ दीर्घ)nsec < 0)
		nsec = -nsec;

	प्रत्यागम do_div(nsec, 1000000);
}

#परिभाषय विभज_NS(x) nsec_उच्चः(x), nsec_नीचः(x)

#परिभाषय SCHED_FEAT(name, enabled)	\
	#name ,

स्थिर ध्रुव वर्ण * ध्रुव sched_feat_नामानि[] = {
#अन्तर्भाव "features.h"
};

#अपरिभाषय SCHED_FEAT

स्थिर पूर्णाङ्क sched_feat_show(संरचना seq_file *m, शून्य *v)
{
	पूर्णाङ्क i;

	प्रति (i = 0; i < __SCHED_FEAT_NR; i++) {
		यदि (!(sysctl_sched_features & (1UL << i)))
			seq_puts(m, "NO_");
		seq_printf(m, "%s ", sched_feat_नामानि[i]);
	}
	seq_puts(m, "\n");

	प्रत्यागम 0;
}

#यदि_परिभाषितम् CONFIG_JUMP_LABEL

#परिभाषय jump_label_कुञ्चिका_true  { .कुञ्चिका_true  = STATIC_कुञ्चिका_TRUE_INIT  }
#परिभाषय jump_label_कुञ्चिका_false { .कुञ्चिका_false = STATIC_कुञ्चिका_FALSE_INIT }

#परिभाषय SCHED_FEAT(name, enabled)	\
	jump_label_कुञ्चिका##enabled ,

सङ्घ sched_feat_कुञ्चिका sched_feat_कुञ्चिकाः[__SCHED_FEAT_NR] = {
#अन्तर्भाव "features.h"
};

#अपरिभाषय SCHED_FEAT

स्थिर शून्य sched_feat_disable(पूर्णाङ्क i)
{
	static_branch_disable_cpuslocked(&sched_feat_कुञ्चिकाः[i].कुञ्चिका_true);
}

स्थिर शून्य sched_feat_enable(पूर्णाङ्क i)
{
	static_branch_enable_cpuslocked(&sched_feat_कुञ्चिकाः[i].कुञ्चिका_false);
}
#अन्यथा /* !CONFIG_JUMP_LABEL: */
स्थिर शून्य sched_feat_disable(पूर्णाङ्क i) { };
स्थिर शून्य sched_feat_enable(पूर्णाङ्क i) { };
#यदि_अन्तः /* !CONFIG_JUMP_LABEL */

स्थिर पूर्णाङ्क sched_feat_समुच्चयः(वर्ण *cmp)
{
	पूर्णाङ्क i;
	पूर्णाङ्क neg = 0;

	यदि (वाक्ये_तुलय(cmp, "NO_", 3) == 0) {
		neg = 1;
		cmp += 3;
	}

	i = मेलनम्_string(sched_feat_नामानि, __SCHED_FEAT_NR, cmp);
	यदि (i < 0)
		प्रत्यागम i;

	यदि (neg) {
		sysctl_sched_features &= ~(1UL << i);
		sched_feat_disable(i);
	} अन्यथा {
		sysctl_sched_features |= (1UL << i);
		sched_feat_enable(i);
	}

	प्रत्यागम 0;
}

स्थिर ssize_t
sched_feat_write(संरचना file *filp, ध्रुव वर्ण __user *ubuf,
		आकार_t cnt, loff_t *ppos)
{
	वर्ण buf[64];
	वर्ण *cmp;
	पूर्णाङ्क ret;
	संरचना inode *inode;

	यदि (cnt > 63)
		cnt = 63;

	यदि (copy_from_user(buf, ubuf, cnt))
		प्रत्यागम -EFAULT;

	buf[cnt] = 0;
	cmp = strstrip(buf);

	/* Ensure the static_key remains in a consistent state */
	inode = file_inode(filp);
	cpus_read_तालकम्();
	inode_तालकम्(inode);
	ret = sched_feat_समुच्चयः(cmp);
	inode_unlock(inode);
	cpus_read_unlock();
	यदि (ret < 0)
		प्रत्यागम ret;

	*ppos += cnt;

	प्रत्यागम cnt;
}

स्थिर पूर्णाङ्क sched_feat_open(संरचना inode *inode, संरचना file *filp)
{
	प्रत्यागम single_open(filp, sched_feat_show, रिक्त);
}

स्थिर ध्रुव संरचना file_operations sched_feat_fops = {
	.उद्घाटय		= sched_feat_open,
	.लिख		= sched_feat_write,
	.पठ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

स्थिर ssize_t sched_scaling_write(संरचना file *filp, ध्रुव वर्ण __user *ubuf,
				   आकार_t cnt, loff_t *ppos)
{
	अचिह्नित पूर्णाङ्क scaling;
	पूर्णाङ्क ret;

	ret = kstrtouint_from_user(ubuf, cnt, 10, &scaling);
	यदि (ret)
		प्रत्यागम ret;

	यदि (scaling >= SCHED_TUNABLESCALING_अन्तः)
		प्रत्यागम -EINVAL;

	sysctl_sched_tunable_scaling = scaling;
	यदि (sched_update_scaling())
		प्रत्यागम -EINVAL;

	*ppos += cnt;
	प्रत्यागम cnt;
}

स्थिर पूर्णाङ्क sched_scaling_show(संरचना seq_file *m, शून्य *v)
{
	seq_printf(m, "%d\n", sysctl_sched_tunable_scaling);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क sched_scaling_open(संरचना inode *inode, संरचना file *filp)
{
	प्रत्यागम single_open(filp, sched_scaling_show, रिक्त);
}

स्थिर ध्रुव संरचना file_operations sched_scaling_fops = {
	.उद्घाटय		= sched_scaling_open,
	.लिख		= sched_scaling_write,
	.पठ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#यदि_परिभाषितम् CONFIG_SCHED_CACHE
स्थिर ssize_t
sched_cache_enable_write(संरचना file *filp, ध्रुव वर्ण __user *ubuf,
			 आकार_t cnt, loff_t *ppos)
{
	bool val;
	पूर्णाङ्क ret;

	ret = kstrtobool_from_user(ubuf, cnt, &val);
	यदि (ret)
		प्रत्यागम ret;

	sysctl_sched_cache_user = val;

	sched_cache_सक्रिय_समुच्चयः();

	*ppos += cnt;

	प्रत्यागम cnt;
}

स्थिर पूर्णाङ्क sched_cache_enable_show(संरचना seq_file *m, शून्य *v)
{
	seq_printf(m, "%d\n", sysctl_sched_cache_user);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क sched_cache_enable_open(संरचना inode *inode,
				   संरचना file *filp)
{
	प्रत्यागम single_open(filp, sched_cache_enable_show, रिक्त);
}

स्थिर ध्रुव संरचना file_operations sched_cache_enable_fops = {
	.उद्घाटय           = sched_cache_enable_open,
	.लिख          = sched_cache_enable_write,
	.पठ           = seq_read,
	.llseek         = seq_lseek,
	.release        = single_release,
};
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_PREEMPT_DYNAMIC

स्थिर ssize_t sched_dynamic_write(संरचना file *filp, ध्रुव वर्ण __user *ubuf,
				   आकार_t cnt, loff_t *ppos)
{
	वर्ण buf[16];
	पूर्णाङ्क बहुलकम्;

	यदि (cnt > 15)
		cnt = 15;

	यदि (copy_from_user(buf, ubuf, cnt))
		प्रत्यागम -EFAULT;

	buf[cnt] = 0;
	बहुलकम् = sched_dynamic_बहुलकम्(strstrip(buf));
	यदि (बहुलकम् < 0)
		प्रत्यागम बहुलकम्;

	sched_dynamic_update(बहुलकम्);

	*ppos += cnt;

	प्रत्यागम cnt;
}

स्थिर पूर्णाङ्क sched_dynamic_show(संरचना seq_file *m, शून्य *v)
{
	पूर्णाङ्क बहुलकम् = READ_ONCE(preempt_dynamic_बहुलकम्);

	/* Stop at NULL terminator */
	प्रति (पूर्णाङ्क i = 0; preempt_modes[i]; i++) {
		यदि (बहुलकम् == i)
			seq_puts(m, "(");
		seq_puts(m, preempt_modes[i]);
		यदि (बहुलकम् == i)
			seq_puts(m, ")");

		seq_puts(m, " ");
	}

	seq_puts(m, "\n");
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क sched_dynamic_open(संरचना inode *inode, संरचना file *filp)
{
	प्रत्यागम single_open(filp, sched_dynamic_show, रिक्त);
}

स्थिर ध्रुव संरचना file_operations sched_dynamic_fops = {
	.उद्घाटय		= sched_dynamic_open,
	.लिख		= sched_dynamic_write,
	.पठ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#यदि_अन्तः /* CONFIG_PREEMPT_DYNAMIC */

__read_mostly bool sched_debug_verbose;

स्थिर संरचना dentry           *sd_dentry;


स्थिर ssize_t sched_verbose_write(संरचना file *filp, ध्रुव वर्ण __user *ubuf,
				  आकार_t cnt, loff_t *ppos)
{
	ssize_t परिणाम;
	bool orig;

	cpus_read_तालकम्();
	sched_domains_mutex_तालकम्();

	orig = sched_debug_verbose;
	परिणाम = debugfs_write_file_bool(filp, ubuf, cnt, ppos);

	यदि (sched_debug_verbose && !orig)
		update_sched_domain_debugfs();
	अन्यथा यदि (!sched_debug_verbose && orig) {
		debugfs_remove(sd_dentry);
		sd_dentry = रिक्त;
	}

	sched_domains_mutex_unlock();
	cpus_read_unlock();

	प्रत्यागम परिणाम;
}

स्थिर ध्रुव संरचना file_operations sched_verbose_fops = {
	.पठ =         debugfs_read_file_bool,
	.लिख =        sched_verbose_write,
	.उद्घाटय =         simple_open,
	.llseek =       default_llseek,
};

स्थिर ध्रुव संरचना seq_operations sched_debug_sops;

स्थिर पूर्णाङ्क sched_debug_open(संरचना inode *inode, संरचना file *filp)
{
	प्रत्यागम seq_open(filp, &sched_debug_sops);
}

स्थिर ध्रुव संरचना file_operations sched_debug_fops = {
	.उद्घाटय		= sched_debug_open,
	.पठ		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

गणना dl_param {
	DL_RUNTIME = 0,
	DL_PERIOD,
};

स्थिर अचिह्नित दीर्घ dl_server_period_max = (1UL << 22) * NSEC_प्रत्येकम्_USEC; /* ~4 seconds */
स्थिर अचिह्नित दीर्घ dl_server_period_min = (100) * NSEC_प्रत्येकम्_USEC;     /* 100 us */

स्थिर ssize_t sched_server_write_common(संरचना file *filp, ध्रुव वर्ण __user *ubuf,
					 आकार_t cnt, loff_t *ppos, गणना dl_param param,
					 शून्य *server)
{
	दीर्घ cpu = (दीर्घ) ((संरचना seq_file *) filp->वैयक्तिकम्_दत्त)->गुप्त;
	संरचना sched_dl_entity *dl_se = (संरचना sched_dl_entity *)server;
	u64 old_runtime, runtime, period;
	संरचना rq *rq = cpu_rq(cpu);
	पूर्णाङ्क retval = 0;
	आकार_t err;
	u64 मानम्;

	err = kstrtoull_from_user(ubuf, cnt, 10, &मानम्);
	यदि (err)
		प्रत्यागम err;

	scoped_guard (rq_तालकम्_irqsave, rq) {
		old_runtime = runtime = dl_se->dl_runtime;
		period = dl_se->dl_period;

		विकल्प (param) {
		पक्ष DL_RUNTIME:
			यदि (runtime == मानम्)
				भङ्ग;
			runtime = मानम्;
			भङ्ग;
		पक्ष DL_PERIOD:
			यदि (मानम् == period)
				भङ्ग;
			period = मानम्;
			भङ्ग;
		}

		यदि (runtime > period ||
		    period > dl_server_period_max ||
		    period < dl_server_period_min) {
			प्रत्यागम  -EINVAL;
		}

		यदि (!cpu_online(cpu_सम्बन्धि(rq)))
			प्रत्यागम -EBUSY;

		update_rq_clock(rq);
		dl_server_stop(dl_se);
		retval = dl_server_apply_params(dl_se, runtime, period, 0);
		dl_server_start(dl_se);

		यदि (retval < 0)
			प्रत्यागम retval;
	}

	यदि (!!old_runtime ^ !!runtime) {
		pr_info("%s server %sabled on CPU %d%s.\n",
			server == &rq->fair_server ? "Fair" : "Ext",
			runtime ? "en" : "dis",
			cpu_सम्बन्धि(rq),
			runtime ? "" : ", system may malfunction due to starvation");
	}

	*ppos += cnt;
	प्रत्यागम cnt;
}

स्थिर आकार_t sched_server_show_common(संरचना seq_file *m, शून्य *v, गणना dl_param param,
				       शून्य *server)
{
	संरचना sched_dl_entity *dl_se = (संरचना sched_dl_entity *)server;
	u64 मानम्;

	विकल्प (param) {
	पक्ष DL_RUNTIME:
		मानम् = dl_se->dl_runtime;
		भङ्ग;
	पक्ष DL_PERIOD:
		मानम् = dl_se->dl_period;
		भङ्ग;
	}

	seq_printf(m, "%llu\n", मानम्);
	प्रत्यागम 0;
}

स्थिर ssize_t
sched_fair_server_runtime_write(संरचना file *filp, ध्रुव वर्ण __user *ubuf,
				आकार_t cnt, loff_t *ppos)
{
	दीर्घ cpu = (दीर्घ) ((संरचना seq_file *) filp->वैयक्तिकम्_दत्त)->गुप्त;
	संरचना rq *rq = cpu_rq(cpu);

	प्रत्यागम sched_server_write_common(filp, ubuf, cnt, ppos, DL_RUNTIME,
					&rq->fair_server);
}

स्थिर पूर्णाङ्क sched_fair_server_runtime_show(संरचना seq_file *m, शून्य *v)
{
	अचिह्नित दीर्घ cpu = (अचिह्नित दीर्घ) m->गुप्त;
	संरचना rq *rq = cpu_rq(cpu);

	प्रत्यागम sched_server_show_common(m, v, DL_RUNTIME, &rq->fair_server);
}

स्थिर पूर्णाङ्क sched_fair_server_runtime_open(संरचना inode *inode, संरचना file *filp)
{
	प्रत्यागम single_open(filp, sched_fair_server_runtime_show, inode->i_वैयक्तिकम्);
}

स्थिर ध्रुव संरचना file_operations fair_server_runtime_fops = {
	.उद्घाटय		= sched_fair_server_runtime_open,
	.लिख		= sched_fair_server_runtime_write,
	.पठ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

स्थिर संरचना dentry *debugfs_sched;

#यदि_परिभाषितम् CONFIG_SCHED_CLASS_EXT
स्थिर ssize_t
sched_ext_server_runtime_write(संरचना file *filp, ध्रुव वर्ण __user *ubuf,
			       आकार_t cnt, loff_t *ppos)
{
	दीर्घ cpu = (दीर्घ) ((संरचना seq_file *) filp->वैयक्तिकम्_दत्त)->गुप्त;
	संरचना rq *rq = cpu_rq(cpu);

	प्रत्यागम sched_server_write_common(filp, ubuf, cnt, ppos, DL_RUNTIME,
					&rq->ext_server);
}

स्थिर पूर्णाङ्क sched_ext_server_runtime_show(संरचना seq_file *m, शून्य *v)
{
	अचिह्नित दीर्घ cpu = (अचिह्नित दीर्घ) m->गुप्त;
	संरचना rq *rq = cpu_rq(cpu);

	प्रत्यागम sched_server_show_common(m, v, DL_RUNTIME, &rq->ext_server);
}

स्थिर पूर्णाङ्क sched_ext_server_runtime_open(संरचना inode *inode, संरचना file *filp)
{
	प्रत्यागम single_open(filp, sched_ext_server_runtime_show, inode->i_वैयक्तिकम्);
}

स्थिर ध्रुव संरचना file_operations ext_server_runtime_fops = {
	.उद्घाटय		= sched_ext_server_runtime_open,
	.लिख		= sched_ext_server_runtime_write,
	.पठ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

स्थिर ssize_t
sched_ext_server_period_write(संरचना file *filp, ध्रुव वर्ण __user *ubuf,
			      आकार_t cnt, loff_t *ppos)
{
	दीर्घ cpu = (दीर्घ) ((संरचना seq_file *) filp->वैयक्तिकम्_दत्त)->गुप्त;
	संरचना rq *rq = cpu_rq(cpu);

	प्रत्यागम sched_server_write_common(filp, ubuf, cnt, ppos, DL_PERIOD,
					&rq->ext_server);
}

स्थिर पूर्णाङ्क sched_ext_server_period_show(संरचना seq_file *m, शून्य *v)
{
	अचिह्नित दीर्घ cpu = (अचिह्नित दीर्घ) m->गुप्त;
	संरचना rq *rq = cpu_rq(cpu);

	प्रत्यागम sched_server_show_common(m, v, DL_PERIOD, &rq->ext_server);
}

स्थिर पूर्णाङ्क sched_ext_server_period_open(संरचना inode *inode, संरचना file *filp)
{
	प्रत्यागम single_open(filp, sched_ext_server_period_show, inode->i_वैयक्तिकम्);
}

स्थिर ध्रुव संरचना file_operations ext_server_period_fops = {
	.उद्घाटय		= sched_ext_server_period_open,
	.लिख		= sched_ext_server_period_write,
	.पठ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

स्थिर शून्य debugfs_ext_server_init(शून्य)
{
	संरचना dentry *d_ext;
	अचिह्नित दीर्घ cpu;

	d_ext = debugfs_create_dir("ext_server", debugfs_sched);
	यदि (!d_ext)
		प्रत्यागम;

	for_each_possible_cpu(cpu) {
		संरचना dentry *d_cpu;
		वर्ण buf[32];

		snprintf(buf, परिमाण(buf), "cpu%lu", cpu);
		d_cpu = debugfs_create_dir(buf, d_ext);

		debugfs_create_file("runtime", 0644, d_cpu, (शून्य *) cpu, &ext_server_runtime_fops);
		debugfs_create_file("period", 0644, d_cpu, (शून्य *) cpu, &ext_server_period_fops);
	}
}
#यदि_अन्तः /* CONFIG_SCHED_CLASS_EXT */

स्थिर ssize_t
sched_fair_server_period_write(संरचना file *filp, ध्रुव वर्ण __user *ubuf,
			       आकार_t cnt, loff_t *ppos)
{
	दीर्घ cpu = (दीर्घ) ((संरचना seq_file *) filp->वैयक्तिकम्_दत्त)->गुप्त;
	संरचना rq *rq = cpu_rq(cpu);

	प्रत्यागम sched_server_write_common(filp, ubuf, cnt, ppos, DL_PERIOD,
					&rq->fair_server);
}

स्थिर पूर्णाङ्क sched_fair_server_period_show(संरचना seq_file *m, शून्य *v)
{
	अचिह्नित दीर्घ cpu = (अचिह्नित दीर्घ) m->गुप्त;
	संरचना rq *rq = cpu_rq(cpu);

	प्रत्यागम sched_server_show_common(m, v, DL_PERIOD, &rq->fair_server);
}

स्थिर पूर्णाङ्क sched_fair_server_period_open(संरचना inode *inode, संरचना file *filp)
{
	प्रत्यागम single_open(filp, sched_fair_server_period_show, inode->i_वैयक्तिकम्);
}

स्थिर ध्रुव संरचना file_operations fair_server_period_fops = {
	.उद्घाटय		= sched_fair_server_period_open,
	.लिख		= sched_fair_server_period_write,
	.पठ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

स्थिर शून्य debugfs_fair_server_init(शून्य)
{
	संरचना dentry *d_fair;
	अचिह्नित दीर्घ cpu;

	d_fair = debugfs_create_dir("fair_server", debugfs_sched);
	यदि (!d_fair)
		प्रत्यागम;

	for_each_possible_cpu(cpu) {
		संरचना dentry *d_cpu;
		वर्ण buf[32];

		snprintf(buf, परिमाण(buf), "cpu%lu", cpu);
		d_cpu = debugfs_create_dir(buf, d_fair);

		debugfs_create_file("runtime", 0644, d_cpu, (शून्य *) cpu, &fair_server_runtime_fops);
		debugfs_create_file("period", 0644, d_cpu, (शून्य *) cpu, &fair_server_period_fops);
	}
}

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
स्थिर पूर्णाङ्क cgroup_बहुलकम् = 2;

/* See __sched_cgroup_mode_update(). */
स्थिर ध्रुव वर्ण *cgroup_बहुलकम्_str[] = {
	"up",
	"smp",
	"concur",
	"max",
	"tasks",
};

स्थिर पूर्णाङ्क sched_cgroup_बहुलकम्(ध्रुव वर्ण *str)
{
	प्रति (पूर्णाङ्क i = 0; i < ARRAY_आकार(cgroup_बहुलकम्_str); i++) {
		यदि (!वाक्यं_तुलय(str, cgroup_बहुलकम्_str[i]))
			प्रत्यागम i;
	}
	प्रत्यागम -EINVAL;
}

स्थिर ssize_t sched_cgroup_write(संरचना file *filp, ध्रुव वर्ण __user *ubuf,
				   आकार_t cnt, loff_t *ppos)
{
	वर्ण buf[16];
	पूर्णाङ्क बहुलकम्;

	यदि (cnt > 15)
		cnt = 15;

	यदि (copy_from_user(buf, ubuf, cnt))
		प्रत्यागम -EFAULT;

	buf[cnt] = 0;
	बहुलकम् = sched_cgroup_बहुलकम्(strstrip(buf));
	यदि (बहुलकम् < 0)
		प्रत्यागम बहुलकम्;

	__sched_cgroup_mode_update(बहुलकम्);
	WRITE_ONCE(cgroup_बहुलकम्, बहुलकम्);

	*ppos += cnt;
	प्रत्यागम cnt;
}

स्थिर पूर्णाङ्क sched_cgroup_show(संरचना seq_file *m, शून्य *v)
{
	पूर्णाङ्क बहुलकम् = READ_ONCE(cgroup_बहुलकम्);

	प्रति (पूर्णाङ्क i = 0; i < ARRAY_आकार(cgroup_बहुलकम्_str); i++) {
		यदि (बहुलकम् == i)
			seq_puts(m, "(");
		seq_puts(m, cgroup_बहुलकम्_str[i]);
		यदि (बहुलकम् == i)
			seq_puts(m, ")");

		seq_puts(m, " ");
	}
	seq_puts(m, "\n");
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क sched_cgroup_open(संरचना inode *inode, संरचना file *filp)
{
	प्रत्यागम single_open(filp, sched_cgroup_show, रिक्त);
}

स्थिर ध्रुव संरचना file_operations sched_cgroup_fops = {
	.उद्घाटय		= sched_cgroup_open,
	.लिख		= sched_cgroup_write,
	.पठ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};
#यदि_अन्तः

स्थिर __init पूर्णाङ्क sched_init_debug(शून्य)
{
	संरचना dentry __maybe_unused *numa, *llc;

	debugfs_sched = debugfs_create_dir("sched", रिक्त);

	debugfs_create_file("features", 0644, debugfs_sched, रिक्त, &sched_feat_fops);
	debugfs_create_file_unsafe("verbose", 0644, debugfs_sched, &sched_debug_verbose, &sched_verbose_fops);
#यदि_परिभाषितम् CONFIG_PREEMPT_DYNAMIC
	debugfs_create_file("preempt", 0644, debugfs_sched, रिक्त, &sched_dynamic_fops);
#यदि_अन्तः

	debugfs_create_u32("base_slice_ns", 0644, debugfs_sched, &sysctl_sched_आधारः_खण्डिका);

	debugfs_create_u32("latency_warn_ms", 0644, debugfs_sched, &sysctl_resched_latency_warn_ms);
	debugfs_create_u32("latency_warn_once", 0644, debugfs_sched, &sysctl_resched_latency_warn_once);

	debugfs_create_file("tunable_scaling", 0644, debugfs_sched, रिक्त, &sched_scaling_fops);
	debugfs_create_u32("migration_cost_ns", 0644, debugfs_sched, &sysctl_sched_migration_cost);
	debugfs_create_u32("nr_migrate", 0644, debugfs_sched, &sysctl_sched_nr_migrate);

	sched_domains_mutex_तालकम्();
	update_sched_domain_debugfs();
	sched_domains_mutex_unlock();

#यदि_परिभाषितम् CONFIG_NUMA_BALANCING
	numa = debugfs_create_dir("numa_balancing", debugfs_sched);

	debugfs_create_u32("scan_delay_ms", 0644, numa, &sysctl_numa_balancing_अवलोकनम्_delay);
	debugfs_create_u32("scan_period_min_ms", 0644, numa, &sysctl_numa_balancing_अवलोकनम्_period_min);
	debugfs_create_u32("scan_period_max_ms", 0644, numa, &sysctl_numa_balancing_अवलोकनम्_period_max);
	debugfs_create_u32("scan_size_mb", 0644, numa, &sysctl_numa_balancing_अवलोकनम्_आकार);
	debugfs_create_u32("hot_threshold_ms", 0644, numa, &sysctl_numa_balancing_hot_देहली);
#यदि_अन्तः /* CONFIG_NUMA_BALANCING */

#यदि_परिभाषितम् CONFIG_SCHED_CACHE
	llc = debugfs_create_dir("llc_balancing", debugfs_sched);
	debugfs_create_file("enabled", 0644, llc, रिक्त,
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
#यदि_अन्तः

	debugfs_create_file("debug", 0444, debugfs_sched, रिक्त, &sched_debug_fops);

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
	debugfs_create_file("cgroup_mode", 0644, debugfs_sched, रिक्त, &sched_cgroup_fops);
#यदि_अन्तः

	debugfs_fair_server_init();
#यदि_परिभाषितम् CONFIG_SCHED_CLASS_EXT
	debugfs_ext_server_init();
#यदि_अन्तः

	प्रत्यागम 0;
}
late_initcall(sched_init_debug);

स्थिर cpumask_var_t		sd_sysctl_cpus;

स्थिर पूर्णाङ्क sd_flags_show(संरचना seq_file *m, शून्य *v)
{
	अचिह्नित दीर्घ flags = *(अचिह्नित पूर्णाङ्क *)m->गुप्त;
	पूर्णाङ्क idx;

	for_each_समुच्चयः_bit(idx, &flags, __SD_FLAG_CNT) {
		seq_puts(m, sd_flag_debug[idx].name);
		seq_puts(m, " ");
	}
	seq_puts(m, "\n");

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क sd_flags_open(संरचना inode *inode, संरचना file *file)
{
	प्रत्यागम single_open(file, sd_flags_show, inode->i_वैयक्तिकम्);
}

स्थिर ध्रुव संरचना file_operations sd_flags_fops = {
	.उद्घाटय		= sd_flags_open,
	.पठ		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

स्थिर शून्य पञ्जिकाकोशः_sd(संरचना sched_domain *sd, संरचना dentry *parent)
{
#परिभाषय SDM(type, बहुलकम्, member)	\
	debugfs_create_##type(#member, बहुलकम्, parent, &sd->member)

	SDM(ulong, 0644, min_interval);
	SDM(ulong, 0644, max_interval);
	SDM(u64,   0644, max_newidle_lb_cost);
	SDM(u32,   0644, busy_गुणकः);
	SDM(u32,   0644, imbalance_pct);
	SDM(u32,   0644, cache_nice_tries);
	SDM(str,   0444, name);

#अपरिभाषय SDM

	debugfs_create_file("flags", 0444, parent, &sd->flags, &sd_flags_fops);
	debugfs_create_file("groups_flags", 0444, parent, &sd->groups->flags, &sd_flags_fops);
	debugfs_create_u32("level", 0444, parent, (u32 *)&sd->level);

	यदि (sd->flags & SD_ASYM_PACKING)
		debugfs_create_u32("group_asym_prefer_cpu", 0444, parent,
				   (u32 *)&sd->groups->asym_prefer_cpu);
}

शून्य update_sched_domain_debugfs(शून्य)
{
	पूर्णाङ्क cpu, i;

	/*
	 * This can unfortunately be invoked before sched_debug_init() creates
	 * the debug directory. Don't touch sd_sysctl_cpus until then.
	 */
	यदि (!debugfs_sched)
		प्रत्यागम;

	यदि (!sched_debug_verbose)
		प्रत्यागम;

	यदि (!cpumask_उपलब्धम्(sd_sysctl_cpus)) {
		यदि (!alloc_cpumask_var(&sd_sysctl_cpus, GFP_KERNEL))
			प्रत्यागम;
		cpumask_copy(sd_sysctl_cpus, cpu_possible_आवरणम्);
	}

	यदि (!sd_dentry) {
		sd_dentry = debugfs_create_dir("domains", debugfs_sched);

		/* rebuild sd_sysctl_cpus if empty since it gets cleared below */
		यदि (cpumask_रिक्तानि(sd_sysctl_cpus))
			cpumask_copy(sd_sysctl_cpus, cpu_online_आवरणम्);
	}

	for_each_cpu(cpu, sd_sysctl_cpus) {
		संरचना sched_domain *sd;
		संरचना dentry *d_cpu;
		वर्ण buf[32];

		snprintf(buf, परिमाण(buf), "cpu%d", cpu);
		debugfs_lookup_and_remove(buf, sd_dentry);
		d_cpu = debugfs_create_dir(buf, sd_dentry);

		i = 0;
		for_each_domain(cpu, sd) {
			संरचना dentry *d_sd;

			snprintf(buf, परिमाण(buf), "domain%d", i);
			d_sd = debugfs_create_dir(buf, d_cpu);

			पञ्जिकाकोशः_sd(sd, d_sd);
			i++;
		}

		__cpumask_clear_cpu(cpu, sd_sysctl_cpus);
	}
}

शून्य dirty_sched_domain_sysctl(पूर्णाङ्क cpu)
{
	यदि (cpumask_उपलब्धम्(sd_sysctl_cpus))
		__cpumask_set_cpu(cpu, sd_sysctl_cpus);
}

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
स्थिर शून्य print_cfs_गणः_stats(संरचना seq_file *m, पूर्णाङ्क cpu, संरचना task_गणः *tg)
{
	संरचना sched_entity *se = tg_se(tg, cpu);

#परिभाषय P(F)		SEQ_printf(m, "  .%-30s: %lld\n",	#F, (दीर्घ दीर्घ)F)
#परिभाषय P_SCHEDSTAT(F)	SEQ_printf(m, "  .%-30s: %lld\n",	\
		#F, (दीर्घ दीर्घ)schedstat_val(stats->F))
#परिभाषय PN(F)		SEQ_printf(m, "  .%-30s: %lld.%06ld\n", #F, विभज_NS((दीर्घ दीर्घ)F))
#परिभाषय PN_SCHEDSTAT(F)	SEQ_printf(m, "  .%-30s: %lld.%06ld\n", \
		#F, विभज_NS((दीर्घ दीर्घ)schedstat_val(stats->F)))

	यदि (!se)
		प्रत्यागम;

	PN(se->exec_start);
	PN(se->vruntime);
	PN(se->योग_exec_runtime);

	यदि (schedstat_enabled()) {
		संरचना sched_statistics *stats;
		stats = __schedstats_from_se(se);

		PN_SCHEDSTAT(wait_start);
		PN_SCHEDSTAT(sleep_start);
		PN_SCHEDSTAT(खण्डम्_start);
		PN_SCHEDSTAT(sleep_max);
		PN_SCHEDSTAT(खण्डम्_max);
		PN_SCHEDSTAT(exec_max);
		PN_SCHEDSTAT(खण्डिका_max);
		PN_SCHEDSTAT(wait_max);
		PN_SCHEDSTAT(wait_योग);
		P_SCHEDSTAT(wait_गणनम्);
	}

	P(se->आरोपय.weight);
	P(se->avg.आरोपय_avg);
	P(se->avg.util_avg);
	P(se->avg.runnable_avg);

#अपरिभाषय PN_SCHEDSTAT
#अपरिभाषय PN
#अपरिभाषय P_SCHEDSTAT
#अपरिभाषय P
}
#यदि_अन्तः /* CONFIG_FAIR_GROUP_SCHED */

#यदि_परिभाषितम् CONFIG_CGROUP_SCHED
स्थिर DEFINE_SPINLOCK(sched_debug_तालकम्);
स्थिर वर्ण गणः_मार्गः[मार्गः_MAX];

स्थिर शून्य task_गणः_मार्गः(संरचना task_गणः *tg, वर्ण *मार्गः, पूर्णाङ्क plen)
{
	यदि (autogroup_मार्गः(tg, मार्गः, plen))
		प्रत्यागम;

	cgroup_मार्गः(tg->css.cgroup, मार्गः, plen);
}

/*
 * Only 1 SEQ_printf_task_group_path() caller can use the full length
 * group_path[] for cgroup path. Other simultaneous callers will have
 * to use a shorter stack buffer. A "..." suffix is appended at the end
 * of the stack buffer so that it will show up in case the output length
 * matches the given buffer size to indicate possible path name truncation.
 */
#परिभाषय SEQ_printf_task_गणः_मार्गः(m, tg, fmt...)			\
{									\
	यदि (spin_trylock(&sched_debug_तालकम्)) {				\
		task_गणः_मार्गः(tg, गणः_मार्गः, परिमाण(गणः_मार्गः));	\
		SEQ_printf(m, fmt, गणः_मार्गः);				\
		spin_unlock(&sched_debug_तालकम्);				\
	} अन्यथा {							\
		वर्ण buf[128];						\
		वर्ण *bufend = buf + परिमाण(buf) - 3;			\
		task_गणः_मार्गः(tg, buf, bufend - buf);			\
		वाक्यं_प्रतिलिख(bufend - 1, "...");				\
		SEQ_printf(m, fmt, buf);				\
	}								\
}
#यदि_अन्तः

स्थिर शून्य
print_task(संरचना seq_file *m, संरचना rq *rq, संरचना task_struct *p)
{
	यदि (task_current(rq, p))
		SEQ_printf(m, ">R");
	अन्यथा
		SEQ_printf(m, " %c", task_state_गन्तव्यम्_char(p));

	SEQ_printf(m, " %15s %5d %10ld %9Ld.%06ld   %c   %9Ld.%06ld %c %9Ld.%06ld %9Ld.%06ld %9Ld   %5d ",
		p->comm, task_pid_nr(p),
		p->se.h_आरोपय.weight,
		विभज_NS(p->se.vruntime),
		entity_eligible(&rq->cfs, &p->se) ? 'E' : 'N',
		विभज_NS(p->se.deadline),
		p->se.custom_खण्डिका ? 'S' : ' ',
		विभज_NS(p->se.खण्डिका),
		विभज_NS(p->se.योग_exec_runtime),
		(दीर्घ दीर्घ)(p->nvcsw + p->nivcsw),
		p->prio);

	SEQ_printf(m, "%9lld.%06ld %9lld.%06ld %9lld.%06ld",
		विभज_NS(schedstat_val_or_zero(p->stats.wait_योग)),
		विभज_NS(schedstat_val_or_zero(p->stats.योग_sleep_runtime)),
		विभज_NS(schedstat_val_or_zero(p->stats.योग_खण्डम्_runtime)));

#यदि_परिभाषितम् CONFIG_NUMA_BALANCING
	SEQ_printf(m, "   %d      %d", task_ग्रन्थिः(p), task_numa_गणः_id(p));
#यदि_अन्तः
#यदि_परिभाषितम् CONFIG_CGROUP_SCHED
	SEQ_printf_task_गणः_मार्गः(m, task_गणः(p), "        %s")
#यदि_अन्तः

	SEQ_printf(m, "\n");
}

स्थिर शून्य print_rq(संरचना seq_file *m, संरचना rq *rq, पूर्णाङ्क rq_cpu)
{
	संरचना task_struct *g, *p;

	SEQ_printf(m, "\n");
	SEQ_printf(m, "runnable tasks:\n");
	SEQ_printf(m, " S            task   PID     weight       vruntime   eligible    "
		   "deadline             slice          sum-exec      switches  "
		   "prio         wait-time        sum-sleep       sum-block"
#यदि_परिभाषितम् CONFIG_NUMA_BALANCING
		   "  node   group-id"
#यदि_अन्तः
#यदि_परिभाषितम् CONFIG_CGROUP_SCHED
		   "  group-path"
#यदि_अन्तः
		   "\n");
	SEQ_printf(m, "-------------------------------------------------------"
		   "------------------------------------------------------"
		   "------------------------------------------------------"
#यदि_परिभाषितम् CONFIG_NUMA_BALANCING
		   "--------------"
#यदि_अन्तः
#यदि_परिभाषितम् CONFIG_CGROUP_SCHED
		   "--------------"
#यदि_अन्तः
		   "\n");

	rcu_read_तालकम्();
	for_each_process_thread(g, p) {
		यदि (task_cpu(p) != rq_cpu)
			अनुवर्त;

		print_task(m, rq, p);
	}
	rcu_read_unlock();
}

शून्य print_cfs_rq(संरचना seq_file *m, पूर्णाङ्क cpu, संरचना cfs_rq *cfs_rq)
{
	s64 वामः_vruntime = -1, दक्षिणः_vruntime = -1, वामः_deadline = -1, प्रसारः;
	s64 zero_vruntime = -1, योग_w_vruntime = -1;
	u64 avruntime;
	संरचना sched_entity *चरमः, *first, *मूलम्;
	संरचना rq *rq = cpu_rq(cpu);
	अचिह्नित पूर्णाङ्क योग_सरणम्;
	अचिह्नित दीर्घ flags;
	u64 योग_weight;

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
	SEQ_printf(m, "\n");
	SEQ_printf_task_गणः_मार्गः(m, cfs_rq->tg, "cfs_rq[%d]:%s\n", cpu);
#अन्यथा
	SEQ_printf(m, "\n");
	SEQ_printf(m, "cfs_rq[%d]:\n", cpu);
#यदि_अन्तः

	raw_spin_rq_तालकम्_irqsave(rq, flags);
	मूलम् = __pick_root_entity(cfs_rq);
	यदि (मूलम्)
		वामः_vruntime = मूलम्->min_vruntime;
	first = __pick_first_entity(cfs_rq);
	यदि (first)
		वामः_deadline = first->deadline;
	चरमः = __pick_last_entity(cfs_rq);
	यदि (चरमः)
		दक्षिणः_vruntime = चरमः->vruntime;
	zero_vruntime = cfs_rq->zero_vruntime;
	योग_w_vruntime = cfs_rq->योग_w_vruntime;
	योग_weight = cfs_rq->योग_weight;
	योग_सरणम् = cfs_rq->योग_सरणम्;
	avruntime = avg_vruntime(cfs_rq);
	raw_spin_rq_unlock_irqrestore(rq, flags);

	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "left_deadline",
			विभज_NS(वामः_deadline));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "left_vruntime",
			विभज_NS(वामः_vruntime));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "zero_vruntime",
			विभज_NS(zero_vruntime));
	SEQ_printf(m, "  .%-30s: %Ld (%d bits)\n", "sum_w_vruntime",
		   योग_w_vruntime, ilog2(निरपेक्षम्(योग_w_vruntime)));
	SEQ_printf(m, "  .%-30s: %Lu\n", "sum_weight",
		   योग_weight);
	SEQ_printf(m, "  .%-30s: %u\n", "sum_shift", योग_सरणम्);
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "avg_vruntime",
			विभज_NS(avruntime));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "right_vruntime",
			विभज_NS(दक्षिणः_vruntime));
	प्रसारः = दक्षिणः_vruntime - वामः_vruntime;
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "spread", विभज_NS(प्रसारः));
	SEQ_printf(m, "  .%-30s: %d\n", "nr_queued", cfs_rq->nr_queued);
	SEQ_printf(m, "  .%-30s: %d\n", "h_nr_runnable", cfs_rq->h_nr_runnable);
	SEQ_printf(m, "  .%-30s: %d\n", "h_nr_queued", cfs_rq->h_nr_queued);
	SEQ_printf(m, "  .%-30s: %d\n", "h_nr_idle", cfs_rq->h_nr_idle);
	SEQ_printf(m, "  .%-30s: %ld\n", "load", cfs_rq->आरोपय.weight);
	SEQ_printf(m, "  .%-30s: %lu\n", "load_avg",
			cfs_rq->avg.आरोपय_avg);
	SEQ_printf(m, "  .%-30s: %lu\n", "runnable_avg",
			cfs_rq->avg.runnable_avg);
	SEQ_printf(m, "  .%-30s: %lu\n", "util_avg",
			cfs_rq->avg.util_avg);
	SEQ_printf(m, "  .%-30s: %u\n", "util_est",
			cfs_rq->avg.util_est);
	SEQ_printf(m, "  .%-30s: %ld\n", "removed.load_avg",
			cfs_rq->removed.आरोपय_avg);
	SEQ_printf(m, "  .%-30s: %ld\n", "removed.util_avg",
			cfs_rq->removed.util_avg);
	SEQ_printf(m, "  .%-30s: %ld\n", "removed.runnable_avg",
			cfs_rq->removed.runnable_avg);
#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
	SEQ_printf(m, "  .%-30s: %lu\n", "tg_load_avg_contrib",
			cfs_rq->tg_आरोपय_avg_contrib);
	SEQ_printf(m, "  .%-30s: %ld\n", "tg_load_avg",
			atomic_long_read(&cfs_rq->tg->आरोपय_avg));
	SEQ_printf(m, "  .%-30s: %lu\n", "h_load",
			cfs_rq->h_आरोपय);
#यदि_अन्तः /* CONFIG_FAIR_GROUP_SCHED */
#यदि_परिभाषितम् CONFIG_CFS_BANDWIDTH
	SEQ_printf(m, "  .%-30s: %d\n", "throttled",
			cfs_rq->throttled);
	SEQ_printf(m, "  .%-30s: %d\n", "throttle_count",
			cfs_rq->throttle_गणनम्);
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
	print_cfs_गणः_stats(m, cpu, cfs_rq->tg);
#यदि_अन्तः
}

शून्य print_rt_rq(संरचना seq_file *m, पूर्णाङ्क cpu, संरचना rt_rq *rt_rq)
{
#यदि_परिभाषितम् CONFIG_RT_गणः_SCHED
	SEQ_printf(m, "\n");
	SEQ_printf_task_गणः_मार्गः(m, rt_rq->tg, "rt_rq[%d]:%s\n", cpu);
#अन्यथा
	SEQ_printf(m, "\n");
	SEQ_printf(m, "rt_rq[%d]:\n", cpu);
#यदि_अन्तः

#परिभाषय P(x) \
	SEQ_printf(m, "  .%-30s: %Ld\n", #x, (दीर्घ दीर्घ)(rt_rq->x))
#परिभाषय PU(x) \
	SEQ_printf(m, "  .%-30s: %lu\n", #x, (अचिह्नित दीर्घ)(rt_rq->x))
#परिभाषय PN(x) \
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", #x, विभज_NS(rt_rq->x))

	PU(rt_nr_running);

#यदि_परिभाषितम् CONFIG_RT_गणः_SCHED
	P(rt_throttled);
	PN(rt_time);
	PN(rt_runtime);
#यदि_अन्तः

#अपरिभाषय PN
#अपरिभाषय PU
#अपरिभाषय P
}

शून्य print_dl_rq(संरचना seq_file *m, पूर्णाङ्क cpu, संरचना dl_rq *dl_rq)
{
	संरचना dl_bw *dl_bw;

	SEQ_printf(m, "\n");
	SEQ_printf(m, "dl_rq[%d]:\n", cpu);

#परिभाषय PU(x) \
	SEQ_printf(m, "  .%-30s: %lu\n", #x, (अचिह्नित दीर्घ)(dl_rq->x))

	PU(dl_nr_running);
	dl_bw = &cpu_rq(cpu)->rd->dl_bw;
	SEQ_printf(m, "  .%-30s: %lld\n", "dl_bw->bw", dl_bw->bw);
	SEQ_printf(m, "  .%-30s: %lld\n", "dl_bw->total_bw", dl_bw->योग_bw);

#अपरिभाषय PU
}

स्थिर शून्य print_cpu(संरचना seq_file *m, पूर्णाङ्क cpu)
{
	संरचना rq *rq = cpu_rq(cpu);

#यदि_परिभाषितम् CONFIG_X86
	{
		अचिह्नित पूर्णाङ्क freq = cpu_khz ? : 1;

		SEQ_printf(m, "cpu#%d, %u.%03u MHz\n",
			   cpu, freq / 1000, (freq % 1000));
	}
#अन्यथा /* !CONFIG_X86: */
	SEQ_printf(m, "cpu#%d\n", cpu);
#यदि_अन्तः /* !CONFIG_X86 */

#परिभाषय P(x)								\
कुरु {									\
	यदि (परिमाण(rq->x) == 4)						\
		SEQ_printf(m, "  .%-30s: %d\n", #x, (पूर्णाङ्क)(rq->x));	\
	अन्यथा								\
		SEQ_printf(m, "  .%-30s: %Ld\n", #x, (दीर्घ दीर्घ)(rq->x));\
} यावत् (0)

#परिभाषय PN(x) \
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", #x, विभज_NS(rq->x))

	P(nr_running);
	P(nr_switches);
	P(nr_uninterruptible);
	PN(अग्र_balance);
	SEQ_printf(m, "  .%-30s: %ld\n", "curr->pid", (दीर्घ)(task_pid_nr(rq->curr)));
	PN(clock);
	PN(clock_task);
#अपरिभाषय P
#अपरिभाषय PN

#परिभाषय P64(n) SEQ_printf(m, "  .%-30s: %Ld\n", #n, rq->n);
	P64(avg_idle);
	P64(max_idle_balance_cost);
#अपरिभाषय P64

#परिभाषय P(n) SEQ_printf(m, "  .%-30s: %d\n", #n, schedstat_val(rq->n));
	यदि (schedstat_enabled()) {
		P(yld_गणनम्);
		P(sched_गणनम्);
		P(sched_goidle);
		P(ttwu_गणनम्);
		P(ttwu_local);
	}
#अपरिभाषय P

	print_cfs_stats(m, cpu);
	print_rt_stats(m, cpu);
	print_dl_stats(m, cpu);

	print_rq(m, rq, cpu);
	SEQ_printf(m, "\n");
}

स्थिर ध्रुव वर्ण *sched_tunable_scaling_नामानि[] = {
	"none",
	"logarithmic",
	"linear"
};

स्थिर शून्य sched_debug_शीर्षकम्(संरचना seq_file *m)
{
	u64 ktime, sched_clk, cpu_clk;
	अचिह्नित दीर्घ flags;

	local_irq_save(flags);
	ktime = ktime_गन्तव्यम्_ns(ktime_get());
	sched_clk = sched_clock();
	cpu_clk = local_clock();
	local_irq_restore(flags);

	SEQ_printf(m, "Sched Debug Version: v0.11, %s %.*s\n",
		init_utsname()->release,
		(पूर्णाङ्क)strcspn(init_utsname()->संस्करणम्, " "),
		init_utsname()->संस्करणम्);

#परिभाषय P(x) \
	SEQ_printf(m, "%-40s: %Ld\n", #x, (दीर्घ दीर्घ)(x))
#परिभाषय PN(x) \
	SEQ_printf(m, "%-40s: %Ld.%06ld\n", #x, विभज_NS(x))
	PN(ktime);
	PN(sched_clk);
	PN(cpu_clk);
	P(jiffies);
#यदि_परिभाषितम् CONFIG_HAVE_UNSTABLE_SCHED_CLOCK
	P(sched_clock_stable());
#यदि_अन्तः
#अपरिभाषय PN
#अपरिभाषय P

	SEQ_printf(m, "\n");
	SEQ_printf(m, "sysctl_sched\n");

#परिभाषय P(x) \
	SEQ_printf(m, "  .%-40s: %Ld\n", #x, (दीर्घ दीर्घ)(x))
#परिभाषय PN(x) \
	SEQ_printf(m, "  .%-40s: %Ld.%06ld\n", #x, विभज_NS(x))
	PN(sysctl_sched_आधारः_खण्डिका);
	P(sysctl_sched_features);
#अपरिभाषय PN
#अपरिभाषय P

	SEQ_printf(m, "  .%-40s: %d (%s)\n",
		"sysctl_sched_tunable_scaling",
		sysctl_sched_tunable_scaling,
		sched_tunable_scaling_नामानि[sysctl_sched_tunable_scaling]);
	SEQ_printf(m, "\n");
}

स्थिर पूर्णाङ्क sched_debug_show(संरचना seq_file *m, शून्य *v)
{
	पूर्णाङ्क cpu = (अचिह्नित दीर्घ)(v - 2);

	यदि (cpu != -1)
		print_cpu(m, cpu);
	अन्यथा
		sched_debug_शीर्षकम्(m);

	प्रत्यागम 0;
}

शून्य sysrq_sched_debug_show(शून्य)
{
	पूर्णाङ्क cpu;

	sched_debug_शीर्षकम्(रिक्त);
	for_each_online_cpu(cpu) {
		/*
		 * Need to reset softlockup watchdogs on all CPUs, because
		 * another CPU might be blocked waiting for us to process
		 * an IPI or stop_machine.
		 */
		touch_nmi_watchdog();
		touch_all_softlockup_watchdogs();
		print_cpu(रिक्त, cpu);
	}
}

/*
 * This iterator needs some explanation.
 * It returns 1 for the header position.
 * This means 2 is CPU 0.
 * In a hotplugged system some CPUs, including CPU 0, may be missing so we have
 * to use cpumask_* to iterate over the CPUs.
 */
स्थिर शून्य *sched_debug_start(संरचना seq_file *file, loff_t *विस्थापनम्)
{
	अचिह्नित दीर्घ n = *विस्थापनम्;

	यदि (n == 0)
		प्रत्यागम (शून्य *) 1;

	n--;

	यदि (n > 0)
		n = cpumask_अग्र(n - 1, cpu_online_आवरणम्);
	अन्यथा
		n = cpumask_first(cpu_online_आवरणम्);

	*विस्थापनम् = n + 1;

	यदि (n < nr_cpu_ids)
		प्रत्यागम (शून्य *)(अचिह्नित दीर्घ)(n + 2);

	प्रत्यागम रिक्त;
}

स्थिर शून्य *sched_debug_अग्र(संरचना seq_file *file, शून्य *दत्त, loff_t *विस्थापनम्)
{
	(*विस्थापनम्)++;
	प्रत्यागम sched_debug_start(file, विस्थापनम्);
}

स्थिर शून्य sched_debug_stop(संरचना seq_file *file, शून्य *दत्त)
{
}

स्थिर ध्रुव संरचना seq_operations sched_debug_sops = {
	.start		= sched_debug_start,
	.अग्र		= sched_debug_अग्र,
	.stop		= sched_debug_stop,
	.show		= sched_debug_show,
};

#परिभाषय __PS(S, F) SEQ_printf(m, "%-45s:%21Ld\n", S, (दीर्घ दीर्घ)(F))
#परिभाषय __P(F) __PS(#F, F)
#परिभाषय   P(F) __PS(#F, p->F)
#परिभाषय   PM(F, M) __PS(#F, p->F & (M))
#परिभाषय __PSN(S, F) SEQ_printf(m, "%-45s:%14Ld.%06ld\n", S, विभज_NS((दीर्घ दीर्घ)(F)))
#परिभाषय __PN(F) __PSN(#F, F)
#परिभाषय   PN(F) __PSN(#F, p->F)


#यदि_परिभाषितम् CONFIG_NUMA_BALANCING
शून्य print_numa_stats(संरचना seq_file *m, पूर्णाङ्क ग्रन्थिः, अचिह्नित दीर्घ tsf,
		अचिह्नित दीर्घ tpf, अचिह्नित दीर्घ gsf, अचिह्नित दीर्घ gpf)
{
	SEQ_printf(m, "numa_faults node=%d ", ग्रन्थिः);
	SEQ_printf(m, "task_private=%lu task_shared=%lu ", tpf, tsf);
	SEQ_printf(m, "group_private=%lu group_shared=%lu\n", gpf, gsf);
}
#यदि_अन्तः


स्थिर शून्य sched_show_numa(संरचना task_struct *p, संरचना seq_file *m)
{
#यदि_परिभाषितम् CONFIG_NUMA_BALANCING
	यदि (p->mm)
		P(mm->numa_अवलोकनम्_seq);

	P(numa_पृष्ठानि_migrated);
	P(numa_preferred_nid);
	P(योग_numa_दोषाः);
	SEQ_printf(m, "current_node=%d, numa_group_id=%d\n",
			task_ग्रन्थिः(p), task_numa_गणः_id(p));
	show_numa_stats(p, m);
#यदि_अन्तः /* CONFIG_NUMA_BALANCING */
}

शून्य proc_sched_show_task(संरचना task_struct *p, संरचना pid_namespace *ns,
						  संरचना seq_file *m)
{
	अचिह्नित दीर्घ nr_switches;

	SEQ_printf(m, "%s (%d, #threads: %d)\n", p->comm, task_pid_nr_ns(p, ns),
						get_nr_तन्तवः(p));
	SEQ_printf(m,
		"---------------------------------------------------------"
		"----------\n");

#परिभाषय P_SCHEDSTAT(F)  __PS(#F, schedstat_val(p->stats.F))
#परिभाषय PN_SCHEDSTAT(F) __PSN(#F, schedstat_val(p->stats.F))

	PN(se.exec_start);
	PN(se.vruntime);
	PN(se.योग_exec_runtime);

	nr_switches = p->nvcsw + p->nivcsw;

	P(se.nr_migrations);

	यदि (schedstat_enabled()) {
		u64 avg_atom, avg_प्रत्येकम्_cpu;

		PN_SCHEDSTAT(योग_sleep_runtime);
		PN_SCHEDSTAT(योग_खण्डम्_runtime);
		PN_SCHEDSTAT(wait_start);
		PN_SCHEDSTAT(sleep_start);
		PN_SCHEDSTAT(खण्डम्_start);
		PN_SCHEDSTAT(sleep_max);
		PN_SCHEDSTAT(खण्डम्_max);
		PN_SCHEDSTAT(exec_max);
		PN_SCHEDSTAT(खण्डिका_max);
		PN_SCHEDSTAT(wait_max);
		PN_SCHEDSTAT(wait_योग);
		P_SCHEDSTAT(wait_गणनम्);
		PN_SCHEDSTAT(iowait_योग);
		P_SCHEDSTAT(iowait_गणनम्);
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

		avg_atom = p->se.योग_exec_runtime;
		यदि (nr_switches)
			avg_atom = div64_ul(avg_atom, nr_switches);
		अन्यथा
			avg_atom = -1LL;

		avg_प्रत्येकम्_cpu = p->se.योग_exec_runtime;
		यदि (p->se.nr_migrations) {
			avg_प्रत्येकम्_cpu = div64_u64(avg_प्रत्येकम्_cpu,
						p->se.nr_migrations);
		} अन्यथा {
			avg_प्रत्येकम्_cpu = -1LL;
		}

		__PN(avg_atom);
		__PN(avg_प्रत्येकम्_cpu);

#यदि_परिभाषितम् CONFIG_SCHED_CORE
		PN_SCHEDSTAT(core_forceidle_योग);
#यदि_अन्तः
	}

	__P(nr_switches);
	__PS("nr_voluntary_switches", p->nvcsw);
	__PS("nr_involuntary_switches", p->nivcsw);

	P(se.आरोपय.weight);
	P(se.avg.आरोपय_योग);
	P(se.avg.runnable_योग);
	P(se.avg.util_योग);
	P(se.avg.आरोपय_avg);
	P(se.avg.runnable_avg);
	P(se.avg.util_avg);
	P(se.avg.चरमः_update_time);
	PM(se.avg.util_est, ~UTIL_AVG_UNCHANGED);
#यदि_परिभाषितम् CONFIG_UCLAMP_TASK
	__PS("uclamp.min", p->uclamp_req[UCLAMP_MIN].मानम्);
	__PS("uclamp.max", p->uclamp_req[UCLAMP_MAX].मानम्);
	__PS("effective uclamp.min", uclamp_eff_मानम्(p, UCLAMP_MIN));
	__PS("effective uclamp.max", uclamp_eff_मानम्(p, UCLAMP_MAX));
#यदि_अन्तः /* CONFIG_UCLAMP_TASK */
	P(policy);
	P(prio);
	यदि (task_has_dl_policy(p)) {
		P(dl.runtime);
		P(dl.deadline);
	} अन्यथा यदि (fair_policy(p->policy)) {
		P(se.खण्डिका);
	}
#यदि_परिभाषितम् CONFIG_SCHED_CLASS_EXT
	__PS("ext.enabled", task_on_scx(p));
#यदि_अन्तः
#अपरिभाषय PN_SCHEDSTAT
#अपरिभाषय P_SCHEDSTAT

	{
		अचिह्नित पूर्णाङ्क this_cpu = raw_smp_processor_id();
		u64 t0, t1;

		t0 = cpu_clock(this_cpu);
		t1 = cpu_clock(this_cpu);
		__PS("clock-delta", t1-t0);
	}

	sched_show_numa(p, m);
}

शून्य proc_sched_समुच्चयः_task(संरचना task_struct *p)
{
#यदि_परिभाषितम् CONFIG_SCHEDSTATS
	स्मृतिं_पूरय(&p->stats, 0, परिमाण(p->stats));
#यदि_अन्तः
}

शून्य resched_latency_warn(पूर्णाङ्क cpu, u64 latency)
{
	स्थिर DEFINE_RATELIMIT_STATE(latency_check_ratelimit, 60 * 60 * HZ, 1);

	यदि (likely(!__ratelimit(&latency_check_ratelimit)))
		प्रत्यागम;

	pr_err("sched: CPU %d need_resched set for > %llu ns (%d ticks) without schedule\n",
	       cpu, latency, cpu_rq(cpu)->ticks_without_resched);
	dump_राशिः();
}
