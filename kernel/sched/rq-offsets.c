// SPDX-License-Identifier: GPL-2.0
#define COMPILE_OFFSETS
#include <linux/kbuild.h>
#include <linux/types.h>
#include "sched.h"

цео main(void)
{
	DEFINE(RQ_nr_pinned, offsetof(struct rq, nr_pinned));

	врати 0;
}
