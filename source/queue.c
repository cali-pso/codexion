/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapoggi <rapoggi@student.42perpignan.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 18:25:23 by rapoggi           #+#    #+#             */
/*   Updated: 2026/05/27 19:28:04 by rapoggi          ###   ########.fr       */
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
		return (ft_get_fifo_req(cur));
	else if (strcmp(scheduler, "edf") == 0)
		return (ft_get_edf_req(cur));
	return (prior);
}

t_request	*ft_get_edf_req(t_request *cur)
{
	t_request	*prior;

	prior = cur;
	while (cur)
	{
		if (prior->deadline > cur->deadline)
			prior = cur;
		else if (prior->deadline == cur->deadline)
			if (prior->cdr->id < cur->cdr->id)
				prior = cur;
		cur = cur->next;
	}
	return (prior);
}

t_request	*ft_get_fifo_req(t_request *cur)
{
	t_request	*prior;

	prior = cur;
	while (cur)
	{
		if (prior->timestamp < cur->timestamp)
			prior = cur;
		else if (prior->timestamp == cur->timestamp)
			if (prior->cdr->id < cur->cdr->id)
				prior = cur;
		cur = cur->next;
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
