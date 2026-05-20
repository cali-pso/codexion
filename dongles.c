/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapoggi <rapoggi@student.42perpignan.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 21:20:05 by rapoggi           #+#    #+#             */
/*   Updated: 2026/05/20 00:57:13 by rapoggi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

t_dngl_info	*ft_generate_dngls_info(t_parameters *param)
{
	t_dngl_info	*dngls;
	int			nb_cdrs;
	int			i;

	nb_cdrs = param->nb_cdrs;
	i = 0;
	dngls = (t_dngl_info *) malloc(sizeof(t_dngl_info) * nb_cdrs);
	while (i < nb_cdrs)
	{
		dngls[i].id = i;
		dngls[i].held = 0;
		dngls[i].free_at = ft_get_time() + param->dngl_cooldown;
		dngls[i].cooldown = param->dngl_cooldown;
		pthread_mutex_init(&dngls[i].lock, NULL);
		i++;
	}
	return (dngls);
}


void	ft_lock_dngls(t_dngl_info *dngls[2])
{
	if (dngls[0]->id < dngls[1]->id)
	{
        pthread_mutex_lock(&dngls[0]->lock);
        pthread_mutex_lock(&dngls[1]->lock);
    }
	else 
	{
        pthread_mutex_lock(&dngls[1]->lock);
        pthread_mutex_lock(&dngls[0]->lock);
    }
	return ;
}

void	ft_print_took_dngl(t_cdr_wrpr *wrpr)
{
	t_cdr_info	*cdr;
	t_system	*sys;
	long long	now;

	cdr = wrpr->cdr;
	sys = wrpr->system;
	now = ft_get_time() - sys->start_time;

	pthread_mutex_lock(&sys->output->lock);
	printf("%lli %i has taken a dongle\n", now, cdr->id);
	pthread_mutex_unlock(&sys->output->lock);
}

void	ft_take_dngls(t_cdr_wrpr *wrpr)
{
	t_cdr_info	*cdr;
	
	cdr = wrpr->cdr;

	if (cdr->dngls[0]->id < cdr->dngls[1]->id)
	{
        pthread_mutex_lock(&cdr->dngls[0]->lock);
		if (!wrpr->system->running)
			return ;
		ft_print_took_dngl(wrpr);
        pthread_mutex_lock(&cdr->dngls[1]->lock);
		if (!wrpr->system->running)
			return ;
		ft_print_took_dngl(wrpr);
    }
	else 
	{
        pthread_mutex_lock(&cdr->dngls[1]->lock);
		if (!wrpr->system->running)
			return ;
		ft_print_took_dngl(wrpr);
        pthread_mutex_lock(&cdr->dngls[0]->lock);
		if (!wrpr->system->running)
			return ;
		ft_print_took_dngl(wrpr);
    }
	return ;
}

void	ft_unlock_dngls(t_dngl_info *dngls[2])
{
	pthread_mutex_unlock(&dngls[0]->lock);
	pthread_mutex_unlock(&dngls[1]->lock);
}

int		ft_check_dngls(t_dngl_info *dngls[2])
{
	long long	now;

	now = ft_get_time();
	if (dngls[0]->held == 0 && dngls[0]->free_at <= now)
		if (dngls[1]->held == 0 && dngls[1]->free_at <= now)
			return (1);
	return (0);
}

void	ft_own_dngl(t_dngl_info *dngls[2])
{
	dngls[0]->held = 1;
	dngls[1]->held = 1;
	return ;
}

void ft_give_dngl(t_dngl_info *dngls[2])
{
	int i;

	i = 0;
	while (i < 2)
	{
		dngls[i]->held = 0;
		dngls[i]->free_at = ft_get_time() + dngls[i]->cooldown;
		i++;
	}
	return ;
}

