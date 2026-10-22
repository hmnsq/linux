// SPDX-License-Identifier: GPL-2.0

/*
 * Auto-group scheduling implementation:
 */

#അന്തര്ഭാവ "autogroup.h"
#അന്തര്ഭാവ "sched.h"

unsigned പൂർണ്ണസംഖ്യ __read_mostly sysctl_sched_autogroup_enabled = 1;
സ്ഥിതം ഘടന autogroup autogroup_default;
സ്ഥിതം atomic_t autogroup_seq_nr;

#ifdef CONFIG_SYSCTL
സ്ഥിതം സ്ഥിരം ഘടന ctl_table sched_autogroup_sysctls[] = {
	{
		.procname       = "sched_autogroup_enabled",
		.data           = &sysctl_sched_autogroup_enabled,
		.maxlen         = വലുപ്പം(unsigned പൂർണ്ണസംഖ്യ),
		.mode           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

സ്ഥിതം ശൂന്യം __init sched_autogroup_sysctl_init(ശൂന്യം)
{
	register_sysctl_init("kernel", sched_autogroup_sysctls);
}
#അല്ലെങ്കിൽ /* !CONFIG_SYSCTL: */
#define sched_autogroup_sysctl_init() ചെയ്യുക { } ആകുമ്പോൾ (0)
#endif /* !CONFIG_SYSCTL */

ശൂന്യം __init autogroup_init(ഘടന task_struct *init_task)
{
	autogroup_default.tg = &വേര്_task_group;
	kref_init(&autogroup_default.kref);
	init_rwsem(&autogroup_default.lock);
	init_task->സൂചന->autogroup = &autogroup_default;
	sched_autogroup_sysctl_init();
}

ശൂന്യം autogroup_free(ഘടന task_group *tg)
{
	kfree(tg->autogroup);
}

സ്ഥിതം വരിയിൽ ശൂന്യം autogroup_destroy(ഘടന kref *kref)
{
	ഘടന autogroup *ag = container_of(kref, ഘടന autogroup, kref);

#ifdef CONFIG_RT_GROUP_SCHED
	/* We've redirected RT tasks to the root task group... */
	ag->tg->rt_se = ശൂന്യ;
	ag->tg->rt_rq = ശൂന്യ;
#endif
	sched_release_group(ag->tg);
	sched_destroy_group(ag->tg);
}

സ്ഥിതം വരിയിൽ ശൂന്യം autogroup_kref_put(ഘടന autogroup *ag)
{
	kref_put(&ag->kref, autogroup_destroy);
}

സ്ഥിതം വരിയിൽ ഘടന autogroup *autogroup_kref_get(ഘടന autogroup *ag)
{
	kref_get(&ag->kref);
	തിരികെ ag;
}

സ്ഥിതം വരിയിൽ ഘടന autogroup *autogroup_task_get(ഘടന task_struct *p)
{
	ഘടന autogroup *ag;
	unsigned long flags;

	എങ്കിൽ (!lock_task_sighand(p, &flags))
		തിരികെ autogroup_kref_get(&autogroup_default);

	ag = autogroup_kref_get(p->സൂചന->autogroup);
	unlock_task_sighand(p, &flags);

	തിരികെ ag;
}

സ്ഥിതം വരിയിൽ ഘടന autogroup *autogroup_create(ശൂന്യം)
{
	ഘടന autogroup *ag = kzalloc_obj(*ag);
	ഘടന task_group *tg;

	എങ്കിൽ (!ag)
		പോകുക out_fail;

	tg = sched_create_group(&വേര്_task_group);
	എങ്കിൽ (IS_ERR(tg))
		പോകുക out_free;

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
	tg->rt_se = വേര്_task_group.rt_se;
	tg->rt_rq = വേര്_task_group.rt_rq;
#endif /* CONFIG_RT_GROUP_SCHED */
	tg->autogroup = ag;

	sched_online_group(tg, &വേര്_task_group);
	തിരികെ ag;

out_free:
	kfree(ag);
out_fail:
	എങ്കിൽ (printk_ratelimit()) {
		printk(KERN_WARNING "autogroup_create: %s failure.\n",
			ag ? "sched_create_group()" : "kzalloc()");
	}

	തിരികെ autogroup_kref_get(&autogroup_default);
}

bool task_wants_autogroup(ഘടന task_struct *p, ഘടന task_group *tg)
{
	എങ്കിൽ (tg != &വേര്_task_group)
		തിരികെ false;
	/*
	 * If we race with autogroup_move_group() the caller can use the old
	 * value of signal->autogroup but in this case sched_move_task() will
	 * be called again before autogroup_kref_put().
	 *
	 * However, there is no way sched_autogroup_exit_task() could tell us
	 * to avoid autogroup->tg, so we abuse PF_EXITING flag for this case.
	 */
	എങ്കിൽ (p->flags & PF_EXITING)
		തിരികെ false;

	തിരികെ true;
}

ശൂന്യം sched_autogroup_exit_task(ഘടന task_struct *p)
{
	/*
	 * We are going to call exit_notify() and autogroup_move_group() can't
	 * see this thread after that: we can no longer use signal->autogroup.
	 * See the PF_EXITING check in task_wants_autogroup().
	 */
	sched_move_task(p, true);
}

സ്ഥിതം ശൂന്യം
autogroup_move_group(ഘടന task_struct *p, ഘടന autogroup *ag)
{
	ഘടന autogroup *prev;
	ഘടന task_struct *t;
	unsigned long flags;

	എങ്കിൽ (WARN_ON_ONCE(!lock_task_sighand(p, &flags)))
		തിരികെ;

	prev = p->സൂചന->autogroup;
	എങ്കിൽ (prev == ag) {
		unlock_task_sighand(p, &flags);
		തിരികെ;
	}

	p->സൂചന->autogroup = autogroup_kref_get(ag);
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
ശൂന്യം sched_autogroup_create_attach(ഘടന task_struct *p)
{
	ഘടന autogroup *ag = autogroup_create();

	autogroup_move_group(p, ag);

	/* Drop extra reference added by autogroup_create(): */
	autogroup_kref_put(ag);
}
EXPORT_ചിഹ്നം(sched_autogroup_create_attach);

/* Cannot be called under siglock. Currently has no users: */
ശൂന്യം sched_autogroup_detach(ഘടന task_struct *p)
{
	autogroup_move_group(p, &autogroup_default);
}
EXPORT_ചിഹ്നം(sched_autogroup_detach);

ശൂന്യം sched_autogroup_fork(ഘടന signal_struct *sig)
{
	sig->autogroup = autogroup_task_get(current);
}

ശൂന്യം sched_autogroup_exit(ഘടന signal_struct *sig)
{
	autogroup_kref_put(sig->autogroup);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ __init setup_autogroup(അക്ഷരം *str)
{
	sysctl_sched_autogroup_enabled = 0;

	തിരികെ 1;
}
__setup("noautogroup", setup_autogroup);

#ifdef CONFIG_PROC_FS

പൂർണ്ണസംഖ്യ proc_sched_autogroup_set_nice(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ nice)
{
	സ്ഥിതം unsigned long next = INITIAL_JIFFIES;
	ഘടന autogroup *ag;
	unsigned long shares;
	പൂർണ്ണസംഖ്യ err, idx;

	എങ്കിൽ (nice < MIN_NICE || nice > MAX_NICE)
		തിരികെ -EINVAL;

	err = security_task_setnice(current, nice);
	എങ്കിൽ (err)
		തിരികെ err;

	എങ്കിൽ (nice < 0 && !can_nice(current, nice))
		തിരികെ -EPERM;

	/* This is a heavy operation, taking global locks.. */
	എങ്കിൽ (!capable(CAP_SYS_ADMIN) && time_before(jiffies, next))
		തിരികെ -EAGAIN;

	next = HZ / 10 + jiffies;
	ag = autogroup_task_get(p);

	idx = array_index_nospec(nice + 20, 40);
	shares = scale_load(sched_prio_to_weight[idx]);

	down_write(&ag->lock);
	err = sched_group_set_shares(ag->tg, shares);
	എങ്കിൽ (!err)
		ag->nice = nice;
	up_write(&ag->lock);

	autogroup_kref_put(ag);

	തിരികെ err;
}

ശൂന്യം proc_sched_autogroup_show_task(ഘടന task_struct *p, ഘടന seq_file *m)
{
	ഘടന autogroup *ag = autogroup_task_get(p);

	എങ്കിൽ (!task_group_is_autogroup(ag->tg))
		പോകുക out;

	down_read(&ag->lock);
	seq_printf(m, "/autogroup-%ld nice %d\n", ag->id, ag->nice);
	up_read(&ag->lock);

out:
	autogroup_kref_put(ag);
}
#endif /* CONFIG_PROC_FS */

പൂർണ്ണസംഖ്യ autogroup_path(ഘടന task_group *tg, അക്ഷരം *buf, പൂർണ്ണസംഖ്യ buflen)
{
	എങ്കിൽ (!task_group_is_autogroup(tg))
		തിരികെ 0;

	തിരികെ snprintf(buf, buflen, "%s-%ld", "/autogroup", tg->autogroup->id);
}
