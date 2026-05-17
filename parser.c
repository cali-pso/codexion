/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapoggi <rapoggi@student.42perpignan.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 18:23:21 by rapoggi           #+#    #+#             */
/*   Updated: 2026/05/17 18:25:07 by rapoggi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

t_parameters	*ft_parse(int argc, char **argv)
{
	t_parameters	*parameters;

	parameters = NULL;
	if (argc != 9 || !ft_check_numbers(argv) || !ft_check_str(argv))
		return (parameters);
	parameters = (t_parameters *) malloc(sizeof(t_parameters));
	parameters->nb_cdrs = atoi(argv[1]);
	parameters->tt_burnout = atoi(argv[2]);
	parameters->tt_compile = atoi(argv[3]);
	parameters->tt_debug = atoi(argv[4]);
	parameters->tt_refactor = atoi(argv[5]);
	parameters->nb_compiles_required = atoi(argv[6]);
	parameters->dngl_cooldown = atoi(argv[7]);
	parameters->scheduler = argv[8];
	return (parameters);
}

int	ft_check_numbers(char **argv)
{
	int	i;
	int	y;
	int	len;

	i = 1;
	while (i < 8)
	{
		y = 0;
		len = strlen(argv[i]);
		while (y < len)
		{
			if (argv[i][y] < '0' || argv[i][y] > '9')
				return (0);
			y++;
		}
		i++;
	}
	return (1);
}

int	ft_check_str(char **argv)
{
	if (strcmp("fifo", argv[8]) != 0 && strcmp("edf", argv[8]) != 0)
		return (0);
	return (1);
}
