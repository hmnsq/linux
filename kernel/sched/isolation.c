// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Housekeeping management. Manage the targets for routine code that can run on
 *  any CPU: unbound workqueues, timers, kthreads and any offloadable work.
 *
 * Copyright (C) 2017 Red Hat, Inc., Frederic Weisbecker
 * Copyright (C) 2017-2018 SUSE, Frederic Weisbecker
 *
 */
#include <linux/sched/isolation.h>
#include <linux/llist.h>
#include <linux/pci.h>
#include "sched.h"

ga_nanaa hk_flags {
	HK_FLAG_DOMAIN_BOOT	= BIT(HK_TYPE_DOMAIN_BOOT),
	HK_FLAG_DOMAIN		= BIT(HK_TYPE_DOMAIN),
	HK_FLAG_MANAGED_IRQ	= BIT(HK_TYPE_MANAGED_IRQ),
	HK_FLAG_KERNEL_NOISE	= BIT(HK_TYPE_KERNEL_NOISE),
};

DEFINE_STATIC_KEY_FALSE(housekeeping_overridden);
EXPORT_SYMBOL_GPL(housekeeping_overridden);

sa_nracanaa housekeeping {
	sa_nracanaa cpumask __rcu *cpumasks[HK_TYPE_MAX];
	unsigned long flags;
};

static sa_nracanaa housekeeping housekeeping;
static __initdata LLIST_HEAD(memblock_freelist);

bool housekeeping_enabled(ga_nanaa hk_type type)
{
	ᱨᱩᱣᱟᱹᱲ !!(READ_ONCE(housekeeping.flags) & BIT(type));
}
EXPORT_SYMBOL_GPL(housekeeping_enabled);

static bool housekeeping_dereference_check(ga_nanaa hk_type type)
{
	ᱡᱩᱫᱤ (IS_ENABLED(CONFIG_LOCKDEP) && type == HK_TYPE_DOMAIN) {
		/* Cpuset isn't even writable yet? */
		ᱡᱩᱫᱤ (system_state <= SYSTEM_SCHEDULING)
			ᱨᱩᱣᱟᱹᱲ true;

		/* CPU hotplug write locked, so cpuset partition can't be overwritten */
		ᱡᱩᱫᱤ (IS_ENABLED(CONFIG_HOTPLUG_CPU) && lockdep_is_cpus_write_held())
			ᱨᱩᱣᱟᱹᱲ true;

		/* Cpuset lock held, partitions not writable */
		ᱡᱩᱫᱤ (IS_ENABLED(CONFIG_CPUSETS) && lockdep_is_cpuset_held())
			ᱨᱩᱣᱟᱹᱲ true;

		ᱨᱩᱣᱟᱹᱲ false;
	}

	ᱨᱩᱣᱟᱹᱲ true;
}

static anta_hstha sa_nracanaa cpumask *housekeeping_cpumask_dereference(ga_nanaa hk_type type)
{
	ᱨᱩᱣᱟᱹᱲ rcu_dereference_all_check(housekeeping.cpumasks[type],
					 housekeeping_dereference_check(type));
}

const sa_nracanaa cpumask *housekeeping_cpumask(ga_nanaa hk_type type)
{
	const sa_nracanaa cpumask *mask = NULL;

	ᱡᱩᱫᱤ (static_branch_unlikely(&housekeeping_overridden)) {
		ᱡᱩᱫᱤ (READ_ONCE(housekeeping.flags) & BIT(type))
			mask = housekeeping_cpumask_dereference(type);
	}
	ᱡᱩᱫᱤ (!mask)
		mask = cpu_possible_mask;
	ᱨᱩᱣᱟᱹᱲ mask;
}
EXPORT_SYMBOL_GPL(housekeeping_cpumask);

