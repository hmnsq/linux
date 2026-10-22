// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/cpupri.c
 *
 *  CPU priority management
 *
 *  Copyright (C) 2007-2008 Novell
 *
 *  Author: Gregory Haskins <ghaskins@novell.com>
 *
 *  This code tracks the priority of each CPU so that global migration
 *  decisions are easy to calculate.  Each CPU can be in a state as follows:
 *
 *                 (INVALID), NORMAL, RT1, ... RT99, HIGHER
 *
 *  going from the lowest priority to the highest.  CPUs in the INVALID state
 *  are not eligible for routing.  The system maintains this state with
 *  a 2 dimensional bitmap (the first for priority class, the second for CPUs
 *  in that class).  Therefore a typical application without affinity
 *  restrictions can find a suitable CPU with O(1) complexity (e.g. two bit
 *  searches).  For tasks with affinity restrictions, the algorithm has a
 *  worst case complexity of O(min(101, nr_domcpus)), though the scenario that
 *  yields the worst case search is fairly contrived.
 */
#అన్తర్భావ "sched.h"

/*
 * p->rt_priority   p->prio   newpri   cpupri
 *
 *				  -1       -1 (CPUPRI_INVALID)
 *
 *				  99        0 (CPUPRI_NORMAL)
 *
 *		1        98       98        1
 *	      ...
 *	       49        50       50       49
 *	       50        49       49       50
 *	      ...
 *	       99         0        0       99
 *
 *				 100	  100 (CPUPRI_HIGHER)
 */
స్థిర పూర్ణాంకం convert_prio(పూర్ణాంకం prio)
{
	పూర్ణాంకం cpupri;

	ఎంపిక (prio) {
	స్థితి CPUPRI_INVALID:
		cpupri = CPUPRI_INVALID;	/* -1 */
		ఆపు;

	స్థితి 0 ... 98:
		cpupri = MAX_RT_PRIO-1 - prio;	/* 1 ... 99 */
		ఆపు;

	స్థితి MAX_RT_PRIO-1:
		cpupri = CPUPRI_సాధారణ;		/*  0 */
		ఆపు;

	స్థితి MAX_RT_PRIO:
		cpupri = CPUPRI_HIGHER;		/* 100 */
		ఆపు;
	}

	తిరిగి cpupri;
}

స్థిర అంతఃవరుస పూర్ణాంకం __cpupri_find(నిర్మాణం cpupri *cp, నిర్మాణం task_struct *p,
				నిర్మాణం cpumask *అత్యల్ప_ముసుగు, పూర్ణాంకం idx)
{
	నిర్మాణం cpupri_vec *vec  = &cp->pri_గమ్యం_cpu[idx];
	పూర్ణాంకం skip = 0;

	ఒకవేళ (!atomic_read(&(vec)->లెక్క))
		skip = 1;
	/*
	 * When looking at the vector, we need to read the counter,
	 * do a memory barrier, then read the mask.
	 *
	 * Note: This is still all racy, but we can deal with it.
	 *  Ideally, we only want to look at masks that are set.
	 *
	 *  If a mask is not set, then the only thing wrong is that we
	 *  did a little more work than necessary.
	 *
	 *  If we read a zero count but the mask is set, because of the
	 *  memory barriers, that can only happen when the highest prio
	 *  task for a run queue has left the run queue, in which case,
	 *  it will be followed by a pull. If the task we are processing
	 *  fails to find a proper place to go, that pull request will
	 *  pull this task if the run queue is running at a lower
	 *  priority.
	 */
	smp_rmb();

	/* Need to do the rmb for every iteration */
	ఒకవేళ (skip)
		తిరిగి 0;

	ఒకవేళ (cpumask_any_and(&p->cpus_ముసుగు, vec->ముసుగు) >= nr_cpu_ids)
		తిరిగి 0;

	ఒకవేళ (అత్యల్ప_ముసుగు) {
		cpumask_and(అత్యల్ప_ముసుగు, &p->cpus_ముసుగు, vec->ముసుగు);
		cpumask_and(అత్యల్ప_ముసుగు, అత్యల్ప_ముసుగు, cpu_చురుకైన_ముసుగు);

		/*
		 * We have to ensure that we have at least one bit
		 * still set in the array, since the map could have
		 * been concurrently emptied between the first and
		 * second reads of vec->mask.  If we hit this
		 * condition, simply act as though we never hit this
		 * priority level and continue on.
		 */
		ఒకవేళ (cpumask_ఖాళీలు(అత్యల్ప_ముసుగు))
			తిరిగి 0;
	}

	తిరిగి 1;
}

