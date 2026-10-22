// SPDX-License-Identifier: GPL-2.0

/*
 * Auto-group scheduling implementation:
 */

#ضمن "autogroup.h"
#ضمن "sched.h"

unsigned صحيح __read_mostly sysctl_sched_autogroup_enabled = 1;
ساكن بنية autogroup autogroup_default;
ساكن atomic_t autogroup_seq_nr;

#ifdef CONFIG_SYSCTL
ساكن ثابت بنية ctl_table sched_autogroup_sysctls[] = {
	{
		.procname       = "sched_autogroup_enabled",
		.data           = &sysctl_sched_autogroup_enabled,
		.maxlen         = حجم(unsigned صحيح),
		.mode           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

ساكن فراغ __init sched_autogroup_sysctl_init(فراغ)
{
	register_sysctl_init("kernel", sched_autogroup_sysctls);
}
#وإلا /* !CONFIG_SYSCTL: */
#define sched_autogroup_sysctl_init() افعل { } طالما (0)
#endif /* !CONFIG_SYSCTL */

فراغ __init autogroup_init(بنية task_struct *init_task)
{
	autogroup_default.tg = &جڑ_task_group;
	kref_init(&autogroup_default.kref);
	init_rwsem(&autogroup_default.lock);
	init_task->إشارة->autogroup = &autogroup_default;
	sched_autogroup_sysctl_init();
}

فراغ autogroup_free(بنية task_group *tg)
{
	kfree(tg->autogroup);
}

ساكن مضمن فراغ autogroup_destroy(بنية kref *kref)
{
	بنية autogroup *ag = container_of(kref, بنية autogroup, kref);

#ifdef CONFIG_RT_GROUP_SCHED
	/* We've redirected RT tasks to the root task group... */
	ag->tg->rt_se = عدم;
	ag->tg->rt_rq = عدم;
#endif
	sched_release_group(ag->tg);
	sched_destroy_group(ag->tg);
}

ساكن مضمن فراغ autogroup_kref_put(بنية autogroup *ag)
{
	kref_put(&ag->kref, autogroup_destroy);
}

ساكن مضمن بنية autogroup *autogroup_kref_get(بنية autogroup *ag)
{
	kref_get(&ag->kref);
	أرجع ag;
}

ساكن مضمن بنية autogroup *autogroup_task_get(بنية task_struct *p)
{
	بنية autogroup *ag;
	unsigned long flags;

	إذا (!lock_task_sighand(p, &flags))
		أرجع autogroup_kref_get(&autogroup_default);

	ag = autogroup_kref_get(p->إشارة->autogroup);
	unlock_task_sighand(p, &flags);

	أرجع ag;
}

ساكن مضمن بنية autogroup *autogroup_create(فراغ)
{
	بنية autogroup *ag = kzalloc_obj(*ag);
	بنية task_group *tg;

	إذا (!ag)
		اذهب out_fail;

	tg = sched_create_group(&جڑ_task_group);
	إذا (IS_ERR(tg))
		اذهب out_free;

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
	tg->rt_se = جڑ_task_group.rt_se;
	tg->rt_rq = جڑ_task_group.rt_rq;
#endif /* CONFIG_RT_GROUP_SCHED */
	tg->autogroup = ag;

	sched_online_group(tg, &جڑ_task_group);
	أرجع ag;

out_free:
	kfree(ag);
out_fail:
	إذا (printk_ratelimit()) {
		printk(KERN_WARNING "autogroup_create: %s failure.\n",
			ag ? "sched_create_group()" : "kzalloc()");
	}

	أرجع autogroup_kref_get(&autogroup_default);
}

bool task_wants_autogroup(بنية task_struct *p, بنية task_group *tg)
{
	إذا (tg != &جڑ_task_group)
		أرجع false;
	/*
	 * If we race with autogroup_move_group() the caller can use the old
	 * value of signal->autogroup but in this case sched_move_task() will
	 * be called again before autogroup_kref_put().
	 *
	 * However, there is no way sched_autogroup_exit_task() could tell us
	 * to avoid autogroup->tg, so we abuse PF_EXITING flag for this case.
	 */
	إذا (p->flags & PF_EXITING)
		أرجع false;

	أرجع true;
}

فراغ sched_autogroup_exit_task(بنية task_struct *p)
{
	/*
	 * We are going to call exit_notify() and autogroup_move_group() can't
	 * see this thread after that: we can no longer use signal->autogroup.
	 * See the PF_EXITING check in task_wants_autogroup().
	 */
	sched_move_task(p, true);
}

ساكن فراغ
autogroup_move_group(بنية task_struct *p, بنية autogroup *ag)
{
	بنية autogroup *prev;
	بنية task_struct *t;
	unsigned long flags;

	إذا (WARN_ON_ONCE(!lock_task_sighand(p, &flags)))
		أرجع;

	prev = p->إشارة->autogroup;
	إذا (prev == ag) {
		unlock_task_sighand(p, &flags);
		أرجع;
	}

	p->إشارة->autogroup = autogroup_kref_get(ag);
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
فراغ sched_autogroup_create_attach(بنية task_struct *p)
{
	بنية autogroup *ag = autogroup_create();

	autogroup_move_group(p, ag);

	/* Drop extra reference added by autogroup_create(): */
	autogroup_kref_put(ag);
}
EXPORT_علامت(sched_autogroup_create_attach);

/* Cannot be called under siglock. Currently has no users: */
فراغ sched_autogroup_detach(بنية task_struct *p)
{
	autogroup_move_group(p, &autogroup_default);
}
EXPORT_علامت(sched_autogroup_detach);

فراغ sched_autogroup_fork(بنية signal_struct *sig)
{
	sig->autogroup = autogroup_task_get(current);
}

فراغ sched_autogroup_exit(بنية signal_struct *sig)
{
	autogroup_kref_put(sig->autogroup);
}

ساكن صحيح __init setup_autogroup(حرف *str)
{
	sysctl_sched_autogroup_enabled = 0;

	أرجع 1;
}
__setup("noautogroup", setup_autogroup);

#ifdef CONFIG_PROC_FS

صحيح proc_sched_autogroup_set_nice(بنية task_struct *p, صحيح nice)
{
	ساكن unsigned long next = INITIAL_JIFFIES;
	بنية autogroup *ag;
	unsigned long shares;
	صحيح err, idx;

	إذا (nice < MIN_NICE || nice > MAX_NICE)
		أرجع -EINVAL;

	err = security_task_setnice(current, nice);
	إذا (err)
		أرجع err;

	إذا (nice < 0 && !can_nice(current, nice))
		أرجع -EPERM;

	/* This is a heavy operation, taking global locks.. */
	إذا (!capable(CAP_SYS_ADMIN) && time_before(jiffies, next))
		أرجع -EAGAIN;

	next = HZ / 10 + jiffies;
	ag = autogroup_task_get(p);

	idx = array_index_nospec(nice + 20, 40);
	shares = scale_load(sched_prio_to_weight[idx]);

	down_write(&ag->lock);
	err = sched_group_set_shares(ag->tg, shares);
	إذا (!err)
		ag->nice = nice;
	up_write(&ag->lock);

	autogroup_kref_put(ag);

	أرجع err;
}

فراغ proc_sched_autogroup_show_task(بنية task_struct *p, بنية seq_file *m)
{
	بنية autogroup *ag = autogroup_task_get(p);

	إذا (!task_group_is_autogroup(ag->tg))
		اذهب out;

	down_read(&ag->lock);
	seq_printf(m, "/autogroup-%ld nice %d\n", ag->id, ag->nice);
	up_read(&ag->lock);

out:
	autogroup_kref_put(ag);
}
#endif /* CONFIG_PROC_FS */

صحيح autogroup_path(بنية task_group *tg, حرف *buf, صحيح buflen)
{
	إذا (!task_group_is_autogroup(tg))
		أرجع 0;

	أرجع snprintf(buf, buflen, "%s-%ld", "/autogroup", tg->autogroup->id);
}
