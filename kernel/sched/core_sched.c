// SPDX-License-Identifier: GPL-2.0-only

/*
 * A simple wrapper around refcount. An allocated sched_core_cookie's
 * address is used to compute the cookie of the task.
 */
#ଅନ୍ତର୍ଭାଵ "sched.h"

ସଂରଚନା sched_core_cookie {
	refcount_t refcnt;
};

ସ୍ଥିର unsigned long sched_core_alloc_cookie(ଶୂନ୍ୟ)
{
	ସଂରଚନା sched_core_cookie *ck = kmalloc_obj(*ck);
	ଯଦି (!ck)
		ଫେରନ୍ତୁ 0;

	refcount_set(&ck->refcnt, 1);
	sched_core_get();

	ଫେରନ୍ତୁ (unsigned long)ck;
}

ସ୍ଥିର ଶୂନ୍ୟ sched_core_put_cookie(unsigned long cookie)
{
	ସଂରଚନା sched_core_cookie *ptr = (ଶୂନ୍ୟ *)cookie;

	ଯଦି (ptr && refcount_dec_and_test(&ptr->refcnt)) {
		kfree(ptr);
		sched_core_put();
	}
}

ସ୍ଥିର unsigned long sched_core_get_cookie(unsigned long cookie)
{
	ସଂରଚନା sched_core_cookie *ptr = (ଶୂନ୍ୟ *)cookie;

	ଯଦି (ptr)
		refcount_inc(&ptr->refcnt);

	ଫେରନ୍ତୁ cookie;
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
ସ୍ଥିର unsigned long sched_core_update_cookie(ସଂରଚନା task_struct *p,
					      unsigned long cookie)
{
	unsigned long old_cookie;
	ସଂରଚନା rq_flags rf;
	ସଂରଚନା rq *rq;

	rq = task_rq_lock(p, &rf);

	/*
	 * Since creating a cookie implies sched_core_get(), and we cannot set
	 * a cookie until after we've created it, similarly, we cannot destroy
	 * a cookie until after we've removed it, we must have core scheduling
	 * enabled here.
	 */
	WARN_ON_ONCE((p->core_cookie || cookie) && !sched_core_enabled(rq));

	ଯଦି (sched_core_enqueued(p))
		sched_core_dequeue(rq, p, DEQUEUE_SAVE);

	old_cookie = p->core_cookie;
	p->core_cookie = cookie;

	/*
	 * Consider the cases: !prev_cookie and !cookie.
	 */
	ଯଦି (cookie && task_on_rq_queued(p))
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
	ଯଦି (task_on_cpu(rq, p))
		resched_curr(rq);

	task_rq_unlock(rq, p, &rf);

	ଫେରନ୍ତୁ old_cookie;
}

ସ୍ଥିର unsigned long sched_core_clone_cookie(ସଂରଚନା task_struct *p)
{
	unsigned long cookie, flags;

	raw_spin_lock_irqsave(&p->pi_lock, flags);
	cookie = sched_core_get_cookie(p->core_cookie);
	raw_spin_unlock_irqrestore(&p->pi_lock, flags);

	ଫେରନ୍ତୁ cookie;
}

ଶୂନ୍ୟ sched_core_fork(ସଂରଚନା task_struct *p)
{
	RB_CLEAR_ଗଣ୍ଠି(&p->core_ଗଣ୍ଠି);
	p->core_cookie = sched_core_clone_cookie(current);
}

ଶୂନ୍ୟ sched_core_free(ସଂରଚନା task_struct *p)
{
	sched_core_put_cookie(p->core_cookie);
}

ସ୍ଥିର ଶୂନ୍ୟ __sched_core_set(ସଂରଚନା task_struct *p, unsigned long cookie)
{
	cookie = sched_core_get_cookie(cookie);
	cookie = sched_core_update_cookie(p, cookie);
	sched_core_put_cookie(cookie);
}

/* Called from prctl interface: PR_SCHED_CORE */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_core_share_pid(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd, pid_t pid, ଗଣନା pid_type type,
			 unsigned long uaddr)
{
	unsigned long cookie = 0, id = 0;
	ସଂରଚନା task_struct *task, *p;
	ସଂରଚନା pid *grp;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = 0;

	ଯଦି (!sched_smt_active())
		ଫେରନ୍ତୁ -ENODEV;

	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD != PIDTYPE_PID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD_GROUP != PIDTYPE_TGID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_PROCESS_GROUP != PIDTYPE_PGID);

	ଯଦି (type > PIDTYPE_PGID || cmd >= PR_SCHED_CORE_MAX || pid < 0 ||
	    (cmd != PR_SCHED_CORE_GET && uaddr))
		ଫେରନ୍ତୁ -EINVAL;

	rcu_read_lock();
	ଯଦି (pid == 0) {
		task = current;
	} ଅନ୍ୟଥା {
		task = find_task_by_vpid(pid);
		ଯଦି (!task) {
			rcu_read_unlock();
			ଫେରନ୍ତୁ -ESRCH;
		}
	}
	get_task_struct(task);
	rcu_read_unlock();

	/*
	 * Check if this process has the right to modify the specified
	 * process. Use the regular "ptrace_may_access()" checks.
	 */
	ଯଦି (!ptrace_may_access(task, PTRACE_MODE_READ_REALCREDS)) {
		err = -EPERM;
		ଯାଅ out;
	}

	ବାଛ (cmd) {
	ସ୍ଥିତି PR_SCHED_CORE_GET:
		ଯଦି (type != PIDTYPE_PID || uaddr & 7) {
			err = -EINVAL;
			ଯାଅ out;
		}
		cookie = sched_core_clone_cookie(task);
		ଯଦି (cookie) {
			/* XXX improve ? */
			ptr_to_hashval((ଶୂନ୍ୟ *)cookie, &id);
		}
		err = put_user(id, (u64 __user *)uaddr);
		ଯାଅ out;

	ସ୍ଥିତି PR_SCHED_CORE_CREATE:
		cookie = sched_core_alloc_cookie();
		ଯଦି (!cookie) {
			err = -ENOMEM;
			ଯାଅ out;
		}
		ଭାଙ୍ଗ;

	ସ୍ଥିତି PR_SCHED_CORE_SHARE_TO:
		cookie = sched_core_clone_cookie(current);
		ଭାଙ୍ଗ;

	ସ୍ଥିତି PR_SCHED_CORE_SHARE_FROM:
		ଯଦି (type != PIDTYPE_PID) {
			err = -EINVAL;
			ଯାଅ out;
		}
		cookie = sched_core_clone_cookie(task);
		__sched_core_set(current, cookie);
		ଯାଅ out;

	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		err = -EINVAL;
		ଯାଅ out;
	}

	ଯଦି (type == PIDTYPE_PID) {
		__sched_core_set(task, cookie);
		ଯାଅ out;
	}

	read_lock(&tasklist_lock);
	grp = task_pid_type(task, type);

	do_each_pid_thread(grp, type, p) {
		ଯଦି (!ptrace_may_access(p, PTRACE_MODE_READ_REALCREDS)) {
			err = -EPERM;
			ଯାଅ out_tasklist;
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
	ଫେରନ୍ତୁ err;
}

#ifdef CONFIG_SCHEDSTATS

/* REQUIRES: rq->core's clock recently updated. */
ଶୂନ୍ୟ __sched_core_account_forceidle(ସଂରଚନା rq *rq)
{
	ଅଚଳ ସଂରଚନା cpumask *smt_mask = cpu_smt_mask(cpu_of(rq));
	u64 delta, now = rq_clock(rq->core);
	ସଂରଚନା rq *rq_i;
	ସଂରଚନା task_struct *p;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	lockdep_assert_rq_held(rq);

	WARN_ON_ONCE(!rq->core->core_forceidle_ଗଣନା);

	ଯଦି (rq->core->core_forceidle_start == 0)
		ଫେରନ୍ତୁ;

	delta = now - rq->core->core_forceidle_start;
	ଯଦି (unlikely((s64)delta <= 0))
		ଫେରନ୍ତୁ;

	rq->core->core_forceidle_start = now;

	ଯଦି (WARN_ON_ONCE(!rq->core->core_forceidle_occupation)) {
		/* can't be forced idle without a running task */
	} ଅନ୍ୟଥା ଯଦି (rq->core->core_forceidle_ଗଣନା > 1 ||
		   rq->core->core_forceidle_occupation > 1) {
		/*
		 * For larger SMT configurations, we need to scale the charged
		 * forced idle amount since there can be more than one forced
		 * idle sibling and more than one running cookied task.
		 */
		delta *= rq->core->core_forceidle_ଗଣନା;
		delta = div_u64(delta, rq->core->core_forceidle_occupation);
	}

	for_each_cpu(i, smt_mask) {
		rq_i = cpu_rq(i);
		p = rq_i->core_pick ?: rq_i->curr;

		ଯଦି (p == rq_i->idle)
			ଜାରି;

		/*
		 * Note: this will account forceidle to the current CPU, even
		 * if it comes from our SMT sibling.
		 */
		__account_forceidle_time(p, delta);
	}
}

ଶୂନ୍ୟ __sched_core_tick(ସଂରଚନା rq *rq)
{
	ଯଦି (!rq->core->core_forceidle_ଗଣନା)
		ଫେରନ୍ତୁ;

	ଯଦି (rq != rq->core)
		update_rq_clock(rq->core);

	__sched_core_account_forceidle(rq);
}

#endif /* CONFIG_SCHEDSTATS */
