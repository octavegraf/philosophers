/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:26:55 by ocgraf            #+#    #+#             */
/*   Updated: 2025/07/31 15:16:04 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	int	i;

	if (argc != 5 && argc != 6)
		return (ft_putstr(USAGE), ft_putstr(USAGE2), 1);
	i = 0;
	while (argv[++i] && i < argc)
		if (!ft_isint(argv[i]) || !ft_atol(argv[i]))
			return (ft_putstr(ERROR1), 1);
	create_threads(ft_atol(argv[1]));
}
