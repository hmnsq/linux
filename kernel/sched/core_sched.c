// SPDX-License-Identifier: GPL-2.0-only

/*
 * A simple wrapper around refcount. An allocated sched_core_cookie's
 * address is used to compute the cookie of the task.
 */
#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"

ਬਣਤਰ sched_core_cookie {
	refcount_t refcnt;
};

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ sched_core_alloc_cookie(ਖਾਲੀ)
{
	ਬਣਤਰ sched_core_cookie *ck = kmalloc_obj(*ck);
	ਜੇ (!ck)
		ਵਾਪਸ 0;

	refcount_ਸੈੱਟ(&ck->refcnt, 1);
	sched_core_get();

	ਵਾਪਸ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)ck;
}

ਸਥਿਰ ਖਾਲੀ sched_core_put_cookie(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cookie)
{
	ਬਣਤਰ sched_core_cookie *ptr = (ਖਾਲੀ *)cookie;

	ਜੇ (ptr && refcount_dec_and_test(&ptr->refcnt)) {
		kfree(ptr);
		sched_core_put();
	}
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ sched_core_get_cookie(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cookie)
{
	ਬਣਤਰ sched_core_cookie *ptr = (ਖਾਲੀ *)cookie;

	ਜੇ (ptr)
		refcount_inc(&ptr->refcnt);

	ਵਾਪਸ cookie;
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
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ sched_core_update_cookie(ਬਣਤਰ task_struct *p,
					      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cookie)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ old_cookie;
	ਬਣਤਰ rq_flags rf;
	ਬਣਤਰ rq *rq;

	rq = task_rq_ਤਾਲਾ_2(p, &rf);

	/*
	 * Since creating a cookie implies sched_core_get(), and we cannot set
	 * a cookie until after we've created it, similarly, we cannot destroy
	 * a cookie until after we've removed it, we must have core scheduling
	 * enabled here.
	 */
	WARN_ON_ONCE((p->core_cookie || cookie) && !sched_core_enabled(rq));

	ਜੇ (sched_core_enqueued(p))
		sched_core_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(rq, p, ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SAVE);

	old_cookie = p->core_cookie;
	p->core_cookie = cookie;

	/*
	 * Consider the cases: !prev_cookie and !cookie.
	 */
	ਜੇ (cookie && task_on_rq_queued(p))
		sched_core_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(rq, p);

	/*
	 * If task is currently running, it may not be compatible anymore after
	 * the cookie change, so enter the scheduler on its CPU to schedule it
	 * away.
	 *
	 * Note that it is possible that as a result of this cookie change, the
	 * core has now entered/left forced idle state. Defer accounting to the
	 * next scheduling edge, rather than always forcing a reschedule here.
	 */
	ਜੇ (task_on_cpu(rq, p))
		resched_curr(rq);

	task_rq_unlock(rq, p, &rf);

	ਵਾਪਸ old_cookie;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ sched_core_clone_cookie(ਬਣਤਰ task_struct *p)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cookie, flags;

	raw_spin_ਤਾਲਾ_irqsave(&p->pi_ਤਾਲਾ, flags);
	cookie = sched_core_get_cookie(p->core_cookie);
	raw_spin_unlock_irqrestore(&p->pi_ਤਾਲਾ, flags);

	ਵਾਪਸ cookie;
}

ਖਾਲੀ sched_core_fork(ਬਣਤਰ task_struct *p)
{
	RB_CLEAR_ਗੰਢ(&p->core_ਗੰਢ);
	p->core_cookie = sched_core_clone_cookie(current);
}

ਖਾਲੀ sched_core_free(ਬਣਤਰ task_struct *p)
{
	sched_core_put_cookie(p->core_cookie);
}

ਸਥਿਰ ਖਾਲੀ __sched_core_set(ਬਣਤਰ task_struct *p, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cookie)
{
	cookie = sched_core_get_cookie(cookie);
	cookie = sched_core_update_cookie(p, cookie);
	sched_core_put_cookie(cookie);
}

/* Called from prctl interface: PR_SCHED_CORE */
ਪੂਰਨ sched_core_share_pid(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cmd, pid_t pid, ਗਿਣਤੀ pid_type type,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ uaddr)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cookie = 0, id = 0;
	ਬਣਤਰ task_struct *task, *p;
	ਬਣਤਰ pid *grp;
	ਪੂਰਨ err = 0;

	ਜੇ (!sched_smt_ਸਰਗਰਮ())
		ਵਾਪਸ -ENODEV;

	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD != PIDTYPE_PID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD_ਸਮੂਹ != PIDTYPE_TGID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_PROCESS_ਸਮੂਹ != PIDTYPE_PGID);

	ਜੇ (type > PIDTYPE_PGID || cmd >= PR_SCHED_CORE_MAX || pid < 0 ||
	    (cmd != PR_SCHED_CORE_GET && uaddr))
		ਵਾਪਸ -EINVAL;

	rcu_read_ਤਾਲਾ();
	ਜੇ (pid == 0) {
		task = current;
	} ਨਹੀਂਤਾਂ {
		task = ਲੱਭੋ_task_by_vpid(pid);
		ਜੇ (!task) {
			rcu_read_unlock();
			ਵਾਪਸ -ESRCH;
		}
	}
	get_task_struct(task);
	rcu_read_unlock();

	/*
	 * Check if this process has the right to modify the specified
	 * process. Use the regular "ptrace_may_access()" checks.
	 */
	ਜੇ (!ptrace_may_access(task, PTRACE_ਬਹੁਲਕ_READ_REALCREDS)) {
		err = -EPERM;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਚੋਣ (cmd) {
	ਹਾਲਤ PR_SCHED_CORE_GET:
		ਜੇ (type != PIDTYPE_PID || uaddr & 7) {
			err = -EINVAL;
			ਜਾਓ ਬਾਹਰਲਾ;
		}
		cookie = sched_core_clone_cookie(task);
		ਜੇ (cookie) {
			/* XXX improve ? */
			ptr_ਮੰਜ਼ਿਲ_hashval((ਖਾਲੀ *)cookie, &id);
		}
		err = put_user(id, (u64 __user *)uaddr);
		ਜਾਓ ਬਾਹਰਲਾ;

	ਹਾਲਤ PR_SCHED_CORE_CREATE:
		cookie = sched_core_alloc_cookie();
		ਜੇ (!cookie) {
			err = -ENOMEM;
			ਜਾਓ ਬਾਹਰਲਾ;
		}
		ਰੋਕੋ;

	ਹਾਲਤ PR_SCHED_CORE_SHARE_ਮੰਜ਼ਿਲ:
		cookie = sched_core_clone_cookie(current);
		ਰੋਕੋ;

	ਹਾਲਤ PR_SCHED_CORE_SHARE_FROM:
		ਜੇ (type != PIDTYPE_PID) {
			err = -EINVAL;
			ਜਾਓ ਬਾਹਰਲਾ;
		}
		cookie = sched_core_clone_cookie(task);
		__sched_core_set(current, cookie);
		ਜਾਓ ਬਾਹਰਲਾ;

	ਮੂਲ:
		err = -EINVAL;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (type == PIDTYPE_PID) {
		__sched_core_set(task, cookie);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	read_ਤਾਲਾ(&tasklist_ਤਾਲਾ);
	grp = task_pid_type(task, type);

	do_each_pid_thread(grp, type, p) {
		ਜੇ (!ptrace_may_access(p, PTRACE_ਬਹੁਲਕ_READ_REALCREDS)) {
			err = -EPERM;
			ਜਾਓ ਬਾਹਰਲਾ_tasklist;
		}
	} while_each_pid_thread(grp, type, p);

	do_each_pid_thread(grp, type, p) {
		__sched_core_set(p, cookie);
	} while_each_pid_thread(grp, type, p);
ਬਾਹਰਲਾ_tasklist:
	read_unlock(&tasklist_ਤਾਲਾ);

ਬਾਹਰਲਾ:
	sched_core_put_cookie(cookie);
	put_task_struct(task);
	ਵਾਪਸ err;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHEDSTATS

/* REQUIRES: rq->core's clock recently updated. */
ਖਾਲੀ __sched_core_account_forceidle(ਬਣਤਰ rq *rq)
{
	ਅਚਲ ਬਣਤਰ cpumask *smt_ਨਕਾਬ = cpu_smt_ਨਕਾਬ(cpu_ਦਾ(rq));
	u64 delta, now = rq_clock(rq->core);
	ਬਣਤਰ rq *rq_i;
	ਬਣਤਰ task_struct *p;
	ਪੂਰਨ i;

	lockdep_assert_rq_held(rq);

	WARN_ON_ONCE(!rq->core->core_forceidle_ਗਿਣਤੀ);

	ਜੇ (rq->core->core_forceidle_ਸ਼ੁਰੂ == 0)
		ਵਾਪਸ;

	delta = now - rq->core->core_forceidle_ਸ਼ੁਰੂ;
	ਜੇ (unlikely((s64)delta <= 0))
		ਵਾਪਸ;

	rq->core->core_forceidle_ਸ਼ੁਰੂ = now;

	ਜੇ (WARN_ON_ONCE(!rq->core->core_forceidle_occupation)) {
		/* can't be forced idle without a running task */
	} ਨਹੀਂਤਾਂ ਜੇ (rq->core->core_forceidle_ਗਿਣਤੀ > 1 ||
		   rq->core->core_forceidle_occupation > 1) {
		/*
		 * For larger SMT configurations, we need to scale the charged
		 * forced idle amount since there can be more than one forced
		 * idle sibling and more than one running cookied task.
		 */
		delta *= rq->core->core_forceidle_ਗਿਣਤੀ;
		delta = div_u64(delta, rq->core->core_forceidle_occupation);
	}

	for_each_cpu(i, smt_ਨਕਾਬ) {
		rq_i = cpu_rq(i);
		p = rq_i->core_pick ?: rq_i->curr;

		ਜੇ (p == rq_i->idle)
			ਜਾਰੀ;

		/*
		 * Note: this will account forceidle to the current CPU, even
		 * if it comes from our SMT sibling.
		 */
		__account_forceidle_time(p, delta);
	}
}

ਖਾਲੀ __sched_core_tick(ਬਣਤਰ rq *rq)
{
	ਜੇ (!rq->core->core_forceidle_ਗਿਣਤੀ)
		ਵਾਪਸ;

	ਜੇ (rq != rq->core)
		update_rq_clock(rq->core);

	__sched_core_account_forceidle(rq);
}

#ਜੇ_ਅੰਤ /* CONFIG_SCHEDSTATS */
