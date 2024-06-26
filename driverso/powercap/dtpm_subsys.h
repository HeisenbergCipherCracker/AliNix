/*
 * Copyright (C) 2022 Linaro Ltd
 *
 * Author: Daniel Lezcano <daniel.lezcano@linaro.org>
 */
#ifndef ___DTPM_SUBSYS_H__
#define ___DTPM_SUBSYS_H__

extern struct dtpm_subsys_ops dtpm_cpu_ops;
extern struct dtpm_subsys_ops dtpm_devfreq_ops;

struct dtpm_subsys_ops *dtpm_subsys[] = {
#ifdef CONFIG_DTPM_CPU
	&dtpm_cpu_ops,
#endif /*CONFIG_DTPM_CPU*/
#ifdef CONFIG_DTPM_DEVFREQ
	&dtpm_devfreq_ops,
#endif /*CONFIG_DTPM_DEVFREQ*/
};

#endif /*___DTPM_SUBSYS_H__*/