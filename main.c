/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhal <mbelrhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 00:00:00 by mbelrhal          #+#    #+#             */
/*   Updated: 2026/08/24 00:00:00 by mbelrhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	create_coders(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		if (pthread_create(&data->coders[i].thread, NULL,
				coder_routine, &data->coders[i]) != 0)
		{
			printf("Failed to create coder %d\n", i + 1);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	join_coders(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		if (pthread_join(data->coders[i].thread, NULL) != 0)
		{
			printf("Failed to join coder %d\n", i + 1);
			return (1);
		}
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_data		data;
	pthread_t	monitor;

	if (parse_args(&data, ac, av) != 0)
		return (1);
	if (init_data(&data) != 0)
		return (1);
	if (pthread_create(&monitor, NULL, monitor_routine, &data) != 0)
	{
		printf("Failed to create monitor thread\n");
		return (1);
	}
	if (create_coders(&data) || join_coders(&data))
		return (1);
	if (pthread_join(monitor, NULL) != 0)
	{
		printf("Failed to join monitor\n");
		return (1);
	}
	cleanup(&data);
	return (0);
}
