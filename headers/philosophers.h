/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:35:10 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/16 16:23:11 by ocgraf           ###   ########.fr       */
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

/**
 * @brief Structure for each philosopher.
 * 
 * @note foucault name is a reference to Michel Foucault, a french philosopher
 * who wrote about power structures and social dynamics, which can be linked to
 * the dining philosophers problem.
 * @param name Philosopher's number (starts at 1).
 * @param thread The philosopher's thread.
 * @param l_fork Pointer to the left fork mutex.
 * @param r_fork Pointer to the right fork mutex.
 * @param hmta How many timesHow many times the philosopher has eaten.
 * @param lmt Timestamp of the "last meal time" in milliseconds.
 * @param data Pointer to the shared simulation data.
 */
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
 * @param print_mutex Mutex to protect the printing.
 * @param simulation_started Boolean indicating if the simulation has started.
 * @param start_mutex Mutex to protect the start of the simulation.
 * @param start_time Timestamp of the start of the simulation.
 * @param monitor_thread Thread for monitoring philosophers.
 * @param simulation_stopped Boolean indicating if simulation stopped.
 * @param stop_mutex Mutex to protect the stop variable.
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
	bool			simulation_started;
	pthread_mutex_t	start_mutex;
	struct timeval	start_time;
	pthread_t		monitor_thread;
	bool			simulation_stopped;
	pthread_mutex_t	stop_mutex;
}	t_data;

typedef struct s_foucault
{
	int				name;
	pthread_t		thread;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	int				hmta;
	pthread_mutex_t	death_mutex;
	struct timeval	lmt;
	t_data			*data;
}	t_foucault;

//	monitoring.c
/**
 * @brief Check if a philosopher is dead.
 * 
 * @param philo Philosopher structure.
 * @return True if the philosopher is dead, false otherwise.
 */
bool			am_i_dead(t_foucault *philo);
/**
 * @brief Check if a philosopher has eaten enough times.
 * 
 * @param philo Philosopher structure.
 * @return true if the philosopher has eaten enough times, false otherwise.
 */
bool			am_i_full(t_foucault *philo);
/**
 * @brief Monitoring function for philosopher threads. Will check if any
 * philosopher is dead.
 * 
 * @param arg Pointer to the simulation data structure. Threads needs a void*
 * argument.
 * @return void* Return NULL when finished.
 */
void			*monitoring(void *arg);

//	mutex.c
/**
 * @brief Create a forks array and initialize mutexes.
 * 
 * @param[in, out] data Structure for simulation data.
 * @return int 0 on success, 1 on failure.
 */
int				create_forks(t_data *data);
/**
 * @brief Distribute forks to t_foucault structures.
 * 
 * @param[in, out] data Structure for simulation data.
 * @return int 0 on success, 1 on failure.
 */
int				distribute_forks(t_data *data);
/**
 * @brief Handle fork acquisition for a philosopher.
 * 
 * @param[in, out] philo Philosopher structure.
 * @return int 0 on success, 1 on failure.
 */
int				fork_handler(t_foucault *philo);
/**
 * @brief Read a variable protected by a mutex.
 * 
 * @param mutex Pointer to the mutex protecting the variable.
 * @param variable Pointer to the variable to read.
 * @return int Value of the variable, -1 on failure.
 */
int				read_mutex(pthread_mutex_t *mutex, int *variable);
/**
 * @brief Modify a variable protected by a mutex.
 * 
 * @param mutex Pointer to the mutex protecting the variable.
 * @param variable Pointer to the variable to modify.
 * @param new_value New value to set.
 * @return int 0 on success, 1 on failure.
 */
int				modify_mutex(pthread_mutex_t *mutex, int *variable,
					int new_value);
// parsing.c
/**
 * @brief Initialize the data structure.
 * 
 * @param[in] argc Number of arguments.
 * @param[in] argv Arguments array.
 * @return t_data* Pointer to the initialized data structure,
 * or NULL on failure.
 */
t_data			*initialize_data(int argc, char **argv);
/**
 * @brief Create foucault array and initialize each philosopher.
 * 
 * @param[in, out] data Structure for simulation data.
 * @return int 0 on success, 1 on failure.
 */
int				create_foucaults(t_data *data);

// print.c
/**
 * @brief Print a formatted message respecting mutex locking.
 * 
 * @param[in] size Number of arguments to print.
 * @param[in, out] data Structure for simulation data.
 * @param[in] ... Arguments to print.
 */
void			print_phrase(t_data *data, int size, ...);
/**
 * @brief Print an action of a philosopher with timescode.
 * 
 * @param[in] philo Philosopher structure.
 * @param[in] action String describing the action.
 * @return int 0 on success, 1 on failure.
 */
int				print_action(t_foucault *philo, char *action);
/**
 * @brief Get the current time since the start of the program.
 *
 * @param[in] data Pointer to the simulation data structure.
 * @return long long int Current time in milliseconds.
 */
long long int	current_time(t_data *data);

//	threads.c
/**
 * @brief Start all philosopher threads.
 * 
 * @param[in, out] data Structure for simulation data.
 * @return int 0 on success, 1 on failure.
 */
int				start_threads(t_data *data);
/**
 * @brief Philosopher's routine function.
 * 
 * @param[in, out] arg Pointer to the philosopher structure. Threads needs a 
 * void* argument.
 * @return void* Return NULL when finished.
 */
void			*discipline_punish(void *arg);
/**
 * @brief Same as usleep but checks if someone is dead.
 * 
 * @param time_in_ms Time to sleep in milliseconds. (Value * 1000 for usleep)
 * @param data Pointer to the simulation data structure.
 * @return int 0 if nothing happened, 1 if someone is dead.
 */
int				better_usleep(int time_in_ms, t_data *data);

//	utils.c
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
//	wrapper.c
/**
 * @brief Free all allocated memory and exit the program.
 * 
 * @param[in, out] data Structure for simulation data.
 * @param[in] exit_code Exit code to return.
 */
void			exit_all(t_data *data, int exit_code);

# define USAGE "./philosophers number_of_philosophers time_to_die time_to_eat "
# define USAGE2 "time_to_sleep [number_of_times_each_philosopher_must_eat]\n"
# define ERROR1 "Not a number or int or above 0.\n"
# define ERROR2 "Can't allocate memory.\n"
# define ERROR3 "Error locking/unlocking mutex\n"
# define ERROR4 "Error creating/destroying mutex\n"

#endif