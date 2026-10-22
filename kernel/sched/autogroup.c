// SPDX-License-Identifier: GPL-2.0

/*
 * Auto-group scheduling implementation:
 */

#अन्तर्भाव "autogroup.h"
#अन्तर्भाव "sched.h"

अचिह्नित पूर्णाङ्क __read_mostly sysctl_sched_autogroup_enabled = 1;
स्थिर संरचना autogroup autogroup_default;
स्थिर atomic_t autogroup_seq_nr;

#यदि_परिभाषितम् CONFIG_SYSCTL
स्थिर ध्रुव संरचना ctl_सारणी sched_autogroup_sysctls[] = {
	{
		.procname       = "sched_autogroup_enabled",
		.दत्त           = &sysctl_sched_autogroup_enabled,
		.maxlen         = परिमाण(अचिह्नित पूर्णाङ्क),
		.बहुलकम्           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

स्थिर शून्य __init sched_autogroup_sysctl_init(शून्य)
{
	पञ्जिकाकोशः_sysctl_init("kernel", sched_autogroup_sysctls);
}
#अन्यथा /* !CONFIG_SYSCTL: */
#परिभाषय sched_autogroup_sysctl_init() कुरु { } यावत् (0)
#यदि_अन्तः /* !CONFIG_SYSCTL */

शून्य __init autogroup_init(संरचना task_struct *init_task)
{
	autogroup_default.tg = &मूलम्_task_गणः;
	kref_init(&autogroup_default.kref);
	init_rwsem(&autogroup_default.तालकम्);
	init_task->सङ्केत->autogroup = &autogroup_default;
	sched_autogroup_sysctl_init();
}

शून्य autogroup_free(संरचना task_गणः *tg)
{
	kfree(tg->autogroup);
}

स्थिर अन्तःस्थ शून्य autogroup_destroy(संरचना kref *kref)
{
	संरचना autogroup *ag = container_सम्बन्धि(kref, संरचना autogroup, kref);

#यदि_परिभाषितम् CONFIG_RT_गणः_SCHED
	/* We've redirected RT tasks to the root task group... */
	ag->tg->rt_se = रिक्त;
	ag->tg->rt_rq = रिक्त;
#यदि_अन्तः
	sched_release_गणः(ag->tg);
	sched_destroy_गणः(ag->tg);
}

स्थिर अन्तःस्थ शून्य autogroup_kref_put(संरचना autogroup *ag)
{
	kref_put(&ag->kref, autogroup_destroy);
}

स्थिर अन्तःस्थ संरचना autogroup *autogroup_kref_get(संरचना autogroup *ag)
{
	kref_get(&ag->kref);
	प्रत्यागम ag;
}

स्थिर अन्तःस्थ संरचना autogroup *autogroup_task_get(संरचना task_struct *p)
{
	संरचना autogroup *ag;
	अचिह्नित दीर्घ flags;

	यदि (!तालकम्_task_sighand(p, &flags))
		प्रत्यागम autogroup_kref_get(&autogroup_default);

	ag = autogroup_kref_get(p->सङ्केत->autogroup);
	unlock_task_sighand(p, &flags);

	प्रत्यागम ag;
}

स्थिर अन्तःस्थ संरचना autogroup *autogroup_create(शून्य)
{
	संरचना autogroup *ag = kzalloc_obj(*ag);
	संरचना task_गणः *tg;

	यदि (!ag)
		गच्छ बहिर्गमनम्_fail;

	tg = sched_create_गणः(&मूलम्_task_गणः);
	यदि (IS_ERR(tg))
		गच्छ बहिर्गमनम्_free;

	kref_init(&ag->kref);
	init_rwsem(&ag->तालकम्);
	ag->id = atomic_inc_return(&autogroup_seq_nr);
	ag->tg = tg;
#यदि_परिभाषितम् CONFIG_RT_गणः_SCHED
	/*
	 * Autogroup RT tasks are redirected to the root task group
	 * so we don't have to move tasks around upon policy change,
	 * or flail around trying to allocate bandwidth on the fly.
	 * A bandwidth exception in __sched_setscheduler() allows
	 * the policy change to proceed.
	 */
	free_rt_sched_गणः(tg);
	tg->rt_se = मूलम्_task_गणः.rt_se;
	tg->rt_rq = मूलम्_task_गणः.rt_rq;
#यदि_अन्तः /* CONFIG_RT_GROUP_SCHED */
	tg->autogroup = ag;

	sched_online_गणः(tg, &मूलम्_task_गणः);
	प्रत्यागम ag;

बहिर्गमनम्_free:
	kfree(ag);
बहिर्गमनम्_fail:
	यदि (printk_ratelimit()) {
		printk(KERN_WARNING "autogroup_create: %s failure.\n",
			ag ? "sched_create_group()" : "kzalloc()");
	}

	प्रत्यागम autogroup_kref_get(&autogroup_default);
}

bool task_इच्छति_autogroup(संरचना task_struct *p, संरचना task_गणः *tg)
{
	यदि (tg != &मूलम्_task_गणः)
		प्रत्यागम false;
	/*
	 * If we race with autogroup_move_group() the caller can use the old
	 * value of signal->autogroup but in this case sched_move_task() will
	 * be called again before autogroup_kref_put().
	 *
	 * However, there is no way sched_autogroup_exit_task() could tell us
	 * to avoid autogroup->tg, so we abuse PF_EXITING flag for this case.
	 */
	यदि (p->flags & PF_EXITING)
		प्रत्यागम false;

	प्रत्यागम true;
}

शून्य sched_autogroup_exit_task(संरचना task_struct *p)
{
	/*
	 * We are going to call exit_notify() and autogroup_move_group() can't
	 * see this thread after that: we can no longer use signal->autogroup.
	 * See the PF_EXITING check in task_wants_autogroup().
	 */
	sched_move_task(p, true);
}

स्थिर शून्य
autogroup_move_गणः(संरचना task_struct *p, संरचना autogroup *ag)
{
	संरचना autogroup *पूर्व;
	संरचना task_struct *t;
	अचिह्नित दीर्घ flags;

	यदि (WARN_ON_ONCE(!तालकम्_task_sighand(p, &flags)))
		प्रत्यागम;

	पूर्व = p->सङ्केत->autogroup;
	यदि (पूर्व == ag) {
		unlock_task_sighand(p, &flags);
		प्रत्यागम;
	}

	p->सङ्केत->autogroup = autogroup_kref_get(ag);
	/*
	 * We can't avoid sched_move_task() after we changed signal->autogroup,
	 * this process can already run with task_group() == prev->tg or we can
	 * race with cgroup code which can read autogroup = prev under rq->lock.
	 * In the latter case for_each_thread() can not miss a migrating thread,
	 * cpu_cgroup_attach() must not be possible after cgroup_task_exit()
	 * and it can't be removed from thread list, we hold ->siglock.
	 *
	 * If an exiting thread was already removed from thread list we rely on
	 * sched_autogroup_exit_task().
	 */
	for_each_thread(p, t)
		sched_move_task(t, true);

	unlock_task_sighand(p, &flags);
	autogroup_kref_put(पूर्व);
}

/* Allocates GFP_KERNEL, cannot be called under any spinlock: */
शून्य sched_autogroup_create_attach(संरचना task_struct *p)
{
	संरचना autogroup *ag = autogroup_create();

	autogroup_move_गणः(p, ag);

	/* Drop extra reference added by autogroup_create(): */
	autogroup_kref_put(ag);
}
EXPORT_चिह्नम्(sched_autogroup_create_attach);

/* Cannot be called under siglock. Currently has no users: */
शून्य sched_autogroup_detach(संरचना task_struct *p)
{
	autogroup_move_गणः(p, &autogroup_default);
}
EXPORT_चिह्नम्(sched_autogroup_detach);

शून्य sched_autogroup_fork(संरचना signal_struct *sig)
{
	sig->autogroup = autogroup_task_get(current);
}

शून्य sched_autogroup_exit(संरचना signal_struct *sig)
{
	autogroup_kref_put(sig->autogroup);
}

स्थिर पूर्णाङ्क __init सज्जता_autogroup(वर्ण *str)
{
	sysctl_sched_autogroup_enabled = 0;

	प्रत्यागम 1;
}
__setup("noautogroup", सज्जता_autogroup);

#यदि_परिभाषितम् CONFIG_PROC_FS

पूर्णाङ्क proc_sched_autogroup_समुच्चयः_nice(संरचना task_struct *p, पूर्णाङ्क nice)
{
	स्थिर अचिह्नित दीर्घ अग्र = INITIAL_JIFFIES;
	संरचना autogroup *ag;
	अचिह्नित दीर्घ shares;
	पूर्णाङ्क err, idx;

	यदि (nice < MIN_NICE || nice > MAX_NICE)
		प्रत्यागम -EINVAL;

	err = security_task_setnice(current, nice);
	यदि (err)
		प्रत्यागम err;

	यदि (nice < 0 && !can_nice(current, nice))
		प्रत्यागम -EPERM;

	/* This is a heavy operation, taking global locks.. */
	यदि (!capable(CAP_SYS_ADMIN) && time_before(jiffies, अग्र))
		प्रत्यागम -EAGAIN;

	अग्र = HZ / 10 + jiffies;
	ag = autogroup_task_get(p);

	idx = array_सूचकांक_nospec(nice + 20, 40);
	shares = scale_आरोपय(sched_prio_गन्तव्यम्_weight[idx]);

	अधस्तात्_write(&ag->तालकम्);
	err = sched_गणः_समुच्चयः_shares(ag->tg, shares);
	यदि (!err)
		ag->nice = nice;
	ऊर्ध्वम्_write(&ag->तालकम्);

	autogroup_kref_put(ag);

	प्रत्यागम err;
}

शून्य proc_sched_autogroup_show_task(संरचना task_struct *p, संरचना seq_file *m)
{
	संरचना autogroup *ag = autogroup_task_get(p);

	यदि (!task_गणः_is_autogroup(ag->tg))
		गच्छ बहिर्गमनम्;

	अधस्तात्_read(&ag->तालकम्);
	seq_printf(m, "/autogroup-%ld nice %d\n", ag->id, ag->nice);
	ऊर्ध्वम्_read(&ag->तालकम्);

बहिर्गमनम्:
	autogroup_kref_put(ag);
}
#यदि_अन्तः /* CONFIG_PROC_FS */

पूर्णाङ्क autogroup_मार्गः(संरचना task_गणः *tg, वर्ण *buf, पूर्णाङ्क buflen)
{
	यदि (!task_गणः_is_autogroup(tg))
		प्रत्यागम 0;

	प्रत्यागम snprintf(buf, buflen, "%s-%ld", "/autogroup", tg->autogroup->id);
}
