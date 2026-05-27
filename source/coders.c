/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapoggi <rapoggi@student.42perpignan.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 17:33:55 by rapoggi           #+#    #+#             */
/*   Updated: 2026/05/27 15:38:44 by rapoggi          ###   ########.fr       */
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
		ft_wait_for_dongles((t_cdr_wrpr *) arg);
		if (!ft_compile((t_cdr_wrpr *) arg))
			break ;
		ft_give_back_dongles(self);
		if (!ft_debug((t_cdr_wrpr *) arg))
			break ;
		if (!ft_refactor((t_cdr_wrpr *) arg))
			break ;
	}
	free(arg);
	return (NULL);
}

void	ft_give_back_dongles(t_cdr_info *cdr)
{
	ft_give_dongle(cdr->dngls[1]);
	ft_give_dongle(cdr->dngls[0]);
}

void	ft_give_dongle(t_dngl_info *dngl)
{
	pthread_mutex_lock(&dngl->lock);
	dngl->held = 0;
	dngl->free_at = ft_get_time() + dngl->cooldown;
	pthread_cond_broadcast(&dngl->cond);
	pthread_mutex_unlock(&dngl->lock);
}

void	ft_wait_for_dongles(t_cdr_wrpr *wrpr)
{
	t_cdr_info	*cdr;

	cdr = wrpr->cdr;
	ft_take_dngl(cdr->dngls[0], wrpr);
	if (ft_get_running_state(wrpr->system))
		ft_print_took_dngl(wrpr);
	ft_take_dngl(cdr->dngls[1], wrpr);
	if (ft_get_running_state(wrpr->system))
		ft_print_took_dngl(wrpr);
}

void	ft_take_dngl(t_dngl_info *dngl, t_cdr_wrpr *wrpr)
{
	t_cdr_info	*cdr;
	t_system	*sys;
	t_request	*my_req;
	t_request	*prio_req;
	long long	to_wait;

	cdr = wrpr->cdr;
	sys = wrpr->system;
	my_req = ft_create_request(dngl->queue, cdr, sys->param->tt_burnout);
	pthread_mutex_lock(&dngl->lock);
	while (ft_get_running_state(sys))
	{
		pthread_mutex_lock(&dngl->queue->lock);
		prio_req = ft_get_priority_req(sys->param->scheduler, dngl->queue);
		pthread_mutex_unlock(&dngl->queue->lock);
		if (prio_req && prio_req->cdr->id == cdr->id && dngl->held == 0)
		{
			if (dngl->free_at <= ft_get_time())
			{
				dngl->held = 1;
				pthread_mutex_lock(&dngl->queue->lock);
				ft_pop_node(dngl->queue, my_req);
				pthread_mutex_unlock(&dngl->queue->lock);
				free(my_req);
				break ;
			}
			else
			{
				to_wait = dngl->free_at - ft_get_time();
				pthread_mutex_unlock(&dngl->lock);
				ft_precise_usleep(to_wait, sys);
				pthread_mutex_lock(&dngl->lock);
				continue ;
			}
		}
		pthread_cond_wait(&dngl->cond, &dngl->lock);
	}
	pthread_mutex_unlock(&dngl->lock);
}

int	ft_got_dongle(t_dngl_info *dngl, t_cdr_info *cdr)
{
	if (&cdr->dngls[0] == &dngl && cdr->held_dngls[0] == 1)
		return (1);
	else if (&cdr->dngls[0] == &dngl)
		return (0);
	else if (&cdr->dngls[1] == &dngl && cdr->held_dngls[1] == 1)
		return (1);
	return (0);
}

int	ft_compile(t_cdr_wrpr *wrpr)
{
	t_system	*system;
	t_cdr_info	*self;

	system = wrpr->system;
	self = wrpr->cdr;
	if (!ft_get_running_state(system))
		return (0);
	ft_print_dngl_state(wrpr, "compiling");
	pthread_mutex_lock(&self->lock);
	self->last_compile = ft_get_time() - system->start_time;
	pthread_mutex_unlock(&self->lock);
	ft_precise_usleep(system->param->tt_compile, system);
	pthread_mutex_lock(&self->lock);
	self->compiles_done++;
	pthread_mutex_unlock(&self->lock);
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
	ft_precise_usleep(tt_debug, system);
	if (!ft_get_running_state(system))
		return (0);
	return (1);
}

int	ft_refactor(t_cdr_wrpr *wrpr)
{
	t_system	*system;

	system = wrpr->system;
	ft_print_dngl_state(wrpr, "refactoring");
	ft_precise_usleep(system->param->tt_refactor, system);
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
	to_push->timestamp = ft_get_time();
	to_push->next = NULL;
	to_push->prev = NULL;
	pthread_mutex_lock(&queue->lock);
	ft_push_node(queue, to_push);
	pthread_cond_broadcast(&queue->cond);
	pthread_mutex_unlock(&queue->lock);
	return (to_push);
}
