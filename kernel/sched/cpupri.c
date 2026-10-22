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
#ಅನ್ತರ್ಭಾವ "sched.h"

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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ convert_prio(ಪೂರ್ಣಾಂಕ prio)
{
	ಪೂರ್ಣಾಂಕ cpupri;

	ಆಯ್ಕೆ (prio) {
	ಸ್ಥಿತಿ CPUPRI_INVALID:
		cpupri = CPUPRI_INVALID;	/* -1 */
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ 0 ... 98:
		cpupri = MAX_RT_PRIO-1 - prio;	/* 1 ... 99 */
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ MAX_RT_PRIO-1:
		cpupri = CPUPRI_ಸಾಮಾನ್ಯ;		/*  0 */
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ MAX_RT_PRIO:
		cpupri = CPUPRI_HIGHER;		/* 100 */
		ನಿಲ್ಲಿಸು;
	}

	ಹಿಂತಿರುಗಿ cpupri;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ __cpupri_find(ರಚನೆ cpupri *cp, ರಚನೆ task_struct *p,
				ರಚನೆ cpumask *ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ idx)
{
	ರಚನೆ cpupri_vec *vec  = &cp->pri_ಗಮ್ಯ_cpu[idx];
	ಪೂರ್ಣಾಂಕ skip = 0;

	ಆದರೆ (!atomic_read(&(vec)->ಎಣಿಕೆ_2))
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
	ಆದರೆ (skip)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (cpumask_any_and(&p->cpus_ಮುಖವಾಡ, vec->ಮುಖವಾಡ) >= nr_cpu_ids)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ) {
		cpumask_and(ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ, &p->cpus_ಮುಖವಾಡ, vec->ಮುಖವಾಡ);
		cpumask_and(ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ, ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ, cpu_ಸಕ್ರಿಯ_ಮುಖವಾಡ);

		/*
		 * We have to ensure that we have at least one bit
		 * still set in the array, since the map could have
		 * been concurrently emptied between the first and
		 * second reads of vec->mask.  If we hit this
		 * condition, simply act as though we never hit this
		 * priority level and continue on.
		 */
		ಆದರೆ (cpumask_ಖಾಲಿಗಳು(ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ))
			ಹಿಂತಿರುಗಿ 0;
	}

	ಹಿಂತಿರುಗಿ 1;
}

