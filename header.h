#ifndef HEADER_H
#define HEADER_H

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
	pthread_mutex_t output_lock;
}			t_parameters;

typedef struct s_cdr_info
{
	int	cdr_id;
	int	nearby_dngls[2];
	int	owned_dngls[2];
	int	compiles_done;
}		t_cdr_info;

typedef struct s_dngl_info
{
	int				dngl_id;
	pthread_mutex_t	owned;
}					t_dngl_info;

t_parameters	*ft_parse(int argc, char **argv);
t_cdr_info		*ft_generate_coders_info(t_parameters *parameters);
int				ft_check_numbers(char **argv);
int				ft_check_str(char **argv);

#endif
