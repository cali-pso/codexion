#ifndef HEADER_H
#define HEADER_H

# include <pthread.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_parameters
{	
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	char	*scheduler;
}			t_parameters;

t_parameters	*ft_parse(int argc, char **argv);
bool			ft_check_numbers(char **argv);
bool			ft_check_str(char **argv);

#endif
