/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 11:48:26 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/08 17:31:08 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

t_data	*initialize_data(int argc, char **argv)
{
	t_data	*data;
	int		i;

	i = 0;
	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	while (argv[++i] && i < argc)
		if (!ft_isint(argv[i]) || !ft_atol(argv[i]))
			return (ft_putstr(ERROR1), NULL);
	data->nb = ft_atol(argv[1]);
	data->ttd = ft_atol(argv[2]);
	data->tte = ft_atol(argv[3]);
	data->tts = ft_atol(argv[4]);
	data->notepme = -1;
	if (argc == 6)
		data->notepme = ft_atol(argv[5]);
	data->foucault_array = malloc(sizeof(t_foucault) * (data->nb + 1));
	if (!data->foucault_array)
		return (printf(ERROR2), exit_all(data, 1), NULL);
	data->fork_array = malloc(sizeof(pthread_mutex_t) * (data->nb + 1));
	if (!data->fork_array)
		return (printf(ERROR2), exit_all(data, 1), NULL);
	return (data);
}
