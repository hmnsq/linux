// SPDX-License-Identifier: GPL-2.0

/*
 * Auto-group scheduling implementation:
 */

#ಅನ್ತರ್ಭಾವ "autogroup.h"
#ಅನ್ತರ್ಭಾವ "sched.h"

ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ __read_mostly sysctl_sched_autogroup_enabled = 1;
ಸ್ಥಿರ ರಚನೆ autogroup autogroup_default;
ಸ್ಥಿರ atomic_t autogroup_seq_nr;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SYSCTL
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ ctl_ಕೋಷ್ಟಕ sched_autogroup_sysctls[] = {
	{
		.procname       = "sched_autogroup_enabled",
		.ದತ್ತಾಂಶ           = &sysctl_sched_autogroup_enabled,
		.maxlen         = ಗಾತ್ರ(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ),
		.ಬಹುಲಕ           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

ಸ್ಥಿರ ಶೂನ್ಯ __init sched_autogroup_sysctl_init(ಶೂನ್ಯ)
{
	ರಿಜಿಸ್ಟರ್_sysctl_init("kernel", sched_autogroup_sysctls);
}
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_SYSCTL: */
#ವ್ಯಾಖ್ಯಾನಿಸು sched_autogroup_sysctl_init() ಮಾಡು { } ಆಗ (0)
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_SYSCTL */

ಶೂನ್ಯ __init autogroup_init(ರಚನೆ task_struct *init_task)
{
	autogroup_default.tg = &ಬೇರು_task_ಗುಂಪು;
	kref_init(&autogroup_default.kref);
	init_rwsem(&autogroup_default.ಬೀಗ);
	init_task->ಸಂಕೇತ->autogroup = &autogroup_default;
	sched_autogroup_sysctl_init();
}

ಶೂನ್ಯ autogroup_free(ರಚನೆ task_ಗುಂಪು *tg)
{
	kfree(tg->autogroup);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ autogroup_destroy(ರಚನೆ kref *kref)
{
	ರಚನೆ autogroup *ag = container_ದ(kref, ರಚನೆ autogroup, kref);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED
	/* We've redirected RT tasks to the root task group... */
	ag->tg->rt_se = ಖಾಲಿ;
	ag->tg->rt_rq = ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ
	sched_release_ಗುಂಪು(ag->tg);
	sched_destroy_ಗುಂಪು(ag->tg);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ autogroup_kref_put(ರಚನೆ autogroup *ag)
{
	kref_put(&ag->kref, autogroup_destroy);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ autogroup *autogroup_kref_get(ರಚನೆ autogroup *ag)
{
	kref_get(&ag->kref);
	ಹಿಂತಿರುಗಿ ag;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ autogroup *autogroup_task_get(ರಚನೆ task_struct *p)
{
	ರಚನೆ autogroup *ag;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	ಆದರೆ (!ಬೀಗ_task_sighand(p, &flags))
		ಹಿಂತಿರುಗಿ autogroup_kref_get(&autogroup_default);

	ag = autogroup_kref_get(p->ಸಂಕೇತ->autogroup);
	unlock_task_sighand(p, &flags);

	ಹಿಂತಿರುಗಿ ag;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ autogroup *autogroup_create(ಶೂನ್ಯ)
{
	ರಚನೆ autogroup *ag = kzalloc_obj(*ag);
	ರಚನೆ task_ಗುಂಪು *tg;

	ಆದರೆ (!ag)
		ಹೋಗು ಹೊರಹೋಗು_fail;

	tg = sched_create_ಗುಂಪು(&ಬೇರು_task_ಗುಂಪು);
	ಆದರೆ (IS_ERR(tg))
		ಹೋಗು ಹೊರಹೋಗು_free;

	kref_init(&ag->kref);
	init_rwsem(&ag->ಬೀಗ);
	ag->id = atomic_inc_return(&autogroup_seq_nr);
	ag->tg = tg;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED
	/*
	 * Autogroup RT tasks are redirected to the root task group
	 * so we don't have to move tasks around upon policy change,
	 * or flail around trying to allocate bandwidth on the fly.
	 * A bandwidth exception in __sched_setscheduler() allows
	 * the policy change to proceed.
	 */
	free_rt_sched_ಗುಂಪು(tg);
	tg->rt_se = ಬೇರು_task_ಗುಂಪು.rt_se;
	tg->rt_rq = ಬೇರು_task_ಗುಂಪು.rt_rq;
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_RT_GROUP_SCHED */
	tg->autogroup = ag;

	sched_online_ಗುಂಪು(tg, &ಬೇರು_task_ಗುಂಪು);
	ಹಿಂತಿರುಗಿ ag;

ಹೊರಹೋಗು_free:
	kfree(ag);
ಹೊರಹೋಗು_fail:
	ಆದರೆ (printk_ratelimit()) {
		printk(KERN_WARNING "autogroup_create: %s failure.\n",
			ag ? "sched_create_group()" : "kzalloc()");
	}

	ಹಿಂತಿರುಗಿ autogroup_kref_get(&autogroup_default);
}

bool task_ಬಯಸುತ್ತದೆ_autogroup(ರಚನೆ task_struct *p, ರಚನೆ task_ಗುಂಪು *tg)
{
	ಆದರೆ (tg != &ಬೇರು_task_ಗುಂಪು)
		ಹಿಂತಿರುಗಿ false;
	/*
	 * If we race with autogroup_move_group() the caller can use the old
	 * value of signal->autogroup but in this case sched_move_task() will
	 * be called again before autogroup_kref_put().
	 *
	 * However, there is no way sched_autogroup_exit_task() could tell us
	 * to avoid autogroup->tg, so we abuse PF_EXITING flag for this case.
	 */
	ಆದರೆ (p->flags & PF_EXITING)
		ಹಿಂತಿರುಗಿ false;

	ಹಿಂತಿರುಗಿ true;
}

ಶೂನ್ಯ sched_autogroup_exit_task(ರಚನೆ task_struct *p)
{
	/*
	 * We are going to call exit_notify() and autogroup_move_group() can't
	 * see this thread after that: we can no longer use signal->autogroup.
	 * See the PF_EXITING check in task_wants_autogroup().
	 */
	sched_move_task(p, true);
}

ಸ್ಥಿರ ಶೂನ್ಯ
autogroup_move_ಗುಂಪು(ರಚನೆ task_struct *p, ರಚನೆ autogroup *ag)
{
	ರಚನೆ autogroup *prev;
	ರಚನೆ task_struct *t;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	ಆದರೆ (WARN_ON_ONCE(!ಬೀಗ_task_sighand(p, &flags)))
		ಹಿಂತಿರುಗಿ;

	prev = p->ಸಂಕೇತ->autogroup;
	ಆದರೆ (prev == ag) {
		unlock_task_sighand(p, &flags);
		ಹಿಂತಿರುಗಿ;
	}

	p->ಸಂಕೇತ->autogroup = autogroup_kref_get(ag);
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
ಶೂನ್ಯ sched_autogroup_create_attach(ರಚನೆ task_struct *p)
{
	ರಚನೆ autogroup *ag = autogroup_create();

	autogroup_move_ಗುಂಪು(p, ag);

	/* Drop extra reference added by autogroup_create(): */
	autogroup_kref_put(ag);
}
EXPORT_ಚಿಹ್ನೆ(sched_autogroup_create_attach);

/* Cannot be called under siglock. Currently has no users: */
ಶೂನ್ಯ sched_autogroup_detach(ರಚನೆ task_struct *p)
{
	autogroup_move_ಗುಂಪು(p, &autogroup_default);
}
EXPORT_ಚಿಹ್ನೆ(sched_autogroup_detach);

ಶೂನ್ಯ sched_autogroup_fork(ರಚನೆ signal_struct *sig)
{
	sig->autogroup = autogroup_task_get(current);
}

ಶೂನ್ಯ sched_autogroup_exit(ರಚನೆ signal_struct *sig)
{
	autogroup_kref_put(sig->autogroup);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init ಸಿದ್ಧತೆ_autogroup(ಅಕ್ಷರ *str)
{
	sysctl_sched_autogroup_enabled = 0;

	ಹಿಂತಿರುಗಿ 1;
}
__setup("noautogroup", ಸಿದ್ಧತೆ_autogroup);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PROC_FS

ಪೂರ್ಣಾಂಕ proc_sched_autogroup_ಗಣ_nice(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ nice)
{
	ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಮುಂದಿನ = INITIAL_JIFFIES;
	ರಚನೆ autogroup *ag;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ shares;
	ಪೂರ್ಣಾಂಕ err, idx;

	ಆದರೆ (nice < MIN_NICE || nice > MAX_NICE)
		ಹಿಂತಿರುಗಿ -EINVAL;

	err = security_task_setnice(current, nice);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ err;

	ಆದರೆ (nice < 0 && !can_nice(current, nice))
		ಹಿಂತಿರುಗಿ -EPERM;

	/* This is a heavy operation, taking global locks.. */
	ಆದರೆ (!capable(CAP_SYS_ADMIN) && ಸಮಯ_before(jiffies, ಮುಂದಿನ))
		ಹಿಂತಿರುಗಿ -EAGAIN;

	ಮುಂದಿನ = HZ / 10 + jiffies;
	ag = autogroup_task_get(p);

	idx = array_ಸೂಚ್ಯಂಕ_nospec(nice + 20, 40);
	shares = scale_ತುಂಬು(sched_prio_ಗಮ್ಯ_ತೂಕ[idx]);

	ಕೆಳಕ್ಕೆ_write(&ag->ಬೀಗ);
	err = sched_ಗುಂಪು_ಗಣ_shares(ag->tg, shares);
	ಆದರೆ (!err)
		ag->nice = nice;
	ಮೇಲೆ_write(&ag->ಬೀಗ);

	autogroup_kref_put(ag);

	ಹಿಂತಿರುಗಿ err;
}

ಶೂನ್ಯ proc_sched_autogroup_show_task(ರಚನೆ task_struct *p, ರಚನೆ seq_file *m)
{
	ರಚನೆ autogroup *ag = autogroup_task_get(p);

	ಆದರೆ (!task_ಗುಂಪು_is_autogroup(ag->tg))
		ಹೋಗು ಹೊರಹೋಗು;

	ಕೆಳಕ್ಕೆ_read(&ag->ಬೀಗ);
	seq_printf(m, "/autogroup-%ld nice %d\n", ag->id, ag->nice);
	ಮೇಲೆ_read(&ag->ಬೀಗ);

ಹೊರಹೋಗು:
	autogroup_kref_put(ag);
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_PROC_FS */

ಪೂರ್ಣಾಂಕ autogroup_ಮಾರ್ಗ(ರಚನೆ task_ಗುಂಪು *tg, ಅಕ್ಷರ *buf, ಪೂರ್ಣಾಂಕ buflen)
{
	ಆದರೆ (!task_ಗುಂಪು_is_autogroup(tg))
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ snprintf(buf, buflen, "%s-%ld", "/autogroup", tg->autogroup->id);
}
