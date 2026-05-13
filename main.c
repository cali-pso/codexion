#include "header.h"

int main(int argc, char **argv)
{
	t_parameters *parameters = ft_parse(argc, argv);
	for (int i = 0; i < argc; i++)
	{
		printf("%i\n\n", atoi(argv[i]));
	}
	return 0;
}

t_parameters *ft_parse(int argc, char **argv)
{
	t_parameters *parameters;

	parameters = (t_parameters *) malloc(sizeof(t_parameters));
	if (argc != 9)
		return parameters;
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
