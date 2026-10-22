// SPDX-License-Identifier: GPL-2.0-only

/*
 * A simple wrapper around refcount. An allocated sched_core_cookie's
 * address is used to compute the cookie of the task.
 */
#അന്തര്ഭാവ "sched.h"

ഘടന sched_core_cookie {
	refcount_t refcnt;
};

സ്ഥിതം unsigned long sched_core_alloc_cookie(ശൂന്യം)
{
	ഘടന sched_core_cookie *ck = kmalloc_obj(*ck);
	എങ്കിൽ (!ck)
		തിരികെ 0;

	refcount_set(&ck->refcnt, 1);
	sched_core_get();

	തിരികെ (unsigned long)ck;
}

സ്ഥിതം ശൂന്യം sched_core_put_cookie(unsigned long cookie)
{
	ഘടന sched_core_cookie *ptr = (ശൂന്യം *)cookie;

	എങ്കിൽ (ptr && refcount_dec_and_test(&ptr->refcnt)) {
		kfree(ptr);
		sched_core_put();
	}
}

സ്ഥിതം unsigned long sched_core_get_cookie(unsigned long cookie)
{
	ഘടന sched_core_cookie *ptr = (ശൂന്യം *)cookie;

	എങ്കിൽ (ptr)
		refcount_inc(&ptr->refcnt);

	തിരികെ cookie;
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
സ്ഥിതം unsigned long sched_core_update_cookie(ഘടന task_struct *p,
					      unsigned long cookie)
{
	unsigned long old_cookie;
	ഘടന rq_flags rf;
	ഘടന rq *rq;

	rq = task_rq_lock(p, &rf);

	/*
	 * Since creating a cookie implies sched_core_get(), and we cannot set
	 * a cookie until after we've created it, similarly, we cannot destroy
	 * a cookie until after we've removed it, we must have core scheduling
	 * enabled here.
	 */
	WARN_ON_ONCE((p->core_cookie || cookie) && !sched_core_enabled(rq));

	എങ്കിൽ (sched_core_enqueued(p))
		sched_core_dequeue(rq, p, DEQUEUE_SAVE);

	old_cookie = p->core_cookie;
	p->core_cookie = cookie;

	/*
	 * Consider the cases: !prev_cookie and !cookie.
	 */
	എങ്കിൽ (cookie && task_on_rq_queued(p))
		sched_core_enqueue(rq, p);

	/*
	 * If task is currently running, it may not be compatible anymore after
	 * the cookie change, so enter the scheduler on its CPU to schedule it
	 * away.
	 *
	 * Note that it is possible that as a result of this cookie change, the
	 * core has now entered/left forced idle state. Defer accounting to the
	 * next scheduling edge, rather than always forcing a reschedule here.
	 */
	എങ്കിൽ (task_on_cpu(rq, p))
		resched_curr(rq);

	task_rq_unlock(rq, p, &rf);

	തിരികെ old_cookie;
}

സ്ഥിതം unsigned long sched_core_clone_cookie(ഘടന task_struct *p)
{
	unsigned long cookie, flags;

	raw_spin_lock_irqsave(&p->pi_lock, flags);
	cookie = sched_core_get_cookie(p->core_cookie);
	raw_spin_unlock_irqrestore(&p->pi_lock, flags);

	തിരികെ cookie;
}

ശൂന്യം sched_core_fork(ഘടന task_struct *p)
{
	RB_CLEAR_കണ്ണി(&p->core_കണ്ണി);
	p->core_cookie = sched_core_clone_cookie(current);
}

ശൂന്യം sched_core_free(ഘടന task_struct *p)
{
	sched_core_put_cookie(p->core_cookie);
}

സ്ഥിതം ശൂന്യം __sched_core_set(ഘടന task_struct *p, unsigned long cookie)
{
	cookie = sched_core_get_cookie(cookie);
	cookie = sched_core_update_cookie(p, cookie);
	sched_core_put_cookie(cookie);
}

/* Called from prctl interface: PR_SCHED_CORE */
പൂർണ്ണസംഖ്യ sched_core_share_pid(unsigned പൂർണ്ണസംഖ്യ cmd, pid_t pid, എണ്ണൽ pid_type type,
			 unsigned long uaddr)
{
	unsigned long cookie = 0, id = 0;
	ഘടന task_struct *task, *p;
	ഘടന pid *grp;
	പൂർണ്ണസംഖ്യ err = 0;

	എങ്കിൽ (!sched_smt_active())
		തിരികെ -ENODEV;

	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD != PIDTYPE_PID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD_GROUP != PIDTYPE_TGID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_PROCESS_GROUP != PIDTYPE_PGID);

	എങ്കിൽ (type > PIDTYPE_PGID || cmd >= PR_SCHED_CORE_MAX || pid < 0 ||
	    (cmd != PR_SCHED_CORE_GET && uaddr))
		തിരികെ -EINVAL;

	rcu_read_lock();
	എങ്കിൽ (pid == 0) {
		task = current;
	} അല്ലെങ്കിൽ {
		task = find_task_by_vpid(pid);
		എങ്കിൽ (!task) {
			rcu_read_unlock();
			തിരികെ -ESRCH;
		}
	}
	get_task_struct(task);
	rcu_read_unlock();

	/*
	 * Check if this process has the right to modify the specified
	 * process. Use the regular "ptrace_may_access()" checks.
	 */
	എങ്കിൽ (!ptrace_may_access(task, PTRACE_MODE_READ_REALCREDS)) {
		err = -EPERM;
		പോകുക out;
	}

	തിരഞ്ഞെടുപ്പ് (cmd) {
	അവസ്ഥ PR_SCHED_CORE_GET:
		എങ്കിൽ (type != PIDTYPE_PID || uaddr & 7) {
			err = -EINVAL;
			പോകുക out;
		}
		cookie = sched_core_clone_cookie(task);
		എങ്കിൽ (cookie) {
			/* XXX improve ? */
			ptr_to_hashval((ശൂന്യം *)cookie, &id);
		}
		err = put_user(id, (u64 __user *)uaddr);
		പോകുക out;

	അവസ്ഥ PR_SCHED_CORE_CREATE:
		cookie = sched_core_alloc_cookie();
		എങ്കിൽ (!cookie) {
			err = -ENOMEM;
			പോകുക out;
		}
		നിർത്തുക;

	അവസ്ഥ PR_SCHED_CORE_SHARE_TO:
		cookie = sched_core_clone_cookie(current);
		നിർത്തുക;

	അവസ്ഥ PR_SCHED_CORE_SHARE_FROM:
		എങ്കിൽ (type != PIDTYPE_PID) {
			err = -EINVAL;
			പോകുക out;
		}
		cookie = sched_core_clone_cookie(task);
		__sched_core_set(current, cookie);
		പോകുക out;

	സ്ഥിരസ്ഥിതി:
		err = -EINVAL;
		പോകുക out;
	}

	എങ്കിൽ (type == PIDTYPE_PID) {
		__sched_core_set(task, cookie);
		പോകുക out;
	}

	read_lock(&tasklist_lock);
	grp = task_pid_type(task, type);

	do_each_pid_thread(grp, type, p) {
		എങ്കിൽ (!ptrace_may_access(p, PTRACE_MODE_READ_REALCREDS)) {
			err = -EPERM;
			പോകുക out_tasklist;
		}
	} while_each_pid_thread(grp, type, p);

	do_each_pid_thread(grp, type, p) {
		__sched_core_set(p, cookie);
	} while_each_pid_thread(grp, type, p);
out_tasklist:
	read_unlock(&tasklist_lock);

out:
	sched_core_put_cookie(cookie);
	put_task_struct(task);
	തിരികെ err;
}

