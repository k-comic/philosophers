/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leroy <leroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 02:32:41 by leroy             #+#    #+#             */
/*   Updated: 2022/06/10 02:34:34 by leroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	eated_enough(t_philo *philo)
{
	sem_wait(philo->rules->change);
	if (philo->meals >= philo->rules->at_least
		&& philo->rules->at_least != -1)
	{
		sem_post(philo->rules->change);
		return (1);
	}
	sem_post(philo->rules->change);
	return (0);
}

static void	*watcher(void *data)
{
	long	tflm;
	t_philo	*philo;

	philo = (t_philo *)data;
	usleep(philo->rules->time_to_die * 1000 / 2);
	while (1)
	{
		sem_wait(philo->rules->change);
		tflm = time_from(philo->last_meal);
		sem_post(philo->rules->change);
		if (tflm > (long)(philo->rules->time_to_die))
		{
			ft_print(philo, "died");
			sem_wait(philo->rules->print);
			sem_wait(philo->rules->change);
			philo->dead = 1;
			sem_post(philo->rules->change);
			exit(1);
		}
		usleep(1000);
		if (eated_enough(philo))
			break ;
	}
	return (NULL);
}

static int	philo_life2(t_philo *philo, int *flag)
{
	sem_wait(philo->rules->forks);
	ft_print(philo, "has taken a fork");
	ft_print(philo, "is eating");
	sem_wait(philo->rules->change);
	gettimeofday(&(philo->last_meal), NULL);
	sem_post(philo->rules->change);
	usleep(1000 * philo->rules->time_to_eat);
	sem_post(philo->rules->forks);
	sem_post(philo->rules->forks);
	sem_wait(philo->rules->change);
	philo->meals++;
	sem_post(philo->rules->change);
	*flag = 1;
	ft_print(philo, "is sleeping");
	if (philo->meals >= philo->rules->at_least && philo->rules->at_least != -1)
		return (1);
	usleep(1000 * philo->rules->time_to_sleep);
	sem_wait(philo->rules->change);
	return (0);
}

void	*philo_life(void *data)
{
	t_philo	*philo;
	int		flag;

	philo = (t_philo *)data;
	pthread_create(&(philo->death_check), NULL, watcher, philo);
	flag = 0;
	sem_wait(philo->rules->change);
	while (!philo->dead)
	{
		sem_post(philo->rules->change);
		ft_print(philo, "is thinking");
		if (time_from(philo->last_meal) < (long)(philo->rules->time_to_sleep
			+ philo->rules->time_to_eat + 5) && flag)
			usleep(1000 * 5);
		sem_wait(philo->rules->forks);
		ft_print(philo, "has taken a fork");
		if (philo_life2(philo, &flag))
			break ;
	}
	sem_post(philo->rules->change);
	pthread_join(philo->death_check, NULL);
	exit(0);
	return (NULL);
}
