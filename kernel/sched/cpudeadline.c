// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/cpudeadline.c
 *
 *  Global CPU deadline management
 *
 *  Author: Juri Lelli <j.lelli@sssup.it>
 */
#અન્તર્ભાવ "sched.h"

સ્થિર અંતઃરેખ પૂર્ણાંક parent(પૂર્ણાંક i)
{
	પાછા (i - 1) >> 1;
}

સ્થિર અંતઃરેખ પૂર્ણાંક ડાબું_child(પૂર્ણાંક i)
{
	પાછા (i << 1) + 1;
}

સ્થિર અંતઃરેખ પૂર્ણાંક જમણું_child(પૂર્ણાંક i)
{
	પાછા (i << 1) + 2;
}

સ્થિર શૂન્ય cpudl_heapify_down(સંરચના cpudl *cp, પૂર્ણાંક idx)
{
	પૂર્ણાંક l, r, largest;

	પૂર્ણાંક orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	જો (ડાબું_child(idx) >= cp->size)
		પાછા;

	/* adapted from lib/prio_heap.c */
	જ્યારે (1) {
		u64 largest_dl;

		l = ડાબું_child(idx);
		r = જમણું_child(idx);
		largest = idx;
		largest_dl = orig_dl;

		જો ((l < cp->size) && dl_time_before(orig_dl,
						cp->elements[l].dl)) {
			largest = l;
			largest_dl = cp->elements[l].dl;
		}
		જો ((r < cp->size) && dl_time_before(largest_dl,
						cp->elements[r].dl))
			largest = r;

		જો (largest == idx)
			તોડો;

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

સ્થિર શૂન્ય cpudl_heapify_up(સંરચના cpudl *cp, પૂર્ણાંક idx)
{
	પૂર્ણાંક p;

	પૂર્ણાંક orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	જો (idx == 0)
		પાછા;

	કરો {
		p = parent(idx);
		જો (dl_time_before(orig_dl, cp->elements[p].dl))
			તોડો;
		/* pull parent onto idx */
		cp->elements[idx].cpu = cp->elements[p].cpu;
		cp->elements[idx].dl = cp->elements[p].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = p;
	} જ્યારે (idx != 0);
	/* actual push up of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

સ્થિર શૂન્ય cpudl_heapify(સંરચના cpudl *cp, પૂર્ણાંક idx)
{
	જો (idx > 0 && dl_time_before(cp->elements[parent(idx)].dl,
				cp->elements[idx].dl))
		cpudl_heapify_up(cp, idx);
	નહીંતર
		cpudl_heapify_down(cp, idx);
}

સ્થિર અંતઃરેખ પૂર્ણાંક cpudl_maximum(સંરચના cpudl *cp)
{
	પાછા cp->elements[0].cpu;
}

/*
 * cpudl_find - find the best (later-dl) CPU in the system
 * @cp: the cpudl max-heap context
 * @p: the task
 * @later_mask: a mask to fill in with the selected CPUs (or NULL)
 *
 * Returns: int - CPUs were found
 */
પૂર્ણાંક cpudl_find(સંરચના cpudl *cp, સંરચના task_struct *p,
	       સંરચના cpumask *later_mask)
{
	અચલ સંરચના sched_dl_entity *dl_se = &p->dl;

	જો (later_mask &&
	    cpumask_and(later_mask, cp->free_cpus, &p->cpus_mask)) {
		unsigned long cap, max_cap = 0;
		પૂર્ણાંક cpu, max_cpu = -1;

		જો (!sched_asym_cpucap_active())
			પાછા 1;

		/* Ensure the capacity of the CPUs fits the task. */
		for_each_cpu(cpu, later_mask) {
			જો (!dl_task_fits_capacity(p, cpu)) {
				cpumask_clear_cpu(cpu, later_mask);

				cap = arch_scale_cpu_capacity(cpu);

				જો (cap > max_cap ||
				    (cpu == task_cpu(p) && cap == max_cap)) {
					max_cap = cap;
					max_cpu = cpu;
				}
			}
		}

		જો (cpumask_empty(later_mask))
			cpumask_set_cpu(max_cpu, later_mask);

		પાછા 1;
	} નહીંતર {
		પૂર્ણાંક best_cpu = cpudl_maximum(cp);

		WARN_ON(best_cpu != -1 && !cpu_present(best_cpu));

		જો (cpumask_test_cpu(best_cpu, &p->cpus_mask) &&
		    dl_time_before(dl_se->deadline, cp->elements[0].dl)) {
			જો (later_mask)
				cpumask_set_cpu(best_cpu, later_mask);

			પાછા 1;
		}
	}
	પાછા 0;
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
શૂન્ય cpudl_clear(સંરચના cpudl *cp, પૂર્ણાંક cpu, bool online)
{
	પૂર્ણાંક old_idx, new_cpu;
	unsigned long flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	જો (old_idx == IDX_INVALID) {
		/*
		 * Nothing to remove if old_idx was invalid.
		 * This could happen if rq_online_dl or rq_offline_dl is
		 * called for a CPU without -dl tasks running.
		 */
	} નહીંતર {
		new_cpu = cp->elements[cp->size - 1].cpu;
		cp->elements[old_idx].dl = cp->elements[cp->size - 1].dl;
		cp->elements[old_idx].cpu = new_cpu;
		cp->size--;
		cp->elements[new_cpu].idx = old_idx;
		cp->elements[cpu].idx = IDX_INVALID;
		cpudl_heapify(cp, old_idx);
	}
	જો (likely(online))
		__cpumask_set_cpu(cpu, cp->free_cpus);
	નહીંતર
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
શૂન્ય cpudl_set(સંરચના cpudl *cp, પૂર્ણાંક cpu, u64 dl)
{
	પૂર્ણાંક old_idx;
	unsigned long flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	જો (old_idx == IDX_INVALID) {
		પૂર્ણાંક new_idx = cp->size++;

		cp->elements[new_idx].dl = dl;
		cp->elements[new_idx].cpu = cpu;
		cp->elements[cpu].idx = new_idx;
		cpudl_heapify_up(cp, new_idx);
		__cpumask_clear_cpu(cpu, cp->free_cpus);
	} નહીંતર {
		cp->elements[old_idx].dl = dl;
		cpudl_heapify(cp, old_idx);
	}

	raw_spin_unlock_irqrestore(&cp->lock, flags);
}

/*
 * cpudl_init - initialize the cpudl structure
 * @cp: the cpudl max-heap context
 */
પૂર્ણાંક cpudl_init(સંરચના cpudl *cp)
{
	પૂર્ણાંક i;

	raw_spin_lock_init(&cp->lock);
	cp->size = 0;

	cp->elements = kzalloc_objs(સંરચના cpudl_item, nr_cpu_ids);
	જો (!cp->elements)
		પાછા -ENOMEM;

	જો (!zalloc_cpumask_var(&cp->free_cpus, GFP_KERNEL)) {
		kfree(cp->elements);
		પાછા -ENOMEM;
	}

	for_each_possible_cpu(i)
		cp->elements[i].idx = IDX_INVALID;

	પાછા 0;
}

/*
 * cpudl_cleanup - clean up the cpudl structure
 * @cp: the cpudl max-heap context
 */
શૂન્ય cpudl_cleanup(સંરચના cpudl *cp)
{
	free_cpumask_var(cp->free_cpus);
	kfree(cp->elements);
}
