// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/cpudeadline.c
 *
 *  Global CPU deadline management
 *
 *  Author: Juri Lelli <j.lelli@sssup.it>
 */
#ଅନ୍ତର୍ଭାଵ "sched.h"

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା parent(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i)
{
	ଫେରନ୍ତୁ (i - 1) >> 1;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବାମ_child(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i)
{
	ଫେରନ୍ତୁ (i << 1) + 1;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଡାହାଣ_child(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i)
{
	ଫେରନ୍ତୁ (i << 1) + 2;
}

ସ୍ଥିର ଶୂନ୍ୟ cpudl_heapify_down(ସଂରଚନା cpudl *cp, ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା l, r, largest;

	ପୂର୍ଣ୍ଣସଂଖ୍ୟା orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	ଯଦି (ବାମ_child(idx) >= cp->size)
		ଫେରନ୍ତୁ;

	/* adapted from lib/prio_heap.c */
	ଯେତେବେଳେ (1) {
		u64 largest_dl;

		l = ବାମ_child(idx);
		r = ଡାହାଣ_child(idx);
		largest = idx;
		largest_dl = orig_dl;

		ଯଦି ((l < cp->size) && dl_time_before(orig_dl,
						cp->elements[l].dl)) {
			largest = l;
			largest_dl = cp->elements[l].dl;
		}
		ଯଦି ((r < cp->size) && dl_time_before(largest_dl,
						cp->elements[r].dl))
			largest = r;

		ଯଦି (largest == idx)
			ଭାଙ୍ଗ;

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

ସ୍ଥିର ଶୂନ୍ୟ cpudl_heapify_up(ସଂରଚନା cpudl *cp, ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା p;

	ପୂର୍ଣ୍ଣସଂଖ୍ୟା orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	ଯଦି (idx == 0)
		ଫେରନ୍ତୁ;

	କର {
		p = parent(idx);
		ଯଦି (dl_time_before(orig_dl, cp->elements[p].dl))
			ଭାଙ୍ଗ;
		/* pull parent onto idx */
		cp->elements[idx].cpu = cp->elements[p].cpu;
		cp->elements[idx].dl = cp->elements[p].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = p;
	} ଯେତେବେଳେ (idx != 0);
	/* actual push up of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

ସ୍ଥିର ଶୂନ୍ୟ cpudl_heapify(ସଂରଚନା cpudl *cp, ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx)
{
	ଯଦି (idx > 0 && dl_time_before(cp->elements[parent(idx)].dl,
				cp->elements[idx].dl))
		cpudl_heapify_up(cp, idx);
	ଅନ୍ୟଥା
		cpudl_heapify_down(cp, idx);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpudl_maximum(ସଂରଚନା cpudl *cp)
{
	ଫେରନ୍ତୁ cp->elements[0].cpu;
}

/*
 * cpudl_find - find the best (later-dl) CPU in the system
 * @cp: the cpudl max-heap context
 * @p: the task
 * @later_mask: a mask to fill in with the selected CPUs (or NULL)
 *
 * Returns: int - CPUs were found
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpudl_find(ସଂରଚନା cpudl *cp, ସଂରଚନା task_struct *p,
	       ସଂରଚନା cpumask *later_mask)
{
	ଅଚଳ ସଂରଚନା sched_dl_entity *dl_se = &p->dl;

	ଯଦି (later_mask &&
	    cpumask_and(later_mask, cp->free_cpus, &p->cpus_mask)) {
		unsigned long cap, max_cap = 0;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, max_cpu = -1;

		ଯଦି (!sched_asym_cpucap_active())
			ଫେରନ୍ତୁ 1;

		/* Ensure the capacity of the CPUs fits the task. */
		for_each_cpu(cpu, later_mask) {
			ଯଦି (!dl_task_fits_capacity(p, cpu)) {
				cpumask_clear_cpu(cpu, later_mask);

				cap = arch_scale_cpu_capacity(cpu);

				ଯଦି (cap > max_cap ||
				    (cpu == task_cpu(p) && cap == max_cap)) {
					max_cap = cap;
					max_cpu = cpu;
				}
			}
		}

		ଯଦି (cpumask_empty(later_mask))
			cpumask_set_cpu(max_cpu, later_mask);

		ଫେରନ୍ତୁ 1;
	} ଅନ୍ୟଥା {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା best_cpu = cpudl_maximum(cp);

		WARN_ON(best_cpu != -1 && !cpu_present(best_cpu));

		ଯଦି (cpumask_test_cpu(best_cpu, &p->cpus_mask) &&
		    dl_time_before(dl_se->deadline, cp->elements[0].dl)) {
			ଯଦି (later_mask)
				cpumask_set_cpu(best_cpu, later_mask);

			ଫେରନ୍ତୁ 1;
		}
	}
	ଫେରନ୍ତୁ 0;
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
ଶୂନ୍ୟ cpudl_clear(ସଂରଚନା cpudl *cp, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, bool online)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_idx, new_cpu;
	unsigned long flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	ଯଦି (old_idx == IDX_INVALID) {
		/*
		 * Nothing to remove if old_idx was invalid.
		 * This could happen if rq_online_dl or rq_offline_dl is
		 * called for a CPU without -dl tasks running.
		 */
	} ଅନ୍ୟଥା {
		new_cpu = cp->elements[cp->size - 1].cpu;
		cp->elements[old_idx].dl = cp->elements[cp->size - 1].dl;
		cp->elements[old_idx].cpu = new_cpu;
		cp->size--;
		cp->elements[new_cpu].idx = old_idx;
		cp->elements[cpu].idx = IDX_INVALID;
		cpudl_heapify(cp, old_idx);
	}
	ଯଦି (likely(online))
		__cpumask_set_cpu(cpu, cp->free_cpus);
	ଅନ୍ୟଥା
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
ଶୂନ୍ୟ cpudl_set(ସଂରଚନା cpudl *cp, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, u64 dl)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_idx;
	unsigned long flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	ଯଦି (old_idx == IDX_INVALID) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_idx = cp->size++;

		cp->elements[new_idx].dl = dl;
		cp->elements[new_idx].cpu = cpu;
		cp->elements[cpu].idx = new_idx;
		cpudl_heapify_up(cp, new_idx);
		__cpumask_clear_cpu(cpu, cp->free_cpus);
	} ଅନ୍ୟଥା {
		cp->elements[old_idx].dl = dl;
		cpudl_heapify(cp, old_idx);
	}

	raw_spin_unlock_irqrestore(&cp->lock, flags);
}

/*
 * cpudl_init - initialize the cpudl structure
 * @cp: the cpudl max-heap context
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpudl_init(ସଂରଚନା cpudl *cp)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	raw_spin_lock_init(&cp->lock);
	cp->size = 0;

	cp->elements = kzalloc_objs(ସଂରଚନା cpudl_item, nr_cpu_ids);
	ଯଦି (!cp->elements)
		ଫେରନ୍ତୁ -ENOMEM;

	ଯଦି (!zalloc_cpumask_var(&cp->free_cpus, GFP_KERNEL)) {
		kfree(cp->elements);
		ଫେରନ୍ତୁ -ENOMEM;
	}

	for_each_possible_cpu(i)
		cp->elements[i].idx = IDX_INVALID;

	ଫେରନ୍ତୁ 0;
}

/*
 * cpudl_cleanup - clean up the cpudl structure
 * @cp: the cpudl max-heap context
 */
ଶୂନ୍ୟ cpudl_cleanup(ସଂରଚନା cpudl *cp)
{
	free_cpumask_var(cp->free_cpus);
	kfree(cp->elements);
}
