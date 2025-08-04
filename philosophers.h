/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:35:10 by ocgraf            #+#    #+#             */
/*   Updated: 2025/08/01 10:13:19 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <limits.h>
# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_foucault
{
	int			i;
	pthread_t	*th;
	t_foucault	*previous;
	t_foucault	*next;
}	t_foucault;

typedef struct s_data
{
	int	i;
	int	ttd;
	int	tte;
	int	tts;
	int	number_of_times_each_philosopher_must_eat;
	int	fork;
}	t_data;


//threads
void			free_it(void **array);
void			*ft_print(void *args);
pthread_t		**create_threads(int i);

//libft
int				ft_strlen(char *str);
void			ft_putstr(char *str);
int				ft_isint(char *str);
long long int	ft_atol(const char *str);

# define USAGE "./philosophers number_of_philosophers time_to_die time_to_eat "
# define USAGE2 "time_to_sleep [number_of_times_each_philosopher_must_eat]\n"
# define ERROR1 "Not a number or int or above 0.\n"

#endif