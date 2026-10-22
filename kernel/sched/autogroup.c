// SPDX-License-Identifier: GPL-2.0

/*
 * Auto-group scheduling implementation:
 */

#ଅନ୍ତର୍ଭାଵ "autogroup.h"
#ଅନ୍ତର୍ଭାଵ "sched.h"

unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା __read_mostly sysctl_sched_autogroup_enabled = 1;
ସ୍ଥିର ସଂରଚନା autogroup autogroup_default;
ସ୍ଥିର atomic_t autogroup_seq_nr;

#ifdef CONFIG_SYSCTL
ସ୍ଥିର ଅଚଳ ସଂରଚନା ctl_table sched_autogroup_sysctls[] = {
	{
		.procname       = "sched_autogroup_enabled",
		.data           = &sysctl_sched_autogroup_enabled,
		.maxlen         = ଆକାର(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା),
		.mode           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

ସ୍ଥିର ଶୂନ୍ୟ __init sched_autogroup_sysctl_init(ଶୂନ୍ୟ)
{
	register_sysctl_init("kernel", sched_autogroup_sysctls);
}
#ଅନ୍ୟଥା /* !CONFIG_SYSCTL: */
#define sched_autogroup_sysctl_init() କର { } ଯେତେବେଳେ (0)
#endif /* !CONFIG_SYSCTL */

ଶୂନ୍ୟ __init autogroup_init(ସଂରଚନା task_struct *init_task)
{
	autogroup_default.tg = &ମୂଳ_task_group;
	kref_init(&autogroup_default.kref);
	init_rwsem(&autogroup_default.lock);
	init_task->ସଙ୍କେତ->autogroup = &autogroup_default;
	sched_autogroup_sysctl_init();
}

ଶୂନ୍ୟ autogroup_free(ସଂରଚନା task_group *tg)
{
	kfree(tg->autogroup);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ autogroup_destroy(ସଂରଚନା kref *kref)
{
	ସଂରଚନା autogroup *ag = container_of(kref, ସଂରଚନା autogroup, kref);

#ifdef CONFIG_RT_GROUP_SCHED
	/* We've redirected RT tasks to the root task group... */
	ag->tg->rt_se = ଖାଲି;
	ag->tg->rt_rq = ଖାଲି;
#endif
	sched_release_group(ag->tg);
	sched_destroy_group(ag->tg);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ autogroup_kref_put(ସଂରଚନା autogroup *ag)
{
	kref_put(&ag->kref, autogroup_destroy);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା autogroup *autogroup_kref_get(ସଂରଚନା autogroup *ag)
{
	kref_get(&ag->kref);
	ଫେରନ୍ତୁ ag;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା autogroup *autogroup_task_get(ସଂରଚନା task_struct *p)
{
	ସଂରଚନା autogroup *ag;
	unsigned long flags;

	ଯଦି (!lock_task_sighand(p, &flags))
		ଫେରନ୍ତୁ autogroup_kref_get(&autogroup_default);

	ag = autogroup_kref_get(p->ସଙ୍କେତ->autogroup);
	unlock_task_sighand(p, &flags);

	ଫେରନ୍ତୁ ag;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା autogroup *autogroup_create(ଶୂନ୍ୟ)
{
	ସଂରଚନା autogroup *ag = kzalloc_obj(*ag);
	ସଂରଚନା task_group *tg;

	ଯଦି (!ag)
		ଯାଅ out_fail;

	tg = sched_create_group(&ମୂଳ_task_group);
	ଯଦି (IS_ERR(tg))
		ଯାଅ out_free;

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
	tg->rt_se = ମୂଳ_task_group.rt_se;
	tg->rt_rq = ମୂଳ_task_group.rt_rq;
#endif /* CONFIG_RT_GROUP_SCHED */
	tg->autogroup = ag;

	sched_online_group(tg, &ମୂଳ_task_group);
	ଫେରନ୍ତୁ ag;

out_free:
	kfree(ag);
out_fail:
	ଯଦି (printk_ratelimit()) {
		printk(KERN_WARNING "autogroup_create: %s failure.\n",
			ag ? "sched_create_group()" : "kzalloc()");
	}

	ଫେରନ୍ତୁ autogroup_kref_get(&autogroup_default);
}

bool task_wants_autogroup(ସଂରଚନା task_struct *p, ସଂରଚନା task_group *tg)
{
	ଯଦି (tg != &ମୂଳ_task_group)
		ଫେରନ୍ତୁ false;
	/*
	 * If we race with autogroup_move_group() the caller can use the old
	 * value of signal->autogroup but in this case sched_move_task() will
	 * be called again before autogroup_kref_put().
	 *
	 * However, there is no way sched_autogroup_exit_task() could tell us
	 * to avoid autogroup->tg, so we abuse PF_EXITING flag for this case.
	 */
	ଯଦି (p->flags & PF_EXITING)
		ଫେରନ୍ତୁ false;

	ଫେରନ୍ତୁ true;
}

ଶୂନ୍ୟ sched_autogroup_exit_task(ସଂରଚନା task_struct *p)
{
	/*
	 * We are going to call exit_notify() and autogroup_move_group() can't
	 * see this thread after that: we can no longer use signal->autogroup.
	 * See the PF_EXITING check in task_wants_autogroup().
	 */
	sched_move_task(p, true);
}

ସ୍ଥିର ଶୂନ୍ୟ
autogroup_move_group(ସଂରଚନା task_struct *p, ସଂରଚନା autogroup *ag)
{
	ସଂରଚନା autogroup *prev;
	ସଂରଚନା task_struct *t;
	unsigned long flags;

	ଯଦି (WARN_ON_ONCE(!lock_task_sighand(p, &flags)))
		ଫେରନ୍ତୁ;

	prev = p->ସଙ୍କେତ->autogroup;
	ଯଦି (prev == ag) {
		unlock_task_sighand(p, &flags);
		ଫେରନ୍ତୁ;
	}

	p->ସଙ୍କେତ->autogroup = autogroup_kref_get(ag);
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
ଶୂନ୍ୟ sched_autogroup_create_attach(ସଂରଚନା task_struct *p)
{
	ସଂରଚନା autogroup *ag = autogroup_create();

	autogroup_move_group(p, ag);

	/* Drop extra reference added by autogroup_create(): */
	autogroup_kref_put(ag);
}
EXPORT_ପ୍ରତୀକ(sched_autogroup_create_attach);

/* Cannot be called under siglock. Currently has no users: */
ଶୂନ୍ୟ sched_autogroup_detach(ସଂରଚନା task_struct *p)
{
	autogroup_move_group(p, &autogroup_default);
}
EXPORT_ପ୍ରତୀକ(sched_autogroup_detach);

ଶୂନ୍ୟ sched_autogroup_fork(ସଂରଚନା signal_struct *sig)
{
	sig->autogroup = autogroup_task_get(current);
}

ଶୂନ୍ୟ sched_autogroup_exit(ସଂରଚନା signal_struct *sig)
{
	autogroup_kref_put(sig->autogroup);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init setup_autogroup(ଅକ୍ଷର *str)
{
	sysctl_sched_autogroup_enabled = 0;

	ଫେରନ୍ତୁ 1;
}
__setup("noautogroup", setup_autogroup);

#ifdef CONFIG_PROC_FS

ପୂର୍ଣ୍ଣସଂଖ୍ୟା proc_sched_autogroup_set_nice(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା nice)
{
	ସ୍ଥିର unsigned long next = INITIAL_JIFFIES;
	ସଂରଚନା autogroup *ag;
	unsigned long shares;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err, idx;

	ଯଦି (nice < MIN_NICE || nice > MAX_NICE)
		ଫେରନ୍ତୁ -EINVAL;

	err = security_task_setnice(current, nice);
	ଯଦି (err)
		ଫେରନ୍ତୁ err;

	ଯଦି (nice < 0 && !can_nice(current, nice))
		ଫେରନ୍ତୁ -EPERM;

	/* This is a heavy operation, taking global locks.. */
	ଯଦି (!capable(CAP_SYS_ADMIN) && time_before(jiffies, next))
		ଫେରନ୍ତୁ -EAGAIN;

	next = HZ / 10 + jiffies;
	ag = autogroup_task_get(p);

	idx = array_index_nospec(nice + 20, 40);
	shares = scale_load(sched_prio_to_weight[idx]);

	down_write(&ag->lock);
	err = sched_group_set_shares(ag->tg, shares);
	ଯଦି (!err)
		ag->nice = nice;
	up_write(&ag->lock);

	autogroup_kref_put(ag);

	ଫେରନ୍ତୁ err;
}

ଶୂନ୍ୟ proc_sched_autogroup_show_task(ସଂରଚନା task_struct *p, ସଂରଚନା seq_file *m)
{
	ସଂରଚନା autogroup *ag = autogroup_task_get(p);

	ଯଦି (!task_group_is_autogroup(ag->tg))
		ଯାଅ out;

	down_read(&ag->lock);
	seq_printf(m, "/autogroup-%ld nice %d\n", ag->id, ag->nice);
	up_read(&ag->lock);

out:
	autogroup_kref_put(ag);
}
#endif /* CONFIG_PROC_FS */

ପୂର୍ଣ୍ଣସଂଖ୍ୟା autogroup_path(ସଂରଚନା task_group *tg, ଅକ୍ଷର *buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା buflen)
{
	ଯଦି (!task_group_is_autogroup(tg))
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ snprintf(buf, buflen, "%s-%ld", "/autogroup", tg->autogroup->id);
}
