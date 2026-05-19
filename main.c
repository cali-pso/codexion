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
	pthread_t	orchestrator;
	pthread_t	*coders;

	system = NULL;
	system = ft_init_system(system, argc, argv);
	if (!system)
		return (1);
	pthread_create(&orchestrator, NULL, ft_orchestrator, system);
	coders = ft_init_coders(system);
	ft_wait(&orchestrator, coders, system);
	return (0);
}

void	ft_wait(pthread_t *orchestr, pthread_t *coders, t_system *system)
{
	int	i;

	i = 0;
	while (i < system->param->nb_cdrs)
	{
		pthread_join(coders[i], NULL);
		i++;
	}
	pthread_join(*orchestr, NULL);
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
	pthread_mutex_init(&(sys->queue->lock), NULL);
	pthread_cond_init(&(sys->queue->cond), NULL);
	pthread_mutex_init(&(sys->lock), NULL);
	sys->running = 1;
	return (sys);
}

void	*ft_orchestrator(void *arg)
{
	t_system	*system;
	t_queue		*queue;
	t_request	*cur_req;
	t_cdr_info	*cdr;
	t_request	*new_req;

	system = (t_system *) arg;
	queue = system->queue;
	while (system->running)
	{
		pthread_mutex_lock(&queue->lock);
		while (queue->node == NULL && system->running)
			pthread_cond_wait(&queue->cond, &queue->lock);
		if (!system->running)
		{
			pthread_mutex_unlock(&queue->lock);
			break ;
		}
		cur_req = ft_get_priority_req(system->param->scheduler, queue);
		ft_pop_node(queue, cur_req);
		pthread_mutex_unlock(&queue->lock);
		// p

		cdr = cur_req->cdr;
		printf("\npicked up request from %i\n", cdr->id);
		while (!cdr->req_met)
		{
			ft_lock_dngls(cdr->dngls);
			if (ft_check_dngls(cdr->dngls))
			{
				ft_unlock_dngls(cdr->dngls);
				cdr->req_met = 1;
				pthread_cond_broadcast(&cdr->cond);
			}
			else
			{
				ft_unlock_dngls(cdr->dngls);
				pthread_mutex_lock(&queue->lock);
				while (queue->node == NULL && system->running)
					pthread_cond_wait(&queue->cond, &queue->lock);
				new_req = ft_get_priority_req(system->param->scheduler, queue);
				printf("\nre------picked %i\n\n", new_req->cdr->id);
				ft_push_node(queue, cur_req);
				pthread_mutex_unlock(&queue->lock);
				cur_req = new_req;
				cdr = cur_req->cdr;
			}
		}
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
