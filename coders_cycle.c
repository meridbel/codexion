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

void	run_compile(t_coder *coder)
{
	usleep(coder->data->time_to_compile * 1000);
	pthread_mutex_lock(&coder->data->state_mutex);
	coder->compile_count++;
	coder->last_compile = get_time_ms();
	pthread_mutex_unlock(&coder->data->state_mutex);
}

void	debug_coder(t_coder *coder)
{
	log_state(coder->data, coder->id, "is debugging");
	usleep(coder->data->time_to_debug * 1000);
}

void	refactor_coder(t_coder *coder)
{
	log_state(coder->data, coder->id, "is refactoring");
	usleep(coder->data->time_to_refactor * 1000);
}
