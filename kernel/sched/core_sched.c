// SPDX-License-Identifier: GPL-2.0-only

/*
 * A simple wrapper around refcount. An allocated sched_core_cookie's
 * address is used to compute the cookie of the task.
 */
#شامل "sched.h"

ساختار sched_core_cookie {
	refcount_t refcnt;
};

ایستا unsigned long sched_core_alloc_cookie(تهی)
{
	ساختار sched_core_cookie *ck = kmalloc_obj(*ck);
	اگر (!ck)
		بازگشت 0;

	refcount_set(&ck->refcnt, 1);
	sched_core_get();

	بازگشت (unsigned long)ck;
}

ایستا تهی sched_core_put_cookie(unsigned long cookie)
{
	ساختار sched_core_cookie *ptr = (تهی *)cookie;

	اگر (ptr && refcount_dec_and_test(&ptr->refcnt)) {
		kfree(ptr);
		sched_core_put();
	}
}

ایستا unsigned long sched_core_get_cookie(unsigned long cookie)
{
	ساختار sched_core_cookie *ptr = (تهی *)cookie;

	اگر (ptr)
		refcount_inc(&ptr->refcnt);

	بازگشت cookie;
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
ایستا unsigned long sched_core_update_cookie(ساختار task_struct *p,
					      unsigned long cookie)
{
	unsigned long old_cookie;
	ساختار rq_flags rf;
	ساختار rq *rq;

	rq = task_rq_lock(p, &rf);

	/*
	 * Since creating a cookie implies sched_core_get(), and we cannot set
	 * a cookie until after we've created it, similarly, we cannot destroy
	 * a cookie until after we've removed it, we must have core scheduling
	 * enabled here.
	 */
	WARN_ON_ONCE((p->core_cookie || cookie) && !sched_core_enabled(rq));

	اگر (sched_core_enqueued(p))
		sched_core_dequeue(rq, p, DEQUEUE_SAVE);

	old_cookie = p->core_cookie;
	p->core_cookie = cookie;

	/*
	 * Consider the cases: !prev_cookie and !cookie.
	 */
	اگر (cookie && task_on_rq_queued(p))
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
	اگر (task_on_cpu(rq, p))
		resched_curr(rq);

	task_rq_unlock(rq, p, &rf);

	بازگشت old_cookie;
}

ایستا unsigned long sched_core_clone_cookie(ساختار task_struct *p)
{
	unsigned long cookie, flags;

	raw_spin_lock_irqsave(&p->pi_lock, flags);
	cookie = sched_core_get_cookie(p->core_cookie);
	raw_spin_unlock_irqrestore(&p->pi_lock, flags);

	بازگشت cookie;
}

تهی sched_core_fork(ساختار task_struct *p)
{
	RB_CLEAR_گره(&p->core_گره);
	p->core_cookie = sched_core_clone_cookie(current);
}

تهی sched_core_free(ساختار task_struct *p)
{
	sched_core_put_cookie(p->core_cookie);
}

ایستا تهی __sched_core_set(ساختار task_struct *p, unsigned long cookie)
{
	cookie = sched_core_get_cookie(cookie);
	cookie = sched_core_update_cookie(p, cookie);
	sched_core_put_cookie(cookie);
}

/* Called from prctl interface: PR_SCHED_CORE */
صحیح sched_core_share_pid(unsigned صحیح cmd, pid_t pid, شمارش pid_type type,
			 unsigned long uaddr)
{
	unsigned long cookie = 0, id = 0;
	ساختار task_struct *task, *p;
	ساختار pid *grp;
	صحیح err = 0;

	اگر (!sched_smt_active())
		بازگشت -ENODEV;

	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD != PIDTYPE_PID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD_GROUP != PIDTYPE_TGID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_PROCESS_GROUP != PIDTYPE_PGID);

	اگر (type > PIDTYPE_PGID || cmd >= PR_SCHED_CORE_MAX || pid < 0 ||
	    (cmd != PR_SCHED_CORE_GET && uaddr))
		بازگشت -EINVAL;

	rcu_read_lock();
	اگر (pid == 0) {
		task = current;
	} وگرنه {
		task = find_task_by_vpid(pid);
		اگر (!task) {
			rcu_read_unlock();
			بازگشت -ESRCH;
		}
	}
	get_task_struct(task);
	rcu_read_unlock();

	/*
	 * Check if this process has the right to modify the specified
	 * process. Use the regular "ptrace_may_access()" checks.
	 */
	اگر (!ptrace_may_access(task, PTRACE_MODE_READ_REALCREDS)) {
		err = -EPERM;
		برو out;
	}

	انتخاب (cmd) {
	حالت PR_SCHED_CORE_GET:
		اگر (type != PIDTYPE_PID || uaddr & 7) {
			err = -EINVAL;
			برو out;
		}
		cookie = sched_core_clone_cookie(task);
		اگر (cookie) {
			/* XXX improve ? */
			ptr_to_hashval((تهی *)cookie, &id);
		}
		err = put_user(id, (u64 __user *)uaddr);
		برو out;

	حالت PR_SCHED_CORE_CREATE:
		cookie = sched_core_alloc_cookie();
		اگر (!cookie) {
			err = -ENOMEM;
			برو out;
		}
		بشکن;

	حالت PR_SCHED_CORE_SHARE_TO:
		cookie = sched_core_clone_cookie(current);
		بشکن;

	حالت PR_SCHED_CORE_SHARE_FROM:
		اگر (type != PIDTYPE_PID) {
			err = -EINVAL;
			برو out;
		}
		cookie = sched_core_clone_cookie(task);
		__sched_core_set(current, cookie);
		برو out;

	پیشفرض:
		err = -EINVAL;
		برو out;
	}

	اگر (type == PIDTYPE_PID) {
		__sched_core_set(task, cookie);
		برو out;
	}

	read_lock(&tasklist_lock);
	grp = task_pid_type(task, type);

	do_each_pid_thread(grp, type, p) {
		اگر (!ptrace_may_access(p, PTRACE_MODE_READ_REALCREDS)) {
			err = -EPERM;
			برو out_tasklist;
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
	بازگشت err;
}

#ifdef CONFIG_SCHEDSTATS

/* REQUIRES: rq->core's clock recently updated. */
تهی __sched_core_account_forceidle(ساختار rq *rq)
{
	ثابت ساختار cpumask *smt_mask = cpu_smt_mask(cpu_of(rq));
	u64 delta, now = rq_clock(rq->core);
	ساختار rq *rq_i;
	ساختار task_struct *p;
	صحیح i;

	lockdep_assert_rq_held(rq);

	WARN_ON_ONCE(!rq->core->core_forceidle_شمار);

	اگر (rq->core->core_forceidle_start == 0)
		بازگشت;

	delta = now - rq->core->core_forceidle_start;
	اگر (unlikely((s64)delta <= 0))
		بازگشت;

	rq->core->core_forceidle_start = now;

	اگر (WARN_ON_ONCE(!rq->core->core_forceidle_occupation)) {
		/* can't be forced idle without a running task */
	} وگرنه اگر (rq->core->core_forceidle_شمار > 1 ||
		   rq->core->core_forceidle_occupation > 1) {
		/*
		 * For larger SMT configurations, we need to scale the charged
		 * forced idle amount since there can be more than one forced
		 * idle sibling and more than one running cookied task.
		 */
		delta *= rq->core->core_forceidle_شمار;
		delta = div_u64(delta, rq->core->core_forceidle_occupation);
	}

	for_each_cpu(i, smt_mask) {
		rq_i = cpu_rq(i);
		p = rq_i->core_pick ?: rq_i->curr;

		اگر (p == rq_i->idle)
			ادامه;

		/*
		 * Note: this will account forceidle to the current CPU, even
		 * if it comes from our SMT sibling.
		 */
		__account_forceidle_time(p, delta);
	}
}

تهی __sched_core_tick(ساختار rq *rq)
{
	اگر (!rq->core->core_forceidle_شمار)
		بازگشت;

	اگر (rq != rq->core)
		update_rq_clock(rq->core);

	__sched_core_account_forceidle(rq);
}

#endif /* CONFIG_SCHEDSTATS */
