/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapoggi <rapoggi@student.42perpignan.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 17:38:03 by rapoggi           #+#    #+#             */
/*   Updated: 2026/05/20 01:43:56 by rapoggi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	main(int argc, char **argv)
{
	t_system	*system;

	system = NULL;
	system = ft_init_system(system, argc, argv);
	if (!system)
		return (1);
	ft_wait(system);
	ft_free(system);
	return (0);
}

void	ft_free(t_system *system)
{
	int	i;

	i = 0;
	while (i < system->param->nb_cdrs)
	{
		pthread_mutex_destroy(&system->cdrs_info[i].lock);
		pthread_cond_destroy(&system->cdrs_info[i].cond);
		pthread_mutex_destroy(&system->dngls_info[i].lock);
		i++;
	}
	free(system->cdrs_info);
	free(system->dngls_info);
	pthread_mutex_destroy(&system->queue->lock);
	pthread_cond_destroy(&system->queue->cond);
	free(system->queue);
	free(system->param);
	pthread_mutex_destroy(&system->output->lock);
	free(system->output);
	pthread_mutex_destroy(&system->lock);
	free(system);
}

int ft_get_running_state(t_system *sys)
{
	int state;

	pthread_mutex_lock(&sys->lock);
	state = sys->running;
	pthread_mutex_unlock(&sys->lock);
	return (state);
}

void ft_stop_system(t_system *sys)
{
	int i;

	pthread_mutex_lock(&sys->lock);
	sys->running = 0;
	pthread_mutex_unlock(&sys->lock);
	pthread_cond_broadcast(&sys->queue->cond);
	i = 0;
	while (i < sys->param->nb_cdrs)
	{
		pthread_cond_broadcast(&sys->cdrs_info[i].cond);
		i++;
	}
}

void	ft_wait(t_system *system)
{
	int	i;

	i = 0;
	while (i < system->param->nb_cdrs)
	{
		pthread_join(system->my_threads->coders[i], NULL);
		i++;
	}
	pthread_join(system->my_threads->orchestrator, NULL);
	pthread_join(system->my_threads->monitor, NULL);
}

t_system	*ft_init_system(t_system *sys, int argc, char **argv)
{
	sys = (t_system *) malloc(sizeof(t_system));
	if (!sys)
		return (NULL);
	sys->param = ft_parse(argc, argv);
	sys->dngls_info = ft_generate_dngls_info(sys->param);
	sys->cdrs_info = ft_generate_coders_info(sys->param, sys->dngls_info);
	sys->queue = (t_queue *) malloc(sizeof(t_queue));
	sys->queue->node = NULL;
	sys->output = (t_output *) malloc(sizeof(t_output));
	pthread_mutex_init(&sys->output->lock, NULL);
	pthread_mutex_init(&(sys->queue->lock), NULL);
	pthread_cond_init(&(sys->queue->cond), NULL);
	pthread_mutex_init(&(sys->lock), NULL);
	sys->start_time = ft_get_time();
	sys->running = 1;
	sys->my_threads = ft_init_threads(sys);
	return (sys);
}

t_my_threads	*ft_init_threads(t_system *system)
{
	t_my_threads	*my_threads;

	my_threads = (t_my_threads *) malloc(sizeof(t_my_threads));
	pthread_create(&my_threads->monitor, NULL, ft_monitor, system);
	pthread_create(&my_threads->orchestrator, NULL, ft_orchestrator, system);
	my_threads->coders = ft_init_coders(system);
	return my_threads;
}

int		ft_are_coders_done(t_system *sys)
{
	int	cdrs_done;
	int	i;
	int	nb_cdrs;

	nb_cdrs = sys->param->nb_cdrs;
	cdrs_done = 0;
	i = 0;
	while (i < nb_cdrs)
	{
		pthread_mutex_lock(&sys->cdrs_info[i].lock);
		if (sys->cdrs_info[i].compiles_done == sys->param->nb_compiles_required)
			cdrs_done++;
		pthread_mutex_unlock(&sys->cdrs_info[i].lock);
		i++;
	}
	if (cdrs_done == nb_cdrs)
		return (1);
	return (0);
}

void	*ft_monitor(void *arg)
{
	t_system	*sys;
	int			i;
	long long	last_compile;
	long long	now;

	sys = (t_system *) arg;
	now = 0;
	while (ft_get_running_state(sys))
	{
		i = 0;
		while (i < sys->param->nb_cdrs)
		{
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
				return (NULL);
			}
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

void	*ft_orchestrator(void *arg)
{
	t_system	*system;
	t_queue		*queue;
	t_request	*cur_req;
	t_cdr_info	*cdr;
	int			is_met;

	system = (t_system *) arg;
	queue = system->queue;
	while (ft_get_running_state(system))
	{
		pthread_mutex_lock(&queue->lock);
		while (queue->node == NULL && ft_get_running_state(system))
			pthread_cond_wait(&queue->cond, &queue->lock);
		if (!ft_get_running_state(system))
		{
			pthread_mutex_unlock(&queue->lock);
			break ;
		}
		cur_req = ft_get_priority_req(system->param->scheduler, queue);

		cdr = cur_req->cdr;
		pthread_mutex_unlock(&queue->lock);
		is_met = 0;
		while (!is_met)
		{
			ft_lock_dngls(cdr->dngls);
			if (ft_check_dngls(cdr->dngls))
			{
				ft_own_dngl(cdr->dngls);
				pthread_mutex_lock(&cdr->lock);
				cdr->req_met = 1;
				is_met = 1;
				pthread_cond_broadcast(&cdr->cond);
				pthread_mutex_unlock(&cdr->lock);
				ft_unlock_dngls(cdr->dngls);
			}
			else
				ft_unlock_dngls(cdr->dngls);
		}
		free(cur_req);
	}
	return (NULL);
}

long long	ft_get_time()
{
	struct timeval	tv;
	long long		time_in_ms;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	time_in_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time_in_ms);
}
