/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapoggi <rapoggi@student.42perpignan.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 21:20:05 by rapoggi           #+#    #+#             */
/*   Updated: 2026/05/27 19:25:23 by rapoggi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

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
			if (dngl->free_at <= ft_get_time() && ft_grab_dongle(dngl, my_req))
				break ;
			else if (ft_wait_for_dongle_cooldown(dngl, sys))
				continue ;
		}
		pthread_cond_wait(&dngl->cond, &dngl->lock);
	}
	pthread_mutex_unlock(&dngl->lock);
}

int	ft_wait_for_dongle_cooldown(t_dngl_info *dngl, t_system *sys)
{
	long long	to_wait;

	to_wait = dngl->free_at - ft_get_time();
	pthread_mutex_unlock(&dngl->lock);
	ft_precise_usleep(to_wait, sys);
	pthread_mutex_lock(&dngl->lock);
	return (1);
}

int	ft_grab_dongle(t_dngl_info *dngl, t_request *req)
{
	dngl->held = 1;
	pthread_mutex_lock(&dngl->queue->lock);
	ft_pop_node(dngl->queue, req);
	pthread_mutex_unlock(&dngl->queue->lock);
	free(req);
	return (1);
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
