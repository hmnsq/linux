// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/cpudeadline.c
 *
 *  Global CPU deadline management
 *
 *  Author: Juri Lelli <j.lelli@sssup.it>
 */
#شامل "sched.h"

ساکن اندرونی صحیح والد(صحیح i)
{
	واپس (i - 1) >> 1;
}

ساکن اندرونی صحیح بایاں_بچہ(صحیح i)
{
	واپس (i << 1) + 1;
}

ساکن اندرونی صحیح دایاں_بچہ(صحیح i)
{
	واپس (i << 1) + 2;
}

ساکن خالی cpudl_heapify_نیچے_کو(ساخت cpudl *cp, صحیح idx)
{
	صحیح l, r, سب_سے_بڑا;

	صحیح orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	اگر (بایاں_بچہ(idx) >= cp->سائز)
		واپس;

	/* adapted from lib/prio_heap.c */
	جب (1) {
		u64 سب_سے_بڑا_dl;

		l = بایاں_بچہ(idx);
		r = دایاں_بچہ(idx);
		سب_سے_بڑا = idx;
		سب_سے_بڑا_dl = orig_dl;

		اگر ((l < cp->سائز) && dl_وقت_before(orig_dl,
						cp->elements[l].dl)) {
			سب_سے_بڑا = l;
			سب_سے_بڑا_dl = cp->elements[l].dl;
		}
		اگر ((r < cp->سائز) && dl_وقت_before(سب_سے_بڑا_dl,
						cp->elements[r].dl))
			سب_سے_بڑا = r;

		اگر (سب_سے_بڑا == idx)
			توڑو;

		/* pull largest child onto idx */
		cp->elements[idx].cpu = cp->elements[سب_سے_بڑا].cpu;
		cp->elements[idx].dl = cp->elements[سب_سے_بڑا].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = سب_سے_بڑا;
	}
	/* actual push down of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

ساکن خالی cpudl_heapify_اوپر(ساخت cpudl *cp, صحیح idx)
{
	صحیح p;

	صحیح orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	اگر (idx == 0)
		واپس;

	کرو {
		p = والد(idx);
		اگر (dl_وقت_before(orig_dl, cp->elements[p].dl))
			توڑو;
		/* pull parent onto idx */
		cp->elements[idx].cpu = cp->elements[p].cpu;
		cp->elements[idx].dl = cp->elements[p].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = p;
	} جب (idx != 0);
	/* actual push up of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

ساکن خالی cpudl_heapify(ساخت cpudl *cp, صحیح idx)
{
	اگر (idx > 0 && dl_وقت_before(cp->elements[والد(idx)].dl,
				cp->elements[idx].dl))
		cpudl_heapify_اوپر(cp, idx);
	ورنہ
		cpudl_heapify_نیچے_کو(cp, idx);
}

ساکن اندرونی صحیح cpudl_زیادہ_سے_زیادہ(ساخت cpudl *cp)
{
	واپس cp->elements[0].cpu;
}

/*
 * cpudl_find - find the best (later-dl) CPU in the system
 * @cp: the cpudl max-heap context
 * @p: the task
 * @later_mask: a mask to fill in with the selected CPUs (or NULL)
 *
 * Returns: int - CPUs were found
 */
