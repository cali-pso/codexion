/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapoggi <rapoggi@student.42perpignan.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 19:28:17 by rapoggi           #+#    #+#             */
/*   Updated: 2026/05/27 19:29:10 by rapoggi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

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

void	ft_free_queue(t_queue *queue)
{
	t_request	*cur;
	t_request	*prev;

	cur = queue->node;
	while (cur)
	{
		prev = cur;
		cur = cur->next;
		free(prev);
	}
	return ;
}
