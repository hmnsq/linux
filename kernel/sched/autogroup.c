// SPDX-License-Identifier: GPL-2.0

/*
 * Auto-group scheduling implementation:
 */

#شامل "autogroup.h"
#شامل "sched.h"

غیرعلامتی صحیح __read_mostly sysctl_sched_autogroup_enabled = 1;
ساکن ساخت autogroup autogroup_default;
ساکن atomic_t autogroup_seq_nr;

#اگر_تعریف_شدہ CONFIG_SYSCTL
ساکن ثابت ساخت ctl_جدول sched_autogroup_sysctls[] = {
	{
		.procname       = "sched_autogroup_enabled",
		.ڈیٹا           = &sysctl_sched_autogroup_enabled,
		.maxlen         = حجم(غیرعلامتی صحیح),
		.منوال           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

ساکن خالی __init sched_autogroup_sysctl_init(خالی)
{
	رجسٹر_sysctl_init("kernel", sched_autogroup_sysctls);
}
#ورنہ /* !CONFIG_SYSCTL: */
#تعریف_کرو sched_autogroup_sysctl_init() کرو { } جب (0)
#اگر_ختم /* !CONFIG_SYSCTL */

خالی __init autogroup_init(ساخت task_struct *init_task)
{
	autogroup_default.tg = &جڑ_task_گروہ;
	kref_init(&autogroup_default.kref);
	init_rwsem(&autogroup_default.تالا);
	init_task->اشارہ->autogroup = &autogroup_default;
	sched_autogroup_sysctl_init();
}

خالی autogroup_free(ساخت task_گروہ *tg)
{
	kfree(tg->autogroup);
}

ساکن اندرونی خالی autogroup_destroy(ساخت kref *kref)
{
	ساخت autogroup *ag = container_کا(kref, ساخت autogroup, kref);

#اگر_تعریف_شدہ CONFIG_RT_گروہ_SCHED
	/* We've redirected RT tasks to the root task group... */
	ag->tg->rt_se = تہی;
	ag->tg->rt_rq = تہی;
#اگر_ختم
	sched_release_گروہ(ag->tg);
	sched_destroy_گروہ(ag->tg);
}

ساکن اندرونی خالی autogroup_kref_put(ساخت autogroup *ag)
{
	kref_put(&ag->kref, autogroup_destroy);
}

ساکن اندرونی ساخت autogroup *autogroup_kref_get(ساخت autogroup *ag)
{
	kref_get(&ag->kref);
	واپس ag;
}

ساکن اندرونی ساخت autogroup *autogroup_task_get(ساخت task_struct *p)
{
	ساخت autogroup *ag;
	غیرعلامتی طویل flags;

	اگر (!تالا_task_sighand(p, &flags))
		واپس autogroup_kref_get(&autogroup_default);

	ag = autogroup_kref_get(p->اشارہ->autogroup);
	unlock_task_sighand(p, &flags);

	واپس ag;
}

ساکن اندرونی ساخت autogroup *autogroup_create(خالی)
{
	ساخت autogroup *ag = kzalloc_obj(*ag);
	ساخت task_گروہ *tg;

	اگر (!ag)
		جاؤ برآمد_fail;

	tg = sched_create_گروہ(&جڑ_task_گروہ);
	اگر (IS_ERR(tg))
		جاؤ برآمد_free;

	kref_init(&ag->kref);
	init_rwsem(&ag->تالا);
	ag->id = atomic_inc_return(&autogroup_seq_nr);
	ag->tg = tg;
#اگر_تعریف_شدہ CONFIG_RT_گروہ_SCHED
	/*
	 * Autogroup RT tasks are redirected to the root task group
	 * so we don't have to move tasks around upon policy change,
	 * or flail around trying to allocate bandwidth on the fly.
	 * A bandwidth exception in __sched_setscheduler() allows
	 * the policy change to proceed.
	 */
	free_rt_sched_گروہ(tg);
	tg->rt_se = جڑ_task_گروہ.rt_se;
	tg->rt_rq = جڑ_task_گروہ.rt_rq;
#اگر_ختم /* CONFIG_RT_GROUP_SCHED */
	tg->autogroup = ag;

	sched_online_گروہ(tg, &جڑ_task_گروہ);
	واپس ag;

برآمد_free:
	kfree(ag);
برآمد_fail:
	اگر (printk_ratelimit()) {
		printk(KERN_WARNING "autogroup_create: %s failure.\n",
			ag ? "sched_create_group()" : "kzalloc()");
	}

	واپس autogroup_kref_get(&autogroup_default);
}

bool task_چاہتا_autogroup(ساخت task_struct *p, ساخت task_گروہ *tg)
{
	اگر (tg != &جڑ_task_گروہ)
		واپس false;
	/*
	 * If we race with autogroup_move_group() the caller can use the old
	 * value of signal->autogroup but in this case sched_move_task() will
	 * be called again before autogroup_kref_put().
	 *
	 * However, there is no way sched_autogroup_exit_task() could tell us
	 * to avoid autogroup->tg, so we abuse PF_EXITING flag for this case.
	 */
	اگر (p->flags & PF_EXITING)
		واپس false;

	واپس true;
}

خالی sched_autogroup_exit_task(ساخت task_struct *p)
{
	/*
	 * We are going to call exit_notify() and autogroup_move_group() can't
	 * see this thread after that: we can no longer use signal->autogroup.
	 * See the PF_EXITING check in task_wants_autogroup().
	 */
	sched_move_task(p, true);
}

ساکن خالی
autogroup_move_گروہ(ساخت task_struct *p, ساخت autogroup *ag)
{
	ساخت autogroup *پچھلا;
	ساخت task_struct *t;
	غیرعلامتی طویل flags;

	اگر (WARN_ON_ONCE(!تالا_task_sighand(p, &flags)))
		واپس;

	پچھلا = p->اشارہ->autogroup;
	اگر (پچھلا == ag) {
		unlock_task_sighand(p, &flags);
		واپس;
	}

	p->اشارہ->autogroup = autogroup_kref_get(ag);
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
	autogroup_kref_put(پچھلا);
}

/* Allocates GFP_KERNEL, cannot be called under any spinlock: */
خالی sched_autogroup_create_attach(ساخت task_struct *p)
{
	ساخت autogroup *ag = autogroup_create();

	autogroup_move_گروہ(p, ag);

	/* Drop extra reference added by autogroup_create(): */
	autogroup_kref_put(ag);
}
EXPORT_علامت(sched_autogroup_create_attach);

/* Cannot be called under siglock. Currently has no users: */
خالی sched_autogroup_detach(ساخت task_struct *p)
{
	autogroup_move_گروہ(p, &autogroup_default);
}
EXPORT_علامت(sched_autogroup_detach);

خالی sched_autogroup_fork(ساخت signal_struct *sig)
{
	sig->autogroup = autogroup_task_get(current);
}

خالی sched_autogroup_exit(ساخت signal_struct *sig)
{
	autogroup_kref_put(sig->autogroup);
}

ساکن صحیح __init تیاری_autogroup(حرف *str)
{
	sysctl_sched_autogroup_enabled = 0;

	واپس 1;
}
__setup("noautogroup", تیاری_autogroup);

#اگر_تعریف_شدہ CONFIG_PROC_FS

صحیح proc_sched_autogroup_سیٹ_nice(ساخت task_struct *p, صحیح nice)
{
	ساکن غیرعلامتی طویل اگلا = INITIAL_JIFFIES;
	ساخت autogroup *ag;
	غیرعلامتی طویل shares;
	صحیح err, idx;

	اگر (nice < MIN_NICE || nice > MAX_NICE)
		واپس -EINVAL;

	err = security_task_setnice(current, nice);
	اگر (err)
		واپس err;

	اگر (nice < 0 && !can_nice(current, nice))
		واپس -EPERM;

	/* This is a heavy operation, taking global locks.. */
	اگر (!capable(CAP_SYS_ADMIN) && وقت_before(jiffies, اگلا))
		واپس -EAGAIN;

	اگلا = HZ / 10 + jiffies;
	ag = autogroup_task_get(p);

	idx = array_اشاریہ_nospec(nice + 20, 40);
	shares = scale_بھرو(sched_prio_مقصد_وزن[idx]);

	نیچے_کو_write(&ag->تالا);
	err = sched_گروہ_سیٹ_shares(ag->tg, shares);
	اگر (!err)
		ag->nice = nice;
	اوپر_write(&ag->تالا);

	autogroup_kref_put(ag);

	واپس err;
}

خالی proc_sched_autogroup_show_task(ساخت task_struct *p, ساخت seq_file *m)
{
	ساخت autogroup *ag = autogroup_task_get(p);

	اگر (!task_گروہ_is_autogroup(ag->tg))
		جاؤ برآمد;

	نیچے_کو_read(&ag->تالا);
	seq_printf(m, "/autogroup-%ld nice %d\n", ag->id, ag->nice);
	اوپر_read(&ag->تالا);

برآمد:
	autogroup_kref_put(ag);
}
#اگر_ختم /* CONFIG_PROC_FS */

صحیح autogroup_راستہ(ساخت task_گروہ *tg, حرف *buf, صحیح buflen)
{
	اگر (!task_گروہ_is_autogroup(tg))
		واپس 0;

	واپس snprintf(buf, buflen, "%s-%ld", "/autogroup", tg->autogroup->id);
}