ಪೂರ್ಣಾಂಕ cpupri_ಕಂಡುಹಿಡಿ(ರಚನೆ cpupri *cp, ರಚನೆ task_struct *p,
		ರಚನೆ cpumask *ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ)
{
	ಹಿಂತಿರುಗಿ cpupri_ಕಂಡುಹಿಡಿ_fitness(cp, p, ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ, ಖಾಲಿ);
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
ಪೂರ್ಣಾಂಕ cpupri_ಕಂಡುಹಿಡಿ_fitness(ರಚನೆ cpupri *cp, ರಚನೆ task_struct *p,
		ರಚನೆ cpumask *ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ,
		bool (*fitness_fn)(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu))
{
	ಪೂರ್ಣಾಂಕ task_pri = convert_prio(p->prio);
	ಪೂರ್ಣಾಂಕ idx, cpu;

	WARN_ON_ONCE(task_pri >= CPUPRI_NR_PRIORITIES);

	ಗಾಗಿ (idx = 0; idx < task_pri; idx++) {

		ಆದರೆ (!__cpupri_find(cp, p, ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ, idx))
			ಮುಂದುವರಿ;

		ಆದರೆ (!ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ || !fitness_fn)
			ಹಿಂತಿರುಗಿ 1;

		/* Ensure the capacity of the CPUs fit the task */
		for_each_cpu(cpu, ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ) {
			ಆದರೆ (!fitness_fn(p, cpu))
				cpumask_clear_cpu(cpu, ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ);
		}

		/*
		 * If no CPU at the current priority can fit the task
		 * continue looking
		 */
		ಆದರೆ (cpumask_ಖಾಲಿಗಳು(ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ))
			ಮುಂದುವರಿ;

		ಹಿಂತಿರುಗಿ 1;
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
	ಆದರೆ (fitness_fn)
		ಹಿಂತಿರುಗಿ cpupri_ಕಂಡುಹಿಡಿ(cp, p, ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ);

	ಹಿಂತಿರುಗಿ 0;
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
ಶೂನ್ಯ cpupri_ಗಣ(ರಚನೆ cpupri *cp, ಪೂರ್ಣಾಂಕ cpu, ಪೂರ್ಣಾಂಕ newpri)
{
	ಪೂರ್ಣಾಂಕ *currpri = &cp->cpu_ಗಮ್ಯ_pri[cpu];
	ಪೂರ್ಣಾಂಕ oldpri = *currpri;
	ಪೂರ್ಣಾಂಕ do_mb = 0;

	newpri = convert_prio(newpri);

	BUG_ON(newpri >= CPUPRI_NR_PRIORITIES);

	ಆದರೆ (newpri == oldpri)
		ಹಿಂತಿರುಗಿ;

	/*
	 * If the CPU was currently mapped to a different value, we
	 * need to map it to the new value then remove the old value.
	 * Note, we must add the new value first, otherwise we risk the
	 * cpu being missed by the priority loop in cpupri_find.
	 */
	ಆದರೆ (likely(newpri != CPUPRI_INVALID)) {
		ರಚನೆ cpupri_vec *vec = &cp->pri_ಗಮ್ಯ_cpu[newpri];

		cpumask_ಗಣ_cpu(cpu, vec->ಮುಖವಾಡ);
		/*
		 * When adding a new vector, we update the mask first,
		 * do a write memory barrier, and then update the count, to
		 * make sure the vector is visible when count is set.
		 */
		smp_mb__before_atomic();
		atomic_inc(&(vec)->ಎಣಿಕೆ_2);
		do_mb = 1;
	}
	ಆದರೆ (likely(oldpri != CPUPRI_INVALID)) {
		ರಚನೆ cpupri_vec *vec  = &cp->pri_ಗಮ್ಯ_cpu[oldpri];

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
		ಆದರೆ (do_mb)
			smp_mb__after_atomic();

		/*
		 * When removing from the vector, we decrement the counter first
		 * do a memory barrier and then clear the mask.
		 */
		atomic_dec(&(vec)->ಎಣಿಕೆ_2);
		smp_mb__after_atomic();
		cpumask_clear_cpu(cpu, vec->ಮುಖವಾಡ);
	}

	*currpri = newpri;
}

/**
 * cpupri_init - initialize the cpupri structure
 * @cp: The cpupri context
 *
 * Return: -ENOMEM on memory allocation failure.
 */
ಪೂರ್ಣಾಂಕ cpupri_init(ರಚನೆ cpupri *cp)
{
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; i < CPUPRI_NR_PRIORITIES; i++) {
		ರಚನೆ cpupri_vec *vec = &cp->pri_ಗಮ್ಯ_cpu[i];

		atomic_ಗಣ(&vec->ಎಣಿಕೆ_2, 0);
		ಆದರೆ (!zalloc_cpumask_var(&vec->ಮುಖವಾಡ, GFP_KERNEL))
			ಹೋಗು cleanup;
	}

	cp->cpu_ಗಮ್ಯ_pri = kzalloc_objs(ಪೂರ್ಣಾಂಕ, nr_cpu_ids);
	ಆದರೆ (!cp->cpu_ಗಮ್ಯ_pri)
		ಹೋಗು cleanup;

	for_each_possible_cpu(i)
		cp->cpu_ಗಮ್ಯ_pri[i] = CPUPRI_INVALID;

	ಹಿಂತಿರುಗಿ 0;

cleanup:
	ಗಾಗಿ (i--; i >= 0; i--)
		free_cpumask_var(cp->pri_ಗಮ್ಯ_cpu[i].ಮುಖವಾಡ);
	ಹಿಂತಿರುಗಿ -ENOMEM;
}

/**
 * cpupri_cleanup - clean up the cpupri structure
 * @cp: The cpupri context
 */
ಶೂನ್ಯ cpupri_cleanup(ರಚನೆ cpupri *cp)
{
	ಪೂರ್ಣಾಂಕ i;

	kfree(cp->cpu_ಗಮ್ಯ_pri);
	ಗಾಗಿ (i = 0; i < CPUPRI_NR_PRIORITIES; i++)
		free_cpumask_var(cp->pri_ಗಮ್ಯ_cpu[i].ಮುಖವಾಡ);
}
