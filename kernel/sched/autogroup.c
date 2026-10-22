// SPDX-License-Identifier: GPL-2.0

/*
 * Auto-group scheduling implementation:
 */

#ضمن "autogroup.h"
#ضمن "sched.h"

بیعلامت صحيح __read_mostly sysctl_sched_autogroup_enabled = 1;
ساكن بنية autogroup autogroup_default;
ساكن atomic_t autogroup_seq_nr;

#اگر_تعریف_شده CONFIG_SYSCTL
ساكن ثابت بنية ctl_جدول sched_autogroup_sysctls[] = {
	{
		.procname       = "sched_autogroup_enabled",
		.داده           = &sysctl_sched_autogroup_enabled,
		.maxlen         = حجم(بیعلامت صحيح),
		.نما           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

ساكن فراغ __init sched_autogroup_sysctl_init(فراغ)
{
	ثبات_sysctl_init("kernel", sched_autogroup_sysctls);
}
#وإلا /* !CONFIG_SYSCTL: */
#تعریف_کن sched_autogroup_sysctl_init() افعل { } طالما (0)
#پایان_اگر /* !CONFIG_SYSCTL */

فراغ __init autogroup_init(بنية task_struct *init_task)
{
	autogroup_default.tg = &ریشه_task_گروه;
	kref_init(&autogroup_default.kref);
	init_rwsem(&autogroup_default.قفل);
	init_task->إشارة->autogroup = &autogroup_default;
	sched_autogroup_sysctl_init();
}

فراغ autogroup_free(بنية task_گروه *tg)
{
	kfree(tg->autogroup);
}

ساكن مضمن فراغ autogroup_destroy(بنية kref *kref)
{
	بنية autogroup *ag = container_از(kref, بنية autogroup, kref);

#اگر_تعریف_شده CONFIG_RT_گروه_SCHED
	/* We've redirected RT tasks to the root task group... */
	ag->tg->rt_se = عدم;
	ag->tg->rt_rq = عدم;
#پایان_اگر
	sched_release_گروه(ag->tg);
	sched_destroy_گروه(ag->tg);
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
	بیعلامت دراز flags;

	إذا (!قفل_task_sighand(p, &flags))
		أرجع autogroup_kref_get(&autogroup_default);

	ag = autogroup_kref_get(p->إشارة->autogroup);
	unlock_task_sighand(p, &flags);

	أرجع ag;
}

ساكن مضمن بنية autogroup *autogroup_create(فراغ)
{
	بنية autogroup *ag = kzalloc_obj(*ag);
	بنية task_گروه *tg;

	إذا (!ag)
		اذهب بیرون_fail;

	tg = sched_create_گروه(&ریشه_task_گروه);
	إذا (IS_ERR(tg))
		اذهب بیرون_free;

	kref_init(&ag->kref);
	init_rwsem(&ag->قفل);
	ag->id = atomic_inc_return(&autogroup_seq_nr);
	ag->tg = tg;
#اگر_تعریف_شده CONFIG_RT_گروه_SCHED
	/*
	 * Autogroup RT tasks are redirected to the root task group
	 * so we don't have to move tasks around upon policy change,
	 * or flail around trying to allocate bandwidth on the fly.
	 * A bandwidth exception in __sched_setscheduler() allows
	 * the policy change to proceed.
	 */
	free_rt_sched_گروه(tg);
	tg->rt_se = ریشه_task_گروه.rt_se;
	tg->rt_rq = ریشه_task_گروه.rt_rq;
#پایان_اگر /* CONFIG_RT_GROUP_SCHED */
	tg->autogroup = ag;

	sched_online_گروه(tg, &ریشه_task_گروه);
	أرجع ag;

بیرون_free:
	kfree(ag);
بیرون_fail:
	إذا (printk_ratelimit()) {
		printk(KERN_WARNING "autogroup_create: %s failure.\n",
			ag ? "sched_create_group()" : "kzalloc()");
	}

	أرجع autogroup_kref_get(&autogroup_default);
}

bool task_میخواهد_autogroup(بنية task_struct *p, بنية task_گروه *tg)
{
	إذا (tg != &ریشه_task_گروه)
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
autogroup_move_گروه(بنية task_struct *p, بنية autogroup *ag)
{
	بنية autogroup *prev;
	بنية task_struct *t;
	بیعلامت دراز flags;

	إذا (WARN_ON_ONCE(!قفل_task_sighand(p, &flags)))
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

	autogroup_move_گروه(p, ag);

	/* Drop extra reference added by autogroup_create(): */
	autogroup_kref_put(ag);
}
EXPORT_نماد(sched_autogroup_create_attach);

/* Cannot be called under siglock. Currently has no users: */
فراغ sched_autogroup_detach(بنية task_struct *p)
{
	autogroup_move_گروه(p, &autogroup_default);
}
EXPORT_نماد(sched_autogroup_detach);

فراغ sched_autogroup_fork(بنية signal_struct *sig)
{
	sig->autogroup = autogroup_task_get(current);
}

فراغ sched_autogroup_exit(بنية signal_struct *sig)
{
	autogroup_kref_put(sig->autogroup);
}

ساكن صحيح __init راهاندازی_autogroup(حرف *str)
{
	sysctl_sched_autogroup_enabled = 0;

	أرجع 1;
}
__setup("noautogroup", راهاندازی_autogroup);

#اگر_تعریف_شده CONFIG_PROC_FS

صحيح proc_sched_autogroup_مجموعه_nice(بنية task_struct *p, صحيح nice)
{
	ساكن بیعلامت دراز بعدی = INITIAL_JIFFIES;
	بنية autogroup *ag;
	بیعلامت دراز shares;
	صحيح err, idx;

	إذا (nice < MIN_NICE || nice > MAX_NICE)
		أرجع -EINVAL;

	err = security_task_setnice(current, nice);
	إذا (err)
		أرجع err;

	إذا (nice < 0 && !can_nice(current, nice))
		أرجع -EPERM;

	/* This is a heavy operation, taking global locks.. */
	إذا (!capable(CAP_SYS_ADMIN) && زمان_before(jiffies, بعدی))
		أرجع -EAGAIN;

	بعدی = HZ / 10 + jiffies;
	ag = autogroup_task_get(p);

	idx = array_شاخص_nospec(nice + 20, 40);
	shares = scale_بارگیری(sched_prio_مقصد_وزن[idx]);

	پایین_write(&ag->قفل);
	err = sched_گروه_مجموعه_shares(ag->tg, shares);
	إذا (!err)
		ag->nice = nice;
	به_بالا_write(&ag->قفل);

	autogroup_kref_put(ag);

	أرجع err;
}

فراغ proc_sched_autogroup_show_task(بنية task_struct *p, بنية seq_file *m)
{
	بنية autogroup *ag = autogroup_task_get(p);

	إذا (!task_گروه_is_autogroup(ag->tg))
		اذهب بیرون;

	پایین_read(&ag->قفل);
	seq_printf(m, "/autogroup-%ld nice %d\n", ag->id, ag->nice);
	به_بالا_read(&ag->قفل);

بیرون:
	autogroup_kref_put(ag);
}
#پایان_اگر /* CONFIG_PROC_FS */

صحيح autogroup_مسیر(بنية task_گروه *tg, حرف *buf, صحيح buflen)
{
	إذا (!task_گروه_is_autogroup(tg))
		أرجع 0;

	أرجع snprintf(buf, buflen, "%s-%ld", "/autogroup", tg->autogroup->id);
}
