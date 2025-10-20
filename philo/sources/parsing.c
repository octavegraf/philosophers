/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 11:48:26 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/20 18:11:59 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

static t_data	*initialize_data2(t_data *data);

t_data	*initialize_data(int argc, char **argv)
{
	t_data	*data;
	int		i;

	i = 0;
	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	memset(data, 0, sizeof(t_data));
	while (argv[++i] && i < argc)
		if (!ft_isint(argv[i]) || !ft_atol(argv[i]))
			return (printf(ERROR1), exit_all(data, 1), NULL);
	data->nb = ft_atol(argv[1]);
	data->ttd = ft_atol(argv[2]);
	data->tte = ft_atol(argv[3]);
	data->tts = ft_atol(argv[4]);
	data->notepme = -1;
	if (argc == 6)
		data->notepme = ft_atol(argv[5]);
	return (initialize_data2(data));
}

static t_data	*initialize_data2(t_data *data)
{
	if (!data)
		return (NULL);
	if (pthread_mutex_init(&data->print_mutex, NULL))
		return (printf(ERROR4), free(data), NULL);
	if (pthread_mutex_init(&data->start_mutex, NULL))
		return (printf(ERROR4), pthread_mutex_destroy(&data->print_mutex),
			free(data), NULL);
	if (pthread_mutex_init(&data->stop_mutex, NULL))
		return (printf(ERROR4), pthread_mutex_destroy(&data->print_mutex),
			pthread_mutex_destroy(&data->start_mutex), free(data), NULL);
	data->foucault_array = malloc(sizeof(t_foucault *) * (data->nb + 1));
	if (!data->foucault_array)
		return (printf(ERROR2), exit_all(data, 1), NULL);
	memset(data->foucault_array, 0, sizeof(t_foucault *) * (data->nb + 1));
	data->fork_array = malloc(sizeof(pthread_mutex_t *) * (data->nb + 1));
	if (!data->fork_array)
		return (printf(ERROR2), exit_all(data, 1), NULL);
	return (data);
}

int	create_foucaults(t_data *data)
{
	int		i;

	i = -1;
	while (++i < data->nb)
	{
		data->foucault_array[i] = malloc(sizeof(t_foucault));
		if (!data->foucault_array[i])
			return (printf(ERROR2), exit_all(data, 1), 1);
		memset(data->foucault_array[i], 0, sizeof(t_foucault));
		data->foucault_array[i]->name = i + 1;
		data->foucault_array[i]->data = data;
		data->foucault_array[i]->hmta = 0;
		if (pthread_mutex_init(&data->foucault_array[i]->death_mutex, NULL))
			return (printf(ERROR4), exit_all(data, 1), 1);
		data->foucault_array[i]->lmt = 0;
	}
	return (0);
}
