/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapoggi <rapoggi@student.42perpignan.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 17:37:34 by rapoggi           #+#    #+#             */
/*   Updated: 2026/05/17 20:10:05 by rapoggi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H

# include <pthread.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_parameters
{
	int				nb_cdrs;
	int				tt_burnout;
	int				tt_compile;
	int				tt_debug;
	int				tt_refactor;
	int				nb_compiles_required;
	int				dngl_cooldown;
	char			*scheduler;
}			t_parameters;

typedef struct s_cdr_info
{
	int				cdr_id;
	int				nearby_dngls[2];
	int				owned_dngls[2];
	int				compiles_done;
	int				last_compile;
	pthread_cond_t	cond;
}		t_cdr_info;

typedef struct s_dngl_info
{
	int				dngl_id;
	int				owned;
	int				released_time;
	pthread_mutex_t	lock;
}					t_dngl_info;

typedef struct s_request
{
	t_cdr_info			*cdr;
	int					deadline;
	struct s_request	*next;
	struct s_request	*prev;
}						t_request;

typedef struct s_queue
{
	t_request		*node;
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
}					t_queue;

typedef struct s_system
{
	t_parameters	*parameters;
	t_cdr_info		*cdrs_info;
	t_queue			*queue;
	int				running;
}					t_system;

typedef struct s_cdr_wrpr
{
	t_system	*system;
	t_cdr_info	*cdr;
}				t_cdr_wrpr;

t_system		*ft_init_system(t_system *system, int argc, char **argv);
t_parameters	*ft_parse(int argc, char **argv);
t_cdr_info		*ft_generate_coders_info(t_parameters *parameters);
void			ft_create_request(t_queue *queue, t_cdr_info *cdr, int tt_bo);
t_request		*ft_get_priority_req(char *scheduler, t_queue *queue);
void			ft_pop_node(t_queue *queue, t_request *node);
void			ft_push_node(t_queue *queue, t_request *node);
int				ft_check_numbers(char **argv);
int				ft_check_str(char **argv);

#endif
