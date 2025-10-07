/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocgraf <ocgraf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:28:04 by ocgraf            #+#    #+#             */
/*   Updated: 2025/10/07 11:32:53 by ocgraf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_putstr(char *str)
{
	write(1, str, ft_strlen(str));
}

int	ft_isint(char *str)
{
	int	i;
	int	zero;

	i = 0;
	if (str[0] == '+')
		i++;
	while (str[i] == '0')
		i++;
	zero = i;
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			i++;
		else
			return (0);
	}
	if (i - zero > 10)
		return (0);
	if (ft_atol(str) > INT_MAX || ft_atol(str) < INT_MIN)
		return (0);
	return (1);
}

long long int	ft_atol(const char *str)
{
	int				i;
	int				minus;
	long long int	nb;

	i = 0;
	minus = 1;
	nb = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			minus *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb *= 10;
		nb += str[i] - '0';
		i++;
	}
	return (nb * minus);
}

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
