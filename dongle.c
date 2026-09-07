/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhal <mbelrhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 00:00:00 by mbelrhal          #+#    #+#             */
/*   Updated: 2026/08/24 00:00:00 by mbelrhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	dongle_ready(t_data *data, t_dongle *d)
{
	(void)data;
	return (!d->held && get_time_ms() >= d->release_time);
}

static int	waiter_is_unblocked(t_data *data, t_waiter *w)
{
	int	a;
	int	b;

	dongle_pair(data, w->coder_id, &a, &b);
	if (!dongle_ready(data, &data->dongles[a]))
		return (0);
	if (a != b && !dongle_ready(data, &data->dongles[b]))
		return (0);
	return (1);
}

static int	queue_blocks(t_data *data, t_dongle *d, t_waiter *self)
{
	int	i;

	i = 0;
	while (i < d->queue.size)
	{
		if (d->queue.items[i].coder_id != self->coder_id
			&& waiter_less(&d->queue.items[i], self)
			&& waiter_is_unblocked(data, &d->queue.items[i]))
			return (1);
		i++;
	}
	return (0);
}

static int	can_take_both(t_data *data, int a, int b, t_waiter *self)
{
	if (!dongle_ready(data, &data->dongles[a]))
		return (0);
	if (a != b && !dongle_ready(data, &data->dongles[b]))
		return (0);
	if (queue_blocks(data, &data->dongles[a], self))
		return (0);
	if (a != b && queue_blocks(data, &data->dongles[b], self))
		return (0);
	return (1);
}

int	try_take_both(t_data *data, t_coder *coder)
{
	int			a;
	int			b;
	t_waiter	self;

	dongle_pair(data, coder->id, &a, &b);
	self.coder_id = coder->id;
	self.value = coder->wait_value;
	pthread_mutex_lock(&data->dongle_mutex);
	if (!can_take_both(data, a, b, &self))
	{
		pthread_mutex_unlock(&data->dongle_mutex);
		return (0);
	}
	data->dongles[a].held = 1;
	if (a != b)
		data->dongles[b].held = 1;
	heap_remove(&data->dongles[a].queue, coder->id);
	if (a != b)
		heap_remove(&data->dongles[b].queue, coder->id);
	pthread_mutex_unlock(&data->dongle_mutex);
	return (1);
}
