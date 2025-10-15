/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:02:46 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/15 15:21:50 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

int	create_forks(t_data *data)
{
	int	i;

	i = -1;
	memset(data->fork_array, 0, sizeof(pthread_mutex_t *) * (data->nb + 1));
	while (++i < data->nb)
	{
		data->fork_array[i] = malloc(sizeof(pthread_mutex_t));
		if (!data->fork_array[i])
			return (printf(ERROR2), exit_all(data, 1), 1);
		if (pthread_mutex_init(data->fork_array[i], NULL))
			return (printf(ERROR4), exit_all(data, 1), 1);
	}
	return (0);
}

int	distribute_forks(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb)
	{
		data->foucault_array[i]->left_fork = data->fork_array[i];
		if (i < data->nb - 1)
			data->foucault_array[i]->right_fork = data->fork_array[i + 1];
		else
			data->foucault_array[i]->right_fork = data->fork_array[0];
	}
	return (0);
}

int	fork_handler(t_foucault *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;
	int				return_value;

	if (philo->name % 2 == 0)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	return_value = pthread_mutex_lock(first_fork);
	if (return_value != 0)
		return (printf(ERROR3), exit_all(philo->data, 1), 1);
	print_action(philo, "has taken a fork");
	return_value = pthread_mutex_lock(second_fork);
	if (return_value != 0)
		return (pthread_mutex_unlock(first_fork),
			printf(ERROR3), exit_all(philo->data, 1), 1);
	print_action(philo, "has taken a fork");
	return (0);
}

int	read_mutex(pthread_mutex_t *mutex, int *variable)
{
	int	return_value;
	int	value;

	return_value = pthread_mutex_lock(mutex);
	if (return_value != 0)
		return (printf(ERROR3), -1);
	value = *variable;
	return_value = pthread_mutex_unlock(mutex);
	if (return_value != 0)
		return (printf(ERROR3), -1);
	return (0);
}

int	modify_mutex(pthread_mutex_t *mutex, int *variable, int new_value)
{
	int	return_value;

	return_value = pthread_mutex_lock(mutex);
	if (return_value != 0)
		return (printf(ERROR3), -1);
	*variable = new_value;
	return_value = pthread_mutex_unlock(mutex);
	if (return_value != 0)
		return (printf(ERROR3), -1);
	return (0);
}
