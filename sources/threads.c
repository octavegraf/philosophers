/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:34:58 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/15 17:44:46 by ocgraf           ###   ########.fr       */
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

void	*discipline_punish(void *arg)
{
	t_foucault	*philo;
	int			return_value;

	philo = (t_foucault *)arg;
	while (!read_mutex(&philo->data->start_mutex,
			(int *)&philo->data->simulation_started))
		usleep(100);
	if (philo->name % 2 != 0)
		usleep(100);
	while (philo->hmta < philo->data->notepme
		|| philo->data->notepme == -1)
	{
		if (read_mutex(&philo->data->sd_mutex, (int *)&philo->data->sd))
			return (NULL);
		return_value = fork_handler(philo);
		if (return_value != 0)
			return (NULL);
		if (read_mutex(&philo->data->sd_mutex, (int *)&philo->data->sd))
			return (NULL);
		print_action(philo, "is eating");
		gettimeofday(&philo->lmt, NULL);
		if (read_mutex(&philo->data->sd_mutex, (int *)&philo->data->sd))
			return (NULL);
		usleep(philo->data->tte * 1000);
		if (read_mutex(&philo->data->sd_mutex, (int *)&philo->data->sd))
			return (NULL);
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
		modify_mutex(&philo->death_mutex, &philo->hmta,
			read_mutex(&philo->death_mutex, &philo->hmta) + 1);
		if (read_mutex(&philo->data->sd_mutex, (int *)&philo->data->sd))
			return (NULL);
		print_action(philo, "is sleeping");
		usleep(philo->data->tts * 1000);
		if (read_mutex(&philo->data->sd_mutex, (int *)&philo->data->sd))
			return (NULL);
		print_action(philo, "is thinking");
	}
	return (NULL);
}

int better_usleep()