/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:55:31 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/14 16:55:31 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

void	print_action(t_foucault *philo, char *action)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%lld\t%d\t%s.\n", current_time(philo->data), philo->name, action);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

long long int	current_time(t_data *data)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((((current_time.tv_sec - data->start_time.tv_sec) * 1000)
			+ (current_time.tv_usec - data->start_time.tv_usec) / 1000));
}
