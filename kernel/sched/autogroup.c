// SPDX-License-Identifier: GPL-2.0

/*
 * Auto-group scheduling implementation:
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "autogroup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"

ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ __read_mostly sysctl_sched_autogroup_enabled = 1;
ਸਥਿਰ ਬਣਤਰ autogroup autogroup_default;
ਸਥਿਰ atomic_t autogroup_seq_nr;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SYSCTL
ਸਥਿਰ ਅਚਲ ਬਣਤਰ ctl_ਸਾਰਣੀ sched_autogroup_sysctls[] = {
	{
		.procname       = "sched_autogroup_enabled",
		.ਅੰਕੜੇ           = &sysctl_sched_autogroup_enabled,
		.maxlen         = ਆਕਾਰ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ),
		.ਬਹੁਲਕ           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

ਸਥਿਰ ਖਾਲੀ __init sched_autogroup_sysctl_init(ਖਾਲੀ)
{
	ਰਜਿਸਟਰ_sysctl_init("kernel", sched_autogroup_sysctls);
}
#ਨਹੀਂਤਾਂ /* !CONFIG_SYSCTL: */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sched_autogroup_sysctl_init() ਕਰੋ { } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ /* !CONFIG_SYSCTL */

ਖਾਲੀ __init autogroup_init(ਬਣਤਰ task_struct *init_task)
{
	autogroup_default.tg = &ਜੜ੍ਹ_task_ਸਮੂਹ;
	kref_init(&autogroup_default.kref);
	init_rwsem(&autogroup_default.ਤਾਲਾ);
	init_task->ਸੰਕੇਤ->autogroup = &autogroup_default;
	sched_autogroup_sysctl_init();
}