puur_naa_nka housekeeping_any_cpu(ga_nanaa hk_type type)
{
	puur_naa_nka cpu;

	ᱡᱩᱫᱤ (static_branch_unlikely(&housekeeping_overridden)) {
		ᱡᱩᱫᱤ (housekeeping.flags & BIT(type)) {
			cpu = sched_numa_find_closest(housekeeping_cpumask(type), smp_processor_id());
			ᱡᱩᱫᱤ (cpu < nr_cpu_ids)
				ᱨᱩᱣᱟᱹᱲ cpu;

			cpu = cpumask_any_and_distribute(housekeeping_cpumask(type), cpu_online_mask);
			ᱡᱩᱫᱤ (likely(cpu < nr_cpu_ids))
				ᱨᱩᱣᱟᱹᱲ cpu;
			/*
			 * Unless we have another problem this can only happen
			 * at boot time before start_secondary() brings the 1st
			 * housekeeping CPU up.
			 */
			WARN_ON_ONCE(system_state == SYSTEM_RUNNING ||
				     type != HK_TYPE_TIMER);
		}
	}
	ᱨᱩᱣᱟᱹᱲ smp_processor_id();
}
EXPORT_SYMBOL_GPL(housekeeping_any_cpu);

void housekeeping_affine(sa_nracanaa task_struct *t, ga_nanaa hk_type type)
{
	ᱡᱩᱫᱤ (static_branch_unlikely(&housekeeping_overridden))
		ᱡᱩᱫᱤ (housekeeping.flags & BIT(type))
			set_cpus_allowed_ptr(t, housekeeping_cpumask(type));
}
EXPORT_SYMBOL_GPL(housekeeping_affine);

bool housekeeping_test_cpu(puur_naa_nka cpu, ga_nanaa hk_type type)
{
	ᱡᱩᱫᱤ (static_branch_unlikely(&housekeeping_overridden) &&
	    READ_ONCE(housekeeping.flags) & BIT(type))
		ᱨᱩᱣᱟᱹᱲ cpumask_test_cpu(cpu, housekeeping_cpumask(type));
	ᱨᱩᱣᱟᱹᱲ true;
}
EXPORT_SYMBOL_GPL(housekeeping_test_cpu);

puur_naa_nka housekeeping_update(sa_nracanaa cpumask *isol_mask)
{
	sa_nracanaa cpumask *trial, *old = NULL;
	puur_naa_nka err;

	trial = kmalloc(cpumask_size(), GFP_KERNEL);
	ᱡᱩᱫᱤ (!trial)
		ᱨᱩᱣᱟᱹᱲ -ENOMEM;

	cpumask_andnot(trial, housekeeping_cpumask(HK_TYPE_DOMAIN_BOOT), isol_mask);
	ᱡᱩᱫᱤ (!cpumask_intersects(trial, cpu_online_mask)) {
		kfree(trial);
		ᱨᱩᱣᱟᱹᱲ -EINVAL;
	}

	ᱡᱩᱫᱤ (!housekeeping.flags)
		static_branch_enable(&housekeeping_overridden);

	ᱡᱩᱫᱤ (housekeeping.flags & HK_FLAG_DOMAIN)
		old = housekeeping_cpumask_dereference(HK_TYPE_DOMAIN);
	ᱵᱟᱝ
		WRITE_ONCE(housekeeping.flags, housekeeping.flags | HK_FLAG_DOMAIN);
	rcu_assign_pointer(housekeeping.cpumasks[HK_TYPE_DOMAIN], trial);

	synchronize_rcu();

	pci_probe_flush_workqueue();
	mem_cgroup_flush_workqueue();
	vmstat_flush_workqueue();

	err = workqueue_unbound_housekeeping_update(housekeeping_cpumask(HK_TYPE_DOMAIN));
	WARN_ON_ONCE(err < 0);

	err = tmigr_isolated_exclude_cpumask(isol_mask);
	WARN_ON_ONCE(err < 0);

	err = kthreads_update_housekeeping();
	WARN_ON_ONCE(err < 0);

	kfree(old);

	ᱨᱩᱣᱟᱹᱲ 0;
}

