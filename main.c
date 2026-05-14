#include "header.h"

int main(int argc, char **argv)
{
	t_parameters *parameters = ft_parse(argc, argv);
	if (!parameters)
		printf("cmon man");
	return 0;
}
