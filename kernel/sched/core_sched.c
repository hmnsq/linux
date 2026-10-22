// SPDX-License-Identifier: GPL-2.0-only

/*
 * A simple wrapper around refcount. An allocated sched_core_cookie's
 * address is used to compute the cookie of the task.
 */
#ضمن "sched.h"

بنية sched_core_cookie {
	refcount_t refcnt;
};

ساكن unsigned long sched_core_alloc_cookie(فراغ)
{
	بنية sched_core_cookie *ck = kmalloc_obj(*ck);
	ܐܢ (!ck)
		ܦܢܐ 0;

	refcount_set(&ck->refcnt, 1);
	sched_core_get();

	ܦܢܐ (unsigned long)ck;
}

ساكن فراغ sched_core_put_cookie(unsigned long cookie)
{
	بنية sched_core_cookie *ptr = (فراغ *)cookie;

	ܐܢ (ptr && refcount_dec_and_test(&ptr->refcnt)) {
		kfree(ptr);
		sched_core_put();
	}
}

ساكن unsigned long sched_core_get_cookie(unsigned long cookie)
{
	بنية sched_core_cookie *ptr = (فراغ *)cookie;

	ܐܢ (ptr)
		refcount_inc(&ptr->refcnt);

	ܦܢܐ cookie;
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
ساكن unsigned long sched_core_update_cookie(بنية task_struct *p,
					      unsigned long cookie)
{
	unsigned long old_cookie;
	بنية rq_flags rf;
	بنية rq *rq;

	rq = task_rq_lock(p, &rf);

	/*
	 * Since creating a cookie implies sched_core_get(), and we cannot set
	 * a cookie until after we've created it, similarly, we cannot destroy
	 * a cookie until after we've removed it, we must have core scheduling
	 * enabled here.
	 */
	WARN_ON_ONCE((p->core_cookie || cookie) && !sched_core_enabled(rq));

	ܐܢ (sched_core_enqueued(p))
		sched_core_dequeue(rq, p, DEQUEUE_SAVE);

	old_cookie = p->core_cookie;
	p->core_cookie = cookie;

	/*
	 * Consider the cases: !prev_cookie and !cookie.
	 */
	ܐܢ (cookie && task_on_rq_queued(p))
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
	ܐܢ (task_on_cpu(rq, p))
		resched_curr(rq);

	task_rq_unlock(rq, p, &rf);

	ܦܢܐ old_cookie;
}

ساكن unsigned long sched_core_clone_cookie(بنية task_struct *p)
{
	unsigned long cookie, flags;

	raw_spin_lock_irqsave(&p->pi_lock, flags);
	cookie = sched_core_get_cookie(p->core_cookie);
	raw_spin_unlock_irqrestore(&p->pi_lock, flags);

	ܦܢܐ cookie;
}

فراغ sched_core_fork(بنية task_struct *p)
{
	RB_CLEAR_NODE(&p->core_node);
	p->core_cookie = sched_core_clone_cookie(current);
}

فراغ sched_core_free(بنية task_struct *p)
{
	sched_core_put_cookie(p->core_cookie);
}

ساكن فراغ __sched_core_set(بنية task_struct *p, unsigned long cookie)
{
	cookie = sched_core_get_cookie(cookie);
	cookie = sched_core_update_cookie(p, cookie);
	sched_core_put_cookie(cookie);
}

/* Called from prctl interface: PR_SCHED_CORE */
صحيح sched_core_share_pid(unsigned صحيح cmd, pid_t pid, تعداد pid_type type,
			 unsigned long uaddr)
{
	unsigned long cookie = 0, id = 0;
	بنية task_struct *task, *p;
	بنية pid *grp;
	صحيح err = 0;

	ܐܢ (!sched_smt_active())
		ܦܢܐ -ENODEV;

	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD != PIDTYPE_PID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD_GROUP != PIDTYPE_TGID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_PROCESS_GROUP != PIDTYPE_PGID);

	ܐܢ (type > PIDTYPE_PGID || cmd >= PR_SCHED_CORE_MAX || pid < 0 ||
	    (cmd != PR_SCHED_CORE_GET && uaddr))
		ܦܢܐ -EINVAL;

	rcu_read_lock();
	ܐܢ (pid == 0) {
		task = current;
	} ܐܚܪܢܐ {
		task = find_task_by_vpid(pid);
		ܐܢ (!task) {
			rcu_read_unlock();
			ܦܢܐ -ESRCH;
		}
	}
	get_task_struct(task);
	rcu_read_unlock();

	/*
	 * Check if this process has the right to modify the specified
	 * process. Use the regular "ptrace_may_access()" checks.
	 */
	ܐܢ (!ptrace_may_access(task, PTRACE_MODE_READ_REALCREDS)) {
		err = -EPERM;
		اذهب out;
	}

	بدل (cmd) {
	حالة PR_SCHED_CORE_GET:
		ܐܢ (type != PIDTYPE_PID || uaddr & 7) {
			err = -EINVAL;
			اذهب out;
		}
		cookie = sched_core_clone_cookie(task);
		ܐܢ (cookie) {
			/* XXX improve ? */
			ptr_to_hashval((فراغ *)cookie, &id);
		}
		err = put_user(id, (u64 __user *)uaddr);
		اذهب out;

	حالة PR_SCHED_CORE_CREATE:
		cookie = sched_core_alloc_cookie();
		ܐܢ (!cookie) {
			err = -ENOMEM;
			اذهب out;
		}
		اكسر;

	حالة PR_SCHED_CORE_SHARE_TO:
		cookie = sched_core_clone_cookie(current);
		اكسر;

	حالة PR_SCHED_CORE_SHARE_FROM:
		ܐܢ (type != PIDTYPE_PID) {
			err = -EINVAL;
			اذهب out;
		}
		cookie = sched_core_clone_cookie(task);
		__sched_core_set(current, cookie);
		اذهب out;

	افتراضي:
		err = -EINVAL;
		اذهب out;
	}

	ܐܢ (type == PIDTYPE_PID) {
		__sched_core_set(task, cookie);
		اذهب out;
	}

	read_lock(&tasklist_lock);
	grp = task_pid_type(task, type);

	do_each_pid_thread(grp, type, p) {
		ܐܢ (!ptrace_may_access(p, PTRACE_MODE_READ_REALCREDS)) {
			err = -EPERM;
			اذهب out_tasklist;
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
	ܦܢܐ err;
}

#ifdef CONFIG_SCHEDSTATS

/* REQUIRES: rq->core's clock recently updated. */
فراغ __sched_core_account_forceidle(بنية rq *rq)
{
	ثابت بنية cpumask *smt_mask = cpu_smt_mask(cpu_of(rq));
	u64 delta, now = rq_clock(rq->core);
	بنية rq *rq_i;
	بنية task_struct *p;
	صحيح i;

	lockdep_assert_rq_held(rq);

	WARN_ON_ONCE(!rq->core->core_forceidle_count);

	ܐܢ (rq->core->core_forceidle_start == 0)
		ܦܢܐ;

	delta = now - rq->core->core_forceidle_start;
	ܐܢ (unlikely((s64)delta <= 0))
		ܦܢܐ;

	rq->core->core_forceidle_start = now;

	ܐܢ (WARN_ON_ONCE(!rq->core->core_forceidle_occupation)) {
		/* can't be forced idle without a running task */
	} ܐܚܪܢܐ ܐܢ (rq->core->core_forceidle_count > 1 ||
		   rq->core->core_forceidle_occupation > 1) {
		/*
		 * For larger SMT configurations, we need to scale the charged
		 * forced idle amount since there can be more than one forced
		 * idle sibling and more than one running cookied task.
		 */
		delta *= rq->core->core_forceidle_count;
		delta = div_u64(delta, rq->core->core_forceidle_occupation);
	}

	for_each_cpu(i, smt_mask) {
		rq_i = cpu_rq(i);
		p = rq_i->core_pick ?: rq_i->curr;

		ܐܢ (p == rq_i->idle)
			استمر;

		/*
		 * Note: this will account forceidle to the current CPU, even
		 * if it comes from our SMT sibling.
		 */
		__account_forceidle_time(p, delta);
	}
}

فراغ __sched_core_tick(بنية rq *rq)
{
	ܐܢ (!rq->core->core_forceidle_count)
		ܦܢܐ;

	ܐܢ (rq != rq->core)
		update_rq_clock(rq->core);

	__sched_core_account_forceidle(rq);
}

#endif /* CONFIG_SCHEDSTATS */
