/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhal <mbelrhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 00:00:00 by mbelrhal          #+#    #+#             */
/*   Updated: 2026/08/24 00:00:00 by mbelrhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	swap(t_waiter *a, t_waiter *b)
{
	t_waiter	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int	waiter_less(t_waiter *a, t_waiter *b)
{
	if (a->value != b->value)
		return (a->value < b->value);
	return (a->coder_id < b->coder_id);
}

void	heap_push(t_heap *h, t_waiter w)
{
	int	i;
	int	parent;

	h->items[h->size] = w;
	i = h->size;
	h->size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!waiter_less(&h->items[i], &h->items[parent]))
			break ;
		swap(&h->items[i], &h->items[parent]);
		i = parent;
	}
}

static void	bubble_down(t_heap *h, int i)
{
	int	left;
	int	right;
	int	smallest;

	while (1)
	{
		left = i * 2 + 1;
		right = i * 2 + 2;
		smallest = i;
		if (left < h->size && waiter_less(&h->items[left], &h->items[smallest]))
			smallest = left;
		if (right < h->size
			&& waiter_less(&h->items[right], &h->items[smallest]))
			smallest = right;
		if (smallest == i)
			break ;
		swap(&h->items[i], &h->items[smallest]);
		i = smallest;
	}
}

void	heap_remove(t_heap *h, int coder_id)
{
	int	i;

	i = 0;
	while (i < h->size && h->items[i].coder_id != coder_id)
		i++;
	if (i == h->size)
		return ;
	h->size--;
	h->items[i] = h->items[h->size];
	bubble_down(h, i);
}
