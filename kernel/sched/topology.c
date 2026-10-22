// SPDX-License-Identifier: GPL-2.0
/*
 * Scheduler topology setup/handling methods
 */

#ಅನ್ತರ್ಭಾವ <linux/sched/isolation.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/clock.h>
#ಅನ್ತರ್ಭಾವ <linux/bsearch.h>
#ಅನ್ತರ್ಭಾವ "sched.h"

DEFINE_MUTEX(sched_domains_mutex);
ಶೂನ್ಯ sched_domains_mutex_ಬೀಗ(ಶೂನ್ಯ)
{
	mutex_ಬೀಗ(&sched_domains_mutex);
}
ಶೂನ್ಯ sched_domains_mutex_unlock(ಶೂನ್ಯ)
{
	mutex_unlock(&sched_domains_mutex);
}

/* Protected by sched_domains_mutex: */
ಸ್ಥಿರ cpumask_var_t sched_domains_llc_id_allocmask;
ಸ್ಥಿರ cpumask_var_t sched_domains_tmpmask;
ಸ್ಥಿರ cpumask_var_t sched_domains_tmpmask2;
ಪೂರ್ಣಾಂಕ max_lid;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init sched_debug_ಸಿದ್ಧತೆ(ಅಕ್ಷರ *str)
{
	sched_debug_verbose = true;

	ಹಿಂತಿರುಗಿ 0;
}
early_param("sched_verbose", sched_debug_ಸಿದ್ಧತೆ);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_SMT
ಸ್ಥಿರ bool sched_smt_asym_packing __read_mostly;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init ಸಿದ್ಧತೆ_sched_smt_asym_packing(ಅಕ್ಷರ *str)
{
	ಆದರೆ (strcmp(str, "on"))
		ಹಿಂತಿರುಗಿ 0;

	sched_smt_asym_packing = true;
	ಹಿಂತಿರುಗಿ 1;
}
__setup("sched_smt_asym_packing=", ಸಿದ್ಧತೆ_sched_smt_asym_packing);
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sched_debug(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ sched_debug_verbose;
}

#ವ್ಯಾಖ್ಯಾನಿಸು SD_FLAG(_name, mflags) [__##_name] = { .meta_flags = mflags, .name = #_name },
ಸ್ಥಿರಾಂಕ ರಚನೆ sd_flag_debug sd_flag_debug[] = {
#ಅನ್ತರ್ಭಾವ <linux/sched/sd_flags.h>
};
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು SD_FLAG

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_domain_debug_one(ರಚನೆ sched_domain *sd, ಪೂರ್ಣಾಂಕ cpu, ಪೂರ್ಣಾಂಕ level,
				  ರಚನೆ cpumask *groupmask)
{
	ರಚನೆ sched_ಗುಂಪು *ಗುಂಪು = sd->groups;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags = sd->flags;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ idx;

	cpumask_clear(groupmask);

	printk(KERN_DEBUG "%*s domain-%d: ", level, "", level);
	printk(KERN_CONT "span=%*pbl level=%s\n",
	       cpumask_pr_ವಾದಗಳು(sched_domain_ಹರವು(sd)), sd->name);

	ಆದರೆ (!cpumask_test_cpu(cpu, sched_domain_ಹರವು(sd))) {
		printk(KERN_ERR "ERROR: domain->span does not contain CPU%d\n", cpu);
	}
	ಆದರೆ (ಗುಂಪು && !cpumask_test_cpu(cpu, sched_ಗುಂಪು_ಹರವು(ಗುಂಪು))) {
		printk(KERN_ERR "ERROR: domain->groups does not contain CPU%d\n", cpu);
	}

	for_each_ಗಣ_ಬಿಟ್(idx, &flags, __SD_FLAG_CNT) {
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flag = ಬಿಟ್(idx);
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ meta_flags = sd_flag_debug[idx].meta_flags;

		ಆದರೆ ((meta_flags & SDF_SHARED_ಮಗು) && sd->ಮಗು &&
		    !(sd->ಮಗು->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in child\n",
			       sd_flag_debug[idx].name);

		ಆದರೆ ((meta_flags & SDF_SHARED_ಪೋಷಕ) && sd->ಪೋಷಕ &&
		    !(sd->ಪೋಷಕ->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in parent\n",
			       sd_flag_debug[idx].name);
	}

	printk(KERN_DEBUG "%*s groups:", level + 1, "");
	ಮಾಡು {
		ಆದರೆ (!ಗುಂಪು) {
			printk("\n");
			printk(KERN_ERR "ERROR: group is NULL\n");
			ನಿಲ್ಲಿಸು;
		}

		ಆದರೆ (cpumask_ಖಾಲಿಗಳು(sched_ಗುಂಪು_ಹರವು(ಗುಂಪು))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: empty group\n");
			ನಿಲ್ಲಿಸು;
		}

		ಆದರೆ (!(sd->flags & SD_NUMA) &&
		    cpumask_intersects(groupmask, sched_ಗುಂಪು_ಹರವು(ಗುಂಪು))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: repeated CPUs\n");
			ನಿಲ್ಲಿಸು;
		}

		cpumask_or(groupmask, groupmask, sched_ಗುಂಪು_ಹರವು(ಗುಂಪು));

		printk(KERN_CONT " %d:{ span=%*pbl",
				ಗುಂಪು->sgc->id,
				cpumask_pr_ವಾದಗಳು(sched_ಗುಂಪು_ಹರವು(ಗುಂಪು)));

		ಆದರೆ ((sd->flags & SD_NUMA) &&
		    !cpumask_equal(ಗುಂಪು_balance_ಮುಖವಾಡ(ಗುಂಪು), sched_ಗುಂಪು_ಹರವು(ಗುಂಪು))) {
			printk(KERN_CONT " mask=%*pbl",
				cpumask_pr_ವಾದಗಳು(ಗುಂಪು_balance_ಮುಖವಾಡ(ಗುಂಪು)));
		}

		ಆದರೆ (ಗುಂಪು->sgc->ಸಾಮರ್ಥ್ಯ != SCHED_ಸಾಮರ್ಥ್ಯ_SCALE)
			printk(KERN_CONT " cap=%lu", ಗುಂಪು->sgc->ಸಾಮರ್ಥ್ಯ);

		ಆದರೆ (ಗುಂಪು == sd->groups && sd->ಮಗು &&
		    !cpumask_equal(sched_domain_ಹರವು(sd->ಮಗು),
				   sched_ಗುಂಪು_ಹರವು(ಗುಂಪು))) {
			printk(KERN_ERR "ERROR: domain->groups does not match domain->child\n");
		}

		printk(KERN_CONT " }");

		ಗುಂಪು = ಗುಂಪು->ಮುಂದಿನ;

		ಆದರೆ (ಗುಂಪು != sd->groups)
			printk(KERN_CONT ",");

	} ಆಗ (ಗುಂಪು != sd->groups);
	printk(KERN_CONT "\n");

	ಆದರೆ (!cpumask_equal(sched_domain_ಹರವು(sd), groupmask))
		printk(KERN_ERR "ERROR: groups don't span domain->span\n");

	ಆದರೆ (sd->ಪೋಷಕ &&
	    !cpumask_subset(groupmask, sched_domain_ಹರವು(sd->ಪೋಷಕ)))
		printk(KERN_ERR "ERROR: parent span is not a superset of domain->span\n");
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_domain_debug(ರಚನೆ sched_domain *sd, ಪೂರ್ಣಾಂಕ cpu)
{
	ಪೂರ್ಣಾಂಕ level = 0;

	ಆದರೆ (!sched_debug_verbose)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!sd) {
		printk(KERN_DEBUG "CPU%d attaching NULL sched-domain.\n", cpu);
		ಹಿಂತಿರುಗಿ;
	}

	printk(KERN_DEBUG "CPU%d attaching sched-domain(s):\n", cpu);

	ಗಾಗಿ (;;) {
		ಆದರೆ (sched_domain_debug_one(sd, cpu, level, sched_domains_tmpmask))
			ನಿಲ್ಲಿಸು;
		level++;
		sd = sd->ಪೋಷಕ;
		ಆದರೆ (!sd)
			ನಿಲ್ಲಿಸು;
	}
}

/* Generate a mask of SD flags with the SDF_NEEDS_GROUPS metaflag */
#ವ್ಯಾಖ್ಯಾನಿಸು SD_FLAG(name, mflags) (name * !!((mflags) & SDF_NEEDS_GROUPS)) |
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ SD_DEGENERATE_GROUPS_ಮುಖವಾಡ =
#ಅನ್ತರ್ಭಾವ <linux/sched/sd_flags.h>
0;
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು SD_FLAG

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sd_degenerate(ರಚನೆ sched_domain *sd)
{
	ಆದರೆ (cpumask_ತೂಕ(sched_domain_ಹರವು(sd)) == 1)
		ಹಿಂತಿರುಗಿ 1;

	/* Following flags need at least 2 groups */
	ಆದರೆ ((sd->flags & SD_DEGENERATE_GROUPS_ಮುಖವಾಡ) &&
	    (sd->groups != sd->groups->ಮುಂದಿನ))
		ಹಿಂತಿರುಗಿ 0;

	/* Following flags don't use groups */
	ಆದರೆ (sd->flags & (SD_WAKE_AFFINE))
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
sd_ಪೋಷಕ_degenerate(ರಚನೆ sched_domain *sd, ರಚನೆ sched_domain *ಪೋಷಕ)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cflags = sd->flags, pflags = ಪೋಷಕ->flags;

	ಆದರೆ (sd_degenerate(ಪೋಷಕ))
		ಹಿಂತಿರುಗಿ 1;

	ಆದರೆ (!cpumask_equal(sched_domain_ಹರವು(sd), sched_domain_ಹರವು(ಪೋಷಕ)))
		ಹಿಂತಿರುಗಿ 0;

	/* Flags needing groups don't count if only 1 group in parent */
	ಆದರೆ (ಪೋಷಕ->groups == ಪೋಷಕ->groups->ಮುಂದಿನ)
		pflags &= ~SD_DEGENERATE_GROUPS_ಮುಖವಾಡ;

	ಆದರೆ (~cflags & pflags)
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ 1;
}

#ಆದರೆ defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
DEFINE_STATIC_ಕೀಲಿ_FALSE(sched_energy_present);
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_sched_energy_aware = 1;
ಸ್ಥಿರ DEFINE_MUTEX(sched_energy_mutex);
ಸ್ಥಿರ bool sched_energy_update;

ಸ್ಥಿರ bool sched_is_eas_possible(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_ಮುಖವಾಡ)
{
	bool any_asym_ಸಾಮರ್ಥ್ಯ = false;
	ಪೂರ್ಣಾಂಕ i;

	/* EAS is enabled for asymmetric CPU capacity topologies. */
	for_each_cpu(i, cpu_ಮುಖವಾಡ) {
		ಆದರೆ (rcu_access_ಸೂಚಕ(ಪ್ರತಿ_cpu(sd_asym_cpucapacity, i))) {
			any_asym_ಸಾಮರ್ಥ್ಯ = true;
			ನಿಲ್ಲಿಸು;
		}
	}
	ಆದರೆ (!any_asym_ಸಾಮರ್ಥ್ಯ) {
		ಆದರೆ (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, CPUs do not have asymmetric capacities\n",
				cpumask_pr_ವಾದಗಳು(cpu_ಮುಖವಾಡ));
		}
		ಹಿಂತಿರುಗಿ false;
	}

	/* EAS definitely does *not* handle SMT */
	ಆದರೆ (sched_smt_ಸಕ್ರಿಯ()) {
		ಆದರೆ (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, SMT is not supported\n",
				cpumask_pr_ವಾದಗಳು(cpu_ಮುಖವಾಡ));
		}
		ಹಿಂತಿರುಗಿ false;
	}

	ಆದರೆ (!arch_scale_freq_invariant()) {
		ಆದರೆ (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: frequency-invariant load tracking not yet supported",
				cpumask_pr_ವಾದಗಳು(cpu_ಮುಖವಾಡ));
		}
		ಹಿಂತಿರುಗಿ false;
	}

	ಆದರೆ (!cpufreq_ready_for_eas(cpu_ಮುಖವಾಡ)) {
		ಆದರೆ (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: cpufreq is not ready\n",
				cpumask_pr_ವಾದಗಳು(cpu_ಮುಖವಾಡ));
		}
		ಹಿಂತಿರುಗಿ false;
	}

	ಹಿಂತಿರುಗಿ true;
}

