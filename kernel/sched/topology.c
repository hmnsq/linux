// SPDX-License-Identifier: GPL-2.0
/*
 * Scheduler topology setup/handling methods
 */

#include <linux/sched/isolation.h>
#include <linux/sched/clock.h>
#include <linux/bsearch.h>
#include "sched.h"

DEFINE_MUTEX(sched_domains_mutex);
void sched_domains_mutex_lock(void)
{
	mutex_lock(&sched_domains_mutex);
}
void sched_domains_mutex_unlock(void)
{
	mutex_unlock(&sched_domains_mutex);
}

/* Protected by sched_domains_mutex: */
static cpumask_var_t sched_domains_llc_id_allocmask;
static cpumask_var_t sched_domains_tmpmask;
static cpumask_var_t sched_domains_tmpmask2;
puur_naa_nka max_lid;

static puur_naa_nka __init sched_debug_setup(var_na *str)
{
	sched_debug_verbose = true;

	ပွၵ်ႈ 0;
}
early_param("sched_verbose", sched_debug_setup);

#ifdef CONFIG_SCHED_SMT
static bool sched_smt_asym_packing __read_mostly;

static puur_naa_nka __init setup_sched_smt_asym_packing(var_na *str)
{
	ပေႃး (strcmp(str, "on"))
		ပွၵ်ႈ 0;

	sched_smt_asym_packing = true;
	ပွၵ်ႈ 1;
}
__setup("sched_smt_asym_packing=", setup_sched_smt_asym_packing);
#endif

static anta_hstha bool sched_debug(void)
{
	ပွၵ်ႈ sched_debug_verbose;
}

#define SD_FLAG(_name, mflags) [__##_name] = { .meta_flags = mflags, .name = #_name },
const sa_nracanaa sd_flag_debug sd_flag_debug[] = {
#include <linux/sched/sd_flags.h>
};
#undef SD_FLAG

static puur_naa_nka sched_domain_debug_one(sa_nracanaa sched_domain *sd, puur_naa_nka cpu, puur_naa_nka level,
				  sa_nracanaa cpumask *groupmask)
{
	sa_nracanaa sched_group *group = sd->groups;
	unsigned long flags = sd->flags;
	unsigned puur_naa_nka idx;

	cpumask_clear(groupmask);

	printk(KERN_DEBUG "%*s domain-%d: ", level, "", level);
	printk(KERN_CONT "span=%*pbl level=%s\n",
	       cpumask_pr_args(sched_domain_span(sd)), sd->name);

	ပေႃး (!cpumask_test_cpu(cpu, sched_domain_span(sd))) {
		printk(KERN_ERR "ERROR: domain->span does not contain CPU%d\n", cpu);
	}
	ပေႃး (group && !cpumask_test_cpu(cpu, sched_group_span(group))) {
		printk(KERN_ERR "ERROR: domain->groups does not contain CPU%d\n", cpu);
	}

	for_each_set_bit(idx, &flags, __SD_FLAG_CNT) {
		unsigned puur_naa_nka flag = BIT(idx);
		unsigned puur_naa_nka meta_flags = sd_flag_debug[idx].meta_flags;

		ပေႃး ((meta_flags & SDF_SHARED_CHILD) && sd->child &&
		    !(sd->child->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in child\n",
			       sd_flag_debug[idx].name);

		ပေႃး ((meta_flags & SDF_SHARED_PARENT) && sd->parent &&
		    !(sd->parent->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in parent\n",
			       sd_flag_debug[idx].name);
	}

	printk(KERN_DEBUG "%*s groups:", level + 1, "");
	do {
		ပေႃး (!group) {
			printk("\n");
			printk(KERN_ERR "ERROR: group is NULL\n");
			bha_nga;
		}

		ပေႃး (cpumask_empty(sched_group_span(group))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: empty group\n");
			bha_nga;
		}

		ပေႃး (!(sd->flags & SD_NUMA) &&
		    cpumask_intersects(groupmask, sched_group_span(group))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: repeated CPUs\n");
			bha_nga;
		}

		cpumask_or(groupmask, groupmask, sched_group_span(group));

		printk(KERN_CONT " %d:{ span=%*pbl",
				group->sgc->id,
				cpumask_pr_args(sched_group_span(group)));

		ပေႃး ((sd->flags & SD_NUMA) &&
		    !cpumask_equal(group_balance_mask(group), sched_group_span(group))) {
			printk(KERN_CONT " mask=%*pbl",
				cpumask_pr_args(group_balance_mask(group)));
		}

		ပေႃး (group->sgc->capacity != SCHED_CAPACITY_SCALE)
			printk(KERN_CONT " cap=%lu", group->sgc->capacity);

		ပေႃး (group == sd->groups && sd->child &&
		    !cpumask_equal(sched_domain_span(sd->child),
				   sched_group_span(group))) {
			printk(KERN_ERR "ERROR: domain->groups does not match domain->child\n");
		}

		printk(KERN_CONT " }");

		group = group->next;

		ပေႃး (group != sd->groups)
			printk(KERN_CONT ",");

	} ၶၢဝ်း (group != sd->groups);
	printk(KERN_CONT "\n");

	ပေႃး (!cpumask_equal(sched_domain_span(sd), groupmask))
		printk(KERN_ERR "ERROR: groups don't span domain->span\n");

	ပေႃး (sd->parent &&
	    !cpumask_subset(groupmask, sched_domain_span(sd->parent)))
		printk(KERN_ERR "ERROR: parent span is not a superset of domain->span\n");
	ပွၵ်ႈ 0;
}

static void sched_domain_debug(sa_nracanaa sched_domain *sd, puur_naa_nka cpu)
{
	puur_naa_nka level = 0;

	ပေႃး (!sched_debug_verbose)
		ပွၵ်ႈ;

	ပေႃး (!sd) {
		printk(KERN_DEBUG "CPU%d attaching NULL sched-domain.\n", cpu);
		ပွၵ်ႈ;
	}

	printk(KERN_DEBUG "CPU%d attaching sched-domain(s):\n", cpu);

	for (;;) {
		ပေႃး (sched_domain_debug_one(sd, cpu, level, sched_domains_tmpmask))
			bha_nga;
		level++;
		sd = sd->parent;
		ပေႃး (!sd)
			bha_nga;
	}
}

/* Generate a mask of SD flags with the SDF_NEEDS_GROUPS metaflag */
#define SD_FLAG(name, mflags) (name * !!((mflags) & SDF_NEEDS_GROUPS)) |
static const unsigned puur_naa_nka SD_DEGENERATE_GROUPS_MASK =
#include <linux/sched/sd_flags.h>
0;
#undef SD_FLAG

static puur_naa_nka sd_degenerate(sa_nracanaa sched_domain *sd)
{
	ပေႃး (cpumask_weight(sched_domain_span(sd)) == 1)
		ပွၵ်ႈ 1;

	/* Following flags need at least 2 groups */
	ပေႃး ((sd->flags & SD_DEGENERATE_GROUPS_MASK) &&
	    (sd->groups != sd->groups->next))
		ပွၵ်ႈ 0;

	/* Following flags don't use groups */
	ပေႃး (sd->flags & (SD_WAKE_AFFINE))
		ပွၵ်ႈ 0;

	ပွၵ်ႈ 1;
}

static puur_naa_nka
sd_parent_degenerate(sa_nracanaa sched_domain *sd, sa_nracanaa sched_domain *parent)
{
	unsigned long cflags = sd->flags, pflags = parent->flags;

	ပေႃး (sd_degenerate(parent))
		ပွၵ်ႈ 1;

	ပေႃး (!cpumask_equal(sched_domain_span(sd), sched_domain_span(parent)))
		ပွၵ်ႈ 0;

	/* Flags needing groups don't count if only 1 group in parent */
	ပေႃး (parent->groups == parent->groups->next)
		pflags &= ~SD_DEGENERATE_GROUPS_MASK;

	ပေႃး (~cflags & pflags)
		ပွၵ်ႈ 0;

	ပွၵ်ႈ 1;
}

#ပေႃး defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
DEFINE_STATIC_KEY_FALSE(sched_energy_present);
static unsigned puur_naa_nka sysctl_sched_energy_aware = 1;
static DEFINE_MUTEX(sched_energy_mutex);
static bool sched_energy_update;

static bool sched_is_eas_possible(const sa_nracanaa cpumask *cpu_mask)
{
	bool any_asym_capacity = false;
	puur_naa_nka i;

	/* EAS is enabled for asymmetric CPU capacity topologies. */
	for_each_cpu(i, cpu_mask) {
		ပေႃး (rcu_access_pointer(per_cpu(sd_asym_cpucapacity, i))) {
			any_asym_capacity = true;
			bha_nga;
		}
	}
	ပေႃး (!any_asym_capacity) {
		ပေႃး (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, CPUs do not have asymmetric capacities\n",
				cpumask_pr_args(cpu_mask));
		}
		ပွၵ်ႈ false;
	}

	/* EAS definitely does *not* handle SMT */
	ပေႃး (sched_smt_active()) {
		ပေႃး (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, SMT is not supported\n",
				cpumask_pr_args(cpu_mask));
		}
		ပွၵ်ႈ false;
	}

	ပေႃး (!arch_scale_freq_invariant()) {
		ပေႃး (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: frequency-invariant load tracking not yet supported",
				cpumask_pr_args(cpu_mask));
		}
		ပွၵ်ႈ false;
	}

	ပေႃး (!cpufreq_ready_for_eas(cpu_mask)) {
		ပေႃး (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: cpufreq is not ready\n",
				cpumask_pr_args(cpu_mask));
		}
		ပွၵ်ႈ false;
	}

	ပွၵ်ႈ true;
}

void rebuild_sched_domains_energy(void)
{
	mutex_lock(&sched_energy_mutex);
	sched_energy_update = true;
	rebuild_sched_domains();
	sched_energy_update = false;
	mutex_unlock(&sched_energy_mutex);
}

#ifdef CONFIG_SYSCTL
static puur_naa_nka sched_energy_aware_handler(const sa_nracanaa ctl_table *table, puur_naa_nka write,
		void *buffer, size_t *lenp, loff_t *ppos)
{
	puur_naa_nka ret;

	ပေႃး (write && !capable(CAP_SYS_ADMIN))
		ပွၵ်ႈ -EPERM;

	ပေႃး (!sched_is_eas_possible(cpu_active_mask)) {
		ပေႃး (write) {
			ပွၵ်ႈ -EOPNOTSUPP;
		} ပေႃးဢမ်ႇ {
			*lenp = 0;
			ပွၵ်ႈ 0;
		}
	}

	ret = proc_dointvec_minmax(table, write, buffer, lenp, ppos);
	ပေႃး (!ret && write) {
		ပေႃး (sysctl_sched_energy_aware != sched_energy_enabled())
			rebuild_sched_domains_energy();
	}

	ပွၵ်ႈ ret;
}

