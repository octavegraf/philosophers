/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper.c.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:26:55 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/07 12:01:34 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

int	main(int argc, char **argv)
{
	t_data	*data;

	if (argc != 5 && argc != 6)
		return (ft_putstr(USAGE), ft_putstr(USAGE2), 1);
	data = initialize_data(argc, argv);
	if (!data)
		return (1);
	create_threads(ft_atol(argv[1]));
}

// Parsing
// Initialiser structures
// Creer threads
// Execution / mutex / gestion des forks
// Message
// Nettoyage / free
