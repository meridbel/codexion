/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhal <mbelrhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 00:00:00 by mbelrhal          #+#    #+#             */
/*   Updated: 2026/08/24 00:00:00 by mbelrhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	dongle_pair(t_data *data, int coder_id, int *a, int *b)
{
	int	left;
	int	right;

	left = coder_id - 1;
	right = coder_id % data->number_of_coders;
	if (left < right)
	{
		*a = left;
		*b = right;
	}
	else
	{
		*a = right;
		*b = left;
	}
}

long	coder_priority(t_coder *coder)
{
	if (coder->data->scheduler == FIFO)
		return (get_time_ms());
	return (coder->last_compile_start + coder->data->time_to_burnout);
}

static int	is_waiting(t_heap *h, int coder_id)
{
	int	i;

	i = 0;
	while (i < h->size)
	{
		if (h->items[i].coder_id == coder_id)
			return (1);
		i++;
	}
	return (0);
}

void	register_waiter(t_data *data, t_coder *coder)
{
	t_waiter	w;
	int			a;
	int			b;

	dongle_pair(data, coder->id, &a, &b);
	w.coder_id = coder->id;
	w.value = coder->wait_value;
	pthread_mutex_lock(&data->dongle_mutex);
	if (!is_waiting(&data->dongles[a].queue, coder->id))
		heap_push(&data->dongles[a].queue, w);
	if (a != b && !is_waiting(&data->dongles[b].queue, coder->id))
		heap_push(&data->dongles[b].queue, w);
	pthread_mutex_unlock(&data->dongle_mutex);
}

void	dongle_release(t_data *data, t_dongle *d)
{
	pthread_mutex_lock(&data->dongle_mutex);
	d->held = 0;
	d->release_time = get_time_ms() + data->dongle_cooldown;
	pthread_mutex_unlock(&data->dongle_mutex);
}
