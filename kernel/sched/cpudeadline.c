// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/cpudeadline.c
 *
 *  Global CPU deadline management
 *
 *  Author: Juri Lelli <j.lelli@sssup.it>
 */
#അന്തര്ഭാവ "sched.h"

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ parent(പൂർണ്ണസംഖ്യ i)
{
	തിരികെ (i - 1) >> 1;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ ഇടത്_child(പൂർണ്ണസംഖ്യ i)
{
	തിരികെ (i << 1) + 1;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ വലത്_child(പൂർണ്ണസംഖ്യ i)
{
	തിരികെ (i << 1) + 2;
}

സ്ഥിതം ശൂന്യം cpudl_heapify_down(ഘടന cpudl *cp, പൂർണ്ണസംഖ്യ idx)
{
	പൂർണ്ണസംഖ്യ l, r, largest;

	പൂർണ്ണസംഖ്യ orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	എങ്കിൽ (ഇടത്_child(idx) >= cp->size)
		തിരികെ;

	/* adapted from lib/prio_heap.c */
	ആകുമ്പോൾ (1) {
		u64 largest_dl;

		l = ഇടത്_child(idx);
		r = വലത്_child(idx);
		largest = idx;
		largest_dl = orig_dl;

		എങ്കിൽ ((l < cp->size) && dl_time_before(orig_dl,
						cp->elements[l].dl)) {
			largest = l;
			largest_dl = cp->elements[l].dl;
		}
		എങ്കിൽ ((r < cp->size) && dl_time_before(largest_dl,
						cp->elements[r].dl))
			largest = r;

		എങ്കിൽ (largest == idx)
			നിർത്തുക;

		/* pull largest child onto idx */
		cp->elements[idx].cpu = cp->elements[largest].cpu;
		cp->elements[idx].dl = cp->elements[largest].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = largest;
	}
	/* actual push down of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

സ്ഥിതം ശൂന്യം cpudl_heapify_up(ഘടന cpudl *cp, പൂർണ്ണസംഖ്യ idx)
{
	പൂർണ്ണസംഖ്യ p;

	പൂർണ്ണസംഖ്യ orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	എങ്കിൽ (idx == 0)
		തിരികെ;

	ചെയ്യുക {
		p = parent(idx);
		എങ്കിൽ (dl_time_before(orig_dl, cp->elements[p].dl))
			നിർത്തുക;
		/* pull parent onto idx */
		cp->elements[idx].cpu = cp->elements[p].cpu;
		cp->elements[idx].dl = cp->elements[p].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = p;
	} ആകുമ്പോൾ (idx != 0);
	/* actual push up of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

സ്ഥിതം ശൂന്യം cpudl_heapify(ഘടന cpudl *cp, പൂർണ്ണസംഖ്യ idx)
{
	എങ്കിൽ (idx > 0 && dl_time_before(cp->elements[parent(idx)].dl,
				cp->elements[idx].dl))
		cpudl_heapify_up(cp, idx);
	അല്ലെങ്കിൽ
		cpudl_heapify_down(cp, idx);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ cpudl_maximum(ഘടന cpudl *cp)
{
	തിരികെ cp->elements[0].cpu;
}

/*
 * cpudl_find - find the best (later-dl) CPU in the system
 * @cp: the cpudl max-heap context
 * @p: the task
 * @later_mask: a mask to fill in with the selected CPUs (or NULL)
 *
 * Returns: int - CPUs were found
 */
പൂർണ്ണസംഖ്യ cpudl_find(ഘടന cpudl *cp, ഘടന task_struct *p,
	       ഘടന cpumask *later_mask)
{
	സ്ഥിരം ഘടന sched_dl_entity *dl_se = &p->dl;

	എങ്കിൽ (later_mask &&
	    cpumask_and(later_mask, cp->free_cpus, &p->cpus_mask)) {
		unsigned long cap, max_cap = 0;
		പൂർണ്ണസംഖ്യ cpu, max_cpu = -1;

		എങ്കിൽ (!sched_asym_cpucap_active())
			തിരികെ 1;

		/* Ensure the capacity of the CPUs fits the task. */
		for_each_cpu(cpu, later_mask) {
			എങ്കിൽ (!dl_task_fits_capacity(p, cpu)) {
				cpumask_clear_cpu(cpu, later_mask);

				cap = arch_scale_cpu_capacity(cpu);

				എങ്കിൽ (cap > max_cap ||
				    (cpu == task_cpu(p) && cap == max_cap)) {
					max_cap = cap;
					max_cpu = cpu;
				}
			}
		}

		എങ്കിൽ (cpumask_empty(later_mask))
			cpumask_set_cpu(max_cpu, later_mask);

		തിരികെ 1;
	} അല്ലെങ്കിൽ {
		പൂർണ്ണസംഖ്യ best_cpu = cpudl_maximum(cp);

		WARN_ON(best_cpu != -1 && !cpu_present(best_cpu));

		എങ്കിൽ (cpumask_test_cpu(best_cpu, &p->cpus_mask) &&
		    dl_time_before(dl_se->deadline, cp->elements[0].dl)) {
			എങ്കിൽ (later_mask)
				cpumask_set_cpu(best_cpu, later_mask);

			തിരികെ 1;
		}
	}
	തിരികെ 0;
}

/*
 * cpudl_clear - remove a CPU from the cpudl max-heap
 * @cp: the cpudl max-heap context
 * @cpu: the target CPU
 * @online: the online state of the deadline runqueue
 *
 * Notes: assumes cpu_rq(cpu)->lock is locked
 *
 * Returns: (void)
 */
ശൂന്യം cpudl_clear(ഘടന cpudl *cp, പൂർണ്ണസംഖ്യ cpu, bool online)
{
	പൂർണ്ണസംഖ്യ old_idx, new_cpu;
	unsigned long flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	എങ്കിൽ (old_idx == IDX_INVALID) {
		/*
		 * Nothing to remove if old_idx was invalid.
		 * This could happen if rq_online_dl or rq_offline_dl is
		 * called for a CPU without -dl tasks running.
		 */
	} അല്ലെങ്കിൽ {
		new_cpu = cp->elements[cp->size - 1].cpu;
		cp->elements[old_idx].dl = cp->elements[cp->size - 1].dl;
		cp->elements[old_idx].cpu = new_cpu;
		cp->size--;
		cp->elements[new_cpu].idx = old_idx;
		cp->elements[cpu].idx = IDX_INVALID;
		cpudl_heapify(cp, old_idx);
	}
	എങ്കിൽ (likely(online))
		__cpumask_set_cpu(cpu, cp->free_cpus);
	അല്ലെങ്കിൽ
		__cpumask_clear_cpu(cpu, cp->free_cpus);

	raw_spin_unlock_irqrestore(&cp->lock, flags);
}

/*
 * cpudl_set - update the cpudl max-heap
 * @cp: the cpudl max-heap context
 * @cpu: the target CPU
 * @dl: the new earliest deadline for this CPU
 *
 * Notes: assumes cpu_rq(cpu)->lock is locked
 *
 * Returns: (void)
 */
ശൂന്യം cpudl_set(ഘടന cpudl *cp, പൂർണ്ണസംഖ്യ cpu, u64 dl)
{
	പൂർണ്ണസംഖ്യ old_idx;
	unsigned long flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	എങ്കിൽ (old_idx == IDX_INVALID) {
		പൂർണ്ണസംഖ്യ new_idx = cp->size++;

		cp->elements[new_idx].dl = dl;
		cp->elements[new_idx].cpu = cpu;
		cp->elements[cpu].idx = new_idx;
		cpudl_heapify_up(cp, new_idx);
		__cpumask_clear_cpu(cpu, cp->free_cpus);
	} അല്ലെങ്കിൽ {
		cp->elements[old_idx].dl = dl;
		cpudl_heapify(cp, old_idx);
	}

	raw_spin_unlock_irqrestore(&cp->lock, flags);
}

/*
 * cpudl_init - initialize the cpudl structure
 * @cp: the cpudl max-heap context
 */
പൂർണ്ണസംഖ്യ cpudl_init(ഘടന cpudl *cp)
{
	പൂർണ്ണസംഖ്യ i;

	raw_spin_lock_init(&cp->lock);
	cp->size = 0;

	cp->elements = kzalloc_objs(ഘടന cpudl_item, nr_cpu_ids);
	എങ്കിൽ (!cp->elements)
		തിരികെ -ENOMEM;

	എങ്കിൽ (!zalloc_cpumask_var(&cp->free_cpus, GFP_KERNEL)) {
		kfree(cp->elements);
		തിരികെ -ENOMEM;
	}

	for_each_possible_cpu(i)
		cp->elements[i].idx = IDX_INVALID;

	തിരികെ 0;
}

/*
 * cpudl_cleanup - clean up the cpudl structure
 * @cp: the cpudl max-heap context
 */
ശൂന്യം cpudl_cleanup(ഘടന cpudl *cp)
{
	free_cpumask_var(cp->free_cpus);
	kfree(cp->elements);
}
