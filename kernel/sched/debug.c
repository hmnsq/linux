// SPDX-License-Identifier: GPL-2.0-only
/*
 * kernel/sched/debug.c
 *
 * Print the CFS rbtree and other debugging details
 *
 * Copyright(C) 2007, Red Hat, Inc., Ingo Molnar
 */
#समावेश <linux/debugfs.h>
#समावेश <linux/nmi.h>
#समावेश <linux/log2.h>
#समावेश "sched.h"

/*
 * This allows printing both to /sys/kernel/debug/sched/debug and
 * to the console
 */
#घोषणा SEQ_printf(m, x...)			\
 करो {						\
	अगर (m)					\
		seq_printf(m, x);		\
	अन्यथा					\
		pr_cont(x);			\
 } जबतक (0)

/*
 * Ease the printing of nsec fields:
 */
अटल दीर्घ दीर्घ nsec_ऊपरी(अचिन्हित दीर्घ दीर्घ nsec)
{
	अगर ((दीर्घ दीर्घ)nsec < 0) {
		nsec = -nsec;
		do_div(nsec, 1000000);
		वापस -nsec;
	}
	do_div(nsec, 1000000);

	वापस nsec;
}

अटल अचिन्हित दीर्घ nsec_निचला(अचिन्हित दीर्घ दीर्घ nsec)
{
	अगर ((दीर्घ दीर्घ)nsec < 0)
		nsec = -nsec;

	वापस do_div(nsec, 1000000);
}

#घोषणा SPLIT_NS(x) nsec_ऊपरी(x), nsec_निचला(x)

#घोषणा SCHED_FEAT(name, enabled)	\
	#name ,

अटल स्थिर अक्षर * स्थिर sched_feat_names[] = {
#समावेश "features.h"
};

#अघोषित SCHED_FEAT

अटल पूर्णांक sched_feat_show(काष्ठा seq_file *m, व्योम *v)
{
	पूर्णांक i;

	क्रम (i = 0; i < __SCHED_FEAT_NR; i++) {
		अगर (!(sysctl_sched_features & (1UL << i)))
			seq_puts(m, "NO_");
		seq_printf(m, "%s ", sched_feat_names[i]);
	}
	seq_puts(m, "\n");

	वापस 0;
}

#अगर_घोषित CONFIG_JUMP_LABEL

#घोषणा jump_label_कुंजी_true  { .कुंजी_true  = STATIC_कुंजी_TRUE_INIT  }
#घोषणा jump_label_कुंजी_false { .कुंजी_false = STATIC_कुंजी_FALSE_INIT }

#घोषणा SCHED_FEAT(name, enabled)	\
	jump_label_कुंजी##enabled ,

जोड़ sched_feat_कुंजी sched_feat_कुंजियाँ[__SCHED_FEAT_NR] = {
#समावेश "features.h"
};

#अघोषित SCHED_FEAT

अटल व्योम sched_feat_disable(पूर्णांक i)
{
	static_branch_disable_cpuslocked(&sched_feat_कुंजियाँ[i].कुंजी_true);
}

अटल व्योम sched_feat_enable(पूर्णांक i)
{
	static_branch_enable_cpuslocked(&sched_feat_कुंजियाँ[i].कुंजी_false);
}
#अन्यथा /* !CONFIG_JUMP_LABEL: */
अटल व्योम sched_feat_disable(पूर्णांक i) { };
अटल व्योम sched_feat_enable(पूर्णांक i) { };
#पूर्ण_अगर /* !CONFIG_JUMP_LABEL */

अटल पूर्णांक sched_feat_set(अक्षर *cmp)
{
	पूर्णांक i;
	पूर्णांक neg = 0;

	अगर (म_भेदन(cmp, "NO_", 3) == 0) {
		neg = 1;
		cmp += 3;
	}

	i = match_string(sched_feat_names, __SCHED_FEAT_NR, cmp);
	अगर (i < 0)
		वापस i;

	अगर (neg) {
		sysctl_sched_features &= ~(1UL << i);
		sched_feat_disable(i);
	} अन्यथा {
		sysctl_sched_features |= (1UL << i);
		sched_feat_enable(i);
	}

	वापस 0;
}

अटल ssize_t
sched_feat_write(काष्ठा file *filp, स्थिर अक्षर __user *ubuf,
		माप_प्रकार cnt, loff_t *ppos)
{
	अक्षर buf[64];
	अक्षर *cmp;
	पूर्णांक ret;
	काष्ठा inode *inode;

	अगर (cnt > 63)
		cnt = 63;

	अगर (copy_from_user(buf, ubuf, cnt))
		वापस -EFAULT;

	buf[cnt] = 0;
	cmp = strstrip(buf);

	/* Ensure the static_key remains in a consistent state */
	inode = file_inode(filp);
	cpus_read_lock();
	inode_lock(inode);
	ret = sched_feat_set(cmp);
	inode_unlock(inode);
	cpus_read_unlock();
	अगर (ret < 0)
		वापस ret;

	*ppos += cnt;

	वापस cnt;
}

अटल पूर्णांक sched_feat_open(काष्ठा inode *inode, काष्ठा file *filp)
{
	वापस single_open(filp, sched_feat_show, शून्य);
}

