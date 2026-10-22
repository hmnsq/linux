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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ convert_prio(ಪೂರ್ಣಾಙ್ಕ prio)
{
	ಪೂರ್ಣಾಙ್ಕ cpupri;

	ವಿಕಲ್ಪ (prio) {
	ಪಕ್ಷ CPUPRI_INVALID:
		cpupri = CPUPRI_INVALID;	/* -1 */
		ಭಙ್ಗ;

	ಪಕ್ಷ 0 ... 98:
		cpupri = MAX_RT_PRIO-1 - prio;	/* 1 ... 99 */
		ಭಙ್ಗ;

	ಪಕ್ಷ MAX_RT_PRIO-1:
		cpupri = CPUPRI_NORMAL;		/*  0 */
		ಭಙ್ಗ;

	ಪಕ್ಷ MAX_RT_PRIO:
		cpupri = CPUPRI_HIGHER;		/* 100 */
		ಭಙ್ಗ;
	}

	ಪ್ರತ್ಯಾಗಮ cpupri;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ __cpupri_find(ಸಂರಚನಾ cpupri *cp, ಸಂರಚನಾ task_struct *p,
				ಸಂರಚನಾ cpumask *lowest_mask, ಪೂರ್ಣಾಙ್ಕ idx)
{
	ಸಂರಚನಾ cpupri_vec *vec  = &cp->pri_to_cpu[idx];
	ಪೂರ್ಣಾಙ್ಕ skip = 0;

	ಯದಿ (!atomic_read(&(vec)->ಎಣಿಕೆ))
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
	ಯದಿ (skip)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (cpumask_any_and(&p->cpus_mask, vec->mask) >= nr_cpu_ids)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (lowest_mask) {
		cpumask_and(lowest_mask, &p->cpus_mask, vec->mask);
		cpumask_and(lowest_mask, lowest_mask, cpu_active_mask);

		/*
		 * We have to ensure that we have at least one bit
		 * still set in the array, since the map could have
		 * been concurrently emptied between the first and
		 * second reads of vec->mask.  If we hit this
		 * condition, simply act as though we never hit this
		 * priority level and continue on.
		 */
		ಯದಿ (cpumask_empty(lowest_mask))
			ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಪ್ರತ್ಯಾಗಮ 1;
}

ಪೂರ್ಣಾಙ್ಕ cpupri_find(ಸಂರಚನಾ cpupri *cp, ಸಂರಚನಾ task_struct *p,
		ಸಂರಚನಾ cpumask *lowest_mask)
{
	ಪ್ರತ್ಯಾಗಮ cpupri_find_fitness(cp, p, lowest_mask, ರಿಕ್ತ);
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
ಪೂರ್ಣಾಙ್ಕ cpupri_find_fitness(ಸಂರಚನಾ cpupri *cp, ಸಂರಚನಾ task_struct *p,
		ಸಂರಚನಾ cpumask *lowest_mask,
		bool (*fitness_fn)(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ cpu))
{
	ಪೂರ್ಣಾಙ್ಕ task_pri = convert_prio(p->prio);
	ಪೂರ್ಣಾಙ್ಕ idx, cpu;

	WARN_ON_ONCE(task_pri >= CPUPRI_NR_PRIORITIES);

	ಪ್ರತಿ (idx = 0; idx < task_pri; idx++) {

		ಯದಿ (!__cpupri_find(cp, p, lowest_mask, idx))
			ಅನುವರ್ತ;

		ಯದಿ (!lowest_mask || !fitness_fn)
			ಪ್ರತ್ಯಾಗಮ 1;

		/* Ensure the capacity of the CPUs fit the task */
		for_each_cpu(cpu, lowest_mask) {
			ಯದಿ (!fitness_fn(p, cpu))
				cpumask_clear_cpu(cpu, lowest_mask);
		}

		/*
		 * If no CPU at the current priority can fit the task
		 * continue looking
		 */
		ಯದಿ (cpumask_empty(lowest_mask))
			ಅನುವರ್ತ;

		ಪ್ರತ್ಯಾಗಮ 1;
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
	ಯದಿ (fitness_fn)
		ಪ್ರತ್ಯಾಗಮ cpupri_find(cp, p, lowest_mask);

	ಪ್ರತ್ಯಾಗಮ 0;
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
ಶೂನ್ಯ cpupri_set(ಸಂರಚನಾ cpupri *cp, ಪೂರ್ಣಾಙ್ಕ cpu, ಪೂರ್ಣಾಙ್ಕ newpri)
{
	ಪೂರ್ಣಾಙ್ಕ *currpri = &cp->cpu_to_pri[cpu];
	ಪೂರ್ಣಾಙ್ಕ oldpri = *currpri;
	ಪೂರ್ಣಾಙ್ಕ do_mb = 0;

	newpri = convert_prio(newpri);

	BUG_ON(newpri >= CPUPRI_NR_PRIORITIES);

	ಯದಿ (newpri == oldpri)
		ಪ್ರತ್ಯಾಗಮ;

	/*
	 * If the CPU was currently mapped to a different value, we
	 * need to map it to the new value then remove the old value.
	 * Note, we must add the new value first, otherwise we risk the
	 * cpu being missed by the priority loop in cpupri_find.
	 */
	ಯದಿ (likely(newpri != CPUPRI_INVALID)) {
		ಸಂರಚನಾ cpupri_vec *vec = &cp->pri_to_cpu[newpri];

		cpumask_set_cpu(cpu, vec->mask);
		/*
		 * When adding a new vector, we update the mask first,
		 * do a write memory barrier, and then update the count, to
		 * make sure the vector is visible when count is set.
		 */
		smp_mb__before_atomic();
		atomic_inc(&(vec)->ಎಣಿಕೆ);
		do_mb = 1;
	}
	ಯದಿ (likely(oldpri != CPUPRI_INVALID)) {
		ಸಂರಚನಾ cpupri_vec *vec  = &cp->pri_to_cpu[oldpri];

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
		ಯದಿ (do_mb)
			smp_mb__after_atomic();

		/*
		 * When removing from the vector, we decrement the counter first
		 * do a memory barrier and then clear the mask.
		 */
		atomic_dec(&(vec)->ಎಣಿಕೆ);
		smp_mb__after_atomic();
		cpumask_clear_cpu(cpu, vec->mask);
	}

	*currpri = newpri;
}

/**
 * cpupri_init - initialize the cpupri structure
 * @cp: The cpupri context
 *
 * Return: -ENOMEM on memory allocation failure.
 */
ಪೂರ್ಣಾಙ್ಕ cpupri_init(ಸಂರಚನಾ cpupri *cp)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = 0; i < CPUPRI_NR_PRIORITIES; i++) {
		ಸಂರಚನಾ cpupri_vec *vec = &cp->pri_to_cpu[i];

		atomic_set(&vec->ಎಣಿಕೆ, 0);
		ಯದಿ (!zalloc_cpumask_var(&vec->mask, GFP_KERNEL))
			ಗಚ್ಛ cleanup;
	}

	cp->cpu_to_pri = kzalloc_objs(ಪೂರ್ಣಾಙ್ಕ, nr_cpu_ids);
	ಯದಿ (!cp->cpu_to_pri)
		ಗಚ್ಛ cleanup;

	for_each_possible_cpu(i)
		cp->cpu_to_pri[i] = CPUPRI_INVALID;

	ಪ್ರತ್ಯಾಗಮ 0;

cleanup:
	ಪ್ರತಿ (i--; i >= 0; i--)
		free_cpumask_var(cp->pri_to_cpu[i].mask);
	ಪ್ರತ್ಯಾಗಮ -ENOMEM;
}

/**
 * cpupri_cleanup - clean up the cpupri structure
 * @cp: The cpupri context
 */
ಶೂನ್ಯ cpupri_cleanup(ಸಂರಚನಾ cpupri *cp)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	kfree(cp->cpu_to_pri);
	ಪ್ರತಿ (i = 0; i < CPUPRI_NR_PRIORITIES; i++)
		free_cpumask_var(cp->pri_to_cpu[i].mask);
}
