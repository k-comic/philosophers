/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leroy <leroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 08:57:21 by leroy             #+#    #+#             */
/*   Updated: 2022/06/10 01:27:17 by leroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	check_args(int argc, char **argv, t_rules *rules)
{
	if (argc < 5 || 6 < argc)
	{
		write(2, "Error: Wrong arguments.\n", 24);
		return (-1);
	}
	if (ft_atoi(argv[1]) < 1 || ft_atoi(argv[2]) < 0 || ft_atoi(argv[3]) < 0
		|| ft_atoi(argv[4]) < 0 || (argc == 6 && ft_atoi(argv[5]) < 0))
	{
		write(2, "Error: Wrong format.\n", 21);
		return (-1);
	}
	rules->number_of_philos = ft_atoi(argv[1]);
	rules->time_to_die = ft_atoi(argv[2]);
	rules->time_to_eat = ft_atoi(argv[3]);
	rules->time_to_sleep = ft_atoi(argv[4]);
	rules->at_least = -1;
	rules->dead_or_enough = 0;
	rules->fed_philos = 0;
	if (argc == 6)
	{
		if (ft_atoi(argv[5]) == 0)
			return (0);
		rules->at_least = ft_atoi(argv[5]);
	}
	return (1);
}

static void	philo_life2(t_philo *philo, int *flag)
{
	pthread_mutex_lock(philo->expen_fork);
	ft_print(philo, "has taken a fork");
	ft_print(philo, "is eating");
	pthread_mutex_lock(philo->rules->m_change);
	gettimeofday(&(philo->last_meal), NULL);
	pthread_mutex_unlock(philo->rules->m_change);
	usleep(1000 * philo->rules->time_to_eat);
	pthread_mutex_unlock(philo->expen_fork);
	pthread_mutex_unlock(philo->cheap_fork);
	pthread_mutex_lock(philo->rules->m_change);
	philo->meals++;
	if (philo->meals == philo->rules->at_least)
		philo->rules->fed_philos++;
	pthread_mutex_unlock(philo->rules->m_change);
	*flag = 1;
	ft_print(philo, "is sleeping");
	usleep(1000 * philo->rules->time_to_sleep);
	pthread_mutex_lock(philo->rules->m_change);
}

void	*philo_life(void *data)
{
	t_philo	*philo;
	int		flag;

	philo = (t_philo *)data;
	flag = 0;
	pthread_mutex_lock(philo->rules->m_change);
	while (philo->rules->dead_or_enough == 0)
	{
		pthread_mutex_unlock(philo->rules->m_change);
		ft_print(philo, "is thinking");
		if (philo->id % 2 && !flag)
			usleep(philo->rules->time_to_eat * 1000 / 2);
		if (time_from(philo->last_meal) < (long)(philo->rules->time_to_sleep
			+ philo->rules->time_to_eat + 5) && flag)
			usleep(1000 * 5);
		pthread_mutex_lock(philo->cheap_fork);
		ft_print(philo, "has taken a fork");
		if (philo->rules->number_of_philos == 1)
			return (NULL);
		philo_life2(philo, &flag);
	}
	pthread_mutex_unlock(philo->rules->m_change);
	return (NULL);
}

static void	watcher(t_philo *philos)
{
	int		i;
	long	tflm;

	i = 0;
	usleep(philos->rules->time_to_die * 1000 / 2);
	pthread_mutex_lock(philos->rules->m_change);
	while (philos->rules->fed_philos < philos->rules->number_of_philos)
	{
		tflm = time_from((philos + i)->last_meal);
		pthread_mutex_unlock(philos->rules->m_change);
		if (tflm > (long)(philos->rules->time_to_die))
		{
			ft_print(philos + i, "died");
			pthread_mutex_lock(philos->rules->m_change);
			philos->rules->dead_or_enough = 1;
			pthread_mutex_unlock(philos->rules->m_change);
			return ;
		}
		i = (i + 1) % philos->rules->number_of_philos;
		pthread_mutex_lock(philos->rules->m_change);
	}
	philos->rules->dead_or_enough = 1;
	pthread_mutex_unlock(philos->rules->m_change);
}

int	main(int argc, char **argv)
{
	t_rules			rules;
	t_philo			*philos;
	pthread_mutex_t	*mutexes;
	pthread_t		*threads;
	int				i;

	if (check_args(argc, argv, &rules) < 1)
		return (-1);
	if (init_ptr(&philos, &mutexes, &threads, &rules))
		return (-1);
	rules.m_printf = mutexes + rules.number_of_philos;
	rules.m_change = mutexes + rules.number_of_philos + 1;
	if (init_mutexes(rules, philos, mutexes, threads))
		return (-1);
	init_philos(&rules, philos, mutexes);
	if (init_threads(rules, philos, mutexes, threads))
		return (-1);
	watcher(philos);
	i = 0;
	while (i < rules.number_of_philos)
		pthread_join(*(threads + i++), NULL);
	free(philos);
	free(mutexes);
	free(threads);
	return (0);
}
