// SPDX-License-Identifier: GPL-2.0

/*
 * Auto-group scheduling implementation:
 */

#અન્તર્ભાવ "autogroup.h"
#અન્તર્ભાવ "sched.h"

unsigned પૂર્ણાંક __read_mostly sysctl_sched_autogroup_enabled = 1;
સ્થિર સંરચના autogroup autogroup_default;
સ્થિર atomic_t autogroup_seq_nr;

#ifdef CONFIG_SYSCTL
સ્થિર અચલ સંરચના ctl_table sched_autogroup_sysctls[] = {
	{
		.procname       = "sched_autogroup_enabled",
		.data           = &sysctl_sched_autogroup_enabled,
		.maxlen         = કદ(unsigned પૂર્ણાંક),
		.mode           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

સ્થિર શૂન્ય __init sched_autogroup_sysctl_init(શૂન્ય)
{
	register_sysctl_init("kernel", sched_autogroup_sysctls);
}
#નહીંતર /* !CONFIG_SYSCTL: */
#define sched_autogroup_sysctl_init() કરો { } જ્યારે (0)
#endif /* !CONFIG_SYSCTL */

શૂન્ય __init autogroup_init(સંરચના task_struct *init_task)
{
	autogroup_default.tg = &મૂળ_task_group;
	kref_init(&autogroup_default.kref);
	init_rwsem(&autogroup_default.lock);
	init_task->સંકેત->autogroup = &autogroup_default;
	sched_autogroup_sysctl_init();
}

શૂન્ય autogroup_free(સંરચના task_group *tg)
{
	kfree(tg->autogroup);
}

સ્થિર અંતઃરેખ શૂન્ય autogroup_destroy(સંરચના kref *kref)
{
	સંરચના autogroup *ag = container_of(kref, સંરચના autogroup, kref);

#ifdef CONFIG_RT_GROUP_SCHED
	/* We've redirected RT tasks to the root task group... */
	ag->tg->rt_se = ખાલી;
	ag->tg->rt_rq = ખાલી;
#endif
	sched_release_group(ag->tg);
	sched_destroy_group(ag->tg);
}

સ્થિર અંતઃરેખ શૂન્ય autogroup_kref_put(સંરચના autogroup *ag)
{
	kref_put(&ag->kref, autogroup_destroy);
}

સ્થિર અંતઃરેખ સંરચના autogroup *autogroup_kref_get(સંરચના autogroup *ag)
{
	kref_get(&ag->kref);
	પાછા ag;
}

સ્થિર અંતઃરેખ સંરચના autogroup *autogroup_task_get(સંરચના task_struct *p)
{
	સંરચના autogroup *ag;
	unsigned long flags;

	જો (!lock_task_sighand(p, &flags))
		પાછા autogroup_kref_get(&autogroup_default);

	ag = autogroup_kref_get(p->સંકેત->autogroup);
	unlock_task_sighand(p, &flags);

	પાછા ag;
}

સ્થિર અંતઃરેખ સંરચના autogroup *autogroup_create(શૂન્ય)
{
	સંરચના autogroup *ag = kzalloc_obj(*ag);
	સંરચના task_group *tg;

	જો (!ag)
		જાઓ out_fail;

	tg = sched_create_group(&મૂળ_task_group);
	જો (IS_ERR(tg))
		જાઓ out_free;

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
	tg->rt_se = મૂળ_task_group.rt_se;
	tg->rt_rq = મૂળ_task_group.rt_rq;
#endif /* CONFIG_RT_GROUP_SCHED */
	tg->autogroup = ag;

	sched_online_group(tg, &મૂળ_task_group);
	પાછા ag;

out_free:
	kfree(ag);
out_fail:
	જો (printk_ratelimit()) {
		printk(KERN_WARNING "autogroup_create: %s failure.\n",
			ag ? "sched_create_group()" : "kzalloc()");
	}

	પાછા autogroup_kref_get(&autogroup_default);
}

bool task_wants_autogroup(સંરચના task_struct *p, સંરચના task_group *tg)
{
	જો (tg != &મૂળ_task_group)
		પાછા false;
	/*
	 * If we race with autogroup_move_group() the caller can use the old
	 * value of signal->autogroup but in this case sched_move_task() will
	 * be called again before autogroup_kref_put().
	 *
	 * However, there is no way sched_autogroup_exit_task() could tell us
	 * to avoid autogroup->tg, so we abuse PF_EXITING flag for this case.
	 */
	જો (p->flags & PF_EXITING)
		પાછા false;

	પાછા true;
}

શૂન્ય sched_autogroup_exit_task(સંરચના task_struct *p)
{
	/*
	 * We are going to call exit_notify() and autogroup_move_group() can't
	 * see this thread after that: we can no longer use signal->autogroup.
	 * See the PF_EXITING check in task_wants_autogroup().
	 */
	sched_move_task(p, true);
}

સ્થિર શૂન્ય
autogroup_move_group(સંરચના task_struct *p, સંરચના autogroup *ag)
{
	સંરચના autogroup *prev;
	સંરચના task_struct *t;
	unsigned long flags;

	જો (WARN_ON_ONCE(!lock_task_sighand(p, &flags)))
		પાછા;

	prev = p->સંકેત->autogroup;
	જો (prev == ag) {
		unlock_task_sighand(p, &flags);
		પાછા;
	}

	p->સંકેત->autogroup = autogroup_kref_get(ag);
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
શૂન્ય sched_autogroup_create_attach(સંરચના task_struct *p)
{
	સંરચના autogroup *ag = autogroup_create();

	autogroup_move_group(p, ag);

	/* Drop extra reference added by autogroup_create(): */
	autogroup_kref_put(ag);
}
EXPORT_પ્રતીક(sched_autogroup_create_attach);

/* Cannot be called under siglock. Currently has no users: */
શૂન્ય sched_autogroup_detach(સંરચના task_struct *p)
{
	autogroup_move_group(p, &autogroup_default);
}
EXPORT_પ્રતીક(sched_autogroup_detach);

શૂન્ય sched_autogroup_fork(સંરચના signal_struct *sig)
{
	sig->autogroup = autogroup_task_get(current);
}

શૂન્ય sched_autogroup_exit(સંરચના signal_struct *sig)
{
	autogroup_kref_put(sig->autogroup);
}

સ્થિર પૂર્ણાંક __init setup_autogroup(અક્ષર *str)
{
	sysctl_sched_autogroup_enabled = 0;

	પાછા 1;
}
__setup("noautogroup", setup_autogroup);

#ifdef CONFIG_PROC_FS

પૂર્ણાંક proc_sched_autogroup_set_nice(સંરચના task_struct *p, પૂર્ણાંક nice)
{
	સ્થિર unsigned long next = INITIAL_JIFFIES;
	સંરચના autogroup *ag;
	unsigned long shares;
	પૂર્ણાંક err, idx;

	જો (nice < MIN_NICE || nice > MAX_NICE)
		પાછા -EINVAL;

	err = security_task_setnice(current, nice);
	જો (err)
		પાછા err;

	જો (nice < 0 && !can_nice(current, nice))
		પાછા -EPERM;

	/* This is a heavy operation, taking global locks.. */
	જો (!capable(CAP_SYS_ADMIN) && time_before(jiffies, next))
		પાછા -EAGAIN;

	next = HZ / 10 + jiffies;
	ag = autogroup_task_get(p);

	idx = array_index_nospec(nice + 20, 40);
	shares = scale_load(sched_prio_to_weight[idx]);

	down_write(&ag->lock);
	err = sched_group_set_shares(ag->tg, shares);
	જો (!err)
		ag->nice = nice;
	up_write(&ag->lock);

	autogroup_kref_put(ag);

	પાછા err;
}

શૂન્ય proc_sched_autogroup_show_task(સંરચના task_struct *p, સંરચના seq_file *m)
{
	સંરચના autogroup *ag = autogroup_task_get(p);

	જો (!task_group_is_autogroup(ag->tg))
		જાઓ out;

	down_read(&ag->lock);
	seq_printf(m, "/autogroup-%ld nice %d\n", ag->id, ag->nice);
	up_read(&ag->lock);

out:
	autogroup_kref_put(ag);
}
#endif /* CONFIG_PROC_FS */

પૂર્ણાંક autogroup_path(સંરચના task_group *tg, અક્ષર *buf, પૂર્ણાંક buflen)
{
	જો (!task_group_is_autogroup(tg))
		પાછા 0;

	પાછા snprintf(buf, buflen, "%s-%ld", "/autogroup", tg->autogroup->id);
}
