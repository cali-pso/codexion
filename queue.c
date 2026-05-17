/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapoggi <rapoggi@student.42perpignan.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 18:25:23 by rapoggi           #+#    #+#             */
/*   Updated: 2026/05/17 18:25:47 by rapoggi          ###   ########.fr       */
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
		cur = cur->next;
	}
	return (prior);
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
	free(node);
	return ;
}
