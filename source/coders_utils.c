/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapoggi <rapoggi@student.42perpignan.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 17:01:03 by rapoggi           #+#    #+#             */
/*   Updated: 2026/05/27 15:34:46 by rapoggi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

t_cdr_info	*ft_generate_cdrs_info(t_parameters *prm, t_dngl_info *dngls)
{
	t_cdr_info	*cdrs_info;
	t_cdr_info	*cdr;
	int			left;
	int			right;
	int			i;

	cdrs_info = (t_cdr_info *) malloc(sizeof(t_cdr_info) * prm->nb_cdrs);
	i = 0;
	while (i < prm->nb_cdrs)
	{
		cdr = &cdrs_info[i];
		cdr->id = i + 1;
		right = i;
		left = (i + prm->nb_cdrs + 1) % prm->nb_cdrs;
		if (right > left)
		{
			cdr->dngls[0] = &dngls[left];
			cdr->dngls[1] = &dngls[right];
		}
		else
		{
			cdr->dngls[0] = &dngls[right];
			cdr->dngls[1] = &dngls[left];
		}
		cdr->last_compile = 0;
		cdr->compiles_done = 0;
		cdr->req_met = 0;
		pthread_cond_init(&(cdr->cond), NULL);
		pthread_mutex_init(&(cdr->lock), NULL);
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

int	ft_is_coder_done(t_cdr_info *cdr, t_system *sys)
{
	if (cdr->compiles_done < sys->param->nb_compiles_required)
		if (ft_get_running_state(sys))
			return (1);
	return (0);
}

void    *ft_single_coder_routine(t_cdr_wrpr *wrpr)
{
    t_system    *sys;
    t_cdr_info  *cdr;

    sys = wrpr->system;
    cdr = wrpr->cdr;
    pthread_mutex_lock(&cdr->dngls[0]->lock);
    ft_print_took_dngl(wrpr);
    while (ft_get_running_state(sys))
        usleep(1000);
    pthread_mutex_unlock(&cdr->dngls[0]->lock);
    free(wrpr);
    return (NULL);
}
