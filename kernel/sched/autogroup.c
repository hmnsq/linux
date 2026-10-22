// SPDX-License-Identifier: GPL-2.0

/*
 * Auto-group scheduling implementation:
 */

#அந்தர்பாவ "autogroup.h"
#அந்தர்பாவ "sched.h"

unsigned முழு __read_mostly sysctl_sched_autogroup_enabled = 1;
நிலையான கட்டமைப்பு autogroup autogroup_default;
நிலையான atomic_t autogroup_seq_nr;

#ifdef CONFIG_SYSCTL
நிலையான மாறிலி கட்டமைப்பு ctl_table sched_autogroup_sysctls[] = {
	{
		.procname       = "sched_autogroup_enabled",
		.data           = &sysctl_sched_autogroup_enabled,
		.maxlen         = அளவு(unsigned முழு),
		.mode           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

நிலையான வெறுமை __init sched_autogroup_sysctl_init(வெறுமை)
{
	register_sysctl_init("kernel", sched_autogroup_sysctls);
}
#இல்லையெனில் /* !CONFIG_SYSCTL: */
#define sched_autogroup_sysctl_init() செய் { } போது (0)
#endif /* !CONFIG_SYSCTL */

வெறுமை __init autogroup_init(கட்டமைப்பு task_struct *init_task)
{
	autogroup_default.tg = &வேர்_task_group;
	kref_init(&autogroup_default.kref);
	init_rwsem(&autogroup_default.lock);
	init_task->சமிக்ஞை->autogroup = &autogroup_default;
	sched_autogroup_sysctl_init();
}

வெறுமை autogroup_free(கட்டமைப்பு task_group *tg)
{
	kfree(tg->autogroup);
}

நிலையான உள்வரி வெறுமை autogroup_destroy(கட்டமைப்பு kref *kref)
{
	கட்டமைப்பு autogroup *ag = container_of(kref, கட்டமைப்பு autogroup, kref);

#ifdef CONFIG_RT_GROUP_SCHED
	/* We've redirected RT tasks to the root task group... */
	ag->tg->rt_se = வெற்று;
	ag->tg->rt_rq = வெற்று;
#endif
	sched_release_group(ag->tg);
	sched_destroy_group(ag->tg);
}

நிலையான உள்வரி வெறுமை autogroup_kref_put(கட்டமைப்பு autogroup *ag)
{
	kref_put(&ag->kref, autogroup_destroy);
}

நிலையான உள்வரி கட்டமைப்பு autogroup *autogroup_kref_get(கட்டமைப்பு autogroup *ag)
{
	kref_get(&ag->kref);
	திரும்பு ag;
}

நிலையான உள்வரி கட்டமைப்பு autogroup *autogroup_task_get(கட்டமைப்பு task_struct *p)
{
	கட்டமைப்பு autogroup *ag;
	unsigned long flags;

	எனில் (!lock_task_sighand(p, &flags))
		திரும்பு autogroup_kref_get(&autogroup_default);

	ag = autogroup_kref_get(p->சமிக்ஞை->autogroup);
	unlock_task_sighand(p, &flags);

	திரும்பு ag;
}

நிலையான உள்வரி கட்டமைப்பு autogroup *autogroup_create(வெறுமை)
{
	கட்டமைப்பு autogroup *ag = kzalloc_obj(*ag);
	கட்டமைப்பு task_group *tg;

	எனில் (!ag)
		செல் out_fail;

	tg = sched_create_group(&வேர்_task_group);
	எனில் (IS_ERR(tg))
		செல் out_free;

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
	tg->rt_se = வேர்_task_group.rt_se;
	tg->rt_rq = வேர்_task_group.rt_rq;
#endif /* CONFIG_RT_GROUP_SCHED */
	tg->autogroup = ag;

	sched_online_group(tg, &வேர்_task_group);
	திரும்பு ag;

out_free:
	kfree(ag);
out_fail:
	எனில் (printk_ratelimit()) {
		printk(KERN_WARNING "autogroup_create: %s failure.\n",
			ag ? "sched_create_group()" : "kzalloc()");
	}

	திரும்பு autogroup_kref_get(&autogroup_default);
}

bool task_wants_autogroup(கட்டமைப்பு task_struct *p, கட்டமைப்பு task_group *tg)
{
	எனில் (tg != &வேர்_task_group)
		திரும்பு false;
	/*
	 * If we race with autogroup_move_group() the caller can use the old
	 * value of signal->autogroup but in this case sched_move_task() will
	 * be called again before autogroup_kref_put().
	 *
	 * However, there is no way sched_autogroup_exit_task() could tell us
	 * to avoid autogroup->tg, so we abuse PF_EXITING flag for this case.
	 */
	எனில் (p->flags & PF_EXITING)
		திரும்பு false;

	திரும்பு true;
}

வெறுமை sched_autogroup_exit_task(கட்டமைப்பு task_struct *p)
{
	/*
	 * We are going to call exit_notify() and autogroup_move_group() can't
	 * see this thread after that: we can no longer use signal->autogroup.
	 * See the PF_EXITING check in task_wants_autogroup().
	 */
	sched_move_task(p, true);
}

நிலையான வெறுமை
autogroup_move_group(கட்டமைப்பு task_struct *p, கட்டமைப்பு autogroup *ag)
{
	கட்டமைப்பு autogroup *prev;
	கட்டமைப்பு task_struct *t;
	unsigned long flags;

	எனில் (WARN_ON_ONCE(!lock_task_sighand(p, &flags)))
		திரும்பு;

	prev = p->சமிக்ஞை->autogroup;
	எனில் (prev == ag) {
		unlock_task_sighand(p, &flags);
		திரும்பு;
	}

	p->சமிக்ஞை->autogroup = autogroup_kref_get(ag);
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
வெறுமை sched_autogroup_create_attach(கட்டமைப்பு task_struct *p)
{
	கட்டமைப்பு autogroup *ag = autogroup_create();

	autogroup_move_group(p, ag);

	/* Drop extra reference added by autogroup_create(): */
	autogroup_kref_put(ag);
}
EXPORT_குறியீடு(sched_autogroup_create_attach);

/* Cannot be called under siglock. Currently has no users: */
வெறுமை sched_autogroup_detach(கட்டமைப்பு task_struct *p)
{
	autogroup_move_group(p, &autogroup_default);
}
EXPORT_குறியீடு(sched_autogroup_detach);

வெறுமை sched_autogroup_fork(கட்டமைப்பு signal_struct *sig)
{
	sig->autogroup = autogroup_task_get(current);
}

வெறுமை sched_autogroup_exit(கட்டமைப்பு signal_struct *sig)
{
	autogroup_kref_put(sig->autogroup);
}

நிலையான முழு __init setup_autogroup(எழுத்து *str)
{
	sysctl_sched_autogroup_enabled = 0;

	திரும்பு 1;
}
__setup("noautogroup", setup_autogroup);

#ifdef CONFIG_PROC_FS

முழு proc_sched_autogroup_set_nice(கட்டமைப்பு task_struct *p, முழு nice)
{
	நிலையான unsigned long next = INITIAL_JIFFIES;
	கட்டமைப்பு autogroup *ag;
	unsigned long shares;
	முழு err, idx;

	எனில் (nice < MIN_NICE || nice > MAX_NICE)
		திரும்பு -EINVAL;

	err = security_task_setnice(current, nice);
	எனில் (err)
		திரும்பு err;

	எனில் (nice < 0 && !can_nice(current, nice))
		திரும்பு -EPERM;

	/* This is a heavy operation, taking global locks.. */
	எனில் (!capable(CAP_SYS_ADMIN) && time_before(jiffies, next))
		திரும்பு -EAGAIN;

	next = HZ / 10 + jiffies;
	ag = autogroup_task_get(p);

	idx = array_index_nospec(nice + 20, 40);
	shares = scale_load(sched_prio_to_weight[idx]);

	down_write(&ag->lock);
	err = sched_group_set_shares(ag->tg, shares);
	எனில் (!err)
		ag->nice = nice;
	up_write(&ag->lock);

	autogroup_kref_put(ag);

	திரும்பு err;
}

வெறுமை proc_sched_autogroup_show_task(கட்டமைப்பு task_struct *p, கட்டமைப்பு seq_file *m)
{
	கட்டமைப்பு autogroup *ag = autogroup_task_get(p);

	எனில் (!task_group_is_autogroup(ag->tg))
		செல் out;

	down_read(&ag->lock);
	seq_printf(m, "/autogroup-%ld nice %d\n", ag->id, ag->nice);
	up_read(&ag->lock);

out:
	autogroup_kref_put(ag);
}
#endif /* CONFIG_PROC_FS */

முழு autogroup_path(கட்டமைப்பு task_group *tg, எழுத்து *buf, முழு buflen)
{
	எனில் (!task_group_is_autogroup(tg))
		திரும்பு 0;

	திரும்பு snprintf(buf, buflen, "%s-%ld", "/autogroup", tg->autogroup->id);
}
