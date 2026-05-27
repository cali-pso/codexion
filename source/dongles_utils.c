/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapoggi <rapoggi@student.42perpignan.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 17:00:44 by rapoggi           #+#    #+#             */
/*   Updated: 2026/05/27 15:22:49 by rapoggi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

t_dngl_info	*ft_generate_dngls_info(t_parameters *prm)
{
	t_dngl_info	*dngls;
	int			nb_cdrs;
	int			i;

	nb_cdrs = prm->nb_cdrs;
	i = 0;
	dngls = (t_dngl_info *) malloc(sizeof(t_dngl_info) * nb_cdrs);
	while (i < nb_cdrs)
	{
		dngls[i].id = i;
		dngls[i].held = 0;
		dngls[i].cooldown = prm->dngl_cooldown;
		dngls[i].free_at = 0;
		dngls[i].queue = (t_queue *) malloc(sizeof(t_queue));
		dngls[i].queue->node = NULL;
		dngls[i].queue->cur_req = NULL;
		pthread_mutex_init(&dngls[i].queue->lock, NULL);
		pthread_cond_init(&dngls[i].queue->cond, NULL);
		pthread_cond_init(&dngls[i].cond, NULL);
		pthread_mutex_init(&dngls[i].lock, NULL);
		i++;
	}
	return (dngls);
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

void	ft_print_dngl_state(t_cdr_wrpr *wrpr, char *state)
{
	t_cdr_info	*cdr;
	t_system	*sys;
	long long	now;

	cdr = wrpr->cdr;
	sys = wrpr->system;
	now = ft_get_time() - sys->start_time;
	pthread_mutex_lock(&sys->output->lock);
	if (ft_get_running_state(sys))
		printf("%lli %i is %s\n", now, cdr->id, state);
	pthread_mutex_unlock(&sys->output->lock);
}
