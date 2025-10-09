/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:55:31 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/09 15:11:43 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

int	print_mutex(t_data *data)
{
	int	return_value;

	return_value = pthread_mutex_lock(&data->print_mutex);
	if (!return_value)
		return (PM_LOCKED);
	if (return_value == EBUSY)
		return (PM_ALREADY_LOCKED);
	else if (return_value == EDEADLK)
		return_value = pthread_mutex_unlock(&data->print_mutex);
	if (!return_value)
		return (PM_UNLOCKED);
	if (return_value)
		return (printf(ERROR3), exit_all(data, 1), PM_ERROR);
	return (0);
}
