#include "header.h"

void	ft_take_dngls(t_cdr_wrpr *wrpr)
{
	t_cdr_info	*cdr;

	cdr = wrpr->cdr;
	if (cdr->dngls[0]->id < cdr->dngls[1]->id)
	{
		pthread_mutex_lock(&cdr->dngls[0]->lock);
		ft_print_took_dngl(wrpr);
		pthread_mutex_lock(&cdr->dngls[1]->lock);
		ft_print_took_dngl(wrpr);
	}
	else
	{
		pthread_mutex_lock(&cdr->dngls[1]->lock);
		ft_print_took_dngl(wrpr);
		pthread_mutex_lock(&cdr->dngls[0]->lock);
		ft_print_took_dngl(wrpr);
	}
	return ;
}

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
		dngls[i].free_at = 0;
		dngls[i].cooldown = prm->dngl_cooldown;
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
	printf("%lli %i is %s\n", now, cdr->id, state);
	pthread_mutex_unlock(&sys->output->lock);
}
