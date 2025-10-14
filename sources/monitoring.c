/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:26:29 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/14 17:05:47 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

int	am_i_dead(t_foucault *philo)
{
	struct timeval	current;
	long long int	last_meal_ms;
	long long int	current_ms;
	long long int	time_since_meal;

	gettimeofday(&current, NULL);
	last_meal_ms = (long long int)(philo->last_meal_time.tv_sec * 1000)
		+ (philo->last_meal_time.tv_usec / 1000);
	current_ms = (long long int)(current.tv_sec * 1000)
		+ (current.tv_usec / 1000);
	time_since_meal = current_ms - last_meal_ms;
	if (time_since_meal > philo->data->ttd)
	{
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%lld %d died\n", current_time(philo->data), philo->name);
		pthread_mutex_unlock(&philo->data->print_mutex);
		return (1);
	}
	return (0);
}

void	*monitoring(void *arg)
{
	int		i;
	t_data	*data;

	i = -1;
	data = (t_data *)arg;
	while (++i < data->nb)
	{
		if (am_i_dead(data->foucault_array[i]))
			exit_all(data, 0);
		if (i >= data->nb -1)
			i = -1;
	}
	return (NULL);
}
