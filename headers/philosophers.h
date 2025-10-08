/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:35:10 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/08 14:57:22 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <limits.h>
# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <stdbool.h>
# include <errno.h>

typedef struct s_foucault	t_foucault;
/**
 * @brief Structure for simulation data.
 * 
 * @param nb Number of philosophers.
 * @param ttd Time to die.
 * @param tte Time to eat.
 * @param tts Time to sleep.
 * @param notepme Number of times each philosopher must eat. Optional,
 * simulation stops when all have eaten at least this many times.
 * @param foucault Double pointer to philosopher threads. 
 * @param mutex Pointer to mutex for forks.
 */
typedef struct s_data
{
	int				nb;
	int				ttd;
	int				tte;
	int				tts;
	int				notepme;
	t_foucault		**foucault_array;
	pthread_mutex_t	**fork_array;
}	t_data;

/**
 * @brief Structure for each philosopher.
 * 
 * @note foucault name is a reference to Michel Foucault, a french philosopher
 * who wrote about power structures and social dynamics, which can be linked to
 * the dining philosophers problem.
 * @param name Philosopher's number (starts at 1).
 * @param thread Pointer to the philosopher's thread.
 * @param left_fork Pointer to the left fork mutex.
 * @param right_fork Pointer to the right fork mutex.
 * @param how_many_times_ate How many times the philosopher has eaten.
 * @param last_meal_time Timestamp of the last meal time in milliseconds.
 * @param data Pointer to the shared simulation data.
 */
typedef struct s_foucault
{
	int				name;
	pthread_t		*thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				how_many_times_ate;
	long long int	last_meal_time;
	t_data			*data;
}	t_foucault;

// threads.c


// utils.c
/**
 * @brief Reproduce strlen function.
 * 
 * @param[in] str String to measure.
 * @return int Length of the string.
 */
int				ft_strlen(char *str);
/**
 * @brief Reproduce putstr function.
 * 
 * @param[in] str String to print.
 */
void			ft_putstr(char *str);
/**
 * @brief Reproduce isdigit function but for integers.
 * 
 * @param[in] str String to check.
 * @return int 1 if the string is a valid integer, 0 otherwise.
 */
int				ft_isint(char *str);
/**
 * @brief Reproduce atol function.
 * 
 * @param[in] str String to convert.
 * @return long long int Converted value.
 */
long long int	ft_atol(const char *str);
/**
 * @brief Free a double pointer array.
 * 
 * @param[in, out] array Double pointer array to free.
 * @note Does not verify if the pointers exits.
 */
void			free_it(void **array);

# define USAGE "./philosophers number_of_philosophers time_to_die time_to_eat "
# define USAGE2 "time_to_sleep [number_of_times_each_philosopher_must_eat]\n"
# define ERROR1 "Not a number or int or above 0.\n"
# define ERROR2 "Can't allocate memory.\n"

#endif