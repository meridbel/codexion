/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhal <mbelrhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 00:00:00 by mbelrhal          #+#    #+#             */
/*   Updated: 2026/08/24 00:00:00 by mbelrhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	allocate_memory(t_data *data)
{
	data->coders = malloc(sizeof(t_coder) * (data->number_of_coders));
	if (!data->coders)
		return (1);
	data->dongles = malloc(sizeof(t_dongle) * (data->number_of_coders));
	if (!data->dongles)
	{
		free(data->coders);
		return (1);
	}
	return (0);
}

int	init_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->print_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&data->stop_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&data->dongle_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&data->state_mutex, NULL))
		return (1);
	return (0);
}

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

char	*ft_lower(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] = str[i] + 32;
		i++;
	}
	return (str);
}

int	is_stopped(t_data *data)
{
	int	stopped;

	pthread_mutex_lock(&data->stop_mutex);
	stopped = data->stop;
	pthread_mutex_unlock(&data->stop_mutex);
	return (stopped);
}
