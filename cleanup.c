/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhal <mbelrhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 00:00:00 by mbelrhal          #+#    #+#             */
/*   Updated: 2026/08/24 00:00:00 by mbelrhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	cleanup(t_data *data)
{
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->stop_mutex);
	pthread_mutex_destroy(&data->dongle_mutex);
	pthread_mutex_destroy(&data->state_mutex);
	free(data->coders);
	free(data->dongles);
}
