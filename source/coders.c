/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapoggi <rapoggi@student.42perpignan.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 17:33:55 by rapoggi           #+#    #+#             */
/*   Updated: 2026/05/26 19:00:06 by rapoggi          ###   ########.fr       */
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
		ft_wait_for_dongles(self, system);
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
	t_dngl_info **sort_dngls;

	sort_dngls = NULL;
	sort_dngls = ft_get_sorted_dongles(cdr->dngls, sort_dngls);
	ft_give_dongle(sort_dngls[0]);
	ft_give_dongle(sort_dngls[1]);
}

void	ft_give_dongle(t_dngl_info *dngl)
{
	pthread_mutex_lock(&dngl->lock);
	dngl->held = 0;
	dngl->free_at = ft_get_time() + dngl->cooldown;
	pthread_cond_broadcast(&dngl->cond);
	pthread_mutex_unlock(&dngl->lock);
}

void	ft_wait_for_dongles(t_cdr_info *cdr, t_system *system)
{
	t_dngl_info	**sort_dngls;

	sort_dngls = NULL;
	sort_dngls = ft_get_sorted_dongles(cdr->dngls, sort_dngls);
	ft_take_dngl(sort_dngls[0], cdr, system);
	ft_take_dngl(sort_dngls[1], cdr, system);
}

void	ft_take_dngl(t_dngl_info *dngl, t_cdr_info *cdr, t_system *sys)
{
	t_request	*my_req;
	t_request	*prio_req;
	long long	now;

	my_req = ft_create_request(dngl->queue, cdr, sys->param->tt_burnout);
	pthread_mutex_lock(&dngl->lock);
	while (ft_get_running_state(sys))
	{
		prio_req = ft_get_priority_req(sys->param->scheduler, dngl->queue);
		now = ft_get_time();
		if (&prio_req->cdr == &cdr && dngl->held == 0 && dngl->free_at <= now)
		{
			dngl->held = 1;
			ft_pop_node(dngl->queue, my_req);
			free(my_req);
			break ;
		}
		pthread_cond_wait(&dngl->cond, &dngl->lock);
	}
	pthread_mutex_unlock(&dngl->lock);
}

t_dngl_info	**ft_get_sorted_dongles(t_dngl_info *dngls[2], t_dngl_info **sort)
{
	sort = (t_dngl_info **) malloc(sizeof(t_dngl_info *) * 2);

	pthread_mutex_lock(&dngls[0]->lock);
	pthread_mutex_lock(&dngls[1]->lock);
	if (dngls[0]->id > dngls[1]->id)
	{
		sort[0] = dngls[0];
		sort[1] = dngls[1];
	}
	else
	{
		sort[0] = dngls[1];
		sort[1] = dngls[0];
	}
	pthread_mutex_unlock(&dngls[0]->lock);
	pthread_mutex_unlock(&dngls[1]->lock);
	return (sort);
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
	usleep(system->param->tt_compile * 1000);
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
	usleep(tt_debug * 1000);
	if (!ft_get_running_state(system))
		return (0);
	return (1);
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
