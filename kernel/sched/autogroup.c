// SPDX-License-Identifier: GPL-2.0

/*
 * Auto-group scheduling implementation:
 */

#شامل "autogroup.h"
#شامل "sched.h"

unsigned صحیح __read_mostly sysctl_sched_autogroup_enabled = 1;
ایستا ساختار autogroup autogroup_default;
ایستا atomic_t autogroup_seq_nr;

#ifdef CONFIG_SYSCTL
ایستا ثابت ساختار ctl_table sched_autogroup_sysctls[] = {
	{
		.procname       = "sched_autogroup_enabled",
		.data           = &sysctl_sched_autogroup_enabled,
		.maxlen         = اندازه(unsigned صحیح),
		.mode           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

ایستا تهی __init sched_autogroup_sysctl_init(تهی)
{
	register_sysctl_init("kernel", sched_autogroup_sysctls);
}
#وگرنه /* !CONFIG_SYSCTL: */
#define sched_autogroup_sysctl_init() انجامبده { } وقتی (0)
#endif /* !CONFIG_SYSCTL */

تهی __init autogroup_init(ساختار task_struct *init_task)
{
	autogroup_default.tg = &ریشه_task_group;
	kref_init(&autogroup_default.kref);
	init_rwsem(&autogroup_default.lock);
	init_task->سیگنال->autogroup = &autogroup_default;
	sched_autogroup_sysctl_init();
}

تهی autogroup_free(ساختار task_group *tg)
{
	kfree(tg->autogroup);
}

ایستا درونخطی تهی autogroup_destroy(ساختار kref *kref)
{
	ساختار autogroup *ag = container_of(kref, ساختار autogroup, kref);

#ifdef CONFIG_RT_GROUP_SCHED
	/* We've redirected RT tasks to the root task group... */
	ag->tg->rt_se = پوچ;
	ag->tg->rt_rq = پوچ;
#endif
	sched_release_group(ag->tg);
	sched_destroy_group(ag->tg);
}

ایستا درونخطی تهی autogroup_kref_put(ساختار autogroup *ag)
{
	kref_put(&ag->kref, autogroup_destroy);
}

ایستا درونخطی ساختار autogroup *autogroup_kref_get(ساختار autogroup *ag)
{
	kref_get(&ag->kref);
	بازگشت ag;
}

ایستا درونخطی ساختار autogroup *autogroup_task_get(ساختار task_struct *p)
{
	ساختار autogroup *ag;
	unsigned long flags;

	اگر (!lock_task_sighand(p, &flags))
		بازگشت autogroup_kref_get(&autogroup_default);

	ag = autogroup_kref_get(p->سیگنال->autogroup);
	unlock_task_sighand(p, &flags);

	بازگشت ag;
}

ایستا درونخطی ساختار autogroup *autogroup_create(تهی)
{
	ساختار autogroup *ag = kzalloc_obj(*ag);
	ساختار task_group *tg;

	اگر (!ag)
		برو out_fail;

	tg = sched_create_group(&ریشه_task_group);
	اگر (IS_ERR(tg))
		برو out_free;

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
	tg->rt_se = ریشه_task_group.rt_se;
	tg->rt_rq = ریشه_task_group.rt_rq;
#endif /* CONFIG_RT_GROUP_SCHED */
	tg->autogroup = ag;

	sched_online_group(tg, &ریشه_task_group);
	بازگشت ag;

out_free:
	kfree(ag);
out_fail:
	اگر (printk_ratelimit()) {
		printk(KERN_WARNING "autogroup_create: %s failure.\n",
			ag ? "sched_create_group()" : "kzalloc()");
	}

	بازگشت autogroup_kref_get(&autogroup_default);
}

bool task_wants_autogroup(ساختار task_struct *p, ساختار task_group *tg)
{
	اگر (tg != &ریشه_task_group)
		بازگشت false;
	/*
	 * If we race with autogroup_move_group() the caller can use the old
	 * value of signal->autogroup but in this case sched_move_task() will
	 * be called again before autogroup_kref_put().
	 *
	 * However, there is no way sched_autogroup_exit_task() could tell us
	 * to avoid autogroup->tg, so we abuse PF_EXITING flag for this case.
	 */
	اگر (p->flags & PF_EXITING)
		بازگشت false;

	بازگشت true;
}

تهی sched_autogroup_exit_task(ساختار task_struct *p)
{
	/*
	 * We are going to call exit_notify() and autogroup_move_group() can't
	 * see this thread after that: we can no longer use signal->autogroup.
	 * See the PF_EXITING check in task_wants_autogroup().
	 */
	sched_move_task(p, true);
}

ایستا تهی
autogroup_move_group(ساختار task_struct *p, ساختار autogroup *ag)
{
	ساختار autogroup *prev;
	ساختار task_struct *t;
	unsigned long flags;

	اگر (WARN_ON_ONCE(!lock_task_sighand(p, &flags)))
		بازگشت;

	prev = p->سیگنال->autogroup;
	اگر (prev == ag) {
		unlock_task_sighand(p, &flags);
		بازگشت;
	}

	p->سیگنال->autogroup = autogroup_kref_get(ag);
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
تهی sched_autogroup_create_attach(ساختار task_struct *p)
{
	ساختار autogroup *ag = autogroup_create();

	autogroup_move_group(p, ag);

	/* Drop extra reference added by autogroup_create(): */
	autogroup_kref_put(ag);
}
EXPORT_نماد(sched_autogroup_create_attach);

/* Cannot be called under siglock. Currently has no users: */
تهی sched_autogroup_detach(ساختار task_struct *p)
{
	autogroup_move_group(p, &autogroup_default);
}
EXPORT_نماد(sched_autogroup_detach);

تهی sched_autogroup_fork(ساختار signal_struct *sig)
{
	sig->autogroup = autogroup_task_get(current);
}

تهی sched_autogroup_exit(ساختار signal_struct *sig)
{
	autogroup_kref_put(sig->autogroup);
}

ایستا صحیح __init setup_autogroup(نویسه *str)
{
	sysctl_sched_autogroup_enabled = 0;

	بازگشت 1;
}
__setup("noautogroup", setup_autogroup);

#ifdef CONFIG_PROC_FS

صحیح proc_sched_autogroup_set_nice(ساختار task_struct *p, صحیح nice)
{
	ایستا unsigned long next = INITIAL_JIFFIES;
	ساختار autogroup *ag;
	unsigned long shares;
	صحیح err, idx;

	اگر (nice < MIN_NICE || nice > MAX_NICE)
		بازگشت -EINVAL;

	err = security_task_setnice(current, nice);
	اگر (err)
		بازگشت err;

	اگر (nice < 0 && !can_nice(current, nice))
		بازگشت -EPERM;

	/* This is a heavy operation, taking global locks.. */
	اگر (!capable(CAP_SYS_ADMIN) && time_before(jiffies, next))
		بازگشت -EAGAIN;

	next = HZ / 10 + jiffies;
	ag = autogroup_task_get(p);

	idx = array_index_nospec(nice + 20, 40);
	shares = scale_load(sched_prio_to_weight[idx]);

	down_write(&ag->lock);
	err = sched_group_set_shares(ag->tg, shares);
	اگر (!err)
		ag->nice = nice;
	up_write(&ag->lock);

	autogroup_kref_put(ag);

	بازگشت err;
}

تهی proc_sched_autogroup_show_task(ساختار task_struct *p, ساختار seq_file *m)
{
	ساختار autogroup *ag = autogroup_task_get(p);

	اگر (!task_group_is_autogroup(ag->tg))
		برو out;

	down_read(&ag->lock);
	seq_printf(m, "/autogroup-%ld nice %d\n", ag->id, ag->nice);
	up_read(&ag->lock);

out:
	autogroup_kref_put(ag);
}
#endif /* CONFIG_PROC_FS */

صحیح autogroup_path(ساختار task_group *tg, نویسه *buf, صحیح buflen)
{
	اگر (!task_group_is_autogroup(tg))
		بازگشت 0;

	بازگشت snprintf(buf, buflen, "%s-%ld", "/autogroup", tg->autogroup->id);
}
