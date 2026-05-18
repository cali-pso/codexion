/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapoggi <rapoggi@student.42perpignan.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 17:38:03 by rapoggi           #+#    #+#             */
/*   Updated: 2026/05/18 22:17:08 by rapoggi          ###   ########.fr       */
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
	while (i < system->parameters->nb_cdrs)
	{
		pthread_join(coders[i], NULL);
		i++;
	}
	printf("coders done");
	pthread_join(*orchestr, NULL);
}

t_system	*ft_init_system(t_system *system, int argc, char **argv)
{
	system = (t_system *) malloc(sizeof(t_system));
	if (!system)
		return (NULL);
	system->parameters = ft_parse(argc, argv);
	system->cdrs_info = ft_generate_coders_info(system->parameters);
	system->queue = (t_queue *) malloc(sizeof(t_queue));
	system->queue->node = NULL;
	pthread_mutex_init(&(system->queue->lock), NULL);
	pthread_cond_init(&(system->queue->cond), NULL);
	system->running = 1;
	return (system);
}

void	*ft_orchestrator(void *arg)
{
	t_system	*system;
	t_queue		*queue;
	t_request	*cur_req;

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
		printf("orchestrator reaching for request %i left\n", ft_queue_len(queue));
		cur_req = ft_get_priority_req(system->parameters->scheduler, queue);
		ft_pop_node(queue, cur_req);
		printf("treating coder %i\n", cur_req->cdr->cdr_id);
		cur_req->cdr->dummy = 1;
		pthread_mutex_unlock(&queue->lock);
		pthread_cond_broadcast(&cur_req->cdr->cond);
		if (ft_queue_len(queue) == 0)
			system->running = 0;

	}
	return (NULL);
}
