/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:34:58 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/07 17:28:03 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

int	create_michels(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb)
	{
		data->foucault_array[i] = malloc(sizeof(t_foucault));
		if (!data->foucault_array[i])
			return (printf(ERROR2), 1);
		memset(data->foucault_array[i], 0, sizeof(t_foucault));
		data->foucault_array[i]->name = i + 1;
		data->foucault_array[i]->data = data;
		if (pthread_create(&data->foucault_array[i]->thread, NULL,
				discipline_punish, data->foucault_array[i]))
			return (printf("Error creating thread\n"), 1);

	}
	return (0);
}

void	*discipline_punish(void *arg)
{
	t_foucault	*philo;

	philo = (t_foucault *)arg;
	while (philo->how_many_times_ate < philo->data->notepme
		|| philo->data->notepme == -1)
	{
		pthread_mutex_lock(philo->left_fork);
		printf("%lld %d has taken a fork\n", get_time_ms(), philo->name);
		pthread_mutex_lock(philo->right_fork);
		printf("%lld %d has taken a fork\n", get_time_ms(), philo->name);
		printf("%lld %d is eating\n", get_time_ms(), philo->name);
	}
}

// Donner les fouchettes (2 au premier, 1 aux autres)
// Prendre les fourchettes 
// Manger
// Poser les fourchettes
// Dormir
// Penser
// Recommencer
