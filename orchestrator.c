#include "header.h"

void	*ft_orchestrator(void *arg)
{
	t_system	*system;
	t_cdr_info	*cdr;
	int			is_met;

	system = (t_system *) arg;
	while (ft_get_running_state(system))
	{
		cdr = ft_get_priority_coder(system);
		if (!cdr)
			break ;
		is_met = 0;
		while (!is_met)
		{
			ft_lock_dngls(cdr->dngls);
			if (ft_check_dngls(cdr->dngls))
				is_met = ft_satisfy_coder_request(cdr);
			else
				ft_unlock_dngls(cdr->dngls);
		}
		free(system->queue->cur_req);
	}
	return (NULL);
}

t_cdr_info	*ft_get_priority_coder(t_system *system)
{
	t_queue		*queue;
	t_cdr_info	*cdr;

	queue = system->queue;
	pthread_mutex_lock(&queue->lock);
	while (queue->node == NULL && ft_get_running_state(system))
		pthread_cond_wait(&queue->cond, &queue->lock);
	if (!ft_get_running_state(system))
	{
		pthread_mutex_unlock(&queue->lock);
		return (NULL);
	}
	queue->cur_req = ft_get_priority_req(system->param->scheduler, queue);
	cdr = queue->cur_req->cdr;
	pthread_mutex_unlock(&queue->lock);
	return (cdr);
}

int	ft_satisfy_coder_request(t_cdr_info *cdr)
{
	int	is_met;

	ft_own_dngl(cdr->dngls);
	pthread_mutex_lock(&cdr->lock);
	cdr->req_met = 1;
	is_met = 1;
	pthread_cond_broadcast(&cdr->cond);
	pthread_mutex_unlock(&cdr->lock);
	ft_unlock_dngls(cdr->dngls);
	return (is_met);
}
