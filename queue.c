/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapoggi <rapoggi@student.42perpignan.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 18:25:23 by rapoggi           #+#    #+#             */
/*   Updated: 2026/05/19 14:54:09 by rapoggi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

t_request	*ft_get_priority_req(char *scheduler, t_queue *queue)
{
	t_request	*prior;
	t_request	*cur;

	prior = NULL;
	cur = queue->node;
	while (cur)
	{
		if (strcmp(scheduler, "fifo") == 0)
			prior = cur;
		else if (!prior || cur->deadline < prior->deadline)
			prior = cur;
		else if (cur->deadline == prior->deadline)
			if (cur->cdr->id < prior->cdr->id)
				prior = cur;
		cur = cur->next;
	}
	ft_pop_node(queue, prior);
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
