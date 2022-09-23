/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcomic <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 19:08:31 by kcomic            #+#    #+#             */
/*   Updated: 2021/10/27 19:08:35 by kcomic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

long	time_from(struct timeval from)
{
	struct timeval	now;
	long			ans;

	gettimeofday(&now, NULL);
	ans = (now.tv_sec - from.tv_sec) * 1000
		+ (now.tv_usec - from.tv_usec) / 1000;
	return (ans);
}

void	ft_print(t_philo *philo, char *message)
{
	pthread_mutex_lock(philo->rules->m_change);
	if (philo->rules->dead_or_enough == 0)
	{
		pthread_mutex_unlock(philo->rules->m_change);
		pthread_mutex_lock(philo->rules->m_printf);
		printf("%ld ", time_from(philo->rules->time_start));
		printf("%d ", philo->id + 1);
		printf("%s\n", message);
		pthread_mutex_unlock(philo->rules->m_printf);
	}
	else
		pthread_mutex_unlock(philo->rules->m_change);
}

static long	atoi_number(char *str, int sign)
{
	int		i;
	long	number;

	i = 0;
	number = 0;
	while (('0' <= str[i]) && (str[i] <= '9'))
	{
		number = (number * 10) + (str[i] - '0');
		i++;
		if (number < 0)
		{
			if (sign == 1)
				return (-1);
			else
				return (0);
		}
	}
	if ((str[i] < '0' || '9' < str[i]) && str[i] != '\0')
		return (-1);
	return (number);
}

int	ft_atoi(const char *str)
{
	int		i;
	long	ans;
	int		sign;

	i = 0;
	sign = 1;
	while ((str[i] == ' ') || (str[i] == '\t') || (str[i] == '\n')
		|| (str[i] == '\v') || (str[i] == '\f') || (str[i] == '\r'))
		i++;
	if ((str[i] == '-') || (str[i] == '+'))
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	ans = (long)sign * atoi_number((char *)str + i, sign);
	if (ans < 0 || ans > INT_MAX)
		return (-1);
	return (ans);
}
