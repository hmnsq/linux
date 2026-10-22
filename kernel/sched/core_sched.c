// SPDX-License-Identifier: GPL-2.0-only

/*
 * A simple wrapper around refcount. An allocated sched_core_cookie's
 * address is used to compute the cookie of the task.
 */
#అన్తర్భావ "sched.h"

నిర్మాణం sched_core_cookie {
	refcount_t refcnt;
};

స్థిర గుర్తులేని దీర్ఘ sched_core_alloc_cookie(శూన్య)
{
	నిర్మాణం sched_core_cookie *ck = kmalloc_obj(*ck);
	ఒకవేళ (!ck)
		తిరిగి 0;

	refcount_సమితి(&ck->refcnt, 1);
	sched_core_get();

	తిరిగి (గుర్తులేని దీర్ఘ)ck;
}

స్థిర శూన్య sched_core_put_cookie(గుర్తులేని దీర్ఘ cookie)
{
	నిర్మాణం sched_core_cookie *ptr = (శూన్య *)cookie;

	ఒకవేళ (ptr && refcount_dec_and_test(&ptr->refcnt)) {
		kfree(ptr);
		sched_core_put();
	}
}

స్థిర గుర్తులేని దీర్ఘ sched_core_get_cookie(గుర్తులేని దీర్ఘ cookie)
{
	నిర్మాణం sched_core_cookie *ptr = (శూన్య *)cookie;

	ఒకవేళ (ptr)
		refcount_inc(&ptr->refcnt);

	తిరిగి cookie;
}

/*
 * sched_core_update_cookie - replace the cookie on a task
 * @p: the task to update
 * @cookie: the new cookie
 *
 * Effectively exchange the task cookie; caller is responsible for lifetimes on
 * both ends.
 *
 * Returns: the old cookie
 */
స్థిర గుర్తులేని దీర్ఘ sched_core_update_cookie(నిర్మాణం task_struct *p,
					      గుర్తులేని దీర్ఘ cookie)
{
	గుర్తులేని దీర్ఘ old_cookie;
	నిర్మాణం rq_flags rf;
	నిర్మాణం rq *rq;

	rq = task_rq_తాళం_2(p, &rf);

	/*
	 * Since creating a cookie implies sched_core_get(), and we cannot set
	 * a cookie until after we've created it, similarly, we cannot destroy
	 * a cookie until after we've removed it, we must have core scheduling
	 * enabled here.
	 */
	WARN_ON_ONCE((p->core_cookie || cookie) && !sched_core_enabled(rq));

	ఒకవేళ (sched_core_enqueued(p))
		sched_core_వరుస_నుండి_తీయి(rq, p, వరుస_నుండి_తీయి_SAVE);

	old_cookie = p->core_cookie;
	p->core_cookie = cookie;

	/*
	 * Consider the cases: !prev_cookie and !cookie.
	 */
	ఒకవేళ (cookie && task_on_rq_queued(p))
		sched_core_వరుసలో_చేర్చు(rq, p);

	/*
	 * If task is currently running, it may not be compatible anymore after
	 * the cookie change, so enter the scheduler on its CPU to schedule it
	 * away.
	 *
	 * Note that it is possible that as a result of this cookie change, the
	 * core has now entered/left forced idle state. Defer accounting to the
	 * next scheduling edge, rather than always forcing a reschedule here.
	 */
	ఒకవేళ (task_on_cpu(rq, p))
		resched_curr(rq);

	task_rq_unlock(rq, p, &rf);

	తిరిగి old_cookie;
}

స్థిర గుర్తులేని దీర్ఘ sched_core_clone_cookie(నిర్మాణం task_struct *p)
{
	గుర్తులేని దీర్ఘ cookie, flags;

	raw_spin_తాళం_irqsave(&p->pi_తాళం, flags);
	cookie = sched_core_get_cookie(p->core_cookie);
	raw_spin_unlock_irqrestore(&p->pi_తాళం, flags);

	తిరిగి cookie;
}

శూన్య sched_core_fork(నిర్మాణం task_struct *p)
{
	RB_CLEAR_కణుపు(&p->core_కణుపు);
	p->core_cookie = sched_core_clone_cookie(current);
}

శూన్య sched_core_free(నిర్మాణం task_struct *p)
{
	sched_core_put_cookie(p->core_cookie);
}

స్థిర శూన్య __sched_core_set(నిర్మాణం task_struct *p, గుర్తులేని దీర్ఘ cookie)
{
	cookie = sched_core_get_cookie(cookie);
	cookie = sched_core_update_cookie(p, cookie);
	sched_core_put_cookie(cookie);
}

