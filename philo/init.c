/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leroy <leroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 03:10:27 by leroy             #+#    #+#             */
/*   Updated: 2022/06/07 03:13:38 by leroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	init_ptr(t_philo **philos, pthread_mutex_t **mutexes,
	pthread_t **threads, t_rules *rules)
{
	*philos = (t_philo *)malloc(sizeof(t_philo) * rules->number_of_philos);
	if (!philos)
	{
		printf("Error: malloc philos failed.\n");
		return (-1);
	}
	*mutexes = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* (rules->number_of_philos + 2));
	if (!mutexes)
	{
		printf("Error: malloc mutexes failed.\n");
		free(*philos);
		return (-1);
	}
	*threads = (pthread_t *)malloc(sizeof(pthread_t) * rules->number_of_philos);
	if (!threads)
	{
		printf("Error: malloc threads failed.\n");
		free(*philos);
		free(*mutexes);
		return (-1);
	}
	return (0);
}

int	init_mutexes(t_rules rules,
	t_philo *philos, pthread_mutex_t *mutexes, pthread_t *threads)
{
	int	i;

	i = 0;
	while (i < rules.number_of_philos + 2)
	{
		if (pthread_mutex_init(mutexes + i, NULL))
		{
			printf("Error: mutexes init failed.\n");
			i--;
			while (i >= 0)
			{
				if (pthread_mutex_destroy(mutexes + i))
					printf("Error: mutexes destroy failed.\n");
				i--;
			}
			free(philos);
			free(mutexes);
			free(threads);
			return (-1);
		}
		i++;
	}
	return (0);
}

void	init_philos(t_rules *rules, t_philo *philos, pthread_mutex_t *mutexes)
{
	int	i;

	i = 0;
	gettimeofday(&(rules->time_start), NULL);
	while (i < rules->number_of_philos)
	{	
		philos[i].id = i;
		philos[i].rules = rules;
		if (philos[i].id != rules->number_of_philos - 1)
		{
			philos[i].cheap_fork = mutexes + i;
			philos[i].expen_fork = mutexes + (i + 1) % rules->number_of_philos;
		}
		else
		{
			philos[i].cheap_fork = mutexes + (i + 1) % rules->number_of_philos;
			philos[i].expen_fork = mutexes + i;
		}
		philos[i].meals = 0;
		philos[i].last_meal = rules->time_start;
		i++;
	}
}

int	init_threads(t_rules rules,
	t_philo *philos, pthread_mutex_t *mutexes, pthread_t *threads)
{
	int	i;

	i = 0;
	while (i < rules.number_of_philos)
	{
		if (pthread_create(threads + i, NULL, philo_life, (void *)(philos + i)))
		{
			i = 0;
			while (i < rules.number_of_philos)
			{
				if (pthread_mutex_destroy(mutexes + i))
					printf("Error: mutexes destroy failed.\n");
				i++;
			}
			free(philos);
			free(mutexes);
			free(threads);
			return (-1);
		}
		i++;
	}
	return (0);
}