#ifdef CONFIG_SCHEDSTATS

/* REQUIRES: rq->core's clock recently updated. */
ശൂന്യം __sched_core_account_forceidle(ഘടന rq *rq)
{
	സ്ഥിരം ഘടന cpumask *smt_mask = cpu_smt_mask(cpu_of(rq));
	u64 delta, now = rq_clock(rq->core);
	ഘടന rq *rq_i;
	ഘടന task_struct *p;
	പൂർണ്ണസംഖ്യ i;

	lockdep_assert_rq_held(rq);

	WARN_ON_ONCE(!rq->core->core_forceidle_എണ്ണം);

	എങ്കിൽ (rq->core->core_forceidle_start == 0)
		തിരികെ;

	delta = now - rq->core->core_forceidle_start;
	എങ്കിൽ (unlikely((s64)delta <= 0))
		തിരികെ;

	rq->core->core_forceidle_start = now;

	എങ്കിൽ (WARN_ON_ONCE(!rq->core->core_forceidle_occupation)) {
		/* can't be forced idle without a running task */
	} അല്ലെങ്കിൽ എങ്കിൽ (rq->core->core_forceidle_എണ്ണം > 1 ||
		   rq->core->core_forceidle_occupation > 1) {
		/*
		 * For larger SMT configurations, we need to scale the charged
		 * forced idle amount since there can be more than one forced
		 * idle sibling and more than one running cookied task.
		 */
		delta *= rq->core->core_forceidle_എണ്ണം;
		delta = div_u64(delta, rq->core->core_forceidle_occupation);
	}

	for_each_cpu(i, smt_mask) {
		rq_i = cpu_rq(i);
		p = rq_i->core_pick ?: rq_i->curr;

		എങ്കിൽ (p == rq_i->idle)
			തുടരുക;

		/*
		 * Note: this will account forceidle to the current CPU, even
		 * if it comes from our SMT sibling.
		 */
		__account_forceidle_time(p, delta);
	}
}

ശൂന്യം __sched_core_tick(ഘടന rq *rq)
{
	എങ്കിൽ (!rq->core->core_forceidle_എണ്ണം)
		തിരികെ;

	എങ്കിൽ (rq != rq->core)
		update_rq_clock(rq->core);

	__sched_core_account_forceidle(rq);
}

#endif /* CONFIG_SCHEDSTATS */
