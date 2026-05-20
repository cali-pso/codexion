/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapoggi <rapoggi@student.42perpignan.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 17:33:55 by rapoggi           #+#    #+#             */
/*   Updated: 2026/05/20 01:43:57 by rapoggi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

t_cdr_info	*ft_generate_coders_info(t_parameters *param, t_dngl_info *dngls)
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
		cdr->id = i + 1;
		cdr->dngls[0] = &dngls[i];
		cdr->dngls[1] = &dngls[(i + nb_cdrs + 1) % nb_cdrs];
		cdr->compiles_done = 0;
		pthread_cond_init(&(cdr->cond), NULL);
		i++;
	}
	return (cdrs_info);
}

pthread_t	*ft_init_coders(t_system *system)
{ 
	pthread_t	*cdrs;
	t_cdr_wrpr	*cdr_arg;
	int			i;
	int			nb_cdrs;

	nb_cdrs = system->param->nb_cdrs;
	cdrs = (pthread_t *) malloc(sizeof(pthread_t) * nb_cdrs);
	i = 0;
	while (i < nb_cdrs)
	{
		cdr_arg = (t_cdr_wrpr *) malloc(sizeof(t_cdr_wrpr));
		cdr_arg->system = system;
		cdr_arg->cdr = &system->cdrs_info[i];
		pthread_create(&cdrs[i], NULL, ft_coder, cdr_arg);
		i++;
	}
	return (cdrs);
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
	printf("%lli %i is %s\n", now, cdr->id, state);
	pthread_mutex_unlock(&sys->output->lock);
}

void	*ft_coder(void *arg)
{
	t_system	*system;
	t_cdr_info	*self;

	system = ((t_cdr_wrpr *) arg)->system;
	self = ((t_cdr_wrpr *) arg)->cdr;
	while (self->compiles_done < system->param->nb_compiles_required)
	{
		pthread_mutex_lock(&system->queue->lock);
		ft_create_request(system->queue, self, system->param->tt_burnout);
		pthread_cond_broadcast(&system->queue->cond);
		pthread_mutex_unlock(&system->queue->lock);

		pthread_mutex_lock(&self->lock);
		while (self->req_met == 0 && system->running)
			pthread_cond_wait(&self->cond, &self->lock);
		ft_take_dngls((t_cdr_wrpr *) arg);
		ft_print_dngl_state((t_cdr_wrpr *) arg, "compiling");
		self->req_met = 0;
		pthread_mutex_unlock(&self->lock);
		usleep(system->param->tt_compile * 1000);
		self->last_compile = ft_get_time();
		ft_give_dngl(self->dngls);
		ft_unlock_dngls(self->dngls);

		self->compiles_done++;
		ft_print_dngl_state((t_cdr_wrpr *) arg, "debugging");
		usleep(system->param->tt_debug * 1000);
		ft_print_dngl_state((t_cdr_wrpr *) arg, "refactoring");
		usleep(system->param->tt_refactor * 1000);
	}

	free(arg);
	return (NULL);
}

t_request	*ft_create_request(t_queue *queue, t_cdr_info *cdr, int tt_bo)
{
	t_request	*to_push;

	to_push = (t_request *) malloc(sizeof(t_request));
	to_push->cdr = cdr;
	to_push->deadline = cdr->last_compile + tt_bo;
	ft_push_node(queue, to_push);
	return to_push;
}
