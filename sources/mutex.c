/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:02:46 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/09 17:55:06 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

int	create_forks(t_data *data)
{
	int	i;

	i = -1;
	memset(data->fork_array, 0, sizeof(pthread_mutex_t *) * (data->nb + 1));
	if (pthread_mutex_init(&data->print_mutex, NULL))
		return (printf(ERROR4), exit_all(data, 1), 1);
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

int	fork_handler(t_foucault *philo)
{
	bool	has_left_fork;
	bool	has_right_fork;
	int		return_value;

	has_left_fork = false;
	has_right_fork = false;
	while (!has_left_fork || !has_right_fork)
	{
		if (!has_left_fork)
			return_value = has_fork(philo, philo->left_fork, &has_left_fork);
		if (return_value < 0)
			return (1);
		if (!has_right_fork)
			return_value = has_fork(philo, philo->right_fork, &has_right_fork);
		if (return_value < 0)
			return (1);
		return_value = 0;
	}
	return (0);
}

int	has_fork(t_foucault *philo, pthread_mutex_t *fork, bool *has_fork)
{
	int	return_value;

	while (!*has_fork)
	{
		return_value = pthread_mutex_lock(fork);
		if (!return_value)
		{
			while (1)
				if (print_mutex(philo->data) == PM_LOCKED)
					break ;
			printf("%s %d has taken a fork\n", "[TIME]", philo->name);
			if (print_mutex(philo->data) != PM_UNLOCKED)
				return (exit_all(philo->data, 1), 1);
			*has_fork = true;
			return (0);
		}
		else if (return_value != EBUSY || return_value != EDEADLK)
			return (printf(ERROR3),
				exit_all(philo->data, 1), 1);
	}
	return (1);
}
 