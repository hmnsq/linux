/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Inline definitions layered on top of internal.h and cid.h.
 *
 * Copyright (c) 2026 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2026 Tejun Heo <tj@kernel.org>
 */
#यदि_अपरिभाषितम् _KERNEL_SCHED_EXT_INLINES_H
#परिभाषय _KERNEL_SCHED_EXT_INLINES_H

#अन्तर्भाव "internal.h"
#अन्तर्भाव "cid.h"

/* what dispatch concluded, consumed by the pick that follows */
गणना scx_dsp_verdict {
	SCX_DSP_NONE,		/* nothing to run */
	SCX_DSP_LOCAL,		/* local DSQ has tasks */
	SCX_DSP_पूर्व,		/* keep running @prev */
	SCX_DSP_RETRY,		/* pick helpers only: restart the pick */
};

/*
 * One user of this function is scx_bpf_sub_dispatch() which can be called
 * recursively as sub-sched dispatches nest. Always inline to reduce stack usage
 * from the call frame.
 */
स्थिर __always_inline गणना scx_dsp_verdict
scx_dispatch_sched(संरचना scx_sched *sch, संरचना rq *rq,
		   संरचना task_struct *पूर्व, bool nested)
{
	संरचना scx_dsp_ctx *dspc = &this_cpu_ptr(sch->pcpu)->dsp_ctx;
	पूर्णाङ्क nr_loops = SCX_DSP_MAX_LOOPS;
	s32 cpu = cpu_सम्बन्धि(rq);
	bool पूर्व_on_sch = (पूर्व->sched_class == &ext_sched_class) &&
		scx_task_on_sched(sch, पूर्व);

	यदि (scx_consume_global_dsq(sch, rq))
		प्रत्यागम SCX_DSP_LOCAL;

	यदि (scx_bypass_dsp_enabled(sch)) {
		/* if @sch is bypassing, only the bypass DSQs are active */
		यदि (scx_bypassing(sch, cpu)) {
			यदि (scx_consume_dispatch_q(sch, rq, scx_bypass_dsq(sch, cpu), 0))
				प्रत्यागम SCX_DSP_LOCAL;
			प्रत्यागम SCX_DSP_NONE;
		}

#यदि_परिभाषितम् CONFIG_EXT_SUB_SCHED
		/*
		 * If @sch isn't bypassing but its children are, @sch is
		 * responsible for making forward progress for both its own
		 * tasks that aren't bypassing and the bypassing descendants'
		 * tasks. The following implements a simple built-in behavior -
		 * let each CPU try to run the bypass DSQ every Nth time.
		 *
		 * Later, if necessary, we can add an ops flag to suppress the
		 * auto-consumption and a kfunc to consume the bypass DSQ and,
		 * so that the BPF scheduler can fully control scheduling of
		 * bypassed tasks.
		 */
		संरचना scx_sched_pcpu *pcpu = प्रत्येकम्_cpu_ptr(sch->pcpu, cpu);

		यदि (!(pcpu->bypass_host_seq++ % SCX_BYPASS_HOST_NTH) &&
		    scx_consume_dispatch_q(sch, rq, scx_bypass_dsq(sch, cpu), 0)) {
			__scx_add_event(sch, SCX_EV_SUB_BYPASS_DISPATCH, 1);
			प्रत्यागम SCX_DSP_LOCAL;
		}
#यदि_अन्तः	/* CONFIG_EXT_SUB_SCHED */
	}

	/*
	 * scx_rq_online() can't be used. Its cpu_active() test goes false
	 * before CPU hotplug waits for an RCU grace period, and
	 * rq_offline_scx() moves this CPU's tasks to the local DSQ only after
	 * the wait. The grace period can depend on those tasks running.
	 */
	यदि (unlikely(!SCX_HAS_OP(sch, dispatch)) || !(rq->scx.flags & SCX_RQ_ONLINE))
		प्रत्यागम SCX_DSP_NONE;

	dspc->rq = rq;

	/*
	 * The dispatch loop. Because scx_flush_dispatch_buf() may drop the rq
	 * lock, the local DSQ might still end up empty after a successful
	 * ops.dispatch(). If the local DSQ is empty even after ops.dispatch()
	 * produced some tasks, retry. The BPF scheduler may depend on this
	 * looping behavior to simplify its implementation.
	 */
	कुरु {
		dspc->nr_tasks = 0;

#यदि_परिभाषितम् CONFIG_EXT_SUB_SCHED
		/* stash @prev so that nested invocations can access it */
		यदि (!nested)
			rq->scx.sub_dispatch_पूर्व = पूर्व;
#यदि_अन्तः

		SCX_CALL_OP(sch, dispatch, rq, scx_cpu_arg(cpu),
			    पूर्व_on_sch ? पूर्व : रिक्त);

#यदि_परिभाषितम् CONFIG_EXT_SUB_SCHED
		यदि (!nested)
			rq->scx.sub_dispatch_पूर्व = रिक्त;
#यदि_अन्तः

		scx_flush_dispatch_buf(sch, rq);

		यदि ((पूर्व->scx.flags & SCX_TASK_QUEUED) && पूर्व->scx.खण्डिका)
			प्रत्यागम SCX_DSP_पूर्व;
		यदि (rq->scx.local_dsq.nr)
			प्रत्यागम SCX_DSP_LOCAL;
		यदि (scx_consume_global_dsq(sch, rq))
			प्रत्यागम SCX_DSP_LOCAL;

		/*
		 * ops.dispatch() can trap us in this loop by repeatedly
		 * dispatching ineligible tasks. Break out once in a while to
		 * allow the watchdog to run. As IRQ can't be enabled in
		 * dispatch, we want to complete this scheduling cycle and then
		 * start a new one. IOW, we want to call resched_curr() on the
		 * next, most likely idle, task, not the current one. Use
		 * __scx_bpf_kick_cpu() for deferred kicking.
		 */
		यदि (unlikely(!--nr_loops)) {
			scx_kick_cpu(sch, cpu, 0);
			भङ्ग;
		}
	} यावत् (dspc->nr_tasks);

	/*
	 * Prevent the CPU from going idle while bypassed descendants have tasks
	 * queued. Without this fallback, bypassed tasks could stall if the host
	 * scheduler's ops.dispatch() doesn't yield any tasks.
	 */
	यदि (scx_bypass_dsp_enabled(sch) &&
	    scx_consume_dispatch_q(sch, rq, scx_bypass_dsq(sch, cpu), 0)) {
		__scx_add_event(sch, SCX_EV_SUB_BYPASS_DISPATCH, 1);
		प्रत्यागम SCX_DSP_LOCAL;
	}

	प्रत्यागम SCX_DSP_NONE;
}

#यदि_अन्तः /* _KERNEL_SCHED_EXT_INLINES_H */
