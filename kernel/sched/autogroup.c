// SPDX-License-Identifier: GPL-2.0

/*
 * Auto-group scheduling implementation:
 */

#অন্তর্ভাব "autogroup.h"
#অন্তর্ভাব "sched.h"

unsigned পূর্ণসংখ্যা __read_mostly sysctl_sched_autogroup_enabled = 1;
স্থির কাঠামো autogroup autogroup_default;
স্থির atomic_t autogroup_seq_nr;

#ifdef CONFIG_SYSCTL
স্থির ধ্রুবক কাঠামো ctl_table sched_autogroup_sysctls[] = {
	{
		.procname       = "sched_autogroup_enabled",
		.তথ্য           = &sysctl_sched_autogroup_enabled,
		.maxlen         = আকার(unsigned পূর্ণসংখ্যা),
		.mode           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

স্থির শূন্য __init sched_autogroup_sysctl_init(শূন্য)
{
	register_sysctl_init("kernel", sched_autogroup_sysctls);
}
#অন্যথা /* !CONFIG_SYSCTL: */
#define sched_autogroup_sysctl_init() করো { } যখন (0)
#endif /* !CONFIG_SYSCTL */

শূন্য __init autogroup_init(কাঠামো task_struct *init_task)
{
	autogroup_default.tg = &মূল_task_group;
	kref_init(&autogroup_default.kref);
	init_rwsem(&autogroup_default.lock);
	init_task->সংকেত->autogroup = &autogroup_default;
	sched_autogroup_sysctl_init();
}

শূন্য autogroup_free(কাঠামো task_group *tg)
{
	kfree(tg->autogroup);
}

স্থির অন্তঃরেখ শূন্য autogroup_destroy(কাঠামো kref *kref)
{
	কাঠামো autogroup *ag = container_of(kref, কাঠামো autogroup, kref);

#ifdef CONFIG_RT_GROUP_SCHED
	/* We've redirected RT tasks to the root task group... */
	ag->tg->rt_se = ফাঁকা;
	ag->tg->rt_rq = ফাঁকা;
#endif
	sched_release_group(ag->tg);
	sched_destroy_group(ag->tg);
}

স্থির অন্তঃরেখ শূন্য autogroup_kref_put(কাঠামো autogroup *ag)
{
	kref_put(&ag->kref, autogroup_destroy);
}

স্থির অন্তঃরেখ কাঠামো autogroup *autogroup_kref_get(কাঠামো autogroup *ag)
{
	kref_get(&ag->kref);
	ফেরত ag;
}

স্থির অন্তঃরেখ কাঠামো autogroup *autogroup_task_get(কাঠামো task_struct *p)
{
	কাঠামো autogroup *ag;
	unsigned long flags;

	যদি (!lock_task_sighand(p, &flags))
		ফেরত autogroup_kref_get(&autogroup_default);

	ag = autogroup_kref_get(p->সংকেত->autogroup);
	unlock_task_sighand(p, &flags);

	ফেরত ag;
}

স্থির অন্তঃরেখ কাঠামো autogroup *autogroup_create(শূন্য)
{
	কাঠামো autogroup *ag = kzalloc_obj(*ag);
	কাঠামো task_group *tg;

	যদি (!ag)
		যাও out_fail;

	tg = sched_create_group(&মূল_task_group);
	যদি (IS_ERR(tg))
		যাও out_free;

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
	tg->rt_se = মূল_task_group.rt_se;
	tg->rt_rq = মূল_task_group.rt_rq;
#endif /* CONFIG_RT_GROUP_SCHED */
	tg->autogroup = ag;

	sched_online_group(tg, &মূল_task_group);
	ফেরত ag;

out_free:
	kfree(ag);
out_fail:
	যদি (printk_ratelimit()) {
		printk(KERN_WARNING "autogroup_create: %s failure.\n",
			ag ? "sched_create_group()" : "kzalloc()");
	}

	ফেরত autogroup_kref_get(&autogroup_default);
}

bool task_wants_autogroup(কাঠামো task_struct *p, কাঠামো task_group *tg)
{
	যদি (tg != &মূল_task_group)
		ফেরত false;
	/*
	 * If we race with autogroup_move_group() the caller can use the old
	 * value of signal->autogroup but in this case sched_move_task() will
	 * be called again before autogroup_kref_put().
	 *
	 * However, there is no way sched_autogroup_exit_task() could tell us
	 * to avoid autogroup->tg, so we abuse PF_EXITING flag for this case.
	 */
	যদি (p->flags & PF_EXITING)
		ফেরত false;

	ফেরত true;
}

শূন্য sched_autogroup_exit_task(কাঠামো task_struct *p)
{
	/*
	 * We are going to call exit_notify() and autogroup_move_group() can't
	 * see this thread after that: we can no longer use signal->autogroup.
	 * See the PF_EXITING check in task_wants_autogroup().
	 */
	sched_move_task(p, true);
}

স্থির শূন্য
autogroup_move_group(কাঠামো task_struct *p, কাঠামো autogroup *ag)
{
	কাঠামো autogroup *পূর্ব;
	কাঠামো task_struct *t;
	unsigned long flags;

	যদি (WARN_ON_ONCE(!lock_task_sighand(p, &flags)))
		ফেরত;

	পূর্ব = p->সংকেত->autogroup;
	যদি (পূর্ব == ag) {
		unlock_task_sighand(p, &flags);
		ফেরত;
	}

	p->সংকেত->autogroup = autogroup_kref_get(ag);
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
	autogroup_kref_put(পূর্ব);
}

/* Allocates GFP_KERNEL, cannot be called under any spinlock: */
শূন্য sched_autogroup_create_attach(কাঠামো task_struct *p)
{
	কাঠামো autogroup *ag = autogroup_create();

	autogroup_move_group(p, ag);

	/* Drop extra reference added by autogroup_create(): */
	autogroup_kref_put(ag);
}
EXPORT_প্রতীক(sched_autogroup_create_attach);

/* Cannot be called under siglock. Currently has no users: */
শূন্য sched_autogroup_detach(কাঠামো task_struct *p)
{
	autogroup_move_group(p, &autogroup_default);
}
EXPORT_প্রতীক(sched_autogroup_detach);

শূন্য sched_autogroup_fork(কাঠামো signal_struct *sig)
{
	sig->autogroup = autogroup_task_get(current);
}

শূন্য sched_autogroup_exit(কাঠামো signal_struct *sig)
{
	autogroup_kref_put(sig->autogroup);
}

স্থির পূর্ণসংখ্যা __init setup_autogroup(অক্ষর *str)
{
	sysctl_sched_autogroup_enabled = 0;

	ফেরত 1;
}
__setup("noautogroup", setup_autogroup);

#ifdef CONFIG_PROC_FS

পূর্ণসংখ্যা proc_sched_autogroup_set_nice(কাঠামো task_struct *p, পূর্ণসংখ্যা nice)
{
	স্থির unsigned long পরবর্তী = INITIAL_JIFFIES;
	কাঠামো autogroup *ag;
	unsigned long shares;
	পূর্ণসংখ্যা err, idx;

	যদি (nice < MIN_NICE || nice > MAX_NICE)
		ফেরত -EINVAL;

	err = security_task_setnice(current, nice);
	যদি (err)
		ফেরত err;

	যদি (nice < 0 && !can_nice(current, nice))
		ফেরত -EPERM;

	/* This is a heavy operation, taking global locks.. */
	যদি (!capable(CAP_SYS_ADMIN) && time_before(jiffies, পরবর্তী))
		ফেরত -EAGAIN;

	পরবর্তী = HZ / 10 + jiffies;
	ag = autogroup_task_get(p);

	idx = array_সূচক_nospec(nice + 20, 40);
	shares = scale_load(sched_prio_to_weight[idx]);

	down_write(&ag->lock);
	err = sched_group_set_shares(ag->tg, shares);
	যদি (!err)
		ag->nice = nice;
	up_write(&ag->lock);

	autogroup_kref_put(ag);

	ফেরত err;
}

শূন্য proc_sched_autogroup_show_task(কাঠামো task_struct *p, কাঠামো seq_file *m)
{
	কাঠামো autogroup *ag = autogroup_task_get(p);

	যদি (!task_group_is_autogroup(ag->tg))
		যাও out;

	down_read(&ag->lock);
	seq_printf(m, "/autogroup-%ld nice %d\n", ag->id, ag->nice);
	up_read(&ag->lock);

out:
	autogroup_kref_put(ag);
}
#endif /* CONFIG_PROC_FS */

পূর্ণসংখ্যা autogroup_path(কাঠামো task_group *tg, অক্ষর *buf, পূর্ণসংখ্যা buflen)
{
	যদি (!task_group_is_autogroup(tg))
		ফেরত 0;

	ফেরত snprintf(buf, buflen, "%s-%ld", "/autogroup", tg->autogroup->id);
}
