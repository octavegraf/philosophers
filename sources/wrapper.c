/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:26:55 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/15 17:37:19 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

void	exit_all(t_data *data, int exit_code)
{
	int	i;

	i = -1;
	if (!data)
		exit(exit_code);
	while (++i < data->nb)
	{
		if (data->fork_array && data->fork_array[i])
		{
			pthread_mutex_destroy(data->fork_array[i]);
			free(data->fork_array[i]);
		}
		if (data->foucault_array && data->foucault_array[i])
		{
			pthread_mutex_destroy(&data->foucault_array[i]->death_mutex);
			free(data->foucault_array[i]);
		}
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->start_mutex);
	pthread_mutex_destroy(&data->sd_mutex);
	if (data->foucault_array)
		free(data->foucault_array);
	if (data->fork_array)
		free(data->fork_array);
	free(data);
	exit(exit_code);
}

int	main(int argc, char **argv)
{
	t_data	*data;
	int		i;

	if (argc != 5 && argc != 6)
		return (ft_putstr(USAGE), ft_putstr(USAGE2), 1);
	data = initialize_data(argc, argv);
	if (!data)
		return (1);
	create_foucaults(data);
	create_forks(data);
	distribute_forks(data);
	start_threads(data);
	pthread_create(&data->monitor_thread, NULL, monitoring, data);
	pthread_mutex_lock(&data->start_mutex);
	data->simulation_started = true;
	pthread_mutex_unlock(&data->start_mutex);
	i = -1;
	while (++i < data->nb)
		pthread_join(data->foucault_array[i]->thread, NULL);
	pthread_join(data->monitor_thread, NULL);
	usleep(1000);
	exit_all(data, 0);
}

// Parsing
// Initialiser structures
// Creer threads
// Execution / mutex / gestion des forks
// Message
// Nettoyage / free
