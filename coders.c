/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapoggi <rapoggi@student.42perpignan.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 17:33:55 by rapoggi           #+#    #+#             */
/*   Updated: 2026/05/17 20:31:04 by rapoggi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

t_cdr_info	*ft_generate_coders_info(t_parameters *param)
{
	t_cdr_info	*cdrs_info;
	t_cdr_info	*cdr;
	int			nb_cdrs;
	int			i;

	nb_cdrs = param->nb_cdrs;
	cdrs_info = (t_cdr_info *) malloc(sizeof(t_cdr_info) * nb_cdrs);
	i = 0;
	while (i < nb_cdrs)
	{
		cdr = &cdrs_info[i];
		cdr->cdr_id = i + 1;
		cdr->nearby_dngls[0] = i;
		cdr->nearby_dngls[1] = (i + nb_cdrs + 1) % nb_cdrs;
		cdr->owned_dngls[0] = 0;
		cdr->owned_dngls[1] = 0;
		cdr->compiles_done = 0;
		i++;
	}
	return (cdrs_info);
}

void	*coder(void *arg)
{
	t_system	*system;
	t_cdr_info	*self;

	system = ((t_cdr_wrpr *) arg)->system;
	self = ((t_cdr_wrpr *) arg)->cdr;
	ft_create_request(system->queue, self, system->parameters->tt_burnout);
	pthread_cond_wait(&self->cond, &system->queue->lock);
	printf("gotem");
	return (NULL);
}

void	ft_create_request(t_queue *queue, t_cdr_info *cdr, int tt_bo)
{
	t_request	*to_push;

	to_push = (t_request *) malloc(sizeof(t_request));
	to_push->cdr = cdr;
	to_push->deadline = cdr->last_compile + tt_bo;
	pthread_mutex_lock(&queue->lock);
	ft_push_node(queue, to_push);
	pthread_mutex_unlock(&queue->lock);
	pthread_cond_broadcast(&queue->cond);
	return ;
}