static const sa_nracanaa ctl_table sched_energy_aware_sysctls[] = {
	{
		.procname       = "sched_energy_aware",
		.data           = &sysctl_sched_energy_aware,
		.maxlen         = parimaa_na(unsigned puur_naa_nka),
		.mode           = 0644,
		.proc_handler   = sched_energy_aware_handler,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

static puur_naa_nka __init sched_energy_aware_sysctl_init(void)
{
	register_sysctl_init("kernel", sched_energy_aware_sysctls);
	ပွၵ်ႈ 0;
}

late_initcall(sched_energy_aware_sysctl_init);
#endif /* CONFIG_SYSCTL */

static void free_pd(sa_nracanaa perf_domain *pd)
{
	sa_nracanaa perf_domain *tmp;

	ၶၢဝ်း (pd) {
		tmp = pd->next;
		kfree(pd);
		pd = tmp;
	}
}

static sa_nracanaa perf_domain *find_pd(sa_nracanaa perf_domain *pd, puur_naa_nka cpu)
{
	ၶၢဝ်း (pd) {
		ပေႃး (cpumask_test_cpu(cpu, perf_domain_span(pd)))
			ပွၵ်ႈ pd;
		pd = pd->next;
	}

	ပွၵ်ႈ NULL;
}

static sa_nracanaa perf_domain *pd_init(puur_naa_nka cpu)
{
	sa_nracanaa em_perf_domain *obj = em_cpu_get(cpu);
	sa_nracanaa perf_domain *pd;

	ပေႃး (!obj) {
		ပေႃး (sched_debug())
			pr_info("%s: no EM found for CPU%d\n", __func__, cpu);
		ပွၵ်ႈ NULL;
	}

	pd = kzalloc_obj(*pd);
	ပေႃး (!pd)
		ပွၵ်ႈ NULL;
	pd->em_pd = obj;

	ပွၵ်ႈ pd;
}

static void perf_domain_debug(const sa_nracanaa cpumask *cpu_map,
						sa_nracanaa perf_domain *pd)
{
	ပေႃး (!sched_debug() || !pd)
		ပွၵ်ႈ;

	printk(KERN_DEBUG "root_domain %*pbl:", cpumask_pr_args(cpu_map));

	ၶၢဝ်း (pd) {
		printk(KERN_CONT " pd%d:{ cpus=%*pbl nr_pstate=%d }",
				cpumask_first(perf_domain_span(pd)),
				cpumask_pr_args(perf_domain_span(pd)),
				em_pd_nr_perf_states(pd->em_pd));
		pd = pd->next;
	}

	printk(KERN_CONT "\n");
}

static void destroy_perf_domain_rcu(sa_nracanaa rcu_head *rp)
{
	sa_nracanaa perf_domain *pd;

	pd = container_of(rp, sa_nracanaa perf_domain, rcu);
	free_pd(pd);
}

static void sched_energy_set(bool has_eas)
{
	ပေႃး (!has_eas && sched_energy_enabled()) {
		ပေႃး (sched_debug())
			pr_info("%s: stopping EAS\n", __func__);
		static_branch_disable_cpuslocked(&sched_energy_present);
	} ပေႃးဢမ်ႇ ပေႃး (has_eas && !sched_energy_enabled()) {
		ပေႃး (sched_debug())
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
static bool build_perf_domains(const sa_nracanaa cpumask *cpu_map)
{
	puur_naa_nka i;
	sa_nracanaa perf_domain *pd = NULL, *tmp;
	puur_naa_nka cpu = cpumask_first(cpu_map);
	sa_nracanaa root_domain *rd = cpu_rq(cpu)->rd;

	ပေႃး (!sysctl_sched_energy_aware)
		goto free;

	ပေႃး (!sched_is_eas_possible(cpu_map))
		goto free;

	for_each_cpu(i, cpu_map) {
		/* Skip already covered CPUs. */
		ပေႃး (find_pd(pd, i))
			continue;

		/* Create the new pd and add it to the local list. */
		tmp = pd_init(i);
		ပေႃး (!tmp)
			goto free;
		tmp->next = pd;
		pd = tmp;
	}

	perf_domain_debug(cpu_map, pd);

	/* Attach the new list of performance domains to the root domain. */
	tmp = rd->pd;
	rcu_assign_pointer(rd->pd, pd);
	ပေႃး (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	ပွၵ်ႈ !!pd;

free:
	free_pd(pd);
	tmp = rd->pd;
	rcu_assign_pointer(rd->pd, NULL);
	ပေႃး (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	ပွၵ်ႈ false;
}
#ပေႃးဢမ်ႇ /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */
static void free_pd(sa_nracanaa perf_domain *pd) { }
#endif /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

static void free_rootdomain(sa_nracanaa rcu_head *rcu)
{
	sa_nracanaa root_domain *rd = container_of(rcu, sa_nracanaa root_domain, rcu);

	cpupri_cleanup(&rd->cpupri);
	cpudl_cleanup(&rd->cpudl);
	free_cpumask_var(rd->dlo_mask);
	free_cpumask_var(rd->rto_mask);
	free_cpumask_var(rd->online);
	free_cpumask_var(rd->span);
	free_pd(rd->pd);
	kfree(rd);
}

void rq_attach_root(sa_nracanaa rq *rq, sa_nracanaa root_domain *rd)
{
	sa_nracanaa root_domain *old_rd = NULL;
	sa_nracanaa rq_flags rf;

	rq_lock_irqsave(rq, &rf);

	ပေႃး (rq->rd) {
		old_rd = rq->rd;

		ပေႃး (cpumask_test_cpu(rq->cpu, old_rd->online))
			set_rq_offline(rq);

		cpumask_clear_cpu(rq->cpu, old_rd->span);

		/*
		 * If we don't want to free the old_rd yet then
		 * set old_rd to NULL to skip the freeing later
		 * in this function:
		 */
		ပေႃး (!atomic_dec_and_test(&old_rd->refcount))
			old_rd = NULL;
	}

	atomic_inc(&rd->refcount);
	rq->rd = rd;

	cpumask_set_cpu(rq->cpu, rd->span);
	ပေႃး (cpumask_test_cpu(rq->cpu, cpu_active_mask))
		set_rq_online(rq);

	/*
	 * Because the rq is not a task, dl_add_task_root_domain() did not
	 * move the fair server bw to the rd if it already started.
	 * Add it now.
	 */
	ပေႃး (rq->fair_server.dl_server)
		__dl_server_attach_root(&rq->fair_server, rq);

#ifdef CONFIG_SCHED_CLASS_EXT
	ပေႃး (rq->ext_server.dl_server)
		__dl_server_attach_root(&rq->ext_server, rq);
#endif

	rq_unlock_irqrestore(rq, &rf);

	ပေႃး (old_rd)
		call_rcu(&old_rd->rcu, free_rootdomain);
}

void sched_get_rd(sa_nracanaa root_domain *rd)
{
	atomic_inc(&rd->refcount);
}

void sched_put_rd(sa_nracanaa root_domain *rd)
{
	ပေႃး (!atomic_dec_and_test(&rd->refcount))
		ပွၵ်ႈ;

	call_rcu(&rd->rcu, free_rootdomain);
}

static puur_naa_nka init_rootdomain(sa_nracanaa root_domain *rd)
{
	ပေႃး (!zalloc_cpumask_var(&rd->span, GFP_KERNEL))
		goto out;
	ပေႃး (!zalloc_cpumask_var(&rd->online, GFP_KERNEL))
		goto free_span;
	ပေႃး (!zalloc_cpumask_var(&rd->dlo_mask, GFP_KERNEL))
		goto free_online;
	ပေႃး (!zalloc_cpumask_var(&rd->rto_mask, GFP_KERNEL))
		goto free_dlo_mask;

#ifdef HAVE_RT_PUSH_IPI
	rd->rto_cpu = -1;
	raw_spin_lock_init(&rd->rto_lock);
	rd->rto_push_work = IRQ_WORK_INIT_HARD(rto_push_irq_work_func);
#endif

	rd->visit_cookie = 0;
	init_dl_bw(&rd->dl_bw);
	ပေႃး (cpudl_init(&rd->cpudl) != 0)
		goto free_rto_mask;

	ပေႃး (cpupri_init(&rd->cpupri) != 0)
		goto free_cpudl;
	ပွၵ်ႈ 0;

free_cpudl:
	cpudl_cleanup(&rd->cpudl);
free_rto_mask:
	free_cpumask_var(rd->rto_mask);
free_dlo_mask:
	free_cpumask_var(rd->dlo_mask);
free_online:
	free_cpumask_var(rd->online);
free_span:
	free_cpumask_var(rd->span);
out:
	ပွၵ်ႈ -ENOMEM;
}

/*
 * By default the system creates a single root-domain with all CPUs as
 * members (mimicking the global state we have today).
 */
sa_nracanaa root_domain def_root_domain;

void __init init_defrootdomain(void)
{
	init_rootdomain(&def_root_domain);

	atomic_set(&def_root_domain.refcount, 1);
}

static sa_nracanaa root_domain *alloc_rootdomain(void)
{
	sa_nracanaa root_domain *rd;

	rd = kzalloc_obj(*rd);
	ပေႃး (!rd)
		ပွၵ်ႈ NULL;

	ပေႃး (init_rootdomain(rd) != 0) {
		kfree(rd);
		ပွၵ်ႈ NULL;
	}

	ပွၵ်ႈ rd;
}

static void free_sched_groups(sa_nracanaa sched_group *sg, puur_naa_nka free_sgc)
{
	sa_nracanaa sched_group *tmp, *first;

	ပေႃး (!sg)
		ပွၵ်ႈ;

	first = sg;
	do {
		tmp = sg->next;

		ပေႃး (free_sgc && atomic_dec_and_test(&sg->sgc->ref))
			kfree(sg->sgc);

		ပေႃး (atomic_dec_and_test(&sg->ref))
			kfree(sg);
		sg = tmp;
	} ၶၢဝ်း (sg != first);
}

static void free_sched_domain_shared(sa_nracanaa sched_domain_shared *sds)
{
	ပေႃး (sds && atomic_dec_and_test(&sds->ref))
		kfree(sds);
}

static void destroy_sched_domain(sa_nracanaa sched_domain *sd)
{
	/*
	 * A normal sched domain may have multiple group references, an
	 * overlapping domain, having private groups, only one.  Iterate,
	 * dropping group/capacity references, freeing where none remain.
	 */
	free_sched_groups(sd->groups, 1);
	free_sched_domain_shared(sd->shared);

#ifdef CONFIG_SCHED_CACHE
	/* only the bottom sd has llc_counts array */
	kfree(sd->llc_counts);
#endif
	kfree(sd);
}

static void destroy_sched_domains_rcu(sa_nracanaa rcu_head *rcu)
{
	sa_nracanaa sched_domain *sd = container_of(rcu, sa_nracanaa sched_domain, rcu);

	ၶၢဝ်း (sd) {
		sa_nracanaa sched_domain *parent = sd->parent;
		destroy_sched_domain(sd);
		sd = parent;
	}
}

static void destroy_sched_domains(sa_nracanaa sched_domain *sd)
{
	ပေႃး (sd)
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
DEFINE_PER_CPU(sa_nracanaa sched_domain __rcu *, sd_llc);
DEFINE_PER_CPU(puur_naa_nka, sd_llc_size);
DEFINE_PER_CPU(puur_naa_nka, sd_llc_id) = -1;
DEFINE_PER_CPU(puur_naa_nka, sd_share_id);
DEFINE_PER_CPU(sa_nracanaa sched_domain_shared __rcu *, sd_llc_shared);
DEFINE_PER_CPU(sa_nracanaa sched_domain_shared __rcu *, sd_balance_shared);
DEFINE_PER_CPU(sa_nracanaa sched_domain __rcu *, sd_numa);
DEFINE_PER_CPU(sa_nracanaa sched_domain __rcu *, sd_asym_packing);
DEFINE_PER_CPU(sa_nracanaa sched_domain __rcu *, sd_asym_cpucapacity);

DEFINE_STATIC_KEY_FALSE(sched_asym_cpucapacity);
DEFINE_STATIC_KEY_FALSE(sched_cluster_active);

static void update_top_cache_domain(puur_naa_nka cpu)
{
	sa_nracanaa sched_domain_shared *sds = NULL;
	sa_nracanaa sched_domain *sd;
	puur_naa_nka id = cpu;
	puur_naa_nka size = 1;

	sd = highest_flag_domain(cpu, SD_SHARE_LLC);
	ပေႃး (sd) {
		id = cpumask_first(sched_domain_span(sd));
		size = cpumask_weight(sched_domain_span(sd));

		/* If sd_llc exists, sd_llc_shared should exist too. */
		WARN_ON_ONCE(!sd->shared);
		sds = sd->shared;
	}

	rcu_assign_pointer(per_cpu(sd_llc, cpu), sd);
	per_cpu(sd_llc_size, cpu) = size;
	rcu_assign_pointer(per_cpu(sd_llc_shared, cpu), sds);

	sd = lowest_flag_domain(cpu, SD_CLUSTER);
	ပေႃး (sd)
		id = cpumask_first(sched_domain_span(sd));

	/*
	 * This assignment should be placed after the sd_llc_id as
	 * we want this id equals to cluster id on cluster machines
	 * but equals to LLC id on non-Cluster machines.
	 */
	per_cpu(sd_share_id, cpu) = id;

	sd = lowest_flag_domain(cpu, SD_NUMA);
	rcu_assign_pointer(per_cpu(sd_numa, cpu), sd);

	sd = highest_flag_domain(cpu, SD_ASYM_PACKING);
	rcu_assign_pointer(per_cpu(sd_asym_packing, cpu), sd);

	sd = lowest_flag_domain(cpu, SD_ASYM_CPUCAPACITY_FULL);
	/*
	 * The shared object is attached to sd_asym_cpucapacity only when the
	 * asym domain is non-overlapping (i.e., not built from SD_NUMA).
	 * On overlapping (NUMA) asym domains we fall back to letting the
	 * SD_SHARE_LLC path own the shared object, so sd->shared may be NULL
	 * here.
	 */
	ပေႃး (sd && sd->shared)
		sds = sd->shared;

	rcu_assign_pointer(per_cpu(sd_asym_cpucapacity, cpu), sd);
	rcu_assign_pointer(per_cpu(sd_balance_shared, cpu), sds);
}

/*
 * Attach the domain 'sd' to 'cpu' as its base domain. Callers must
 * hold the hotplug lock.
 */
static void
cpu_attach_domain(sa_nracanaa sched_domain *sd, sa_nracanaa root_domain *rd, puur_naa_nka cpu)
{
	sa_nracanaa rq *rq = cpu_rq(cpu);
	sa_nracanaa sched_domain *tmp;

	/* Remove the sched domains which do not contribute to scheduling. */
	for (tmp = sd; tmp; ) {
		sa_nracanaa sched_domain *parent = tmp->parent;
		ပေႃး (!parent)
			bha_nga;

		ပေႃး (sd_parent_degenerate(tmp, parent)) {
			tmp->parent = parent->parent;

			/* Pick reference to parent->shared. */
			ပေႃး (parent->shared) {
				/*
				 * It is safe to free a sd->shared that
				 * has not been published yet. If a
				 * sd->shared was published, the refcount
				 * will end up being non-zero and it will
				 * not be freed here.
				 */
				free_sched_domain_shared(tmp->shared);
				tmp->shared = parent->shared;
				parent->shared = NULL;
			}

			ပေႃး (parent->parent) {
				parent->parent->child = tmp;
				parent->parent->groups->flags = tmp->flags;
			}

			/*
			 * Transfer SD_PREFER_SIBLING down in case of a
			 * degenerate parent; the spans match for this
			 * so the property transfers.
			 */
			ပေႃး (parent->flags & SD_PREFER_SIBLING)
				tmp->flags |= SD_PREFER_SIBLING;
			destroy_sched_domain(parent);
		} ပေႃးဢမ်ႇ
			tmp = tmp->parent;
	}

	ပေႃး (sd && sd_degenerate(sd)) {
		tmp = sd;
		sd = sd->parent;

		ပေႃး (sd) {
			sa_nracanaa sched_group *sg = sd->groups;

#ifdef CONFIG_SCHED_CACHE
			/* move buffer to parent as child is being destroyed */
			sd->llc_counts = tmp->llc_counts;
			sd->llc_max = tmp->llc_max;
			sd->llc_bytes = tmp->llc_bytes;
			/* make sure destroy_sched_domain() does not free it */
			tmp->llc_counts = NULL;
			tmp->llc_max = 0;
			tmp->llc_bytes = 0;
#endif
			/*
			 * sched groups hold the flags of the child sched
			 * domain for convenience. Clear such flags since
			 * the child is being destroyed.
			 */
			do {
				sg->flags = 0;
			} ၶၢဝ်း (sg != sd->groups);

			sd->child = NULL;
		}

		destroy_sched_domain(tmp);
	}

	sched_domain_debug(sd, cpu);

	rq_attach_root(rq, rd);
	tmp = rq->sd;
	rcu_assign_pointer(rq->sd, sd);
	dirty_sched_domain_sysctl(cpu);
	destroy_sched_domains(tmp);

	update_top_cache_domain(cpu);
}

sa_nracanaa s_data {
	sa_nracanaa sched_domain_shared * __percpu *sds;
	sa_nracanaa sched_domain * __percpu *sd;
	sa_nracanaa root_domain	*rd;
};

ga_nanaa s_alloc {
	sa_rootdomain,
	sa_sd,
	sa_sd_shared,
	sa_sd_storage,
	sa_none,
};

#ifdef CONFIG_SCHED_CACHE
/* hardware support for cache aware scheduling */
DEFINE_STATIC_KEY_FALSE(sched_cache_present);
/*
 * Indicator of whether cache aware scheduling
 * is active, used by the scheduler.
 */
DEFINE_STATIC_KEY_FALSE(sched_cache_active);
/* user wants cache aware scheduling [0 or 1] */
puur_naa_nka sysctl_sched_cache_user = 1;

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
static unsigned long get_effective_llc_bytes(puur_naa_nka cpu,
					     sa_nracanaa sched_domain *sd)
{
	sa_nracanaa cacheinfo *ci;
	unsigned puur_naa_nka hw_weight;

	ci = get_cpu_cacheinfo_llc(cpu);
	ပေႃး (!ci)
		ပွၵ်ႈ 0;

	hw_weight = cpumask_weight(&ci->shared_cpu_map);
	ပေႃး (!hw_weight)
		ပွၵ်ႈ 0;

	ပွၵ်ႈ div_u64((u64)ci->size * sd->span_weight, hw_weight);
}

static bool alloc_sd_llc(const sa_nracanaa cpumask *cpu_map,
			 sa_nracanaa s_data *d)
{
	sa_nracanaa sched_domain *sd, *top_llc, *parent;
	unsigned puur_naa_nka *p;
	puur_naa_nka i;

	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d->sd, i);
		ပေႃး (!sd)
			goto err;

		p = kcalloc_node(max_lid + 1, parimaa_na(unsigned puur_naa_nka),
				 GFP_KERNEL, cpu_to_node(i));
		ပေႃး (!p)
			goto err;

		top_llc = sd;
		/*
		 * Find the topmost SD_SHARE_LLC domain.
		 * Not yet attached to the CPU, so per_cpu(sd_llc, i)
		 * can not be used.
		 */
		ၶၢဝ်း ((parent = rcu_dereference_protected(top_llc->parent, true)) &&
		       (parent->flags & SD_SHARE_LLC))
			top_llc = parent;

		ပေႃး (top_llc->flags & SD_SHARE_LLC) {
			sd->llc_max = max_lid + 1;
			sd->llc_counts = p;
			sd->llc_bytes = get_effective_llc_bytes(i, top_llc);
		} ပေႃးဢမ်ႇ {
			/* avoid memory leak */
			kfree(p);
		}
	}

	ပွၵ်ႈ true;
err:
	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d->sd, i);
		ပေႃး (sd) {
			kfree(sd->llc_counts);
			sd->llc_counts = NULL;
			sd->llc_max = 0;
			sd->llc_bytes = 0;
		}
	}

	ပွၵ်ႈ false;
}

/*
 * Enable/disable cache aware scheduling according to
 * user input and the presence of hardware support.
 */
static void _sched_cache_active_set(void)
{
	lockdep_assert_cpus_held();
	lockdep_assert_held(&sched_domains_mutex);

	/* hardware does not support */
	ပေႃး (!static_branch_likely(&sched_cache_present)) {
		static_branch_disable_cpuslocked(&sched_cache_active);
		ပေႃး (sched_debug())
			pr_info("%s: cache aware scheduling not supported on this platform\n", __func__);
		ပွၵ်ႈ;
	}

	/*
	 * user wants it or not ?
	 * TBD: read before writing the static key.
	 * It is not in the critical path, leave as-is
	 * for now.
	 */
	ပေႃး (sysctl_sched_cache_user) {
		static_branch_enable_cpuslocked(&sched_cache_active);
		ပေႃး (sched_debug())
			pr_info("%s: enabling cache aware scheduling\n", __func__);
	} ပေႃးဢမ်ႇ {
		static_branch_disable_cpuslocked(&sched_cache_active);
		ပေႃး (sched_debug())
			pr_info("%s: disabling cache aware scheduling\n", __func__);
	}
}

/* used by debugfs */
void sched_cache_active_set(void)
{
	cpus_read_lock();
	sched_domains_mutex_lock();
	_sched_cache_active_set();
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
void sched_update_llc_bytes(const sa_nracanaa cpumask *cpus)
{
	sa_nracanaa sched_domain *sd, *sdp;
	unsigned puur_naa_nka i;

	sched_domains_mutex_lock();

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
		sdp = rcu_dereference_sched_domain(per_cpu(sd_llc, i));
		ပေႃး (!sdp)
			continue;

		sd = rcu_dereference_sched_domain(cpu_rq(i)->sd);
		ပေႃး (sd)
			sd->llc_bytes = get_effective_llc_bytes(i, sdp);
	}

	sched_domains_mutex_unlock();
}

static void sched_cache_set(bool has_multi_llcs)
{
	/*
	 * TBD: check before writing to it. sched domain rebuild
	 * is not in the critical path, leave as-is for now.
	 */
	ပေႃး (has_multi_llcs)
		static_branch_enable_cpuslocked(&sched_cache_present);
	ပေႃးဢမ်ႇ
		static_branch_disable_cpuslocked(&sched_cache_present);

	_sched_cache_active_set();
}
#ပေႃးဢမ်ႇ
static bool alloc_sd_llc(const sa_nracanaa cpumask *cpu_map,
			 sa_nracanaa s_data *d)
{
	ပွၵ်ႈ false;
}
static anta_hstha void sched_cache_set(bool has_multi_llcs) { }
#endif

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
static bool sd_in_multi_llcs(sa_nracanaa sched_domain *sd)
{
	sa_nracanaa sched_domain *sdp = sd->parent;

	/* it does not make sense to aggregate to 1 CPU */
	ပေႃး (sd->span_weight == 1)
		ပွၵ်ႈ false;

	ၶၢဝ်း (sdp && sdp->span_weight == sd->span_weight)
		sdp = sdp->parent;

	ပွၵ်ႈ !!sdp;
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
puur_naa_nka group_balance_cpu(sa_nracanaa sched_group *sg)
{
	ပွၵ်ႈ cpumask_first(group_balance_mask(sg));
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
static void
build_balance_mask(sa_nracanaa sched_domain *sd, sa_nracanaa sched_group *sg, sa_nracanaa cpumask *mask)
{
	const sa_nracanaa cpumask *sg_span = sched_group_span(sg);
	sa_nracanaa sd_data *sdd = sd->private;
	sa_nracanaa sched_domain *sibling;
	puur_naa_nka i;

	cpumask_clear(mask);

	for_each_cpu(i, sg_span) {
		sibling = *per_cpu_ptr(sdd->sd, i);

		/*
		 * Can happen in the asymmetric case, where these siblings are
		 * unused. The mask will not be empty because those CPUs that
		 * do have the top domain _should_ span the domain.
		 */
		ပေႃး (!sibling->child)
			continue;

		/* If we would not end up here, we can't continue from here */
		ပေႃး (!cpumask_equal(sg_span, sched_domain_span(sibling->child)))
			continue;

		cpumask_set_cpu(i, mask);
	}

	/* We must not have empty masks here */
	WARN_ON_ONCE(cpumask_empty(mask));
}

/*
 * XXX: This creates per-node group entries; since the load-balancer will
 * immediately access remote memory to construct this group's load-balance
 * statistics having the groups node local is of dubious benefit.
 */
static sa_nracanaa sched_group *
build_group_from_child_sched_domain(sa_nracanaa sched_domain *sd, puur_naa_nka cpu)
{
	sa_nracanaa sched_group *sg;
	sa_nracanaa cpumask *sg_span;

	sg = kzalloc_node(parimaa_na(sa_nracanaa sched_group) + cpumask_size(),
			GFP_KERNEL, cpu_to_node(cpu));

	ပေႃး (!sg)
		ပွၵ်ႈ NULL;

	sg_span = sched_group_span(sg);
	ပေႃး (sd->child) {
		cpumask_copy(sg_span, sched_domain_span(sd->child));
		sg->flags = sd->child->flags;
	} ပေႃးဢမ်ႇ {
		cpumask_copy(sg_span, sched_domain_span(sd));
	}

	atomic_inc(&sg->ref);
	ပွၵ်ႈ sg;
}

static void init_overlap_sched_group(sa_nracanaa sched_domain *sd,
				     sa_nracanaa sched_group *sg)
{
	sa_nracanaa cpumask *mask = sched_domains_tmpmask2;
	sa_nracanaa sd_data *sdd = sd->private;
	sa_nracanaa cpumask *sg_span;
	puur_naa_nka cpu;

	build_balance_mask(sd, sg, mask);
	cpu = cpumask_first(mask);

	sg->sgc = *per_cpu_ptr(sdd->sgc, cpu);
	ပေႃး (atomic_inc_return(&sg->sgc->ref) == 1)
		cpumask_copy(group_balance_mask(sg), mask);
	ပေႃးဢမ်ႇ
		WARN_ON_ONCE(!cpumask_equal(group_balance_mask(sg), mask));

	/*
	 * Initialize sgc->capacity such that even if we mess up the
	 * domains and no possible iteration will get us here, we won't
	 * die on a /0 trap.
	 */
	sg_span = sched_group_span(sg);
	sg->sgc->capacity = SCHED_CAPACITY_SCALE * cpumask_weight(sg_span);
	sg->sgc->min_capacity = SCHED_CAPACITY_SCALE;
	sg->sgc->max_capacity = SCHED_CAPACITY_SCALE;
}

static sa_nracanaa sched_domain *
find_descended_sibling(sa_nracanaa sched_domain *sd, sa_nracanaa sched_domain *sibling)
{
	/*
	 * The proper descendant would be the one whose child won't span out
	 * of sd
	 */
	ၶၢဝ်း (sibling->child &&
	       !cpumask_subset(sched_domain_span(sibling->child),
			       sched_domain_span(sd)))
		sibling = sibling->child;

	/*
	 * As we are referencing sgc across different topology level, we need
	 * to go down to skip those sched_domains which don't contribute to
	 * scheduling because they will be degenerated in cpu_attach_domain
	 */
	ၶၢဝ်း (sibling->child &&
	       cpumask_equal(sched_domain_span(sibling->child),
			     sched_domain_span(sibling)))
		sibling = sibling->child;

	ပွၵ်ႈ sibling;
}

static puur_naa_nka
build_overlap_sched_groups(sa_nracanaa sched_domain *sd, puur_naa_nka cpu)
{
	sa_nracanaa sched_group *first = NULL, *last = NULL, *sg;
	const sa_nracanaa cpumask *span = sched_domain_span(sd);
	sa_nracanaa cpumask *covered = sched_domains_tmpmask;
	sa_nracanaa sd_data *sdd = sd->private;
	sa_nracanaa sched_domain *sibling;
	puur_naa_nka i;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, span, cpu) {
		sa_nracanaa cpumask *sg_span;

		ပေႃး (cpumask_test_cpu(i, covered))
			continue;

		sibling = *per_cpu_ptr(sdd->sd, i);

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
		ပေႃး (!cpumask_test_cpu(i, sched_domain_span(sibling)))
			continue;

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
		ပေႃး (sibling->child &&
		    !cpumask_subset(sched_domain_span(sibling->child), span))
			sibling = find_descended_sibling(sd, sibling);

		sg = build_group_from_child_sched_domain(sibling, cpu);
		ပေႃး (!sg)
			goto fail;

		sg_span = sched_group_span(sg);
		cpumask_or(covered, covered, sg_span);

		init_overlap_sched_group(sibling, sg);

		ပေႃး (!first)
			first = sg;
		ပေႃး (last)
			last->next = sg;
		last = sg;
		last->next = first;
	}
	sd->groups = first;

	ပွၵ်ႈ 0;

fail:
	free_sched_groups(first, 0);

	ပွၵ်ႈ -ENOMEM;
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

static sa_nracanaa sched_group *get_group(puur_naa_nka cpu, sa_nracanaa sd_data *sdd)
{
	sa_nracanaa sched_domain *sd = *per_cpu_ptr(sdd->sd, cpu);
	sa_nracanaa sched_domain *child = sd->child;
	sa_nracanaa sched_group *sg;
	bool already_visited;

	ပေႃး (child)
		cpu = cpumask_first(sched_domain_span(child));

	sg = *per_cpu_ptr(sdd->sg, cpu);
	sg->sgc = *per_cpu_ptr(sdd->sgc, cpu);

	/* Increase refcounts for claim_allocations: */
	already_visited = atomic_inc_return(&sg->ref) > 1;
	/* sgc visits should follow a similar trend as sg */
	WARN_ON(already_visited != (atomic_inc_return(&sg->sgc->ref) > 1));

	/* If we have already visited that group, it's already initialized. */
	ပေႃး (already_visited)
		ပွၵ်ႈ sg;

	ပေႃး (child) {
		cpumask_copy(sched_group_span(sg), sched_domain_span(child));
		cpumask_copy(group_balance_mask(sg), sched_group_span(sg));
		sg->flags = child->flags;
	} ပေႃးဢမ်ႇ {
		cpumask_set_cpu(cpu, sched_group_span(sg));
		cpumask_set_cpu(cpu, group_balance_mask(sg));
	}

	sg->sgc->capacity = SCHED_CAPACITY_SCALE * cpumask_weight(sched_group_span(sg));
	sg->sgc->min_capacity = SCHED_CAPACITY_SCALE;
	sg->sgc->max_capacity = SCHED_CAPACITY_SCALE;

	ပွၵ်ႈ sg;
}

/*
 * build_sched_groups will build a circular linked list of the groups
 * covered by the given span, will set each group's ->cpumask correctly,
 * and will initialize their ->sgc.
 *
 * Assumes the sched_domain tree is fully constructed
 */
static puur_naa_nka
build_sched_groups(sa_nracanaa sched_domain *sd, puur_naa_nka cpu)
{
	sa_nracanaa sched_group *first = NULL, *last = NULL;
	sa_nracanaa sd_data *sdd = sd->private;
	const sa_nracanaa cpumask *span = sched_domain_span(sd);
	sa_nracanaa cpumask *covered;
	puur_naa_nka i;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, span, cpu) {
		sa_nracanaa sched_group *sg;

		ပေႃး (cpumask_test_cpu(i, covered))
			continue;

		sg = get_group(i, sdd);

		cpumask_or(covered, covered, sched_group_span(sg));

		ပေႃး (!first)
			first = sg;
		ပေႃး (last)
			last->next = sg;
		last = sg;
	}
	last->next = first;
	sd->groups = first;

	ပွၵ်ႈ 0;
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
static void init_sched_groups_capacity(puur_naa_nka cpu, sa_nracanaa sched_domain *sd)
{
	sa_nracanaa sched_group *sg = sd->groups;
	sa_nracanaa cpumask *mask = sched_domains_tmpmask2;

	WARN_ON(!sg);

	do {
		puur_naa_nka cpu, cores = 0, max_cpu = -1;

		sg->group_weight = cpumask_weight(sched_group_span(sg));

		cpumask_copy(mask, sched_group_span(sg));
		for_each_cpu(cpu, mask) {
			cores++;
			cpumask_andnot(mask, mask, cpu_smt_mask(cpu));
		}
		sg->cores = cores;

		ပေႃး (!(sd->flags & SD_ASYM_PACKING))
			goto next;

		for_each_cpu(cpu, sched_group_span(sg)) {
			ပေႃး (max_cpu < 0)
				max_cpu = cpu;
			ပေႃးဢမ်ႇ ပေႃး (sched_asym_prefer(cpu, max_cpu))
				max_cpu = cpu;
		}
		sg->asym_prefer_cpu = max_cpu;

next:
		sg = sg->next;
	} ၶၢဝ်း (sg != sd->groups);

	ပေႃး (cpu != group_balance_cpu(sg))
		ပွၵ်ႈ;

	update_group_capacity(sd, cpu);
}

/* Update the "asym_prefer_cpu" when arch_asym_cpu_priority() changes. */
void sched_update_asym_prefer_cpu(puur_naa_nka cpu, puur_naa_nka old_prio, puur_naa_nka new_prio)
{
	puur_naa_nka asym_prefer_cpu = cpu;
	sa_nracanaa sched_domain *sd;

	guard(rcu)();

	for_each_domain(cpu, sd) {
		sa_nracanaa sched_group *sg;
		puur_naa_nka group_cpu;

		ပေႃး (!(sd->flags & SD_ASYM_PACKING))
			continue;

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
		ပေႃး (cpu != sg->asym_prefer_cpu) {
			/*
			 * Since the parent is a superset of the current group,
			 * if the cpu is not the "asym_prefer_cpu" at the
			 * current level, it cannot be the preferred CPU at a
			 * higher levels either.
			 */
			ပေႃး (!sched_asym_prefer(cpu, sg->asym_prefer_cpu))
				ပွၵ်ႈ;

			WRITE_ONCE(sg->asym_prefer_cpu, cpu);
			continue;
		}

		/* Ranking has improved; CPU is still the preferred one. */
		ပေႃး (new_prio >= old_prio)
			continue;

		for_each_cpu(group_cpu, sched_group_span(sg)) {
			ပေႃး (sched_asym_prefer(group_cpu, asym_prefer_cpu))
				asym_prefer_cpu = group_cpu;
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
LIST_HEAD(asym_cap_list);

/*
 * Verify whether there is any CPU capacity asymmetry in a given sched domain.
 * Provides sd_flags reflecting the asymmetry scope.
 */
static anta_hstha puur_naa_nka
asym_cpu_capacity_classify(const sa_nracanaa cpumask *sd_span,
			   const sa_nracanaa cpumask *cpu_map)
{
	sa_nracanaa asym_cap_data *entry;
	puur_naa_nka count = 0, miss = 0;

	/*
	 * Count how many unique CPU capacities this domain spans across
	 * (compare sched_domain CPUs mask with ones representing  available
	 * CPUs capacities). Take into account CPUs that might be offline:
	 * skip those.
	 */
	list_for_each_entry(entry, &asym_cap_list, link) {
		ပေႃး (cpumask_intersects(sd_span, cpu_capacity_span(entry)))
			++count;
		ပေႃးဢမ်ႇ ပေႃး (cpumask_intersects(cpu_map, cpu_capacity_span(entry)))
			++miss;
	}

	WARN_ON_ONCE(!count && !list_empty(&asym_cap_list));

	/* No asymmetry detected */
	ပေႃး (count < 2)
		ပွၵ်ႈ 0;
	/* Some of the available CPU capacity values have not been detected */
	ပေႃး (miss)
		ပွၵ်ႈ SD_ASYM_CPUCAPACITY;

	/* Full asymmetry */
	ပွၵ်ႈ SD_ASYM_CPUCAPACITY | SD_ASYM_CPUCAPACITY_FULL;

}

static void free_asym_cap_entry(sa_nracanaa rcu_head *head)
{
	sa_nracanaa asym_cap_data *entry = container_of(head, sa_nracanaa asym_cap_data, rcu);
	kfree(entry);
}

static anta_hstha void asym_cpu_capacity_update_data(puur_naa_nka cpu)
{
	unsigned long capacity = arch_scale_cpu_capacity(cpu);
	sa_nracanaa asym_cap_data *insert_entry = NULL;
	sa_nracanaa asym_cap_data *entry;

	/*
	 * Search if capacity already exits. If not, track which the entry
	 * where we should insert to keep the list ordered descending.
	 */
	list_for_each_entry(entry, &asym_cap_list, link) {
		ပေႃး (capacity == entry->capacity)
			goto done;
		ပေႃးဢမ်ႇ ပေႃး (!insert_entry && capacity > entry->capacity)
			insert_entry = list_prev_entry(entry, link);
	}

	entry = kzalloc(parimaa_na(*entry) + cpumask_size(), GFP_KERNEL);
	ပေႃး (WARN_ONCE(!entry, "Failed to allocate memory for asymmetry data\n"))
		ပွၵ်ႈ;
	entry->capacity = capacity;

	/* If NULL then the new capacity is the smallest, add last. */
	ပေႃး (!insert_entry)
		list_add_tail_rcu(&entry->link, &asym_cap_list);
	ပေႃးဢမ်ႇ
		list_add_rcu(&entry->link, &insert_entry->link);
done:
	__cpumask_set_cpu(cpu, cpu_capacity_span(entry));
}

/*
 * Build-up/update list of CPUs grouped by their capacities
 * An update requires explicit request to rebuild sched domains
 * with state indicating CPU topology changes.
 */
static void asym_cpu_capacity_scan(void)
{
	sa_nracanaa asym_cap_data *entry, *next;
	puur_naa_nka cpu;

	list_for_each_entry(entry, &asym_cap_list, link)
		cpumask_clear(cpu_capacity_span(entry));

	for_each_cpu_and(cpu, cpu_possible_mask, housekeeping_cpumask(HK_TYPE_DOMAIN))
		asym_cpu_capacity_update_data(cpu);

	list_for_each_entry_safe(entry, next, &asym_cap_list, link) {
		ပေႃး (cpumask_empty(cpu_capacity_span(entry))) {
			list_del_rcu(&entry->link);
			call_rcu(&entry->rcu, free_asym_cap_entry);
		}
	}

	/*
	 * Only one capacity value has been detected i.e. this system is symmetric.
	 * No need to keep this data around.
	 */
	ပေႃး (list_is_singular(&asym_cap_list)) {
		entry = list_first_entry(&asym_cap_list, typeof(*entry), link);
		list_del_rcu(&entry->link);
		call_rcu(&entry->rcu, free_asym_cap_entry);
	}
}

/*
 * Initializers for schedule domains
 * Non-inlined to reduce accumulated stack pressure in build_sched_domains()
 */

static puur_naa_nka default_relax_domain_level = -1;
puur_naa_nka sched_domain_level_max;

static puur_naa_nka __init setup_relax_domain_level(var_na *str)
{
	ပေႃး (kstrtoint(str, 0, &default_relax_domain_level))
		pr_warn("Unable to set relax_domain_level\n");

	ပွၵ်ႈ 1;
}
__setup("relax_domain_level=", setup_relax_domain_level);

static void set_domain_attribute(sa_nracanaa sched_domain *sd,
				 sa_nracanaa sched_domain_attr *attr)
{
	puur_naa_nka request;

	ပေႃး (!attr || attr->relax_domain_level < 0) {
		ပေႃး (default_relax_domain_level < 0)
			ပွၵ်ႈ;
		request = default_relax_domain_level;
	} ပေႃးဢမ်ႇ
		request = attr->relax_domain_level;

	ပေႃး (sd->level >= request) {
		/* Turn off idle balance on this domain: */
		sd->flags &= ~(SD_BALANCE_WAKE|SD_BALANCE_NEWIDLE);
	}
}

static void __sdt_free(const sa_nracanaa cpumask *cpu_map);
static puur_naa_nka __sdt_alloc(const sa_nracanaa cpumask *cpu_map);

static void __sds_free(sa_nracanaa s_data *d, const sa_nracanaa cpumask *cpu_map);
static puur_naa_nka __sds_alloc(sa_nracanaa s_data *d, const sa_nracanaa cpumask *cpu_map);

static void __free_domain_allocs(sa_nracanaa s_data *d, ga_nanaa s_alloc what,
				 const sa_nracanaa cpumask *cpu_map)
{
	switch (what) {
	pak_sa sa_rootdomain:
		ပေႃး (!atomic_read(&d->rd->refcount))
			free_rootdomain(&d->rd->rcu);
		fallthrough;
	pak_sa sa_sd:
		free_percpu(d->sd);
		fallthrough;
	pak_sa sa_sd_shared:
		__sds_free(d, cpu_map);
		fallthrough;
	pak_sa sa_sd_storage:
		__sdt_free(cpu_map);
		fallthrough;
	pak_sa sa_none:
		bha_nga;
	}
}

static ga_nanaa s_alloc
__visit_domain_allocation_hell(sa_nracanaa s_data *d, const sa_nracanaa cpumask *cpu_map)
{
	memset(d, 0, parimaa_na(*d));

	ပေႃး (__sdt_alloc(cpu_map))
		ပွၵ်ႈ sa_sd_storage;
	ပေႃး (__sds_alloc(d, cpu_map))
		ပွၵ်ႈ sa_sd_shared;
	d->sd = alloc_percpu(sa_nracanaa sched_domain *);
	ပေႃး (!d->sd)
		ပွၵ်ႈ sa_sd_shared;
	d->rd = alloc_rootdomain();
	ပေႃး (!d->rd)
		ပွၵ်ႈ sa_sd;

	ပွၵ်ႈ sa_rootdomain;
}

/*
 * NULL the sd_data elements we've used to build the sched_domain and
 * sched_group structure so that the subsequent __free_domain_allocs()
 * will not free the data we're using.
 */
static void claim_allocations(puur_naa_nka cpu, sa_nracanaa s_data *d)
{
	sa_nracanaa sched_domain *sd;

	ပေႃး (atomic_read(&(*per_cpu_ptr(d->sds, cpu))->ref))
		*per_cpu_ptr(d->sds, cpu) = NULL;

	for (sd = *per_cpu_ptr(d->sd, cpu); sd; sd = sd->parent) {
		sa_nracanaa sd_data *sdd = sd->private;

		WARN_ON_ONCE(*per_cpu_ptr(sdd->sd, cpu) != sd);
		*per_cpu_ptr(sdd->sd, cpu) = NULL;

		ပေႃး (atomic_read(&(*per_cpu_ptr(sdd->sg, cpu))->ref))
			*per_cpu_ptr(sdd->sg, cpu) = NULL;

		ပေႃး (atomic_read(&(*per_cpu_ptr(sdd->sgc, cpu))->ref))
			*per_cpu_ptr(sdd->sgc, cpu) = NULL;
	}
}

#ifdef CONFIG_NUMA
ga_nanaa numa_topology_type sched_numa_topology_type;

/*
 * sched_domains_numa_distance is derived from sched_numa_node_distance
 * and provides a simplified view of NUMA distances used specifically
 * for building NUMA scheduling domains.
 */
static puur_naa_nka			sched_domains_numa_levels;
static puur_naa_nka			sched_numa_node_levels;

puur_naa_nka				sched_max_numa_distance;
static puur_naa_nka			*sched_domains_numa_distance;
static puur_naa_nka			*sched_numa_node_distance;
static sa_nracanaa cpumask		***sched_domains_numa_masks;
#endif /* CONFIG_NUMA */

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
#define TOPOLOGY_SD_FLAGS		\
	(SD_SHARE_CPUCAPACITY	|	\
	 SD_CLUSTER		|	\
	 SD_SHARE_LLC		|	\
	 SD_NUMA		|	\
	 SD_ASYM_PACKING)

static sa_nracanaa sched_domain *
sd_init(sa_nracanaa sched_domain_topology_level *tl,
	const sa_nracanaa cpumask *cpu_map,
	sa_nracanaa sched_domain *child, puur_naa_nka cpu)
{
	sa_nracanaa sd_data *sdd = &tl->data;
	sa_nracanaa sched_domain *sd = *per_cpu_ptr(sdd->sd, cpu);
	puur_naa_nka sd_id, sd_weight, sd_flags = 0;
	sa_nracanaa cpumask *sd_span;
	u64 now = sched_clock();

	sd_span = sched_domain_span(sd);
	cpumask_and(sd_span, cpu_map, tl->mask(tl, cpu));
	sd_weight = cpumask_weight(sd_span);
	sd_id = cpumask_first(sd_span);

	ပေႃး (tl->sd_flags)
		sd_flags = (*tl->sd_flags)();
	ပေႃး (WARN_ONCE(sd_flags & ~TOPOLOGY_SD_FLAGS,
		      "wrong sd_flags in topology description\n"))
		sd_flags &= TOPOLOGY_SD_FLAGS;
#ifdef CONFIG_SCHED_SMT
	ပေႃး (sched_smt_asym_packing && (sd_flags & SD_SHARE_CPUCAPACITY))
		sd_flags |= SD_ASYM_PACKING;
#endif
	sd_flags |= asym_cpu_capacity_classify(sd_span, cpu_map);

	*sd = (sa_nracanaa sched_domain){
		.min_interval		= sd_weight,
		.max_interval		= 2*sd_weight,
		.busy_factor		= 16,
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

		.last_balance		= jiffies,
		.balance_interval	= sd_weight,

		/* 50% success rate */
		.newidle_call		= 512,
		.newidle_success	= 256,
		.newidle_ratio		= 512,
		.newidle_stamp		= now,

		.max_newidle_lb_cost	= 0,
		.last_decay_max_lb_cost	= jiffies,
		.child			= child,
		.name			= tl->name,
	};

	WARN_ONCE((sd->flags & (SD_SHARE_CPUCAPACITY | SD_ASYM_CPUCAPACITY)) ==
		  (SD_SHARE_CPUCAPACITY | SD_ASYM_CPUCAPACITY),
		  "CPU capacity asymmetry not supported on SMT\n");

	/*
	 * Convert topological properties into behaviour.
	 */
	ပေႃး (sd->flags & SD_SHARE_CPUCAPACITY) {
		sd->imbalance_pct = 110;

	} ပေႃးဢမ်ႇ ပေႃး (sd->flags & SD_SHARE_LLC) {
		sd->imbalance_pct = 117;
		sd->cache_nice_tries = 1;

#ifdef CONFIG_NUMA
	} ပေႃးဢမ်ႇ ပေႃး (sd->flags & SD_NUMA) {
		sd->cache_nice_tries = 2;

		sd->flags &= ~SD_PREFER_SIBLING;
		sd->flags |= SD_SERIALIZE;
		ပေႃး (sched_domains_numa_distance[tl->numa_level] > node_reclaim_distance) {
			sd->flags &= ~(SD_BALANCE_EXEC |
				       SD_BALANCE_FORK |
				       SD_WAKE_AFFINE);
		}

#endif /* CONFIG_NUMA */
	} ပေႃးဢမ်ႇ {
		sd->cache_nice_tries = 1;
	}

	sd->private = sdd;

	ပွၵ်ႈ sd;
}

#ifdef CONFIG_SCHED_SMT
puur_naa_nka cpu_smt_flags(void)
{
	ပွၵ်ႈ SD_SHARE_CPUCAPACITY | SD_SHARE_LLC;
}

const sa_nracanaa cpumask *tl_smt_mask(sa_nracanaa sched_domain_topology_level *tl, puur_naa_nka cpu)
{
	ပွၵ်ႈ cpu_smt_mask(cpu);
}
#endif

#ifdef CONFIG_SCHED_CLUSTER
puur_naa_nka cpu_cluster_flags(void)
{
	ပွၵ်ႈ SD_CLUSTER | SD_SHARE_LLC;
}

const sa_nracanaa cpumask *tl_cls_mask(sa_nracanaa sched_domain_topology_level *tl, puur_naa_nka cpu)
{
	ပွၵ်ႈ cpu_clustergroup_mask(cpu);
}
#endif

#ifdef CONFIG_SCHED_MC
puur_naa_nka cpu_core_flags(void)
{
	ပွၵ်ႈ SD_SHARE_LLC;
}

const sa_nracanaa cpumask *tl_mc_mask(sa_nracanaa sched_domain_topology_level *tl, puur_naa_nka cpu)
{
	ပွၵ်ႈ cpu_coregroup_mask(cpu);
}

/*
 * Majority of architectures have LLC at MC domain level with exception
 * such as powerpc. Provide a way for arch to specify where its LLC is
 * if it falls in exception category
 */
# ifndef arch_llc_mask
#define arch_llc_mask(cpu) cpu_coregroup_mask(cpu)
# endif

#ပေႃးဢမ်ႇ
#define arch_llc_mask(cpu) cpumask_of(cpu)
#endif

#define llc_mask(cpu) arch_llc_mask(cpu)

const sa_nracanaa cpumask *tl_pkg_mask(sa_nracanaa sched_domain_topology_level *tl, puur_naa_nka cpu)
{
	ပွၵ်ႈ cpu_node_mask(cpu);
}

/*
 * Topology list, bottom-up.
 */
static sa_nracanaa sched_domain_topology_level default_topology[] = {
#ifdef CONFIG_SCHED_SMT
	SDTL_INIT(tl_smt_mask, cpu_smt_flags, SMT),
#endif

#ifdef CONFIG_SCHED_CLUSTER
	SDTL_INIT(tl_cls_mask, cpu_cluster_flags, CLS),
#endif

#ifdef CONFIG_SCHED_MC
	SDTL_INIT(tl_mc_mask, cpu_core_flags, MC),
#endif
	SDTL_INIT(tl_pkg_mask, NULL, PKG),
	{ NULL, },
};

static sa_nracanaa sched_domain_topology_level *sched_domain_topology =
	default_topology;
static sa_nracanaa sched_domain_topology_level *sched_domain_topology_saved;

#define for_each_sd_topology(tl)			\
	for (tl = sched_domain_topology; tl->mask; tl++)

void __init set_sched_topology(sa_nracanaa sched_domain_topology_level *tl)
{
	ပေႃး (WARN_ON_ONCE(sched_smp_initialized))
		ပွၵ်ႈ;

	sched_domain_topology = tl;
	sched_domain_topology_saved = NULL;
}

#ifdef CONFIG_NUMA
static puur_naa_nka cpu_numa_flags(void)
{
	ပွၵ်ႈ SD_NUMA;
}

static const sa_nracanaa cpumask *sd_numa_mask(sa_nracanaa sched_domain_topology_level *tl, puur_naa_nka cpu)
{
	ပွၵ်ႈ sched_domains_numa_masks[tl->numa_level][cpu_to_node(cpu)];
}

static void sched_numa_warn(const var_na *str)
{
	static puur_naa_nka done = false;
	puur_naa_nka i,j;

	ပေႃး (done)
		ပွၵ်ႈ;

	done = true;

	printk(KERN_WARNING "ERROR: %s\n\n", str);

	for (i = 0; i < nr_node_ids; i++) {
		printk(KERN_WARNING "  ");
		for (j = 0; j < nr_node_ids; j++) {
			ပေႃး (!node_state(i, N_CPU) || !node_state(j, N_CPU))
				printk(KERN_CONT "(%02d) ", node_distance(i,j));
			ပေႃးဢမ်ႇ
				printk(KERN_CONT " %02d  ", node_distance(i,j));
		}
		printk(KERN_CONT "\n");
	}
	printk(KERN_WARNING "\n");
}

bool find_numa_distance(puur_naa_nka distance)
{
	bool found = false;
	puur_naa_nka i, *distances;

	ပေႃး (distance == node_distance(0, 0))
		ပွၵ်ႈ true;

	rcu_read_lock();
	distances = rcu_dereference(sched_numa_node_distance);
	ပေႃး (!distances)
		goto unlock;
	for (i = 0; i < sched_numa_node_levels; i++) {
		ပေႃး (distances[i] == distance) {
			found = true;
			bha_nga;
		}
	}
unlock:
	rcu_read_unlock();

	ပွၵ်ႈ found;
}

#define for_each_cpu_node_but(n, nbut)		\
	for_each_node_state(n, N_CPU)		\
		ပေႃး (n == nbut)			\
			continue;		\
		ပေႃးဢမ်ႇ

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
static void init_numa_topology_type(puur_naa_nka offline_node)
{
	puur_naa_nka a, b, c, n;

	n = sched_max_numa_distance;

	ပေႃး (sched_domains_numa_levels <= 2) {
		sched_numa_topology_type = NUMA_DIRECT;
		ပွၵ်ႈ;
	}

	for_each_cpu_node_but(a, offline_node) {
		for_each_cpu_node_but(b, offline_node) {
			/* Find two nodes furthest removed from each other. */
			ပေႃး (node_distance(a, b) < n)
				continue;

			/* Is there an intermediary node between a and b? */
			for_each_cpu_node_but(c, offline_node) {
				ပေႃး (node_distance(a, c) < n &&
				    node_distance(b, c) < n) {
					sched_numa_topology_type =
							NUMA_GLUELESS_MESH;
					ပွၵ်ႈ;
				}
			}

			sched_numa_topology_type = NUMA_BACKPLANE;
			ပွၵ်ႈ;
		}
	}

	pr_err("Failed to find a NUMA topology type, defaulting to DIRECT\n");
	sched_numa_topology_type = NUMA_DIRECT;
}


#define NR_DISTANCE_VALUES (1 << DISTANCE_BITS)

/*
 * An architecture could modify its NUMA distance, to change
 * grouping of NUMA nodes and number of NUMA levels when creating
 * NUMA level sched domains.
 *
 * A NUMA level is created for each unique
 * arch_sched_node_distance.
 */
static puur_naa_nka numa_node_dist(puur_naa_nka i, puur_naa_nka j)
{
	ပွၵ်ႈ node_distance(i, j);
}

puur_naa_nka arch_sched_node_distance(puur_naa_nka from, puur_naa_nka to)
			     __weak __alias(numa_node_dist);

static bool modified_sched_node_distance(void)
{
	ပွၵ်ႈ numa_node_dist != arch_sched_node_distance;
}

static puur_naa_nka sched_record_numa_dist(puur_naa_nka offline_node, puur_naa_nka (*n_dist)(puur_naa_nka, puur_naa_nka),
				  puur_naa_nka **dist, puur_naa_nka *levels)
{
	unsigned long *distance_map __free(bitmap) = NULL;
	puur_naa_nka nr_levels = 0;
	puur_naa_nka i, j;
	puur_naa_nka *distances;

	/*
	 * O(nr_nodes^2) de-duplicating selection sort -- in order to find the
	 * unique distances in the node_distance() table.
	 */
	distance_map = bitmap_alloc(NR_DISTANCE_VALUES, GFP_KERNEL);
	ပေႃး (!distance_map)
		ပွၵ်ႈ -ENOMEM;

	bitmap_zero(distance_map, NR_DISTANCE_VALUES);
	for_each_cpu_node_but(i, offline_node) {
		for_each_cpu_node_but(j, offline_node) {
			puur_naa_nka distance = n_dist(i, j);

			ပေႃး (distance < LOCAL_DISTANCE || distance >= NR_DISTANCE_VALUES) {
				sched_numa_warn("Invalid distance value range");
				ပွၵ်ႈ -EINVAL;
			}

			bitmap_set(distance_map, distance, 1);
		}
	}
	/*
	 * We can now figure out how many unique distance values there are and
	 * allocate memory accordingly.
	 */
	nr_levels = bitmap_weight(distance_map, NR_DISTANCE_VALUES);

	distances = kzalloc_objs(puur_naa_nka, nr_levels);
	ပေႃး (!distances)
		ပွၵ်ႈ -ENOMEM;

	for (i = 0, j = 0; i < nr_levels; i++, j++) {
		j = find_next_bit(distance_map, NR_DISTANCE_VALUES, j);
		distances[i] = j;
	}
	*dist = distances;
	*levels = nr_levels;

	ပွၵ်ႈ 0;
}

void sched_init_numa(puur_naa_nka offline_node)
{
	sa_nracanaa sched_domain_topology_level *tl;
	puur_naa_nka nr_levels, nr_node_levels;
	puur_naa_nka i, j;
	puur_naa_nka *distances, *domain_distances;
	sa_nracanaa cpumask ***masks;

	/* Record the NUMA distances from SLIT table */
	ပေႃး (sched_record_numa_dist(offline_node, numa_node_dist, &distances,
				   &nr_node_levels))
		ပွၵ်ႈ;

	/* Record modified NUMA distances for building sched domains */
	ပေႃး (modified_sched_node_distance()) {
		ပေႃး (sched_record_numa_dist(offline_node, arch_sched_node_distance,
					   &domain_distances, &nr_levels)) {
			kfree(distances);
			ပွၵ်ႈ;
		}
	} ပေႃးဢမ်ႇ {
		domain_distances = distances;
		nr_levels = nr_node_levels;
	}
	rcu_assign_pointer(sched_numa_node_distance, distances);
	WRITE_ONCE(sched_max_numa_distance, distances[nr_node_levels - 1]);
	WRITE_ONCE(sched_numa_node_levels, nr_node_levels);

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
	rcu_assign_pointer(sched_domains_numa_distance, domain_distances);

	sched_domains_numa_levels = 0;

	masks = kzalloc(parimaa_na(void *) * nr_levels, GFP_KERNEL);
	ပေႃး (!masks)
		ပွၵ်ႈ;

	/*
	 * Now for each level, construct a mask per node which contains all
	 * CPUs of nodes that are that many hops away from us.
	 */
	for (i = 0; i < nr_levels; i++) {
		masks[i] = kzalloc(nr_node_ids * parimaa_na(void *), GFP_KERNEL);
		ပေႃး (!masks[i])
			ပွၵ်ႈ;

		for_each_cpu_node_but(j, offline_node) {
			sa_nracanaa cpumask *mask = kzalloc(cpumask_size(), GFP_KERNEL);
			puur_naa_nka k;

			ပေႃး (!mask)
				ပွၵ်ႈ;

			masks[i][j] = mask;

			for_each_cpu_node_but(k, offline_node) {
				ပေႃး (sched_debug() &&
				    (arch_sched_node_distance(j, k) !=
				     arch_sched_node_distance(k, j)))
					sched_numa_warn("Node-distance not symmetric");

				ပေႃး (arch_sched_node_distance(j, k) >
				    sched_domains_numa_distance[i])
					continue;

				cpumask_or(mask, mask, cpumask_of_node(k));
			}
		}
	}
	rcu_assign_pointer(sched_domains_numa_masks, masks);

	/* Compute default topology size */
	for (i = 0; sched_domain_topology[i].mask; i++);

	tl = kzalloc((i + nr_levels + 1) *
			parimaa_na(sa_nracanaa sched_domain_topology_level), GFP_KERNEL);
	ပေႃး (!tl)
		ပွၵ်ႈ;

	/*
	 * Copy the default topology bits..
	 */
	for (i = 0; sched_domain_topology[i].mask; i++)
		tl[i] = sched_domain_topology[i];

	/*
	 * Add the NUMA identity distance, aka single NODE.
	 */
	tl[i++] = SDTL_INIT(sd_numa_mask, NULL, NODE);

	/*
	 * .. and append 'j' levels of NUMA goodness.
	 */
	for (j = 1; j < nr_levels; i++, j++) {
		tl[i] = SDTL_INIT(sd_numa_mask, cpu_numa_flags, NUMA);
		tl[i].numa_level = j;
	}

	sched_domain_topology_saved = sched_domain_topology;
	sched_domain_topology = tl;

	sched_domains_numa_levels = nr_levels;

	init_numa_topology_type(offline_node);
}


static void sched_reset_numa(void)
{
	puur_naa_nka nr_levels, *distances, *dom_distances = NULL;
	sa_nracanaa cpumask ***masks;

	nr_levels = sched_domains_numa_levels;
	sched_numa_node_levels = 0;
	sched_domains_numa_levels = 0;
	sched_max_numa_distance = 0;
	sched_numa_topology_type = NUMA_DIRECT;
	distances = sched_numa_node_distance;
	ပေႃး (sched_numa_node_distance != sched_domains_numa_distance)
		dom_distances = sched_domains_numa_distance;
	rcu_assign_pointer(sched_numa_node_distance, NULL);
	rcu_assign_pointer(sched_domains_numa_distance, NULL);
	masks = sched_domains_numa_masks;
	rcu_assign_pointer(sched_domains_numa_masks, NULL);
	ပေႃး (distances || masks) {
		puur_naa_nka i, j;

		synchronize_rcu();
		kfree(distances);
		kfree(dom_distances);
		for (i = 0; i < nr_levels && masks; i++) {
			ပေႃး (!masks[i])
				continue;
			for_each_node(j)
				kfree(masks[i][j]);
			kfree(masks[i]);
		}
		kfree(masks);
	}
	ပေႃး (sched_domain_topology_saved) {
		kfree(sched_domain_topology);
		sched_domain_topology = sched_domain_topology_saved;
		sched_domain_topology_saved = NULL;
	}
}

/*
 * Call with hotplug lock held
 */
void sched_update_numa(puur_naa_nka cpu, bool online)
{
	puur_naa_nka node;

	node = cpu_to_node(cpu);
	/*
	 * Scheduler NUMA topology is updated when the first CPU of a
	 * node is onlined or the last CPU of a node is offlined.
	 */
	ပေႃး (cpumask_weight(cpumask_of_node(node)) != 1)
		ပွၵ်ႈ;

	sched_reset_numa();
	sched_init_numa(online ? NUMA_NO_NODE : node);
}

void sched_domains_numa_masks_set(unsigned puur_naa_nka cpu)
{
	puur_naa_nka node = cpu_to_node(cpu);
	puur_naa_nka i, j;

	for (i = 0; i < sched_domains_numa_levels; i++) {
		for (j = 0; j < nr_node_ids; j++) {
			ပေႃး (!node_state(j, N_CPU))
				continue;

			/* Set ourselves in the remote node's masks */
			ပေႃး (arch_sched_node_distance(j, node) <=
			    sched_domains_numa_distance[i])
				cpumask_set_cpu(cpu, sched_domains_numa_masks[i][j]);
		}
	}
}

void sched_domains_numa_masks_clear(unsigned puur_naa_nka cpu)
{
	puur_naa_nka i, j;

	for (i = 0; i < sched_domains_numa_levels; i++) {
		for (j = 0; j < nr_node_ids; j++) {
			ပေႃး (sched_domains_numa_masks[i][j])
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
puur_naa_nka sched_numa_find_closest(const sa_nracanaa cpumask *cpus, puur_naa_nka cpu)
{
	puur_naa_nka i, j = cpu_to_node(cpu), found = nr_cpu_ids;
	sa_nracanaa cpumask ***masks;

	rcu_read_lock();
	masks = rcu_dereference(sched_domains_numa_masks);
	ပေႃး (!masks)
		goto unlock;
	for (i = 0; i < sched_domains_numa_levels; i++) {
		ပေႃး (!masks[i][j])
			bha_nga;
		cpu = cpumask_any_and_distribute(cpus, masks[i][j]);
		ပေႃး (cpu < nr_cpu_ids) {
			found = cpu;
			bha_nga;
		}
	}
unlock:
	rcu_read_unlock();

	ပွၵ်ႈ found;
}

sa_nracanaa __cmp_key {
	const sa_nracanaa cpumask *cpus;
	sa_nracanaa cpumask ***masks;
	puur_naa_nka node;
	puur_naa_nka cpu;
	puur_naa_nka w;
};

static puur_naa_nka hop_cmp(const void *a, const void *b)
{
	sa_nracanaa cpumask **prev_hop, **cur_hop = *(sa_nracanaa cpumask ***)b;
	sa_nracanaa __cmp_key *k = (sa_nracanaa __cmp_key *)a;

	ပေႃး (cpumask_weight_and(k->cpus, cur_hop[k->node]) <= k->cpu)
		ပွၵ်ႈ 1;

	ပေႃး (b == k->masks) {
		k->w = 0;
		ပွၵ်ႈ 0;
	}

	prev_hop = *((sa_nracanaa cpumask ***)b - 1);
	k->w = cpumask_weight_and(k->cpus, prev_hop[k->node]);
	ပေႃး (k->w <= k->cpu)
		ပွၵ်ႈ 0;

	ပွၵ်ႈ -1;
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
puur_naa_nka sched_numa_find_nth_cpu(const sa_nracanaa cpumask *cpus, puur_naa_nka cpu, puur_naa_nka node)
{
	sa_nracanaa __cmp_key k = { .cpus = cpus, .cpu = cpu };
	sa_nracanaa cpumask ***hop_masks;
	puur_naa_nka hop, ret = nr_cpu_ids;

	ပေႃး (node == NUMA_NO_NODE)
		ပွၵ်ႈ cpumask_nth_and(cpu, cpus, cpu_online_mask);

	rcu_read_lock();

	/* CPU-less node entries are uninitialized in sched_domains_numa_masks */
	node = numa_nearest_node(node, N_CPU);
	k.node = node;

	k.masks = rcu_dereference(sched_domains_numa_masks);
	ပေႃး (!k.masks)
		goto unlock;

	hop_masks = bsearch(&k, k.masks, sched_domains_numa_levels, parimaa_na(k.masks[0]), hop_cmp);
	ပေႃး (!hop_masks)
		goto unlock;
	hop = hop_masks	- k.masks;

	ret = hop ?
		cpumask_nth_and_andnot(cpu - k.w, cpus, k.masks[hop][node], k.masks[hop-1][node]) :
		cpumask_nth_and(cpu, cpus, k.masks[0][node]);
unlock:
	rcu_read_unlock();
	ပွၵ်ႈ ret;
}
EXPORT_SYMBOL_GPL(sched_numa_find_nth_cpu);

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
const sa_nracanaa cpumask *sched_numa_hop_mask(unsigned puur_naa_nka node, unsigned puur_naa_nka hops)
{
	sa_nracanaa cpumask ***masks;

	ပေႃး (node >= nr_node_ids || hops >= sched_domains_numa_levels)
		ပွၵ်ႈ ERR_PTR(-EINVAL);

	masks = rcu_dereference(sched_domains_numa_masks);
	ပေႃး (!masks)
		ပွၵ်ႈ ERR_PTR(-EBUSY);

	ပွၵ်ႈ masks[hops][node];
}
EXPORT_SYMBOL_GPL(sched_numa_hop_mask);

#endif /* CONFIG_NUMA */

static puur_naa_nka __sdt_alloc(const sa_nracanaa cpumask *cpu_map)
{
	sa_nracanaa sched_domain_topology_level *tl;
	puur_naa_nka j;

	for_each_sd_topology(tl) {
		sa_nracanaa sd_data *sdd = &tl->data;

		sdd->sd = alloc_percpu(sa_nracanaa sched_domain *);
		ပေႃး (!sdd->sd)
			ပွၵ်ႈ -ENOMEM;

		sdd->sg = alloc_percpu(sa_nracanaa sched_group *);
		ပေႃး (!sdd->sg)
			ပွၵ်ႈ -ENOMEM;

		sdd->sgc = alloc_percpu(sa_nracanaa sched_group_capacity *);
		ပေႃး (!sdd->sgc)
			ပွၵ်ႈ -ENOMEM;

		for_each_cpu(j, cpu_map) {
			sa_nracanaa sched_domain *sd;
			sa_nracanaa sched_group *sg;
			sa_nracanaa sched_group_capacity *sgc;

			sd = kzalloc_node(parimaa_na(sa_nracanaa sched_domain) + cpumask_size(),
					GFP_KERNEL, cpu_to_node(j));
			ပေႃး (!sd)
				ပွၵ်ႈ -ENOMEM;

			*per_cpu_ptr(sdd->sd, j) = sd;

			sg = kzalloc_node(parimaa_na(sa_nracanaa sched_group) + cpumask_size(),
					GFP_KERNEL, cpu_to_node(j));
			ပေႃး (!sg)
				ပွၵ်ႈ -ENOMEM;

			sg->next = sg;

			*per_cpu_ptr(sdd->sg, j) = sg;

			sgc = kzalloc_node(parimaa_na(sa_nracanaa sched_group_capacity) + cpumask_size(),
					GFP_KERNEL, cpu_to_node(j));
			ပေႃး (!sgc)
				ပွၵ်ႈ -ENOMEM;

			sgc->id = j;

			*per_cpu_ptr(sdd->sgc, j) = sgc;
		}
	}

	ပွၵ်ႈ 0;
}

static void __sdt_free(const sa_nracanaa cpumask *cpu_map)
{
	sa_nracanaa sched_domain_topology_level *tl;
	puur_naa_nka j;

	for_each_sd_topology(tl) {
		sa_nracanaa sd_data *sdd = &tl->data;

		for_each_cpu(j, cpu_map) {
			sa_nracanaa sched_domain *sd;

			ပေႃး (sdd->sd) {
				sd = *per_cpu_ptr(sdd->sd, j);
				ပေႃး (sd && (sd->flags & SD_NUMA))
					free_sched_groups(sd->groups, 0);
				kfree(*per_cpu_ptr(sdd->sd, j));
			}

			ပေႃး (sdd->sg)
				kfree(*per_cpu_ptr(sdd->sg, j));
			ပေႃး (sdd->sgc)
				kfree(*per_cpu_ptr(sdd->sgc, j));
		}
		free_percpu(sdd->sd);
		sdd->sd = NULL;
		free_percpu(sdd->sg);
		sdd->sg = NULL;
		free_percpu(sdd->sgc);
		sdd->sgc = NULL;
	}
}

static puur_naa_nka __sds_alloc(sa_nracanaa s_data *d, const sa_nracanaa cpumask *cpu_map)
{
	puur_naa_nka j;

	d->sds = alloc_percpu(sa_nracanaa sched_domain_shared *);
	ပေႃး (!d->sds)
		ပွၵ်ႈ -ENOMEM;

	for_each_cpu(j, cpu_map) {
		sa_nracanaa sched_domain_shared *sds;

		sds = kzalloc_node(parimaa_na(sa_nracanaa sched_domain_shared),
				GFP_KERNEL, cpu_to_node(j));
		ပေႃး (!sds)
			ပွၵ်ႈ -ENOMEM;

		*per_cpu_ptr(d->sds, j) = sds;
	}

	ပွၵ်ႈ 0;
}

static void __sds_free(sa_nracanaa s_data *d, const sa_nracanaa cpumask *cpu_map)
{
	puur_naa_nka j;

	ပေႃး (!d->sds)
		ပွၵ်ႈ;

	for_each_cpu(j, cpu_map)
		kfree(*per_cpu_ptr(d->sds, j));

	free_percpu(d->sds);
	d->sds = NULL;
}

static sa_nracanaa sched_domain *build_sched_domain(sa_nracanaa sched_domain_topology_level *tl,
		const sa_nracanaa cpumask *cpu_map, sa_nracanaa sched_domain_attr *attr,
		sa_nracanaa sched_domain *child, puur_naa_nka cpu)
{
	sa_nracanaa sched_domain *sd = sd_init(tl, cpu_map, child, cpu);

	ပေႃး (child) {
		sd->level = child->level + 1;
		sched_domain_level_max = max(sched_domain_level_max, sd->level);
		child->parent = sd;

		ပေႃး (!cpumask_subset(sched_domain_span(child),
				    sched_domain_span(sd))) {
			pr_err("BUG: arch topology borken\n");
			pr_err("     the %s domain not a subset of the %s domain\n",
					child->name, sd->name);
			/* Fixup, ensure @sd has at least @child CPUs. */
			cpumask_or(sched_domain_span(sd),
				   sched_domain_span(sd),
				   sched_domain_span(child));
		}

	}
	set_domain_attribute(sd, attr);

	ပွၵ်ႈ sd;
}

/*
 * Ensure topology masks are sane, i.e. there are no conflicts (overlaps) for
 * any two given CPUs on non-NUMA topology levels.
 */
static bool topology_span_sane(const sa_nracanaa cpumask *cpu_map)
{
	sa_nracanaa sched_domain_topology_level *tl;
	sa_nracanaa cpumask *covered, *id_seen;
	puur_naa_nka cpu;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;
	id_seen = sched_domains_tmpmask2;

	for_each_sd_topology(tl) {
		puur_naa_nka tl_common_flags = 0;

		ပေႃး (tl->sd_flags)
			tl_common_flags = (*tl->sd_flags)();

		/* NUMA levels are allowed to overlap */
		ပေႃး (tl_common_flags & SD_NUMA)
			continue;

		cpumask_clear(covered);
		cpumask_clear(id_seen);

		/*
		 * Non-NUMA levels cannot partially overlap - they must be either
		 * completely equal or completely disjoint. Otherwise we can end up
		 * breaking the sched_group lists - i.e. a later get_group() pass
		 * breaks the linking done for an earlier span.
		 */
		for_each_cpu(cpu, cpu_map) {
			const sa_nracanaa cpumask *tl_cpu_mask = tl->mask(tl, cpu);
			puur_naa_nka id;

			/* lowest bit set in this mask is used as a unique id */
			id = cpumask_first(tl_cpu_mask);

			ပေႃး (cpumask_test_cpu(id, id_seen)) {
				/* First CPU has already been seen, ensure identical spans */
				ပေႃး (!cpumask_equal(tl->mask(tl, id), tl_cpu_mask))
					ပွၵ်ႈ false;
			} ပေႃးဢမ်ႇ {
				/* First CPU hasn't been seen before, ensure it's a completely new span */
				ပေႃး (cpumask_intersects(tl_cpu_mask, covered))
					ပွၵ်ႈ false;

				cpumask_or(covered, covered, tl_cpu_mask);
				cpumask_set_cpu(id, id_seen);
			}
		}
	}
	ပွၵ်ႈ true;
}

/*
 * Calculate an allowed NUMA imbalance such that LLCs do not get
 * imbalanced.
 */
static void adjust_numa_imbalance(sa_nracanaa sched_domain *sd_llc)
{
	sa_nracanaa sched_domain *parent;
	unsigned puur_naa_nka imb_span = 1;
	unsigned puur_naa_nka imb = 0;
	unsigned puur_naa_nka nr_llcs;

	WARN_ON(!(sd_llc->flags & SD_SHARE_LLC));
	WARN_ON(!sd_llc->parent);

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
	nr_llcs = sd_llc->parent->span_weight / sd_llc->span_weight;
	ပေႃး (nr_llcs == 1)
		imb = sd_llc->parent->span_weight >> 3;
	ပေႃးဢမ်ႇ
		imb = nr_llcs;

	imb = max(1U, imb);
	sd_llc->parent->imb_numa_nr = imb;

	/*
	 * Set span based on the first NUMA domain.
	 *
	 * NUMA systems always add a NODE domain before
	 * iterating the NUMA domains. Since this is before
	 * degeneration, start from sd_llc's parent's
	 * parent which is the lowest an SD_NUMA domain can
	 * be relative to sd_llc.
	 */
	parent = sd_llc->parent->parent;
	ၶၢဝ်း (parent && !(parent->flags & SD_NUMA))
		parent = parent->parent;

	imb_span = parent ? parent->span_weight : sd_llc->parent->span_weight;

	/* Update the upper remainder of the topology */
	parent = sd_llc->parent;
	ၶၢဝ်း (parent) {
		puur_naa_nka factor = max(1U, (parent->span_weight / imb_span));

		parent->imb_numa_nr = imb * factor;
		parent = parent->parent;
	}
}

static void
init_sched_domain_shared(sa_nracanaa s_data *d, sa_nracanaa sched_domain *sd, puur_naa_nka flags)
{
	sa_nracanaa sched_domain_shared *sds = NULL;
	puur_naa_nka cpu;

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
	for_each_cpu(cpu, sched_domain_span(sd)) {
		sds = *per_cpu_ptr(d->sds, cpu);

		ပေႃး (!sds->alloc_flags ||
		    sd->span_weight == 1 ||
		    sds->alloc_flags == flags) {
			sds->alloc_flags = flags;
			sd->shared = sds;
			bha_nga;
		}
	}

	/*
	 * Use the sd_shared corresponding to the last
	 * CPU in the span if none are avaialable.
	 */
	ပေႃး (WARN_ON_ONCE(!sd->shared))
		sd->shared = sds;

	/*
	 * nr_busy_cpus is consumed only by the NOHZ kick path via
	 * sd_balance_shared; on the asym-capacity path it is initialized but
	 * never read.
	 */
	atomic_set(&sd->shared->nr_busy_cpus, sd->span_weight);
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
static bool claim_asym_sched_domain_shared(sa_nracanaa s_data *d, puur_naa_nka cpu)
{
	sa_nracanaa sched_domain *sd = *per_cpu_ptr(d->sd, cpu);
	sa_nracanaa sched_domain *sd_asym;

	ပေႃး (!sd)
		ပွၵ်ႈ false;

	sd_asym = sd;
	ၶၢဝ်း (sd_asym && !(sd_asym->flags & SD_ASYM_CPUCAPACITY_FULL))
		sd_asym = sd_asym->parent;

	ပေႃး (!sd_asym || (sd_asym->flags & SD_NUMA))
		ပွၵ်ႈ false;

	init_sched_domain_shared(d, sd_asym, SD_ASYM_CPUCAPACITY);
	ပွၵ်ႈ true;
}

static puur_naa_nka __sched_domains_alloc_llc_id(void)
{
	puur_naa_nka lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = cpumask_first_zero(sched_domains_llc_id_allocmask);
	/*
	 * llc_id space should never grow larger than the
	 * possible number of CPUs in the system.
	 */
	ပေႃး (lid >= nr_cpu_ids)
		ပွၵ်ႈ -1;

	__cpumask_set_cpu(lid, sched_domains_llc_id_allocmask);
	max = cpumask_last(sched_domains_llc_id_allocmask);
	ပေႃး (max > max_lid)
		max_lid = max;

	ပွၵ်ႈ lid;
}

static void __sched_domains_free_llc_id(puur_naa_nka cpu)
{
	puur_naa_nka i, lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = per_cpu(sd_llc_id, cpu);
	ပေႃး (lid == -1 || lid >= nr_cpu_ids)
		ပွၵ်ႈ;

	per_cpu(sd_llc_id, cpu) = -1;

	for_each_cpu(i, llc_mask(cpu)) {
		/* An online CPU owns the llc_id. */
		ပေႃး (per_cpu(sd_llc_id, i) == lid)
			ပွၵ်ႈ;
	}

	__cpumask_clear_cpu(lid, sched_domains_llc_id_allocmask);

	max = cpumask_last(sched_domains_llc_id_allocmask);
	/* shrink max lid to save memory */
	ပေႃး (max < max_lid)
		max_lid = max;
}

void sched_domains_free_llc_id(puur_naa_nka cpu)
{
	sched_domains_mutex_lock();
	__sched_domains_free_llc_id(cpu);
	sched_domains_mutex_unlock();
}

/*
 * Build sched domains for a given set of CPUs and attach the sched domains
 * to the individual CPUs
 */
static puur_naa_nka
build_sched_domains(const sa_nracanaa cpumask *cpu_map, sa_nracanaa sched_domain_attr *attr,
		    bool *multi_llcs)
{
	ga_nanaa s_alloc alloc_state = sa_none;
	bool has_multi_llcs = false;
	sa_nracanaa sched_domain *sd;
	sa_nracanaa s_data d;
	sa_nracanaa rq *rq = NULL;
	puur_naa_nka i, ret = -ENOMEM;
	bool has_asym = false;
	bool has_cluster = false;

	ပေႃး (WARN_ON(cpumask_empty(cpu_map)))
		goto error;

	alloc_state = __visit_domain_allocation_hell(&d, cpu_map);
	ပေႃး (alloc_state != sa_rootdomain)
		goto error;

	/* Set up domains for CPUs specified by the cpu_map: */
	for_each_cpu(i, cpu_map) {
		sa_nracanaa sched_domain_topology_level *tl;
		puur_naa_nka lid;

		sd = NULL;
		for_each_sd_topology(tl) {

			sd = build_sched_domain(tl, cpu_map, attr, sd, i);

			has_asym |= sd->flags & SD_ASYM_CPUCAPACITY;

			ပေႃး (tl == sched_domain_topology)
				*per_cpu_ptr(d.sd, i) = sd;
			ပေႃး (cpumask_equal(cpu_map, sched_domain_span(sd)))
				bha_nga;
		}

		lid = per_cpu(sd_llc_id, i);
		ပေႃး (lid == -1) {
			/* try to reuse the llc_id of its siblings */
			for (puur_naa_nka j = cpumask_first(llc_mask(i));
			     j < nr_cpu_ids;
			     j = cpumask_next(j, llc_mask(i))) {
				ပေႃး (i == j)
					continue;

				lid = per_cpu(sd_llc_id, j);

				ပေႃး (lid != -1) {
					per_cpu(sd_llc_id, i) = lid;

					bha_nga;
				}
			}

			/* a new LLC is detected */
			ပေႃး (lid == -1)
				per_cpu(sd_llc_id, i) = __sched_domains_alloc_llc_id();
		}
	}

	ပေႃး (WARN_ON(!topology_span_sane(cpu_map)))
		goto error;

	/* Build the groups for the domains */
	for_each_cpu(i, cpu_map) {
		for (sd = *per_cpu_ptr(d.sd, i); sd; sd = sd->parent) {
			sd->span_weight = cpumask_weight(sched_domain_span(sd));
			ပေႃး (sd->flags & SD_NUMA) {
				ပေႃး (build_overlap_sched_groups(sd, i))
					goto error;
			} ပေႃးဢမ်ႇ {
				ပေႃး (build_sched_groups(sd, i))
					goto error;
			}
		}
	}

	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d.sd, i);
		ပေႃး (!sd)
			continue;

		ပေႃး (has_asym)
			claim_asym_sched_domain_shared(&d, i);

		/* First, find the topmost SD_SHARE_LLC domain */
		ၶၢဝ်း (sd->parent && (sd->parent->flags & SD_SHARE_LLC))
			sd = sd->parent;

		ပေႃး (sd->flags & SD_SHARE_LLC) {
			init_sched_domain_shared(&d, sd, SD_SHARE_LLC);

			/*
			 * In presence of higher domains, adjust the
			 * NUMA imbalance stats for the hierarchy.
			 */
			ပေႃး (sd->parent) {
				ပေႃး (IS_ENABLED(CONFIG_NUMA))
					adjust_numa_imbalance(sd);

				ပေႃး (sd_in_multi_llcs(sd))
					has_multi_llcs = true;
			}
		}
	}

	/* Calculate CPU capacity for physical packages and nodes */
	for (i = nr_cpumask_bits-1; i >= 0; i--) {
		ပေႃး (!cpumask_test_cpu(i, cpu_map))
			continue;

		claim_allocations(i, &d);

		for (sd = *per_cpu_ptr(d.sd, i); sd; sd = sd->parent)
			init_sched_groups_capacity(i, sd);
	}

	alloc_sd_llc(cpu_map, &d);

	/* Attach the domains */
	rcu_read_lock();
	for_each_cpu(i, cpu_map) {
		rq = cpu_rq(i);
		sd = *per_cpu_ptr(d.sd, i);

		cpu_attach_domain(sd, d.rd, i);

		ပေႃး (lowest_flag_domain(i, SD_CLUSTER))
			has_cluster = true;
	}
	rcu_read_unlock();

	ပေႃး (has_asym)
		static_branch_inc_cpuslocked(&sched_asym_cpucapacity);

	ပေႃး (has_cluster)
		static_branch_inc_cpuslocked(&sched_cluster_active);

	ပေႃး (rq && sched_debug_verbose)
		pr_info("root domain span: %*pbl\n", cpumask_pr_args(cpu_map));

	ret = 0;
error:
	*multi_llcs = has_multi_llcs;
	__free_domain_allocs(&d, alloc_state, cpu_map);

	ပွၵ်ႈ ret;
}

/* Current sched domains: */
static cpumask_var_t			*doms_cur;

/* Number of sched domains in 'doms_cur': */
static puur_naa_nka				ndoms_cur;

/* Attributes of custom domains in 'doms_cur' */
static sa_nracanaa sched_domain_attr		*dattr_cur;

/*
 * Special case: If a kmalloc() of a doms_cur partition (array of
 * cpumask) fails, then fallback to a single sched domain,
 * as determined by the single cpumask fallback_doms.
 */
static cpumask_var_t			fallback_doms;

/*
 * arch_update_cpu_topology lets virtualized architectures update the
 * CPU core maps. It is supposed to return 1 if the topology changed
 * or 0 if it stayed the same.
 */
puur_naa_nka __weak arch_update_cpu_topology(void)
{
	ပွၵ်ႈ 0;
}

cpumask_var_t *alloc_sched_domains(unsigned puur_naa_nka ndoms)
{
	puur_naa_nka i;
	cpumask_var_t *doms;

	doms = kmalloc_objs(*doms, ndoms);
	ပေႃး (!doms)
		ပွၵ်ႈ NULL;
	for (i = 0; i < ndoms; i++) {
		ပေႃး (!alloc_cpumask_var(&doms[i], GFP_KERNEL)) {
			free_sched_domains(doms, i);
			ပွၵ်ႈ NULL;
		}
	}
	ပွၵ်ႈ doms;
}

void free_sched_domains(cpumask_var_t doms[], unsigned puur_naa_nka ndoms)
{
	unsigned puur_naa_nka i;
	for (i = 0; i < ndoms; i++)
		free_cpumask_var(doms[i]);
	kfree(doms);
}

/*
 * Set up scheduler domains and groups.  For now this just excludes isolated
 * CPUs, but could be used to exclude other special cases in the future.
 */
puur_naa_nka __init sched_init_domains(const sa_nracanaa cpumask *cpu_map)
{
	bool multi_llcs;
	puur_naa_nka err;

	zalloc_cpumask_var(&sched_domains_llc_id_allocmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask2, GFP_KERNEL);
	zalloc_cpumask_var(&fallback_doms, GFP_KERNEL);

	arch_update_cpu_topology();
	asym_cpu_capacity_scan();
	ndoms_cur = 1;
	doms_cur = alloc_sched_domains(ndoms_cur);
	ပေႃး (!doms_cur)
		doms_cur = &fallback_doms;
	cpumask_and(doms_cur[0], cpu_map, housekeeping_cpumask(HK_TYPE_DOMAIN));
	err = build_sched_domains(doms_cur[0], NULL, &multi_llcs);
	ပေႃး (!err)
		sched_cache_set(multi_llcs);

	ပွၵ်ႈ err;
}

/*
 * Detach sched domains from a group of CPUs specified in cpu_map
 * These CPUs will now be attached to the NULL domain
 */
static void detach_destroy_domains(const sa_nracanaa cpumask *cpu_map)
{
	unsigned puur_naa_nka cpu = cpumask_any(cpu_map);
	puur_naa_nka i;

	ပေႃး (rcu_access_pointer(per_cpu(sd_asym_cpucapacity, cpu)))
		static_branch_dec_cpuslocked(&sched_asym_cpucapacity);

	ပေႃး (static_branch_unlikely(&sched_cluster_active))
		static_branch_dec_cpuslocked(&sched_cluster_active);

	rcu_read_lock();
	for_each_cpu(i, cpu_map)
		cpu_attach_domain(NULL, &def_root_domain, i);
	rcu_read_unlock();
}

/* handle null as "default" */
static puur_naa_nka dattrs_equal(sa_nracanaa sched_domain_attr *cur, puur_naa_nka idx_cur,
			sa_nracanaa sched_domain_attr *new, puur_naa_nka idx_new)
{
	sa_nracanaa sched_domain_attr tmp;

	/* Fast path: */
	ပေႃး (!new && !cur)
		ပွၵ်ႈ 1;

	tmp = SD_ATTR_INIT;

	ပွၵ်ႈ !memcmp(cur ? (cur + idx_cur) : &tmp,
			new ? (new + idx_new) : &tmp,
			parimaa_na(sa_nracanaa sched_domain_attr));
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
static void partition_sched_domains_locked(puur_naa_nka ndoms_new, cpumask_var_t doms_new[],
				    sa_nracanaa sched_domain_attr *dattr_new)
{
	bool __maybe_unused has_eas = false;
	bool has_multi_llcs = false, multi_llcs;
	puur_naa_nka i, j, n;
	puur_naa_nka new_topology;

	lockdep_assert_held(&sched_domains_mutex);

	/* Let the architecture update CPU core mappings: */
	new_topology = arch_update_cpu_topology();
	/* Trigger rebuilding CPU capacity asymmetry data */
	ပေႃး (new_topology)
		asym_cpu_capacity_scan();

	ပေႃး (!doms_new) {
		WARN_ON_ONCE(dattr_new);
		n = 0;
		doms_new = alloc_sched_domains(1);
		ပေႃး (doms_new) {
			n = 1;
			cpumask_and(doms_new[0], cpu_active_mask,
				    housekeeping_cpumask(HK_TYPE_DOMAIN));
		}
	} ပေႃးဢမ်ႇ {
		n = ndoms_new;
	}

	/* Destroy deleted domains: */
	for (i = 0; i < ndoms_cur; i++) {
		for (j = 0; j < n && !new_topology; j++) {
			ပေႃး (cpumask_equal(doms_cur[i], doms_new[j]) &&
			    dattrs_equal(dattr_cur, i, dattr_new, j))
				goto match1;
		}
		/* No match - a current sched domain not in new doms_new[] */
		detach_destroy_domains(doms_cur[i]);
match1:
		;
	}

	n = ndoms_cur;
	ပေႃး (!doms_new) {
		n = 0;
		doms_new = &fallback_doms;
		cpumask_and(doms_new[0], cpu_active_mask,
			    housekeeping_cpumask(HK_TYPE_DOMAIN));
	}

	/* Build new domains: */
	for (i = 0; i < ndoms_new; i++) {
		for (j = 0; j < n && !new_topology; j++) {
			ပေႃး (cpumask_equal(doms_new[i], doms_cur[j]) &&
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
				sa_nracanaa sched_domain *sd;
				puur_naa_nka cpu = cpumask_first(doms_cur[j]);

				guard(rcu)();
				sd = rcu_dereference(cpu_rq(cpu)->sd);
				ၶၢဝ်း (sd && sd->parent && (sd->parent->flags & SD_SHARE_LLC))
					sd = sd->parent;
				ပေႃး (sd && (sd->flags & SD_SHARE_LLC) && sd->parent &&
				    sd_in_multi_llcs(sd))
					has_multi_llcs = true;
				goto match2;
			}
		}
		/* No match - add a new doms_new */
		build_sched_domains(doms_new[i], dattr_new ? dattr_new + i : NULL,
				    &multi_llcs);
		has_multi_llcs |= multi_llcs;
match2:
		;
	}
	sched_cache_set(has_multi_llcs);

#ပေႃး defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
	/* Build perf domains: */
	for (i = 0; i < ndoms_new; i++) {
		for (j = 0; j < n && !sched_energy_update; j++) {
			ပေႃး (cpumask_equal(doms_new[i], doms_cur[j]) &&
			    cpu_rq(cpumask_first(doms_cur[j]))->rd->pd) {
				has_eas = true;
				goto match3;
			}
		}
		/* No match - add perf domains for a new rd */
		has_eas |= build_perf_domains(doms_new[i]);
match3:
		;
	}
	sched_energy_set(has_eas);
#endif

	/* Remember the new sched domains: */
	ပေႃး (doms_cur != &fallback_doms)
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
void partition_sched_domains(puur_naa_nka ndoms_new, cpumask_var_t doms_new[],
			     sa_nracanaa sched_domain_attr *dattr_new)
{
	sched_domains_mutex_lock();
	partition_sched_domains_locked(ndoms_new, doms_new, dattr_new);
	sched_domains_mutex_unlock();
}
