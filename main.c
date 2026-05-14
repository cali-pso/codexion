#include "header.h"

int main(int argc, char **argv)
{
	t_parameters *parameters = ft_parse(argc, argv);
	t_cdr_info *coders_info = ft_generate_coders_info(parameters);
	pthread_mutex_destroy(&parameters->output_lock);
	return 0;
}

void	*orchestrator(void	*arg)
{
	pthread_t		*coders;
	t_parameters	*param;
	int				i;

	param = arg;
	coders = (pthread_t *) malloc(sizeof(pthread_t) * param->nb_cdrs); 
	i = 0;
	while (i < param->nb_cdrs)
		pthread_create(&coders[i], NULL, NULL, NULL);
	return NULL;
}
