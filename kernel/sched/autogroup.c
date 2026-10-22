// SPDX-License-Identifier: GPL-2.0

/*
 * Auto-group scheduling implementation:
 */

#అన్తర్భావ "autogroup.h"
#అన్తర్భావ "sched.h"

గుర్తులేని పూర్ణాంకం __read_mostly sysctl_sched_autogroup_enabled = 1;
స్థిర నిర్మాణం autogroup autogroup_default;
స్థిర atomic_t autogroup_seq_nr;

#నిర్వచితమైతే CONFIG_SYSCTL
స్థిర స్థిరరాశి నిర్మాణం ctl_పట్టిక sched_autogroup_sysctls[] = {
	{
		.procname       = "sched_autogroup_enabled",
		.దత్తాంశం           = &sysctl_sched_autogroup_enabled,
		.maxlen         = పరిమాణం(గుర్తులేని పూర్ణాంకం),
		.బహుళకం           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

స్థిర శూన్య __init sched_autogroup_sysctl_init(శూన్య)
{
	రిజిస్టరు_sysctl_init("kernel", sched_autogroup_sysctls);
}
#లేకపోతే /* !CONFIG_SYSCTL: */
#నిర్వచించు sched_autogroup_sysctl_init() చేయి { } అయితే (0)
#అయితే_ముగింపు /* !CONFIG_SYSCTL */

శూన్య __init autogroup_init(నిర్మాణం task_struct *init_task)
{
	autogroup_default.tg = &మూలం_task_సమూహం;
	kref_init(&autogroup_default.kref);
	init_rwsem(&autogroup_default.తాళం);
	init_task->సంకేతం->autogroup = &autogroup_default;
	sched_autogroup_sysctl_init();
}

శూన్య autogroup_free(నిర్మాణం task_సమూహం *tg)
{
	kfree(tg->autogroup);
}

స్థిర అంతఃవరుస శూన్య autogroup_destroy(నిర్మాణం kref *kref)
{
	నిర్మాణం autogroup *ag = container_యొక్క(kref, నిర్మాణం autogroup, kref);

#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED
	/* We've redirected RT tasks to the root task group... */
	ag->tg->rt_se = ఖాళీ;
	ag->tg->rt_rq = ఖాళీ;
#అయితే_ముగింపు
	sched_release_సమూహం(ag->tg);
	sched_destroy_సమూహం(ag->tg);
}

స్థిర అంతఃవరుస శూన్య autogroup_kref_put(నిర్మాణం autogroup *ag)
{
	kref_put(&ag->kref, autogroup_destroy);
}

స్థిర అంతఃవరుస నిర్మాణం autogroup *autogroup_kref_get(నిర్మాణం autogroup *ag)
{
	kref_get(&ag->kref);
	తిరిగి ag;
}

స్థిర అంతఃవరుస నిర్మాణం autogroup *autogroup_task_get(నిర్మాణం task_struct *p)
{
	నిర్మాణం autogroup *ag;
	గుర్తులేని దీర్ఘ flags;

	ఒకవేళ (!తాళం_task_sighand(p, &flags))
		తిరిగి autogroup_kref_get(&autogroup_default);

	ag = autogroup_kref_get(p->సంకేతం->autogroup);
	unlock_task_sighand(p, &flags);

	తిరిగి ag;
}

స్థిర అంతఃవరుస నిర్మాణం autogroup *autogroup_create(శూన్య)
{
	నిర్మాణం autogroup *ag = kzalloc_obj(*ag);
	నిర్మాణం task_సమూహం *tg;

	ఒకవేళ (!ag)
		వెళ్ళు బహిర్గతం_fail;

	tg = sched_create_సమూహం(&మూలం_task_సమూహం);
	ఒకవేళ (IS_ERR(tg))
		వెళ్ళు బహిర్గతం_free;

	kref_init(&ag->kref);
	init_rwsem(&ag->తాళం);
	ag->id = atomic_inc_return(&autogroup_seq_nr);
	ag->tg = tg;
#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED
	/*
	 * Autogroup RT tasks are redirected to the root task group
	 * so we don't have to move tasks around upon policy change,
	 * or flail around trying to allocate bandwidth on the fly.
	 * A bandwidth exception in __sched_setscheduler() allows
	 * the policy change to proceed.
	 */
	free_rt_sched_సమూహం(tg);
	tg->rt_se = మూలం_task_సమూహం.rt_se;
	tg->rt_rq = మూలం_task_సమూహం.rt_rq;
#అయితే_ముగింపు /* CONFIG_RT_GROUP_SCHED */
	tg->autogroup = ag;

	sched_online_సమూహం(tg, &మూలం_task_సమూహం);
	తిరిగి ag;

బహిర్గతం_free:
	kfree(ag);
బహిర్గతం_fail:
	ఒకవేళ (printk_ratelimit()) {
		printk(KERN_WARNING "autogroup_create: %s failure.\n",
			ag ? "sched_create_group()" : "kzalloc()");
	}

	తిరిగి autogroup_kref_get(&autogroup_default);
}

bool task_కోరుతుంది_autogroup(నిర్మాణం task_struct *p, నిర్మాణం task_సమూహం *tg)
{
	ఒకవేళ (tg != &మూలం_task_సమూహం)
		తిరిగి false;
	/*
	 * If we race with autogroup_move_group() the caller can use the old
	 * value of signal->autogroup but in this case sched_move_task() will
	 * be called again before autogroup_kref_put().
	 *
	 * However, there is no way sched_autogroup_exit_task() could tell us
	 * to avoid autogroup->tg, so we abuse PF_EXITING flag for this case.
	 */
	ఒకవేళ (p->flags & PF_EXITING)
		తిరిగి false;

	తిరిగి true;
}

శూన్య sched_autogroup_exit_task(నిర్మాణం task_struct *p)
{
	/*
	 * We are going to call exit_notify() and autogroup_move_group() can't
	 * see this thread after that: we can no longer use signal->autogroup.
	 * See the PF_EXITING check in task_wants_autogroup().
	 */
	sched_move_task(p, true);
}

స్థిర శూన్య
autogroup_move_సమూహం(నిర్మాణం task_struct *p, నిర్మాణం autogroup *ag)
{
	నిర్మాణం autogroup *prev;
	నిర్మాణం task_struct *t;
	గుర్తులేని దీర్ఘ flags;

	ఒకవేళ (WARN_ON_ONCE(!తాళం_task_sighand(p, &flags)))
		తిరిగి;

	prev = p->సంకేతం->autogroup;
	ఒకవేళ (prev == ag) {
		unlock_task_sighand(p, &flags);
		తిరిగి;
	}

	p->సంకేతం->autogroup = autogroup_kref_get(ag);
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
శూన్య sched_autogroup_create_attach(నిర్మాణం task_struct *p)
{
	నిర్మాణం autogroup *ag = autogroup_create();

	autogroup_move_సమూహం(p, ag);

	/* Drop extra reference added by autogroup_create(): */
	autogroup_kref_put(ag);
}
EXPORT_చిహ్నం(sched_autogroup_create_attach);

/* Cannot be called under siglock. Currently has no users: */
శూన్య sched_autogroup_detach(నిర్మాణం task_struct *p)
{
	autogroup_move_సమూహం(p, &autogroup_default);
}
EXPORT_చిహ్నం(sched_autogroup_detach);

శూన్య sched_autogroup_fork(నిర్మాణం signal_struct *sig)
{
	sig->autogroup = autogroup_task_get(current);
}

శూన్య sched_autogroup_exit(నిర్మాణం signal_struct *sig)
{
	autogroup_kref_put(sig->autogroup);
}

స్థిర పూర్ణాంకం __init అమరిక_autogroup(అక్షరం *str)
{
	sysctl_sched_autogroup_enabled = 0;

	తిరిగి 1;
}
__setup("noautogroup", అమరిక_autogroup);

#నిర్వచితమైతే CONFIG_PROC_FS

పూర్ణాంకం proc_sched_autogroup_సమితి_nice(నిర్మాణం task_struct *p, పూర్ణాంకం nice)
{
	స్థిర గుర్తులేని దీర్ఘ తదుపరి = INITIAL_JIFFIES;
	నిర్మాణం autogroup *ag;
	గుర్తులేని దీర్ఘ shares;
	పూర్ణాంకం err, idx;

	ఒకవేళ (nice < MIN_NICE || nice > MAX_NICE)
		తిరిగి -EINVAL;

	err = security_task_setnice(current, nice);
	ఒకవేళ (err)
		తిరిగి err;

	ఒకవేళ (nice < 0 && !can_nice(current, nice))
		తిరిగి -EPERM;

	/* This is a heavy operation, taking global locks.. */
	ఒకవేళ (!capable(CAP_SYS_ADMIN) && సమయం_before(jiffies, తదుపరి))
		తిరిగి -EAGAIN;

	తదుపరి = HZ / 10 + jiffies;
	ag = autogroup_task_get(p);

	idx = array_సూచీ_nospec(nice + 20, 40);
	shares = scale_ఎక్కించు(sched_prio_గమ్యం_బరువు[idx]);

	కిందికి_write(&ag->తాళం);
	err = sched_సమూహం_సమితి_shares(ag->tg, shares);
	ఒకవేళ (!err)
		ag->nice = nice;
	పైకి_write(&ag->తాళం);

	autogroup_kref_put(ag);

	తిరిగి err;
}

శూన్య proc_sched_autogroup_show_task(నిర్మాణం task_struct *p, నిర్మాణం seq_file *m)
{
	నిర్మాణం autogroup *ag = autogroup_task_get(p);

	ఒకవేళ (!task_సమూహం_is_autogroup(ag->tg))
		వెళ్ళు బహిర్గతం;

	కిందికి_read(&ag->తాళం);
	seq_printf(m, "/autogroup-%ld nice %d\n", ag->id, ag->nice);
	పైకి_read(&ag->తాళం);

బహిర్గతం:
	autogroup_kref_put(ag);
}
#అయితే_ముగింపు /* CONFIG_PROC_FS */

పూర్ణాంకం autogroup_మార్గం(నిర్మాణం task_సమూహం *tg, అక్షరం *buf, పూర్ణాంకం buflen)
{
	ఒకవేళ (!task_సమూహం_is_autogroup(tg))
		తిరిగి 0;

	తిరిగి snprintf(buf, buflen, "%s-%ld", "/autogroup", tg->autogroup->id);
}
