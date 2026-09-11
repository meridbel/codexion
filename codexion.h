/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhal <mbelrhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 00:00:00 by mbelrhal          #+#    #+#             */
/*   Updated: 2026/08/24 00:00:00 by mbelrhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/time.h>

# define FIFO 0
# define EDF 1

typedef struct s_waiter
{
	int		coder_id;
	long	value;
}	t_waiter;

typedef struct s_heap
{
	t_waiter	items[3];
	int			size;
}	t_heap;

typedef struct s_dongle
{
	int			held;
	long		release_time;
	t_heap		queue;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	long			last_compile;
	long			last_compile_start;
	long			wait_value;
	int				compile_count;
	pthread_t		thread;
	struct s_data	*data;
}	t_coder;

typedef struct s_data
{
	int				number_of_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				number_of_compiles_required;
	long			dongle_cooldown;
	int				scheduler;
	int				stop;
	long			start_time;
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	dongle_mutex;
	pthread_mutex_t	state_mutex;
}	t_data;

int		parse_args(t_data *data, int ac, char **av);
int		allocate_memory(t_data *data);
int		init_mutexes(t_data *data);
long	get_time_ms(void);
int		init_data(t_data *data);
void	*coder_routine(void *arg);
char	*ft_lower(char *str);
int		is_stopped(t_data *data);
void	run_compile(t_coder *coder);
void	debug_coder(t_coder *coder);
void	refactor_coder(t_coder *coder);
void	log_state(t_data *data, int id, const char *msg);
void	*monitor_routine(void *arg);
void	cleanup(t_data *data);
void	dongle_pair(t_data *data, int coder_id, int *a, int *b);
int		dongle_ready(t_data *data, t_dongle *d);
long	coder_priority(t_coder *coder);
void	heap_push(t_heap *h, t_waiter w);
void	heap_remove(t_heap *h, int coder_id);
int		waiter_less(t_waiter *a, t_waiter *b);
void	register_waiter(t_data *data, t_coder *coder);
void	dongle_release(t_data *data, t_dongle *d);
int		try_take_both(t_data *data, t_coder *coder);
int		try_take_single(t_data *data, t_coder *coder);
void	smart_sleep(long time, t_data *data);

# endif
