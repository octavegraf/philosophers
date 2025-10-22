/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:02:46 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/17 18:24:47 by ocgraf           ###   ########.fr       */
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
		data->foucault_array[i]->l_fork = data->fork_array[i];
		if (i < data->nb - 1)
			data->foucault_array[i]->r_fork = data->fork_array[i + 1];
		else
			data->foucault_array[i]->r_fork = data->fork_array[0];
	}
	return (0);
}
static int	fork_handler2(t_foucault *philo,
		pthread_mutex_t *first_fork, pthread_mutex_t *second_fork);

int	fork_handler(t_foucault *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->data->nb == 1)
	{
		pthread_mutex_lock(philo->l_fork);
		print_action(philo, "has taken a fork");
		better_usleep(philo->data->ttd, philo->data);
		pthread_mutex_unlock(philo->l_fork);
		return (1);
	}
	if (philo->l_fork < philo->r_fork)
	{
		first_fork = philo->l_fork;
		second_fork = philo->r_fork;
	}
	else
	{
		first_fork = philo->r_fork;
		second_fork = philo->l_fork;
	}
	if (fork_handler2(philo, first_fork, second_fork))
		return (1);
	return (0);
}

static int	fork_handler2(t_foucault *philo,
	pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	int	return_value;

	return_value = pthread_mutex_lock(first_fork);
	if (return_value != 0)
		return (printf(ERROR3), exit_all(philo->data, 1), 1);
	if (read_mutex(&philo->data->stop_mutex,
			(int *)&philo->data->simulation_stopped)
		|| print_action(philo, "has taken a fork"))
		return (pthread_mutex_unlock(first_fork), 1);
	if (read_mutex(&philo->data->stop_mutex,
			(int *)&philo->data->simulation_stopped))
		return (pthread_mutex_unlock(first_fork), 1);
	return_value = pthread_mutex_lock(second_fork);
	if (return_value != 0)
		return (pthread_mutex_unlock(first_fork),
			printf(ERROR3), exit_all(philo->data, 1), 1);
	if (read_mutex(&philo->data->stop_mutex,
			(int *)&philo->data->simulation_stopped)
		|| print_action(philo, "has taken a fork"))
		return (pthread_mutex_unlock(first_fork),
			pthread_mutex_unlock(second_fork), 1);
	return (0);
}
