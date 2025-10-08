/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:34:58 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/08 16:04:43 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

int	create_michels(t_data *data, bool avoid_even_numbers)
{
	int		i;
	int		return_value;

	i = avoid_even_numbers - 1;
	while (++i < data->nb)
	{
		data->foucault_array[i] = malloc(sizeof(t_foucault));
		if (!data->foucault_array[i])
			return (printf(ERROR2), 1);
		memset(data->foucault_array[i], 0, sizeof(t_foucault));
		data->foucault_array[i]->name = i + 1;
		data->foucault_array[i]->data = data;
		if (i % 2 == 0)
			if (pthread_create(&data->foucault_array[i]->thread, NULL,
					discipline_punish, data->foucault_array[i]))
				return (printf("Error creating thread\n"), 1);
		++i;
	}
	if (!avoid_even_numbers)
		create_michels(data, true);
	return (0);
}

void	*discipline_punish(void *arg)
{
	t_foucault	*philo;
	int			return_value;

	return_value = 1;
	philo = (t_foucault *)arg;
	while (philo->how_many_times_ate < philo->data->notepme
		|| philo->data->notepme == -1)
	{
		while (return_value != 0)
		{
			return_value = fork_handler(philo);
			if (return_value < 0)
				return (NULL);
			break ;
		}
		printf("%s %d is eating\n", "[TIME]", philo->name);
		// usleep(philo->data->tte * 1000);
		philo->how_many_times_ate++;
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		//philo->last_meal_time = get_time_in_ms();
		printf("%s %d is sleeping\n", "[TIME]", philo->name);
		// usleep(philo->data->tts * 1000);
		printf("%s %d is thinking\n", "[TIME]", philo->name);
	}
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
			return_value = has_fork(philo, &has_left_fork);
		if (return_value)
			return (-1);
		if (!has_right_fork)
			return_value = has_fork(philo, &has_right_fork);
		if (return_value)
			return (-1);
	}
	return (0);
}

int	has_fork(t_foucault *philo, bool *has_fork)
{
	int	return_value;

	while (!*has_fork)
	{
		return_value = pthread_mutex_lock(philo->left_fork);
		if (!return_value)
		{
			printf("%s %d has taken a fork\n", "[TIME]", philo->name);
			*has_fork = true;
			return (0);
		}
		else if (return_value != EBUSY || return_value != EDEADLK)
			return (printf("Error locking mutex\n. Quitting"), -1);
	}
	return (1);
}
