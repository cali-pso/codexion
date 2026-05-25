#include "header.h"

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

int	ft_are_coders_done(t_system *sys)
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

long long	ft_get_time(void)
{
	struct timeval	tv;
	long long		time_in_ms;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	time_in_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time_in_ms);
}
