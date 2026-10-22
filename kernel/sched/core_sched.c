// SPDX-License-Identifier: GPL-2.0-only

/*
 * A simple wrapper around refcount. An allocated sched_core_cookie's
 * address is used to compute the cookie of the task.
 */
#ಅನ್ತರ್ಭಾವ "sched.h"

ರಚನೆ sched_core_cookie {
	refcount_t refcnt;
};

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ sched_core_alloc_cookie(ಶೂನ್ಯ)
{
	ರಚನೆ sched_core_cookie *ck = kmalloc_obj(*ck);
	ಆದರೆ (!ck)
		ಹಿಂತಿರುಗಿ 0;

	refcount_ಗಣ(&ck->refcnt, 1);
	sched_core_get();

	ಹಿಂತಿರುಗಿ (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)ck;
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_core_put_cookie(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cookie)
{
	ರಚನೆ sched_core_cookie *ptr = (ಶೂನ್ಯ *)cookie;

	ಆದರೆ (ptr && refcount_dec_and_test(&ptr->refcnt)) {
		kfree(ptr);
		sched_core_put();
	}
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ sched_core_get_cookie(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cookie)
{
	ರಚನೆ sched_core_cookie *ptr = (ಶೂನ್ಯ *)cookie;

	ಆದರೆ (ptr)
		refcount_inc(&ptr->refcnt);

	ಹಿಂತಿರುಗಿ cookie;
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
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ sched_core_update_cookie(ರಚನೆ task_struct *p,
					      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cookie)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ old_cookie;
	ರಚನೆ rq_flags rf;
	ರಚನೆ rq *rq;

	rq = task_rq_ಬೀಗ_2(p, &rf);

	/*
	 * Since creating a cookie implies sched_core_get(), and we cannot set
	 * a cookie until after we've created it, similarly, we cannot destroy
	 * a cookie until after we've removed it, we must have core scheduling
	 * enabled here.
	 */
	WARN_ON_ONCE((p->core_cookie || cookie) && !sched_core_enabled(rq));

	ಆದರೆ (sched_core_enqueued(p))
		sched_core_ಸರತಿಯಿಂದ_ತೆಗೆ(rq, p, ಸರತಿಯಿಂದ_ತೆಗೆ_SAVE);

	old_cookie = p->core_cookie;
	p->core_cookie = cookie;

	/*
	 * Consider the cases: !prev_cookie and !cookie.
	 */
	ಆದರೆ (cookie && task_on_rq_queued(p))
		sched_core_ಸರತಿಗೆ_ಸೇರಿಸು(rq, p);

	/*
	 * If task is currently running, it may not be compatible anymore after
	 * the cookie change, so enter the scheduler on its CPU to schedule it
	 * away.
	 *
	 * Note that it is possible that as a result of this cookie change, the
	 * core has now entered/left forced idle state. Defer accounting to the
	 * next scheduling edge, rather than always forcing a reschedule here.
	 */
	ಆದರೆ (task_on_cpu(rq, p))
		resched_curr(rq);

	task_rq_unlock(rq, p, &rf);

	ಹಿಂತಿರುಗಿ old_cookie;
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ sched_core_clone_cookie(ರಚನೆ task_struct *p)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cookie, flags;

	raw_spin_ಬೀಗ_irqsave(&p->pi_ಬೀಗ, flags);
	cookie = sched_core_get_cookie(p->core_cookie);
	raw_spin_unlock_irqrestore(&p->pi_ಬೀಗ, flags);

	ಹಿಂತಿರುಗಿ cookie;
}

ಶೂನ್ಯ sched_core_fork(ರಚನೆ task_struct *p)
{
	RB_CLEAR_ಗಂಟು(&p->core_ಗಂಟು);
	p->core_cookie = sched_core_clone_cookie(current);
}

ಶೂನ್ಯ sched_core_free(ರಚನೆ task_struct *p)
{
	sched_core_put_cookie(p->core_cookie);
}

ಸ್ಥಿರ ಶೂನ್ಯ __sched_core_set(ರಚನೆ task_struct *p, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cookie)
{
	cookie = sched_core_get_cookie(cookie);
	cookie = sched_core_update_cookie(p, cookie);
	sched_core_put_cookie(cookie);
}

/* Called from prctl interface: PR_SCHED_CORE */
ಪೂರ್ಣಾಂಕ sched_core_share_pid(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cmd, pid_t pid, ಎಣಿಕೆ pid_type type,
			 ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ uaddr)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cookie = 0, id = 0;
	ರಚನೆ task_struct *task, *p;
	ರಚನೆ pid *grp;
	ಪೂರ್ಣಾಂಕ err = 0;

	ಆದರೆ (!sched_smt_ಸಕ್ರಿಯ())
		ಹಿಂತಿರುಗಿ -ENODEV;

	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD != PIDTYPE_PID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD_ಗುಂಪು != PIDTYPE_TGID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_PROCESS_ಗುಂಪು != PIDTYPE_PGID);

	ಆದರೆ (type > PIDTYPE_PGID || cmd >= PR_SCHED_CORE_MAX || pid < 0 ||
	    (cmd != PR_SCHED_CORE_GET && uaddr))
		ಹಿಂತಿರುಗಿ -EINVAL;

	rcu_read_ಬೀಗ();
	ಆದರೆ (pid == 0) {
		task = current;
	} ಇಲ್ಲದಿದ್ದರೆ {
		task = ಕಂಡುಹಿಡಿ_task_by_vpid(pid);
		ಆದರೆ (!task) {
			rcu_read_unlock();
			ಹಿಂತಿರುಗಿ -ESRCH;
		}
	}
	get_task_struct(task);
	rcu_read_unlock();

	/*
	 * Check if this process has the right to modify the specified
	 * process. Use the regular "ptrace_may_access()" checks.
	 */
	ಆದರೆ (!ptrace_may_access(task, PTRACE_ಬಹುಲಕ_READ_REALCREDS)) {
		err = -EPERM;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆಯ್ಕೆ (cmd) {
	ಸ್ಥಿತಿ PR_SCHED_CORE_GET:
		ಆದರೆ (type != PIDTYPE_PID || uaddr & 7) {
			err = -EINVAL;
			ಹೋಗು ಹೊರಹೋಗು;
		}
		cookie = sched_core_clone_cookie(task);
		ಆದರೆ (cookie) {
			/* XXX improve ? */
			ptr_ಗಮ್ಯ_hashval((ಶೂನ್ಯ *)cookie, &id);
		}
		err = put_user(id, (u64 __user *)uaddr);
		ಹೋಗು ಹೊರಹೋಗು;

	ಸ್ಥಿತಿ PR_SCHED_CORE_CREATE:
		cookie = sched_core_alloc_cookie();
		ಆದರೆ (!cookie) {
			err = -ENOMEM;
			ಹೋಗು ಹೊರಹೋಗು;
		}
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ PR_SCHED_CORE_SHARE_ಗಮ್ಯ:
		cookie = sched_core_clone_cookie(current);
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ PR_SCHED_CORE_SHARE_FROM:
		ಆದರೆ (type != PIDTYPE_PID) {
			err = -EINVAL;
			ಹೋಗು ಹೊರಹೋಗು;
		}
		cookie = sched_core_clone_cookie(task);
		__sched_core_set(current, cookie);
		ಹೋಗು ಹೊರಹೋಗು;

	ಪೂರ್ವನಿಯೋಜಿತ:
		err = -EINVAL;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (type == PIDTYPE_PID) {
		__sched_core_set(task, cookie);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	read_ಬೀಗ(&tasklist_ಬೀಗ);
	grp = task_pid_type(task, type);

	do_each_pid_thread(grp, type, p) {
		ಆದರೆ (!ptrace_may_access(p, PTRACE_ಬಹುಲಕ_READ_REALCREDS)) {
			err = -EPERM;
			ಹೋಗು ಹೊರಹೋಗು_tasklist;
		}
	} while_each_pid_thread(grp, type, p);

	do_each_pid_thread(grp, type, p) {
		__sched_core_set(p, cookie);
	} while_each_pid_thread(grp, type, p);
ಹೊರಹೋಗು_tasklist:
	read_unlock(&tasklist_ಬೀಗ);

ಹೊರಹೋಗು:
	sched_core_put_cookie(cookie);
	put_task_struct(task);
	ಹಿಂತಿರುಗಿ err;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHEDSTATS

/* REQUIRES: rq->core's clock recently updated. */
ಶೂನ್ಯ __sched_core_account_forceidle(ರಚನೆ rq *rq)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *smt_ಮುಖವಾಡ = cpu_smt_ಮುಖವಾಡ(cpu_ದ(rq));
	u64 delta, now = rq_clock(rq->core);
	ರಚನೆ rq *rq_i;
	ರಚನೆ task_struct *p;
	ಪೂರ್ಣಾಂಕ i;

	lockdep_assert_rq_held(rq);

	WARN_ON_ONCE(!rq->core->core_forceidle_ಎಣಿಕೆ);

	ಆದರೆ (rq->core->core_forceidle_ಆರಂಭ == 0)
		ಹಿಂತಿರುಗಿ;

	delta = now - rq->core->core_forceidle_ಆರಂಭ;
	ಆದರೆ (unlikely((s64)delta <= 0))
		ಹಿಂತಿರುಗಿ;

	rq->core->core_forceidle_ಆರಂಭ = now;

	ಆದರೆ (WARN_ON_ONCE(!rq->core->core_forceidle_occupation)) {
		/* can't be forced idle without a running task */
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (rq->core->core_forceidle_ಎಣಿಕೆ > 1 ||
		   rq->core->core_forceidle_occupation > 1) {
		/*
		 * For larger SMT configurations, we need to scale the charged
		 * forced idle amount since there can be more than one forced
		 * idle sibling and more than one running cookied task.
		 */
		delta *= rq->core->core_forceidle_ಎಣಿಕೆ;
		delta = div_u64(delta, rq->core->core_forceidle_occupation);
	}

	for_each_cpu(i, smt_ಮುಖವಾಡ) {
		rq_i = cpu_rq(i);
		p = rq_i->core_pick ?: rq_i->curr;

		ಆದರೆ (p == rq_i->idle)
			ಮುಂದುವರಿ;

		/*
		 * Note: this will account forceidle to the current CPU, even
		 * if it comes from our SMT sibling.
		 */
		__account_forceidle_time(p, delta);
	}
}

ಶೂನ್ಯ __sched_core_tick(ರಚನೆ rq *rq)
{
	ಆದರೆ (!rq->core->core_forceidle_ಎಣಿಕೆ)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (rq != rq->core)
		update_rq_clock(rq->core);

	__sched_core_account_forceidle(rq);
}

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SCHEDSTATS */
