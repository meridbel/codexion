/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhal <mbelrhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 00:00:00 by mbelrhal          #+#    #+#             */
/*   Updated: 2026/08/24 00:00:00 by mbelrhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_dongles(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		data->dongles[i].held = 0;
		data->dongles[i].release_time = data->start_time;
		data->dongles[i].queue.size = 0;
		i++;
	}
	return (0);
}

static int	init_coders(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].compile_count = 0;
		data->coders[i].last_compile = data->start_time;
		data->coders[i].last_compile_start = data->start_time;
		data->coders[i].wait_value = 0;
		data->coders[i].data = data;
		i++;
	}
	return (0);
}

int	init_data(t_data *data)
{
	data->start_time = get_time_ms();
	data->stop = 0;
	if (allocate_memory(data))
		return (1);
	if (init_mutexes(data))
		return (1);
	init_dongles(data);
	init_coders(data);
	return (0);
}
