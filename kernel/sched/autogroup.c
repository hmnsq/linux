// SPDX-License-Identifier: GPL-2.0

/*
 * Auto-group scheduling implementation:
 */

#ضمن "autogroup.h"
#ضمن "sched.h"

بینښه صحيح __read_mostly sysctl_sched_autogroup_enabled = 1;
ساكن بنية autogroup autogroup_default;
ساكن atomic_t autogroup_seq_nr;

#که_تعریف_شوی CONFIG_SYSCTL
ساكن ثابت بنية ctl_جدول sched_autogroup_sysctls[] = {
	{
		.procname       = "sched_autogroup_enabled",
		.معلومات           = &sysctl_sched_autogroup_enabled,
		.maxlen         = حجم(بینښه صحيح),
		.ډېرپېښ           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

ساكن فراغ __init sched_autogroup_sysctl_init(فراغ)
{
	ثبتوونکی_sysctl_init("kernel", sched_autogroup_sysctls);
}
#وإلا /* !CONFIG_SYSCTL: */
#تعریف_کړه sched_autogroup_sysctl_init() افعل { } طالما (0)
#د_که_پای /* !CONFIG_SYSCTL */

فراغ __init autogroup_init(بنية task_struct *init_task)
{
	autogroup_default.tg = &ریښه_task_ډله;
	kref_init(&autogroup_default.kref);
	init_rwsem(&autogroup_default.قلف);
	init_task->إشارة->autogroup = &autogroup_default;
	sched_autogroup_sysctl_init();
}

فراغ autogroup_free(بنية task_ډله *tg)
{
	kfree(tg->autogroup);
}

ساكن مضمن فراغ autogroup_destroy(بنية kref *kref)
{
	بنية autogroup *ag = container_د(kref, بنية autogroup, kref);

#که_تعریف_شوی CONFIG_RT_ډله_SCHED
	/* We've redirected RT tasks to the root task group... */
	ag->tg->rt_se = عدم;
	ag->tg->rt_rq = عدم;
#د_که_پای
	sched_release_ډله(ag->tg);
	sched_destroy_ډله(ag->tg);
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
	بینښه اوږد flags;

	إذا (!قلف_task_sighand(p, &flags))
		أرجع autogroup_kref_get(&autogroup_default);

	ag = autogroup_kref_get(p->إشارة->autogroup);
	unlock_task_sighand(p, &flags);

	أرجع ag;
}

ساكن مضمن بنية autogroup *autogroup_create(فراغ)
{
	بنية autogroup *ag = kzalloc_obj(*ag);
	بنية task_ډله *tg;

	إذا (!ag)
		اذهب بهر_fail;

	tg = sched_create_ډله(&ریښه_task_ډله);
	إذا (IS_ERR(tg))
		اذهب بهر_free;

	kref_init(&ag->kref);
	init_rwsem(&ag->قلف);
	ag->id = atomic_inc_return(&autogroup_seq_nr);
	ag->tg = tg;
#که_تعریف_شوی CONFIG_RT_ډله_SCHED
	/*
	 * Autogroup RT tasks are redirected to the root task group
	 * so we don't have to move tasks around upon policy change,
	 * or flail around trying to allocate bandwidth on the fly.
	 * A bandwidth exception in __sched_setscheduler() allows
	 * the policy change to proceed.
	 */
	free_rt_sched_ډله(tg);
	tg->rt_se = ریښه_task_ډله.rt_se;
	tg->rt_rq = ریښه_task_ډله.rt_rq;
#د_که_پای /* CONFIG_RT_GROUP_SCHED */
	tg->autogroup = ag;

	sched_online_ډله(tg, &ریښه_task_ډله);
	أرجع ag;

بهر_free:
	kfree(ag);
بهر_fail:
	إذا (printk_ratelimit()) {
		printk(KERN_WARNING "autogroup_create: %s failure.\n",
			ag ? "sched_create_group()" : "kzalloc()");
	}

	أرجع autogroup_kref_get(&autogroup_default);
}

bool task_غواړي_autogroup(بنية task_struct *p, بنية task_ډله *tg)
{
	إذا (tg != &ریښه_task_ډله)
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
autogroup_move_ډله(بنية task_struct *p, بنية autogroup *ag)
{
	بنية autogroup *prev;
	بنية task_struct *t;
	بینښه اوږد flags;

	إذا (WARN_ON_ONCE(!قلف_task_sighand(p, &flags)))
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

	autogroup_move_ډله(p, ag);

	/* Drop extra reference added by autogroup_create(): */
	autogroup_kref_put(ag);
}
EXPORT_نښه(sched_autogroup_create_attach);

/* Cannot be called under siglock. Currently has no users: */
فراغ sched_autogroup_detach(بنية task_struct *p)
{
	autogroup_move_ډله(p, &autogroup_default);
}
EXPORT_نښه(sched_autogroup_detach);

فراغ sched_autogroup_fork(بنية signal_struct *sig)
{
	sig->autogroup = autogroup_task_get(current);
}

فراغ sched_autogroup_exit(بنية signal_struct *sig)
{
	autogroup_kref_put(sig->autogroup);
}

ساكن صحيح __init تیاری_autogroup(حرف *str)
{
	sysctl_sched_autogroup_enabled = 0;

	أرجع 1;
}
__setup("noautogroup", تیاری_autogroup);

#که_تعریف_شوی CONFIG_PROC_FS

صحيح proc_sched_autogroup_سېټ_nice(بنية task_struct *p, صحيح nice)
{
	ساكن بینښه اوږد بل = INITIAL_JIFFIES;
	بنية autogroup *ag;
	بینښه اوږد shares;
	صحيح err, idx;

	إذا (nice < MIN_NICE || nice > MAX_NICE)
		أرجع -EINVAL;

	err = security_task_setnice(current, nice);
	إذا (err)
		أرجع err;

	إذا (nice < 0 && !can_nice(current, nice))
		أرجع -EPERM;

	/* This is a heavy operation, taking global locks.. */
	إذا (!capable(CAP_SYS_ADMIN) && وخت_before(jiffies, بل))
		أرجع -EAGAIN;

	بل = HZ / 10 + jiffies;
	ag = autogroup_task_get(p);

	idx = array_شاخص_nospec(nice + 20, 40);
	shares = scale_بارول(sched_prio_منزل_وزن[idx]);

	ښکته_write(&ag->قلف);
	err = sched_ډله_سېټ_shares(ag->tg, shares);
	إذا (!err)
		ag->nice = nice;
	پورته_write(&ag->قلف);

	autogroup_kref_put(ag);

	أرجع err;
}

فراغ proc_sched_autogroup_show_task(بنية task_struct *p, بنية seq_file *m)
{
	بنية autogroup *ag = autogroup_task_get(p);

	إذا (!task_ډله_is_autogroup(ag->tg))
		اذهب بهر;

	ښکته_read(&ag->قلف);
	seq_printf(m, "/autogroup-%ld nice %d\n", ag->id, ag->nice);
	پورته_read(&ag->قلف);

بهر:
	autogroup_kref_put(ag);
}
#د_که_پای /* CONFIG_PROC_FS */

صحيح autogroup_لار(بنية task_ډله *tg, حرف *buf, صحيح buflen)
{
	إذا (!task_ډله_is_autogroup(tg))
		أرجع 0;

	أرجع snprintf(buf, buflen, "%s-%ld", "/autogroup", tg->autogroup->id);
}
