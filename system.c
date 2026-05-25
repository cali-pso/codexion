#include "header.h"

t_system	*ft_init_system(t_system *sys, int argc, char **argv)
{
	sys = (t_system *) malloc(sizeof(t_system));
	if (!sys)
		return (NULL);
	sys->param = ft_parse(argc, argv);
	sys->dngls_info = ft_generate_dngls_info(sys->param);
	sys->cdrs_info = ft_generate_cdrs_info(sys->param, sys->dngls_info);
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
	return (my_threads);
}

int	ft_get_running_state(t_system *sys)
{
	int	state;

	pthread_mutex_lock(&sys->lock);
	state = sys->running;
	pthread_mutex_unlock(&sys->lock);
	return (state);
}

void	ft_stop_system(t_system *sys)
{
	int	i;

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
