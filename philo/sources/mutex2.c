/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 13:56:52 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/16 13:57:08 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

int	read_mutex(pthread_mutex_t *mutex, int *variable)
{
	int	return_value;
	int	value;

	return_value = pthread_mutex_lock(mutex);
	if (return_value != 0)
		return (printf(ERROR3), -1);
	value = *variable;
	return_value = pthread_mutex_unlock(mutex);
	if (return_value != 0)
		return (printf(ERROR3), -1);
	return (value);
}

int	modify_mutex(pthread_mutex_t *mutex, int *variable, int new_value)
{
	int	return_value;

	return_value = pthread_mutex_lock(mutex);
	if (return_value != 0)
		return (printf(ERROR3), -1);
	*variable = new_value;
	return_value = pthread_mutex_unlock(mutex);
	if (return_value != 0)
		return (printf(ERROR3), -1);
	return (0);
}
