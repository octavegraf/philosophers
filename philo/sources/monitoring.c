/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:26:29 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/17 18:43:00 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

bool	am_i_dead(t_foucault *philo)
{
	struct timeval	current;
	long long int	last_meal_ms;
	long long int	current_ms;
	long long int	time_since_meal;

	gettimeofday(&current, NULL);
	pthread_mutex_lock(&philo->death_mutex);
	last_meal_ms = ((philo->lmt.tv_sec - philo->data->start_time.tv_sec)
			* 1000LL) + ((philo->lmt.tv_usec - philo->data->start_time.tv_usec)
			/ 1000LL);
	pthread_mutex_unlock(&philo->death_mutex);
	current_ms = ((current.tv_sec - philo->data->start_time.tv_sec) * 1000LL)
		+ ((current.tv_usec - philo->data->start_time.tv_usec) / 1000LL);
	time_since_meal = current_ms - last_meal_ms;
	if (time_since_meal >= philo->data->ttd)
	{
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%lld %d died\n", current_time(philo->data), philo->name);
		modify_mutex(&philo->data->stop_mutex,
			(int *)&philo->data->simulation_stopped, true);
		usleep(1000);
		pthread_mutex_unlock(&philo->data->print_mutex);
		return (true);
	}
	return (false);
}

bool	am_i_full(t_foucault *philo)
{
	int	meals;

	if (philo->data->notepme == -1)
		return (false);
	meals = read_mutex(&philo->death_mutex, &philo->hmta);
	if (meals >= philo->data->notepme)
		return (true);
	return (false);
}

void	*monitoring(void *arg)
{
	int		i;
	t_data	*data;
	int		full_count;
	int		notepme;

	data = (t_data *)arg;
	notepme = data->notepme;
	while (1)
	{
		i = -1;
		full_count = 0;
		while (++i < data->nb)
		{
			if (am_i_dead(data->foucault_array[i]))
				return (NULL);
			if (notepme != -1 && am_i_full(data->foucault_array[i]))
				full_count++;
		}
		if (full_count == data->nb)
			return (modify_mutex(&data->stop_mutex,
					(int *)&data->simulation_stopped, true), NULL);
		usleep(100);
	}
	return (NULL);
}
