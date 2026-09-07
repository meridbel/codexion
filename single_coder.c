/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_coder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhal <mbelrhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 00:00:00 by mbelrhal          #+#    #+#             */
/*   Updated: 2026/08/24 00:00:00 by mbelrhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	try_take_single(t_data *data, t_coder *coder)
{
	int	a;
	int	b;

	dongle_pair(data, coder->id, &a, &b);
	(void)b;
	pthread_mutex_lock(&data->dongle_mutex);
	if (!dongle_ready(data, &data->dongles[a]))
	{
		pthread_mutex_unlock(&data->dongle_mutex);
		return (0);
	}
	data->dongles[a].held = 1;
	pthread_mutex_unlock(&data->dongle_mutex);
	return (1);
}
