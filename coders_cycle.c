/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_cycle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhal <mbelrhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 00:00:00 by mbelrhal          #+#    #+#             */
/*   Updated: 2026/08/24 00:00:00 by mbelrhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_state(t_data *data, int id, const char *msg)
{
	pthread_mutex_lock(&data->print_mutex);
	if (!is_stopped(data))
		printf("%ld %d %s\n", get_time_ms() - data->start_time, id, msg);
	pthread_mutex_unlock(&data->print_mutex);
}

void	smart_sleep(long time, t_data *data)
{
	long	start;
	long	elapsed;
	long	remaining;

	start = get_time_ms();
	while (!is_stopped(data))
	{
		elapsed = get_time_ms() - start;
		if (elapsed >= time)
			break ;
		remaining = time - elapsed;
		if (remaining > 1)
			usleep(1000);
		else
			usleep(100);
	}
}

void	run_compile(t_coder *coder)
{
	smart_sleep(coder->data->time_to_compile, coder->data);
	if (is_stopped(coder->data))
		return ;
	pthread_mutex_lock(&coder->data->state_mutex);
	coder->compile_count++;
	coder->last_compile = get_time_ms();
	pthread_mutex_unlock(&coder->data->state_mutex);
}

void	debug_coder(t_coder *coder)
{
	log_state(coder->data, coder->id, "is debugging");
	smart_sleep(coder->data->time_to_debug, coder->data);
}

void	refactor_coder(t_coder *coder)
{
	log_state(coder->data, coder->id, "is refactoring");
	smart_sleep(coder->data->time_to_refactor, coder->data);
}