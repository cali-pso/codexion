/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapoggi <rapoggi@student.42perpignan.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 17:33:55 by rapoggi           #+#    #+#             */
/*   Updated: 2026/05/26 12:27:50 by rapoggi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	*ft_coder(void *arg)
{
	t_system	*system;
	t_cdr_info	*self;

	system = ((t_cdr_wrpr *) arg)->system;
	self = ((t_cdr_wrpr *) arg)->cdr;
	if (system->param->nb_cdrs == 1)
        return (ft_single_coder_routine((t_cdr_wrpr *) arg));
	while (ft_is_coder_done(self, system))
	{
		ft_create_request(system->queue, self, system->param->tt_burnout);
		pthread_mutex_lock(&self->lock);
		while (self->req_met == 0 && ft_get_running_state(system))
			pthread_cond_wait(&self->cond, &self->lock);
		pthread_mutex_unlock(&self->lock);
		if (!ft_compile((t_cdr_wrpr *) arg))
			break ;
		if (!ft_debug((t_cdr_wrpr *) arg))
			break ;
		if (!ft_refactor((t_cdr_wrpr *) arg))
			break ;
	}
	free(arg);
	return (NULL);
}

int	ft_refactor(t_cdr_wrpr *wrpr)
{
	t_system	*system;

	system = wrpr->system;
	ft_print_dngl_state(wrpr, "refactoring");
	usleep(system->param->tt_refactor * 1000);
	if (!ft_get_running_state(system))
		return (0);
	return (1);
}

int	ft_debug(t_cdr_wrpr *wrpr)
{
	t_system	*system;
	int			tt_debug;

	system = wrpr->system;
	ft_print_dngl_state(wrpr, "debugging");
	pthread_mutex_lock(&system->lock);
	tt_debug = system->param->tt_debug;
	pthread_mutex_unlock(&system->lock);
	usleep(tt_debug * 1000);
	if (!ft_get_running_state(system))
		return (0);
	return (1);
}

int	ft_compile(t_cdr_wrpr *wrpr)
{
	t_system	*system;
	t_cdr_info	*self;

	system = wrpr->system;
	self = wrpr->cdr;
	ft_take_dngls(wrpr);
	if (!ft_get_running_state(system))
		return (0);
	ft_print_dngl_state(wrpr, "compiling");
	pthread_mutex_lock(&self->lock);
	self->last_compile = ft_get_time() - system->start_time;
	self->req_met = 0;
	pthread_mutex_unlock(&self->lock);
	usleep(system->param->tt_compile * 1000);
	pthread_mutex_lock(&self->lock);
	self->compiles_done++;
	pthread_mutex_unlock(&self->lock);
	ft_give_dngl(self->dngls);
	ft_unlock_dngls(self->dngls);
	if (!ft_get_running_state(system))
		return (0);
	return (1);
}

t_request	*ft_create_request(t_queue *queue, t_cdr_info *cdr, int tt_bo)
{
	t_request	*to_push;

	to_push = (t_request *) malloc(sizeof(t_request));
	to_push->cdr = cdr;
	to_push->deadline = cdr->last_compile + tt_bo;
	to_push->next = NULL;
	to_push->prev = NULL;
	pthread_mutex_lock(&queue->lock);
	ft_push_node(queue, to_push);
	pthread_cond_broadcast(&queue->cond);
	pthread_mutex_unlock(&queue->lock);
	return (to_push);
}
