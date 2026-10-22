/* SPDX-License-Identifier: GPL-2.0 */
#अगर_अघोषित _KERNEL_SCHED_AUTOGROUP_H
#घोषणा _KERNEL_SCHED_AUTOGROUP_H

#समावेश "sched.h"

#अगर_घोषित CONFIG_SCHED_AUTOGROUP

काष्ठा autogroup {
	/*
	 * Reference doesn't mean how many threads attach to this
	 * autogroup now. It just stands for the number of tasks
	 * which could use this autogroup.
	 */
	काष्ठा kref		kref;
	काष्ठा task_group	*tg;
	काष्ठा rw_semaphore	lock;
	अचिन्हित दीर्घ		id;
	पूर्णांक			nice;
};

बाह्य व्योम autogroup_init(काष्ठा task_struct *init_task);
बाह्य व्योम autogroup_free(काष्ठा task_group *tg);

अटल अंतरभूत bool task_group_is_autogroup(काष्ठा task_group *tg)
{
	वापस !!tg->autogroup;
}

बाह्य bool task_wants_autogroup(काष्ठा task_struct *p, काष्ठा task_group *tg);

अटल अंतरभूत काष्ठा task_group *
autogroup_task_group(काष्ठा task_struct *p, काष्ठा task_group *tg)
{
	बाह्य अचिन्हित पूर्णांक sysctl_sched_autogroup_enabled;
	पूर्णांक enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	अगर (enabled && task_wants_autogroup(p, tg))
		वापस p->संकेत->autogroup->tg;

	वापस tg;
}

बाह्य पूर्णांक autogroup_path(काष्ठा task_group *tg, अक्षर *buf, पूर्णांक buflen);

#अन्यथा /* !CONFIG_SCHED_AUTOGROUP: */

अटल अंतरभूत व्योम autogroup_init(काष्ठा task_struct *init_task) {  }
अटल अंतरभूत व्योम autogroup_free(काष्ठा task_group *tg) { }
अटल अंतरभूत bool task_group_is_autogroup(काष्ठा task_group *tg)
{
	वापस 0;
}

अटल अंतरभूत काष्ठा task_group *
autogroup_task_group(काष्ठा task_struct *p, काष्ठा task_group *tg)
{
	वापस tg;
}

अटल अंतरभूत पूर्णांक autogroup_path(काष्ठा task_group *tg, अक्षर *buf, पूर्णांक buflen)
{
	वापस 0;
}

#पूर्ण_अगर /* !CONFIG_SCHED_AUTOGROUP */

#पूर्ण_अगर /* _KERNEL_SCHED_AUTOGROUP_H */
