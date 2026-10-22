// SPDX-License-Identifier: GPL-2.0-only

/*
 * A simple wrapper around refcount. An allocated sched_core_cookie's
 * address is used to compute the cookie of the task.
 */
#अन्तर्भाव "sched.h"

संरचना sched_core_cookie {
	refcount_t refcnt;
};

स्थिर अचिन्हित दीर्घ sched_core_alloc_cookie(शून्य)
{
	संरचना sched_core_cookie *ck = kmalloc_obj(*ck);
	जर (!ck)
		परत 0;

	refcount_set(&ck->refcnt, 1);
	sched_core_get();

	परत (अचिन्हित दीर्घ)ck;
}

स्थिर शून्य sched_core_put_cookie(अचिन्हित दीर्घ cookie)
{
	संरचना sched_core_cookie *ptr = (शून्य *)cookie;

	जर (ptr && refcount_dec_and_test(&ptr->refcnt)) {
		kfree(ptr);
		sched_core_put();
	}
}

स्थिर अचिन्हित दीर्घ sched_core_get_cookie(अचिन्हित दीर्घ cookie)
{
	संरचना sched_core_cookie *ptr = (शून्य *)cookie;

	जर (ptr)
		refcount_inc(&ptr->refcnt);

	परत cookie;
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
स्थिर अचिन्हित दीर्घ sched_core_update_cookie(संरचना task_struct *p,
					      अचिन्हित दीर्घ cookie)
{
	अचिन्हित दीर्घ old_cookie;
	संरचना rq_flags rf;
	संरचना rq *rq;

	rq = task_rq_lock(p, &rf);

	/*
	 * Since creating a cookie implies sched_core_get(), and we cannot set
	 * a cookie until after we've created it, similarly, we cannot destroy
	 * a cookie until after we've removed it, we must have core scheduling
	 * enabled here.
	 */
	WARN_ON_ONCE((p->core_cookie || cookie) && !sched_core_enabled(rq));

	जर (sched_core_enqueued(p))
		sched_core_dequeue(rq, p, DEQUEUE_SAVE);

	old_cookie = p->core_cookie;
	p->core_cookie = cookie;

	/*
	 * Consider the cases: !prev_cookie and !cookie.
	 */
	जर (cookie && task_on_rq_queued(p))
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
	जर (task_on_cpu(rq, p))
		resched_curr(rq);

	task_rq_unlock(rq, p, &rf);

	परत old_cookie;
}

स्थिर अचिन्हित दीर्घ sched_core_clone_cookie(संरचना task_struct *p)
{
	अचिन्हित दीर्घ cookie, flags;

	raw_spin_lock_irqsave(&p->pi_lock, flags);
	cookie = sched_core_get_cookie(p->core_cookie);
	raw_spin_unlock_irqrestore(&p->pi_lock, flags);

	परत cookie;
}

शून्य sched_core_fork(संरचना task_struct *p)
{
	RB_CLEAR_गाठ(&p->core_गाठ);
	p->core_cookie = sched_core_clone_cookie(current);
}

शून्य sched_core_free(संरचना task_struct *p)
{
	sched_core_put_cookie(p->core_cookie);
}

स्थिर शून्य __sched_core_set(संरचना task_struct *p, अचिन्हित दीर्घ cookie)
{
	cookie = sched_core_get_cookie(cookie);
	cookie = sched_core_update_cookie(p, cookie);
	sched_core_put_cookie(cookie);
}

/* Called from prctl interface: PR_SCHED_CORE */
पूर्णांक sched_core_share_pid(अचिन्हित पूर्णांक cmd, pid_t pid, गणना pid_type type,
			 अचिन्हित दीर्घ uaddr)
{
	अचिन्हित दीर्घ cookie = 0, id = 0;
	संरचना task_struct *task, *p;
	संरचना pid *grp;
	पूर्णांक err = 0;

	जर (!sched_smt_active())
		परत -ENODEV;

	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD != PIDTYPE_PID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD_GROUP != PIDTYPE_TGID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_PROCESS_GROUP != PIDTYPE_PGID);

	जर (type > PIDTYPE_PGID || cmd >= PR_SCHED_CORE_MAX || pid < 0 ||
	    (cmd != PR_SCHED_CORE_GET && uaddr))
		परत -EINVAL;

	rcu_read_lock();
	जर (pid == 0) {
		task = current;
	} अन्यथा {
		task = find_task_by_vpid(pid);
		जर (!task) {
			rcu_read_unlock();
			परत -ESRCH;
		}
	}
	get_task_struct(task);
	rcu_read_unlock();

	/*
	 * Check if this process has the right to modify the specified
	 * process. Use the regular "ptrace_may_access()" checks.
	 */
	जर (!ptrace_may_access(task, PTRACE_MODE_READ_REALCREDS)) {
		err = -EPERM;
		जा out;
	}

	निवड (cmd) {
	स्थिती PR_SCHED_CORE_GET:
		जर (type != PIDTYPE_PID || uaddr & 7) {
			err = -EINVAL;
			जा out;
		}
		cookie = sched_core_clone_cookie(task);
		जर (cookie) {
			/* XXX improve ? */
			ptr_to_hashval((शून्य *)cookie, &id);
		}
		err = put_user(id, (u64 __user *)uaddr);
		जा out;

	स्थिती PR_SCHED_CORE_CREATE:
		cookie = sched_core_alloc_cookie();
		जर (!cookie) {
			err = -ENOMEM;
			जा out;
		}
		थांबा;

	स्थिती PR_SCHED_CORE_SHARE_TO:
		cookie = sched_core_clone_cookie(current);
		थांबा;

	स्थिती PR_SCHED_CORE_SHARE_FROM:
		जर (type != PIDTYPE_PID) {
			err = -EINVAL;
			जा out;
		}
		cookie = sched_core_clone_cookie(task);
		__sched_core_set(current, cookie);
		जा out;

	पूर्वनिर्धारित:
		err = -EINVAL;
		जा out;
	}

	जर (type == PIDTYPE_PID) {
		__sched_core_set(task, cookie);
		जा out;
	}

	read_lock(&tasklist_lock);
	grp = task_pid_type(task, type);

	do_each_pid_thread(grp, type, p) {
		जर (!ptrace_may_access(p, PTRACE_MODE_READ_REALCREDS)) {
			err = -EPERM;
			जा out_tasklist;
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
	परत err;
}

#ifdef CONFIG_SCHEDSTATS

/* REQUIRES: rq->core's clock recently updated. */
शून्य __sched_core_account_forceidle(संरचना rq *rq)
{
	स्थिरांक संरचना cpumask *smt_mask = cpu_smt_mask(cpu_of(rq));
	u64 delta, now = rq_clock(rq->core);
	संरचना rq *rq_i;
	संरचना task_struct *p;
	पूर्णांक i;

	lockdep_assert_rq_held(rq);

	WARN_ON_ONCE(!rq->core->core_forceidle_मोजणी);

	जर (rq->core->core_forceidle_start == 0)
		परत;

	delta = now - rq->core->core_forceidle_start;
	जर (unlikely((s64)delta <= 0))
		परत;

	rq->core->core_forceidle_start = now;

	जर (WARN_ON_ONCE(!rq->core->core_forceidle_occupation)) {
		/* can't be forced idle without a running task */
	} अन्यथा जर (rq->core->core_forceidle_मोजणी > 1 ||
		   rq->core->core_forceidle_occupation > 1) {
		/*
		 * For larger SMT configurations, we need to scale the charged
		 * forced idle amount since there can be more than one forced
		 * idle sibling and more than one running cookied task.
		 */
		delta *= rq->core->core_forceidle_मोजणी;
		delta = div_u64(delta, rq->core->core_forceidle_occupation);
	}

	for_each_cpu(i, smt_mask) {
		rq_i = cpu_rq(i);
		p = rq_i->core_pick ?: rq_i->curr;

		जर (p == rq_i->idle)
			सुरूठेवा;

		/*
		 * Note: this will account forceidle to the current CPU, even
		 * if it comes from our SMT sibling.
		 */
		__account_forceidle_time(p, delta);
	}
}

शून्य __sched_core_tick(संरचना rq *rq)
{
	जर (!rq->core->core_forceidle_मोजणी)
		परत;

	जर (rq != rq->core)
		update_rq_clock(rq->core);

	__sched_core_account_forceidle(rq);
}

#endif /* CONFIG_SCHEDSTATS */
