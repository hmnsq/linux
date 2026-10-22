// SPDX-License-Identifier: GPL-2.0-only

/*
 * A simple wrapper around refcount. An allocated sched_core_cookie's
 * address is used to compute the cookie of the task.
 */
#অন্তর্ভাৱ "sched.h"

সংরচনা sched_core_cookie {
	refcount_t refcnt;
};

স্থির unsigned long sched_core_alloc_cookie(শূন্য)
{
	সংরচনা sched_core_cookie *ck = kmalloc_obj(*ck);
	যদি (!ck)
		ঘূৰাই 0;

	refcount_set(&ck->refcnt, 1);
	sched_core_get();

	ঘূৰাই (unsigned long)ck;
}

স্থির শূন্য sched_core_put_cookie(unsigned long cookie)
{
	সংরচনা sched_core_cookie *ptr = (শূন্য *)cookie;

	যদি (ptr && refcount_dec_and_test(&ptr->refcnt)) {
		kfree(ptr);
		sched_core_put();
	}
}

স্থির unsigned long sched_core_get_cookie(unsigned long cookie)
{
	সংরচনা sched_core_cookie *ptr = (শূন্য *)cookie;

	যদি (ptr)
		refcount_inc(&ptr->refcnt);

	ঘূৰাই cookie;
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
স্থির unsigned long sched_core_update_cookie(সংরচনা task_struct *p,
					      unsigned long cookie)
{
	unsigned long old_cookie;
	সংরচনা rq_flags rf;
	সংরচনা rq *rq;

	rq = task_rq_lock(p, &rf);

	/*
	 * Since creating a cookie implies sched_core_get(), and we cannot set
	 * a cookie until after we've created it, similarly, we cannot destroy
	 * a cookie until after we've removed it, we must have core scheduling
	 * enabled here.
	 */
	WARN_ON_ONCE((p->core_cookie || cookie) && !sched_core_enabled(rq));

	যদি (sched_core_enqueued(p))
		sched_core_dequeue(rq, p, DEQUEUE_SAVE);

	old_cookie = p->core_cookie;
	p->core_cookie = cookie;

	/*
	 * Consider the cases: !prev_cookie and !cookie.
	 */
	যদি (cookie && task_on_rq_queued(p))
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
	যদি (task_on_cpu(rq, p))
		resched_curr(rq);

	task_rq_unlock(rq, p, &rf);

	ঘূৰাই old_cookie;
}

স্থির unsigned long sched_core_clone_cookie(সংরচনা task_struct *p)
{
	unsigned long cookie, flags;

	raw_spin_lock_irqsave(&p->pi_lock, flags);
	cookie = sched_core_get_cookie(p->core_cookie);
	raw_spin_unlock_irqrestore(&p->pi_lock, flags);

	ঘূৰাই cookie;
}

শূন্য sched_core_fork(সংরচনা task_struct *p)
{
	RB_CLEAR_গ্ৰন্থি(&p->core_গ্ৰন্থি);
	p->core_cookie = sched_core_clone_cookie(current);
}

শূন্য sched_core_free(সংরচনা task_struct *p)
{
	sched_core_put_cookie(p->core_cookie);
}

স্থির শূন্য __sched_core_set(সংরচনা task_struct *p, unsigned long cookie)
{
	cookie = sched_core_get_cookie(cookie);
	cookie = sched_core_update_cookie(p, cookie);
	sched_core_put_cookie(cookie);
}

/* Called from prctl interface: PR_SCHED_CORE */
পূর্ণাঙ্ক sched_core_share_pid(unsigned পূর্ণাঙ্ক cmd, pid_t pid, গণনা pid_type type,
			 unsigned long uaddr)
{
	unsigned long cookie = 0, id = 0;
	সংরচনা task_struct *task, *p;
	সংরচনা pid *grp;
	পূর্ণাঙ্ক err = 0;

	যদি (!sched_smt_active())
		ঘূৰাই -ENODEV;

	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD != PIDTYPE_PID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD_GROUP != PIDTYPE_TGID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_PROCESS_GROUP != PIDTYPE_PGID);

	যদি (type > PIDTYPE_PGID || cmd >= PR_SCHED_CORE_MAX || pid < 0 ||
	    (cmd != PR_SCHED_CORE_GET && uaddr))
		ঘূৰাই -EINVAL;

	rcu_read_lock();
	যদি (pid == 0) {
		task = current;
	} অন্যথা {
		task = find_task_by_vpid(pid);
		যদি (!task) {
			rcu_read_unlock();
			ঘূৰাই -ESRCH;
		}
	}
	get_task_struct(task);
	rcu_read_unlock();

	/*
	 * Check if this process has the right to modify the specified
	 * process. Use the regular "ptrace_may_access()" checks.
	 */
	যদি (!ptrace_may_access(task, PTRACE_MODE_READ_REALCREDS)) {
		err = -EPERM;
		গচ্ছ out;
	}

	ৱিকল্প (cmd) {
	পক্ষ PR_SCHED_CORE_GET:
		যদি (type != PIDTYPE_PID || uaddr & 7) {
			err = -EINVAL;
			গচ্ছ out;
		}
		cookie = sched_core_clone_cookie(task);
		যদি (cookie) {
			/* XXX improve ? */
			ptr_to_hashval((শূন্য *)cookie, &id);
		}
		err = put_user(id, (u64 __user *)uaddr);
		গচ্ছ out;

	পক্ষ PR_SCHED_CORE_CREATE:
		cookie = sched_core_alloc_cookie();
		যদি (!cookie) {
			err = -ENOMEM;
			গচ্ছ out;
		}
		ভঙ্গ;

	পক্ষ PR_SCHED_CORE_SHARE_TO:
		cookie = sched_core_clone_cookie(current);
		ভঙ্গ;

	পক্ষ PR_SCHED_CORE_SHARE_FROM:
		যদি (type != PIDTYPE_PID) {
			err = -EINVAL;
			গচ্ছ out;
		}
		cookie = sched_core_clone_cookie(task);
		__sched_core_set(current, cookie);
		গচ্ছ out;

	স্ৱভাৱ:
		err = -EINVAL;
		গচ্ছ out;
	}

	যদি (type == PIDTYPE_PID) {
		__sched_core_set(task, cookie);
		গচ্ছ out;
	}

	read_lock(&tasklist_lock);
	grp = task_pid_type(task, type);

	do_each_pid_thread(grp, type, p) {
		যদি (!ptrace_may_access(p, PTRACE_MODE_READ_REALCREDS)) {
			err = -EPERM;
			গচ্ছ out_tasklist;
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
	ঘূৰাই err;
}

#ifdef CONFIG_SCHEDSTATS

/* REQUIRES: rq->core's clock recently updated. */
শূন্য __sched_core_account_forceidle(সংরচনা rq *rq)
{
	ধ্রুৱ সংরচনা cpumask *smt_mask = cpu_smt_mask(cpu_of(rq));
	u64 delta, now = rq_clock(rq->core);
	সংরচনা rq *rq_i;
	সংরচনা task_struct *p;
	পূর্ণাঙ্ক i;

	lockdep_assert_rq_held(rq);

	WARN_ON_ONCE(!rq->core->core_forceidle_গুনতি);

	যদি (rq->core->core_forceidle_start == 0)
		ঘূৰাই;

	delta = now - rq->core->core_forceidle_start;
	যদি (unlikely((s64)delta <= 0))
		ঘূৰাই;

	rq->core->core_forceidle_start = now;

	যদি (WARN_ON_ONCE(!rq->core->core_forceidle_occupation)) {
		/* can't be forced idle without a running task */
	} অন্যথা যদি (rq->core->core_forceidle_গুনতি > 1 ||
		   rq->core->core_forceidle_occupation > 1) {
		/*
		 * For larger SMT configurations, we need to scale the charged
		 * forced idle amount since there can be more than one forced
		 * idle sibling and more than one running cookied task.
		 */
		delta *= rq->core->core_forceidle_গুনতি;
		delta = div_u64(delta, rq->core->core_forceidle_occupation);
	}

	for_each_cpu(i, smt_mask) {
		rq_i = cpu_rq(i);
		p = rq_i->core_pick ?: rq_i->curr;

		যদি (p == rq_i->idle)
			অনুৱর্ত;

		/*
		 * Note: this will account forceidle to the current CPU, even
		 * if it comes from our SMT sibling.
		 */
		__account_forceidle_time(p, delta);
	}
}

শূন্য __sched_core_tick(সংরচনা rq *rq)
{
	যদি (!rq->core->core_forceidle_গুনতি)
		ঘূৰাই;

	যদি (rq != rq->core)
		update_rq_clock(rq->core);

	__sched_core_account_forceidle(rq);
}

#endif /* CONFIG_SCHEDSTATS */
