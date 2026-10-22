// SPDX-License-Identifier: GPL-2.0

/*
 * Auto-group scheduling implementation:
 */

#ಅನ್ತರ್ಭಾವ "autogroup.h"
#ಅನ್ತರ್ಭಾವ "sched.h"

unsigned ಪೂರ್ಣಾಙ್ಕ __read_mostly sysctl_sched_autogroup_enabled = 1;
ಸ್ಥಿರ ಸಂರಚನಾ autogroup autogroup_default;
ಸ್ಥಿರ atomic_t autogroup_seq_nr;

#ifdef CONFIG_SYSCTL
ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ ctl_table sched_autogroup_sysctls[] = {
	{
		.procname       = "sched_autogroup_enabled",
		.data           = &sysctl_sched_autogroup_enabled,
		.maxlen         = ಪರಿಮಾಣ(unsigned ಪೂರ್ಣಾಙ್ಕ),
		.mode           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

ಸ್ಥಿರ ಶೂನ್ಯ __init sched_autogroup_sysctl_init(ಶೂನ್ಯ)
{
	register_sysctl_init("kernel", sched_autogroup_sysctls);
}
#ಅನ್ಯಥಾ /* !CONFIG_SYSCTL: */
#define sched_autogroup_sysctl_init() ಕುರು { } ಯಾವತ್ (0)
#endif /* !CONFIG_SYSCTL */

ಶೂನ್ಯ __init autogroup_init(ಸಂರಚನಾ task_struct *init_task)
{
	autogroup_default.tg = &ಬೇರು_task_group;
	kref_init(&autogroup_default.kref);
	init_rwsem(&autogroup_default.lock);
	init_task->ಸಙ್ಕೇತ->autogroup = &autogroup_default;
	sched_autogroup_sysctl_init();
}

ಶೂನ್ಯ autogroup_free(ಸಂರಚನಾ task_group *tg)
{
	kfree(tg->autogroup);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ autogroup_destroy(ಸಂರಚನಾ kref *kref)
{
	ಸಂರಚನಾ autogroup *ag = container_of(kref, ಸಂರಚನಾ autogroup, kref);

#ifdef CONFIG_RT_GROUP_SCHED
	/* We've redirected RT tasks to the root task group... */
	ag->tg->rt_se = ರಿಕ್ತ;
	ag->tg->rt_rq = ರಿಕ್ತ;
#endif
	sched_release_group(ag->tg);
	sched_destroy_group(ag->tg);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ autogroup_kref_put(ಸಂರಚನಾ autogroup *ag)
{
	kref_put(&ag->kref, autogroup_destroy);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ autogroup *autogroup_kref_get(ಸಂರಚನಾ autogroup *ag)
{
	kref_get(&ag->kref);
	ಪ್ರತ್ಯಾಗಮ ag;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ autogroup *autogroup_task_get(ಸಂರಚನಾ task_struct *p)
{
	ಸಂರಚನಾ autogroup *ag;
	unsigned long flags;

	ಯದಿ (!lock_task_sighand(p, &flags))
		ಪ್ರತ್ಯಾಗಮ autogroup_kref_get(&autogroup_default);

	ag = autogroup_kref_get(p->ಸಙ್ಕೇತ->autogroup);
	unlock_task_sighand(p, &flags);

	ಪ್ರತ್ಯಾಗಮ ag;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ autogroup *autogroup_create(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ autogroup *ag = kzalloc_obj(*ag);
	ಸಂರಚನಾ task_group *tg;

	ಯದಿ (!ag)
		ಗಚ್ಛ out_fail;

	tg = sched_create_group(&ಬೇರು_task_group);
	ಯದಿ (IS_ERR(tg))
		ಗಚ್ಛ out_free;

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
	tg->rt_se = ಬೇರು_task_group.rt_se;
	tg->rt_rq = ಬೇರು_task_group.rt_rq;
#endif /* CONFIG_RT_GROUP_SCHED */
	tg->autogroup = ag;

	sched_online_group(tg, &ಬೇರು_task_group);
	ಪ್ರತ್ಯಾಗಮ ag;

out_free:
	kfree(ag);
out_fail:
	ಯದಿ (printk_ratelimit()) {
		printk(KERN_WARNING "autogroup_create: %s failure.\n",
			ag ? "sched_create_group()" : "kzalloc()");
	}

	ಪ್ರತ್ಯಾಗಮ autogroup_kref_get(&autogroup_default);
}

bool task_wants_autogroup(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ task_group *tg)
{
	ಯದಿ (tg != &ಬೇರು_task_group)
		ಪ್ರತ್ಯಾಗಮ false;
	/*
	 * If we race with autogroup_move_group() the caller can use the old
	 * value of signal->autogroup but in this case sched_move_task() will
	 * be called again before autogroup_kref_put().
	 *
	 * However, there is no way sched_autogroup_exit_task() could tell us
	 * to avoid autogroup->tg, so we abuse PF_EXITING flag for this case.
	 */
	ಯದಿ (p->flags & PF_EXITING)
		ಪ್ರತ್ಯಾಗಮ false;

	ಪ್ರತ್ಯಾಗಮ true;
}

ಶೂನ್ಯ sched_autogroup_exit_task(ಸಂರಚನಾ task_struct *p)
{
	/*
	 * We are going to call exit_notify() and autogroup_move_group() can't
	 * see this thread after that: we can no longer use signal->autogroup.
	 * See the PF_EXITING check in task_wants_autogroup().
	 */
	sched_move_task(p, true);
}

ಸ್ಥಿರ ಶೂನ್ಯ
autogroup_move_group(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ autogroup *ag)
{
	ಸಂರಚನಾ autogroup *prev;
	ಸಂರಚನಾ task_struct *t;
	unsigned long flags;

	ಯದಿ (WARN_ON_ONCE(!lock_task_sighand(p, &flags)))
		ಪ್ರತ್ಯಾಗಮ;

	prev = p->ಸಙ್ಕೇತ->autogroup;
	ಯದಿ (prev == ag) {
		unlock_task_sighand(p, &flags);
		ಪ್ರತ್ಯಾಗಮ;
	}

	p->ಸಙ್ಕೇತ->autogroup = autogroup_kref_get(ag);
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
ಶೂನ್ಯ sched_autogroup_create_attach(ಸಂರಚನಾ task_struct *p)
{
	ಸಂರಚನಾ autogroup *ag = autogroup_create();

	autogroup_move_group(p, ag);

	/* Drop extra reference added by autogroup_create(): */
	autogroup_kref_put(ag);
}
EXPORT_ಚಿಹ್ನೆ(sched_autogroup_create_attach);

/* Cannot be called under siglock. Currently has no users: */
ಶೂನ್ಯ sched_autogroup_detach(ಸಂರಚನಾ task_struct *p)
{
	autogroup_move_group(p, &autogroup_default);
}
EXPORT_ಚಿಹ್ನೆ(sched_autogroup_detach);

ಶೂನ್ಯ sched_autogroup_fork(ಸಂರಚನಾ signal_struct *sig)
{
	sig->autogroup = autogroup_task_get(current);
}

ಶೂನ್ಯ sched_autogroup_exit(ಸಂರಚನಾ signal_struct *sig)
{
	autogroup_kref_put(sig->autogroup);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ __init setup_autogroup(ವರ್ಣ *str)
{
	sysctl_sched_autogroup_enabled = 0;

	ಪ್ರತ್ಯಾಗಮ 1;
}
__setup("noautogroup", setup_autogroup);

#ifdef CONFIG_PROC_FS

ಪೂರ್ಣಾಙ್ಕ proc_sched_autogroup_set_nice(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ nice)
{
	ಸ್ಥಿರ unsigned long next = INITIAL_JIFFIES;
	ಸಂರಚನಾ autogroup *ag;
	unsigned long shares;
	ಪೂರ್ಣಾಙ್ಕ err, idx;

	ಯದಿ (nice < MIN_NICE || nice > MAX_NICE)
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	err = security_task_setnice(current, nice);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ err;

	ಯದಿ (nice < 0 && !can_nice(current, nice))
		ಪ್ರತ್ಯಾಗಮ -EPERM;

	/* This is a heavy operation, taking global locks.. */
	ಯದಿ (!capable(CAP_SYS_ADMIN) && time_before(jiffies, next))
		ಪ್ರತ್ಯಾಗಮ -EAGAIN;

	next = HZ / 10 + jiffies;
	ag = autogroup_task_get(p);

	idx = array_index_nospec(nice + 20, 40);
	shares = scale_load(sched_prio_to_weight[idx]);

	down_write(&ag->lock);
	err = sched_group_set_shares(ag->tg, shares);
	ಯದಿ (!err)
		ag->nice = nice;
	up_write(&ag->lock);

	autogroup_kref_put(ag);

	ಪ್ರತ್ಯಾಗಮ err;
}

ಶೂನ್ಯ proc_sched_autogroup_show_task(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ seq_file *m)
{
	ಸಂರಚನಾ autogroup *ag = autogroup_task_get(p);

	ಯದಿ (!task_group_is_autogroup(ag->tg))
		ಗಚ್ಛ out;

	down_read(&ag->lock);
	seq_printf(m, "/autogroup-%ld nice %d\n", ag->id, ag->nice);
	up_read(&ag->lock);

out:
	autogroup_kref_put(ag);
}
#endif /* CONFIG_PROC_FS */

ಪೂರ್ಣಾಙ್ಕ autogroup_path(ಸಂರಚನಾ task_group *tg, ವರ್ಣ *buf, ಪೂರ್ಣಾಙ್ಕ buflen)
{
	ಯದಿ (!task_group_is_autogroup(tg))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತ್ಯಾಗಮ snprintf(buf, buflen, "%s-%ld", "/autogroup", tg->autogroup->id);
}
