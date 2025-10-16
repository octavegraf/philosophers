/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:34:58 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/16 16:55:28 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

int	start_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb)
	{
		if (pthread_create(&data->foucault_array[i]->thread, NULL,
				discipline_punish, data->foucault_array[i]))
			return (printf("Error creating thread\n"),
				exit_all(data, 1), 1);
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
	if (philo->name % 2 != 0)
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
	if (philo->data->notepme != -1 && current_meals + 1 >= philo->data->notepme)
		return (1);
	if (read_mutex(&philo->data->stop_mutex,
			(int *)&philo->data->simulation_stopped))
		return (1);
	if (print_action(philo, "is sleeping") || (better_usleep(philo->data->tts,
				philo->data) || (print_action(philo, "is thinking"))))
		return (1);
	return (0);
}

int	better_usleep(int time_in_ms, t_data *data)
{
	struct timeval	start;
	struct timeval	current;
	long long int	start_ms;
	long long int	current_ms;

	gettimeofday(&start, NULL);
	start_ms = (long long int)(start.tv_sec * 1000) + (start.tv_usec / 1000);
	while (1)
	{
		if (read_mutex(&data->stop_mutex, (int *)&data->simulation_stopped))
			return (1);
		gettimeofday(&current, NULL);
		current_ms = (long long int)(current.tv_sec * 1000)
			+ (current.tv_usec / 1000);
		if (current_ms - start_ms >= time_in_ms)
			return (0);
		usleep(1000);
	}
	return (0);
}
