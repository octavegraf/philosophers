/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:34:58 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/17 18:41:48 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

int	start_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb)
	{
		if (data->foucault_array[i]->name % 2 == 0)
		{
			if (pthread_create(&data->foucault_array[i]->thread, NULL,
					discipline_punish, data->foucault_array[i]))
				return (printf(ERROR5), exit_all(data, 1), 1);
		}
		i++;
	}
	i = 0;
	while (i < data->nb)
	{
		if (data->foucault_array[i]->name % 2 != 0)
		{
			if (pthread_create(&data->foucault_array[i]->thread, NULL,
					discipline_punish, data->foucault_array[i]))
				return (printf(ERROR5), exit_all(data, 1), 1);
		}
		i++;
	}
	return (0);
}
static int	discipline_punish2(t_foucault *philo);

void	*discipline_punish(void *arg)
{
	t_foucault	*philo;
	int			return_value;

	philo = (t_foucault *)arg;
	while (!read_mutex(&philo->data->start_mutex,
			(int *)&philo->data->simulation_started))
		usleep(100);
	if (philo->name % 2 == 0)
		usleep(1000);
	while (philo->hmta < philo->data->notepme
		|| philo->data->notepme == -1)
	{
		if (read_mutex(&philo->data->stop_mutex,
				(int *)&philo->data->simulation_stopped))
			return (NULL);
		return_value = fork_handler(philo);
		if (return_value != 0)
			return (NULL);
		if (print_action(philo, "is eating"))
			return (NULL);
		if (discipline_punish2(philo))
			return (NULL);
	}
	return (NULL);
}

static int	discipline_punish2(t_foucault *philo)
{
	int	current_meals;

	if (better_usleep(philo->data->tte, philo->data))
	{
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
		return (1);
	}
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_lock(&philo->death_mutex);
	gettimeofday(&philo->lmt, NULL);
	pthread_mutex_unlock(&philo->death_mutex);
	pthread_mutex_lock(&philo->death_mutex);
	current_meals = ++philo->hmta;
	pthread_mutex_unlock(&philo->death_mutex);
	if (philo->data->notepme != -1 && current_meals >= philo->data->notepme)
		return (1);
	if (read_mutex(&philo->data->stop_mutex,
			(int *)&philo->data->simulation_stopped))
		return (1);
	if (print_action(philo, "is sleeping") || (better_usleep(philo->data->tts,
				philo->data) || (print_action(philo, "is thinking"))))
		return (1);
	return (usleep(200), 0);
}

int	better_usleep(int time_in_ms, t_data *data)
{
	struct timeval	start;
	struct timeval	current;
	long long int	elapsed;
	long long int	target_time;
	long long int	remaining;

	gettimeofday(&start, NULL);
	target_time = (start.tv_sec * 1000LL) + (start.tv_usec / 1000LL)
		+ time_in_ms;
	while (1)
	{
		if (read_mutex(&data->stop_mutex, (int *)&data->simulation_stopped))
			return (1);
		gettimeofday(&current, NULL);
		elapsed = (current.tv_sec * 1000LL) + (current.tv_usec / 1000LL);
		if (elapsed >= target_time)
			return (0);
		remaining = target_time - elapsed;
		if (remaining > 1)
			usleep(500);
		else
			usleep(10);
	}
	return (0);
}
