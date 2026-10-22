// SPDX-License-Identifier: GPL-2.0

/*
 * Auto-group scheduling implementation:
 */

#समावेश "autogroup.h"
#समावेश "sched.h"

अचिन्हित पूर्णांक __read_mostly sysctl_sched_autogroup_enabled = 1;
अटल काष्ठा autogroup autogroup_default;
अटल atomic_t autogroup_seq_nr;

#अगर_घोषित CONFIG_SYSCTL
अटल स्थिर काष्ठा ctl_table sched_autogroup_sysctls[] = {
	{
		.procname       = "sched_autogroup_enabled",
		.तथ्य           = &sysctl_sched_autogroup_enabled,
		.maxlen         = माप(अचिन्हित पूर्णांक),
		.mode           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

अटल व्योम __init sched_autogroup_sysctl_init(व्योम)
{
	register_sysctl_init("kernel", sched_autogroup_sysctls);
}
#अन्यथा /* !CONFIG_SYSCTL: */
#घोषणा sched_autogroup_sysctl_init() करो { } जबतक (0)
#पूर्ण_अगर /* !CONFIG_SYSCTL */

व्योम __init autogroup_init(काष्ठा task_struct *init_task)
{
	autogroup_default.tg = &मूल_task_group;
	kref_init(&autogroup_default.kref);
	init_rwsem(&autogroup_default.lock);
	init_task->संकेत->autogroup = &autogroup_default;
	sched_autogroup_sysctl_init();
}

व्योम autogroup_free(काष्ठा task_group *tg)
{
	kfree(tg->autogroup);
}

अटल अंतरभूत व्योम autogroup_destroy(काष्ठा kref *kref)
{
	काष्ठा autogroup *ag = container_of(kref, काष्ठा autogroup, kref);

#अगर_घोषित CONFIG_RT_GROUP_SCHED
	/* We've redirected RT tasks to the root task group... */
	ag->tg->rt_se = शून्य;
	ag->tg->rt_rq = शून्य;
#पूर्ण_अगर
	sched_release_group(ag->tg);
	sched_destroy_group(ag->tg);
}

अटल अंतरभूत व्योम autogroup_kref_put(काष्ठा autogroup *ag)
{
	kref_put(&ag->kref, autogroup_destroy);
}

अटल अंतरभूत काष्ठा autogroup *autogroup_kref_get(काष्ठा autogroup *ag)
{
	kref_get(&ag->kref);
	वापस ag;
}

अटल अंतरभूत काष्ठा autogroup *autogroup_task_get(काष्ठा task_struct *p)
{
	काष्ठा autogroup *ag;
	अचिन्हित दीर्घ flags;

	अगर (!lock_task_sighand(p, &flags))
		वापस autogroup_kref_get(&autogroup_default);

	ag = autogroup_kref_get(p->संकेत->autogroup);
	unlock_task_sighand(p, &flags);

	वापस ag;
}

अटल अंतरभूत काष्ठा autogroup *autogroup_create(व्योम)
{
	काष्ठा autogroup *ag = kzalloc_obj(*ag);
	काष्ठा task_group *tg;

	अगर (!ag)
		जाओ out_fail;

	tg = sched_create_group(&मूल_task_group);
	अगर (IS_ERR(tg))
		जाओ out_free;

	kref_init(&ag->kref);
	init_rwsem(&ag->lock);
	ag->id = atomic_inc_return(&autogroup_seq_nr);
	ag->tg = tg;
#अगर_घोषित CONFIG_RT_GROUP_SCHED
	/*
	 * Autogroup RT tasks are redirected to the root task group
	 * so we don't have to move tasks around upon policy change,
	 * or flail around trying to allocate bandwidth on the fly.
	 * A bandwidth exception in __sched_setscheduler() allows
	 * the policy change to proceed.
	 */
	free_rt_sched_group(tg);
	tg->rt_se = मूल_task_group.rt_se;
	tg->rt_rq = मूल_task_group.rt_rq;
#पूर्ण_अगर /* CONFIG_RT_GROUP_SCHED */
	tg->autogroup = ag;

	sched_online_group(tg, &मूल_task_group);
	वापस ag;

out_free:
	kfree(ag);
out_fail:
	अगर (printk_ratelimit()) {
		printk(KERN_WARNING "autogroup_create: %s failure.\n",
			ag ? "sched_create_group()" : "kzalloc()");
	}

	वापस autogroup_kref_get(&autogroup_default);
}

bool task_wants_autogroup(काष्ठा task_struct *p, काष्ठा task_group *tg)
{
	अगर (tg != &मूल_task_group)
		वापस false;
	/*
	 * If we race with autogroup_move_group() the caller can use the old
	 * value of signal->autogroup but in this case sched_move_task() will
	 * be called again before autogroup_kref_put().
	 *
	 * However, there is no way sched_autogroup_exit_task() could tell us
	 * to avoid autogroup->tg, so we abuse PF_EXITING flag for this case.
	 */
	अगर (p->flags & PF_EXITING)
		वापस false;

	वापस true;
}

व्योम sched_autogroup_exit_task(काष्ठा task_struct *p)
{
	/*
	 * We are going to call exit_notify() and autogroup_move_group() can't
	 * see this thread after that: we can no longer use signal->autogroup.
	 * See the PF_EXITING check in task_wants_autogroup().
	 */
	sched_move_task(p, true);
}

अटल व्योम
autogroup_move_group(काष्ठा task_struct *p, काष्ठा autogroup *ag)
{
	काष्ठा autogroup *पूर्व;
	काष्ठा task_struct *t;
	अचिन्हित दीर्घ flags;

	अगर (WARN_ON_ONCE(!lock_task_sighand(p, &flags)))
		वापस;

	पूर्व = p->संकेत->autogroup;
	अगर (पूर्व == ag) {
		unlock_task_sighand(p, &flags);
		वापस;
	}

	p->संकेत->autogroup = autogroup_kref_get(ag);
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
	autogroup_kref_put(पूर्व);
}

/* Allocates GFP_KERNEL, cannot be called under any spinlock: */
व्योम sched_autogroup_create_attach(काष्ठा task_struct *p)
{
	काष्ठा autogroup *ag = autogroup_create();

	autogroup_move_group(p, ag);

	/* Drop extra reference added by autogroup_create(): */
	autogroup_kref_put(ag);
}
EXPORT_प्रतीक(sched_autogroup_create_attach);

/* Cannot be called under siglock. Currently has no users: */
व्योम sched_autogroup_detach(काष्ठा task_struct *p)
{
	autogroup_move_group(p, &autogroup_default);
}
EXPORT_प्रतीक(sched_autogroup_detach);

व्योम sched_autogroup_fork(काष्ठा signal_struct *sig)
{
	sig->autogroup = autogroup_task_get(current);
}

व्योम sched_autogroup_exit(काष्ठा signal_struct *sig)
{
	autogroup_kref_put(sig->autogroup);
}

अटल पूर्णांक __init setup_autogroup(अक्षर *str)
{
	sysctl_sched_autogroup_enabled = 0;

	वापस 1;
}
__setup("noautogroup", setup_autogroup);

#अगर_घोषित CONFIG_PROC_FS

पूर्णांक proc_sched_autogroup_set_nice(काष्ठा task_struct *p, पूर्णांक nice)
{
	अटल अचिन्हित दीर्घ अग्र = INITIAL_JIFFIES;
	काष्ठा autogroup *ag;
	अचिन्हित दीर्घ shares;
	पूर्णांक err, idx;

	अगर (nice < MIN_NICE || nice > MAX_NICE)
		वापस -EINVAL;

	err = security_task_setnice(current, nice);
	अगर (err)
		वापस err;

	अगर (nice < 0 && !can_nice(current, nice))
		वापस -EPERM;

	/* This is a heavy operation, taking global locks.. */
	अगर (!capable(CAP_SYS_ADMIN) && time_before(jiffies, अग्र))
		वापस -EAGAIN;

	अग्र = HZ / 10 + jiffies;
	ag = autogroup_task_get(p);

	idx = array_सूचकांक_nospec(nice + 20, 40);
	shares = scale_load(sched_prio_to_weight[idx]);

	down_write(&ag->lock);
	err = sched_group_set_shares(ag->tg, shares);
	अगर (!err)
		ag->nice = nice;
	up_write(&ag->lock);

	autogroup_kref_put(ag);

	वापस err;
}

व्योम proc_sched_autogroup_show_task(काष्ठा task_struct *p, काष्ठा seq_file *m)
{
	काष्ठा autogroup *ag = autogroup_task_get(p);

	अगर (!task_group_is_autogroup(ag->tg))
		जाओ out;

	down_read(&ag->lock);
	seq_printf(m, "/autogroup-%ld nice %d\n", ag->id, ag->nice);
	up_read(&ag->lock);

out:
	autogroup_kref_put(ag);
}
#पूर्ण_अगर /* CONFIG_PROC_FS */

पूर्णांक autogroup_path(काष्ठा task_group *tg, अक्षर *buf, पूर्णांक buflen)
{
	अगर (!task_group_is_autogroup(tg))
		वापस 0;

	वापस snprintf(buf, buflen, "%s-%ld", "/autogroup", tg->autogroup->id);
}
