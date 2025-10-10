/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:34:58 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/10 14:40:45 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

int	start_threads(t_data *data, bool avoid_deadlock)
{
	int	i;

	i = avoid_deadlock;
	while (i < data->nb)
	{
		if (pthread_create(&data->foucault_array[i]->thread, NULL,
				discipline_punish, data->foucault_array[i]))
			return (printf("Error creating thread\n"),
				exit_all(data, 1), 1);
		i += 2;
	}
	if (!avoid_deadlock)
		start_threads(data, !avoid_deadlock);
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
				exit_all(philo->data, 1);
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
	return (NULL);
}
