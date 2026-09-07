/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhal <mbelrhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 00:00:00 by mbelrhal          #+#    #+#             */
/*   Updated: 2026/08/24 00:00:00 by mbelrhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	all_done(t_data *data)
{
	int	i;
	int	done;

	i = 0;
	done = 1;
	pthread_mutex_lock(&data->state_mutex);
	while (i < data->number_of_coders)
	{
		if (data->coders[i].compile_count < data->number_of_compiles_required)
			done = 0;
		i++;
	}
	pthread_mutex_unlock(&data->state_mutex);
	return (done);
}

static void	log_burnout(t_data *data, int id)
{
	pthread_mutex_lock(&data->print_mutex);
	pthread_mutex_lock(&data->stop_mutex);
	data->stop = 1;
	pthread_mutex_unlock(&data->stop_mutex);
	printf("%ld %d burned out\n", get_time_ms() - data->start_time, id);
	pthread_mutex_unlock(&data->print_mutex);
}

static int	coder_overdue(t_data *data, int idx, long now)
{
	int		overdue;
	long	elapsed;

	pthread_mutex_lock(&data->state_mutex);
	if (data->coders[idx].compile_count >= data->number_of_compiles_required)
		overdue = 0;
	else
	{
		elapsed = now - data->coders[idx].last_compile_start;
		overdue = (elapsed > data->time_to_burnout);
	}
	pthread_mutex_unlock(&data->state_mutex);
	return (overdue);
}

static int	check_burnout(t_data *data)
{
	int		i;
	long	now;

	i = 0;
	while (i < data->number_of_coders)
	{
		now = get_time_ms();
		if (coder_overdue(data, i, now))
		{
			log_burnout(data, data->coders[i].id);
			return (1);
		}
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		if (all_done(data))
		{
			pthread_mutex_lock(&data->stop_mutex);
			data->stop = 1;
			pthread_mutex_unlock(&data->stop_mutex);
			break ;
		}
		if (check_burnout(data))
			return (NULL);
		usleep(500);
	}
	return (NULL);
}