ಶೂನ್ಯ rebuild_sched_domains_energy(ಶೂನ್ಯ)
{
	mutex_ಬೀಗ(&sched_energy_mutex);
	sched_energy_update = true;
	rebuild_sched_domains();
	sched_energy_update = false;
	mutex_unlock(&sched_energy_mutex);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SYSCTL
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_energy_aware_handler(ಸ್ಥಿರಾಂಕ ರಚನೆ ctl_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ, ಪೂರ್ಣಾಂಕ ಲಿಖ,
		ಶೂನ್ಯ *ಬಫರ್, ಗಾತ್ರ_t *lenp, loff_t *ppos)
{
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (ಲಿಖ && !capable(CAP_SYS_ADMIN))
		ಹಿಂತಿರುಗಿ -EPERM;

	ಆದರೆ (!sched_is_eas_possible(cpu_ಸಕ್ರಿಯ_ಮುಖವಾಡ)) {
		ಆದರೆ (ಲಿಖ) {
			ಹಿಂತಿರುಗಿ -EOPNOTSUPP;
		} ಇಲ್ಲದಿದ್ದರೆ {
			*lenp = 0;
			ಹಿಂತಿರುಗಿ 0;
		}
	}

	ret = proc_dointvec_minmax(ಕೋಷ್ಟಕ, ಲಿಖ, ಬಫರ್, lenp, ppos);
	ಆದರೆ (!ret && ಲಿಖ) {
		ಆದರೆ (sysctl_sched_energy_aware != sched_energy_enabled())
			rebuild_sched_domains_energy();
	}

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ ctl_ಕೋಷ್ಟಕ sched_energy_aware_sysctls[] = {
	{
		.procname       = "sched_energy_aware",
		.ದತ್ತಾಂಶ           = &sysctl_sched_energy_aware,
		.maxlen         = ಗಾತ್ರ(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ),
		.ಬಹುಲಕ           = 0644,
		.proc_handler   = sched_energy_aware_handler,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init sched_energy_aware_sysctl_init(ಶೂನ್ಯ)
{
	ರಿಜಿಸ್ಟರ್_sysctl_init("kernel", sched_energy_aware_sysctls);
	ಹಿಂತಿರುಗಿ 0;
}

late_initcall(sched_energy_aware_sysctl_init);
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SYSCTL */

ಸ್ಥಿರ ಶೂನ್ಯ free_pd(ರಚನೆ perf_domain *pd)
{
	ರಚನೆ perf_domain *tmp;

	ಆಗ (pd) {
		tmp = pd->ಮುಂದಿನ;
		kfree(pd);
		pd = tmp;
	}
}

ಸ್ಥಿರ ರಚನೆ perf_domain *ಕಂಡುಹಿಡಿ_pd(ರಚನೆ perf_domain *pd, ಪೂರ್ಣಾಂಕ cpu)
{
	ಆಗ (pd) {
		ಆದರೆ (cpumask_test_cpu(cpu, perf_domain_ಹರವು(pd)))
			ಹಿಂತಿರುಗಿ pd;
		pd = pd->ಮುಂದಿನ;
	}

	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ರಚನೆ perf_domain *pd_init(ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ em_perf_domain *obj = em_cpu_get(cpu);
	ರಚನೆ perf_domain *pd;

	ಆದರೆ (!obj) {
		ಆದರೆ (sched_debug())
			pr_info("%s: no EM found for CPU%d\n", __func__, cpu);
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}

	pd = kzalloc_obj(*pd);
	ಆದರೆ (!pd)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	pd->em_pd = obj;

	ಹಿಂತಿರುಗಿ pd;
}

ಸ್ಥಿರ ಶೂನ್ಯ perf_domain_debug(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_map,
						ರಚನೆ perf_domain *pd)
{
	ಆದರೆ (!sched_debug() || !pd)
		ಹಿಂತಿರುಗಿ;

	printk(KERN_DEBUG "root_domain %*pbl:", cpumask_pr_ವಾದಗಳು(cpu_map));

	ಆಗ (pd) {
		printk(KERN_CONT " pd%d:{ cpus=%*pbl nr_pstate=%d }",
				cpumask_ಮೊದಲ(perf_domain_ಹರವು(pd)),
				cpumask_pr_ವಾದಗಳು(perf_domain_ಹರವು(pd)),
				em_pd_nr_perf_ಸ್ಥಿತಿಗಳು(pd->em_pd));
		pd = pd->ಮುಂದಿನ;
	}

	printk(KERN_CONT "\n");
}

ಸ್ಥಿರ ಶೂನ್ಯ destroy_perf_domain_rcu(ರಚನೆ rcu_ತಲೆ *rp)
{
	ರಚನೆ perf_domain *pd;

	pd = container_ದ(rp, ರಚನೆ perf_domain, rcu);
	free_pd(pd);
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_energy_ಗಣ(bool has_eas)
{
	ಆದರೆ (!has_eas && sched_energy_enabled()) {
		ಆದರೆ (sched_debug())
			pr_info("%s: stopping EAS\n", __func__);
		static_branch_disable_cpuslocked(&sched_energy_present);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (has_eas && !sched_energy_enabled()) {
		ಆದರೆ (sched_debug())
			pr_info("%s: starting EAS\n", __func__);
		static_branch_enable_cpuslocked(&sched_energy_present);
	}
}

/*
 * EAS can be used on a root domain if it meets all the following conditions:
 *    1. an Energy Model (EM) is available;
 *    2. the SD_ASYM_CPUCAPACITY flag is set in the sched_domain hierarchy.
 *    3. no SMT is detected.
 *    4. schedutil is driving the frequency of all CPUs of the rd;
 *    5. frequency invariance support is present;
 */
ಸ್ಥಿರ bool build_perf_domains(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_map)
{
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ perf_domain *pd = ಖಾಲಿ, *tmp;
	ಪೂರ್ಣಾಂಕ cpu = cpumask_ಮೊದಲ(cpu_map);
	ರಚನೆ ಬೇರು_domain *rd = cpu_rq(cpu)->rd;

	ಆದರೆ (!sysctl_sched_energy_aware)
		ಹೋಗು ಬಿಡುಗಡೆ;

	ಆದರೆ (!sched_is_eas_possible(cpu_map))
		ಹೋಗು ಬಿಡುಗಡೆ;

	for_each_cpu(i, cpu_map) {
		/* Skip already covered CPUs. */
		ಆದರೆ (ಕಂಡುಹಿಡಿ_pd(pd, i))
			ಮುಂದುವರಿ;

		/* Create the new pd and add it to the local list. */
		tmp = pd_init(i);
		ಆದರೆ (!tmp)
			ಹೋಗು ಬಿಡುಗಡೆ;
		tmp->ಮುಂದಿನ = pd;
		pd = tmp;
	}

	perf_domain_debug(cpu_map, pd);

	/* Attach the new list of performance domains to the root domain. */
	tmp = rd->pd;
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(rd->pd, pd);
	ಆದರೆ (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	ಹಿಂತಿರುಗಿ !!pd;

ಬಿಡುಗಡೆ:
	free_pd(pd);
	tmp = rd->pd;
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(rd->pd, ಖಾಲಿ);
	ಆದರೆ (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	ಹಿಂತಿರುಗಿ false;
}
#ಇಲ್ಲದಿದ್ದರೆ /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */
ಸ್ಥಿರ ಶೂನ್ಯ free_pd(ರಚನೆ perf_domain *pd) { }
#ಆದರೆ_ಅಂತ್ಯ /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

ಸ್ಥಿರ ಶೂನ್ಯ free_rootdomain(ರಚನೆ rcu_ತಲೆ *rcu)
{
	ರಚನೆ ಬೇರು_domain *rd = container_ದ(rcu, ರಚನೆ ಬೇರು_domain, rcu);

	cpupri_cleanup(&rd->cpupri);
	cpudl_cleanup(&rd->cpudl);
	free_cpumask_var(rd->dlo_ಮುಖವಾಡ);
	free_cpumask_var(rd->rto_ಮುಖವಾಡ);
	free_cpumask_var(rd->online);
	free_cpumask_var(rd->ಹರವು);
	free_pd(rd->pd);
	kfree(rd);
}

ಶೂನ್ಯ rq_attach_ಬೇರು(ರಚನೆ rq *rq, ರಚನೆ ಬೇರು_domain *rd)
{
	ರಚನೆ ಬೇರು_domain *old_rd = ಖಾಲಿ;
	ರಚನೆ rq_flags rf;

	rq_ಬೀಗ_irqsave(rq, &rf);

	ಆದರೆ (rq->rd) {
		old_rd = rq->rd;

		ಆದರೆ (cpumask_test_cpu(rq->cpu, old_rd->online))
			ಗಣ_rq_offline(rq);

		cpumask_clear_cpu(rq->cpu, old_rd->ಹರವು);

		/*
		 * If we don't want to free the old_rd yet then
		 * set old_rd to NULL to skip the freeing later
		 * in this function:
		 */
		ಆದರೆ (!atomic_dec_and_test(&old_rd->refcount))
			old_rd = ಖಾಲಿ;
	}

	atomic_inc(&rd->refcount);
	rq->rd = rd;

	cpumask_ಗಣ_cpu(rq->cpu, rd->ಹರವು);
	ಆದರೆ (cpumask_test_cpu(rq->cpu, cpu_ಸಕ್ರಿಯ_ಮುಖವಾಡ))
		ಗಣ_rq_online(rq);

	/*
	 * Because the rq is not a task, dl_add_task_root_domain() did not
	 * move the fair server bw to the rd if it already started.
	 * Add it now.
	 */
	ಆದರೆ (rq->fair_server.dl_server)
		__dl_server_attach_root(&rq->fair_server, rq);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CLASS_EXT
	ಆದರೆ (rq->ext_server.dl_server)
		__dl_server_attach_root(&rq->ext_server, rq);
#ಆದರೆ_ಅಂತ್ಯ

	rq_unlock_irqrestore(rq, &rf);

	ಆದರೆ (old_rd)
		call_rcu(&old_rd->rcu, free_rootdomain);
}

ಶೂನ್ಯ sched_get_rd(ರಚನೆ ಬೇರು_domain *rd)
{
	atomic_inc(&rd->refcount);
}

ಶೂನ್ಯ sched_put_rd(ರಚನೆ ಬೇರು_domain *rd)
{
	ಆದರೆ (!atomic_dec_and_test(&rd->refcount))
		ಹಿಂತಿರುಗಿ;

	call_rcu(&rd->rcu, free_rootdomain);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ init_rootdomain(ರಚನೆ ಬೇರು_domain *rd)
{
	ಆದರೆ (!zalloc_cpumask_var(&rd->ಹರವು, GFP_KERNEL))
		ಹೋಗು ಹೊರಹೋಗು;
	ಆದರೆ (!zalloc_cpumask_var(&rd->online, GFP_KERNEL))
		ಹೋಗು free_ಹರವು;
	ಆದರೆ (!zalloc_cpumask_var(&rd->dlo_ಮುಖವಾಡ, GFP_KERNEL))
		ಹೋಗು free_online;
	ಆದರೆ (!zalloc_cpumask_var(&rd->rto_ಮುಖವಾಡ, GFP_KERNEL))
		ಹೋಗು free_dlo_ಮುಖವಾಡ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_RT_ತಳ್ಳು_IPI
	rd->rto_cpu = -1;
	raw_spin_ಬೀಗ_init(&rd->rto_ಬೀಗ);
	rd->rto_ತಳ್ಳು_work = IRQ_WORK_INIT_HARD(rto_ತಳ್ಳು_irq_work_func);
#ಆದರೆ_ಅಂತ್ಯ

	rd->ಭೇಟಿ_cookie = 0;
	init_dl_bw(&rd->dl_bw);
	ಆದರೆ (cpudl_init(&rd->cpudl) != 0)
		ಹೋಗು free_rto_ಮುಖವಾಡ;

	ಆದರೆ (cpupri_init(&rd->cpupri) != 0)
		ಹೋಗು free_cpudl;
	ಹಿಂತಿರುಗಿ 0;

free_cpudl:
	cpudl_cleanup(&rd->cpudl);
free_rto_ಮುಖವಾಡ:
	free_cpumask_var(rd->rto_ಮುಖವಾಡ);
free_dlo_ಮುಖವಾಡ:
	free_cpumask_var(rd->dlo_ಮುಖವಾಡ);
free_online:
	free_cpumask_var(rd->online);
free_ಹರವು:
	free_cpumask_var(rd->ಹರವು);
ಹೊರಹೋಗು:
	ಹಿಂತಿರುಗಿ -ENOMEM;
}

/*
 * By default the system creates a single root-domain with all CPUs as
 * members (mimicking the global state we have today).
 */
ರಚನೆ ಬೇರು_domain def_ಬೇರು_domain;

ಶೂನ್ಯ __init init_defrootdomain(ಶೂನ್ಯ)
{
	init_rootdomain(&def_ಬೇರು_domain);

	atomic_ಗಣ(&def_ಬೇರು_domain.refcount, 1);
}

ಸ್ಥಿರ ರಚನೆ ಬೇರು_domain *alloc_rootdomain(ಶೂನ್ಯ)
{
	ರಚನೆ ಬೇರು_domain *rd;

	rd = kzalloc_obj(*rd);
	ಆದರೆ (!rd)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಆದರೆ (init_rootdomain(rd) != 0) {
		kfree(rd);
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}

	ಹಿಂತಿರುಗಿ rd;
}

ಸ್ಥಿರ ಶೂನ್ಯ free_sched_groups(ರಚನೆ sched_ಗುಂಪು *sg, ಪೂರ್ಣಾಂಕ free_sgc)
{
	ರಚನೆ sched_ಗುಂಪು *tmp, *ಮೊದಲ;

	ಆದರೆ (!sg)
		ಹಿಂತಿರುಗಿ;

	ಮೊದಲ = sg;
	ಮಾಡು {
		tmp = sg->ಮುಂದಿನ;

		ಆದರೆ (free_sgc && atomic_dec_and_test(&sg->sgc->ref))
			kfree(sg->sgc);

		ಆದರೆ (atomic_dec_and_test(&sg->ref))
			kfree(sg);
		sg = tmp;
	} ಆಗ (sg != ಮೊದಲ);
}

ಸ್ಥಿರ ಶೂನ್ಯ free_sched_domain_shared(ರಚನೆ sched_domain_shared *sds)
{
	ಆದರೆ (sds && atomic_dec_and_test(&sds->ref))
		kfree(sds);
}

ಸ್ಥಿರ ಶೂನ್ಯ destroy_sched_domain(ರಚನೆ sched_domain *sd)
{
	/*
	 * A normal sched domain may have multiple group references, an
	 * overlapping domain, having private groups, only one.  Iterate,
	 * dropping group/capacity references, freeing where none remain.
	 */
	free_sched_groups(sd->groups, 1);
	free_sched_domain_shared(sd->shared);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CACHE
	/* only the bottom sd has llc_counts array */
	kfree(sd->llc_counts);
#ಆದರೆ_ಅಂತ್ಯ
	kfree(sd);
}

ಸ್ಥಿರ ಶೂನ್ಯ destroy_sched_domains_rcu(ರಚನೆ rcu_ತಲೆ *rcu)
{
	ರಚನೆ sched_domain *sd = container_ದ(rcu, ರಚನೆ sched_domain, rcu);

	ಆಗ (sd) {
		ರಚನೆ sched_domain *ಪೋಷಕ = sd->ಪೋಷಕ;
		destroy_sched_domain(sd);
		sd = ಪೋಷಕ;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ destroy_sched_domains(ರಚನೆ sched_domain *sd)
{
	ಆದರೆ (sd)
		call_rcu(&sd->rcu, destroy_sched_domains_rcu);
}

/*
 * Keep a special pointer to the highest sched_domain that has SD_SHARE_LLC set
 * (Last Level Cache Domain) for this allows us to avoid some pointer chasing
 * select_idle_sibling().
 *
 * Also keep a unique ID per domain (we use the first CPU number in the cpumask
 * of the domain), this allows us to quickly tell if two CPUs are in the same
 * cache domain, see cpus_share_cache().
 */
DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ sched_domain __rcu *, sd_llc);
DEFINE_ಪ್ರತಿ_CPU(ಪೂರ್ಣಾಂಕ, sd_llc_ಗಾತ್ರ);
DEFINE_ಪ್ರತಿ_CPU(ಪೂರ್ಣಾಂಕ, sd_llc_id) = -1;
DEFINE_ಪ್ರತಿ_CPU(ಪೂರ್ಣಾಂಕ, sd_share_id);
DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ sched_domain_shared __rcu *, sd_llc_shared);
DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ sched_domain_shared __rcu *, sd_balance_shared);
DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ sched_domain __rcu *, sd_numa);
DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ sched_domain __rcu *, sd_asym_packing);
DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ sched_domain __rcu *, sd_asym_cpucapacity);

DEFINE_STATIC_ಕೀಲಿ_FALSE(sched_asym_cpucapacity);
DEFINE_STATIC_ಕೀಲಿ_FALSE(sched_cluster_ಸಕ್ರಿಯ);

ಸ್ಥಿರ ಶೂನ್ಯ update_ತುದಿ_cache_domain(ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ sched_domain_shared *sds = ಖಾಲಿ;
	ರಚನೆ sched_domain *sd;
	ಪೂರ್ಣಾಂಕ id = cpu;
	ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2 = 1;

	sd = highest_flag_domain(cpu, SD_SHARE_LLC);
	ಆದರೆ (sd) {
		id = cpumask_ಮೊದಲ(sched_domain_ಹರವು(sd));
		ಗಾತ್ರ_2 = cpumask_ತೂಕ(sched_domain_ಹರವು(sd));

		/* If sd_llc exists, sd_llc_shared should exist too. */
		WARN_ON_ONCE(!sd->shared);
		sds = sd->shared;
	}

	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(ಪ್ರತಿ_cpu(sd_llc, cpu), sd);
	ಪ್ರತಿ_cpu(sd_llc_ಗಾತ್ರ, cpu) = ಗಾತ್ರ_2;
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(ಪ್ರತಿ_cpu(sd_llc_shared, cpu), sds);

	sd = ಅತಿಕಡಿಮೆ_flag_domain(cpu, SD_CLUSTER);
	ಆದರೆ (sd)
		id = cpumask_ಮೊದಲ(sched_domain_ಹರವು(sd));

	/*
	 * This assignment should be placed after the sd_llc_id as
	 * we want this id equals to cluster id on cluster machines
	 * but equals to LLC id on non-Cluster machines.
	 */
	ಪ್ರತಿ_cpu(sd_share_id, cpu) = id;

	sd = ಅತಿಕಡಿಮೆ_flag_domain(cpu, SD_NUMA);
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(ಪ್ರತಿ_cpu(sd_numa, cpu), sd);

	sd = highest_flag_domain(cpu, SD_ASYM_PACKING);
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(ಪ್ರತಿ_cpu(sd_asym_packing, cpu), sd);

	sd = ಅತಿಕಡಿಮೆ_flag_domain(cpu, SD_ASYM_CPUCAPACITY_ತುಂಬಿದ);
	/*
	 * The shared object is attached to sd_asym_cpucapacity only when the
	 * asym domain is non-overlapping (i.e., not built from SD_NUMA).
	 * On overlapping (NUMA) asym domains we fall back to letting the
	 * SD_SHARE_LLC path own the shared object, so sd->shared may be NULL
	 * here.
	 */
	ಆದರೆ (sd && sd->shared)
		sds = sd->shared;

	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(ಪ್ರತಿ_cpu(sd_asym_cpucapacity, cpu), sd);
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(ಪ್ರತಿ_cpu(sd_balance_shared, cpu), sds);
}

/*
 * Attach the domain 'sd' to 'cpu' as its base domain. Callers must
 * hold the hotplug lock.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
cpu_attach_domain(ರಚನೆ sched_domain *sd, ರಚನೆ ಬೇರು_domain *rd, ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);
	ರಚನೆ sched_domain *tmp;

	/* Remove the sched domains which do not contribute to scheduling. */
	ಗಾಗಿ (tmp = sd; tmp; ) {
		ರಚನೆ sched_domain *ಪೋಷಕ = tmp->ಪೋಷಕ;
		ಆದರೆ (!ಪೋಷಕ)
			ನಿಲ್ಲಿಸು;

		ಆದರೆ (sd_ಪೋಷಕ_degenerate(tmp, ಪೋಷಕ)) {
			tmp->ಪೋಷಕ = ಪೋಷಕ->ಪೋಷಕ;

			/* Pick reference to parent->shared. */
			ಆದರೆ (ಪೋಷಕ->shared) {
				/*
				 * It is safe to free a sd->shared that
				 * has not been published yet. If a
				 * sd->shared was published, the refcount
				 * will end up being non-zero and it will
				 * not be freed here.
				 */
				free_sched_domain_shared(tmp->shared);
				tmp->shared = ಪೋಷಕ->shared;
				ಪೋಷಕ->shared = ಖಾಲಿ;
			}

			ಆದರೆ (ಪೋಷಕ->ಪೋಷಕ) {
				ಪೋಷಕ->ಪೋಷಕ->ಮಗು = tmp;
				ಪೋಷಕ->ಪೋಷಕ->groups->flags = tmp->flags;
			}

			/*
			 * Transfer SD_PREFER_SIBLING down in case of a
			 * degenerate parent; the spans match for this
			 * so the property transfers.
			 */
			ಆದರೆ (ಪೋಷಕ->flags & SD_PREFER_SIBLING)
				tmp->flags |= SD_PREFER_SIBLING;
			destroy_sched_domain(ಪೋಷಕ);
		} ಇಲ್ಲದಿದ್ದರೆ
			tmp = tmp->ಪೋಷಕ;
	}

	ಆದರೆ (sd && sd_degenerate(sd)) {
		tmp = sd;
		sd = sd->ಪೋಷಕ;

		ಆದರೆ (sd) {
			ರಚನೆ sched_ಗುಂಪು *sg = sd->groups;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CACHE
			/* move buffer to parent as child is being destroyed */
			sd->llc_counts = tmp->llc_counts;
			sd->llc_max = tmp->llc_max;
			sd->llc_ಬೈಟುಗಳು = tmp->llc_ಬೈಟುಗಳು;
			/* make sure destroy_sched_domain() does not free it */
			tmp->llc_counts = ಖಾಲಿ;
			tmp->llc_max = 0;
			tmp->llc_ಬೈಟುಗಳು = 0;
#ಆದರೆ_ಅಂತ್ಯ
			/*
			 * sched groups hold the flags of the child sched
			 * domain for convenience. Clear such flags since
			 * the child is being destroyed.
			 */
			ಮಾಡು {
				sg->flags = 0;
			} ಆಗ (sg != sd->groups);

			sd->ಮಗು = ಖಾಲಿ;
		}

		destroy_sched_domain(tmp);
	}

	sched_domain_debug(sd, cpu);

	rq_attach_ಬೇರು(rq, rd);
	tmp = rq->sd;
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(rq->sd, sd);
	dirty_sched_domain_sysctl(cpu);
	destroy_sched_domains(tmp);

	update_ತುದಿ_cache_domain(cpu);
}

ರಚನೆ s_ದತ್ತಾಂಶ {
	ರಚನೆ sched_domain_shared * __percpu *sds;
	ರಚನೆ sched_domain * __percpu *sd;
	ರಚನೆ ಬೇರು_domain	*rd;
};

ಎಣಿಕೆ s_alloc {
	sa_rootdomain,
	sa_sd,
	sa_sd_shared,
	sa_sd_storage,
	sa_none,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CACHE
/* hardware support for cache aware scheduling */
DEFINE_STATIC_ಕೀಲಿ_FALSE(sched_cache_present);
/*
 * Indicator of whether cache aware scheduling
 * is active, used by the scheduler.
 */
DEFINE_STATIC_ಕೀಲಿ_FALSE(sched_cache_ಸಕ್ರಿಯ);
/* user wants cache aware scheduling [0 or 1] */
ಪೂರ್ಣಾಂಕ sysctl_sched_cache_user = 1;

/*
 * Get the effective LLC size in bytes that @cpu's bottom sched_domain
 * can use. A CPU within a cpuset partition can only use a proportion
 * of the physical LLC, scaled by the ratio of the partition's span
 * weight to the hardware LLC sharing weight. @sd should be the
 * topmost domain with SD_SHARE_LLC.
 *
 * Returns 0 if cacheinfo is not yet populated. This happens during
 * early boot when build_sched_domains() runs before the generic
 * cacheinfo framework has been initialized (cacheinfo_cpu_online()
 * is a device_initcall cpuhp callback). In that case,
 * cacheinfo_cpu_online() will later call sched_update_llc_bytes()
 * to fill in the bottom domain's llc_bytes once the cache attributes
 * are available.
 */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ get_effective_llc_ಬೈಟುಗಳು(ಪೂರ್ಣಾಂಕ cpu,
					     ರಚನೆ sched_domain *sd)
{
	ರಚನೆ cacheinfo *ci;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ hw_ತೂಕ;

	ci = get_cpu_cacheinfo_llc(cpu);
	ಆದರೆ (!ci)
		ಹಿಂತಿರುಗಿ 0;

	hw_ತೂಕ = cpumask_ತೂಕ(&ci->shared_cpu_map);
	ಆದರೆ (!hw_ತೂಕ)
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ div_u64((u64)ci->ಗಾತ್ರ_2 * sd->ಹರವು_ತೂಕ, hw_ತೂಕ);
}

ಸ್ಥಿರ bool alloc_sd_llc(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_map,
			 ರಚನೆ s_ದತ್ತಾಂಶ *d)
{
	ರಚನೆ sched_domain *sd, *ತುದಿ_llc, *ಪೋಷಕ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *p;
	ಪೂರ್ಣಾಂಕ i;

	for_each_cpu(i, cpu_map) {
		sd = *ಪ್ರತಿ_cpu_ptr(d->sd, i);
		ಆದರೆ (!sd)
			ಹೋಗು err;

		p = kcalloc_ಗಂಟು(max_lid + 1, ಗಾತ್ರ(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ),
				 GFP_KERNEL, cpu_ಗಮ್ಯ_ಗಂಟು(i));
		ಆದರೆ (!p)
			ಹೋಗು err;

		ತುದಿ_llc = sd;
		/*
		 * Find the topmost SD_SHARE_LLC domain.
		 * Not yet attached to the CPU, so per_cpu(sd_llc, i)
		 * can not be used.
		 */
		ಆಗ ((ಪೋಷಕ = rcu_dereference_protected(ತುದಿ_llc->ಪೋಷಕ, true)) &&
		       (ಪೋಷಕ->flags & SD_SHARE_LLC))
			ತುದಿ_llc = ಪೋಷಕ;

		ಆದರೆ (ತುದಿ_llc->flags & SD_SHARE_LLC) {
			sd->llc_max = max_lid + 1;
			sd->llc_counts = p;
			sd->llc_ಬೈಟುಗಳು = get_effective_llc_ಬೈಟುಗಳು(i, ತುದಿ_llc);
		} ಇಲ್ಲದಿದ್ದರೆ {
			/* avoid memory leak */
			kfree(p);
		}
	}

	ಹಿಂತಿರುಗಿ true;
err:
	for_each_cpu(i, cpu_map) {
		sd = *ಪ್ರತಿ_cpu_ptr(d->sd, i);
		ಆದರೆ (sd) {
			kfree(sd->llc_counts);
			sd->llc_counts = ಖಾಲಿ;
			sd->llc_max = 0;
			sd->llc_ಬೈಟುಗಳು = 0;
		}
	}

	ಹಿಂತಿರುಗಿ false;
}

/*
 * Enable/disable cache aware scheduling according to
 * user input and the presence of hardware support.
 */
ಸ್ಥಿರ ಶೂನ್ಯ sched_cache_ಸಕ್ರಿಯ_ಗಣ_2(ಶೂನ್ಯ)
{
	lockdep_assert_cpus_held();
	lockdep_assert_held(&sched_domains_mutex);

	/* hardware does not support */
	ಆದರೆ (!static_branch_likely(&sched_cache_present)) {
		static_branch_disable_cpuslocked(&sched_cache_ಸಕ್ರಿಯ);
		ಆದರೆ (sched_debug())
			pr_info("%s: cache aware scheduling not supported on this platform\n", __func__);
		ಹಿಂತಿರುಗಿ;
	}

	/*
	 * user wants it or not ?
	 * TBD: read before writing the static key.
	 * It is not in the critical path, leave as-is
	 * for now.
	 */
	ಆದರೆ (sysctl_sched_cache_user) {
		static_branch_enable_cpuslocked(&sched_cache_ಸಕ್ರಿಯ);
		ಆದರೆ (sched_debug())
			pr_info("%s: enabling cache aware scheduling\n", __func__);
	} ಇಲ್ಲದಿದ್ದರೆ {
		static_branch_disable_cpuslocked(&sched_cache_ಸಕ್ರಿಯ);
		ಆದರೆ (sched_debug())
			pr_info("%s: disabling cache aware scheduling\n", __func__);
	}
}

/* used by debugfs */
ಶೂನ್ಯ sched_cache_ಸಕ್ರಿಯ_ಗಣ(ಶೂನ್ಯ)
{
	cpus_read_ಬೀಗ();
	sched_domains_mutex_ಬೀಗ();
	sched_cache_ಸಕ್ರಿಯ_ಗಣ_2();
	sched_domains_mutex_unlock();
	cpus_read_unlock();
}

/*
 * Update the bottom sched_domain's llc_bytes for @cpus sharing a physical
 * LLC. Called from cacheinfo_cpu_online() or
 * cacheinfo_cpu_pre_down() with cpu hotplug lock held.
 *
 * Note: get_effective_llc_bytes() returns 0 on PowerPC.
 * thus cache aware scheduling is disabled on PowerPC for
 * now. PowerPC does not use the generic cacheinfo framework --
 * it has its own cacheinfo with a separate struct cache hierarchy
 * and does not populates the per-CPU struct cpu_cacheinfo array
 * that get_cpu_cacheinfo_llc() reads.
 */
ಶೂನ್ಯ sched_update_llc_ಬೈಟುಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpus)
{
	ರಚನೆ sched_domain *sd, *sdp;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i;

	sched_domains_mutex_ಬೀಗ();

	/*
	 * ci->shared_cpu_map is built incrementally as CPUs come
	 * online, so the first CPU in an LLC initially sees
	 * hw_weight == 1 and computes an inflated llc_bytes in
	 * get_effective_llc_bytes().  Re-evaluating every LLC
	 * sibling on each online event corrects this once the full
	 * shared_cpu_map is known.
	 *
	 * The departing CPU's domains have already been detached when
	 * cacheinfo removes it. Use the surviving cache siblings instead.
	 * They may belong to different cpuset partitions, so use each CPU's
	 * own LLC domain to scale its share of the physical cache.
	 */
	for_each_cpu(i, cpus) {
		sdp = rcu_dereference_sched_domain(ಪ್ರತಿ_cpu(sd_llc, i));
		ಆದರೆ (!sdp)
			ಮುಂದುವರಿ;

		sd = rcu_dereference_sched_domain(cpu_rq(i)->sd);
		ಆದರೆ (sd)
			sd->llc_ಬೈಟುಗಳು = get_effective_llc_ಬೈಟುಗಳು(i, sdp);
	}

	sched_domains_mutex_unlock();
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_cache_ಗಣ(bool has_multi_llcs)
{
	/*
	 * TBD: check before writing to it. sched domain rebuild
	 * is not in the critical path, leave as-is for now.
	 */
	ಆದರೆ (has_multi_llcs)
		static_branch_enable_cpuslocked(&sched_cache_present);
	ಇಲ್ಲದಿದ್ದರೆ
		static_branch_disable_cpuslocked(&sched_cache_present);

	sched_cache_ಸಕ್ರಿಯ_ಗಣ_2();
}
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ bool alloc_sd_llc(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_map,
			 ರಚನೆ s_ದತ್ತಾಂಶ *d)
{
	ಹಿಂತಿರುಗಿ false;
}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_cache_ಗಣ(bool has_multi_llcs) { }
#ಆದರೆ_ಅಂತ್ಯ

/*
 * Return true if @sd belongs to an LLC group whose enclosing
 * partition spans more than one LLC. @sd must be the topmost
 * SD_SHARE_LLC domain.
 *
 * Any duplicated parent domains with the same span as @sd are
 * skipped: before cpu_attach_domain() degeneration these still
 * exist, after degeneration the loop is a no-op. This makes the
 * helper usable both during sched domain build and against an
 * already-attached domain tree.
 *
 * Note: For systems with a single LLC per node, cache-aware
 * scheduling is still enabled when multiple nodes exist.
 * However, NUMA balancing decisions take precedence over
 * cache-aware scheduling. Conversely, if there is only one
 * LLC per partition, cache-aware scheduling should be disabled.
 */
ಸ್ಥಿರ bool sd_ಒಳ_multi_llcs(ರಚನೆ sched_domain *sd)
{
	ರಚನೆ sched_domain *sdp = sd->ಪೋಷಕ;

	/* it does not make sense to aggregate to 1 CPU */
	ಆದರೆ (sd->ಹರವು_ತೂಕ == 1)
		ಹಿಂತಿರುಗಿ false;

	ಆಗ (sdp && sdp->ಹರವು_ತೂಕ == sd->ಹರವು_ತೂಕ)
		sdp = sdp->ಪೋಷಕ;

	ಹಿಂತಿರುಗಿ !!sdp;
}

/*
 * Return the canonical balance CPU for this group, this is the first CPU
 * of this group that's also in the balance mask.
 *
 * The balance mask are all those CPUs that could actually end up at this
 * group. See build_balance_mask().
 *
 * Also see should_we_balance().
 */
ಪೂರ್ಣಾಂಕ ಗುಂಪು_balance_cpu(ರಚನೆ sched_ಗುಂಪು *sg)
{
	ಹಿಂತಿರುಗಿ cpumask_ಮೊದಲ(ಗುಂಪು_balance_ಮುಖವಾಡ(sg));
}


/*
 * NUMA topology (first read the regular topology blurb below)
 *
 * Given a node-distance table, for example:
 *
 *   node   0   1   2   3
 *     0:  10  20  30  20
 *     1:  20  10  20  30
 *     2:  30  20  10  20
 *     3:  20  30  20  10
 *
 * which represents a 4 node ring topology like:
 *
 *   0 ----- 1
 *   |       |
 *   |       |
 *   |       |
 *   3 ----- 2
 *
 * We want to construct domains and groups to represent this. The way we go
 * about doing this is to build the domains on 'hops'. For each NUMA level we
 * construct the mask of all nodes reachable in @level hops.
 *
 * For the above NUMA topology that gives 3 levels:
 *
 * NUMA-2	0-3		0-3		0-3		0-3
 *  groups:	{0-1,3},{1-3}	{0-2},{0,2-3}	{1-3},{0-1,3}	{0,2-3},{0-2}
 *
 * NUMA-1	0-1,3		0-2		1-3		0,2-3
 *  groups:	{0},{1},{3}	{0},{1},{2}	{1},{2},{3}	{0},{2},{3}
 *
 * NUMA-0	0		1		2		3
 *
 *
 * As can be seen; things don't nicely line up as with the regular topology.
 * When we iterate a domain in child domain chunks some nodes can be
 * represented multiple times -- hence the "overlap" naming for this part of
 * the topology.
 *
 * In order to minimize this overlap, we only build enough groups to cover the
 * domain. For instance Node-0 NUMA-2 would only get groups: 0-1,3 and 1-3.
 *
 * Because:
 *
 *  - the first group of each domain is its child domain; this
 *    gets us the first 0-1,3
 *  - the only uncovered node is 2, who's child domain is 1-3.
 *
 * However, because of the overlap, computing a unique CPU for each group is
 * more complicated. Consider for instance the groups of NODE-1 NUMA-2, both
 * groups include the CPUs of Node-0, while those CPUs would not in fact ever
 * end up at those groups (they would end up in group: 0-1,3).
 *
 * To correct this we have to introduce the group balance mask. This mask
 * will contain those CPUs in the group that can reach this group given the
 * (child) domain tree.
 *
 * With this we can once again compute balance_cpu and sched_group_capacity
 * relations.
 *
 * XXX include words on how balance_cpu is unique and therefore can be
 * used for sched_group_capacity links.
 *
 *
 * Another 'interesting' topology is:
 *
 *   node   0   1   2   3
 *     0:  10  20  20  30
 *     1:  20  10  20  20
 *     2:  20  20  10  20
 *     3:  30  20  20  10
 *
 * Which looks a little like:
 *
 *   0 ----- 1
 *   |     / |
 *   |   /   |
 *   | /     |
 *   2 ----- 3
 *
 * This topology is asymmetric, nodes 1,2 are fully connected, but nodes 0,3
 * are not.
 *
 * This leads to a few particularly weird cases where the sched_domain's are
 * not of the same number for each CPU. Consider:
 *
 * NUMA-2	0-3						0-3
 *  groups:	{0-2},{1-3}					{1-3},{0-2}
 *
 * NUMA-1	0-2		0-3		0-3		1-3
 *
 * NUMA-0	0		1		2		3
 *
 */


/*
 * Build the balance mask; it contains only those CPUs that can arrive at this
 * group and should be considered to continue balancing.
 *
 * We do this during the group creation pass, therefore the group information
 * isn't complete yet, however since each group represents a (child) domain we
 * can fully construct this using the sched_domain bits (which are already
 * complete).
 */
ಸ್ಥಿರ ಶೂನ್ಯ
build_balance_ಮುಖವಾಡ(ರಚನೆ sched_domain *sd, ರಚನೆ sched_ಗುಂಪು *sg, ರಚನೆ cpumask *ಮುಖವಾಡ)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *sg_ಹರವು = sched_ಗುಂಪು_ಹರವು(sg);
	ರಚನೆ sd_ದತ್ತಾಂಶ *sdd = sd->ಖಾಸಗಿ;
	ರಚನೆ sched_domain *sibling;
	ಪೂರ್ಣಾಂಕ i;

	cpumask_clear(ಮುಖವಾಡ);

	for_each_cpu(i, sg_ಹರವು) {
		sibling = *ಪ್ರತಿ_cpu_ptr(sdd->sd, i);

		/*
		 * Can happen in the asymmetric case, where these siblings are
		 * unused. The mask will not be empty because those CPUs that
		 * do have the top domain _should_ span the domain.
		 */
		ಆದರೆ (!sibling->ಮಗು)
			ಮುಂದುವರಿ;

		/* If we would not end up here, we can't continue from here */
		ಆದರೆ (!cpumask_equal(sg_ಹರವು, sched_domain_ಹರವು(sibling->ಮಗು)))
			ಮುಂದುವರಿ;

		cpumask_ಗಣ_cpu(i, ಮುಖವಾಡ);
	}

	/* We must not have empty masks here */
	WARN_ON_ONCE(cpumask_ಖಾಲಿಗಳು(ಮುಖವಾಡ));
}

/*
 * XXX: This creates per-node group entries; since the load-balancer will
 * immediately access remote memory to construct this group's load-balance
 * statistics having the groups node local is of dubious benefit.
 */
ಸ್ಥಿರ ರಚನೆ sched_ಗುಂಪು *
build_ಗುಂಪು_from_ಮಗು_sched_domain(ರಚನೆ sched_domain *sd, ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ sched_ಗುಂಪು *sg;
	ರಚನೆ cpumask *sg_ಹರವು;

	sg = kzalloc_ಗಂಟು(ಗಾತ್ರ(ರಚನೆ sched_ಗುಂಪು) + cpumask_ಗಾತ್ರ(),
			GFP_KERNEL, cpu_ಗಮ್ಯ_ಗಂಟು(cpu));

	ಆದರೆ (!sg)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	sg_ಹರವು = sched_ಗುಂಪು_ಹರವು(sg);
	ಆದರೆ (sd->ಮಗು) {
		cpumask_copy(sg_ಹರವು, sched_domain_ಹರವು(sd->ಮಗು));
		sg->flags = sd->ಮಗು->flags;
	} ಇಲ್ಲದಿದ್ದರೆ {
		cpumask_copy(sg_ಹರವು, sched_domain_ಹರವು(sd));
	}

	atomic_inc(&sg->ref);
	ಹಿಂತಿರುಗಿ sg;
}

ಸ್ಥಿರ ಶೂನ್ಯ init_overlap_sched_ಗುಂಪು(ರಚನೆ sched_domain *sd,
				     ರಚನೆ sched_ಗುಂಪು *sg)
{
	ರಚನೆ cpumask *ಮುಖವಾಡ = sched_domains_tmpmask2;
	ರಚನೆ sd_ದತ್ತಾಂಶ *sdd = sd->ಖಾಸಗಿ;
	ರಚನೆ cpumask *sg_ಹರವು;
	ಪೂರ್ಣಾಂಕ cpu;

	build_balance_ಮುಖವಾಡ(sd, sg, ಮುಖವಾಡ);
	cpu = cpumask_ಮೊದಲ(ಮುಖವಾಡ);

	sg->sgc = *ಪ್ರತಿ_cpu_ptr(sdd->sgc, cpu);
	ಆದರೆ (atomic_inc_return(&sg->sgc->ref) == 1)
		cpumask_copy(ಗುಂಪು_balance_ಮುಖವಾಡ(sg), ಮುಖವಾಡ);
	ಇಲ್ಲದಿದ್ದರೆ
		WARN_ON_ONCE(!cpumask_equal(ಗುಂಪು_balance_ಮುಖವಾಡ(sg), ಮುಖವಾಡ));

	/*
	 * Initialize sgc->capacity such that even if we mess up the
	 * domains and no possible iteration will get us here, we won't
	 * die on a /0 trap.
	 */
	sg_ಹರವು = sched_ಗುಂಪು_ಹರವು(sg);
	sg->sgc->ಸಾಮರ್ಥ್ಯ = SCHED_ಸಾಮರ್ಥ್ಯ_SCALE * cpumask_ತೂಕ(sg_ಹರವು);
	sg->sgc->min_ಸಾಮರ್ಥ್ಯ = SCHED_ಸಾಮರ್ಥ್ಯ_SCALE;
	sg->sgc->max_ಸಾಮರ್ಥ್ಯ = SCHED_ಸಾಮರ್ಥ್ಯ_SCALE;
}

ಸ್ಥಿರ ರಚನೆ sched_domain *
ಕಂಡುಹಿಡಿ_descended_sibling(ರಚನೆ sched_domain *sd, ರಚನೆ sched_domain *sibling)
{
	/*
	 * The proper descendant would be the one whose child won't span out
	 * of sd
	 */
	ಆಗ (sibling->ಮಗು &&
	       !cpumask_subset(sched_domain_ಹರವು(sibling->ಮಗು),
			       sched_domain_ಹರವು(sd)))
		sibling = sibling->ಮಗು;

	/*
	 * As we are referencing sgc across different topology level, we need
	 * to go down to skip those sched_domains which don't contribute to
	 * scheduling because they will be degenerated in cpu_attach_domain
	 */
	ಆಗ (sibling->ಮಗು &&
	       cpumask_equal(sched_domain_ಹರವು(sibling->ಮಗು),
			     sched_domain_ಹರವು(sibling)))
		sibling = sibling->ಮಗು;

	ಹಿಂತಿರುಗಿ sibling;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
build_overlap_sched_groups(ರಚನೆ sched_domain *sd, ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ sched_ಗುಂಪು *ಮೊದಲ = ಖಾಲಿ, *ಕೊನೆಯ = ಖಾಲಿ, *sg;
	ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *ಹರವು = sched_domain_ಹರವು(sd);
	ರಚನೆ cpumask *covered = sched_domains_tmpmask;
	ರಚನೆ sd_ದತ್ತಾಂಶ *sdd = sd->ಖಾಸಗಿ;
	ರಚನೆ sched_domain *sibling;
	ಪೂರ್ಣಾಂಕ i;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, ಹರವು, cpu) {
		ರಚನೆ cpumask *sg_ಹರವು;

		ಆದರೆ (cpumask_test_cpu(i, covered))
			ಮುಂದುವರಿ;

		sibling = *ಪ್ರತಿ_cpu_ptr(sdd->sd, i);

		/*
		 * Asymmetric node setups can result in situations where the
		 * domain tree is of unequal depth, make sure to skip domains
		 * that already cover the entire range.
		 *
		 * In that case build_sched_domains() will have terminated the
		 * iteration early and our sibling sd spans will be empty.
		 * Domains should always include the CPU they're built on, so
		 * check that.
		 */
		ಆದರೆ (!cpumask_test_cpu(i, sched_domain_ಹರವು(sibling)))
			ಮುಂದುವರಿ;

		/*
		 * Usually we build sched_group by sibling's child sched_domain
		 * But for machines whose NUMA diameter are 3 or above, we move
		 * to build sched_group by sibling's proper descendant's child
		 * domain because sibling's child sched_domain will span out of
		 * the sched_domain being built as below.
		 *
		 * Smallest diameter=3 topology is:
		 *
		 *   node   0   1   2   3
		 *     0:  10  20  30  40
		 *     1:  20  10  20  30
		 *     2:  30  20  10  20
		 *     3:  40  30  20  10
		 *
		 *   0 --- 1 --- 2 --- 3
		 *
		 * NUMA-3       0-3             N/A             N/A             0-3
		 *  groups:     {0-2},{1-3}                                     {1-3},{0-2}
		 *
		 * NUMA-2       0-2             0-3             0-3             1-3
		 *  groups:     {0-1},{1-3}     {0-2},{2-3}     {1-3},{0-1}     {2-3},{0-2}
		 *
		 * NUMA-1       0-1             0-2             1-3             2-3
		 *  groups:     {0},{1}         {1},{2},{0}     {2},{3},{1}     {3},{2}
		 *
		 * NUMA-0       0               1               2               3
		 *
		 * The NUMA-2 groups for nodes 0 and 3 are obviously buggered, as the
		 * group span isn't a subset of the domain span.
		 */
		ಆದರೆ (sibling->ಮಗು &&
		    !cpumask_subset(sched_domain_ಹರವು(sibling->ಮಗು), ಹರವು))
			sibling = ಕಂಡುಹಿಡಿ_descended_sibling(sd, sibling);

		sg = build_ಗುಂಪು_from_ಮಗು_sched_domain(sibling, cpu);
		ಆದರೆ (!sg)
			ಹೋಗು fail;

		sg_ಹರವು = sched_ಗುಂಪು_ಹರವು(sg);
		cpumask_or(covered, covered, sg_ಹರವು);

		init_overlap_sched_ಗುಂಪು(sibling, sg);

		ಆದರೆ (!ಮೊದಲ)
			ಮೊದಲ = sg;
		ಆದರೆ (ಕೊನೆಯ)
			ಕೊನೆಯ->ಮುಂದಿನ = sg;
		ಕೊನೆಯ = sg;
		ಕೊನೆಯ->ಮುಂದಿನ = ಮೊದಲ;
	}
	sd->groups = ಮೊದಲ;

	ಹಿಂತಿರುಗಿ 0;

fail:
	free_sched_groups(ಮೊದಲ, 0);

	ಹಿಂತಿರುಗಿ -ENOMEM;
}


/*
 * Package topology (also see the load-balance blurb in fair.c)
 *
 * The scheduler builds a tree structure to represent a number of important
 * topology features. By default (default_topology[]) these include:
 *
 *  - Simultaneous multithreading (SMT)
 *  - Multi-Core Cache (MC)
 *  - Package (PKG)
 *
 * Where the last one more or less denotes everything up to a NUMA node.
 *
 * The tree consists of 3 primary data structures:
 *
 *	sched_domain -> sched_group -> sched_group_capacity
 *	    ^ ^             ^ ^
 *          `-'             `-'
 *
 * The sched_domains are per-CPU and have a two way link (parent & child) and
 * denote the ever growing mask of CPUs belonging to that level of topology.
 *
 * Each sched_domain has a circular (double) linked list of sched_group's, each
 * denoting the domains of the level below (or individual CPUs in case of the
 * first domain level). The sched_group linked by a sched_domain includes the
 * CPU of that sched_domain [*].
 *
 * Take for instance a 2 threaded, 2 core, 2 cache cluster part:
 *
 * CPU   0   1   2   3   4   5   6   7
 *
 * PKG  [                             ]
 * MC   [             ] [             ]
 * SMT  [     ] [     ] [     ] [     ]
 *
 *  - or -
 *
 * PKG  0-7 0-7 0-7 0-7 0-7 0-7 0-7 0-7
 * MC	0-3 0-3 0-3 0-3 4-7 4-7 4-7 4-7
 * SMT  0-1 0-1 2-3 2-3 4-5 4-5 6-7 6-7
 *
 * CPU   0   1   2   3   4   5   6   7
 *
 * One way to think about it is: sched_domain moves you up and down among these
 * topology levels, while sched_group moves you sideways through it, at child
 * domain granularity.
 *
 * sched_group_capacity ensures each unique sched_group has shared storage.
 *
 * There are two related construction problems, both require a CPU that
 * uniquely identify each group (for a given domain):
 *
 *  - The first is the balance_cpu (see should_we_balance() and the
 *    load-balance blurb in fair.c); for each group we only want 1 CPU to
 *    continue balancing at a higher domain.
 *
 *  - The second is the sched_group_capacity; we want all identical groups
 *    to share a single sched_group_capacity.
 *
 * Since these topologies are exclusive by construction. That is, its
 * impossible for an SMT thread to belong to multiple cores, and cores to
 * be part of multiple caches. There is a very clear and unique location
 * for each CPU in the hierarchy.
 *
 * Therefore computing a unique CPU for each group is trivial (the iteration
 * mask is redundant and set all 1s; all CPUs in a group will end up at _that_
 * group), we can simply pick the first CPU in each group.
 *
 *
 * [*] in other words, the first group of each domain is its child domain.
 */

ಸ್ಥಿರ ರಚನೆ sched_ಗುಂಪು *get_ಗುಂಪು(ಪೂರ್ಣಾಂಕ cpu, ರಚನೆ sd_ದತ್ತಾಂಶ *sdd)
{
	ರಚನೆ sched_domain *sd = *ಪ್ರತಿ_cpu_ptr(sdd->sd, cpu);
	ರಚನೆ sched_domain *ಮಗು = sd->ಮಗು;
	ರಚನೆ sched_ಗುಂಪು *sg;
	bool already_ಭೇಟಿಯಾದ;

	ಆದರೆ (ಮಗು)
		cpu = cpumask_ಮೊದಲ(sched_domain_ಹರವು(ಮಗು));

	sg = *ಪ್ರತಿ_cpu_ptr(sdd->sg, cpu);
	sg->sgc = *ಪ್ರತಿ_cpu_ptr(sdd->sgc, cpu);

	/* Increase refcounts for claim_allocations: */
	already_ಭೇಟಿಯಾದ = atomic_inc_return(&sg->ref) > 1;
	/* sgc visits should follow a similar trend as sg */
	WARN_ON(already_ಭೇಟಿಯಾದ != (atomic_inc_return(&sg->sgc->ref) > 1));

	/* If we have already visited that group, it's already initialized. */
	ಆದರೆ (already_ಭೇಟಿಯಾದ)
		ಹಿಂತಿರುಗಿ sg;

	ಆದರೆ (ಮಗು) {
		cpumask_copy(sched_ಗುಂಪು_ಹರವು(sg), sched_domain_ಹರವು(ಮಗು));
		cpumask_copy(ಗುಂಪು_balance_ಮುಖವಾಡ(sg), sched_ಗುಂಪು_ಹರವು(sg));
		sg->flags = ಮಗು->flags;
	} ಇಲ್ಲದಿದ್ದರೆ {
		cpumask_ಗಣ_cpu(cpu, sched_ಗುಂಪು_ಹರವು(sg));
		cpumask_ಗಣ_cpu(cpu, ಗುಂಪು_balance_ಮುಖವಾಡ(sg));
	}

	sg->sgc->ಸಾಮರ್ಥ್ಯ = SCHED_ಸಾಮರ್ಥ್ಯ_SCALE * cpumask_ತೂಕ(sched_ಗುಂಪು_ಹರವು(sg));
	sg->sgc->min_ಸಾಮರ್ಥ್ಯ = SCHED_ಸಾಮರ್ಥ್ಯ_SCALE;
	sg->sgc->max_ಸಾಮರ್ಥ್ಯ = SCHED_ಸಾಮರ್ಥ್ಯ_SCALE;

	ಹಿಂತಿರುಗಿ sg;
}

/*
 * build_sched_groups will build a circular linked list of the groups
 * covered by the given span, will set each group's ->cpumask correctly,
 * and will initialize their ->sgc.
 *
 * Assumes the sched_domain tree is fully constructed
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
build_sched_groups(ರಚನೆ sched_domain *sd, ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ sched_ಗುಂಪು *ಮೊದಲ = ಖಾಲಿ, *ಕೊನೆಯ = ಖಾಲಿ;
	ರಚನೆ sd_ದತ್ತಾಂಶ *sdd = sd->ಖಾಸಗಿ;
	ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *ಹರವು = sched_domain_ಹರವು(sd);
	ರಚನೆ cpumask *covered;
	ಪೂರ್ಣಾಂಕ i;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, ಹರವು, cpu) {
		ರಚನೆ sched_ಗುಂಪು *sg;

		ಆದರೆ (cpumask_test_cpu(i, covered))
			ಮುಂದುವರಿ;

		sg = get_ಗುಂಪು(i, sdd);

		cpumask_or(covered, covered, sched_ಗುಂಪು_ಹರವು(sg));

		ಆದರೆ (!ಮೊದಲ)
			ಮೊದಲ = sg;
		ಆದರೆ (ಕೊನೆಯ)
			ಕೊನೆಯ->ಮುಂದಿನ = sg;
		ಕೊನೆಯ = sg;
	}
	ಕೊನೆಯ->ಮುಂದಿನ = ಮೊದಲ;
	sd->groups = ಮೊದಲ;

	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Initialize sched groups cpu_capacity.
 *
 * cpu_capacity indicates the capacity of sched group, which is used while
 * distributing the load between different sched groups in a sched domain.
 * Typically cpu_capacity for all the groups in a sched domain will be same
 * unless there are asymmetries in the topology. If there are asymmetries,
 * group having more cpu_capacity will pickup more load compared to the
 * group having less cpu_capacity.
 */
ಸ್ಥಿರ ಶೂನ್ಯ init_sched_groups_ಸಾಮರ್ಥ್ಯ(ಪೂರ್ಣಾಂಕ cpu, ರಚನೆ sched_domain *sd)
{
	ರಚನೆ sched_ಗುಂಪು *sg = sd->groups;
	ರಚನೆ cpumask *ಮುಖವಾಡ = sched_domains_tmpmask2;

	WARN_ON(!sg);

	ಮಾಡು {
		ಪೂರ್ಣಾಂಕ cpu, cores = 0, max_cpu = -1;

		sg->ಗುಂಪು_ತೂಕ = cpumask_ತೂಕ(sched_ಗುಂಪು_ಹರವು(sg));

		cpumask_copy(ಮುಖವಾಡ, sched_ಗುಂಪು_ಹರವು(sg));
		for_each_cpu(cpu, ಮುಖವಾಡ) {
			cores++;
			cpumask_andnot(ಮುಖವಾಡ, ಮುಖವಾಡ, cpu_smt_ಮುಖವಾಡ(cpu));
		}
		sg->cores = cores;

		ಆದರೆ (!(sd->flags & SD_ASYM_PACKING))
			ಹೋಗು ಮುಂದಿನ;

		for_each_cpu(cpu, sched_ಗುಂಪು_ಹರವು(sg)) {
			ಆದರೆ (max_cpu < 0)
				max_cpu = cpu;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (sched_asym_prefer(cpu, max_cpu))
				max_cpu = cpu;
		}
		sg->asym_prefer_cpu = max_cpu;

ಮುಂದಿನ:
		sg = sg->ಮುಂದಿನ;
	} ಆಗ (sg != sd->groups);

	ಆದರೆ (cpu != ಗುಂಪು_balance_cpu(sg))
		ಹಿಂತಿರುಗಿ;

	update_ಗುಂಪು_ಸಾಮರ್ಥ್ಯ(sd, cpu);
}

/* Update the "asym_prefer_cpu" when arch_asym_cpu_priority() changes. */
ಶೂನ್ಯ sched_update_asym_prefer_cpu(ಪೂರ್ಣಾಂಕ cpu, ಪೂರ್ಣಾಂಕ old_prio, ಪೂರ್ಣಾಂಕ new_prio)
{
	ಪೂರ್ಣಾಂಕ asym_prefer_cpu = cpu;
	ರಚನೆ sched_domain *sd;

	guard(rcu)();

	for_each_domain(cpu, sd) {
		ರಚನೆ sched_ಗುಂಪು *sg;
		ಪೂರ್ಣಾಂಕ ಗುಂಪು_cpu;

		ಆದರೆ (!(sd->flags & SD_ASYM_PACKING))
			ಮುಂದುವರಿ;

		/*
		 * Groups of overlapping domain are replicated per NUMA
		 * node and will require updating "asym_prefer_cpu" on
		 * each local copy.
		 *
		 * If you are hitting this warning, consider moving
		 * "sg->asym_prefer_cpu" to "sg->sgc->asym_prefer_cpu"
		 * which is shared by all the overlapping groups.
		 */
		WARN_ON_ONCE(sd->flags & SD_NUMA);

		sg = sd->groups;
		ಆದರೆ (cpu != sg->asym_prefer_cpu) {
			/*
			 * Since the parent is a superset of the current group,
			 * if the cpu is not the "asym_prefer_cpu" at the
			 * current level, it cannot be the preferred CPU at a
			 * higher levels either.
			 */
			ಆದರೆ (!sched_asym_prefer(cpu, sg->asym_prefer_cpu))
				ಹಿಂತಿರುಗಿ;

			WRITE_ONCE(sg->asym_prefer_cpu, cpu);
			ಮುಂದುವರಿ;
		}

		/* Ranking has improved; CPU is still the preferred one. */
		ಆದರೆ (new_prio >= old_prio)
			ಮುಂದುವರಿ;

		for_each_cpu(ಗುಂಪು_cpu, sched_ಗುಂಪು_ಹರವು(sg)) {
			ಆದರೆ (sched_asym_prefer(ಗುಂಪು_cpu, asym_prefer_cpu))
				asym_prefer_cpu = ಗುಂಪು_cpu;
		}

		WRITE_ONCE(sg->asym_prefer_cpu, asym_prefer_cpu);
	}
}

/*
 * Set of available CPUs grouped by their corresponding capacities
 * Each list entry contains a CPU mask reflecting CPUs that share the same
 * capacity.
 * The lifespan of data is unlimited.
 */
LIST_ತಲೆ(asym_cap_list);

/*
 * Verify whether there is any CPU capacity asymmetry in a given sched domain.
 * Provides sd_flags reflecting the asymmetry scope.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
asym_cpu_ಸಾಮರ್ಥ್ಯ_classify(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *sd_ಹರವು,
			   ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_map)
{
	ರಚನೆ asym_cap_ದತ್ತಾಂಶ *ನಮೂದು;
	ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0, ತಪ್ಪಿದ = 0;

	/*
	 * Count how many unique CPU capacities this domain spans across
	 * (compare sched_domain CPUs mask with ones representing  available
	 * CPUs capacities). Take into account CPUs that might be offline:
	 * skip those.
	 */
	list_for_each_ನಮೂದು(ನಮೂದು, &asym_cap_list, link) {
		ಆದರೆ (cpumask_intersects(sd_ಹರವು, cpu_ಸಾಮರ್ಥ್ಯ_ಹರವು(ನಮೂದು)))
			++ಎಣಿಕೆ_2;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cpumask_intersects(cpu_map, cpu_ಸಾಮರ್ಥ್ಯ_ಹರವು(ನಮೂದು)))
			++ತಪ್ಪಿದ;
	}

	WARN_ON_ONCE(!ಎಣಿಕೆ_2 && !list_ಖಾಲಿಗಳು(&asym_cap_list));

	/* No asymmetry detected */
	ಆದರೆ (ಎಣಿಕೆ_2 < 2)
		ಹಿಂತಿರುಗಿ 0;
	/* Some of the available CPU capacity values have not been detected */
	ಆದರೆ (ತಪ್ಪಿದ)
		ಹಿಂತಿರುಗಿ SD_ASYM_CPUCAPACITY;

	/* Full asymmetry */
	ಹಿಂತಿರುಗಿ SD_ASYM_CPUCAPACITY | SD_ASYM_CPUCAPACITY_ತುಂಬಿದ;

}

ಸ್ಥಿರ ಶೂನ್ಯ free_asym_cap_ನಮೂದು(ರಚನೆ rcu_ತಲೆ *ತಲೆ)
{
	ರಚನೆ asym_cap_ದತ್ತಾಂಶ *ನಮೂದು = container_ದ(ತಲೆ, ರಚನೆ asym_cap_ದತ್ತಾಂಶ, rcu);
	kfree(ನಮೂದು);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ asym_cpu_ಸಾಮರ್ಥ್ಯ_update_ದತ್ತಾಂಶ(ಪೂರ್ಣಾಂಕ cpu)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸಾಮರ್ಥ್ಯ = arch_scale_cpu_ಸಾಮರ್ಥ್ಯ(cpu);
	ರಚನೆ asym_cap_ದತ್ತಾಂಶ *ಸೇರಿಸು_ನಮೂದು = ಖಾಲಿ;
	ರಚನೆ asym_cap_ದತ್ತಾಂಶ *ನಮೂದು;

	/*
	 * Search if capacity already exits. If not, track which the entry
	 * where we should insert to keep the list ordered descending.
	 */
	list_for_each_ನಮೂದು(ನಮೂದು, &asym_cap_list, link) {
		ಆದರೆ (ಸಾಮರ್ಥ್ಯ == ನಮೂದು->ಸಾಮರ್ಥ್ಯ)
			ಹೋಗು ಮುಗಿದ;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ಸೇರಿಸು_ನಮೂದು && ಸಾಮರ್ಥ್ಯ > ನಮೂದು->ಸಾಮರ್ಥ್ಯ)
			ಸೇರಿಸು_ನಮೂದು = list_prev_ನಮೂದು(ನಮೂದು, link);
	}

	ನಮೂದು = kzalloc(ಗಾತ್ರ(*ನಮೂದು) + cpumask_ಗಾತ್ರ(), GFP_KERNEL);
	ಆದರೆ (WARN_ONCE(!ನಮೂದು, "Failed to allocate memory for asymmetry data\n"))
		ಹಿಂತಿರುಗಿ;
	ನಮೂದು->ಸಾಮರ್ಥ್ಯ = ಸಾಮರ್ಥ್ಯ;

	/* If NULL then the new capacity is the smallest, add last. */
	ಆದರೆ (!ಸೇರಿಸು_ನಮೂದು)
		list_ಕೂಡಿಸು_ಬಾಲ_rcu(&ನಮೂದು->link, &asym_cap_list);
	ಇಲ್ಲದಿದ್ದರೆ
		list_ಕೂಡಿಸು_rcu(&ನಮೂದು->link, &ಸೇರಿಸು_ನಮೂದು->link);
ಮುಗಿದ:
	__cpumask_set_cpu(cpu, cpu_ಸಾಮರ್ಥ್ಯ_ಹರವು(ನಮೂದು));
}

/*
 * Build-up/update list of CPUs grouped by their capacities
 * An update requires explicit request to rebuild sched domains
 * with state indicating CPU topology changes.
 */
ಸ್ಥಿರ ಶೂನ್ಯ asym_cpu_ಸಾಮರ್ಥ್ಯ_ಪರಿಶೀಲನೆ(ಶೂನ್ಯ)
{
	ರಚನೆ asym_cap_ದತ್ತಾಂಶ *ನಮೂದು, *ಮುಂದಿನ;
	ಪೂರ್ಣಾಂಕ cpu;

	list_for_each_ನಮೂದು(ನಮೂದು, &asym_cap_list, link)
		cpumask_clear(cpu_ಸಾಮರ್ಥ್ಯ_ಹರವು(ನಮೂದು));

	for_each_cpu_and(cpu, cpu_possible_ಮುಖವಾಡ, housekeeping_cpumask(HK_TYPE_DOMAIN))
		asym_cpu_ಸಾಮರ್ಥ್ಯ_update_ದತ್ತಾಂಶ(cpu);

	list_for_each_ನಮೂದು_ಸುರಕ್ಷಿತ(ನಮೂದು, ಮುಂದಿನ, &asym_cap_list, link) {
		ಆದರೆ (cpumask_ಖಾಲಿಗಳು(cpu_ಸಾಮರ್ಥ್ಯ_ಹರವು(ನಮೂದು))) {
			list_del_rcu(&ನಮೂದು->link);
			call_rcu(&ನಮೂದು->rcu, free_asym_cap_ನಮೂದು);
		}
	}

	/*
	 * Only one capacity value has been detected i.e. this system is symmetric.
	 * No need to keep this data around.
	 */
	ಆದರೆ (list_is_singular(&asym_cap_list)) {
		ನಮೂದು = list_ಮೊದಲ_ನಮೂದು(&asym_cap_list, typeof(*ನಮೂದು), link);
		list_del_rcu(&ನಮೂದು->link);
		call_rcu(&ನಮೂದು->rcu, free_asym_cap_ನಮೂದು);
	}
}

/*
 * Initializers for schedule domains
 * Non-inlined to reduce accumulated stack pressure in build_sched_domains()
 */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ default_relax_domain_level = -1;
ಪೂರ್ಣಾಂಕ sched_domain_level_max;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init ಸಿದ್ಧತೆ_relax_domain_level(ಅಕ್ಷರ *str)
{
	ಆದರೆ (kstrtoint(str, 0, &default_relax_domain_level))
		pr_warn("Unable to set relax_domain_level\n");

	ಹಿಂತಿರುಗಿ 1;
}
__setup("relax_domain_level=", ಸಿದ್ಧತೆ_relax_domain_level);

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_domain_attribute(ರಚನೆ sched_domain *sd,
				 ರಚನೆ sched_domain_attr *attr)
{
	ಪೂರ್ಣಾಂಕ request;

	ಆದರೆ (!attr || attr->relax_domain_level < 0) {
		ಆದರೆ (default_relax_domain_level < 0)
			ಹಿಂತಿರುಗಿ;
		request = default_relax_domain_level;
	} ಇಲ್ಲದಿದ್ದರೆ
		request = attr->relax_domain_level;

	ಆದರೆ (sd->level >= request) {
		/* Turn off idle balance on this domain: */
		sd->flags &= ~(SD_BALANCE_WAKE|SD_BALANCE_NEWIDLE);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ __sdt_free(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_map);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __sdt_alloc(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_map);

ಸ್ಥಿರ ಶೂನ್ಯ __sds_free(ರಚನೆ s_ದತ್ತಾಂಶ *d, ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_map);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __sds_alloc(ರಚನೆ s_ದತ್ತಾಂಶ *d, ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_map);

ಸ್ಥಿರ ಶೂನ್ಯ __free_domain_allocs(ರಚನೆ s_ದತ್ತಾಂಶ *d, ಎಣಿಕೆ s_alloc what,
				 ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_map)
{
	ಆಯ್ಕೆ (what) {
	ಸ್ಥಿತಿ sa_rootdomain:
		ಆದರೆ (!atomic_read(&d->rd->refcount))
			free_rootdomain(&d->rd->rcu);
		fallthrough;
	ಸ್ಥಿತಿ sa_sd:
		free_percpu(d->sd);
		fallthrough;
	ಸ್ಥಿತಿ sa_sd_shared:
		__sds_free(d, cpu_map);
		fallthrough;
	ಸ್ಥಿತಿ sa_sd_storage:
		__sdt_free(cpu_map);
		fallthrough;
	ಸ್ಥಿತಿ sa_none:
		ನಿಲ್ಲಿಸು;
	}
}

ಸ್ಥಿರ ಎಣಿಕೆ s_alloc
__visit_domain_allocation_hell(ರಚನೆ s_ದತ್ತಾಂಶ *d, ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_map)
{
	memset(d, 0, ಗಾತ್ರ(*d));

	ಆದರೆ (__sdt_alloc(cpu_map))
		ಹಿಂತಿರುಗಿ sa_sd_storage;
	ಆದರೆ (__sds_alloc(d, cpu_map))
		ಹಿಂತಿರುಗಿ sa_sd_shared;
	d->sd = alloc_percpu(ರಚನೆ sched_domain *);
	ಆದರೆ (!d->sd)
		ಹಿಂತಿರುಗಿ sa_sd_shared;
	d->rd = alloc_rootdomain();
	ಆದರೆ (!d->rd)
		ಹಿಂತಿರುಗಿ sa_sd;

	ಹಿಂತಿರುಗಿ sa_rootdomain;
}

/*
 * NULL the sd_data elements we've used to build the sched_domain and
 * sched_group structure so that the subsequent __free_domain_allocs()
 * will not free the data we're using.
 */
ಸ್ಥಿರ ಶೂನ್ಯ claim_allocations(ಪೂರ್ಣಾಂಕ cpu, ರಚನೆ s_ದತ್ತಾಂಶ *d)
{
	ರಚನೆ sched_domain *sd;

	ಆದರೆ (atomic_read(&(*ಪ್ರತಿ_cpu_ptr(d->sds, cpu))->ref))
		*ಪ್ರತಿ_cpu_ptr(d->sds, cpu) = ಖಾಲಿ;

	ಗಾಗಿ (sd = *ಪ್ರತಿ_cpu_ptr(d->sd, cpu); sd; sd = sd->ಪೋಷಕ) {
		ರಚನೆ sd_ದತ್ತಾಂಶ *sdd = sd->ಖಾಸಗಿ;

		WARN_ON_ONCE(*ಪ್ರತಿ_cpu_ptr(sdd->sd, cpu) != sd);
		*ಪ್ರತಿ_cpu_ptr(sdd->sd, cpu) = ಖಾಲಿ;

		ಆದರೆ (atomic_read(&(*ಪ್ರತಿ_cpu_ptr(sdd->sg, cpu))->ref))
			*ಪ್ರತಿ_cpu_ptr(sdd->sg, cpu) = ಖಾಲಿ;

		ಆದರೆ (atomic_read(&(*ಪ್ರತಿ_cpu_ptr(sdd->sgc, cpu))->ref))
			*ಪ್ರತಿ_cpu_ptr(sdd->sgc, cpu) = ಖಾಲಿ;
	}
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA
ಎಣಿಕೆ numa_topology_type sched_numa_topology_type;

/*
 * sched_domains_numa_distance is derived from sched_numa_node_distance
 * and provides a simplified view of NUMA distances used specifically
 * for building NUMA scheduling domains.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ			sched_domains_numa_ಮಟ್ಟಗಳು;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ			sched_numa_ಗಂಟು_ಮಟ್ಟಗಳು;

ಪೂರ್ಣಾಂಕ				sched_max_numa_ದೂರ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ			*sched_domains_numa_ದೂರ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ			*sched_numa_ಗಂಟು_ದೂರ;
ಸ್ಥಿರ ರಚನೆ cpumask		***sched_domains_numa_masks;
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_NUMA */

/*
 * SD_flags allowed in topology descriptions.
 *
 * These flags are purely descriptive of the topology and do not prescribe
 * behaviour. Behaviour is artificial and mapped in the below sd_init()
 * function. For details, see include/linux/sched/sd_flags.h.
 *
 *   SD_SHARE_CPUCAPACITY
 *   SD_SHARE_LLC
 *   SD_CLUSTER
 *   SD_NUMA
 *
 * Odd one out, which beside describing the topology has a quirk also
 * prescribes the desired behaviour that goes along with it:
 *
 *   SD_ASYM_PACKING        - describes SMT quirks
 */
#ವ್ಯಾಖ್ಯಾನಿಸು TOPOLOGY_SD_FLAGS		\
	(SD_SHARE_CPUCAPACITY	|	\
	 SD_CLUSTER		|	\
	 SD_SHARE_LLC		|	\
	 SD_NUMA		|	\
	 SD_ASYM_PACKING)

ಸ್ಥಿರ ರಚನೆ sched_domain *
sd_init(ರಚನೆ sched_domain_topology_level *tl,
	ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_map,
	ರಚನೆ sched_domain *ಮಗು, ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ sd_ದತ್ತಾಂಶ *sdd = &tl->ದತ್ತಾಂಶ;
	ರಚನೆ sched_domain *sd = *ಪ್ರತಿ_cpu_ptr(sdd->sd, cpu);
	ಪೂರ್ಣಾಂಕ sd_id, sd_ತೂಕ, sd_flags = 0;
	ರಚನೆ cpumask *sd_ಹರವು;
	u64 now = sched_clock();

	sd_ಹರವು = sched_domain_ಹರವು(sd);
	cpumask_and(sd_ಹರವು, cpu_map, tl->ಮುಖವಾಡ(tl, cpu));
	sd_ತೂಕ = cpumask_ತೂಕ(sd_ಹರವು);
	sd_id = cpumask_ಮೊದಲ(sd_ಹರವು);

	ಆದರೆ (tl->sd_flags)
		sd_flags = (*tl->sd_flags)();
	ಆದರೆ (WARN_ONCE(sd_flags & ~TOPOLOGY_SD_FLAGS,
		      "wrong sd_flags in topology description\n"))
		sd_flags &= TOPOLOGY_SD_FLAGS;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_SMT
	ಆದರೆ (sched_smt_asym_packing && (sd_flags & SD_SHARE_CPUCAPACITY))
		sd_flags |= SD_ASYM_PACKING;
#ಆದರೆ_ಅಂತ್ಯ
	sd_flags |= asym_cpu_ಸಾಮರ್ಥ್ಯ_classify(sd_ಹರವು, cpu_map);

	*sd = (ರಚನೆ sched_domain){
		.min_interval		= sd_ತೂಕ,
		.max_interval		= 2*sd_ತೂಕ,
		.busy_ಗುಣಕ		= 16,
		.imbalance_pct		= 117,

		.cache_nice_tries	= 0,

		.flags			= 1*SD_BALANCE_NEWIDLE
					| 1*SD_BALANCE_EXEC
					| 1*SD_BALANCE_FORK
					| 0*SD_BALANCE_WAKE
					| 1*SD_WAKE_AFFINE
					| 0*SD_SHARE_CPUCAPACITY
					| 0*SD_SHARE_LLC
					| 0*SD_SERIALIZE
					| 1*SD_PREFER_SIBLING
					| 0*SD_NUMA
					| sd_flags
					,

		.ಕೊನೆಯ_balance		= jiffies,
		.balance_interval	= sd_ತೂಕ,

		/* 50% success rate */
		.newidle_call		= 512,
		.newidle_success	= 256,
		.newidle_ಅನುಪಾತ		= 512,
		.newidle_stamp		= now,

		.max_newidle_lb_ವೆಚ್ಚ	= 0,
		.ಕೊನೆಯ_decay_max_lb_ವೆಚ್ಚ	= jiffies,
		.ಮಗು			= ಮಗು,
		.name			= tl->name,
	};

	WARN_ONCE((sd->flags & (SD_SHARE_CPUCAPACITY | SD_ASYM_CPUCAPACITY)) ==
		  (SD_SHARE_CPUCAPACITY | SD_ASYM_CPUCAPACITY),
		  "CPU capacity asymmetry not supported on SMT\n");

	/*
	 * Convert topological properties into behaviour.
	 */
	ಆದರೆ (sd->flags & SD_SHARE_CPUCAPACITY) {
		sd->imbalance_pct = 110;

	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (sd->flags & SD_SHARE_LLC) {
		sd->imbalance_pct = 117;
		sd->cache_nice_tries = 1;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (sd->flags & SD_NUMA) {
		sd->cache_nice_tries = 2;

		sd->flags &= ~SD_PREFER_SIBLING;
		sd->flags |= SD_SERIALIZE;
		ಆದರೆ (sched_domains_numa_ದೂರ[tl->numa_level] > ಗಂಟು_reclaim_ದೂರ) {
			sd->flags &= ~(SD_BALANCE_EXEC |
				       SD_BALANCE_FORK |
				       SD_WAKE_AFFINE);
		}

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_NUMA */
	} ಇಲ್ಲದಿದ್ದರೆ {
		sd->cache_nice_tries = 1;
	}

	sd->ಖಾಸಗಿ = sdd;

	ಹಿಂತಿರುಗಿ sd;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_SMT
ಪೂರ್ಣಾಂಕ cpu_smt_flags(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ SD_SHARE_CPUCAPACITY | SD_SHARE_LLC;
}

ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *tl_smt_ಮುಖವಾಡ(ರಚನೆ sched_domain_topology_level *tl, ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ cpu_smt_ಮುಖವಾಡ(cpu);
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CLUSTER
ಪೂರ್ಣಾಂಕ cpu_cluster_flags(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ SD_CLUSTER | SD_SHARE_LLC;
}

ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *tl_cls_ಮುಖವಾಡ(ರಚನೆ sched_domain_topology_level *tl, ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ cpu_clustergroup_ಮುಖವಾಡ(cpu);
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_MC
ಪೂರ್ಣಾಂಕ cpu_core_flags(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ SD_SHARE_LLC;
}

ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *tl_mc_ಮುಖವಾಡ(ರಚನೆ sched_domain_topology_level *tl, ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ cpu_coregroup_ಮುಖವಾಡ(cpu);
}

/*
 * Majority of architectures have LLC at MC domain level with exception
 * such as powerpc. Provide a way for arch to specify where its LLC is
 * if it falls in exception category
 */
# ifndef arch_llc_ಮುಖವಾಡ
#ವ್ಯಾಖ್ಯಾನಿಸು arch_llc_ಮುಖವಾಡ(cpu) cpu_coregroup_ಮುಖವಾಡ(cpu)
# endif

#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು arch_llc_ಮುಖವಾಡ(cpu) cpumask_ದ(cpu)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು llc_ಮುಖವಾಡ(cpu) arch_llc_ಮುಖವಾಡ(cpu)

ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *tl_pkg_ಮುಖವಾಡ(ರಚನೆ sched_domain_topology_level *tl, ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ cpu_ಗಂಟು_ಮುಖವಾಡ(cpu);
}

/*
 * Topology list, bottom-up.
 */
ಸ್ಥಿರ ರಚನೆ sched_domain_topology_level default_topology[] = {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_SMT
	SDTL_INIT(tl_smt_ಮುಖವಾಡ, cpu_smt_flags, SMT),
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CLUSTER
	SDTL_INIT(tl_cls_ಮುಖವಾಡ, cpu_cluster_flags, CLS),
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_MC
	SDTL_INIT(tl_mc_ಮುಖವಾಡ, cpu_core_flags, MC),
#ಆದರೆ_ಅಂತ್ಯ
	SDTL_INIT(tl_pkg_ಮುಖವಾಡ, ಖಾಲಿ, PKG),
	{ ಖಾಲಿ, },
};

ಸ್ಥಿರ ರಚನೆ sched_domain_topology_level *sched_domain_topology =
	default_topology;
ಸ್ಥಿರ ರಚನೆ sched_domain_topology_level *sched_domain_topology_saved;

#ವ್ಯಾಖ್ಯಾನಿಸು for_each_sd_topology(tl)			\
	ಗಾಗಿ (tl = sched_domain_topology; tl->ಮುಖವಾಡ; tl++)

ಶೂನ್ಯ __init ಗಣ_sched_topology(ರಚನೆ sched_domain_topology_level *tl)
{
	ಆದರೆ (WARN_ON_ONCE(sched_smp_initialized))
		ಹಿಂತಿರುಗಿ;

	sched_domain_topology = tl;
	sched_domain_topology_saved = ಖಾಲಿ;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cpu_numa_flags(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ SD_NUMA;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *sd_numa_ಮುಖವಾಡ(ರಚನೆ sched_domain_topology_level *tl, ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ sched_domains_numa_masks[tl->numa_level][cpu_ಗಮ್ಯ_ಗಂಟು(cpu)];
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_numa_warn(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಮುಗಿದ = false;
	ಪೂರ್ಣಾಂಕ i,j;

	ಆದರೆ (ಮುಗಿದ)
		ಹಿಂತಿರುಗಿ;

	ಮುಗಿದ = true;

	printk(KERN_WARNING "ERROR: %s\n\n", str);

	ಗಾಗಿ (i = 0; i < nr_ಗಂಟು_ids; i++) {
		printk(KERN_WARNING "  ");
		ಗಾಗಿ (j = 0; j < nr_ಗಂಟು_ids; j++) {
			ಆದರೆ (!ಗಂಟು_state(i, N_CPU) || !ಗಂಟು_state(j, N_CPU))
				printk(KERN_CONT "(%02d) ", ಗಂಟು_ದೂರ(i,j));
			ಇಲ್ಲದಿದ್ದರೆ
				printk(KERN_CONT " %02d  ", ಗಂಟು_ದೂರ(i,j));
		}
		printk(KERN_CONT "\n");
	}
	printk(KERN_WARNING "\n");
}

bool ಕಂಡುಹಿಡಿ_numa_ದೂರ(ಪೂರ್ಣಾಂಕ ದೂರ)
{
	bool ಸಿಕ್ಕಿತು = false;
	ಪೂರ್ಣಾಂಕ i, *distances;

	ಆದರೆ (ದೂರ == ಗಂಟು_ದೂರ(0, 0))
		ಹಿಂತಿರುಗಿ true;

	rcu_read_ಬೀಗ();
	distances = rcu_dereference(sched_numa_ಗಂಟು_ದೂರ);
	ಆದರೆ (!distances)
		ಹೋಗು unlock;
	ಗಾಗಿ (i = 0; i < sched_numa_ಗಂಟು_ಮಟ್ಟಗಳು; i++) {
		ಆದರೆ (distances[i] == ದೂರ) {
			ಸಿಕ್ಕಿತು = true;
			ನಿಲ್ಲಿಸು;
		}
	}
unlock:
	rcu_read_unlock();

	ಹಿಂತಿರುಗಿ ಸಿಕ್ಕಿತು;
}

#ವ್ಯಾಖ್ಯಾನಿಸು for_each_cpu_ಗಂಟು_but(n, nbut)		\
	for_each_ಗಂಟು_state(n, N_CPU)		\
		ಆದರೆ (n == nbut)			\
			ಮುಂದುವರಿ;		\
		ಇಲ್ಲದಿದ್ದರೆ

/*
 * A system can have three types of NUMA topology:
 * NUMA_DIRECT: all nodes are directly connected, or not a NUMA system
 * NUMA_GLUELESS_MESH: some nodes reachable through intermediary nodes
 * NUMA_BACKPLANE: nodes can reach other nodes through a backplane
 *
 * The difference between a glueless mesh topology and a backplane
 * topology lies in whether communication between not directly
 * connected nodes goes through intermediary nodes (where programs
 * could run), or through backplane controllers. This affects
 * placement of programs.
 *
 * The type of topology can be discerned with the following tests:
 * - If the maximum distance between any nodes is 1 hop, the system
 *   is directly connected.
 * - If for two nodes A and B, located N > 1 hops away from each other,
 *   there is an intermediary node C, which is < N hops away from both
 *   nodes A and B, the system is a glueless mesh.
 */
ಸ್ಥಿರ ಶೂನ್ಯ init_numa_topology_type(ಪೂರ್ಣಾಂಕ offline_ಗಂಟು)
{
	ಪೂರ್ಣಾಂಕ a, b, c, n;

	n = sched_max_numa_ದೂರ;

	ಆದರೆ (sched_domains_numa_ಮಟ್ಟಗಳು <= 2) {
		sched_numa_topology_type = NUMA_ನೇರ;
		ಹಿಂತಿರುಗಿ;
	}

	for_each_cpu_ಗಂಟು_but(a, offline_ಗಂಟು) {
		for_each_cpu_ಗಂಟು_but(b, offline_ಗಂಟು) {
			/* Find two nodes furthest removed from each other. */
			ಆದರೆ (ಗಂಟು_ದೂರ(a, b) < n)
				ಮುಂದುವರಿ;

			/* Is there an intermediary node between a and b? */
			for_each_cpu_ಗಂಟು_but(c, offline_ಗಂಟು) {
				ಆದರೆ (ಗಂಟು_ದೂರ(a, c) < n &&
				    ಗಂಟು_ದೂರ(b, c) < n) {
					sched_numa_topology_type =
							NUMA_GLUELESS_MESH;
					ಹಿಂತಿರುಗಿ;
				}
			}

			sched_numa_topology_type = NUMA_BACKPLANE;
			ಹಿಂತಿರುಗಿ;
		}
	}

	pr_err("Failed to find a NUMA topology type, defaulting to DIRECT\n");
	sched_numa_topology_type = NUMA_ನೇರ;
}


#ವ್ಯಾಖ್ಯಾನಿಸು NR_ದೂರ_ಮೌಲ್ಯಗಳು (1 << ದೂರ_ಬಿಟ್ಟುಗಳು)

/*
 * An architecture could modify its NUMA distance, to change
 * grouping of NUMA nodes and number of NUMA levels when creating
 * NUMA level sched domains.
 *
 * A NUMA level is created for each unique
 * arch_sched_node_distance.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ numa_ಗಂಟು_dist(ಪೂರ್ಣಾಂಕ i, ಪೂರ್ಣಾಂಕ j)
{
	ಹಿಂತಿರುಗಿ ಗಂಟು_ದೂರ(i, j);
}

ಪೂರ್ಣಾಂಕ arch_sched_ಗಂಟು_ದೂರ(ಪೂರ್ಣಾಂಕ from, ಪೂರ್ಣಾಂಕ ಗಮ್ಯ)
			     __weak __alias(numa_ಗಂಟು_dist);

ಸ್ಥಿರ bool modified_sched_ಗಂಟು_ದೂರ(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ numa_ಗಂಟು_dist != arch_sched_ಗಂಟು_ದೂರ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_ದಾಖಲೆ_numa_dist(ಪೂರ್ಣಾಂಕ offline_ಗಂಟು, ಪೂರ್ಣಾಂಕ (*n_dist)(ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ),
				  ಪೂರ್ಣಾಂಕ **dist, ಪೂರ್ಣಾಂಕ *ಮಟ್ಟಗಳು)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *ದೂರ_map __free(bitmap) = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ nr_ಮಟ್ಟಗಳು = 0;
	ಪೂರ್ಣಾಂಕ i, j;
	ಪೂರ್ಣಾಂಕ *distances;

	/*
	 * O(nr_nodes^2) de-duplicating selection sort -- in order to find the
	 * unique distances in the node_distance() table.
	 */
	ದೂರ_map = bitmap_alloc(NR_ದೂರ_ಮೌಲ್ಯಗಳು, GFP_KERNEL);
	ಆದರೆ (!ದೂರ_map)
		ಹಿಂತಿರುಗಿ -ENOMEM;

	bitmap_zero(ದೂರ_map, NR_ದೂರ_ಮೌಲ್ಯಗಳು);
	for_each_cpu_ಗಂಟು_but(i, offline_ಗಂಟು) {
		for_each_cpu_ಗಂಟು_but(j, offline_ಗಂಟು) {
			ಪೂರ್ಣಾಂಕ ದೂರ = n_dist(i, j);

			ಆದರೆ (ದೂರ < LOCAL_ದೂರ || ದೂರ >= NR_ದೂರ_ಮೌಲ್ಯಗಳು) {
				sched_numa_warn("Invalid distance value range");
				ಹಿಂತಿರುಗಿ -EINVAL;
			}

			bitmap_ಗಣ(ದೂರ_map, ದೂರ, 1);
		}
	}
	/*
	 * We can now figure out how many unique distance values there are and
	 * allocate memory accordingly.
	 */
	nr_ಮಟ್ಟಗಳು = bitmap_ತೂಕ(ದೂರ_map, NR_ದೂರ_ಮೌಲ್ಯಗಳು);

	distances = kzalloc_objs(ಪೂರ್ಣಾಂಕ, nr_ಮಟ್ಟಗಳು);
	ಆದರೆ (!distances)
		ಹಿಂತಿರುಗಿ -ENOMEM;

	ಗಾಗಿ (i = 0, j = 0; i < nr_ಮಟ್ಟಗಳು; i++, j++) {
		j = ಕಂಡುಹಿಡಿ_ಮುಂದಿನ_ಬಿಟ್(ದೂರ_map, NR_ದೂರ_ಮೌಲ್ಯಗಳು, j);
		distances[i] = j;
	}
	*dist = distances;
	*ಮಟ್ಟಗಳು = nr_ಮಟ್ಟಗಳು;

	ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ sched_init_numa(ಪೂರ್ಣಾಂಕ offline_ಗಂಟು)
{
	ರಚನೆ sched_domain_topology_level *tl;
	ಪೂರ್ಣಾಂಕ nr_ಮಟ್ಟಗಳು, nr_ಗಂಟು_ಮಟ್ಟಗಳು;
	ಪೂರ್ಣಾಂಕ i, j;
	ಪೂರ್ಣಾಂಕ *distances, *domain_distances;
	ರಚನೆ cpumask ***masks;

	/* Record the NUMA distances from SLIT table */
	ಆದರೆ (sched_ದಾಖಲೆ_numa_dist(offline_ಗಂಟು, numa_ಗಂಟು_dist, &distances,
				   &nr_ಗಂಟು_ಮಟ್ಟಗಳು))
		ಹಿಂತಿರುಗಿ;

	/* Record modified NUMA distances for building sched domains */
	ಆದರೆ (modified_sched_ಗಂಟು_ದೂರ()) {
		ಆದರೆ (sched_ದಾಖಲೆ_numa_dist(offline_ಗಂಟು, arch_sched_ಗಂಟು_ದೂರ,
					   &domain_distances, &nr_ಮಟ್ಟಗಳು)) {
			kfree(distances);
			ಹಿಂತಿರುಗಿ;
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		domain_distances = distances;
		nr_ಮಟ್ಟಗಳು = nr_ಗಂಟು_ಮಟ್ಟಗಳು;
	}
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(sched_numa_ಗಂಟು_ದೂರ, distances);
	WRITE_ONCE(sched_max_numa_ದೂರ, distances[nr_ಗಂಟು_ಮಟ್ಟಗಳು - 1]);
	WRITE_ONCE(sched_numa_ಗಂಟು_ಮಟ್ಟಗಳು, nr_ಗಂಟು_ಮಟ್ಟಗಳು);

	/*
	 * 'nr_levels' contains the number of unique distances
	 *
	 * The sched_domains_numa_distance[] array includes the actual distance
	 * numbers.
	 */

	/*
	 * Here, we should temporarily reset sched_domains_numa_levels to 0.
	 * If it fails to allocate memory for array sched_domains_numa_masks[][],
	 * the array will contain less then 'nr_levels' members. This could be
	 * dangerous when we use it to iterate array sched_domains_numa_masks[][]
	 * in other functions.
	 *
	 * We reset it to 'nr_levels' at the end of this function.
	 */
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(sched_domains_numa_ದೂರ, domain_distances);

	sched_domains_numa_ಮಟ್ಟಗಳು = 0;

	masks = kzalloc(ಗಾತ್ರ(ಶೂನ್ಯ *) * nr_ಮಟ್ಟಗಳು, GFP_KERNEL);
	ಆದರೆ (!masks)
		ಹಿಂತಿರುಗಿ;

	/*
	 * Now for each level, construct a mask per node which contains all
	 * CPUs of nodes that are that many hops away from us.
	 */
	ಗಾಗಿ (i = 0; i < nr_ಮಟ್ಟಗಳು; i++) {
		masks[i] = kzalloc(nr_ಗಂಟು_ids * ಗಾತ್ರ(ಶೂನ್ಯ *), GFP_KERNEL);
		ಆದರೆ (!masks[i])
			ಹಿಂತಿರುಗಿ;

		for_each_cpu_ಗಂಟು_but(j, offline_ಗಂಟು) {
			ರಚನೆ cpumask *ಮುಖವಾಡ = kzalloc(cpumask_ಗಾತ್ರ(), GFP_KERNEL);
			ಪೂರ್ಣಾಂಕ k;

			ಆದರೆ (!ಮುಖವಾಡ)
				ಹಿಂತಿರುಗಿ;

			masks[i][j] = ಮುಖವಾಡ;

			for_each_cpu_ಗಂಟು_but(k, offline_ಗಂಟು) {
				ಆದರೆ (sched_debug() &&
				    (arch_sched_ಗಂಟು_ದೂರ(j, k) !=
				     arch_sched_ಗಂಟು_ದೂರ(k, j)))
					sched_numa_warn("Node-distance not symmetric");

				ಆದರೆ (arch_sched_ಗಂಟು_ದೂರ(j, k) >
				    sched_domains_numa_ದೂರ[i])
					ಮುಂದುವರಿ;

				cpumask_or(ಮುಖವಾಡ, ಮುಖವಾಡ, cpumask_ದ_ಗಂಟು(k));
			}
		}
	}
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(sched_domains_numa_masks, masks);

	/* Compute default topology size */
	ಗಾಗಿ (i = 0; sched_domain_topology[i].ಮುಖವಾಡ; i++);

	tl = kzalloc((i + nr_ಮಟ್ಟಗಳು + 1) *
			ಗಾತ್ರ(ರಚನೆ sched_domain_topology_level), GFP_KERNEL);
	ಆದರೆ (!tl)
		ಹಿಂತಿರುಗಿ;

	/*
	 * Copy the default topology bits..
	 */
	ಗಾಗಿ (i = 0; sched_domain_topology[i].ಮುಖವಾಡ; i++)
		tl[i] = sched_domain_topology[i];

	/*
	 * Add the NUMA identity distance, aka single NODE.
	 */
	tl[i++] = SDTL_INIT(sd_numa_ಮುಖವಾಡ, ಖಾಲಿ, ಗಂಟು_2);

	/*
	 * .. and append 'j' levels of NUMA goodness.
	 */
	ಗಾಗಿ (j = 1; j < nr_ಮಟ್ಟಗಳು; i++, j++) {
		tl[i] = SDTL_INIT(sd_numa_ಮುಖವಾಡ, cpu_numa_flags, NUMA);
		tl[i].numa_level = j;
	}

	sched_domain_topology_saved = sched_domain_topology;
	sched_domain_topology = tl;

	sched_domains_numa_ಮಟ್ಟಗಳು = nr_ಮಟ್ಟಗಳು;

	init_numa_topology_type(offline_ಗಂಟು);
}


ಸ್ಥಿರ ಶೂನ್ಯ sched_reset_numa(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ nr_ಮಟ್ಟಗಳು, *distances, *dom_distances = ಖಾಲಿ;
	ರಚನೆ cpumask ***masks;

	nr_ಮಟ್ಟಗಳು = sched_domains_numa_ಮಟ್ಟಗಳು;
	sched_numa_ಗಂಟು_ಮಟ್ಟಗಳು = 0;
	sched_domains_numa_ಮಟ್ಟಗಳು = 0;
	sched_max_numa_ದೂರ = 0;
	sched_numa_topology_type = NUMA_ನೇರ;
	distances = sched_numa_ಗಂಟು_ದೂರ;
	ಆದರೆ (sched_numa_ಗಂಟು_ದೂರ != sched_domains_numa_ದೂರ)
		dom_distances = sched_domains_numa_ದೂರ;
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(sched_numa_ಗಂಟು_ದೂರ, ಖಾಲಿ);
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(sched_domains_numa_ದೂರ, ಖಾಲಿ);
	masks = sched_domains_numa_masks;
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(sched_domains_numa_masks, ಖಾಲಿ);
	ಆದರೆ (distances || masks) {
		ಪೂರ್ಣಾಂಕ i, j;

		synchronize_rcu();
		kfree(distances);
		kfree(dom_distances);
		ಗಾಗಿ (i = 0; i < nr_ಮಟ್ಟಗಳು && masks; i++) {
			ಆದರೆ (!masks[i])
				ಮುಂದುವರಿ;
			for_each_ಗಂಟು(j)
				kfree(masks[i][j]);
			kfree(masks[i]);
		}
		kfree(masks);
	}
	ಆದರೆ (sched_domain_topology_saved) {
		kfree(sched_domain_topology);
		sched_domain_topology = sched_domain_topology_saved;
		sched_domain_topology_saved = ಖಾಲಿ;
	}
}

/*
 * Call with hotplug lock held
 */
ಶೂನ್ಯ sched_update_numa(ಪೂರ್ಣಾಂಕ cpu, bool online)
{
	ಪೂರ್ಣಾಂಕ ಗಂಟು;

	ಗಂಟು = cpu_ಗಮ್ಯ_ಗಂಟು(cpu);
	/*
	 * Scheduler NUMA topology is updated when the first CPU of a
	 * node is onlined or the last CPU of a node is offlined.
	 */
	ಆದರೆ (cpumask_ತೂಕ(cpumask_ದ_ಗಂಟು(ಗಂಟು)) != 1)
		ಹಿಂತಿರುಗಿ;

	sched_reset_numa();
	sched_init_numa(online ? NUMA_NO_ಗಂಟು : ಗಂಟು);
}

ಶೂನ್ಯ sched_domains_numa_masks_ಗಣ(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu)
{
	ಪೂರ್ಣಾಂಕ ಗಂಟು = cpu_ಗಮ್ಯ_ಗಂಟು(cpu);
	ಪೂರ್ಣಾಂಕ i, j;

	ಗಾಗಿ (i = 0; i < sched_domains_numa_ಮಟ್ಟಗಳು; i++) {
		ಗಾಗಿ (j = 0; j < nr_ಗಂಟು_ids; j++) {
			ಆದರೆ (!ಗಂಟು_state(j, N_CPU))
				ಮುಂದುವರಿ;

			/* Set ourselves in the remote node's masks */
			ಆದರೆ (arch_sched_ಗಂಟು_ದೂರ(j, ಗಂಟು) <=
			    sched_domains_numa_ದೂರ[i])
				cpumask_ಗಣ_cpu(cpu, sched_domains_numa_masks[i][j]);
		}
	}
}

ಶೂನ್ಯ sched_domains_numa_masks_clear(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu)
{
	ಪೂರ್ಣಾಂಕ i, j;

	ಗಾಗಿ (i = 0; i < sched_domains_numa_ಮಟ್ಟಗಳು; i++) {
		ಗಾಗಿ (j = 0; j < nr_ಗಂಟು_ids; j++) {
			ಆದರೆ (sched_domains_numa_masks[i][j])
				cpumask_clear_cpu(cpu, sched_domains_numa_masks[i][j]);
		}
	}
}

/*
 * sched_numa_find_closest() - given the NUMA topology, find the cpu
 *                             closest to @cpu from @cpumask.
 * cpumask: cpumask to find a cpu from
 * cpu: cpu to be close to
 *
 * returns: cpu, or nr_cpu_ids when nothing found.
 */
ಪೂರ್ಣಾಂಕ sched_numa_ಕಂಡುಹಿಡಿ_closest(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpus, ಪೂರ್ಣಾಂಕ cpu)
{
	ಪೂರ್ಣಾಂಕ i, j = cpu_ಗಮ್ಯ_ಗಂಟು(cpu), ಸಿಕ್ಕಿತು = nr_cpu_ids;
	ರಚನೆ cpumask ***masks;

	rcu_read_ಬೀಗ();
	masks = rcu_dereference(sched_domains_numa_masks);
	ಆದರೆ (!masks)
		ಹೋಗು unlock;
	ಗಾಗಿ (i = 0; i < sched_domains_numa_ಮಟ್ಟಗಳು; i++) {
		ಆದರೆ (!masks[i][j])
			ನಿಲ್ಲಿಸು;
		cpu = cpumask_any_and_distribute(cpus, masks[i][j]);
		ಆದರೆ (cpu < nr_cpu_ids) {
			ಸಿಕ್ಕಿತು = cpu;
			ನಿಲ್ಲಿಸು;
		}
	}
unlock:
	rcu_read_unlock();

	ಹಿಂತಿರುಗಿ ಸಿಕ್ಕಿತು;
}

ರಚನೆ __cmp_key {
	ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpus;
	ರಚನೆ cpumask ***masks;
	ಪೂರ್ಣಾಂಕ ಗಂಟು;
	ಪೂರ್ಣಾಂಕ cpu;
	ಪೂರ್ಣಾಂಕ w;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ hop_cmp(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b)
{
	ರಚನೆ cpumask **prev_hop, **cur_hop = *(ರಚನೆ cpumask ***)b;
	ರಚನೆ __cmp_key *k = (ರಚನೆ __cmp_key *)a;

	ಆದರೆ (cpumask_ತೂಕ_and(k->cpus, cur_hop[k->ಗಂಟು]) <= k->cpu)
		ಹಿಂತಿರುಗಿ 1;

	ಆದರೆ (b == k->masks) {
		k->w = 0;
		ಹಿಂತಿರುಗಿ 0;
	}

	prev_hop = *((ರಚನೆ cpumask ***)b - 1);
	k->w = cpumask_ತೂಕ_and(k->cpus, prev_hop[k->ಗಂಟು]);
	ಆದರೆ (k->w <= k->cpu)
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ -1;
}

/**
 * sched_numa_find_nth_cpu() - given the NUMA topology, find the Nth closest CPU
 *                             from @cpus to @cpu, taking into account distance
 *                             from a given @node.
 * @cpus: cpumask to find a cpu from
 * @cpu: CPU to start searching
 * @node: NUMA node to order CPUs by distance
 *
 * Return: cpu, or nr_cpu_ids when nothing found.
 */
ಪೂರ್ಣಾಂಕ sched_numa_ಕಂಡುಹಿಡಿ_nth_cpu(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpus, ಪೂರ್ಣಾಂಕ cpu, ಪೂರ್ಣಾಂಕ ಗಂಟು)
{
	ರಚನೆ __cmp_key k = { .cpus = cpus, .cpu = cpu };
	ರಚನೆ cpumask ***hop_masks;
	ಪೂರ್ಣಾಂಕ hop, ret = nr_cpu_ids;

	ಆದರೆ (ಗಂಟು == NUMA_NO_ಗಂಟು)
		ಹಿಂತಿರುಗಿ cpumask_nth_and(cpu, cpus, cpu_online_ಮುಖವಾಡ);

	rcu_read_ಬೀಗ();

	/* CPU-less node entries are uninitialized in sched_domains_numa_masks */
	ಗಂಟು = numa_ಹತ್ತಿರದ_ಗಂಟು(ಗಂಟು, N_CPU);
	k.ಗಂಟು = ಗಂಟು;

	k.masks = rcu_dereference(sched_domains_numa_masks);
	ಆದರೆ (!k.masks)
		ಹೋಗು unlock;

	hop_masks = bsearch(&k, k.masks, sched_domains_numa_ಮಟ್ಟಗಳು, ಗಾತ್ರ(k.masks[0]), hop_cmp);
	ಆದರೆ (!hop_masks)
		ಹೋಗು unlock;
	hop = hop_masks	- k.masks;

	ret = hop ?
		cpumask_nth_and_andnot(cpu - k.w, cpus, k.masks[hop][ಗಂಟು], k.masks[hop-1][ಗಂಟು]) :
		cpumask_nth_and(cpu, cpus, k.masks[0][ಗಂಟು]);
unlock:
	rcu_read_unlock();
	ಹಿಂತಿರುಗಿ ret;
}
EXPORT_ಚಿಹ್ನೆ_GPL(sched_numa_ಕಂಡುಹಿಡಿ_nth_cpu);

/**
 * sched_numa_hop_mask() - Get the cpumask of CPUs at most @hops hops away from
 *                         @node
 * @node: The node to count hops from.
 * @hops: Include CPUs up to that many hops away. 0 means local node.
 *
 * Return: On success, a pointer to a cpumask of CPUs at most @hops away from
 * @node, an error value otherwise.
 *
 * Requires rcu_lock to be held. Returned cpumask is only valid within that
 * read-side section, copy it if required beyond that.
 *
 * Note that not all hops are equal in distance; see sched_init_numa() for how
 * distances and masks are handled.
 * Also note that this is a reflection of sched_domains_numa_masks, which may change
 * during the lifetime of the system (offline nodes are taken out of the masks).
 */
ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *sched_numa_hop_ಮುಖವಾಡ(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಗಂಟು, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ hops)
{
	ರಚನೆ cpumask ***masks;

	ಆದರೆ (ಗಂಟು >= nr_ಗಂಟು_ids || hops >= sched_domains_numa_ಮಟ್ಟಗಳು)
		ಹಿಂತಿರುಗಿ ERR_PTR(-EINVAL);

	masks = rcu_dereference(sched_domains_numa_masks);
	ಆದರೆ (!masks)
		ಹಿಂತಿರುಗಿ ERR_PTR(-EBUSY);

	ಹಿಂತಿರುಗಿ masks[hops][ಗಂಟು];
}
EXPORT_ಚಿಹ್ನೆ_GPL(sched_numa_hop_ಮುಖವಾಡ);

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_NUMA */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __sdt_alloc(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_map)
{
	ರಚನೆ sched_domain_topology_level *tl;
	ಪೂರ್ಣಾಂಕ j;

	for_each_sd_topology(tl) {
		ರಚನೆ sd_ದತ್ತಾಂಶ *sdd = &tl->ದತ್ತಾಂಶ;

		sdd->sd = alloc_percpu(ರಚನೆ sched_domain *);
		ಆದರೆ (!sdd->sd)
			ಹಿಂತಿರುಗಿ -ENOMEM;

		sdd->sg = alloc_percpu(ರಚನೆ sched_ಗುಂಪು *);
		ಆದರೆ (!sdd->sg)
			ಹಿಂತಿರುಗಿ -ENOMEM;

		sdd->sgc = alloc_percpu(ರಚನೆ sched_ಗುಂಪು_ಸಾಮರ್ಥ್ಯ *);
		ಆದರೆ (!sdd->sgc)
			ಹಿಂತಿರುಗಿ -ENOMEM;

		for_each_cpu(j, cpu_map) {
			ರಚನೆ sched_domain *sd;
			ರಚನೆ sched_ಗುಂಪು *sg;
			ರಚನೆ sched_ಗುಂಪು_ಸಾಮರ್ಥ್ಯ *sgc;

			sd = kzalloc_ಗಂಟು(ಗಾತ್ರ(ರಚನೆ sched_domain) + cpumask_ಗಾತ್ರ(),
					GFP_KERNEL, cpu_ಗಮ್ಯ_ಗಂಟು(j));
			ಆದರೆ (!sd)
				ಹಿಂತಿರುಗಿ -ENOMEM;

			*ಪ್ರತಿ_cpu_ptr(sdd->sd, j) = sd;

			sg = kzalloc_ಗಂಟು(ಗಾತ್ರ(ರಚನೆ sched_ಗುಂಪು) + cpumask_ಗಾತ್ರ(),
					GFP_KERNEL, cpu_ಗಮ್ಯ_ಗಂಟು(j));
			ಆದರೆ (!sg)
				ಹಿಂತಿರುಗಿ -ENOMEM;

			sg->ಮುಂದಿನ = sg;

			*ಪ್ರತಿ_cpu_ptr(sdd->sg, j) = sg;

			sgc = kzalloc_ಗಂಟು(ಗಾತ್ರ(ರಚನೆ sched_ಗುಂಪು_ಸಾಮರ್ಥ್ಯ) + cpumask_ಗಾತ್ರ(),
					GFP_KERNEL, cpu_ಗಮ್ಯ_ಗಂಟು(j));
			ಆದರೆ (!sgc)
				ಹಿಂತಿರುಗಿ -ENOMEM;

			sgc->id = j;

			*ಪ್ರತಿ_cpu_ptr(sdd->sgc, j) = sgc;
		}
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ __sdt_free(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_map)
{
	ರಚನೆ sched_domain_topology_level *tl;
	ಪೂರ್ಣಾಂಕ j;

	for_each_sd_topology(tl) {
		ರಚನೆ sd_ದತ್ತಾಂಶ *sdd = &tl->ದತ್ತಾಂಶ;

		for_each_cpu(j, cpu_map) {
			ರಚನೆ sched_domain *sd;

			ಆದರೆ (sdd->sd) {
				sd = *ಪ್ರತಿ_cpu_ptr(sdd->sd, j);
				ಆದರೆ (sd && (sd->flags & SD_NUMA))
					free_sched_groups(sd->groups, 0);
				kfree(*ಪ್ರತಿ_cpu_ptr(sdd->sd, j));
			}

			ಆದರೆ (sdd->sg)
				kfree(*ಪ್ರತಿ_cpu_ptr(sdd->sg, j));
			ಆದರೆ (sdd->sgc)
				kfree(*ಪ್ರತಿ_cpu_ptr(sdd->sgc, j));
		}
		free_percpu(sdd->sd);
		sdd->sd = ಖಾಲಿ;
		free_percpu(sdd->sg);
		sdd->sg = ಖಾಲಿ;
		free_percpu(sdd->sgc);
		sdd->sgc = ಖಾಲಿ;
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __sds_alloc(ರಚನೆ s_ದತ್ತಾಂಶ *d, ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_map)
{
	ಪೂರ್ಣಾಂಕ j;

	d->sds = alloc_percpu(ರಚನೆ sched_domain_shared *);
	ಆದರೆ (!d->sds)
		ಹಿಂತಿರುಗಿ -ENOMEM;

	for_each_cpu(j, cpu_map) {
		ರಚನೆ sched_domain_shared *sds;

		sds = kzalloc_ಗಂಟು(ಗಾತ್ರ(ರಚನೆ sched_domain_shared),
				GFP_KERNEL, cpu_ಗಮ್ಯ_ಗಂಟು(j));
		ಆದರೆ (!sds)
			ಹಿಂತಿರುಗಿ -ENOMEM;

		*ಪ್ರತಿ_cpu_ptr(d->sds, j) = sds;
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ __sds_free(ರಚನೆ s_ದತ್ತಾಂಶ *d, ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_map)
{
	ಪೂರ್ಣಾಂಕ j;

	ಆದರೆ (!d->sds)
		ಹಿಂತಿರುಗಿ;

	for_each_cpu(j, cpu_map)
		kfree(*ಪ್ರತಿ_cpu_ptr(d->sds, j));

	free_percpu(d->sds);
	d->sds = ಖಾಲಿ;
}

ಸ್ಥಿರ ರಚನೆ sched_domain *build_sched_domain(ರಚನೆ sched_domain_topology_level *tl,
		ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_map, ರಚನೆ sched_domain_attr *attr,
		ರಚನೆ sched_domain *ಮಗು, ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ sched_domain *sd = sd_init(tl, cpu_map, ಮಗು, cpu);

	ಆದರೆ (ಮಗು) {
		sd->level = ಮಗು->level + 1;
		sched_domain_level_max = max(sched_domain_level_max, sd->level);
		ಮಗು->ಪೋಷಕ = sd;

		ಆದರೆ (!cpumask_subset(sched_domain_ಹರವು(ಮಗು),
				    sched_domain_ಹರವು(sd))) {
			pr_err("BUG: arch topology borken\n");
			pr_err("     the %s domain not a subset of the %s domain\n",
					ಮಗು->name, sd->name);
			/* Fixup, ensure @sd has at least @child CPUs. */
			cpumask_or(sched_domain_ಹರವು(sd),
				   sched_domain_ಹರವು(sd),
				   sched_domain_ಹರವು(ಮಗು));
		}

	}
	ಗಣ_domain_attribute(sd, attr);

	ಹಿಂತಿರುಗಿ sd;
}

/*
 * Ensure topology masks are sane, i.e. there are no conflicts (overlaps) for
 * any two given CPUs on non-NUMA topology levels.
 */
ಸ್ಥಿರ bool topology_ಹರವು_sane(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_map)
{
	ರಚನೆ sched_domain_topology_level *tl;
	ರಚನೆ cpumask *covered, *id_ನೋಡಿದ;
	ಪೂರ್ಣಾಂಕ cpu;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;
	id_ನೋಡಿದ = sched_domains_tmpmask2;

	for_each_sd_topology(tl) {
		ಪೂರ್ಣಾಂಕ tl_common_flags = 0;

		ಆದರೆ (tl->sd_flags)
			tl_common_flags = (*tl->sd_flags)();

		/* NUMA levels are allowed to overlap */
		ಆದರೆ (tl_common_flags & SD_NUMA)
			ಮುಂದುವರಿ;

		cpumask_clear(covered);
		cpumask_clear(id_ನೋಡಿದ);

		/*
		 * Non-NUMA levels cannot partially overlap - they must be either
		 * completely equal or completely disjoint. Otherwise we can end up
		 * breaking the sched_group lists - i.e. a later get_group() pass
		 * breaks the linking done for an earlier span.
		 */
		for_each_cpu(cpu, cpu_map) {
			ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *tl_cpu_ಮುಖವಾಡ = tl->ಮುಖವಾಡ(tl, cpu);
			ಪೂರ್ಣಾಂಕ id;

			/* lowest bit set in this mask is used as a unique id */
			id = cpumask_ಮೊದಲ(tl_cpu_ಮುಖವಾಡ);

			ಆದರೆ (cpumask_test_cpu(id, id_ನೋಡಿದ)) {
				/* First CPU has already been seen, ensure identical spans */
				ಆದರೆ (!cpumask_equal(tl->ಮುಖವಾಡ(tl, id), tl_cpu_ಮುಖವಾಡ))
					ಹಿಂತಿರುಗಿ false;
			} ಇಲ್ಲದಿದ್ದರೆ {
				/* First CPU hasn't been seen before, ensure it's a completely new span */
				ಆದರೆ (cpumask_intersects(tl_cpu_ಮುಖವಾಡ, covered))
					ಹಿಂತಿರುಗಿ false;

				cpumask_or(covered, covered, tl_cpu_ಮುಖವಾಡ);
				cpumask_ಗಣ_cpu(id, id_ನೋಡಿದ);
			}
		}
	}
	ಹಿಂತಿರುಗಿ true;
}

/*
 * Calculate an allowed NUMA imbalance such that LLCs do not get
 * imbalanced.
 */
ಸ್ಥಿರ ಶೂನ್ಯ adjust_numa_imbalance(ರಚನೆ sched_domain *sd_llc)
{
	ರಚನೆ sched_domain *ಪೋಷಕ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ imb_ಹರವು = 1;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ imb = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr_llcs;

	WARN_ON(!(sd_llc->flags & SD_SHARE_LLC));
	WARN_ON(!sd_llc->ಪೋಷಕ);

	/*
	 * For a single LLC per node, allow an
	 * imbalance up to 12.5% of the node. This is
	 * arbitrary cutoff based two factors -- SMT and
	 * memory channels. For SMT-2, the intent is to
	 * avoid premature sharing of HT resources but
	 * SMT-4 or SMT-8 *may* benefit from a different
	 * cutoff. For memory channels, this is a very
	 * rough estimate of how many channels may be
	 * active and is based on recent CPUs with
	 * many cores.
	 *
	 * For multiple LLCs, allow an imbalance
	 * until multiple tasks would share an LLC
	 * on one node while LLCs on another node
	 * remain idle. This assumes that there are
	 * enough logical CPUs per LLC to avoid SMT
	 * factors and that there is a correlation
	 * between LLCs and memory channels.
	 */
	nr_llcs = sd_llc->ಪೋಷಕ->ಹರವು_ತೂಕ / sd_llc->ಹರವು_ತೂಕ;
	ಆದರೆ (nr_llcs == 1)
		imb = sd_llc->ಪೋಷಕ->ಹರವು_ತೂಕ >> 3;
	ಇಲ್ಲದಿದ್ದರೆ
		imb = nr_llcs;

	imb = max(1U, imb);
	sd_llc->ಪೋಷಕ->imb_numa_nr = imb;

	/*
	 * Set span based on the first NUMA domain.
	 *
	 * NUMA systems always add a NODE domain before
	 * iterating the NUMA domains. Since this is before
	 * degeneration, start from sd_llc's parent's
	 * parent which is the lowest an SD_NUMA domain can
	 * be relative to sd_llc.
	 */
	ಪೋಷಕ = sd_llc->ಪೋಷಕ->ಪೋಷಕ;
	ಆಗ (ಪೋಷಕ && !(ಪೋಷಕ->flags & SD_NUMA))
		ಪೋಷಕ = ಪೋಷಕ->ಪೋಷಕ;

	imb_ಹರವು = ಪೋಷಕ ? ಪೋಷಕ->ಹರವು_ತೂಕ : sd_llc->ಪೋಷಕ->ಹರವು_ತೂಕ;

	/* Update the upper remainder of the topology */
	ಪೋಷಕ = sd_llc->ಪೋಷಕ;
	ಆಗ (ಪೋಷಕ) {
		ಪೂರ್ಣಾಂಕ ಗುಣಕ = max(1U, (ಪೋಷಕ->ಹರವು_ತೂಕ / imb_ಹರವು));

		ಪೋಷಕ->imb_numa_nr = imb * ಗುಣಕ;
		ಪೋಷಕ = ಪೋಷಕ->ಪೋಷಕ;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ
init_sched_domain_shared(ರಚನೆ s_ದತ್ತಾಂಶ *d, ರಚನೆ sched_domain *sd, ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ sched_domain_shared *sds = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ cpu;

	/*
	 * Multiple domains can try to claim a shared object like
	 * SD_ASYM_CPUCAPACITY and SD_SHARE_LLC which can alias to
	 * same cpumask_first(sched_domain_span(sd)) CPU and can
	 * cause "nr_idle_scan" to be populated incorrectly during
	 * load balancing.
	 *
	 * Find the first CPU in sched_domain_span(sd) with an
	 * unclaimed domain (!alloc_flags) or where the alloc_flag
	 * matches the requested flag (SD_* flag)
	 *
	 * If the domain only has single CPU, allow temporary overlap
	 * in allocation since the domains will be degenerated later.
	 */
	for_each_cpu(cpu, sched_domain_ಹರವು(sd)) {
		sds = *ಪ್ರತಿ_cpu_ptr(d->sds, cpu);

		ಆದರೆ (!sds->alloc_flags ||
		    sd->ಹರವು_ತೂಕ == 1 ||
		    sds->alloc_flags == flags) {
			sds->alloc_flags = flags;
			sd->shared = sds;
			ನಿಲ್ಲಿಸು;
		}
	}

	/*
	 * Use the sd_shared corresponding to the last
	 * CPU in the span if none are avaialable.
	 */
	ಆದರೆ (WARN_ON_ONCE(!sd->shared))
		sd->shared = sds;

	/*
	 * nr_busy_cpus is consumed only by the NOHZ kick path via
	 * sd_balance_shared; on the asym-capacity path it is initialized but
	 * never read.
	 */
	atomic_ಗಣ(&sd->shared->nr_busy_cpus, sd->ಹರವು_ತೂಕ);
	atomic_inc(&sd->shared->ref);
}

/*
 * For asymmetric CPU capacity, attach sched_domain_shared on the innermost
 * SD_ASYM_CPUCAPACITY_FULL ancestor of @cpu's base domain when that ancestor is
 * not an overlapping NUMA-built domain (then LLC should claim shared).
 *
 * A CPU may lack any FULL ancestor (e.g., exclusive cpuset symmetric island),
 * then LLC must claim shared instead.
 *
 * Note: SD_ASYM_CPUCAPACITY_FULL is only set when all CPU capacity values
 * are present in the domain span, so the asym domain we attach to cannot
 * degenerate into a single-capacity group. The relevant edge cases are instead
 * covered by the caveats above.
 *
 * Return true if this CPU's asym path claimed sd->shared, false otherwise.
 */
ಸ್ಥಿರ bool claim_asym_sched_domain_shared(ರಚನೆ s_ದತ್ತಾಂಶ *d, ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ sched_domain *sd = *ಪ್ರತಿ_cpu_ptr(d->sd, cpu);
	ರಚನೆ sched_domain *sd_asym;

	ಆದರೆ (!sd)
		ಹಿಂತಿರುಗಿ false;

	sd_asym = sd;
	ಆಗ (sd_asym && !(sd_asym->flags & SD_ASYM_CPUCAPACITY_ತುಂಬಿದ))
		sd_asym = sd_asym->ಪೋಷಕ;

	ಆದರೆ (!sd_asym || (sd_asym->flags & SD_NUMA))
		ಹಿಂತಿರುಗಿ false;

	init_sched_domain_shared(d, sd_asym, SD_ASYM_CPUCAPACITY);
	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __sched_domains_alloc_llc_id(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = cpumask_ಮೊದಲ_zero(sched_domains_llc_id_allocmask);
	/*
	 * llc_id space should never grow larger than the
	 * possible number of CPUs in the system.
	 */
	ಆದರೆ (lid >= nr_cpu_ids)
		ಹಿಂತಿರುಗಿ -1;

	__cpumask_set_cpu(lid, sched_domains_llc_id_allocmask);
	max = cpumask_ಕೊನೆಯ(sched_domains_llc_id_allocmask);
	ಆದರೆ (max > max_lid)
		max_lid = max;

	ಹಿಂತಿರುಗಿ lid;
}

ಸ್ಥಿರ ಶೂನ್ಯ __sched_domains_free_llc_id(ಪೂರ್ಣಾಂಕ cpu)
{
	ಪೂರ್ಣಾಂಕ i, lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = ಪ್ರತಿ_cpu(sd_llc_id, cpu);
	ಆದರೆ (lid == -1 || lid >= nr_cpu_ids)
		ಹಿಂತಿರುಗಿ;

	ಪ್ರತಿ_cpu(sd_llc_id, cpu) = -1;

	for_each_cpu(i, llc_ಮುಖವಾಡ(cpu)) {
		/* An online CPU owns the llc_id. */
		ಆದರೆ (ಪ್ರತಿ_cpu(sd_llc_id, i) == lid)
			ಹಿಂತಿರುಗಿ;
	}

	__cpumask_clear_cpu(lid, sched_domains_llc_id_allocmask);

	max = cpumask_ಕೊನೆಯ(sched_domains_llc_id_allocmask);
	/* shrink max lid to save memory */
	ಆದರೆ (max < max_lid)
		max_lid = max;
}

ಶೂನ್ಯ sched_domains_free_llc_id(ಪೂರ್ಣಾಂಕ cpu)
{
	sched_domains_mutex_ಬೀಗ();
	__sched_domains_free_llc_id(cpu);
	sched_domains_mutex_unlock();
}

/*
 * Build sched domains for a given set of CPUs and attach the sched domains
 * to the individual CPUs
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
build_sched_domains(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_map, ರಚನೆ sched_domain_attr *attr,
		    bool *multi_llcs)
{
	ಎಣಿಕೆ s_alloc alloc_state = sa_none;
	bool has_multi_llcs = false;
	ರಚನೆ sched_domain *sd;
	ರಚನೆ s_ದತ್ತಾಂಶ d;
	ರಚನೆ rq *rq = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ i, ret = -ENOMEM;
	bool has_asym = false;
	bool has_cluster = false;

	ಆದರೆ (WARN_ON(cpumask_ಖಾಲಿಗಳು(cpu_map)))
		ಹೋಗು error;

	alloc_state = __visit_domain_allocation_hell(&d, cpu_map);
	ಆದರೆ (alloc_state != sa_rootdomain)
		ಹೋಗು error;

	/* Set up domains for CPUs specified by the cpu_map: */
	for_each_cpu(i, cpu_map) {
		ರಚನೆ sched_domain_topology_level *tl;
		ಪೂರ್ಣಾಂಕ lid;

		sd = ಖಾಲಿ;
		for_each_sd_topology(tl) {

			sd = build_sched_domain(tl, cpu_map, attr, sd, i);

			has_asym |= sd->flags & SD_ASYM_CPUCAPACITY;

			ಆದರೆ (tl == sched_domain_topology)
				*ಪ್ರತಿ_cpu_ptr(d.sd, i) = sd;
			ಆದರೆ (cpumask_equal(cpu_map, sched_domain_ಹರವು(sd)))
				ನಿಲ್ಲಿಸು;
		}

		lid = ಪ್ರತಿ_cpu(sd_llc_id, i);
		ಆದರೆ (lid == -1) {
			/* try to reuse the llc_id of its siblings */
			ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = cpumask_ಮೊದಲ(llc_ಮುಖವಾಡ(i));
			     j < nr_cpu_ids;
			     j = cpumask_ಮುಂದಿನ(j, llc_ಮುಖವಾಡ(i))) {
				ಆದರೆ (i == j)
					ಮುಂದುವರಿ;

				lid = ಪ್ರತಿ_cpu(sd_llc_id, j);

				ಆದರೆ (lid != -1) {
					ಪ್ರತಿ_cpu(sd_llc_id, i) = lid;

					ನಿಲ್ಲಿಸು;
				}
			}

			/* a new LLC is detected */
			ಆದರೆ (lid == -1)
				ಪ್ರತಿ_cpu(sd_llc_id, i) = __sched_domains_alloc_llc_id();
		}
	}

	ಆದರೆ (WARN_ON(!topology_ಹರವು_sane(cpu_map)))
		ಹೋಗು error;

	/* Build the groups for the domains */
	for_each_cpu(i, cpu_map) {
		ಗಾಗಿ (sd = *ಪ್ರತಿ_cpu_ptr(d.sd, i); sd; sd = sd->ಪೋಷಕ) {
			sd->ಹರವು_ತೂಕ = cpumask_ತೂಕ(sched_domain_ಹರವು(sd));
			ಆದರೆ (sd->flags & SD_NUMA) {
				ಆದರೆ (build_overlap_sched_groups(sd, i))
					ಹೋಗು error;
			} ಇಲ್ಲದಿದ್ದರೆ {
				ಆದರೆ (build_sched_groups(sd, i))
					ಹೋಗು error;
			}
		}
	}

	for_each_cpu(i, cpu_map) {
		sd = *ಪ್ರತಿ_cpu_ptr(d.sd, i);
		ಆದರೆ (!sd)
			ಮುಂದುವರಿ;

		ಆದರೆ (has_asym)
			claim_asym_sched_domain_shared(&d, i);

		/* First, find the topmost SD_SHARE_LLC domain */
		ಆಗ (sd->ಪೋಷಕ && (sd->ಪೋಷಕ->flags & SD_SHARE_LLC))
			sd = sd->ಪೋಷಕ;

		ಆದರೆ (sd->flags & SD_SHARE_LLC) {
			init_sched_domain_shared(&d, sd, SD_SHARE_LLC);

			/*
			 * In presence of higher domains, adjust the
			 * NUMA imbalance stats for the hierarchy.
			 */
			ಆದರೆ (sd->ಪೋಷಕ) {
				ಆದರೆ (IS_ENABLED(CONFIG_NUMA))
					adjust_numa_imbalance(sd);

				ಆದರೆ (sd_ಒಳ_multi_llcs(sd))
					has_multi_llcs = true;
			}
		}
	}

	/* Calculate CPU capacity for physical packages and nodes */
	ಗಾಗಿ (i = nr_cpumask_ಬಿಟ್ಟುಗಳು-1; i >= 0; i--) {
		ಆದರೆ (!cpumask_test_cpu(i, cpu_map))
			ಮುಂದುವರಿ;

		claim_allocations(i, &d);

		ಗಾಗಿ (sd = *ಪ್ರತಿ_cpu_ptr(d.sd, i); sd; sd = sd->ಪೋಷಕ)
			init_sched_groups_ಸಾಮರ್ಥ್ಯ(i, sd);
	}

	alloc_sd_llc(cpu_map, &d);

	/* Attach the domains */
	rcu_read_ಬೀಗ();
	for_each_cpu(i, cpu_map) {
		rq = cpu_rq(i);
		sd = *ಪ್ರತಿ_cpu_ptr(d.sd, i);

		cpu_attach_domain(sd, d.rd, i);

		ಆದರೆ (ಅತಿಕಡಿಮೆ_flag_domain(i, SD_CLUSTER))
			has_cluster = true;
	}
	rcu_read_unlock();

	ಆದರೆ (has_asym)
		static_branch_inc_cpuslocked(&sched_asym_cpucapacity);

	ಆದರೆ (has_cluster)
		static_branch_inc_cpuslocked(&sched_cluster_ಸಕ್ರಿಯ);

	ಆದರೆ (rq && sched_debug_verbose)
		pr_info("root domain span: %*pbl\n", cpumask_pr_ವಾದಗಳು(cpu_map));

	ret = 0;
error:
	*multi_llcs = has_multi_llcs;
	__free_domain_allocs(&d, alloc_state, cpu_map);

	ಹಿಂತಿರುಗಿ ret;
}

/* Current sched domains: */
ಸ್ಥಿರ cpumask_var_t			*doms_cur;

/* Number of sched domains in 'doms_cur': */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ				ndoms_cur;

/* Attributes of custom domains in 'doms_cur' */
ಸ್ಥಿರ ರಚನೆ sched_domain_attr		*dattr_cur;

/*
 * Special case: If a kmalloc() of a doms_cur partition (array of
 * cpumask) fails, then fallback to a single sched domain,
 * as determined by the single cpumask fallback_doms.
 */
ಸ್ಥಿರ cpumask_var_t			fallback_doms;

/*
 * arch_update_cpu_topology lets virtualized architectures update the
 * CPU core maps. It is supposed to return 1 if the topology changed
 * or 0 if it stayed the same.
 */
ಪೂರ್ಣಾಂಕ __weak arch_update_cpu_topology(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ 0;
}

cpumask_var_t *alloc_sched_domains(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ndoms)
{
	ಪೂರ್ಣಾಂಕ i;
	cpumask_var_t *doms;

	doms = kmalloc_objs(*doms, ndoms);
	ಆದರೆ (!doms)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	ಗಾಗಿ (i = 0; i < ndoms; i++) {
		ಆದರೆ (!alloc_cpumask_var(&doms[i], GFP_KERNEL)) {
			free_sched_domains(doms, i);
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;
		}
	}
	ಹಿಂತಿರುಗಿ doms;
}

ಶೂನ್ಯ free_sched_domains(cpumask_var_t doms[], ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ndoms)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i;
	ಗಾಗಿ (i = 0; i < ndoms; i++)
		free_cpumask_var(doms[i]);
	kfree(doms);
}

/*
 * Set up scheduler domains and groups.  For now this just excludes isolated
 * CPUs, but could be used to exclude other special cases in the future.
 */
ಪೂರ್ಣಾಂಕ __init sched_init_domains(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_map)
{
	bool multi_llcs;
	ಪೂರ್ಣಾಂಕ err;

	zalloc_cpumask_var(&sched_domains_llc_id_allocmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask2, GFP_KERNEL);
	zalloc_cpumask_var(&fallback_doms, GFP_KERNEL);

	arch_update_cpu_topology();
	asym_cpu_ಸಾಮರ್ಥ್ಯ_ಪರಿಶೀಲನೆ();
	ndoms_cur = 1;
	doms_cur = alloc_sched_domains(ndoms_cur);
	ಆದರೆ (!doms_cur)
		doms_cur = &fallback_doms;
	cpumask_and(doms_cur[0], cpu_map, housekeeping_cpumask(HK_TYPE_DOMAIN));
	err = build_sched_domains(doms_cur[0], ಖಾಲಿ, &multi_llcs);
	ಆದರೆ (!err)
		sched_cache_ಗಣ(multi_llcs);

	ಹಿಂತಿರುಗಿ err;
}

/*
 * Detach sched domains from a group of CPUs specified in cpu_map
 * These CPUs will now be attached to the NULL domain
 */
ಸ್ಥಿರ ಶೂನ್ಯ detach_destroy_domains(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_map)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu = cpumask_any(cpu_map);
	ಪೂರ್ಣಾಂಕ i;

	ಆದರೆ (rcu_access_ಸೂಚಕ(ಪ್ರತಿ_cpu(sd_asym_cpucapacity, cpu)))
		static_branch_dec_cpuslocked(&sched_asym_cpucapacity);

	ಆದರೆ (static_branch_unlikely(&sched_cluster_ಸಕ್ರಿಯ))
		static_branch_dec_cpuslocked(&sched_cluster_ಸಕ್ರಿಯ);

	rcu_read_ಬೀಗ();
	for_each_cpu(i, cpu_map)
		cpu_attach_domain(ಖಾಲಿ, &def_ಬೇರು_domain, i);
	rcu_read_unlock();
}

/* handle null as "default" */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ dattrs_equal(ರಚನೆ sched_domain_attr *cur, ಪೂರ್ಣಾಂಕ idx_cur,
			ರಚನೆ sched_domain_attr *new, ಪೂರ್ಣಾಂಕ idx_new)
{
	ರಚನೆ sched_domain_attr tmp;

	/* Fast path: */
	ಆದರೆ (!new && !cur)
		ಹಿಂತಿರುಗಿ 1;

	tmp = SD_ATTR_INIT;

	ಹಿಂತಿರುಗಿ !memcmp(cur ? (cur + idx_cur) : &tmp,
			new ? (new + idx_new) : &tmp,
			ಗಾತ್ರ(ರಚನೆ sched_domain_attr));
}

/*
 * Partition sched domains as specified by the 'ndoms_new'
 * cpumasks in the array doms_new[] of cpumasks. This compares
 * doms_new[] to the current sched domain partitioning, doms_cur[].
 * It destroys each deleted domain and builds each new domain.
 *
 * 'doms_new' is an array of cpumask_var_t's of length 'ndoms_new'.
 * The masks don't intersect (don't overlap.) We should setup one
 * sched domain for each mask. CPUs not in any of the cpumasks will
 * not be load balanced. If the same cpumask appears both in the
 * current 'doms_cur' domains and in the new 'doms_new', we can leave
 * it as it is.
 *
 * The passed in 'doms_new' should be allocated using
 * alloc_sched_domains.  This routine takes ownership of it and will
 * free_sched_domains it when done with it. If the caller failed the
 * alloc call, then it can pass in doms_new == NULL && ndoms_new == 1,
 * and partition_sched_domains() will fallback to the single partition
 * 'fallback_doms', it also forces the domains to be rebuilt.
 *
 * If doms_new == NULL it will be replaced with cpu_online_mask.
 * ndoms_new == 0 is a special case for destroying existing domains,
 * and it will not create the default domain.
 *
 * Call with hotplug lock and sched_domains_mutex held
 */
ಸ್ಥಿರ ಶೂನ್ಯ ವಿಭಜನೆ_sched_domains_locked(ಪೂರ್ಣಾಂಕ ndoms_new, cpumask_var_t doms_new[],
				    ರಚನೆ sched_domain_attr *dattr_new)
{
	bool __maybe_unused has_eas = false;
	bool has_multi_llcs = false, multi_llcs;
	ಪೂರ್ಣಾಂಕ i, j, n;
	ಪೂರ್ಣಾಂಕ new_topology;

	lockdep_assert_held(&sched_domains_mutex);

	/* Let the architecture update CPU core mappings: */
	new_topology = arch_update_cpu_topology();
	/* Trigger rebuilding CPU capacity asymmetry data */
	ಆದರೆ (new_topology)
		asym_cpu_ಸಾಮರ್ಥ್ಯ_ಪರಿಶೀಲನೆ();

	ಆದರೆ (!doms_new) {
		WARN_ON_ONCE(dattr_new);
		n = 0;
		doms_new = alloc_sched_domains(1);
		ಆದರೆ (doms_new) {
			n = 1;
			cpumask_and(doms_new[0], cpu_ಸಕ್ರಿಯ_ಮುಖವಾಡ,
				    housekeeping_cpumask(HK_TYPE_DOMAIN));
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		n = ndoms_new;
	}

	/* Destroy deleted domains: */
	ಗಾಗಿ (i = 0; i < ndoms_cur; i++) {
		ಗಾಗಿ (j = 0; j < n && !new_topology; j++) {
			ಆದರೆ (cpumask_equal(doms_cur[i], doms_new[j]) &&
			    dattrs_equal(dattr_cur, i, dattr_new, j))
				ಹೋಗು match1;
		}
		/* No match - a current sched domain not in new doms_new[] */
		detach_destroy_domains(doms_cur[i]);
match1:
		;
	}

	n = ndoms_cur;
	ಆದರೆ (!doms_new) {
		n = 0;
		doms_new = &fallback_doms;
		cpumask_and(doms_new[0], cpu_ಸಕ್ರಿಯ_ಮುಖವಾಡ,
			    housekeeping_cpumask(HK_TYPE_DOMAIN));
	}

	/* Build new domains: */
	ಗಾಗಿ (i = 0; i < ndoms_new; i++) {
		ಗಾಗಿ (j = 0; j < n && !new_topology; j++) {
			ಆದರೆ (cpumask_equal(doms_new[i], doms_cur[j]) &&
			    dattrs_equal(dattr_new, i, dattr_cur, j)) {
				/*
				 * Reused partition has to be taken care
				 * of here, because there could be a corner
				 * case that if the reused partition is skipped
				 * and only new partition is considered, an
				 * incorrect has_multi_llcs would be set. For
				 * example:
				 * If the only multi-LLC partition is reused
				 * and a new single-LLC partition is built,
				 * sched_cache_set(false) disables cache-aware
				 * scheduling globally despite the reused
				 * multi-LLC partition still being active.
				 */
				ರಚನೆ sched_domain *sd;
				ಪೂರ್ಣಾಂಕ cpu = cpumask_ಮೊದಲ(doms_cur[j]);

				guard(rcu)();
				sd = rcu_dereference(cpu_rq(cpu)->sd);
				ಆಗ (sd && sd->ಪೋಷಕ && (sd->ಪೋಷಕ->flags & SD_SHARE_LLC))
					sd = sd->ಪೋಷಕ;
				ಆದರೆ (sd && (sd->flags & SD_SHARE_LLC) && sd->ಪೋಷಕ &&
				    sd_ಒಳ_multi_llcs(sd))
					has_multi_llcs = true;
				ಹೋಗು match2;
			}
		}
		/* No match - add a new doms_new */
		build_sched_domains(doms_new[i], dattr_new ? dattr_new + i : ಖಾಲಿ,
				    &multi_llcs);
		has_multi_llcs |= multi_llcs;
match2:
		;
	}
	sched_cache_ಗಣ(has_multi_llcs);

#ಆದರೆ defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
	/* Build perf domains: */
	ಗಾಗಿ (i = 0; i < ndoms_new; i++) {
		ಗಾಗಿ (j = 0; j < n && !sched_energy_update; j++) {
			ಆದರೆ (cpumask_equal(doms_new[i], doms_cur[j]) &&
			    cpu_rq(cpumask_ಮೊದಲ(doms_cur[j]))->rd->pd) {
				has_eas = true;
				ಹೋಗು match3;
			}
		}
		/* No match - add perf domains for a new rd */
		has_eas |= build_perf_domains(doms_new[i]);
match3:
		;
	}
	sched_energy_ಗಣ(has_eas);
#ಆದರೆ_ಅಂತ್ಯ

	/* Remember the new sched domains: */
	ಆದರೆ (doms_cur != &fallback_doms)
		free_sched_domains(doms_cur, ndoms_cur);

	kfree(dattr_cur);
	doms_cur = doms_new;
	dattr_cur = dattr_new;
	ndoms_cur = ndoms_new;

	update_sched_domain_debugfs();
	dl_rebuild_rd_accounting();
}

/*
 * Call with hotplug lock held
 */
ಶೂನ್ಯ ವಿಭಜನೆ_sched_domains(ಪೂರ್ಣಾಂಕ ndoms_new, cpumask_var_t doms_new[],
			     ರಚನೆ sched_domain_attr *dattr_new)
{
	sched_domains_mutex_ಬೀಗ();
	ವಿಭಜನೆ_sched_domains_locked(ndoms_new, doms_new, dattr_new);
	sched_domains_mutex_unlock();
}
