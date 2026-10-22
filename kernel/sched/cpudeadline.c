// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/cpudeadline.c
 *
 *  Global CPU deadline management
 *
 *  Author: Juri Lelli <j.lelli@sssup.it>
 */
#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ ਮਾਪਾ(ਪੂਰਨ i)
{
	ਵਾਪਸ (i - 1) >> 1;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ ਖੱਬਾ_ਬੱਚਾ(ਪੂਰਨ i)
{
	ਵਾਪਸ (i << 1) + 1;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ ਸੱਜਾ_ਬੱਚਾ(ਪੂਰਨ i)
{
	ਵਾਪਸ (i << 1) + 2;
}

ਸਥਿਰ ਖਾਲੀ cpudl_heapify_ਥੱਲੇ(ਬਣਤਰ cpudl *cp, ਪੂਰਨ idx)
{
	ਪੂਰਨ l, r, ਸਭ_ਤੋਂ_ਵੱਡਾ;

	ਪੂਰਨ orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	ਜੇ (ਖੱਬਾ_ਬੱਚਾ(idx) >= cp->ਮਾਪ)
		ਵਾਪਸ;

	/* adapted from lib/prio_heap.c */
	ਜਦੋਂ (1) {
		u64 ਸਭ_ਤੋਂ_ਵੱਡਾ_dl;

		l = ਖੱਬਾ_ਬੱਚਾ(idx);
		r = ਸੱਜਾ_ਬੱਚਾ(idx);
		ਸਭ_ਤੋਂ_ਵੱਡਾ = idx;
		ਸਭ_ਤੋਂ_ਵੱਡਾ_dl = orig_dl;

		ਜੇ ((l < cp->ਮਾਪ) && dl_ਸਮਾਂ_before(orig_dl,
						cp->elements[l].dl)) {
			ਸਭ_ਤੋਂ_ਵੱਡਾ = l;
			ਸਭ_ਤੋਂ_ਵੱਡਾ_dl = cp->elements[l].dl;
		}
		ਜੇ ((r < cp->ਮਾਪ) && dl_ਸਮਾਂ_before(ਸਭ_ਤੋਂ_ਵੱਡਾ_dl,
						cp->elements[r].dl))
			ਸਭ_ਤੋਂ_ਵੱਡਾ = r;

		ਜੇ (ਸਭ_ਤੋਂ_ਵੱਡਾ == idx)
			ਰੋਕੋ;

		/* pull largest child onto idx */
		cp->elements[idx].cpu = cp->elements[ਸਭ_ਤੋਂ_ਵੱਡਾ].cpu;
		cp->elements[idx].dl = cp->elements[ਸਭ_ਤੋਂ_ਵੱਡਾ].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = ਸਭ_ਤੋਂ_ਵੱਡਾ;
	}
	/* actual push down of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

ਸਥਿਰ ਖਾਲੀ cpudl_heapify_ਉੱਪਰ(ਬਣਤਰ cpudl *cp, ਪੂਰਨ idx)
{
	ਪੂਰਨ p;

	ਪੂਰਨ orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	ਜੇ (idx == 0)
		ਵਾਪਸ;

	ਕਰੋ {
		p = ਮਾਪਾ(idx);
		ਜੇ (dl_ਸਮਾਂ_before(orig_dl, cp->elements[p].dl))
			ਰੋਕੋ;
		/* pull parent onto idx */
		cp->elements[idx].cpu = cp->elements[p].cpu;
		cp->elements[idx].dl = cp->elements[p].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = p;
	} ਜਦੋਂ (idx != 0);
	/* actual push up of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

ਸਥਿਰ ਖਾਲੀ cpudl_heapify(ਬਣਤਰ cpudl *cp, ਪੂਰਨ idx)
{
	ਜੇ (idx > 0 && dl_ਸਮਾਂ_before(cp->elements[ਮਾਪਾ(idx)].dl,
				cp->elements[idx].dl))
		cpudl_heapify_ਉੱਪਰ(cp, idx);
	ਨਹੀਂਤਾਂ
		cpudl_heapify_ਥੱਲੇ(cp, idx);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ cpudl_ਵੱਧ_ਤੋਂ_ਵੱਧ(ਬਣਤਰ cpudl *cp)
{
	ਵਾਪਸ cp->elements[0].cpu;
}

/*
 * cpudl_find - find the best (later-dl) CPU in the system
 * @cp: the cpudl max-heap context
 * @p: the task
 * @later_mask: a mask to fill in with the selected CPUs (or NULL)
 *
 * Returns: int - CPUs were found
 */
ਪੂਰਨ cpudl_ਲੱਭੋ(ਬਣਤਰ cpudl *cp, ਬਣਤਰ task_struct *p,
	       ਬਣਤਰ cpumask *later_ਨਕਾਬ)
{
	ਅਚਲ ਬਣਤਰ sched_dl_entity *dl_se = &p->dl;

	ਜੇ (later_ਨਕਾਬ &&
	    cpumask_and(later_ਨਕਾਬ, cp->free_cpus, &p->cpus_ਨਕਾਬ)) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cap, max_cap = 0;
		ਪੂਰਨ cpu, max_cpu = -1;

		ਜੇ (!sched_asym_cpucap_ਸਰਗਰਮ())
			ਵਾਪਸ 1;

		/* Ensure the capacity of the CPUs fits the task. */
		for_each_cpu(cpu, later_ਨਕਾਬ) {
			ਜੇ (!dl_task_ਸਮਾਉਂਦਾ_ਸਮਰੱਥਾ(p, cpu)) {
				cpumask_clear_cpu(cpu, later_ਨਕਾਬ);

				cap = arch_scale_cpu_ਸਮਰੱਥਾ(cpu);

				ਜੇ (cap > max_cap ||
				    (cpu == task_cpu(p) && cap == max_cap)) {
					max_cap = cap;
					max_cpu = cpu;
				}
			}
		}

		ਜੇ (cpumask_ਖਾਲੀ_ਥਾਵਾਂ(later_ਨਕਾਬ))
			cpumask_ਸੈੱਟ_cpu(max_cpu, later_ਨਕਾਬ);

		ਵਾਪਸ 1;
	} ਨਹੀਂਤਾਂ {
		ਪੂਰਨ ਉੱਤਮ_cpu = cpudl_ਵੱਧ_ਤੋਂ_ਵੱਧ(cp);

		WARN_ON(ਉੱਤਮ_cpu != -1 && !cpu_present(ਉੱਤਮ_cpu));

		ਜੇ (cpumask_test_cpu(ਉੱਤਮ_cpu, &p->cpus_ਨਕਾਬ) &&
		    dl_ਸਮਾਂ_before(dl_se->deadline, cp->elements[0].dl)) {
			ਜੇ (later_ਨਕਾਬ)
				cpumask_ਸੈੱਟ_cpu(ਉੱਤਮ_cpu, later_ਨਕਾਬ);

			ਵਾਪਸ 1;
		}
	}
	ਵਾਪਸ 0;
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
ਖਾਲੀ cpudl_clear(ਬਣਤਰ cpudl *cp, ਪੂਰਨ cpu, bool online)
{
	ਪੂਰਨ old_idx, new_cpu;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_ਤਾਲਾ_irqsave(&cp->ਤਾਲਾ, flags);

	old_idx = cp->elements[cpu].idx;
	ਜੇ (old_idx == IDX_INVALID) {
		/*
		 * Nothing to remove if old_idx was invalid.
		 * This could happen if rq_online_dl or rq_offline_dl is
		 * called for a CPU without -dl tasks running.
		 */
	} ਨਹੀਂਤਾਂ {
		new_cpu = cp->elements[cp->ਮਾਪ - 1].cpu;
		cp->elements[old_idx].dl = cp->elements[cp->ਮਾਪ - 1].dl;
		cp->elements[old_idx].cpu = new_cpu;
		cp->ਮਾਪ--;
		cp->elements[new_cpu].idx = old_idx;
		cp->elements[cpu].idx = IDX_INVALID;
		cpudl_heapify(cp, old_idx);
	}
	ਜੇ (likely(online))
		__cpumask_set_cpu(cpu, cp->free_cpus);
	ਨਹੀਂਤਾਂ
		__cpumask_clear_cpu(cpu, cp->free_cpus);

	raw_spin_unlock_irqrestore(&cp->ਤਾਲਾ, flags);
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
ਖਾਲੀ cpudl_ਸੈੱਟ(ਬਣਤਰ cpudl *cp, ਪੂਰਨ cpu, u64 dl)
{
	ਪੂਰਨ old_idx;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_ਤਾਲਾ_irqsave(&cp->ਤਾਲਾ, flags);

	old_idx = cp->elements[cpu].idx;
	ਜੇ (old_idx == IDX_INVALID) {
		ਪੂਰਨ new_idx = cp->ਮਾਪ++;

		cp->elements[new_idx].dl = dl;
		cp->elements[new_idx].cpu = cpu;
		cp->elements[cpu].idx = new_idx;
		cpudl_heapify_ਉੱਪਰ(cp, new_idx);
		__cpumask_clear_cpu(cpu, cp->free_cpus);
	} ਨਹੀਂਤਾਂ {
		cp->elements[old_idx].dl = dl;
		cpudl_heapify(cp, old_idx);
	}

	raw_spin_unlock_irqrestore(&cp->ਤਾਲਾ, flags);
}

/*
 * cpudl_init - initialize the cpudl structure
 * @cp: the cpudl max-heap context
 */
ਪੂਰਨ cpudl_init(ਬਣਤਰ cpudl *cp)
{
	ਪੂਰਨ i;

	raw_spin_ਤਾਲਾ_init(&cp->ਤਾਲਾ);
	cp->ਮਾਪ = 0;

	cp->elements = kzalloc_objs(ਬਣਤਰ cpudl_ਵਸਤੂ, nr_cpu_ids);
	ਜੇ (!cp->elements)
		ਵਾਪਸ -ENOMEM;

	ਜੇ (!zalloc_cpumask_var(&cp->free_cpus, GFP_KERNEL)) {
		kfree(cp->elements);
		ਵਾਪਸ -ENOMEM;
	}

	for_each_possible_cpu(i)
		cp->elements[i].idx = IDX_INVALID;

	ਵਾਪਸ 0;
}

/*
 * cpudl_cleanup - clean up the cpudl structure
 * @cp: the cpudl max-heap context
 */
ਖਾਲੀ cpudl_cleanup(ਬਣਤਰ cpudl *cp)
{
	free_cpumask_var(cp->free_cpus);
	kfree(cp->elements);
}