صحیح cpudl_ڈھونڈو(ساخت cpudl *cp, ساخت task_struct *p,
	       ساخت cpumask *later_نقاب)
{
	ثابت ساخت sched_dl_entity *dl_se = &p->dl;

	اگر (later_نقاب &&
	    cpumask_and(later_نقاب, cp->free_cpus, &p->cpus_نقاب)) {
		غیرعلامتی طویل cap, max_cap = 0;
		صحیح cpu, max_cpu = -1;

		اگر (!sched_asym_cpucap_فعال())
			واپس 1;

		/* Ensure the capacity of the CPUs fits the task. */
		for_each_cpu(cpu, later_نقاب) {
			اگر (!dl_task_سماتا_استعداد(p, cpu)) {
				cpumask_clear_cpu(cpu, later_نقاب);

				cap = arch_scale_cpu_استعداد(cpu);

				اگر (cap > max_cap ||
				    (cpu == task_cpu(p) && cap == max_cap)) {
					max_cap = cap;
					max_cpu = cpu;
				}
			}
		}

		اگر (cpumask_خالی_خانے(later_نقاب))
			cpumask_سیٹ_cpu(max_cpu, later_نقاب);

		واپس 1;
	} ورنہ {
		صحیح بہترین_cpu = cpudl_زیادہ_سے_زیادہ(cp);

		WARN_ON(بہترین_cpu != -1 && !cpu_present(بہترین_cpu));

		اگر (cpumask_test_cpu(بہترین_cpu, &p->cpus_نقاب) &&
		    dl_وقت_before(dl_se->deadline, cp->elements[0].dl)) {
			اگر (later_نقاب)
				cpumask_سیٹ_cpu(بہترین_cpu, later_نقاب);

			واپس 1;
		}
	}
	واپس 0;
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
خالی cpudl_clear(ساخت cpudl *cp, صحیح cpu, bool online)
{
	صحیح old_idx, new_cpu;
	غیرعلامتی طویل flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_تالا_irqsave(&cp->تالا, flags);

	old_idx = cp->elements[cpu].idx;
	اگر (old_idx == IDX_INVALID) {
		/*
		 * Nothing to remove if old_idx was invalid.
		 * This could happen if rq_online_dl or rq_offline_dl is
		 * called for a CPU without -dl tasks running.
		 */
	} ورنہ {
		new_cpu = cp->elements[cp->سائز - 1].cpu;
		cp->elements[old_idx].dl = cp->elements[cp->سائز - 1].dl;
		cp->elements[old_idx].cpu = new_cpu;
		cp->سائز--;
		cp->elements[new_cpu].idx = old_idx;
		cp->elements[cpu].idx = IDX_INVALID;
		cpudl_heapify(cp, old_idx);
	}
	اگر (likely(online))
		__cpumask_set_cpu(cpu, cp->free_cpus);
	ورنہ
		__cpumask_clear_cpu(cpu, cp->free_cpus);

	raw_spin_unlock_irqrestore(&cp->تالا, flags);
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
خالی cpudl_سیٹ(ساخت cpudl *cp, صحیح cpu, u64 dl)
{
	صحیح old_idx;
	غیرعلامتی طویل flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_تالا_irqsave(&cp->تالا, flags);

	old_idx = cp->elements[cpu].idx;
	اگر (old_idx == IDX_INVALID) {
		صحیح new_idx = cp->سائز++;

		cp->elements[new_idx].dl = dl;
		cp->elements[new_idx].cpu = cpu;
		cp->elements[cpu].idx = new_idx;
		cpudl_heapify_اوپر(cp, new_idx);
		__cpumask_clear_cpu(cpu, cp->free_cpus);
	} ورنہ {
		cp->elements[old_idx].dl = dl;
		cpudl_heapify(cp, old_idx);
	}

	raw_spin_unlock_irqrestore(&cp->تالا, flags);
}

/*
 * cpudl_init - initialize the cpudl structure
 * @cp: the cpudl max-heap context
 */
صحیح cpudl_init(ساخت cpudl *cp)
{
	صحیح i;

	raw_spin_تالا_init(&cp->تالا);
	cp->سائز = 0;

	cp->elements = kzalloc_objs(ساخت cpudl_شے, nr_cpu_ids);
	اگر (!cp->elements)
		واپس -ENOMEM;

	اگر (!zalloc_cpumask_var(&cp->free_cpus, GFP_KERNEL)) {
		kfree(cp->elements);
		واپس -ENOMEM;
	}

	for_each_possible_cpu(i)
		cp->elements[i].idx = IDX_INVALID;

	واپس 0;
}

/*
 * cpudl_cleanup - clean up the cpudl structure
 * @cp: the cpudl max-heap context
 */
خالی cpudl_cleanup(ساخت cpudl *cp)
{
	free_cpumask_var(cp->free_cpus);
	kfree(cp->elements);
}
