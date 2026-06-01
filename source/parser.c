/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapoggi <rapoggi@student.42perpignan.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 18:23:21 by rapoggi           #+#    #+#             */
/*   Updated: 2026/06/01 18:18:35 by rapoggi          ###   ########.fr       */
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

char	*ft_get_max_int(void)
{
	char	*max_int;

	max_int = (char *) malloc(sizeof(char) * 11);
	max_int[10] = '\0';
	max_int[0] = '2';
	max_int[1] = '1';
	max_int[2] = '4';
	max_int[3] = '7';
	max_int[4] = '4';
	max_int[5] = '8';
	max_int[6] = '3';
	max_int[7] = '6';
	max_int[8] = '4';
	max_int[9] = '7';
	return (max_int);
}

int	ft_check_int_overflow(char *nb)
{
	char	*max_int;
	int		i;

	if (!nb)
		return (0);
	max_int = ft_get_max_int();
	i = 0;
	while (i < 11 && max_int[i] && nb[i])
	{
		if (nb[i] < max_int[i])
		{
			free(max_int);
			return (0);
		}
		if (nb[i] > max_int[i])
		{
			free(max_int);
			return (1);
		}
		i++;
	}
	free(max_int);
	return (0);
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
		if ((len == 1 && argv[i][y] == '0') || len > 10)
			return (0);
		if (len == 10)
			if (ft_check_int_overflow(argv[i]))
				return (0);
		while (y < len)
		{
			if (!argv[i][y] || (argv[i][y] < '0' || argv[i][y] > '9'))
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
