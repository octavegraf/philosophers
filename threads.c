/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers__threads.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:34:58 by ocgraf            #+#    #+#             */
/*   Updated: 2025/07/31 15:29:05 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_it(void **array)
{
	int	i;

	i = 0;
	while (array[i++])
	{
		free(array[i]);
		array[i] = NULL;
	}
	free(array);
	array = NULL;
}

void	*ft_print(void *args)
{
	char	*arg;

	arg = (char *)args;
	printf("%s\n", arg);
	return (NULL);
}

pthread_t	**create_threads(int i)
{
	pthread_t		**array;
	int				j;
	void			*args;

	array = calloc(i + 1, sizeof(pthread_t *));
	if (!array)
		return (NULL);
	j = 0;
	while (j++ < i)
	{
		array[j] = calloc(1, sizeof(pthread_t));
		if (!array[j])
			return (free_it((void **)array), NULL);
	}
	j = 0;
	args = "Test thread";
	while (j++ < i)
	{
		if (pthread_create(array[j], NULL, ft_print, args) == 0)
		{
			pthread_join(*array[j], NULL);
			printf("Thread created : %p\n", (void *)array[j]);
		}
	}
	return (array);
}

/* void	*ft_print(void *args)
{
	char	*arg;

	arg = (char *)args;
	printf("%s\n", arg);
	return (NULL);
}

int	main(void)
{
	pthread_t	thread;
	void		*args;
	char		arg[5] = "Test";

	args = (void *)arg;
	if (pthread_create(&thread, NULL, ft_print, args) == 0)
	{	
		pthread_join(thread, NULL);
		printf("Thread created : %p\n", (void *)&thread);
	}
	else
		printf("Can't create thread.\n");
	return (0);
}
 */