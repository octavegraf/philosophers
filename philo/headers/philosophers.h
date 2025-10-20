/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:35:10 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/20 17:54:58 by ocgraf           ###   ########.fr       */
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
 * @param hmta How many times the philosopher has eaten.
 * @param death_mutex Mutex to protect the philosopher's last meal time.
 * @param lmt Last meal time in milliseconds (relative to start_time).
 * @param data Pointer to the shared simulation data.
 */
typedef struct s_foucault	t_foucault;

/**
 * @brief Structure for simulation data.
 * 
 * @param nb Number of philosophers.
 * @param ttd Time to die in milliseconds.
 * @param tte Time to eat in milliseconds.
 * @param tts Time to sleep in milliseconds.
 * @param notepme Number of times each philosopher must eat. -1 if no limit.
 * @param foucault_array Pointer to array of philosopher structures.
 * @param fork_array Pointer to array of fork mutexes.
 * @param print_mutex Mutex to protect printing.
 * @param simulation_started Boolean indicating if simulation has started.
 * @param start_mutex Mutex to protect simulation_started (unused, kept for
 * compatibility).
 * @param start_time Timestamp of simulation start in milliseconds.
 * @param monitor_thread Thread ID for the monitoring thread.
 * @param simulation_stopped Boolean indicating if simulation should stop.
 * @param stop_mutex Mutex to protect simulation_stopped.
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
	long long		start_time;
	pthread_t		monitor_thread;
	bool			simulation_stopped;
	pthread_mutex_t	stop_mutex;
}	t_data;

/**
 * @brief Structure for each philosopher (full definition).
 * 
 * @param name Philosopher's number (starts at 1).
 * @param thread The philosopher's thread ID.
 * @param l_fork Pointer to the left fork mutex.
 * @param r_fork Pointer to the right fork mutex.
 * @param hmta How many times this philosopher has eaten.
 * @param death_mutex Mutex to protect last meal time updates.
 * @param lmt Last meal time in milliseconds (relative to data->start_time).
 * @param data Pointer to the shared simulation data.
 */
typedef struct s_foucault
{
	int				name;
	pthread_t		thread;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	int				hmta;
	pthread_mutex_t	death_mutex;
	long long		lmt;
	t_data			*data;
}	t_foucault;

//	monitoring.c
/**
 * @brief Check if a philosopher has died (no meal for time_to_die ms).
 * 
 * @param[in] philo Philosopher structure.
 * @return true if the philosopher has died, false otherwise.
 */
bool			am_i_dead(t_foucault *philo);
/**
 * @brief Check if a philosopher has eaten enough times.
 * 
 * @param[in] philo Philosopher structure.
 * @return true if philosopher has eaten enough times, false otherwise.
 */
bool			am_i_full(t_foucault *philo);
/**
 * @brief Monitoring function for checking dead philosophers and simulation end.
 * 
 * @param[in] arg Pointer to simulation data structure.
 * @return void* Always returns NULL.
 */
void			*monitoring(void *arg);

//	mutex.c and mutex2.c
/**
 * @brief Create and initialize fork mutexes.
 * 
 * @param[in, out] data Simulation data structure.
 * @return int 0 on success, 1 on failure.
 */
int				create_forks(t_data *data);
/**
 * @brief Assign forks to each philosopher.
 * 
 * @param[in, out] data Simulation data structure.
 * @return int 0 on success, 1 on failure.
 */
int				distribute_forks(t_data *data);
/**
 * @brief Acquire both forks for a philosopher (with deadlock prevention).
 * 
 * @param[in, out] philo Philosopher structure.
 * @return int 0 on success, 1 on failure or simulation stopped.
 */
int				fork_handler(t_foucault *philo);
/**
 * @brief Read a mutex-protected integer variable.
 * 
 * @param[in] mutex Pointer to the mutex protecting the variable.
 * @param[in] variable Pointer to the variable to read.
 * @return int The value of the variable.
 */
int				read_mutex(pthread_mutex_t *mutex, int *variable);
/**
 * @brief Modify a mutex-protected integer variable.
 * 
 * @param[in] mutex Pointer to the mutex protecting the variable.
 * @param[in] variable Pointer to the variable to modify.
 * @param[in] new_value New value to set.
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
 * @brief Create and initialize philosopher structures.
 * 
 * @param[in, out] data Simulation data structure.
 * @return int 0 on success, 1 on failure.
 */
int				create_foucaults(t_data *data);

// print.c
/**
 * @brief Print an action of a philosopher with timestamp.
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
 * @return long long int Current time in milliseconds since simulation start.
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
 * @brief Philosopher's main routine function.
 * 
 * @param[in] arg Pointer to philosopher structure (cast from void*).
 * @return void* Always returns NULL when thread exits.
 */
void			*discipline_punish(void *arg);
/**
 * @brief Sleep for a specified time while checking if simulation stopped.
 * 
 * @param[in] time_in_ms Time to sleep in milliseconds.
 * @param[in] data Pointer to simulation data (for checking stop condition).
 * @return int 0 if sleep completed, 1 if simulation stopped during sleep.
 */
int				better_usleep(int time_in_ms, t_data *data);

//	utils.c
/**
 * @brief Check if a string represents a valid positive integer.
 * 
 * @param[in] str String to check.
 * @return int 1 if valid positive integer, 0 otherwise.
 */
int				ft_isint(char *str);
/**
 * @brief Convert a string to a long long integer.
 * 
 * @param[in] str String to convert.
 * @return long long int Converted value.
 */
long long int	ft_atol(const char *str);
/**
 * @brief Free a dynamically allocated array of pointers.
 * 
 * @param[in, out] array Array of pointers to free.
 */
void			free_it(void **array);
/**
 * @brief Get current timestamp in milliseconds since epoch.
 * 
 * @return long long Current time in milliseconds.
 */
long long		get_time_ms(void);
//	wrapper.c
/**
 * @brief Clean up all resources and exit the program.
 * 
 * @param[in] data Simulation data structure (will be freed).
 * @param[in] exit_code Exit code to return.
 */
void			exit_all(t_data *data, int exit_code);

# define USAGE "./philosophers number_of_philosophers time_to_die time_to_eat "
# define USAGE2 "time_to_sleep [number_of_times_each_philosopher_must_eat]\n"
# define ERROR1 "Not a number or int or above 0.\n"
# define ERROR2 "Can't allocate memory.\n"
# define ERROR3 "Error locking/unlocking mutex\n"
# define ERROR4 "Error creating/destroying mutex\n"
# define ERROR5 "Error creating thread\n"

#endif