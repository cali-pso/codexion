/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapoggi <rapoggi@student.42perpignan.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 17:38:03 by rapoggi           #+#    #+#             */
/*   Updated: 2026/05/17 19:57:01 by rapoggi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	main(int argc, char **argv)
{
	t_system	*system;

	system = ft_init_system(system, argc, argv);
	return (0);
}

t_system	*ft_init_system(t_system *system, int argc, char **argv)
{
	system = (t_system *) malloc(sizeof(t_system));
	if (!system)
		return (NULL);
	system->parameters = ft_parse(argc, argv);
	system->cdrs_info = ft_generate_coders_info(system->parameters);
	system->queue = (t_queue *) malloc(sizeof(t_queue));
	pthread_mutex_init(&(system->queue->lock), NULL);
	system->running = 0;
	return (system);
}

void	*orchestrator(void	*arg)
{
	t_system	*system;
	t_queue		*queue;
	t_request	*cur_req;
	int			req_dealt;

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
		printf("ok man\n");
////	cur_req = ft_get_priority_req(system->parameters->scheduler, queue);
////	req_dealt = 0;
////	while (cur_req != NULL)
////	{
////		// dispatch dongles
////	}
	}
	return (NULL);
}
