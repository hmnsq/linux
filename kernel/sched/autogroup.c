// SPDX-License-Identifier: GPL-2.0

/*
 * Auto-group scheduling implementation:
 */

#अन्तर्भाव "autogroup.h"
#अन्तर्भाव "sched.h"

अचिन्हित पूर्णांक __read_mostly sysctl_sched_autogroup_enabled = 1;
स्थिर संरचना autogroup autogroup_default;
स्थिर atomic_t autogroup_seq_nr;

#ifdef CONFIG_SYSCTL
स्थिर स्थिरांक संरचना ctl_table sched_autogroup_sysctls[] = {
	{
		.procname       = "sched_autogroup_enabled",
		.data           = &sysctl_sched_autogroup_enabled,
		.maxlen         = आकार(अचिन्हित पूर्णांक),
		.mode           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

स्थिर शून्य __init sched_autogroup_sysctl_init(शून्य)
{
	register_sysctl_init("kernel", sched_autogroup_sysctls);
}
#अन्यथा /* !CONFIG_SYSCTL: */
#define sched_autogroup_sysctl_init() करा { } पर्यंत (0)
#endif /* !CONFIG_SYSCTL */

शून्य __init autogroup_init(संरचना task_struct *init_task)
{
	autogroup_default.tg = &मूळ_task_group;
	kref_init(&autogroup_default.kref);
	init_rwsem(&autogroup_default.lock);
	init_task->संकेत->autogroup = &autogroup_default;
	sched_autogroup_sysctl_init();
}

शून्य autogroup_free(संरचना task_group *tg)
{
	kfree(tg->autogroup);
}

स्थिर ओळीत शून्य autogroup_destroy(संरचना kref *kref)
{
	संरचना autogroup *ag = container_of(kref, संरचना autogroup, kref);

#ifdef CONFIG_RT_GROUP_SCHED
	/* We've redirected RT tasks to the root task group... */
	ag->tg->rt_se = रिक्त;
	ag->tg->rt_rq = रिक्त;
#endif
	sched_release_group(ag->tg);
	sched_destroy_group(ag->tg);
}

स्थिर ओळीत शून्य autogroup_kref_put(संरचना autogroup *ag)
{
	kref_put(&ag->kref, autogroup_destroy);
}

स्थिर ओळीत संरचना autogroup *autogroup_kref_get(संरचना autogroup *ag)
{
	kref_get(&ag->kref);
	परत ag;
}

स्थिर ओळीत संरचना autogroup *autogroup_task_get(संरचना task_struct *p)
{
	संरचना autogroup *ag;
	अचिन्हित दीर्घ flags;

	जर (!lock_task_sighand(p, &flags))
		परत autogroup_kref_get(&autogroup_default);

	ag = autogroup_kref_get(p->संकेत->autogroup);
	unlock_task_sighand(p, &flags);

	परत ag;
}

स्थिर ओळीत संरचना autogroup *autogroup_create(शून्य)
{
	संरचना autogroup *ag = kzalloc_obj(*ag);
	संरचना task_group *tg;

	जर (!ag)
		जा out_fail;

	tg = sched_create_group(&मूळ_task_group);
	जर (IS_ERR(tg))
		जा out_free;

	kref_init(&ag->kref);
	init_rwsem(&ag->lock);
	ag->id = atomic_inc_return(&autogroup_seq_nr);
	ag->tg = tg;
#ifdef CONFIG_RT_GROUP_SCHED
	/*
	 * Autogroup RT tasks are redirected to the root task group
	 * so we don't have to move tasks around upon policy change,
	 * or flail around trying to allocate bandwidth on the fly.
	 * A bandwidth exception in __sched_setscheduler() allows
	 * the policy change to proceed.
	 */
	free_rt_sched_group(tg);
	tg->rt_se = मूळ_task_group.rt_se;
	tg->rt_rq = मूळ_task_group.rt_rq;
#endif /* CONFIG_RT_GROUP_SCHED */
	tg->autogroup = ag;

	sched_online_group(tg, &मूळ_task_group);
	परत ag;

out_free:
	kfree(ag);
out_fail:
	जर (printk_ratelimit()) {
		printk(KERN_WARNING "autogroup_create: %s failure.\n",
			ag ? "sched_create_group()" : "kzalloc()");
	}

	परत autogroup_kref_get(&autogroup_default);
}

bool task_wants_autogroup(संरचना task_struct *p, संरचना task_group *tg)
{
	जर (tg != &मूळ_task_group)
		परत false;
	/*
	 * If we race with autogroup_move_group() the caller can use the old
	 * value of signal->autogroup but in this case sched_move_task() will
	 * be called again before autogroup_kref_put().
	 *
	 * However, there is no way sched_autogroup_exit_task() could tell us
	 * to avoid autogroup->tg, so we abuse PF_EXITING flag for this case.
	 */
	जर (p->flags & PF_EXITING)
		परत false;

	परत true;
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
autogroup_move_group(संरचना task_struct *p, संरचना autogroup *ag)
{
	संरचना autogroup *prev;
	संरचना task_struct *t;
	अचिन्हित दीर्घ flags;

	जर (WARN_ON_ONCE(!lock_task_sighand(p, &flags)))
		परत;

	prev = p->संकेत->autogroup;
	जर (prev == ag) {
		unlock_task_sighand(p, &flags);
		परत;
	}

	p->संकेत->autogroup = autogroup_kref_get(ag);
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
	autogroup_kref_put(prev);
}

/* Allocates GFP_KERNEL, cannot be called under any spinlock: */
शून्य sched_autogroup_create_attach(संरचना task_struct *p)
{
	संरचना autogroup *ag = autogroup_create();

	autogroup_move_group(p, ag);

	/* Drop extra reference added by autogroup_create(): */
	autogroup_kref_put(ag);
}
EXPORT_चिन्ह(sched_autogroup_create_attach);

/* Cannot be called under siglock. Currently has no users: */
शून्य sched_autogroup_detach(संरचना task_struct *p)
{
	autogroup_move_group(p, &autogroup_default);
}
EXPORT_चिन्ह(sched_autogroup_detach);

शून्य sched_autogroup_fork(संरचना signal_struct *sig)
{
	sig->autogroup = autogroup_task_get(current);
}

शून्य sched_autogroup_exit(संरचना signal_struct *sig)
{
	autogroup_kref_put(sig->autogroup);
}

स्थिर पूर्णांक __init setup_autogroup(अक्षर *str)
{
	sysctl_sched_autogroup_enabled = 0;

	परत 1;
}
__setup("noautogroup", setup_autogroup);

#ifdef CONFIG_PROC_FS

पूर्णांक proc_sched_autogroup_set_nice(संरचना task_struct *p, पूर्णांक nice)
{
	स्थिर अचिन्हित दीर्घ next = INITIAL_JIFFIES;
	संरचना autogroup *ag;
	अचिन्हित दीर्घ shares;
	पूर्णांक err, idx;

	जर (nice < MIN_NICE || nice > MAX_NICE)
		परत -EINVAL;

	err = security_task_setnice(current, nice);
	जर (err)
		परत err;

	जर (nice < 0 && !can_nice(current, nice))
		परत -EPERM;

	/* This is a heavy operation, taking global locks.. */
	जर (!capable(CAP_SYS_ADMIN) && time_before(jiffies, next))
		परत -EAGAIN;

	next = HZ / 10 + jiffies;
	ag = autogroup_task_get(p);

	idx = array_index_nospec(nice + 20, 40);
	shares = scale_load(sched_prio_to_weight[idx]);

	down_write(&ag->lock);
	err = sched_group_set_shares(ag->tg, shares);
	जर (!err)
		ag->nice = nice;
	up_write(&ag->lock);

	autogroup_kref_put(ag);

	परत err;
}

शून्य proc_sched_autogroup_show_task(संरचना task_struct *p, संरचना seq_file *m)
{
	संरचना autogroup *ag = autogroup_task_get(p);

	जर (!task_group_is_autogroup(ag->tg))
		जा out;

	down_read(&ag->lock);
	seq_printf(m, "/autogroup-%ld nice %d\n", ag->id, ag->nice);
	up_read(&ag->lock);

out:
	autogroup_kref_put(ag);
}
#endif /* CONFIG_PROC_FS */

पूर्णांक autogroup_path(संरचना task_group *tg, अक्षर *buf, पूर्णांक buflen)
{
	जर (!task_group_is_autogroup(tg))
		परत 0;

	परत snprintf(buf, buflen, "%s-%ld", "/autogroup", tg->autogroup->id);
}
