/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapoggi <rapoggi@student.42perpignan.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 17:37:34 by rapoggi           #+#    #+#             */
/*   Updated: 2026/05/26 18:51:31 by rapoggi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H

# include <pthread.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

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

typedef struct s_dngl_info
{
	int				id;
	int				held;
	long long		free_at;
	int				cooldown;
	struct s_queue	*queue;
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
}					t_dngl_info;

typedef struct s_cdr_info
{
	int				id;
	int				compiles_done;
	long long		last_compile;
	int				req_met;
	t_dngl_info		*dngls[2];
	int				held_dngls[2];
	pthread_cond_t	cond;
	pthread_mutex_t	lock;
}		t_cdr_info;

typedef struct s_request
{
	t_cdr_info			*cdr;
	int					deadline;
	struct s_request	*next;
	struct s_request	*prev;
}						t_request;

typedef struct s_queue
{
	t_request		*cur_req;
	t_request		*node;
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
}					t_queue;

typedef struct s_output
{
	pthread_mutex_t	lock;
}					t_output;

typedef struct s_my_threads
{
	pthread_t		monitor;
	pthread_t		*coders;
}					t_my_threads;

typedef struct s_system
{
	t_parameters	*param;
	t_cdr_info		*cdrs_info;
	t_dngl_info		*dngls_info;
	int				running;
	t_output		*output;
	t_my_threads	*my_threads;
	long long		start_time;
	pthread_mutex_t	lock;
}					t_system;

typedef struct s_cdr_wrpr
{
	t_system	*system;
	t_cdr_info	*cdr;
}				t_cdr_wrpr;

typedef struct s_dngl_wrpr
{
	t_system	*system;
	t_dngl_info	*dngl;
}				t_dngl_wrpr;

void	ft_wait_for_dongles(t_cdr_info *cdr, t_system *sys);
int	ft_got_dongle(t_dngl_info *dngl, t_cdr_info *cdr);
t_dngl_info	**ft_get_sorted_dongles(t_dngl_info *dngls[2], t_dngl_info **sort);

void	*ft_dongle(void *arg);

t_system		*ft_init_system(t_system *system, int argc, char **argv);
t_parameters	*ft_parse(int argc, char **argv);
t_cdr_info		*ft_generate_cdrs_info(t_parameters *prm, t_dngl_info *dngls);
t_dngl_info		*ft_generate_dngls_info(t_parameters *prm);
t_request		*ft_create_request(t_queue *queue, t_cdr_info *cdr, int tt_bo);
t_request		*ft_get_priority_req(char *scheduler, t_queue *queue);
int				ft_are_coders_done(t_system *sys);
int				ft_is_coder_done(t_cdr_info *cdr, t_system *sys);
void			ft_pop_node(t_queue *queue, t_request *node);
void			ft_push_node(t_queue *queue, t_request *node);
int				ft_check_numbers(char **argv);
int				ft_check_str(char **argv);
void			*ft_orchestrator(void *arg);
int				ft_satisfy_coder_request(t_cdr_info *cdr);
t_cdr_info		*ft_get_priority_coder(t_system *system);
void			*ft_monitor(void *arg);
int				ft_check_for_burnout(int i, t_system *sys);
pthread_t		*ft_init_coders(t_system *system);
t_my_threads	*ft_init_threads(t_system *system);
void			*ft_coder(void *arg);
int				ft_refactor(t_cdr_wrpr *wrpr);
int				ft_debug(t_cdr_wrpr *wrpr);
int				ft_compile(t_cdr_wrpr *wrpr);
void			ft_wait(t_system *system);
void    		*ft_single_coder_routine(t_cdr_wrpr *wrpr);
int				ft_queue_len(t_queue *queue);
void			ft_print_dngl_state(t_cdr_wrpr *wrpr, char *state);
void			ft_print_took_dngl(t_cdr_wrpr *wrpr);
void			ft_take_dngl(t_dngl_info *dngl, t_cdr_info *cdr, t_system *sys);
void			ft_give_dongle(t_dngl_info *dngl);
void			ft_give_back_dongles(t_cdr_info *cdr);
long long		ft_get_time(void);
int				ft_get_running_state(t_system *sys);
void			ft_stop_system(t_system *sys);
void			ft_free(t_system *system);
void			ft_free_queue(t_queue *queue);

#endif
