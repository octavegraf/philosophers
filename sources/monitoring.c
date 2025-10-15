/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:26:29 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/15 17:44:31 by ocgraf           ###   ########.fr       */
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
	last_meal_ms = (long long int)(philo->lmt.tv_sec * 1000)
		+ (philo->lmt.tv_usec / 1000);
	current_ms = (long long int)(current.tv_sec * 1000)
		+ (current.tv_usec / 1000);
	time_since_meal = current_ms - last_meal_ms;
	if (time_since_meal > philo->data->ttd)
	{
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%lld %d died\n", current_time(philo->data), philo->name);
		pthread_mutex_unlock(&philo->data->print_mutex);
		modify_mutex(&philo->data->sd_mutex, (int *)&philo->data->sd, true);
		return (true);
	}
	return (false);
}

bool	all_have_eaten(t_data *data)
{
	int	i;

	if (data->notepme == -1)
		return (false);
	i = 0;
	while (i < data->nb)
	{
		if (read_mutex(&data->foucault_array[i]->death_mutex,
				&data->foucault_array[i]->hmta) < data->notepme)
			return (false);
		i++;
	}
	return (true);
}

void	*monitoring(void *arg)
{
	int		i;
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		i = -1;
		while (++i < data->nb)
			if (am_i_dead(data->foucault_array[i]))
				return (NULL);
		if (all_have_eaten(data))
			break ;
		usleep(1000);
	}
	return (NULL);
}

