#include "header.h"

t_cdr_info	*ft_generate_coders_info(t_parameters *param)
{
	t_cdr_info	*cdrs_info;
	t_cdr_info 	*cdr;
	int			nb_cdrs;
	int			i;

	nb_cdrs = param->nb_cdrs;
	cdrs_info = (t_cdr_info *) malloc(sizeof(t_cdr_info) * nb_cdrs);

	i = 1;
	while (i <= nb_cdrs)
	{
		cdr = &cdrs_info[i];
		cdr->cdr_id = i;
		cdr->nearby_dngls[0] = (i - 1 + nb_cdrs) % nb_cdrs;
		cdr->nearby_dngls[1] = i;
		cdr->owned_dngls[0] = 0;
		cdr->owned_dngls[1] = 0;
		cdr->compiles_done = 0;
		printf("coder %i can reach dongle %i and %i\n", cdr->cdr_id, cdr->nearby_dngls[0], cdr->nearby_dngls[1]);
		i++;
	}
	return cdrs_info;
}

void	*coder(void *arg)
{
	return NULL;
}
