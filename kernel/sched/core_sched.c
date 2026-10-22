// SPDX-License-Identifier: GPL-2.0-only

/*
 * A simple wrapper around refcount. An allocated sched_core_cookie's
 * address is used to compute the cookie of the task.
 */
#அந்தர்பாவ "sched.h"

கட்டமைப்பு sched_core_cookie {
	refcount_t refcnt;
};

நிலையான unsigned long sched_core_alloc_cookie(வெறுமை)
{
	கட்டமைப்பு sched_core_cookie *ck = kmalloc_obj(*ck);
	எனில் (!ck)
		திரும்பு 0;

	refcount_set(&ck->refcnt, 1);
	sched_core_get();

	திரும்பு (unsigned long)ck;
}

நிலையான வெறுமை sched_core_put_cookie(unsigned long cookie)
{
	கட்டமைப்பு sched_core_cookie *ptr = (வெறுமை *)cookie;

	எனில் (ptr && refcount_dec_and_test(&ptr->refcnt)) {
		kfree(ptr);
		sched_core_put();
	}
}

நிலையான unsigned long sched_core_get_cookie(unsigned long cookie)
{
	கட்டமைப்பு sched_core_cookie *ptr = (வெறுமை *)cookie;

	எனில் (ptr)
		refcount_inc(&ptr->refcnt);

	திரும்பு cookie;
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
நிலையான unsigned long sched_core_update_cookie(கட்டமைப்பு task_struct *p,
					      unsigned long cookie)
{
	unsigned long old_cookie;
	கட்டமைப்பு rq_flags rf;
	கட்டமைப்பு rq *rq;

	rq = task_rq_lock(p, &rf);

	/*
	 * Since creating a cookie implies sched_core_get(), and we cannot set
	 * a cookie until after we've created it, similarly, we cannot destroy
	 * a cookie until after we've removed it, we must have core scheduling
	 * enabled here.
	 */
	WARN_ON_ONCE((p->core_cookie || cookie) && !sched_core_enabled(rq));

	எனில் (sched_core_enqueued(p))
		sched_core_dequeue(rq, p, DEQUEUE_SAVE);

	old_cookie = p->core_cookie;
	p->core_cookie = cookie;

	/*
	 * Consider the cases: !prev_cookie and !cookie.
	 */
	எனில் (cookie && task_on_rq_queued(p))
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
	எனில் (task_on_cpu(rq, p))
		resched_curr(rq);

	task_rq_unlock(rq, p, &rf);

	திரும்பு old_cookie;
}

நிலையான unsigned long sched_core_clone_cookie(கட்டமைப்பு task_struct *p)
{
	unsigned long cookie, flags;

	raw_spin_lock_irqsave(&p->pi_lock, flags);
	cookie = sched_core_get_cookie(p->core_cookie);
	raw_spin_unlock_irqrestore(&p->pi_lock, flags);

	திரும்பு cookie;
}

வெறுமை sched_core_fork(கட்டமைப்பு task_struct *p)
{
	RB_CLEAR_முனை(&p->core_முனை);
	p->core_cookie = sched_core_clone_cookie(current);
}

வெறுமை sched_core_free(கட்டமைப்பு task_struct *p)
{
	sched_core_put_cookie(p->core_cookie);
}

நிலையான வெறுமை __sched_core_set(கட்டமைப்பு task_struct *p, unsigned long cookie)
{
	cookie = sched_core_get_cookie(cookie);
	cookie = sched_core_update_cookie(p, cookie);
	sched_core_put_cookie(cookie);
}

/* Called from prctl interface: PR_SCHED_CORE */
முழு sched_core_share_pid(unsigned முழு cmd, pid_t pid, எண்ணிக்கை pid_type type,
			 unsigned long uaddr)
{
	unsigned long cookie = 0, id = 0;
	கட்டமைப்பு task_struct *task, *p;
	கட்டமைப்பு pid *grp;
	முழு err = 0;

	எனில் (!sched_smt_active())
		திரும்பு -ENODEV;

	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD != PIDTYPE_PID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD_GROUP != PIDTYPE_TGID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_PROCESS_GROUP != PIDTYPE_PGID);

	எனில் (type > PIDTYPE_PGID || cmd >= PR_SCHED_CORE_MAX || pid < 0 ||
	    (cmd != PR_SCHED_CORE_GET && uaddr))
		திரும்பு -EINVAL;

	rcu_read_lock();
	எனில் (pid == 0) {
		task = current;
	} இல்லையெனில் {
		task = find_task_by_vpid(pid);
		எனில் (!task) {
			rcu_read_unlock();
			திரும்பு -ESRCH;
		}
	}
	get_task_struct(task);
	rcu_read_unlock();

	/*
	 * Check if this process has the right to modify the specified
	 * process. Use the regular "ptrace_may_access()" checks.
	 */
	எனில் (!ptrace_may_access(task, PTRACE_MODE_READ_REALCREDS)) {
		err = -EPERM;
		செல் out;
	}

	தேர்வு (cmd) {
	நிலை PR_SCHED_CORE_GET:
		எனில் (type != PIDTYPE_PID || uaddr & 7) {
			err = -EINVAL;
			செல் out;
		}
		cookie = sched_core_clone_cookie(task);
		எனில் (cookie) {
			/* XXX improve ? */
			ptr_to_hashval((வெறுமை *)cookie, &id);
		}
		err = put_user(id, (u64 __user *)uaddr);
		செல் out;

	நிலை PR_SCHED_CORE_CREATE:
		cookie = sched_core_alloc_cookie();
		எனில் (!cookie) {
			err = -ENOMEM;
			செல் out;
		}
		நிறுத்து;

	நிலை PR_SCHED_CORE_SHARE_TO:
		cookie = sched_core_clone_cookie(current);
		நிறுத்து;

	நிலை PR_SCHED_CORE_SHARE_FROM:
		எனில் (type != PIDTYPE_PID) {
			err = -EINVAL;
			செல் out;
		}
		cookie = sched_core_clone_cookie(task);
		__sched_core_set(current, cookie);
		செல் out;

	இயல்பு:
		err = -EINVAL;
		செல் out;
	}

	எனில் (type == PIDTYPE_PID) {
		__sched_core_set(task, cookie);
		செல் out;
	}

	read_lock(&tasklist_lock);
	grp = task_pid_type(task, type);

	do_each_pid_thread(grp, type, p) {
		எனில் (!ptrace_may_access(p, PTRACE_MODE_READ_REALCREDS)) {
			err = -EPERM;
			செல் out_tasklist;
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
	திரும்பு err;
}

#ifdef CONFIG_SCHEDSTATS

/* REQUIRES: rq->core's clock recently updated. */
வெறுமை __sched_core_account_forceidle(கட்டமைப்பு rq *rq)
{
	மாறிலி கட்டமைப்பு cpumask *smt_mask = cpu_smt_mask(cpu_of(rq));
	u64 delta, now = rq_clock(rq->core);
	கட்டமைப்பு rq *rq_i;
	கட்டமைப்பு task_struct *p;
	முழு i;

	lockdep_assert_rq_held(rq);

	WARN_ON_ONCE(!rq->core->core_forceidle_மொத்தம்);

	எனில் (rq->core->core_forceidle_start == 0)
		திரும்பு;

	delta = now - rq->core->core_forceidle_start;
	எனில் (unlikely((s64)delta <= 0))
		திரும்பு;

	rq->core->core_forceidle_start = now;

	எனில் (WARN_ON_ONCE(!rq->core->core_forceidle_occupation)) {
		/* can't be forced idle without a running task */
	} இல்லையெனில் எனில் (rq->core->core_forceidle_மொத்தம் > 1 ||
		   rq->core->core_forceidle_occupation > 1) {
		/*
		 * For larger SMT configurations, we need to scale the charged
		 * forced idle amount since there can be more than one forced
		 * idle sibling and more than one running cookied task.
		 */
		delta *= rq->core->core_forceidle_மொத்தம்;
		delta = div_u64(delta, rq->core->core_forceidle_occupation);
	}

	for_each_cpu(i, smt_mask) {
		rq_i = cpu_rq(i);
		p = rq_i->core_pick ?: rq_i->curr;

		எனில் (p == rq_i->idle)
			தொடர்;

		/*
		 * Note: this will account forceidle to the current CPU, even
		 * if it comes from our SMT sibling.
		 */
		__account_forceidle_time(p, delta);
	}
}

வெறுமை __sched_core_tick(கட்டமைப்பு rq *rq)
{
	எனில் (!rq->core->core_forceidle_மொத்தம்)
		திரும்பு;

	எனில் (rq != rq->core)
		update_rq_clock(rq->core);

	__sched_core_account_forceidle(rq);
}

#endif /* CONFIG_SCHEDSTATS */