अटल स्थिर काष्ठा file_operations sched_feat_fops = {
	.खोलो		= sched_feat_open,
	.ग_लिखो		= sched_feat_write,
	.पढ़ो		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

अटल ssize_t sched_scaling_write(काष्ठा file *filp, स्थिर अक्षर __user *ubuf,
				   माप_प्रकार cnt, loff_t *ppos)
{
	अचिन्हित पूर्णांक scaling;
	पूर्णांक ret;

	ret = kstrtouint_from_user(ubuf, cnt, 10, &scaling);
	अगर (ret)
		वापस ret;

	अगर (scaling >= SCHED_TUNABLESCALING_END)
		वापस -EINVAL;

	sysctl_sched_tunable_scaling = scaling;
	अगर (sched_update_scaling())
		वापस -EINVAL;

	*ppos += cnt;
	वापस cnt;
}

अटल पूर्णांक sched_scaling_show(काष्ठा seq_file *m, व्योम *v)
{
	seq_printf(m, "%d\n", sysctl_sched_tunable_scaling);
	वापस 0;
}

अटल पूर्णांक sched_scaling_open(काष्ठा inode *inode, काष्ठा file *filp)
{
	वापस single_open(filp, sched_scaling_show, शून्य);
}

अटल स्थिर काष्ठा file_operations sched_scaling_fops = {
	.खोलो		= sched_scaling_open,
	.ग_लिखो		= sched_scaling_write,
	.पढ़ो		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#अगर_घोषित CONFIG_SCHED_CACHE
अटल ssize_t
sched_cache_enable_write(काष्ठा file *filp, स्थिर अक्षर __user *ubuf,
			 माप_प्रकार cnt, loff_t *ppos)
{
	bool val;
	पूर्णांक ret;

	ret = kstrtobool_from_user(ubuf, cnt, &val);
	अगर (ret)
		वापस ret;

	sysctl_sched_cache_user = val;

	sched_cache_active_set();

	*ppos += cnt;

	वापस cnt;
}

अटल पूर्णांक sched_cache_enable_show(काष्ठा seq_file *m, व्योम *v)
{
	seq_printf(m, "%d\n", sysctl_sched_cache_user);
	वापस 0;
}

अटल पूर्णांक sched_cache_enable_open(काष्ठा inode *inode,
				   काष्ठा file *filp)
{
	वापस single_open(filp, sched_cache_enable_show, शून्य);
}

अटल स्थिर काष्ठा file_operations sched_cache_enable_fops = {
	.खोलो           = sched_cache_enable_open,
	.ग_लिखो          = sched_cache_enable_write,
	.पढ़ो           = seq_read,
	.llseek         = seq_lseek,
	.release        = single_release,
};
#पूर्ण_अगर

#अगर_घोषित CONFIG_PREEMPT_DYNAMIC

अटल ssize_t sched_dynamic_write(काष्ठा file *filp, स्थिर अक्षर __user *ubuf,
				   माप_प्रकार cnt, loff_t *ppos)
{
	अक्षर buf[16];
	पूर्णांक mode;

	अगर (cnt > 15)
		cnt = 15;

	अगर (copy_from_user(buf, ubuf, cnt))
		वापस -EFAULT;

	buf[cnt] = 0;
	mode = sched_dynamic_mode(strstrip(buf));
	अगर (mode < 0)
		वापस mode;

	sched_dynamic_update(mode);

	*ppos += cnt;

	वापस cnt;
}

अटल पूर्णांक sched_dynamic_show(काष्ठा seq_file *m, व्योम *v)
{
	पूर्णांक mode = READ_ONCE(preempt_dynamic_mode);

	/* Stop at NULL terminator */
	क्रम (पूर्णांक i = 0; preempt_modes[i]; i++) {
		अगर (mode == i)
			seq_puts(m, "(");
		seq_puts(m, preempt_modes[i]);
		अगर (mode == i)
			seq_puts(m, ")");

		seq_puts(m, " ");
	}

	seq_puts(m, "\n");
	वापस 0;
}

अटल पूर्णांक sched_dynamic_open(काष्ठा inode *inode, काष्ठा file *filp)
{
	वापस single_open(filp, sched_dynamic_show, शून्य);
}

अटल स्थिर काष्ठा file_operations sched_dynamic_fops = {
	.खोलो		= sched_dynamic_open,
	.ग_लिखो		= sched_dynamic_write,
	.पढ़ो		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#पूर्ण_अगर /* CONFIG_PREEMPT_DYNAMIC */

__read_mostly bool sched_debug_verbose;

अटल काष्ठा dentry           *sd_dentry;


अटल ssize_t sched_verbose_write(काष्ठा file *filp, स्थिर अक्षर __user *ubuf,
				  माप_प्रकार cnt, loff_t *ppos)
{
	ssize_t परिणाम;
	bool orig;

	cpus_read_lock();
	sched_domains_mutex_lock();

	orig = sched_debug_verbose;
	परिणाम = debugfs_write_file_bool(filp, ubuf, cnt, ppos);

	अगर (sched_debug_verbose && !orig)
		update_sched_domain_debugfs();
	अन्यथा अगर (!sched_debug_verbose && orig) {
		debugfs_remove(sd_dentry);
		sd_dentry = शून्य;
	}

	sched_domains_mutex_unlock();
	cpus_read_unlock();

	वापस परिणाम;
}

अटल स्थिर काष्ठा file_operations sched_verbose_fops = {
	.पढ़ो =         debugfs_read_file_bool,
	.ग_लिखो =        sched_verbose_write,
	.खोलो =         simple_open,
	.llseek =       default_llseek,
};

अटल स्थिर काष्ठा seq_operations sched_debug_sops;

अटल पूर्णांक sched_debug_open(काष्ठा inode *inode, काष्ठा file *filp)
{
	वापस seq_open(filp, &sched_debug_sops);
}

अटल स्थिर काष्ठा file_operations sched_debug_fops = {
	.खोलो		= sched_debug_open,
	.पढ़ो		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

क्रमागत dl_param {
	DL_RUNTIME = 0,
	DL_PERIOD,
};

अटल अचिन्हित दीर्घ dl_server_period_max = (1UL << 22) * NSEC_PER_USEC; /* ~4 seconds */
अटल अचिन्हित दीर्घ dl_server_period_min = (100) * NSEC_PER_USEC;     /* 100 us */

अटल ssize_t sched_server_write_common(काष्ठा file *filp, स्थिर अक्षर __user *ubuf,
					 माप_प्रकार cnt, loff_t *ppos, क्रमागत dl_param param,
					 व्योम *server)
{
	दीर्घ cpu = (दीर्घ) ((काष्ठा seq_file *) filp->private_तथ्य)->निजी;
	काष्ठा sched_dl_entity *dl_se = (काष्ठा sched_dl_entity *)server;
	u64 old_runtime, runtime, period;
	काष्ठा rq *rq = cpu_rq(cpu);
	पूर्णांक retval = 0;
	माप_प्रकार err;
	u64 मूल्य;

	err = kstrtoull_from_user(ubuf, cnt, 10, &मूल्य);
	अगर (err)
		वापस err;

	scoped_guard (rq_lock_irqsave, rq) {
		old_runtime = runtime = dl_se->dl_runtime;
		period = dl_se->dl_period;

		चयन (param) {
		हाल DL_RUNTIME:
			अगर (runtime == मूल्य)
				अवरोध;
			runtime = मूल्य;
			अवरोध;
		हाल DL_PERIOD:
			अगर (मूल्य == period)
				अवरोध;
			period = मूल्य;
			अवरोध;
		}

		अगर (runtime > period ||
		    period > dl_server_period_max ||
		    period < dl_server_period_min) {
			वापस  -EINVAL;
		}

		अगर (!cpu_online(cpu_of(rq)))
			वापस -EBUSY;

		update_rq_clock(rq);
		dl_server_stop(dl_se);
		retval = dl_server_apply_params(dl_se, runtime, period, 0);
		dl_server_start(dl_se);

		अगर (retval < 0)
			वापस retval;
	}

	अगर (!!old_runtime ^ !!runtime) {
		pr_info("%s server %sabled on CPU %d%s.\n",
			server == &rq->fair_server ? "Fair" : "Ext",
			runtime ? "en" : "dis",
			cpu_of(rq),
			runtime ? "" : ", system may malfunction due to starvation");
	}

	*ppos += cnt;
	वापस cnt;
}

अटल माप_प्रकार sched_server_show_common(काष्ठा seq_file *m, व्योम *v, क्रमागत dl_param param,
				       व्योम *server)
{
	काष्ठा sched_dl_entity *dl_se = (काष्ठा sched_dl_entity *)server;
	u64 मूल्य;

	चयन (param) {
	हाल DL_RUNTIME:
		मूल्य = dl_se->dl_runtime;
		अवरोध;
	हाल DL_PERIOD:
		मूल्य = dl_se->dl_period;
		अवरोध;
	}

	seq_printf(m, "%llu\n", मूल्य);
	वापस 0;
}

अटल ssize_t
sched_fair_server_runtime_write(काष्ठा file *filp, स्थिर अक्षर __user *ubuf,
				माप_प्रकार cnt, loff_t *ppos)
{
	दीर्घ cpu = (दीर्घ) ((काष्ठा seq_file *) filp->private_तथ्य)->निजी;
	काष्ठा rq *rq = cpu_rq(cpu);

	वापस sched_server_write_common(filp, ubuf, cnt, ppos, DL_RUNTIME,
					&rq->fair_server);
}

अटल पूर्णांक sched_fair_server_runtime_show(काष्ठा seq_file *m, व्योम *v)
{
	अचिन्हित दीर्घ cpu = (अचिन्हित दीर्घ) m->निजी;
	काष्ठा rq *rq = cpu_rq(cpu);

	वापस sched_server_show_common(m, v, DL_RUNTIME, &rq->fair_server);
}

अटल पूर्णांक sched_fair_server_runtime_open(काष्ठा inode *inode, काष्ठा file *filp)
{
	वापस single_open(filp, sched_fair_server_runtime_show, inode->i_private);
}

अटल स्थिर काष्ठा file_operations fair_server_runtime_fops = {
	.खोलो		= sched_fair_server_runtime_open,
	.ग_लिखो		= sched_fair_server_runtime_write,
	.पढ़ो		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

अटल काष्ठा dentry *debugfs_sched;

#अगर_घोषित CONFIG_SCHED_CLASS_EXT
अटल ssize_t
sched_ext_server_runtime_write(काष्ठा file *filp, स्थिर अक्षर __user *ubuf,
			       माप_प्रकार cnt, loff_t *ppos)
{
	दीर्घ cpu = (दीर्घ) ((काष्ठा seq_file *) filp->private_तथ्य)->निजी;
	काष्ठा rq *rq = cpu_rq(cpu);

	वापस sched_server_write_common(filp, ubuf, cnt, ppos, DL_RUNTIME,
					&rq->ext_server);
}

अटल पूर्णांक sched_ext_server_runtime_show(काष्ठा seq_file *m, व्योम *v)
{
	अचिन्हित दीर्घ cpu = (अचिन्हित दीर्घ) m->निजी;
	काष्ठा rq *rq = cpu_rq(cpu);

	वापस sched_server_show_common(m, v, DL_RUNTIME, &rq->ext_server);
}

अटल पूर्णांक sched_ext_server_runtime_open(काष्ठा inode *inode, काष्ठा file *filp)
{
	वापस single_open(filp, sched_ext_server_runtime_show, inode->i_private);
}

अटल स्थिर काष्ठा file_operations ext_server_runtime_fops = {
	.खोलो		= sched_ext_server_runtime_open,
	.ग_लिखो		= sched_ext_server_runtime_write,
	.पढ़ो		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

अटल ssize_t
sched_ext_server_period_write(काष्ठा file *filp, स्थिर अक्षर __user *ubuf,
			      माप_प्रकार cnt, loff_t *ppos)
{
	दीर्घ cpu = (दीर्घ) ((काष्ठा seq_file *) filp->private_तथ्य)->निजी;
	काष्ठा rq *rq = cpu_rq(cpu);

	वापस sched_server_write_common(filp, ubuf, cnt, ppos, DL_PERIOD,
					&rq->ext_server);
}

अटल पूर्णांक sched_ext_server_period_show(काष्ठा seq_file *m, व्योम *v)
{
	अचिन्हित दीर्घ cpu = (अचिन्हित दीर्घ) m->निजी;
	काष्ठा rq *rq = cpu_rq(cpu);

	वापस sched_server_show_common(m, v, DL_PERIOD, &rq->ext_server);
}

अटल पूर्णांक sched_ext_server_period_open(काष्ठा inode *inode, काष्ठा file *filp)
{
	वापस single_open(filp, sched_ext_server_period_show, inode->i_private);
}

अटल स्थिर काष्ठा file_operations ext_server_period_fops = {
	.खोलो		= sched_ext_server_period_open,
	.ग_लिखो		= sched_ext_server_period_write,
	.पढ़ो		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

अटल व्योम debugfs_ext_server_init(व्योम)
{
	काष्ठा dentry *d_ext;
	अचिन्हित दीर्घ cpu;

	d_ext = debugfs_create_dir("ext_server", debugfs_sched);
	अगर (!d_ext)
		वापस;

	for_each_possible_cpu(cpu) {
		काष्ठा dentry *d_cpu;
		अक्षर buf[32];

		snprintf(buf, माप(buf), "cpu%lu", cpu);
		d_cpu = debugfs_create_dir(buf, d_ext);

		debugfs_create_file("runtime", 0644, d_cpu, (व्योम *) cpu, &ext_server_runtime_fops);
		debugfs_create_file("period", 0644, d_cpu, (व्योम *) cpu, &ext_server_period_fops);
	}
}
#पूर्ण_अगर /* CONFIG_SCHED_CLASS_EXT */

अटल ssize_t
sched_fair_server_period_write(काष्ठा file *filp, स्थिर अक्षर __user *ubuf,
			       माप_प्रकार cnt, loff_t *ppos)
{
	दीर्घ cpu = (दीर्घ) ((काष्ठा seq_file *) filp->private_तथ्य)->निजी;
	काष्ठा rq *rq = cpu_rq(cpu);

	वापस sched_server_write_common(filp, ubuf, cnt, ppos, DL_PERIOD,
					&rq->fair_server);
}

अटल पूर्णांक sched_fair_server_period_show(काष्ठा seq_file *m, व्योम *v)
{
	अचिन्हित दीर्घ cpu = (अचिन्हित दीर्घ) m->निजी;
	काष्ठा rq *rq = cpu_rq(cpu);

	वापस sched_server_show_common(m, v, DL_PERIOD, &rq->fair_server);
}

अटल पूर्णांक sched_fair_server_period_open(काष्ठा inode *inode, काष्ठा file *filp)
{
	वापस single_open(filp, sched_fair_server_period_show, inode->i_private);
}

अटल स्थिर काष्ठा file_operations fair_server_period_fops = {
	.खोलो		= sched_fair_server_period_open,
	.ग_लिखो		= sched_fair_server_period_write,
	.पढ़ो		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

अटल व्योम debugfs_fair_server_init(व्योम)
{
	काष्ठा dentry *d_fair;
	अचिन्हित दीर्घ cpu;

	d_fair = debugfs_create_dir("fair_server", debugfs_sched);
	अगर (!d_fair)
		वापस;

	for_each_possible_cpu(cpu) {
		काष्ठा dentry *d_cpu;
		अक्षर buf[32];

		snprintf(buf, माप(buf), "cpu%lu", cpu);
		d_cpu = debugfs_create_dir(buf, d_fair);

		debugfs_create_file("runtime", 0644, d_cpu, (व्योम *) cpu, &fair_server_runtime_fops);
		debugfs_create_file("period", 0644, d_cpu, (व्योम *) cpu, &fair_server_period_fops);
	}
}

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
अटल पूर्णांक cgroup_mode = 2;

/* See __sched_cgroup_mode_update(). */
अटल स्थिर अक्षर *cgroup_mode_str[] = {
	"up",
	"smp",
	"concur",
	"max",
	"tasks",
};

अटल पूर्णांक sched_cgroup_mode(स्थिर अक्षर *str)
{
	क्रम (पूर्णांक i = 0; i < ARRAY_आकार(cgroup_mode_str); i++) {
		अगर (!म_भेद(str, cgroup_mode_str[i]))
			वापस i;
	}
	वापस -EINVAL;
}

अटल ssize_t sched_cgroup_write(काष्ठा file *filp, स्थिर अक्षर __user *ubuf,
				   माप_प्रकार cnt, loff_t *ppos)
{
	अक्षर buf[16];
	पूर्णांक mode;

	अगर (cnt > 15)
		cnt = 15;

	अगर (copy_from_user(buf, ubuf, cnt))
		वापस -EFAULT;

	buf[cnt] = 0;
	mode = sched_cgroup_mode(strstrip(buf));
	अगर (mode < 0)
		वापस mode;

	__sched_cgroup_mode_update(mode);
	WRITE_ONCE(cgroup_mode, mode);

	*ppos += cnt;
	वापस cnt;
}

अटल पूर्णांक sched_cgroup_show(काष्ठा seq_file *m, व्योम *v)
{
	पूर्णांक mode = READ_ONCE(cgroup_mode);

	क्रम (पूर्णांक i = 0; i < ARRAY_आकार(cgroup_mode_str); i++) {
		अगर (mode == i)
			seq_puts(m, "(");
		seq_puts(m, cgroup_mode_str[i]);
		अगर (mode == i)
			seq_puts(m, ")");

		seq_puts(m, " ");
	}
	seq_puts(m, "\n");
	वापस 0;
}

अटल पूर्णांक sched_cgroup_open(काष्ठा inode *inode, काष्ठा file *filp)
{
	वापस single_open(filp, sched_cgroup_show, शून्य);
}

अटल स्थिर काष्ठा file_operations sched_cgroup_fops = {
	.खोलो		= sched_cgroup_open,
	.ग_लिखो		= sched_cgroup_write,
	.पढ़ो		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};
#पूर्ण_अगर

अटल __init पूर्णांक sched_init_debug(व्योम)
{
	काष्ठा dentry __maybe_unused *numa, *llc;

	debugfs_sched = debugfs_create_dir("sched", शून्य);

	debugfs_create_file("features", 0644, debugfs_sched, शून्य, &sched_feat_fops);
	debugfs_create_file_unsafe("verbose", 0644, debugfs_sched, &sched_debug_verbose, &sched_verbose_fops);
#अगर_घोषित CONFIG_PREEMPT_DYNAMIC
	debugfs_create_file("preempt", 0644, debugfs_sched, शून्य, &sched_dynamic_fops);
#पूर्ण_अगर

	debugfs_create_u32("base_slice_ns", 0644, debugfs_sched, &sysctl_sched_base_slice);

	debugfs_create_u32("latency_warn_ms", 0644, debugfs_sched, &sysctl_resched_latency_warn_ms);
	debugfs_create_u32("latency_warn_once", 0644, debugfs_sched, &sysctl_resched_latency_warn_once);

	debugfs_create_file("tunable_scaling", 0644, debugfs_sched, शून्य, &sched_scaling_fops);
	debugfs_create_u32("migration_cost_ns", 0644, debugfs_sched, &sysctl_sched_migration_cost);
	debugfs_create_u32("nr_migrate", 0644, debugfs_sched, &sysctl_sched_nr_migrate);

	sched_domains_mutex_lock();
	update_sched_domain_debugfs();
	sched_domains_mutex_unlock();

#अगर_घोषित CONFIG_NUMA_BALANCING
	numa = debugfs_create_dir("numa_balancing", debugfs_sched);

	debugfs_create_u32("scan_delay_ms", 0644, numa, &sysctl_numa_balancing_scan_delay);
	debugfs_create_u32("scan_period_min_ms", 0644, numa, &sysctl_numa_balancing_scan_period_min);
	debugfs_create_u32("scan_period_max_ms", 0644, numa, &sysctl_numa_balancing_scan_period_max);
	debugfs_create_u32("scan_size_mb", 0644, numa, &sysctl_numa_balancing_scan_आकार);
	debugfs_create_u32("hot_threshold_ms", 0644, numa, &sysctl_numa_balancing_hot_threshold);
#पूर्ण_अगर /* CONFIG_NUMA_BALANCING */

#अगर_घोषित CONFIG_SCHED_CACHE
	llc = debugfs_create_dir("llc_balancing", debugfs_sched);
	debugfs_create_file("enabled", 0644, llc, शून्य,
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
#पूर्ण_अगर

	debugfs_create_file("debug", 0444, debugfs_sched, शून्य, &sched_debug_fops);

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
	debugfs_create_file("cgroup_mode", 0644, debugfs_sched, शून्य, &sched_cgroup_fops);
#पूर्ण_अगर

	debugfs_fair_server_init();
#अगर_घोषित CONFIG_SCHED_CLASS_EXT
	debugfs_ext_server_init();
#पूर्ण_अगर

	वापस 0;
}
late_initcall(sched_init_debug);

अटल cpumask_var_t		sd_sysctl_cpus;

अटल पूर्णांक sd_flags_show(काष्ठा seq_file *m, व्योम *v)
{
	अचिन्हित दीर्घ flags = *(अचिन्हित पूर्णांक *)m->निजी;
	पूर्णांक idx;

	for_each_set_bit(idx, &flags, __SD_FLAG_CNT) {
		seq_puts(m, sd_flag_debug[idx].name);
		seq_puts(m, " ");
	}
	seq_puts(m, "\n");

	वापस 0;
}

अटल पूर्णांक sd_flags_open(काष्ठा inode *inode, काष्ठा file *file)
{
	वापस single_open(file, sd_flags_show, inode->i_private);
}

अटल स्थिर काष्ठा file_operations sd_flags_fops = {
	.खोलो		= sd_flags_open,
	.पढ़ो		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

अटल व्योम register_sd(काष्ठा sched_domain *sd, काष्ठा dentry *parent)
{
#घोषणा SDM(type, mode, member)	\
	debugfs_create_##type(#member, mode, parent, &sd->member)

	SDM(ulong, 0644, min_interval);
	SDM(ulong, 0644, max_interval);
	SDM(u64,   0644, max_newidle_lb_cost);
	SDM(u32,   0644, busy_factor);
	SDM(u32,   0644, imbalance_pct);
	SDM(u32,   0644, cache_nice_tries);
	SDM(str,   0444, name);

#अघोषित SDM

	debugfs_create_file("flags", 0444, parent, &sd->flags, &sd_flags_fops);
	debugfs_create_file("groups_flags", 0444, parent, &sd->groups->flags, &sd_flags_fops);
	debugfs_create_u32("level", 0444, parent, (u32 *)&sd->level);

	अगर (sd->flags & SD_ASYM_PACKING)
		debugfs_create_u32("group_asym_prefer_cpu", 0444, parent,
				   (u32 *)&sd->groups->asym_prefer_cpu);
}

व्योम update_sched_domain_debugfs(व्योम)
{
	पूर्णांक cpu, i;

	/*
	 * This can unfortunately be invoked before sched_debug_init() creates
	 * the debug directory. Don't touch sd_sysctl_cpus until then.
	 */
	अगर (!debugfs_sched)
		वापस;

	अगर (!sched_debug_verbose)
		वापस;

	अगर (!cpumask_available(sd_sysctl_cpus)) {
		अगर (!alloc_cpumask_var(&sd_sysctl_cpus, GFP_KERNEL))
			वापस;
		cpumask_copy(sd_sysctl_cpus, cpu_possible_mask);
	}

	अगर (!sd_dentry) {
		sd_dentry = debugfs_create_dir("domains", debugfs_sched);

		/* rebuild sd_sysctl_cpus if empty since it gets cleared below */
		अगर (cpumask_empty(sd_sysctl_cpus))
			cpumask_copy(sd_sysctl_cpus, cpu_online_mask);
	}

	for_each_cpu(cpu, sd_sysctl_cpus) {
		काष्ठा sched_domain *sd;
		काष्ठा dentry *d_cpu;
		अक्षर buf[32];

		snprintf(buf, माप(buf), "cpu%d", cpu);
		debugfs_lookup_and_remove(buf, sd_dentry);
		d_cpu = debugfs_create_dir(buf, sd_dentry);

		i = 0;
		for_each_domain(cpu, sd) {
			काष्ठा dentry *d_sd;

			snprintf(buf, माप(buf), "domain%d", i);
			d_sd = debugfs_create_dir(buf, d_cpu);

			register_sd(sd, d_sd);
			i++;
		}

		__cpumask_clear_cpu(cpu, sd_sysctl_cpus);
	}
}

व्योम dirty_sched_domain_sysctl(पूर्णांक cpu)
{
	अगर (cpumask_available(sd_sysctl_cpus))
		__cpumask_set_cpu(cpu, sd_sysctl_cpus);
}

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
अटल व्योम print_cfs_group_stats(काष्ठा seq_file *m, पूर्णांक cpu, काष्ठा task_group *tg)
{
	काष्ठा sched_entity *se = tg_se(tg, cpu);

#घोषणा P(F)		SEQ_printf(m, "  .%-30s: %lld\n",	#F, (दीर्घ दीर्घ)F)
#घोषणा P_SCHEDSTAT(F)	SEQ_printf(m, "  .%-30s: %lld\n",	\
		#F, (दीर्घ दीर्घ)schedstat_val(stats->F))
#घोषणा PN(F)		SEQ_printf(m, "  .%-30s: %lld.%06ld\n", #F, SPLIT_NS((दीर्घ दीर्घ)F))
#घोषणा PN_SCHEDSTAT(F)	SEQ_printf(m, "  .%-30s: %lld.%06ld\n", \
		#F, SPLIT_NS((दीर्घ दीर्घ)schedstat_val(stats->F)))

	अगर (!se)
		वापस;

	PN(se->exec_start);
	PN(se->vruntime);
	PN(se->योग_exec_runtime);

	अगर (schedstat_enabled()) {
		काष्ठा sched_statistics *stats;
		stats = __schedstats_from_se(se);

		PN_SCHEDSTAT(wait_start);
		PN_SCHEDSTAT(sleep_start);
		PN_SCHEDSTAT(block_start);
		PN_SCHEDSTAT(sleep_max);
		PN_SCHEDSTAT(block_max);
		PN_SCHEDSTAT(exec_max);
		PN_SCHEDSTAT(slice_max);
		PN_SCHEDSTAT(wait_max);
		PN_SCHEDSTAT(wait_योग);
		P_SCHEDSTAT(wait_गिनती);
	}

	P(se->load.weight);
	P(se->avg.load_avg);
	P(se->avg.util_avg);
	P(se->avg.runnable_avg);

#अघोषित PN_SCHEDSTAT
#अघोषित PN
#अघोषित P_SCHEDSTAT
#अघोषित P
}
#पूर्ण_अगर /* CONFIG_FAIR_GROUP_SCHED */

#अगर_घोषित CONFIG_CGROUP_SCHED
अटल DEFINE_SPINLOCK(sched_debug_lock);
अटल अक्षर group_path[PATH_MAX];

अटल व्योम task_group_path(काष्ठा task_group *tg, अक्षर *path, पूर्णांक plen)
{
	अगर (autogroup_path(tg, path, plen))
		वापस;

	cgroup_path(tg->css.cgroup, path, plen);
}

/*
 * Only 1 SEQ_printf_task_group_path() caller can use the full length
 * group_path[] for cgroup path. Other simultaneous callers will have
 * to use a shorter stack buffer. A "..." suffix is appended at the end
 * of the stack buffer so that it will show up in case the output length
 * matches the given buffer size to indicate possible path name truncation.
 */
#घोषणा SEQ_printf_task_group_path(m, tg, fmt...)			\
{									\
	अगर (spin_trylock(&sched_debug_lock)) {				\
		task_group_path(tg, group_path, माप(group_path));	\
		SEQ_printf(m, fmt, group_path);				\
		spin_unlock(&sched_debug_lock);				\
	} अन्यथा {							\
		अक्षर buf[128];						\
		अक्षर *bufend = buf + माप(buf) - 3;			\
		task_group_path(tg, buf, bufend - buf);			\
		म_नकल(bufend - 1, "...");				\
		SEQ_printf(m, fmt, buf);				\
	}								\
}
#पूर्ण_अगर

अटल व्योम
print_task(काष्ठा seq_file *m, काष्ठा rq *rq, काष्ठा task_struct *p)
{
	अगर (task_current(rq, p))
		SEQ_printf(m, ">R");
	अन्यथा
		SEQ_printf(m, " %c", task_state_to_char(p));

	SEQ_printf(m, " %15s %5d %10ld %9Ld.%06ld   %c   %9Ld.%06ld %c %9Ld.%06ld %9Ld.%06ld %9Ld   %5d ",
		p->comm, task_pid_nr(p),
		p->se.h_load.weight,
		SPLIT_NS(p->se.vruntime),
		entity_eligible(&rq->cfs, &p->se) ? 'E' : 'N',
		SPLIT_NS(p->se.deadline),
		p->se.custom_slice ? 'S' : ' ',
		SPLIT_NS(p->se.slice),
		SPLIT_NS(p->se.योग_exec_runtime),
		(दीर्घ दीर्घ)(p->nvcsw + p->nivcsw),
		p->prio);

	SEQ_printf(m, "%9lld.%06ld %9lld.%06ld %9lld.%06ld",
		SPLIT_NS(schedstat_val_or_zero(p->stats.wait_योग)),
		SPLIT_NS(schedstat_val_or_zero(p->stats.योग_sleep_runtime)),
		SPLIT_NS(schedstat_val_or_zero(p->stats.योग_block_runtime)));

#अगर_घोषित CONFIG_NUMA_BALANCING
	SEQ_printf(m, "   %d      %d", task_गाँठ(p), task_numa_group_id(p));
#पूर्ण_अगर
#अगर_घोषित CONFIG_CGROUP_SCHED
	SEQ_printf_task_group_path(m, task_group(p), "        %s")
#पूर्ण_अगर

	SEQ_printf(m, "\n");
}

अटल व्योम print_rq(काष्ठा seq_file *m, काष्ठा rq *rq, पूर्णांक rq_cpu)
{
	काष्ठा task_struct *g, *p;

	SEQ_printf(m, "\n");
	SEQ_printf(m, "runnable tasks:\n");
	SEQ_printf(m, " S            task   PID     weight       vruntime   eligible    "
		   "deadline             slice          sum-exec      switches  "
		   "prio         wait-time        sum-sleep       sum-block"
#अगर_घोषित CONFIG_NUMA_BALANCING
		   "  node   group-id"
#पूर्ण_अगर
#अगर_घोषित CONFIG_CGROUP_SCHED
		   "  group-path"
#पूर्ण_अगर
		   "\n");
	SEQ_printf(m, "-------------------------------------------------------"
		   "------------------------------------------------------"
		   "------------------------------------------------------"
#अगर_घोषित CONFIG_NUMA_BALANCING
		   "--------------"
#पूर्ण_अगर
#अगर_घोषित CONFIG_CGROUP_SCHED
		   "--------------"
#पूर्ण_अगर
		   "\n");

	rcu_read_lock();
	for_each_process_thread(g, p) {
		अगर (task_cpu(p) != rq_cpu)
			जारी;

		print_task(m, rq, p);
	}
	rcu_read_unlock();
}

व्योम print_cfs_rq(काष्ठा seq_file *m, पूर्णांक cpu, काष्ठा cfs_rq *cfs_rq)
{
	s64 बायाँ_vruntime = -1, दायाँ_vruntime = -1, बायाँ_deadline = -1, spread;
	s64 zero_vruntime = -1, योग_w_vruntime = -1;
	u64 avruntime;
	काष्ठा sched_entity *last, *first, *मूल;
	काष्ठा rq *rq = cpu_rq(cpu);
	अचिन्हित पूर्णांक योग_shift;
	अचिन्हित दीर्घ flags;
	u64 योग_weight;

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
	SEQ_printf(m, "\n");
	SEQ_printf_task_group_path(m, cfs_rq->tg, "cfs_rq[%d]:%s\n", cpu);
#अन्यथा
	SEQ_printf(m, "\n");
	SEQ_printf(m, "cfs_rq[%d]:\n", cpu);
#पूर्ण_अगर

	raw_spin_rq_lock_irqsave(rq, flags);
	मूल = __pick_root_entity(cfs_rq);
	अगर (मूल)
		बायाँ_vruntime = मूल->min_vruntime;
	first = __pick_first_entity(cfs_rq);
	अगर (first)
		बायाँ_deadline = first->deadline;
	last = __pick_last_entity(cfs_rq);
	अगर (last)
		दायाँ_vruntime = last->vruntime;
	zero_vruntime = cfs_rq->zero_vruntime;
	योग_w_vruntime = cfs_rq->योग_w_vruntime;
	योग_weight = cfs_rq->योग_weight;
	योग_shift = cfs_rq->योग_shift;
	avruntime = avg_vruntime(cfs_rq);
	raw_spin_rq_unlock_irqrestore(rq, flags);

	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "left_deadline",
			SPLIT_NS(बायाँ_deadline));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "left_vruntime",
			SPLIT_NS(बायाँ_vruntime));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "zero_vruntime",
			SPLIT_NS(zero_vruntime));
	SEQ_printf(m, "  .%-30s: %Ld (%d bits)\n", "sum_w_vruntime",
		   योग_w_vruntime, ilog2(असल(योग_w_vruntime)));
	SEQ_printf(m, "  .%-30s: %Lu\n", "sum_weight",
		   योग_weight);
	SEQ_printf(m, "  .%-30s: %u\n", "sum_shift", योग_shift);
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "avg_vruntime",
			SPLIT_NS(avruntime));
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", "right_vruntime",
			SPLIT_NS(दायाँ_vruntime));
	spread = दायाँ_vruntime - बायाँ_vruntime;
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
#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
	SEQ_printf(m, "  .%-30s: %lu\n", "tg_load_avg_contrib",
			cfs_rq->tg_load_avg_contrib);
	SEQ_printf(m, "  .%-30s: %ld\n", "tg_load_avg",
			atomic_long_read(&cfs_rq->tg->load_avg));
	SEQ_printf(m, "  .%-30s: %lu\n", "h_load",
			cfs_rq->h_load);
#पूर्ण_अगर /* CONFIG_FAIR_GROUP_SCHED */
#अगर_घोषित CONFIG_CFS_BANDWIDTH
	SEQ_printf(m, "  .%-30s: %d\n", "throttled",
			cfs_rq->throttled);
	SEQ_printf(m, "  .%-30s: %d\n", "throttle_count",
			cfs_rq->throttle_गिनती);
#पूर्ण_अगर

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
	print_cfs_group_stats(m, cpu, cfs_rq->tg);
#पूर्ण_अगर
}

व्योम print_rt_rq(काष्ठा seq_file *m, पूर्णांक cpu, काष्ठा rt_rq *rt_rq)
{
#अगर_घोषित CONFIG_RT_GROUP_SCHED
	SEQ_printf(m, "\n");
	SEQ_printf_task_group_path(m, rt_rq->tg, "rt_rq[%d]:%s\n", cpu);
#अन्यथा
	SEQ_printf(m, "\n");
	SEQ_printf(m, "rt_rq[%d]:\n", cpu);
#पूर्ण_अगर

#घोषणा P(x) \
	SEQ_printf(m, "  .%-30s: %Ld\n", #x, (दीर्घ दीर्घ)(rt_rq->x))
#घोषणा PU(x) \
	SEQ_printf(m, "  .%-30s: %lu\n", #x, (अचिन्हित दीर्घ)(rt_rq->x))
#घोषणा PN(x) \
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", #x, SPLIT_NS(rt_rq->x))

	PU(rt_nr_running);

#अगर_घोषित CONFIG_RT_GROUP_SCHED
	P(rt_throttled);
	PN(rt_time);
	PN(rt_runtime);
#पूर्ण_अगर

#अघोषित PN
#अघोषित PU
#अघोषित P
}

व्योम print_dl_rq(काष्ठा seq_file *m, पूर्णांक cpu, काष्ठा dl_rq *dl_rq)
{
	काष्ठा dl_bw *dl_bw;

	SEQ_printf(m, "\n");
	SEQ_printf(m, "dl_rq[%d]:\n", cpu);

#घोषणा PU(x) \
	SEQ_printf(m, "  .%-30s: %lu\n", #x, (अचिन्हित दीर्घ)(dl_rq->x))

	PU(dl_nr_running);
	dl_bw = &cpu_rq(cpu)->rd->dl_bw;
	SEQ_printf(m, "  .%-30s: %lld\n", "dl_bw->bw", dl_bw->bw);
	SEQ_printf(m, "  .%-30s: %lld\n", "dl_bw->total_bw", dl_bw->योग_bw);

#अघोषित PU
}

अटल व्योम print_cpu(काष्ठा seq_file *m, पूर्णांक cpu)
{
	काष्ठा rq *rq = cpu_rq(cpu);

#अगर_घोषित CONFIG_X86
	{
		अचिन्हित पूर्णांक freq = cpu_khz ? : 1;

		SEQ_printf(m, "cpu#%d, %u.%03u MHz\n",
			   cpu, freq / 1000, (freq % 1000));
	}
#अन्यथा /* !CONFIG_X86: */
	SEQ_printf(m, "cpu#%d\n", cpu);
#पूर्ण_अगर /* !CONFIG_X86 */

#घोषणा P(x)								\
करो {									\
	अगर (माप(rq->x) == 4)						\
		SEQ_printf(m, "  .%-30s: %d\n", #x, (पूर्णांक)(rq->x));	\
	अन्यथा								\
		SEQ_printf(m, "  .%-30s: %Ld\n", #x, (दीर्घ दीर्घ)(rq->x));\
} जबतक (0)

#घोषणा PN(x) \
	SEQ_printf(m, "  .%-30s: %Ld.%06ld\n", #x, SPLIT_NS(rq->x))

	P(nr_running);
	P(nr_switches);
	P(nr_uninterruptible);
	PN(अग्र_balance);
	SEQ_printf(m, "  .%-30s: %ld\n", "curr->pid", (दीर्घ)(task_pid_nr(rq->curr)));
	PN(घड़ी);
	PN(clock_task);
#अघोषित P
#अघोषित PN

#घोषणा P64(n) SEQ_printf(m, "  .%-30s: %Ld\n", #n, rq->n);
	P64(avg_idle);
	P64(max_idle_balance_cost);
#अघोषित P64

#घोषणा P(n) SEQ_printf(m, "  .%-30s: %d\n", #n, schedstat_val(rq->n));
	अगर (schedstat_enabled()) {
		P(yld_गिनती);
		P(sched_गिनती);
		P(sched_goidle);
		P(ttwu_गिनती);
		P(ttwu_local);
	}
#अघोषित P

	print_cfs_stats(m, cpu);
	print_rt_stats(m, cpu);
	print_dl_stats(m, cpu);

	print_rq(m, rq, cpu);
	SEQ_printf(m, "\n");
}

अटल स्थिर अक्षर *sched_tunable_scaling_names[] = {
	"none",
	"logarithmic",
	"linear"
};

अटल व्योम sched_debug_header(काष्ठा seq_file *m)
{
	u64 ktime, sched_clk, cpu_clk;
	अचिन्हित दीर्घ flags;

	local_irq_save(flags);
	ktime = ktime_to_ns(ktime_get());
	sched_clk = sched_clock();
	cpu_clk = local_clock();
	local_irq_restore(flags);

	SEQ_printf(m, "Sched Debug Version: v0.11, %s %.*s\n",
		init_utsname()->release,
		(पूर्णांक)म_खोज(init_utsname()->version, " "),
		init_utsname()->version);

#घोषणा P(x) \
	SEQ_printf(m, "%-40s: %Ld\n", #x, (दीर्घ दीर्घ)(x))
#घोषणा PN(x) \
	SEQ_printf(m, "%-40s: %Ld.%06ld\n", #x, SPLIT_NS(x))
	PN(ktime);
	PN(sched_clk);
	PN(cpu_clk);
	P(jiffies);
#अगर_घोषित CONFIG_HAVE_UNSTABLE_SCHED_CLOCK
	P(sched_clock_stable());
#पूर्ण_अगर
#अघोषित PN
#अघोषित P

	SEQ_printf(m, "\n");
	SEQ_printf(m, "sysctl_sched\n");

#घोषणा P(x) \
	SEQ_printf(m, "  .%-40s: %Ld\n", #x, (दीर्घ दीर्घ)(x))
#घोषणा PN(x) \
	SEQ_printf(m, "  .%-40s: %Ld.%06ld\n", #x, SPLIT_NS(x))
	PN(sysctl_sched_base_slice);
	P(sysctl_sched_features);
#अघोषित PN
#अघोषित P

	SEQ_printf(m, "  .%-40s: %d (%s)\n",
		"sysctl_sched_tunable_scaling",
		sysctl_sched_tunable_scaling,
		sched_tunable_scaling_names[sysctl_sched_tunable_scaling]);
	SEQ_printf(m, "\n");
}

अटल पूर्णांक sched_debug_show(काष्ठा seq_file *m, व्योम *v)
{
	पूर्णांक cpu = (अचिन्हित दीर्घ)(v - 2);

	अगर (cpu != -1)
		print_cpu(m, cpu);
	अन्यथा
		sched_debug_header(m);

	वापस 0;
}

व्योम sysrq_sched_debug_show(व्योम)
{
	पूर्णांक cpu;

	sched_debug_header(शून्य);
	for_each_online_cpu(cpu) {
		/*
		 * Need to reset softlockup watchdogs on all CPUs, because
		 * another CPU might be blocked waiting for us to process
		 * an IPI or stop_machine.
		 */
		touch_nmi_watchdog();
		touch_all_softlockup_watchdogs();
		print_cpu(शून्य, cpu);
	}
}

/*
 * This iterator needs some explanation.
 * It returns 1 for the header position.
 * This means 2 is CPU 0.
 * In a hotplugged system some CPUs, including CPU 0, may be missing so we have
 * to use cpumask_* to iterate over the CPUs.
 */
अटल व्योम *sched_debug_start(काष्ठा seq_file *file, loff_t *offset)
{
	अचिन्हित दीर्घ n = *offset;

	अगर (n == 0)
		वापस (व्योम *) 1;

	n--;

	अगर (n > 0)
		n = cpumask_अग्र(n - 1, cpu_online_mask);
	अन्यथा
		n = cpumask_first(cpu_online_mask);

	*offset = n + 1;

	अगर (n < nr_cpu_ids)
		वापस (व्योम *)(अचिन्हित दीर्घ)(n + 2);

	वापस शून्य;
}

अटल व्योम *sched_debug_अग्र(काष्ठा seq_file *file, व्योम *तथ्य, loff_t *offset)
{
	(*offset)++;
	वापस sched_debug_start(file, offset);
}

अटल व्योम sched_debug_stop(काष्ठा seq_file *file, व्योम *तथ्य)
{
}

अटल स्थिर काष्ठा seq_operations sched_debug_sops = {
	.start		= sched_debug_start,
	.अग्र		= sched_debug_अग्र,
	.stop		= sched_debug_stop,
	.show		= sched_debug_show,
};

#घोषणा __PS(S, F) SEQ_printf(m, "%-45s:%21Ld\n", S, (दीर्घ दीर्घ)(F))
#घोषणा __P(F) __PS(#F, F)
#घोषणा   P(F) __PS(#F, p->F)
#घोषणा   PM(F, M) __PS(#F, p->F & (M))
#घोषणा __PSN(S, F) SEQ_printf(m, "%-45s:%14Ld.%06ld\n", S, SPLIT_NS((दीर्घ दीर्घ)(F)))
#घोषणा __PN(F) __PSN(#F, F)
#घोषणा   PN(F) __PSN(#F, p->F)


#अगर_घोषित CONFIG_NUMA_BALANCING
व्योम print_numa_stats(काष्ठा seq_file *m, पूर्णांक गाँठ, अचिन्हित दीर्घ tsf,
		अचिन्हित दीर्घ tpf, अचिन्हित दीर्घ gsf, अचिन्हित दीर्घ gpf)
{
	SEQ_printf(m, "numa_faults node=%d ", गाँठ);
	SEQ_printf(m, "task_private=%lu task_shared=%lu ", tpf, tsf);
	SEQ_printf(m, "group_private=%lu group_shared=%lu\n", gpf, gsf);
}
#पूर्ण_अगर


अटल व्योम sched_show_numa(काष्ठा task_struct *p, काष्ठा seq_file *m)
{
#अगर_घोषित CONFIG_NUMA_BALANCING
	अगर (p->mm)
		P(mm->numa_scan_seq);

	P(numa_pages_migrated);
	P(numa_preferred_nid);
	P(योग_numa_faults);
	SEQ_printf(m, "current_node=%d, numa_group_id=%d\n",
			task_गाँठ(p), task_numa_group_id(p));
	show_numa_stats(p, m);
#पूर्ण_अगर /* CONFIG_NUMA_BALANCING */
}

व्योम proc_sched_show_task(काष्ठा task_struct *p, काष्ठा pid_namespace *ns,
						  काष्ठा seq_file *m)
{
	अचिन्हित दीर्घ nr_switches;

	SEQ_printf(m, "%s (%d, #threads: %d)\n", p->comm, task_pid_nr_ns(p, ns),
						get_nr_threads(p));
	SEQ_printf(m,
		"---------------------------------------------------------"
		"----------\n");

#घोषणा P_SCHEDSTAT(F)  __PS(#F, schedstat_val(p->stats.F))
#घोषणा PN_SCHEDSTAT(F) __PSN(#F, schedstat_val(p->stats.F))

	PN(se.exec_start);
	PN(se.vruntime);
	PN(se.योग_exec_runtime);

	nr_switches = p->nvcsw + p->nivcsw;

	P(se.nr_migrations);

	अगर (schedstat_enabled()) {
		u64 avg_atom, avg_per_cpu;

		PN_SCHEDSTAT(योग_sleep_runtime);
		PN_SCHEDSTAT(योग_block_runtime);
		PN_SCHEDSTAT(wait_start);
		PN_SCHEDSTAT(sleep_start);
		PN_SCHEDSTAT(block_start);
		PN_SCHEDSTAT(sleep_max);
		PN_SCHEDSTAT(block_max);
		PN_SCHEDSTAT(exec_max);
		PN_SCHEDSTAT(slice_max);
		PN_SCHEDSTAT(wait_max);
		PN_SCHEDSTAT(wait_योग);
		P_SCHEDSTAT(wait_गिनती);
		PN_SCHEDSTAT(iowait_योग);
		P_SCHEDSTAT(iowait_गिनती);
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
		अगर (nr_switches)
			avg_atom = div64_ul(avg_atom, nr_switches);
		अन्यथा
			avg_atom = -1LL;

		avg_per_cpu = p->se.योग_exec_runtime;
		अगर (p->se.nr_migrations) {
			avg_per_cpu = div64_u64(avg_per_cpu,
						p->se.nr_migrations);
		} अन्यथा {
			avg_per_cpu = -1LL;
		}

		__PN(avg_atom);
		__PN(avg_per_cpu);

#अगर_घोषित CONFIG_SCHED_CORE
		PN_SCHEDSTAT(core_forceidle_योग);
#पूर्ण_अगर
	}

	__P(nr_switches);
	__PS("nr_voluntary_switches", p->nvcsw);
	__PS("nr_involuntary_switches", p->nivcsw);

	P(se.load.weight);
	P(se.avg.load_योग);
	P(se.avg.runnable_योग);
	P(se.avg.util_योग);
	P(se.avg.load_avg);
	P(se.avg.runnable_avg);
	P(se.avg.util_avg);
	P(se.avg.last_update_time);
	PM(se.avg.util_est, ~UTIL_AVG_UNCHANGED);
#अगर_घोषित CONFIG_UCLAMP_TASK
	__PS("uclamp.min", p->uclamp_req[UCLAMP_MIN].मूल्य);
	__PS("uclamp.max", p->uclamp_req[UCLAMP_MAX].मूल्य);
	__PS("effective uclamp.min", uclamp_eff_मूल्य(p, UCLAMP_MIN));
	__PS("effective uclamp.max", uclamp_eff_मूल्य(p, UCLAMP_MAX));
#पूर्ण_अगर /* CONFIG_UCLAMP_TASK */
	P(policy);
	P(prio);
	अगर (task_has_dl_policy(p)) {
		P(dl.runtime);
		P(dl.deadline);
	} अन्यथा अगर (fair_policy(p->policy)) {
		P(se.slice);
	}
#अगर_घोषित CONFIG_SCHED_CLASS_EXT
	__PS("ext.enabled", task_on_scx(p));
#पूर्ण_अगर
#अघोषित PN_SCHEDSTAT
#अघोषित P_SCHEDSTAT

	{
		अचिन्हित पूर्णांक this_cpu = raw_smp_processor_id();
		u64 t0, t1;

		t0 = cpu_clock(this_cpu);
		t1 = cpu_clock(this_cpu);
		__PS("clock-delta", t1-t0);
	}

	sched_show_numa(p, m);
}

व्योम proc_sched_set_task(काष्ठा task_struct *p)
{
#अगर_घोषित CONFIG_SCHEDSTATS
	स_रखो(&p->stats, 0, माप(p->stats));
#पूर्ण_अगर
}

व्योम resched_latency_warn(पूर्णांक cpu, u64 latency)
{
	अटल DEFINE_RATELIMIT_STATE(latency_check_ratelimit, 60 * 60 * HZ, 1);

	अगर (likely(!__ratelimit(&latency_check_ratelimit)))
		वापस;

	pr_err("sched: CPU %d need_resched set for > %llu ns (%d ticks) without schedule\n",
	       cpu, latency, cpu_rq(cpu)->ticks_without_resched);
	dump_ढेर();
}