పూర్ణాంకం cpupri_కనుగొను(నిర్మాణం cpupri *cp, నిర్మాణం task_struct *p,
		నిర్మాణం cpumask *అత్యల్ప_ముసుగు)
{
	తిరిగి cpupri_కనుగొను_fitness(cp, p, అత్యల్ప_ముసుగు, ఖాళీ);
}

/**
 * cpupri_find_fitness - find the best (lowest-pri) CPU in the system
 * @cp: The cpupri context
 * @p: The task
 * @lowest_mask: A mask to fill in with selected CPUs (or NULL)
 * @fitness_fn: A pointer to a function to do custom checks whether the CPU
 *              fits a specific criteria so that we only return those CPUs.
 *
 * Note: This function returns the recommended CPUs as calculated during the
 * current invocation.  By the time the call returns, the CPUs may have in
 * fact changed priorities any number of times.  While not ideal, it is not
 * an issue of correctness since the normal rebalancer logic will correct
 * any discrepancies created by racing against the uncertainty of the current
 * priority configuration.
 *
 * Return: (int)bool - CPUs were found
 */
పూర్ణాంకం cpupri_కనుగొను_fitness(నిర్మాణం cpupri *cp, నిర్మాణం task_struct *p,
		నిర్మాణం cpumask *అత్యల్ప_ముసుగు,
		bool (*fitness_fn)(నిర్మాణం task_struct *p, పూర్ణాంకం cpu))
{
	పూర్ణాంకం task_pri = convert_prio(p->prio);
	పూర్ణాంకం idx, cpu;

	WARN_ON_ONCE(task_pri >= CPUPRI_NR_PRIORITIES);

	కొరకు (idx = 0; idx < task_pri; idx++) {

		ఒకవేళ (!__cpupri_find(cp, p, అత్యల్ప_ముసుగు, idx))
			కొనసాగు;

		ఒకవేళ (!అత్యల్ప_ముసుగు || !fitness_fn)
			తిరిగి 1;

		/* Ensure the capacity of the CPUs fit the task */
		for_each_cpu(cpu, అత్యల్ప_ముసుగు) {
			ఒకవేళ (!fitness_fn(p, cpu))
				cpumask_clear_cpu(cpu, అత్యల్ప_ముసుగు);
		}

		/*
		 * If no CPU at the current priority can fit the task
		 * continue looking
		 */
		ఒకవేళ (cpumask_ఖాళీలు(అత్యల్ప_ముసుగు))
			కొనసాగు;

		తిరిగి 1;
	}

	/*
	 * If we failed to find a fitting lowest_mask, kick off a new search
	 * but without taking into account any fitness criteria this time.
	 *
	 * This rule favours honouring priority over fitting the task in the
	 * correct CPU (Capacity Awareness being the only user now).
	 * The idea is that if a higher priority task can run, then it should
	 * run even if this ends up being on unfitting CPU.
	 *
	 * The cost of this trade-off is not entirely clear and will probably
	 * be good for some workloads and bad for others.
	 *
	 * The main idea here is that if some CPUs were over-committed, we try
	 * to spread which is what the scheduler traditionally did. Sys admins
	 * must do proper RT planning to avoid overloading the system if they
	 * really care.
	 */
	ఒకవేళ (fitness_fn)
		తిరిగి cpupri_కనుగొను(cp, p, అత్యల్ప_ముసుగు);

	తిరిగి 0;
}

/**
 * cpupri_set - update the CPU priority setting
 * @cp: The cpupri context
 * @cpu: The target CPU
 * @newpri: The priority (INVALID,NORMAL,RT1-RT99,HIGHER) to assign to this CPU
 *
 * Note: Assumes cpu_rq(cpu)->lock is locked
 *
 * Returns: (void)
 */
