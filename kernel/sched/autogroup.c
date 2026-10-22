// SPDX-License-Identifier: GPL-2.0

/*
 * Auto-group scheduling implementation:
 */

#অন্তর্ভাব "autogroup.h"
#অন্তর্ভাব "sched.h"

unsigned পূর্ণাঙ্ক __read_mostly sysctl_sched_autogroup_enabled = 1;
স্থির সংরচনা autogroup autogroup_default;
স্থির atomic_t autogroup_seq_nr;

#ifdef CONFIG_SYSCTL
স্থির ধ্রুব সংরচনা ctl_table sched_autogroup_sysctls[] = {
	{
		.procname       = "sched_autogroup_enabled",
		.data           = &sysctl_sched_autogroup_enabled,
		.maxlen         = পরিমাণ(unsigned পূর্ণাঙ্ক),
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
#define sched_autogroup_sysctl_init() কুরু { } যাবত্ (0)
#endif /* !CONFIG_SYSCTL */

শূন্য __init autogroup_init(সংরচনা task_struct *init_task)
{
	autogroup_default.tg = &মূল_task_group;
	kref_init(&autogroup_default.kref);
	init_rwsem(&autogroup_default.lock);
	init_task->সঙ্কেত->autogroup = &autogroup_default;
	sched_autogroup_sysctl_init();
}

শূন্য autogroup_free(সংরচনা task_group *tg)
{
	kfree(tg->autogroup);
}

স্থির অন্তঃস্থ শূন্য autogroup_destroy(সংরচনা kref *kref)
{
	সংরচনা autogroup *ag = container_of(kref, সংরচনা autogroup, kref);

#ifdef CONFIG_RT_GROUP_SCHED
	/* We've redirected RT tasks to the root task group... */
	ag->tg->rt_se = রিক্ত;
	ag->tg->rt_rq = রিক্ত;
#endif
	sched_release_group(ag->tg);
	sched_destroy_group(ag->tg);
}

স্থির অন্তঃস্থ শূন্য autogroup_kref_put(সংরচনা autogroup *ag)
{
	kref_put(&ag->kref, autogroup_destroy);
}

স্থির অন্তঃস্থ সংরচনা autogroup *autogroup_kref_get(সংরচনা autogroup *ag)
{
	kref_get(&ag->kref);
	প্রত্যাগম ag;
}

স্থির অন্তঃস্থ সংরচনা autogroup *autogroup_task_get(সংরচনা task_struct *p)
{
	সংরচনা autogroup *ag;
	unsigned long flags;

	যদি (!lock_task_sighand(p, &flags))
		প্রত্যাগম autogroup_kref_get(&autogroup_default);

	ag = autogroup_kref_get(p->সঙ্কেত->autogroup);
	unlock_task_sighand(p, &flags);

	প্রত্যাগম ag;
}

স্থির অন্তঃস্থ সংরচনা autogroup *autogroup_create(শূন্য)
{
	সংরচনা autogroup *ag = kzalloc_obj(*ag);
	সংরচনা task_group *tg;

	যদি (!ag)
		গচ্ছ out_fail;

	tg = sched_create_group(&মূল_task_group);
	যদি (IS_ERR(tg))
		গচ্ছ out_free;

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
	প্রত্যাগম ag;

out_free:
	kfree(ag);
out_fail:
	যদি (printk_ratelimit()) {
		printk(KERN_WARNING "autogroup_create: %s failure.\n",
			ag ? "sched_create_group()" : "kzalloc()");
	}

	প্রত্যাগম autogroup_kref_get(&autogroup_default);
}

bool task_wants_autogroup(সংরচনা task_struct *p, সংরচনা task_group *tg)
{
	যদি (tg != &মূল_task_group)
		প্রত্যাগম false;
	/*
	 * If we race with autogroup_move_group() the caller can use the old
	 * value of signal->autogroup but in this case sched_move_task() will
	 * be called again before autogroup_kref_put().
	 *
	 * However, there is no way sched_autogroup_exit_task() could tell us
	 * to avoid autogroup->tg, so we abuse PF_EXITING flag for this case.
	 */
	যদি (p->flags & PF_EXITING)
		প্রত্যাগম false;

	প্রত্যাগম true;
}

শূন্য sched_autogroup_exit_task(সংরচনা task_struct *p)
{
	/*
	 * We are going to call exit_notify() and autogroup_move_group() can't
	 * see this thread after that: we can no longer use signal->autogroup.
	 * See the PF_EXITING check in task_wants_autogroup().
	 */
	sched_move_task(p, true);
}

স্থির শূন্য
autogroup_move_group(সংরচনা task_struct *p, সংরচনা autogroup *ag)
{
	সংরচনা autogroup *prev;
	সংরচনা task_struct *t;
	unsigned long flags;

	যদি (WARN_ON_ONCE(!lock_task_sighand(p, &flags)))
		প্রত্যাগম;

	prev = p->সঙ্কেত->autogroup;
	যদি (prev == ag) {
		unlock_task_sighand(p, &flags);
		প্রত্যাগম;
	}

	p->সঙ্কেত->autogroup = autogroup_kref_get(ag);
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
শূন্য sched_autogroup_create_attach(সংরচনা task_struct *p)
{
	সংরচনা autogroup *ag = autogroup_create();

	autogroup_move_group(p, ag);

	/* Drop extra reference added by autogroup_create(): */
	autogroup_kref_put(ag);
}
EXPORT_প্রতীক(sched_autogroup_create_attach);

/* Cannot be called under siglock. Currently has no users: */
শূন্য sched_autogroup_detach(সংরচনা task_struct *p)
{
	autogroup_move_group(p, &autogroup_default);
}
EXPORT_প্রতীক(sched_autogroup_detach);

শূন্য sched_autogroup_fork(সংরচনা signal_struct *sig)
{
	sig->autogroup = autogroup_task_get(current);
}

শূন্য sched_autogroup_exit(সংরচনা signal_struct *sig)
{
	autogroup_kref_put(sig->autogroup);
}

স্থির পূর্ণাঙ্ক __init setup_autogroup(বর্ণ *str)
{
	sysctl_sched_autogroup_enabled = 0;

	প্রত্যাগম 1;
}
__setup("noautogroup", setup_autogroup);

#ifdef CONFIG_PROC_FS

পূর্ণাঙ্ক proc_sched_autogroup_set_nice(সংরচনা task_struct *p, পূর্ণাঙ্ক nice)
{
	স্থির unsigned long next = INITIAL_JIFFIES;
	সংরচনা autogroup *ag;
	unsigned long shares;
	পূর্ণাঙ্ক err, idx;

	যদি (nice < MIN_NICE || nice > MAX_NICE)
		প্রত্যাগম -EINVAL;

	err = security_task_setnice(current, nice);
	যদি (err)
		প্রত্যাগম err;

	যদি (nice < 0 && !can_nice(current, nice))
		প্রত্যাগম -EPERM;

	/* This is a heavy operation, taking global locks.. */
	যদি (!capable(CAP_SYS_ADMIN) && time_before(jiffies, next))
		প্রত্যাগম -EAGAIN;

	next = HZ / 10 + jiffies;
	ag = autogroup_task_get(p);

	idx = array_index_nospec(nice + 20, 40);
	shares = scale_load(sched_prio_to_weight[idx]);

	down_write(&ag->lock);
	err = sched_group_set_shares(ag->tg, shares);
	যদি (!err)
		ag->nice = nice;
	up_write(&ag->lock);

	autogroup_kref_put(ag);

	প্রত্যাগম err;
}

শূন্য proc_sched_autogroup_show_task(সংরচনা task_struct *p, সংরচনা seq_file *m)
{
	সংরচনা autogroup *ag = autogroup_task_get(p);

	যদি (!task_group_is_autogroup(ag->tg))
		গচ্ছ out;

	down_read(&ag->lock);
	seq_printf(m, "/autogroup-%ld nice %d\n", ag->id, ag->nice);
	up_read(&ag->lock);

out:
	autogroup_kref_put(ag);
}
#endif /* CONFIG_PROC_FS */

পূর্ণাঙ্ক autogroup_path(সংরচনা task_group *tg, বর্ণ *buf, পূর্ণাঙ্ক buflen)
{
	যদি (!task_group_is_autogroup(tg))
		প্রত্যাগম 0;

	প্রত্যাগম snprintf(buf, buflen, "%s-%ld", "/autogroup", tg->autogroup->id);
}
