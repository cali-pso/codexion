/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapoggi <rapoggi@student.42perpignan.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 18:25:23 by rapoggi           #+#    #+#             */
/*   Updated: 2026/05/27 15:05:32 by rapoggi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

t_request	*ft_get_priority_req(char *scheduler, t_queue *queue)
{
	t_request	*prior;
	t_request	*cur;

	cur = queue->node;
	prior = cur;
	if (strcmp(scheduler, "fifo") == 0)
	{
		while (cur)
		{
			if (prior->timestamp < cur->timestamp)
				prior = cur;
			else if (prior->timestamp == cur->timestamp)
				if (prior->cdr->id < cur->cdr->id)
					prior = cur;
			cur = cur->next;
		}
	}
	else if (strcmp(scheduler, "edf") == 0)
	{
		while (cur)
		{
			if (prior->deadline > cur->deadline)
				prior = cur;
			else if (prior->deadline == cur->deadline)
				if (prior->cdr->id < cur->cdr->id)
					prior = cur;
			cur = cur->next;
		}
	}
	return (prior);
}

t_request	*ft_get_queue_last(t_queue *queue)
{
	t_request	*last;

	last = queue->node;
	while (last && last->next)
		last = last->next;
	return (last);
}

void	ft_push_node(t_queue *queue, t_request *node)
{
	if (!queue)
		return ;
	else if (!queue->node)
		queue->node = node;
	else
	{
		queue->node->prev = node;
		node->next = queue->node;
		queue->node = node;
	}
	return ;
}

void	ft_pop_node(t_queue *queue, t_request *node)
{
	t_request	*next;
	t_request	*prev;

	if (!node)
		return ;
	next = node->next;
	prev = node->prev;
	if (prev)
		prev->next = next;
	if (next)
		next->prev = prev;
	if (queue->node == node)
		queue->node = next;
	return ;
}

int	ft_queue_len(t_queue *queue)
{
	int			i;
	t_request	*cur;

	i = 0;
	cur = queue->node;
	while (cur)
	{
		i++;
		cur = cur->next;
	}
	return (i);
}