శూన్య cpupri_సమితి(నిర్మాణం cpupri *cp, పూర్ణాంకం cpu, పూర్ణాంకం newpri)
{
	పూర్ణాంకం *currpri = &cp->cpu_గమ్యం_pri[cpu];
	పూర్ణాంకం oldpri = *currpri;
	పూర్ణాంకం do_mb = 0;

	newpri = convert_prio(newpri);

	BUG_ON(newpri >= CPUPRI_NR_PRIORITIES);

	ఒకవేళ (newpri == oldpri)
		తిరిగి;

	/*
	 * If the CPU was currently mapped to a different value, we
	 * need to map it to the new value then remove the old value.
	 * Note, we must add the new value first, otherwise we risk the
	 * cpu being missed by the priority loop in cpupri_find.
	 */
	ఒకవేళ (likely(newpri != CPUPRI_INVALID)) {
		నిర్మాణం cpupri_vec *vec = &cp->pri_గమ్యం_cpu[newpri];

		cpumask_సమితి_cpu(cpu, vec->ముసుగు);
		/*
		 * When adding a new vector, we update the mask first,
		 * do a write memory barrier, and then update the count, to
		 * make sure the vector is visible when count is set.
		 */
		smp_mb__before_atomic();
		atomic_inc(&(vec)->లెక్క);
		do_mb = 1;
	}
	ఒకవేళ (likely(oldpri != CPUPRI_INVALID)) {
		నిర్మాణం cpupri_vec *vec  = &cp->pri_గమ్యం_cpu[oldpri];

		/*
		 * Because the order of modification of the vec->count
		 * is important, we must make sure that the update
		 * of the new prio is seen before we decrement the
		 * old prio. This makes sure that the loop sees
		 * one or the other when we raise the priority of
		 * the run queue. We don't care about when we lower the
		 * priority, as that will trigger an rt pull anyway.
		 *
		 * We only need to do a memory barrier if we updated
		 * the new priority vec.
		 */
		ఒకవేళ (do_mb)
			smp_mb__after_atomic();

		/*
		 * When removing from the vector, we decrement the counter first
		 * do a memory barrier and then clear the mask.
		 */
		atomic_dec(&(vec)->లెక్క);
		smp_mb__after_atomic();
		cpumask_clear_cpu(cpu, vec->ముసుగు);
	}

	*currpri = newpri;
}

/**
 * cpupri_init - initialize the cpupri structure
 * @cp: The cpupri context
 *
 * Return: -ENOMEM on memory allocation failure.
 */
పూర్ణాంకం cpupri_init(నిర్మాణం cpupri *cp)
{
	పూర్ణాంకం i;

	కొరకు (i = 0; i < CPUPRI_NR_PRIORITIES; i++) {
		నిర్మాణం cpupri_vec *vec = &cp->pri_గమ్యం_cpu[i];

		atomic_సమితి(&vec->లెక్క, 0);
		ఒకవేళ (!zalloc_cpumask_var(&vec->ముసుగు, GFP_KERNEL))
			వెళ్ళు cleanup;
	}

	cp->cpu_గమ్యం_pri = kzalloc_objs(పూర్ణాంకం, nr_cpu_ids);
	ఒకవేళ (!cp->cpu_గమ్యం_pri)
		వెళ్ళు cleanup;

	for_each_possible_cpu(i)
		cp->cpu_గమ్యం_pri[i] = CPUPRI_INVALID;

	తిరిగి 0;

cleanup:
	కొరకు (i--; i >= 0; i--)
		free_cpumask_var(cp->pri_గమ్యం_cpu[i].ముసుగు);
	తిరిగి -ENOMEM;
}

/**
 * cpupri_cleanup - clean up the cpupri structure
 * @cp: The cpupri context
 */
శూన్య cpupri_cleanup(నిర్మాణం cpupri *cp)
{
	పూర్ణాంకం i;

	kfree(cp->cpu_గమ్యం_pri);
	కొరకు (i = 0; i < CPUPRI_NR_PRIORITIES; i++)
		free_cpumask_var(cp->pri_గమ్యం_cpu[i].ముసుగు);
}
