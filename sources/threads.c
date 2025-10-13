/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:34:58 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/13 15:06:13 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

int	start_threads(t_data *data, bool avoid_deadlock)
{
	int	i;

	(void)avoid_deadlock;
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

void	*discipline_punish(void *arg)
{
	t_foucault	*philo;
	int			return_value;

	philo = (t_foucault *)arg;
	pthread_mutex_lock(&philo->data->start_mutex);
	while (!philo->data->simulation_started)
	{
		pthread_mutex_unlock(&philo->data->start_mutex);
		usleep(100);
		pthread_mutex_lock(&philo->data->start_mutex);
	}
	pthread_mutex_unlock(&philo->data->start_mutex);
	if (philo->name % 2 != 0)
		usleep(100);
	while (philo->how_many_times_ate < philo->data->notepme
		|| philo->data->notepme == -1)
	{
		return_value = fork_handler(philo);
		if (return_value != 0)
			exit_all(philo->data, 1);
		print_action(philo, "is eating");
		usleep(philo->data->tte * 1000);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		philo->how_many_times_ate++;
		print_action(philo, "is sleeping");
		usleep(philo->data->tts * 1000);
		print_action(philo, "is thinking");
	}
	return (NULL);
}