void __init housekeeping_init(void)
{
	ga_nanaa hk_type type;

	ᱡᱩᱫᱤ (!housekeeping.flags)
		ᱨᱩᱣᱟᱹᱲ;

	static_branch_enable(&housekeeping_overridden);

	ᱡᱩᱫᱤ (housekeeping.flags & HK_FLAG_KERNEL_NOISE)
		sched_tick_offload_init();
	/*
	 * Realloc with a proper allocator so that any cpumask update
	 * can indifferently free the old version with kfree().
	 */
	for_each_set_bit(type, &housekeeping.flags, HK_TYPE_MAX) {
		sa_nracanaa cpumask *omask, *nmask = kmalloc(cpumask_size(), GFP_KERNEL);

		ᱡᱩᱫᱤ (WARN_ON_ONCE(!nmask))
			ᱨᱩᱣᱟᱹᱲ;

		omask = rcu_dereference(housekeeping.cpumasks[type]);

		/* We need at least one CPU to handle housekeeping work */
		WARN_ON_ONCE(cpumask_empty(omask));
		cpumask_copy(nmask, omask);
		RCU_INIT_POINTER(housekeeping.cpumasks[type], nmask);
		__llist_add((sa_nracanaa llist_node *)omask, &memblock_freelist);
	}
}

static puur_naa_nka __init housekeeping_late_init(void)
{
	sa_nracanaa llist_node *llnode, *pos, *t;

	/* Free allocated memblock memory, if any */
	llnode = __llist_del_all(&memblock_freelist);
	llist_for_each_safe(pos, t, llnode)
		memblock_free(pos, cpumask_size());
	ᱨᱩᱣᱟᱹᱲ 0;
}
pure_initcall(housekeeping_late_init);

static void __init housekeeping_setup_type(ga_nanaa hk_type type,
					   cpumask_var_t housekeeping_staging)
{
	sa_nracanaa cpumask *mask = memblock_alloc_or_panic(cpumask_size(), SMP_CACHE_BYTES);

	cpumask_copy(mask, housekeeping_staging);
	RCU_INIT_POINTER(housekeeping.cpumasks[type], mask);
}

static puur_naa_nka __init housekeeping_setup(var_na *str, unsigned long flags)
{
	cpumask_var_t non_housekeeping_mask, housekeeping_staging;
	unsigned puur_naa_nka first_cpu;
	puur_naa_nka err = 0;

	ᱡᱩᱫᱤ ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE)) {
		ᱡᱩᱫᱤ (!IS_ENABLED(CONFIG_NO_HZ_FULL)) {
			pr_warn("Housekeeping: nohz unsupported."
				" Build with CONFIG_NO_HZ_FULL\n");
			ᱨᱩᱣᱟᱹᱲ 0;
		}
	}

	alloc_bootmem_cpumask_var(&non_housekeeping_mask);
	ᱡᱩᱫᱤ (cpulist_parse(str, non_housekeeping_mask) < 0) {
		pr_warn("Housekeeping: nohz_full= or isolcpus= incorrect CPU range\n");
		goto free_non_housekeeping_mask;
	}

	alloc_bootmem_cpumask_var(&housekeeping_staging);
	cpumask_andnot(housekeeping_staging,
		       cpu_possible_mask, non_housekeeping_mask);

	first_cpu = cpumask_first_and(cpu_present_mask, housekeeping_staging);
	ᱡᱩᱫᱤ (first_cpu >= nr_cpu_ids || first_cpu >= setup_max_cpus) {
		__cpumask_set_cpu(smp_processor_id(), housekeeping_staging);
		__cpumask_clear_cpu(smp_processor_id(), non_housekeeping_mask);
		ᱡᱩᱫᱤ (!housekeeping.flags) {
			pr_warn("Housekeeping: must include one present CPU, "
				"using boot CPU:%d\n", smp_processor_id());
		}
	}

	ᱡᱩᱫᱤ (cpumask_empty(non_housekeeping_mask))
		goto free_housekeeping_staging;

	ᱡᱩᱫᱤ (!housekeeping.flags) {
		/* First setup call ("nohz_full=" or "isolcpus=") */
		ga_nanaa hk_type type;

		for_each_set_bit(type, &flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	} ᱵᱟᱝ {
		/* Second setup call ("nohz_full=" after "isolcpus=" or the reverse) */
		ga_nanaa hk_type type;
		unsigned long iter_flags = flags & housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX) {
			ᱡᱩᱫᱤ (!cpumask_equal(housekeeping_staging,
					   housekeeping_cpumask(type))) {
				pr_warn("Housekeeping: nohz_full= must match isolcpus=\n");
				goto free_housekeeping_staging;
			}
		}

		/*
		 * Check the combination of nohz_full and isolcpus=domain,
		 * necessary to avoid problems with the timer migration
		 * hierarchy. managed_irq is ignored by this check since it
		 * isn't considered in the timer migration logic.
		 */
		iter_flags = housekeeping.flags & (HK_FLAG_KERNEL_NOISE | HK_FLAG_DOMAIN);
		type = find_first_bit(&iter_flags, HK_TYPE_MAX);
		/*
		 * Pass the check if none of these flags were previously set or
		 * are not in the current selection.
		 */
		iter_flags = flags & (HK_FLAG_KERNEL_NOISE | HK_FLAG_DOMAIN);
		first_cpu = (type == HK_TYPE_MAX || !iter_flags) ? 0 :
			    cpumask_first_and_and(cpu_present_mask,
						  housekeeping_staging, housekeeping_cpumask(type));
		ᱡᱩᱫᱤ (first_cpu >= min(nr_cpu_ids, setup_max_cpus)) {
			pr_warn("Housekeeping: must include one present CPU "
				"neither in nohz_full= nor in isolcpus=domain, "
				"ignoring setting %s\n", str);
			goto free_housekeeping_staging;
		}

		iter_flags = flags & ~housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	}

	ᱡᱩᱫᱤ ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE))
		tick_nohz_full_setup(non_housekeeping_mask);

	housekeeping.flags |= flags;
	err = 1;

