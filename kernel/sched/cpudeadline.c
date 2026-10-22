// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/cpudeadline.c
 *
 *  Global CPU deadline management
 *
 *  Author: Juri Lelli <j.lelli@sssup.it>
 */
#अन्तर्भाव "sched.h"

स्थिर अन्तःस्थ पूर्णाङ्क parent(पूर्णाङ्क i)
{
	प्रत्यागम (i - 1) >> 1;
}

स्थिर अन्तःस्थ पूर्णाङ्क बायाँ_child(पूर्णाङ्क i)
{
	प्रत्यागम (i << 1) + 1;
}

स्थिर अन्तःस्थ पूर्णाङ्क दायाँ_child(पूर्णाङ्क i)
{
	प्रत्यागम (i << 1) + 2;
}

स्थिर शून्य cpudl_heapify_down(संरचना cpudl *cp, पूर्णाङ्क idx)
{
	पूर्णाङ्क l, r, largest;

	पूर्णाङ्क orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	यदि (बायाँ_child(idx) >= cp->size)
		प्रत्यागम;

	/* adapted from lib/prio_heap.c */
	यावत् (1) {
		u64 largest_dl;

		l = बायाँ_child(idx);
		r = दायाँ_child(idx);
		largest = idx;
		largest_dl = orig_dl;

		यदि ((l < cp->size) && dl_time_before(orig_dl,
						cp->elements[l].dl)) {
			largest = l;
			largest_dl = cp->elements[l].dl;
		}
		यदि ((r < cp->size) && dl_time_before(largest_dl,
						cp->elements[r].dl))
			largest = r;

		यदि (largest == idx)
			भङ्ग;

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

स्थिर शून्य cpudl_heapify_up(संरचना cpudl *cp, पूर्णाङ्क idx)
{
	पूर्णाङ्क p;

	पूर्णाङ्क orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	यदि (idx == 0)
		प्रत्यागम;

	कुरु {
		p = parent(idx);
		यदि (dl_time_before(orig_dl, cp->elements[p].dl))
			भङ्ग;
		/* pull parent onto idx */
		cp->elements[idx].cpu = cp->elements[p].cpu;
		cp->elements[idx].dl = cp->elements[p].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = p;
	} यावत् (idx != 0);
	/* actual push up of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

स्थिर शून्य cpudl_heapify(संरचना cpudl *cp, पूर्णाङ्क idx)
{
	यदि (idx > 0 && dl_time_before(cp->elements[parent(idx)].dl,
				cp->elements[idx].dl))
		cpudl_heapify_up(cp, idx);
	अन्यथा
		cpudl_heapify_down(cp, idx);
}

स्थिर अन्तःस्थ पूर्णाङ्क cpudl_maximum(संरचना cpudl *cp)
{
	प्रत्यागम cp->elements[0].cpu;
}

/*
 * cpudl_find - find the best (later-dl) CPU in the system
 * @cp: the cpudl max-heap context
 * @p: the task
 * @later_mask: a mask to fill in with the selected CPUs (or NULL)
 *
 * Returns: int - CPUs were found
 */
पूर्णाङ्क cpudl_find(संरचना cpudl *cp, संरचना task_struct *p,
	       संरचना cpumask *later_mask)
{
	ध्रुव संरचना sched_dl_entity *dl_se = &p->dl;

	यदि (later_mask &&
	    cpumask_and(later_mask, cp->free_cpus, &p->cpus_mask)) {
		unsigned long cap, max_cap = 0;
		पूर्णाङ्क cpu, max_cpu = -1;

		यदि (!sched_asym_cpucap_active())
			प्रत्यागम 1;

		/* Ensure the capacity of the CPUs fits the task. */
		for_each_cpu(cpu, later_mask) {
			यदि (!dl_task_fits_capacity(p, cpu)) {
				cpumask_clear_cpu(cpu, later_mask);

				cap = arch_scale_cpu_capacity(cpu);

				यदि (cap > max_cap ||
				    (cpu == task_cpu(p) && cap == max_cap)) {
					max_cap = cap;
					max_cpu = cpu;
				}
			}
		}

		यदि (cpumask_empty(later_mask))
			cpumask_set_cpu(max_cpu, later_mask);

		प्रत्यागम 1;
	} अन्यथा {
		पूर्णाङ्क best_cpu = cpudl_maximum(cp);

		WARN_ON(best_cpu != -1 && !cpu_present(best_cpu));

		यदि (cpumask_test_cpu(best_cpu, &p->cpus_mask) &&
		    dl_time_before(dl_se->deadline, cp->elements[0].dl)) {
			यदि (later_mask)
				cpumask_set_cpu(best_cpu, later_mask);

			प्रत्यागम 1;
		}
	}
	प्रत्यागम 0;
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
शून्य cpudl_clear(संरचना cpudl *cp, पूर्णाङ्क cpu, bool online)
{
	पूर्णाङ्क old_idx, new_cpu;
	unsigned long flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	यदि (old_idx == IDX_INVALID) {
		/*
		 * Nothing to remove if old_idx was invalid.
		 * This could happen if rq_online_dl or rq_offline_dl is
		 * called for a CPU without -dl tasks running.
		 */
	} अन्यथा {
		new_cpu = cp->elements[cp->size - 1].cpu;
		cp->elements[old_idx].dl = cp->elements[cp->size - 1].dl;
		cp->elements[old_idx].cpu = new_cpu;
		cp->size--;
		cp->elements[new_cpu].idx = old_idx;
		cp->elements[cpu].idx = IDX_INVALID;
		cpudl_heapify(cp, old_idx);
	}
	यदि (likely(online))
		__cpumask_set_cpu(cpu, cp->free_cpus);
	अन्यथा
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
शून्य cpudl_set(संरचना cpudl *cp, पूर्णाङ्क cpu, u64 dl)
{
	पूर्णाङ्क old_idx;
	unsigned long flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	यदि (old_idx == IDX_INVALID) {
		पूर्णाङ्क new_idx = cp->size++;

		cp->elements[new_idx].dl = dl;
		cp->elements[new_idx].cpu = cpu;
		cp->elements[cpu].idx = new_idx;
		cpudl_heapify_up(cp, new_idx);
		__cpumask_clear_cpu(cpu, cp->free_cpus);
	} अन्यथा {
		cp->elements[old_idx].dl = dl;
		cpudl_heapify(cp, old_idx);
	}

	raw_spin_unlock_irqrestore(&cp->lock, flags);
}

/*
 * cpudl_init - initialize the cpudl structure
 * @cp: the cpudl max-heap context
 */
पूर्णाङ्क cpudl_init(संरचना cpudl *cp)
{
	पूर्णाङ्क i;

	raw_spin_lock_init(&cp->lock);
	cp->size = 0;

	cp->elements = kzalloc_objs(संरचना cpudl_item, nr_cpu_ids);
	यदि (!cp->elements)
		प्रत्यागम -ENOMEM;

	यदि (!zalloc_cpumask_var(&cp->free_cpus, GFP_KERNEL)) {
		kfree(cp->elements);
		प्रत्यागम -ENOMEM;
	}

	for_each_possible_cpu(i)
		cp->elements[i].idx = IDX_INVALID;

	प्रत्यागम 0;
}

/*
 * cpudl_cleanup - clean up the cpudl structure
 * @cp: the cpudl max-heap context
 */
शून्य cpudl_cleanup(संरचना cpudl *cp)
{
	free_cpumask_var(cp->free_cpus);
	kfree(cp->elements);
}
