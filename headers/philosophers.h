/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:35:10 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/13 13:55:09 by ocgraf           ###   ########.fr       */
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
# include <stdarg.h>

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
 * @param foucault Pointer to philosophers structures.
 * @param fork_array Pointer to array of mutex for forks.
 * @param print_mutex Mutex to avoid data races when printing.
 * @param start_mutex Mutex to control the start of the simulation.
 * @param simulation_started Boolean indicating if the simulation has started.
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
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	start_mutex;
	bool			simulation_started;
}	t_data;

/**
 * @brief Structure for each philosopher.
 * 
 * @note foucault name is a reference to Michel Foucault, a french philosopher
 * who wrote about power structures and social dynamics, which can be linked to
 * the dining philosophers problem.
 * @param name Philosopher's number (starts at 1).
 * @param thread The philosopher's thread.
 * @param left_fork Pointer to the left fork mutex.
 * @param right_fork Pointer to the right fork mutex.
 * @param how_many_times_ate How many times the philosopher has eaten.
 * @param last_meal_time Timestamp of the last meal time in milliseconds.
 * @param data Pointer to the shared simulation data.
 */
typedef struct s_foucault
{
	int				name;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				how_many_times_ate;
	long long int	last_meal_time;
	t_data			*data;
}	t_foucault;

//	mutex.c
int				create_forks(t_data *data);
int				distribute_forks(t_data *data);

// parsing.c
t_data			*initialize_data(int argc, char **argv);

// print.c
/**
 * @brief Print a formatted message respecting mutex locking.
 * 
 * @param size[in] Number of arguments to print.
 * @param data [in, out] Structure for simulation data.
 * @param ... Arguments to print.
 */
void			print_phrase(t_data *data, int size, ...);
void			print_action(t_foucault *philo, char *action);
//	threads.c

int				create_michels(t_data *data);
int				start_threads(t_data *data, bool avoid_deadlock);
void			*discipline_punish(void *arg);
int				fork_handler(t_foucault *philo);

//	utils.c
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

//	utils2.c
/**
 * @brief Convert an integer to a string.
 *
 * @param n Integer to convert.
 * @return char* Pointer to the resulting string.
 */
char			*ft_itoa(int n);

//	wrapper.c
void			exit_all(t_data *data, int exit_code);

# define USAGE "./philosophers number_of_philosophers time_to_die time_to_eat "
# define USAGE2 "time_to_sleep [number_of_times_each_philosopher_must_eat]\n"
# define ERROR1 "Not a number or int or above 0.\n"
# define ERROR2 "Can't allocate memory.\n"
# define ERROR3 "Error locking/unlocking mutex\n"
# define ERROR4 "Error creating/destroying mutex\n"

/**
 * @brief Status codes for print_mutex function.
 * @param KED Mutex successfully locked.
 * @param OCKED Mutex successfully unlocked.
 * @param EADY_LOCKED Mutex was already locked somewhere else.
 * @param OR An error occurred while locking or unlocking the mutex.
 */

enum	e_mutex_status
{
	LOCKED,
	UNLOCKED,
	ALREADY_LOCKED,
	ERROR
};

#endif