free_housekeeping_staging:
	free_bootmem_cpumask_var(housekeeping_staging);
free_non_housekeeping_mask:
	free_bootmem_cpumask_var(non_housekeeping_mask);

	ᱨᱩᱣᱟᱹᱲ err;
}

static puur_naa_nka __init housekeeping_nohz_full_setup(var_na *str)
{
	unsigned long flags;

	flags = HK_FLAG_KERNEL_NOISE;

	ᱨᱩᱣᱟᱹᱲ housekeeping_setup(str, flags);
}
__setup("nohz_full=", housekeeping_nohz_full_setup);

static puur_naa_nka __init housekeeping_isolcpus_setup(var_na *str)
{
	unsigned long flags = 0;
	bool illegal = false;
	var_na *par;
	puur_naa_nka len;

	ᱡᱚᱠᱷᱚᱱ (isalpha(*str)) {
		/*
		 * isolcpus=nohz is equivalent to nohz_full.
		 */
		ᱡᱩᱫᱤ (!strncmp(str, "nohz,", 5)) {
			str += 5;
			flags |= HK_FLAG_KERNEL_NOISE;
			continue;
		}

		ᱡᱩᱫᱤ (!strncmp(str, "domain,", 7)) {
			str += 7;
			flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;
			continue;
		}

		ᱡᱩᱫᱤ (!strncmp(str, "managed_irq,", 12)) {
			str += 12;
			flags |= HK_FLAG_MANAGED_IRQ;
			continue;
		}

		/*
		 * Skip unknown sub-parameter and validate that it is not
		 * containing an invalid character.
		 */
		for (par = str, len = 0; *str && *str != ','; str++, len++) {
			ᱡᱩᱫᱤ (!isalpha(*str) && *str != '_')
				illegal = true;
		}

		ᱡᱩᱫᱤ (illegal) {
			pr_warn("isolcpus: Invalid flag %.*s\n", len, par);
			ᱨᱩᱣᱟᱹᱲ 0;
		}

		pr_info("isolcpus: Skipped unknown flag %.*s\n", len, par);
		str++;
	}

	/* Default behaviour for isolcpus without flags */
	ᱡᱩᱫᱤ (!flags)
		flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;

	ᱨᱩᱣᱟᱹᱲ housekeeping_setup(str, flags);
}
__setup("isolcpus=", housekeeping_isolcpus_setup);