/* Called from prctl interface: PR_SCHED_CORE */
పూర్ణాంకం sched_core_share_pid(గుర్తులేని పూర్ణాంకం cmd, pid_t pid, గణన pid_type type,
			 గుర్తులేని దీర్ఘ uaddr)
{
	గుర్తులేని దీర్ఘ cookie = 0, id = 0;
	నిర్మాణం task_struct *task, *p;
	నిర్మాణం pid *grp;
	పూర్ణాంకం err = 0;

	ఒకవేళ (!sched_smt_చురుకైన())
		తిరిగి -ENODEV;

	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD != PIDTYPE_PID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD_సమూహం != PIDTYPE_TGID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_PROCESS_సమూహం != PIDTYPE_PGID);

	ఒకవేళ (type > PIDTYPE_PGID || cmd >= PR_SCHED_CORE_MAX || pid < 0 ||
	    (cmd != PR_SCHED_CORE_GET && uaddr))
		తిరిగి -EINVAL;

	rcu_read_తాళం();
	ఒకవేళ (pid == 0) {
		task = current;
	} లేకపోతే {
		task = కనుగొను_task_by_vpid(pid);
		ఒకవేళ (!task) {
			rcu_read_unlock();
			తిరిగి -ESRCH;
		}
	}
	get_task_struct(task);
	rcu_read_unlock();

	/*
	 * Check if this process has the right to modify the specified
	 * process. Use the regular "ptrace_may_access()" checks.
	 */
	ఒకవేళ (!ptrace_may_access(task, PTRACE_బహుళకం_READ_REALCREDS)) {
		err = -EPERM;
		వెళ్ళు బహిర్గతం;
	}

	ఎంపిక (cmd) {
	స్థితి PR_SCHED_CORE_GET:
		ఒకవేళ (type != PIDTYPE_PID || uaddr & 7) {
			err = -EINVAL;
			వెళ్ళు బహిర్గతం;
		}
		cookie = sched_core_clone_cookie(task);
		ఒకవేళ (cookie) {
			/* XXX improve ? */
			ptr_గమ్యం_hashval((శూన్య *)cookie, &id);
		}
		err = put_user(id, (u64 __user *)uaddr);
		వెళ్ళు బహిర్గతం;

	స్థితి PR_SCHED_CORE_CREATE:
		cookie = sched_core_alloc_cookie();
		ఒకవేళ (!cookie) {
			err = -ENOMEM;
			వెళ్ళు బహిర్గతం;
		}
		ఆపు;

	స్థితి PR_SCHED_CORE_SHARE_గమ్యం:
		cookie = sched_core_clone_cookie(current);
		ఆపు;

	స్థితి PR_SCHED_CORE_SHARE_FROM:
		ఒకవేళ (type != PIDTYPE_PID) {
			err = -EINVAL;
			వెళ్ళు బహిర్గతం;
		}
		cookie = sched_core_clone_cookie(task);
		__sched_core_set(current, cookie);
		వెళ్ళు బహిర్గతం;

	అప్రమేయ:
		err = -EINVAL;
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (type == PIDTYPE_PID) {
		__sched_core_set(task, cookie);
		వెళ్ళు బహిర్గతం;
	}

	read_తాళం(&tasklist_తాళం);
	grp = task_pid_type(task, type);

	do_each_pid_thread(grp, type, p) {
		ఒకవేళ (!ptrace_may_access(p, PTRACE_బహుళకం_READ_REALCREDS)) {
			err = -EPERM;
			వెళ్ళు బహిర్గతం_tasklist;
		}
	} while_each_pid_thread(grp, type, p);

	do_each_pid_thread(grp, type, p) {
		__sched_core_set(p, cookie);
	} while_each_pid_thread(grp, type, p);
బహిర్గతం_tasklist:
	read_unlock(&tasklist_తాళం);

బహిర్గతం:
	sched_core_put_cookie(cookie);
	put_task_struct(task);
	తిరిగి err;
}

#నిర్వచితమైతే CONFIG_SCHEDSTATS

/* REQUIRES: rq->core's clock recently updated. */
శూన్య __sched_core_account_forceidle(నిర్మాణం rq *rq)
{
	స్థిరరాశి నిర్మాణం cpumask *smt_ముసుగు = cpu_smt_ముసుగు(cpu_యొక్క(rq));
	u64 delta, now = rq_clock(rq->core);
	నిర్మాణం rq *rq_i;
	నిర్మాణం task_struct *p;
	పూర్ణాంకం i;

	lockdep_assert_rq_held(rq);

	WARN_ON_ONCE(!rq->core->core_forceidle_లెక్క);

	ఒకవేళ (rq->core->core_forceidle_ప్రారంభం == 0)
		తిరిగి;

	delta = now - rq->core->core_forceidle_ప్రారంభం;
	ఒకవేళ (unlikely((s64)delta <= 0))
		తిరిగి;

	rq->core->core_forceidle_ప్రారంభం = now;

	ఒకవేళ (WARN_ON_ONCE(!rq->core->core_forceidle_occupation)) {
		/* can't be forced idle without a running task */
	} లేకపోతే ఒకవేళ (rq->core->core_forceidle_లెక్క > 1 ||
		   rq->core->core_forceidle_occupation > 1) {
		/*
		 * For larger SMT configurations, we need to scale the charged
		 * forced idle amount since there can be more than one forced
		 * idle sibling and more than one running cookied task.
		 */
		delta *= rq->core->core_forceidle_లెక్క;
		delta = div_u64(delta, rq->core->core_forceidle_occupation);
	}

	for_each_cpu(i, smt_ముసుగు) {
		rq_i = cpu_rq(i);
		p = rq_i->core_pick ?: rq_i->curr;

		ఒకవేళ (p == rq_i->idle)
			కొనసాగు;

		/*
		 * Note: this will account forceidle to the current CPU, even
		 * if it comes from our SMT sibling.
		 */
		__account_forceidle_time(p, delta);
	}
}

శూన్య __sched_core_tick(నిర్మాణం rq *rq)
{
	ఒకవేళ (!rq->core->core_forceidle_లెక్క)
		తిరిగి;

	ఒకవేళ (rq != rq->core)
		update_rq_clock(rq->core);

	__sched_core_account_forceidle(rq);
}

#అయితే_ముగింపు /* CONFIG_SCHEDSTATS */
