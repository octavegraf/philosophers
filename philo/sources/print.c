/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:55:31 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/20 17:16:54 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

int	print_action(t_foucault *philo, char *action)
{
	if (pthread_mutex_lock(&philo->data->print_mutex))
		return (1);
	if (read_mutex(&philo->data->stop_mutex,
			(int *)&philo->data->simulation_stopped))
		return (pthread_mutex_unlock(&philo->data->print_mutex), 1);
	printf("%lld %d %s\n", current_time(philo->data), philo->name, action);
	if (pthread_mutex_unlock(&philo->data->print_mutex))
		return (1);
	return (0);
}

long long int	current_time(t_data *data)
{
	return (get_time_ms() - data->start_time);
}
