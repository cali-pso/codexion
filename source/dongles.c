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

void	ft_unlock_dngls(t_dngl_info *dngls[2])
{
	pthread_mutex_unlock(&dngls[0]->lock);
	pthread_mutex_unlock(&dngls[1]->lock);
}

int	ft_check_dngls(t_dngl_info *dngls[2])
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

void	ft_give_dngl(t_dngl_info *dngls[2])
{
	int	i;

	i = 0;
	while (i < 2)
	{
		dngls[i]->held = 0;
		dngls[i]->free_at = ft_get_time() + dngls[i]->cooldown;
		i++;
	}
	return ;
}
