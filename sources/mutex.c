/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:02:46 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/08 17:31:23 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

int	create_forks(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb)
	{
		data->fork_array[i] = malloc(sizeof(pthread_mutex_t));
		if (!data->fork_array[i])
			return (printf(ERROR2), 1);
		if (pthread_mutex_init(data->fork_array[i], NULL))
			return (printf("Error initializing mutex\n"), exit_all(data, 1), 1);
	}
	return (0);
}

int	distribute_forks(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb)
	{
		if (i == data->nb - 1)
		{
			data->foucault_array[i]->left_fork = data->fork_array[i];
			data->foucault_array[i]->right_fork = data->fork_array[0];
		}
		else
		{
			data->foucault_array[i]->left_fork = data->fork_array[i];
			data->foucault_array[i]->right_fork = data->fork_array[i + 1];
		}
	}
	return (0);
}
