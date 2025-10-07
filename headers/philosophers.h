/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:35:10 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/07 11:18:53 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <limits.h>
# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>

/**
 * @brief Structure representing a philosopher (reference to Michel Foucault).
 * 
 * @note Foucault is a reference to Michel Foucault, a French philosopher.
 * @param i Index of the philosopher.
 * @param th Thread of the philosopher.
 * @param previous Pointer to the previous philosopher in the list.
 * @param next Pointer to the next philosopher in the list.
 */
typedef struct s_foucault
{
	int					i;
	pthread_t			*th;
	struct s_foucault	*previous;
	struct s_foucault	*next;
}	t_foucault;

/**
 * @brief Structure for simulation data.
 * 
 * @param nb Number of philosophers.
 * @param ttd Time to die.
 * @param tte Time to eat.
 * @param tts Time to sleep.
 * @param notepme Number of times each philosopher must eat. Optional,
 * simulation stops when all have eaten at least this many times.
 * @param fork Number of forks.
 */
typedef struct s_data
{
	int				nb;
	int				ttd;
	int				tte;
	int				tts;
	int				notepme;
	int				fork;
	pthread_mutex_t	*mutex;
}	t_data;


// threads.c
void			free_it(void **array);
void			*ft_print(void *args);
pthread_t		**create_threads(int i);

// libft.c
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

# define USAGE "./philosophers number_of_philosophers time_to_die time_to_eat "
# define USAGE2 "time_to_sleep [number_of_times_each_philosopher_must_eat]\n"
# define ERROR1 "Not a number or int or above 0.\n"

#endif