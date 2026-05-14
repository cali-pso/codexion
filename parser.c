#include "header.h"

t_parameters *ft_parse(int argc, char **argv)
{
	t_parameters *parameters;

	parameters = NULL;
	if (argc != 9 || !ft_check_numbers(argv) || !ft_check_str(argv))
		return parameters;
	parameters = (t_parameters *) malloc(sizeof(t_parameters));
	parameters->number_of_coders = atoi(argv[1]);
	parameters->time_to_burnout = atoi(argv[2]);
	parameters->time_to_compile = atoi(argv[3]);
	parameters->time_to_debug = atoi(argv[4]);
	parameters->time_to_refactor = atoi(argv[5]);
	parameters->number_of_compiles_required = atoi(argv[6]);
	parameters->dongle_cooldown = atoi(argv[7]);
	parameters->scheduler = argv[8];

	return parameters;
}

bool	ft_check_numbers(char **argv)
{
	int i;
	int	y;
	int len;

	i = 1;
	while (i < 8)
	{
		y = 0;
		len = strlen(argv[i]);
		while (y < len)
		{
			if (argv[i][y] < '0' || argv[i][y] > '9')
				return false;
			y++;
		}
		i++;
	}
	return true;
}

bool	ft_check_str(char **argv)
{
	if (strcmp("fifo", argv[8]) != 0 && strcmp("edf", argv[8]) != 0)
		return false;
	return true;
}