ਖਾਲੀ autogroup_free(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	kfree(tg->autogroup);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ autogroup_destroy(ਬਣਤਰ kref *kref)
{
	ਬਣਤਰ autogroup *ag = container_ਦਾ(kref, ਬਣਤਰ autogroup, kref);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED
	/* We've redirected RT tasks to the root task group... */
	ag->tg->rt_se = ਨਿਰਾ;
	ag->tg->rt_rq = ਨਿਰਾ;
#ਜੇ_ਅੰਤ
	sched_release_ਸਮੂਹ(ag->tg);
	sched_destroy_ਸਮੂਹ(ag->tg);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ autogroup_kref_put(ਬਣਤਰ autogroup *ag)
{
	kref_put(&ag->kref, autogroup_destroy);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ autogroup *autogroup_kref_get(ਬਣਤਰ autogroup *ag)
{
	kref_get(&ag->kref);
	ਵਾਪਸ ag;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ autogroup *autogroup_task_get(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ autogroup *ag;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	ਜੇ (!ਤਾਲਾ_task_sighand(p, &flags))
		ਵਾਪਸ autogroup_kref_get(&autogroup_default);

	ag = autogroup_kref_get(p->ਸੰਕੇਤ->autogroup);
	unlock_task_sighand(p, &flags);

	ਵਾਪਸ ag;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ autogroup *autogroup_create(ਖਾਲੀ)
{
	ਬਣਤਰ autogroup *ag = kzalloc_obj(*ag);
	ਬਣਤਰ task_ਸਮੂਹ *tg;

	ਜੇ (!ag)
		ਜਾਓ ਬਾਹਰਲਾ_fail;

	tg = sched_create_ਸਮੂਹ(&ਜੜ੍ਹ_task_ਸਮੂਹ);
	ਜੇ (IS_ERR(tg))
		ਜਾਓ ਬਾਹਰਲਾ_free;

	kref_init(&ag->kref);
	init_rwsem(&ag->ਤਾਲਾ);
	ag->id = atomic_inc_return(&autogroup_seq_nr);
	ag->tg = tg;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED
	/*
	 * Autogroup RT tasks are redirected to the root task group
	 * so we don't have to move tasks around upon policy change,
	 * or flail around trying to allocate bandwidth on the fly.
	 * A bandwidth exception in __sched_setscheduler() allows
	 * the policy change to proceed.
	 */
	free_rt_sched_ਸਮੂਹ(tg);
	tg->rt_se = ਜੜ੍ਹ_task_ਸਮੂਹ.rt_se;
	tg->rt_rq = ਜੜ੍ਹ_task_ਸਮੂਹ.rt_rq;
#ਜੇ_ਅੰਤ /* CONFIG_RT_GROUP_SCHED */
	tg->autogroup = ag;

	sched_online_ਸਮੂਹ(tg, &ਜੜ੍ਹ_task_ਸਮੂਹ);
	ਵਾਪਸ ag;

ਬਾਹਰਲਾ_free:
	kfree(ag);
ਬਾਹਰਲਾ_fail:
	ਜੇ (printk_ratelimit()) {
		printk(KERN_WARNING "autogroup_create: %s failure.\n",
			ag ? "sched_create_group()" : "kzalloc()");
	}

	ਵਾਪਸ autogroup_kref_get(&autogroup_default);
}

bool task_ਚਾਹੁੰਦਾ_autogroup(ਬਣਤਰ task_struct *p, ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	ਜੇ (tg != &ਜੜ੍ਹ_task_ਸਮੂਹ)
		ਵਾਪਸ false;
	/*
	 * If we race with autogroup_move_group() the caller can use the old
	 * value of signal->autogroup but in this case sched_move_task() will
	 * be called again before autogroup_kref_put().
	 *
	 * However, there is no way sched_autogroup_exit_task() could tell us
	 * to avoid autogroup->tg, so we abuse PF_EXITING flag for this case.
	 */
	ਜੇ (p->flags & PF_EXITING)
		ਵਾਪਸ false;

	ਵਾਪਸ true;
}

ਖਾਲੀ sched_autogroup_exit_task(ਬਣਤਰ task_struct *p)
{
	/*
	 * We are going to call exit_notify() and autogroup_move_group() can't
	 * see this thread after that: we can no longer use signal->autogroup.
	 * See the PF_EXITING check in task_wants_autogroup().
	 */
	sched_move_task(p, true);
}

ਸਥਿਰ ਖਾਲੀ
autogroup_move_ਸਮੂਹ(ਬਣਤਰ task_struct *p, ਬਣਤਰ autogroup *ag)
{
	ਬਣਤਰ autogroup *prev;
	ਬਣਤਰ task_struct *t;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	ਜੇ (WARN_ON_ONCE(!ਤਾਲਾ_task_sighand(p, &flags)))
		ਵਾਪਸ;

	prev = p->ਸੰਕੇਤ->autogroup;
	ਜੇ (prev == ag) {
		unlock_task_sighand(p, &flags);
		ਵਾਪਸ;
	}

	p->ਸੰਕੇਤ->autogroup = autogroup_kref_get(ag);
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
ਖਾਲੀ sched_autogroup_create_attach(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ autogroup *ag = autogroup_create();

	autogroup_move_ਸਮੂਹ(p, ag);

	/* Drop extra reference added by autogroup_create(): */
	autogroup_kref_put(ag);
}
EXPORT_ਚਿੰਨ੍ਹ(sched_autogroup_create_attach);

/* Cannot be called under siglock. Currently has no users: */
ਖਾਲੀ sched_autogroup_detach(ਬਣਤਰ task_struct *p)
{
	autogroup_move_ਸਮੂਹ(p, &autogroup_default);
}
EXPORT_ਚਿੰਨ੍ਹ(sched_autogroup_detach);

ਖਾਲੀ sched_autogroup_fork(ਬਣਤਰ signal_struct *sig)
{
	sig->autogroup = autogroup_task_get(current);
}

ਖਾਲੀ sched_autogroup_exit(ਬਣਤਰ signal_struct *sig)
{
	autogroup_kref_put(sig->autogroup);
}

ਸਥਿਰ ਪੂਰਨ __init ਤਿਆਰੀ_autogroup(ਅੱਖਰ *str)
{
	sysctl_sched_autogroup_enabled = 0;

	ਵਾਪਸ 1;
}
__setup("noautogroup", ਤਿਆਰੀ_autogroup);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PROC_FS

ਪੂਰਨ proc_sched_autogroup_ਸੈੱਟ_nice(ਬਣਤਰ task_struct *p, ਪੂਰਨ nice)
{
	ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਅਗਲਾ = INITIAL_JIFFIES;
	ਬਣਤਰ autogroup *ag;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ shares;
	ਪੂਰਨ err, idx;

	ਜੇ (nice < MIN_NICE || nice > MAX_NICE)
		ਵਾਪਸ -EINVAL;

	err = security_task_setnice(current, nice);
	ਜੇ (err)
		ਵਾਪਸ err;

	ਜੇ (nice < 0 && !can_nice(current, nice))
		ਵਾਪਸ -EPERM;

	/* This is a heavy operation, taking global locks.. */
	ਜੇ (!capable(CAP_SYS_ADMIN) && ਸਮਾਂ_before(jiffies, ਅਗਲਾ))
		ਵਾਪਸ -EAGAIN;

	ਅਗਲਾ = HZ / 10 + jiffies;
	ag = autogroup_task_get(p);

	idx = array_ਸੂਚਕਾਂਕ_nospec(nice + 20, 40);
	shares = scale_ਲੱਦੋ(sched_prio_ਮੰਜ਼ਿਲ_ਭਾਰ[idx]);

	ਥੱਲੇ_write(&ag->ਤਾਲਾ);
	err = sched_ਸਮੂਹ_ਸੈੱਟ_shares(ag->tg, shares);
	ਜੇ (!err)
		ag->nice = nice;
	ਉੱਪਰ_write(&ag->ਤਾਲਾ);

	autogroup_kref_put(ag);

	ਵਾਪਸ err;
}

ਖਾਲੀ proc_sched_autogroup_show_task(ਬਣਤਰ task_struct *p, ਬਣਤਰ seq_file *m)
{
	ਬਣਤਰ autogroup *ag = autogroup_task_get(p);

	ਜੇ (!task_ਸਮੂਹ_is_autogroup(ag->tg))
		ਜਾਓ ਬਾਹਰਲਾ;

	ਥੱਲੇ_read(&ag->ਤਾਲਾ);
	seq_printf(m, "/autogroup-%ld nice %d\n", ag->id, ag->nice);
	ਉੱਪਰ_read(&ag->ਤਾਲਾ);

ਬਾਹਰਲਾ:
	autogroup_kref_put(ag);
}
#ਜੇ_ਅੰਤ /* CONFIG_PROC_FS */

ਪੂਰਨ autogroup_ਰਸਤਾ(ਬਣਤਰ task_ਸਮੂਹ *tg, ਅੱਖਰ *buf, ਪੂਰਨ buflen)
{
	ਜੇ (!task_ਸਮੂਹ_is_autogroup(tg))
		ਵਾਪਸ 0;

	ਵਾਪਸ snprintf(buf, buflen, "%s-%ld", "/autogroup", tg->autogroup->id);
}
