/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhal <mbelrhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 00:00:00 by mbelrhal          #+#    #+#             */
/*   Updated: 2026/08/24 00:00:00 by mbelrhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	acquire_single(t_coder *coder)
{
	while (!is_stopped(coder->data))
	{
		if (try_take_single(coder->data, coder))
			log_state(coder->data, coder->id, "has taken a dongle");
		usleep(100);
	}
	return (0);
}

static void	log_acquired(t_data *data, int id)
{
	long	t;

	pthread_mutex_lock(&data->print_mutex);
	if (!is_stopped(data))
	{
		t = get_time_ms() - data->start_time;
		printf("%ld %d has taken a dongle\n", t, id);
		t = get_time_ms() - data->start_time;
		printf("%ld %d has taken a dongle\n", t, id);
		t = get_time_ms() - data->start_time;
		printf("%ld %d is compiling\n", t, id);
	}
	pthread_mutex_unlock(&data->print_mutex);
}

static int	acquire_dongles(t_coder *coder)
{
	int	a;
	int	b;

	dongle_pair(coder->data, coder->id, &a, &b);
	if (a == b)
		return (acquire_single(coder));
	coder->wait_value = coder_priority(coder);
	register_waiter(coder->data, coder);
	while (!is_stopped(coder->data))
	{
		if (try_take_both(coder->data, coder))
		{
			log_acquired(coder->data, coder->id);
			return (1);
		}
		usleep(100);
	}
	return (0);
}

static void	release_dongles(t_coder *coder)
{
	int	a;
	int	b;

	dongle_pair(coder->data, coder->id, &a, &b);
	dongle_release(coder->data, &coder->data->dongles[a]);
	if (a != b)
		dongle_release(coder->data, &coder->data->dongles[b]);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!is_stopped(coder->data)
		&& coder->compile_count < coder->data->number_of_compiles_required)
	{
		if (!acquire_dongles(coder))
			break ;
		pthread_mutex_lock(&coder->data->state_mutex);
		coder->last_compile_start = get_time_ms();
		pthread_mutex_unlock(&coder->data->state_mutex);
		run_compile(coder);
		release_dongles(coder);
		debug_coder(coder);
		refactor_coder(coder);
	}
	return (NULL);
}
