/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhal <mbelrhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 00:00:00 by mbelrhal          #+#    #+#             */
/*   Updated: 2026/08/24 00:00:00 by mbelrhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_number(char *str)
{
	int	i;
	int	digits;

	i = 0;
	digits = 0;
	while (str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		digits++;
		i++;
	}
	while (str[i] == ' ')
		i++;
	if (str[i] || !digits)
		return (1);
	return (0);
}

static long	ft_atol(char *str)
{
	int		i;
	long	num;
	int		signe;

	i = 0;
	num = 0;
	signe = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			signe = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = num * 10 + (str[i] - 48);
		i++;
	}
	return (num * signe);
}

static int	fill_data(t_data *data, char **av)
{
	char	*scheduler;

	data->number_of_coders = ft_atol(av[1]);
	data->time_to_burnout = ft_atol(av[2]);
	data->time_to_compile = ft_atol(av[3]);
	data->time_to_debug = ft_atol(av[4]);
	data->time_to_refactor = ft_atol(av[5]);
	data->number_of_compiles_required = ft_atol(av[6]);
	data->dongle_cooldown = ft_atol(av[7]);
	scheduler = ft_lower(av[8]);
	if (strcmp(scheduler, "fifo") == 0)
		data->scheduler = FIFO;
	else if (strcmp(scheduler, "edf") == 0)
		data->scheduler = EDF;
	else
		return (fprintf(stderr, "Last argument must be fifo or edf\n"), 1);
	return (0);
}

static int	validate_values(t_data *data)
{
	if (data->number_of_coders <= 0
		|| data->time_to_burnout <= 0
		|| data->time_to_compile <= 0
		|| data->time_to_debug <= 0
		|| data->time_to_refactor <= 0
		|| data->number_of_compiles_required <= 0
		|| data->dongle_cooldown < 0)
		return (fprintf(stderr, "Invalid values\n"), 1);
	return (0);
}

int	parse_args(t_data *data, int ac, char **av)
{
	int	i;

	i = 1;
	if (ac != 9)
		return (fprintf(stderr, "You must enter 9 arguments\n"), 1);
	while (i < 8)
	{
		if (is_number(av[i]))
			return (fprintf(stderr, "Invalid number\n"), 1);
		i++;
	}
	if (fill_data(data, av))
		return (1);
	if (validate_values(data))
		return (1);
	return (0);
}
