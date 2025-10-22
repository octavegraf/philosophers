/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:34:58 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/21 14:26:35 by ocgraf           ###   ########.fr       */
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
			return (printf(ERROR5), exit_all(data, 1), 1);
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
	modify_mutex(&philo->death_mutex, (int *)&philo->lmt,
		get_time_ms() - philo->data->start_time);
	if (philo->name % 2 != 0)
		usleep((philo->data->tte * 2 - philo->data->tts) * 1000);
	while (philo->data->notepme == -1 || philo->hmta < philo->data->notepme)
	{
		if (read_mutex(&philo->data->stop_mutex,
				(int *)&philo->data->simulation_stopped))
			return (NULL);
		return_value = fork_handler(philo);
		if (return_value != 0)
			return (NULL);
		pthread_mutex_lock(&philo->death_mutex);
		philo->lmt = get_time_ms() - philo->data->start_time;
		pthread_mutex_unlock(&philo->death_mutex);
		if (discipline_punish2(philo))
			return (NULL);
	}
	return (NULL);
}

static int	discipline_punish2(t_foucault *philo)
{
	int	current_meals;

	if (print_action(philo, "is eating"))
		return (1);
	if (better_usleep(philo->data->tte, philo->data))
	{
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
		return (1);
	}
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
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
	long long int	start_ms;
	long long int	current_ms;
	long long int	target_ms;
	long long int	remaining;

	start_ms = get_time_ms();
	target_ms = start_ms + time_in_ms;
	while (1)
	{
		if (read_mutex(&data->stop_mutex, (int *)&data->simulation_stopped))
			return (1);
		current_ms = get_time_ms();
		if (current_ms >= target_ms)
			return (0);
		remaining = target_ms - current_ms;
		if (remaining > 1)
			usleep(500);
		else
			usleep(10);
	}
	return (0);
}
