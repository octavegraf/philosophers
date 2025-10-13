/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:55:31 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/13 16:54:33 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

void	print_phrase(t_data *data, int size, ...)
{
	va_list	args;
	char	*str;

	pthread_mutex_lock(&data->print_mutex);
	va_start(args, size);
	while (size--)
	{
		str = va_arg(args, char *);
		printf("%s", str);
		if (size > 0)
			printf(" ");
		else
			printf("\n");
	}
	va_end(args);
	pthread_mutex_unlock(&data->print_mutex);
}

void	print_action(t_foucault *philo, char *action)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%d %d %s\n", philo->data->start_time * 1000, philo->name, action);
	pthread_mutex_unlock(&philo->data->print_mutex);
}
