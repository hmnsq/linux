// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/cpudeadline.c
 *
 *  Global CPU deadline management
 *
 *  Author: Juri Lelli <j.lelli@sssup.it>
 */
#समावेश "sched.h"

अटल अंतरभूत पूर्णांक parent(पूर्णांक i)
{
	वापस (i - 1) >> 1;
}

अटल अंतरभूत पूर्णांक बायाँ_child(पूर्णांक i)
{
	वापस (i << 1) + 1;
}

अटल अंतरभूत पूर्णांक दायाँ_child(पूर्णांक i)
{
	वापस (i << 1) + 2;
}

अटल व्योम cpudl_heapify_down(काष्ठा cpudl *cp, पूर्णांक idx)
{
	पूर्णांक l, r, largest;

	पूर्णांक orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	अगर (बायाँ_child(idx) >= cp->आकार)
		वापस;

	/* adapted from lib/prio_heap.c */
	जबतक (1) {
		u64 largest_dl;

		l = बायाँ_child(idx);
		r = दायाँ_child(idx);
		largest = idx;
		largest_dl = orig_dl;

		अगर ((l < cp->आकार) && dl_time_before(orig_dl,
						cp->elements[l].dl)) {
			largest = l;
			largest_dl = cp->elements[l].dl;
		}
		अगर ((r < cp->आकार) && dl_time_before(largest_dl,
						cp->elements[r].dl))
			largest = r;

		अगर (largest == idx)
			अवरोध;

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

अटल व्योम cpudl_heapify_up(काष्ठा cpudl *cp, पूर्णांक idx)
{
	पूर्णांक p;

	पूर्णांक orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	अगर (idx == 0)
		वापस;

	करो {
		p = parent(idx);
		अगर (dl_time_before(orig_dl, cp->elements[p].dl))
			अवरोध;
		/* pull parent onto idx */
		cp->elements[idx].cpu = cp->elements[p].cpu;
		cp->elements[idx].dl = cp->elements[p].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = p;
	} जबतक (idx != 0);
	/* actual push up of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

अटल व्योम cpudl_heapify(काष्ठा cpudl *cp, पूर्णांक idx)
{
	अगर (idx > 0 && dl_time_before(cp->elements[parent(idx)].dl,
				cp->elements[idx].dl))
		cpudl_heapify_up(cp, idx);
	अन्यथा
		cpudl_heapify_down(cp, idx);
}

अटल अंतरभूत पूर्णांक cpudl_maximum(काष्ठा cpudl *cp)
{
	वापस cp->elements[0].cpu;
}

/*
 * cpudl_find - find the best (later-dl) CPU in the system
 * @cp: the cpudl max-heap context
 * @p: the task
 * @later_mask: a mask to fill in with the selected CPUs (or NULL)
 *
 * Returns: int - CPUs were found
 */
पूर्णांक cpudl_find(काष्ठा cpudl *cp, काष्ठा task_struct *p,
	       काष्ठा cpumask *later_mask)
{
	स्थिर काष्ठा sched_dl_entity *dl_se = &p->dl;

	अगर (later_mask &&
	    cpumask_and(later_mask, cp->free_cpus, &p->cpus_mask)) {
		अचिन्हित दीर्घ cap, max_cap = 0;
		पूर्णांक cpu, max_cpu = -1;

		अगर (!sched_asym_cpucap_active())
			वापस 1;

		/* Ensure the capacity of the CPUs fits the task. */
		for_each_cpu(cpu, later_mask) {
			अगर (!dl_task_fits_capacity(p, cpu)) {
				cpumask_clear_cpu(cpu, later_mask);

				cap = arch_scale_cpu_capacity(cpu);

				अगर (cap > max_cap ||
				    (cpu == task_cpu(p) && cap == max_cap)) {
					max_cap = cap;
					max_cpu = cpu;
				}
			}
		}

		अगर (cpumask_empty(later_mask))
			cpumask_set_cpu(max_cpu, later_mask);

		वापस 1;
	} अन्यथा {
		पूर्णांक best_cpu = cpudl_maximum(cp);

		WARN_ON(best_cpu != -1 && !cpu_present(best_cpu));

		अगर (cpumask_test_cpu(best_cpu, &p->cpus_mask) &&
		    dl_time_before(dl_se->deadline, cp->elements[0].dl)) {
			अगर (later_mask)
				cpumask_set_cpu(best_cpu, later_mask);

			वापस 1;
		}
	}
	वापस 0;
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
व्योम cpudl_clear(काष्ठा cpudl *cp, पूर्णांक cpu, bool online)
{
	पूर्णांक old_idx, new_cpu;
	अचिन्हित दीर्घ flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	अगर (old_idx == IDX_INVALID) {
		/*
		 * Nothing to remove if old_idx was invalid.
		 * This could happen if rq_online_dl or rq_offline_dl is
		 * called for a CPU without -dl tasks running.
		 */
	} अन्यथा {
		new_cpu = cp->elements[cp->आकार - 1].cpu;
		cp->elements[old_idx].dl = cp->elements[cp->आकार - 1].dl;
		cp->elements[old_idx].cpu = new_cpu;
		cp->आकार--;
		cp->elements[new_cpu].idx = old_idx;
		cp->elements[cpu].idx = IDX_INVALID;
		cpudl_heapify(cp, old_idx);
	}
	अगर (likely(online))
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
व्योम cpudl_set(काष्ठा cpudl *cp, पूर्णांक cpu, u64 dl)
{
	पूर्णांक old_idx;
	अचिन्हित दीर्घ flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	अगर (old_idx == IDX_INVALID) {
		पूर्णांक new_idx = cp->आकार++;

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
पूर्णांक cpudl_init(काष्ठा cpudl *cp)
{
	पूर्णांक i;

	raw_spin_lock_init(&cp->lock);
	cp->आकार = 0;

	cp->elements = kzalloc_objs(काष्ठा cpudl_item, nr_cpu_ids);
	अगर (!cp->elements)
		वापस -ENOMEM;

	अगर (!zalloc_cpumask_var(&cp->free_cpus, GFP_KERNEL)) {
		kfree(cp->elements);
		वापस -ENOMEM;
	}

	for_each_possible_cpu(i)
		cp->elements[i].idx = IDX_INVALID;

	वापस 0;
}

/*
 * cpudl_cleanup - clean up the cpudl structure
 * @cp: the cpudl max-heap context
 */
व्योम cpudl_cleanup(काष्ठा cpudl *cp)
{
	free_cpumask_var(cp->free_cpus);
	kfree(cp->elements);
}
