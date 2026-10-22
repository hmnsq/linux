// SPDX-License-Identifier: GPL-2.0-only

/*
 * A simple wrapper around refcount. An allocated sched_core_cookie's
 * address is used to compute the cookie of the task.
 */
#અન્તર્ભાવ "sched.h"

સંરચના sched_core_cookie {
	refcount_t refcnt;
};

સ્થિર unsigned long sched_core_alloc_cookie(શૂન્ય)
{
	સંરચના sched_core_cookie *ck = kmalloc_obj(*ck);
	જો (!ck)
		પાછા 0;

	refcount_set(&ck->refcnt, 1);
	sched_core_get();

	પાછા (unsigned long)ck;
}

સ્થિર શૂન્ય sched_core_put_cookie(unsigned long cookie)
{
	સંરચના sched_core_cookie *ptr = (શૂન્ય *)cookie;

	જો (ptr && refcount_dec_and_test(&ptr->refcnt)) {
		kfree(ptr);
		sched_core_put();
	}
}

સ્થિર unsigned long sched_core_get_cookie(unsigned long cookie)
{
	સંરચના sched_core_cookie *ptr = (શૂન્ય *)cookie;

	જો (ptr)
		refcount_inc(&ptr->refcnt);

	પાછા cookie;
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
સ્થિર unsigned long sched_core_update_cookie(સંરચના task_struct *p,
					      unsigned long cookie)
{
	unsigned long old_cookie;
	સંરચના rq_flags rf;
	સંરચના rq *rq;

	rq = task_rq_lock(p, &rf);

	/*
	 * Since creating a cookie implies sched_core_get(), and we cannot set
	 * a cookie until after we've created it, similarly, we cannot destroy
	 * a cookie until after we've removed it, we must have core scheduling
	 * enabled here.
	 */
	WARN_ON_ONCE((p->core_cookie || cookie) && !sched_core_enabled(rq));

	જો (sched_core_enqueued(p))
		sched_core_dequeue(rq, p, DEQUEUE_SAVE);

	old_cookie = p->core_cookie;
	p->core_cookie = cookie;

	/*
	 * Consider the cases: !prev_cookie and !cookie.
	 */
	જો (cookie && task_on_rq_queued(p))
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
	જો (task_on_cpu(rq, p))
		resched_curr(rq);

	task_rq_unlock(rq, p, &rf);

	પાછા old_cookie;
}

સ્થિર unsigned long sched_core_clone_cookie(સંરચના task_struct *p)
{
	unsigned long cookie, flags;

	raw_spin_lock_irqsave(&p->pi_lock, flags);
	cookie = sched_core_get_cookie(p->core_cookie);
	raw_spin_unlock_irqrestore(&p->pi_lock, flags);

	પાછા cookie;
}

શૂન્ય sched_core_fork(સંરચના task_struct *p)
{
	RB_CLEAR_ગાંઠ(&p->core_ગાંઠ);
	p->core_cookie = sched_core_clone_cookie(current);
}

શૂન્ય sched_core_free(સંરચના task_struct *p)
{
	sched_core_put_cookie(p->core_cookie);
}

સ્થિર શૂન્ય __sched_core_set(સંરચના task_struct *p, unsigned long cookie)
{
	cookie = sched_core_get_cookie(cookie);
	cookie = sched_core_update_cookie(p, cookie);
	sched_core_put_cookie(cookie);
}

/* Called from prctl interface: PR_SCHED_CORE */
પૂર્ણાંક sched_core_share_pid(unsigned પૂર્ણાંક cmd, pid_t pid, ગણના pid_type type,
			 unsigned long uaddr)
{
	unsigned long cookie = 0, id = 0;
	સંરચના task_struct *task, *p;
	સંરચના pid *grp;
	પૂર્ણાંક err = 0;

	જો (!sched_smt_active())
		પાછા -ENODEV;

	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD != PIDTYPE_PID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD_GROUP != PIDTYPE_TGID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_PROCESS_GROUP != PIDTYPE_PGID);

	જો (type > PIDTYPE_PGID || cmd >= PR_SCHED_CORE_MAX || pid < 0 ||
	    (cmd != PR_SCHED_CORE_GET && uaddr))
		પાછા -EINVAL;

	rcu_read_lock();
	જો (pid == 0) {
		task = current;
	} નહીંતર {
		task = find_task_by_vpid(pid);
		જો (!task) {
			rcu_read_unlock();
			પાછા -ESRCH;
		}
	}
	get_task_struct(task);
	rcu_read_unlock();

	/*
	 * Check if this process has the right to modify the specified
	 * process. Use the regular "ptrace_may_access()" checks.
	 */
	જો (!ptrace_may_access(task, PTRACE_MODE_READ_REALCREDS)) {
		err = -EPERM;
		જાઓ out;
	}

	પસંદગી (cmd) {
	સ્થિતિ PR_SCHED_CORE_GET:
		જો (type != PIDTYPE_PID || uaddr & 7) {
			err = -EINVAL;
			જાઓ out;
		}
		cookie = sched_core_clone_cookie(task);
		જો (cookie) {
			/* XXX improve ? */
			ptr_to_hashval((શૂન્ય *)cookie, &id);
		}
		err = put_user(id, (u64 __user *)uaddr);
		જાઓ out;

	સ્થિતિ PR_SCHED_CORE_CREATE:
		cookie = sched_core_alloc_cookie();
		જો (!cookie) {
			err = -ENOMEM;
			જાઓ out;
		}
		તોડો;

	સ્થિતિ PR_SCHED_CORE_SHARE_TO:
		cookie = sched_core_clone_cookie(current);
		તોડો;

	સ્થિતિ PR_SCHED_CORE_SHARE_FROM:
		જો (type != PIDTYPE_PID) {
			err = -EINVAL;
			જાઓ out;
		}
		cookie = sched_core_clone_cookie(task);
		__sched_core_set(current, cookie);
		જાઓ out;

	મૂળભૂત:
		err = -EINVAL;
		જાઓ out;
	}

	જો (type == PIDTYPE_PID) {
		__sched_core_set(task, cookie);
		જાઓ out;
	}

	read_lock(&tasklist_lock);
	grp = task_pid_type(task, type);

	do_each_pid_thread(grp, type, p) {
		જો (!ptrace_may_access(p, PTRACE_MODE_READ_REALCREDS)) {
			err = -EPERM;
			જાઓ out_tasklist;
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
	પાછા err;
}

#ifdef CONFIG_SCHEDSTATS

/* REQUIRES: rq->core's clock recently updated. */
શૂન્ય __sched_core_account_forceidle(સંરચના rq *rq)
{
	અચલ સંરચના cpumask *smt_mask = cpu_smt_mask(cpu_of(rq));
	u64 delta, now = rq_clock(rq->core);
	સંરચના rq *rq_i;
	સંરચના task_struct *p;
	પૂર્ણાંક i;

	lockdep_assert_rq_held(rq);

	WARN_ON_ONCE(!rq->core->core_forceidle_ગણતરી);

	જો (rq->core->core_forceidle_start == 0)
		પાછા;

	delta = now - rq->core->core_forceidle_start;
	જો (unlikely((s64)delta <= 0))
		પાછા;

	rq->core->core_forceidle_start = now;

	જો (WARN_ON_ONCE(!rq->core->core_forceidle_occupation)) {
		/* can't be forced idle without a running task */
	} નહીંતર જો (rq->core->core_forceidle_ગણતરી > 1 ||
		   rq->core->core_forceidle_occupation > 1) {
		/*
		 * For larger SMT configurations, we need to scale the charged
		 * forced idle amount since there can be more than one forced
		 * idle sibling and more than one running cookied task.
		 */
		delta *= rq->core->core_forceidle_ગણતરી;
		delta = div_u64(delta, rq->core->core_forceidle_occupation);
	}

	for_each_cpu(i, smt_mask) {
		rq_i = cpu_rq(i);
		p = rq_i->core_pick ?: rq_i->curr;

		જો (p == rq_i->idle)
			ચાલુ;

		/*
		 * Note: this will account forceidle to the current CPU, even
		 * if it comes from our SMT sibling.
		 */
		__account_forceidle_time(p, delta);
	}
}

શૂન્ય __sched_core_tick(સંરચના rq *rq)
{
	જો (!rq->core->core_forceidle_ગણતરી)
		પાછા;

	જો (rq != rq->core)
		update_rq_clock(rq->core);

	__sched_core_account_forceidle(rq);
}

#endif /* CONFIG_SCHEDSTATS */
