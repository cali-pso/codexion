/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapoggi <rapoggi@student.42perpignan.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 17:01:20 by rapoggi           #+#    #+#             */
/*   Updated: 2026/05/25 17:01:21 by rapoggi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	*ft_monitor(void *arg)
{
	t_system	*sys;
	int			i;

	sys = (t_system *) arg;
	while (ft_get_running_state(sys))
	{
		i = 0;
		while (i < sys->param->nb_cdrs)
		{
			if (ft_check_for_burnout(i, sys))
				return (NULL);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

int	ft_check_for_burnout(int i, t_system *sys)
{
	long long	last_compile;
	long long	now;

	now = ft_get_time() - sys->start_time;
	pthread_mutex_lock(&sys->cdrs_info[i].lock);
	last_compile = sys->cdrs_info[i].last_compile;
	pthread_mutex_unlock(&sys->cdrs_info[i].lock);
	if (now >= (last_compile + sys->param->tt_burnout))
	{
		if (!ft_are_coders_done(sys))
		{
			pthread_mutex_lock(&sys->output->lock);
			printf("%lli %i burned out\n", now, sys->cdrs_info[i].id);
			pthread_mutex_unlock(&sys->output->lock);
			ft_stop_system(sys);
		}
		else
			ft_stop_system(sys);
		return (1);
	}
	return (0);
}
